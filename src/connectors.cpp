#include <fsmconnections.h>

namespace app {

void connectApplication(netcore::PublicConnector& publicConnector,
                        netcore::TenantConnector& tenantConnector,
                        netcore::KafkaConnector& kafkaConnector,
                        netcore::MinioConnector& minioConnector,
                        control::Updater& updater,
                        QApplication& application)
{
    connectMessaging(publicConnector, tenantConnector);
    connectMiddle(kafkaConnector, minioConnector);
    connectControl(publicConnector, tenantConnector, kafkaConnector, updater);
    connectFrontshell(updater, application);
}

void connectApplication(netcore::LocalStub& localStub,
                        control::Updater& updater,
                        QApplication& application)
{
    connectMessaging(localStub);
    connectMiddle(localStub);
    connectControl(localStub, updater);
    connectFrontshell(updater, application);
}

} // namespace app
