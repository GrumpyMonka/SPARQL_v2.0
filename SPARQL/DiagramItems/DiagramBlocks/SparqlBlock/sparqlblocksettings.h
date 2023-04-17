#ifndef SPARQLBLOCKSETTINGS_H
#define SPARQLBLOCKSETTINGS_H

#include <atomblocksettings.h>
#include <basedblocksettings.h>

class SparqlBlockSettings : public DiagramItemSettings
{
public:
    int typeSettings() const override { return SparqlItemSettinsType; }

    SparqlBlockSettings();
    ~SparqlBlockSettings();

    struct LineSaver
    {
        int start_block;
        int end_block;
        QString text;
    };

    struct AreaSaver
    {
        QVector<AtomBlockSettings*> blocks;
        QVector<LineSaver> lines;
        AtomBlockSettings* settings;
        QString filter;
    };

    int limit;

    QVector<LineSaver> lines;

    QVector<AreaSaver> areas;
    int start_area;
    QString query;

    void setSettingFromJson( const QJsonObject& object ) override;
    QJsonObject getJsonFromSetting() override;
    QPixmap image() const override;
    QJsonArray getJsonArrayFromLineSaver( const QVector<LineSaver>& );
    void clear();
    QString getScript() override;
    QString getQuery();

    static SparqlBlockSettings* CreateTemplateSparqlSettings();
};

#endif // SPARQLBLOCKSETTINGS_H
