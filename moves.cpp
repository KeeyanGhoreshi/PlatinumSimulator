#include "moves.h"
#include "battle_effects.h"
#include <string>

Move::Move() {
    id = MoveId::NO_MOVE;
}
Move::Move(MoveId _id, std::string _name, int _flags, int _power = 0, int _accuracy = 0, bool _secondary = false, int _secondaryAccuracy = 0, int _effect = 0, Type _moveType = Type::None, DamageType _defend = DamageType::NONE, DamageType _attack = DamageType::NONE, int _priority = 0, bool _highCritRatio = false, int _range = RANGE_TARGET) {
    id = _id;
    name = _name;
    power = _power;
    accuracy = _accuracy;
    secondary = _secondary;
    secondaryAccuracy = _secondaryAccuracy;
    effect = _effect;
    moveType = _moveType;
    attack = _attack;
    defend = _defend;
    priority = _priority;
    highCritRatio = _highCritRatio;
    disabled = false;
    range = _range;
    flags = _flags;
}


Move Empty = Move();
Move Struggle = Move(MoveId::STRUGGLE, "Struggle", FLAG_CONTACT_PROTECT, 40, 100, false, 0, 0, Type::None, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move Tackle = Move(MoveId::TACKLE, "Tackle", FLAG_CONTACT_PROTECT, 35,100,false,0, 0, Type::Normal, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move Return = Move(MoveId::RETURN, "Return", FLAG_CONTACT_PROTECT, 102,100,false,0, 0, Type::Normal, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move Growl = Move(MoveId::GROWL, "Growl",FLAG_CAN_PROTECT, 0, 100, false, 0, BATTLE_EFFECT_ATK_DOWN, Type::Normal);
Move Headbutt = Move(MoveId::HEADBUTT, "Headbutt", FLAG_CONTACT_PROTECT, 70, 100, true, 30, BATTLE_EFFECT_FLINCH_HIT, Type::Normal, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);

Move RockThrow = Move(MoveId::ROCK_THROW, "Rock Throw", FLAG_CAN_PROTECT, 50, 90, false, 0, BATTLE_EFFECT_HIT, Type::Rock, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move FlameWheel = Move(MoveId::FLAME_WHEEL, "Flame Wheel", FLAG_CONTACT_PROTECT, 60, 100, true, 10, BATTLE_EFFECT_BURN_HIT, Type::Fire, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move Taunt = Move(MoveId::TAUNT, "Taunt", FLAG_CAN_PROTECT, 0, 100, false, 0, BATTLE_EFFECT_TAUNT, Type::Dark, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move NightSlash = Move(MoveId::NIGHT_SLASH, "Night Slash", FLAG_CONTACT_PROTECT, 70, 100, false, 0, BATTLE_EFFECT_HIGH_CRITICAL, Type::Dark, DamageType::PHYSICAL, DamageType::PHYSICAL, 0, true);
Move PoisonGas = Move(MoveId::POISON_GAS, "Poison Gas", 0, 55, false, 0, BATTLE_EFFECT_STATUS_POISON, Type::Poison);
Move Screech = Move(MoveId::SCREECH, "Screech", FLAG_CAN_PROTECT, 0, 85, false, 0, BATTLE_EFFECT_DEF_DOWN_2, Type::Normal, DamageType::NONE, DamageType::NONE, 0);
Move Smokescreen = Move(MoveId::SMOKESCREEN, "SmokeScreen", 0, 100, false, 0, BATTLE_EFFECT_ACC_DOWN, Type::Normal);
Move QuickAttack = Move(MoveId::QUICK_ATTACK, "Quick Attack",FLAG_CONTACT_PROTECT, 40, 100, false, 0, BATTLE_EFFECT_PRIORITY_1, Type::Normal, DamageType::PHYSICAL, DamageType::PHYSICAL, 1);

Move AquaJet = Move(MoveId::AQUA_JET, "Aqua Jet",FLAG_CONTACT_PROTECT, 40, 100, false, 0, BATTLE_EFFECT_PRIORITY_1, Type::Water, DamageType::PHYSICAL, DamageType::PHYSICAL, 1);
Move WingAttack = Move(MoveId::WING_ATTACK, "Wing Attack",FLAG_CONTACT_PROTECT, 60,100,false,0, 0, Type::Flying, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move DoubleTeam = Move(MoveId::DOUBLE_TEAM, "Double Team",0, 0, 0, false, 0, BATTLE_EFFECT_EVA_UP, Type::Normal, DamageType::NONE, DamageType::NONE, 0, false, RANGE_SELF);
Move Endeavor = Move(MoveId::ENDEAVOR, "Endeavor",FLAG_CONTACT_PROTECT, 1, 100, false, 0, BATTLE_EFFECT_SET_HP_EQUAL_TO_USER, Type::Normal, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move Charm = Move(MoveId::CHARM, "Charm",FLAG_CAN_PROTECT, 0, 100, false, 0, BATTLE_EFFECT_ATK_DOWN_2, Type::Normal);
Move Spark = Move(MoveId::SPARK, "Spark",FLAG_CONTACT_PROTECT, 65, 100, true, 30, BATTLE_EFFECT_PARALYZE_HIT, Type::Electric, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move ConfuseRay = Move(MoveId::CONFUSE_RAY, "Confuse Ray",FLAG_CAN_PROTECT, 0, 100, false, 0, BATTLE_EFFECT_STATUS_CONFUSE, Type::Ghost);
Move Psybeam = Move(MoveId::PSYBEAM, "Psybeam",FLAG_CAN_PROTECT, 65, 100, true, 10, BATTLE_EFFECT_CONFUSE_HIT, Type::Psychic, DamageType::SPECIAL, DamageType::SPECIAL);
Move MagicalLeaf = Move(MoveId::MAGICAL_LEAF, "Magical Leaf",FLAG_CAN_PROTECT, 60, 0, false, 0, BATTLE_EFFECT_BYPASS_ACCURACY, Type::Grass, DamageType::SPECIAL, DamageType::SPECIAL);
Move ShadowBall = Move(MoveId::SHADOW_BALL, "Shadow Ball",FLAG_CAN_PROTECT, 80, 100, true, 20, BATTLE_EFFECT_LOWER_SP_DEF_HIT, Type::Ghost, DamageType::SPECIAL, DamageType::SPECIAL);
Move Bite = Move(MoveId::BITE, "Bite",FLAG_CONTACT_PROTECT, 60, 100, true, 30, BATTLE_EFFECT_FLINCH_HIT, Type::Dark, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move Substitute = Move(MoveId::SUBSTITUTE, "Substitute",0, 0, 0, false, 0, BATTLE_EFFECT_SET_SUBSTITUTE, Type::Normal, DamageType::NONE, DamageType::NONE, 0, false, RANGE_SELF);
Move SandAttack = Move(MoveId::SAND_ATTACK, "Sand Attack",FLAG_CAN_PROTECT, 0, 100, false, 0, BATTLE_EFFECT_ACC_DOWN, Type::Ground);
Move TakeDown = Move(MoveId::TAKE_DOWN, "Take Down",FLAG_CONTACT_PROTECT, 90, 85, false, 0, BATTLE_EFFECT_RECOIL_QUARTER, Type::Normal, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move Yawn = Move(MoveId::YAWN, "Yawn",FLAG_CAN_PROTECT, 0,0, false, 0, BATTLE_EFFECT_STATUS_SLEEP_NEXT_TURN, Type::Normal, DamageType::NONE, DamageType::NONE);
Move Toxic = Move(MoveId::TOXIC, "Toxic",FLAG_CAN_PROTECT, 0, 85, false, 0, BATTLE_EFFECT_STATUS_BADLY_POISON, Type::Poison);
Move Supersonic = Move(MoveId::SUPERSONIC, "Supersonic",FLAG_CAN_PROTECT, 0, 55, false, 0, BATTLE_EFFECT_STATUS_CONFUSE, Type::Normal);
Move AirCutter = Move(MoveId::AIR_CUTTER, "Air Cutter",FLAG_CONTACT_PROTECT, 55, 95, false, 0, BATTLE_EFFECT_HIGH_CRITICAL, Type::Flying, DamageType::SPECIAL, DamageType::SPECIAL, 0, true);
Move Surf = Move(MoveId::SURF, "Surf",FLAG_CAN_PROTECT, 95,100,false,0, 0, Type::Water, DamageType::SPECIAL, DamageType::SPECIAL);
Move Revenge = Move(MoveId::REVENGE, "Revenge",FLAG_CONTACT_PROTECT, 3, 100, false, 0, BATTLE_EFFECT_DOUBLE_POWER_IF_HIT, Type::Fighting, DamageType::PHYSICAL, DamageType::PHYSICAL, -4);
Move PoisonJab = Move(MoveId::POISON_JAB, "Poison Jab",FLAG_CONTACT_PROTECT, 80, 100, true, 30, BATTLE_EFFECT_POISON_HIT, Type::Poison, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move FaintAttack = Move(MoveId::FAINT_ATTACK, "Faint Attack",FLAG_CONTACT_PROTECT, 60, 0, false, 0, BATTLE_EFFECT_BYPASS_ACCURACY, Type::Dark, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move MudBomb = Move(MoveId::MUD_BOMB, "Mud Bomb",FLAG_CAN_PROTECT, 65, 85, true, 30, BATTLE_EFFECT_LOWER_ACCURACY_HIT, Type::Ground, DamageType::SPECIAL, DamageType::SPECIAL);
Move Earthquake = Move(MoveId::EARTHQUAKE, "Earthquake", FLAG_CAN_PROTECT, 100, 100, false, 0, 0, Type::Ground, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move ChargeBeam = Move(MoveId::CHARGE_BEAM, "Charge Beam",FLAG_CAN_PROTECT, 50, 90, true, 70, BATTLE_EFFECT_RAISE_SP_ATK_HIT, Type::Electric, DamageType::SPECIAL, DamageType::SPECIAL);
Move ThunderWave = Move(MoveId::THUNDER_WAVE, "Thunder Wave",FLAG_CAN_PROTECT, 0, 100, false, 0, BATTLE_EFFECT_STATUS_PARALYZE, Type::Electric);
Move IronTail = Move(MoveId::IRON_TAIL, "Iron Tail",FLAG_CONTACT_PROTECT, 100, 75, true, 30, BATTLE_EFFECT_LOWER_DEFENSE_HIT, Type::Steel, DamageType::PHYSICAL, DamageType::PHYSICAL);
Move SignalBeam = Move(MoveId::SIGNAL_BEAM, "Signal Beam", FLAG_CAN_PROTECT, 75, 100, true, 10, BATTLE_EFFECT_CONFUSE_HIT, Type::Bug, DamageType::SPECIAL, DamageType::SPECIAL);
Move FocusBlast = Move(MoveId::FOCUS_BLAST, "Focus Blast", FLAG_CAN_PROTECT, 120, 70, true, 10, BATTLE_EFFECT_LOWER_SP_DEF_HIT, Type::Fighting, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move ThunderFang = Move(MoveId::THUNDER_FANG, "Thunder Fang", FLAG_CONTACT_PROTECT, 65, 95, false, 0, BATTLE_EFFECT_FLINCH_PARALYZE_HIT, Type::Electric, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move FireFang = Move(MoveId::FIRE_FANG, "Fire Fang", FLAG_CONTACT_PROTECT, 65, 95, false, 0, BATTLE_EFFECT_FLINCH_BURN_HIT, Type::Fire, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move IceFang = Move(MoveId::ICE_FANG, "Ice Fang", FLAG_CONTACT_PROTECT, 65, 95, false, 0, BATTLE_EFFECT_FLINCH_FREEZE_HIT, Type::Ice, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move Crunch = Move(MoveId::CRUNCH, "Crunch", FLAG_CONTACT_PROTECT, 80, 100, true, 20, BATTLE_EFFECT_LOWER_DEFENSE_HIT, Type::Dark, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);

Move ThunderPunch = Move(MoveId::THUNDERPUNCH, "ThunderPunch", FLAG_CONTACT_PROTECT, 75, 100, true, 10, BATTLE_EFFECT_PARALYZE_HIT, Type::Electric, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move FirePunch = Move(MoveId::FIRE_PUNCH, "Fire Punch", FLAG_CONTACT_PROTECT, 75, 100, true, 10, BATTLE_EFFECT_BURN_HIT, Type::Fire, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move GigaImpact = Move(MoveId::GIGA_IMPACT, "Giga Impact", FLAG_CONTACT_PROTECT, 150, 90, false, 0, BATTLE_EFFECT_RECHARGE_AFTER, Type::Normal, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move Protect = Move(MoveId::PROTECT, "Protect", 0, 0, 0, false, 0, BATTLE_EFFECT_PROTECT, Type::Normal, DamageType::NONE, DamageType::NONE, 3, false, RANGE_SELF);
Move SuckerPunch = Move(MoveId::SUCKER_PUNCH, "Sucker Punch", FLAG_CONTACT_PROTECT, 80, 100, false, 0, BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING, Type::Dark, DamageType::PHYSICAL, DamageType::PHYSICAL, 1);
Move ToxicSpikes = Move(MoveId::TOXIC_SPIKES, "Toxic Spikes", 0, 0, 0, false, 0, BATTLE_EFFECT_TOXIC_SPIKES, Type::Poison, DamageType::NONE, DamageType::NONE, 0);

Move AerialAce = Move(MoveId::AERIAL_ACE, "Aerial Ace", FLAG_CONTACT_PROTECT, 60, 0, false, 0, BATTLE_EFFECT_BYPASS_ACCURACY, Type::Flying, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move AirSlash = Move(MoveId::AIR_SLASH, "Air Slash", FLAG_CAN_PROTECT, 75, 95, true, 30, BATTLE_EFFECT_FLINCH_HIT, Type::Flying, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move AuraSphere = Move(MoveId::AURA_SPHERE, "Aura Sphere", FLAG_CAN_PROTECT, 90, 0, false, 0, BATTLE_EFFECT_BYPASS_ACCURACY, Type::Fighting, DamageType::SPECIAL, DamageType::SPECIAL, 0);
// bounce not implemented fully
Move Bounce = Move(MoveId::BOUNCE, "Bounce", FLAG_CONTACT_PROTECT, 85, 85, true, 30, BATTLE_EFFECT_BOUNCE, Type::Flying, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move Avalanche = Move(MoveId::AVALANCHE, "Avalanche", FLAG_CONTACT_PROTECT, 60, 100, false, 0, BATTLE_EFFECT_DOUBLE_POWER_IF_HIT, Type::Ice, DamageType::PHYSICAL, DamageType::PHYSICAL, -4);
Move AttackOrder = Move(MoveId::ATTACK_ORDER, "Attack Order", FLAG_CAN_PROTECT, 90, 100, false, 0, BATTLE_EFFECT_HIGH_CRITICAL, Type::Bug, DamageType::PHYSICAL, DamageType::PHYSICAL, 0, true);
Move BugBuzz = Move(MoveId::BUG_BUZZ, "Bug Buzz", FLAG_CAN_PROTECT, 90, 100, true, 10, BATTLE_EFFECT_LOWER_SP_DEF_HIT, Type::Bug, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move AquaTail = Move(MoveId::AQUA_TAIL, "Aqua Tail", FLAG_CONTACT_PROTECT, 90, 90, false, 0, BATTLE_EFFECT_HIT, Type::Water, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move CalmMind = Move(MoveId::CALM_MIND, "Calm Mind", 0, 0, 0, false, 0, BATTLE_EFFECT_SP_ATK_SP_DEF_UP, Type::Psychic, DamageType::NONE, DamageType::NONE, 0, false, RANGE_SELF);
Move CloseCombat = Move(MoveId::CLOSE_COMBAT, "Close Combat", FLAG_CONTACT_PROTECT, 120, 100, false, 0, BATTLE_EFFECT_DEF_SPD_DOWN_HIT, Type::Fighting, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move DefendOrder = Move(MoveId::DEFEND_ORDER, "Defend Order", 0, 0, 0, false, 0, BATTLE_EFFECT_DEF_SPD_UP, Type::Bug, DamageType::NONE, DamageType::NONE, 0);
Move DarkPulse = Move(MoveId::DARK_PULSE, "Dark Pulse", FLAG_CAN_PROTECT, 80, 100, true, 20, BATTLE_EFFECT_FLINCH_HIT, Type::Dark, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move CrossPoison = Move(MoveId::CROSS_POISON, "Cross Poison", FLAG_CONTACT_PROTECT, 70, 100, true, 10, BATTLE_EFFECT_HIGH_CRITICAL_POISON_HIT, Type::Poison, DamageType::PHYSICAL, DamageType::PHYSICAL, 0, true);
Move DragonPulse = Move(MoveId::DRAGON_PULSE, "Dragon Pulse", FLAG_CAN_PROTECT, 90, 100, false, 0, BATTLE_EFFECT_HIT, Type::Dragon, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move DrainPunch = Move(MoveId::DRAIN_PUNCH, "Drain Punch", FLAG_CONTACT_PROTECT, 60, 100, false, 0, BATTLE_EFFECT_RECOVER_HALF_DAMAGE_DEALT, Type::Fighting, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move DragonRush = Move(MoveId::DRAGON_RUSH, "Dragon Rush", FLAG_CONTACT_PROTECT, 100, 75, true, 20, BATTLE_EFFECT_FLINCH_HIT, Type::Dragon, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move EnergyBall = Move(MoveId::ENERGY_BALL, "Energy Ball", FLAG_CAN_PROTECT, 80, 100, true, 10, BATTLE_EFFECT_LOWER_SP_DEF_HIT, Type::Grass, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move EarthPower = Move(MoveId::EARTH_POWER, "Earth Power", FLAG_CAN_PROTECT, 90, 100, true, 10, BATTLE_EFFECT_LOWER_SP_DEF_HIT, Type::Ground, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move Extrasensory = Move(MoveId::EXTRASENSORY, "Extrasensory", FLAG_CAN_PROTECT, 80, 100, true, 10, BATTLE_EFFECT_FLINCH_HIT, Type::Psychic, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move Flamethrower = Move(MoveId::FLAMETHROWER, "Flamethrower", FLAG_CAN_PROTECT, 95, 100, true, 10, BATTLE_EFFECT_BURN_HIT, Type::Fire, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move ExtremeSpeed = Move(MoveId::EXTREME_SPEED, "ExtremeSpeed", FLAG_CONTACT_PROTECT, 80, 100, false, 0, BATTLE_EFFECT_PRIORITY_1, Type::Normal, DamageType::PHYSICAL, DamageType::PHYSICAL, 1);
Move FlareBlitz = Move(MoveId::FLARE_BLITZ, "Flare Blitz", FLAG_CONTACT_PROTECT, 120, 100, true, 10, BATTLE_EFFECT_RECOIL_BURN_HIT, Type::Fire, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move GyroBall = Move(MoveId::GYRO_BALL, "Gyro Ball", FLAG_CONTACT_PROTECT, 1, 100, false, 0, BATTLE_EFFECT_POWER_BASED_ON_LOW_SPEED, Type::Steel, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move IceBeam = Move(MoveId::ICE_BEAM, "Ice Beam", FLAG_CAN_PROTECT, 95, 100, true, 10, BATTLE_EFFECT_FREEZE_HIT, Type::Ice, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move HealOrder = Move(MoveId::HEAL_ORDER, "Heal Order", 0, 0, 0, false, 0, BATTLE_EFFECT_RESTORE_HALF_HP, Type::Bug, DamageType::NONE, DamageType::NONE, 0);
Move HyperBeam = Move(MoveId::HYPER_BEAM, "Hyper Beam", FLAG_CAN_PROTECT, 150, 90, false, 0, BATTLE_EFFECT_RECHARGE_AFTER, Type::Normal, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move IronHead = Move(MoveId::IRON_HEAD, "Iron Head", FLAG_CONTACT_PROTECT, 80, 100, true, 30, BATTLE_EFFECT_FLINCH_HIT, Type::Steel, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move Megahorn = Move(MoveId::MEGAHORN, "Megahorn", FLAG_CONTACT_PROTECT, 120, 85, false, 0, BATTLE_EFFECT_HIT, Type::Bug, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
// light screen not implemented
Move LightScreen = Move(MoveId::LIGHT_SCREEN, "Light Screen", 0, 0, 0, false, 0, BATTLE_EFFECT_SET_LIGHT_SCREEN, Type::Psychic, DamageType::NONE, DamageType::NONE, 0);
Move LeafBlade = Move(MoveId::LEAF_BLADE, "Leaf Blade", FLAG_CONTACT_PROTECT, 90, 100, false, 0, BATTLE_EFFECT_HIGH_CRITICAL, Type::Grass, DamageType::PHYSICAL, DamageType::PHYSICAL, 0, true);
Move MachPunch = Move(MoveId::MACH_PUNCH, "Mach Punch", FLAG_CONTACT_PROTECT, 40, 100, false, 0, BATTLE_EFFECT_PRIORITY_1, Type::Fighting, DamageType::PHYSICAL, DamageType::PHYSICAL, 1);
Move MirrorCoat = Move(MoveId::MIRROR_COAT, "Mirror Coat", 0, 1, 100, false, 0, BATTLE_EFFECT_MIRROR_COAT, Type::Psychic, DamageType::SPECIAL, DamageType::SPECIAL, -5);
Move PowerGem = Move(MoveId::POWER_GEM, "Power Gem", FLAG_CAN_PROTECT, 70, 100, false, 0, BATTLE_EFFECT_HIT, Type::Rock, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move Overheat = Move(MoveId::OVERHEAT, "Overheat", FLAG_CAN_PROTECT, 140, 90, true, 100, BATTLE_EFFECT_USER_SP_ATK_DOWN_2, Type::Fire, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move PsychoCut = Move(MoveId::PSYCHO_CUT, "Psycho Cut", FLAG_CAN_PROTECT, 70, 100, false, 0, BATTLE_EFFECT_HIGH_CRITICAL, Type::Psychic, DamageType::PHYSICAL, DamageType::PHYSICAL, 0, true);
Move RockWrecker = Move(MoveId::ROCK_WRECKER, "Rock Wrecker", FLAG_CAN_PROTECT, 150, 90, false, 0, BATTLE_EFFECT_RECHARGE_AFTER, Type::Rock, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move Sandstorm = Move(MoveId::SANDSTORM, "Sandstorm", 0, 0, 0, false, 0, BATTLE_EFFECT_WEATHER_SANDSTORM, Type::Rock, DamageType::NONE, DamageType::NONE, 0);
Move Recover = Move(MoveId::RECOVER, "Recover", 0, 0, 0, false, 0, BATTLE_EFFECT_RESTORE_HALF_HP, Type::Normal, DamageType::NONE, DamageType::NONE, 0);
// reflect not implemented
Move Reflect = Move(MoveId::REFLECT, "Reflect", 0, 0, 0, false, 0, BATTLE_EFFECT_SET_REFLECT, Type::Psychic, DamageType::NONE, DamageType::NONE, 0);
Move Solarbeam = Move(MoveId::SOLARBEAM, "SolarBeam", FLAG_CAN_PROTECT, 120, 100, false, 0, BATTLE_EFFECT_SKIP_CHARGE_TURN_IN_SUN, Type::Grass, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move ShockWave = Move(MoveId::SHOCK_WAVE, "Shock Wave", FLAG_CAN_PROTECT, 60, 0, false, 0, BATTLE_EFFECT_BYPASS_ACCURACY, Type::Electric, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move SilverWind = Move(MoveId::SILVER_WIND, "Silver Wind", FLAG_CAN_PROTECT, 60, 100, true, 10, BATTLE_EFFECT_RAISE_ALL_STATS_HIT, Type::Bug, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move SludgeBomb = Move(MoveId::SLUDGE_BOMB, "Sludge Bomb", FLAG_CAN_PROTECT, 90, 100, true, 30, BATTLE_EFFECT_POISON_HIT, Type::Poison, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move SunnyDay = Move(MoveId::SUNNY_DAY, "Sunny Day", 0, 0, 0, false, 0, BATTLE_EFFECT_WEATHER_SUN, Type::Fire, DamageType::NONE, DamageType::NONE, 0);
// Uturn not fully implemented
Move UTurn = Move(MoveId::U_TURN, "U-turn", FLAG_CONTACT_PROTECT, 70, 100, false, 0, BATTLE_EFFECT_SWITCH_HIT, Type::Bug, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move Thunderbolt = Move(MoveId::THUNDERBOLT, "Thunderbolt", FLAG_CAN_PROTECT, 95, 100, true, 10, BATTLE_EFFECT_PARALYZE_HIT, Type::Electric, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move StoneEdge = Move(MoveId::STONE_EDGE, "Stone Edge", FLAG_CAN_PROTECT, 100, 80, false, 0, BATTLE_EFFECT_HIGH_CRITICAL, Type::Rock, DamageType::PHYSICAL, DamageType::PHYSICAL, 0, true);
Move WaterPulse = Move(MoveId::WATER_PULSE, "Water Pulse", FLAG_CAN_PROTECT, 60, 100, true, 20, BATTLE_EFFECT_CONFUSE_HIT, Type::Water, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move ZenHeadbutt = Move(MoveId::ZEN_HEADBUTT, "Zen Headbutt", FLAG_CONTACT_PROTECT, 80, 90, true, 20, BATTLE_EFFECT_FLINCH_HIT, Type::Psychic, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move WillOWisp = Move(MoveId::WILL_O_WISP, "Will-O-Wisp", FLAG_CAN_PROTECT, 0, 75, false, 0, BATTLE_EFFECT_STATUS_BURN, Type::Fire, DamageType::NONE, DamageType::NONE, 0);
Move XScissor = Move(MoveId::X_SCISSOR, "X-Scissor", FLAG_CONTACT_PROTECT, 80, 100, false, 0, BATTLE_EFFECT_HIT, Type::Bug, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);

Move StealthRock = Move(MoveId::STEALTH_ROCK, "Stealth Rock", 0, 0, 0, false, 0, BATTLE_EFFECT_STEALTH_ROCK, Type::Rock, DamageType::NONE, DamageType::NONE, 0);
Move Encore = Move(MoveId::ENCORE, "Encore", FLAG_CAN_PROTECT, 0, 100, false, 0, BATTLE_EFFECT_ENCORE, Type::Normal, DamageType::NONE, DamageType::NONE, 0);
Move PsychicMove = Move(MoveId::PSYCHIC, "Psychic",FLAG_CAN_PROTECT, 90, 100, true, 10, BATTLE_EFFECT_LOWER_SP_DEF_HIT, Type::Psychic, DamageType::SPECIAL, DamageType::SPECIAL);
Move TrickRoom = Move(MoveId::TRICK_ROOM, "Trick Room", 0, 0, 0, false, 0, BATTLE_EFFECT_TRICK_ROOM, Type::Psychic, DamageType::NONE, DamageType::NONE, -7);
Move Hail = Move(MoveId::HAIL, "Hail", 0, 0, 0, false, 0, BATTLE_EFFECT_WEATHER_HAIL, Type::Ice, DamageType::NONE, DamageType::NONE, 0);
Move BatonPass = Move(MoveId::BATON_PASS, "Baton Pass", 0, 0, 0, false, 0, BATTLE_EFFECT_PASS_STATS_AND_STATUS, Type::Normal, DamageType::NONE, DamageType::NONE, 0);
Move AquaRing = Move(MoveId::AQUA_RING, "Aqua Ring", 0, 0, 0, false, 0, BATTLE_EFFECT_RESTORE_HP_EVERY_TURN, Type::Water, DamageType::NONE, DamageType::NONE, 0);
Move Brine = Move(MoveId::BRINE, "Brine", FLAG_CAN_PROTECT, 65, 100, false, 0, BATTLE_EFFECT_DOUBLE_POWER_WHEN_BELOW_HALF, Type::Water, DamageType::SPECIAL, DamageType::SPECIAL, 0);
Move Waterfall = Move(MoveId::WATERFALL, "Waterfall", FLAG_CONTACT_PROTECT, 80, 100, true, 20, BATTLE_EFFECT_FLINCH_HIT, Type::Water, DamageType::PHYSICAL, DamageType::PHYSICAL, 0);
Move Twister = Move(MoveId::TWISTER, "Twister",FLAG_CONTACT_PROTECT, 40, 100, true, 20, BATTLE_EFFECT_FLINCH_HIT, Type::Dragon, DamageType::PHYSICAL, DamageType::PHYSICAL);
