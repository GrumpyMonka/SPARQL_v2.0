#include "sparqlblockwindow.h"

SparqlBlockWindow::SparqlBlockWindow( QWidget* parent )
    : SGraphicsView( parent )
{
    createSidePanel();
}

QWidget* SparqlBlockWindow::addCustomWidget()
{
    return new QWidget( this );
}

QWidget* SparqlBlockWindow::addCustomBotWidget()
{
    return new QWidget( this );
}
