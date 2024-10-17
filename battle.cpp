#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <vector>
#include "battle.h"
#include "battle_effects.h"
#include "tr_ai.h"
void log(std::string logText) {
    if(DEBUG) {
        std::cout << logText << std::endl;
    }
}
std::string logName(Pokemon p, int dmg = 0) {
    std::string hpString = "";
    if(dmg == 0) {
        hpString = " (" + std::to_string(p.bVal.bHp) + "/" + std::to_string(p.cHp) + ")";
    } else {
        int newVal = p.bVal.bHp - dmg;
        if(newVal < 0) {
            newVal = 0;
        }
        hpString = " (" + std::to_string(p.bVal.bHp) + "->" + std::to_string(newVal) + ")";
    }
    return p.info.name + hpString;
}
void log(int logText) {
    if(DEBUG) {
        std::cout << logText << std::endl;
    }
}
unsigned short advanceSeed(BattleContext *bc, std::string blurb){

    unsigned long seed = (1103515245 * bc -> battleRng.seed + 24691) & 0xFFFFFFFF;
    unsigned short result = seed / 65536;
    bc -> battleRng = {seed, result};
    if(DEBUG & 4) {
        if(blurb == "none") {
            std::cout << seed << std::endl;
        } else {
            std::cout << std::to_string(seed) << " -> " << blurb << std::endl;
        }
    }
    return result;
};
int critStages[6] = {16,8,4,3,2,2};


std::map<Type, std::map<Type, float>> typeMap = {
    {Normal,     {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 1.0}, {Poison, 1.0}, {Ground, 1.0}, {Rock, 0.5}, {Bug, 1.0}, {Ghost, 0.0}, {Steel, 0.5}, {Fire, 1.0}, {Water, 1.0}, {Grass, 1.0}, {Electric, 1.0}, {Psychic, 1.0}, {Ice, 1.0}, {Dragon, 1.0}, {Dark, 1.0}, {None, 1.0}}},
    {Fighting,   {{Normal, 2.0}, {Fighting, 1.0}, {Flying, 0.5}, {Poison, 0.5}, {Ground, 1.0}, {Rock, 2.0}, {Bug, 0.5}, {Ghost, 0.0}, {Steel, 2.0}, {Fire, 1.0}, {Water, 1.0}, {Grass, 1.0}, {Electric, 1.0}, {Psychic, 0.5}, {Ice, 2.0}, {Dragon, 1.0}, {Dark, 2.0}, {None, 1.0}}},
    {Flying,     {{Normal, 1.0}, {Fighting, 2.0}, {Flying, 1.0}, {Poison, 1.0}, {Ground, 1.0}, {Rock, 0.5}, {Bug, 2.0}, {Ghost, 1.0}, {Steel, 0.5}, {Fire, 1.0}, {Water, 1.0}, {Grass, 2.0}, {Electric, 0.5}, {Psychic, 1.0}, {Ice, 1.0}, {Dragon, 1.0}, {Dark, 1.0}, {None, 1.0}}},
    {Poison,     {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 1.0}, {Poison, 0.5}, {Ground, 0.5}, {Rock, 0.5}, {Bug, 1.0}, {Ghost, 0.5}, {Steel, 0.0}, {Fire, 1.0}, {Water, 1.0}, {Grass, 2.0}, {Electric, 1.0}, {Psychic, 1.0}, {Ice, 1.0}, {Dragon, 1.0}, {Dark, 1.0}, {None, 1.0}}},
    {Ground,     {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 0.0}, {Poison, 2.0}, {Ground, 1.0}, {Rock, 2.0}, {Bug, 0.5}, {Ghost, 1.0}, {Steel, 2.0}, {Fire, 2.0}, {Water, 1.0}, {Grass, 0.5}, {Electric, 2.0}, {Psychic, 1.0}, {Ice, 1.0}, {Dragon, 1.0}, {Dark, 1.0}, {None, 1.0}}},
    {Rock,       {{Normal, 1.0}, {Fighting, 0.5}, {Flying, 2.0}, {Poison, 1.0}, {Ground, 0.5}, {Rock, 1.0}, {Bug, 2.0}, {Ghost, 1.0}, {Steel, 0.5}, {Fire, 2.0}, {Water, 1.0}, {Grass, 1.0}, {Electric, 1.0}, {Psychic, 1.0}, {Ice, 2.0}, {Dragon, 1.0}, {Dark, 1.0}, {None, 1.0}}},
    {Bug,        {{Normal, 1.0}, {Fighting, 0.5}, {Flying, 0.5}, {Poison, 0.5}, {Ground, 1.0}, {Rock, 1.0}, {Bug, 1.0}, {Ghost, 0.5}, {Steel, 0.5}, {Fire, 0.5}, {Water, 1.0}, {Grass, 2.0}, {Electric, 1.0}, {Psychic, 2.0}, {Ice, 1.0}, {Dragon, 1.0}, {Dark, 2.0}, {None, 1.0}}},
    {Ghost,      {{Normal, 0.0}, {Fighting, 1.0}, {Flying, 1.0}, {Poison, 1.0}, {Ground, 1.0}, {Rock, 1.0}, {Bug, 1.0}, {Ghost, 2.0}, {Steel, 0.5}, {Fire, 1.0}, {Water, 1.0}, {Grass, 1.0}, {Electric, 1.0}, {Psychic, 2.0}, {Ice, 1.0}, {Dragon, 1.0}, {Dark, 0.5}, {None, 1.0}}},
    {Steel,      {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 1.0}, {Poison, 1.0}, {Ground, 1.0}, {Rock, 2.0}, {Bug, 1.0}, {Ghost, 1.0}, {Steel, 0.5}, {Fire, 0.5}, {Water, 0.5}, {Grass, 1.0}, {Electric, 0.5}, {Psychic, 1.0}, {Ice, 2.0}, {Dragon, 1.0}, {Dark, 1.0}, {None, 1.0}}},
    {Fire,       {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 1.0}, {Poison, 1.0}, {Ground, 1.0}, {Rock, 0.5}, {Bug, 2.0}, {Ghost, 1.0}, {Steel, 2.0}, {Fire, 0.5}, {Water, 0.5}, {Grass, 2.0}, {Electric, 1.0}, {Psychic, 1.0}, {Ice, 2.0}, {Dragon, 0.5}, {Dark, 1.0}, {None, 1.0}}},
    {Water,      {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 1.0}, {Poison, 1.0}, {Ground, 2.0}, {Rock, 2.0}, {Bug, 1.0}, {Ghost, 1.0}, {Steel, 1.0}, {Fire, 2.0}, {Water, 0.5}, {Grass, 0.5}, {Electric, 1.0}, {Psychic, 1.0}, {Ice, 1.0}, {Dragon, 0.5}, {Dark, 1.0}, {None, 1.0}}},
    {Grass,      {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 0.5}, {Poison, 0.5}, {Ground, 2.0}, {Rock, 2.0}, {Bug, 0.5}, {Ghost, 1.0}, {Steel, 0.5}, {Fire, 0.5}, {Water, 2.0}, {Grass, 0.5}, {Electric, 1.0}, {Psychic, 1.0}, {Ice, 1.0}, {Dragon, 0.5}, {Dark, 1.0}, {None, 1.0}}},
    {Electric,   {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 2.0}, {Poison, 1.0}, {Ground, 0.0}, {Rock, 1.0}, {Bug, 1.0}, {Ghost, 1.0}, {Steel, 1.0}, {Fire, 1.0}, {Water, 2.0}, {Grass, 0.5}, {Electric, 0.5}, {Psychic, 1.0}, {Ice, 1.0}, {Dragon, 0.5}, {Dark, 1.0}, {None, 1.0}}},
    {Psychic,    {{Normal, 1.0}, {Fighting, 2.0}, {Flying, 1.0}, {Poison, 2.0}, {Ground, 1.0}, {Rock, 1.0}, {Bug, 1.0}, {Ghost, 1.0}, {Steel, 0.5}, {Fire, 1.0}, {Water, 1.0}, {Grass, 1.0}, {Electric, 1.0}, {Psychic, 0.5}, {Ice, 1.0}, {Dragon, 1.0}, {Dark, 0.0}, {None, 1.0}}},
    {Ice,        {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 2.0}, {Poison, 1.0}, {Ground, 2.0}, {Rock, 1.0}, {Bug, 1.0}, {Ghost, 1.0}, {Steel, 0.5}, {Fire, 0.5}, {Water, 0.5}, {Grass, 2.0}, {Electric, 1.0}, {Psychic, 1.0}, {Ice, 0.5}, {Dragon, 2.0}, {Dark, 1.0}, {None, 1.0}}},
    {Dragon,     {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 1.0}, {Poison, 1.0}, {Ground, 1.0}, {Rock, 1.0}, {Bug, 1.0}, {Ghost, 1.0}, {Steel, 0.5}, {Fire, 1.0}, {Water, 1.0}, {Grass, 1.0}, {Electric, 1.0}, {Psychic, 1.0}, {Ice, 1.0}, {Dragon, 2.0}, {Dark, 1.0}, {None, 1.0}}},
    {Dark,       {{Normal, 1.0}, {Fighting, 0.5}, {Flying, 1.0}, {Poison, 1.0}, {Ground, 1.0}, {Rock, 1.0}, {Bug, 1.0}, {Ghost, 2.0}, {Steel, 0.5}, {Fire, 1.0}, {Water, 1.0}, {Grass, 1.0}, {Electric, 1.0}, {Psychic, 2.0}, {Ice, 1.0}, {Dragon, 1.0}, {Dark, 0.5}, {None, 1.0}}},
    {None,       {{Normal, 1.0}, {Fighting, 1.0}, {Flying, 1.0}, {Poison, 1.0}, {Ground, 1.0}, {Rock, 1.0}, {Bug, 1.0}, {Ghost, 1.0}, {Steel, 1.0}, {Fire, 1.0}, {Water, 1.0}, {Grass, 1.0}, {Electric, 1.0}, {Psychic, 1.0}, {Ice, 1.0}, {Dragon, 1.0}, {Dark, 1.0}, {None, 1.0}}},

};

PokeClient::PokeClient(void): previouslySelectedMove() {
    sideConditions = 0;
    safeGuardTurns = 0;
    mistTurns = 0;
    toxicSpikeLayers = 0;
    isWinner = false;
    triggers = 0;
    aiJumpNum = 2;
    useItems[0] = 0;
    useItems[1] = 0;
    useItems[2] = 0;
    useItems[3] = 0;
}

void PokeClient::pokeSwitch(int nextBattler) {
    battler = nextBattler;
    Pokemon& p = team[battler];
    p.sendOut(); // refresh
    log("Sent out " + p.info.name);
    if(p.cAbility == AbilityId::INTIMIDATE) {
        triggers |= TRIGGER_INTIMIDATE;
    }
    if(p.cAbility == AbilityId::SAND_STREAM) {
        triggers |= TRIGGER_SAND_STREAM;
    }
    if(sideConditions & SIDE_CONDITION_TOXIC_SPIKES) {
        triggers |= TRIGGER_TOXIC_SPIKES;
    }
    if(sideConditions & SIDE_CONDITION_STEALTH_ROCK) {
        triggers |= TRIGGER_STEALTH_ROCK;
    }
    previouslySelectedMove = Empty;
}
int modifyStatValue(int base, int stage){
    int interm = base * StatBonusByStage[stage].numerator;
    return interm / StatBonusByStage[stage].denominator;
}

int calcSpeed(PokeClient *pc) {
    int speed = modifyStatValue(pc->team[pc->battler].cSpd, pc->team[pc->battler].bVal.spdStg);
    if(pc->team[pc->battler].bVal.condition & MON_CONDITION_PARALYSIS) {
        if(4 & DEBUG) {
            std::cout << "Reducing speed due to paralysis" << std::endl;
        }
        speed = speed/4; // paralysis reduction occurs after staged increases
    }
    return speed;
}
int getPriorityScore(PokeClient *pc) {
    int command1 = pc->command;
    int priorityScore = 0;
    if(command1 & COMMAND_MOVE) {
        // moves have lowest priority
        priorityScore += 6;
        int moveNum = log2(command1 & COMMAND_MOVE);
        priorityScore += pc->team[pc->battler].moveset[moveNum].priority;
    } else if(command1 & COMMAND_USE_ITEM) {
        priorityScore += 13; // higher than any move 
    } else if(command1 & COMMAND_SWITCH) {
        priorityScore += 16; // highest priority score for switching mons
    }
    return priorityScore;
}
//BattleSystem_CompareBattlerSpeed
bool determineOrder(BattleContext *bc) {
    bool attackerGoesFirst = true;
    int priorityScore1 = getPriorityScore(&bc->attacker);
    int priorityScore2 = getPriorityScore(&bc->defender);
    int speed1 = calcSpeed(&bc->attacker);
    int speed2 = calcSpeed(&bc->defender);
    if(priorityScore1 == priorityScore2) {
        // same priority bracket, resort to speed calc
        if(speed1 > speed2) {
            attackerGoesFirst = true;
        } else {
            if(speed1 == speed2) {
                attackerGoesFirst = advanceSeed(bc) % 2 == 0;
            } else {
                attackerGoesFirst = false; //TODO - Handle speed ties

            }
        }
        if(bc->weather & FIELD_CONDITION_TRICK_ROOM) {
            attackerGoesFirst = !attackerGoesFirst; // reverso
        }
    } else if(priorityScore1  > priorityScore2) {
        attackerGoesFirst = true;
    } else {
        attackerGoesFirst = false;
    }
    return attackerGoesFirst;
}
int getCritStage(Move move, BattleContext *bc){
    int stage = bc->attacker.team[bc->attacker.battler].bVal.critStg;
    if(move.highCritRatio) {
        stage++;
    }
    if(stage > 4) {
        stage = 4;
    }
    // std::cout << "\tcrit stage: " << stage << std::endl;
    return critStages[stage];
}
int getTypeMultiplier(PokeClient *attacker, PokeClient *defender, Move move, int damage, int *moveStatus) {
    if(!moveStatus) {
        moveStatus = 0;
    }
    int mul = 0;
    if(defender->team[defender->battler].cAbility == AbilityId::LEVITATE && move.moveType == Type::Ground) {
        *moveStatus |= MOVE_STATUS_LEVITATED;
        *moveStatus |= MOVE_STATUS_IMMUNE;
    } else if(defender->team[defender->battler].cAbility == AbilityId::WATER_ABSORB && move.moveType == Type::Water) {
        *moveStatus |= MOVE_STATUS_IMMUNE;
        *moveStatus |= MOVE_STATUS_ABSORBED;
    } else {
        int type1Bonus = typeMap[move.moveType][defender->team[defender->battler].info.primaryType] * 10;
        int type2Bonus = typeMap[move.moveType][defender->team[defender->battler].info.secondaryType] * 10;
        mul = type1Bonus * type2Bonus;
        if(mul >= 200) {
            *moveStatus |= MOVE_STATUS_SUPER_EFFECTIVE;
        } else if(mul < 100 && mul > 0) {
            *moveStatus |= MOVE_STATUS_NOT_VERY_EFFECTIVE;
        }
    }

    if(mul == 0) {
        *moveStatus |= MOVE_STATUS_IMMUNE;
    }
    int stabBonus = 100;
    if(move.moveType != Type::None) {
        stabBonus = (attacker->team[attacker->battler].info.primaryType == move.moveType || attacker->team[attacker->battler].info.secondaryType == move.moveType) ? 150 : 100;
    }


    damage = damage * stabBonus / 100;
    damage = (damage * mul) / 100;

    return damage;
}
// calculates the type multiplier, as applied to a 40 damage attack
// ie. will return 10 if 1/4 effective, 20 if 1/2, 40 if 1, and so on
int calcTypeMultiplier(Type attack, Type defend1, Type defend2) {
    int score = 0;
    return typeMap[attack][defend1] * typeMap[attack][defend2] * NORMAL_EFFECTIVE;
}
int calcDamage(BattleContext *bc, Move move, int crit, int randomRoll, bool hurtSelf, bool defenderAttacking) {
    PokeClient attacker;
    PokeClient defender;
    if(hurtSelf) {
        attacker = bc->attacker;
        defender = bc->attacker;
    } else {
        if(defenderAttacking) {
            attacker = bc->defender;
            defender = bc->attacker;
        } else {
            attacker = bc->attacker;
            defender = bc->defender;
        }

    }
    if(defender.team[defender.battler].cAbility == BATTLE_ARMOR) {
        crit = 1; // no crits against battle armor 
    }
    int lvlBase = ((2 * attacker.team[attacker.battler].level) / 5);
    lvlBase = lvlBase + 2;

    int cAtkStat = 0;
    int cDefStat = 0;
    if( move.attack ==  DamageType::PHYSICAL) {
        cAtkStat = attacker.team[attacker.battler].cAtk;
        if(attacker.team[attacker.battler].bVal.atkStg < 6 && crit>1){
            // we crit, so ignore lowered atk stage
            cAtkStat = modifyStatValue(cAtkStat, 6);
        } else {
            cAtkStat = modifyStatValue(cAtkStat, attacker.team[attacker.battler].bVal.atkStg);
        }
    } else {
        cAtkStat = attacker.team[attacker.battler].cSpAtk;
        if(attacker.team[attacker.battler].bVal.spAtkStg < 6 && crit>1){
            // we crit, so ignore lowered atk stage
            cAtkStat = modifyStatValue(cAtkStat, 6);
        } else {
            cAtkStat = modifyStatValue(cAtkStat, attacker.team[attacker.battler].bVal.spAtkStg);
        }
    }
    if( move.defend ==  DamageType::PHYSICAL) {
        cDefStat = defender.team[defender.battler].cDef;
        if(defender.team[defender.battler].bVal.defStg > 6 && crit > 1) {
            // ignore higher defense on crit
            cDefStat = modifyStatValue(cDefStat, 6);
        } else {
            cDefStat = modifyStatValue(cDefStat, defender.team[defender.battler].bVal.defStg);
        }
    } else {
        cDefStat = defender.team[defender.battler].cSpDef;
        if(defender.team[defender.battler].bVal.spDefStg > 6 && crit > 1) {
            // ignore higher defense on crit
            cDefStat = modifyStatValue(cDefStat, 6);
        } else {
            cDefStat = modifyStatValue(cDefStat, defender.team[defender.battler].bVal.spDefStg);
        }
    }
    int movePower = move.power;
    if(movePower == 1 || movePower == 3) {
        bc->cDmg = 0;
        bc->cPwr = 0;
        // move has variable (or flat) power
        // endeavor, gyro ball, crush grip, dragon rage, etc.
        if(applyEffect(move, bc)){
            // success
            if(bc->cDmg > 0) {
                // current damage was set, move deals flat damage (no crits)
                return bc->cDmg;
            } else {
                // if cDmg wasn't set, power must be. Set move power to the variable power result 
                movePower = bc->cPwr;
            }
        } else {
            return 0; // move failed, return 0 for damage 
        }
    }
    if(attacker.team[attacker.battler].bVal.item == ITEM_BOOST_DARK && move.moveType == Type::Dark) {
        // black sunglasses, dread plate
        movePower = movePower * 120 / 100; // 20% damage boosto
    }
    if(attacker.team[attacker.battler].bVal.item == ITEM_BOOST_ELECTRIC && move.moveType == Type::Electric) {
        // magnet, zap plate
        movePower = movePower * 120 / 100; // 20% damage boosto
    }
    if(attacker.team[attacker.battler].bVal.item == ITEM_BOOST_GROUND && move.moveType == Type::Ground) {
        // magnet, zap plate
        movePower = movePower * 120 / 100; // 20% damage boosto
    }
    if(attacker.team[attacker.battler].bVal.item == ITEM_BOOST_WATER && move.moveType == Type::Water) {
        // wave incense, splash plate, mystic water
        movePower = movePower * 120 / 100; // 20% damage boosto
    }
    if(move.moveType == Type::Fire && attacker.team[attacker.battler].cAbility == AbilityId::BLAZE && attacker.team[attacker.battler].bVal.bHp <= attacker.team[attacker.battler].cHp/3){
        movePower = movePower * 150 / 100;
    }

    if(attacker.team[attacker.battler].cAbility == AbilityId::RIVALRY) {
        // TODO: Should check the gender here. For our purposes, it doesn't matter, Rivalry will always be positive bonus for our opponents and we don't have access to it
        movePower = movePower * 125 / 100;
    }
    int powerBase = (lvlBase * movePower * (cAtkStat));
    int defDivide = powerBase / cDefStat;
    int powerResult = (defDivide/50);
    if((attacker.team[attacker.battler].bVal.condition & MON_CONDITION_BURN) && move.attack == DamageType::PHYSICAL) {
        powerResult = powerResult/2;
    }
    if(move.moveType == Type::Fire && bc->weather & FIELD_CONDITION_SUNNY) {
        powerResult = powerResult * 150 / 100;
    }
    powerResult = powerResult + 2;
    powerResult = powerResult * crit; // should be 2 if we critted :A
    powerResult = powerResult * (100 - randomRoll) / 100;
    int moveStatus = 0;
    powerResult = getTypeMultiplier(&attacker, &defender, move, powerResult, &moveStatus);
    return powerResult;
};
bool rollAccuracy(Move move, BattleContext *bc) {
    int accuracy = move.accuracy;
    if(accuracy == 0){
        return true; // bypass accuracy check
    }
    int accStages = bc->attacker.team[bc->attacker.battler].bVal.accStg - 6;
    int evaStages = 6 - bc->defender.team[bc->defender.battler].bVal.evaStg;
    int sum = 6 + evaStages + accStages;
    if (sum < 0) {
        sum = 0;
    } else if (sum > 12) {
        sum = 12;
    }
    accuracy *= HitRateByStage[sum].numerator;
    accuracy /= HitRateByStage[sum].denominator;
    // add evasion and accuracy checks
    log("\trolling accuracy");
    return (advanceSeed(bc) % 100) < accuracy;
}
void heal(Pokemon *p, int amount) {
    p->bVal.bHp = p->bVal.bHp + amount;
    if(p->bVal.bHp > p->cHp){
        p->bVal.bHp = p->cHp;
    }
}
// returns the real damage done to the pokemon, taking into account
// the fact that damage stops being dealt if a pokemon faints or after 
// a substitute fades, etc.
int dealDamage(Pokemon *p,  int damage, bool directSource) {
    int realDamageDealt = 0;
    if(directSource && p->bVal.substituteHp > 0) {
        p->bVal.substituteHp = p->bVal.substituteHp - damage;
        if(p->bVal.substituteHp <= 0) {
            realDamageDealt = p->bVal.substituteHp;
            p->bVal.substituteHp = 0;
        } else {
            realDamageDealt = damage;
        }
        p->bVal.substituteWasHit = true;
    } else {
        realDamageDealt = damage;
        p->bVal.bHp = p->bVal.bHp - damage;
    }


    if(p->bVal.bHp < 0) {
        realDamageDealt += p->bVal.bHp; // add the negative hp back to the damage dealt
        p->bVal.bHp = 0;
    }
    if((p->bVal.bHp <= (p->cHp / 2)) && p->bVal.bHp > 0) {
        // trigger effects that occur below half health
        if(p->bVal.item == ITEM_ORAN_BERRY) {
            int oldHp = p->bVal.bHp;
            heal(p, 10);
            p->bVal.item = ITEM_NONE; // consumed
            if(DEBUG) {
                std::cout << "\t\tOran berry activates: " << p->info.name << " heals " <<  oldHp << "->" << p->bVal.bHp << std::endl;
            }
        } else if(p->bVal.item == ITEM_SITRUS_BERRY) {
            int oldHp = p->bVal.bHp;
            heal(p, p->cHp/4);
            p->bVal.item = ITEM_NONE; // consumed
            if(DEBUG) {
                std::cout << "\t\tSitrus berry activates: " << p->info.name << " heals " <<  oldHp << "->" << p->bVal.bHp << std::endl;
            }
        } else if(p->bVal.item == ITEM_NATURE_BERRY) {
            int oldHp = p->bVal.bHp;
            heal(p, p->cHp/8);
            p->bVal.item = ITEM_NONE; // consumed
            if(DEBUG){
                std::cout << "\t\tNature berry activates: " << p->info.name << " heals " <<  oldHp << "->" << p->bVal.bHp << std::endl;
            }
        }
    }
    return realDamageDealt;
}
bool preMove(Move move, BattleContext *bc) {
    bc->realDmg = 0;
    bool statusDisrupted = checkStatusDisruption(bc, move); // paralysis, sleep, flinch
    if(statusDisrupted) {
        return false;
    }

    if(bc->attacker.team[bc->attacker.battler].bVal.bHp == 0) {
        return false; // im dead
    }
    if(bc->defender.team[bc->defender.battler].bVal.bHp == 0 && move.range == RANGE_TARGET) {
        return false; // no valid target
    }
    bc->moveWasSuccessful = true;
    if(move.effect == BATTLE_EFFECT_SKIP_CHARGE_TURN_IN_SUN) {
        if(bc->weather & FIELD_CONDITION_SUNNY) {
            // let solarbeam complete
            bc->attacker.team[bc->attacker.battler].bVal.isCharging = false;
        } else {
            if(bc->attacker.team[bc->attacker.battler].bVal.isCharging) {
                // we're on charging turn, let solar beam complete
                bc->attacker.team[bc->attacker.battler].bVal.isCharging = false;
            } else {
                bc->attacker.team[bc->attacker.battler].bVal.isCharging = true;
            }
        }
    }
    return true;
}
bool postMove(Move move, BattleContext *bc){
    return true;
}

bool accuracyOverride(Move move, BattleContext *bc, int *movestatus) {
    if(bc->defender.team[bc->defender.battler].bVal.isProtected && (move.flags & FLAG_CAN_PROTECT)) {
        *movestatus |= MOVE_STATUS_MISSED;
        log("\tProtected");
        return true;
    }
    if(move.effect == BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING) {
        if(bc->defender.command & COMMAND_MOVE) {
            int moveCommand = log2(bc->defender.command & COMMAND_MOVE);
            if(bc->defender.team[bc->defender.battler].moveset[moveCommand].power == 0){
                *movestatus |= MOVE_STATUS_MISSED;
                return true;
            }
        }
    }
    return false;
}
// return true if move was successful, false if not
bool useMove(Move move, BattleContext *bc) {
    if(DEBUG) {
        std::cout << "Battler " << logName(bc->attacker.team[bc->attacker.battler]) << " uses " << move.name << std::endl;
    }
    bool success = false;
    if(preMove(move, bc) && !bc->attacker.team[bc->attacker.battler].bVal.isCharging){

        int crit = 15;
        int dmgRoll = 15;
        int dmg = 0;
        if(move.power < 2){
            // non-attacking move
            // or move with variable flat damage 
        } else {
            log("\trolling crit");
            crit = advanceSeed(bc) % getCritStage(move, bc);
            crit = crit == 0;
            crit = crit + 1;
            log("\trolling damage");
            dmgRoll = advanceSeed(bc) % 16;
        }
        bool accRoll = rollAccuracy(move, bc);
        int moveStatus = 0;
        bool overridden = accuracyOverride(move, bc, &moveStatus);
        if(accRoll && !overridden) {
            // move hits
            if(move.range == RANGE_TARGET && move.power > 0 || move.id == MoveId::THUNDER_WAVE) {
                // for whatever reason thunder wave specifically is affected by type matchups
                // type matchups are calculated after accuracy is rolled... apparently?

                int dmg = getTypeMultiplier(&bc->attacker, &bc->defender, move, 40, &moveStatus); // mock a multiplier to see if we're immune
                if(moveStatus & MOVE_STATUS_IMMUNE) {
                    if(moveStatus & MOVE_STATUS_ABSORBED) {
                        if(bc->defender.team[bc->defender.battler].cAbility == AbilityId::WATER_ABSORB) {
                            // we absorbed a water move
                            heal(&bc->defender.team[bc->defender.battler], bc->defender.team[bc->defender.battler].cHp / 4);
                        }
                        success = true; // absorbtion by ability like water absorb renders as a success in the game for the purpose of triggering success RNG
                    } else {
                        success = false;
                    }
                }
            }
            if(!(moveStatus & MOVE_STATUS_IMMUNE)) {
                if(move.power == 0) {
                    // non-attacking move handler
                } else {
                    dmg = calcDamage(bc, move, crit, dmgRoll);
                    if(dmg > 0){
                        success = true; // dmg move successfully executed
                        if(DEBUG) {
                            if(crit > 1) {
                                std::cout << "\tCrit" << std::endl;
                            }
                            std::cout << "\tDamage: " << dmg << std::endl;
                        }

                        bc->realDmg = dealDamage(&bc->defender.team[bc->defender.battler], dmg);
                        if(move.flags & FLAG_MAKES_CONTACT) {
                            if(bc->defender.team[bc->defender.battler].bVal.item == ITEM_STICKY_BARB) {
                                if(bc->attacker.team[bc->attacker.battler].bVal.item == 0) {
                                    bc->attacker.team[bc->attacker.battler].bVal.item = ITEM_STICKY_BARB;
                                    bc->defender.team[bc->defender.battler].bVal.item = 0; // transfer sticky barb
                                }
                            }
                        }
                    }
                }
                bool shouldApplyEffect = false;
                if(move.secondary) {
                    //when checking if opponenet is alive, we may need
                    // to verify if the moves secondary targets us or them
                    // e.g. ancientpower buff vs flamewheel burn
                    if(bc->defender.team[bc->defender.battler].bVal.bHp > 0) {
                        log("\tChecking secondary effect chance");
                        shouldApplyEffect = (advanceSeed(bc) % 100) < move.secondaryAccuracy;
                        // in certain scenarios, hitting a substitute prevents secondary effect from triggering
                        // exact details are a bit fuzzy, but psybeam (10% confusion chance) does not trigger
                        // even if it knocks out the substitute with its damage.  But flare blitz does apply burn (supposedly, haven't tested yet).
                        // TODO: test this further, don't forget about this interaction when using substitute
                        if(bc->defender.team[bc->defender.battler].bVal.substituteWasHit) {
                            shouldApplyEffect = false; // do not apply effect instead.  We still roll the random value though.  
                        }                
                    } else {
                        advanceSeed(bc); // even if the defender has fainted, we still use the rng
                        // most likely this is because some moves have a secondary effect that affects the user
                    }

                } else {
                    if (move.effect != BATTLE_EFFECT_HIT){
                        shouldApplyEffect = true; // if the effect is the prime component of the move, we do it so long as we hit
                    }
                }
                if(shouldApplyEffect) {
                    if(DEBUG) {
                        std::cout << "\tTry Applying Effect" << std::endl;
                    }
                    // regardless of whether the effect was a secondary or a primary, we apply it
                    bool didApply = applyEffect(move, bc);
                    if(!move.secondary && didApply){
                        success = true; // status move successfully executed
                    }
                }
            }
            

        } else {
            log("\tMove Missed");
        }
    }
    if(bc->attacker.team[bc->attacker.battler].bVal.isCharging) {
        success = true; // we're charging
    }
    if(!success) {
        log("\tMove Failed");
    }
    // afterMove (technically should occur after any status disruption as well but we have a different control flow than the original code)
    updateMoveBuffers(bc, move);
    return success;
}
bool useItem(BattleContext *bc, int item) {
    int index;
    if(item & POKE_SLOTS) {
        index = getSwitchNum(item);
    } else {
        index = bc->attacker.battler;
    }
    if(item & COMMAND_USE_ITEM_GUARD_SPEC) {
        log(bc->attacker.name + " uses Guard Spec");
        applyMist(bc);
        return true;
    } else if(item & COMMAND_USE_ITEM_HYPER_POTION) {
        log(bc->attacker.name + " uses Hyper Potion");
        if(bc->attacker.team[index].bVal.bHp < bc->attacker.team[index].cHp) {
            heal(&bc->attacker.team[index], 200);
            return true;
        } else {
            return false;
        }
    } else if(item & COMMAND_USE_ITEM_SUPER_POTION) {
        log(bc->attacker.name + " uses Super Potion");
        if(bc->attacker.team[index].bVal.bHp < bc->attacker.team[index].cHp) {
            heal(&bc->attacker.team[index], 50);
            return true;
        } else {
            return false;
        }
    } else if(item & COMMAND_USE_ITEM_POTION) {
        log(bc->attacker.name + " uses Potion");
        if(bc->attacker.team[index].bVal.bHp < bc->attacker.team[index].cHp) {
            heal(&bc->attacker.team[index], 20);
            return true;
        } else {
            return false;
        }
    } else if(item & COMMAND_USE_ITEM_PRLZ_HEAL) {
        log(bc->attacker.name + " uses Prlz Heal");
        if(bc->attacker.team[index].bVal.condition & MON_CONDITION_PARALYSIS) {
            bc->attacker.team[index].bVal.condition = MON_CONDITION_NONE; // clear paralysis
        }
    } else if(item & COMMAND_USE_ITEM_FULL_HEAL) {
        log(bc->attacker.name + " uses Full Heal");
        if(bc->attacker.team[index].bVal.condition > 0 || bc->attacker.team[index].bVal.volConditions & VOLATILE_CONDITION_CONFUSION) {
            bc->attacker.team[index].bVal.condition = 0;
            bc->attacker.team[index].bVal.volConditions &= ~VOLATILE_CONDITION_CONFUSION;
        }
    } else if(item & COMMAND_USE_ITEM_FULL_RESTORE) {
        log(bc->attacker.name + " uses Full Restore");
        bool didHeal = false;
        bool didStatus = false;
        if(bc->attacker.team[index].bVal.bHp < bc->attacker.team[index].cHp) {
            heal(&bc->attacker.team[index], bc->attacker.team[index].cHp);
            didHeal = true;
        }
        if(bc->attacker.team[index].bVal.condition > 0 || bc->attacker.team[index].bVal.volConditions & VOLATILE_CONDITION_CONFUSION) {
            bc->attacker.team[index].bVal.condition = 0;
            bc->attacker.team[index].bVal.volConditions &= ~VOLATILE_CONDITION_CONFUSION;
            didStatus = true;
        }
        return didHeal || didStatus;
    } else if(item & COMMAND_USE_ITEM_X_ACCURACY) {
        log(bc->attacker.name + " uses X Accuracy");
        modifyStat(true, Stat::ACCURACY, 1, bc);
    } else if(item & COMMAND_USE_ITEM_X_SPEED) {
        log(bc->attacker.name + " uses X Speed");
        modifyStat(true, Stat::SPEED, 1, bc);
    } else if(item & COMMAND_USE_ITEM_X_ATK) {
        log(bc->attacker.name + " uses X ATK");
        modifyStat(true, Stat::ATTACK, 1, bc);
    } else if(item & COMMAND_USE_ITEM_ANTIDOTE) {
        log(bc->attacker.name + " uses Antidote");
        if(bc->attacker.team[index].bVal.condition & MON_CONDITION_ANY_POISON) {
            bc->attacker.team[index].bVal.condition = MON_CONDITION_NONE; // clear poison
        }
    };
    return false;
}
void setupBattle(BattleContext *bc) {
    bc->turnNumber = 0;
    bc->weather = 0;
    bc->moveWasSuccessful = false;
}
// returns true if the move was disrupted by a status and the user
// will be skipping their turn
bool checkStatusDisruption(BattleContext *bc, Move move) {
    PokeClient* attacker = &bc->attacker;
    Pokemon* attackingPokemon = &attacker->team[attacker->battler];
    if(attackingPokemon->bVal.volConditions & VOLATILE_CONDITION_FLINCH) {
        attackingPokemon->bVal.volConditions &= ~VOLATILE_CONDITION_FLINCH; // flinch removes itself 
        log(attackingPokemon->info.name + " was FLINCHED!");
        return true;
    }
    if(bc->attacker.team[bc->attacker.battler].bVal.turnsTaunted > 0) {
        // we got taunted :(((
        if(move.power == 0) {
            return true;
        }
    }
    if(attackingPokemon->bVal.volConditions & VOLATILE_CONDITION_CONFUSION) {
        attackingPokemon->bVal.volConditions -= (1 << VOLATILE_CONDITION_CONFUSION_SHIFT);
        if(attackingPokemon->bVal.volConditions & VOLATILE_CONDITION_CONFUSION) {
            // we are still confused after decrementing turn counter
            if(advanceSeed(bc) & 1) {
                // odd number, proceed as normal

            } else {
                // we hurt ourselves in confusion
                int dmgRoll = advanceSeed(bc) % 16;
                int dmg = calcDamage(bc, Struggle, 1, dmgRoll, true);
                log("hurt self");
                log(dmg);
                dealDamage(&bc->attacker.team[bc->attacker.battler], dmg, false);
                return true;
            }
        } else {
            // we broke free of confusion!
            if(DEBUG) {
                std::cout << "Battler " << logName(*attackingPokemon) << " snaps out of its confusion! " << std::endl;
            }
        }
    }
    if(attackingPokemon->bVal.condition & MON_CONDITION_PARALYSIS) {
        if(attackingPokemon->cAbility != AbilityId::MAGIC_GUARD) {
            if(advanceSeed(bc) % 4 == 0) {
                if(DEBUG) {
                    std::cout << "\tFully Paralyzed!" << std::endl;
                }
                return true; // disrupted
            }
        }
    }
    if(attackingPokemon->bVal.condition & MON_CONDITION_SLEEP) {
        int sleepDec = 1; // can be 2 if the pokemon has ability Early Bird, but we don't run into that
        if ((attackingPokemon->bVal.condition & MON_CONDITION_SLEEP) < sleepDec) {
            attackingPokemon->bVal.condition &= ~MON_CONDITION_SLEEP;
        } else {
            attackingPokemon->bVal.condition -= sleepDec;
        }
        if(attackingPokemon->bVal.condition & MON_CONDITION_SLEEP) {
            if(DEBUG) {
                std::cout << "\t " << attackingPokemon->info.name << " is fast asleep!" << std::endl;
            }
            return true; // disrupted
        }
    }
    if(attackingPokemon->bVal.condition & MON_CONDITION_FREEZE) {
        if(advanceSeed(bc) %5 !=0) {
            return true; // disrupted
        } else {
            attackingPokemon->bVal.condition &= ~MON_CONDITION_FREEZE; // thaw out

        }
    }

    return false;
}
void updateMoveBuffers(BattleContext *bc, Move move) {
    if(bc->moveWasSuccessful) {
        bc->attacker.previouslySelectedMove = move;
        bc->attacker.successfulMove = true;
    }
    updateFlagsWhenHit(bc, move);
}
void updateFlagsWhenHit(BattleContext *bc, Move move) {
    if(bc->moveWasSuccessful) {
        bc->defender.team[bc->defender.battler].bVal.moveHit = move;
    } else {
        bc->defender.team[bc->defender.battler].bVal.moveHit = Empty;
    }

    bc->defender.team[bc->defender.battler].bVal.substituteWasHit = false;

}
void resetContext(BattleContext *bc) {
    bc->moveWasSuccessful = false;
    bc->attacker.successfulMove = false;
    bc->previousDmg = bc->realDmg;
    bc->realDmg = 0;
}
void executeCommand(BattleContext *bc) {
    resetContext(bc);
    int command = bc->attacker.command;
    if(command & COMMAND_MOVE) {
        Move commandMove;
        if(bc->attacker.team[bc->attacker.battler].bVal.encoredMove.id != MoveId::NO_MOVE) {
            // we are encored
            commandMove = bc->attacker.team[bc->attacker.battler].bVal.encoredMove;
        } else {
            int moveNum = log2(command & COMMAND_MOVE);
            commandMove = bc->attacker.team[bc->attacker.battler].moveset[moveNum];
        }
        bool success = useMove(commandMove, bc);
        if(success) {
            log("\tMove Succeeded > RNG advances 2");
            // not sure why this occurs, but pretty certain this is correct now
            // what these random values are used for is unknown.
            if(bc->attacker.team[bc->attacker.battler].bVal.bHp > 0) {
                advanceSeed(bc);
            }
            if(bc->defender.team[bc->defender.battler].bVal.bHp > 0) {
                advanceSeed(bc);

            }
        }
        if(!bc->attacker.team[bc->attacker.battler].bVal.isCharging) {
            bc->attacker.command &= ~COMMAND_MOVE; // remove command move 
        }

    } else if(command & COMMAND_USE_ITEM) {
        bc->attacker.previouslySelectedMove = Empty;
        useItem(bc, command & (COMMAND_USE_ITEM | POKE_SLOTS));
    } else if(command & COMMAND_SWITCH) {
        bc->attacker.previouslySelectedMove = Empty;
        bc->attacker.pokeSwitch(getSwitchNum(command));
        bc->attacker.command &= ~COMMAND_SWITCH; // remove command switch
    }

}
bool isFightOver(BattleContext *bc) {
    if(bc->terminate) {
        // force termination
        if(bc->attacker.aiControl){
            bc->defender.isWinner = true;
        } else {
            bc->attacker.isWinner = true;
        }
        return true;
    }
    bool attackerWon = true;
    bool defenderWon = true;
    int i;
    for(i=0;i<6;i++){
        if(bc->attacker.team[i].bVal.bHp > 0) {
            defenderWon = false;
        }
    }
    for(i=0;i<6;i++){
        if(bc->defender.team[i].bVal.bHp > 0) {
            attackerWon = false;
        }
    }
    if(attackerWon || defenderWon) {
        if(attackerWon){
            bc->attacker.isWinner = true;
            log(bc->attacker.name + " wins");
        } else {
            bc->defender.isWinner = true;
            log(bc->defender.name + " wins");
        }
        return true;
    }
    return false;
}
void triggerGlobalEndTurnConditions(BattleContext *bc) {
    if(bc->weather & FIELD_CONDITION_SANDSTORM) {
        if(bc->weather & FIELD_CONDITION_SANDSTORM_PERM) {
            // do nothing
            log("\tSandstorm rages");
        } else if(--bc->weatherTurns == 0) {
            log("\tSandstorm subsides");
            bc->weather = 0;
            bc->weatherTurns = 0;
        } else {
            log("\tSandstorm rages");
        }
    }
    if(bc->weather & FIELD_CONDITION_SUNNY) {
        if(bc->weather & FIELD_CONDITION_SUNNY_PERM) {
            // do nothing
            log("\tThe sunlight is strong");
        } else if(--bc->weatherTurns == 0) {
            log("\tThe sunlight fades");
            bc->weather = 0;
            bc->weatherTurns = 0;
        } else {
            log("\tThe sunlight is strong");
        }
    }
    if(bc->weather & FIELD_CONDITION_HAILING) {
        if(bc->weather & FIELD_CONDITION_HAILING_PERM) {
            // do nothing
            log("\tIt continues to hail");
        } else if(--bc->weatherTurns == 0) {
            log("\tThe hail stops");
            bc->weather = 0;
            bc->weatherTurns = 0;
        } else {
            log("\tIt continues to hail");
        }
    }
    if(bc->weather & FIELD_CONDITION_TRICK_ROOM) {
        bc->weather -= (1 << FIELD_CONDITION_TRICK_ROOM_SHIFT);
        if(bc->weather & FIELD_CONDITION_TRICK_ROOM) {

        } else {
            log("\tTrick Room ends");
        }
    }
}
void triggerEndTurnConditions(PokeClient *p, BattleContext *bc) {
    // reset sub hit flag
    p->team[p->battler].bVal.substituteWasHit = false;
    // reset protection
    if(p->team[p->battler].bVal.isProtected) {
        p->team[p->battler].bVal.isProtected = false;
    } else {
        p->team[p->battler].bVal.turnsProtected = 0;
    }
    
    // flinch wears off
    p->team[p->battler].bVal.volConditions &= ~VOLATILE_CONDITION_FLINCH;

    // FIELD CONDITIONS //

    if(p->team[p->battler].bVal.bHp && bc->weather & FIELD_CONDITION_SANDSTORM) {
        Type type1 = p->team[p->battler].info.primaryType;
        Type type2 = p->team[p->battler].info.secondaryType;
        if(type1 != Type::Steel && type1 != Type::Rock && type1 != Type::Ground &&
        type2 != Type::Steel && type2 != Type::Rock && type2 != Type::Ground){
            // deal 1/16 damage
            dealDamage(&p->team[p->battler], p->team[p->battler].cHp/16, false); // sandstorm deals indirect damage
        }

    }

    // mist check occurs before condition checks
    if(p->team[p->battler].bVal.bHp && p->sideConditions & SIDE_CONDITION_MIST) {
        if(--p->mistTurns == 0) {
            p->sideConditions &= ~SIDE_CONDITION_MIST;
        }
    }

    // ITEMS //

    if(p->team[p->battler].bVal.item == ITEM_STICKY_BARB && p->team[p->battler].bVal.bHp) {
        dealDamage(&p->team[p->battler], p->team[p->battler].cHp/8, false); // sticky barb deals indirect damage
    }

    // REGULAR CONDITIONS //
    if(p->team[p->battler].bVal.moveEffectsMask & MOVE_EFFECT_AQUA_RING && p->team[p->battler].bVal.bHp) {
        int healAmount = p->team[p->battler].cHp/16;
        heal(&p->team[p->battler], healAmount);
    }
    if((p->team[p->battler].bVal.condition & MON_CONDITION_POISON) && p->team[p->battler].bVal.bHp) {
        int psnDmg = p->team[p->battler].cHp/8;
        if(DEBUG) {
            std::cout << "Poison deals " << psnDmg << " damage to " << logName(p->team[p->battler], psnDmg) << std::endl;
        }
        dealDamage(&p->team[p->battler], psnDmg, false);

    } else if((p->team[p->battler].bVal.condition & MON_CONDITION_BURN) && p->team[p->battler].bVal.bHp) {
        int brnDmg = p->team[p->battler].cHp/8;
        if(DEBUG) {
            std::cout << "Burn deals " << brnDmg << " damage to " << logName(p->team[p->battler], brnDmg) << std::endl;
        }
        dealDamage(&p->team[p->battler], brnDmg, false);

    } else if((p->team[p->battler].bVal.condition & MON_CONDITION_TOXIC) && p->team[p->battler].bVal.bHp) {
        int psnDmg = p->team[p->battler].cHp/16;
        if((p->team[p->battler].bVal.condition & MON_CONDITION_TOXIC_COUNTER) != MON_CONDITION_TOXIC_COUNTER) {
            p->team[p->battler].bVal.condition += MON_CONDITION_TOXIC_COUNT_INC; // increment the toxic counter
        }
        psnDmg = psnDmg * ((p->team[p->battler].bVal.condition & MON_CONDITION_TOXIC_COUNTER) >> 8);
        dealDamage(&p->team[p->battler], psnDmg, false); // indirectly deal damage
    }
    // taunt check
    if(p->team[p->battler].bVal.bHp && p->team[p->battler].bVal.turnsTaunted > 0) {
        if(--p->team[p->battler].bVal.turnsTaunted > 0) {
            for(int i=0;i<4;i++){
                if(p->team[p->battler].moveset[i].power == 0) {
                    p->team[p->battler].moveset[i].disabled = true;
                }
            }
        } else {
            for(int i=0;i<4;i++){
                if(p->team[p->battler].moveset[i].power == 0) {
                    p->team[p->battler].moveset[i].disabled = false;
                }
            }
        }
    }
    // yawn check 
    if(p->team[p->battler].bVal.bHp && p->team[p->battler].bVal.moveEffectsMask & MOVE_EFFECT_YAWN) {
        p->team[p->battler].bVal.moveEffectsMask -= (1 << MOVE_EFFECT_YAWN_SHIFT);
        if((p->team[p->battler].bVal.moveEffectsMask & MOVE_EFFECT_YAWN) == 0) {
            applySleep(&(p->team[p->battler]), bc, true); // put them to sleep
        }
    }
    // encore check
    if(p->team[p->battler].bVal.encoredMove.id != MoveId::NO_MOVE) {
        if(p->team[p->battler].bVal.turnsEncored > 0) {
            p->team[p->battler].bVal.turnsEncored--;
        } else {
            log("\tEncore ends");
            p->team[p->battler].bVal.encoredMove = Empty;
        }
    }



}
// doAttacker -> whether to trigger the attacker's effect or defenders
// so that we don't have to do everything twice.
bool activateTriggers(BattleContext *bc, bool doAttacker) {
    PokeClient *applyClient;
    PokeClient *receiveClient;
    if(doAttacker) {
        applyClient = &bc->attacker;
        receiveClient = &bc->defender;
    } else {
        applyClient = &bc->defender;
        receiveClient = &bc->attacker;
    }

    if(applyClient->triggers & TRIGGER_INTIMIDATE) {
        // doAttacker false, receive client is attacker
        // so targetSelf true, because self == attacker 
        modifyStat(!doAttacker, Stat::ATTACK, -1, bc);
        // announce ability (intimidate)
        applyClient->team[applyClient->battler].bVal.abilityKnownToAi = true;
        if(DEBUG) {
            std::cout << "Intimidate lowers " << logName(receiveClient->team[receiveClient->battler]) << " attack stat by 1" << std::endl;
        }
    }
    if(applyClient->triggers & TRIGGER_SAND_STREAM) {
        bc->weather &= ~FIELD_CONDITION_SUNNY;
        bc->weather &= ~FIELD_CONDITION_HAILING;
        bc->weather &= ~FIELD_CONDITION_RAINING;
        bc->weather |= FIELD_CONDITION_SANDSTORM_PERM;
        bc->weatherTurns = 100;
        log("Sand stream whips up a sandstorm");
        applyClient->team[applyClient->battler].bVal.abilityKnownToAi = true; // announced
    }
    if(applyClient->triggers & TRIGGER_TOXIC_SPIKES) {
        if(applyClient->team[applyClient->battler].info.primaryType == Type::Poison || applyClient->team[applyClient->battler].info.secondaryType == Type::Poison) {
            if(!(applyClient->team[applyClient->battler].info.primaryType == Type::Flying || applyClient->team[applyClient->battler].info.secondaryType == Type::Flying)) {
                applyClient->sideConditions &= ~SIDE_CONDITION_TOXIC_SPIKES;
                applyClient->toxicSpikeLayers = 0; //  remove all toxic spikes
            }
        }
        if(
            applyClient->team[applyClient->battler].info.primaryType == Type::Steel || 
            applyClient->team[applyClient->battler].info.secondaryType == Type::Steel ||
            applyClient->team[applyClient->battler].info.primaryType == Type::Flying ||
            applyClient->team[applyClient->battler].info.secondaryType == Type::Flying) {
                // do nothing
        } else {
            if(applyClient->toxicSpikeLayers == 1) {
                applyPoison(&applyClient->team[applyClient->battler], bc);
            } else {
                applyToxic(&applyClient->team[applyClient->battler], bc);
            }
        }
    }
    if(applyClient->triggers & TRIGGER_STEALTH_ROCK) {
        int typeResult = calcTypeMultiplier(Type::Rock, applyClient->team[applyClient->battler].info.primaryType, applyClient->team[applyClient->battler].info.secondaryType);
        int stealthDamage = applyClient->team[applyClient->battler].cHp / (320 / typeResult);
        if(DEBUG) {
            std::cout << "Stealth rock deals " << stealthDamage << " damage to " << logName(applyClient->team[applyClient->battler], stealthDamage) << std::endl;
        }
        dealDamage(&applyClient->team[applyClient->battler], stealthDamage, false); // indirect damage
    }
    applyClient->triggers = 0; // reset all triggers.  TODO: might need to not reset all of them
    return true;
}

int getSwitchNum(unsigned long command) {
    int switchNum = log2(command & POKE_SLOTS);
    switchNum = switchNum - COMMAND_SWITCH_OFFSET; // from 4-9 to 0-5
    return switchNum;
}
// switch in a new mon if we need it
// returns true if we swapped
// return false if we didn't.
bool switchIn(BattleContext *bc, bool attacker) {
    PokeClient *pc;
    PokeClient *opc;
    if(attacker) {
        pc = &bc->attacker;
        opc = &bc->defender;
    } else {
        pc = &bc->defender;
        opc = &bc->attacker;
    }
    int numValidMons = 0;
    if(!pc->aiControl) {
        // player control
        if(pc->command & COMMAND_SWITCH) { // we have a switch command that has persisted through the command selection
            int switchNum = getSwitchNum(pc->command);
            if(pc->team[switchNum].bVal.bHp > 0) {
                log(pc->name + " sends out " + pc->team[switchNum].info.name);
                pc->pokeSwitch(switchNum);
                return true;
            }
        } else {
            for(int i = 0; i<6; i++) {
                if(pc->team[i].bVal.bHp > 0) {
                    pc->pokeSwitch(i);
                    log(pc->name + " sends out " + pc->team[i].info.name);
                    return true;
                }
            }
        }
    } else {
        // the opponent has fainted
        int pick = AI_PostKOSwitch(bc);

        if(opc->command & COMMAND_SWITCH) {
            int switchNum = getSwitchNum(opc->command);
            
            if(opc->team[switchNum].bVal.bHp > 0) {
                log(opc->name + " sends out " + opc->team[switchNum].info.name);
                opc->pokeSwitch(switchNum);
                // do not return here, because the fight could be over
            }
        }
        if(pick < 6) {
            pc->pokeSwitch(pick);
            return true;
        }
        for(int i = 0; i<6; i++) {
            if(pc->team[i].bVal.bHp > 0) {
                    pc->pokeSwitch(i);
                    log(pc->name + " sends out " + pc->team[i].info.name);
                    return true;
                }
        }
    }

    return false;
}

bool endOfTurn(BattleContext *bc) {
    bool shouldContinue = true;
    bc->previousDmg = 0; // reset

    if(isFightOver(bc)){
        return false;
    } else {
        // advanceSeed(bc); // end of turn rng advancement, reason unknown
        triggerGlobalEndTurnConditions(bc); // do any battler-non-specific routines like weather
        bc->turnNumber++;
        // unknown why this seed advancement occurs
        if(bc->defender.team[bc->defender.battler].bVal.bHp <= 0) {
            if(bc->defender.command & COMMAND_BRANCH_IF_KO) {
                bc->branch++;
            }
            // im tired
        } else {
            advanceSeed(bc, "Unknown, defender isn't dead");
        }

        triggerEndTurnConditions(&bc->defender, bc);
        triggerEndTurnConditions(&bc->attacker, bc);
        if(bc->defender.team[bc->defender.battler].bVal.bHp <= 0) {
            shouldContinue = switchIn(bc, false);
            bc->defender.isSwitching = true;
        }

        if(shouldContinue) {
            advanceSeed(bc);
            if(bc->attacker.team[bc->attacker.battler].bVal.bHp <= 0) {
                if(bc->attacker.command & COMMAND_BRANCH_IF_KO) {
                    bc->branch++;
                }
                shouldContinue = switchIn(bc, true);
                bc->attacker.isSwitching = true;
            }

            // if((bc->attacker.isSwitching && bc->attacker.aiControl) || bc->defender.isSwitching && bc->defender.aiControl) {
            //     // the AI is doing an end-of-turn switch (Post-KO switch). We should allow a free switch in for the player .
            //     unsigned long command = bc->switchCommands[bc->switchCommandIndex];
            //     bc->switchCommandIndex++;
            //     log("Doing post-ko player switch");
            //     if(command > 0) {
            //         int switchNum = log2(command);
            //         switchNum = switchNum - COMMAND_SWITCH_OFFSET; // from 4-9 to 0-5
            //         if(bc->attacker.aiControl) {
            //             bc->defender.pokeSwitch(switchNum);
            //         } else {
            //             bc->attacker.pokeSwitch(switchNum);
            //         }
            //     }
            // }
            if(shouldContinue) {
                // do not do AI section if we are in the middle of a charge move
                log("before AI starts >> RNG advances 4");
                advanceSeed(bc);
                advanceSeed(bc); // ai advances the seed 4 for some reason we don't know yet
                activateTriggers(bc, true); // attacker effects
                activateTriggers(bc, false); // defender effects
                if(bc->defender.aiControl && bc->defender.team[bc->defender.battler].bVal.isCharging) {
                    return true;
                }
                if(bc->attacker.aiControl && bc->attacker.team[bc->attacker.battler].bVal.isCharging) {
                    return true;
                }


                processAI(bc);
                return true;
            } else {
                log(bc->defender.name + " Wins");
                bc->defender.isWinner = true;
                return false;
            }
        } else {
            log(bc->attacker.name + " Wins");
            bc->attacker.isWinner = true;
            return false;
        }
    }
}
// returns true if the battle should continue
bool doTurn(BattleContext *bc) {
    if(bc->attacker.command == COMMAND_TERMINATE || bc->defender.command == COMMAND_TERMINATE) {
        log("--Terminating--");
        bc->terminate = true;
        return endOfTurn(bc);
    }
    bc->attacker.isSwitching = false;
    bc->defender.isSwitching = false;
    log("\t\t\t\t\t\t\t[ Turn " + std::to_string(bc->turnNumber) + " ]");
    if(!determineOrder(bc)) {
        // need to switch current order
        PokeClient pc = bc->attacker;
        bc->attacker = bc->defender;
        bc->defender = pc;
    }
    activateTriggers(bc, true); // attacker effects
    activateTriggers(bc, false); // defender effects

    // turn overall start advances 4
    log("Turn Starts > RNG Advances 4"); // speed rand values that we don't really need
    advanceSeed(bc);
    advanceSeed(bc);
    advanceSeed(bc);
    advanceSeed(bc);
    executeCommand(bc);
    PokeClient tempC = bc->attacker;
    bc->attacker = bc->defender;
    bc->defender = tempC;
    activateTriggers(bc, true); // attacker effects
    activateTriggers(bc, false); // defender effects
    if(bc->attacker.team[bc->attacker.battler].bVal.bHp > 0) {
        // only do command if we're still alive
        log("Next battler turn > Rng advances 2");
        advanceSeed(bc);
        advanceSeed(bc);
        executeCommand(bc);
    } else {
        log(bc->attacker.team[bc->attacker.battler].info.name + " Fainted");
    }
    return endOfTurn(bc);
}

static const unsigned long Unk_ov6_02249522[4] = {
    ((1 << 8) | (3 & 0xff)),
    ((11 << 8) | (2 & 0xff)),
    ((12 << 8) | (4 & 0xff)),
    ((29 << 8) | (2 & 0xff))
};

int test() {
    std::cout << 2 << std::endl;
    return 1;
}
struct Tester {
    int val;
};
