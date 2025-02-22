// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CLIENTSUBSCRIPTION_H
#define CLIENTSUBSCRIPTION_H

#include "websocketiodevice.h"

#include <QtCore/QObject>
#include <QtMqtt/QMqttClient>
#include <QtWebSockets/QWebSocket>

class ClientSubscription : public QObject
{
    Q_OBJECT
public:
    ClientSubscription(QObject *parent = nullptr);

    void setUrl(const QUrl &url); // ie ws://broker.hivemq.com:8000/mqtt
    void setTopic(const QString &topic);
    void setVersion(int v);
signals:
    void messageReceived(QByteArray);
    void errorOccured();
    void disconnectedFormHost();
    void connectedToMqtt();
public slots:
    void connectAndSubscribe();
    void disconnectFromHost();
    bool isDisconnectedFromHost();
    void handleMessage(const QByteArray &msgContent);

private:
    void onMqttConnected();
    void onMqttDisconnected();
    void onSocketConnected();
    void onSocketDisconnected();

private:
    QMqttClient m_client;
    QMqttSubscription *m_subscription;
    QUrl m_url;
    QString m_topic;
    WebSocketIODevice ioDevice;
    int m_version;
    bool wantConnect = false;
};

#endif // CLIENTSUBSCRIPTION_H
