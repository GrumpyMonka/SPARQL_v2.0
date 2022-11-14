#include "stoolbox.h"

#include <QLabel>
#include <QPushButton>
#include <QToolButton>

#include <compositeblocksettings.h>
#include <sparqlblocksettings.h>

SToolBox::SToolBox( QWidget* parent )
    : QToolBox( parent )
{
    createToolButtonGroup();
}

void SToolBox::createToolButtonGroup()
{
    button_group = new QButtonGroup( this );
    button_group->setExclusive( false );
    connect( button_group, SIGNAL( buttonClicked( int ) ),
        this, SLOT( buttonGroupClicked( int ) ) );

    QWidget* buttonGroupWidget = new QWidget( this );
    button_layout = new QGridLayout( buttonGroupWidget );
    button_layout->setRowStretch( 10, 10 );
    button_layout->setColumnStretch( 3, 10 );
    buttonGroupWidget->setLayout( button_layout );
    button_layout->setSpacing( 0 );
    button_layout->setMargin( 0 );

    for ( int i = 0; i < 3; ++i )
    {
        addDiagramItem( nullptr, false );
    }

    for ( int i = 0; i < 3; ++i )
    {
        settings_list.removeLast();
        widget_list.removeLast();
    }

    setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Ignored ) );
    setMinimumWidth( 230 );
    addItem( buttonGroupWidget, tr( "Blocks" ) );
}

void SToolBox::buttonGroupClicked( int pos )
{
    auto button_list = button_group->buttons();
    for ( int i = 0; i < button_list.size(); ++i )
    {
        if ( pos != i )
            button_list.at( i )->setChecked( false );
    }
    emit itemPressed( settings_list.at( pos ) );
}

void SToolBox::setDiagramItems( const QVector<DiagramItemSettings*>& items )
{
    deleteDiagramItems( settings_list );

    addDiagramItems( items );
}

void SToolBox::addDiagramItem( DiagramItemSettings* item, bool addButtonGroup )
{
    QIcon icon;
    QString name;
    if ( nullptr == item )
    {
        name = "";
    }
    else
    {
        name = item->block_name;
        icon = QIcon( item->pixmap.scaled( SIZE, SIZE ) );
    }
    /*
        else if ( BasedBlockSettings::Type == item->type() )
        {
            BasedBlockSettings* setting = static_cast<BasedBlockSettings*>( item );
            name = setting->block_name;
            icon = QIcon( setting->pixmap.scaled( SIZE, SIZE ) );
        }

        else if ( CompositeBlockSetting::Type == settingf->type() )
        {
        }
        else if ( SparqlBlockSetting::Type == settingf->type() )
        {
            SparqlBlockSetting* setting = ( SparqlBlockSetting* )( settingf );
            name = setting->name;
            QPixmap pixmap( ":/images/sparqlicon.jpg" );
            icon = QIcon( pixmap.scaled( 50, 50 ) );
        }
        else if ( AtomBlockSettings::Type == item->type() )
        {
            AtomBlockSettings* setting = static_cast<AtomBlockSettings*>( item );

        }
        */

    QToolButton* button = new QToolButton;
    button->setIcon( icon );
    button->setIconSize( QSize( SIZE, SIZE ) );
    button->setCheckable( true );
    if ( addButtonGroup )
        button_group->addButton( button, button_group->buttons().size() );

    QGridLayout* layout = new QGridLayout;
    layout->addWidget( button, 0, 0, Qt::AlignHCenter );
    auto temp_label = new QLabel( name );
    temp_label->setMaximumWidth( 50 );
    layout->addWidget( temp_label, 1, 0, Qt::AlignCenter );

    QWidget* widget = new QWidget;
    widget->setLayout( layout );

    if ( !addButtonGroup )
        widget->setEnabled( false );

    addWidget( item, widget );
}

void SToolBox::addDiagramItems( const QVector<DiagramItemSettings*>& items )
{
    for ( auto item : items )
    {
        addDiagramItem( item );
    }
}

void SToolBox::deleteDiagramItem( DiagramItemSettings* item )
{
    auto pos = settings_list.indexOf( item );
    widget_list.at( pos )->deleteLater();
    widget_list.remove( pos );
    settings_list.remove( pos );
    button_group->removeButton( button_group->buttons().at( pos ) );
}

void SToolBox::deleteDiagramItems( const QVector<DiagramItemSettings*> items )
{
    for ( auto item : items )
    {
        deleteDiagramItem( item );
    }
}

void SToolBox::addWidget( DiagramItemSettings* settings, QWidget* widget )
{
    int size = widget_list.size();
    int row = size / COUNT_COLUMN;
    int column = size % COUNT_COLUMN;
    button_layout->addWidget( widget, row, column );
    widget_list.push_back( widget );
    settings_list.push_back( settings );
}
