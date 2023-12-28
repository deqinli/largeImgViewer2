QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./include/
INCLUDEPATH += ./include/opencv2/

CONFIG += debug_and_release
CONFIG(debug,debug|release){
    LIBS += -L$$PWD/lib/ -lopencv_world480d
    TARGET = loadTiles2d
}
CONFIG(release,debug|release){
    LIBS += -L$$PWD/lib/ -lopencv_world480
    TARGET = loadTiles2
}

DESTDIR = ..\bin

SOURCES += \
    MyGView.cpp \
    loadImg.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    MyGView.h \
    loadImg.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
