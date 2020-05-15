escaped_traitor_quest_vinzel = Creature:new {
	objectName = "@mob/creature_names:abandoned_rebel_private",
	socialGroup = "rebel",
	faction = "rebel",
	level = 27,
	chanceHit = 0.36,
	damageMin = 240,
	damageMax = 250,
	baseXp = 2822,
	baseHAM = 7200,
	baseHAMmax = 8800,
	armor = 0,
	resists = {30,30,40,40,-1,40,-1,-1,-1},
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
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_rebel_trooper_bith_m_01.iff",
		"object/mobile/dressed_rebel_trooper_twk_male_01.iff",
		"object/mobile/dressed_rebel_trooper_twk_female_01.iff",
		"object/mobile/dressed_rebel_trooper_human_female_01.iff",
		"object/mobile/dressed_rebel_trooper_human_male_01.iff",
		"object/mobile/dressed_rebel_trooper_sullustan_male_01.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "warden_vinzel_haylon_mission_target_convotemplate",
	reactionStf = "@npc_reaction/military",
	attacks = brawlermaster
}

CreatureTemplates:addCreatureTemplate(escaped_traitor_quest_vinzel, "escaped_traitor_quest_vinzel")
