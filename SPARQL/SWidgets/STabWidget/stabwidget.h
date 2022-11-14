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

    void addWidget( QWidget* widget, int mode, const QString& name );
    QVector<DiagramItem*> getBlocksForRun();

public slots:
    void setSceneMode( int );
    void itemForAdd( DiagramItemSettings* );
    void resetBlocks();
    void deleteItemOnScene();

private:
    void createTabWidgets();

private slots:
    void tabClose( int );
    void slotCurrentTab( int );

signals:
    void currentMode( int );
    void error( QString );

private:
    QMap<QWidget*, int> widget_lib;
};

#endif // STABWIDGET_H
