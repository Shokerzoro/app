#include <fsmconnections.h>

#include <localstub.h>
#include <netbridge.h>
#include <public/publicconnector.h>
#include <tenant/tenantconnector.h>

#include <QObject>

namespace app {

void connectMessaging(netcore::PublicConnector& publicConnector,
                      netcore::TenantConnector& tenantConnector)
{
    auto& bridge = netbridge::NetBridge::instance();

    QObject::connect(&bridge, &netbridge::NetBridge::signalSendPublicMessage,
                     &publicConnector, &netcore::PublicConnector::onSendPublicMessage);
    QObject::connect(&publicConnector, &netcore::PublicConnector::signalReceivePublicMessage,
                     &bridge, &netbridge::NetBridge::onReceivePublicMessage);
    QObject::connect(&bridge, &netbridge::NetBridge::signalSendTenantMessage,
                     &tenantConnector, &netcore::TenantConnector::onSendTenantMessage);
    QObject::connect(&tenantConnector, &netcore::TenantConnector::signalReceiveTenantMessage,
                     &bridge, &netbridge::NetBridge::onReceiveTenantMessage);
    QObject::connect(&bridge, &netbridge::NetBridge::signalSendCrudMessage,
                     &tenantConnector, &netcore::TenantConnector::onSendCrudMessage);
    QObject::connect(&tenantConnector, &netcore::TenantConnector::signalReceiveCrudMessage,
                     &bridge, &netbridge::NetBridge::onReceiveCrudMessage);
}

void connectMessaging(netcore::LocalStub& localStub)
{
    auto& bridge = netbridge::NetBridge::instance();

    QObject::connect(&bridge, &netbridge::NetBridge::signalSendPublicMessage,
                     &localStub, &netcore::LocalStub::onSendPublicMessage);
    QObject::connect(&localStub, &netcore::LocalStub::signalReceivePublicMessage,
                     &bridge, &netbridge::NetBridge::onReceivePublicMessage);
    QObject::connect(&bridge, &netbridge::NetBridge::signalSendTenantMessage,
                     &localStub, &netcore::LocalStub::onSendTenantMessage);
    QObject::connect(&localStub, &netcore::LocalStub::signalReceiveTenantMessage,
                     &bridge, &netbridge::NetBridge::onReceiveTenantMessage);
    QObject::connect(&bridge, &netbridge::NetBridge::signalSendCrudMessage,
                     &localStub, &netcore::LocalStub::onSendCrudMessage);
    QObject::connect(&localStub, &netcore::LocalStub::signalReceiveCrudMessage,
                     &bridge, &netbridge::NetBridge::onReceiveCrudMessage);
}

} // namespace app
