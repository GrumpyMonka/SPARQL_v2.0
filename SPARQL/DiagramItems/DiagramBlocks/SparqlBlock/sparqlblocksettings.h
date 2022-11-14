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

    struct LineSaver
    {
        int startBlock;
        int endBlock;
        QString text;
    };

    struct BlockSaver
    {
        QString text;
        QPointF pos;
        QString path;
        QString type;
    };

    struct AreaSaver
    {
        QPolygonF polygon;
        QPointF pos;
        QString name;
    };

    int limit;

    QVector<BlockSaver> blocks;
    QVector<LineSaver> lines;

    QVector<AreaSaver> areas;

    AtomBlockSettings setting_var;
    AtomBlockSettings setting_value;
    AtomBlockSettings setting_area;

    void setSettingFromJson( const QJsonValue& value ) override;
    QJsonObject getJsonFromSetting() override;
    QPixmap image() const override;

    static BasedBlockSettings* ConvertToBasedBlockSetting( SparqlBlockSettings* );
    static SparqlBlockSettings* CreateTemplateSparqlSettings();
};

#endif // SPARQLBLOCKSETTINGS_H
