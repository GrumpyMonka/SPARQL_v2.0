#include "stabwidget.h"

STabWidget::STabWidget( QWidget* parent )
    : QTabWidget( parent )
{
    createTabWidgets();
}

void STabWidget::addWidget( QWidget* widget, int mode, const QString& name )
{
    addTab( widget, name );
    setCurrentWidget( widget );
    widget_lib.insert( widget, mode );
}

void STabWidget::createTabWidgets()
{
    setTabShape( QTabWidget::Triangular );
    setTabsClosable( true );
    setUsesScrollButtons( true );
    setMovable( true );
    setFont( QFont( "Consolas", 12 ) );
    setStyleSheet( "QTabBar::tab "
                   "{ "
                   "    height: 25px; "
                   "    width: 175px; "
                   "}" );
    connect( this, SIGNAL( tabCloseRequested( int ) ),
        this, SLOT( tabClose( int ) ) );
    connect( this, SIGNAL( currentChanged( int ) ),
        this, SLOT( slotCurrentTab( int ) ) );
}

void STabWidget::slotCurrentTab( int index )
{
    if ( nullptr == widget( index ) )
        return;
    int mode = widget_lib[widget( index )];
    emit currentMode( mode );
}

void STabWidget::tabClose( int index )
{
    widget( index )->deleteLater();
    removeTab( index );
}
