theme_park_imperial_staff_corporal = Creature:new {
  objectName = "@mob/creature_names:imperial_staff_corporal",
  randomNameType = NAME_GENERIC,
  randomNameTag = true,
  socialGroup = "imperial",
  faction = "imperial",
  level = 17,
  chanceHit = 0.32,
  damageMin = 160,
  damageMax = 170,
  baseXp = 112,
  baseHAM = 3500,
  baseHAMmax = 4300,
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
  optionsBitmask = AIENABLED + CONVERSABLE,
  diet = HERBIVORE,

  templates = {"object/mobile/dressed_imperial_soldier_m.iff"},
  lootGroups = {},
  weapons = {},
  conversationTemplate = "theme_park_nightsister_mission_target_convotemplate",
  attacks = merge(brawlermaster,marksmanmaster)
}

CreatureTemplates:addCreatureTemplate(theme_park_imperial_staff_corporal, "theme_park_imperial_staff_corporal")