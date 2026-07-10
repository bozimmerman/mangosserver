#include "botpch.h"
#include "../../playerbot.h"
#include "CastCustomSpellAction.h"

using namespace ai;

/**
 * Executes a custom spell cast request against the selected or named target.
 */
bool CastCustomSpellAction::Execute(Event event)
{
    Unit* target = NULL;

    Player* master = GetMaster();
    if (master && master->GetSelectionGuid())
    {
        target = ai->GetUnit(master->GetSelectionGuid());
    }

    if (!target)
    {
        target = bot;
    }

    string text = event.getParam();

    uint32 spell = chat->parseSpell(text);
    if(!spell)
    {
        spell = AI_VALUE2(uint32, "spell id", text);
    }

    if (spell)
    {
        SpellEntry const* spellInfo = sSpellStore.LookupEntry(spell);
        if (spellInfo)
        {
            ShapeshiftForm form = bot->GetShapeshiftForm();
            if (form != FORM_NONE)
            {
                if (spellInfo->ShapeshiftMask == 0 ||
                    !(spellInfo->ShapeshiftMask & (1 << (form - 1))))
                {
                    ai->RemoveShapeshift();
                }
            }
        }
    }

    ostringstream msg;
    if (!ai->CanCastSpell(spell, target))
    {
        msg << "Cannot cast " << text << " on " << target->GetName();
        ai->TellMaster(msg.str());
        return false;
    }

    bool result = false;
    if (spell)
    {
        result = ai->CastSpell(spell, target);
    }
    else
    {
        ai->CastSpell(text, target);
    }

    if (result)
    {
        msg << "Casting " << text << " on " << target->GetName();
        ai->TellMasterNoFacing(msg.str());
    }
    else
    {
        msg << "Cast " << text << " on " << target->GetName() << " is failed";
        ai->TellMaster(msg.str());
    }

    return result;
}
