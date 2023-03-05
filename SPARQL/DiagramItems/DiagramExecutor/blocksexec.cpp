#include "blocksexec.h"

BlocksExec::BlocksExec( QObject* parent )
    : QObject{ parent }
{
    flag_of_work = false;
    allow_work = true;
}

QString BlocksExec::runBlock()
{
    logs_exec.clear();
    logs_exec.push_back( "\n---->New Start<----\n" );
    setInputData( checkInputValue() );
    logs_exec.push_back( "INPUT DATA: " );
    for ( auto data : getInputData() )
    {
        logs_exec.push_back( data.toString() );
    }

    try
    {
        auto engine = createEngine();

        execScript( engine, "var x = [];" );
        execScript( engine, "var y = [];" );
        for ( int i = 0; i < input_data.size(); ++i )
        {
            QString name_temp = "input_data" + QString::number( i );
            QScriptValue temp_value( input_data[i].toString() );
            engine->globalObject().setProperty( name_temp, temp_value );
            execScript( engine, "x.push( " + name_temp + " );" );
        }
        engine->globalObject().setProperty( "input", user_data );

        execScript( engine, main_script );

        output_data = engine->globalObject().property( "y" );
        logs_exec.push_back( "\nOUTPUT DATA: " + output_data.toString() );
        if ( nullptr != block )
        {
            block->setOutputText( output_data.toString() );
        }
    }
    catch ( QString& err )
    {
        return err;
    }
    logs_exec.push_back( "\n----> Success <----\n" );

    flag_of_work = true;
    emit blockFinished();
    emit logs( logs_exec );
    return {};
}

void BlocksExec::loadScript( QScriptEngine* engine, const QString& path )
{
    QFile f( path );
    if ( f.open( QIODevice::ReadOnly ) )
    {
        QString str = f.readAll();
        execScript( engine, str, false );
    }
    else
    {
        emit error( "Failed load! (" + path + ")" );
    }
}

QScriptEngine* BlocksExec::createEngine()
{
    QScriptEngine* engine = new QScriptEngine( this );
    for ( auto& key : default_value_list.keys() )
    {
        auto script_value = engine->newQObject( default_value_list[key] );
        engine->globalObject().setProperty( key, script_value );
    }
    loadScript( engine, ":/Sources/scripts/script.js" );
    loadScript( engine, ":/Sources/scripts/XMLHttpRequest.js" );

    return engine;
}

void BlocksExec::execScript( QScriptEngine* engine, const QString& script, bool logs_flag, bool exception )
{
    auto result = engine->evaluate( script );
    if ( logs_flag )
        logs_exec.push_back( script );
    if ( result.isError() )
    {
        logs_exec.push_back( "\n---->  Fail  <----\n" );
        logs_exec.push_back( "Error: " + result.toString() );
        emit error( result.toString() );
        if ( exception )
        {
            throw result.toString();
        }
    }
}

QStringList BlocksExec::getLogs()
{
    return logs_exec;
}

void BlocksExec::removeConnections()
{
    for ( auto block : prev_blocks )
    {
        block->removeConnect( this );
    }
    for ( auto block : next_blocks )
    {
        block->removeConnect( this );
    }
}

void BlocksExec::addBlockConnectName( const QString& name, BlocksExec* block )
{
    block_connect_name.insert( name, block );
}

QString BlocksExec::getBlockConnectName( BlocksExec* block )
{
    return block_connect_name.key( block );
}

void BlocksExec::removeConnect( BlocksExec* block )
{
    auto it = prev_blocks.indexOf( block );
    if ( it != -1 )
    {
        prev_blocks.erase( prev_blocks.begin() + it );
        return;
    }
    it = next_blocks.indexOf( block );
    if ( it != -1 )
    {
        next_blocks.erase( next_blocks.begin() + it );
    }
}

bool BlocksExec::checkForWork()
{
    for ( auto block : prev_blocks )
    {
        if ( !block->getFlagOfWorking() )
            return false;
    }
    return true;
}

QScriptValueList BlocksExec::checkInputValue()
{
    QScriptValueList value_list;
    for ( auto block : prev_blocks )
    {
        value_list.push_back( block->getOutputData() );
    }
    return value_list;
}

void BlocksExec::setValueForEngine( const QMap<QString, QObject*>& value_list )
{
    default_value_list = value_list;
}

void BlocksExec::setDiagramItem( DiagramItem* item )
{
    block = item;
}

DiagramItem* BlocksExec::getDiagramItem()
{
    return block;
}

void BlocksExec::setTags( const QMap<QString, QString>& tags )
{
    block_tags = tags;
}

void BlocksExec::addTag( const QString& key, const QString& value )
{
    block_tags.insert( key, value );
}

QMap<QString, QString> BlocksExec::getTags()
{
    return block_tags;
}

QString BlocksExec::getTag( const QString& key )
{
    return block_tags[key];
}

void BlocksExec::deleteTags()
{
    block_tags.clear();
}

void BlocksExec::deleteTag( const QString& key )
{
    auto it = block_tags.find( key );
    if ( block_tags.end() != it )
    {
        block_tags.erase( it );
    }
}

void BlocksExec::disconnectDiagramItem()
{
    setDiagramItem( nullptr );
}

void BlocksExec::setSettings( DiagramItemSettings* settings )
{
    block_settings = settings;
}

DiagramItemSettings* BlocksExec::getSettings()
{
    return block_settings;
}

void BlocksExec::setFlagOfWorking( bool flag )
{
    flag_of_work = flag;
}

void BlocksExec::setAllowWork( bool flag )
{
    allow_work = flag;
}

void BlocksExec::setScript( const QString& script )
{
    main_script = script;
}

void BlocksExec::setInputData( const QScriptValueList& input_list )
{
    input_data = input_list;
}

void BlocksExec::addInputData( const QScriptValue& input )
{
    input_data.push_back( input );
}

void BlocksExec::setUserData( const QString& data )
{
    user_data = data;
}

void BlocksExec::setNextBlocks( const QVector<BlocksExec*>& next_list )
{
    next_blocks = next_list;
}

void BlocksExec::addNextBlocks( BlocksExec* next )
{
    next_blocks.push_back( next );
}

void BlocksExec::setPrevBlocks( const QVector<BlocksExec*>& prev_list )
{
    prev_blocks = prev_list;
}

void BlocksExec::addPrevBlocks( BlocksExec* prev )
{
    prev_blocks.push_back( prev );
}

bool BlocksExec::getFlagOfWorking()
{
    return flag_of_work;
}

bool BlocksExec::getAllowWork()
{
    return allow_work;
}

QString BlocksExec::getScript()
{
    return main_script;
}

QScriptValueList BlocksExec::getInputData()
{
    return input_data;
}

QString BlocksExec::getUserData()
{
    return user_data;
}

QScriptValue BlocksExec::getOutputData()
{
    return output_data;
}

QVector<BlocksExec*> BlocksExec::getNextBlocks()
{
    return next_blocks;
}

QVector<BlocksExec*> BlocksExec::getPrevBlocks()
{
    return prev_blocks;
}
