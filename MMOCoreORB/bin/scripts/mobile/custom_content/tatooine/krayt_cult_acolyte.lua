krayt_cult_acolyte = Creature:new {
	customName = "a Krayt cult acolyte",
	socialGroup = "krayt_cult",
	faction = "krayt_cult",
	level = 17,
	chanceHit = 0.31,
	damageMin = 180,
	damageMax = 200,
	baseXp = 1192,
	baseHAM = 3000,
	baseHAMmax = 3600,
	armor = 0,
	resists = {5,5,5,25,-1,-1,-1,-1,-1},
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


	templates = {
		"object/mobile/dressed_commoner_artisan_bith_male_01.iff",
		"object/mobile/dressed_commoner_artisan_sullustan_male_01.iff",
		"object/mobile/dressed_commoner_artisan_trandoshan_male_01.iff",
		"object/mobile/dressed_commoner_fat_human_female_01.iff",
		"object/mobile/dressed_commoner_fat_human_female_02.iff",
		"object/mobile/dressed_commoner_fat_human_male_02.iff",
		"object/mobile/dressed_commoner_fat_twilek_female_01.iff",
		"object/mobile/dressed_commoner_fat_twilek_female_02.iff",
		"object/mobile/dressed_commoner_fat_twilek_male_01.iff",
		"object/mobile/dressed_commoner_fat_zabrak_female_01.iff",
		"object/mobile/dressed_commoner_fat_zabrak_male_01.iff",
		"object/mobile/dressed_commoner_naboo_bothan_female_01.iff",
		"object/mobile/dressed_commoner_naboo_bothan_male_01.iff",
		"object/mobile/dressed_commoner_naboo_human_female_01.iff",
		"object/mobile/dressed_commoner_naboo_human_female_03.iff",
		"object/mobile/dressed_commoner_naboo_human_female_05.iff",
		"object/mobile/dressed_commoner_naboo_human_female_07.iff",
		"object/mobile/dressed_commoner_naboo_human_male_01.iff",
		"object/mobile/dressed_commoner_naboo_human_male_03.iff",
		"object/mobile/dressed_commoner_naboo_human_male_05.iff",
		"object/mobile/dressed_commoner_naboo_human_male_07.iff",
		"object/mobile/dressed_commoner_naboo_moncal_female_01.iff",
		"object/mobile/dressed_commoner_naboo_moncal_male_02.iff",
		"object/mobile/dressed_commoner_naboo_twilek_female_02.iff",
		"object/mobile/dressed_commoner_naboo_twilek_male_02.iff",
		"object/mobile/dressed_commoner_naboo_zabrak_male_01.iff",
		"object/mobile/dressed_commoner_old_human_female_02.iff",
		"object/mobile/dressed_commoner_old_human_male_02.iff",
		"object/mobile/dressed_commoner_tatooine_aqualish_female_03.iff",
		"object/mobile/dressed_commoner_tatooine_aqualish_female_08.iff",
		"object/mobile/dressed_commoner_tatooine_aqualish_male_04.iff",
		"object/mobile/dressed_commoner_tatooine_aqualish_male_07.iff",
		"object/mobile/dressed_commoner_tatooine_bith_female_01.iff",
		"object/mobile/dressed_commoner_tatooine_bith_female_03.iff",
		"object/mobile/dressed_commoner_tatooine_bith_male_03.iff",
		"object/mobile/dressed_commoner_tatooine_bith_male_06.iff",
		"object/mobile/dressed_commoner_tatooine_devaronian_male_02.iff",
		"object/mobile/dressed_commoner_tatooine_devaronian_male_04.iff",
		"object/mobile/dressed_commoner_tatooine_ishitib_male_03.iff",
		"object/mobile/dressed_commoner_tatooine_nikto_male_01.iff",
		"object/mobile/dressed_commoner_tatooine_nikto_male_06.iff",
		"object/mobile/dressed_commoner_tatooine_rodian_female_03.iff",
		"object/mobile/dressed_commoner_tatooine_rodian_male_02.iff",
		"object/mobile/dressed_commoner_tatooine_sullustan_male_02.iff",
		"object/mobile/dressed_commoner_tatooine_trandoshan_female_01.iff",
		"object/mobile/dressed_commoner_tatooine_trandoshan_male_01.iff"
	},
	lootGroups = {},
	weapons = {"pirate_weapons_light"},
	conversationTemplate = "",
	attacks = merge(marksmannovice,brawlernovice)
}

CreatureTemplates:addCreatureTemplate(krayt_cult_acolyte, "krayt_cult_acolyte")
