QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

TARGET = SpecificHeat
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    datachartform.cpp \
    modelmixture.cpp

HEADERS += \
        mainwindow.h \
    datachartform.h \
    modelmixture.h

FORMS += \
        mainwindow.ui \
    datachartform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L/usr/lib/kappa/ -lkappa++

INCLUDEPATH += /usr/include/kappa/
DEPENDPATH += /usr/include/kappa/

unix:!macx: LIBS += -lyaml -lyaml-cpp
