#include "botpch.h"
#include "../../playerbot.h"
#include "ReviveFromCorpseAction.h"
#include "../../PlayerbotFactory.h"
#include "../../PlayerbotAIConfig.h"

using namespace ai;

bool ReviveFromCorpseAction::Execute(Event event)
{
    Corpse* corpse = bot->GetCorpse();
    if (!corpse)
    {
        return false;
    }

    time_t reclaimTime = corpse->GetGhostTime() + bot->GetCorpseReclaimDelay( corpse->GetType()==CORPSE_RESURRECTABLE_PVP );
    if (reclaimTime > time(0))
    {
        return false;
    }

    bool shouldTeleport = (!bot->GetGroup()); // simulate corpse run if NOT grouped
    if (bot->GetGroup() && ai->HasStrategy("stay", BOT_STATE_DEAD)) // .. or if not following
        shouldTeleport = true;

    // Teleport to corpse location
    if (shouldTeleport && corpse->GetDistance(bot) > sPlayerbotAIConfig.spellDistance)
    {
        bot->TeleportTo(corpse->GetMapId(),
                       corpse->GetPositionX(),
                       corpse->GetPositionY(),
                       corpse->GetPositionZ(),
                       bot->GetOrientation());
        return true;
    }

    if (corpse->GetDistance(bot) <= sPlayerbotAIConfig.spellDistance)
    {
        PlayerbotChatHandler ch(bot);
        if (!ch.revive(*bot))
        {
            ai->TellMaster(".. could not be revived ..");
            return false;
        }
        context->GetValue<Unit*>("current target")->Set(NULL);
        bot->SetSelectionGuid(ObjectGuid());
        return true;
    }
    return false; // wait at graveyard
}

bool SpiritHealerAction::Execute(Event event)
{
    Corpse* corpse = bot->GetCorpse();
    if (!corpse)
    {
        return false;
    }

    list<ObjectGuid> npcs = AI_VALUE(list<ObjectGuid>, "nearest npcs");
    for (list<ObjectGuid>::iterator i = npcs.begin(); i != npcs.end(); i++)
    {
        Unit* unit = ai->GetUnit(*i);
        if (unit && unit->IsSpiritHealer())
        {
            PlayerbotChatHandler ch(bot);
            if (! ch.revive(*bot))
            {
                ai->TellMaster(".. could not be revived ..");
                return false;
            }
            context->GetValue<Unit*>("current target")->Set(NULL);
            bot->SetSelectionGuid(ObjectGuid());
            return true;
        }
    }

    ai->TellMaster("Cannot find any spirit healer nearby");
    return false;
}
