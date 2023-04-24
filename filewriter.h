#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QFile>

#include "parserofmessage.h"

class FileWriter: public QObject
{
    Q_OBJECT
public:
    FileWriter();

public slots:

   void WriteToFile(const QString &results);
};

#endif // FILEWRITER_H
