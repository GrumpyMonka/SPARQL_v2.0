#include "diagramitembased.h"

DiagramItemBased::DiagramItemBased( QMenu* contextMenu,
    BasedBlockSettings* setting, QGraphicsItem* parent )
    : DiagramItem( contextMenu, parent )
{
    setBrush( Qt::white );

    name = new QGraphicsTextItem( this );
    label = new QGraphicsTextItem( this );
    input_data = new QGraphicsTextItem( this );
    output_data = new QGraphicsTextItem( this );

    proxy_line_edit = new QGraphicsProxyWidget( this );
    proxy_picture = new QGraphicsProxyWidget( this );

    line_edit = new QLineEdit();
    QRect rect( getStartPos().x() + 4, getEndPos().y() - 30,
        getEndPos().x() - getStartPos().x() - 8, 25 );
    line_edit->setGeometry( rect );
    line_edit->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    line_edit->setStyleSheet( "font-size: 12px; color: blue;" );

    proxy_line_edit->setWidget( line_edit );

    name->setPos( getStartPos() );
    label->setPos( getStartPos().x(), getEndPos().y() - 50 );
    input_data->setPos( getStartPos().x(), getStartPos().y() - 25 );
    output_data->setPos( getStartPos().x(), getEndPos().y() - 5 );

    picture = new QLabel();
    picture->setGeometry( getEndPos().x() - 32, getStartPos().y() + 2, 30, 30 );
    proxy_picture->setWidget( picture );

    setSetting( setting );
}

void DiagramItemBased::setSetting( BasedBlockSettings* setting )
{
    setting_ = setting;
    name->setHtml( "<span style=\"font-size: 15px; text-decoration: underline;\">" + setting->block_name + "</span>" );
    if ( setting_->label )
    {
        label->setHtml( "<span style=\"font-size: 12px;\">" + setting->label_text + "</span>" );
    }
    else
    {
        label->hide();
    }

    if ( setting->line_edit )
    {
        line_edit->setText( setting->line_edit_text );
    }
    else
    {
        line_edit->hide();
    }
    picture->setPixmap( setting->pixmap.scaled( 30, 30 ) );
}
/*
QPixmap DiagramItemBased::image() const
{
    if ( setting_->type_image == "self" )
    {
        QPixmap pixmap( 150, 150 );
        pixmap.fill( Qt::transparent );
        QPainter painter( &pixmap );
        painter.setPen( QPen( Qt::black, 8 ) );
        painter.translate( 75, 75 );
        painter.drawPolyline( my_polygon );

        painter.setPen( QPen( Qt::black, 4 ) );
        int index = 150 / setting_->block_name.size() * 0.75;
        painter.setFont( QFont( "Consolas", index ) );
        painter.drawText( -75, -75, 150, 150, Qt::AlignCenter, setting_->block_name );
        return pixmap;
    }
    return setting_->pixmap;
}
*/
QString DiagramItemBased::getName()
{
    return setting_->block_name;
}

BasedBlockSettings* DiagramItemBased::getSetting()
{
    return setting_;
}

QString DiagramItemBased::GetInputData()
{
    return line_edit->text();
}

void DiagramItemBased::setDrawPicture( bool flag )
{
    picture->setVisible( flag );
}

void DiagramItemBased::setInputData( QString vec )
{
    QString result = "<span style=\"color: green; font-size: 15px;\">";
    result += vec;
    result += "</span>";
    input_data->setHtml( result );
}

void DiagramItemBased::setOutputData( QString vec )
{
    QString result = "<span style=\"color: red; font-size: 15px\">";
    result += vec;
    result += "</span>";
    output_data->setHtml( result );
}
