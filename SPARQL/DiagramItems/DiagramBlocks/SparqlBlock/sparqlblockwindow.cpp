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

void SparqlBlockWindow::setSettings( SparqlBlockSettings* settings )
{
}

void SparqlBlockWindow::createDefaultScene()
{
    clearScene();
    auto atom_list = AtomBlockSettings::GetBasedAtomBlocks();
}

void SparqlBlockWindow::clearScene()
{
    getScene()->clear();
}
