TEMPLATE = lib
DEFINES += LIFTUI_LIBRARY
QT += widgets sql testlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lift-ui.cpp \
    liftdatabase.cpp \
    liftquerymodel.cpp \
    liftquerynode.cpp \
    liftsearchform.cpp

HEADERS += \
    lift-ui_global.h \
    lift-ui.h \
    liftdatabase.h \
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

win32:CONFIG(release, debug|release): LIBS += -Lc:/QtBuilds/build-rude-mechanicals-Desktop_Qt_5_15_1_MinGW_64_bit/release/ -lrude-mechanicals
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/QtBuilds/build-rude-mechanicals-Desktop_Qt_5_15_1_MinGW_64_bit/debug/ -lrude-mechanicals
else:unix: LIBS += -Lc:/QtBuilds/build-rude-mechanicals-Desktop_Qt_5_15_1_MinGW_64_bit/ -lrude-mechanicals

INCLUDEPATH += $$PWD/../../rude-mechanicals
DEPENDPATH += $$PWD/../../rude-mechanicals


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../QtBuilds/build-mortal-engine-Desktop_Qt_5_15_1_MinGW_64_bit/mortal-engine/release/ -lmortalengine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../QtBuilds/build-mortal-engine-Desktop_Qt_5_15_1_MinGW_64_bit/mortal-engine/debug/ -lmortalengine
else:unix: LIBS += -L$$PWD/../../../../../../QtBuilds/build-mortal-engine-Desktop_Qt_5_15_1_MinGW_64_bit/mortal-engine/ -lmortalengine

INCLUDEPATH += $$PWD/../../mortal-engine/mortal-engine
DEPENDPATH += $$PWD/../../mortal-engine/mortal-engine
