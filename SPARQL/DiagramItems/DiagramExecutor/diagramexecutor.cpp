#include "diagramexecutor.h"

#include <QGridLayout>
#include <QPushButton>

#include <diagramarrow.h>
#include <diagramitembased.h>
#include <diagramitemio.h>
#include <diagramitemsparql.h>

DiagramExecutor::DiagramExecutor( QWidget* parent )
    : SWidget( parent )
{
    createWindow();

    network_api = new SNetwork( this );
    api = new ApiJS( this );
}
/*
QString CreateScriptForBlock( QVector<DiagramItem*>& block_list, int index )
{
    QString result = "";
    DiagramItem* diagram_item = block_list[index];

    result += getHtmlLine( "\nblocks_list.push( new Block( " );
    result += getHtmlLine( "\tfunction( x, index, vec ) {" );

    if ( diagram_item->getInputData() != "" )
        result += getHtmlLine( "\t\tvar input = " + diagram_item->getInputData() + ";" );

    result += getHtmlLine( "\t\tvar y = [];" );
    result += getHtmlLine( "\t\tvar dep = [];" );

    QStringList list;
    if ( DiagramItem::IOItemType != diagram_item->type() )
    {
        list = diagram_item->getScript().split( "\n" );
    }

    foreach ( QString iter, list )
    {
        for ( int i = 0; i < iter.size(); i++ )
        {
            if ( iter[i] == "<" )
            {
                iter = iter.mid( 0, i ) + "&lt;" + iter.mid( i + 1, iter.size() );
            }
            if ( iter[i] == ">" )
            {
                iter = iter.mid( 0, i ) + "&gt;" + iter.mid( i + 1, iter.size() );
            }
        }
        result += getHtmlLine( "\t\t" + iter );
    }

    result += getHtmlLine( "\t\tfor( var i = 0; i &lt; dep.length; i++ ){" );
    result += getHtmlLine( "\t\t\tapi.addDep( index, dep[i] );\n\t\t}" );
    result += getHtmlLine( "\t\treturn y;" );
    result += getHtmlLine( "\t}," );

    if ( DiagramItem::IOItemType == diagram_item->type() )
    {
        result += "\t[ \"" + ( static_cast<DiagramItemIO*>( diagram_item ) )->block_name + "\" ],";
    }
    else
    {
        result += getHtmlLine( "\t[ ]," );
    }

    QString temp = "[ ";

    foreach ( DiagramArrow* arrow, block_list[index]->getArrows() )
    {
        if ( arrow->startItem() != block_list[index] )
        {
            temp += QString::number( block_list.indexOf( arrow->startItem() ) );
            temp += ", ";
        }
    }

    if ( temp[temp.size() - 2] == "," )
        temp.remove( temp.size() - 2, 1 );

    temp += "]";

    result += getHtmlLine( "\t" + temp );
    result += getHtmlLine( "));\n" );
    return result;
}
*/
void DiagramExecutor::setDiagramItem( QVector<DiagramItem*>& item_list )
{
    for ( auto item : item_list )
    {
        BlocksExec* block_exec = new BlocksExec( this );
        block_exec->setValueForEngine(
            { { "output", text_edit_output },
                { "network", network_api } } );
        block_exec->setScript( item->getScript() );
        block_exec->setDiagramItem( item );
        block_exec->setUserData( QScriptValue( item->getInputData() ) );
        blocks_exec_list.push_back( block_exec );
    }

    for ( int i = 0; i < item_list.size(); ++i )
    {
        auto arrows = item_list[i]->getEndArrows();
        for ( auto arrow : arrows )
        {
            int index = item_list.indexOf( arrow->startItem() );
            blocks_exec_list[i]->addPrevBlocks( blocks_exec_list[index] );
            blocks_exec_list[index]->addNextBlocks( blocks_exec_list[i] );
        }
    }
}

void DiagramExecutor::createWindow()
{
    QGridLayout* grid_layout = new QGridLayout( this );
    text_edit_script = new QTextEdit( this );
    text_edit_script->setTabStopDistance( 30 );
    text_edit_output = new QTextEdit( this );
    text_edit_output->setTabStopDistance( 30 );
    text_edit_output->setMaximumHeight( 300 );
    QPushButton* button_exec = new QPushButton( "Execute", this );
    connect( button_exec, SIGNAL( clicked() ), this, SLOT( execute() ) );

    grid_layout->addWidget( text_edit_script, 0, 0 );
    grid_layout->addWidget( text_edit_output, 1, 0 );
    grid_layout->addWidget( button_exec, 2, 0 );
}

void DiagramExecutor::execute()
{
}
