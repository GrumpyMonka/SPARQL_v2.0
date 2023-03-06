#include "projectwindow.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <diagramitembased.h>
#include <projectwindowsettings.h>

#include <diagramitembased.h>
#include <diagramitemcomposite.h>
#include <diagramitemio.h>

ProjectWindow::ProjectWindow( QMenu* context_menu, QWidget* parent )
    : SGraphicsView( context_menu, parent )
    , context_menu( context_menu )
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
            line_saver.text_start = ( static_cast<DiagramItemIO*>( arrow->startItem() ) )->getName();
        }

        if ( DiagramItem::IOItemType == arrow->endItem()->type() )
        {
            if ( arrow->startItem() == arrow->endItem()->parentItem() )
                continue;
            line_saver.end_block = blocks_list.indexOf( static_cast<DiagramItem*>( arrow->endItem()->parentItem() ) );
            line_saver.text_end = ( static_cast<DiagramItemIO*>( arrow->endItem() ) )->getName();
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
        blocks_list.push_back( DiagramItem::FactoryDiagramItem( context_menu, block ) );
        getScene()->addItem( blocks_list.back() );
        blocks_list.back()->setPos( block->pos );
    }

    for ( auto& line : settings.lines_list )
    {
        DiagramItem* startItem = blocks_list.at( line.start_block );
        DiagramItem* endItem = blocks_list.at( line.end_block );

        if ( DiagramItem::CompositeItemType == startItem->type() )
        {
            if ( !line.text_start.isEmpty() )
            {
                auto composite_item = static_cast<DiagramItemComposite*>( startItem );
                startItem = composite_item->getOutputBlock( line.text_start );
            }
            else
            {
                emit ERROR( "ProjectWindow::openProject() -> Line without text" );
            }
        }

        if ( DiagramItem::CompositeItemType == endItem->type() )
        {
            if ( !line.text_end.isEmpty() )
            {
                auto composite_item = static_cast<DiagramItemComposite*>( endItem );
                endItem = composite_item->getInputBlock( line.text_end );
            }
            else
            {
                emit ERROR( "ProjectWindow::openProject() -> Line without text" );
            }
        }

        getScene()->createArrow( startItem, endItem );
    }
}
