######################################################################
# Automatically generated by qmake (3.0) ? 7? 14 20:43:31 2018
######################################################################

TEMPLATE = app
TARGET = gui

QT += opengl
QT += uitools

LIBS += -L/usr/local/lib -lkqt -lkqtgl -lktl -lGLU 

# Input
HEADERS += gl.h mainwindow.h
SOURCES += main.cpp mainwindow.cpp gl.cpp ../modules/arm.cpp

FORMS += parameter.ui 






