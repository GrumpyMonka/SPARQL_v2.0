#include "diagramitemsparql.h"

DiagramItemSparql::DiagramItemSparql( QMenu* contextMenu, QGraphicsItem* parent, SparqlBlockSettings* setting )
    : DiagramItem( contextMenu, parent )
{
    setBrush( Qt::green );

    name = new QGraphicsTextItem( this );
    name->setHtml( "<span style=\"font-size: 15px; text-decoration: underline;\">" + setting->block_name + "</span>" );
    name->setPos( getStartPos() );

    picture = new QLabel();
    picture->setGeometry( getEndPos().x() - 32, getStartPos().y() + 2, 30, 30 );

    proxy_picture = new QGraphicsProxyWidget( this );
    proxy_picture->setWidget( picture );
    picture->setPixmap( QPixmap( ":/Sources/images/sparqlicon.jpg" ).scaled( 30, 30 ) );
    settings = setting;
}

SparqlBlockSettings* DiagramItemSparql::getSettings()
{
    settings->pos = pos();
    return settings;
}

QString DiagramItemSparql::getScript()
{
    return settings->getScript();
}
