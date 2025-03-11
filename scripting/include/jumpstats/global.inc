#define NSTRAFES 14
#define NDUCKS 100
#define NJUMPS 100

//new bool:g_bDebugMode;

enum _:ChatDist {
	good = 0, pro, holy, leet, god
};

new const g_szSounds[ChatDist][] = {
    "jumpstats/impressive.wav",
    "jumpstats/wickedsick.wav",
	"jumpstats/luche.wav",
	"jumpstats/ne_very.wav",
	"jumpstats/vlastb.wav"
}

enum JumpType {
	jt_LongJump = 0,
	jt_HighJump = 1,
	jt_CountJump = 2,
	jt_Multi_CountJump = 3,
	jt_StandUPCountJump = 4,
	jt_Multi_StandUPCountJump = 5,
	jt_BhopJump = 6,
	jt_StandUpBhopJump = 7,
	jt_DuckBhopJump = 8,
	jt_WeirdJump = 9,
	jt_LadderJump = 10,
	jt_BhopInDuck = 11,
	jt_Not = 12
};


new JumpType:g_eWhichJump[MAX_PLAYERS + 1];

enum _:JUMPSTATS {
	Float:js_flDistance,
	js_iJumpBlock,
	Float:js_flEndSpeed,
	js_iFrames,
	Float:js_flJof,
	Float:js_flLanding
};

new g_eJumpstats[MAX_PLAYERS + 1][JUMPSTATS];

enum _:PRESTATS {
	ptSideWays,
	ptBackWards,
	ptLadderBhop
};

new bool:g_ePreStats[MAX_PLAYERS + 1][PRESTATS];

enum BUTTONS_S {
	bi_not = 0, bi_W, bi_A, bi_S, bi_D
};

new g_szStrButtonsName[BUTTONS_S][] = { "err", "W", "A", "S", "D" };

enum _:StrafeStats {
	Float:st_flSpeed,
	Float:st_flSpeedFail,
	st_iFrameGood,
	st_iFrameBad,
	st_iFrame,
	BUTTONS_S:st_iButton
};

new g_eStrafeStats[MAX_PLAYERS + 1][NSTRAFES][StrafeStats];
new g_iStrafes[MAX_PLAYERS + 1];
new g_iStrAllEff[MAX_PLAYERS + 1];
new Float:g_flStrGain[MAX_PLAYERS + 1];

enum PLAYER_VER {
	IS_MIDDLE,
	IS_UPPED,
	IS_DROPPED
};

new PLAYER_VER:g_iVerInfo[MAX_PLAYERS + 1];
new Float:g_flDuckFirstZ[MAX_PLAYERS + 1];
new Float:g_flJumpFirstZ[MAX_PLAYERS + 1];

enum DUCK_DATA {
	IS_DUCK_NOT = 0,
	IS_DUCK,
	IS_DD,
	IS_SGS
};

new DUCK_DATA:g_eDuckType[MAX_PLAYERS + 1];

enum _:STATS_DUCKS {
	Float:DUCK_PRE,
	DUCK_FOG
};

new g_eDuckData[MAX_PLAYERS + 1][NDUCKS][STATS_DUCKS];
new g_iDucks[MAX_PLAYERS + 1];

enum JUMP_DATA {
	IS_JUMP_NOT = 0,
	IS_JUMP,
	IS_BHJ,
	IS_DUCKBHOP,
	IS_SBJ
};

new JUMP_DATA:g_eJumpType[MAX_PLAYERS + 1];

enum _:STATS_JUMPS {
	Float:JUMP_PRE,
	JUMP_FOG
};

new g_eJumpData[MAX_PLAYERS + 1][NJUMPS][STATS_JUMPS];
new g_iJumps[MAX_PLAYERS + 1];

enum FAIL_JUMP {
	fj_good = 0,
	fj_fail,
	fj_notshow
};

new FAIL_JUMP:g_eFailJump[MAX_PLAYERS + 1];

new bool:g_isOldGround[MAX_PLAYERS + 1];

new g_iPrevButtons[MAX_PLAYERS + 1];
new g_iOldButtons[MAX_PLAYERS + 1];

new g_iFog[MAX_PLAYERS + 1]

new Float:g_flOrigin[MAX_PLAYERS + 1][3];
new Float:g_flPrevOrigin[MAX_PLAYERS + 1][3];
new Float:g_flFirstJump[MAX_PLAYERS + 1][3];

new Float:g_flVelocity[MAX_PLAYERS + 1][3];
new Float:g_flPrevVelocity[MAX_PLAYERS + 1][3];

new Float:g_flHorSpeed[MAX_PLAYERS + 1];
new Float:g_flPrevHorSpeed[MAX_PLAYERS + 1];
new Float:g_flPreHorSpeed[MAX_PLAYERS + 1];
new Float:g_flStartSpeed[MAX_PLAYERS + 1];

new bool:g_bInDuck[MAX_PLAYERS + 1];
new bool:g_bPrevInDuck[MAX_PLAYERS + 1];

new bool:g_bPrevLadder[MAX_PLAYERS + 1];
new bool:g_isLadderBhop[MAX_PLAYERS + 1];

new Float:g_flLadderXYZ[MAX_PLAYERS + 1][3];
new Float:g_flLadderSize[MAX_PLAYERS + 1][3];
new Float:g_flLadderDist[MAX_PLAYERS + 1];

new g_iOldStrButtons[MAX_PLAYERS + 1];
new Float:g_flStrOldAngle[MAX_PLAYERS + 1];

new bool:g_isFalling[MAX_PLAYERS + 1];
new bool:g_isTouched[MAX_PLAYERS + 1];

new g_iDetectHJ[MAX_PLAYERS + 1];

new g_isUserSpec[MAX_PLAYERS + 1];

new g_hudStrafe;
new g_hudStats;
new g_hudPreSpeed;

new bool:g_bOneReset[MAX_PLAYERS + 1];