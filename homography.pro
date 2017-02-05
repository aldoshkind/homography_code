#-------------------------------------------------
#
# Project created by QtCreator 2017-01-16T22:54:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = homography
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    resultwindow.cpp

HEADERS  += mainwindow.h \
    resultwindow.h

FORMS    += mainwindow.ui \
    resultwindow.ui


#Подключение заголовочных файлов OpenCV
INCLUDEPATH+=C:/opencv/build/include

#Подключение библиотек OpenCV


LIBS+=C:/opencv/build/x86/vc10/bin/opencv_imgproc249.dll
LIBS+=C:/opencv/build/x86/vc10/bin/opencv_highgui249.dll
LIBS+=C:/opencv/build/x86/vc10/bin/opencv_core249.dll

RESOURCES += \
    myres.qrc
