#include "diagramitemio.h"

DiagramItemIO::DiagramItemIO( QMenu* context_menu, QGraphicsItem* parent, IOBlockSettings* settings )
    : DiagramItem( context_menu, parent )
{
    if ( settings == nullptr )
        return;
    proxy_line_edit = new QGraphicsProxyWidget( this );
    line_edit = new QLineEdit();
    QRect rect( getStartPos().x() + 5, -12,
        getEndPos().x() - getStartPos().x() - 10, 25 );
    line_edit->setGeometry( rect );
    line_edit->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    line_edit->setStyleSheet( QString( "QLineEdit {"
                                       "border-width: 1px;"
                                       "border-style: solid;"
                                       "border-color: rgb(255, 255, 255);"
                                       "font-size: 12px;"
                                       "color: green;"
                                       "}" ) );

    proxy_line_edit->setWidget( line_edit );

    setText( settings->text );
    type_block = settings->type_block;
    support_add_item = settings->support_add_item;
    setZValue( settings->z_value );
    setPos( settings->pos );
    block_name = settings->block_name;

    if ( IOBlockSettings::Input == type_block )
    {
        setBrush( Qt::green );
    }
    else
    {
        setBrush( Qt::red );
    }
    setPolygon( settings->polygon );

    delete settings;
}

IOBlockSettings* DiagramItemIO::getSettings()
{
    IOBlockSettings* settings = new IOBlockSettings();
    settings->block_name = block_name;
    settings->pos = pos();
    settings->text = getText();
    settings->type_block = type_block;
    settings->support_add_item = support_add_item;
    settings->z_value = zValue();
    settings->polygon = polygon();
    return settings;
}

QString DiagramItemIO::getText()
{
    if ( line_edit == nullptr )
        return block_name;
    return line_edit->text();
};

void DiagramItemIO::setText( const QString& text )
{
    line_edit->setText( text );
}

void DiagramItemIO::setName( const QString& name )
{
    block_name = name;
}

QString DiagramItemIO::getName()
{
    return block_name;
}
