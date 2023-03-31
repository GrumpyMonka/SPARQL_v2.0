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
        block_exec->setUserData( item->getInputData() );
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
            auto text = static_cast<IOBlockSettings*>( block_settings )->text;
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
                        next->removeConnectName( io_block );
                        next->addBlockConnectName( text, prev );
                        if ( DiagramItemSettings::CompositeItemSettingsType == prev->getSettings()->typeSettings() )
                        {
                            auto name_connect = prev->getBlockConnectName( io_block );
                            if ( !name_connect.isEmpty() )
                            {
                                prev->removeConnectName( io_block );
                                prev->addBlockConnectName( name_connect, next );
                            }
                        }
                    }

                    if ( IOBlockSettings::TypeIO::Output == type_block
                        && DiagramItemSettings::CompositeItemSettingsType == prev->getSettings()->typeSettings() )
                    {
                        prev->removeConnectName( io_block );
                        prev->addBlockConnectName( static_cast<IOBlockSettings*>( block_settings )->text, next );
                        prev->addLinkedItemForComposite( static_cast<IOBlockSettings*>( block_settings )->text, io_block->getDiagramItem() );
                        if ( DiagramItemSettings::CompositeItemSettingsType == next->getSettings()->typeSettings() )
                        {
                            auto name_connect = next->getBlockConnectName( io_block );
                            if ( !name_connect.isEmpty() )
                            {
                                next->removeConnectName( io_block );
                                next->addBlockConnectName( name_connect, prev );
                            }
                        }
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
                    block_exec->setUserData( static_cast<BasedBlockSettings*>( block_set )->line_edit_text );
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

                if ( DiagramItemSettings::CompositeItemSettingsType == start_block->getSettings()->typeSettings() )
                {
                    if ( !line.text_start.isEmpty() )
                    {
                        start_block->addBlockConnectName( line.text_start, end_block );
                    }
                    else
                    {
                        emit ERROR( "DiagramExecutor::setDiagramItem() -> Line without text" );
                    }
                }
                if ( DiagramItemSettings::CompositeItemSettingsType == end_block->getSettings()->typeSettings() )
                {
                    if ( !line.text_end.isEmpty() )
                    {
                        end_block->addBlockConnectName( line.text_end, start_block );
                    }
                    else
                    {
                        emit ERROR( "DiagramExecutor::setDiagramItem() -> Line without text" );
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
                                    if ( DiagramItemSettings::CompositeItemSettingsType == io->getSettings()->typeSettings() )
                                    {
                                        io->addBlockConnectName( io->getBlockConnectName( composite ), next );
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
                                auto item = composite->getLinkedItemForComposite( composite->getBlockConnectName( io ) );
                                for ( auto prev : block->getPrevBlocks() )
                                {
                                    prev->setDiagramItem( item );
                                    io->addPrevBlocks( prev );
                                    prev->addNextBlocks( io );
                                    if ( DiagramItemSettings::CompositeItemSettingsType == prev->getSettings()->typeSettings() )
                                    {
                                        prev->addBlockConnectName( prev->getBlockConnectName( block ), io );
                                    }
                                    if ( DiagramItemSettings::CompositeItemSettingsType == io->getSettings()->typeSettings() )
                                    {
                                        io->addBlockConnectName( io->getBlockConnectName( composite ), prev );
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
            + "{input:|output:|user data:|in/out}|{{[]}|{"
            + ( ( "SPARQL" != blocks_exec_list[i]->getSettings()->getNameType() ) ? blocks_exec_list[i]->getOutputData().toString() : "" )
            + "}|{"
            + blocks_exec_list[i]->getUserData() + "}|{"
            + QString::number( blocks_exec_list[i]->getPrevBlocks().size() ) + " / "
            + QString::number( blocks_exec_list[i]->getNextBlocks().size() ) + "}}\"]\n";
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

#ifdef Q_OS_LINUX
    if ( path.isEmpty() )
    {
        auto time = QString::number( QDateTime::currentMSecsSinceEpoch() );
        path = "logs/" + time + "/";
        QDir( "logs/" ).mkpath( time );
    }
    QString file_name = QDir( path + QString::number( QDateTime::currentMSecsSinceEpoch() ) + ".png" ).absolutePath();
    gvRender( gvc, graph, "png", fopen( file_name.toStdString().c_str(), "w" ) );
#endif

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
    text_edit_output->clear();
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
    QString output = text_edit_output->toPlainText();
    paint();
    text_edit_output->insertPlainText( output + "\n-------------------------------------------------------------------------------\n" );
}
