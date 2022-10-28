#ifndef COMPOSITEBLOCKSETTINGS_H
#define COMPOSITEBLOCKSETTINGS_H

#include <basedblocksettings.h>

class CompositeBlockSettings : public DiagramItemSettings
{
public:
    enum
    {
        Type = 1
    };
    int type() override { return Type; };

    CompositeBlockSettings();
};

#endif // COMPOSITEBLOCKSETTINGS_H
