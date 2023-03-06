#include "compositeblockwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>

#include <diagramitemcomposite.h>
#include <diagramitemio.h>

CompositeBlockWindow::CompositeBlockWindow( QMenu* context_menu, QWidget* parent )
    : SGraphicsView( context_menu, parent )
{
    createSidePanel();
}

QWidget* CompositeBlockWindow::addCustomWidget()
{
    QWidget* widget = new QWidget( this );
    QGridLayout* grid_layout = new QGridLayout();
    widget->setLayout( grid_layout );

    QLabel* label = new QLabel( "Name block:", this );
    label->setMaximumHeight( 30 );
    line_name_block = new QLineEdit( this );
    line_name_block->setText( "Composite" );

    grid_layout->addWidget( label, 0, 0 );
    grid_layout->addWidget( line_name_block, 1, 0 );
    widget->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );

    return widget;
}

QWidget* CompositeBlockWindow::addCustomBotWidget()
{
    QWidget* widget = new QWidget( this );
    QGridLayout* grid_layout = new QGridLayout();
    widget->setLayout( grid_layout );

    QPushButton* button_create_block = new QPushButton( tr( "Create" ), this );
    connect( button_create_block, SIGNAL( clicked() ), this, SLOT( slotOnCreateButtonClicked() ) );

    QPushButton* button_save_block = new QPushButton( tr( "Save" ), this );
    connect( button_save_block, SIGNAL( clicked() ), this, SLOT( slotOnSaveButtonClicked() ) );

    QPushButton* button_open_block = new QPushButton( tr( "Open" ), this );
    connect( button_open_block, SIGNAL( clicked() ), this, SLOT( slotOnOpenButtonClicked() ) );

    grid_layout->addWidget( button_create_block, 0, 0 );
    grid_layout->addWidget( button_save_block, 1, 0 );
    grid_layout->addWidget( button_open_block, 2, 0 );
    return widget;
}

void CompositeBlockWindow::slotOnCreateButtonClicked()
{
    emit blockCreated( getSettings() );
}

void CompositeBlockWindow::slotOnSaveButtonClicked()
{
    auto settings = getSettings();
    QJsonDocument json;
    json.setObject( settings->getJsonFromSetting() );
    saveFile( json.toJson() );
    delete settings;
}

void CompositeBlockWindow::slotOnOpenButtonClicked()
{
    CompositeBlockSettings* settings = new CompositeBlockSettings();
    settings->setSettingFromString( openFile() );
    setSettings( settings );
}

void CompositeBlockWindow::setSettings( CompositeBlockSettings* settings )
{
    line_name_block->setText( settings->block_name );
    QVector<DiagramItem*> blocks_list;
    for ( auto& block : settings->blocks )
    {
        blocks_list.push_back( DiagramItem::FactoryDiagramItem( nullptr, block ) );
        getScene()->addItem( blocks_list.back() );
        blocks_list.back()->setPos( block->pos );
    }

    for ( auto& line : settings->lines )
    {
        DiagramItem* startItem = blocks_list.at( line.start_block );
        DiagramItem* endItem = blocks_list.at( line.end_block );

        if ( DiagramItem::CompositeItemType == startItem->type() )
        {
            if ( !line.text_start.isEmpty() )
            {
                auto composite_item = static_cast<DiagramItemComposite*>( startItem );
                startItem = composite_item->getOutputBlock( line.text_start );
            }
            else
            {
                emit ERROR( "CompositeBlockWindow::setSettings() -> Line without text" );
            }
        }

        if ( DiagramItem::CompositeItemType == endItem->type() )
        {
            if ( !line.text_end.isEmpty() )
            {
                auto composite_item = static_cast<DiagramItemComposite*>( endItem );
                endItem = composite_item->getInputBlock( line.text_end );
            }
            else
            {
                emit ERROR( "CompositeBlockWindow::setSettings() -> Line without text" );
            }
        }

        getScene()->createArrow( startItem, endItem );
    }
}

QVector<DiagramItem*> CompositeBlockWindow::getDiagramItems()
{
    QVector<DiagramItem*> result;
    auto list = getScene()->items();
    for ( auto item : list )
    {
        if ( DiagramItem::CheckItemOnDiagramItem( item->type() ) )
        {
            result.push_back( static_cast<DiagramItem*>( item ) );
        }
    }
    return result;
}

QVector<DiagramArrow*> CompositeBlockWindow::getDiagramArrows()
{
    QVector<DiagramArrow*> result;
    auto list = getScene()->items();
    for ( auto item : list )
    {
        if ( DiagramItem::DiagramArrowType == item->type() )
        {
            result.push_back( static_cast<DiagramArrow*>( item ) );
        }
    }
    return result;
}

CompositeBlockSettings* CompositeBlockWindow::getSettings()
{
    auto settings = new CompositeBlockSettings();
    settings->block_name = line_name_block->text();
    auto item_list = getDiagramItems();
    auto arrow_list = getDiagramArrows();
    QVector<DiagramItem*> blocks_list;

    for ( auto item : item_list )
    {
        if ( DiagramItem::IOItemType == item->type() )
        {
            if ( nullptr == item->parentItem() )
            {
                auto setting = ( static_cast<DiagramItemIO*>( item ) )->getSettings();
                if ( IOBlockSettings::Input == setting->type_block )
                {
                    settings->input_names.push_back( setting->text );
                }
                else
                {
                    settings->output_names.push_back( setting->text );
                }
                delete setting;
            }
            else
            {
                continue;
            }
        }
        settings->blocks.push_back( item->getSettings() );
        blocks_list.push_back( item );
    }

    for ( auto arrow : arrow_list )
    {
        CompositeBlockSettings::LineSaver line_saver;
        line_saver.start_block = blocks_list.indexOf( arrow->startItem() );
        line_saver.end_block = blocks_list.indexOf( arrow->endItem() );

        if ( DiagramItem::IOItemType == arrow->startItem()->type()
            && nullptr != arrow->startItem()->parentItem() )
        {
            if ( arrow->endItem() == arrow->startItem()->parentItem() )
                continue;
            line_saver.start_block = blocks_list.indexOf( static_cast<DiagramItem*>( arrow->startItem()->parentItem() ) );
            line_saver.text_start = ( static_cast<DiagramItemIO*>( arrow->startItem() ) )->getName();
        }

        if ( DiagramItem::IOItemType == arrow->endItem()->type()
            && nullptr != arrow->endItem()->parentItem() )
        {
            if ( arrow->startItem() == arrow->endItem()->parentItem() )
                continue;
            line_saver.end_block = blocks_list.indexOf( static_cast<DiagramItem*>( arrow->endItem()->parentItem() ) );
            line_saver.text_end = ( static_cast<DiagramItemIO*>( arrow->endItem() ) )->getName();
        }

        settings->lines.push_back( line_saver );
    }

    return settings;
}
