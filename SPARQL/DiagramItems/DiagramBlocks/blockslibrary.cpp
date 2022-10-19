#include "blockslibrary.h"

#include <QDebug>
#include <QDir>
#include <QJsonDocument>

BlocksLibrary::BlocksLibrary()
{
}

void BlocksLibrary::loadBlocksFormFiles( const QString& folder )
{
    QDir dir;
    dir.setPath( folder );
    if ( !dir.exists() )
    {
        return;
    }

    dir.setFilter( QDir::Files | QDir::Dirs );
    QFileInfoList list = dir.entryInfoList();
    QVector<QString> files;
    for ( int i = 0; i < list.size(); i++ )
    {
        if ( list[i].fileName() == "." || list[i].fileName() == ".." )
            continue;

        if ( list[i].isFile() )
        {
            files.push_back( list[i].absoluteFilePath() );
        }
        else
        {
            dir.setPath( "Blocks/" + list[i].fileName() );
            qDebug() << dir.path();
            QFileInfoList lst = dir.entryInfoList();
            for ( int j = 0; j < lst.size(); j++ )
            {
                if ( lst[j].isFile() )
                {
                    files.push_back( lst[j].absoluteFilePath() );
                }
            }
        }
    }

    for ( int i = 0; i < files.size(); i++ )
    {
        QFile file( files[i] );
        if ( file.open( QIODevice::ReadOnly ) )
        {
            QString text = file.readAll();
            QJsonDocument json = QJsonDocument::fromJson( text.toUtf8() );

            QString type = json["type"].toString();
            if ( "basic" == type )
            {
                BasedBlockSettings* setting = new BasedBlockSettings();
                setting->setSettingFromJson( text );
                addBlock( setting );
            }
            // else if ( "sparql" == type )
            //{
            //     SparqlBlockSetting* setting = new SparqlBlockSetting();
            //     setting->setSettingFromJson( text );
            //     virtual_blocks_list.push_back( setting );
            // }
        }
        file.close();
    }
}

void BlocksLibrary::addBlock( DiagramItemSettings* settings )
{
    ModeBlocks mode = getMode( settings );
    /*DiagramItem* item;
    switch ( mode )
    {
    case Based:
        item = new DiagramItemBased( nullptr, static_cast<BasedBlockSettings*>( setting ) );
        break;
    case Composite:
        break;
    case SPARQL:
        break;
    case Fail:
        break;
    default:
        throw "BlocksLibrary item = nullptr";
        break;
    }*/

    library.insert( settings, mode );
}

void BlocksLibrary::addBlocks( QVector<DiagramItemSettings*>& settings_list )
{
    for ( auto settings : settings_list )
    {
        addBlock( settings );
    }
}
/*
void BlocksLibrary::addBlock( DiagramItem* item )
{
    library.insert( item, getMode( item ) );
}

void BlocksLibrary::addBlocks( QVector<DiagramItem*>& items )
{
    for ( auto item : items )
    {
        addBlock( item );
    }
}

void BlocksLibrary::deleteBlock( DiagramItem* item )
{
    library.remove( item );
}
*/
void BlocksLibrary::deleteBlocks( int modes_blocks )
{
    auto block_list = getBlocks( modes_blocks );
    for ( auto block : block_list )
    {
        library.remove( block );
    }
}

QVector<DiagramItemSettings*> BlocksLibrary::getBlocks( int modes_blocks )
{
    if ( library.empty() )
        return {};
    QVector<DiagramItemSettings*> block_list;
    QSet<ModeBlocks> mode_list;
    if ( ( modes_blocks & Based ) )
    {
        mode_list.insert( Based );
    }
    if ( ( modes_blocks & Composite ) )
    {
        mode_list.insert( Composite );
    }
    if ( ( modes_blocks & SPARQL ) )
    {
        mode_list.insert( SPARQL );
    }

    int chel = library.size();
    for ( auto lib : library.keys() )
    {
        if ( mode_list.end() != mode_list.find( library[lib] ) )
        {
            block_list.push_back( lib );
        }
    }
    return block_list;
}

int BlocksLibrary::getSize()
{
    return library.size();
}

BlocksLibrary::ModeBlocks BlocksLibrary::getMode( DiagramItemSettings* settings )
{
    switch ( settings->type() )
    {
    case BasedBlockSettings::Type:
        return Based;
        break;
    // case CompositeBlockSettings::Type:
    //     return Composite
    //     break;
    // case SparqlBlockSettings::Type:
    //     return Composite
    //     break;
    default:
        return Fail;
        break;
    }
}

BlocksLibrary::ModeBlocks BlocksLibrary::getMode( DiagramItem* item )
{
    switch ( item->type() )
    {
    case DiagramItemBased::Type:
        return Based;
        break;
    // case DiagramItemComposite::Type:
    //     return Composite
    //     break;
    // case DiagramItemSparql::Type:
    //     return Composite
    //     break;
    default:
        return Fail;
        break;
    }
}
