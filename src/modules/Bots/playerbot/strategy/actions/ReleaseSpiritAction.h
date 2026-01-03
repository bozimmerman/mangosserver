#pragma once

#include "../Action.h"
#include "MovementActions.h"
#include "../values/LastMovementValue.h"

namespace ai
{
    class ReleaseSpiritAction : public Action {
    public:
        ReleaseSpiritAction(PlayerbotAI* ai) : Action(ai, "release") {}

    public:
        virtual bool Execute(Event event)
        {
            if (bot->IsAlive() || bot->GetCorpse())
            {
                return false;
            }

            uint32 timeDead = (6 * MINUTE * IN_MILLISECONDS) - bot->GetDeathTimer();
            if (timeDead < 10000)
            {
                return false;  // Wait 10 seconds before release
            }

            // 1. If you are doing a dungeon or grouped, nofollow adds pain to bot mgmt.
            // 2. If the bot is on his own (or ungrouped), there is no one to follow anyway, so its OK.
            if (!bot->GetGroup())
            {
                ai->ChangeStrategy("-follow,+stay", BOT_STATE_NON_COMBAT);
            }
            else
            if (ai->GetMaster())
            {
                ai->TellMaster("I've died and am waiting at the graveyard for escort");
            }

            bot->SetBotDeathTimer();
            bot->BuildPlayerRepop();

            bot->RepopAtGraveyard();
            return true;
        }
    };

}
