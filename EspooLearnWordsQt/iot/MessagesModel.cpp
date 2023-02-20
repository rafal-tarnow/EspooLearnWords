#include "MessagesModel.h"
#include <QNetworkDatagram>
#include <QtQml>

MessagesModel::MessagesModel(QObject *parent) : QAbstractListModel(parent)
{


//    //  client = new QMQTT::Client(QHostAddress::LocalHost, 1883);
//          qDebug() << "RTT New QMQTT::Client";
//     client = new QMQTT::Client("ws://localhost:9001/", "<origin>", QWebSocketProtocol::VersionLatest);
//   //   client->setHost("ws://broker.hivemq.com:8884/mqtt");
//     client->setHostName()
//      client->setPort(9001);
//      //client->setClientId(QString("clientId"));
//      //client->setUsername("user");
//      //client->setPassword("password");
//      //client->setHostName("ws://localhost:9001/");
//      client->connectToHost();
//      QObject::connect(client, &QMQTT::Client::connected, [&]() {
//          // Investigate the errors here, if you find no serious problems, call ignoreSslErrors()
//          // to continue connecting.
//          qDebug( )<< "Connected mqtt";
//          client->subscribe("outTopic");
//      });
//      QObject::connect(client, &QMQTT::Client::received, [&](const QMQTT::Message &message) {
//          // Investigate the errors here, if you find no serious problems, call ignoreSslErrors()
//          // to continue connecting.
//          qDebug() << message.payload();
//          append(message.payload(), "M0001", "faldnfa", "1234", "57230457");
//      });



/*
    m_client = new QMqttWebsocketClient(this);
    m_client->setHostname("ws://broker.hivemq.com:8000/mqtt");
    m_client->setPort(8000);
   // m_client->setClientId("dfadfadfad");

   // connect(m_client, &QMqttClient::stateChanged, this, &MainWindow::updateLogStateChange);
   // connect(m_client, &QMqttClient::disconnected, this, &MainWindow::brokerDisconnected);


    connect(m_client, &QMqttClient::connected, this, [this](){

        auto subscription = m_client->subscribe(QString("rafal-tarnow"));
        if (!subscription) {
            //QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
            return;
        }

    });
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(" Received Topic: ")
                    + topic.name()
                    + QLatin1String(" Message: ")
                    + message
                    + QLatin1Char('\n');

          append(content, "M0001", "faldnfa", "1234", "57230457");
       // ui->editLog->insertPlainText(content);
    });

    connect(m_client, &QMqttClient::pingResponseReceived, this, [this]() {
//        ui->buttonPing->setEnabled(true);
//        const QString content = QDateTime::currentDateTime().toString()
//                    + QLatin1String(" PingResponse")
//                    + QLatin1Char('\n');
//        ui->editLog->insertPlainText(content);
    });

      m_client->connectToWebSocketHost();
*/

    clientsub = new ClientSubscription();
    clientsub->setUrl(QUrl("wss://broker.hivemq.com:8884/mqtt"));
    //  clientsub->setUrl(QUrl("ws://localhost:8000"));
    //clientsub.setVersion(4);
    clientsub->setVersion(3);


    connect(clientsub, &ClientSubscription::messageReceived, this, [this](const QByteArray & message){
        qDebug() << "Newwwww messsage  " << message;
        append(QString(message), "M0001", "faldnfa", "1234", "57230457");
    });
    clientsub->setTopic("AA:BB:CC");
    clientsub->connectAndSubscribe();

}

MessagesModel::~MessagesModel()
{
    if(clientsub != nullptr)
        delete clientsub;
}

void MessagesModel::setSearchStatus(const bool &search)
{
    searchStatus = search;
    if (searchStatus) {

    }
    else {

    }
    Q_EMIT seachChanged();
}


bool MessagesModel::searchDevices() const { return searchStatus; }


bool MessagesModel::deviceArleadyAdded(const QString &ipAddress)
{
    for (auto &device : messages) {
        if (device.ipAddress == ipAddress) {
            return true;
        }
    }
    return false;
}

int MessagesModel::rowCount(const QModelIndex &) const { return messages.count(); }

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role) {
        case DeviceNameRole:
            return messages.at(index.row()).deviceName;
        case ModuleType:
            return messages.at(index.row()).moduleType;
        case IpAddressRole:
            return messages.at(index.row()).ipAddress;
        case PortRole:
            return messages.at(index.row()).port;
        case SerialNumberRole:
            return messages.at(index.row()).serialNumber;
        default:
            return QVariant();
        }
    return QVariant();
}

QHash<int, QByteArray> MessagesModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{DeviceNameRole, "deviceName"}, {ModuleType, "moduleType"}, {IpAddressRole, "ipAddress"}, {PortRole, "port"}, {SerialNumberRole, "serialNumber"}};
    return roles;
}

QVariantMap MessagesModel::get(int row) const
{
    const Device device = messages.value(row);
    return {{"deviceName", device.deviceName}, {"moduleType", device.moduleType}, {"ipAddress", device.ipAddress}, {"port", device.port}, {"serialNumber", device.serialNumber}};
}

void MessagesModel::append(const QString &deviceName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber)
{
    int row = 0;
    while (row < messages.count() && deviceName > messages.at(row).deviceName)
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    messages.insert(row, {deviceName, moduleType, ipAddress, port, serialNumber});
    endInsertRows();
}

void MessagesModel::set(int row, const QString &deviceName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber)
{
    if (row < 0 || row >= messages.count())
        return;

    messages.replace(row, {deviceName, moduleType, ipAddress, port, serialNumber});
    dataChanged(index(row, 0), index(row, 0), {DeviceNameRole, ModuleType, IpAddressRole, PortRole, SerialNumberRole});
}

void MessagesModel::remove(int row)
{
    if (row < 0 || row >= messages.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    messages.removeAt(row);
    endRemoveRows();
}

void MessagesModel::newConnection(QString topic){
    qDebug() << "MessagesModel::newConnection topic = " << topic;
    clientsub->disconnectFromHost();
    this->clear();
    clientsub->setTopic("AA:BB:CC");
    clientsub->connectAndSubscribe();
}

void MessagesModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, messages.count() - 1);
    messages.clear();
    endRemoveRows();
}

void registerQmlMessagesModelTypes() { qmlRegisterType<MessagesModel>("Backend", 1, 0, "MessagesModel"); }

Q_COREAPP_STARTUP_FUNCTION(registerQmlMessagesModelTypes)
