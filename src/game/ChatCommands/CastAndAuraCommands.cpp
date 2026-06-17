/**
 * MaNGOS is a full featured server for World of Warcraft, supporting
 * the following clients: 1.12.x, 2.4.3, 3.3.5a, 4.3.4a and 5.4.8
 *
 * Copyright (C) 2005-2025 MaNGOS <https://www.getmangos.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

/**
 * @file CastAndAuraCommands.cpp
 * @brief Implementation of spell casting and aura management chat commands.
 *
 * This file contains chat command handlers for spell and aura operations including:
 * - Spell casting on players and creatures
 * - Aura application and removal
 * - Spell effect testing and debugging
 */

#include "Chat.h"
#include "Language.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "Util.h"

/**********************************************************************
 CommandTable : castCommandTable
 ***********************************************************************/

bool AddAuraToPlayer(const SpellEntry* spellInfo, Unit* target, WorldObject* caster);

/**
 * @brief Handler for HandleCastCommand command.
 *
 * @param args Command arguments.
 * @returns True if the command executed successfully, false otherwise.
 */
bool ChatHandler::HandleCastCommand(char* args)
{
    if (!*args)
    {
        return false;
    }

    Unit* target = getSelectedUnit();

    if (!target)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
    uint32 spell = ExtractSpellIdFromLink(&args);
    if (!spell)
    {
        bool multipleExact = false;
        std::vector<std::pair<uint32, std::string>> candidates;
        spell = FindSpellByName(args, multipleExact, candidates);

        if (!spell)
        {
            if (multipleExact || !candidates.empty())
            {
                SendSysMessage(multipleExact
                    ? "Multiple spells found with that name:"
                    : "No exact match. Close matches:");
                for (auto const& c : candidates)
                {
                    PSendSysMessage("  %u - %s", c.first, c.second.c_str());
                }
            }
            else
            {
                SendSysMessage(LANG_COMMAND_NOSPELLFOUND);
            }
            SetSentErrorMessage(true);
            return false;
        }
    }

    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spell);
    if (!spellInfo)
    {
        return false;
    }

    if (!m_session)
    {
        target->CastSpell(target, spell, true);
<<<<<<< HEAD
        PSendSysMessage("Cast spell %u (%s) on %s",
=======
        PSendSysMessage("Cast spell %u (%s) on %s (triggered, console)",
>>>>>>> 89fc099d (console cast spell by name)
            spell, spellInfo->SpellName[GetSessionDbcLocale()],
            target->GetName());
        return true;
    }

    if (!SpellMgr::IsSpellValid(spellInfo, m_session->GetPlayer()))
    {
        PSendSysMessage(LANG_COMMAND_SPELL_BROKEN, spell);
        SetSentErrorMessage(true);
        return false;
    }

    bool triggered = ExtractLiteralArg(&args, "triggered") != NULL;
    if (!triggered && *args)                                // can be fail also at syntax error
    {
        return false;
    }

    m_session->GetPlayer()->CastSpell(target, spell, triggered);

    return true;
}

/**
 * @brief Handler for HandleCastBackCommand command.
 *
 * @param args Command arguments.
 * @returns True if the command executed successfully, false otherwise.
 */
bool ChatHandler::HandleCastBackCommand(char* args)
{
    Creature* caster = getSelectedCreature();

    if (!caster)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r
    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
    uint32 spell = ExtractSpellIdFromLink(&args);
    if (!spell || !sSpellStore.LookupEntry(spell))
    {
        return false;
    }

    bool triggered = ExtractLiteralArg(&args, "triggered") != NULL;
    if (!triggered && *args)                                // can be fail also at syntax error
    {
        return false;
    }

    caster->SetFacingToObject(m_session->GetPlayer());

    caster->CastSpell(m_session->GetPlayer(), spell, triggered);

    return true;
}

/**
 * @brief Handler for HandleCastDistCommand command.
 *
 * @param args Command arguments.
 * @returns True if the command executed successfully, false otherwise.
 */
bool ChatHandler::HandleCastDistCommand(char* args)
{
    if (!*args)
    {
        return false;
    }

    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
    uint32 spell = ExtractSpellIdFromLink(&args);
    if (!spell)
    {
        return false;
    }

    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spell);
    if (!spellInfo)
    {
        return false;
    }

    if (!SpellMgr::IsSpellValid(spellInfo, m_session->GetPlayer()))
    {
        PSendSysMessage(LANG_COMMAND_SPELL_BROKEN, spell);
        SetSentErrorMessage(true);
        return false;
    }

    float dist;
    if (!ExtractFloat(&args, dist))
    {
        return false;
    }

    bool triggered = ExtractLiteralArg(&args, "triggered") != NULL;
    if (!triggered && *args)                                // can be fail also at syntax error
    {
        return false;
    }

    float x, y, z;
    m_session->GetPlayer()->GetClosePoint(x, y, z, dist);

    m_session->GetPlayer()->CastSpell(x, y, z, spell, triggered);
    return true;
}

/**
 * @brief Handler for HandleCastTargetCommand command.
 *
 * @param args Command arguments.
 * @returns True if the command executed successfully, false otherwise.
 */
bool ChatHandler::HandleCastTargetCommand(char* args)
{
    Creature* caster = getSelectedCreature();

    if (!caster)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    if (!caster->getVictim())
    {
        SendSysMessage(LANG_SELECTED_TARGET_NOT_HAVE_VICTIM);
        SetSentErrorMessage(true);
        return false;
    }

    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
    uint32 spell = ExtractSpellIdFromLink(&args);
    if (!spell || !sSpellStore.LookupEntry(spell))
    {
        return false;
    }

    bool triggered = ExtractLiteralArg(&args, "triggered") != NULL;
    if (!triggered && *args)                                // can be fail also at syntax error
    {
        return false;
    }

    caster->SetFacingToObject(m_session->GetPlayer());

    caster->CastSpell(caster->getVictim(), spell, triggered);

    return true;
}

/**
 * @brief Handler for HandleCastSelfCommand command.
 *
 * @param args Command arguments.
 * @returns True if the command executed successfully, false otherwise.
 */
bool ChatHandler::HandleCastSelfCommand(char* args)
{
    if (!*args)
    {
        return false;
    }

    Unit* target = getSelectedUnit();

    if (!target)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
    uint32 spell = ExtractSpellIdFromLink(&args);
    if (!spell)
    {
        return false;
    }

    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spell);
    if (!spellInfo)
    {
        return false;
    }

    if (!SpellMgr::IsSpellValid(spellInfo, m_session->GetPlayer()))
    {
        PSendSysMessage(LANG_COMMAND_SPELL_BROKEN, spell);
        SetSentErrorMessage(true);
        return false;
    }

    bool triggered = ExtractLiteralArg(&args, "triggered") != NULL;
    if (!triggered && *args)                                // can be fail also at syntax error
    {
        return false;
    }

    target->CastSpell(target, spell, triggered);

    return true;
}

/**
 * @brief Handler for HandleAuraCommand command.
 *
 * @param args Command arguments.
 * @returns True if the command executed successfully, false otherwise.
 */
bool ChatHandler::HandleAuraCommand(char* args)
{
    Unit* target = getSelectedUnit();
    if (!target)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
    uint32 spellID = ExtractSpellIdFromLink(&args);

    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellID);
    if (!spellInfo)
    {
        return false;
    }

    if (!IsSpellAppliesAura(spellInfo, (1 << EFFECT_INDEX_0) | (1 << EFFECT_INDEX_1) | (1 << EFFECT_INDEX_2)) &&
        !spellInfo->HasSpellEffect(SPELL_EFFECT_PERSISTENT_AREA_AURA))
    {
        PSendSysMessage(LANG_SPELL_NO_HAVE_AURAS, spellID);
        SetSentErrorMessage(true);
        return false;
    }

    return AddAuraToPlayer(spellInfo, target, m_session->GetPlayer());
}

/**
 * @brief Handler for HandleUnAuraCommand command.
 *
 * @param args Command arguments.
 * @returns True if the command executed successfully, false otherwise.
 */
bool ChatHandler::HandleUnAuraCommand(char* args)
{
    Unit* target = getSelectedUnit();
    if (!target)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    std::string argstr = args;
    if (argstr == "all")
    {
        target->RemoveAllAuras();
        return true;
    }

    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
    uint32 spellID = ExtractSpellIdFromLink(&args);
    if (!spellID)
    {
        return false;
    }

    target->RemoveAurasDueToSpell(spellID);

    return true;
}

/**
 * @brief Handler for HandleAuraGroupCommand command.
 *
 * @param args Command arguments.
 * @returns True if the command executed successfully, false otherwise.
 */
bool ChatHandler::HandleAuraGroupCommand(char* args)
{
    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
    uint32 spellID = ExtractSpellIdFromLink(&args);

    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellID);
    if (!spellInfo)
    {
        PSendSysMessage("Spell %u does not exists", spellID);
        return false;
    }

    if (!IsSpellAppliesAura(spellInfo, (1 << EFFECT_INDEX_0) | (1 << EFFECT_INDEX_1) | (1 << EFFECT_INDEX_2)) &&
        !spellInfo->HasSpellEffect(SPELL_EFFECT_PERSISTENT_AREA_AURA))
    {
        PSendSysMessage(LANG_SPELL_NO_HAVE_AURAS, spellID);
        SetSentErrorMessage(true);
        return false;
    }

    Unit* rawTarget = getSelectedUnit();
    Player* playerTarget ;

    if (rawTarget)
    {
        if (rawTarget->GetTypeId() == TYPEID_UNIT)
        {
            SendSysMessage(LANG_NO_CHAR_SELECTED);
            SetSentErrorMessage(true);
            return false;
        }

        playerTarget = (Player*)rawTarget;
    }
    else
    {
        playerTarget = m_session->GetPlayer();
    }

    Group* grp = playerTarget->GetGroup();

    if (!grp)
    {
        std::string nameLink = GetNameLink(playerTarget);

        if (playerTarget->IsDead())
        {
            PSendSysMessage(LANG_COMMAND_AURAGROUP_CANNOT_APPLY_AURA_PLAYER_IS_DEAD, nameLink.c_str());
            return false;
        }
        else
        {
            AddAuraToPlayer(spellInfo, playerTarget, m_session->GetPlayer());
            PSendSysMessage(LANG_COMMAND_AURAGROUP_AURA_APPLIED, spellInfo->Id, nameLink.c_str());
            return true;
        }
    }
    else
    {
        // Apply to all members of the group
        for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* pl = itr->getSource();

            //Skip if player is not found
            if (!pl || !pl->GetSession())
            {
                continue;
            }

            std::string nameLink = GetNameLink(pl);

            //skip if player is dead
            if (pl->IsDead())
            {
                PSendSysMessage(LANG_COMMAND_AURAGROUP_CANNOT_APPLY_AURA_PLAYER_IS_DEAD, nameLink.c_str());
                continue;
            }

            AddAuraToPlayer(spellInfo, pl, m_session->GetPlayer());
            PSendSysMessage(LANG_COMMAND_AURAGROUP_AURA_APPLIED, spellInfo->Id, nameLink.c_str());

        }

        return true;
    }
}

/**
 * @brief Handler for HandleUnAuraGroupCommand command.
 *
 * @param args Command arguments.
 * @returns True if the command executed successfully, false otherwise.
 */
bool ChatHandler::HandleUnAuraGroupCommand(char* args)
{
    // Must have args : spellId or "all"
    if (!*args)
    {
        return false;
    }

    bool removeAll = false;

    std::string argstr = args;
    if (argstr == "all")
    {
        removeAll = true;
    }

    uint32 spellIdToRemove;

    if (!removeAll)
    {
        spellIdToRemove = ExtractSpellIdFromLink(&args);
        if (!spellIdToRemove)
        {
            return false;
        }
    }

    // Now remove the aura(s)
    Unit* rawTarget = getSelectedUnit();
    Player* playerTarget;

    if (rawTarget)
    {
        if (rawTarget->GetTypeId() == TYPEID_UNIT)
        {
            SendSysMessage(LANG_NO_CHAR_SELECTED);
            SetSentErrorMessage(true);
            return false;
        }

        playerTarget = (Player*)rawTarget;
    }
    else
    {
        playerTarget = m_session->GetPlayer();
    }

    Group* grp = playerTarget->GetGroup();

    if (!grp)
    {
        std::string nameLink = GetNameLink(playerTarget);

        //security : avoid to remove ghost form if player is dead
        if (playerTarget->IsDead())
        {
            PSendSysMessage(LANG_COMMAND_AURAGROUP_CANNOT_UNAURA_DEAD_PLAYER, nameLink.c_str());
            return false;
        }
        else
        {
            if (removeAll)
            {
                playerTarget->RemoveAllAuras();
                PSendSysMessage(LANG_COMMAND_AURAGROUP_ALL_AURA_REMOVED, nameLink.c_str());
            }
            else
            {
                playerTarget->RemoveAurasDueToSpell(spellIdToRemove);
                PSendSysMessage(LANG_COMMAND_AURAGROUP_AURA_REMOVED_FOR_SPELL, spellIdToRemove, nameLink.c_str());
            }

            return true;
        }
    }
    else
    {
        // Apply to all members of the group
        for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* pl = itr->getSource();

            if (!pl || !pl->GetSession())
            {
                continue;
            }

            std::string nameLink = GetNameLink(pl);
            if (pl->IsDead())
            {
                PSendSysMessage(LANG_COMMAND_AURAGROUP_CANNOT_UNAURA_DEAD_PLAYER, nameLink.c_str());
                continue;
            }
            else
            {
                if (removeAll)
                {
                    pl->RemoveAllAuras();
                    PSendSysMessage(LANG_COMMAND_AURAGROUP_ALL_AURA_REMOVED, nameLink.c_str());
                }
                else
                {
                    pl->RemoveAurasDueToSpell(spellIdToRemove);
                    PSendSysMessage(LANG_COMMAND_AURAGROUP_AURA_REMOVED_FOR_SPELL, spellIdToRemove, nameLink.c_str());
                }

            }

        }

        return true;
    }
}

uint32 ChatHandler::FindSpellByName(char const* args, bool& multipleExact, std::vector<std::pair<uint32, std::string>>& candidates) const
{
    LocaleConstant locale = GetSessionDbcLocale();

    if (!args || !*args)
    {
        return 0;
    }

    std::string nameStr = args;
    std::wstring wname;

    if (!Utf8toWStr(nameStr, wname))
    {
        return 0;
    }

    wstrToLower(wname);

    uint32 exactId = 0;
    for (uint32 id = 0; id < sSpellStore.GetNumRows(); ++id)
    {
        SpellEntry const* spell = sSpellStore.LookupEntry(id);
        if (!spell)
        {
            continue;
        }

        std::string sName = spell->SpellName[locale];
        if (sName.empty())
        {
            continue;
        }

        std::wstring wSpellName;
        if (!Utf8toWStr(sName, wSpellName))
        {
            continue;
        }
        wstrToLower(wSpellName);

        if (wSpellName == wname)
        {
            if (exactId == 0)
            {
                exactId = id;
            }
            else
            {
                multipleExact = true;
            }
        }
    }

    if (exactId != 0 && !multipleExact)
    {
        candidates.clear();
        return exactId;
    }

    candidates.clear();

    if (multipleExact)
    {
        std::wstring wname2;
        Utf8toWStr(nameStr, wname2);
        wstrToLower(wname2);
        for (uint32 id = 0; id < sSpellStore.GetNumRows(); ++id)
        {
            SpellEntry const* spell = sSpellStore.LookupEntry(id);
            if (!spell)
            {
                continue;
            }
            std::string sName = spell->SpellName[locale];
            if (sName.empty())
            {
                continue;
            }
            std::wstring wSpellName2;
            if (!Utf8toWStr(sName, wSpellName2))
            {
                continue;
            }
            wstrToLower(wSpellName2);
            if (wSpellName2 == wname2)
            {
                candidates.push_back({id, spell->SpellName[locale]});
            }
        }
    }
    else
    {
        for (uint32 id = 0; id < sSpellStore.GetNumRows(); ++id)
        {
            SpellEntry const* spell = sSpellStore.LookupEntry(id);
            if (!spell)
            {
                continue;
            }

            int loc = locale;
            std::string sName = spell->SpellName[loc];
            if (sName.empty())
            {
                continue;
            }

            if (!Utf8FitTo(sName, wname))
            {
                loc = 0;
                for (; loc < MAX_LOCALE; ++loc)
                {
                    if (loc == locale)
                    {
                        continue;
                    }
                    sName = spell->SpellName[loc];
                    if (sName.empty())
                    {
                        continue;
                    }
                    if (Utf8FitTo(sName, wname))
                    {
                        break;
                    }
                }
            }

            if (loc < MAX_LOCALE)
            {
                candidates.push_back({id, spell->SpellName[locale]});
            }
        }
    }

    return 0;
}
