kima_nazith = Creature:new {
	objectName = "",
	customName = "Kima Nazith",
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 100,
	chanceHit = 1,
	damageMin = 645,
	damageMax = 1000,
	baseXp = 9429,
	baseHAM = 24000,
	baseHAMmax = 30000,
	armor = 0,
	resists = {0,0,0,0,0,0,0,0,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_commoner_naboo_human_female_03.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "kima_nazith_mission_giver_convotemplate",
	attacks = {}
}

CreatureTemplates:addCreatureTemplate(kima_nazith, "kima_nazith")
