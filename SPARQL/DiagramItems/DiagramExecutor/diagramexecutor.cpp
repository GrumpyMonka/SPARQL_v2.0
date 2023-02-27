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

BlocksExec* DiagramExecutor::createBlocksExecObject( DiagramItemSettings* settings )
{
    BlocksExec* block_exec = new BlocksExec( this );
    block_exec->setValueForEngine(
        { { "output", text_edit_output },
            { "network", network_api } } );
    block_exec->setScript( settings->getScript() );
    block_exec->setSettings( settings );
    return block_exec;
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

    // Удаление IO блоков
    for ( auto i = 0; i < blocks_exec_list.size(); ++i )
    {
        auto block_settings = blocks_exec_list[i]->getSettings();
        if ( DiagramItemSettings::IOItemSettingsType == block_settings->typeSettings() )
        {
            auto type_block = static_cast<IOBlockSettings*>( block_settings )->type_block;
            auto io_block = blocks_exec_list[i];
            auto vec_prev_blocks = io_block->getPrevBlocks();
            auto vec_next_blocks = io_block->getNextBlocks();
            for ( auto prev : vec_prev_blocks )
            {
                for ( auto next : vec_next_blocks )
                {
                    prev->addNextBlocks( next );
                    next->addPrevBlocks( prev );
                    if ( IOBlockSettings::TypeIO::Input == type_block
                        && DiagramItemSettings::CompositeItemSettingsType == next->getSettings()->typeSettings() )
                    {
                        next->addBlockConnectName( static_cast<IOBlockSettings*>( block_settings )->text, prev );
                    }
                    else if ( IOBlockSettings::TypeIO::Output == type_block
                        && DiagramItemSettings::CompositeItemSettingsType == prev->getSettings()->typeSettings() )
                    {
                        prev->addBlockConnectName( static_cast<IOBlockSettings*>( block_settings )->text, next );
                    }
                    else
                    {
                        emit ERROR( "Line has text, but not composite blocks!" );
                    }
                }
            }
            io_block->removeConnections();
            io_block->deleteLater();
            blocks_exec_list.erase( blocks_exec_list.begin() + i );
            --i;
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
                auto start_block = new_blocks[line.start_block];
                auto end_block = new_blocks[line.end_block];
                start_block->addNextBlocks( end_block );
                end_block->addPrevBlocks( start_block );

                if ( !line.text.isEmpty() )
                {
                    if ( DiagramItemSettings::CompositeItemSettingsType == start_block->getSettings()->typeSettings() )
                    {
                        start_block->addBlockConnectName( line.text, end_block );
                    }
                    else if ( DiagramItemSettings::CompositeItemSettingsType == end_block->getSettings()->typeSettings() )
                    {
                        end_block->addBlockConnectName( line.text, start_block );
                    }
                    else
                    {
                        emit ERROR( "Line has text, but not composite blocks!" );
                    }
                }
            }

            for ( int k = 0; k < new_blocks.size(); ++k )
            {
                auto block = new_blocks[k];
                if ( DiagramItemSettings::IOItemSettingsType == block->getSettings()->typeSettings() )
                {
                    if ( IOBlockSettings::TypeIO::Input == static_cast<IOBlockSettings*>( block->getSettings() )->type_block )
                    {
                        for ( auto io : composite->getPrevBlocks() )
                        {
                            if ( composite->getBlockConnectName( io )
                                == static_cast<IOBlockSettings*>( block->getSettings() )->text )
                            {
                                for ( auto next : block->getNextBlocks() )
                                {
                                    io->addNextBlocks( next );
                                    next->addPrevBlocks( io );
                                    if ( DiagramItemSettings::CompositeItemSettingsType == next->getSettings()->typeSettings() )
                                    {
                                        next->addBlockConnectName( next->getBlockConnectName( block ), io );
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        for ( auto io : composite->getNextBlocks() )
                        {
                            if ( composite->getBlockConnectName( io )
                                == static_cast<IOBlockSettings*>( block->getSettings() )->text )
                            {
                                for ( auto prev : block->getPrevBlocks() )
                                {
                                    io->addPrevBlocks( prev );
                                    prev->addNextBlocks( io );
                                    if ( DiagramItemSettings::CompositeItemSettingsType == prev->getSettings()->typeSettings() )
                                    {
                                        prev->addBlockConnectName( prev->getBlockConnectName( block ), io );
                                    }
                                }
                            }
                        }
                    }
                    block->removeConnections();
                    blocks_exec_list.erase( blocks_exec_list.begin() + blocks_exec_list.indexOf( block ) );
                    block->deleteLater();
                    new_blocks.erase( new_blocks.begin() + k );
                    --k;
                }
            }
            blocks_exec_list[i]->removeConnections();
            blocks_exec_list[i]->deleteLater();
            blocks_exec_list.erase( blocks_exec_list.begin() + i );
            --i;
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
