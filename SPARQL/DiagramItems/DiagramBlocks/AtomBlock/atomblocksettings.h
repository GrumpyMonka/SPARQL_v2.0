#ifndef ATOMBLOCKSETTINGS_H
#define ATOMBLOCKSETTINGS_H

#include <QPolygonF>

#include <diagramitemsettings.h>

class AtomBlockSettings : public DiagramItemSettings
{
public:
    enum
    {
        Type = 4
    };
    int type() override { return Type; }

    AtomBlockSettings();

    QString block_name;

    QPolygonF polygon;

    bool flag_text;
    QString text;

    QColor color;
    QString color_text;

    bool transparent;

    void setSettingFromJson( const QJsonValue& value ) override;
    QJsonObject getJsonFromSetting() override;
    QPixmap image() const override;

    static QVector<DiagramItemSettings*> GetBasedAtomBlocks();
};

#endif // ATOMBLOCKSETTINGS_H
