#pragma once

#include <cstdint>
#include <QObject>
#include "Controller.hpp"
#include "./src/ObjectCounter.hpp"

class J0001Controller : public Controller
{
    struct BrickStateMirror {
        uint64_t counter = 0;
        float milliliters = 0.0;
        float flowRate = 0.0;
        bool run = false;
        float dropletVolumeConfig = 0.05f; //0.05 ml
        uint8_t windowDurationConfig = 5; //5 seconds
        uint64_t runtime_s = 0;
        float maxFlowDisplay = 80;
    };

    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(uint64_t count READ count NOTIFY countChanged)
    Q_PROPERTY(bool run READ run WRITE setRun NOTIFY runChanged)
    Q_PROPERTY(float dropletVolume READ dropletVolume WRITE setDropletVolume NOTIFY dropletVolumeChanged)
    Q_PROPERTY(float milliliters READ milliliters NOTIFY millilitersChanged)
    Q_PROPERTY(double flowRate READ flowRate NOTIFY flowRateChanged)
    Q_PROPERTY(uint8_t windowDuration READ windowDuration WRITE setWindowDuration NOTIFY windowDurationChanged)
    Q_PROPERTY(uint32_t runtime READ runtime NOTIFY runtimeChanged FINAL)
    Q_PROPERTY(float maxFlowDisplay READ maxFlowDisplay WRITE setMaxFlowDisplay NOTIFY maxFlowDisplayChanged FINAL)

public:
    explicit J0001Controller(QObject *parent = nullptr, QString id = "", QString name = "");
    Q_INVOKABLE uint64_t count();
    Q_INVOKABLE double milliliters();
    Q_INVOKABLE bool run();
    Q_INVOKABLE float dropletVolume();
    Q_INVOKABLE void setRun(bool newState);
    Q_INVOKABLE void setDropletVolume(float dropletVolume);
    Q_INVOKABLE double flowRate(); // Calculate flow rate
    Q_INVOKABLE uint8_t windowDuration() const;
    Q_INVOKABLE void setWindowDuration(uint8_t duration);

    Q_INVOKABLE float maxFlowDisplay();
    Q_INVOKABLE void setMaxFlowDisplay(float maxFlowDisplay);

    Q_INVOKABLE void resetMeasurement();
    Q_INVOKABLE uint32_t runtime();



    QString type() override;

signals:
    void countChanged();
    void runChanged();
    void millilitersChanged();
    void flowRateChanged();
    void windowDurationChanged();
    void dropletVolumeChanged();
    void runtimeChanged();
    void maxFlowDisplayChanged();

protected:
    uint8_t handleProtocolFrame(QByteArray & frame) override;

private:
    void cmdSetRunState(bool run);
    void cmdSetDropletVolume(float dropletVolume);
    void cmdSetWindowDuration(uint8_t windowDuration);
    void cmdResetMeasurement();
    void cmdSetMaxFlowDisplay(float maxFlowDisplay);

private:
    BrickStateMirror m_stateMirror;

    std::deque<std::pair<uint64_t, qint64>> m_dropletHistory; // Store droplet history
    qint64 m_windowDuration = 5000; // Window duration in milliseconds

    DBG_COUNT("J0001Controller");
};
