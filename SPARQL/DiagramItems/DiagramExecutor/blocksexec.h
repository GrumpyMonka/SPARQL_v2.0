#ifndef BLOCKSEXEC_H
#define BLOCKSEXEC_H

#include <QMap>
#include <QObject>
#include <QScriptEngine>

#include <diagramitem.h>

class BlocksExec : public QObject
{
    Q_OBJECT
public:
    explicit BlocksExec( QObject* parent = nullptr );

    bool checkForWork();
    QScriptValueList checkInputValue();

    void setValueForEngine( const QMap<QString, QObject*>& value_list );

    void setFlagOfWorking( bool flag );
    void setAllowWork( bool flag );
    void setScript( const QString& script );
    void setInputData( const QScriptValueList& input_list );
    void addInputData( const QScriptValue& input );
    void setUserData( const QScriptValue& data );
    void setNextBlocks( const QVector<BlocksExec*>& next_list );
    void addNextBlocks( BlocksExec* next );
    void setPrevBlocks( const QVector<BlocksExec*>& prev_list );
    void addPrevBlocks( BlocksExec* prev );
    void setDiagramItem( DiagramItem* item );
    void setSettings( DiagramItemSettings* settings );
    void setTags( const QMap<QString, QString>& tags );
    void addTag( const QString& key, const QString& value );
    void addBlockConnectName( const QString& tag, BlocksExec* block );

    bool getFlagOfWorking();
    bool getAllowWork();
    QString getScript();
    QScriptValueList getInputData();
    QScriptValue getUserData();
    QScriptValue getOutputData();
    QVector<BlocksExec*> getNextBlocks();
    QVector<BlocksExec*> getPrevBlocks();
    DiagramItem* getDiagramItem();
    DiagramItemSettings* getSettings();
    QMap<QString, QString> getTags();
    QString getTag( const QString& key );
    QString getBlockConnectName( BlocksExec* block );

    void removeConnections();
    void removeConnect( BlocksExec* );
    void disconnectDiagramItem();
    void deleteTags();
    void deleteTag( const QString& key );

    QStringList getLogs();

private:
    QScriptEngine* createEngine();
    void loadScript( QScriptEngine* engine, const QString& path );
    void execScript( QScriptEngine* engine, const QString& script, bool logs_flag = true, bool exception = true );

public slots:
    QString runBlock();

signals:
    void blockFinished();
    void error( QString );
    void logs( QStringList );

private:
    struct BlocksExecArrow
    {
        BlocksExec* startItem;
        BlocksExec* endItem;
        QString nameConnectionStart;
        QString nameConnectionEnd;
    };

    DiagramItem* block = nullptr;
    DiagramItemSettings* block_settings = nullptr;

    bool flag_of_work;
    bool allow_work;

    QString main_script;
    QScriptValueList input_data;
    QScriptValue user_data;
    QScriptValue output_data;

    QMap<QString, QObject*> default_value_list;
    QMap<QString, QString> block_tags;

    QVector<BlocksExecArrow> next_blocks_arrow;
    QVector<BlocksExecArrow> prev_blocks_arrow;
    QMap<BlocksExec*, QString> block_connect_name;

    QStringList logs_exec;
};

#endif // BLOCKSEXEC_H
