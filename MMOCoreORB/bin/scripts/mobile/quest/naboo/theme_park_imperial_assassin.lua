theme_park_imperial_assassin = Creature:new {
	objectName = "@mob/creature_names:assassin",
	socialGroup = "thug",
	faction = "thug",
	level = 18,
	chanceHit = 0.32,
	damageMin = 170,
	damageMax = 180,
	baseXp = 1257,
	baseHAM = 4100,
	baseHAMmax = 5000,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
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
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_cobral_assassin_twk_male_01.iff",
			"object/mobile/dressed_criminal_assassin_human_female_01.iff",
			"object/mobile/dressed_criminal_assassin_human_male_01.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 4500000},
				{group = "tailor_components", chance = 1500000},
				{group = "loot_kit_parts", chance = 2000000},
				{group = "printer_parts", chance = 1000000},
				{group = "wearables_common", chance = 1000000}
			}
		}
	},
	weapons = {"pirate_weapons_light"},
	conversationTemplate = "",
	attacks = merge(brawlermaster,marksmanmaster)
}

CreatureTemplates:addCreatureTemplate(theme_park_imperial_assassin, "theme_park_imperial_assassin")
