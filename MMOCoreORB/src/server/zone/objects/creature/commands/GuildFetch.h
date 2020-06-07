/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef GUILDFETCHCOMMAND_H_
#define GUILDFETCHCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/managers/guild/GuildManager.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/guild/GuildObject.h"
#include "server/zone/objects/tangible/terminal/guild/GuildTerminal.h"

class GuildFetchCommand : public QueueCommand {
public:

	GuildFetchCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return INVALIDPARAMETERS;

		CreatureObject* player = cast<CreatureObject*>(creature);

		if (!player->isInGuild()) {
			player->sendSystemMessage("@base_player:guildremove_not_in_guild"); //You are not in a guild.
			return GENERALERROR;
		}

		if(server == nullptr)
			return GENERALERROR;

		ManagedReference<ZoneServer* > zserv = server->getZoneServer();
		if( zserv == nullptr )
			return GENERALERROR;

		ManagedReference<GuildObject*> guild = player->getGuildObject().get();
		ManagedReference<GuildManager*> guildManager = zserv->getGuildManager();
		ManagedReference<PlayerManager*> playerManager = zserv->getPlayerManager();

		if( guild == nullptr || guildManager == nullptr || playerManager == nullptr)
			return GENERALERROR;

        ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
        if (ghost == nullptr)
		    return GENERALERROR;

        player->getPlayerObject()->closeSuiWindowType(SuiWindowType::GUILD_MEMBER_LIST);

		ManagedReference<SuiListBox*> suiBox = new SuiListBox(player, SuiWindowType::GUILD_MEMBER_LIST);
		suiBox->setPromptTitle("@guild:members_title"); // Guild Members
		suiBox->setPromptText("These are the guild memebers."); // These are the current members of the guild. To perform an operation on a guild member, select them in the list and press Ok.

		GuildMemberList* memberList = guild->getGuildMemberList();

		for (int i = 0; i < memberList->size(); ++i) {
			GuildMemberInfo* gmi = &memberList->get(i);

			if (gmi == nullptr)
				continue;

			uint64 playerID = gmi->getPlayerID();
			ManagedReference<SceneObject*> obj = zserv->getObject(playerID);

			if (obj == nullptr || !obj->isPlayerCreature())
				continue;

			CreatureObject* member = cast<CreatureObject*>( obj.get());
			bool IsOnline = member->isOnline();
			String isOnlineTxt = (IsOnline ? " - \\#00FF00\\Online" : " - \\#FF0000\\Offline");

			suiBox->addMenuItem(member->getDisplayedName() + isOnlineTxt, playerID);
		} 

		suiBox->setCancelButton(false, "");
		player->getPlayerObject()->addSuiBox(suiBox);
		player->sendMessage(suiBox->generateMessage());

		return SUCCESS;
	}

};

#endif //GUILDFETCHCOMMAND_H_
