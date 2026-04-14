#include "botpch.h"
#include "../../playerbot.h"
#include "../Strategy.h"
#include "DeadStrategy.h"

using namespace ai;

void DeadStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    PassTroughStrategy::InitTriggers(triggers);

    /* Trigger name must be "bot dead", not "dead". The AI context uses "dead" as the name of
       a boolean value (AI_VALUE2(bool, "dead", ...)), so registering a trigger also named "dead"
       caused a namespace collision: TriggerContext::creators["dead"] pointed to the trigger
       factory, but the engine's context lookup resolved "dead" as the value instead, so the
       trigger was never found and dead bots never executed revive from corpse. */
    triggers.push_back(new TriggerNode(
        "bot dead",
        NextAction::array(0, new NextAction("revive from corpse", relevance), NULL)));

    triggers.push_back(new TriggerNode(
        "resurrect request",
        NextAction::array(0, new NextAction("accept resurrect", relevance), NULL)));
}

DeadStrategy::DeadStrategy(PlayerbotAI* ai) : PassTroughStrategy(ai)
{
}
