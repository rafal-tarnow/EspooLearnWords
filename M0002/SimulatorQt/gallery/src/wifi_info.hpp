#pragma once

#include <QObject>
#include <QNetworkInformation>

class NetworkInfo : public QObject {
    Q_OBJECT

public:
    NetworkInfo(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE QString getConnectedWifiName() {


        return "Not connected";
    }
};

