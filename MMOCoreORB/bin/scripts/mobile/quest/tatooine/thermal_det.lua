thermal_det = Creature:new {
	objectName = "",
	customName = "Thermal Det",
	socialGroup = "",
	faction = "",
	level = 13,
	chanceHit = 0.3,
	damageMin = 140,
	damageMax = 150,
	baseXp = 714,
	baseHAM = 1500,
	baseHAMmax = 1900,
	armor = 0,
	resists = {0,0,0,0,10,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = NONE,
	optionsBitmask = INVULNERABLE + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/kitonak_male.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "kitster_banai_mission_target_convotemplate",
	attacks = brawlermid
}

CreatureTemplates:addCreatureTemplate(thermal_det, "thermal_det")
