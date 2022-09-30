#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>

class DiagramScene : public QGraphicsScene
{
public:
    enum Mode
    {
        InsertItem,
        InsertLine,
        MoveItem
    };

    explicit DiagramScene( QObject* parent = nullptr );

};

#endif // DIAGRAMSCENE_H
