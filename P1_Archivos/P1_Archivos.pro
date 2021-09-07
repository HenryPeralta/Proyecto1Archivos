QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        comando_fdisk.cpp \
        comando_mkdisk.cpp \
        comando_mount.cpp \
        comando_rmdisk.cpp \
        estructuras.cpp \
        main.cpp \
        parser.cpp \
        scanner.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    lexico.l \
    parser.y

HEADERS += \
    comando_fdisk.h \
    comando_mkdisk.h \
    comando_mount.h \
    comando_rmdisk.h \
    estructuras.h \
    parser.h \
    scanner.h
