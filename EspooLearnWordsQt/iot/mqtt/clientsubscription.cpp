// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "clientsubscription.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QLoggingCategory>

Q_LOGGING_CATEGORY(lcWebSocketMqtt, "qtdemo.websocket.mqtt")

ClientSubscription::ClientSubscription(QObject *parent) : QObject(parent)
{
#warning "we should quit when error?"
    connect(this, &ClientSubscription::errorOccured, qApp, &QCoreApplication::quit);
    connect(&ioDevice, &WebSocketIODevice::socketConnected, this, &ClientSubscription::onSocketConnected);
    connect(&ioDevice, &WebSocketIODevice::socketDisconnected, this, &ClientSubscription::onSocketDisconnected);
    connect(&m_client, &QMqttClient::connected, this, &ClientSubscription::onMqttConnected);
    connect(&m_client, &QMqttClient::disconnected, this, &ClientSubscription::onMqttDisconnected);
    m_client.setTransport(&ioDevice, QMqttClient::IODevice);
}

void ClientSubscription::setUrl(const QUrl &url)
{
    m_url = url;
    ioDevice.setUrl(m_url);
}

void ClientSubscription::setTopic(const QString &topic)
{
    m_topic = topic;
}

void ClientSubscription::setVersion(int v)
{
    m_version = v;
    ioDevice.setProtocol(m_version == 3 ? "mqttv3.1" : "mqtt");
    m_client.setProtocolVersion(m_version == 3 ? QMqttClient::MQTT_3_1 : QMqttClient::MQTT_3_1_1);
}

void ClientSubscription::disconnectFromHost(){
    connect(&m_client, &QMqttClient::disconnected, this, [this]() {
        qCDebug(lcWebSocketMqtt) << "MQTT disconnected";

        disconnect(&m_client, &QMqttClient::connected, nullptr, nullptr);
        disconnect(m_subscription, &QMqttSubscription::stateChanged,nullptr, nullptr);
        disconnect(m_subscription, &QMqttSubscription::messageReceived,nullptr, nullptr);


    });
    m_client.disconnectFromHost();
}

void ClientSubscription::onMqttDisconnected(){

}

void ClientSubscription::onMqttConnected(){
    qCDebug(lcWebSocketMqtt) << "MQTT connection established";

    m_subscription = m_client.subscribe(m_topic);
    if (!m_subscription) {
        qDebug() << "Failed to subscribe to " << m_topic;
        emit errorOccured();
    }

    connect(m_subscription, &QMqttSubscription::stateChanged,
            [](QMqttSubscription::SubscriptionState s) {
        qCDebug(lcWebSocketMqtt) << "Subscription state changed:" << s;
    });

    connect(m_subscription, &QMqttSubscription::messageReceived,
            [this](QMqttMessage msg) {
        handleMessage(msg.payload());
    });
}

void ClientSubscription::onSocketDisconnected(){

}

void ClientSubscription::onSocketConnected(){
    qCDebug(lcWebSocketMqtt) << "WebSocket connected, initializing MQTT connection.";
    m_client.connectToHost();
}

void ClientSubscription::connectAndSubscribe()
{
    qCDebug(lcWebSocketMqtt) << "Connecting to broker at " << m_url;

    if (!ioDevice.open(QIODevice::ReadWrite))
        qDebug() << "Could not open socket device";
}

void ClientSubscription::handleMessage(const QByteArray &msgContent)
{
    // Should happen when the internal device has ready read?
    qInfo() << "New message:" << msgContent;
    emit messageReceived(msgContent);
}
