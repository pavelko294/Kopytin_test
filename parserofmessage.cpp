#include "parserofmessage.h"

ParserOfMessage::ParserOfMessage()
{

}

    void ParserOfMessage::Parser(const QVector<quint8> &payload, quint8 MessageID)
    {
        switch(MessageID)
        {
        case 102:
            parser_102(payload);
            break;

        case 149:
            parser_149(payload);
            break;

        case 154:
            parser_154(payload);
            break;

        case 156:
            parser_156(payload);
            break;

        case 157:
            parser_157(payload);
            break;
        case 159:
            parser_159(payload);
            break;
        default:
            qDebug() << "god knows... god knows i want to break free...";
            return;
        }

    }


    void ParserOfMessage::parser_102(const QVector<quint8>&payload) {
        results += "\nСообщение ID=102\n";
        quint16 StatusMPI = payload[1];
        StatusMPI <<= 8;
        StatusMPI |= payload[0]; // статус МПИ
        results += "Статус МПИ:" + QString::number(StatusMPI, 2) + "\n";
        QString slovo;
            if(StatusMPI == 0){
                qDebug()<< "МПИ исправен, в процессе контроля сбои не зафиксированы";
                slovo = static_cast<QString>("МПИ исправен, в процессе контроля сбои не зафиксированы") + "\n";
            }
            else if (StatusMPI == 4){
                qDebug()<< "МПИ исправен, в процессе контроля зафиксированы сбои ";
                slovo = static_cast<QString>("МПИ исправен, в процессе контроля зафиксированы сбои ") + "\n";
            }
            else if (StatusMPI == 1){
                qDebug()<< "МПИ неисправен, в процессе контроля сбои не зафиксированы";
                slovo = static_cast<QString>("МПИ неисправен, в процессе контроля сбои не зафиксированы ") + "\n";
            }
            else if (StatusMPI == 5){
                qDebug()<< "МПИ неисправен, в процессе контроля зафиксированы сбои ";
                slovo = static_cast<QString>("МПИ неисправен, в процессе контроля зафиксированы сбои ") + "\n";
            }
            else if (StatusMPI == 3){
                qDebug()<< "Отказ МПИ, в процессе контроля сбои не зафиксированы";
                slovo = static_cast<QString>("Отказ МПИ, в процессе контроля сбои не зафиксированы") + "\n";
            }
            else if (StatusMPI == 7){
                qDebug()<< "Отказ МПИ, в процессе контроля зафиксированы сбои ";
                slovo = static_cast<QString>("Отказ МПИ, в процессе контроля зафиксированы сбои ") + "\n";
            }
            else{
                qDebug() << "биты не распознаны";
                slovo = static_cast<QString>("биты не распознаны") + "\n";
            }
            results += slovo;
        quint16 NumberTest = payload[3];
        NumberTest <<= 8;
        NumberTest += payload[2];
        qDebug() << "Номер теста: " << NumberTest;
        results += "Номер теста: " + QString::number(NumberTest) + "\n";
        quint32 DescriptionOfViolationFirst = payload[7];
        DescriptionOfViolationFirst <<= 8;
        DescriptionOfViolationFirst += payload[6];
        DescriptionOfViolationFirst <<= 8;
        DescriptionOfViolationFirst += payload[5];
        DescriptionOfViolationFirst <<= 8;
        DescriptionOfViolationFirst += payload[4];//описание нарушения, первое слово
        results += "Описание нарушения, первое слово:" + QString::number(DescriptionOfViolationFirst) + "\n";
        qDebug() << "Описание нарушения, первое слово:" << QString::number(DescriptionOfViolationFirst, 2);
        quint16 DescriptionOfViolationTwo = payload[9];
        DescriptionOfViolationTwo <<= 8;
        DescriptionOfViolationTwo += payload[8];// описание нарушения, второе слово
        qDebug() << "Описание нарушения, второе слово: " << QString::number(DescriptionOfViolationTwo, 2);
        results += "Описание нарушения, второе слово:" + QString::number(DescriptionOfViolationTwo, 2) + "\n";
        emit writer(results);
    }




    void ParserOfMessage::parser_149(const QVector<quint8>&payload){
        results += "\nСообщение ID=149\n";
        quint16 SignWord = payload[1];
          SignWord <<= 8;
          SignWord += payload[0];
        qDebug() << "Слово признаков: " << QString::number(SignWord, 2);
        quint8 seconds = payload[4];
        quint8 minutes = payload[3];
        quint8 hours = payload[2];
        qDebug() << "Время: "<< hours <<":"<<minutes<<":"<<seconds;
        quint32 fragment_time = payload[9];
        fragment_time <<= 8;
        fragment_time += payload[8];
        fragment_time <<= 8;
        fragment_time += payload[7];
        fragment_time <<= 8;
        fragment_time += payload[6];
        qDebug() << "Дробная часть времени" << fragment_time;
        quint8 year = payload[10];
        quint8 month = payload[11];
        quint8 day = payload[12];
        qDebug() << "Дата:" << day <<"."<<month<<"."<<year;
        qint64 X = 0;
        qint64 Y = 0;
        qint64 Z = 0;
        int h = 0;
        for(h=0; h<=7; h++){
            X += payload[14+h];
            Y += payload[22+h];
            Z += payload[30+h];
            X <<= 8;
            Y <<= 8;
            Z <<= 8;
        }
        double X_finish = static_cast<double>(X);
        double Y_finish = static_cast<double>(Y);
        double Z_finish = static_cast<double>(Z);
        qDebug() << "Latitide" << X_finish;
        qDebug() << "Longitude" <<Y_finish;
        qDebug() << "Height" << Z_finish;
        qint32 CKP_speed_X = 0;
        qint32 CKP_speed_Y = 0;
        qint32 CKP_speed_Z = 0;
        qint32 CKP_coordinates = 0;
        qint32 CKP_height = 0;
        qint32 CKP_time = 0;
        qint32 CKP_plane_speed = 0;
        qint32 CKP_vertical_speed = 0;
        for(int h = 0; h <= 3; h++)
        {
            CKP_speed_X += payload[41-h];
            CKP_speed_Y += payload[45-h];
            CKP_speed_Z += payload[49-h];
            CKP_coordinates += payload[53-h];
            CKP_height += payload[57-h];
            CKP_time += payload[61-h];
            CKP_plane_speed += payload[65-h];
            CKP_vertical_speed += payload[69-h];
            CKP_speed_X <<= 8;
            CKP_speed_Y <<= 8;
            CKP_speed_Z <<= 8;
            CKP_coordinates <<= 8;
            CKP_height <<= 8;
            CKP_time <<= 8;
            CKP_plane_speed <<= 8;
            CKP_vertical_speed <<= 8;
        }
        qDebug() << "Составляющая скорости по широте: " << static_cast<float>(CKP_speed_X);
        qDebug() << "Составляющая скорости по долготе: " << static_cast<float>(CKP_speed_Y);
        qDebug() << "Составляющая скорости по высоте: " << static_cast<float>(CKP_speed_Z);
        qDebug() << "СКП определения плановых координат: " << static_cast<float>(CKP_coordinates);
        qDebug() << "СКП определения высоты: " << static_cast<float>(CKP_height);
        qDebug() << "СКП определения времени: " << static_cast<float>(CKP_time);
        qDebug() << "СКП определения плановой составляющей скорости: " << static_cast<float>(CKP_plane_speed);
        qDebug() << "СКП определения вертикальной составляющей скорости: " << static_cast<float>(CKP_vertical_speed);
        results += "Слово признаков: " +QString::number(SignWord, 2) + "\n" +
                "Время: " + QString::number(hours) + ":" + QString::number(minutes) + ":" + QString::number(seconds) + "\n" +
                "Дробная часть времени: " + QString::number(fragment_time) + "\n" +
                "Latitide: " + QString::number(X_finish) + "\n" +
                "Longitude: " + QString::number(Y_finish) + "\n" +
                "Height: " + QString::number(Z_finish) + "\n" +
                "Составляющая скорости по широте: " + QString::number(static_cast<float>(CKP_speed_X)) + "\n" +
                "Составляющая скорости по долготе: " + QString::number(static_cast<float>(CKP_speed_Y)) + "\n" +
                "Составляющая скорости по высоте: " + QString::number(static_cast<float>(CKP_speed_Z))+ "\n" +
                "СКП определения плановых координат: " + QString::number(static_cast<float>(CKP_coordinates)) + "\n" +
                "СКП определения высоты: " + QString::number(static_cast<float>(CKP_height)) + "\n" +
                "СКП определения времени: " + QString::number(static_cast<float>(CKP_time)) + "\n" +
                "СКП определения плановой составляющей скорости: " + QString::number(static_cast<float>(CKP_plane_speed)) + "\n" +
                "СКП определения вертикальной составляющей скорости: " + QString::number(static_cast<float>(CKP_vertical_speed)) + "\n";
       }

    void ParserOfMessage::parser_154(const QVector<quint8>&payload){
        //так как полезная нагрузка составляет 30 байт, нам понадобится обработать 30 элементов массива payload
        results += "\nСообщение ID=154\n";
        quint16 SignWord = payload[1];
          SignWord <<= 8;
          SignWord += payload[0];
        qDebug() << QString::number(SignWord, 2);
        quint8 seconds = payload[4];
        quint8 minutes = payload[3];
        quint8 hours = payload[2];
        qDebug() << "Время: "<< hours <<":"<<minutes<<":"<<seconds;
        quint32 fragment_time = payload[9];
        fragment_time <<= 8;
        fragment_time += payload[8];
        fragment_time <<= 8;
        fragment_time += payload[7];
        fragment_time <<= 8;
        fragment_time += payload[6];
        qDebug() << "Дробная часть времени:" << fragment_time;
        quint64 frequency_fix = 0;
        int h = 0;
        for(h=0; h<=7; h++){
            frequency_fix <<= 8;
            frequency_fix += payload[17-h];
        }
        double frequency_fix_finish = static_cast<double>(frequency_fix);
        qDebug() << "Нормированная поправка по частоте:" << frequency_fix_finish;
        quint32 error_time = 0;
        quint32 error_frequency = 0;
        for(int h = 0; h <= 3; h++)
        {
            error_time = payload[18+h];
            error_frequency = payload[22+h];
            error_time <<= 8;
            error_frequency <<= 8;
        }
        float error_time_finish = static_cast<float>(error_time);
        float error_frequency_finish = static_cast<float>(error_frequency);
        qDebug() << "СКП определения времени: " << error_time_finish;
        qDebug() << "СКП нормированной поправки по частоте: " << error_frequency_finish;
        results += "Слово признаков:" + QString::number(SignWord, 2) + "\n" +
                "Время: " + QString::number(hours) + ":" + QString::number(minutes) + ":" + QString::number(seconds) + "\n" +
                "Дробная часть времени: " + QString::number(fragment_time) + "\n" +
                "Нормированная поправка по частоте: " + QString::number(frequency_fix_finish) + "\n" +
                "СКП определения времени: " + QString::number(error_time_finish) + "\n" +
                "СКП нормированной поправки по частоте: " + QString::number(error_frequency_finish) + "\n";
        }


    void ParserOfMessage::parser_156(const QVector<quint8>&payload){
        //так как сообщение содержит неопределенное количество байт, которое зависит от количества видимых НКА, нам понадобится количество элементов массива payload, зависящее от количества видимых спутников НКА
        results += "\nСообщение ID=156\n";
        quint16 StateWord = payload[1];
          StateWord <<= 8;
          StateWord += payload[0];
          qDebug() << "Слово состояния: " << QString::number(StateWord, 2);
          results += "Слово состояния: " + QString::number(StateWord, 2) + "\n";
        quint16 quantityNKA = payload[3];
        quantityNKA <<= 8;
        quantityNKA += payload[2];
        qDebug() << "Количество спутников: " << quantityNKA;
        results += "Количество спутников: " + QString::number(quantityNKA) + "\n";
        qint32 angleSee = 0;
        for(int h = 0; h <= 3; h++)
        {
            angleSee += payload[7-h];
            angleSee <<= 8;
        }
        float angleSee_finish = static_cast<float>(angleSee);
        qDebug() << "Угол видимости: " << angleSee_finish;
        results += "Угол видимости: " + QString::number(angleSee_finish) + "\n";
        quint16 quantitySeeNKA = payload[9];
        quantitySeeNKA <<= 8;
        quantitySeeNKA += payload[8];
        qDebug() << "Количество видимых спутников: " << quantitySeeNKA;
        results += "Количество видимых спутников: " + QString::number(quantitySeeNKA) + "\n";
        for (int h=0; h <= quantitySeeNKA; h++) // информация об одном НКА содержит 14 байт
        {
            quint16 numberNKA = payload[11+14*h];
            numberNKA <<= 8;
            numberNKA += payload[10+14*h];
            qDebug() << "Номер НКА:" << numberNKA;
            results += "Количество видимых спутников: " + QString::number(numberNKA) + "\n";
            quint16 StateNKA = payload[13+14*h];
            StateNKA <<= 8;
            StateNKA += payload[12+14*h];
            qDebug() << "Состояние НКА с номером: " << numberNKA << ":" << QString::number(StateNKA, 2);
            results += "Состояние НКА с номером: " + QString::number(numberNKA) + ":" + QString::number(StateNKA, 2) + "\n";
            qint16 AgeAlmanah = payload[15+14*h];
            AgeAlmanah <<= 8;
            AgeAlmanah += payload[14+14*h];
            qDebug() << "Возраст альманаха НКА с номером: " << numberNKA << ":" << AgeAlmanah;
            results += "Возраст альманаха НКА с номером: " + QString::number(numberNKA) + ":" + QString::number(AgeAlmanah) + "\n";
            qint32 AngleOverHorizon = 0;
            qint32 Azimut = 0;
            for (int t = 0; t <= 3; t++)
            {
                AngleOverHorizon += payload[19-t+14*h];
                Azimut += payload[23-t+14*h];
                AngleOverHorizon <<= 8;
                Azimut <<= 8;
        }
           float AngleOverHorizon_finish = static_cast<float>(AngleOverHorizon);
           qDebug() << "Угол возвышения: " << AngleOverHorizon_finish;
           results += "Угол возвышения НКА с номером: " + QString::number(numberNKA) + ":" + QString::number(AngleOverHorizon_finish) + "\n";
           float Azimut_finish = static_cast<float>(Azimut);
           qDebug() << "Азимут(курс): " << Azimut_finish;
           results += "Азимут(курс) НКА с номером: " + QString::number(numberNKA) + ":" + QString::number(Azimut_finish) + "\n";
        }
    }

    void ParserOfMessage::parser_157(const QVector<quint8>&payload){
        results += "\nСообщение ID=157\n";
        quint16 NKAchannel = 0;
        quint16 StateOfChannel = 0;
        qint32 angleNKA = 0;
        qint32 azimut = 0;
        qint32 signal_noise = 0;
        for (int h = 0; h <= 15; h++)
        {
            int counter = 1;
            NKAchannel += payload[1+16*h];
            NKAchannel <<= 8;
            NKAchannel += payload[0+16*h];
            qDebug() << "Идентификатор НКА на канале i=" << counter << ": " << QString::number(NKAchannel, 2);
            results += "Идентификатор НКА на канале i=" + QString::number(counter) + ":" + QString::number(NKAchannel, 2) + "\n";
            StateOfChannel += payload[3+16*h];
            StateOfChannel <<= 8;
            StateOfChannel += payload[2+16*h];
            qDebug() << "Состояние канала i=" << counter << ": " << QString::number(StateOfChannel, 2);
            results += "Состояние канала i=" + QString::number(counter) + ":" + QString::number(StateOfChannel, 2) + "\n";
            for (int i = 0; i <= 3; i++)
            {
                angleNKA += payload[7-i+16*h];
                azimut += payload[11-i+16*h];
                signal_noise += payload[15-i+16*h];
                angleNKA <<= 8;
                azimut <<= 8;
                signal_noise <<= 8;
            }
            float angleNKA_finish = static_cast<float>(angleNKA);
            float azimut_finish = static_cast<float>(azimut);
            float signal_noise_finish = static_cast<float>(signal_noise);
            qDebug() << "Угол места НКА i=" << counter << ": " << angleNKA_finish;
            results += "Угол места НКА i=" + QString::number(counter) + ": " + QString::number(angleNKA_finish) + "\n";
            qDebug() << "Азимут(курс) НКА i=" << counter << ": " << azimut_finish;
            results += "Азимут(курс) НКА i=" + QString::number(counter) + ": " + QString::number(azimut_finish) + "\n";
            qDebug() << "Отношение сигнал/шум для НКА i=" << counter << ": " << signal_noise_finish;
            results += "Отношение сигнал/шум для НКА i=" + QString::number(counter) + ": " + QString::number(signal_noise_finish) + "\n";
        }
    }


    void ParserOfMessage::parser_159(const QVector<quint8>&payload){
        results += "\nСообщение ID=159\n";
        quint16 SignWord = payload[1];
          SignWord <<= 8;
          SignWord += payload[0];
        qDebug() << QString::number(SignWord, 2);
        quint16 AntennaStatus = payload[3];
        AntennaStatus <<= 8;
        AntennaStatus += payload[2];
        qDebug() << "Статус антенны" << QString::number(AntennaStatus, 2);
        quint8 seconds = payload[6];
        quint8 minutes = payload[5];
        quint8 hours = payload[4];
        qDebug() << "Время: "<< hours <<":"<<minutes<<":"<<seconds;
        quint32 fragment_time = 0;
        for (int h = 0; h <= 3; h++)
        {
            fragment_time += payload[11-h];
            fragment_time <<= 8;
        }
        qDebug() << "Дробная часть времени:" << fragment_time;
        quint8 year = payload[12];
        quint8 month = payload[13];
        quint8 day = payload[14];
        qDebug() << "Дата:" << day <<"."<<month<<"."<<year;
        // РУБИКОН - ДАЛЬШЕ НЕ РАБОТАЕТ
        qint64 X = 0;
        qint64 Y = 0;
        qint64 Z = 0;
        int h = 0;
        for(h=0; h<=7; h++){
            X += payload[15+h];
            Y += payload[23+h];
            Z += payload[31+h];
            X <<= 8;
            Y <<= 8;
            Z <<= 8;
        }
        double X_finish = static_cast<double>(X);
        double Y_finish = static_cast<double>(Y);
        double Z_finish = static_cast<double>(Z);
        qDebug() << "Latitide =" << qToBigEndian(X_finish);
        qDebug() << "Longitude =" << qToBigEndian(Y_finish);
        qDebug() << "Height =" << qToBigEndian(Z_finish);
        quint32 SeeNKA_GPS = 0;
        quint32 SeeNKA_GLONASS = 0;
        quint32 Navigation_NKA_GPS = 0;
        quint32 Navigation_NKA_GLONASS = 0;
        for (int h = 0; h <= 3; h++)
        {
          quint32 SeeNKA_GPS = payload[39+h];
          SeeNKA_GPS <<= 8;
          quint32 SeeNKA_GLONASS = payload[43+h];
          SeeNKA_GLONASS <<= 8;
          quint32 Navigation_NKA_GPS = payload[47+h];
          Navigation_NKA_GPS <<= 8;
          quint32 Navigation_NKA_GLONASS = payload[51+h];
          Navigation_NKA_GLONASS <<= 8;
        }
        qDebug() << "Видимые НКА GPS: " << QString::number(SeeNKA_GPS, 2);
        qDebug() << "Видимые НКА ГЛОНАСС: " << QString::number(SeeNKA_GLONASS, 2);
        qDebug() << "Навигационные НКА GPS: " << QString::number(Navigation_NKA_GPS, 2);
        qDebug() << "Навигационные НКА ГЛОНАСС: " << QString::number(Navigation_NKA_GLONASS, 2);


        results += "Слово признаков: " +QString::number(SignWord, 2) + "\n" +
                "Статус антенны: " + QString::number(AntennaStatus, 2) + "\n" +
                "Время: " + QString::number(hours) + ":" + QString::number(minutes) + ":" + QString::number(seconds) + "\n" +
                "Дробная часть времени: " + QString::number(fragment_time) + "\n" +
                "Latitide: " + QString::number(X_finish) + "\n" +
                "Longitude: " + QString::number(Y_finish) + "\n" +
                "Height: " + QString::number(Z_finish) + "\n" +
                "Видимые НКА GPS: " + QString::number(SeeNKA_GPS, 2) + "\n" +
                "Видимые НКА ГЛОНАСС: " + QString::number(SeeNKA_GLONASS, 2) + "\n" +
                "Навигационные НКА GPS: " + QString::number(Navigation_NKA_GPS, 2) + "\n" +
                "Навигационные НКА ГЛОНАСС: " + QString::number(Navigation_NKA_GLONASS, 2) + "\n";


        quint16 NKAchannel = 0;
        quint16 StateOfChannel = 0;
        qint32 angleNKA = 0;
        qint32 azimut = 0;
        qint32 signal_noise = 0;
        for (int h = 0; h <= 15; h++)
        {
            int counter = 1;
            NKAchannel += payload[56+16*h];
            NKAchannel <<= 8;
            NKAchannel += payload[55+16*h];
            qDebug() << "Идентификатор НКА на канале i=" << counter << ": " << QString::number(NKAchannel, 2);
            results += "Идентификатор НКА на канале i=" + QString::number(counter) + ": " + QString::number(NKAchannel, 2);
            int id = NKAchannel & 0x1100000000000000;
            if (id == 0){ // GPS
                quint16 liter_GPS = NKAchannel & 0x0000000011111111;
                qDebug() << liter_GPS;
                results += "Литера спутника GPS:" + QString::number(liter_GPS) + "\n";
            }
            else if (id==2){
                quint16 id_glonass = NKAchannel & 0x0000001111100000;
                quint16 liter_glonass = NKAchannel & 0x0000000000011111;
                qDebug() << id_glonass;
                qDebug() << liter_glonass;
                results += "Идентификатор спутника ГЛОНАСС:" + QString::number(id_glonass) + "\n" +
                        "Литера спутника ГЛОНАСС: " + QString::number(liter_glonass) + "\n";
            }
            StateOfChannel += payload[58+16*h];
            StateOfChannel <<= 8;
            StateOfChannel += payload[57+16*h];
            qDebug() << "Состояние канала i=" << counter << ": " << QString::number(StateOfChannel, 2);
            results += "Состояние канала i=" + QString::number(counter) + ": " + QString::number(StateOfChannel, 2) + "\n";
            for (int i = 0; i <= 3; i++)
            {
                angleNKA += payload[62-i+16*h];
                azimut += payload[66-i+16*h];
                signal_noise += payload[70-i+16*h];
                angleNKA <<= 8;
                azimut <<= 8;
                signal_noise <<= 8;
            }
            float angleNKA_finish = static_cast<float>(angleNKA);
            float azimut_finish = static_cast<float>(azimut);
            float signal_noise_finish = static_cast<float>(signal_noise);
            qDebug() << "Угол места НКА i=" << counter << ": " << angleNKA_finish;
            qDebug() << "Азимут(курс) НКА i=" << counter << ": " << azimut_finish;
            qDebug() << "Отношение сигнал/шум для НКА i=" << counter << ": " << signal_noise_finish;

            results += "Угол места НКА i=" + QString::number(counter) + ": " + QString::number(angleNKA_finish) + "\n" +
            "Азимут(курс) НКА i=" + QString::number(counter) + ": " + QString::number(azimut_finish) + "\n" +
            "Отношение сигнал/шум для НКА i=" + QString::number(counter) + ": " + QString::number(signal_noise_finish) + "\n";

        }
    }
