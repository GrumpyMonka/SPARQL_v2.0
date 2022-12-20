#ifndef IOBLOCKSETTINGS_H
#define IOBLOCKSETTINGS_H

#include <QMap>

#include "diagramitemsettings.h"

class IOBlockSettings : public DiagramItemSettings
{
public:
    enum TypeIO
    {
        Input,
        Output
    };

    QMap<TypeIO, QString> ConvertTypeBlock = {
        { Input, "Input" },
        { Output, "Output" }
    };

    TypeIO type_block;
    QString text;
    QPolygonF polygon;

    IOBlockSettings();
    int typeSettings() const { return IOItemSettingsType; }

    void setSettingFromJson( const QJsonObject& object );
    QJsonObject getJsonFromSetting();
    QPixmap image() const;
    void setType( TypeIO type );

    static QVector<DiagramItemSettings*> GetBasedIOBlocks();
};

#endif // IOBLOCKSETTINGS_H
