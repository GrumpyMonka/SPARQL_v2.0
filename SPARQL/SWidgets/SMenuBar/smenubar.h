#ifndef SMENUBAR_H
#define SMENUBAR_H

#include <QMenuBar>

class SMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit SMenuBar( QWidget* parent = nullptr );

private:
    void CreateActions();
    void CreateMenus();

signals:
    void createItemMenu( QMenu* );

public:
    QMenu* fileMenu;
    QMenu* newItemMenu;
    QMenu* itemMenu;
    QMenu* aboutMenu;

    QAction* toFrontAction;
    QAction* sendBackAction;
    QAction* aboutAction;

    QAction* exitAction;
    QAction* addAction;
    QAction* deleteAction;
    QAction* reworkBlockAction;
    QAction* runSceneScriptAction;
    QAction* createBasedBlockAction;
    QAction* createCompositeBlockAction;
    QAction* createSparqlBlockAction;
    QAction* createNewProjectAction;
    QAction* openProjectAction;
    QAction* saveProjectAction;
};

#endif // SMENUBAR_H
