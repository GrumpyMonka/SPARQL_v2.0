#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <sgraphicsview.h>

class ProjectWindow : public SGraphicsView
{
    Q_OBJECT
public:
    enum
    {
        DiagramMode = 7
    };

    enum
    {
        Type = 1
    };
    int type() override { return Type; }

    explicit ProjectWindow( QWidget* parent = nullptr );

    QVector<DiagramItem*> getDiagramItems();

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;

signals:
};

#endif // PROJECTWINDOW_H
