#ifndef BINARYTPARSER_H
#define BINARYTPARSER_H

#include <QVector>

// Парсер протокола BinaryT.
class BinaryTParser
{
public:
    enum class State
    {
        WaitingForHeader_1,
        WaitingForHeader_2,
        WaitingForDataLength,
        WaitingForMessageId,
        WaitingForChecksumLo,
        WaitingForChecksumHi,
        GettingData
    };

    // Для статистики.
    enum Counters
    {
        Counter_badHeader_1,
        Counter_badHeader_2,
        Counter_badChecksum,

        Counters_count
    };

    // Заголовок (с. 15).
    static constexpr quint8 header_1 {0x57};
    static constexpr quint8 header_2 {0xf1};
    static constexpr quint16 header {((static_cast<quint16>(header_2) << 8) | header_1)};

private:
    State m_state {State::WaitingForHeader_1};
    quint8 m_dataLength {0};
    quint8 m_messageId {0};
    quint16 m_neededChecksum {0};

    // Максимальная длина информационной части - 0xff, выраженная в 16-разрядных словах.
    quint8 m_payload[0xff * 2] {};
    int m_gotPayloadBytes {0}; // сколько данных сейчас принято в m_payload

    // Этот размер устанавливается после полного приёма пакета и не сбрасывается в методе Reset.
    // Он показывает, сколько байт нужно выдать наружу в методе GetPayload.
    int m_payloadValidBytes {-1}; // <0 = в m_payload нет валидных данных

    // Статистика работы.
    qint64 m_counters[Counters_count] {};

public:
    // true - собрался полный пакет.
    bool Feed(quint8 byte);

    // Этот метод можно вызывать только сразу после того, как Feed вернёт true.
    QVector<quint8> GetPayload(quint8 *messageId) const;

private:
    // Перейти в режим ожидания следующего пакета.
    void Reset();
};

#endif // BINARYTPARSER_H
