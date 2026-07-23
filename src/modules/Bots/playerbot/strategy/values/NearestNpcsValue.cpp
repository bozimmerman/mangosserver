#include "botpch.h"
#include "../../playerbot.h"
#include "NearestNpcsValue.h"

#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

using namespace ai;
using namespace MaNGOS;

void NearestNpcsValue::FindUnits(list<Unit*> &targets)
{
    AnyFriendlyUnitInObjectRangeCheck u_check(bot, range);
    UnitListSearcher<AnyFriendlyUnitInObjectRangeCheck> searcher(targets, u_check);
    Cell::VisitAllObjects(bot, searcher, range);
}

bool NearestNpcsValue::AcceptUnit(Unit* unit)
{
    return !dynamic_cast<Player*>(unit);
}

void NearestInteractableNpcsValue::FindUnits(list<Unit*> &targets)
{
    AnyUnitInObjectRangeCheck u_check(bot, range);
    UnitListSearcher<AnyUnitInObjectRangeCheck> searcher(targets, u_check);
    Cell::VisitAllObjects(bot, searcher, range);
}

bool NearestInteractableNpcsValue::AcceptUnit(Unit* unit)
{
    return !dynamic_cast<Player*>(unit) && !bot->IsHostileTo(unit);
}
