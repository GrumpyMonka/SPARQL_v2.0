#include "sparqlblockwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QPushButton>

#include <diagramarrow.h>

SparqlBlockWindow::SparqlBlockWindow( QMenu* context_menu, QWidget* parent )
    : SGraphicsView( context_menu, parent )
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

    QPushButton* button_write_query = new QPushButton( tr( "Query" ), this );
    connect( button_write_query, SIGNAL( clicked() ), this, SLOT( slotCustom() ) );

    grid_layout->addWidget( button_write_query, 0, 0 );
    grid_layout->addWidget( button_create_block, 1, 0 );
    grid_layout->addWidget( button_save_block, 2, 0 );
    grid_layout->addWidget( button_open_block, 3, 0 );

    return widget;
}

void SparqlBlockWindow::slotCustom()
{
    if ( nullptr == text_edit )
    {
        QGridLayout* grid_widget = new QGridLayout();
        getWidgetOnGraphicsView()->setLayout( grid_widget );
        text_edit = new QTextEdit();
        text_edit->setStyleSheet( "color: black;" );
        auto settings = getSettings();
        text_edit->setText( settings->getQuery() );
        delete settings;
        getWidgetOnGraphicsView()->setGeometry( QRect( ( int )( width() / 2 - 600 ), ( int )( height() / 2 - 500 ), 1200, 1000 ) );
        grid_widget->addWidget( text_edit, 0, 0 );
    }
    else if ( text_edit->isHidden() )

    {
        text_edit->show();
        getWidgetOnGraphicsView()->setGeometry( QRect( ( int )( width() / 2 - 600 ), ( int )( height() / 2 - 500 ), 1200, 1000 ) );
        auto settings = getSettings();
        text_edit->setText( settings->getQuery() );
        delete settings;
    }
    else
    {
        getWidgetOnGraphicsView()->setGeometry( QRect( 0, 0, 0, 0 ) );
        text_edit->hide();
    }
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
    delete settings;
}

void SparqlBlockWindow::slotOnOpenButtonClicked()
{
    SparqlBlockSettings* settings = new SparqlBlockSettings();
    settings->setSettingFromString( openFile() );
    setSettings( settings );
}

void SparqlBlockWindow::setSettings( SparqlBlockSettings* settings )
{
    getScene()->clear();
    QVector<DiagramItemAtom*> areas_items;
    for ( const auto& area : settings->areas )
    {
        DiagramItemAtom* area_item = new DiagramItemAtom( nullptr, nullptr, area.settings );
        getScene()->addItem( area_item );
        areas_items.push_back( area_item );

        QVector<DiagramItem*> blocks;
        for ( const auto& block : area.blocks )
        {
            DiagramItemAtom* item = new DiagramItemAtom( nullptr, area_item, block );
            blocks.push_back( item );
        }

        for ( const auto& line : area.lines )
        {
            auto arrow = getScene()->createArrow( blocks.at( line.start_block ), blocks.at( line.end_block ) );
            if ( line.text != "" )
                arrow->setText( line.text );
        }
    }

    for ( const auto& line : settings->lines )
    {
        auto arrow = getScene()->createArrow( areas_items.at( line.start_block ), areas_items.at( line.end_block ) );
        if ( line.text != "" )
            arrow->setText( line.text );
    }

    line_name_block->setText( settings->block_name );
    delete settings;
}

SparqlBlockSettings* SparqlBlockWindow::getSettings()
{
    SparqlBlockSettings* settings = new SparqlBlockSettings();
    settings->block_name = line_name_block->text();
    if ( nullptr != text_edit && text_edit->isVisible() )
    {
        settings->query = text_edit->toPlainText();
        return settings;
    }

    QMap<DiagramItemAtom*, QVector<DiagramItemAtom*>> blocks_area;
    DiagramItemAtom* start_area;
    auto full_items = getScene()->items();

    // check areas
    for ( QGraphicsItem* item : full_items )
    {
        if ( DiagramItem::AtomItemType == item->type() )
        {
            auto atom_item = static_cast<DiagramItemAtom*>( item );
            if ( DEFAULT_AREA == atom_item->getSettings()->type_block )
            {
                blocks_area.insert( atom_item, {} );
            }
        }
    }

    // check blocks
    for ( QGraphicsItem* item : full_items )
    {
        if ( DiagramItem::AtomItemType == item->type() )
        {
            auto atom_item = static_cast<DiagramItemAtom*>( item );
            if ( ( DEFAULT_VAR == atom_item->getSettings()->type_block
                     || DEFAULT_VALUE == atom_item->getSettings()->type_block )
                && nullptr != atom_item->parentItem()
                && DiagramItem::AtomItemType == atom_item->parentItem()->type() )
            {
                auto parent_item = static_cast<DiagramItemAtom*>( atom_item->parentItem() );
                blocks_area[parent_item].push_back( atom_item );
            }
        }
    }

    // find start
    bool flag_find = false;
    for ( auto& area : blocks_area.keys() )
    {
        auto arrows = area->getEndArrows();
        if ( 0 == arrows.size() )
        {
            if ( flag_find )
            {
                emit ERROR( "Sparql wrong format! Find > 1 start area!" );
                return {};
            }
            else
            {
                start_area = area;
                flag_find = true;
            }
        }
    }

    // generatesettings
    auto area_list = blocks_area.keys();
    for ( auto& area : area_list )
    {
        SparqlBlockSettings::AreaSaver area_saver;
        area_saver.settings = area->getSettings();

        // blocks
        auto blocks_item = blocks_area[area];
        for ( auto& item : blocks_item )
        {
            area_saver.blocks.push_back( item->getSettings() );
            auto arrows = item->getStartArrows();
            for ( auto& arrow : arrows )
            {
                SparqlBlockSettings::LineSaver line_saver;
                line_saver.text = arrow->getText();
                line_saver.start_block = blocks_item.indexOf( item );
                if ( DiagramItem::AtomItemType == arrow->endItem()->type() )
                {
                    line_saver.end_block = blocks_item.indexOf( static_cast<DiagramItemAtom*>( arrow->endItem() ) );
                }
                else
                {
                    emit ERROR( "Sparql wrong format! Unknown type blocks!" );
                }
                area_saver.lines.push_back( line_saver );
            }
        }

        settings->areas.push_back( area_saver );
        auto arrows = area->getStartArrows();
        for ( auto& arrow : arrows )
        {
            SparqlBlockSettings::LineSaver line_saver;
            line_saver.text = arrow->getText();
            line_saver.start_block = area_list.indexOf( area );
            if ( DiagramItem::AtomItemType == arrow->endItem()->type() )
            {
                line_saver.end_block = area_list.indexOf( static_cast<DiagramItemAtom*>( arrow->endItem() ) );
            }
            else
            {
                emit ERROR( "Sparql wrong format! Unknown type blocks!" );
            }
            settings->lines.push_back( line_saver );
        }
    }

    settings->start_area = area_list.indexOf( start_area );
    return settings;
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
