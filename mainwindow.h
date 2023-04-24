#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialportsettings.h"
#include "binarytparser.h"
#include "filewriter.h"

#include <QMainWindow>
#include <QFile>

class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *const m_ui;
    QLabel *const m_statusLabel;
    QSerialPort m_port;
    qint64 m_bytesRead {0};
    BinaryTParser m_parser;
    QFile m_file;

private:
    bool OpenAndTune(const SerialPortSettings &settings, QString *culprit);
    ParserOfMessage parser;
    FileWriter filer;

private slots:
    void on_action_openPort_triggered();
    void OnSerialPortErrorOccurred(QSerialPort::SerialPortError error);
    void OnSerialPortReadyRead(void);
};

#endif // MAINWINDOW_H
