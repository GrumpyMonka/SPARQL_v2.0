#ifndef DIAGRAMEXECUTOR_H
#define DIAGRAMEXECUTOR_H

#include <QScriptEngine>
#include <QTextEdit>

#include <apijs.h>
#include <blocksexec.h>
#include <diagramitem.h>
#include <snetwork.h>
#include <swidget.h>

class DiagramExecutor : public SWidget
{
    Q_OBJECT
public:
    explicit DiagramExecutor( QWidget* parent = nullptr );

    void setDiagramItem( QVector<DiagramItem*>& );

signals:
    void ERROR( QString );

private:
    void createWindow();
    BlocksExec* createBlocksExecObject( DiagramItemSettings* );

private slots:
    void
    execute();
    void logs_sniff( QStringList );

private:
    QVector<DiagramItem*> items_for_run;
    QVector<BlocksExec*> blocks_exec_list;

    QTextEdit* text_edit_script;
    QTextEdit* text_edit_output;
    SNetwork* network_api;
    ApiJS* api;
};

#endif // DIAGRAMEXECUTOR_H
