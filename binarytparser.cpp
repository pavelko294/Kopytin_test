#include "binarytparser.h"
#include "crutches.h"

#include <QtEndian> // формат чисел - LE (с. 12)
#include <QDebug>

bool BinaryTParser::Feed(quint8 byte)
{
    switch (m_state)
    {
    case State::WaitingForHeader_1:
        if(byte != header_1)
        {
            m_counters[Counter_badHeader_1] ++;
            qDebug() << "badHeader_1: " << byte;
            Reset();
        }
        else
            m_state = State::WaitingForHeader_2;
        return false;

    case State::WaitingForHeader_2:
        if(byte != header_2)
        {
            m_counters[Counter_badHeader_2] ++;
             qDebug() << "badHeader_2: " << byte;
            Reset();
        }
        else
            m_state = State::WaitingForDataLength;
        return false;

    case State::WaitingForDataLength:
        m_dataLength = byte;
        m_state = State::WaitingForMessageId;
        return false;

    case State::WaitingForMessageId:
        m_messageId = byte;
        m_state = State::WaitingForChecksumLo;
        return false;

    case State::WaitingForChecksumLo:
        m_neededChecksum = byte;
        m_state = State::WaitingForChecksumHi;
        return false;

    case State::WaitingForChecksumHi:
    {
        quint16 t {byte};
        t <<= 8;
        m_neededChecksum |= t;

        ::memset(m_payload, 0x00, sizeof(m_payload));
        m_payloadValidBytes = -1; // если в буфере был валидный пакет, сейчас он уже не валиден

        // Протокол допускает пустые пакеты.
        if(m_dataLength == 0)
        {
            m_payloadValidBytes = 0;
            Reset();
            return true;
        }

        // Если данные есть - приступаем к их приёму.
        m_state = State::GettingData;
        return false;
    }

    case State::GettingData:
        Q_ASSERT(m_gotPayloadBytes >= 0 && m_gotPayloadBytes < static_cast<qint64>(sizeof(m_payload)));
        m_payload[m_gotPayloadBytes] = byte;
        m_gotPayloadBytes ++;

        int neededPayloadBytes {m_dataLength};
        neededPayloadBytes *= 2; // длина измеряется в в 16-разрядных словах

        if(m_gotPayloadBytes == neededPayloadBytes) // приняли пакет до конца
        {
            // Вычисляем контрольную сумму.
            quint64 checksum {0};

            checksum += header;

            {
                quint16 t {m_messageId};
                t <<= 8;
                t |= m_dataLength;
                checksum += t;
            }

            for(int idx = 0; idx < m_dataLength; idx ++)
                checksum += qFromLittleEndian<quint16>(&(m_payload[idx * 2]));

            // "Биты переноса с битовой позиции 15, генерируемые в процессе суммирования, складываются
            // с результатом суммирования" (с. 15) <--- это как?? А если после этого сложения они опять вылезут за второй байт?..
            {
                quint64 t {checksum};
                t &= 0xffffffffffff0000u;
                t >>= 16;
                checksum += t;
                checksum &= 0xffffu;
            }

            if(checksum != m_neededChecksum)
            {
                m_counters[Counter_badChecksum] ++;
                 qDebug() << "badChecksum: " << checksum << "!=" << m_neededChecksum;
                Reset();
                return false;
            }

            m_payloadValidBytes = m_gotPayloadBytes;
            Reset();
            return true;
        }

        Q_ASSERT(m_gotPayloadBytes < neededPayloadBytes);
        return false;
    }
    qFatal("Invalid state");
    return false;
}

QVector<quint8> BinaryTParser::GetPayload(quint8 *messageId) const
{
    Q_ASSERT(m_payloadValidBytes >= 0 && m_payloadValidBytes < static_cast<qint64>(sizeof(m_payload)));

    QVector<quint8> ret (m_payloadValidBytes);
    ::memcpy(ret.data(), m_payload, ret.size());

    if(messageId)
        (*messageId) = m_messageId;
    return ret;
}

void BinaryTParser::Reset()
{
    m_state = State::WaitingForHeader_1;
    //m_messageId = 0;
    m_dataLength = 0;
    m_neededChecksum = 0;
    // NB: не зануляем m_payload
    m_gotPayloadBytes = 0;
    // NB: не сбрасываем m_payloadValidBytes
}
