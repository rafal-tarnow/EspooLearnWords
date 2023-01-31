#ifdef QT_WEBSOCKETS_LIB

#include "qmqtt_websocketiodevice_p.h"

#include <QWebSocket>

QMQTT::WebSocketIODevice::WebSocketIODevice(QWebSocket *socket, QObject *parent)
    : QIODevice(parent)
    , _webSocket(socket)
{
    connect(_webSocket, &QWebSocket::bytesWritten, this, &WebSocketIODevice::bytesWritten);
    connect(_webSocket, &QWebSocket::binaryMessageReceived,
            this, &WebSocketIODevice::binaryMessageReceived);
}

bool QMQTT::WebSocketIODevice::connectToHost(const QNetworkRequest &request)
{
    _webSocket->open(request);
    return QIODevice::open(QIODevice::ReadWrite);
}

qint64 QMQTT::WebSocketIODevice::bytesAvailable() const
{
    return _buffer.size();
}

void QMQTT::WebSocketIODevice::binaryMessageReceived(const QByteArray &frame)
{
    _buffer.append(frame);
    emit readyRead();
}

qint64 QMQTT::WebSocketIODevice::readData(char *data, qint64 maxSize)
{
    int size = qMin(static_cast<int>(maxSize), _buffer.size());
    for (int i=0; i<size; ++ i)
        data[i] = _buffer[i];
    _buffer.remove(0, size);
    return size;
}

qint64 QMQTT::WebSocketIODevice::writeData(const char *data, qint64 maxSize)
{
    qDebug() << "QMQTT::WebSocketIODevice::writeData() maxSize = " << maxSize;
    QByteArray d(data, static_cast<int>(maxSize));
    qint64 writeBytes = _webSocket->sendBinaryMessage(d);
    qDebug() << "socket error = " << _webSocket->errorString();
    qDebug() << "socket error = " << _webSocket->error();
    qDebug() << "writeBytes = " << writeBytes;
    return writeBytes;
}

#endif // QT_WEBSOCKETS_LIB
