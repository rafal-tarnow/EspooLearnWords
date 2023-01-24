// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef WebSocketIODevice_2_H
#define WebSocketIODevice_2_H

#include <QtCore/QIODevice>
#include <QtWebSockets/QWebSocket>

class WebSocketIODevice_2 : public QIODevice
{
    Q_OBJECT
public:
    WebSocketIODevice_2(QObject *parent = nullptr);

    bool open(OpenMode mode) override;
    void close() override;

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    void setUrl(const QUrl &url);
    void setProtocol(const QByteArray &data);
Q_SIGNALS:
    void socketConnected();

public slots:
    void handleBinaryMessage(const QByteArray &msg);
    void onSocketConnected();

private:
    QByteArray m_protocol;
    QByteArray m_buffer;
    QWebSocket m_socket;
    QUrl m_url;
};

#endif // WebSocketIODevice_2_H
