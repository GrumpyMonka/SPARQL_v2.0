#ifndef DIAGRAMITEMSPARQL_H
#define DIAGRAMITEMSPARQL_H

#include <QGraphicsProxyWidget>
#include <QLabel>

#include <diagramitembased.h>
#include <sparqlblocksettings.h>

class DiagramItemSparql : public DiagramItem
{
public:
    int type() const override { return SparqlItemType; };

    DiagramItemSparql( QMenu* contextMenu, QGraphicsItem* parent = nullptr,
        SparqlBlockSettings* setting = new SparqlBlockSettings() );

    SparqlBlockSettings* getSettings() override;
    QString getScript() override;

private:
    SparqlBlockSettings* settings;
    QGraphicsTextItem* name;
    QGraphicsProxyWidget* proxy_picture;
    QLabel* picture;
};

#endif // DIAGRAMITEMSPARQL_H
