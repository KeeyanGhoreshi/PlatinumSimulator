#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include "battle.h"
#include "battle_effects.h"

bool checkMoveEffect(Pokemon *p, int mc) {
    if(p->bVal.substituteHp > 0) {
        return false; // substitute blocks conditions
    }
    if(p->bVal.moveEffectsMask & mc) {
        return false; // cannot apply an effect that's already active
    }
    // normally we would apply the condition here, but some moves have special protocol that isn't standardized, so just do it in the function
    return true;
}
bool applyStatus(Pokemon *p, int sc, BattleContext *bc){
    if(p->bVal.substituteHp > 0) {
        return false; // substitute blocks conditions
    }
    if(p->bVal.condition == 0) {
        p->bVal.condition |= sc;
        return true;
    }
    return false;

}
bool applyVolatileStatus(Pokemon *p, int vc, BattleContext *bc) {
    bool canBeAdded = true;
    if(p->bVal.substituteHp > 0) {
        canBeAdded = false; // substitute blocks most volatile conditions
    }
    if(vc && p->bVal.volConditions){
        canBeAdded = false;
    }
    if(canBeAdded) {
        p->bVal.volConditions |= vc;
        return true;
    }
    return false;
}
int getStatStage(Pokemon *p, Stat stat) {
    switch(stat) {
        case Stat::ATTACK:
            return p->bVal.atkStg;
        case Stat::DEFENSE:
            return p->bVal.defStg;
        case Stat::SPECIAL_ATTACK:
            return p->bVal.spAtkStg;
        case Stat::SPECIAL_DEFENSE:
            return p->bVal.spDefStg;
        case Stat::SPEED:
            return p->bVal.spdStg;
        case Stat::ACCURACY:
            return p->bVal.accStg;
        case Stat::EVASION:
            return p->bVal.evaStg;
        default:
            return 6;
    }
}
bool modifyStat(bool affectSelf, Stat stat, int change, BattleContext *bc) {
    Pokemon* p;
    if(affectSelf) {
        p = &bc->attacker.team[bc->attacker.battler];
    } else {
        if(bc->defender.sideConditions & SIDE_CONDITION_MIST) {
            if(DEBUG) {
                std::cout << "\tStat reduction prevented by Mist" << std::endl;
            }
            return true; // prevented by mist // still successful, apparently, though I would think this is not true ?  TODO
        }
        p = &bc->defender.team[bc->defender.battler];
        if(p->bVal.substituteHp > 0) {
            return false; // prevented by substitute
        }
    }
    int stageModifier = getStatStage(p, stat);
    if(stageModifier == 12) {
        return true; // TODO might need more testing, but I'm pretty sure this doesn't count as true failure for the purpose of advancing rng twice, so we return true still
    } else if(stageModifier == 0) {
        return true;
    } else {
        stageModifier = stageModifier + change;
        if(stageModifier > 12) {
            stageModifier = 12;
        } else if(stageModifier < 0) {
            stageModifier = 0;
        }
        switch(stat) {
            case Stat::ATTACK:
                p->bVal.atkStg = stageModifier;
                break;
            case Stat::DEFENSE:
                p->bVal.defStg = stageModifier;
                break;
            case Stat::SPECIAL_ATTACK:
                p->bVal.spAtkStg = stageModifier;
                break;
            case Stat::SPECIAL_DEFENSE:
                p->bVal.spDefStg = stageModifier;
                break;
            case Stat::SPEED:
                p->bVal.spdStg = stageModifier;
                break;
            case Stat::ACCURACY:
                p->bVal.accStg = stageModifier;
                break;
            case Stat::EVASION:
                p->bVal.evaStg = stageModifier;
                break;
            default:
                return false;
        }
        return true;

    }
    return false;
}
bool applyBurn(Pokemon *p, BattleContext *bc) {
    if(p->info.primaryType == Type::Fire || p->info.secondaryType == Type::Fire) {
        return false;
    }
    bool appliedBurn = applyStatus(p, MON_CONDITION_BURN, bc);
    if(p->bVal.item == ITEM_RAWST_BERRY && appliedBurn) {
        if(DEBUG) {
            std::cout << "\tRawst Berry Prevents Burn" << std::endl;
        }
        p->bVal.item = 0; // consume rawst berry
        p->bVal.condition = 0; // reset condition
        return true; // the effect still technically succeeds
    }
    return appliedBurn;
}
bool applyPoison(Pokemon *p, BattleContext *bc) {
    bool appliedPoison = applyStatus(p, MON_CONDITION_POISON, bc);
    if(p->bVal.item == ITEM_PECHA_BERRY && appliedPoison) {
        if(DEBUG) {
            std::cout << "\tPecha Berry Prevents Poison" << std::endl;
        }
        p->bVal.item = 0; // consume pecha berry
        p->bVal.condition = 0; // reset condition
        return true; // the poisoning effect still technically succeeds
    }
    return appliedPoison;
}
bool applyParalysis(Pokemon *p, BattleContext *bc) {
    bool appliedParalysis = applyStatus(p, MON_CONDITION_PARALYSIS, bc);
    if(p->bVal.item == ITEM_CHERI_BERRY && appliedParalysis) {
        if(DEBUG) {
            std::cout << "\tCheri Berry Prevents Paralysis" << std::endl;
        }
        p->bVal.item = 0; // consume cheri berry
        p->bVal.condition = 0; // reset condition
        return true; // the paralysis effect still technically succeeds
    }
    return appliedParalysis;
}
bool applyFreeze(Pokemon *p, BattleContext *bc) {
    bool appliedFreeze = applyStatus(p, MON_CONDITION_FREEZE, bc);
    if(p->bVal.item == ITEM_ASPEAR_BERRY && appliedFreeze) {
        if(DEBUG) {
            std::cout << "\tAspear Berry Prevents Freeze" << std::endl;
        }
        p->bVal.item = 0; // consume aspear berry
        p->bVal.condition = 0; // reset condition
        return true; // the paralysis effect still technically succeeds
    }
    return appliedFreeze;
}

bool applyConfusion(Pokemon *p, BattleContext *bc) {
    if(p->bVal.volConditions & VOLATILE_CONDITION_CONFUSION) {
        return false;
    }
    if(p->bVal.substituteHp > 0) {
        return false; // prevented by substitute
    }
    int turnsRemaining = 2 + advanceSeed(bc, "determine how long to be confused for") % 4;
    if(DEBUG) {
        std::cout << "Confused for " << turnsRemaining << " turns" << std::endl;
    }
    turnsRemaining = turnsRemaining << VOLATILE_CONDITION_CONFUSION_SHIFT;
    return applyVolatileStatus(p, turnsRemaining, bc);
}
bool applySleep(Pokemon *p, BattleContext *bc, bool bypass) {
    // 0018 subscript
    if(p->bVal.condition & MON_CONDITION_ANY) {
        return false;
    }
    // bypass substitute if this is a bypassed sleep, eg from yawn
    if(p->bVal.substituteHp > 0 && !bypass) {
        return false; // prevented by substitute
    }
    // TODO logic for various interactions if required eg vital spirit, insomnia

    int turnsRemaining = 2 + advanceSeed(bc, "determine how long to be asleep for") % 4;
    if(DEBUG) {
        std::cout << turnsRemaining << " turns asleep" << std::endl;
    }
    return applyStatus(p, turnsRemaining, bc);
}
bool applyTaunt(Pokemon *p, BattleContext *bc) {
    if(p->bVal.turnsTaunted > 0) {
        return false;
    } else {
        p->bVal.turnsTaunted = 3 + (advanceSeed(bc) % 3);
    }
    if(DEBUG) {
        std::cout << "\tTaunt will last " << p->bVal.turnsTaunted << " turns" << std::endl;
    }
    return true;
}
bool applyMist(BattleContext *bc) {
    if(bc->attacker.sideConditions & SIDE_CONDITION_MIST) {
        return false; // can't reapply mist until it's over
    } else {
        bc->attacker.mistTurns = 5;
        bc->attacker.sideConditions |= SIDE_CONDITION_MIST;
        return true;
    }
}
bool applyEndeavor(BattleContext *bc) {
    int aHp = bc->attacker.team[bc->attacker.battler].bVal.bHp;
    int dHp = bc->defender.team[bc->defender.battler].bVal.bHp;
    if(aHp < dHp){
        bc->cDmg = dHp - aHp;
        return true;
    }
    return false;
}
bool applyMirrorCoat(BattleContext *bc) {
    if(bc->defender.previouslySelectedMove.attack == DamageType::SPECIAL && bc->previousDmg > 0) {
        bc->cDmg = bc->previousDmg * 2;
        return true;
    } else {
        return false;
    }
}
bool applyGyroBall(BattleContext *bc) {
    bc->cPwr = 1 + 25 * calcSpeed(&bc->defender) / calcSpeed(&bc->attacker);
    if(bc->cPwr > 150) {
        bc->cPwr = 150;
    }
    return true;
}
bool applySubstitute(BattleContext *bc) {
    Pokemon* attacker = &bc->attacker.team[bc->attacker.battler];
    if(attacker->bVal.substituteHp == 0) {
        int subHealth = attacker->cHp / 4;
        if(attacker->bVal.bHp > subHealth) {
            dealDamage(attacker, subHealth, false);
            attacker->bVal.substituteHp = subHealth;
            return true;
        }
    }
    return false;
}
bool applyFlinch(Pokemon *p, BattleContext *bc) {
    bool appliedFlinch = applyVolatileStatus(p, VOLATILE_CONDITION_FLINCH, bc);
    return appliedFlinch;
}
bool applyRecoilQuarter(BattleContext *bc) {
    int recoil = bc->realDmg / 4;
    dealDamage(&bc->attacker.team[bc->attacker.battler], recoil, false); // apply indirect damage equal to a quarter of real damage dealt
    return true; // this effect always succeeds if its triggered -- unless the pokemon has an ability that cancels it but we'll TODO that if it comes up
}
bool applyDrainHalf(BattleContext *bc) {
    int drain = bc->realDmg / 2;
    heal(&bc->attacker.team[bc->attacker.battler], drain);
    return true;
}
bool applyRecover(BattleContext *bc) {
    if(bc->attacker.team[bc->attacker.battler].bVal.bHp == bc->attacker.team[bc->attacker.battler].cHp) {
        return false; // at full hp, fails
    }
    heal(&bc->attacker.team[bc->attacker.battler], bc->attacker.team[bc->attacker.battler].cHp/2);
    return true;
}

bool applyYawn(BattleContext *bc) {
    bool appliedYawn = checkMoveEffect(&bc->defender.team[bc->defender.battler], MOVE_EFFECT_YAWN);
    if(bc->defender.team[bc->defender.battler].bVal.condition & MON_CONDITION_ANY) {
        appliedYawn = false; // cannot apply yawn to a pokemon with any non-volatile condition already
    }
    if(appliedYawn) {
        bc->defender.team[bc->defender.battler].bVal.moveEffectsMask |= (1 << (MOVE_EFFECT_YAWN_SHIFT + 1));
    }
    return appliedYawn;
}
bool applyToxic(Pokemon *p, BattleContext *bc) {
    bool appliedPoison = applyStatus(p, MON_CONDITION_TOXIC, bc);
    if(p->bVal.item == ITEM_PECHA_BERRY && appliedPoison) {
        if(DEBUG) {
            std::cout << "\tPecha Berry Prevents Poison" << std::endl;
        }
        p->bVal.item = 0; // consume pecha berry
        p->bVal.condition = 0; // reset condition
        return true; // the poisoning effect still technically succeeds
    }
    return appliedPoison;
}

bool applyRevenge(BattleContext *bc) {
    bc->cPwr = 60; // TODO -> if we ever actually need to simulate revenge doubling its power, finish this function. Otherwise, we can just leave it as is
    return true;
}

bool applyFlinchPlus(BattleContext *bc, int condition) {

    if((advanceSeed(bc) % 100) < 10) {
        if(condition & MON_CONDITION_PARALYSIS) {
            applyParalysis(&bc->defender.team[bc->defender.battler], bc);
        } else if(condition & MON_CONDITION_BURN) {
            applyBurn(&bc->defender.team[bc->defender.battler], bc);
        } else if(condition & MON_CONDITION_FREEZE) {
            applyFreeze(&bc->defender.team[bc->defender.battler], bc);
        }
    }
    if((advanceSeed(bc) % 100) < 10) {
        applyFlinch(&bc->defender.team[bc->defender.battler], bc);
    }
    return true;

}

bool applyProtect(BattleContext *bc) {
    unsigned long protectSuccessRate[4] = {
        0xFFFF,
        0x7FFF, 
        0x3FFF, 
        0x1FFF, 
    };
    if(protectSuccessRate[bc->attacker.team[bc->attacker.battler].bVal.turnsProtected] >= advanceSeed(bc)){
        bc->attacker.team[bc->attacker.battler].bVal.isProtected = true;
        bc->attacker.team[bc->attacker.battler].bVal.turnsProtected++;
        if(bc->attacker.team[bc->attacker.battler].bVal.turnsProtected > 3) {
            bc->attacker.team[bc->attacker.battler].bVal.turnsProtected = 3;
        }
        return true;
    } else {
        return false;
    }
}
bool applyToxicSpikes(BattleContext *bc) {
    if(bc->defender.sideConditions & SIDE_CONDITION_TOXIC_SPIKES) {
        if(bc->defender.toxicSpikeLayers == 1) {
            bc->defender.toxicSpikeLayers++;
            return true;
        } else {
            return false; // already applied 2 layers
        }
    } else {
        bc->defender.toxicSpikeLayers = 1;
        bc->defender.sideConditions |= SIDE_CONDITION_TOXIC_SPIKES;
        return true;
    }
}
bool applySandstorm(BattleContext *bc) {
    if(bc->weather & FIELD_CONDITION_SANDSTORM) {
        return false; // fails
    }
    bc->weather = FIELD_CONDITION_SANDSTORM_TEMP;
    bc->weatherTurns = 5;
    return true;
}
bool applySunnyDay(BattleContext *bc) {
    if(bc->weather & FIELD_CONDITION_SUNNY) {
        return false; // fails
    }
    bc->weather &= ~FIELD_CONDITION_HAILING;
    bc->weather &= ~FIELD_CONDITION_RAINING;
    bc->weather &= ~FIELD_CONDITION_SANDSTORM;
    bc->weather = FIELD_CONDITION_SUNNY_TEMP;
    bc->weatherTurns = 5;
    return true;
}

bool applyHail(BattleContext *bc) {
    if(bc->weather & FIELD_CONDITION_HAILING) {
        return false; // hail already active
    } else {
        bc->weather &= ~FIELD_CONDITION_SUNNY;
        bc->weather &= ~FIELD_CONDITION_RAINING;
        bc->weather &= ~FIELD_CONDITION_SANDSTORM;
        bc->weather |= FIELD_CONDITION_HAILING_TEMP;
        bc->weatherTurns = 5; // lasts 5 turns
    }
    return true;
}

bool applyStealthRock(BattleContext *bc) {
    if(bc->defender.sideConditions & SIDE_CONDITION_STEALTH_ROCK) {
        return false; // already have stealth rock up
    }
    bc->defender.sideConditions |= SIDE_CONDITION_STEALTH_ROCK;
    return true;
}
bool applyEncore(BattleContext *bc) {
    if(bc->defender.previouslySelectedMove.id != MoveId::NO_MOVE) {
        bc->defender.team[bc->defender.battler].bVal.encoredMove = bc->defender.previouslySelectedMove;
        bc->defender.team[bc->defender.battler].bVal.turnsEncored = advanceSeed(bc, "Encore rng") % 5 + 3;
        return true;
    } else {
        return false;
    }
}
bool applyTrickRoom(BattleContext *bc){
    if(bc->weather & FIELD_CONDITION_TRICK_ROOM) {
        return false; // trick room already active
    }
    bc->weather |= (1<<16 | 1<<18); // apply trick room
    advanceSeed(bc);
    advanceSeed(bc);
    advanceSeed(bc);
    advanceSeed(bc);
    // okay so trick room seems to advance the rng 4? Maybe the game is recalculating the speed rands
    return true;
}
bool applyBatonPass(BattleContext *bc) {
    int switchnum = getSwitchNum(bc->attacker.command & POKE_SLOTS);
    int currentSubHealth = bc->attacker.team[bc->attacker.battler].bVal.substituteHp;
    int currentMoveMask = bc->attacker.team[bc->attacker.battler].bVal.moveEffectsMask;
    int atk = bc->attacker.team[bc->attacker.battler].bVal.atkStg;
    int def = bc->attacker.team[bc->attacker.battler].bVal.defStg;
    int spatk = bc->attacker.team[bc->attacker.battler].bVal.spAtkStg;
    int spdef = bc->attacker.team[bc->attacker.battler].bVal.spDefStg;
    int speed = bc->attacker.team[bc->attacker.battler].bVal.spdStg;
    int acc = bc->attacker.team[bc->attacker.battler].bVal.accStg;
    int eva = bc->attacker.team[bc->attacker.battler].bVal.evaStg;
    if(bc->attacker.team[switchnum].bVal.bHp > 0) {
        bc->attacker.pokeSwitch(switchnum);
        bc->attacker.team[bc->attacker.battler].bVal.substituteHp = currentSubHealth;
        bc->attacker.team[bc->attacker.battler].bVal.moveEffectsMask = currentMoveMask;
        bc->attacker.team[bc->attacker.battler].bVal.atkStg = atk;
        bc->attacker.team[bc->attacker.battler].bVal.defStg = def;
        bc->attacker.team[bc->attacker.battler].bVal.spAtkStg = spatk;
        bc->attacker.team[bc->attacker.battler].bVal.spDefStg = spdef;
        bc->attacker.team[bc->attacker.battler].bVal.spdStg = speed;
        bc->attacker.team[bc->attacker.battler].bVal.accStg = acc;
        bc->attacker.team[bc->attacker.battler].bVal.evaStg = eva;
        return true;
    }

    return false;
}
bool applyAquaRing(BattleContext *bc) {
    if(bc->attacker.team[bc->attacker.battler].bVal.moveEffectsMask & MOVE_EFFECT_AQUA_RING) {
        return false; // already active
    }
    bc->attacker.team[bc->attacker.battler].bVal.moveEffectsMask |= MOVE_EFFECT_AQUA_RING;
    return true;
}
bool applyEffect(Move move, BattleContext *bc) {
    // gigantic switch/case lol
    switch(move.effect) {
        case BATTLE_EFFECT_BURN_HIT:
        case BATTLE_EFFECT_STATUS_BURN:
            return applyBurn(&bc->defender.team[bc->defender.battler], bc);
        case BATTLE_EFFECT_PARALYZE_HIT:
        case BATTLE_EFFECT_STATUS_PARALYZE:
        case BATTLE_EFFECT_BOUNCE:
            return applyParalysis(&bc->defender.team[bc->defender.battler], bc);
        case BATTLE_EFFECT_STATUS_POISON:
        case BATTLE_EFFECT_POISON_HIT:
        case BATTLE_EFFECT_HIGH_CRITICAL_POISON_HIT:
            return applyPoison(&bc->defender.team[bc->defender.battler], bc);
        case BATTLE_EFFECT_FREEZE_HIT:
            return applyFreeze(&bc->defender.team[bc->defender.battler], bc);
        case BATTLE_EFFECT_STATUS_CONFUSE:
        case BATTLE_EFFECT_CONFUSE_HIT:
            return applyConfusion(&bc->defender.team[bc->defender.battler], bc);
        case BATTLE_EFFECT_FLINCH_HIT:
            return applyFlinch(&bc->defender.team[bc->defender.battler], bc);
        case BATTLE_EFFECT_LOWER_DEFENSE_HIT:
            return modifyStat(false, Stat::DEFENSE, -1, bc);
        case BATTLE_EFFECT_ATK_DOWN_2:
            return modifyStat(false, Stat::ATTACK, -2, bc);
        case BATTLE_EFFECT_ATK_DOWN:
            return modifyStat(false, Stat::ATTACK, -1, bc);
        case BATTLE_EFFECT_DEF_DOWN_2:
            return modifyStat(false, Stat::DEFENSE, -2, bc);
        case BATTLE_EFFECT_ACC_DOWN:
        case BATTLE_EFFECT_LOWER_ACCURACY_HIT:
            return modifyStat(false, Stat::ACCURACY, -1, bc);
        case BATTLE_EFFECT_LOWER_SP_DEF_HIT:
            return modifyStat(false, Stat::SPECIAL_DEFENSE, -1, bc);
        case BATTLE_EFFECT_EVA_UP:
            return modifyStat(true, Stat::EVASION, 1, bc);
        case BATTLE_EFFECT_RAISE_SP_ATK_HIT:
            return modifyStat(true, Stat::SPECIAL_ATTACK, 1, bc);
        case BATTLE_EFFECT_TAUNT:
            return applyTaunt(&bc->defender.team[bc->defender.battler], bc);
        case BATTLE_EFFECT_PREVENT_STAT_REDUCTION:
            return applyMist(bc);
        case BATTLE_EFFECT_SET_HP_EQUAL_TO_USER:
            return applyEndeavor(bc);
        case BATTLE_EFFECT_SET_SUBSTITUTE:
            return applySubstitute(bc);
        case BATTLE_EFFECT_RECOIL_QUARTER:
            return applyRecoilQuarter(bc);
        case BATTLE_EFFECT_RECOVER_HALF_DAMAGE_DEALT:
            return applyDrainHalf(bc);
        case BATTLE_EFFECT_RESTORE_HALF_HP:
            return applyRecover(bc);
        case BATTLE_EFFECT_STATUS_SLEEP_NEXT_TURN: 
            return applyYawn(bc);
        case BATTLE_EFFECT_STATUS_BADLY_POISON:
            return applyToxic(&bc->defender.team[bc->defender.battler], bc);
        case BATTLE_EFFECT_DOUBLE_POWER_IF_HIT:
            return applyRevenge(bc);
        case BATTLE_EFFECT_FLINCH_PARALYZE_HIT:
            return applyFlinchPlus(bc, MON_CONDITION_PARALYSIS);
        case BATTLE_EFFECT_FLINCH_BURN_HIT:
            return applyFlinchPlus(bc, MON_CONDITION_BURN);
        case BATTLE_EFFECT_FLINCH_FREEZE_HIT:
            return applyFlinchPlus(bc, MON_CONDITION_FREEZE);
        case BATTLE_EFFECT_PROTECT:
            return applyProtect(bc);
        case BATTLE_EFFECT_TOXIC_SPIKES:
            return applyToxicSpikes(bc);
        case BATTLE_EFFECT_SP_ATK_SP_DEF_UP: 
            modifyStat(true, Stat::SPECIAL_ATTACK, 1, bc);
            return modifyStat(true, Stat::SPECIAL_DEFENSE, 1, bc);
        case BATTLE_EFFECT_DEF_SPD_DOWN_HIT:
            modifyStat(true, Stat::DEFENSE, -1, bc);
            return modifyStat(true, Stat::SPECIAL_DEFENSE, -1, bc);
        case BATTLE_EFFECT_DEF_SPD_UP:
            modifyStat(true, Stat::DEFENSE, 1, bc);
            return modifyStat(true, Stat::SPECIAL_DEFENSE, 1, bc);
        case BATTLE_EFFECT_USER_SP_ATK_DOWN_2:
            return modifyStat(true, Stat::SPECIAL_ATTACK, -2, bc);
        case BATTLE_EFFECT_RAISE_ALL_STATS_HIT:
            modifyStat(true, Stat::ATTACK, 1, bc);
            modifyStat(true, Stat::DEFENSE, 1, bc);
            modifyStat(true, Stat::SPECIAL_ATTACK, 1, bc);
            modifyStat(true, Stat::SPECIAL_DEFENSE, 1, bc);
            modifyStat(true, Stat::SPEED, 1, bc);
            return true;

        case BATTLE_EFFECT_WEATHER_SANDSTORM:
            return applySandstorm(bc);
        case BATTLE_EFFECT_WEATHER_SUN:
            return applySunnyDay(bc);
        case BATTLE_EFFECT_WEATHER_HAIL:
            return applyHail(bc);
        case BATTLE_EFFECT_STEALTH_ROCK:
            return applyStealthRock(bc);
        case BATTLE_EFFECT_ENCORE:
            return applyEncore(bc);
        case BATTLE_EFFECT_TRICK_ROOM:
            return applyTrickRoom(bc);
        case BATTLE_EFFECT_PASS_STATS_AND_STATUS:
            return applyBatonPass(bc);
        case BATTLE_EFFECT_RESTORE_HP_EVERY_TURN:
            return applyAquaRing(bc);
        default:
            return false;
    }
    return false;
}


