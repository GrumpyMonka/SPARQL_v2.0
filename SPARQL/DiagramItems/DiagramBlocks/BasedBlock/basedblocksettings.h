#ifndef BASEDBLOCKSETTINGS_H
#define BASEDBLOCKSETTINGS_H

#include <QPixmap>
#include <QString>
#include <diagramitemsettings.h>

class BasedBlockSettings : public DiagramItemSettings
{
public:
    enum
    {
        Type = 1
    };
    int type() override { return Type; };
    BasedBlockSettings();

    void setSettingFromJson( const QJsonValue& value ) override;
    QJsonObject getJsonFromSetting() override;
    QPixmap image() const override;

public:
    bool label;
    QString label_text;

    bool line_edit;
    QString line_edit_text;

    QString script;

    QString type_image;
};

#endif // BASEDBLOCKSETTINGS_H
