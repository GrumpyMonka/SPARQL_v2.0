#ifndef DIAGRAMARROW_H
#define DIAGRAMARROW_H

#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsProxyWidget>
#include <QLineEdit>

class DiagramArrow : public QGraphicsLineItem
{
public:
    enum
    {
        Type = UserType + 1
    };

    DiagramArrow( QGraphicsPolygonItem* startItem, QGraphicsPolygonItem* endItem,
        QGraphicsItem* parent = 0 );

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor( const QColor& color ) { myColor = color; }
    QGraphicsPolygonItem* startItem() const { return myStartItem; }
    QGraphicsPolygonItem* endItem() const { return myEndItem; }
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;
    void setText( const QString& str );
    QString getText();
    void updatePosition();

protected:
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 ) override;

private:
    QGraphicsPolygonItem* myStartItem;
    QGraphicsPolygonItem* myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
    QLineEdit* line_edit;
    QGraphicsProxyWidget* proxy_widget;
};

#endif // DIAGRAMARROW_H
