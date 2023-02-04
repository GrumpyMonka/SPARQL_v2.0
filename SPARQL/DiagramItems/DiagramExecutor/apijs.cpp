#include "apijs.h"

ApiJS::ApiJS( QObject* parent )
    : QObject( parent )
{
}

void ApiJS::setDiagramItem( QVector<DiagramItem*>& blocks )
{
    blocks_lib = blocks;
}

void ApiJS::setOutputForDiagramItem( int index, QString text )
{
    blocks_lib[index]->setOutputText( text );
}

void ApiJS::setDependecies( int index, QVector<int> vec_dep )
{
    QVector<DiagramItem*> vec_item;
    for ( auto& dep : vec_dep )
    {
        vec_item.push_back( blocks_lib[dep] );
    }
    blocks_lib[index]->setDependecies( vec_item );
}

void ApiJS::addDep( int index, int dep )
{
    blocks_lib[index]->addDependecies( blocks_lib[dep] );
}

void ApiJS::clearDeps()
{
    for ( auto& block : blocks_lib )
    {
        block->clearDependecies();
    }
}
