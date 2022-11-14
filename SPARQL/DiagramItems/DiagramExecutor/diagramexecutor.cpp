#include "diagramexecutor.h"

#include <QGridLayout>
#include <QPushButton>

DiagramExecutor::DiagramExecutor( QWidget* parent )
    : QWidget( parent )
{
    createWindow();

    script_engine = new QScriptEngine( this );

    QScriptValue outputVal = script_engine->newQObject( text_edit_output );
    script_engine->globalObject().setProperty( "output", outputVal );

    // QScriptValue networkVal = engine->newQObject( network );
    // engine->globalObject().setProperty( "network", networkVal );
}

void DiagramExecutor::setScript( QString& )
{
}

QString DiagramExecutor::ConvertDiagramItemToScript( QVector<DiagramItemSettings*>& )
{
}

void DiagramExecutor::createWindow()
{
    QGridLayout* grid_layout = new QGridLayout( this );
    text_edit_script = new QTextEdit( this );
    text_edit_output = new QTextEdit( this );
    text_edit_output->setMaximumHeight( 300 );
    QPushButton* button_exec = new QPushButton( "Execute", this );
    connect( button_exec, SIGNAL( clicked() ), this, SLOT( execute() ) );

    grid_layout->addWidget( text_edit_script, 0, 0 );
    grid_layout->addWidget( text_edit_output, 1, 0 );
    grid_layout->addWidget( button_exec, 2, 0 );
}

QString DiagramExecutor::loadScript( QString path )
{
    QFile f( path );
    QScriptValue result;
    if ( f.open( QIODevice::ReadOnly ) )
    {
        QString str = f.readAll();
        result = script_engine->evaluate( str, path );
        if ( result.isError() )
        {
            return result.toString();
        }
    }
    else
    {
        return "Failed load! (" + path + ")";
    }
    return "";
}

void DiagramExecutor::execute()
{
    script_engine->pushContext();
    text_edit_output->disconnect();
    text_edit_output->clear();
    QScriptValue result;

    QString temp = loadScript( ":/Sources/scripts/script.js" );
    if ( temp.length() )
    {
        text_edit_output->setText( "Defult script: " + temp );
    }

    temp = loadScript( ":/Sources/scripts/XMLHttpRequest.js" );
    if ( temp.length() )
    {
        text_edit_output->setText( "Defult script: " + temp );
    }

    temp = loadScript( ":/Sources/scripts/progress.js" );
    if ( temp.length() )
    {
        text_edit_output->setText( "Progress script: " + temp );
        return;
    }

    result = script_engine->evaluate( text_edit_script->toPlainText() );
    if ( result.isError() )
    {
        text_edit_output->setText( "User script: " + result.toString() );
        return;
    }

    // temp = loadScript("C:/Temp/Study/6 semestr/Kurs/Kurs/scripts/block_logic.js");
    temp = loadScript( ":/Sources/scripts/block_logic.js" );
    if ( temp.length() )
    {
        text_edit_output->setText( "Failed script: " + temp );
        return;
    }

    result = script_engine->evaluate( "indexOfEnd = " + QString::number( -1 ) + ";" );
    if ( result.isError() )
    {
        text_edit_output->setText( "Failed run script: " + result.toString() );
        return;
    }

    result = script_engine->evaluate( "main('run');" );

    if ( result.isError() )
    {
        text_edit_output->setText( "Failed run script: " + result.toString() );
        return;
    }
    script_engine->popContext();
}
