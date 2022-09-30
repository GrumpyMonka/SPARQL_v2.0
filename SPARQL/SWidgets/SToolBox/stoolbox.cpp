#include "stoolbox.h"

SToolBox::SToolBox( QWidget *parent )
    : QToolBox( parent )
{
    createToolButtonGroup();
}
#include <QPushButton>

void SToolBox::createToolButtonGroup()
{

    buttonGroup = new QButtonGroup( this );
    buttonGroup->setExclusive( false );

    QWidget* buttonGroupWidget = new QWidget( this );
    buttonLayout = new QGridLayout( buttonGroupWidget );
    buttonGroupWidget->setLayout( buttonLayout );

    setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Ignored ) );
    setMinimumWidth( buttonGroupWidget->sizeHint().width() );
    addItem( buttonGroupWidget, tr( "Blocks" ) );
}


