#ifndef PROJECTWINDOWSETTINGS_H
#define PROJECTWINDOWSETTINGS_H

#include <QVector>

#include "diagramitemsettings.h"

class ProjectWindowSettings : public DiagramItemSettings
{
public:
    int typeSettings() const override { return ProjectWindowSettingsType; }

    ProjectWindowSettings();

    void setSettingFromJson( const QJsonValue& value ) override;
    QJsonObject getJsonFromSetting() override;
};

#endif // PROJECTWINDOWSETTINGS_H
