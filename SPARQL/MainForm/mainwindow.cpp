#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "projectwindow.h"
#include <QDebug>

#include <atomblocksettings.h>
#include <basedblockwindow.h>
#include <compositeblocksettings.h>
#include <sparqlblockwindow.h>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    library = new BlocksLibrary();
    library->loadBlocksFormFiles( FOLDER_FOR_BLOCKS );
    library->addBlocks( AtomBlockSettings::GetBasedAtomBlocks() );

    createMainForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMainForm()
{
    menu_bar = new SMenuBar( this );
    setMenuBar( menu_bar );

    QWidget* widget = new QWidget( this );
    main_layout = new QGridLayout( widget );
    widget->setLayout( main_layout );

    tab_widget = new STabWidget( this );
    connect( tab_widget, SIGNAL( currentMode( int ) ),
        this, SLOT( slotCurrentTabMode( int ) ) );

    tool_box = new SToolBox( this );
    connect( tool_box, SIGNAL( itemPressed( DiagramItemSettings* ) ),
        tab_widget, SLOT( itemForAdd( DiagramItemSettings* ) ) );

    tool_bar = new SToolBar( *menu_bar, this );
    addToolBar( Qt::TopToolBarArea, tool_bar );
    connect( tool_bar, SIGNAL( setNewDiagramSceneMode( int ) ),
        tab_widget, SLOT( setSceneMode( int ) ) );

    main_layout->addWidget( tool_box, 0, 0 );
    main_layout->addWidget( tab_widget, 0, 1 );

    setCentralWidget( widget );
}

void MainWindow::slotCreateNewProject()
{
    tab_widget->addWidget( new ProjectWindow( this ), ProjectWindow::DiagramMode, tr( "New Project" ) );
}

void MainWindow::slotCreateBasedBlock()
{

    tab_widget->addWidget( new BasedBlockWindow( BasedBlockWindow::CreateMode, tab_widget ), BasedBlockWindow::DiagramMode, tr( "New Block" ) );
}

void MainWindow::slotCreateSparqlBlock()
{
    tab_widget->addWidget( new SparqlBlockWindow( this ), SparqlBlockWindow::DiagramMode, tr( "New Sparql" ) );
}

void MainWindow::slotCurrentTabMode( int mode )
{
    tool_box->setDiagramItems( library->getBlocks( mode ) );
}
