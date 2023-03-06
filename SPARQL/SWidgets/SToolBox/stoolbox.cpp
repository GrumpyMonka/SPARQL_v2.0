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
    QWidget* buttonGroupWidget = new QWidget( parent );
    layout = new QGridLayout( buttonGroupWidget );
    layout->setRowStretch( 10, 10 );
    layout->setColumnStretch( 3, 10 );
    buttonGroupWidget->setLayout( layout );
    layout->setSpacing( 0 );
    layout->setMargin( 0 );

    for ( int i = 0; i < 3; ++i )
    {
        // parent->addDiagramItem( nullptr, false );
    }

    parent->addItem( buttonGroupWidget, str );
}

void SToolBox::createToolButtonGroup()
{
    if ( settings_list.end() == settings_list.find( "kek" ) )
    {
        Box box( "kek", this );
        settings_list.insert( "kek", box );
    }
    if ( settings_list.end() == settings_list.find( "lol" ) )
    {
        Box box( "lol", this );
        settings_list.insert( "lol", box );
    }
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
    // deleteDiagramItems( settings_list );

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
    /*
    auto pos = settings_list.indexOf( item );
    widget_list.at( pos )->deleteLater();
    widget_list.remove( pos );
    settings_list.remove( pos );
    button_group->removeButton( button_group->buttons().at( pos ) );
    */
}

void SToolBox::deleteDiagramItems( const QVector<DiagramItemSettings*> items )
{
    for ( auto item : items )
    {
        // deleteDiagramItem( item );
    }
}

void SToolBox::addWidget( DiagramItemSettings* settings, QWidget* widget )
{
    auto box = settings_list[settings->getNameType()];
    int size = box.settings.size();
    int row = size / COUNT_COLUMN;
    int column = size % COUNT_COLUMN;
    box.layout->addWidget( widget, row, column );
    box.settings[widget] = settings;
}
