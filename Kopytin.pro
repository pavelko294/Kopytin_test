QT += core gui serialport
CONFIG += с++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x060000

equals(QMAKE_CXX, g++) {
QMAKE_CXXFLAGS += \
#    -std=gnu++11 \
    -Wextra \
    -Wall \
    -Werror \
#    -g \
#    -mfpmath=387 \
#    -Wunreachable-code

QMAKE_LFLAGS += \
#    -g

CONFIG(sc_32bit) {
QMAKE_CXXFLAGS += -m32
QMAKE_LFLAGS += -m32
}

!win32 {
QMAKE_CXXFLAGS += \
#    -rdynamic

QMAKE_LFLAGS += \
#    -rdynamic

}
} else {
QMAKE_CXXFLAGS += \
    /W3 \
    /WX

QMAKE_LFLAGS += \
    /WX

}

HEADERS += \
    binarytparser.h \
    crutches.h \
    crutches.tpp \
    filewriter.h \
    mainwindow.h \
    parserofmessage.h \
    portpickerdialog.h \
    serialportinfomodel.h \
    serialportsettings.h

SOURCES += \
    binarytparser.cpp \
    crutches.cpp \
    filewriter.cpp \
    main.cpp \
    mainwindow.cpp \
    parserofmessage.cpp \
    portpickerdialog.cpp \
    serialportinfomodel.cpp \
    serialportsettings.cpp

FORMS += \
    mainwindow.ui \
    portpickerdialog.ui
