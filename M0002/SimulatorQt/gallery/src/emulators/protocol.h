#pragma once
#include <QObject>
#include <QByteArray>

class Protocol : public QObject {
    Q_OBJECT
public:
    void addData(const QByteArray & data);

signals:
    void onFrame(const QByteArray & frame);

private:
    enum State {
        WaitForDataSizeMSB,
        WaitForDataSizeLSB,
        CompleteData
    };
    QByteArray buffer;
    State currentState = WaitForDataSizeMSB;
    int expectedSize;
};
