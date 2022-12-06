#ifndef DIAGRAMITEMSETTINGS_H
#define DIAGRAMITEMSETTINGS_H

#include <QJsonValue>
#include <QPixmap>
#include <QString>

class DiagramItemSettings
{
public:
    enum
    {
        DiagramItemSettingsType = 0,
        BasedItemSettingsType = 2,
        CompositeItemSettingsType = 4,
        SparqlItemSettinsType = 6,
        AtomItemSettingsType = 8,
        ProjectWindowSettingsType = 10
    };
    virtual int typeSettings() const { return DiagramItemSettingsType; }

    DiagramItemSettings();

    QString block_name;
    QPixmap pixmap;
    bool support_add_item;
    int z_value;
    QPointF pos;

    virtual void setSettingFromJson( const QJsonObject& object ) = 0;
    virtual QJsonObject getJsonFromSetting() = 0;
    virtual QPixmap image() const;
    virtual ~DiagramItemSettings() = default;

    void setSettingFromString( const QString& str );

    QJsonValue jsonValFromPixmap( const QPixmap& p );
    QPixmap pixmapFrom( const QJsonValue& value );
    QJsonArray jsonArrayFromPolygon( const QPolygonF& polygon );
    QPolygonF polygonFromJsonArray( const QJsonArray& array );
    QJsonObject jsonFromPointF( const QPointF& point );
    QPointF pointFromJsonObject( const QJsonValue& value );
};

#endif // DIAGRAMITEMSETTINGS_H
