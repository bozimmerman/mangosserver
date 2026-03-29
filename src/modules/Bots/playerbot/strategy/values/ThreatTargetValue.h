#pragma once
#include "../Value.h"
#include "TargetValue.h"

namespace ai
{
    class ThreatTargetValue : public TargetValue
    {
    public:
        ThreatTargetValue(PlayerbotAI* ai) : TargetValue(ai) {}

    public:
        Unit* Calculate();
    };
}
