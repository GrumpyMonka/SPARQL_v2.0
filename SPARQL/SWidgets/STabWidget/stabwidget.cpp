#include "stabwidget.h"

STabWidget::STabWidget( QWidget *parent )
    : QTabWidget( parent )
{
    CreateTabWidgets();
}

void STabWidget::CreateTabWidgets()
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
                   "}"
                   );
    connect( this, SIGNAL( tabCloseRequested( int ) ),
             this, SLOT( tabClose( int ) ) );
}

void STabWidget::TabClose( int index )
{
    widget( index )->deleteLater();
    removeTab( index );
}
