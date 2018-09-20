/*
* This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include "Policies/Singleton.h"
#include "Globals/ObjectMgr.h"
#include "Entities/GameObject.h"
#include "Entities/Player.h"
#include "Globals/SharedDefines.h"
#include <atomic>

enum ZoneIds
{
    ZONEID_HELLFIRE_PENINSULA   = 3483,
    ZONEID_HELLFIRE_RAMPARTS    = 3562,
    ZONEID_HELLFIRE_CITADEL     = 3563,
    ZONEID_BLOOD_FURNACE        = 3713,
    ZONEID_SHATTERED_HALLS      = 3714,
    ZONEID_MAGTHERIDON_LAIR     = 3836,

    ZONEID_SHATTRATH    = 3703,
    ZONEID_BOTANICA     = 3847,
    ZONEID_ARCATRAZ     = 3848,
    ZONEID_MECHANAR     = 3849,
};

enum SpellId
{
    SPELL_TROLLBANES_COMMAND    = 39911,
    SPELL_NAZGRELS_FAVOR        = 39913,

    SPELL_ADAL_SONG_OF_BATTLE   = 39953,
};

enum GoId
{
    OBJECT_MAGTHERIDONS_HEAD = 184640
};

enum Conditions
{
    ORGRIMMAR_UNDERCITY = 164871,
    GROMGOL_ORGRIMMAR   = 175080,
    GROMGOL_UNDERCITY   = 176495,
};

enum Events
{
    CUSTOM_EVENT_ADALS_SONG_OF_BATTLE,
};

// Intended for implementing server wide scripts, note: all behaviour must be safeguarded towards multithreading
class WorldState
{
    public:
        WorldState();
        virtual ~WorldState();

        // Called when a gameobject is created or removed
        virtual void HandleGameObjectUse(GameObject* go, Unit* user);
        virtual void HandleGameObjectRevertState(GameObject* go);

        // called when a player enters an outdoor pvp area
        void HandlePlayerEnterZone(Player* player, uint32 zoneId);

        // called when player leaves an outdoor pvp area
        void HandlePlayerLeaveZone(Player* player, uint32 zoneId);

        bool IsConditionFulfilled(uint32 conditionId, uint32 state) const;
        void HandleConditionStateChange(uint32 conditionId, uint32 state);

        void HandleExternalEvent(uint32 eventId);

        void BuffMagtheridonTeam(Team team);
        void DispelMagtheridonTeam(Team team);

        void BuffAdalsSongOfBattle();
        void DispelAdalsSongOfBattle();

        void Update(const uint32 diff);
    private:
        bool m_isMagtheridonHeadSpawnedHorde;
        bool m_isMagtheridonHeadSpawnedAlliance;
        ObjectGuid m_guidMagtheridonHeadHorde;
        ObjectGuid m_guidMagtheridonHeadAlliance;
        GuidVector m_magtheridonHeadPlayers;

        std::map<uint32, std::atomic<uint32>> m_transportStates; // atomic to avoid having to lock

        GuidVector m_adalSongOfBattlePlayers;
        uint32 m_adalSongOfBattleTimer;

        std::mutex m_mutex; // all World State operations are threat unsafe
};

#define sWorldState MaNGOS::Singleton<WorldState>::Instance()

#endif

