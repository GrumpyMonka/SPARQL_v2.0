#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "projectwindow.h"
#include <QDebug>

#include <basedblockwindow.h>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    library = new BlocksLibrary();
    library->loadBlocksFormFiles( FOLDER_FOR_BLOCKS );

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
    connect( tabWidget, SIGNAL( currentMode( int ) ),
        this, SLOT( slotCurrentTabMode( int ) ) );
    // тест
    BasedBlockWindow* window = new BasedBlockWindow( BasedBlockWindow::CreateMode, tabWidget );
    //tabWidget->addWidget( window, BasedBlockWindow::DiagramMode, "TEST" );
    tabWidget->addTab( window, "TEST" );

    mainLayout->addWidget( toolBox, 0, 0 );
    mainLayout->addWidget( tabWidget, 0, 1 );

    setCentralWidget( widget );
}

void MainWindow::slotCreateNewProject()
{
    tabWidget->addWidget( new ProjectWindow( this ), ProjectWindow::DiagramMode, "kek" );
}

void MainWindow::slotCurrentTabMode( int mode )
{
    toolBox->setDiagramItems( library->getBlocks( mode ) );
}
