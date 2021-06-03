QT       += core gui printsupport widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FieldOfView.cpp \
    Quadrant.cpp \
    Row.cpp \
    Tile.cpp \
    battlescene.cpp \
    entity.cpp \
    group.cpp \
    groupcreationwindow.cpp \
    item.cpp \
    main.cpp \
    mainwindow.cpp \
    mapwidget.cpp \
    token.cpp \
    tokencontrollerwidget.cpp \
    tokencreationwidget.cpp

HEADERS += \
    FieldOfView.h \
    Quadrant.h \
    Row.h \
    Tile.h \
    battlescene.h \
    entity.h \
    group.h \
    groupcreationwindow.h \
    item.h \
    mainwindow.h \
    mapwidget.h \
    token.h \
    tokencontrollerwidget.h \
    tokencreationwidget.h

FORMS += \
    groupcreationwindow.ui \
    mainwindow.ui \
    tokencontrollerwidget.ui \
    tokencreationwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
