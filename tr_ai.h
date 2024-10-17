#ifndef TR_AI_H_
#define TR_AI_H_
#include "battle.h"
#define COMP_POWER_MAX 1
#define COMP_POWER_NOT_MAX 2
#define COMP_POWER_NONE 3
#define COMP_POWER_KO 4

#define QUAD_EFFECTIVE (160)
#define SUPER_EFFECTIVE (80)
#define STAB_EFFECTIVE (60)
#define NORMAL_EFFECTIVE (40)
#define NOT_EFFECTIVE (20)
#define QUAD_NOT_EFFECTIVE (10)
#define IMMUNE_EFFECTIVE (0)

#define AI_BASIC (1 << 0)
#define AI_STRONG (1 << 1)
#define AI_EXPERT (1 << 2)

#define PARA_UNDER 0
#define PARA_EQUAL 1
#define PARA_OVER 2
static	const int NoCompPowerSeqNo[]={
	7, // BATTLE_EFFECT_HALVE_DEFENSE (explosion)
	8, // BATTLE_EFFECT_RECOVER_DAMAGE_SLEEP
	39, // BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT
	75, // BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT_FLINCH
	80, // BATTLE_EFFECT_RECHARGE_AFTER
	145, // BATTLE_EFFECT_CHARGE_TURN_DEF_UP
	151, // BATTLE_EFFECT_SKIP_CHARGE_TURN_IN_SUN
	161, // BATTLE_EFFECT_SPIT_UP
	170, // focus punchie
	182, // BATTLE_EFFECT_LOWER_OWN_ATK_AND_DEF
	190, // BATTLE_EFFECT_DECREASE_POWER_WITH_LESS_USER_HP
	248, // BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING
	269, // BATTLE_EFFECT_RECOIL_HALF
	0xffff,
};

static	const int OkCompPowerSeqNo[]={
	135, // BATTLE_EFFECT_RANDOM_POWER_BASED_ON_IVS	
	219, // BATTLE_EFFECT_POWER_BASED_ON_LOW_SPEED	
	222, // BATTLE_EFFECT_NATURAL_GIFT		
	268, // BATTLE_EFFECT_JUDGEMENT	
	41, // BATTLE_EFFECT_40_DAMAGE_FLAT
	87,	// BATTLE_EFFECT_LEVEL_DAMAGE_FLAT	
	88,	// BATTLE_EFFECT_RANDOM_DAMAGE_1_TO_150_LEVEL
	121, // BATTLE_EFFECT_POWER_BASED_ON_FRIENDSHIP		
	123, // BATTLE_EFFECT_POWER_BASED_ON_LOW_FRIENDSHIP		
	130, // BATTLE_EFFECT_10_DAMAGE_FLAT		
	196, // BATTLE_EFFECT_INCREASE_POWER_WITH_WEIGHT
	0xffff,
};


struct AiContext {
    int damageLoss[4];
    int score[4];

	bool dmgCalcLossFlag;
	bool dmgCalcNoLossFlag;
	int dmgCalcNoLoss[4];
	int dmgCalcLoss[4];

    int currentIndex; // which move is being considered right now
    int currentScore; // score being actively modified before being finalized in the score var
    int currentPowerFlag; // whether the current move is most powerful, not most powerful, or doesn't deal damage
    int currentTypeAdvantage; // current moves type advantage
    bool lossCalcOn;
    PokeClient self;
    PokeClient target;
};
void ailog(std::string text);
int AI_PostKOSwitch(BattleContext *bc);

bool AI_HasAbsorbAbilityInParty(BattleContext *bc, AiContext *ac);
bool AI_HasSuperEffectiveMove(BattleContext *bc, AiContext *ac, bool flag);
int AI_SelectCommand(BattleContext *bc, AiContext *ac);
bool AI_ShouldSwitch(BattleContext *bc, AiContext *ac);
bool AI_ShouldUseItem(BattleContext *bc, AiContext *ac);
bool AI_IfRndUnder(BattleContext *bc, int value);
bool AI_IfRndOver(BattleContext *bc, int value);
bool AI_IfHpUnder(BattleContext *bc, AiContext *ac, int value, bool target);
bool AI_IfHpOver(BattleContext *bc, AiContext *ac, int value, bool target);
bool AI_IfPara(BattleContext *bc, AiContext *ac, int comparator, Stat stg, int value, bool target);
int AI_DamageCalc(BattleContext *bc, AiContext *ac, Move move, int loss);
int AI_CompPowerCalc(BattleContext *bc, AiContext *ac, int *damage);
static bool AI_DoesMoveKo(BattleContext *bc, AiContext *ac);
static void AI_CheckTypeAdvantage(BattleContext *bc, AiContext *ac);
static void AI_CompPower(BattleContext *bc, AiContext *ac);
int processAI(BattleContext *bc);
static void AI_INCDEC(AiContext *ac, int val);
bool AI_DEC1(AiContext *ac);
bool AI_DEC10(AiContext *ac);
bool AI_DEC12(AiContext *ac);
int AI_CheckWeather(BattleContext *bc);
AbilityId AI_CheckAbility(BattleContext *bc, AiContext *ac, bool target);
bool AI_BasicPoison(BattleContext *bc, AiContext *ac);
bool AI_BasicDamageEnd(BattleContext *bc, AiContext *ac);
bool AI_BasicDamage(BattleContext *bc, AiContext *ac);
bool AI_BasicDamageStart(BattleContext *bc, AiContext *ac);
bool AI_BasicSeq(BattleContext *bc, AiContext *ac);
bool AI_CheckEffect(BattleContext *bc, AiContext *ac);
bool AI_BasicEvaUp(BattleContext *bc, AiContext *ac);
bool AI_BasicPoison_NoLeafGuard(BattleContext *bc, AiContext *ac);
bool AI_BasicPoison_NoHydration(BattleContext *bc, AiContext *ac);
bool AI_BasicAttackDown(BattleContext *bc, AiContext *ac);
bool AI_BasicDefenseDown(BattleContext *bc, AiContext *ac);
bool AI_BasicAccuracyDown(BattleContext *bc, AiContext *ac);
bool AI_BasicClearBody(BattleContext *bc, AiContext *ac);
bool AI_BasicRiskyDamage(BattleContext *bc, AiContext *ac);
bool AI_BasicCalmMind(BattleContext *bc, AiContext *ac);
bool AI_BasicCosmicPower(BattleContext *bc, AiContext *ac);
bool AI_BasicRecover(BattleContext *bc, AiContext *ac);
bool AI_BasicSandstorm(BattleContext *bc, AiContext *ac);
bool AI_BasicSunnyDay(BattleContext *bc, AiContext *ac);
bool AI_BasicBurn(BattleContext *bc, AiContext *ac);
bool AI_BasicStealthRock(BattleContext *bc, AiContext *ac);

bool AI_ExpertSeq(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_2(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_3(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_4(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_5(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_6(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_6_1(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_7(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_8(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_9(BattleContext *bc, AiContext *ac);
bool AI_ExpertAccuracyDown_End(BattleContext *bc, AiContext *ac);
bool AI_ExpertPoison(BattleContext *bc, AiContext *ac);
bool AI_ExpertPoison_1(BattleContext *bc, AiContext *ac);
bool AI_ExpertPoison_End(BattleContext *bc, AiContext *ac);
bool AI_ExpertDefenceDown(BattleContext *bc, AiContext *ac);
bool AI_ExpertDefenceDown_1(BattleContext *bc, AiContext *ac);
bool AI_ExpertDefenceDown_2(BattleContext *bc, AiContext *ac);
bool AI_ExpertDefenceDown_End(BattleContext *bc, AiContext *ac);
bool AI_StrongSeq(BattleContext *bc, AiContext *ac);
bool AI_StrongMoveCheck(BattleContext *bc, AiContext *ac);
bool AI_StrongMoveCheck_1(BattleContext *bc, AiContext *ac);
bool AI_StrongSuperEffective(BattleContext *bc, AiContext *ac);
bool AI_Strong10(BattleContext *bc, AiContext *ac);
bool AI_StrongKO(BattleContext *bc, AiContext *ac);
bool AI_StrongKO_Weak(BattleContext *bc, AiContext *ac);
bool AI_StrongKO_Minor(BattleContext *bc, AiContext *ac);
bool AI_StrongKO2(BattleContext *bc, AiContext *ac);
bool AI_StrongEnd(BattleContext *bc, AiContext *ac);
bool AI_ExpertHighCritical(BattleContext *bc, AiContext *ac);
bool AI_ExpertHighCritical_1(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp_1(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp_2(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp_4(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp_5(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp_6(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp_7(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp_8(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp_9(BattleContext *bc, AiContext *ac);
bool AI_ExpertEvasionUp_end(BattleContext *bc, AiContext *ac);
bool AI_ExpertEndeavor(BattleContext *bc, AiContext *ac);
bool AI_ExpertEndeavor_1(BattleContext *bc, AiContext *ac);
bool AI_ExpertEndeavor_2(BattleContext *bc, AiContext *ac);
bool AI_ExpertAttackDown(BattleContext *bc, AiContext *ac);
bool AI_ExpertAttackDown_1(BattleContext *bc, AiContext *ac);
bool AI_ExpertAttackDown_2(BattleContext *bc, AiContext *ac);
bool AI_ExpertAttackDown_3(BattleContext *bc, AiContext *ac);

bool AI_BasicConfuse(BattleContext *bc, AiContext *ac);
bool AI_ExpertConfuse(BattleContext *bc, AiContext *ac);
bool AI_ExpertConfuse_1(BattleContext *bc, AiContext *ac);
bool AI_ExpertConfuse_end(BattleContext *bc, AiContext *ac);

bool AI_ExpertBypassAccuracy(BattleContext *bc, AiContext *ac);
bool AI_ExpertBypassAccuracy_1(BattleContext *bc, AiContext *ac);
bool AI_ExpertBypassAccuracy_2(BattleContext *bc, AiContext *ac);

bool AI_ExpertToxic(BattleContext *bc, AiContext *ac);
bool AI_ExpertToxic_CheckEffects(BattleContext *bc, AiContext *ac);
bool AI_ExpertToxic_CheckHp(BattleContext *bc, AiContext *ac);

bool AI_ExpertRevenge(BattleContext *bc, AiContext *ac);
bool AI_ExpertBrine(BattleContext *bc, AiContext * ac);
bool AI_BasicParalyze(BattleContext *bc, AiContext *ac);
bool AI_ExpertParalyze(BattleContext *bc, AiContext *ac);

bool AI_ExpertRechargeTurn(BattleContext *bc, AiContext *ac);

bool AI_ExpertSpDefenseUp(BattleContext *bc, AiContext *ac);

bool AI_ExpertInvulnTurn(BattleContext *bc, AiContext *ac);

bool AI_ExpertCloseCombat(BattleContext *bc, AiContext *ac);

bool AI_ExpertDrainMove(BattleContext *bc, AiContext *ac);

bool AI_ExpertMirrorCoat(BattleContext *bc, AiContext *ac);
bool AI_ExpertMirrorCoat_Minus1(BattleContext *bc, AiContext *ac);

bool AI_ExpertGyroBall(BattleContext *bc, AiContext *ac);

bool AI_ExpertRecovery(BattleContext *bc, AiContext *ac);

bool AI_ExpertOverheat(BattleContext *bc, AiContext *ac);

bool AI_ExpertChargeTurnNoInvuln(BattleContext *bc, AiContext *ac);

bool AI_ExpertSunnyDay(BattleContext *bc, AiContext *ac);

bool AI_ExpertUTurn(BattleContext *bc, AiContext *ac);

bool AI_ExpertStealthRock(BattleContext *bc, AiContext *ac);
#endif /*TR_AI_H*/