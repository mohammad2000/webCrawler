QT -= gui
QT +=network sql xml 
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QT += webkitwidgets
    QT += widgets
    QT += quick
    QT += qml

} else {
    QT += webkit
    QT += declarative
}
QT -= gui
QT +=network sql
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QT += webkitwidgets
} else {
    QT += webkit
}
CONFIG += c++11
include(QxOrm/QxOrm.pri)

CONFIG -= app_bundle

INCLUDEPATH += QxOrm/include/
LIBS += -L"QxOrm/test/_bin"
!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
PRECOMPILED_HEADER = precompiled.h
}
INCLUDEPATH += QxOrm/include/

include(qamqp.pri)
CONFIG(debug, debug|release) {

LIBS += -L$$PWD/QxOrm/lib/ -lQxOrm

} else {
LIBS +=  -L$$PWD/QxOrm/lib/ -lQxOrm
}
macx: LIBS += -L$$PWD/qamqp-master/src/ -lqamqp.1.0.0

INCLUDEPATH += $$PWD/qamqp-master/src
DEPENDPATH += $$PWD/qamqp-master/src

DEFINES += QT_DEPRECATED_WARNINGS
QT += network


SOURCES += \
    initserver.cpp \
    main.cpp

HEADERS += \
    export.h \
    initserver.h \
    precompiled.h


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
macx: LIBS += -L$$PWD/qamqp-master/src/ -lqamqp.1.0.0

INCLUDEPATH += $$PWD/qamqp-master/src
DEPENDPATH += $$PWD/qamqp-master/src
unix:!macx: LIBS += -L$$PWD/qamqp-master/src/ -lqamqp

INCLUDEPATH += $$PWD/qamqp-master/src
DEPENDPATH += $$PWD/qamqp-master/src
