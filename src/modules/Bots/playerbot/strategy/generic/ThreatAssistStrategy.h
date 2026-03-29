#include "../generic/NonCombatStrategy.h"
#pragma once

namespace ai
{
    class ThreatAssistStrategy : public NonCombatStrategy
    {
    public:
        ThreatAssistStrategy(PlayerbotAI* ai) : NonCombatStrategy(ai) {}
        virtual string getName() { return "threat assist"; }
        virtual int GetType() { return STRATEGY_TYPE_DPS; }

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
    };
}
