#include "diagramitemcomposite.h"

DiagramItemComposite::DiagramItemComposite( QMenu* contextMenu, QGraphicsItem* parent, CompositeBlockSettings* setting )
    : DiagramItem( contextMenu, parent )
{
    setBrush( Qt::blue );

    setSettings( setting );
}

CompositeBlockSettings* DiagramItemComposite::getSettings()
{
    return settings;
}

QString DiagramItemComposite::getScript()
{
}

void DiagramItemComposite::setSettings( CompositeBlockSettings* setting )
{
    int height = 30 + qMax( setting->input_names.size(), settings->output_names.size() ) * 20;
    int widht = 150;

    QPolygonF polygonF;
    polygonF << QPointF( -widht / 2, -height / 2 + 30 )
             << QPointF( -widht / 2, -height / 2 )
             << QPointF( widht / 2, -height / 2 )
             << QPointF( widht / 2, height / 2 )
             << QPointF( -widht / 2, height / 2 )
             << QPointF( -widht / 2, -height / 2 + 30 )
             << QPointF( widht / 2, -height / 2 + 30 );
    setPolygon( polygonF );

    QGraphicsTextItem* name = new QGraphicsTextItem( this );
    name->setHtml( "<span style=\"font-size: 15px; text-decoration: underline;\">" + setting->block_name + "</span>" );
    name->setPos( getStartPos() );

    QLabel* picture = new QLabel();
    picture->setGeometry( getEndPos().x() - 32, getStartPos().y() + 2, 30, 30 );

    QGraphicsProxyWidget* proxy_picture = new QGraphicsProxyWidget( this );
    proxy_picture->setWidget( picture );
    picture->setPixmap( QPixmap( ":/Sources/images/sparqlicon.jpg" ).scaled( 30, 30 ) );

    QPolygonF polygon_io;
    polygon_io << QPointF( -10, -5 )
               << QPointF( 10, -5 )
               << QPointF( 10, 5 )
               << QPointF( -10, 5 )
               << QPointF( -10, -5 );

    for ( int i = 0; i < setting->input_names.size(); ++i )
    {
        DiagramItemIO* input_block = new DiagramItemIO( nullptr, this );
        input_block->setPolygon( polygon_io );
        input_block->setBrush( Qt::black );
        input_block->setPos( widht / 2 - 10, 30 - height + i * 20 + 10 );
        QGraphicsTextItem* input_name = new QGraphicsTextItem( setting->input_names[i], this );
        input_name->setPos( input_block->pos().x() + 10, input_block->pos().y() );
    }

    for ( int i = 0; i < setting->output_names.size(); ++i )
    {
        DiagramItemIO* output_block = new DiagramItemIO( nullptr, this );
        output_block->setPolygon( polygon_io );
        output_block->setBrush( Qt::black );
        output_block->setPos( widht / 2 - 10, 30 - height + i * 20 + 10 );
        QGraphicsTextItem* output_name = new QGraphicsTextItem( setting->output_names[i], this );
        output_name->setPos( output_block->pos().x() + 10, output_block->pos().y() );
    }

    settings = setting;
}
