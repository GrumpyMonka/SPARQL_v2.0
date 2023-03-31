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

void ApiJS::clearDeps()
{
    for ( auto& block : blocks_lib )
    {
        block->clearDependecies();
    }
}
