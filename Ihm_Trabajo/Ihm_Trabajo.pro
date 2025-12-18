QT       += core gui sql svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svgwidgets

CONFIG += c++17

# Include path for navigation library headers
INCLUDEPATH += $$PWD/lib/include

# Include paths for project modules
INCLUDEPATH += $$PWD/controllers
INCLUDEPATH += $$PWD/widgets
INCLUDEPATH += $$PWD/utils

# You can make your code fail to compile if it uses deprecated APIs.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    controllers/logincontroller.cpp \
    controllers/registercontroller.cpp \
    controllers/chartcontroller.cpp \
    controllers/sessioncontroller.cpp \
    controllers/profilecontroller.cpp \
    widgets/chartwidget.cpp \
    widgets/draggableprotractor.cpp \
    widgets/draggableruler.cpp \
    widgets/strokesettingspopup.cpp \
    utils/validators.cpp \
    lib/navigation.cpp \
    lib/navigationdao.cpp \
    views/profileview.cpp \
    views/resultsview.cpp

HEADERS += \
    mainwindow.h \
    controllers/logincontroller.h \
    controllers/registercontroller.h \
    controllers/chartcontroller.h \
    controllers/sessioncontroller.h \
    controllers/profilecontroller.h \
    widgets/chartwidget.h \
    widgets/draggableprotractor.h \
    widgets/draggableruler.h \
    widgets/strokesettingspopup.h \
    views/profileview.h \
    views/resultsview.h \
    utils/validators.h \
    utils/charttypes.h \
    lib/include/navigation.h \
    lib/include/navigationdao.h \
    lib/include/navtypes.h \
    lib/include/navdaoexception.h

FORMS += \
    mainwindow.ui \
    views/loginview.ui \
    views/registerview.ui \
    views/profileview.ui \
    views/resultsview.ui

# Resources
RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

