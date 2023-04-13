#include "diagramitematom.h"

#include <QPainter>
#include <QPolygonF>

DiagramItemAtom::DiagramItemAtom( QMenu* context_menu, QGraphicsItem* parent, AtomBlockSettings* settings )
    : DiagramItem( context_menu, parent )
{
    proxy_line_edit = new QGraphicsProxyWidget( this );
    line_edit = new QLineEdit();
    QRect rect( getStartPos().x() + 5, -12,
        getEndPos().x() - getStartPos().x() - 10, 25 );
    line_edit->setGeometry( rect );
    line_edit->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

    proxy_line_edit->setWidget( line_edit );

    setItemPolygon( settings->polygon );
    setText( settings->text );
    type_block = settings->type_block;
    support_add_item = settings->support_add_item;
    setZValue( settings->z_value );
    setPos( settings->pos );
    if ( !settings->transparent_background )
    {
        setBrush( Qt::white );
    }
    setAllowResize( settings->support_resize );

    if ( DEFAULT_AREA == settings->type_block )
    {
        proxy_label = new QGraphicsProxyWidget( this );
        label = new QLabel( "Filter: " );
        label->setStyleSheet( "background-color: white;" );
        proxy_label->setWidget( label );
        proxy_label->setPos( getStartPos().x() + 5, getEndPos().y() - 25 );
        proxy_line_edit->setPos( getStartPos().x() + 55, getEndPos().y() - 28 );
        line_edit->setMinimumWidth( 300 );
        line_edit->setAlignment( Qt::AlignLeft );
    }
    else
    {
        line_edit->setVisible( settings->flag_text );
        line_edit->setStyleSheet( QString( "QLineEdit {"
                                           "border-width: 1px;"
                                           "border-style: solid;"
                                           "border-color: rgb(255, 255, 255);"
                                           "font-size: 12px;"
                                           "color: %1;"
                                           "}" )
                                      .arg( settings->color_text ) );
    }

    delete settings;
}

void DiagramItemAtom::setPosLineEdit( const QPointF& pos )
{
    proxy_line_edit->setPos( pos );
}

void DiagramItemAtom::setItemPolygon( QPolygonF& polygon )
{
    my_polygon = polygon;
    setPolygon( polygon );
}

AtomBlockSettings* DiagramItemAtom::getSettings()
{
    auto settings = new AtomBlockSettings();
    settings->polygon = polygon();
    settings->type_block = type_block;
    settings->flag_text = line_edit->isVisible();
    settings->pos = pos();
    if ( DEFAULT_AREA == type_block )
    {
        settings->filter = getText();
    }
    else
    {
        settings->text = getText();
    }
    return settings;
}

void DiagramItemAtom::setText( const QString& text )
{
    line_edit->setText( text );
}

QString DiagramItemAtom::getText()
{
    return line_edit->text();
}
/*
QPixmap DiagramItemAtom::image() const
{
    // if ( setting_->type_image == "self" )
    //{
    QPixmap pixmap( 150, 150 );
    pixmap.fill( Qt::transparent );
    QPainter painter( &pixmap );
    painter.setPen( QPen( Qt::black, 8 ) );
    painter.translate( 75, 75 );
    painter.drawPolyline( my_polygon );

    painter.setPen( QPen( Qt::black, 4 ) );
    // int index = 150 / setting_->block_name.size() * 0.75;
    // painter.setFont( QFont( "Consolas", index ) );
    // painter.drawText( -75, -75, 150, 150, Qt::AlignCenter, setting_->block_name );
    return pixmap;
}
*/
