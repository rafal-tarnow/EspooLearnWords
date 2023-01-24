#ifndef QMQTTWEBSOCKETCLIENT_H
#define QMQTTWEBSOCKETCLIENT_H

#include <QtMqtt/QMqttClient>
#include "websocketiodevice.h"

// A simple QMqttClient wrapper that supports connecting via WebSocket
class QMqttWebsocketClient : public QMqttClient
{
public:
    QMqttWebsocketClient(QObject *parent)
        :QMqttClient(parent)
    {
        setProtocolVersion(QMqttClient::MQTT_3_1);
        setTransport(&m_device, QMqttClient::IODevice);

        connect(&m_device, &WebSocketIODevice::socketConnected, this, [this]() {
            this->connectToHost();
        });
    }

    void connectToWebSocketHost()
    {
        QUrl url("ws://" + hostname() + ":" + QString::number(port()) + "/");
        m_device.setUrl(url);
        m_device.setProtocol("mqttv3.1");
        if (!m_device.open(QIODevice::ReadWrite))
            qDebug() << "Could not open socket device";
    }

    void disconnectFromWebSocketHost()
    {
        disconnectFromHost();
        m_device.close();
    }

private:
    WebSocketIODevice m_device;
};

#endif // QMQTTWEBSOCKETCLIENT_H
