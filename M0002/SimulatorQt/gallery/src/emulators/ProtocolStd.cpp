#include "ProtocolStd.h"


void ProtocolStd::addData(const std::vector<uint8_t> &data)
{
    addData(data.data(), data.size());
}

void ProtocolStd::addData(const uint8_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        addData(data[i]);
    }
}

void ProtocolStd::addData(const uint8_t data)
{
    if (currentState == WaitForDataSizeMSB)
    {
        expectedSize = 0;
        buffer.clear();
        bufferIndex = 0;
        expectedSize = static_cast<uint16_t>(data) << 8;
        currentState = WaitForDataSizeLSB;
        return;
    }

    if (currentState == WaitForDataSizeLSB)
    {

        expectedSize += static_cast<uint16_t>(data);
        currentState = CompleteData;
        return;
    }

    if (currentState == CompleteData)
    {
        if (expectedSize > maxFrameSize)
        {
            bufferIndex++;
            if (bufferIndex >= expectedSize)
            {
                currentState = WaitForDataSizeMSB;
            }
        }
        else
        {
            buffer.push_back(data);
            if (buffer.size() >= expectedSize)
            {
                std::deque<uint8_t> frame(buffer.begin(), buffer.begin() + expectedSize);
                buffer.erase(buffer.begin(), buffer.begin() + expectedSize);
                expectedSize = 0;
                currentState = WaitForDataSizeMSB;

                if (onFrameCallback)
                {
                    onFrameCallback(frame);
                }
                if (extendedFrameCallback)
                {
                    extendedFrameCallback(this, frame);
                }
                if (onFrameMethodCallback)
                {
                    onFrameMethodCallback(frame);
                }
            }
            else
            {
                return;
            }
        }
    }
}

void ProtocolStd::setOnFrameCallback(FrameCallback callback)
{
    onFrameCallback = callback;
}

void ProtocolStd::setOnFrameCallback(ExtendedFrameCallback callback)
{
    extendedFrameCallback = callback;
}

void ProtocolStd::prepend(std::deque<uint8_t> &deque, const uint16_t &data)
{
    deque.push_front(static_cast<uint8_t>(data & 0xFF));
    deque.push_front(static_cast<uint8_t>((data >> 8) & 0xFF));
}

void ProtocolStd::append(std::vector<uint8_t> &array, const bool &data)
{
    append(array, static_cast<uint8_t>(data));
}
void ProtocolStd::append(std::vector<uint8_t> &array, const uint8_t &data)
{
    array.push_back(data);
}

void ProtocolStd::append(std::vector<uint8_t> &array, const uint16_t &data)
{
    array.push_back(static_cast<uint8_t>((data >> 8) & 0xFF));
    array.push_back(static_cast<uint8_t>(data & 0xFF));
}

void ProtocolStd::append(std::vector<uint8_t> &array, const uint32_t &data)
{
    array.push_back(static_cast<uint8_t>((data >> 24) & 0xFF));
    array.push_back(static_cast<uint8_t>((data >> 16) & 0xFF));
    array.push_back(static_cast<uint8_t>((data >> 8) & 0xFF));
    array.push_back(static_cast<uint8_t>(data & 0xFF));
}

void ProtocolStd::append(std::vector<uint8_t> &array, const uint64_t &data)
{
    array.push_back(static_cast<uint8_t>((data >> 56) & 0xFF));
    array.push_back(static_cast<uint8_t>((data >> 48) & 0xFF));
    array.push_back(static_cast<uint8_t>((data >> 40) & 0xFF));
    array.push_back(static_cast<uint8_t>((data >> 32) & 0xFF));

    array.push_back(static_cast<uint8_t>((data >> 24) & 0xFF));
    array.push_back(static_cast<uint8_t>((data >> 16) & 0xFF));
    array.push_back(static_cast<uint8_t>((data >> 8) & 0xFF));
    array.push_back(static_cast<uint8_t>(data & 0xFF));
}

void ProtocolStd::append(std::vector<uint8_t> &array, const float &data)
{
    static_assert(sizeof(float) == sizeof(uint32_t), "Size of float must be equal to size of uint32_t");
    uint32_t data_uint = *reinterpret_cast<const uint32_t *>(&data);
    append(array, data_uint);
}

void ProtocolStd::append(std::vector<uint8_t> &array, const std::string &data)
{
    append(array, static_cast<uint16_t>(data.size()));
    array.insert(array.end(), data.begin(), data.end());
}

#ifdef QT_CORE_LIB
void ProtocolStd::append(QByteArray &array, const uint8_t &data)
{
    array.append(data);
}

void ProtocolStd::append(QByteArray &array, const uint16_t &data)
{
    array.append(static_cast<uint8_t>((data >> 8) & 0xFF));
    array.append(static_cast<uint8_t>(data & 0xFF));
}

void ProtocolStd::append(QByteArray &array, const uint32_t &data)
{
    array.append(static_cast<uint8_t>((data >> 24) & 0xFF));
    array.append(static_cast<uint8_t>((data >> 16) & 0xFF));
    array.append(static_cast<uint8_t>((data >> 8) & 0xFF));
    array.append(static_cast<uint8_t>(data & 0xFF));
}

void ProtocolStd::append(QByteArray &array, const float &data)
{
    static_assert(sizeof(float) == sizeof(uint32_t), "Size of float must be equal to size of uint32_t");
    uint32_t data_uint = *reinterpret_cast<const uint32_t *>(&data);
    append(array, data_uint);
}

void ProtocolStd::append(QByteArray &array, const QString &text)
{
    QByteArray rawBinaryText = text.toUtf8();
    append(array, static_cast<uint16_t>(rawBinaryText.size()));
    array.append(rawBinaryText);
}

uint8_t ProtocolStd::getUint8_t(QByteArray &array)
{
    if (array.size() < 1)
    {
        return 0;
    }

    uint8_t value = static_cast<uint8_t>(array.at(0));
    array.remove(0, 1);
    return value;
}

uint16_t ProtocolStd::getUint16_t(QByteArray &array)
{
    if (array.size() < 2)
    {
        return 0;
    }

    uint8_t msb = static_cast<uint8_t>(array.at(0));
    array.remove(0, 1);
    uint8_t lsb = static_cast<uint8_t>(array.at(0));
    array.remove(0, 1);

    uint16_t value = (static_cast<uint16_t>(msb) << 8) | lsb;
    return value;
}

uint32_t ProtocolStd::getUint32_t(QByteArray &array)
{
    if (array.size() < sizeof(uint32_t)) {
        return 0;
    }

    uint8_t msb1 = static_cast<uint8_t>(array.at(0));
    uint8_t msb2 = static_cast<uint8_t>(array.at(1));
    uint8_t msb3 = static_cast<uint8_t>(array.at(2));
    uint8_t lsb = static_cast<uint8_t>(array.at(3));

    array.remove(0, 4); // Usuń 4 bajty z początku QByteArray

    uint32_t value = (static_cast<uint32_t>(msb1) << 24) | (static_cast<uint32_t>(msb2) << 16) | (static_cast<uint32_t>(msb3) << 8) | lsb;
    return value;
}

uint64_t ProtocolStd::getUint64_t(QByteArray &array)
{
    if (array.size() < sizeof(uint64_t)) {
        return 0;
    }

    uint8_t msb1 = static_cast<uint8_t>(array.at(0));
    uint8_t msb2 = static_cast<uint8_t>(array.at(1));
    uint8_t msb3 = static_cast<uint8_t>(array.at(2));
    uint8_t msb4 = static_cast<uint8_t>(array.at(3));

    uint8_t msb5 = static_cast<uint8_t>(array.at(4));
    uint8_t msb6 = static_cast<uint8_t>(array.at(5));
    uint8_t msb7 = static_cast<uint8_t>(array.at(6));
    uint8_t lsb = static_cast<uint8_t>(array.at(7));

    array.remove(0, 8); // Usuń 4 bajty z początku QByteArray

    uint64_t value = (static_cast<uint64_t>(msb1) << 56) | (static_cast<uint64_t>(msb2) << 48) | (static_cast<uint64_t>(msb3) << 40) | (static_cast<uint64_t>(msb4) << 32)
                     | (static_cast<uint64_t>(msb5) << 24) | (static_cast<uint64_t>(msb6) << 16) | (static_cast<uint64_t>(msb7) << 8) | lsb;
    return value;
}

float ProtocolStd::getFloat(QByteArray &array)
{
    static_assert(sizeof(float) == sizeof(uint32_t), "Size of float must be equal to size of uint32_t");
    uint32_t value32 = getUint32_t(array);
    float valueFloat = *reinterpret_cast<const float*>(&value32);
    return valueFloat;
}

QString ProtocolStd::getQString(QByteArray &array)
{
    uint16_t stringSize = getUint16_t(array);
    if (array.size() < stringSize)
    {
        return QString();
    }

    QString result = QString::fromUtf8(array.constData(), stringSize);
    //qDebug() << "result " << result;
    array.remove(0, stringSize);

    return result;
}
#endif

bool ProtocolStd::getBool(std::deque<uint8_t> &frame)
{
    if (frame.empty())
    {
        return false;
    }
    uint8_t byte = frame.front();
    frame.pop_front();
    return static_cast<bool>(byte);
}

uint8_t ProtocolStd::getUint8_t(std::vector<uint8_t> &frame)
{
    if (frame.empty())
    {
        return 0;
    }
    uint8_t byte = frame.at(0); // lub frame[0];
    frame.erase(frame.begin());
    return byte;
}

uint8_t ProtocolStd::getUint8_t(std::deque<uint8_t> &frame)
{
    if (frame.empty())
    {
        return 0;
    }
    uint8_t byte = frame.front();
    frame.pop_front();
    return byte;
}

uint16_t ProtocolStd::getUint16_t(std::deque<uint8_t> &frame)
{
    if (frame.size() < 2)
    {
        return 0;
    }
    uint8_t msb = frame.front();
    frame.pop_front();
    uint8_t lsb = frame.front();
    frame.pop_front();
    uint16_t value = (static_cast<uint16_t>(msb) << 8) | lsb;
    return value;
}

uint32_t ProtocolStd::getUint32_t(std::deque<uint8_t> &frame)
{
    if (frame.size() < sizeof(uint32_t))
    {
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

uint64_t ProtocolStd::getUint64_t(std::deque<uint8_t> &frame)
{
    if (frame.size() < sizeof(uint64_t))
    {
        return 0;
    }
    uint8_t msb1 = frame.front();
    frame.pop_front();
    uint8_t msb2 = frame.front();
    frame.pop_front();
    uint8_t msb3 = frame.front();
    frame.pop_front();
    uint8_t msb4 = frame.front();
    frame.pop_front();

    uint8_t msb5 = frame.front();
    frame.pop_front();
    uint8_t msb6 = frame.front();
    frame.pop_front();
    uint8_t msb7 = frame.front();
    frame.pop_front();
    uint8_t lsb = frame.front();
    frame.pop_front();

    uint64_t value = (static_cast<uint64_t>(msb1) << 56) | (static_cast<uint64_t>(msb2) << 48) | (static_cast<uint64_t>(msb3) << 40) | (static_cast<uint64_t>(msb4) << 32)
                     | (static_cast<uint64_t>(msb5) << 24) | (static_cast<uint64_t>(msb6) << 16) | (static_cast<uint64_t>(msb7) << 8) | lsb;
    return value;
}

float ProtocolStd::getFloat(std::deque<uint8_t> &frame)
{
    static_assert(sizeof(float) == sizeof(uint32_t), "Size of float must be equal to size of uint32_t");
    uint32_t value32 = getUint32_t(frame);
    float valueFloat = *reinterpret_cast<const float *>(&value32);
    return valueFloat;
}

std::string ProtocolStd::getString(std::deque<uint8_t> &frame)
{
    uint16_t stringSize = ProtocolStd::getUint16_t(frame);
    if (frame.size() < stringSize)
    {
        return "";
    }
    std::string result(frame.begin(), frame.begin() + stringSize);
    frame.erase(frame.begin(), frame.begin() + stringSize);
    return result;
}

void ProtocolStd::reset()
{
    buffer.clear();
    currentState = WaitForDataSizeMSB;
    expectedSize = 0;
}
