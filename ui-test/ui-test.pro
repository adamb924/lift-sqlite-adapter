QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp

HEADERS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../QtBuilds/build-lift-sqlite-adapter-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/lift-ui/release/ -llift-ui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../QtBuilds/build-lift-sqlite-adapter-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/lift-ui/debug/ -llift-ui
else:unix: LIBS += -L$$PWD/../../../../../../QtBuilds/build-lift-sqlite-adapter-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/lift-ui/ -llift-ui

INCLUDEPATH += $$PWD/../lift-ui
DEPENDPATH += $$PWD/../lift-ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../QtBuilds/build-mortal-engine-Desktop_Qt_5_15_1_MinGW_64_bit/mortal-engine/release/ -lmortalengine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../QtBuilds/build-mortal-engine-Desktop_Qt_5_15_1_MinGW_64_bit/mortal-engine/debug/ -lmortalengine
else:unix: LIBS += -L$$PWD/../../../../../../QtBuilds/build-mortal-engine-Desktop_Qt_5_15_1_MinGW_64_bit/mortal-engine/ -lmortalengine

INCLUDEPATH += $$PWD/../../mortal-engine/mortal-engine
DEPENDPATH += $$PWD/../../mortal-engine/mortal-engine

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../QtBuilds/build-rude-mechanicals-Desktop_Qt_5_15_1_MinGW_64_bit/release/ -lrude-mechanicals
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../QtBuilds/build-rude-mechanicals-Desktop_Qt_5_15_1_MinGW_64_bit/debug/ -lrude-mechanicals
else:unix: LIBS += -L$$PWD/../../../../../../QtBuilds/build-rude-mechanicals-Desktop_Qt_5_15_1_MinGW_64_bit/ -lrude-mechanicals

INCLUDEPATH += $$PWD/../../rude-mechanicals
DEPENDPATH += $$PWD/../../rude-mechanicals
