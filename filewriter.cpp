#include "filewriter.h"

#include <QDateTime>

FileWriter::FileWriter()
{

}

void FileWriter::WriteToFile(const QString &results)
{
    QFile file;
    file.setFileName("results" + QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss") + ".txt");
    if (!file.open(QIODevice::WriteOnly))
        return;
    //qDebug() << "Смотри че высрал" << results;
    file.write(results.toUtf8());
    file.close();
}
