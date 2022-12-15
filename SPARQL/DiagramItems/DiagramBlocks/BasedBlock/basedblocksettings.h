#ifndef BASEDBLOCKSETTINGS_H
#define BASEDBLOCKSETTINGS_H

#include <QPixmap>
#include <QString>
#include <diagramitemsettings.h>

class BasedBlockSettings : public DiagramItemSettings
{
public:
    int typeSettings() const override { return BasedItemSettingsType; };

    BasedBlockSettings();

    void setSettingFromJson( const QJsonObject& object ) override;
    QJsonObject getJsonFromSetting() override;
    QPixmap image() const override;
    QString getScript() override { return script; }

public:
    bool label;
    QString label_text;

    bool line_edit;
    QString line_edit_text;

    QString script;

    QString type_image;
    bool flag_custom_image;
};

#endif // BASEDBLOCKSETTINGS_H
