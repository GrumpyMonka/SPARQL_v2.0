#include "stoolbox.h"

#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <graphviz/geom.h>

SToolBox::SToolBox( QWidget* parent )
    : QToolBox( parent )
{
    createToolButtonGroup();
}

SToolBox::Box::Box( const QString& str, SToolBox* parent )
{
    name = str;
    widget = new QWidget( parent );
    layout = new QGridLayout( widget );
    layout->setRowStretch( 10, 10 );
    layout->setColumnStretch( 3, 10 );
    widget->setLayout( layout );
    layout->setSpacing( 0 );
    layout->setMargin( 0 );

    for ( int i = 0; i < 3; ++i )
    {
        // parent->addDiagramItem( nullptr, false );
    }
    parent->addItem( widget, name );
}

SToolBox::Box::~Box()
{
    for ( auto widget : settings.keys() )
    {
        widget->deleteLater();
    }
}

void SToolBox::createToolButtonGroup()
{
    setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Ignored ) );
    setMinimumWidth( 230 );
}

void SToolBox::buttonGroupClicked( int pos )
{
    auto button_list = button_group->buttons();
    for ( int i = 0; i < button_list.size(); ++i )
    {
        if ( pos != i )
            button_list.at( i )->setChecked( false );
    }
    // emit itemPressed( settings_list.at( pos ) );
}

void SToolBox::setDiagramItems( const QVector<DiagramItemSettings*>& items )
{
    deleteAll();

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

    QToolButton* button = new QToolButton;
    button->setIcon( icon );
    button->setIconSize( QSize( SIZE, SIZE ) );
    button->setCheckable( true );
    // if ( addButtonGroup )
    // button_group->addButton( button, button_group->buttons().size() );

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
    // for ( auto item : settings_list )
    //{
    //     addItem( item.widget, item.name );
    // }
}

void SToolBox::deleteDiagramItem( DiagramItemSettings* item )
{
    auto widget = settings_list[item->getNameType()].settings.key( item );
    settings_list[item->getNameType()].settings.remove( widget );
    widget->deleteLater();
    // button_group->removeButton( button_group->buttons().at( pos ) );
}

void SToolBox::deleteDiagramItems( const QVector<DiagramItemSettings*> items )
{
    for ( auto item : items )
    {
        deleteDiagramItem( item );
    }
}

void SToolBox::deleteAll()
{
    settings_list.clear();
    while ( 0 != count() )
    {
        removeItem( 0 );
    }
}

void SToolBox::addWidget( DiagramItemSettings* settings, QWidget* widget )
{
    if ( settings_list.end() == settings_list.find( settings->getNameType() ) )
    {
        settings_list[settings->getNameType()] = Box( settings->getNameType(), this );
    }
    auto* box = &settings_list[settings->getNameType()];
    int size = box->settings.size();
    int row = size / COUNT_COLUMN;
    int column = size % COUNT_COLUMN;
    box->layout->addWidget( widget, row, column );
    box->settings[widget] = settings;
}
