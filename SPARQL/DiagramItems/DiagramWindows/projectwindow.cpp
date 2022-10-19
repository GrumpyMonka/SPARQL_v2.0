#include "projectwindow.h"

ProjectWindow::ProjectWindow( QWidget* parent )
    : SGraphicsView( parent )
{
    CreateSidePanel();
}

QWidget* ProjectWindow::AddCustomWidget()
{
    return new QWidget( this );
}

QWidget* ProjectWindow::AddCustomBotWidget()
{
    return new QWidget( this );
}
