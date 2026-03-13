#include "botpch.h"
#include "../../playerbot.h"
#include "../actions/GenericActions.h"
#include "HunterActions.h"

using namespace ai;

bool CastSerpentStingAction::isUseful()
{
    return AI_VALUE2(uint8, "health", "current target") > 50;
}

bool CastViperStingAction::isUseful()
{
    return AI_VALUE2(uint8, "mana", "self target") < 50 && AI_VALUE2(uint8, "mana", "current target") >= 30;
}

bool CastAspectOfTheCheetahAction::isUseful()
{
    return !ai->HasAnyAuraOf(GetTarget(), "aspect of the cheetah", "aspect of the pack", NULL);
}

Value<Unit*>* CastFreezingTrap::GetTargetValue()
{
    return context->GetValue<Unit*>("cc target", "freezing trap");
}

bool CastIntimidationAction::isUseful()
{
    return CastSpellAction::isUseful() && AI_VALUE(Unit*, "pet target") != NULL;
}

bool HunterMeleeAction::isUseful()
{
    // Only swing if enemy is already in our face AND targeting us.
    //  Perhaps in the future a ranged/melee hunter strategy would be nice.
    Unit* target = AI_VALUE(Unit*, "current target");
    if (!target || !target->IsAlive()) return false;
    bool victim = target->getVictim() == bot;
    float dist = AI_VALUE2(float, "distance", "current target");
    return victim && dist <= ATTACK_DISTANCE;
}

bool HunterMeleeAction::Execute(Event event)
{
    Unit* target = AI_VALUE(Unit*, "current target");
    if (!target) return false;
    bot->Attack(target, true);
    return true;
}
