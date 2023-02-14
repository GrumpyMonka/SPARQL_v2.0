#include "diagramitemcomposite.h"

#include <diagramarrow.h>
#include <diagramscene.h>

DiagramItemComposite::DiagramItemComposite( QMenu* contextMenu, QGraphicsItem* parent, CompositeBlockSettings* setting )
    : DiagramItem( contextMenu, parent )
{
    setBrush( QColor( 102, 205, 170 ) );

    setSettings( setting );
    setAllowLineToChild( true );
}

CompositeBlockSettings* DiagramItemComposite::getSettings()
{
    settings->pos = pos();
    return settings;
}

QString DiagramItemComposite::getScript()
{
    return settings->getScript();
}

void DiagramItemComposite::setSettings( CompositeBlockSettings* new_settings )
{
    int h_header = 40;
    int h_node = 30;
    int height = h_header + qMax( new_settings->input_names.size(), new_settings->output_names.size() ) * h_node;
    int widht = 150;

    QPolygonF polygonF;
    polygonF << QPointF( -widht / 2, -height / 2 + h_header )
             << QPointF( -widht / 2, -height / 2 )
             << QPointF( widht / 2, -height / 2 )
             << QPointF( widht / 2, height / 2 )
             << QPointF( -widht / 2, height / 2 )
             << QPointF( -widht / 2, -height / 2 + h_header )
             << QPointF( widht / 2, -height / 2 + h_header );
    setPolygon( polygonF );

    QGraphicsTextItem* name = new QGraphicsTextItem( this );
    name->setHtml( "<span style=\"font-size: 15px; text-decoration: underline;\">" + new_settings->block_name + "</span>" );
    name->setPos( -widht / 2 + 2, -height / 2 + 2 );

    QLabel* picture = new QLabel();
    picture->setGeometry( widht / 2 - 32, -height / 2 + 2, 30, 30 );

    QGraphicsProxyWidget* proxy_picture = new QGraphicsProxyWidget( this );
    proxy_picture->setWidget( picture );
    picture->setPixmap( QPixmap( ":/Sources/images/chip.png" ).scaled( 30, 30 ) );

    QPolygonF polygon_io;
    polygon_io << QPointF( -10, -4 )
               << QPointF( 10, -4 )
               << QPointF( 10, 4 )
               << QPointF( -10, 4 )
               << QPointF( -10, -4 );

    for ( int i = 0; i < new_settings->input_names.size(); ++i )
    {
        DiagramItemIO* input_block = new DiagramItemIO( nullptr, this );
        input_block->setPolygon( polygon_io );
        input_block->setBrush( Qt::black );
        input_block->setPos( -widht / 2 - 10, h_header - height / 2 + i * h_node + h_node / 2 );
        input_block->setName( new_settings->input_names[i] );
        input_block->setTypeBlock( IOBlockSettings::Input );
        input_block->setFlag( QGraphicsItem::ItemIsMovable, false );

        DiagramArrow* arrow = new DiagramArrow( input_block, this );
        input_block->addArrow( arrow );
        this->addArrow( arrow );
        arrow->setZValue( qMin( input_block->zValue(), this->zValue() ) - 1 );

        QGraphicsTextItem* input_name = new QGraphicsTextItem( new_settings->input_names[i], this );
        input_name->setPos( input_block->pos().x() + 10, input_block->pos().y() - 15 );
        input_blocks.push_back( input_block );
    }

    for ( int i = 0; i < new_settings->output_names.size(); ++i )
    {
        DiagramItemIO* output_block = new DiagramItemIO( nullptr, this );
        output_block->setPolygon( polygon_io );
        output_block->setBrush( Qt::black );
        output_block->setPos( widht / 2 + 10, h_header - height / 2 + i * h_node + h_node / 2 );
        output_block->setName( new_settings->output_names[i] );
        output_block->setTypeBlock( IOBlockSettings::Output );
        output_block->setFlag( QGraphicsItem::ItemIsMovable, false );

        DiagramArrow* arrow = new DiagramArrow( this, output_block );
        output_block->addArrow( arrow );
        this->addArrow( arrow );
        arrow->setZValue( qMin( output_block->zValue(), this->zValue() ) - 1 );

        QGraphicsTextItem* output_name = new QGraphicsTextItem( new_settings->output_names[i], this );
        output_name->setPos( output_block->pos().x() - 70, output_block->pos().y() - 15 );
        output_blocks.push_back( output_block );
    }

    settings = new_settings;
}

DiagramItemIO* DiagramItemComposite::getInputBlock( QString& name )
{
    for ( const auto& input : input_blocks )
    {
        if ( input->getSettings()->block_name == name )
        {
            return input;
        }
    }
}

DiagramItemIO* DiagramItemComposite::getOutputBlock( QString& name )
{
    for ( const auto& output : output_blocks )
    {
        if ( output->getSettings()->block_name == name )
        {
            return output;
        }
    }
}

QVector<DiagramArrow*> DiagramItemComposite::getArrowForInputBlock( QString& name )
{
    return getInputBlock( name )->getEndArrows();
}

QVector<DiagramArrow*> DiagramItemComposite::getArrowForOutputBlock( QString& name )
{
    return getOutputBlock( name )->getStartArrows();
}
