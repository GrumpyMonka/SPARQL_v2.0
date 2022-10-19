#ifndef STABWIDGET_H
#define STABWIDGET_H

#include <QMap>
#include <QTabWidget>

class STabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit STabWidget( QWidget* parent = nullptr );

    void addWidget( QWidget* widget, int mode, const QString& name );

private:
    void createTabWidgets();

private slots:
    void tabClose( int );
    void slotCurrentTab( int );

signals:
    void currentMode( int );

private:
    QMap<QWidget*, int> widget_lib;
};

#endif // STABWIDGET_H
