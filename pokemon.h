

#ifndef POKEMON_H_
#define POKEMON_H_
#include <vector>
#include "effects.h"
#include "moves.h"
#include <string>


#define ITEM_NONE 0
#define ITEM_SITRUS_BERRY 1
#define ITEM_ORAN_BERRY 2
#define ITEM_PECHA_BERRY 3
#define ITEM_CHERI_BERRY 4
#define ITEM_PERSIM_BERRY 5
#define ITEM_BOOST_DARK 6
#define ITEM_SUPER_POTION 7
#define ITEM_NATURE_BERRY 8 // they're all the same anyway, and we won't ever get confused by one using memory mail
#define ITEM_BOOST_WATER 9
#define ITEM_ASPEAR_BERRY 10
#define ITEM_RAWST_BERRY 11
#define ITEM_HYPER_POTION 12
#define ITEM_FULL_RESTORE 13
#define ITEM_STICKY_BARB 14
#define ITEM_POWER_HERB 15
#define ITEM_BOOST_ELECTRIC 16
#define ITEM_BOOST_GROUND 17
#define ITEM_POTION 18

// int calcDamage(Pokemon attacker, Pokemon defender, Move move, int crit = 1, int randomRoll = 100);


struct Nature {
    int val[5];
};
enum AbilityId {
    NO_ABILITY,
    BLAZE,
    STENCH,
    AFTERMATH,
    IMMUNITY,
    MAGIC_GUARD,
    LEAF_GUARD,
    POISON_HEAL,
    HYDRATION,
    MOLD_BREAKER,
    VOLT_ABSORB,
    WATER_ABSORB, // not implemented yet
    MOTOR_DRIVE,
    FLASH_FIRE,
    WONDER_GUARD,
    LEVITATE,
    SOUNDPROOF,
    CLEAR_BODY,
    WHITE_SMOKE,
    NO_GUARD,
    KEEN_EYE,
    INNER_FOCUS,
    INTIMIDATE,
    SIMPLE, 
    RUN_AWAY,
    HYPER_CUTTER,
    OWN_TEMPO, // niy
    ADAPTABILITY, // niy
    SAND_STREAM,
    HUSTLE, // niy
    SERENCE_GRACE, // niy
    ANTICIPATION, // niy
    DRY_SKIN, // niy
    STATIC, //niy
    RIVALRY, //niy
    NATURAL_CURE,  //niy
    POISON_POINT, // niy
    STEADFAST, //niy
    LIMBER, //niy
    TRUANT, // niy
    SPEED_BOOST, //niy
    SWARM, // niy
    PRESSURE, // niy
    BATTLE_ARMOR, // niy
    OBLIVIOUS, // niy
    ROCK_HEAD, // niy
    LIGHTNING_ROD, // niy
    EARLY_BIRD, //niy
    FLAME_BODY, // niy
    SYNCHRONIZE, //niy
    MARVEL_SCALE, //niy
    SAND_VEIL, //niy
    TINTED_LENS, // niy
    TECHNICIAN, // niy
    SNIPER, //niy
    GUTS, // niy
    STURDY, // niy
    SOLID_ROCK, // niy
    FILTER, // niy
    HEATPROOF, // niy
    FLOWER_GIFT, // niy
    SOLAR_POWER, // niy
    WATER_VEIL, // niy
    SWIFT_SWIM, // niy

};
// note that we really only care what stat nature is reducing
// since we can just extrapolate that our bonus stat is whatever
// is least helpful at the moment
enum NatureId {
    HARDY,
    LONELY,
    BRAVE,
    ADAMANT,
    NAUGHTY,
    BOLD,
    DOCILE,
    RELAXED,
    IMPISH,
    LAX,
    TIMID,
    HASTY,
    SERIOUS,
    JOLLY,
    NAIVE,
    MODEST,
    MILD,
    QUIET,
    BASHFUL,
    RASH,
    CALM,
    GENTLE,
    SASSY,
    CAREFUL,
    QUIRKY
};
enum Mons {
    CHIMCHAR,
    MONFERNO,
    ROSELIA,
    FLOATZEL,
    GYARADOS,
    QUAGSIRE,
    ONIX,
    GEODUDE,
    CRANIDOS,
    SKUNTANK,
    ZUBAT,
    STARAVIA,
    PACHIRISU,
    STARLY,
    MISMAGIUS,
    EEVEE,
    HIPPOPOTAS,
    TOGETIC, 
    VAPOREON,
    GOLBAT,
    TOXICROAK,
    HIPPOWDON,
    JOLTEON, 
    ELECTIVIRE,
    RAICHU,
    LUXRAY,
    TOGEKISS,
    ROSERADE,
    HAUNTER,
    RIOLU,
    INFERNAPE,

    YANMEGA,
    SCIZOR,
    DRAPION,
    HERACROSS,
    VESPIQUEN,

    WHISCASH,
    GLISCOR,
    GOLEM,
    RHYPERIOR,

    HOUNDOOM,
    FLAREON,
    RAPIDASH,
    MAGMORTAR,

    MR_MIME,
    ESPEON,
    ALAKAZAM,
    GALLADE,
    BRONZONG,

    SPIRITOMB,
    MILOTIC,
    LUCARIO,
    GARCHOMP,

    AZELF,
};




struct BattleVal {
    int condition;
    int testStatus;
    int volConditions;
    int moveEffectsMask;
    int item;

    int substituteHp;
    bool substituteWasHit;

    int bHp;
    int atkStg;
    int defStg;
    int spAtkStg;
    int spDefStg;
    int spdStg;
    int accStg;
    int evaStg;
    int critStg;

    int turnsTaunted;
    int turnsEncored;
    bool isCharging; // currently charging a move like solarbeam

    int turnsProtected;
    bool isProtected;

    bool abilityKnownToAi;

    Move moveHit;
    Move movePrevByBattler;
    Move encoredMove;
};
class PokeInfo{
    public:
        std::string name;
        int hp;
        int atk;
        int def;
        int spDef;
        int spAtk;
        int spd;
        Type primaryType;
        Type secondaryType;
        AbilityId ability;
        AbilityId ability2;
        PokeInfo(std::string _name, int _hp, int _atk, int _def, int _spAtk, int _spDef, int _spd, Type _primaryType, Type _secondaryType, AbilityId _ability, AbilityId _ability2);
};
class Pokemon{
    public:
        int level;
        PokeInfo info;
        int cHp;
        int cAtk;
        int cDef;
        int cSpDef;
        int cSpAtk;
        int cSpd;
        AbilityId cAbility;


        int hpIv;
        int atkIv;
        int defIv;
        int spAtkIv;
        int spDefIv;
        int spdIv;

        int hpEv;
        int atkEv;
        int defEv;
        int spAtkEv;
        int spDefEv;
        int spdEv;

        Nature nature;
        BattleVal bVal;
        Move moveset[4];
        Pokemon();
        Pokemon(int _lvl, Nature _nature, int _ivs, Mons _id, Move _moveset[4], int _ability);
        void calcStats();
        void sendOut();
        void setEvs(int hp, int atk, int def, int spAtk, int spDef, int spd);
        void setBHp(int hp);
};

class Ability{
    public:
        AbilityId aid;
};

// fight setup
BattleContext setupJupiterFight(unsigned long startingSeed);
BattleContext setupVarFight(unsigned long startingSeed);
#endif /* POKEMON_H_ */