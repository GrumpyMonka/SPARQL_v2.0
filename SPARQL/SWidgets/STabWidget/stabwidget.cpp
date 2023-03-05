#include "stabwidget.h"

#include <projectwindow.h>

STabWidget::STabWidget( QWidget* parent )
    : QTabWidget( parent )
{
    createTabWidgets();
}

void STabWidget::addWidget( SWidget* widget, const QString& name )
{
    widget_lib.insert( widget, widget->modeDiagramBlocks() );
    addTab( widget, name );
    setCurrentWidget( widget );
    connect( widget, SIGNAL( setTabName( QString ) ), this, SLOT( setTabName( QString ) ) );
}

void STabWidget::setTabName( QString name )
{
    setTabText( indexOf( static_cast<QWidget*>( sender() ) ), name );
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
                   "    width: 250px; "
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
    emit newCurrentMode( mode );
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
    if ( SWidget::SGraphicsViewType == currentSWidget()->typeSWidget()
        && SGraphicsView::ProjectWindowType == ( static_cast<SGraphicsView*>( currentSWidget() ) )->typeSGView() )
    {
        auto window = static_cast<ProjectWindow*>( currentWidget() );
        return window->getDiagramItems();
    }
    else
    {
        return {};
    }
}

SWidget* STabWidget::currentSWidget()
{
    auto widget = currentWidget();
    return static_cast<SWidget*>( widget );
}

void STabWidget::deleteItemOnScene()
{
    auto widget = currentSWidget();
    if ( SWidget::SGraphicsViewType == widget->typeSWidget() )
    {
        ( static_cast<SGraphicsView*>( widget ) )->removeSelectedGraphicsItems();
    }
}

void STabWidget::saveProject()
{
    auto widget = currentSWidget();
    if ( nullptr != widget )
    {
        widget->saveProject();
    }
}

void STabWidget::openProject()
{
    auto widget = currentSWidget();
    if ( nullptr != widget )
    {
        widget->openProject();
    }
}
