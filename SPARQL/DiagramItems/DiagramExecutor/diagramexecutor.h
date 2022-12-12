#ifndef DIAGRAMEXECUTOR_H
#define DIAGRAMEXECUTOR_H

#include <QScriptEngine>
#include <QTextEdit>

#include <apijs.h>
#include <diagramitem.h>
#include <snetwork.h>
#include <swidget.h>

class DiagramExecutor : public SWidget
{
    Q_OBJECT
public:
    explicit DiagramExecutor( QWidget* parent = nullptr );

    void setScript( const QString& );

    QString ConvertDiagramItemToScript( QVector<DiagramItem*>& );

private:
    void createWindow();
    QString loadScript( const QString& path );

private slots:
    void execute();

private:
    QScriptEngine* script_engine;
    QTextEdit* text_edit_script;
    QTextEdit* text_edit_output;
    SNetwork* network_api;
    ApiJS* api;
};

#endif // DIAGRAMEXECUTOR_H
