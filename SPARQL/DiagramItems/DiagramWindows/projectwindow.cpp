#include "projectwindow.h"

ProjectWindow::ProjectWindow( QWidget* parent )
    : SGraphicsView( parent )
{
    createSidePanel();
}

QWidget* ProjectWindow::addCustomWidget()
{
    return new QWidget( this );
}

QWidget* ProjectWindow::addCustomBotWidget()
{
    return new QWidget( this );
}
