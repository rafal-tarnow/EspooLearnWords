#include <QObject>
#include <QUdpSocket>
#include <memory>
#include <QMap>
#include <QNetworkAddressEntry>

class UdpTerminal : public QObject
{
    Q_OBJECT
public:
    explicit UdpTerminal(QObject *parent = nullptr);
    ~UdpTerminal();

    Q_INVOKABLE void bind(const QString& port);
    Q_INVOKABLE void send(const QString& data, const QString & toIP, const QString & toPort);
    Q_INVOKABLE void sendHexString(const QString& hexString, const QString & toIP, const QString & toPort);
    Q_INVOKABLE void sendBroadcast(const QString& data, const QString & toPort);
    Q_INVOKABLE void sendBroadcastHexString(const QString& hexString, const QString & toPort);
    Q_INVOKABLE void close();

signals:
    void dataReceived(QString data, QString formIp, QString fromPort);

private slots:
    void readPendingDatagrams();

private:
    std::unique_ptr<QUdpSocket> udpSocket;
    QMap<int, QNetworkAddressEntry> interfaceAdresses; // int - interface index, QNetworkAddressEntry - adresses at network interface
    QByteArray convertHexStringToByteArray(const QString &inputString);
    void getAllBroadcastAdresses();
    bool isMyAddress(const QHostAddress &address);
};
