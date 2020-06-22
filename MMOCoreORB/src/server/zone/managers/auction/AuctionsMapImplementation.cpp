/*
 * AuctionsMap.cpp
 *
 *  Created on: 6/3/2012
 *      Author: kyle
 */

#include "server/zone/managers/auction/AuctionsMap.h"
#include "server/zone/objects/auction/AuctionItem.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/tangible/components/vendor/VendorDataComponent.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/packets/auction/ItemSoldMessage.h"
#include "server/db/ServerDatabase.h"

int AuctionsMapImplementation::addItem(CreatureObject* player, SceneObject* vendor, AuctionItem* item) {
	if(vendor == nullptr || vendor->getZone() == nullptr)
		return ItemSoldMessage::VENDORNOTWORKING;

	String planet = vendor->getZone()->getZoneName();

	String region = "@planet_n:" + vendor->getZone()->getZoneName();
	ManagedReference<CityRegion*> cityRegion = vendor->getCityRegion().get();
	if(cityRegion != nullptr)
		region = cityRegion->getRegionName();

	if(vendor->isBazaarTerminal())
		return addBazaarItem(player, planet, region, vendor, item);

	return addVendorItem(player, planet, region, vendor, item);
}

int AuctionsMapImplementation::addVendorItem(CreatureObject* player, const String& planet, const String& region, SceneObject* vendor, AuctionItem* item) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if(allItems.contains(item->getAuctionedItemObjectID()))
		return ItemSoldMessage::ALREADYFORSALE;

	if(!vendorItemsForSale.contains(vendor->getObjectID()) && !vendorItemsForSale.createTerminalListing(planet, region, vendor))
		return ItemSoldMessage::VENDORNOTWORKING;

	Reference<TerminalItemList*> vendorItems = vendorItemsForSale.get(vendor->getObjectID());
	if(vendorItems->isEmpty()) {

		VendorDataComponent* vendorData = nullptr;
		DataObjectComponentReference* data = vendor->getDataObjectComponent();
		if(data != nullptr && data->get() != nullptr && data->get()->isVendorData()) {
			vendorData = cast<VendorDataComponent*>(data->get());
			if(vendorData == nullptr)
				return ItemSoldMessage::VENDORNOTWORKING;

			vendorItems->setSearchable(vendorData->isVendorSearchEnabled());
		}
	}

	if(vendorItems == nullptr)
		return ItemSoldMessage::INVALIDAUCTIONER;

	//Locker vlocker(vendorItems);

	int result = vendorItems->put(item);

	if(result == -1)
		return ItemSoldMessage::UNKNOWNERROR;

	allItems.put(item->getAuctionedItemObjectID(), item);

	float xcoord = 0;
	float ycoord = 0;

	if (vendor != nullptr){
		xcoord = vendor->getPositionX();
		ycoord = vendor->getPositionY();
	}

	String itemType = GetObjectTypeName(item->getItemType());

	StringBuffer bazaar_statement;
	bazaar_statement << "INSERT INTO bazaar_market (objectId, ownerid, ownername, amount, objectName, sold, deleted, onbazaar, planet, xcoordinate, ycoordinate, objecttype)";
	bazaar_statement << " SELECT * FROM (SELECT " << item->getObjectID();
	bazaar_statement << " as objectId, " << item->getOwnerID() << " as ownerid, '" << item->getOwnerName();
	bazaar_statement << "' as ownername, " << item->getPrice() << " as amount, '" << item->getItemName();
	bazaar_statement << "' as objectName, 0 as sold, 0 as deleted, ";
	bazaar_statement << (item->isOnBazaar() ? "1" : "0") << " as onbazaar,'" << planet << "' as planet, ";
	if(xcoord != 0)
			bazaar_statement << xcoord << " as xcoordinate, ";
		else
			bazaar_statement << "NULL as xcoordinate, ";
		if(ycoord != 0)
			bazaar_statement << ycoord << " as ycoordinate, ";
		else
			bazaar_statement << "NULL as ycoordinate, ";
	bazaar_statement << "'" << itemType << "' as objecttype ";
	bazaar_statement << ") as tmp";
	bazaar_statement << " WHERE NOT EXISTS (SELECT objectId FROM bazaar_market WHERE objectId = " << item->getObjectID() << " AND deleted = 0 and sold = 0) LIMIT 1; ";

	return ItemSoldMessage::SUCCESS;
}

int AuctionsMapImplementation::addBazaarItem(CreatureObject* player, const String& planet, const String& region, SceneObject* vendor, AuctionItem* item) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if(allItems.contains(item->getAuctionedItemObjectID()))
		return ItemSoldMessage::ALREADYFORSALE;

	if(!bazaarItemsForSale.contains(vendor->getObjectID()) && !bazaarItemsForSale.createTerminalListing(planet, region, vendor))
		return ItemSoldMessage::VENDORNOTWORKING;

	Reference<TerminalItemList*> bazaarItems = bazaarItemsForSale.get(vendor->getObjectID());

	if(bazaarItems == nullptr)
		return ItemSoldMessage::INVALIDAUCTIONER;

	//Locker blocker(bazaarItems);
	int result = bazaarItems->put(item);

	if(result == -1)
		return ItemSoldMessage::UNKNOWNERROR;

	allItems.put(item->getAuctionedItemObjectID(), item);

	float xcoord = 0;
	float ycoord = 0;

	if (vendor != nullptr){
		xcoord = vendor->getPositionX();
		ycoord = vendor->getPositionY();
	}

	String itemType = GetObjectTypeName(item->getItemType());

	StringBuffer bazaar_statement;
	bazaar_statement << "INSERT INTO bazaar_market (objectId, ownerid, ownername, amount, objectName, sold, deleted, onbazaar, planet, xcoordinate, ycoordinate, objecttype)";
	bazaar_statement << " SELECT * FROM (SELECT " << item->getObjectID();
	bazaar_statement << " as objectId, " << item->getOwnerID() << " as ownerid, '" << item->getOwnerName();
	bazaar_statement << "' as ownername, " << item->getPrice() << " as amount, '" << item->getItemName();
	bazaar_statement << "' as objectName, 0 as sold, 0 as deleted, ";
	bazaar_statement << (item->isOnBazaar() ? "1" : "0") << " as onbazaar,'" << planet << "' as planet, ";
	if(xcoord != 0)
			bazaar_statement << xcoord << " as xcoordinate, ";
		else
			bazaar_statement << "NULL as xcoordinate, ";
		if(ycoord != 0)
			bazaar_statement << ycoord << " as ycoordinate, ";
		else
			bazaar_statement << "NULL as ycoordinate, ";
	bazaar_statement << "'" << itemType << "' as objecttype ";
	bazaar_statement << ") as tmp";
	bazaar_statement << " WHERE NOT EXISTS (SELECT objectId FROM bazaar_market WHERE objectId = " << item->getObjectID() << " AND deleted = 0 and sold = 0) LIMIT 1; ";

	ServerDatabase::instance()->executeQuery(bazaar_statement.toString());

	return ItemSoldMessage::SUCCESS;
}

void AuctionsMapImplementation::deleteItem(SceneObject* vendor, AuctionItem* item, bool deleteAuctionedObject) {
	Locker locker(_this.getReferenceUnsafeStaticCast());
	removeItem(vendor, item);

	StringBuffer bazaar_statement;
	bazaar_statement << "UPDATE bazaar_market SET deleted = 1 WHERE objectid = " << item->getObjectID();

	ServerDatabase::instance()->executeQuery(bazaar_statement.toString());

	item->destroyAuctionItemFromDatabase(false, deleteAuctionedObject);
}

void AuctionsMapImplementation::removeItem(SceneObject* vendor, AuctionItem* item) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if(vendor != nullptr) {
		if(vendor->isBazaarTerminal())
			removeBazaarItem(vendor, item);
		else
			removeVendorItem(vendor, item);
	}

	StringBuffer bazaar_statement;
	bazaar_statement << "UPDATE bazaar_market SET deleted = 1 WHERE objectid = " << item->getObjectID();

	ServerDatabase::instance()->executeQuery(bazaar_statement.toString());

	allItems.drop(item->getAuctionedItemObjectID());
}

void AuctionsMapImplementation::removeVendorItem(SceneObject* vendor, AuctionItem* item) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	Reference<TerminalItemList*> vendorItems = vendorItemsForSale.get(vendor->getObjectID());

	if(vendorItems == nullptr)
		return;

	//Locker vlocker(vendorItems);

	StringBuffer bazaar_statement;
	bazaar_statement << "UPDATE bazaar_market SET deleted = 1 WHERE objectid = " << item->getObjectID();

	ServerDatabase::instance()->executeQuery(bazaar_statement.toString());

	if(vendorItems->drop(item)){
		return;
	}

	logger.error("unable to remove vendor item");
}

void AuctionsMapImplementation::removeBazaarItem(SceneObject* vendor,  AuctionItem* item) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	Reference<TerminalItemList*> bazaarItems = bazaarItemsForSale.get(vendor->getObjectID());

	if(bazaarItems == nullptr)
		return;

	StringBuffer bazaar_statement;
	bazaar_statement << "UPDATE bazaar_market SET deleted = 1 WHERE objectid = " << item->getObjectID();

	ServerDatabase::instance()->executeQuery(bazaar_statement.toString());

	//Locker blocker(bazaarItems);

	if(!bazaarItems->drop(item))
		logger.error("unable to remove bazaar item");

}

TerminalListVector AuctionsMapImplementation::getVendorTerminalData(const String& planet, const String& region, SceneObject* vendor) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	return vendorItemsForSale.getTerminalData(planet, region, vendor);
}

TerminalListVector AuctionsMapImplementation::getBazaarTerminalData(const String& planet, const String& region, SceneObject* vendor) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	return bazaarItemsForSale.getTerminalData(planet, region, vendor);
}
int AuctionsMapImplementation::getPlayerItemCount(CreatureObject* player) {
	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

	if (ghost == nullptr)
		return 0;

	const SortedVector<unsigned long long>* ownedVendors = ghost->getOwnedVendors();
	int total = 0;

	for (int i = 0; i < ownedVendors->size(); i++) {
		ManagedReference<SceneObject*> vendor = player->getZoneServer()->getObject(ownedVendors->elementAt(i));

		total += getVendorItemCount(vendor);
	}

	return total;
}

int AuctionsMapImplementation::getVendorItemCount(SceneObject* vendor, bool forSaleOnly) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if(vendor == nullptr) {
		logger.error("null vendor in AuctionsMapImplementation::getVendorItemCount");
		return 0;
	}

	Reference<TerminalItemList*> vendorItems = vendorItemsForSale.get(vendor->getObjectID());

	if(vendorItems == nullptr)
		return 0;

	int size = 0;

	ReadLocker rlocker(vendorItems);

	for (int i = 0; i < vendorItems->size(); ++i) {
		AuctionItem* item = vendorItems->get(i);
		if (item == nullptr)
			continue;

		if (forSaleOnly && item->getStatus() != AuctionItem::FORSALE)
			continue;

		int itemSize = item->getSize();

		if (itemSize > 50)
			size += 50;
		else if (itemSize > 0)
			size += itemSize;
		else
			size++;
	}

	return size;
}

void AuctionsMapImplementation::deleteTerminalItems(SceneObject* vendor) {
	if(vendor->isBazaarTerminal()) {
		return;
	}

	Locker locker(_this.getReferenceUnsafeStaticCast());

	Reference<TerminalItemList*> vendorItems = vendorItemsForSale.get(vendor->getObjectID());
	ZoneServer* zserv = vendor->getZoneServer();

	if(vendorItems != nullptr) {
		ReadLocker rlocker(vendorItems);

		for(int i = 0; i < vendorItems->size(); ++i) {
			Reference<AuctionItem*> item = vendorItems->get(i);

			if(item != nullptr) {
				allItems.drop(item->getAuctionedItemObjectID());
				item->destroyAuctionItemFromDatabase(false, true);
			}
		}
	}

	vendorItemsForSale.dropTerminalListing(vendor);
}

void AuctionsMapImplementation::updateUID(SceneObject* vendor, const String& oldUID, const String& newUID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (vendor == nullptr) {
		logger.error("nullptr vendor while updating UID");
		return;
	}

	Zone* zone = vendor->getZone();

	if (zone == nullptr) {
		logger.error("nullptr zone while updating UID  Vendor Is Bazaar: " + String::valueOf(vendor->isBazaarTerminal()));
		return;
	}

	String planet = zone->getZoneName();

	String region = "@planet_n:" + planet;
	ManagedReference<CityRegion*> cityRegion = vendor->getCityRegion().get();
	if(cityRegion != nullptr)
		region = cityRegion->getRegionName();

	if(vendor->isVendor())
		vendorItemsForSale.updateTerminalUID(planet, region, vendor, newUID);
	else
		bazaarItemsForSale.updateTerminalUID(planet, region, vendor, newUID);
}

void AuctionsMapImplementation::updateVendorSearch(SceneObject* vendor, bool enabled) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (vendor == nullptr)
		return;

	vendorItemsForSale.updateTerminalSearch(vendor, enabled);
}

int AuctionsMapImplementation::getCommodityCount(CreatureObject* player) {

	Locker locker(&commoditiesLimit);

	if(!commoditiesLimit.contains(player->getObjectID()))
		return 0;

	Vector<ManagedWeakReference<AuctionItem*> >* items = &commoditiesLimit.get(player->getObjectID());

	for(int i = items->size() -1; i >= 0; --i) {
		ManagedReference<AuctionItem*> item = items->get(i);
		if(item == nullptr || item->getOwnerID() != player->getObjectID() ||
				item->getStatus() == AuctionItem::RETRIEVED) {
			items->remove(i);
		}
	}

	return items->size();
}

void AuctionsMapImplementation::addToCommodityLimit(AuctionItem* item) {

	Locker locker(&commoditiesLimit);

	if(!commoditiesLimit.contains(item->getOwnerID())) {
		Vector<ManagedWeakReference<AuctionItem*> > newItems;
		commoditiesLimit.put(item->getOwnerID(), newItems);
	}

	Vector<ManagedWeakReference<AuctionItem*> >* items = &commoditiesLimit.get(item->getOwnerID());
	if(items != nullptr)
		items->add(item);

}

void AuctionsMapImplementation::removeFromCommodityLimit(AuctionItem* item) {

	Locker locker(&commoditiesLimit);

	if(!commoditiesLimit.contains(item->getOwnerID()))
		return;

	Vector<ManagedWeakReference<AuctionItem*> >* items = &commoditiesLimit.get(item->getOwnerID());
	if(items != nullptr)
		items->removeElement(item);

	if(items->isEmpty())
		commoditiesLimit.drop(item->getOwnerID());
}


String AuctionsMapImplementation::GetObjectTypeName(int itemTypeId){
	String objectTypeName = "";
	switch(itemTypeId){
		case SceneObjectType::WEAPON:
			objectTypeName = "WEAPON";
			break;
		case SceneObjectType::MELEEWEAPON:
			objectTypeName = "MELEEWEAPON";
			break;
		case SceneObjectType::PISTOL:
			objectTypeName = "PISTOL";
			break;
		case SceneObjectType::RANGEDWEAPON:
			objectTypeName = "RANGEDWEAPON";
			break;
		case SceneObjectType::ONEHANDMELEEWEAPON:
			objectTypeName = "ONEHANDMELEEWEAPON";
			break;
		case SceneObjectType::SPECIALHEAVYWEAPON:
			objectTypeName = "SPECIALHEAVYWEAPON";
			break;
		case SceneObjectType::HEAVYWEAPON:
			objectTypeName = "HEAVYWEAPON";
			break;
		case SceneObjectType::RIFLE:
			objectTypeName = "RIFLE";
			break;
		case SceneObjectType::CARBINE:
			objectTypeName = "CARBINE";
			break;
		case SceneObjectType::POLEARM:
			objectTypeName = "POLEARM";
			break;
		case SceneObjectType::TWOHANDMELEEWEAPON:
			objectTypeName = "TWOHANDMELEEWEAPON";
			break;
		case SceneObjectType::MINE:
			objectTypeName = "MINE";
			break;
		case SceneObjectType::THROWNWEAPON:
			objectTypeName = "THROWNWEAPON";
			break;
		case SceneObjectType::ARMOR:
			objectTypeName = "ARMOR";
			break;
		case SceneObjectType::BODYARMOR:
			objectTypeName = "BODYARMOR";
			break;
		case SceneObjectType::HEADARMOR:
			objectTypeName = "HEADARMOR";
			break;
		case SceneObjectType::MISCARMOR:
			objectTypeName = "MISCARMOR";
			break;
		case SceneObjectType::LEGARMOR:
			objectTypeName = "LEGARMOR";
			break;
		case SceneObjectType::ARMARMOR:
			objectTypeName = "ARMARMOR";
			break;
		case SceneObjectType::FOOTARMOR:
			objectTypeName = "FOOTARMOR";
			break;
		case SceneObjectType::SHIELDGENERATOR:
			objectTypeName = "SHIELDGENERATOR";
			break;
		case SceneObjectType::TOOL:
			objectTypeName = "TOOL";
			break;
		case SceneObjectType::CRAFTINGTOOL:
			objectTypeName = "CRAFTINGTOOL";
			break;
		case SceneObjectType::SURVEYTOOL:
			objectTypeName = "SURVEYTOOL";
			break;
		case SceneObjectType::RECYCLETOOL:
			objectTypeName = "RECYCLETOOL";
			break;
		case SceneObjectType::CRAFTINGSTATION:
			objectTypeName = "CRAFTINGSTATION";
			break;
		case SceneObjectType::FURNITURE:
			objectTypeName = "FURNITURE";
			break;
		case SceneObjectType::FOOD:
			objectTypeName = "FOOD";
			break;
		case SceneObjectType::DRINK:
			objectTypeName = "DRINK";
			break;
		case SceneObjectType::CONTAINER:
			objectTypeName = "CONTAINER";
			break;
		case SceneObjectType::INSTALLATION:
			objectTypeName = "INSTALLATION";
			break;
		case SceneObjectType::FACTORY:
			objectTypeName = "FACTORY";
			break;
		case SceneObjectType::GENERATOR:
			objectTypeName = "GENERATOR";
			break;
		case SceneObjectType::HARVESTER:
			objectTypeName = "HARVESTER";
			break;
		case SceneObjectType::DESTRUCTIBLE:
			objectTypeName = "DESTRUCTIBLE";
			break;
		case SceneObjectType::MINEFIELD:
			objectTypeName = "MINEFIELD";
			break;
		case SceneObjectType::GARAGEINSTALLATION:
			objectTypeName = "GARAGEINSTALLATION";
			break;
		case SceneObjectType::SHUTTLEINSTALLATION:
			objectTypeName = "SHUTTLEINSTALLATION";
			break;
		case SceneObjectType::AMMUNITION:
			objectTypeName = "AMMUNITION";
			break; 
		case SceneObjectType::CHEMICAL:
			objectTypeName = "CHEMICAL";
			break; 
		case SceneObjectType::WEARABLECONTAINER:
			objectTypeName = "WEARABLECONTAINER";
			break; 
		case SceneObjectType::ELECTRONICS:
			objectTypeName = "ELECTRONICS";
			break; 
		case SceneObjectType::FLORA:
			objectTypeName = "FLORA";
			break; 
		case SceneObjectType::INSTRUMENT:
			objectTypeName = "INSTRUMENT";
			break; 
		case SceneObjectType::PHARMACEUTICAL:
			objectTypeName = "PHARMACEUTICAL";
			break; 
		case SceneObjectType::SIGN:
			objectTypeName = "SIGN";
			break; 
		case SceneObjectType::FACTORYCRATE:
			objectTypeName = "FACTORYCRATE";
			break;
		case SceneObjectType::TRAVELTICKET:
			objectTypeName = "TRAVELTICKET";
			break; 
		case SceneObjectType::GENERICITEM:
			objectTypeName = "GENERICITEM";
			break; 
		case SceneObjectType::TRAP:
			objectTypeName = "TRAP";
			break; 
		case SceneObjectType::FISHINGPOLE:
			objectTypeName = "FISHINGPOLE";
			break; 
		case SceneObjectType::FISHINGBAIT:
			objectTypeName = "FISHINGBAIT";
			break; 
		case SceneObjectType::FIREWORK:
			objectTypeName = "FIREWORK";
			break;
		case SceneObjectType::ITEM:
			objectTypeName = "ITEM";
			break; 
		case SceneObjectType::PETMEDECINE:
			objectTypeName = "PETMEDECINE";
			break; 
		case SceneObjectType::FIREWORKSHOW:
			objectTypeName = "FIREWORKSHOW";
			break; 
		case SceneObjectType::CLOTHINGATTACHMENT:
			objectTypeName = "CLOTHINGATTACHMENT";
			break; 
		case SceneObjectType::LIVESAMPLE:
			objectTypeName = "LIVESAMPLE";
			break; 
		case SceneObjectType::ARMORATTACHMENT:
			objectTypeName = "ARMORATTACHMENT";
			break; 
		case SceneObjectType::COMMUNITYCRAFTINGPROJECT:
			objectTypeName = "COMMUNITYCRAFTINGPROJECT";
			break; 
		case SceneObjectType::CRYSTAL:
			objectTypeName = "CRYSTAL";
			break;
		case SceneObjectType::DROIDPROGRAMMINGCHIP:
			objectTypeName = "DROIDPROGRAMMINGCHIP";
			break; 
		case SceneObjectType::ASTEROID:
			objectTypeName = "ASTEROID";
			break; 
		case SceneObjectType::PILOTCHAIR:
			objectTypeName = "PILOTCHAIR";
			break; 
		case SceneObjectType::OPERATIONSCHAIR:
			objectTypeName = "OPERATIONSCHAIR";
			break; 
		case SceneObjectType::TURRETACCESSLADDER:
			objectTypeName = "TURRETACCESSLADDER";
			break; 
		case SceneObjectType::CONTAINER2:
			objectTypeName = "CONTAINER2";
			break; 
		case SceneObjectType::CAMOKIT:
			objectTypeName = "CAMOKIT";
			break; 
		case SceneObjectType::FISH:
			objectTypeName = "FISH";
			break;
		case SceneObjectType::STIMPACK:
			objectTypeName = "STIMPACK";
			break; 
		case SceneObjectType::RANGEDSTIMPACK:
			objectTypeName = "RANGEDSTIMPACK";
			break; 
		case SceneObjectType::ENHANCEPACK:
			objectTypeName = "ENHANCEPACK";
			break; 
		case SceneObjectType::CUREPACK:
			objectTypeName = "CUREPACK";
			break; 
		case SceneObjectType::DOTPACK:
			objectTypeName = "DOTPACK";
			break; 
		case SceneObjectType::WOUNDPACK:
			objectTypeName = "WOUNDPACK";
			break; 
		case SceneObjectType::STATEPACK:
			objectTypeName = "STATEPACK";
			break; 
		case SceneObjectType::REVIVEPACK:
			objectTypeName = "REVIVEPACK";
			break; 
		case SceneObjectType::SLICINGTOOL:
			objectTypeName = "SLICINGTOOL";
			break; 
		case SceneObjectType::MOLECULARCLAMP:
			objectTypeName = "MOLECULARCLAMP";
			break; 
		case SceneObjectType::FLOWANALYZER:
			objectTypeName = "FLOWANALYZER";
			break; 
		case SceneObjectType::LASERKNIFE:
			objectTypeName = "LASERKNIFE";
			break; 
		case SceneObjectType::WEAPONUPGRADEKIT:
			objectTypeName = "WEAPONUPGRADEKIT";
			break; 
		case SceneObjectType::ARMORUPGRADEKIT:
			objectTypeName = "ARMORUPGRADEKIT";
			break; 
		case SceneObjectType::ANTIDECAYKIT:
			objectTypeName = "ANTIDECAYKIT";
			break; 
		case SceneObjectType::VEHICLE:
			objectTypeName = "VEHICLE";
			break; 
		case SceneObjectType::HOVERVEHICLE:
			objectTypeName = "HOVERVEHICLE";
			break; 
		case SceneObjectType::COMPONENT:
			objectTypeName = "COMPONENT";
			break;
		case SceneObjectType::ARMORCOMPONENT:
			objectTypeName = "ARMORCOMPONENT";
			break;
		case SceneObjectType::CHEMISTRYCOMPONENT:
			objectTypeName = "CHEMISTRYCOMPONENT";
			break;
		case SceneObjectType::CLOTHINGCOMPONENT:
			objectTypeName = "CLOTHINGCOMPONENT";
			break;
		case SceneObjectType::DROIDCOMPONENT :
			objectTypeName = "DROIDCOMPONENT";
			break;
		case SceneObjectType::ELECTRONICSCOMPONENT:
			objectTypeName = "ELECTRONICSCOMPONENT";
			break;
		case SceneObjectType::MUNITIONCOMPONENT:
			objectTypeName = "MUNITIONCOMPONENT";
			break;
		case SceneObjectType::STRUCTURECOMPONENT:
			objectTypeName = "STRUCTURECOMPONENT";
			break;
		case SceneObjectType::MELEEWEAPONCOMPONENT:
			objectTypeName = "MELEEWEAPONCOMPONENT";
			break;
		case SceneObjectType::RANGEDWEAPONCOMPONENT:
			objectTypeName = "RANGEDWEAPONCOMPONENT";
			break;
		case SceneObjectType::TISSUECOMPONENT:
			objectTypeName = "TISSUECOMPONENT";
			break;
		case SceneObjectType::GENETICCOMPONENT:
			objectTypeName = "GENETICCOMPONENT";
			break;
		case SceneObjectType::LIGHTSABERCRYSTAL:
			objectTypeName = "LIGHTSABERCRYSTAL";
			break;
		case SceneObjectType::COMMUNITYCRAFTINGCOMPONENT:
			objectTypeName = "COMMUNITYCRAFTINGCOMPONENT";
			break;
		case SceneObjectType::DNACOMPONENT:
			objectTypeName = "DNACOMPONENT";
			break;
		case SceneObjectType::WEAPONPOWERUP:
			objectTypeName = "WEAPONPOWERUP";
			break;
		case SceneObjectType::MELEEWEAPONPOWERUP:
			objectTypeName = "MELEEWEAPONPOWERUP";
			break;
		case SceneObjectType::RANGEDWEAPONPOWERUP:
			objectTypeName = "RANGEDWEAPONPOWERUP";
			break;
		case SceneObjectType::THROWNWEAPONPOWERUP:
			objectTypeName = "THROWNWEAPONPOWERUP";
			break;
		case SceneObjectType::HEAVYWEAPONPOWERUP:
			objectTypeName = "HEAVYWEAPONPOWERUP";
			break;
		case SceneObjectType::MINEPOWERUP:
			objectTypeName = "MINEPOWERUP";
			break;
		case SceneObjectType::SPECIALHEAVYWEAPONPOWERUP:
			objectTypeName = "SPECIALHEAVYWEAPONPOWERUP";
			break;
		case SceneObjectType::ARMORPOWERUP:
			objectTypeName = "ARMORPOWERUP";
			break;
		case SceneObjectType::BODYARMORPOWERUP:
			objectTypeName = "BODYARMORPOWERUP";
			break;
		case SceneObjectType::HEADARMORPOWERUP:
			objectTypeName = "HEADARMORPOWERUP";
			break;
		case SceneObjectType::MISCARMORPOWERUP:
			objectTypeName = "MISCARMORPOWERUP";
			break;
		case SceneObjectType::LEGARMORPOWERUP:
			objectTypeName = "LEGARMORPOWERUP";
			break;
		case SceneObjectType::ARMARMORPOWERUP:
			objectTypeName = "ARMARMORPOWERUP";
			break;
		case SceneObjectType::HANDARMORPOWERUP:
			objectTypeName = "HANDARMORPOWERUP";
			break;
		case SceneObjectType::FOOTARMORPOWERUP:
			objectTypeName = "FOOTARMORPOWERUP";
			break;
		case SceneObjectType::JEWELRY:
			objectTypeName = "JEWELRY";
			break;
		case SceneObjectType::RING:
			objectTypeName = "RING";
			break;
		case SceneObjectType::BRACELET:
			objectTypeName = "BRACELET";
			break;
		case SceneObjectType::NECKLACE:
			objectTypeName = "NECKLACE";
			break;
		case SceneObjectType::EARRING:
			objectTypeName = "EARRING";
			break;
		case SceneObjectType::RESOURCECONTAINER:
			objectTypeName = "RESOURCECONTAINER";
			break;
		case SceneObjectType::ENERGYGAS:
			objectTypeName = "ENERGYGAS";
			break;
		case SceneObjectType::ENERGYLIQUID:
			objectTypeName = "ENERGYLIQUID";
			break;
		case SceneObjectType::ENERGYRADIOACTIVE:
			objectTypeName = "ENERGYRADIOACTIVE";
			break;
		case SceneObjectType::ENERGYSOLID:
			objectTypeName = "ENERGYSOLID";
			break; 
		case SceneObjectType::INORGANICCHEMICAL:
			objectTypeName = "INORGANICCHEMICAL";
			break;
		case SceneObjectType::INORGANICGAS:
			objectTypeName = "INORGANICGAS";
			break;
		case SceneObjectType::INORGANICMINERAL:
			objectTypeName = "INORGANICMINERAL";
			break;
		case SceneObjectType::WATER:
			objectTypeName = "WATER";
			break;
		case SceneObjectType::ORGANICFOOD:
			objectTypeName = "ORGANICFOOD";
			break;
		case SceneObjectType::ORGANICHIDE:
			objectTypeName = "ORGANICHIDE";
			break;
		case SceneObjectType::ORGANICSTRUCTURAL:
			objectTypeName = "ORGANICSTRUCTURAL";
			break;
		case SceneObjectType::QUESTRESOURCE:
			objectTypeName = "QUESTRESOURCE";
			break;
		case SceneObjectType::DEED:
			objectTypeName = "DEED";
			break;
		case SceneObjectType::BUILDINGDEED:
			objectTypeName = "BUILDINGDEED";
			break;
		case SceneObjectType::INSTALLATIONDEED:
			objectTypeName = "INSTALLATIONDEED";
			break;
		case SceneObjectType::PETDEED:
			objectTypeName = "PETDEED";
			break;
		case SceneObjectType::DROIDDEED:
			objectTypeName = "DROIDDEED";
			break;
		case SceneObjectType::VEHICLEDEED:
			objectTypeName = "VEHICLEDEED";
			break;
		case SceneObjectType::RESOURCEDEED:
			objectTypeName = "RESOURCEDEED";
			break;
		case SceneObjectType::CLOTHING:
			objectTypeName = "CLOTHING";
			break; 
		case SceneObjectType::BANDOLIER:
			objectTypeName = "BANDOLIER";
			break;
		case SceneObjectType::BELT:
			objectTypeName = "BELT";
			break;
		case SceneObjectType::BODYSUIT:
			objectTypeName = "BODYSUIT";
			break;
		case SceneObjectType::CAPE:
			objectTypeName = "CAPE";
			break;
		case SceneObjectType::CLOAK:
			objectTypeName = "CLOAK";
			break;
		case SceneObjectType::FOOTWEAR:
			objectTypeName = "FOOTWEAR";
			break;
		case SceneObjectType::DRESS:
			objectTypeName = "DRESS";
			break;
		case SceneObjectType::HANDWEAR:
			objectTypeName = "HANDWEAR";
			break;
		case SceneObjectType::EYEWEAR:
			objectTypeName = "EYEWEAR";
			break;
		case SceneObjectType::HEADWEAR:
			objectTypeName = "HEADWEAR";
			break;
		case SceneObjectType::JACKET:
			objectTypeName = "JACKET";
			break;
		case SceneObjectType::PANTS:
			objectTypeName = "PANTS";
			break;
		case SceneObjectType::ROBE:
			objectTypeName = "ROBE";
			break;
		case SceneObjectType::SHIRT:
			objectTypeName = "SHIRT";
			break;
		case SceneObjectType::VEST:
			objectTypeName = "VEST";
			break; 
		case SceneObjectType::WOOKIEGARB:
			objectTypeName = "WOOKIEGARB";
			break; 
		case SceneObjectType::MISCCLOTHING:
			objectTypeName = "MISCCLOTHING";
			break;
		case SceneObjectType::SKIRT:
			objectTypeName = "SKIRT";
			break;
		default:
			objectTypeName = "MISC";
			break;
	}

	return objectTypeName;
}
