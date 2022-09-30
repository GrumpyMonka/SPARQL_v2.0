#ifndef STABWIDGET_H
#define STABWIDGET_H

#include <QTabWidget>

class STabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit STabWidget( QWidget *parent = nullptr );

private:
    void CreateTabWidgets();

private slots:
    void TabClose( int );

signals:

};

#endif // STABWIDGET_H
