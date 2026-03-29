#include "botpch.h"
#include "../../playerbot.h"
#include "ThreatTargetValue.h"

using namespace ai;

class FindThreatTargetStrategy : public FindTargetStrategy
{
public:
    FindThreatTargetStrategy(PlayerbotAI* ai) : FindTargetStrategy(ai)
    {
        maxMargin = -1.0f;
        m_botIsTank = ai->IsTank(ai->GetBot());
        m_tankExists = false;

        if (!m_botIsTank)
        {
            Player* bot = ai->GetBot();
            Group* group = bot->GetGroup();
            if (group)
            {
                Group::MemberSlotList const& groupSlot = group->GetMemberSlots();
                for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); ++itr)
                {
                    Player* player = sObjectMgr.GetPlayer(itr->guid);
                    if (!player || !player->IsAlive() || player == bot)
                        continue;
                    if (ai->IsTank(player))
                    {
                        m_tankExists = true;
                        break;
                    }
                }
            }
        }
    }

public:
    virtual void CheckAttacker(Unit* creature, ThreatManager* threatManager)
    {
        Player* bot = ai->GetBot();
        Group* group = bot->GetGroup();
        if (!group)
            return;

        float tankThreat = 0.0f;
        Group::MemberSlotList const& groupSlot = group->GetMemberSlots();
        for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); ++itr)
        {
            Player* player = sObjectMgr.GetPlayer(itr->guid);
            if (!player || !player->IsAlive())
                continue;
            if (!ai->IsTank(player))
                continue;
            tankThreat += threatManager->getThreat(player);
        }

        if (!m_botIsTank && m_tankExists)
        {
            float margin = tankThreat - threatManager->getThreat(bot);
            if (margin <= 0.0f)
                return;
            if (!result || margin > maxMargin)
            {
                maxMargin = margin;
                result = creature;
            }
        }
        else
        {
            if (!result || tankThreat > maxMargin)
            {
                maxMargin = tankThreat;
                result = creature;
            }
        }
    }

protected:
    float maxMargin;
    bool m_botIsTank;
    bool m_tankExists;
};

Unit* ThreatTargetValue::Calculate()
{
    FindThreatTargetStrategy strategy(ai);
    return FindTarget(&strategy);
}
