QT       += core gui
QT += core sql
QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Optional: Add debug flags
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g
}

# Добавляем пути для Redis++ и Hiredis
INCLUDEPATH += /usr/local/include/sw/redis++ /usr/local/include
LIBS += -L/usr/local/lib -lredis++ -lhiredis

# Источники и заголовочные файлы
SOURCES += \
    client.cpp \
    database.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp

HEADERS += \
    client.h \
    database.h \
    logindialog.h \
    mainwindow.h \
    registerdialog.h

FORMS += \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui

LIBS += -L/usr/lib -lboost_system -lboost_filesystem

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore
