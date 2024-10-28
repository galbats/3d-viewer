QT       += core gui opengl openglwidgets widgets

include(qgifimage/gifimage/qtgifimage.pri)

CONFIG += c++17

SOURCES += \
    ../backend/model.cpp \
    form.cpp \
    main.cpp \
    customopenglwidget.cpp \
    mainwindow.cpp

HEADERS += \
    ../backend/controller.h \
    ../backend/model.h \
    customopenglwidget.h \
    form.h \
    mainwindow.h

FORMS += \
    form.ui \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
