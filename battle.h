#ifndef BATTLE_H_
#define BATTLE_H_

#include "pokemon.h"
#include "config.h"
#define MAX_CLIENTS 4
#define COMMAND_TERMINATE 19875 // cease the battle early
#define COMMAND_MOVE_SLOT_1 1 << 0
#define COMMAND_MOVE_SLOT_2 1 << 1
#define COMMAND_MOVE_SLOT_3 1 << 2
#define COMMAND_MOVE_SLOT_4 1 << 3
#define COMMAND_MOVE    ( COMMAND_MOVE_SLOT_1 \
                        | COMMAND_MOVE_SLOT_2 \
                        | COMMAND_MOVE_SLOT_3 \
                        | COMMAND_MOVE_SLOT_4)
#define COMMAND_SWITCH_OFFSET 4
#define POKE_SLOT_1 1 << 4
#define POKE_SLOT_2 1 << 5
#define POKE_SLOT_3 1 << 6
#define POKE_SLOT_4 1 << 7
#define POKE_SLOT_5 1 << 8
#define POKE_SLOT_6 1 << 9

#define POKE_SLOTS ( POKE_SLOT_1 \
    | POKE_SLOT_2 \
    | POKE_SLOT_3 \
    | POKE_SLOT_4 \
    | POKE_SLOT_5 \
    | POKE_SLOT_6 )

#define COMMAND_USE_ITEM_SUPER_POTION 1 << 10
#define COMMAND_USE_ITEM_GUARD_SPEC 1 << 11
#define COMMAND_USE_ITEM_HYPER_POTION 1 << 12
#define COMMAND_USE_ITEM_PRLZ_HEAL 1 << 13
#define COMMAND_USE_ITEM_FULL_HEAL 1 << 14
#define COMMAND_USE_ITEM_X_ACCURACY 1 << 15
#define COMMAND_USE_ITEM_X_SPEED 1 << 16
#define COMMAND_USE_ITEM_ANTIDOTE 1 << 17
#define COMMAND_USE_ITEM_FULL_RESTORE 1 << 18
#define COMMAND_USE_ITEM_POTION 1 << 23
#define COMMAND_USE_ITEM_X_ATK 1 << 24
#define COMMAND_USE_ITEM ( COMMAND_USE_ITEM_SUPER_POTION \
                         | COMMAND_USE_ITEM_GUARD_SPEC \
                         | COMMAND_USE_ITEM_HYPER_POTION \
                         | COMMAND_USE_ITEM_PRLZ_HEAL \
                         | COMMAND_USE_ITEM_FULL_HEAL \
                         | COMMAND_USE_ITEM_X_ACCURACY \
                         | COMMAND_USE_ITEM_X_SPEED \
                         | COMMAND_USE_ITEM_ANTIDOTE \
                         | COMMAND_USE_ITEM_FULL_RESTORE\
                         | COMMAND_USE_ITEM_POTION \
                         | COMMAND_USE_ITEM_X_ATK)
#define COMMAND_SWITCH 1 << 19


#define COMMAND_BRANCH_INC 1 << 20
#define COMMAND_BRANCH_DEC 1 << 21
#define COMMAND_BRANCH_IF_KO 1 << 22

// option selects
#define COMMAND_USE_GATEAU_OR_BITE 1 << 27
#define COMMAND_USE_HYPER_OR_BITE 1 << 28

#define TRIGGER_INTIMIDATE 1<<0
#define TRIGGER_SAND_STREAM 1<<1
#define TRIGGER_TOXIC_SPIKES 1<<2
#define TRIGGER_STEALTH_ROCK 1<<3


#define MOVE_STATUS_MISSED              (1 << 0)
#define MOVE_STATUS_SUPER_EFFECTIVE     (1 << 1)
#define MOVE_STATUS_NOT_VERY_EFFECTIVE  (1 << 2)
#define MOVE_STATUS_IMMUNE         (1 << 3)
#define MOVE_STATUS_LEVITATED           (1 << 11)
#define MOVE_STATUS_ABSORBED        (1<<12) // off code
unsigned short advanceSeed(BattleContext *bc, std::string blurb = "none");
bool useMove(Move move, BattleContext *bc);
int calcDamage(BattleContext *bc, Move move, int crit = 1, int randomRoll = 0, bool hurtSelf = false, bool defenderAttacking = false);
int dealDamage(Pokemon *p,  int damage, bool directSource = true);
bool determineOrder(BattleContext *bc);
bool doTurn(BattleContext *bc);
bool checkStatusDisruption(BattleContext *bc, Move move);
void updateMoveBuffers(BattleContext *bc, Move move);
void updateFlagsWhenHit(BattleContext *bc, Move move);
void heal(Pokemon *p, int amount);
int calcTypeMultiplier(Type attack, Type defend1, Type defend2);
int getSwitchNum(unsigned long command);
bool activateTriggers(BattleContext *bc, bool doAttacker);

enum class FieldCondition {
    NONE,
    MIST,
    SAFEGUARD,

};
enum class Weather {
    None,
    Rain,
    Sunny,
    Hail,
    Sandstorm
};
struct RngSeed {
    unsigned long seed;
    unsigned short result;
};
struct SideCondition {
    int remainingTurns;
    FieldCondition condition;
};
struct BattleSide {
    std::vector<SideCondition> sideConditions;
};
struct PokeClient {
    std::string name;
    int sideConditions;
    // specific effect turn counters
    int safeGuardTurns;
    int mistTurns;
    int toxicSpikeLayers;
    bool aiControl;
    int aiJumpNum;
    bool isWinner;
    bool isSwitching; // this client is switching out a new pokemon
    bool successfulMove; // this client made a successful move this turn
    int aiLevel;
    Move previouslySelectedMove;
    int battler; // the index of the current battler in the team list
    Pokemon team[6]; // the team list
    int useItems[4]; // items the AI can use
    int numUseItems;
    unsigned long command; // which command we're using this turn
    PokeClient(void);
    void pokeSwitch(int nextBattler);
    // triggers
    int triggers; // things that need to be triggered at the start of the turn (like intimidate)

};
struct BattleContext {
    RngSeed battleRng;
    PokeClient attacker;
    PokeClient defender;
    PokeClient tempHolder; // useful when we want to attack ourselves and need to hold the actual defender client somewhere 
    int weather;
    int weatherTurns;
    bool moveWasSuccessful; // only has to be used (get past premove), doesn't have to hit
    int cDmg; // variable flat damage
    int cPwr; // variable power 
    int realDmg; // damage dealt by most recent move, used by moves that cause recoil or counter
    int previousDmg; // damage dealt by the previously used move
    int turnNumber;
    int speedTieBreakers[4];
    bool terminate;
    int branch; // what branch we are on.  Whenever divergence occurs (using a potion that doesn't heal, trying to cure status that doesn't exist) the branch increases by 1
    unsigned long switchCommands[6]; // who to switch to when an opponent faints
    int switchCommandIndex;
};
struct Fraction {
    int numerator;
    int denominator;
};
static const Fraction HitRateByStage[] = {
    {  33, 100 },
    {  36, 100 },
    {  43, 100 },
    {  50, 100 },
    {  60, 100 },
    {  75, 100 },
    {   1,   1 },
    { 133, 100 },
    { 166, 100 },
    {   2,   1 },
    { 233, 100 },
    { 133,  50 },
    {   3,   1 }
};
static const Fraction StatBonusByStage[] = {
    {2, 8},//0
    {2, 7},//1
    {2, 6},//2
    {2, 5},//3
    {2, 4},//4
    {2, 3},//5
    {2, 2},//6
    {3, 2},//7
    {4, 2},//8
    {5, 2},//9
    {6, 2},//10
    {7, 2},//11
    {8, 2},//12
};

int calcSpeed(PokeClient *pc);
int getTypeMultiplier(PokeClient *attacker, PokeClient *defender, Move move, int damage, int *moveStatus);

#endif /* BATTLE_H_ */