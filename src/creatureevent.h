/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_CREATUREEVENT_H_73FCAF4608CB41399D53C919316646A9
#define FS_CREATUREEVENT_H_73FCAF4608CB41399D53C919316646A9

#include "luascript.h"
#include "baseevents.h"
#include "enums.h"

enum CreatureEventType_t {
	CREATURE_EVENT_NONE,
	CREATURE_EVENT_LOGIN,
	CREATURE_EVENT_LOGOUT,
	CREATURE_EVENT_THINK,
	CREATURE_EVENT_PREPAREDEATH,
	CREATURE_EVENT_DEATH,
	CREATURE_EVENT_KILL,
	CREATURE_EVENT_ADVANCE,
	CREATURE_EVENT_TEXTEDIT,
	CREATURE_EVENT_HEALTHCHANGE,
	CREATURE_EVENT_MANACHANGE,
	CREATURE_EVENT_EXTENDED_OPCODE // otclient additional network opcodes
};

class CreatureEvent;

class CreatureEvents final : public BaseEvents
{
	public:
		CreatureEvents();
		~CreatureEvents();

		// non-copyable
		CreatureEvents(const CreatureEvents&) = delete;
		CreatureEvents& operator=(const CreatureEvents&) = delete;

		// global events
		bool playerLogin(Player* player) const;
		bool playerLogout(Player* player) const;
		bool playerAdvance(Player* player, skills_t, uint32_t, uint32_t);

		CreatureEvent* getEventByName(const std::string& name, bool forceLoaded = true);

	protected:
		LuaScriptInterface& getScriptInterface() final;
		std::string getScriptBaseName() const final;
		Event* getEvent(const std::string& nodeName) final;
		bool registerEvent(Event* event, const pugi::xml_node& node) final;
		void clear() final;

		//creature events
		typedef std::map<std::string, CreatureEvent*> CreatureEventList;
		CreatureEventList m_creatureEvents;

		LuaScriptInterface m_scriptInterface;
};

class CreatureEvent final : public Event
{
	public:
		explicit CreatureEvent(LuaScriptInterface* _interface);

		bool configureEvent(const pugi::xml_node& node) final;

		CreatureEventType_t getEventType() const {
			return m_type;
		}
		const std::string& getName() const {
			return m_eventName;
		}
		bool isLoaded() const {
			return m_isLoaded;
		}

		void clearEvent();
		void copyEvent(CreatureEvent* creatureEvent);

		//scripting
		bool executeOnLogin(Player* player);
		bool executeOnLogout(Player* player);
		bool executeOnThink(Creature* creature, uint32_t interval);
		bool executeOnPrepareDeath(Creature* creature, Creature* killer);
		bool executeOnDeath(Creature* creature, Item* corpse, Creature* killer, Creature* mostDamageKiller, bool lastHitUnjustified, bool mostDamageUnjustified);
		bool executeOnKill(Creature* creature, Creature* target);
		bool executeAdvance(Player* player, skills_t, uint32_t, uint32_t);
		bool executeTextEdit(Player* player, Item* item, const std::string& text);
		void executeHealthChange(Creature* creature, Creature* attacker, CombatDamage& damage);
		void executeManaChange(Creature* creature, Creature* attacker, int32_t& manaChange, CombatOrigin origin);
		void executeExtendedOpcode(Player* player, uint8_t opcode, const std::string& buffer);
		//

	protected:
		std::string getScriptEventName() const final;

		std::string m_eventName;
		CreatureEventType_t m_type;
		bool m_isLoaded;
};

#endif
