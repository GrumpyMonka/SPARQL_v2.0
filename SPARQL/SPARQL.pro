QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DiagramItems/DiagramScene/diagramscene.cpp \
    SWidgets/SGraphicsView/sgraphicsview.cpp \
    SWidgets/STabWidget/stabwidget.cpp \
    SWidgets/SToolBar/stoolbar.cpp \
    SWidgets/SToolBox/stoolbox.cpp \
    main.cpp \
    MainForm/mainwindow.cpp \
    SWidgets/SMenuBar/smenubar.cpp

HEADERS += \
    DiagramItems/DiagramScene/diagramscene.h \
    MainForm/mainwindow.h \
    SWidgets/SGraphicsView/sgraphicsview.h \
    SWidgets/STabWidget/stabwidget.h \
    SWidgets/SToolBar/stoolbar.h \
    SWidgets/SMenuBar/smenubar.h \
    SWidgets/SToolBox/stoolbox.h

FORMS += \
    MainForm/mainwindow.ui

INCLUDEPATH += \
    MainForm \
    SWidgets/SToolBar \
    SWidgets/SMenuBar \
    SWidgets/SToolBox \
    SWidgets/STabWidget \
    DiagramItems/DiagramScene

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sources.qrc
