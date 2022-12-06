#ifndef DIAGRAMITEMBASED_H
#define DIAGRAMITEMBASED_H

#include <basedblocksettings.h>
#include <diagramitem.h>

#include <QFont>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>

class DiagramItemBased : public DiagramItem
{
public:
    virtual int type() const override { return BasedItemType; }

    DiagramItemBased( QMenu* contextMenu, QGraphicsItem* parent = nullptr, BasedBlockSettings* setting = new BasedBlockSettings() );
    void setSettings( BasedBlockSettings* setting );
    QString getName();
    BasedBlockSettings* getSettings() override;
    QString getInputData() override;
    void setDrawPicture( bool );
    QString getScript() override;

private:
    QGraphicsTextItem* name;
    QGraphicsTextItem* label;
    QLineEdit* line_edit;
    QGraphicsProxyWidget* proxy_line_edit;
    QGraphicsProxyWidget* proxy_picture;
    QLabel* picture;
    QPixmap pixmap;
    QString script;
};

#endif // DIAGRAMITEMBASED_H
