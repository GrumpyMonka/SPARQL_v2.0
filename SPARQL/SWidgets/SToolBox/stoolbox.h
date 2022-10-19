#ifndef STOOLBOX_H
#define STOOLBOX_H

#include <QButtonGroup>
#include <QGridLayout>
#include <QToolBox>

#include <diagramitem.h>
#include <diagramitemsettings.h>

class SToolBox : public QToolBox
{
    Q_OBJECT
public:
    explicit SToolBox( QWidget* parent = nullptr );

public slots:
    void setDiagramItems( const QVector<DiagramItemSettings*>& );
    void addDiagramItem( DiagramItemSettings* );
    void addDiagramItems( const QVector<DiagramItemSettings*>& );
    void deleteDiagramItem( DiagramItemSettings* );
    void deleteDiagramItems( const QVector<DiagramItemSettings*>& );

private:
    void createToolButtonGroup();
    void createCellWidget( DiagramItemSettings* settings = nullptr, bool addButtonGroup = false );
    void addWidget( DiagramItemSettings*, QWidget* );

private slots:
    void buttonGroupClicked( int );

signals:
    void error( QString );
    void itemPressed( DiagramItemSettings* );

private:
    QButtonGroup* buttonGroup;
    QGridLayout* buttonLayout;

    // QMap<QWidget*, DiagramItemSettings*> library;

    QVector<DiagramItemSettings*> settings_list;
    QVector<QWidget*> widget_list;
};

#endif // STOOLBOX_H
