#pragma once
#include "../Value.h"

namespace ai
{
    class NearestQuestGameObject : public ObjectGuidCalculatedValue
    {
    public:
        NearestQuestGameObject(PlayerbotAI* ai) : ObjectGuidCalculatedValue(ai, "nearest quest gameobject") {}

    protected:
        virtual ObjectGuid Calculate();
    };
}
