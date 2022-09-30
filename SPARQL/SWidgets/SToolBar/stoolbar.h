#ifndef STOOLBAR_H
#define STOOLBAR_H

#include <QToolBar>
#include <QButtonGroup>

#include <smenubar.h>

class SToolBar : public QToolBar
{
    Q_OBJECT
public:
    enum Settings
    {
        pointerButtonChecked,
        linePointerButtonChecked
    };

    explicit SToolBar( SMenuBar& menubar, QWidget* parent = nullptr );

public slots:
    void setSettings( Settings );

private:
    void CreateToolBar( SMenuBar& menubar );

signals:
    void onOpenFile();
    void setNewDiagramSceneMode( int );

private:
    QButtonGroup* pointerTypeGroup;
};

#endif // STOOLBAR_H
