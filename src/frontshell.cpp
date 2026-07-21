#include <fsmconnections.h>

#include <fsm/appmanager.h>
#include <controllers/uicontroller.h>
#include <update/updater.h>

#include <QApplication>
#include <QObject>

namespace app {

void connectFrontshell(control::Updater& updater, QApplication& application)
{
    auto& appManager = control::AppManager::instance();
    auto& uiController = frontshell::UIController::instance();

    QObject::connect(&appManager, &control::AppManager::signalFindAuthData,
                     &uiController, &frontshell::UIController::requestAuthData);
    QObject::connect(&uiController, &frontshell::UIController::authDataReady,
                     &appManager, &control::AppManager::onAuthDataReady);
    QObject::connect(&appManager, &control::AppManager::signalAuthFailed,
                     &uiController, &frontshell::UIController::handleAuthFailure);
    QObject::connect(&appManager, &control::AppManager::signalConfigureForEmployee,
                     &uiController, &frontshell::UIController::configureForEmployee);
    QObject::connect(&uiController, &frontshell::UIController::configured,
                     &appManager, &control::AppManager::onConfigured);
    QObject::connect(&appManager, &control::AppManager::signalNetStateChanged,
                     &uiController, [&uiController](control::NetState state) {
        uiController.onNetstateChanged(state == control::NetState::ONLINE);
    });
    QObject::connect(&uiController, &frontshell::UIController::logoutRequested,
                     &appManager, &control::AppManager::onLogout);
    QObject::connect(&uiController, &frontshell::UIController::shutdownRequested,
                     &appManager, &control::AppManager::onShutdown);
    QObject::connect(&uiController, &frontshell::UIController::shutdownRequested,
                     &application, &QApplication::quit);
    QObject::connect(&updater, &control::Updater::signalUpdateReady,
                     &uiController, &frontshell::UIController::handleUpdateReady);
    QObject::connect(&uiController, &frontshell::UIController::updateAccepted,
                     &updater, &control::Updater::onUpdateApproved);
}

} // namespace app
