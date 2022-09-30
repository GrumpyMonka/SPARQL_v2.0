#include "sgraphicsview.h"

#include <QLabel>

SGraphicsView::SGraphicsView( QWidget *parent )
    : QWidget( parent )
{
    setAttribute( Qt::WA_DeleteOnClose );
    CreateDiagramView();
    CreatePositionManager();
}

void SGraphicsView::CreateDiagramView()
{
    gridLayout = new QGridLayout( this );
    setLayout( gridLayout );

    graphicsView = new QGraphicsView( this );
    groupBox = new QGroupBox( this );
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

void SGraphicsView::CreatePositionManager()
{
    /*
    QLabel* label = new QLabel( "Pos", groupBox );
    label->setFont(font);

    x_spin = new QSpinBox(box);
    x_spin->setMaximumWidth(width_box);
    x_spin->setFont(font);
    x_spin->setMaximum(5000);
    x_spin->setMinimum(0);
    connect(x_spin, SIGNAL(valueChanged(int)),
            this, SLOT(updatePosScene()));

    y_spin = new QSpinBox(box);
    y_spin->setMaximumWidth(width_box);
    y_spin->setFont(font);
    y_spin->setMaximum(5000);
    y_spin->setMinimum(0);
    connect(y_spin, SIGNAL(valueChanged(int)),
            this, SLOT(updatePosScene()));

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(x_spin);
    layout->addWidget(y_spin);
    layout->addSpacerItem(new QSpacerItem(0, 2000, QSizePolicy::Maximum, QSizePolicy::Maximum));
    layout->setStretch(10, 10);

    box->setLayout(layout);
    ui->gridLayout->addWidget(box, 0, 1);*/
}
