#ifndef DIAGRAMEXECUTOR_H
#define DIAGRAMEXECUTOR_H

#include <QScriptEngine>
#include <QTextEdit>
#include <QWidget>

#include <diagramitem.h>

class DiagramExecutor : public QWidget
{
    Q_OBJECT
public:
    explicit DiagramExecutor( QWidget* parent = nullptr );

    void setScript( QString& );

    static QString ConvertDiagramItemToScript( QVector<DiagramItemSettings*>& );

private:
    void createWindow();
    QString loadScript( QString path );

private slots:
    void execute();

private:
    QScriptEngine* script_engine;
    QTextEdit* text_edit_script;
    QTextEdit* text_edit_output;
};

#endif // DIAGRAMEXECUTOR_H
