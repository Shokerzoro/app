#include <fsmconnections.h>

#include <fsm/appmanager.h>
#include <args/args.h>
#include <kafka/kafkaconnector.h>
#include <localstub.h>
#include <logging/logger.h>
#include <minio/minioconnector.h>
#include <process/process.h>
#include <public/publicconnector.h>
#include <storage/storage.h>
#include <tenant/tenantconnector.h>
#include <update/updater.h>
#include <widgetstatic/mainwidget.h>

#include <QApplication>
#include <QCoreApplication>

#include <cstdlib>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

namespace {

using UniterOptions = syspilot::CLIOptions<
    syspilot::UniterCommands,
    syspilot::UniterArgs,
    syspilot::UniterFlags>;
using ElevatorOptions = syspilot::CLIOptions<
    syspilot::ElevatorCommands,
    syspilot::ElevatorArgs,
    syspilot::ElevatorFlags>;

void launchCrashRestart()
{
    auto& logger = syspilot::Logger::instance();
    ElevatorOptions options;
    options.set_command(syspilot::ElevatorCommands::restartapp);
    options.set_argument(
        syspilot::ElevatorArgs::apppid,
        std::to_string(static_cast<long long>(QCoreApplication::applicationPid())));

    auto elevator = std::make_unique<syspilot::ChildProcess>(syspilot::BinType::Elevator);
    if (!(*elevator << options)) {
        logger.full_log("launchCrashRestart(): unable to configure elevator process");
        return;
    }
    if (!elevator->run() || !elevator->wait_started()) {
        logger.full_log("launchCrashRestart(): unable to start elevator process");
        return;
    }
    elevator.release();
}

} // namespace

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    auto& logger = syspilot::Logger::instance();

    try {
        syspilot::SettingsStorage storage;
        if (!storage.embed_data()) {
            throw std::runtime_error(std::string{});
        }

        UniterOptions options(argc, argv);
        if (!options.valid()) {
            throw std::runtime_error(options.error());
        }
        if (options.has_command() && !options.command(syspilot::UniterCommands::run)) {
            throw std::runtime_error(std::string{});
        }

        const bool local = options.has_flag(syspilot::UniterFlags::local);
        const bool updated = options.has_flag(syspilot::UniterFlags::updated);
        logger.full_log(
            "main(): application initialized, mode="
            + std::string(local ? "local" : "remote")
            + ", updated=" + std::string(updated ? "true" : "false"));
        control::Updater updater;
        frontshell::MainWidget mainWidget;

        if (local) {
            netcore::LocalStub localStub;
            app::connectApplication(localStub, updater, application);
            mainWidget.show();
            control::AppManager::instance().run(updated);
            return application.exec();
        }

        netcore::PublicConnector publicConnector;
        netcore::TenantConnector tenantConnector;
        netcore::KafkaConnector kafkaConnector;
        netcore::MinioConnector minioConnector;
        app::connectApplication(publicConnector, tenantConnector, kafkaConnector,
                                minioConnector, updater, application);
        mainWidget.show();
        control::AppManager::instance().run(updated);
        return application.exec();
    } catch (const std::exception& exception) {
        logger.full_log(exception.what());
        logger.crash_notify();
        launchCrashRestart();
    } catch (...) {
        logger.crash_notify();
        launchCrashRestart();
    }

    return EXIT_FAILURE;
}
