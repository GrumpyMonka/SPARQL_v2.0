#ifndef SPARQLBLOCKWINDOW_H
#define SPARQLBLOCKWINDOW_H

#include <sgraphicsview.h>
#include <sparqlblocksettings.h>

class SparqlBlockWindow : public SGraphicsView
{
public:
    enum
    {
        DiagramMode = 10
    };

    explicit SparqlBlockWindow( QWidget* parent = nullptr );

    void setSettings( const SparqlBlockSettings& settings = SparqlBlockSettings() );

    SparqlBlockSettings* getSettings();

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;

    void createDefaultcScene();
    void clearScene();
};

#endif // SPARQLBLOCKWINDOW_H
