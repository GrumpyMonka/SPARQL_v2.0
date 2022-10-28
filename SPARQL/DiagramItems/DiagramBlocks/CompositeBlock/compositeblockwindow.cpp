#include "compositeblockwindow.h"

CompositeBlockWindow::CompositeBlockWindow( QWidget* parent )
    : SGraphicsView( parent )
{
    createSidePanel();
}

QWidget* CompositeBlockWindow::addCustomWidget()
{
    return new QWidget( this );
}

QWidget* CompositeBlockWindow::addCustomBotWidget()
{
    return new QWidget( this );
}
