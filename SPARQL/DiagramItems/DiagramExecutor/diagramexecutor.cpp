#include "diagramexecutor.h"

#include <QGridLayout>
#include <QPushButton>

#include <compositeblocksettings.h>
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

BlocksExec* DiagramExecutor::createBlocksExecObject( DiagramItemSettings* settings )
{
    BlocksExec* block_exec = new BlocksExec( this );
    block_exec->setValueForEngine(
        { { "output", text_edit_output },
            { "network", network_api } } );
    block_exec->setScript( settings->getScript() );
    block_exec->setSettings( settings );
}

void DiagramExecutor::setDiagramItem( QVector<DiagramItem*>& item_list )
{
    // Добавление блоков
    for ( auto item : item_list )
    {
        auto block_exec = createBlocksExecObject( item->getSettings() );
        block_exec->setDiagramItem( item );
        block_exec->setUserData( QScriptValue( item->getInputData() ) );
        blocks_exec_list.push_back( block_exec );
    }

    // Настраивание связей
    for ( auto i = 0; i < item_list.size(); ++i )
    {
        auto arrows = item_list[i]->getEndArrows();
        for ( auto arrow : arrows )
        {
            int index = item_list.indexOf( arrow->startItem() );
            blocks_exec_list[i]->addPrevBlocks( blocks_exec_list[index] );
            blocks_exec_list[index]->addNextBlocks( blocks_exec_list[i] );
        }
    }

    // Просмотр композитных блоков
    for ( auto i = 0; i < blocks_exec_list.size(); ++i )
    {
        if ( DiagramItemSettings::CompositeItemSettingsType == blocks_exec_list[i]->getSettings()->typeSettings() )
        {
            auto composite = blocks_exec_list[i];
            auto settings = static_cast<CompositeBlockSettings*>( composite->getSettings() );
            QVector<BlocksExec*> new_blocks;
            for ( auto& block_set : settings->blocks )
            {
                auto block_exec = createBlocksExecObject( block_set );
                if ( DiagramItemSettings::BasedItemSettingsType == block_set->typeSettings() )
                {
                    block_exec->setUserData( QScriptValue( static_cast<BasedBlockSettings*>( block_set )->line_edit_text ) );
                }

                new_blocks.push_back( block_exec );
                blocks_exec_list.push_back( block_exec );
            }

            for ( const auto& line : settings->lines )
            {
                new_blocks[line.start_block]->addNextBlocks( new_blocks[line.end_block] );
                new_blocks[line.end_block]->addPrevBlocks( new_blocks[line.start_block] );
            }

            for ( auto block : new_blocks )
            {
                if ( DiagramItemSettings::IOItemSettingsType == block->getSettings()->typeSettings() )
                {
                    if ( IOBlockSettings::TypeIO::Input == static_cast<IOBlockSettings*>( block->getSettings() )->type_block )
                    {
                        for ( auto io : composite->getPrevBlocks() )
                        {
                            if ( io->getSettings()->block_name == block->getSettings()->block_name )
                            {
                                io->setNextBlocks( { block } );
                                block->addPrevBlocks( io );
                            }
                        }
                    }
                    else
                    {
                        for ( auto io : composite->getNextBlocks() )
                        {
                            if ( io->getSettings()->block_name == block->getSettings()->block_name )
                            {
                                io->setPrevBlocks( { block } );
                                block->addNextBlocks( io );
                            }
                        }
                    }
                }
            }
        }
    }

    // Удаление IO блоков

    // Удаление композитных блоков
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

void DiagramExecutor::logs_sniff( QStringList str_list )
{
    text_edit_script->insertPlainText( str_list.join( "\n" ) );
}

void DiagramExecutor::execute()
{
    while ( true )
    {
        bool flag = false;
        for ( auto block_exec : blocks_exec_list )
        {
            if ( !block_exec->getFlagOfWorking() && block_exec->checkForWork() )
            {
                connect( block_exec, SIGNAL( logs( QStringList ) ),
                    this, SLOT( logs_sniff( QStringList ) ) );

                block_exec->runBlock();
                flag = true;
            }
        }
        if ( !flag )
        {
            break;
        }
    }
}
