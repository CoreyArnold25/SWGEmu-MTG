sand_pirate_guard = Creature:new {
	customName = "a Sand Pirate guard",
	socialGroup = "sand_pirate",
	faction = "sand_pirate",
	level = 24,
	chanceHit = 0.35,
	damageMin = 230,
	damageMax = 240,
	baseXp = 2543,
	baseHAM = 6800,
	baseHAMmax = 8300,
	armor = 0,
	resists = {15,15,10,10,10,-1,-1,-1,-1},
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
	creatureBitmask = PACK + KILLER,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
				"object/mobile/dressed_criminal_thug_aqualish_female_01.iff",
				"object/mobile/dressed_criminal_thug_aqualish_female_02.iff",
				"object/mobile/dressed_criminal_thug_aqualish_male_01.iff",
				"object/mobile/dressed_criminal_thug_aqualish_male_02.iff",
				"object/mobile/dressed_criminal_thug_bothan_female_01.iff",
				"object/mobile/dressed_criminal_thug_bothan_male_01.iff",
				"object/mobile/dressed_criminal_thug_human_female_01.iff",
				"object/mobile/dressed_criminal_thug_human_female_02.iff",
				"object/mobile/dressed_criminal_thug_human_male_01.iff",
				"object/mobile/dressed_criminal_thug_human_male_02.iff",
				"object/mobile/dressed_criminal_thug_rodian_female_01.iff",
				"object/mobile/dressed_criminal_thug_rodian_male_01.iff",
				"object/mobile/dressed_criminal_thug_trandoshan_female_01.iff",
				"object/mobile/dressed_criminal_thug_trandoshan_male_01.iff",
				"object/mobile/dressed_criminal_thug_zabrak_female_01.iff",
				"object/mobile/dressed_criminal_thug_zabrak_male_01.iff",
				"object/mobile/dressed_crook_zabrak_female_01.iff",
				"object/mobile/dressed_crook_zabrak_male_01.iff",
				"object/mobile/dressed_desperado_bith_female_01.iff",
				"object/mobile/dressed_desperado_bith_male_01.iff",
				"object/mobile/dressed_goon_twk_female_01.iff",
				"object/mobile/dressed_goon_twk_male_01.iff",
				"object/mobile/dressed_hoodlum_zabrak_female_01.iff",
				"object/mobile/dressed_hoodlum_zabrak_male_01.iff",
				"object/mobile/dressed_hooligan_rodian_female_01.iff",
				"object/mobile/dressed_hooligan_rodian_male_01.iff",
				"object/mobile/dressed_mugger.iff",
				"object/mobile/dressed_robber_human_female_01.iff",
				"object/mobile/dressed_robber_human_male_01.iff",
				"object/mobile/dressed_robber_twk_female_01.iff",
				"object/mobile/dressed_robber_twk_male_01.iff",
				"object/mobile/dressed_ruffian_zabrak_female_01.iff",
				"object/mobile/dressed_ruffian_zabrak_male_01.iff",
				"object/mobile/dressed_villain_trandoshan_female_01.iff",
				"object/mobile/dressed_villain_trandoshan_male_01.iff"
	},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 4000000},
				{group = "wearables_common", chance = 2000000},
				{group = "corsec_weapons", chance = 2500000},
				{group = "tailor_components", chance = 1500000}
			}
		}
	},
	weapons = {"corsec_police_weapons"},
	conversationTemplate = "",
	reactionStf = "@npc_reaction/slang",
	attacks = merge(brawlermaster,marksmanmaster)
}

CreatureTemplates:addCreatureTemplate(sand_pirate_guard, "sand_pirate_guard")