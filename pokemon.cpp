#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include "battle_effects.h"
#include "moves.h"
#include "battle.h"
// 11.9.23
// What you were working on: Getting last_waza_kind working by having the
// pokeclients keep track of their last used move
// you looked through the game's code and prevMove does not get reset
// if you use an item.  It only occurs when you use a move.
// Switching and using items do not update the previous move
// unless it is reset to NONE somewhere else, I don't think it ever gets reset
// until you make another move even if you've switched pokemon

PokeInfo::PokeInfo(std::string _name = "None", int _hp = 0, int _atk = 0, int _def = 0, int _spAtk = 0, int _spDef = 0, int _spd = 0, Type _primaryType = Type::None, Type _secondaryType = Type::None, AbilityId _ability = NO_ABILITY, AbilityId _ability2 = NO_ABILITY ){
    name = _name;
    hp = _hp;
    atk = _atk;
    def = _def;
    spAtk = _spAtk;
    spDef = _spDef;
    spd = _spd;
    primaryType = _primaryType;
    secondaryType = _secondaryType;
    ability = _ability;
    ability2 = _ability2;
}
// id, power, accuracy, secondary?, secondaryAccuracy, MoveEffect, Move Type, DamageType attack, DamageType defend, priority, HighCritRatio?
/*
Move(MoveId _id, std::string _name, int _power = 0, int _accuracy = 0, bool _secondary = false, int _secondaryAccuracy = 0, int _effect = 0, Type _moveType = None, DamageType _defend = DamageType::NONE, DamageType _attack = DamageType::NONE, int _priority = 0, bool _highCritRatio = false, int _range = 0)
*/

// TODO: Implement spark (and paralysis)

Nature hardy = {100,100,100,100,100};
Nature docile = {100,100,100,100,100};
Nature serious = {100,100,100,100,100};
Nature bashful = {100,100,100,100,100};
Nature quirky = {100,100,100,100,100};
Nature lonely = {110,90,100,100,100};
Nature brave = {110,100,100,100,90};
Nature adamant = {110,100,90,100,100};
Nature naughty = {110,100,100,90,100};
Nature bold = {90,110,100,100,100};
Nature relaxed = {100,110,100,100,90};
Nature impish = {100,110,90,100,100};
Nature lax = {100,110,100,90,100};
Nature timid = {90,100,100,100,110};
Nature hasty = {100,90,100,100,110};
Nature jolly = {100,100,90,100,110};
Nature naive = {100,100,100,90,110};
Nature modest = {90,100,110,100,100};
Nature mild = {100,90,110,100,100};
Nature quiet = {100,100,110,100,90};
Nature rash = {100,100,110,90,100};
Nature calm = {90,100,100,110,100};
Nature gentle = {100,90,100,110,100};
Nature sassy = {100,100,100,110,90};
Nature careful = {100,100,90,110,100};

std::map<Mons, PokeInfo> pokemap = {
    {CHIMCHAR, PokeInfo("Chimchar", 44,58,44,58,44,61,Type::Fire,Type::None,BLAZE)},
    {ONIX, PokeInfo("Onix", 35,45,160,30,45,70,Type::Rock,Type::Ground,ROCK_HEAD)},
    {GEODUDE, PokeInfo("Geodude", 40,80,100,30,30,20,Type::Rock,Type::Ground,ROCK_HEAD)},
    {CRANIDOS, PokeInfo("Cranidos", 67,125,40,30,30,58,Type::Rock,Type::Rock,MOLD_BREAKER)},
    {FLOATZEL, PokeInfo("Floatzel", 85,105,55,85,50,115, Type::Water, Type::None, SWIFT_SWIM)},
    {GYARADOS, PokeInfo("Gyarados", 95,125,79,60,100,81, Type::Water, Type::Flying, INTIMIDATE)},
    {QUAGSIRE, PokeInfo("Quagsire", 95,85,85,65,65,35, Type::Water, Type::Ground, SIMPLE)},
    {ROSELIA, PokeInfo("Roselia", 50,60,45,100,80,65, Type::Grass, Type::Poison, NATURAL_CURE, POISON_POINT)},
    {MONFERNO, PokeInfo("Monferno", 64,78,52,78,52,81,Type::Fire,Type::Fighting,BLAZE)},
    {SKUNTANK, PokeInfo("Skuntank", 103,93,67,71,61,84,Type::Poison, Type::Dark, STENCH, AFTERMATH)},
    {ZUBAT, PokeInfo("Zubat", 40,45,35,30,40,55,Type::Poison,Type::Flying,INNER_FOCUS)},
    {STARAVIA, PokeInfo("Staravia", 55,75,50,40,40,80, Type::Flying, Type::Normal, INTIMIDATE)},
    {PACHIRISU, PokeInfo("Pachirisu", 60,45,70,45,90,95, Type::Electric, Type::None, RUN_AWAY)}, // pickup as well, but who cares we can't catch one anyway
    {STARLY, PokeInfo("Starly", 40, 55, 30, 30, 30, 60, Type::Normal, Type::Flying, KEEN_EYE)},
    {MISMAGIUS, PokeInfo("Mismagius", 60, 60, 60, 105, 105, 105, Type::Ghost, Type::None, LEVITATE)},
    {EEVEE, PokeInfo("Eevee", 55, 55, 50, 45, 65, 55, Type::Normal, Type::None, RUN_AWAY, ADAPTABILITY)},
    {HIPPOPOTAS, PokeInfo("Hippopotas", 68, 72, 78, 38, 42, 32, Type::Ground, Type::None, SAND_STREAM)},
    {TOGETIC, PokeInfo("Togetic", 55, 40, 85, 80, 105, 40, Type::Normal, Type::Flying, HUSTLE)},
    {VAPOREON, PokeInfo("Vaporeon", 130, 65, 60, 110, 95, 65, Type::Water, Type::None, WATER_ABSORB, WATER_ABSORB)},
    {GOLBAT, PokeInfo("Golbat", 75,80,70,65,75,90,Type::Poison,Type::Flying,INNER_FOCUS)},
    {TOXICROAK, PokeInfo("Toxicroak", 83, 106, 65, 86, 65, 85, Type::Poison, Type::Fighting, ANTICIPATION, DRY_SKIN)},
    {HIPPOWDON, PokeInfo("Hippowdon", 108, 112, 118, 68, 72, 47, Type::Ground, Type::None, SAND_STREAM)},
    {JOLTEON, PokeInfo("Jolteon", 65, 65, 60, 110, 95, 130, Type::Electric, Type::None, VOLT_ABSORB, VOLT_ABSORB)},
    {RAICHU, PokeInfo("Raichu", 60, 90, 55, 90, 80, 100, Type::Electric, Type::None, STATIC)},
    {ELECTIVIRE, PokeInfo("Electivire", 75, 123, 67, 95, 85, 95, Type::Electric, Type::None, MOTOR_DRIVE)},
    {LUXRAY, PokeInfo("Luxray", 80, 120, 79, 95, 79, 70, Type::Electric, Type::None, RIVALRY, INTIMIDATE)},
    {TOGEKISS, PokeInfo("Togekiss", 85, 50, 95, 120, 115, 80, Type::Normal, Type::Flying, HUSTLE, SERENCE_GRACE)},
    {ROSERADE, PokeInfo("Roserade", 60, 70, 55, 125, 105, 90, Type::Grass, Type::Poison, NATURAL_CURE, POISON_POINT)},
    {HAUNTER, PokeInfo("Haunter", 45,50,45,115,55,95,Type::Ghost, Type::Poison, LEVITATE)},
    {RIOLU, PokeInfo("Riolu", 40,70,40,35,40,60,Type::Fighting, Type::None, STEADFAST, INNER_FOCUS)},
    {INFERNAPE, PokeInfo("Infernape", 76,104,71,104,71,108,Type::Fire,Type::Fighting,BLAZE)},
    {YANMEGA, PokeInfo("Yanmega", 86,76,86,116,56,95,Type::Bug,Type::Flying,SPEED_BOOST, TINTED_LENS)},
    {SCIZOR, PokeInfo("Scizor", 70,130,100,55,80,65,Type::Bug,Type::Steel,SWARM, TECHNICIAN)},
    {DRAPION, PokeInfo("Drapion", 70,90,110,60,75,95,Type::Poison,Type::Dark,BATTLE_ARMOR, SNIPER)},
    {HERACROSS, PokeInfo("Heracross", 80,125,75,40,95,85,Type::Bug,Type::Fighting,SWARM, GUTS)},
    {VESPIQUEN, PokeInfo("Vespiquen", 70,80,102,80,102,40,Type::Bug,Type::Flying,PRESSURE)},
    {WHISCASH, PokeInfo("Whiscash", 110,78,73,76,71,60,Type::Water,Type::Ground,OBLIVIOUS, ANTICIPATION)},
    {GOLEM, PokeInfo("Golem", 80,110,130,55,65,45,Type::Rock,Type::Ground,ROCK_HEAD, STURDY)},
{GLISCOR, PokeInfo("Gliscor", 75,95,125,45,75,95,Type::Ground,Type::Flying,HYPER_CUTTER, SAND_VEIL)},
{RHYPERIOR, PokeInfo("Rhyperior", 115,140,130,55,55,40,Type::Ground,Type::Rock,LIGHTNING_ROD, SOLID_ROCK)},
{HOUNDOOM, PokeInfo("Houndoom", 75,90,50,110,80,95,Type::Dark,Type::Fire,EARLY_BIRD, FLASH_FIRE)},
{RAPIDASH, PokeInfo("Rapidash", 65,100,70,80,80,105,Type::Fire,Type::Fire,RUN_AWAY, FLASH_FIRE)},
{MAGMORTAR, PokeInfo("Magmortar", 75,95,67,125,95,83,Type::Fire,Type::Fire,FLAME_BODY)},
{FLAREON, PokeInfo("Flareon", 65,130,60,95,110,65,Type::Fire,Type::Fire,FLASH_FIRE, FLASH_FIRE)},
{MR_MIME, PokeInfo("Mr. Mime", 40,45,65,100,120,90,Type::Psychic,Type::Psychic,SOUNDPROOF, FILTER)},
{ESPEON, PokeInfo("Espeon", 65,65,60,130,95,110,Type::Psychic,Type::Psychic,SYNCHRONIZE, SYNCHRONIZE)},
{ALAKAZAM, PokeInfo("Alakazam", 55,50,45,135,85,120,Type::Psychic,Type::Psychic,SYNCHRONIZE, INNER_FOCUS)},
{BRONZONG, PokeInfo("Bronzong", 67,89,116,79,116,33,Type::Steel,Type::Psychic,LEVITATE, HEATPROOF)},
{GALLADE, PokeInfo("Gallade", 68,125,65,65,115,80,Type::Psychic,Type::Fighting,STEADFAST, STEADFAST)},
{SPIRITOMB, PokeInfo("Spiritomb", 50,92,108,92,108,35,Type::Ghost,Type::Dark,PRESSURE)},
{LUCARIO, PokeInfo("Lucario", 70,110,70,115,70,90,Type::Fighting,Type::Steel,STEADFAST, INNER_FOCUS)},
{MILOTIC, PokeInfo("Milotic", 95,60,79,100,125,81,Type::Water,Type::Water,MARVEL_SCALE)},
{GARCHOMP, PokeInfo("Garchomp", 108,130,95,80,85,102,Type::Dragon,Type::Ground,SAND_VEIL)},
{AZELF, PokeInfo("Azelf", 75,125,70,125,70,115,Type::Psychic,Type::None,LEVITATE)},

};


// PokeInfo pokemap[1] = {
//     PokeInfo(64,78,52,78,52,81,Fire,Fighting)
// };
int getStatValue(int base, int iv, int ev, int level, int nature) {
    int evBase = ev/4;
    int statBase = ((2*base + iv + evBase) * level)/100;
    int result = ((statBase + 5) * nature)/100;

    return result;
}
void Pokemon::setEvs(int hp, int atk, int def, int spAtk, int spDef, int spd) {
    hpEv = hp;
    atkEv = atk;
    defEv = def;
    spAtkEv = spAtk;
    spDefEv = spDef;
    spdEv = spd;
}
void Pokemon::calcStats() {
    int evHp = hpEv/4;
    int hpBase = ((2*info.hp + hpIv + evHp)*level)/100;
    cHp = hpBase + level + 10;
    cAtk = getStatValue(info.atk, atkIv, atkEv, level, nature.val[0]);
    cDef = getStatValue(info.def, defIv, defEv, level, nature.val[1]);
    cSpAtk = getStatValue(info.spAtk, spAtkIv, spAtkEv, level, nature.val[2]);
    cSpDef = getStatValue(info.spDef, spDefIv, spDefEv, level, nature.val[3]);
    cSpd = getStatValue(info.spd, spdIv, spdEv, level, nature.val[4]);
    bVal.bHp = cHp;

}
Pokemon::Pokemon(){
    level = 0;
    bVal.bHp = 0;
}
Pokemon::Pokemon(int _lvl, Nature _nature, int _ivs, Mons _id, Move _moveset[4], int _ability = 1){
    level = _lvl;
    nature = _nature;
    hpIv =_ivs;
    atkIv = _ivs;
    defIv = _ivs;
    spAtkIv = _ivs;
    spDefIv = _ivs;
    spdIv = _ivs;
    moveset[0] = _moveset[0];
    moveset[1] = _moveset[1];
    moveset[2] = _moveset[2];
    moveset[3] = _moveset[3];
    setEvs(0,0,0,0,0,0);
    info = pokemap.at(_id);
    if(_ability == 1) {
        cAbility = info.ability;
    } else {
        cAbility = info.ability2;
    }
    calcStats();
    bVal.testStatus = 0;
    bVal.condition = 0;
    bVal.turnsTaunted = 0;
    bVal.moveEffectsMask = 0;
    bVal.abilityKnownToAi = false;
    bVal.item = ITEM_NONE;
    bVal.substituteHp = 0;
}
void Pokemon::sendOut() {
    bVal.atkStg = 6;
    bVal.defStg = 6;
    bVal.spAtkStg = 6;
    bVal.spDefStg = 6;
    bVal.spdStg = 6;
    bVal.evaStg = 6;
    bVal.accStg = 6;
    bVal.critStg = 0;
    bVal.volConditions = 0;
    bVal.turnsTaunted = 0;
    bVal.moveEffectsMask = 0;
    bVal.substituteHp = 0;
    bVal.substituteWasHit = false;

    bVal.movePrevByBattler = Empty;
    bVal.moveHit = Empty;
    bVal.isCharging = false;
    bVal.encoredMove = Empty;
    bVal.turnsEncored = 0;
    bVal.turnsProtected = 0;
    bVal.isProtected = false;
}
void Pokemon::setBHp(int hp) {
    bVal.bHp = hp;
}
void setupClient(PokeClient *pc) {
    pc->previouslySelectedMove = Empty;
}
// Pokemon::~Pokemon(){

// }
// Move::~Move() {
    
// }

// clients 
PokeClient getChimcharClient() {
    PokeClient p1;
    Move moveset1[4] = {Return, Return, Return, Return};
    Pokemon chimchar = Pokemon(9, hasty, 0, CHIMCHAR, moveset1);
    chimchar.setEvs(4,0,0,0,0,0);
    chimchar.calcStats();
    p1.battler = 0;
    p1.aiControl = false;
    p1.team[0] = chimchar;
    p1.name = "Player";
    return p1;
}
PokeClient getTristanClient() {
    PokeClient p2;
    Move moveset2[4] = {Tackle, Growl, QuickAttack, Empty};
    Pokemon stara = Pokemon(5, careful, 0, STARLY, moveset2);
    p2.aiControl = true;
    p2.aiLevel = 1;
    p2.battler = 0;
    p2.team[0] = stara;
    p2.name = "WildStarly";
    return p2;
}
PokeClient getRoarkClient() {
    PokeClient p2;
    Move moveset[4] = {StealthRock, RockThrow, Empty, Empty};
    Move moveset2[4] = {StealthRock, RockThrow, Screech, Empty};
    Move moveset3[4] = {Headbutt, Headbutt, Headbutt, Headbutt};
    Pokemon geodude = Pokemon(12, lax, 6, GEODUDE, moveset);
    Pokemon onix = Pokemon(12, bold, 6, ONIX, moveset2);
    Pokemon cranidos = Pokemon(14, jolly, 6, CRANIDOS, moveset3);
    p2.aiControl = true;
    p2.aiLevel = 7;
    p2.battler = 0;
    p2.team[0] = geodude;
    p2.team[1] = onix;
    p2.team[2] = cranidos;
    p2.useItems[0] = ITEM_POTION;
    p2.useItems[1] = ITEM_POTION;
    p2.numUseItems = 2;
    p2.name = "AI";
    return p2;
}
PokeClient getMonfernoRoarkClient() {
    PokeClient p1;
    Move moveset1[4] = {MachPunch, MachPunch, Taunt, Tackle};	
    Pokemon monferno = Pokemon(18, calm, 0, MONFERNO, moveset1);
    monferno.setEvs(0,0,0,0,0,0);
    monferno.calcStats(); // recalc stats after setting evs
    p1.battler = 0;
    p1.aiControl = false;
    p1.team[0] = monferno;
    p1.name = "Player";
    return p1;
}
PokeClient getMonfernoEternaClient() {
    PokeClient p1;
    Move moveset1[4] = {Tackle, FlameWheel, Taunt, Tackle};	
    Pokemon monferno = Pokemon(33, gentle, 0, MONFERNO, moveset1);
    monferno.setEvs(36,8,16,28,4,64);
    monferno.calcStats(); // recalc stats after setting evs
    // monferno.bVal.condition |= MON_CONDITION_PARALYSIS;
    p1.battler = 0;
    p1.aiControl = false;
    p1.team[0] = monferno;
    p1.name = "Player";
    return p1;
}
PokeClient getPlayerFantinaClient() {
    PokeClient p1;
    Move moveset1[4] = {Tackle, FlameWheel, Taunt, Tackle};	
    Move moveset2[4] = {Tackle, Bite, Substitute, Tackle};
    Pokemon monferno = Pokemon(35, gentle, 0, MONFERNO, moveset1);
    Pokemon eevee = Pokemon(32, timid, 0, EEVEE, moveset2);
    eevee.setEvs(252, 0, 0, 0, 0, 252);
    eevee.calcStats();
    monferno.setEvs(36,8,16,28,8,64);
    monferno.calcStats(); // recalc stats after setting evs
    // monferno.bVal.condition |= MON_CONDITION_PARALYSIS;
    p1.battler = 0;
    p1.aiControl = false;
    p1.team[0] = eevee;
    p1.name = "Player";
    return p1;
}
PokeClient getStaraviaBikeClient() {
    PokeClient p2;
    Move moveset2[4] = {QuickAttack, WingAttack, DoubleTeam, Endeavor};
    Pokemon stara = Pokemon(21, serious, 0, STARAVIA, moveset2);
    p2.aiControl = true;
    p2.aiLevel = 1;
    p2.battler = 0;
    p2.team[0] = stara;
    p2.name = "BikeGirl";
    return p2;
}

PokeClient getPachirisuClient() {
    PokeClient p2;
    Move moveset[4] = {Spark, QuickAttack, Charm, Empty};
    Pokemon pachi = Pokemon(22, bold, 0, PACHIRISU, moveset);
    p2.aiControl = true;
    p2.aiLevel = 7;
    p2.battler = 0;
    p2.team[0] = pachi;
    p2.name = "AI";
    return p2;
}

PokeClient getTripleStarlyClient() {
    PokeClient p2;
    Move moveset[4] = {Endeavor, DoubleTeam, WingAttack, QuickAttack};
    Move moveset2[4] = {QuickAttack, WingAttack, DoubleTeam, Endeavor};
    Pokemon star1 = Pokemon(17, quirky, 0, STARLY, moveset2);
    Pokemon star2 = Pokemon(18, quirky, 0, STARLY, moveset2);
    Pokemon star3 = Pokemon(19, quirky, 0, STARLY, moveset2);
    p2.aiControl = true;
    p2.aiLevel = 1;
    p2.battler = 0;
    p2.aiJumpNum = 4;
    p2.team[0] = star1;
    p2.team[1] = star2;
    p2.team[2] = star3;
    p2.name = "AI";
    return p2;
}


PokeClient getFantinaClient() {
    PokeClient p2;
    Move moveset[4] = {ShadowBall, Psybeam, MagicalLeaf, ConfuseRay};
    Pokemon mismag = Pokemon(26, impish, 12, MISMAGIUS, moveset);
    mismag.bVal.item = ITEM_SITRUS_BERRY;
    p2.aiControl = true;
    p2.aiLevel = 7;
    p2.battler = 0;
    p2.team[0] = mismag;
    p2.useItems[0] = ITEM_SUPER_POTION;
    p2.numUseItems = 1;
    p2.name = "AI";
    return p2;
}
PokeClient getHippopotasClient() {
    PokeClient p2;
    Move moveset[4] = {SandAttack, Bite, Yawn, TakeDown};
    Pokemon hippo = Pokemon(24, lonely, 31, HIPPOPOTAS, moveset);
    hippo.hpIv = 0;
    hippo.spdIv = 0;
    hippo.calcStats(); // recalc stats
    p2.aiControl = true;
    p2.aiLevel = 0;
    p2.battler = 0;
    p2.team[0] = hippo;
    p2.name = "Wild Hippo";
    return p2;
}
PokeClient getPlayerHippoClient() {
    PokeClient p1;
    Move moveset1[4] = {Charm, Charm, Charm, Charm};	
    Move moveset2[4] = {Substitute, Toxic, Substitute, Substitute};
    Pokemon vapor = Pokemon(36, lonely, 0, VAPOREON, moveset2);
    Pokemon togetic = Pokemon(24, lonely, 0, TOGETIC, moveset1);
    togetic.setEvs(6,6,15,7,2,17);
    togetic.calcStats(); // recalc stats after setting evs
    vapor.setEvs(252, 0, 0, 0, 0, 252);
    vapor.calcStats();
    p1.battler = 0;
    p1.aiControl = false;
    p1.team[0] = togetic;
    p1.team[1] = vapor;
    p1.name = "Player";
    return p1;
}

PokeClient getPlayerSaturnClient() {
    PokeClient p1;
    Move moveset1[4] = {Substitute, Surf, Substitute, Substitute};
    Move moveset2[4] = {Earthquake, Earthquake, Earthquake, Earthquake};
    Pokemon vapor = Pokemon(40, jolly, 0, VAPOREON, moveset1);
    Pokemon hippo = Pokemon(40, lonely, 0, HIPPOWDON, moveset2);
    hippo.setEvs(0, 28, 12, 0, 44, 24);
    hippo.calcStats();
    hippo.bVal.item = ITEM_PECHA_BERRY;
    vapor.setEvs(252, 0, 0, 0, 0, 252);
    vapor.calcStats();
    vapor.bVal.item = ITEM_NATURE_BERRY;

    p1.battler = 0;
    p1.aiControl = false;
    p1.team[0] = vapor;
    p1.name = "Player";
    return p1;
}

PokeClient getAISaturnClient() {
    PokeClient p2;
    Move moveset[4] = {AirCutter, Bite, Toxic, Supersonic};
    Move moveset1[4] = {PoisonJab, Revenge, MudBomb, FaintAttack};

    Pokemon bat = Pokemon(38, modest, 24, GOLBAT, moveset);
    Pokemon toad = Pokemon(40, adamant, 24, TOXICROAK, moveset1);
    p2.aiControl = true;
    p2.aiLevel = 7;
    p2.battler = 0;
    // p2.team[0] = bat;
    p2.team[0] = bat;
    p2.name = "Saturn";
    return p2;
}
PokeClient getPlayerPastoriaClient() {
    PokeClient p1;
    Move hippoMoveset[4] = {Earthquake, Yawn, Protect, Crunch};
    Move vaporMoveset[4] = {Protect, Substitute, BatonPass, Toxic};
    Move roseliaMoveset[4] = {Protect, ToxicSpikes, MagicalLeaf, Protect};

    Pokemon vapor = Pokemon(37, lonely, 0, VAPOREON, vaporMoveset);
    vapor.setEvs(252, 0, 0, 0, 0, 252);
    vapor.calcStats();
    Pokemon hippo = Pokemon(37, gentle, 0, HIPPOWDON, hippoMoveset);
    hippo.setEvs(13,0,0,0,120,0);
    hippo.calcStats();
    hippo.bVal.bHp = 100; // prime with one tick of barb

    Pokemon roselia = Pokemon(37, lonely, 0, ROSELIA, roseliaMoveset);
    roselia.bVal.item = ITEM_PERSIM_BERRY;
    roselia.setEvs(0, 0, 100, 0, 252, 0);
    roselia.calcStats();
    p1.name = "Player";
    // hippo.bVal.item = ITEM_ASPEAR_BERRY;

    p1.team[0] = vapor;
    p1.team[1] = hippo;
    p1.team[2] = roselia;
    p1.battler = 0;
    p1.aiControl = false;
    return p1;

}
PokeClient getAIWakeClient() {
    PokeClient p2;
    Move moveset[4] = {Waterfall, Brine, Bite, Twister};
    Move moveset2[4] = {Brine, IceFang, Crunch, AquaJet};
    Move moveset3[4] = {WaterPulse, MudBomb, Yawn, RockThrow};
    Pokemon gyarados = Pokemon(33, lax, 18, GYARADOS, moveset);
    Pokemon floatzel = Pokemon(37, naughty, 18, FLOATZEL, moveset2);
    Pokemon quagsire = Pokemon(34, naughty, 18, QUAGSIRE, moveset3);
    floatzel.bVal.item = ITEM_SITRUS_BERRY;
    p2.useItems[0] = ITEM_HYPER_POTION;
    p2.useItems[1] = ITEM_HYPER_POTION;
    p2.numUseItems = 2;
    p2.name = "Wake";
    p2.team[0] = gyarados;
    p2.team[1] = floatzel;
    p2.team[2] = quagsire;
    p2.aiLevel = 7;
    p2.battler = 0;
    p2.aiControl = true;
    return p2;
}
PokeClient getAIVolknerClient() {
    PokeClient p2;
    Move moveset[4] = {ThunderWave, ChargeBeam, IronTail, QuickAttack};
    Move moveset2[4] = {ChargeBeam, FocusBlast, SignalBeam, QuickAttack};
    Move moveset3[4] = {IceFang, ThunderFang, Crunch, FireFang};
    Move moveset4[4] = {ThunderPunch, FirePunch, QuickAttack, GigaImpact};
    Pokemon jolteon = Pokemon(46, relaxed, 30, JOLTEON, moveset);
    Pokemon raichu = Pokemon(46, gentle, 30, RAICHU, moveset2);
    Pokemon luxray = Pokemon(48, lax, 30, LUXRAY, moveset3);
    Pokemon electivire = Pokemon(50, impish, 0, ELECTIVIRE, moveset4);
    electivire.hpIv = 4;
    electivire.atkIv = 3;
    electivire.defIv = 29;
    electivire.spAtkIv = 18;
    electivire.spDefIv = 15;
    electivire.spdIv = 19; // electivire has a mistake and has its power set to 2500 instead of 250
    electivire.calcStats(); 
    electivire.bVal.item = ITEM_SITRUS_BERRY;
    p2.useItems[0] = ITEM_HYPER_POTION;
    p2.useItems[1] = ITEM_FULL_RESTORE;
    p2.numUseItems = 2;
    p2.name = "Volkner";
    p2.team[0] = jolteon;
    p2.team[1] = raichu;
    p2.team[2] = luxray;
    p2.team[3] = electivire;
    p2.aiLevel = 7;
    p2.battler = 0;
    p2.aiControl = true;
    return p2;
}

PokeClient getPlayerVolknerClient() {
    PokeClient p1;
    Move hippoMoveset[4] = {Earthquake, Yawn, Protect, Earthquake};
    Move vaporMoveset[4] = {Protect, Substitute, Substitute, Substitute};
    Move infernapeMoveset[4] = {Protect, Protect, Protect, Protect};
    Move rioluMoveset[4] = {DoubleTeam, Protect, Protect, Protect};
    Move roseradeMoveset[4] = {Protect, ToxicSpikes, Protect, Protect};
    Move haunterMoveset[4] = {Protect, SuckerPunch, Protect, Protect};

    Pokemon vapor = Pokemon(47, lonely, 0, VAPOREON, vaporMoveset);
    vapor.setEvs(252, 0, 0, 0, 0, 252);
    vapor.calcStats();
    Pokemon hippo = Pokemon(47, modest, 0, HIPPOWDON, hippoMoveset);
    hippo.setEvs(0, 56, 200, 0, 44, 60);
    hippo.calcStats();
    Pokemon haunter = Pokemon(44, lonely, 0, HAUNTER, haunterMoveset);
    Pokemon infernape = Pokemon(47, lonely, 0, INFERNAPE, infernapeMoveset);
    infernape.setEvs(60, 0, 12, 0, 0, 0);
    infernape.calcStats();
    Pokemon riolu = Pokemon(1, hardy, 0, RIOLU, rioluMoveset);
    Pokemon roserade = Pokemon(44, lonely, 0, ROSERADE, roseradeMoveset);
    roserade.setEvs(19, 0, 100, 0, 0, 0);
    roserade.calcStats();
    riolu.bVal.item = ITEM_STICKY_BARB;
    hippo.bVal.item = ITEM_RAWST_BERRY;
    p1.name = "Player";
    p1.team[0] = hippo;
    p1.team[1] = vapor;
    p1.team[2] = roserade;
    p1.team[3] = haunter;
    p1.team[4] = riolu;
    p1.team[5] = infernape;
    p1.battler = 0;
    p1.aiControl = false;
    return p1;

}

PokeClient getAIAaronClient() {
    PokeClient p2;
    Move moveset[4] = {AirSlash, BugBuzz, UTurn, DoubleTeam};
    Move moveset2[4] = {IronHead, XScissor, NightSlash, QuickAttack};
    Move moveset3[4] = {AttackOrder, DefendOrder, HealOrder, PowerGem};
    Move moveset4[4] = {Megahorn, CloseCombat, NightSlash, StoneEdge};
    Move moveset5[4] = {XScissor, CrossPoison, IceFang, AerialAce};
    Pokemon yanmega = Pokemon(49, rash, 30, YANMEGA, moveset);
    Pokemon scizor = Pokemon(49, gentle, 30, SCIZOR, moveset2);
    Pokemon vespiquen = Pokemon(50, careful, 30, VESPIQUEN, moveset3);
    Pokemon heracross = Pokemon(51, naughty, 30, HERACROSS, moveset4);
    Pokemon drapion = Pokemon(53, jolly, 30, DRAPION, moveset5);
    drapion.bVal.item = ITEM_SITRUS_BERRY;
    p2.useItems[0] = ITEM_FULL_RESTORE;
    p2.useItems[1] = ITEM_FULL_RESTORE;
    p2.numUseItems = 2;
    p2.name = "Aaron";
    p2.team[0] = yanmega;
    p2.team[1] = scizor;
    p2.team[2] = vespiquen;
    p2.team[3] = heracross;
    p2.team[4] = drapion;
    p2.aiLevel = 7;
    p2.battler = 0;
    p2.aiControl = true;
    return p2;
}
PokeClient getAIFlintClient() {
    PokeClient p2;
    Move moveset[4] = {Flamethrower, SludgeBomb, DarkPulse, SunnyDay};
    Move moveset2[4] = {Overheat, GigaImpact, QuickAttack, WillOWisp};
    Move moveset3[4] = {FlareBlitz, Solarbeam, Bounce, SunnyDay};
    Move moveset4[4] = {FlareBlitz, ThunderPunch, MachPunch, Earthquake};
    Move moveset5[4] = {Flamethrower, Thunderbolt, Solarbeam, HyperBeam};
    Pokemon houndoom = Pokemon(52, relaxed, 30, HOUNDOOM, moveset);
    Pokemon flareon = Pokemon(55, quirky, 30, FLAREON, moveset2);
    Pokemon rapidash = Pokemon(53, docile, 30, RAPIDASH, moveset3);
    Pokemon infernape = Pokemon(55, bold, 30, INFERNAPE, moveset4);
    Pokemon magmortar = Pokemon(57, gentle, 30, MAGMORTAR, moveset5);
    magmortar.bVal.item = ITEM_SITRUS_BERRY;
    p2.useItems[0] = ITEM_FULL_RESTORE;
    p2.useItems[1] = ITEM_FULL_RESTORE;
    p2.numUseItems = 2;
    p2.name = "Flint";
    p2.team[0] = houndoom;
    p2.team[1] = flareon;
    p2.team[2] = rapidash;
    p2.team[3] = infernape;
    p2.team[4] = magmortar;
    p2.aiLevel = 7;
    p2.battler = 0;
    p2.aiControl = true;
    return p2;
}
PokeClient getPlayerAaronClient() {
    PokeClient p1;
    Move hippoMoveset[4] = {Earthquake, Yawn, Protect, StealthRock};
    Move vaporMoveset[4] = {Protect, Substitute, Substitute, Substitute};
    Move infernapeMoveset[4] = {Flamethrower, Protect, Protect, Protect};
    Move togekissMoveset[4] = {Encore, Protect, Protect, Protect};
    Move azelfMoveset[4] = {Thunderbolt, PsychicMove, Protect, Protect};
    Move haunterMoveset[4] = {TrickRoom, SuckerPunch, Protect, Protect};

    Pokemon vapor = Pokemon(49, lonely, 0, VAPOREON, vaporMoveset);
    vapor.setEvs(252, 0, 0, 0, 0, 252);
    vapor.calcStats();
    Pokemon togekiss = Pokemon(51, lonely, 0, TOGEKISS, togekissMoveset);
    togekiss.setEvs(22, 0, 100, 0, 0, 100);
    togekiss.calcStats();
    Pokemon hippo = Pokemon(49, modest, 0, HIPPOWDON, hippoMoveset);
    hippo.setEvs(10, 56, 220, 0, 44, 60);
    hippo.calcStats();
    Pokemon haunter = Pokemon(48, lonely, 0, HAUNTER, haunterMoveset);
    Pokemon infernape = Pokemon(51, timid, 0, INFERNAPE, infernapeMoveset);
    infernape.setEvs(60, 0, 12, 0, 0, 0);
    infernape.calcStats();
    Pokemon azelf = Pokemon(51, impish, 0, AZELF, azelfMoveset);
    azelf.setEvs(0, 0, 0, 216, 0, 100);
    azelf.calcStats();
    hippo.bVal.item = ITEM_ASPEAR_BERRY;
    togekiss.bVal.item = ITEM_STICKY_BARB;
    azelf.bVal.item = ITEM_BOOST_ELECTRIC;
    // hippo.bVal.item = ITEM_BOOST_GROUND;
    azelf.bVal.bHp = 14;
    vapor.bVal.bHp = 113;
    p1.name = "Player";
    p1.team[0] = azelf;
    p1.team[1] = vapor;
    p1.team[2] = hippo;
    p1.team[3] = haunter;
    p1.team[4] = togekiss;
    p1.team[5] = infernape;
    p1.battler = 0;
    p1.aiControl = false;
    return p1;

}
PokeClient getPlayerFlintClient() {
    PokeClient p1;
    Move hippoMoveset[4] = {Earthquake, Yawn, Protect, StealthRock};
    Move vaporMoveset[4] = {AquaRing, Hail, Substitute, BatonPass};
    Move infernapeMoveset[4] = {Flamethrower, Protect, Protect, Protect};
    Move togekissMoveset[4] = {Encore, Protect, Protect, Protect};
    Move azelfMoveset[4] = {Thunderbolt, PsychicMove, Protect, Protect};
    Move haunterMoveset[4] = {TrickRoom, SuckerPunch, Protect, Protect};

    Pokemon vapor = Pokemon(50, lonely, 0, VAPOREON, vaporMoveset);
    vapor.setEvs(100, 0, 100, 0, 0, 252);
    vapor.calcStats();
    Pokemon togekiss = Pokemon(52, lonely, 0, TOGEKISS, togekissMoveset);
    togekiss.setEvs(22, 0, 100, 0, 0, 100);
    togekiss.calcStats();
    Pokemon hippo = Pokemon(52, modest, 0, HIPPOWDON, hippoMoveset);
    hippo.setEvs(10, 66, 220, 0, 44, 60);
    hippo.calcStats();
    Pokemon haunter = Pokemon(48, lonely, 0, HAUNTER, haunterMoveset);
    Pokemon infernape = Pokemon(51, timid, 0, INFERNAPE, infernapeMoveset);
    infernape.setEvs(60, 0, 12, 0, 0, 0);
    infernape.calcStats();
    Pokemon azelf = Pokemon(51, impish, 0, AZELF, azelfMoveset);
    azelf.setEvs(0, 0, 0, 216, 0, 100);
    azelf.calcStats();
    hippo.bVal.item = ITEM_BOOST_GROUND;
    vapor.bVal.item = ITEM_RAWST_BERRY;
    // hippo.bVal.item = ITEM_BOOST_GROUND;
    p1.name = "Player";
    p1.team[0] = hippo;
    p1.team[1] = vapor;
    p1.team[2] = azelf;
    p1.team[3] = haunter;
    p1.team[4] = togekiss;
    p1.team[5] = infernape;
    p1.battler = 0;
    p1.aiControl = false;
    return p1;

}
BattleContext setupJupiterFight(unsigned long startingSeed) {
    Move moveset1[4] = {Tackle, FlameWheel, Taunt, Tackle};	
    Move moveset2[4] = {NightSlash,PoisonGas, Screech, Smokescreen};
    Move moveset3[4] = {Tackle, Tackle, Tackle, Tackle}; // zubat's moveset doesn't matter, it gets OHKO'd anyway
    Pokemon skuntank = Pokemon(23, modest, 12, SKUNTANK, moveset2);
    Pokemon monferno = Pokemon(33, gentle, 0, MONFERNO, moveset1);
    Pokemon zubat = Pokemon(21,brave,12,ZUBAT,moveset3);
    skuntank.bVal.item = ITEM_SITRUS_BERRY;
    monferno.setEvs(36,8,16,28,4,64);
    monferno.calcStats(); // recalc stats after setting evs
    // monferno.bVal.item = ITEM_ORAN_BERRY;
    // monferno.bVal.item = ITEM_PECHA_BERRY;
    PokeClient p1;
    p1.battler = 0;
    PokeClient p2;
    p2.aiControl = true;
    p2.aiLevel = 7;
    p2.battler = 0;
    p2.team[1] = skuntank;
    p2.team[0] = zubat;
    p1.team[0] = monferno;
    p1.name = "Player";
    p2.name = "AI";
    RngSeed rs = {startingSeed,0};
    p1.pokeSwitch(p1.battler);
    p2.pokeSwitch(p2.battler); 
    BattleContext bc = {rs, p1, p2};
    bc.weather = 0;
    bc.weatherTurns = 0;
    bc.moveWasSuccessful = false;

    // p1.team[p1.battler].sendOut();
    // p2.team[p2.battler].sendOut();
    bc.turnNumber = 0;
    return bc;
}
BattleContext setupVarFight(unsigned long startingSeed) {
    PokeClient p1 = getPlayerVolknerClient();
    PokeClient p2 = getAIVolknerClient();

    p1.pokeSwitch(p1.battler);
    p2.pokeSwitch(p2.battler);

    // p1.team[p1.battler].sendOut();
    // p2.team[p2.battler].sendOut();
    RngSeed rs = {startingSeed, 0};
    BattleContext bc = {rs, p1, p2};
    bc.weather = 0;
    bc.weatherTurns = 0;
    bc.moveWasSuccessful = false;
    bc.turnNumber = 0;
    bc.switchCommandIndex = 0;
    return bc;
}
