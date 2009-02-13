######################################################################
# Automatically generated by qmake (2.01a) Sun Nov 23 21:44:31 2008
######################################################################

TEMPLATE = app
TARGET = AnacondaII
DEPENDPATH += . ../lib
INCLUDEPATH += . ../inc ../src
QT += sql
CONFIG += debug
CONFIG -= release

LIBS += -L../lib -lanaconda

message(PCAN: $$PCAN)
contains(PCAN, fake) {
  LIBS += -L../fakepcan -lpcan
} else {
  LIBS += -lpcan
}

debug {
}
message($$CONFIG)


DESTDIR = ../

# Input
HEADERS += \
           KLevel1Model.h \
           KLevel2View.h \
           KLevel3View.h \
           KMainWindow.h KProgressIndicator.h KSimpleWindow.h \
           KTcpuView.h \
           KTdigModel.h \
           KTdigView.h \
           KThubView.h \
           KSerdesModel.h KSerdesView.h \
           KConsole.h

SOURCES += main.cpp \
           KLevel1Model.cpp \
           KLevel2View.cpp \
           KLevel3View.cpp \
           KMainWindow.cpp KProgressIndicator.cpp KSimpleWindow.cpp \
           KTcpuView.cpp \
           KTdigModel.cpp \
           KTdigView.cpp \
           KThubView.cpp \
           KSerdesModel.cpp KSerdesView.cpp \
           KConsole.cpp

RESOURCES += toolbar.qrc
