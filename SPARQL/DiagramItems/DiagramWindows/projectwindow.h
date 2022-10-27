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

    explicit ProjectWindow( QWidget* parent = nullptr );

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;

signals:
};

#endif // PROJECTWINDOW_H
