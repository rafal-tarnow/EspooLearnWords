#pragma once
#include <vector>
#include <deque>
#include <string>
#include <cstdint>
#include <functional>
#ifdef QT_CORE_LIB //this file is also used in Qt app
#include <QByteArray>
#include <QString>
#endif

class ProtocolStd {
public:
    using FrameCallback= std::function<void(std::deque<uint8_t>& )>;

    void addData(const std::vector<uint8_t>& data);


    void setOnFrameCallback(FrameCallback callback);
    template<typename T>
    void setOnFrameCallback(T *obj,void (T::*method)(std::deque<uint8_t>&)) {
        onFrameMethodCallback = std::bind(method, obj, std::placeholders::_1);
    }
    void reset();

    static void prepend(std::deque<uint8_t>& deque, const uint16_t& data);

    static void append(std::vector<uint8_t>& array, const bool& data);
    static void append(std::vector<uint8_t>& array, const uint8_t& data);
    static void append(std::vector<uint8_t>& array, const uint16_t& data);
    static void append(std::vector<uint8_t>& array, const uint32_t& data);
    static void append(std::vector<uint8_t>& array, const float& data);
#ifdef QT_CORE_LIB
    static void append(QByteArray& array, const uint8_t& data);
    static void append(QByteArray& array, const uint16_t&  data);
    static void append(QByteArray& array, const uint32_t& data);
    static void append(QByteArray& array, const float& data);
    static void append(QByteArray& array, const QString& text);
#endif
    static bool getBool(std::deque<uint8_t>& frame);
    static uint8_t getUint8_t(std::vector<uint8_t> & frame);
    static uint8_t getUint8_t(std::deque<uint8_t>& frame);
    static uint16_t getUint16_t(std::deque<uint8_t>& frame);
    static uint32_t getUint32_t(std::deque<uint8_t>& frame);
    static float getFloat(std::deque<uint8_t>& frame);
    static std::string getString(std::deque<uint8_t>& frame);
private:
    enum State {
        WaitForDataSizeMSB,
        WaitForDataSizeLSB,
        CompleteData
    };
    std::vector<uint8_t> buffer;
    State currentState = WaitForDataSizeMSB;
    uint16_t expectedSize = 0;
    FrameCallback onFrameCallback = nullptr;
    FrameCallback onFrameMethodCallback = nullptr;
};

