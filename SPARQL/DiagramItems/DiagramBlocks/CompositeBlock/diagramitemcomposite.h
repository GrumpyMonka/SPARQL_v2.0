#ifndef DIAGRAMITEMCOMPOSITE_H
#define DIAGRAMITEMCOMPOSITE_H

#include <QGraphicsProxyWidget>
#include <QLabel>

#include <compositeblocksettings.h>
#include <diagramitem.h>
#include <diagramitemio.h>

class DiagramItemComposite : public DiagramItem
{
public:
    int type() const override { return CompositeItemType; }

    DiagramItemComposite( QMenu* contextMenu, QGraphicsItem* parent = nullptr,
        CompositeBlockSettings* settings = new CompositeBlockSettings() );

    CompositeBlockSettings* getSettings() override;
    QString getScript() override;
    void setSettings( CompositeBlockSettings* );
    DiagramItemIO* getInputBlock( QString& name );
    DiagramItemIO* getOutputBlock( QString& name );
    QVector<DiagramArrow*> getArrowForInputBlock( QString& name );
    QVector<DiagramArrow*> getArrowForOutputBlock( QString& name );

private:
    CompositeBlockSettings* settings;
    QVector<DiagramItemIO*> input_blocks;
    QVector<DiagramItemIO*> output_blocks;
};

#endif // DIAGRAMITEMCOMPOSITE_H
