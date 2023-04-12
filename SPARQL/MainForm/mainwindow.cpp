#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "projectwindow.h"
#include <QDebug>

#include <atomblocksettings.h>
#include <basedblockwindow.h>
#include <compositeblocksettings.h>
#include <compositeblockwindow.h>
#include <diagramexecutor.h>
#include <ioblocksettings.h>
#include <sparqlblockwindow.h>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    QCoreApplication::setApplicationName( tr( "FBQL Editor" ) );
    setWindowTitle( QCoreApplication::applicationName() );

    library = new BlocksLibrary();
    library->loadBlocksFromFiles( FOLDER_FOR_BLOCKS );
    library->addBlocks( AtomBlockSettings::GetBasedAtomBlocks() );
    library->addBlocks( IOBlockSettings::GetBasedIOBlocks() );

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
    connect( tab_widget, SIGNAL( newCurrentMode( int ) ),
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
    auto window = new ProjectWindow( item_menu, this );
    tab_widget->addWidget( window, tr( "New Project" ) );
}

void MainWindow::slotCreateBasedBlock()
{
    auto window = new BasedBlockWindow( BasedBlockWindow::CreateMode, tab_widget );
    tab_widget->addWidget( window, tr( "New Block" ) );
}

void MainWindow::slotCreateCompositeBlock()
{
    auto window = new CompositeBlockWindow( item_menu, this );
    tab_widget->addWidget( window, tr( "Composite Block" ) );
    connect( window, SIGNAL( blockCreated( DiagramItemSettings* ) ), this, SLOT( slotOnCreateBlock( DiagramItemSettings* ) ) );
}

void MainWindow::slotCreateSparqlBlock()
{
    auto window = new SparqlBlockWindow( item_menu, this );
    tab_widget->addWidget( window, tr( "New Sparql" ) );
    connect( window, SIGNAL( blockCreated( DiagramItemSettings* ) ), this, SLOT( slotOnCreateBlock( DiagramItemSettings* ) ) );
}

void MainWindow::slotOnCreateBlock( DiagramItemSettings* settings )
{
    library->addBlock( settings );
    tab_widget->resetBlocks();
}

void MainWindow::slotCurrentTabMode( int mode )
{
    tool_box->setDiagramItems( library->getBlocks( mode ) );
}

void MainWindow::slotOnClickedBlockExecutor()
{
    auto list_blocks = tab_widget->getBlocksForRun();
    if ( !list_blocks.empty() )
    {
        auto window = new DiagramExecutor( this );
        window->setDiagramItem( list_blocks );
        tab_widget->addWidget( window, "Exec" );
    }
}

void MainWindow::setItemMenu( QMenu* menu )
{
    item_menu = menu;
}

void MainWindow::slotOnDeleteItemOnScene()
{
    tab_widget->deleteItemOnScene();
}

void MainWindow::slotOnOpenProject()
{
    auto window = new ProjectWindow( item_menu, this );
    tab_widget->addWidget( window, tr( "Project" ) );
    tab_widget->openProject();
}

void MainWindow::slotOnSaveProject()
{
    tab_widget->saveProject();
}
