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

public:
    QString name = "BasicBlock";

    bool label = true;
    QString label_text = "label";

    bool line_edit = true;
    QString line_edit_text = "";

    QString script = "";

    QString type_image = "self";
    QPixmap image;
};

#endif // BASEDBLOCKSETTINGS_H
