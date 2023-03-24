#ifndef STOOLBOX_H
#define STOOLBOX_H

#include <QButtonGroup>
#include <QGridLayout>
#include <QToolBox>
#include <QToolButton>
#include <QMap>

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
    void deleteAll();

private:
    void createToolButtonGroup();
    void addWidget( DiagramItemSettings*, QWidget* );

private slots:
    void buttonGroupClicked();

signals:
    void error( QString );
    void itemPressed( DiagramItemSettings* );

private:
    struct Box
    {
        Box(){};
        Box( const QString& str, SToolBox* parent = nullptr );
        void deleteBox();
        QString name;
        QMap<QWidget*, DiagramItemSettings*> settings;
        QGridLayout* layout;
        QWidget* widget;
    };

    QMap<QToolButton*, DiagramItemSettings*> button_group;

    QMap<QString, Box> settings_list;
};

#endif // STOOLBOX_H
