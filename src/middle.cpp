#include <fsmconnections.h>

#include <datamanager.h>
#include <filemanager.h>
#include <kafka/kafkaconnector.h>
#include <localstub.h>
#include <minio/minioconnector.h>

#include <QObject>

namespace app {

void connectMiddle(netcore::KafkaConnector& kafkaConnector,
                   netcore::MinioConnector& minioConnector)
{
    auto& dataManager = datacore::DataManager::instance();
    auto& fileManager = filetransfer::FileManager::instance();

    QObject::connect(&kafkaConnector, &netcore::KafkaConnector::signalReceiveKafkaMessage,
                     &dataManager, &datacore::DataManager::onReceiveKafkaMessage);
    QObject::connect(&dataManager, &datacore::DataManager::signalKafkaMessageApplied,
                     &kafkaConnector, &netcore::KafkaConnector::onMessageApplied);
    QObject::connect(&fileManager, &filetransfer::FileManager::signalSendFileMessage,
                     &minioConnector, &netcore::MinioConnector::onSendFileMessage);
    QObject::connect(&minioConnector, &netcore::MinioConnector::signalReceiveFileMessage,
                     &fileManager, &filetransfer::FileManager::onReceiveFileMessage);
}

void connectMiddle(netcore::LocalStub& localStub)
{
    auto& dataManager = datacore::DataManager::instance();
    auto& fileManager = filetransfer::FileManager::instance();

    QObject::connect(&localStub, &netcore::LocalStub::signalReceiveKafkaMessage,
                     &dataManager, &datacore::DataManager::onReceiveKafkaMessage);
    QObject::connect(&dataManager, &datacore::DataManager::signalKafkaMessageApplied,
                     &localStub, &netcore::LocalStub::onMessageApplied);
    QObject::connect(&fileManager, &filetransfer::FileManager::signalSendFileMessage,
                     &localStub, &netcore::LocalStub::onSendFileMessage);
    QObject::connect(&localStub, &netcore::LocalStub::signalReceiveFileMessage,
                     &fileManager, &filetransfer::FileManager::onReceiveFileMessage);
}

} // namespace app
