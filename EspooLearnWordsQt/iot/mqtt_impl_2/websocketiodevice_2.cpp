// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "websocketiodevice_2.h"

#include <QtCore/QDebug>

#include <QtWebSockets/qwebsockethandshakeoptions.h>

WebSocketIODevice_2::WebSocketIODevice_2(QObject *parent)
    : QIODevice(parent)
{
    connect(&m_socket, &QWebSocket::connected, this, &WebSocketIODevice_2::onSocketConnected);
    connect(&m_socket, &QWebSocket::binaryMessageReceived, this, &WebSocketIODevice_2::handleBinaryMessage);
}

bool WebSocketIODevice_2::open(QIODevice::OpenMode mode)
{
    QWebSocketHandshakeOptions options;
    options.setSubprotocols({m_protocol});

    m_socket.open(m_url, options);

    return QIODevice::open(mode);
}

void WebSocketIODevice_2::close()
{
    m_socket.close();
    QIODevice::close();
}

qint64 WebSocketIODevice_2::readData(char *data, qint64 maxlen)
{
    qint64 bytesToRead = qMin(maxlen, (qint64)m_buffer.size());
    memcpy(data, m_buffer.constData(), bytesToRead);
    m_buffer = m_buffer.right(m_buffer.size() - bytesToRead);
    return bytesToRead;
}

qint64 WebSocketIODevice_2::writeData(const char *data, qint64 len)
{
    QByteArray msg(data, len);
    const int length = m_socket.sendBinaryMessage(msg);
    return length;
}

void WebSocketIODevice_2::setUrl(const QUrl &url)
{
    m_url = url;
}

void WebSocketIODevice_2::setProtocol(const QByteArray &data)
{
    m_protocol = data;
}

void WebSocketIODevice_2::handleBinaryMessage(const QByteArray &msg)
{
    m_buffer.append(msg);
    emit readyRead();
}

void WebSocketIODevice_2::onSocketConnected()
{
    emit socketConnected();
}
