#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    CreateMainForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateMainForm()
{
    menuBar = new SMenuBar( this );
    setMenuBar( menuBar );

    toolBar = new SToolBar( *menuBar, this );
    addToolBar( Qt::TopToolBarArea, toolBar );

    QWidget* widget = new QWidget( this );
    mainLayout = new QGridLayout( widget );
    widget->setLayout( mainLayout );

    toolBox = new SToolBox( this );
    tabWidget = new STabWidget( this );

    mainLayout->addWidget( toolBox, 0, 0 );
    mainLayout->addWidget( tabWidget, 0, 1 );

    setCentralWidget( widget );
}
