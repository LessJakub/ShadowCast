QT       += core gui printsupport widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/FieldOfView.cpp \
    Sources/Quadrant.cpp \
    Sources/Row.cpp \
    Sources/Tile.cpp \
    Sources/battlescene.cpp \
    Sources/entity.cpp \
    Sources/group.cpp \
    Sources/groupcreationwindow.cpp \
    Sources/item.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp \
    Sources/mapwidget.cpp \
    Sources/token.cpp \
    Sources/tokencontrollerwidget.cpp \
    Sources/tokencreationwidget.cpp

HEADERS += \
    Headers/FieldOfView.h \
    Headers/Quadrant.h \
    Headers/Row.h \
    Headers/Tile.h \
    Headers/battlescene.h \
    Headers/entity.h \
    Headers/group.h \
    Headers/groupcreationwindow.h \
    Headers/item.h \
    Headers/mainwindow.h \
    Headers/mapwidget.h \
    Headers/token.h \
    Headers/tokencontrollerwidget.h \
    Headers/tokencreationwidget.h

FORMS += \
    Forms/groupcreationwindow.ui \
    Forms/mainwindow.ui \
    Forms/tokencontrollerwidget.ui \
    Forms/tokencreationwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
