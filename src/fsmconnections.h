#ifndef UNITER_CLIENT_FSMCONNECTIONS_H
#define UNITER_CLIENT_FSMCONNECTIONS_H

class QApplication;

namespace control {
class Updater;
}

namespace netcore {
class KafkaConnector;
class LocalStub;
class MinioConnector;
class PublicConnector;
class TenantConnector;
}

namespace app {

void connectMessaging(netcore::PublicConnector& publicConnector,
                      netcore::TenantConnector& tenantConnector);
void connectMessaging(netcore::LocalStub& localStub);
void connectMiddle(netcore::KafkaConnector& kafkaConnector,
                   netcore::MinioConnector& minioConnector);
void connectMiddle(netcore::LocalStub& localStub);
void connectControl(netcore::PublicConnector& publicConnector,
                    netcore::TenantConnector& tenantConnector,
                    netcore::KafkaConnector& kafkaConnector,
                    control::Updater& updater);
void connectControl(netcore::LocalStub& localStub, control::Updater& updater);
void connectFrontshell(control::Updater& updater, QApplication& application);
void connectApplication(netcore::PublicConnector& publicConnector,
                        netcore::TenantConnector& tenantConnector,
                        netcore::KafkaConnector& kafkaConnector,
                        netcore::MinioConnector& minioConnector,
                        control::Updater& updater,
                        QApplication& application);
void connectApplication(netcore::LocalStub& localStub,
                        control::Updater& updater,
                        QApplication& application);

} // namespace app

#endif // UNITER_CLIENT_FSMCONNECTIONS_H
