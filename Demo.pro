#-------------------------------------------------
#
# Project created by QtCreator 2014-07-09T10:13:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cimgproc.cpp \
    cepsilonfilter.cpp \
    cimgcontroller.cpp \
    cwebcam.cpp \
    SourceASM/demo_fit.cpp \
    SourceASM/video_camera.cpp \
    SourceASM/vjfacedetect.cpp

HEADERS  += mainwindow.h \
    cimgproc.h \
    cepsilonfilter.h \
    cimgcontroller.h \
    cwebcam.h \
    SourceASM/asmbuilding.h \
    SourceASM/asmfitting.h \
    SourceASM/asmlibrary.h \
    SourceASM/video_camera.h \
    SourceASM/vjfacedetect.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/release/ -lopencv_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/debug/ -lopencv_core
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/ -lopencv_core

INCLUDEPATH += $$PWD/../../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/release/ -lopencv_highgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/debug/ -lopencv_highgui
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/ -lopencv_highgui

INCLUDEPATH += $$PWD/../../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/release/ -lopencv_imgproc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/debug/ -lopencv_imgproc
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/ -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/release/ -lopencv_objdetect
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/debug/ -lopencv_objdetect
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/ -lopencv_objdetect
INCLUDEPATH += $$PWD/../../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../../usr/include

OTHER_FILES += \
    libs/libasmlibrary.so

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/SourceASM/release/ -lasmlibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/SourceASM/debug/ -lasmlibrary
else:unix: LIBS += -L$$PWD/SourceASM/ -lasmlibrary

INCLUDEPATH += $$PWD/SourceASM
DEPENDPATH += $$PWD/SourceASM
