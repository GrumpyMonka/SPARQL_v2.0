#ifndef APIJS_H
#define APIJS_H

#include <QObject>
#include <QVector>

#include <diagramitem.h>

class ApiJS : public QObject
{
    Q_OBJECT
public:
    explicit ApiJS( QObject* parent = nullptr );

    void setDiagramItem( QVector<DiagramItem*>& blocks );

public slots:
    void setOutputForDiagramItem( int index, QString text );
    void clearDeps();

private:
    QVector<DiagramItem*> blocks_lib;
};

#endif // APIJS_H
