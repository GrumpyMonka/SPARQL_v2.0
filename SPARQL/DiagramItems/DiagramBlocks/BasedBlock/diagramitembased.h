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
    enum
    {
        Type = UserType + 6
    };
    int type() const override { return Type; }

    DiagramItemBased( QMenu* contextMenu, BasedBlockSettings* setting = new BasedBlockSettings(),
        QGraphicsItem* parent = nullptr );
    void setSetting( BasedBlockSettings* setting );
    QString getName();
    BasedBlockSettings* getSetting();
    QString GetInputData();
    void setDrawPicture( bool );

public slots:
    void setInputData( QString );
    void setOutputData( QString );

private:
    QGraphicsTextItem* name;
    QGraphicsTextItem* label;
    QGraphicsTextItem* input_data;
    QGraphicsTextItem* output_data;
    QLineEdit* line_edit;
    QGraphicsProxyWidget* proxy_line_edit;
    QGraphicsProxyWidget* proxy_picture;
    BasedBlockSettings* setting_;
    QLabel* picture;
};

#endif // DIAGRAMITEMBASED_H
