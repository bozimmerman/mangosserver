#pragma once
#include "../Trigger.h"
#include "../values/LastMovementValue.h"

namespace ai
{
    class LootAvailableTrigger : public Trigger
    {
    public:
        LootAvailableTrigger(PlayerbotAI* ai) : Trigger(ai, "loot available") {}

        virtual bool IsActive();
    };

    class FarFromCurrentLootTrigger : public Trigger
    {
    public:
        FarFromCurrentLootTrigger(PlayerbotAI* ai) : Trigger(ai, "far from current loot") {}

        virtual bool IsActive();
    };

    class CanLootTrigger : public Trigger
    {
    public:
        CanLootTrigger(PlayerbotAI* ai) : Trigger(ai, "can loot") {}

        virtual bool IsActive();
    };

    class QuestGameObjectNearbyTrigger : public Trigger
    {
    public:
        QuestGameObjectNearbyTrigger(PlayerbotAI* ai) : Trigger(ai, "quest gameobject nearby") {}

        virtual bool IsActive();
    };

    class FarFromQuestGameObjectTrigger : public Trigger
    {
    public:
        FarFromQuestGameObjectTrigger(PlayerbotAI* ai) : Trigger(ai, "far from quest gameobject") {}

        virtual bool IsActive();
    };
}
