#ifndef SPARQLBLOCKSETTINGS_H
#define SPARQLBLOCKSETTINGS_H

#include <atomblocksettings.h>
#include <basedblocksettings.h>

class SparqlBlockSettings : public DiagramItemSettings
{
public:
    enum
    {
        Type = 3
    };

    int type() override { return Type; }

    SparqlBlockSettings();
    ~SparqlBlockSettings();

    struct LineSaver
    {
        int start_block;
        int end_block;
        QString text;
    };

    struct BlockSaver
    {
        QString text;
        QPointF pos;
        QString type;
    };

    struct AreaSaver
    {
        QVector<AtomBlockSettings*> blocks;
        QVector<LineSaver> lines;
        AtomBlockSettings* settings;
    };

    int limit;

    QVector<LineSaver> lines;

    QVector<AreaSaver> areas;
    int start_area;

    void setSettingFromJson( const QJsonValue& value ) override;
    QJsonObject getJsonFromSetting() override;
    QPixmap image() const override;
    QJsonArray getJsonArrayFromLineSaver( const QVector<LineSaver>& );
    void clear();

    static BasedBlockSettings* ConvertToBasedBlockSetting( SparqlBlockSettings* );
    static SparqlBlockSettings* CreateTemplateSparqlSettings();
};

#endif // SPARQLBLOCKSETTINGS_H
