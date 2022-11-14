#ifndef ATOMBLOCKSETTINGS_H
#define ATOMBLOCKSETTINGS_H

#include <QPolygonF>

#include <diagramitemsettings.h>

#define DEFAULT_AREA "AREA"
#define DEFAULT_VAR "VAR"
#define DEFAULT_VALUE "VALUE"

class AtomBlockSettings : public DiagramItemSettings
{
public:
    enum
    {
        Type = 4
    };
    int type() override { return Type; }

    AtomBlockSettings();

    QPolygonF polygon;

    bool flag_text;
    QString text;

    QColor color;
    QString color_text;

    QString type_block;

    bool transparent;

    void setSettingFromJson( const QJsonValue& value ) override;
    QJsonObject getJsonFromSetting() override;
    QPixmap image() const override;

    static QVector<DiagramItemSettings*> GetBasedAtomBlocks();
    static QPolygonF GetDefaultAreaPolygon();
};

#endif // ATOMBLOCKSETTINGS_H
