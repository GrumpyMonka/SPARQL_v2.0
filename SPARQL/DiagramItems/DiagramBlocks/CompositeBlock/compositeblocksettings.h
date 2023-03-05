#ifndef COMPOSITEBLOCKSETTINGS_H
#define COMPOSITEBLOCKSETTINGS_H

#include <basedblocksettings.h>

class CompositeBlockSettings : public DiagramItemSettings
{
public:
    int typeSettings() const override { return CompositeItemSettingsType; };

    CompositeBlockSettings();

    struct LineSaver
    {
        int start_block;
        int end_block;
        QString text_start;
        QString text_end;
    };

    QVector<DiagramItemSettings*> blocks;
    QVector<LineSaver> lines;
    QVector<QString> input_names;
    QVector<QString> output_names;

    void setSettingFromJson( const QJsonObject& object ) override;
    QJsonObject getJsonFromSetting() override;
    QPixmap image() const override;
    QJsonArray getJsonArrayFromLineSaver( const QVector<LineSaver>& lines );
    QString getScript() override;

    static CompositeBlockSettings* CreateTemplateCompositeSettings();
};

#endif // COMPOSITEBLOCKSETTINGS_H
