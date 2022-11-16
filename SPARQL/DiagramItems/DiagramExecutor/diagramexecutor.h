#ifndef DIAGRAMEXECUTOR_H
#define DIAGRAMEXECUTOR_H

#include <QScriptEngine>
#include <QTextEdit>

#include <diagramitem.h>
#include <swidget.h>

class DiagramExecutor : public SWidget
{
    Q_OBJECT
public:
    explicit DiagramExecutor( QWidget* parent = nullptr );

    void setScript( const QString& );

    static QString ConvertDiagramItemToScript( QVector<DiagramItem*>& );

private:
    void createWindow();
    QString loadScript( const QString& path );

private slots:
    void execute();

private:
    QScriptEngine* script_engine;
    QTextEdit* text_edit_script;
    QTextEdit* text_edit_output;
};

#endif // DIAGRAMEXECUTOR_H
