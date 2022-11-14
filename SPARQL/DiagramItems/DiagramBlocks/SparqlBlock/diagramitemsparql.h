#ifndef DIAGRAMITEMSPARQL_H
#define DIAGRAMITEMSPARQL_H

#include <diagramitembased.h>
#include <sparqlblocksettings.h>

class DiagramItemSparql : public DiagramItemBased
{
public:
    enum
    {
        Type = UserType + 10
    };
    int type() const override { return Type; };

    DiagramItemSparql( QMenu* contextMenu, QGraphicsItem* parent = nullptr, SparqlBlockSettings* setting = new SparqlBlockSettings() );
};

#endif // DIAGRAMITEMSPARQL_H
