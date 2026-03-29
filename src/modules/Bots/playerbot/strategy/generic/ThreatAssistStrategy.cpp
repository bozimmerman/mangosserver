#include "botpch.h"
#include "../../playerbot.h"
#include "ThreatAssistStrategy.h"

using namespace ai;

void ThreatAssistStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "no attackers",
        NextAction::array(0, new NextAction("attack threat target", 50.0f), NULL)));
}
