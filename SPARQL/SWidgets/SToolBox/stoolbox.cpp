#include "stoolbox.h"

#include <QLabel>
#include <QPushButton>
#include <QToolButton>

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

    parent->addItem( widget, name );
}

void SToolBox::Box::deleteBox()
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

void SToolBox::buttonGroupClicked()
{
    auto keys = button_group.keys();
    for ( auto button : keys )
    {
        if ( sender() == button )
        {
            emit itemPressed( button_group[button] );
        }
        else
        {
            button->setChecked( false );
        }
    }
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

    QWidget* widget = new QWidget( this );
    QToolButton* button = new QToolButton( widget );
    button->setIcon( icon );
    button->setIconSize( QSize( SIZE, SIZE ) );
    button->setCheckable( true );
    button_group[button] = item;
    connect( button, SIGNAL( clicked() ), this, SLOT( buttonGroupClicked() ) );

    QGridLayout* layout = new QGridLayout;
    layout->addWidget( button, 0, 0, Qt::AlignHCenter );
    auto temp_label = new QLabel( name );
    temp_label->setMaximumWidth( 50 );
    layout->addWidget( temp_label, 1, 0, Qt::AlignCenter );

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
    for ( auto box : settings_list )
    {
        box.deleteBox();
    }
    settings_list.clear();
    button_group.clear();
    while ( 0 != count() )
    {
        removeItem( 0 );
    }
}

void SToolBox::addWidget( DiagramItemSettings* settings, QWidget* widget )
{
    if ( settings_list.end() == settings_list.find( settings->getNameType() ) )
    {
        settings_list.insert( settings->getNameType(), Box( settings->getNameType(), this ) );
    }
    auto* box = &settings_list[settings->getNameType()];
    int size = box->settings.size();
    int row = size / COUNT_COLUMN;
    int column = size % COUNT_COLUMN;
    box->layout->addWidget( widget, row, column );
    box->settings[widget] = settings;
}
