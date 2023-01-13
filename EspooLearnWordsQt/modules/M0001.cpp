#include "M0001.h"
#include <QNetworkDatagram>

M0001::M0001(QString name)
{
    moduleName = name;
    initSocket();
    getIpAddresTimer = new QTimer(this);

    initIpAddress();
}

M0001::~M0001()
{
    delete getIpAddresTimer;
    delete socket;
}

void M0001::initSocket()
{
    socket = new QUdpSocket();
    socket->bind(45554, QUdpSocket::ShareAddress);
    connect(socket, &QUdpSocket::readyRead, this, &M0001::readDatagramCallback);
}

void M0001::initGetIpTimer()
{
    connect(getIpAddresTimer, &QTimer::timeout, this, &M0001::getIpAddressTimerCallback);
    getIpAddresTimer->start(250);
}

void M0001::uninitGetIpTimer()
{
    getIpAddresTimer->stop();
    disconnect(getIpAddresTimer, &QTimer::timeout, this, &M0001::getIpAddressTimerCallback);
}

void M0001::updateAllBroadcastAdresses()
{
    QList<QNetworkInterface> listOfInterfaces = QNetworkInterface::allInterfaces();
    for (auto &interface : listOfInterfaces) {
        QList<QNetworkAddressEntry> listOfAddresEntries = interface.addressEntries();
        for (auto &addresEntry : listOfAddresEntries) {
            if (!addresEntry.broadcast().isNull()) {
                qDebug() << "DevicesManager::findAllBroadcastAdresses() " << addresEntry;
                broadcasts.insert(interface.index(), addresEntry);
            }
        }
    }
}

void M0001::initIpAddress()
{
    updateAllBroadcastAdresses();
    initGetIpTimer();
    getIpAddressTimerCallback(); // call immediatli callback to get fastest Ip
}

void M0001::initDevice()
{
    executeApiCommand(GET_STATUS);
}

void M0001::stopGettingIpAddress()
{
    uninitGetIpTimer();
    // uninitGetIpSocket();
}

void M0001::readDatagramCallback()
{
    qDebug() << "M0001::readIpDatagramCallback() ";

    qDebug() << "M0001: ipAdress.isNull = " << ipAddress.isNull();

    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();
        QByteArray data = datagram.data();
        switch(data.at(0)){
        case 0x01:
            parseEchoAcqFrame(datagram);
            break;
        case 0x04:
            parseFullStatusAcqFrame(data);
            break;
        }
    }
}

void M0001::parseFullStatusAcqFrame(const QByteArray & data){
    qDebug() << "M0001::parseFullStatusAcqFrame()";
    uint8_t secondByte = data.at(1);
      qDebug() << "secondByteee = " << int(data.at(1));
    qDebug() << "secondByte = " << secondByte;
    if(secondByte & 0b00000001){
        deviceStatus.onOff = true;
    }else{
        deviceStatus.onOff = false;
    }

    emit deviceInited();
}

bool M0001::getOnOff(){
    return deviceStatus.onOff;
}

void M0001::parseEchoAcqFrame(const QNetworkDatagram & datagram){
    QByteArray data = datagram.data();
    int startIndex = 1;
    int length = data.size() - startIndex;
    QString recivedDeviceName = QString::fromUtf8(data.mid(startIndex, length).data());
    if (recivedDeviceName == moduleName) {
        ipAddress = datagram.senderAddress();
        ipAddressOK = true;
        qDebug() << "deviceName = " << recivedDeviceName << " IP = " << ipAddress;
    }
    if(ipAddressOK){
        stopGettingIpAddress();
        initDevice();
    }
}

void M0001::getIpAddressTimerCallback()
{
    qDebug() << "M0001::getIpAddressTimerCallback() ";

#warning moze jest mozliwe sluchanie czy nie zmienilo sie polaczenie z siecia zamiat ciagle skanowac?
    updateAllBroadcastAdresses(); // we need update all broadcast addreses to void situation when user first start app and then connect to network

    QByteArray data;
    data.append(char(1));

    QMapIterator<int, QNetworkAddressEntry> iter(broadcasts);

    while (iter.hasNext()) {
        iter.next();

        int interfaceIndex = iter.key();
        QNetworkAddressEntry adresses = iter.value();

        QNetworkDatagram datagram;
        datagram.setInterfaceIndex(interfaceIndex);
        datagram.setDestination(adresses.broadcast(), 45455);
        datagram.setSender(adresses.ip(), 45454);
        datagram.setData(data);
        qDebug() << "send broadcast = " << adresses.broadcast();
        qDebug() << "send = " << socket->writeDatagram(datagram);
    }
}

void M0001::executeApiCommand(ApiCommand cmd, bool val)
{
    qDebug() << "M0001::executeApiCommand = " << cmd;
    if (ipAddressOK) {
        if (cmd == SET_NETWORK_CONFIG) {
            QByteArray data;
            data.append(char(3));
            data.append(char(val));
            qDebug() << "M0001::befor writeDatagram ";
            socket->writeDatagram(data, ipAddress, 45455);
            qDebug() << "M0001::after writeDatagram ";
        }
    }
}

void M0001::executeApiCommand(ApiCommand cmd)
{
    qDebug() << "M0001::executeApiCommand = " << cmd;
    if (ipAddressOK) {
        if (cmd == TURN_OFF_LED) {
            QByteArray data;
            data.append(char(2));
            data.append(char(0));
            qDebug() << "M0001::befor writeDatagram ";
            socket->writeDatagram(data, ipAddress, 45455);
            qDebug() << "M0001::after writeDatagram ";
        }
        else if (cmd == TURN_ON_LED) {
            QByteArray data;
            data.append(char(2));
            data.append(char(1));
            socket->writeDatagram(data, ipAddress, 45455);
        }
        else if (cmd == GET_STATUS) {
            QByteArray data;
            data.append(char(4));
            socket->writeDatagram(data, ipAddress, 45455);
        }
    }
}

void M0001::readPendingDatagrams() {
    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();

        switch(datagram.data().at(0)){
        case 0x04:
            qDebug() << "Full status acq";
            break;
        }

        //        if (recivedDeviceName == moduleName) {

        //            qDebug() << "deviceName = " << recivedDeviceName << " IP = " << ipAddress;
        //        }
    }
}
