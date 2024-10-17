

#ifndef MOVES_H_
#define MOVES_H_
#include <vector>
#include <string>


#define RANGE_TARGET 0
#define RANGE_SELF 1

#define FLAG_MAKES_CONTACT 1<<0
#define FLAG_CAN_PROTECT 1<<1
#define FLAG_CONTACT_PROTECT (FLAG_MAKES_CONTACT | FLAG_CAN_PROTECT)

enum Type {
    Normal,
    Fighting,
    Flying,
    Poison,
    Ground,
    Rock,
    Bug,
    Ghost,
    Steel,
    Fire,
    Water,
    Grass,
    Electric,
    Psychic,
    Ice,
    Dragon,
    Dark,
    None
};


enum class DamageType:int {
    NONE,
    PHYSICAL,
    SPECIAL
};

enum class MoveId {
    NO_MOVE,
    STRUGGLE,
    RETURN,
    ROCK_THROW,
    HEADBUTT,
    GROWL,
    FLAME_WHEEL,
    NIGHT_SLASH,
    TAUNT,
    SMOKESCREEN,
    SCREECH,
    POISON_GAS,
    TACKLE,
    QUICK_ATTACK,
    AQUA_JET,
    WING_ATTACK,
    DOUBLE_TEAM,
    ENDEAVOR,
    SPARK,
    CHARM,
    CONFUSE_RAY,
    PSYBEAM,
    SHADOW_BALL,
    MAGICAL_LEAF,
    BITE,
    SUBSTITUTE,
    SAND_ATTACK,
    YAWN,
    TAKE_DOWN,
    TOXIC,
    SUPERSONIC,
    AIR_CUTTER,
    SURF,
    REVENGE,
    FAINT_ATTACK,
    MUD_BOMB,
    POISON_JAB,
    EARTHQUAKE,
    //
    CHARGE_BEAM,
    THUNDER_WAVE, // niy ai
    IRON_TAIL,
    SIGNAL_BEAM,
    FOCUS_BLAST,
    THUNDER_FANG,
    FIRE_FANG,
    CRUNCH,
    ICE_FANG,
    THUNDERPUNCH,
    FIRE_PUNCH,
    GIGA_IMPACT,

    PROTECT,
    TOXIC_SPIKES,
    SUCKER_PUNCH,

    BUG_BUZZ,
    AIR_SLASH,
    U_TURN,
    IRON_HEAD,
    X_SCISSOR,
    CLOSE_COMBAT,
    MEGAHORN,
    STONE_EDGE,
    ATTACK_ORDER,
    DEFEND_ORDER,
    HEAL_ORDER,
    POWER_GEM,
    CROSS_POISON,
    AERIAL_ACE,

    EARTH_POWER,
    SANDSTORM,
    ZEN_HEADBUTT,
    AQUA_TAIL,
    AVALANCHE,
    ROCK_WRECKER,

    FLAMETHROWER,
    SLUDGE_BOMB,
    DARK_PULSE,
    SUNNY_DAY,
    OVERHEAT,
    WILL_O_WISP,
    FLARE_BLITZ,
    SOLARBEAM,
    BOUNCE,
    MACH_PUNCH,
    THUNDERBOLT,
    HYPER_BEAM,

    LIGHT_SCREEN,
    REFLECT,
    CALM_MIND,
    GYRO_BALL,
    RECOVER,
    ENERGY_BALL,
    PSYCHO_CUT,
    DRAIN_PUNCH,
    LEAF_BLADE,

    SILVER_WIND,
    EXTRASENSORY,
    MIRROR_COAT,
    DRAGON_PULSE,
    ICE_BEAM,
    AURA_SPHERE,
    EXTREME_SPEED,
    WATER_PULSE,
    SHOCK_WAVE,
    DRAGON_RUSH,
    STEALTH_ROCK,
    ENCORE,
    PSYCHIC,
    TRICK_ROOM,
    HAIL,
    BATON_PASS,
    AQUA_RING,
    BRINE,
    WATERFALL,
    TWISTER
};
struct Move {
        MoveId id;
        std::string name;
        int flags;
        int power;
        int accuracy;
        bool secondary;
        int secondaryAccuracy;
        int effect;
        Type moveType;
        DamageType defend; // 0 physical, 1 special
        DamageType attack; // 0 physical, 1 special
        int priority;
        bool highCritRatio;
        bool disabled;
        int range;
        Move();
        Move(MoveId _id, std::string name, int flags, int _power, int _accuracy , bool _secondary, int _secondaryAccuracy, int _effect, Type _moveType, DamageType defend, DamageType attack, int priority, bool _highCritRatio, int range);
};
extern Move Empty;
extern Move Struggle;
extern Move Tackle;
extern Move Return;
extern Move Growl;
extern Move RockThrow;
extern Move Headbutt;
extern Move FlameWheel;
extern Move Taunt;
extern Move NightSlash;
extern Move PoisonGas;
extern Move Screech;
extern Move Smokescreen;
extern Move QuickAttack;
extern Move WingAttack;
extern Move DoubleTeam;
extern Move Endeavor;
extern Move Charm;
extern Move Spark;
extern Move ConfuseRay;
extern Move Psybeam;
extern Move ShadowBall;
extern Move MagicalLeaf;
extern Move Substitute;
extern Move Bite;
extern Move SandAttack;
extern Move Yawn;
extern Move TakeDown;
extern Move Toxic;
extern Move AirCutter;
extern Move Supersonic;
extern Move Surf;
extern Move Revenge;
extern Move PoisonJab;
extern Move FaintAttack;
extern Move MudBomb;
extern Move Earthquake;
//
extern Move ChargeBeam;
extern Move ThunderWave;
extern Move IronTail;
extern Move SignalBeam;
extern Move FocusBlast;
extern Move ThunderFang;
extern Move FireFang;
extern Move IceFang;
extern Move Crunch;
extern Move ThunderPunch;
extern Move FirePunch;
extern Move GigaImpact;

extern Move Protect;
extern Move ToxicSpikes;
extern Move SuckerPunch;

extern Move BugBuzz;
extern Move AirSlash;
extern Move UTurn;
extern Move IronHead;
extern Move XScissor;
extern Move CloseCombat;
extern Move Megahorn;
extern Move StoneEdge;
extern Move AttackOrder;
extern Move DefendOrder;
extern Move HealOrder;
extern Move PowerGem;
extern Move CrossPoison;
extern Move AerialAce;

extern Move EarthPower;
extern Move Sandstorm;
extern Move ZenHeadbutt;
extern Move AquaTail;
extern Move Avalanche;
extern Move RockWrecker;

extern Move Flamethrower;
extern Move SludgeBomb;
extern Move DarkPulse;
extern Move SunnyDay;
extern Move Overheat;
extern Move WillOWisp;
extern Move FlareBlitz;
extern Move Solarbeam;
extern Move Bounce;
extern Move MachPunch;
extern Move Thunderbolt;
extern Move HyperBeam;

extern Move LightScreen;
extern Move Reflect;
extern Move CalmMind;
extern Move GyroBall;
extern Move Recover;
extern Move EnergyBall;
extern Move PsychoCut;
extern Move DrainPunch;
extern Move LeafBlade;

extern Move SilverWind;
extern Move Extrasensory;
extern Move MirrorCoat;
extern Move DragonPulse;
extern Move IceBeam;
extern Move AuraSphere;
extern Move Extremespeed;
extern Move WaterPulse;
extern Move ShockWave;
extern Move DragonRush;

extern Move StealthRock;
extern Move Encore;
extern Move PsychicMove;
extern Move TrickRoom;
extern Move Hail;
extern Move BatonPass;
extern Move AquaRing;
extern Move AquaJet;
extern Move Brine;
extern Move Waterfall;
extern Move Twister;
#endif /* MOVES_H_ */