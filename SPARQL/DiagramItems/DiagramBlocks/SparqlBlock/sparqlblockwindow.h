#ifndef SPARQLBLOCKWINDOW_H
#define SPARQLBLOCKWINDOW_H

#include <atomblocksettings.h>
#include <sgraphicsview.h>

class SparqlBlockWindow : public SGraphicsView
{
public:
    enum
    {
        DiagramMode = 10
    };

    explicit SparqlBlockWindow( QWidget* parent = nullptr );

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;
};

#endif // SPARQLBLOCKWINDOW_H
