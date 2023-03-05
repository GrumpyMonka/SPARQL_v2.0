#ifndef STABWIDGET_H
#define STABWIDGET_H

#include <QMap>
#include <QTabWidget>
#include <sgraphicsview.h>

class STabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit STabWidget( QWidget* parent = nullptr );

    void addWidget( SWidget* widget, const QString& name );
    QVector<DiagramItem*> getBlocksForRun();
    SWidget* currentSWidget();

public slots:
    void setSceneMode( int );
    void itemForAdd( DiagramItemSettings* );
    void resetBlocks();
    void deleteItemOnScene();
    void saveProject();
    void openProject();
    void setTabName( QString );

private:
    void createTabWidgets();

private slots:
    void tabClose( int );
    void slotCurrentTab( int );

signals:
    void newCurrentMode( int );
    void ERROR( QString );

private:
    QMap<QWidget*, int> widget_lib;
};

#endif // STABWIDGET_H
