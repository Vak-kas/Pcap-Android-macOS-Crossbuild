QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common.cpp \
    daemonmanager.cpp \
    main.cpp \
    pcapmanager.cpp \
    widget.cpp

HEADERS += \
    common.h \
    daemonmanager.h \
    pcapmanager.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# macOS
macx {
    INCLUDEPATH += $$PWD/../libpcap/out/macos/include
    LIBS += -L$$PWD/../libpcap/out/macos/lib -lpcap
}

# Android
android {
    INCLUDEPATH += $$PWD/../libpcap/out/android/include
    LIBS += -L$$PWD/../libpcap/out/android/lib -lpcap
}

# static link
LIBS += -lpcap
