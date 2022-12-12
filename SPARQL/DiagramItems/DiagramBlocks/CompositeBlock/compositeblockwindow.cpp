#include "compositeblockwindow.h"

#include <QPushButton>

#include <diagramitemio.h>

CompositeBlockWindow::CompositeBlockWindow( QWidget* parent )
    : SGraphicsView( parent )
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
    line_name_block->setText( "Sparql" );

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
}

void CompositeBlockWindow::slotOnOpenButtonClicked()
{
}

CompositeBlockSettings* CompositeBlockWindow::getSettings()
{
    CompositeBlockSettings* settings = new CompositeBlockSettings();
    auto full_items = getScene()->items();
    QVector<DiagramItem*> blocks;
    for ( auto item : full_items )
    {
        if ( !DiagramItem::CheckItemOnDiagramItem( item->type() ) )
        {
            continue;
        }

        auto diagram_item = static_cast<DiagramItem*>( item );
        if ( DiagramItem::IOItemType == diagram_item->type() )
        {
            auto setting = ( static_cast<DiagramItemIO*>( diagram_item ) )->getSettings();
            if ( IOBlockSettings::Input == setting->type_block )
            {
                settings->input_names.push_back( setting->text );
            }
            else
            {
                settings->output_names.push_back( setting->text );
            }
        }
        blocks.push_back( diagram_item );
        settings->blocks.push_back( diagram_item->getSettings() );
    }

    for ( auto block : blocks )
    {
        auto lines = block->getStartArrows();
        for ( auto line : lines )
        {
            CompositeBlockSettings::LineSaver line_saver;
            line_saver.start_block = blocks.indexOf( block );
            line_saver.end_block = blocks.indexOf( line->endItem() );
            settings->lines.push_back( line_saver );
        }
    }

    settings->block_name = line_name_block->text();
    return settings;
}
