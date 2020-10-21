TEMPLATE = lib
DEFINES += LIFTUI_LIBRARY
QT += widgets sql testlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    liftquerymodel.cpp \
    liftquerynode.cpp \
    liftsearchform.cpp

HEADERS += \
    lift-ui_global.h \
    liftquerymodel.h \
    liftquerynode.h \
    liftsearchform.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    liftsearchform.ui

win32-g++ {
    win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../build-rude-mechanicals-Desktop_Qt_5_15_1_MinGW_64_bit-Release/release/ -lrude-mechanicals
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../build-rude-mechanicals-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/debug/ -lrude-mechanicals
    else:unix: LIBS += -L$$OUT_PWD/../../build-rude-mechanicals-Desktop_Qt_5_15_1_MinGW_64_bit/ -lrude-mechanicals
}

win32-msvc* {
    win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../build-rude-mechanicals-Desktop_Qt_5_15_1_MSVC2019_64bit-Release/release/ -lrude-mechanicals
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../build-rude-mechanicals-Desktop_Qt_5_15_1_MSVC2019_64bit-Debug/debug/ -lrude-mechanicals
    else:unix: LIBS += -L$$OUT_PWD/../../build-rude-mechanicals-Desktop_Qt_5_15_1_MSVC2019_64bit-Debug/ -lrude-mechanicals
}

INCLUDEPATH += $$PWD/../../rude-mechanicals
DEPENDPATH += $$PWD/../../rude-mechanicals

win32-g++ {
    win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../build-mortal-engine-Desktop_Qt_5_15_1_MinGW_64_bit-Release/mortal-engine/release/ -lmortalengine
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../build-mortal-engine-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/mortal-engine/debug/ -lmortalengine
    else:unix: LIBS += -L$$OUT_PWD/../../build-mortal-engine-Desktop_Qt_5_15_1_MinGW_64_bit/mortal-engine/ -lmortalengine
}

win32-msvc* {
    win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../build-mortal-engine-Desktop_Qt_5_15_1_MSVC2019_64bit-Release/mortal-engine/release/ -lmortalengine
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../build-mortal-engine-Desktop_Qt_5_15_1_MSVC2019_64bit-Debug/mortal-engine/debug/ -lmortalengine
    else:unix: LIBS += -L$$OUT_PWD/../../build-mortal-engine-Desktop_Qt_5_15_1_MSVC2019_64bit-Debug/mortal-engine/ -lmortalengine
}

INCLUDEPATH += $$PWD/../../mortal-engine/mortal-engine
DEPENDPATH += $$PWD/../../mortal-engine/mortal-engine

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lift-sqlite-adapter/release/ -llift-sqlite-adapter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lift-sqlite-adapter/debug/ -llift-sqlite-adapter
else:unix: LIBS += -L$$OUT_PWD/../lift-sqlite-adapter/ -llift-sqlite-adapter

INCLUDEPATH += $$PWD/../lift-sqlite-adapter
DEPENDPATH += $$PWD/../lift-sqlite-adapter
