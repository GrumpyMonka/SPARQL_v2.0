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
    try
    {
        auto engine = createEngine();

        execScript( engine, "var x = [];" );
        execScript( engine, "var y = [];" );
        for ( int i = 0; i < input_data.size(); ++i )
        {
            engine->globalObject().setProperty( "in" + QString::number( i ), input_data[i] );
            execScript( engine, "x.push( in" + QString::number( i ) + " );" );
        }
        engine->globalObject().setProperty( "input", user_data );

        execScript( engine, main_script );

        output_data = engine->globalObject().property( "y" );
    }
    catch ( QString err )
    {
        return err;
    }

    emit blockFinished();
    return {};
}

void BlocksExec::loadScript( QScriptEngine* engine, const QString& path )
{
    QFile f( path );
    if ( f.open( QIODevice::ReadOnly ) )
    {
        QString str = f.readAll();
        execScript( engine, str );
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

void BlocksExec::execScript( QScriptEngine* engine, const QString& script, bool exception )
{
    logs_exec.push_back( "\n---->New Start<----\n" );
    auto result = engine->evaluate( script );
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
    logs_exec.push_back( "\n----> Success <----\n" );
}

QStringList BlocksExec::getLogs()
{
    return logs_exec;
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

void BlocksExec::disconnectDiagramItem()
{
    setDiagramItem( nullptr );
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

void BlocksExec::setUserData( const QScriptValue& data )
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

QScriptValue BlocksExec::getUserData()
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
