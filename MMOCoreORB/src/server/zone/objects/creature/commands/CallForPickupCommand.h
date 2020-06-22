#ifndef CALLFORPICKUPCOMMAND_H_
#define CALLFORPICKUPCOMMAND_H_

#include "QueueCommand.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/tangible/ticket/TicketObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/player/sui/callbacks/TicketSelectionSuiCallback.h"
#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/objects/area/ActiveArea.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/managers/planet/PlanetTravelPoint.h"
#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/managers/structure/StructureManager.h"
#include "server/zone/objects/tangible/eventperk/ShuttleBeacon.h"
#include "server/zone/objects/tangible/terminal/travel/TravelTerminal.h"
#include "server/zone/objects/tangible/terminal/ticketcollector/TicketCollector.h"


class CallForPickupCommand : public QueueCommand {
public:

	CallForPickupCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		ManagedReference<Zone*> zone = creature->getZone();

		if (zone == nullptr)
			return GENERALERROR;

		ManagedReference<PlanetManager*> planetManager = zone->getPlanetManager();
		ManagedReference<ZoneServer*> zoneServer = zone->getZoneServer();

		if(zoneServer == nullptr)
			return GENERALERROR;

		if(!canSpawnShuttle(zone, zoneServer, creature, planetManager))
			return GENERALERROR;

		SceneObject* shuttle = zone->getZoneServer()->createObject(STRING_HASHCODE("object/creature/npc/theme_park/player_shuttle.iff"), 0);
		
		if (shuttle == nullptr)
			return GENERALERROR;

		float x = creature->getPositionX() + 15;
		float y = creature->getPositionY();
		float z = creature->getPositionZ();
		
		shuttle->initializePosition(x, z, y);

		CreatureObject* shuttle2 = cast<CreatureObject*>( shuttle);
		ManagedWeakReference<CreatureObject*> shuttleObject = shuttle2;

		if(shuttle2 == nullptr || shuttleObject == nullptr)
			return GENERALERROR;

		ManagedReference<CreatureObject*> strongReference = shuttleObject.get();

		if (strongReference == nullptr) {
			return GENERALERROR;
		}

		Locker _lock(strongReference);

		float xworld = strongReference->getWorldPositionX();
		float yworld = strongReference->getWorldPositionY();
		float zworld = strongReference->getWorldPositionZ();

		Vector3 arrivalVector(xworld, yworld, zworld);

		String zoneName = zone->getZoneName();

		String pointName = creature->getFirstName() + " Shuttle";

		PlanetTravelPoint* planetTravelPoint = new PlanetTravelPoint(zoneName, pointName, arrivalVector, arrivalVector, strongReference);
		
		if(planetTravelPoint == nullptr)
			return GENERALERROR;

		planetManager->addPlayerCityTravelPoint(planetTravelPoint);

		//planetManager->scheduleShuttle(strongReference, 0);
		//planetManager->scheduleShuttle(strongReference, PlanetManager::SHUTTLEPORT);
		zone->transferObject(shuttle, -1, true);

		spawnTerminalAndTicket(zoneServer, creature, zone);

		planetTravelPoint->setShuttle(strongReference);
		planetTravelPoint->setPointName(pointName);

		return SUCCESS;
	}

	bool canSpawnShuttle(Zone* zone, ZoneServer* zoneServer, CreatureObject* player, ManagedReference<PlanetManager*> planetManager) const {
		if(player->isInCombat()){
			player->sendSystemMessage("You must not be in combat.");
			return false;
		}

		if(player->getCityRegion() != nullptr){
			player->sendSystemMessage("You must be outside of a city region to call for a pickup.");
			return false;
		}

		if (zone->getZoneName().contains("space_"))
			return false;

		if (player->isSwimming())
			return false;

		ManagedReference<CityRegion*> city = player->getCityRegion().get();

		if (city != nullptr) {
			if (city->isClientRegion())
				return false;

			if (city->isZoningEnabled() && !city->hasZoningRights(player->getObjectID()))
				return false;
		}

		int x = player->getWorldPositionX();
		int y = player->getWorldPositionY();
		int nearbyPerks = 0;

		CloseObjectsVector* vec = (CloseObjectsVector*) player->getCloseObjects();

		if (vec == nullptr)
			return false;

		SortedVector<QuadTreeEntry*> closeObjects;
		vec->safeCopyTo(closeObjects);

		for (int i = 0; i < closeObjects.size(); ++i) {
			SceneObject* obj = cast<SceneObject*>(closeObjects.get(i));

			if (obj == nullptr)
				continue;

			SharedObjectTemplate* objectTemplate = obj->getObjectTemplate();

			if (objectTemplate == nullptr)
				continue;

			float radius = objectTemplate->getNoBuildRadius();

			if (radius > 0 && player->isInRange(obj, radius))
				return false;

			if (objectTemplate->isSharedStructureObjectTemplate()) {
				if (StructureManager::instance()->isInStructureFootprint(cast<StructureObject*>(obj), x, y, 0))
					return false;
			}

			if (obj->isEventPerk() && player->isInRange(obj, 32) && ++nearbyPerks > 2)
				return false;
		}

		SortedVector<ManagedReference<ActiveArea* > > activeAreas;
		zone->getInRangeActiveAreas(x, y, &activeAreas, true);

		for (int i = 0; i < activeAreas.size(); ++i) {
			ActiveArea* area = activeAreas.get(i);

			if (area->isNoBuildArea())
				return false;
		}

		if (planetManager->isInRangeWithPoi(x, y, 100))
			return false;

		return true;
	}

	int spawnTerminalAndTicket(ZoneServer* zoneServer, CreatureObject* player, Zone* zone) const {
		SceneObject* travel = zone->getZoneServer()->createObject(STRING_HASHCODE("object/tangible/terminal/terminal_travel.iff"), 0);
		
		if (travel == nullptr)
			return GENERALERROR;

		float x = player->getPositionX() + 5;
		float y = player->getPositionY();
		float z = player->getPositionZ();

		travel->initializePosition(x, z, y);
		zone->transferObject(travel, -1, true);

		SceneObject* collector = zoneServer->createObject(STRING_HASHCODE("object/tangible/travel/ticket_collector/ticket_collector.iff"), 0);

		if(collector == nullptr)
			return GENERALERROR;

		float x2 = player->getPositionX() + 10;
		float y2 = player->getPositionY();
		float z2 = player->getPositionZ();

		collector->initializePosition(x2, z2, y2);
		zone->transferObject(collector, -1, true);

		return SUCCESS;
	}
};

#endif 
