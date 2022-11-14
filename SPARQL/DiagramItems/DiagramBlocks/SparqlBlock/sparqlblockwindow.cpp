#include "sparqlblockwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QPushButton>

SparqlBlockWindow::SparqlBlockWindow( QWidget* parent )
    : SGraphicsView( parent )
{
    createSidePanel();
    setSettings( SparqlBlockSettings::CreateTemplateSparqlSettings() );
}

QWidget* SparqlBlockWindow::addCustomWidget()
{
    QWidget* widget = new QWidget( this );
    QGridLayout* grid_layout = new QGridLayout();
    widget->setLayout( grid_layout );

    QLabel* label = new QLabel( "Name block:", this );
    label->setMaximumHeight( 30 );
    line_name_block = new QLineEdit( this );
    line_name_block->setText( "Sparql" );

    grid_layout->addWidget( label, 0, 0 );
    grid_layout->addWidget( line_name_block, 1, 0 );
    widget->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );

    return widget;
}

QWidget* SparqlBlockWindow::addCustomBotWidget()
{
    QWidget* widget = new QWidget( this );
    QGridLayout* grid_layout = new QGridLayout();
    widget->setLayout( grid_layout );

    QPushButton* button_create_block = new QPushButton( tr( "Create" ), this );
    connect( button_create_block, SIGNAL( clicked() ), this, SLOT( slotOnCreateButtonClicked() ) );

    QPushButton* button_save_block = new QPushButton( tr( "Save" ), this );
    connect( button_save_block, SIGNAL( clicked() ), this, SLOT( slotOnSaveButtonClicked() ) );

    QPushButton* button_open_block = new QPushButton( tr( "Open" ), this );
    connect( button_open_block, SIGNAL( clicked() ), this, SLOT( slotOnOpenButtonClicked() ) );

    grid_layout->addWidget( button_create_block, 0, 0 );
    grid_layout->addWidget( button_save_block, 1, 0 );
    grid_layout->addWidget( button_open_block, 2, 0 );

    return widget;
}

void SparqlBlockWindow::slotOnCreateButtonClicked()
{
    emit blockCreated( getSettings() );
}

void SparqlBlockWindow::slotOnSaveButtonClicked()
{
    auto settings = getSettings();
    QJsonDocument json;
    json.setObject( settings->getJsonFromSetting() );
    saveFile( json.toJson() );
}

void SparqlBlockWindow::slotOnOpenButtonClicked()
{
    SparqlBlockSettings* settings = new SparqlBlockSettings();
    settings->setSettingFromString( openFile() );
    setSettings( settings );
}

void SparqlBlockWindow::setSettings( SparqlBlockSettings* settings )
{
    for ( const auto& area : settings->areas )
    {
        AtomBlockSettings* setting = new AtomBlockSettings();
        setting->polygon = area.polygon;
        setting->block_name = area.name;
        setting->type_block = DEFAULT_AREA;
        setting->transparent = true;
        DiagramItemAtom* item = new DiagramItemAtom( nullptr, setting );
        getScene()->addItem( item );
        item->setPos( area.pos );
    }

    QVector<DiagramItem*> blocks;
    for ( const auto& block : settings->blocks )
    {
        AtomBlockSettings* setting = new AtomBlockSettings();
        setting->text = block.text;
        setting->type_block = block.type;
        if ( block.type == DEFAULT_VAR )
        {
            setting->polygon.clear();
            setting->polygon << QPointF( -50, -50 )
                             << QPointF( 50, -50 )
                             << QPointF( 50, 50 )
                             << QPointF( -50, 50 )
                             << QPointF( -50, -50 );
        }
        DiagramItemAtom* item = new DiagramItemAtom( nullptr, setting );
        getScene()->addItem( item );
        item->setPos( block.pos );
        blocks.push_back( item );
    }

    for ( const auto& line : settings->lines )
    {
        auto arrow = getScene()->createArrow( blocks.at( line.startBlock ), blocks.at( line.endBlock ) );
        if ( line.text != "" )
            arrow->setText( line.text );
    }

    line_name_block->setText( settings->block_name );
    delete settings;
}

SparqlBlockSettings* SparqlBlockWindow::getSettings()
{
    SparqlBlockSettings* setting = new SparqlBlockSettings();
    QVector<DiagramItemAtom*> blocks_atom;
    QVector<QString> blocks_type;
    QVector<DiagramItemAtom*> blocks_area;
    QVector<DiagramArrow*> arrows;
    auto full_items = getScene()->items();
    for ( QGraphicsItem* item : full_items )
    {
        if ( DiagramArrow::Type == item->type() )
        {
            arrows.push_back( qgraphicsitem_cast<DiagramArrow*>( item ) );
        }
        else if ( DiagramItem::AtomItemType == item->type() )
        {
            DiagramItemAtom* diagram_item_atom = qgraphicsitem_cast<DiagramItemAtom*>( item );
            auto settings = diagram_item_atom->getSettings();
            auto ttemp = settings->type_block;
            if ( DEFAULT_AREA == settings->type_block )
            {
                blocks_area.push_back( diagram_item_atom );
            }
            else
            {
                blocks_atom.push_back( diagram_item_atom );
                blocks_type.push_back( settings->type_block );
            }
        }
    }

    for ( int i = 0; i < blocks_atom.size(); ++i )
    {
        /*      QString path;
              for ( const auto& area : blocks_area )
              {
                  if ( CheckCollisionArea( block, area ) )
                  {
                      if ( area->getArrows().size() )
                      {
                          DiagramArrow* arrow = area->getArrows()[0];
                          if ( arrow->endItem() == area )
                          {
                              path = arrow->getText();
                          }
                          else
                          {
                              path = "ORIGIN";
                          }
                      }*/
        setting->blocks.push_back( { blocks_atom.at( i )->getText(), blocks_atom.at( i )->pos(), "path", blocks_type.at( i ) } );
        /*            }
                }*/
    }

    for ( auto& arrow : arrows )
    {
        int p1 = blocks_atom.indexOf( qgraphicsitem_cast<DiagramItemAtom*>( arrow->startItem() ) );
        int p2 = blocks_atom.indexOf( qgraphicsitem_cast<DiagramItemAtom*>( arrow->endItem() ) );
        if ( -1 != p1 && -1 != p2 )
        {
            setting->lines.push_back( { p1, p2, arrow->getText() } );
        }
    }

    for ( const auto& area : blocks_area )
    {
        QString path;
        // DiagramArrow* arrow = area->getArrows()[0];
        // if ( arrow->endItem() == area )
        //{
        //     path = arrow->getText();
        // }
        // else
        //{
        path = "ORIGIN";
        //}
        setting->areas.push_back( { area->polygon(),
            area->pos(), path } );
    }

    setting->block_name = line_name_block->text();
    // setting->limit = limit_spin->value();
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

bool SparqlBlockWindow::CheckCollisionArea( DiagramItemAtom* item, DiagramItemAtom* area )
{
    QPointF item_pos = item->pos();
    if ( item_pos.x() > area->pos().x() + area->getStartPos().x()
        && item_pos.x() < area->pos().x() + area->getEndPos().x()
        && item_pos.y() > area->pos().y() + area->getStartPos().y()
        && item_pos.y() < area->pos().y() + area->getEndPos().y() )
    {
        return true;
    }
    return false;
}
