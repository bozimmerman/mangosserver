#include "botpch.h"
#include "../../playerbot.h"
#include "NearestQuestGameObject.h"

using namespace ai;

ObjectGuid NearestQuestGameObject::Calculate()
{
    list<ObjectGuid> gos = AI_VALUE(list<ObjectGuid>, "nearest game objects");

    GameObject* nearest = nullptr;
    float minDist = FLT_MAX;

    for (list<ObjectGuid>::iterator i = gos.begin(); i != gos.end(); ++i)
    {
        if (bot->GetPlayerbotAI()->HasUsed(*i))
            continue;
        GameObject* go = ai->GetGameObject(*i);
        if (!go || !go->isSpawned())
            continue;
        if (go->GetGoType() == GAMEOBJECT_TYPE_CHEST)
            continue;
        if (!go->ActivateToQuest(bot))
            continue;
        float dist = bot->GetDistance(go);
        if (dist < minDist)
        {
            minDist = dist;
            nearest = go;
        }
    }

    return nearest ? nearest->GetObjectGuid() : ObjectGuid();
}
