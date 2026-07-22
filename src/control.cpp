#include <fsmconnections.h>

#include <fsm/appmanager.h>
#include <datamanager.h>
#include <kafka/kafkaconnector.h>
#include <localstub.h>
#include <public/publicconnector.h>
#include <settingsmanager.h>
#include <tenant/tenantconnector.h>
#include <update/updater.h>

#include <QObject>

namespace app {
namespace {

void connectManagers(control::AppManager& appManager, control::Updater& updater)
{
    auto& settingsManager = settings::SettingsManager::instance();
    auto& dataManager = datacore::DataManager::instance();

    QObject::connect(&appManager, &control::AppManager::signalInitializeSettings,
                     &settingsManager, &settings::SettingsManager::onInitialize);
    QObject::connect(&appManager, &control::AppManager::signalCloseSettings,
                     &settingsManager, &settings::SettingsManager::onClose);
    QObject::connect(&settingsManager, &settings::SettingsManager::signalInitialized,
                     &appManager, &control::AppManager::onSettingsInitialized);
    QObject::connect(&appManager, &control::AppManager::signalInitDatabase,
                     &dataManager, &datacore::DataManager::onInitDatabase);
    QObject::connect(&appManager, &control::AppManager::signalApplyDatabaseMigrations,
                     &dataManager, &datacore::DataManager::applyMigrations);
    QObject::connect(&appManager, &control::AppManager::signalApplySettingsMigrations,
                     &settingsManager, &settings::SettingsManager::applyMigrations);
    QObject::connect(&appManager, &control::AppManager::signalClearDatabase,
                     &dataManager, &datacore::DataManager::onClearDatabase);
    QObject::connect(&appManager, &control::AppManager::signalApplyFullSync,
                     &dataManager, &datacore::DataManager::onApplyFullSync);
    QObject::connect(&dataManager, &datacore::DataManager::signalDatabaseInitialized,
                     &appManager, &control::AppManager::onDatabaseInitialized);
    QObject::connect(&dataManager, &datacore::DataManager::signalMigrationsApplied,
                     &appManager, &control::AppManager::onDatabaseMigrationsApplied);
    QObject::connect(&settingsManager, &settings::SettingsManager::signalMigrationsApplied,
                     &appManager, &control::AppManager::onSettingsMigrationsApplied);
    QObject::connect(&dataManager, &datacore::DataManager::signalDatabaseCleared,
                     &appManager, &control::AppManager::onDatabaseCleared);
    QObject::connect(&dataManager, &datacore::DataManager::signalFullSyncApplied,
                     &appManager, &control::AppManager::onFullSyncApplied);
    QObject::connect(&appManager, &control::AppManager::signalStartUpdating,
                     &updater, &control::Updater::onStartUpdating);
    QObject::connect(&updater, &control::Updater::signalUpdateReady,
                     &appManager, &control::AppManager::onUpdateReady);
    QObject::connect(&appManager, &control::AppManager::signalForcedUpdate,
                     &updater, &control::Updater::onUpdateApproved);
}

} // namespace

void connectControl(netcore::PublicConnector& publicConnector,
                    netcore::TenantConnector& tenantConnector,
                    netcore::KafkaConnector& kafkaConnector,
                    control::Updater& updater)
{
    auto& appManager = control::AppManager::instance();

    QObject::connect(&appManager, &control::AppManager::signalCheckWebConnection,
                     &publicConnector, &netcore::PublicConnector::onCheckConnection);
    QObject::connect(&publicConnector, &netcore::PublicConnector::signalWebConnection,
                     &appManager, &control::AppManager::onWebConnection);
    QObject::connect(&appManager, &control::AppManager::signalConnectTenant,
                     &tenantConnector, &netcore::TenantConnector::onMakeConnect);
    QObject::connect(&appManager, &control::AppManager::signalDisconnectTenant,
                     &tenantConnector, &netcore::TenantConnector::onDisconnect);
    QObject::connect(&tenantConnector, &netcore::TenantConnector::signalTenantConnected,
                     &appManager, &control::AppManager::onTenantConnected);
    QObject::connect(&appManager, &control::AppManager::signalConfigureKafka,
                     &kafkaConnector, &netcore::KafkaConnector::onConfigure);
    QObject::connect(&appManager, &control::AppManager::signalStartKafka,
                     &kafkaConnector, &netcore::KafkaConnector::onStart);
    QObject::connect(&appManager, &control::AppManager::signalStopKafka,
                     &kafkaConnector, &netcore::KafkaConnector::onStop);
    QObject::connect(&kafkaConnector, &netcore::KafkaConnector::signalKafkaConfigured,
                     &appManager, &control::AppManager::onKafkaConfigured);
    QObject::connect(&kafkaConnector, &netcore::KafkaConnector::signalKafkaConnected,
                     &appManager, &control::AppManager::onKafkaConnected);
    QObject::connect(&kafkaConnector, &netcore::KafkaConnector::signalKafkaOffsetReady,
                     &appManager, &control::AppManager::onKafkaOffsetReady);
    QObject::connect(&kafkaConnector, &netcore::KafkaConnector::signalUpdateNeeded,
                     &appManager, &control::AppManager::onUpdateNeeded);

    connectManagers(appManager, updater);
}

void connectControl(netcore::LocalStub& localStub, control::Updater& updater)
{
    auto& appManager = control::AppManager::instance();

    QObject::connect(&appManager, &control::AppManager::signalCheckWebConnection,
                     &localStub, &netcore::LocalStub::onCheckConnection);
    QObject::connect(&localStub, &netcore::LocalStub::signalWebConnection,
                     &appManager, &control::AppManager::onWebConnection);
    QObject::connect(&appManager, &control::AppManager::signalConnectTenant,
                     &localStub, &netcore::LocalStub::onMakeConnect);
    QObject::connect(&appManager, &control::AppManager::signalDisconnectTenant,
                     &localStub, &netcore::LocalStub::onDisconnect);
    QObject::connect(&localStub, &netcore::LocalStub::signalTenantConnected,
                     &appManager, &control::AppManager::onTenantConnected);
    QObject::connect(&appManager, &control::AppManager::signalConfigureKafka,
                     &localStub, &netcore::LocalStub::onConfigure);
    QObject::connect(&appManager, &control::AppManager::signalStartKafka,
                     &localStub, &netcore::LocalStub::onStart);
    QObject::connect(&appManager, &control::AppManager::signalStopKafka,
                     &localStub, &netcore::LocalStub::onStop);
    QObject::connect(&localStub, &netcore::LocalStub::signalKafkaConfigured,
                     &appManager, &control::AppManager::onKafkaConfigured);
    QObject::connect(&localStub, &netcore::LocalStub::signalKafkaConnected,
                     &appManager, &control::AppManager::onKafkaConnected);
    QObject::connect(&localStub, &netcore::LocalStub::signalKafkaOffsetReady,
                     &appManager, &control::AppManager::onKafkaOffsetReady);

    connectManagers(appManager, updater);
}

} // namespace app
