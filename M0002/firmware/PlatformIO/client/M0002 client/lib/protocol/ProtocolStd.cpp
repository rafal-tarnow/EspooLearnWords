#include "ProtocolStd.h"


void ProtocolStd::addData(const std::vector<uint8_t>& data) {
    buffer.insert(buffer.end(), data.begin(), data.end());

    while (!buffer.empty()) {
        switch (currentState) {
        case WaitForDataSizeMSB:
            if (buffer.size() >= 1) {
                expectedSize = static_cast<uint16_t>(buffer[0]) << 8;
                buffer.erase(buffer.begin());
                currentState = WaitForDataSizeLSB;
            } else {
                return;
            }
            break;

        case WaitForDataSizeLSB:
            if (buffer.size() >= 1) {
                expectedSize += static_cast<uint16_t>(buffer[0]);
                buffer.erase(buffer.begin());
                currentState = CompleteData;
            } else {
                return;
            }
            break;

        case CompleteData:
            if (buffer.size() >= expectedSize) {
                std::deque<uint8_t> frame(buffer.begin(), buffer.begin() + expectedSize);
                buffer.erase(buffer.begin(), buffer.begin() + expectedSize);
                expectedSize = 0;
                currentState = WaitForDataSizeMSB;

                if (onFrameCallback) {
                    onFrameCallback(frame);
                }
                if (onFrameMethodCallback) {
                    onFrameMethodCallback(frame);
                }
            } else {
                return;
            }
            break;
        }
    }
}

void ProtocolStd::setOnFrameCallback(FrameCallback callback) {
    onFrameCallback = callback;
}

void ProtocolStd::prepend(std::deque<uint8_t>& deque, const uint16_t& data) {
    deque.push_front(static_cast<uint8_t>(data & 0xFF));
    deque.push_front(static_cast<uint8_t>((data >> 8) & 0xFF));
}

void ProtocolStd::append(std::vector<uint8_t>& array, const bool& data) {
    append(array, static_cast<uint8_t>(data));
}
void ProtocolStd::append(std::vector<uint8_t>& array, const uint8_t& data) {
    array.push_back(data);
}

void ProtocolStd::append(std::vector<uint8_t>& array, const uint16_t& data) {
    array.push_back(static_cast<uint8_t>((data >> 8) & 0xFF));
    array.push_back(static_cast<uint8_t>(data & 0xFF));
}

void ProtocolStd::append(std::vector<uint8_t>& array, const uint32_t& data) {
    array.push_back(static_cast<uint8_t>((data >> 24) & 0xFF));
    array.push_back(static_cast<uint8_t>((data >> 16) & 0xFF));
    array.push_back(static_cast<uint8_t>((data >> 8) & 0xFF));
    array.push_back(static_cast<uint8_t>(data & 0xFF));
}

void ProtocolStd::append(std::vector<uint8_t>& array, const float& data) {
    static_assert(sizeof(float) == sizeof(uint32_t), "Size of float must be equal to size of uint32_t");
    uint32_t data_uint = *reinterpret_cast<const uint32_t*>(&data);
    append(array, data_uint);
}

void ProtocolStd::append(std::vector<uint8_t> &array, const std::string &data)
{
    append(array, static_cast<uint16_t>(data.size()));
    array.insert(array.end(), data.begin(), data.end());
}

#ifdef QT_CORE_LIB
void ProtocolStd::append(QByteArray& array, const uint8_t& data) {
    array.append(data);
}

void ProtocolStd::append(QByteArray& array, const uint16_t& data) {
    array.append(static_cast<uint8_t>((data >> 8) & 0xFF));
    array.append(static_cast<uint8_t>(data & 0xFF));
}

void ProtocolStd::append(QByteArray& array, const uint32_t& data) {
    array.append(static_cast<uint8_t>((data >> 24) & 0xFF));
    array.append(static_cast<uint8_t>((data >> 16) & 0xFF));
    array.append(static_cast<uint8_t>((data >> 8) & 0xFF));
    array.append(static_cast<uint8_t>(data & 0xFF));
}


void ProtocolStd::append(QByteArray& array, const float& data) {
    static_assert(sizeof(float) == sizeof(uint32_t), "Size of float must be equal to size of uint32_t");
    uint32_t data_uint = *reinterpret_cast<const uint32_t*>(&data);
    append(array, data_uint);
}

void ProtocolStd::append(QByteArray& array, const QString& text) {
    QByteArray rawBinaryText = text.toUtf8();
    append(array, static_cast<uint16_t>(rawBinaryText.size()));
    array.append(rawBinaryText);
}

uint8_t ProtocolStd::getUint8_t(QByteArray& array){
    if (array.size() < 1) {
        return 0;
    }

    uint8_t value = static_cast<uint8_t>(array.at(0));
    array.remove(0, 1);
    return value;
}

uint16_t ProtocolStd::getUint16_t(QByteArray &array) {
    qDebug() << "ProtocolStd::getUint16_t() = " << array;
    if (array.size() < 2) {
        return 0;
    }

    uint8_t msb = static_cast<uint8_t>(array.at(0));
    qDebug() << "msb = " << msb;
    array.remove(0, 1);
    uint8_t lsb = static_cast<uint8_t>(array.at(0));
    qDebug() << "lsb = " << msb;
    array.remove(0, 1);

    uint16_t value = (static_cast<uint16_t>(msb) << 8) | lsb;
    return value;
}

QString ProtocolStd::getQString(QByteArray &array) {
    qDebug() << "ProtocolStd::getQString() = " << array;
    uint16_t stringSize = getUint16_t(array);
    qDebug() << "stringSize = " << stringSize;
    if (array.size() < stringSize) {
        return QString();
    }

    QString result = QString::fromUtf8(array.constData(), stringSize);
    qDebug() << "result " << result;
    array.remove(0, stringSize);

    return result;
}
#endif

bool ProtocolStd::getBool(std::deque<uint8_t>& frame) {
    if (frame.empty()) {
        return false;
    }
    uint8_t byte = frame.front();
    frame.pop_front();
    return static_cast<bool>(byte);
}

uint8_t ProtocolStd::getUint8_t(std::vector<uint8_t>& frame) {
    if (frame.empty()) {
        return 0;
    }
    uint8_t byte = frame.at(0);  // lub frame[0];
    frame.erase(frame.begin());
    return byte;
}

uint8_t ProtocolStd::getUint8_t(std::deque<uint8_t>& frame) {
    if (frame.empty()) {
        return 0;
    }
    uint8_t byte = frame.front();
    frame.pop_front();
    return byte;
}

uint16_t ProtocolStd::getUint16_t(std::deque<uint8_t>& frame) {
    if (frame.size() < 2) {
        return 0;
    }
    uint8_t msb = frame.front();
    frame.pop_front();
    uint8_t lsb = frame.front();
    frame.pop_front();
    uint16_t value = (static_cast<uint16_t>(msb) << 8) | lsb;
    return value;
}

uint32_t ProtocolStd::getUint32_t(std::deque<uint8_t>& frame) {
    if (frame.size() < 4) {
        return 0;
    }
    uint8_t msb1 = frame.front();
    frame.pop_front();
    uint8_t msb2 = frame.front();
    frame.pop_front();
    uint8_t msb3 = frame.front();
    frame.pop_front();
    uint8_t lsb = frame.front();
    frame.pop_front();

    uint32_t value = (static_cast<uint32_t>(msb1) << 24) | (static_cast<uint32_t>(msb2) << 16) | (static_cast<uint32_t>(msb3) << 8) | lsb;
    return value;
}

float ProtocolStd::getFloat(std::deque<uint8_t>& frame) {
    static_assert(sizeof(float) == sizeof(uint32_t), "Size of float must be equal to size of uint32_t");
    uint32_t value32 = getUint32_t(frame);
    float valueFloat = *reinterpret_cast<const float*>(&value32);
    return valueFloat;
}

std::string ProtocolStd::getString(std::deque<uint8_t>& frame) {
    uint16_t stringSize = ProtocolStd::getUint16_t(frame);
    if (frame.size() < stringSize) {
        return "";
    }
    std::string result(frame.begin(), frame.begin() + stringSize);
    frame.erase(frame.begin(), frame.begin() + stringSize);
    return result;
}

void ProtocolStd::reset() {
    buffer.clear();
    currentState = WaitForDataSizeMSB;
    expectedSize = 0;
}
