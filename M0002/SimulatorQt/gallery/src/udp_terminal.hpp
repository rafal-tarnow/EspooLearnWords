#include <QObject>
#include <QUdpSocket>
#include <memory>

class UdpTerminal : public QObject
{
    Q_OBJECT
public:
    explicit UdpTerminal(QObject *parent = nullptr);
    ~UdpTerminal();

    Q_INVOKABLE void bind(const QString& port);
    Q_INVOKABLE void close();

signals:
    void dataReceived(QString data);

private slots:
    void readPendingDatagrams();

private:
    std::unique_ptr<QUdpSocket> udpSocket;
};
