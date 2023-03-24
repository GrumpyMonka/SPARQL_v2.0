#ifndef DIAGRAMARROW_H
#define DIAGRAMARROW_H

#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QLineEdit>

#include <diagramitem.h>

class DiagramArrow : public QGraphicsLineItem
{
public:
    int type() const override { return DiagramItem::DiagramArrowType; }

    DiagramArrow( DiagramItem* startItem, DiagramItem* endItem,
        QGraphicsItem* parent = 0 );
    ~DiagramArrow();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor( const QColor& color ) { myColor = color; }
    DiagramItem* startItem() const { return myStartItem; }
    DiagramItem* endItem() const { return myEndItem; }
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;
    void setText( const QString& str );
    QString getText();
    void updatePosition();
    void setVisibleText( bool flag );

protected:
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 ) override;

private:
    DiagramItem* myStartItem;
    DiagramItem* myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
    QLineEdit* line_edit;
    QGraphicsProxyWidget* proxy_widget;
};

#endif // DIAGRAMARROW_H
