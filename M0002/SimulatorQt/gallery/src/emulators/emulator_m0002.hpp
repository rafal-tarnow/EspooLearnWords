#pragma once

#include <QObject>
#include <QDebug>
#include <QTimer>
#include "EmulatorM0002States.h"
#include "pseudo_dns.hpp"
#include "tcp_connection.hpp"

class EmulatorM0002  : public QObject{
    Q_OBJECT
public:
    Q_PROPERTY(float temp READ getTemp WRITE setTemp NOTIFY tempChanged)
    explicit EmulatorM0002(QObject *parent = nullptr);
    ~EmulatorM0002();

    Q_INVOKABLE void start(const QString & moduleName, const QString& serverName);
    Q_INVOKABLE void stop();

    float getTemp() const;
    void setTemp(float newTemp);

signals:
    void tempChanged();

private slots:
    void onHostFound(const QString & hostName, const QString & hostIP);
    void onTcpConnected();
    void onTcpConnectingTimeout();
    void onTcpDisconnected();
    void onTcpError(const QString & error);
    void onTcpFrame(const QByteArray & frame);
    void onMeasureTimer();

private:
    void enterIdleState();
    void exitIdleState();

    void enterRunState();
    void exitRunState();

    void enterSearchState();
    void exitSearchState();

    void enterConnectingState();
    void exitConnectingState();

    void enterConnectedState();
    void exitConnectedState();

    void on_capi_getNameAndType();
    void on_capi_setMeasureInterval(const QByteArray &frame);

    void sapi_setNameAndType(QString &moduleName, QString &moduleType);
    void sapi_setTemp(float temp);
private:
    quint16 PORT = 2883;
    std::unique_ptr<PseudoDNSServer> pseudoDNSServer;
    std::unique_ptr<TcpConncetion> tcpConnection;
    std::unique_ptr<QTimer> measureTimer;
    QString mServerName;
    QString mServerIP;
    QString mModuleName;
    QString mModuleType;
    M0002StateMachine machine;
    float m_temp;
};
