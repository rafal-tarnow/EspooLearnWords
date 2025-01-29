#include "protocol.h"

void Protocol::addData(const QByteArray &data)
{
    buffer.append(data);

    while (!buffer.isEmpty()) {
        switch (currentState) {
        case WaitForDataSizeMSB:
            if (buffer.size() >= 1) {
                expectedSize = static_cast<quint8>(buffer[0]) << 8;
                buffer.remove(0, 1);
                currentState = WaitForDataSizeLSB;
            } else {
                return;
            }
            break;

        case WaitForDataSizeLSB:
            if (buffer.size() >= 1) {
                expectedSize += static_cast<quint8>(buffer[0]);
                buffer.remove(0, 1);
                currentState = CompleteData;
            } else {
                return;
            }
            break;

        case CompleteData:
            if (buffer.size() >= expectedSize) {
                QByteArray frame = buffer.left(expectedSize);
                buffer.remove(0, expectedSize);
                expectedSize = 0;
                currentState = WaitForDataSizeMSB;

                emit onFrame(frame);
            } else {
                return;
            }
            break;
        }
    }
}
