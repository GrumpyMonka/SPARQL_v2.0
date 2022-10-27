#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>

class DiagramScene : public QGraphicsScene
{
public:
    enum SceneMode
    {
        InsertItem,
        InsertLine,
        MoveItem
    };

    explicit DiagramScene( QObject* parent = nullptr );
};

#endif // DIAGRAMSCENE_H
