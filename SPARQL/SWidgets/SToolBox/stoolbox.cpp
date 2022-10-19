#include "stoolbox.h"

#include <QPushButton>
#include <QToolButton>

SToolBox::SToolBox( QWidget* parent )
    : QToolBox( parent )
{
    createToolButtonGroup();
}

void SToolBox::createToolButtonGroup()
{
    buttonGroup = new QButtonGroup( this );
    buttonGroup->setExclusive( false );
    connect( buttonGroup, SIGNAL( buttonClicked( int ) ),
        this, SLOT( buttonGroupClicked( int ) ) );

    QWidget* buttonGroupWidget = new QWidget( this );
    buttonLayout = new QGridLayout( buttonGroupWidget );
    buttonGroupWidget->setLayout( buttonLayout );

    setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Ignored ) );
    setMinimumWidth( buttonGroupWidget->sizeHint().width() );
    addItem( buttonGroupWidget, tr( "Blocks" ) );
    setDiagramItems( {} );
}

void SToolBox::buttonGroupClicked( int pos )
{
    emit itemPressed( settings_list[pos] );
}

void SToolBox::setDiagramItems( const QVector<DiagramItemSettings*>& items )
{
    deleteDiagramItems( settings_list );

    int size_list = items.size();
    addDiagramItems( items );

    // BasedBlockSettings* setting = new BasedBlockSetting();
    // setting->name = "";
    while ( size_list < 3 )
    {
        addDiagramItem( nullptr );
        size_list++;
    }
}

void SToolBox::addDiagramItem( DiagramItemSettings* item )
{
    createCellWidget( item );
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
    widget_list.remove( pos );
    settings_list.remove( pos );
}

void SToolBox::deleteDiagramItems( const QVector<DiagramItemSettings*>& items )
{
    for ( auto item : items )
    {
        deleteDiagramItem( item );
    }
}

void SToolBox::createCellWidget( DiagramItemSettings* settings, bool addButtonGroup )
{
    QIcon icon;
    QString name;
    if ( nullptr == settings )
    {
        name = "";
    }
    /*else if ( BasedBlockSetting::Type == settingf->type() )
    {
        BasedBlockSetting* setting = ( BasedBlockSetting* )( settingf );
        name = setting->name;
        icon = QIcon( setting->image.scaled( 50, 50 ) );
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
    }*/

    QToolButton* button = new QToolButton;
    button->setIcon( icon );
    button->setIconSize( QSize( 50, 50 ) );
    button->setCheckable( true );
    if ( addButtonGroup )
        buttonGroup->addButton( button, buttonGroup->buttons().size() );

    QGridLayout* layout = new QGridLayout;
    layout->addWidget( button, 0, 0, Qt::AlignHCenter );
    layout->addWidget( new QLabel( name ), 1, 0, Qt::AlignCenter );

    QWidget* widget = new QWidget;
    widget->setLayout( layout );

    addWidget( settings, widget );
}

void SToolBox::addWidget( DiagramItemSettings* settings, QWidget* widget )
{
    buttonLayout->addWidget( widget );
    widget_list.push_back( widget );
    settings_list.push_back( settings );
}
