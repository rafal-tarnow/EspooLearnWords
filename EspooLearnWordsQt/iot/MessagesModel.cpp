#include "MessagesModel.h"
#include <QNetworkDatagram>
#include <QtQml>

MessagesModel::MessagesModel(QObject *parent) : QAbstractListModel(parent)
{
    append("fadljfnal", "M0001", "faldnfa", "1234", "57230457");
    append("222 fadljfnal", "M0001", "faldnfa", "1234", "57230457");

    /*
  //    client = new QMQTT::Client(QHostAddress::LocalHost, 1883);
          qDebug() << "RTT New QMQTT::Client";
      client = new QMQTT::Client("ws://localhost:9001/", "<origin>", QWebSocketProtocol::VersionLatest);
//      client->setHost(QHostAddress("127.0.0.1"));
//      client->setPort(9001);
      //client->setClientId(QString("clientId"));
      //client->setUsername("user");
      //client->setPassword("password");
      //client->setHostName("ws://localhost:9001/");
      client->connectToHost();
      QObject::connect(client, &QMQTT::Client::connected, [&]() {
          // Investigate the errors here, if you find no serious problems, call ignoreSslErrors()
          // to continue connecting.
          qDebug( )<< "Connected mqtt";
          client->subscribe("outTopic");
      });
      QObject::connect(client, &QMQTT::Client::received, [&](const QMQTT::Message &message) {
          // Investigate the errors here, if you find no serious problems, call ignoreSslErrors()
          // to continue connecting.
          qDebug() << message.payload();
          append(message.payload(), "M0001", "faldnfa", "1234", "57230457");
      });

*/

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
    clientsub->setUrl(QUrl("ws://broker.hivemq.com:8000/mqtt"));
    clientsub->setTopic("rafal-tarnow");

    //clientsub.setVersion(4);
    clientsub->setVersion(3);

    clientsub->connectAndSubscribe();

    connect(clientsub, &ClientSubscription::messageReceived, this, [this](const QByteArray & message){
        qDebug() << "Newwwww messsage  " << message;
        append(QString(message), "M0001", "faldnfa", "1234", "57230457");
    });

}

MessagesModel::~MessagesModel()
{
    if(m_client != nullptr)
        delete m_client;
    if(clientsub != nullptr)
        delete clientsub;
    if(client != nullptr)
        delete client;
}

void MessagesModel::setSearchStatus(const bool &search)
{
    searchStatus = search;
    if (searchStatus) {
        elapsedTimer.start();
    }
    else {

    }
    Q_EMIT seachChanged();
}


bool MessagesModel::searchDevices() const { return searchStatus; }


void MessagesModel::updateLastResponseTime(const QString &devName,const QString &ipAddress){
    for (auto &device : m_devices) {
        if ((device.deviceName == devName) && (device.ipAddress == ipAddress)) {
            device.lastResponseTime = elapsedTimer.elapsed();
        }
    }
}

bool MessagesModel::deviceArleadyAdded(const QString &ipAddress)
{
    for (auto &device : m_devices) {
        if (device.ipAddress == ipAddress) {
            return true;
        }
    }
    return false;
}

int MessagesModel::rowCount(const QModelIndex &) const { return m_devices.count(); }

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role) {
        case DeviceNameRole:
            return m_devices.at(index.row()).deviceName;
        case ModuleType:
            return m_devices.at(index.row()).moduleType;
        case IpAddressRole:
            return m_devices.at(index.row()).ipAddress;
        case PortRole:
            return m_devices.at(index.row()).port;
        case SerialNumberRole:
            return m_devices.at(index.row()).serialNumber;
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
    const Device device = m_devices.value(row);
    return {{"deviceName", device.deviceName}, {"moduleType", device.moduleType}, {"ipAddress", device.ipAddress}, {"port", device.port}, {"serialNumber", device.serialNumber}};
}

void MessagesModel::append(const QString &deviceName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber)
{
    int row = 0;
    while (row < m_devices.count() && deviceName > m_devices.at(row).deviceName)
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    m_devices.insert(row, {deviceName, moduleType, ipAddress, port, serialNumber, elapsedTimer.elapsed()});
    endInsertRows();
}

void MessagesModel::set(int row, const QString &deviceName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber)
{
    if (row < 0 || row >= m_devices.count())
        return;

    m_devices.replace(row, {deviceName, moduleType, ipAddress, port, serialNumber});
    dataChanged(index(row, 0), index(row, 0), {DeviceNameRole, ModuleType, IpAddressRole, PortRole, SerialNumberRole});
}

void MessagesModel::remove(int row)
{
    if (row < 0 || row >= m_devices.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_devices.removeAt(row);
    endRemoveRows();
}

void MessagesModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_devices.count() - 1);
    m_devices.clear();
    endRemoveRows();
}

void registerQmlMessagesModelTypes() { qmlRegisterType<MessagesModel>("Backend", 1, 0, "MessagesModel"); }

Q_COREAPP_STARTUP_FUNCTION(registerQmlMessagesModelTypes)
