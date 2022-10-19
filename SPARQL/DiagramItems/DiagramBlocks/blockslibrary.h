#ifndef BLOCKSLIBRARY_H
#define BLOCKSLIBRARY_H

#include <QMap>
#include <QVector>

#include <diagramitembased.h>
#include <diagramitemcomposite.h>

#define FOLDER_FOR_BLOCKS "Blocks"

class BlocksLibrary
{
public:
    enum ModeBlocks
    {
        Fail = 0,
        Based = 1,
        Composite = 2,
        SPARQL = 4
    };

    BlocksLibrary();

    void loadBlocksFormFiles( const QString& folder );

    void addBlock( DiagramItemSettings* settings );
    void addBlocks( QVector<DiagramItemSettings*>& settings_list );
    // void addBlock( DiagramItem* item );
    // void addBlocks( QVector<DiagramItem*>& items );

    // void deleteBlock( DiagramItem* item );
    void deleteBlocks( int modes_blocks );

    QVector<DiagramItemSettings*> getBlocks( int modes_blocks );
    int getSize();

private:
    ModeBlocks getMode( DiagramItemSettings* settings );
    ModeBlocks getMode( DiagramItem* item );

private:
    QMap<DiagramItemSettings*, ModeBlocks> library;
};

#endif // BLOCKSLIBRARY_H
