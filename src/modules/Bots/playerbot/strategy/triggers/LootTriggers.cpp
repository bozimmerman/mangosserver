#include "botpch.h"
#include "../../playerbot.h"
#include "LootTriggers.h"

using namespace ai;

bool LootAvailableTrigger::IsActive()
{
    return AI_VALUE(bool, "has available loot") && AI_VALUE(uint8, "bag space") < 80;
}

bool FarFromCurrentLootTrigger::IsActive()
{
    return AI_VALUE2(float, "distance", "loot target") > INTERACTION_DISTANCE;
}

bool CanLootTrigger::IsActive()
{
    return AI_VALUE(bool, "can loot");
}

bool QuestGameObjectNearbyTrigger::IsActive()
{
    ObjectGuid guid = AI_VALUE(ObjectGuid, "nearest quest gameobject");
    if (!guid)
        return false;
    GameObject* go = ai->GetGameObject(guid);
    return go && bot->GetDistance(go) <= INTERACTION_DISTANCE;
}

bool FarFromQuestGameObjectTrigger::IsActive()
{
    ObjectGuid guid = AI_VALUE(ObjectGuid, "nearest quest gameobject");
    if (!guid)
        return false;
    GameObject* go = ai->GetGameObject(guid);
    return go && bot->GetDistance(go) > INTERACTION_DISTANCE;
}
