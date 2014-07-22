#-------------------------------------------------
#
# Project created by QtCreator 2014-07-09T10:13:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app


SOURCES +=SourceASM/demo_fit.cpp \
    SourceASM/video_camera.cpp \
    SourceASM/vjfacedetect.cpp \
    src/cepsilonfilter.cpp \
    src/cimgcontroller.cpp \
    src/cimgproc.cpp \
    src/cwebcam.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/cdetecttrack.cpp \
    src/cvideodevice.cpp

HEADERS  +=SourceASM/asmbuilding.h \
    SourceASM/asmfitting.h \
    SourceASM/asmlibrary.h \
    SourceASM/video_camera.h \
    SourceASM/vjfacedetect.h \
    ../OpenCV2ComputerVisionApplicationProgrammingCookbook/3241_Code/Chapter 03/colorDetectController.h \
    include/cepsilonfilter.h \
    include/cimgcontroller.h \
    include/cimgproc.h \
    include/cwebcam.h \
    include/mainwindow.h \
    include/cfacialBeauty.h \
    include/cdetecttrack.h \
    include/cvideodevice.h

FORMS    += mainwindow.ui


OTHER_FILES += \
    libs/libasmlibrary.so \
    Resources/haarcascade_frontalface_alt2.xml \
    Resources/my68-1d.amf



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/release/ -lasmlibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/debug/ -lasmlibrary
else:unix: LIBS += -L$$PWD/libs/ -lasmlibrary

INCLUDEPATH += $$PWD/SourceASM
DEPENDPATH += $$PWD/SourceASM

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/release/ -lopencv_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/debug/ -lopencv_core
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/local/lib/ -lopencv_core

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/release/ -lopencv_imgproc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/debug/ -lopencv_imgproc
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/local/lib/ -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/release/ -lopencv_objdetect
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/debug/ -lopencv_objdetect
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/local/lib/ -lopencv_objdetect

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/release/ -lopencv_highgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/debug/ -lopencv_highgui
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/local/lib/ -lopencv_highgui

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/include
