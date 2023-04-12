#ifndef DIAGRAMITEMATOM_H
#define DIAGRAMITEMATOM_H

#include <QGraphicsProxyWidget>
#include <QLineEdit>

#include <atomblocksettings.h>
#include <diagramitem.h>

class DiagramItemAtom : public DiagramItem
{
public:
    int type() const override { return AtomItemType; }

    DiagramItemAtom( QMenu* context_menu, QGraphicsItem* parent = nullptr,
        AtomBlockSettings* settings = new AtomBlockSettings() );

    void setItemPolygon( QPolygonF& polygon );
    AtomBlockSettings* getSettings() override;
    void setText( const QString& text );
    QString getText();
    void setPosLineEdit( const QPointF& );

private:
    QLineEdit* line_edit;
    QGraphicsProxyWidget* proxy_line_edit;
    QString type_block;
    QGraphicsProxyWidget* proxy_label;
    QLabel* label;
};

#endif // DIAGRAMITEMATOM_H
