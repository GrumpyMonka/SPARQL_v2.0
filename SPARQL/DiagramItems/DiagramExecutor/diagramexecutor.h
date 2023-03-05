#ifndef DIAGRAMEXECUTOR_H
#define DIAGRAMEXECUTOR_H

#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPixmap>
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
    void paint();

private slots:
    void
    execute();
    void logs_sniff( QStringList );

private:
    QVector<DiagramItem*> items_for_run;
    QVector<BlocksExec*> blocks_exec_list;

    // QTextEdit* text_edit_script;
    QString path;
    QTextEdit* text_edit_output;
    QLabel* label_pixmap;
    QGraphicsView* graphics_view;
    SNetwork* network_api;
    ApiJS* api;
};

#endif // DIAGRAMEXECUTOR_H
