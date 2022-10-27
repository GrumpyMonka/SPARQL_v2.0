QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DiagramItems/DiagramArrow/diagramarrow.cpp \
    DiagramItems/DiagramBlocks/BasedBlock/basedblocksettings.cpp \
    DiagramItems/DiagramBlocks/BasedBlock/basedblockwindow.cpp \
    DiagramItems/DiagramBlocks/BasedBlock/diagramitembased.cpp \
    DiagramItems/DiagramBlocks/CompositeBlock/compositeblocksettings.cpp \
    DiagramItems/DiagramBlocks/CompositeBlock/compositeblockwindow.cpp \
    DiagramItems/DiagramBlocks/CompositeBlock/diagramitemcomposite.cpp \
    DiagramItems/DiagramBlocks/SparqlBlock/diagramitemsparql.cpp \
    DiagramItems/DiagramBlocks/SparqlBlock/sparqlblocksettings.cpp \
    DiagramItems/DiagramBlocks/SparqlBlock/sparqlblockwindow.cpp \
    DiagramItems/DiagramBlocks/blockslibrary.cpp \
    DiagramItems/DiagramItem/diagramitem.cpp \
    DiagramItems/DiagramItemSettings/diagramitemsettings.cpp \
    DiagramItems/DiagramScene/diagramscene.cpp \
    DiagramItems/DiagramWindows/projectwindow.cpp \
    SWidgets/SGraphicsView/sgraphicsview.cpp \
    SWidgets/STabWidget/stabwidget.cpp \
    SWidgets/SToolBar/stoolbar.cpp \
    SWidgets/SToolBox/stoolbox.cpp \
    main.cpp \
    MainForm/mainwindow.cpp \
    SWidgets/SMenuBar/smenubar.cpp

HEADERS += \
    DiagramItems/DiagramArrow/diagramarrow.h \
    DiagramItems/DiagramBlocks/BasedBlock/basedblocksettings.h \
    DiagramItems/DiagramBlocks/BasedBlock/basedblockwindow.h \
    DiagramItems/DiagramBlocks/BasedBlock/diagramitembased.h \
    DiagramItems/DiagramBlocks/CompositeBlock/compositeblocksettings.h \
    DiagramItems/DiagramBlocks/CompositeBlock/compositeblockwindow.h \
    DiagramItems/DiagramBlocks/CompositeBlock/diagramitemcomposite.h \
    DiagramItems/DiagramBlocks/SparqlBlock/diagramitemsparql.h \
    DiagramItems/DiagramBlocks/SparqlBlock/sparqlblocksettings.h \
    DiagramItems/DiagramBlocks/SparqlBlock/sparqlblockwindow.h \
    DiagramItems/DiagramBlocks/blockslibrary.h \
    DiagramItems/DiagramItem/diagramitem.h \
    DiagramItems/DiagramItemSettings/diagramitemsettings.h \
    DiagramItems/DiagramScene/diagramscene.h \
    DiagramItems/DiagramWindows/projectwindow.h \
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
    SWidgets/SGraphicsView \
    DiagramItems/DiagramWindows \
    DiagramItems/DiagramItem \
    DiagramItems/DiagramArrow \
    DiagramItems/DiagramScene \
    DiagramItems/DiagramItemSettings \
    DiagramItems/DiagramBlocks/BasedBlock \
    DiagramItems/DiagramBlocks/CompositeBlock \
    DiagramItems/DiagramBlocks/SparqlBlock \
    DiagramItems/DiagramBlocks \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sources.qrc
