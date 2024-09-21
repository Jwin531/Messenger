QT       += core gui
QT += core sql
QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Optional: Add debug flags
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

INCLUDEPATH += /usr/include
LIBS += -L/usr/lib -lboost_system -lboost_filesystem

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore
