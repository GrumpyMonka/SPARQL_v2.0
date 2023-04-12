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
    int typeSettings() const override { return AtomItemSettingsType; }

    AtomBlockSettings();

    QPolygonF polygon;

    bool flag_text;
    QString text;

    QColor color;
    QString color_text;

    QString type_block;
    QString filter;

    bool transparent_background;

    void setSettingFromJson( const QJsonObject& object ) override;
    QJsonObject getJsonFromSetting() override;
    QPixmap image() const override;

    static QVector<DiagramItemSettings*> GetBasedAtomBlocks();
    static AtomBlockSettings* GetAreaAtomBlock();
    static AtomBlockSettings* GetVarAtomBlock();
    static AtomBlockSettings* GetValueAtomBlock();
    static QPolygonF GetDefaultAreaPolygon();
};

#endif // ATOMBLOCKSETTINGS_H
