#include "stoolbar.h"

#include <diagramscene.h>
#include <QToolButton>

SToolBar::SToolBar( SMenuBar& menubar, QWidget* parent )
    : QToolBar( parent )
{
    CreateToolBar( menubar );
}

void SToolBar::CreateToolBar( SMenuBar& menubar )
{
    addAction( menubar.createNewProjectAction );
    addAction( menubar.openProjectAction );
    addAction( menubar.saveProjectAction );

    addSeparator();
    addAction( menubar.createBasedBlockAction );
    addAction( menubar.createCompositeBlockAction );
    addAction( menubar.createSparqlBlockAction );

    addSeparator();
    addAction( menubar.deleteAction );
    addAction( menubar.toFrontAction );
    addAction( menubar.sendBackAction );
    addAction( menubar.reworkBlockAction );
    addAction( menubar.runSceneScriptAction );

    QToolButton* pointerButton = new QToolButton( this );
    pointerButton->setCheckable( true );
    pointerButton->setIcon( QIcon( ":/Sources/images/pointer.png" ) );
    QToolButton* linePointerButton = new QToolButton( this );
    linePointerButton->setCheckable( true );
    linePointerButton->setIcon( QIcon( ":/Sources/images/linepointer.png" ) );

    pointerTypeGroup = new QButtonGroup( this );
    pointerTypeGroup->addButton( pointerButton, int( DiagramScene::MoveItem ) );
    pointerTypeGroup->addButton( linePointerButton, int( DiagramScene::InsertLine ) );
    connect( pointerTypeGroup, SIGNAL( buttonClicked( int ) ),
             this, SIGNAL( setNewDiagramSceneMode( int ) ) );

    setSettings( pointerButtonChecked );

    addSeparator();
    addWidget( pointerButton );
    addWidget( linePointerButton );
}

void SToolBar::setSettings( Settings setting )
{
    auto listButton = pointerTypeGroup->buttons();
    for ( int i = 0; i < listButton.size(); ++i )
    {
        if ( setting != i )
        {
            listButton.at( i )->setChecked( false );
            continue;
        }
        listButton.at( i )->setChecked( true );
    }
}
