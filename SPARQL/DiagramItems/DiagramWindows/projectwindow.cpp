#include "projectwindow.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <diagramitembased.h>
#include <projectwindowsettings.h>

#include <diagramitembased.h>
#include <diagramitemcomposite.h>
#include <diagramitemio.h>

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

QVector<DiagramItem*> ProjectWindow::getDiagramItems()
{
    QVector<DiagramItem*> result;
    auto list = getScene()->items();
    for ( auto item : list )
    {
        if ( DiagramItem::CheckItemOnDiagramItem( item->type() ) )
        {
            result.push_back( static_cast<DiagramItem*>( item ) );
        }
    }
    return result;
}

QVector<DiagramArrow*> ProjectWindow::getDiagramArrows()
{
    QVector<DiagramArrow*> result;
    auto list = getScene()->items();
    for ( auto item : list )
    {
        if ( DiagramItem::DiagramArrowType == item->type() )
        {
            result.push_back( static_cast<DiagramArrow*>( item ) );
        }
    }
    return result;
}

void ProjectWindow::saveProject()
{
    ProjectWindowSettings settings;
    auto item_list = getDiagramItems();
    auto arrow_list = getDiagramArrows();

    QVector<DiagramItem*> blocks_list;
    for ( auto& item : item_list )
    {
        if ( DiagramItem::IOItemType != item->type() )
        {
            auto setting = item->getSettings();
            if ( setting != nullptr )
                settings.blocks_list.push_back( setting );
            blocks_list.push_back( item );
        }
    }

    for ( auto& arrow : arrow_list )
    {
        ProjectWindowSettings::LineSaver line_saver;
        line_saver.start_block = blocks_list.indexOf( arrow->startItem() );
        line_saver.end_block = blocks_list.indexOf( arrow->endItem() );

        if ( DiagramItem::IOItemType == arrow->startItem()->type() )
        {
            if ( arrow->endItem() == arrow->startItem()->parentItem() )
                continue;
            line_saver.start_block = blocks_list.indexOf( static_cast<DiagramItem*>( arrow->startItem()->parentItem() ) );
            line_saver.text = ( static_cast<DiagramItemIO*>( arrow->startItem() ) )->block_name;
        }

        if ( DiagramItem::IOItemType == arrow->endItem()->type() )
        {
            if ( arrow->startItem() == arrow->endItem()->parentItem() )
                continue;
            line_saver.end_block = blocks_list.indexOf( static_cast<DiagramItem*>( arrow->endItem()->parentItem() ) );
            line_saver.text = ( static_cast<DiagramItemIO*>( arrow->endItem() ) )->block_name;
        }

        settings.lines_list.push_back( line_saver );
    }

    QJsonDocument json;
    json.setObject( settings.getJsonFromSetting() );
    saveFile( json.toJson() );
}

void ProjectWindow::openProject()
{
    ProjectWindowSettings settings;
    settings.setSettingFromString( openFile() );

    QVector<DiagramItem*> blocks_list;
    for ( auto& block : settings.blocks_list )
    {
        blocks_list.push_back( DiagramItem::FactoryDiagramItem( nullptr, block ) );
        getScene()->addItem( blocks_list.back() );
        blocks_list.back()->setPos( block->pos );
    }

    for ( auto& line : settings.lines_list )
    {
        DiagramItem* startItem = blocks_list.at( line.start_block );
        DiagramItem* endItem = blocks_list.at( line.end_block );

        if ( line.text != "" )
        {
            if ( DiagramItem::CompositeItemType == startItem->type() )
            {
                auto composite_item = static_cast<DiagramItemComposite*>( startItem );
                startItem = composite_item->getOutputBlock( line.text );
            }

            if ( DiagramItem::CompositeItemType == endItem->type() )
            {
                auto composite_item = static_cast<DiagramItemComposite*>( endItem );
                endItem = composite_item->getInputBlock( line.text );
            }
        }

        getScene()->createArrow( startItem, endItem );
    }
}
