#include "diagramexecutor.h"

#include <QGridLayout>
#include <QPushButton>

#include <compositeblocksettings.h>
#include <diagramarrow.h>
#include <diagramitembased.h>
#include <diagramitemio.h>
#include <diagramitemsparql.h>

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

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
    paint();

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
    paint();

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
    paint();
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

            paint();

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

            paint();
        }
    }
}

void DiagramExecutor::paint()
{
    /*
     * digraph test
 {
    fontname="Helvetica,Arial,sans-serif"
    node [fontname="Helvetica,Arial,sans-serif"]
    edge [fontname="Helvetica,Arial,sans-serif"]
    concentrate=True;
    rankdir=TB;
    node [shape=record];

    0 [label="IO: Input\n|{input:|output:|user data:}|{{[]}|{[]}|{[]}}"];
    10 [label="Based: Summary\n|{input:|output:|user data:}|{{[]}|{[]}|{[]}}"];
    0 -> 10 [label="test"];
}
     */
    QString text_format = "digraph graph_blocks\n"
                          "{\n"
                          "\tfontname=\"Helvetica,Arial,sans-serif\"\n"
                          "\tnode [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                          "\tedge [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                          "\tconcentrate=True;\n"
                          "\trankdir=TB;\n"
                          "\tnode [shape=record];\n";
    for ( int i = 0; i < blocks_exec_list.size(); ++i )
    {
        text_format += "\t" + QString::number( i ) + " "
            + "[label=\""
            + blocks_exec_list[i]->getSettings()->getNameType() + "\\n\\n\\n"
            + blocks_exec_list[i]->getSettings()->block_name + "\\n|"
            + "{input:|output:|user data:|in/out}|{{[]}|{[]}|{"
            + blocks_exec_list[i]->getUserData().toString() + "}|{"
            + QString::number( blocks_exec_list[i]->getPrevBlocks().size() ) + " / "
            + QString::number( blocks_exec_list[i]->getNextBlocks().size() ) + "}}\"]\n";
        // for( auto input : blocks_exec_list[i]->getInputData() )
        //{
        //     text_format +=
        // }
    }
    text_format += "\n\n";
    for ( int i = 0; i < blocks_exec_list.size(); ++i )
    {
        for ( auto block : blocks_exec_list[i]->getNextBlocks() )
        {
            text_format += "\t" + QString::number( i ) + " -> " + QString::number( blocks_exec_list.indexOf( block ) ) + "\n";
        }
    }
    text_format += "}";
    text_edit_output->setText( text_format );

    GVC_t* gvc;
    gvc = gvContext();

    Agraph_t* graph = agmemread( text_format.toStdString().c_str() );

    gvLayout( gvc, graph, "dot" );
    char* png_bytes;
    unsigned int size_png_bytes;
    if ( path.isEmpty() )
    {
        auto time = QString::number( QDateTime::currentMSecsSinceEpoch() );
        path = "/home/grumpymonk/Project/" + time + "/";
        QDir( "/home/grumpymonk/Project/" ).mkpath( time );
    }
    QString file_name = path + QString::number( QDateTime::currentMSecsSinceEpoch() ) + ".png";
    gvRender( gvc, graph, "png", fopen( file_name.toStdString().c_str(), "w" ) );
    gvRenderData( gvc, graph, "png", &png_bytes, &size_png_bytes );

    gvFreeLayout( gvc, graph );
    agclose( graph );

    gvFreeContext( gvc );

    QByteArray byte_array_png( png_bytes, size_png_bytes );

    QPixmap pixmap;
    pixmap.loadFromData( byte_array_png, "PNG" );
    // pixmap = pixmap.scaled( 800, 600 );
    label_pixmap->setPixmap( pixmap );
}

void DiagramExecutor::createWindow()
{
    QGridLayout* grid_layout = new QGridLayout( this );
    // text_edit_script = new QTextEdit( this );
    // text_edit_script->setTabStopDistance( 30 );
    text_edit_output = new QTextEdit( this );
    text_edit_output->setTabStopDistance( 30 );
    text_edit_output->setMaximumHeight( 300 );
    QPushButton* button_exec = new QPushButton( "Execute", this );
    connect( button_exec, SIGNAL( clicked() ), this, SLOT( execute() ) );

    graphics_view = new QGraphicsView( this );
    auto scene = new QGraphicsScene( graphics_view );
    graphics_view->setScene( scene );
    label_pixmap = new QLabel();
    scene->addWidget( label_pixmap );

    // label->setSizePolicy( QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored );
    //  grid_layout->addWidget( new QPushButton( "mem" ), 0, 0 );
    grid_layout->addWidget( graphics_view, 0, 0 );
    //   grid_layout->addWidget( text_edit_script, 0, 0 );
    grid_layout->addWidget( text_edit_output, 1, 0 );
    grid_layout->addWidget( button_exec, 2, 0 );
}

void DiagramExecutor::logs_sniff( QStringList str_list )
{
    // text_edit_script->insertPlainText( str_list.join( "\n" ) );
}

void DiagramExecutor::execute()
{
    // paint();

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
