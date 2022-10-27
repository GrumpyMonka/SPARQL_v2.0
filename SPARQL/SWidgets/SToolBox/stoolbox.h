#ifndef STOOLBOX_H
#define STOOLBOX_H

#include <QButtonGroup>
#include <QGridLayout>
#include <QToolBox>

#include <basedblocksettings.h>
#include <compositeblocksettings.h>
#include <diagramitem.h>
#include <diagramitemsettings.h>

#define COUNT_COLUMN 3
#define SIZE 50

class SToolBox : public QToolBox
{
    Q_OBJECT
public:
    explicit SToolBox( QWidget* parent = nullptr );

public slots:
    void setDiagramItems( const QVector<DiagramItemSettings*>& );
    void addDiagramItem( DiagramItemSettings*, bool addButtonGroup = true );
    void addDiagramItems( const QVector<DiagramItemSettings*>& );
    void deleteDiagramItem( DiagramItemSettings* );
    void deleteDiagramItems( const QVector<DiagramItemSettings*> );

private:
    void createToolButtonGroup();
    void addWidget( DiagramItemSettings*, QWidget* );

private slots:
    void buttonGroupClicked( int );

signals:
    void error( QString );
    void itemPressed( DiagramItemSettings* );

private:
    QButtonGroup* button_group;
    QGridLayout* button_layout;

    // QMap<QWidget*, DiagramItemSettings*> library;

    QVector<DiagramItemSettings*> settings_list;
    QVector<QWidget*> widget_list;
};

#endif // STOOLBOX_H
