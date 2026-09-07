QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    125liveseed.cpp \
    125seedTask.cpp \
    dispTask.cpp \
    healthseed.cpp \
    main.cpp \
    mainwindow.cpp \
    myclient.cpp \
    myserver.cpp \
    mytask.cpp \
    native.cpp \
    simclient.cpp \
    writeEvent.cpp

HEADERS += \
    125liveseed.h \
    125seedTask.h \
    dispTask.h \
    healthseed.h \
    mainwindow.h \
    myclient.h \
    myserver.h \
    mytask.h \
    native.h \
    simclient.h \
    writeEvent.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target





win32:CONFIG(release, debug|release): LIBS += -L$$PWD/mseed/release/ -lmseed
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/mseed/debug/ -lmseed
else:unix: LIBS += -L$$PWD/mseed/ -lmseed

INCLUDEPATH += $$PWD/mseed
DEPENDPATH += $$PWD/mseed

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mseed/release/libmseed.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mseed/debug/libmseed.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mseed/release/mseed.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mseed/debug/mseed.lib
else:unix: PRE_TARGETDEPS += $$PWD/mseed/libmseed.a

