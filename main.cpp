#include <QCoreApplication>
#include <QByteArray>
#include <QFile>
#include <QtEndian>
#include <QDebug>

class ParserOfMessage
{
public:
    //ParserOfMessage();
    void readfile();
    void Parser();
    void parser_102();
    void parser_149();
    void parser_152();
    void parser_154();
    void parser_156();

    enum ID {
           id_102 = 0x66,
           id_149 = 0x95,
           id_152 = 0x98,
           id_154 = 0x9A,
           id_156 = 0x9C
        };

//        enum class Length{
//            datalenght_102 = 0x05,
//            datalength_149 = 0x23,
//            datalength_152 = 0x37,
//            datalength_154 = 0x0F,
//            datalength_156 = 0xA6
//        };

private:
//    quint16 bin_1 = 0000000000000000; // 0
//    quint16 bin_1_2 = 0000000000000100; // 4
//    quint16 bin_2 = 0000000000000001; // 1
//    quint16 bin_2_2 = 0000000000000101; // 5
//    quint16 bin_3 = 0000000000000010; // 2
//    quint16 bin_3_2 = 0000000000000110; // 6
//    quint16 bin_4 = 0000000000000011; // 3
//    quint16 bin_4_2 = 0000000000000111; // 7
    quint16 i;
    quint16 n;
    int f;
    //QByteArray message;
    static constexpr quint8 header_1 {0x57};
    static constexpr quint8 header_2 {0xf1};
    ID msg = id_102;

};

void ParserOfMessage::readfile(){
    QFile file("/home/pavelkopytin/Рабочий стол/build-Kopytin-Desktop-Release/2023-04-07-14-11-37");
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "я ничего не открыл";
        return;
    }
    else{
        qDebug() << "Файл открыт!";
        QByteArray message = file.readAll();
        qDebug() << "Массив создан";
//            qDebug() << message.toHex();
        n = message.count();// определяем количество элементов массива (т.е. байтов)
        for (i = 0; i<=n-1; i++){ //начинаем перебор элементов
            if(static_cast<quint8>(message[i]) == header_1 and static_cast<quint8>(message[i+1]) == header_2){ // ищем идентификатор заголовка
                qDebug() << "found";
                f = i;
                if (static_cast<quint8>(message[i+3]) != msg){
                    qDebug() << "не подходит";
                    //msg = ID::id_149;
                }
                else{
                    QByteArray message_102; // создали массив сообщения с ИД=102
                    int a = 0;
                    for (i=f; i<=f+15; i++) {
                       message_102[a] = message[i]; // заполнили массив сообщения 102
                       qDebug() << message_102[a];
                       a++;
                      }
                    qDebug() << message_102.toHex(); // для проверки
                    quint8 LengthMessageInfo = message_102[2]; // определили длину информационной части сообщения
                    qDebug() << "Длина информационной части:" << LengthMessageInfo; //вывели
                    quint16 CheckSum = message_102[4];
                    CheckSum <<= 8;
                    CheckSum += message_102[5]; // контрольная сумма
                    CheckSum = qToBigEndian(CheckSum); // должен быть типа uint16
                    qDebug() << "Контрольная сумма:" << CheckSum;
                    quint16 StatusMPI = message_102[7];
                    StatusMPI <<= 8;
                    StatusMPI += message_102[6]; // статус МПИ
                    //попробовать вытащить биты из int
                        if(StatusMPI == 0)
                            qDebug()<< "МПИ исправен, в процессе контроля сбои не зафиксированы";
                        else if (StatusMPI == 4)
                            qDebug()<< "МПИ исправен, в процессе контроля зафиксированы сбои ";
                        else if (StatusMPI == 1)
                            qDebug()<< "МПИ неисправен, в процессе контроля сбои не зафиксированы";
                        else if (StatusMPI == 5)
                            qDebug()<< "МПИ неисправен, в процессе контроля зафиксированы сбои ";
                        else if (StatusMPI == 3)
                            qDebug()<< "Отказ МПИ, в процессе контроля сбои не зафиксированы";
                        else if (StatusMPI == 7)
                            qDebug()<< "Отказ МПИ, в процессе контроля зафиксированы сбои ";
                        else
                            qDebug() << "биты не распознаны";
                    quint16 NumberTest = message_102[9];
                    NumberTest <<= 8;
                    NumberTest += message_102[8];
                    qDebug() << "Номер теста: " << NumberTest;
                   // как предусмотреть описание в зависимости от номера теста?..
                    quint32 DescriptionOfViolationFirst = message_102[13];
                    DescriptionOfViolationFirst <<= 8;
                    DescriptionOfViolationFirst += message_102[12];
                    DescriptionOfViolationFirst <<= 8;
                    DescriptionOfViolationFirst += message_102[11];
                    DescriptionOfViolationFirst <<= 8;
                    DescriptionOfViolationFirst += message_102[10];//описание нарушения, первое слово
                    //DescriptionOfViolationFirst = qFromLittleEndian(DescriptionOfViolationFirst); // должен быть типа bin32
                    if (DescriptionOfViolationFirst == 0)
                        qDebug() << "Описание нарушения: Нарушения не зафиксированы, " << DescriptionOfViolationFirst;
                    else if (DescriptionOfViolationFirst == 1)
                        qDebug() << "Описание нарушения, первое слово: Нарушение контроля четности зафиксировано";
                    else if (DescriptionOfViolationFirst == 2)
                        qDebug() << "Описание нарушения, первое слово: Нарушение формата передачи байта зафиксировано";
                    else if (DescriptionOfViolationFirst == 3)
                        qDebug() << "Описание нарушения, первое слово: Нарушения контроля четности и формата передачи байта зафиксированы";
                    quint16 DescriptionOfViolationTwo = message_102[15];
                    DescriptionOfViolationTwo <<= 8;
                    DescriptionOfViolationTwo += message_102[14];// описание нарушения, второе слово
                    //DescriptionOfViolationTwo = qFromLittleEndian(DescriptionOfViolationTwo); // должен быть типа bin16
                    qDebug() << "Описание нарушения, второе слово: Количество переполненных буферов ошибок приема: " << DescriptionOfViolationTwo;
                };// эта функция определит идентификатор сообщения и длину его информационной части
            //TOD0: передать массив или ссылку на него в функцию Parser()
            }
            else {// если идентификатор не определен, двигаемся дальше по массиву
                //qDebug() << "not found";
            }
    }
    }
}

//void ParserOfMessage::Parser()
//{
//    switch(msg){
//    case ID::id_102: // сообщение с нарушением целостности, содержит 16 байт (с. 80)
//        if (static_cast<quint8>(message[i+3]) != msg){
//            qDebug() << "не подходит";
//            msg = ID::id_149;
//        }
//        else
//           ParserOfMessage::parser_102();
//        break;
//        case ID::id_149: // сообщение с решением КВ, содержит 76 байт (с.109)
//            if (static_cast<unsigned char>(message[i+3]) != msg){
//                qDebug() << "не подходит";
//                msg = ID::id_152;
//            }
//            else
//                ParserOfMessage::parser_149();
//            break;
//        case ID::id_152: // сообщение с оперативными данными о решении КВ, содержит 60 байт (с.115)
//            if (message[i+3] != static_cast<unsigned char>(ID::id_149)){
//                qDebug() << "не подходит";
//                msg = ID::id_154;
//            }
//            else
//                ParserOfMessage::parser_152();
//            break;
//        case ID::id_154: // сообщение с параметрами выходной ШВ, содержит 36 байт (с.118)
//            if (message[i+3] != static_cast<unsigned char>(ID::id_149)){
//                qDebug() << "не подходит";
//                msg = ID::id_156;
//            }
//            else
//                ParserOfMessage::parser_154();
//            break;
//        case ID::id_156: // сообщение с данными о НКА, содержит 36 байт (с.121)
//            if (message[i+3] != static_cast<unsigned char>(ID::id_149)){
//                qDebug() << "не подходит";
//            }
//            else
//                ParserOfMessage::parser_156();
//            break;
//    default:
//    qDebug()<< "Сообщение не распознано";
//}
//}

//void ParserOfMessage::parser_102() {
//    //так как сообщение содержит 16 байт, нам понадобится 16 элементов массива message
//    QByteArray message_102; // создали массив сообщения с ИД=102
//    int a = 0;
//    for (i=f; i<=f+15; i++) {
//       message_102[a] = message[i]; // заполнили массив сообщения 102
//       a++;
//      }
//    qDebug() << message_102; // для проверки
//    quint8 LengthMessageInfo = message_102[2]; // определили длину информационной части сообщения
//    qDebug() << "Длина информационной части:";
//    qDebug() << LengthMessageInfo; //вывели
//    quint16 CheckSum = message_102[4];
//    CheckSum <<= 8;
//    CheckSum |= message_102[5]; // контрольная сумма
//    CheckSum = qFromLittleEndian(CheckSum); // должен быть типа uint16
//    quint16 StatusMPI = message_102[7];
//    StatusMPI <<= 8;
//    StatusMPI += message_102[6]; // статус МПИ
//    //StatusMPI = qFromLittleEndian(StatusMPI); // должен быть типа bin16
//    //попробовать вытащить биты из int
//        if (StatusMPI == bin_1)
//            qDebug()<< "МПИ исправен, в процессе контроля сбои не зафиксированы";
//        else if (StatusMPI == bin_1_2)
//            qDebug()<< "МПИ исправен, в процессе контроля сбои зафиксированы";
//        else if (StatusMPI == bin_2)
//            qDebug()<< "МПИ неисправен, в процессе контроля сбои не зафиксированы";
//        else if (StatusMPI == bin_2_2)
//            qDebug()<< "МПИ неисправен, в процессе контроля сбои зафиксированы";
//        else if (StatusMPI == bin_4)
//            qDebug()<< "Отказ МПИ, в процессе контроля сбои не зафиксированы";
//        else if (StatusMPI == bin_4_2)
//            qDebug()<< "Отказ МПИ, в процессе контроля сбои зафиксированы";
//    quint16 NumberTest = message_102[8];
//    NumberTest <<= 8;
//    NumberTest += message_102[9];
//    NumberTest = qFromLittleEndian(NumberTest);
//    qDebug() << "Номер теста: " << NumberTest;
//   // как предусмотреть описание в зависимости от номера теста?..
//    quint32 DescriptionOfViolationFirst = message_102[10];
//    DescriptionOfViolationFirst <<= 8;
//    DescriptionOfViolationFirst += message_102[11];
//    DescriptionOfViolationFirst <<= 8;
//    DescriptionOfViolationFirst += message_102[12];
//    DescriptionOfViolationFirst <<= 8;
//    DescriptionOfViolationFirst += message_102[13];//описание нарушения, первое слово
//    DescriptionOfViolationFirst = qToBigEndian(DescriptionOfViolationFirst); // должен быть типа bin32
//    qDebug() << DescriptionOfViolationFirst;
//    quint16 DescriptionOfViolationTwo = message_102[14];
//    DescriptionOfViolationTwo <<= 8;
//    DescriptionOfViolationTwo += message_102[15];// описание нарушения, второе слово
//    DescriptionOfViolationTwo = qToBigEndian(DescriptionOfViolationTwo); // должен быть типа bin16
//    qDebug() << DescriptionOfViolationTwo;
//}





int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ParserOfMessage psm;
    QByteArray message;
    psm.readfile();
    return a.exec();
}
