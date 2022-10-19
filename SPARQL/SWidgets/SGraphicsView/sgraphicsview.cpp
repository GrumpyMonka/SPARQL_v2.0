#include "sgraphicsview.h"

#include <QLabel>

SGraphicsView::SGraphicsView( QWidget* parent )
    : QWidget( parent )
{
    setAttribute( Qt::WA_DeleteOnClose );
    CreateDiagramView();
}

void SGraphicsView::CreateDiagramView()
{
    gridLayout = new QGridLayout( this );
    setLayout( gridLayout );

    graphicsView = new QGraphicsView( this );
    groupBox = new QGroupBox( this );
    groupBox->setMaximumWidth( 220 );

    gridLayout->addWidget( graphicsView, 0, 0 );
    gridLayout->addWidget( groupBox, 0, 1 );

    SetDiagramScene( CreateDiagramScene() );
}

void SGraphicsView::SetDiagramScene( DiagramScene* scene )
{
    diagramScene = scene;
    graphicsView->setScene( diagramScene );
}

DiagramScene* SGraphicsView::CreateDiagramScene()
{
    return new DiagramScene( graphicsView );
}

void SGraphicsView::CreateSidePanel()
{
    QLabel* label = new QLabel( "Pos", groupBox );

    xSpinBox = new QSpinBox( groupBox );
    xSpinBox->setMaximum( widthScene );
    xSpinBox->setMinimum( 0 );
    connect( xSpinBox, SIGNAL( valueChanged( int ) ),
        this, SLOT( updatePosItems() ) );

    ySpinBox = new QSpinBox( groupBox );
    ySpinBox->setMaximum( widthScene );
    ySpinBox->setMinimum( 0 );
    connect( ySpinBox, SIGNAL( valueChanged( int ) ),
        this, SLOT( updatePosItems() ) );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( label );
    layout->addWidget( xSpinBox );
    layout->addWidget( ySpinBox );

    groupBoxLayout = new QVBoxLayout( groupBox );
    groupBoxLayout->addLayout( layout, 0 );
    groupBoxLayout->addWidget( AddCustomWidget(), 1 );
    groupBoxLayout->addSpacerItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
    groupBoxLayout->addWidget( AddCustomBotWidget() );

    groupBox->setLayout( groupBoxLayout );
}

void SGraphicsView::updatePosData()
{
}

void SGraphicsView::updatePosItems()
{
}
