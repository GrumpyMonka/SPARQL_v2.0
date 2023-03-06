#include "smenubar.h"

SMenuBar::SMenuBar( QWidget* parent )
    : QMenuBar( parent )
{
    CreateActions();
    CreateMenus();
}

void SMenuBar::CreateActions()
{
    connect( this, SIGNAL( createItemMenu( QMenu* ) ),
        parent(), SLOT( setItemMenu( QMenu* ) ) );

    toFrontAction = new QAction( QIcon( ":/Sources/images/bringtofront.png" ),
        tr( "Bring to &Front" ), this );
    toFrontAction->setShortcut( tr( "Ctrl+F" ) );
    toFrontAction->setStatusTip( tr( "Bring item to front" ) );
    // connect( toFrontAction, SIGNAL( triggered() ),
    //          parent(), SLOT( bringToFront() ) );

    sendBackAction = new QAction( QIcon( ":/Sources/images/sendtoback.png" ), tr( "Send to &Back" ), this );
    sendBackAction->setShortcut( tr( "Ctrl+T" ) );
    sendBackAction->setStatusTip( tr( "Send item to back" ) );
    // connect( sendBackAction, SIGNAL( triggered() ),
    //          parent(), SLOT( sendToBack() ) );

    reworkBlockAction = new QAction( QIcon( ":/Sources/images/repair.png" ), tr( "Rework Block" ) );
    reworkBlockAction->setShortcut( tr( "Ctrl+A" ) );
    reworkBlockAction->setStatusTip( tr( "Rework Block" ) );
    // connect( reworkBlockAction, SIGNAL( triggered() ),
    //          parent(), SLOT(slotReworkBlock()));

    createNewProjectAction = new QAction( QIcon( ":/Sources/images/new.png" ), tr( "New project" ), this );
    createNewProjectAction->setShortcut( tr( "Ctrl+N" ) );
    createNewProjectAction->setStatusTip( tr( "Create New Project" ) );
    connect( createNewProjectAction, SIGNAL( triggered() ),
        parent(), SLOT( slotCreateNewProject() ) );

    openProjectAction = new QAction( QIcon( ":/Sources/images/open.png" ), tr( "Open project" ), this );
    openProjectAction->setShortcut( tr( "Ctrl+O" ) );
    openProjectAction->setStatusTip( tr( "Open Project" ) );
    connect( openProjectAction, SIGNAL( triggered() ),
        parent(), SLOT( slotOnOpenProject() ) );

    saveProjectAction = new QAction( QIcon( ":/Sources/images/save.png" ), tr( "Save project" ), this );
    saveProjectAction->setShortcut( tr( "Ctrl+S" ) );
    saveProjectAction->setStatusTip( tr( "Save Project" ) );
    connect( saveProjectAction, SIGNAL( triggered() ),
        parent(), SLOT( slotOnSaveProject() ) );

    createBasedBlockAction = new QAction( QIcon( ":/Sources/images/based.png" ), tr( "Based" ), this );
    createBasedBlockAction->setShortcut( tr( "Ctrl+Q" ) );
    createBasedBlockAction->setStatusTip( tr( "New BasedBlock" ) );
    connect( createBasedBlockAction, SIGNAL( triggered() ),
        parent(), SLOT( slotCreateBasedBlock() ) );

    createCompositeBlockAction = new QAction( QIcon( ":/Sources/images/composite.png" ), tr( "Composite" ), this );
    createCompositeBlockAction->setShortcut( tr( "Ctrl+W" ) );
    createCompositeBlockAction->setStatusTip( tr( "New CompositeBlock" ) );
    connect( createCompositeBlockAction, SIGNAL( triggered() ),
        parent(), SLOT( slotCreateCompositeBlock() ) );

    createSparqlBlockAction = new QAction( QIcon( ":/Sources/images/SPARQL.png" ), tr( "SPARQL" ), this );
    createSparqlBlockAction->setShortcut( tr( "Ctrl+L" ) );
    createSparqlBlockAction->setStatusTip( tr( "New SPARQL Block" ) );
    connect( createSparqlBlockAction, SIGNAL( triggered() ),
        parent(), SLOT( slotCreateSparqlBlock() ) );

    runSceneScriptAction = new QAction( QIcon( ":/Sources/images/execution.png" ), tr( "Run" ), this );
    runSceneScriptAction->setShortcut( tr( "Ctrl+R" ) );
    runSceneScriptAction->setStatusTip( tr( "Run" ) );
    connect( runSceneScriptAction, SIGNAL( triggered() ),
        parent(), SLOT( slotOnClickedBlockExecutor() ) );

    deleteAction = new QAction( QIcon( ":/Sources/images/delete.png" ), tr( "&Delete" ), this );
    deleteAction->setShortcut( tr( "Delete" ) );
    deleteAction->setStatusTip( tr( "Delete item from diagram" ) );
    connect( deleteAction, SIGNAL( triggered() ),
        parent(), SLOT( slotOnDeleteItemOnScene() ) );

    exitAction = new QAction( tr( "E&xit" ), this );
    exitAction->setShortcuts( QKeySequence::Quit );
    exitAction->setStatusTip( tr( "Quit Scenediagram example" ) );
    // connect( exitAction, SIGNAL( triggered() ),
    //          parent(), SLOT( close() ) );

    aboutAction = new QAction( tr( "A&bout" ), this );
    aboutAction->setShortcut( tr( "F1" ) );
    // connect( aboutAction, SIGNAL( triggered() ),
    //          parent(), SLOT( about() ) );
}

void SMenuBar::CreateMenus()
{
    fileMenu = addMenu( tr( "&File" ) );
    fileMenu->addAction( createNewProjectAction );
    fileMenu->addAction( openProjectAction );
    fileMenu->addAction( saveProjectAction );
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );

    newItemMenu = addMenu( tr( "&Blocks" ) );
    newItemMenu->addAction( createBasedBlockAction );
    newItemMenu->addAction( createCompositeBlockAction );
    newItemMenu->addAction( createSparqlBlockAction );

    itemMenu = addMenu( tr( "&Item" ) );
    itemMenu->addAction( deleteAction );
    itemMenu->addSeparator();
    itemMenu->addAction( toFrontAction );
    itemMenu->addAction( sendBackAction );
    itemMenu->addSeparator();
    itemMenu->addAction( reworkBlockAction );
    itemMenu->addAction( runSceneScriptAction );
    emit createItemMenu( itemMenu );

    aboutMenu = addMenu( tr( "&Help" ) );
    aboutMenu->addAction( aboutAction );
}
