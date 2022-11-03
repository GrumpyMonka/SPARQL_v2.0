#include "sparqlblockwindow.h"

#include <QPushButton>

#include <diagramitematom.h>

SparqlBlockWindow::SparqlBlockWindow( QWidget* parent )
    : SGraphicsView( parent )
{
    createSidePanel();
    setSettings();
}

QWidget* SparqlBlockWindow::addCustomWidget()
{
    return new QWidget( this );
}

QWidget* SparqlBlockWindow::addCustomBotWidget()
{
    QWidget* widget = new QWidget( this );
    QGridLayout* grid_layout = new QGridLayout();
    widget->setLayout( grid_layout );

    QPushButton* button_create_block = new QPushButton( tr( "Create" ), this );
    grid_layout->addWidget( button_create_block, 0, 0 );

    return widget;
}

void SparqlBlockWindow::setSettings( const SparqlBlockSettings& settings )
{
}

SparqlBlockSettings* SparqlBlockWindow::getSettings()
{
    SparqlBlockSettings* setting = new SparqlBlockSettings();
    QVector<DiagramItemAtom*> blocks_atom;
    QVector<DiagramItemAtom*> blocks_area;
    QVector<DiagramArrow*> arrows;
    auto full_items = getScene()->items();
    for ( QGraphicsItem* item : full_items )
    {
        if ( DiagramArrow::Type == item->type() )
        {
            arrows.push_back( qgraphicsitem_cast<DiagramArrow*>( item ) );
        }
        else if ( DiagramItemAtom::Type == item->type() )
        {
            DiagramItemAtom* item = qgraphicsitem_cast<DiagramItemAtom*>( item );
            auto settings = item->getSettings();
            if ( settings->type_block )
                blocks_atom.push_back();
        }
    }

    for ( auto& block : blocks_atom )
    {
        QString path;
        for ( const auto& item : Areas )
        {
            if ( CheckCollisionArea( block, item ) )
            {
                if ( item->getArrows().size() )
                {
                    Arrow* arrow = item->getArrows()[0];
                    if ( arrow->endItem() == item )
                    {
                        path = arrow->getText();
                    }
                    else
                    {
                        path = "ORIGIN";
                    }
                }
                setting->blocks.push_back( { block->getText(), block->pos(), path } );
            }
        }
    }

    for ( auto& arrow : arrows )
    {
        int p1 = blocks.indexOf( qgraphicsitem_cast<DiagramSparqlAtom*>( arrow->startItem() ) );
        int p2 = blocks.indexOf( qgraphicsitem_cast<DiagramSparqlAtom*>( arrow->endItem() ) );
        if ( -1 != p1 && -1 != p2 )
        {
            setting->lines.push_back( { p1, p2, arrow->getText() } );
        }
    }

    for ( const auto& area : Areas )
    {
        QString path;
        Arrow* arrow = area->getArrows()[0];
        if ( arrow->endItem() == area )
        {
            path = arrow->getText();
        }
        else
        {
            path = "ORIGIN";
        }
        setting->areas.push_back( { QPointF( area->getEndPos().x() - area->getStartPos().x(),
                                        area->getEndPos().y() - area->getStartPos().y() ),
            area->pos(), path } );
    }

    setting->name = name_line_edit->text();
    setting->limit = limit_spin->value();
    return setting;
}

void SparqlBlockWindow::createDefaultcScene()
{
    clearScene();
    auto atom_list = AtomBlockSettings::GetBasedAtomBlocks();
}

void SparqlBlockWindow::clearScene()
{
    getScene()->clear();
}
