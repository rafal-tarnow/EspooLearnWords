#pragma once

#include <QObject>
#include "pseudo_dns.hpp"

class EmulatorM0002  : public QObject{
    Q_OBJECT
public:
    explicit EmulatorM0002(QObject *parent = nullptr);
    ~EmulatorM0002();

    Q_INVOKABLE void start(const QString & moduleName, const QString& serverName);
    Q_INVOKABLE void stop();

private:
    std::unique_ptr<PseudoDNSServer> pseudoDNSServer;
    QString mServerName;
    QString mModuleName;
};
