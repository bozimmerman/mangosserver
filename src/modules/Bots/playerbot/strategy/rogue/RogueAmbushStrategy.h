#pragma once

#include "../Strategy.h"

namespace ai
{
    class RogueAmbushStrategy : public Strategy
    {
    public:
        RogueAmbushStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        virtual string getName() { return "pull"; }

        virtual NextAction** getDefaultActions()
        {
            // Stealth first; once stealthed, cheap shot moves to target (via reach melee
            // prerequisite) and opens. End pull fires once combat begins.
            return NextAction::array(0,
                new NextAction("stealth", 105.0f),
                new NextAction("cheap shot", 104.0f),
                new NextAction("end pull", 103.0f),
                new NextAction("sinister strike", 100.0f),
                NULL);
        }
    };
}
