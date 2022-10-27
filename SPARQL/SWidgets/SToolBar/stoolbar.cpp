#include "stoolbar.h"

#include <QToolButton>
#include <diagramscene.h>

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

    QToolButton* pointer_button = new QToolButton( this );
    pointer_button->setCheckable( true );
    pointer_button->setIcon( QIcon( ":/Sources/images/pointer.png" ) );
    QToolButton* line_pointer_button = new QToolButton( this );
    line_pointer_button->setCheckable( true );
    line_pointer_button->setIcon( QIcon( ":/Sources/images/linepointer.png" ) );

    pointer_type_group = new QButtonGroup( this );
    pointer_type_group->addButton( pointer_button, int( DiagramScene::MoveItem ) );
    pointer_type_group->addButton( line_pointer_button, int( DiagramScene::InsertArrow ) );
    connect( pointer_type_group, SIGNAL( buttonClicked( int ) ),
        this, SIGNAL( setNewDiagramSceneMode( int ) ) );

    setSettings( pointerButtonChecked );

    addSeparator();
    addWidget( pointer_button );
    addWidget( line_pointer_button );
}

void SToolBar::setSettings( Settings setting )
{
    auto list_button = pointer_type_group->buttons();
    for ( int i = 0; i < list_button.size(); ++i )
    {
        if ( setting != i )
        {
            list_button.at( i )->setChecked( false );
            continue;
        }
        list_button.at( i )->setChecked( true );
    }
}
