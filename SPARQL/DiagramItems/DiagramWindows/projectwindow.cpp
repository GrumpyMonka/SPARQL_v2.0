#include "projectwindow.h"

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
        if ( DiagramItemBased::Type == item->type() )
        {
            result.push_back( static_cast<DiagramItem*>( item ) );
        }
    }
    return result;
}
