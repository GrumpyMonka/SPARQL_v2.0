#include "stabwidget.h"

#include <projectwindow.h>

STabWidget::STabWidget( QWidget* parent )
    : QTabWidget( parent )
{
    createTabWidgets();
}

void STabWidget::addWidget( QWidget* widget, int mode, const QString& name )
{
    widget_lib.insert( widget, mode );
    addTab( widget, name );
    setCurrentWidget( widget );
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

void STabWidget::resetBlocks()
{
    slotCurrentTab( currentIndex() );
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

void STabWidget::itemForAdd( DiagramItemSettings* settings )
{
    ( static_cast<SGraphicsView*>( currentWidget() ) )->setItemForScene( settings );
}
void STabWidget::setSceneMode( int mode )
{
    if ( nullptr != currentWidget() )
        ( static_cast<SGraphicsView*>( currentWidget() ) )->setSceneMode( mode );
}

QVector<DiagramItem*> STabWidget::getBlocksForRun()
{
    if ( ProjectWindow::Type == ( static_cast<SGraphicsView*>( currentWidget() )->type() ) )
    {
        auto window = static_cast<ProjectWindow*>( currentWidget() );
        return window->getDiagramItems();
    }
    else
    {
        return {};
    }
}
