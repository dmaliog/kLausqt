QT += webenginewidgets
QT += multimedia
QT += multimediawidgets
QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32: RC_ICONS += ":/img/2.png"

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scrapper.cpp

HEADERS += \
    mainwindow.h \
    scrapper.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    klausres.qrc

TRANSLATIONS += lang/kLaus_ru_RU.ts \
               lang/kLaus_en_US.ts
