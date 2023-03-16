#include "blockslibrary.h"

#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include <compositeblocksettings.h>
#include <sparqlblocksettings.h>

BlocksLibrary::BlocksLibrary()
{
}

void BlocksLibrary::loadBlocksFromFiles( const QString& folder )
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
        if ( list.at( i ).fileName() == "." || list.at( i ).fileName() == ".." )
            continue;

        if ( list.at( i ).isFile() )
        {
            files.push_back( list.at( i ).absoluteFilePath() );
        }
        else
        {
            dir.setPath( list.at( i ).absoluteFilePath() );
            qDebug() << dir.path();
            QFileInfoList lst = dir.entryInfoList();
            for ( int j = 0; j < lst.size(); j++ )
            {
                if ( lst.at( j ).isFile() )
                {
                    files.push_back( lst.at( j ).absoluteFilePath() );
                }
            }
        }
    }

    for ( int i = 0; i < files.size(); i++ )
    {
        QFile file( files.at( i ) );
        if ( file.open( QIODevice::ReadOnly ) )
        {
            QString text = file.readAll();
            addBlockFromJson( text );
        }
        file.close();
    }
}

void BlocksLibrary::addBlockFromJson( QString& text )
{
    QJsonDocument json = QJsonDocument::fromJson( text.toUtf8() );
    addBlock( DiagramItemSettings::getDiagramSettingsFromJson( json.object() ) );
}

void BlocksLibrary::addBlock( DiagramItemSettings* settings )
{
    ModeBlocks mode = getMode( settings );
    library.insert( settings, mode );
}

void BlocksLibrary::addBlocks( const QVector<DiagramItemSettings*>& settings_list )
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
    QMap<QString, QMap<QString, DiagramItemSettings*>> block_list;
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
    if ( ( modes_blocks & Atom ) )
    {
        mode_list.insert( Atom );
    }
    if ( ( modes_blocks & IO ) )
    {
        mode_list.insert( IO );
    }

    for ( auto lib : library.keys() )
    {
        if ( mode_list.end() != mode_list.find( library[lib] ) )
        {
            block_list[lib->getNameType()][lib->block_name] = lib;
        }
    }
    QVector<DiagramItemSettings*> vec;
    for ( auto type : block_list )
    {
        for ( auto block : type )
        {
            vec.push_back( block );
        }
    }
    return vec;
}

int BlocksLibrary::getSize()
{
    return library.size();
}

BlocksLibrary::ModeBlocks BlocksLibrary::getMode( DiagramItemSettings* settings )
{
    switch ( settings->typeSettings() )
    {
    case DiagramItemSettings::BasedItemSettingsType:
        return Based;
        break;
    case DiagramItemSettings::CompositeItemSettingsType:
        return Composite;
        break;
    case DiagramItemSettings::SparqlItemSettinsType:
        return SPARQL;
        break;
    case DiagramItemSettings::AtomItemSettingsType:
        return Atom;
        break;
    case DiagramItemSettings::IOItemSettingsType:
        return IO;
        break;
    default:
        return None;
        break;
    }
}
/*
BlocksLibrary::ModeBlocks BlocksLibrary::getMode( DiagramItem* item )
{
    switch ( item->type() )
    {
    case DiagramItemBased:
        return Based;
        break;
    // case DiagramItemComposite:
    //     return Composite
    //     break;
    // case DiagramItemSparql:
    //     return Composite
    //     break;
    default:
        return Fail;
        break;
    }
}
*/
