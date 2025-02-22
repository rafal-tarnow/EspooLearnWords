// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef WEBSOCKETIODEVICE_H
#define WEBSOCKETIODEVICE_H

#include <QtCore/QIODevice>
#include <QtWebSockets/QWebSocket>

class WebSocketIODevice : public QIODevice
{
    Q_OBJECT
public:
    WebSocketIODevice(QObject *parent = nullptr);

    bool open(OpenMode mode) override;
    void close() override;

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    bool isDisconnected();
    void setUrl(const QUrl &url);
    void setProtocol(const QByteArray &data);
Q_SIGNALS:
    void socketConnected();
    void socketDisconnected();

public slots:
    void handleBinaryMessage(const QByteArray &msg);
    void onSocketConnected();
    void onSocketDisconnected();

private:
    QByteArray m_protocol;
    QByteArray m_buffer;
    QWebSocket m_socket;
    QUrl m_url;
};

#endif // WEBSOCKETIODEVICE_H
