#include "MessagesModel.h"
#include <QNetworkDatagram>
#include <QtQml>

MessagesModel::MessagesModel(QObject *parent) : QAbstractListModel(parent)
{



    clientsub = new ClientSubscription();
    clientsub->setUrl(QUrl("wss://broker.hivemq.com:8884/mqtt"));
    //clientsub->setUrl(QUrl("ws://localhost:8000"));
    //clientsub.setVersion(4);
    clientsub->setVersion(3);



    connect(clientsub, &ClientSubscription::connectedToMqtt, this, &MessagesModel::connectedToMqtt);
    connect(clientsub, &ClientSubscription::disconnectedFormHost, this, &MessagesModel::disconnectedFromHost);
    connect(clientsub, &ClientSubscription::messageReceived, this, &MessagesModel::onMqttMessageArrive);
//    clientsub->setTopic("AA:BB:CC");
//    clientsub->connectAndSubscribe();

}

void MessagesModel::onMqttMessageArrive(const QByteArray & message){
//    qDebug() << "Newwwww messsage  " << message;

    parseFrame(QString(message));

}

void MessagesModel::parseFrame(QString frame){
    QStringList nameAndValueList = frame.split(';');
    QMap<QString, float> values;

    foreach(QString str, nameAndValueList) {
        QStringList nameAndValue = str.split("=");
        values[nameAndValue[0]] = nameAndValue[1].toFloat();
    }

    foreach(QString key, values.keys()) {
        int row = valueExist(key);
        if(row == -1){
              append(key , QString::number(values[key]), "faldnfa", "1234", "57230457");
        }else{
            set(row, key , QString::number(values[key]), "faldnfa", "1234", "57230457");
        }
    }

}

int MessagesModel::valueExist(const QString & valueName){
    for (int i =0 ; i < mValues.size(); i++) {
        if(mValues.at(i).valueName == valueName)
            return i;
    }
    return -1;
}

MessagesModel::~MessagesModel()
{
    clientsub->disconnectFromHost();
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
    for (auto &device : mValues) {
        if (device.ipAddress == ipAddress) {
            return true;
        }
    }
    return false;
}

int MessagesModel::rowCount(const QModelIndex &) const { return mValues.count(); }

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role) {
        case DeviceNameRole:
            return mValues.at(index.row()).valueName + " = " + mValues.at(index.row()).moduleType;
        case ModuleType:
            return mValues.at(index.row()).moduleType;
        case IpAddressRole:
            return mValues.at(index.row()).ipAddress;
        case PortRole:
            return mValues.at(index.row()).port;
        case SerialNumberRole:
            return mValues.at(index.row()).serialNumber;
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
    const Device device = mValues.value(row);
    return {{"deviceName", device.valueName}, {"moduleType", device.moduleType}, {"ipAddress", device.ipAddress}, {"port", device.port}, {"serialNumber", device.serialNumber}};
}

void MessagesModel::append(const QString &valueName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber)
{
    int row = 0;
    while (row < mValues.count() && valueName > mValues.at(row).valueName)
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    mValues.insert(row, {valueName, moduleType, ipAddress, port, serialNumber, row});
    endInsertRows();
}

void MessagesModel::set(int row, const QString &deviceName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber)
{
    if (row < 0 || row >= mValues.count())
        return;

    mValues.replace(row, {deviceName, moduleType, ipAddress, port, serialNumber});
    dataChanged(index(row, 0), index(row, 0), {DeviceNameRole, ModuleType, IpAddressRole, PortRole, SerialNumberRole});
}

void MessagesModel::remove(int row)
{
    if (row < 0 || row >= mValues.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    mValues.removeAt(row);
    endRemoveRows();
}

bool MessagesModel::isDisconnectedFromHost(){
    return clientsub->isDisconnectedFromHost();
}

void MessagesModel::disconnectFromHost(){
    qDebug() << "MessagesModel::disconnectFromHost()";
    clientsub->disconnectFromHost();
}

void MessagesModel::newConnection(QString topic){
    qDebug() << "MessagesModel::newConnection topic = " << topic;
    clientsub->setTopic(topic);
    clientsub->connectAndSubscribe();
}

void MessagesModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, mValues.count() - 1);
    mValues.clear();
    endRemoveRows();
}

void registerQmlMessagesModelTypes() { qmlRegisterType<MessagesModel>("Backend", 1, 0, "MessagesModel"); }

Q_COREAPP_STARTUP_FUNCTION(registerQmlMessagesModelTypes)
