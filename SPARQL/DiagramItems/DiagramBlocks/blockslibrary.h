#ifndef BLOCKSLIBRARY_H
#define BLOCKSLIBRARY_H

#include <QMap>
#include <QVector>

#include <diagramitemsettings.h>

#define FOLDER_FOR_BLOCKS "Blocks/"

class BlocksLibrary
{
public:
    enum ModeBlocks
    {
        Fail = 0,
        Based = 1,
        Composite = 2,
        SPARQL = 4,
        Atom = 8
    };

    BlocksLibrary();

    void loadBlocksFromFiles( const QString& folder );

    void addBlock( DiagramItemSettings* settings );
    void addBlocks( const QVector<DiagramItemSettings*>& settings_list );
    // void addBlock( DiagramItem* item );
    // void addBlocks( QVector<DiagramItem*>& items );

    // void deleteBlock( DiagramItem* item );
    void deleteBlocks( int modes_blocks );

    QVector<DiagramItemSettings*> getBlocks( int modes_blocks );
    int getSize();

private:
    ModeBlocks getMode( DiagramItemSettings* settings );
    // ModeBlocks getMode( DiagramItem* item );

private:
    QMap<DiagramItemSettings*, ModeBlocks> library;
};

#endif // BLOCKSLIBRARY_H
