#ifndef PROJECTWINDOWSETTINGS_H
#define PROJECTWINDOWSETTINGS_H

#include <QVector>

#include "diagramitemsettings.h"

class ProjectWindowSettings : public DiagramItemSettings
{
public:
    int typeSettings() const override { return ProjectWindowSettingsType; }

    ProjectWindowSettings();
    ~ProjectWindowSettings();

    struct LineSaver
    {
        int start_block;
        int end_block;
        QString text;
    };

    QVector<DiagramItemSettings*> blocks_list;
    QVector<LineSaver> lines_list;

    void clear();
    void setSettingFromJson( const QJsonValue& value ) override;
    QJsonObject getJsonFromSetting() override;

    QJsonArray getJsonArrayFromLineSaver( const QVector<LineSaver>& lines );
};

#endif // PROJECTWINDOWSETTINGS_H
