

#ifndef CustomCommands_H_
#define CustomCommands_H_

#include "server/zone/managers/mission/MissionManager.h"
#include "server/zone/managers/stringid/StringIdManager.h"

class CustomCommands : public QueueCommand {
public:
    CustomCommands(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

    int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

        if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return GENERALERROR;


        ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();

		if (ghost == NULL)
			return GENERALERROR;
			
		int adminLevelCheck = ghost->getAdminLevel();

        StringTokenizer args(arguments.toString());

        try {
			String command;

			if(args.hasMoreTokens()){
				args.getStringToken(command);	
			} else {
				throw Exception();
			}
			
			command = command.toLowerCase();
            if(command == "placebounty"){
				placeBounty(creature, args);
			} else {
				throw Exception();
			}
        } catch (Exception& e){
            StringBuffer text;
			text << endl;
			text << "Custom Commands: The Command" << endl;
			text << "- - - - - - - - - - - - - - - - - - -" << endl;

            text << "/custom placebounty"  << endl;
			text << "- Sets a bounty on a player"  << endl;
			text << endl;

            creature->sendSystemMessage(text.toString());
        }
        
        return SUCCESS;
    }

    void placeBounty(CreatureObject* creature, StringTokenizer args) const {
        String arg0;
        uint64 arg1;

        args.getStringToken(arg0);
        arg1 = args.getLongToken();
    }


};

#endif //CustomCommands_H_
