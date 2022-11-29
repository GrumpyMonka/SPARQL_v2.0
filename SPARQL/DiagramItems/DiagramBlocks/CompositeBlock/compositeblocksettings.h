#ifndef COMPOSITEBLOCKSETTINGS_H
#define COMPOSITEBLOCKSETTINGS_H

#include <basedblocksettings.h>

class CompositeBlockSettings : public DiagramItemSettings
{
public:
    int typeSettings() const override { return CompositeItemSettingsType; };

    CompositeBlockSettings();
};

#endif // COMPOSITEBLOCKSETTINGS_H
