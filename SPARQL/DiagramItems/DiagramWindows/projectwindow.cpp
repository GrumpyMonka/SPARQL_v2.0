#include "projectwindow.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <diagramitembased.h>
#include <projectwindowsettings.h>

#include <diagramitembased.h>

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

    for ( auto& item : item_list )
    {
        settings.blocks_list.push_back( item->getSettings() );
    }

    for ( auto& arrow : arrow_list )
    {
        settings.lines_list.push_back( { item_list.indexOf( arrow->startItem() ),
            item_list.indexOf( arrow->endItem() ),
            arrow->getText() } );
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
        auto arrow = getScene()->createArrow( blocks_list.at( line.start_block ),
            blocks_list.at( line.end_block ) );
        arrow->setText( line.text );
    }
}
