enum _:InfoNames {
	info_hud,
	info_chat,
	info_console
};

new const g_eNameJumps[JumpType][InfoNames][] =  {
	{ "lj",		"LJ",	"Long jump" },
	{ "hj",		"HJ",	"High jump" },
	{ "cj",		"CJ", 	"Count jump" },
	{ "mcj",	"MCJ",	"Multi count jump" },
	{ "scj",	"SCJ",	"stand-up count jump" },
	{ "mscj",	"MSCJ",	"Multi stand-up count jump" },
	{ "bj",		"BJ",	"Bhop jump" },
	{ "sbj",	"SBJ",	"Stand up bhop jump" },
	{ "duckbhop", "Duck BJ", "Duck Bhop jump" },
	{ "wj",		"WJ",	"Weird jump" },
	{ "ldj",	"LDJ",	"Ladder jump" },
	{ "bid",	"BID",	"Bhop in duck" },
	{ 0, 0, 0 },
};

new const g_VerAir[PLAYER_VER][InfoNames][] =  {
	{ "", "", ""},
	{ "up ", "Up ", "Up "},
	{ "drop ", "Drop ", "Drop "}
};

public ready_jumps(id, Float:flEndJump[3]) {
	if (g_eWhichJump[id] == jt_LongJump && g_iDetectHJ[id] == 1) {
		g_eWhichJump[id] = jt_HighJump;
	} else if (g_isFalling[id]) {
		g_eWhichJump[id] = jt_WeirdJump;
	}

	new Float:flOrigin[3];
	flOrigin = flEndJump;

	flOrigin[2] = g_bInDuck[id] ? (flOrigin[2] += 18.0) : flOrigin[2];

	if (g_eWhichJump[id] == jt_LadderJump) {
		g_eJumpstats[id][js_flDistance] = get_distance_f(g_flFirstJump[id], flOrigin) + g_flLadderDist[id];
	} else {
		g_eJumpstats[id][js_flDistance] = get_distance_f(g_flFirstJump[id], flOrigin) + 32.0;
	}

	if (g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_flDistance] > 64.0) {
		g_eJumpstats[id][js_flLanding] = g_eJumpstats[id][js_flDistance] - (g_eJumpstats[id][js_iJumpBlock] + g_eJumpstats[id][js_flJof]);
	}

	if (!g_pCvar[c_iNoslow] && get_entvar(id, var_fuser2) == 0.0 && g_eWhichJump[id] != jt_LadderJump) {
		g_eFailJump[id] = fj_notshow;
	}

	switch(g_eWhichJump[id]) {
		case jt_LongJump, jt_HighJump, jt_CountJump, jt_Multi_CountJump, jt_StandUPCountJump, jt_Multi_StandUPCountJump, jt_WeirdJump: {
			if (g_eJumpstats[id][js_flDistance] < 200.0)
				g_eFailJump[id] = fj_notshow;
		}
		case jt_BhopJump, jt_StandUpBhopJump, jt_DuckBhopJump, jt_LadderJump, jt_BhopInDuck: {
			if (g_eJumpstats[id][js_flDistance] < 120.0)
				g_eFailJump[id] = fj_notshow;
		}
	}

	if (g_eJumpstats[id][js_flDistance] > 290.0)
		g_eFailJump[id] = fj_notshow;

	if (g_isTouched[id]) {
		g_eFailJump[id] = fj_fail;
	}

	calc_strafe(id);

	if (g_eFailJump[id] != fj_notshow) {

		if (g_pCvar[c_iEnableStrafe])
			show_strafe_hud(id);

		if (g_pCvar[c_iEnableStats])
			show_stats_hud(id);

		if (g_pCvar[c_iEnableConsole])
			show_console(id);

		if (g_eFailJump[id] == fj_good) {
			if (g_pCvar[c_iEnableChat])
				show_chat(id);

			if (g_pCvar[c_iEnableSound])
				sound_jumps(id);
		}
	}

	g_eWhichJump[id] = jt_Not;

	arrayset(g_eJumpstats[id], 0, JUMPSTATS);
	g_eJumpstats[id][js_iJumpBlock] = 1000;

	g_ePreStats[id][ptLadderBhop] = false;
	g_ePreStats[id][ptSideWays] = false;

	reset_strafes(id);

	g_eDuckType[id] = IS_DUCK_NOT;
	for (new i = 0; i < NDUCKS; i++) {
   		arrayset(g_eDuckData[id][i], 0, STATS_DUCKS);
	}
	g_iDucks[id] = 0;
	g_flDuckFirstZ[id] = 0.0;

	g_eJumpType[id] = IS_JUMP;

	g_eFailJump[id] = fj_good;
	g_eWhichJump[id] = jt_Not
	
	g_isFalling[id] = false;
	g_isTouched[id] = false;

	g_iDetectHJ[id] = 0;
	g_flStartSpeed[id] = 0.0;
}

public calc_strafe(id) {
	new iSumAD, iSumWS;

	new iStrFrameBad;
	new iStrFrameGood;


	for (new str = 1; str <= g_iStrafes[id]; str++) {
		iStrFrameGood += g_eStrafeStats[id][str][st_iFrameGood];
		iStrFrameBad += g_eStrafeStats[id][str][st_iFrameBad];
		g_flStrGain[id] += g_eStrafeStats[id][str][st_flSpeed];

		if (g_eStrafeStats[id][str][st_flSpeedFail] > 200.0) {
			g_eFailJump[id] = fj_fail;
		}

		if (g_eStrafeStats[id][str][st_iButton] == bi_A || g_eStrafeStats[id][str][st_iButton] == bi_D)
			iSumAD++;
		if (g_eStrafeStats[id][str][st_iButton] == bi_W || g_eStrafeStats[id][str][st_iButton] == bi_S)
			iSumWS++;
	}

	if (iStrFrameGood)
		g_iStrAllEff[id] = (iStrFrameGood * 100 / (iStrFrameGood + iStrFrameBad));

	if (iSumWS > iSumAD) {
		g_ePreStats[id][ptSideWays] = true;
	}
}

public show_strafe_hud(id) {
	new szStrMess[512];
	new iLen;

	for (new str = 1; str <= g_iStrafes[id]; str++) {
		
		new iEffPrecent;
		if ((g_eStrafeStats[id][str][st_iFrameGood] + g_eStrafeStats[id][str][st_iFrameBad]) > 0) {
			iEffPrecent = (g_eStrafeStats[id][str][st_iFrameGood] * 100) / (g_eStrafeStats[id][str][st_iFrameGood] + g_eStrafeStats[id][str][st_iFrameBad]);
		} else {
			iEffPrecent = 0;
		}

		iLen += format(szStrMess[iLen], sizeof szStrMess - iLen, "^t%s  %4.1f  %4.1f  %d  %d%%%%^n", g_szStrButtonsName[g_eStrafeStats[id][str][st_iButton]], g_eStrafeStats[id][str][st_flSpeed], g_eStrafeStats[id][str][st_flSpeedFail], g_eStrafeStats[id][str][st_iFrame], iEffPrecent);
	}

	if (g_iStrafes[id] != 0) {
		iLen += format(szStrMess[iLen], sizeof szStrMess - iLen, "^t    (%4.1f)^t^t (%d%%%%)^n", g_flStrGain[id], g_iStrAllEff[id]);
		if (g_eWhichJump[id] == jt_LadderJump && g_eStrafeStats[id][0][st_flSpeed] != 0) {
			iLen += format(szStrMess[iLen], sizeof szStrMess - iLen, "^t^tturn speed: %4.3f", g_eStrafeStats[id][0][st_flSpeed]);
		}
		if (!g_eFailJump[id] && g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_iJumpBlock] < 290.0 && g_eJumpstats[id][js_iJumpBlock] > 200.0 && g_eJumpstats[id][js_flJof] < 100.0 && g_eJumpstats[id][js_flJof] != 0.0 && (g_eJumpstats[id][js_iJumpBlock] + g_eJumpstats[id][js_flJof]) < g_eJumpstats[id][js_flDistance]) {
			if ((g_eJumpstats[id][js_iJumpBlock] + g_eJumpstats[id][js_flLanding] + g_eJumpstats[id][js_flJof]) > (g_eJumpstats[id][js_flDistance] + 10.0) || g_eJumpstats[id][js_flLanding] <= 0.0) {
				iLen += format(szStrMess[iLen], sizeof szStrMess - iLen, "^n^tblock: %d^n^tJof: %.f", g_eJumpstats[id][js_iJumpBlock], g_eJumpstats[id][js_flJof]);
			} else {
				iLen += format(szStrMess[iLen], sizeof szStrMess - iLen, "^n^tblock: %d^n^tJof: %.f	land: %.f", g_eJumpstats[id][js_iJumpBlock], g_eJumpstats[id][js_flJof], g_eJumpstats[id][js_flLanding]);
			}
		} else if (g_eFailJump[id] && g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_iJumpBlock] < 290.0 && g_eJumpstats[id][js_iJumpBlock] > 200.0 && g_eJumpstats[id][js_flJof] < 100.0 && g_eJumpstats[id][js_flJof] != 0.0) {
			iLen += format(szStrMess[iLen], sizeof szStrMess - iLen, "^n^tblock: %d^n^tJof: %.f", g_eJumpstats[id][js_iJumpBlock], g_eJumpstats[id][js_flJof]);
		} else if (g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_flJof] < 100.0 && g_eJumpstats[id][js_flJof] != 0.0) {
			iLen += format(szStrMess[iLen], sizeof szStrMess - iLen, "^n^tJof: %.f", g_eJumpstats[id][js_flJof]);
		} 

		if (g_isTouched[id]) {
			iLen += format(szStrMess[iLen], sizeof szStrMess - iLen, "^n^ttouch");
		}
	}

	if (!g_eFailJump[id]) {
		set_hudmessage(g_pCvar[c_iStatsR], g_pCvar[c_iStatsG], g_pCvar[c_iStatsB], g_pCvar[c_flStrafeX], g_pCvar[c_flStrafeY], 0, 6.0, 2.5, 0.1, 0.3, g_pCvar[c_iStrafe]);
	} else {
		set_hudmessage(g_pCvar[c_iFailR], g_pCvar[c_iFailG], g_pCvar[c_iFailB], g_pCvar[c_flStrafeX], g_pCvar[c_flStrafeY], 0, 6.0, 2.5, 0.1, 0.3, g_pCvar[c_iStrafe]);
	}

	for (new i = 1; i < MaxClients; i++) {
		if ((i == id || g_isUserSpec[i] == id) && g_eOnOff[id][of_bStrafe] == true) {
			ShowSyncHudMsg(i, g_hudStrafe, szStrMess);
		}
	}
}

public show_stats_hud(id) {
	new szPreMess[64];
	new iLen;

	if (g_ePreStats[id][ptLadderBhop]) {
		iLen += format(szPreMess[iLen], sizeof szPreMess - iLen, "Ladder ");
	}
	
	if (g_ePreStats[id][ptSideWays]) {
		iLen += format(szPreMess[iLen], sizeof szPreMess - iLen, "[sw] ");
	} else if (g_ePreStats[id][ptBackWards]) {
		iLen += format(szPreMess[iLen], sizeof szPreMess - iLen, "[bw] ");
	}

	for (new i = 1; i < MaxClients; i++) {
		if ((i == id || g_isUserSpec[i] == id) && g_eOnOff[i][of_bStats] == true) {

			if (!g_eFailJump[id]) {
				set_hudmessage(g_pCvar[c_iStatsR], g_pCvar[c_iStatsG], g_pCvar[c_iStatsB], g_pCvar[c_flStatsX], g_pCvar[c_flStatsY], 0, 6.0, 2.5, 0.1, 0.3, g_pCvar[c_iStats]);
			} else {
				set_hudmessage(g_pCvar[c_iFailR], g_pCvar[c_iFailG], g_pCvar[c_iFailB], g_pCvar[c_flStatsX], g_pCvar[c_flStatsY], 0, 6.0, 2.5, 0.1, 0.3, g_pCvar[c_iStats]);
			}

			switch (g_eWhichJump[id]) {
				case jt_LongJump, jt_HighJump, jt_LadderJump: {
					ShowSyncHudMsg(i, g_hudStats, "%s%s ^ndist: %.3f^n%s pre: %.1f^nendspeed: %.0f^nstrafes: %d", szPreMess, g_eNameJumps[g_eWhichJump[id]][info_hud], g_eJumpstats[id][js_flDistance], g_eNameJumps[g_eWhichJump[id]][info_hud], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eJumpstats[id][js_flEndSpeed], g_iStrafes[id]);
				}
				case jt_BhopJump, jt_StandUpBhopJump, jt_DuckBhopJump, jt_BhopInDuck, jt_WeirdJump: {
					ShowSyncHudMsg(i, g_hudStats, "%s%s%s ^ndist: %.3f^njump (%d): %.1f^npre: %.1f^nendspeed: %.0f^nstrafes: %d", szPreMess, g_VerAir[g_iVerInfo[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_hud], g_eJumpstats[id][js_flDistance], g_eJumpData[id][g_iJumps[id]][JUMP_FOG], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eJumpData[id][1][JUMP_PRE], g_eJumpstats[id][js_flEndSpeed], g_iStrafes[id]);
				}
				case jt_CountJump, jt_StandUPCountJump: {
					ShowSyncHudMsg(i, g_hudStats, "%s%s%s ^ndist: %.3f^njump (%d): %.1f^nduck: %.1f^nendspeed: %.0f^nstrafes: %d", szPreMess, g_VerAir[g_iVerInfo[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_hud], g_eJumpstats[id][js_flDistance], g_eJumpData[id][g_iJumps[id]][JUMP_FOG], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eDuckData[id][1][DUCK_PRE], g_eJumpstats[id][js_flEndSpeed], g_iStrafes[id]);
				}
				case jt_Multi_CountJump, jt_Multi_StandUPCountJump: {
					if (g_iDucks[id] == 2) {
						ShowSyncHudMsg(i, g_hudStats, "%s%s%s (%d% ducks)^ndist: %.3f^njump (%d): %.1f^ndd (%d): %.1f^nduck: %.1f^nendspeed: %.0f^nstrafes: %d", szPreMess, g_VerAir[g_iVerInfo[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_hud], g_iDucks[id], g_eJumpstats[id][js_flDistance], g_eJumpData[id][g_iJumps[id]][JUMP_FOG], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eDuckData[id][2][DUCK_FOG], g_eDuckData[id][2][DUCK_PRE], g_eDuckData[id][1][DUCK_PRE], g_eJumpstats[id][js_flEndSpeed], g_iStrafes[id]);
					} else if (g_iDucks[id] == 3) {
						ShowSyncHudMsg(i, g_hudStats, "%s%s%s (%d% ducks)^ndist: %.3f^njump (%d): %.1f^ndd (%d): %.1f^ndd (%d): %.1f^nduck: %.1f^nendspeed: %.0f^nstrafes: %d", szPreMess, g_VerAir[g_iVerInfo[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_hud], g_iDucks[id], g_eJumpstats[id][js_flDistance], g_eJumpData[id][g_iJumps[id]][JUMP_FOG], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eDuckData[id][3][DUCK_FOG], g_eDuckData[id][3][DUCK_PRE], g_eDuckData[id][2][DUCK_FOG], g_eDuckData[id][2][DUCK_PRE], g_eDuckData[id][1][DUCK_PRE], g_eJumpstats[id][js_flEndSpeed], g_iStrafes[id]);
					} else {
						ShowSyncHudMsg(i, g_hudStats, "%s%s%s (%d% ducks)^ndist: %.3f^njump (%d): %.1f^ndd (%d): %.1f^n(+%d% dd)^nduck: %.1f^nendspeed: %.0f^nstrafes: %d", szPreMess, g_VerAir[g_iVerInfo[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_hud], g_iDucks[id], g_eJumpstats[id][js_flDistance], g_eJumpData[id][g_iJumps[id]][JUMP_FOG], g_eJumpData[id][g_iJumps[id]][JUMP_PRE],  g_eDuckData[id][g_iDucks[id]][DUCK_FOG], g_eDuckData[id][g_iDucks[id]][DUCK_PRE], g_iDucks[id] - 2, g_eDuckData[id][1][DUCK_PRE], g_eJumpstats[id][js_flEndSpeed], g_iStrafes[id]);
					}
				}
			}
		}
	}
}

public show_console(id) {
	new szPreMess[64];
	new iLen;

	if (g_ePreStats[id][ptLadderBhop]) {
		iLen += format(szPreMess[iLen], sizeof szPreMess - iLen, "Ladder ");
	}
	
	if (g_ePreStats[id][ptSideWays]) {
		iLen += format(szPreMess[iLen], sizeof szPreMess - iLen, "[sw] ");
	} else if (g_ePreStats[id][ptBackWards]) {
		iLen += format(szPreMess[iLen], sizeof szPreMess - iLen, "[bw] ");
	}

	if (g_pCvar[c_iConsoleBug])
		client_print(id, print_console, "^nHNS JumpStats^n");
	else
		client_print(id, print_console, "^nHNS JumpStats");

	switch (g_eWhichJump[id]) {
		case jt_LongJump, jt_HighJump, jt_LadderJump: {
			client_print(id, print_console, "%s (%s)^nDistance: %.3f unit/s^nEndspeed: %.3f unit/s^nPrestrafe: %.0f^nFrames: %d", g_eNameJumps[g_eWhichJump[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_console], g_eJumpstats[id][js_flDistance], g_eJumpstats[id][js_flEndSpeed], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eJumpstats[id][js_iFrames]);
		}
		case jt_BhopJump, jt_StandUpBhopJump, jt_DuckBhopJump, jt_BhopInDuck, jt_WeirdJump: {
			client_print(id, print_console, "%s%s%s (%s)^nDistance: %.3f unit/s^nEndspeed: %.3f unit/s^njump %d FOG: %.0f^nPrestrafe: %.0f^nFrames: %d", szPreMess, g_VerAir[g_iVerInfo[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_console], g_eJumpstats[id][js_flDistance], g_eJumpstats[id][js_flEndSpeed], g_eJumpData[id][g_iJumps[id]][JUMP_FOG], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eJumpData[id][1][JUMP_PRE], g_eJumpstats[id][js_iFrames]);
		}
		case jt_CountJump, jt_StandUPCountJump: {
			client_print(id, print_console, "%s%s%s (%s)^nDistance: %.3f unit/s^nEndspeed: %.3f unit/s^njump %d FOG: %.0f^nDuck pre: %.0f^nFrames: %d", szPreMess, g_VerAir[g_iVerInfo[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_console], g_eJumpstats[id][js_flDistance], g_eJumpstats[id][js_flEndSpeed], g_eJumpData[id][g_iJumps[id]][JUMP_FOG], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eDuckData[id][1][DUCK_PRE], g_eJumpstats[id][js_iFrames]);
		}
		case jt_Multi_CountJump, jt_Multi_StandUPCountJump: {
			if (g_iDucks[id] == 2) {
				client_print(id, print_console, "%s%s%s (%s)^nDistance: %.3f unit/s^nEndspeed: %.3f unit/s^njump %d FOG: %.0f^nDuck pre: %.0f^nDucks: %d%^nFrames: %d", szPreMess, g_VerAir[g_iVerInfo[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_console], g_eJumpstats[id][js_flDistance], g_eJumpstats[id][js_flEndSpeed], g_eJumpData[id][g_iJumps[id]][JUMP_FOG], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eDuckData[id][1][DUCK_PRE], g_iDucks[id], g_eJumpstats[id][js_iFrames]);	
			} else {
				client_print(id, print_console, "%s%s%s (%s)^nDistance: %.3f unit/s^nEndspeed: %.3f unit/s^nPrestrafe: (%.0f) %.0f^nDucks: %d%^nFrames: %d", szPreMess, g_VerAir[g_iVerInfo[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_hud], g_eNameJumps[g_eWhichJump[id]][info_console], g_eJumpstats[id][js_flDistance], g_eJumpstats[id][js_flEndSpeed], g_eDuckData[id][1][DUCK_PRE], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_iDucks[id], g_eJumpstats[id][js_iFrames]);
			}
		}
	}

	client_print(id, print_console, "^n#N button Gain-loss Loss Frame Efficiency");

	for (new str = 1; str <= g_iStrafes[id]; str++) {
		new iEffPrecent;
		if ((g_eStrafeStats[id][str][st_iFrameGood] + g_eStrafeStats[id][str][st_iFrameBad]) > 0) {
			iEffPrecent = (g_eStrafeStats[id][str][st_iFrameGood] * 100) / (g_eStrafeStats[id][str][st_iFrameGood] + g_eStrafeStats[id][str][st_iFrameBad]);
		} else {
			iEffPrecent = 0;
		}

		if (g_pCvar[c_iConsoleBug])
			client_print(id, print_console, "^n%2d.  %s  %4.1f  %4.1f  %d  %3d%", str, g_szStrButtonsName[g_eStrafeStats[id][str][st_iButton]], g_eStrafeStats[id][str][st_flSpeed], g_eStrafeStats[id][str][st_flSpeedFail], g_eStrafeStats[id][str][st_iFrame], iEffPrecent);
		else
			client_print(id, print_console, "%2d.  %s  %4.1f  %4.1f  %d  %3d%", str, g_szStrButtonsName[g_eStrafeStats[id][str][st_iButton]], g_eStrafeStats[id][str][st_flSpeed], g_eStrafeStats[id][str][st_flSpeedFail], g_eStrafeStats[id][str][st_iFrame], iEffPrecent);
	}

	client_print(id, print_console, "^nTotal gain-loss: %4.1f / Efficiency: %d%%", g_flStrGain[id], g_iStrAllEff[id]);

	if (g_eWhichJump[id] == jt_LadderJump && g_eStrafeStats[id][0][st_flSpeed] != 0) {
		client_print(id, print_console, "^nTurn speed: %.f", g_eStrafeStats[id][0][st_flSpeed]);
	} else if (g_eWhichJump[id] == jt_Multi_CountJump || g_eWhichJump[id] == jt_Multi_StandUPCountJump) {
		client_print(id, print_console, "^nPre duck: %.f", g_eDuckData[id][1][DUCK_PRE]);
		for (new duck = 2; duck <= g_iDucks[id]; duck++) {
			if (g_pCvar[c_iConsoleBug])
				client_print(id, print_console, "^nPre dd#%d: %.f, FOG(%d)", duck - 1, g_eDuckData[id][duck][DUCK_PRE], g_eDuckData[id][duck][DUCK_FOG]);
			else
				client_print(id, print_console, "Pre dd#%d: %.f, FOG(%d)", duck - 1, g_eDuckData[id][duck][DUCK_PRE], g_eDuckData[id][duck][DUCK_FOG]);
		}

		if (g_pCvar[c_iConsoleBug])
			client_print(id, print_console, "^nPre jump: %.f, FOG(%d)", g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eJumpData[id][g_iJumps[id]][JUMP_FOG]);
		else
			client_print(id, print_console, "Pre jump: %.f, FOG(%d)", g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_eJumpData[id][g_iJumps[id]][JUMP_FOG]);

	}
	
	if (g_isTouched[id]) {
		client_print(id, print_console, "^nProbably touched something");
	}
	if (!g_eFailJump[id] && g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_iJumpBlock] < 290.0 && g_eJumpstats[id][js_iJumpBlock] > 200.0 && g_eJumpstats[id][js_flJof] < 100.0 && g_eJumpstats[id][js_flJof] != 0.0 && (g_eJumpstats[id][js_iJumpBlock] + g_eJumpstats[id][js_flJof]) < g_eJumpstats[id][js_flDistance]) {
		if ((g_eJumpstats[id][js_iJumpBlock] + g_eJumpstats[id][js_flLanding] + g_eJumpstats[id][js_flJof]) > (g_eJumpstats[id][js_flDistance] + 10.0) || g_eJumpstats[id][js_flLanding] <= 0.0)
			client_print(id, print_console, "^nBlock: %d / Jumpoff: %.f", g_eJumpstats[id][js_iJumpBlock], g_eJumpstats[id][js_flJof]);
		else
			client_print(id, print_console, "^nBlock: %d / Jumpoff: %.f / Landing: %.f", g_eJumpstats[id][js_iJumpBlock], g_eJumpstats[id][js_flJof], g_eJumpstats[id][js_flLanding]);
	} else if (g_eFailJump[id] && g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_iJumpBlock] < 290.0 && g_eJumpstats[id][js_iJumpBlock] > 200.0 && g_eJumpstats[id][js_flJof] < 100.0 && g_eJumpstats[id][js_flJof] != 0.0 && (g_eJumpstats[id][js_iJumpBlock] + g_eJumpstats[id][js_flJof]) < g_eJumpstats[id][js_flDistance]) {
		client_print(id, print_console, "^nFail Block: %d / Jumpoff: %.f", g_eJumpstats[id][js_iJumpBlock], g_eJumpstats[id][js_flJof]);
	} else if (g_eFailJump[id] && g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_flJof] < 100.0 && g_eJumpstats[id][js_flJof] != 0.0 && g_eJumpstats[id][js_iJumpBlock] < 290.0 && g_eJumpstats[id][js_iJumpBlock] > 200.0) {
		client_print(id, print_console, "^nFail Block: %d / Jumpoff: %.f", g_eJumpstats[id][js_iJumpBlock], g_eJumpstats[id][js_flJof]);
	} else if (g_eFailJump[id] && g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_flJof] < 100.0 && g_eJumpstats[id][js_flJof] != 0.0) {
		client_print(id, print_console, "^nFail Jumpoff: %.f", g_eJumpstats[id][js_flJof]);
	} else if (!g_eFailJump[id] && g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_flJof] < 100.0 && g_eJumpstats[id][js_flJof] != 0.0) {
		client_print(id, print_console, "^nJumpoff: %.f", g_eJumpstats[id][js_flJof]);
	}
}

public show_chat(id) {
	new aDistance[ChatDist];
	get_colorchat_by_distance(id, aDistance, g_eWhichJump[id]);

	new iColor;

	if (g_eJumpstats[id][js_flDistance] >= aDistance[god]) {
		iColor = god;
	} else if (g_eJumpstats[id][js_flDistance] >= aDistance[leet]) {
		iColor = leet;
	} else if (g_eJumpstats[id][js_flDistance] >= aDistance[holy]) {
		iColor = holy;
	} else if (g_eJumpstats[id][js_flDistance] >= aDistance[pro]) {
		iColor = pro;
	} else if (g_eJumpstats[id][js_flDistance] >= aDistance[good]) {
		iColor = good;
	} else {
		return;
	}

	new szJumpInfoMess[128];
	new iLen;

	if (g_eWhichJump[id] == jt_Multi_CountJump || g_eWhichJump[id] == jt_Multi_StandUPCountJump) {
		if (iColor == holy || iColor == god)
			iLen += format(szJumpInfoMess[iLen], sizeof szJumpInfoMess - iLen, " (^4%d%s ducks)", g_iDucks[id], iColor == god ? "^3" : "^1");
		else
			iLen += format(szJumpInfoMess[iLen], sizeof szJumpInfoMess - iLen, " (^3%d^1 ducks)", g_iDucks[id]);
	}

	if (g_eWhichJump[id] != jt_LadderJump && g_eJumpstats[id][js_iJumpBlock] < 290.0 && g_eJumpstats[id][js_iJumpBlock] > 200.0 && g_eJumpstats[id][js_flJof] < 100.0 && g_eJumpstats[id][js_flJof] != 0.0 && (g_eJumpstats[id][js_iJumpBlock] + g_eJumpstats[id][js_flJof]) < g_eJumpstats[id][js_flDistance]) {
		if (iColor == holy || iColor == god)
			iLen += format(szJumpInfoMess[iLen], sizeof szJumpInfoMess - iLen, " (block: ^4%d%s)", g_eJumpstats[id][js_iJumpBlock], iColor == god ? "^3" : "^1");
		else
			iLen += format(szJumpInfoMess[iLen], sizeof szJumpInfoMess - iLen, " (block: ^3%d^1)", g_eJumpstats[id][js_iJumpBlock]);
	}
	
	if (g_ePreStats[id][ptSideWays]) {
		if (iColor == holy || iColor == god)
			iLen += format(szJumpInfoMess[iLen], sizeof szJumpInfoMess - iLen, " (^4SideWays%s)", iColor == god ? "^3" : "^1");
		else
			iLen += format(szJumpInfoMess[iLen], sizeof szJumpInfoMess - iLen, " (^3SideWays^1)");
	} else if (g_ePreStats[id][ptBackWards]) {
		if (iColor == holy || iColor == god)
			iLen += format(szJumpInfoMess[iLen], sizeof szJumpInfoMess - iLen, " (^4Backwards%s)", iColor == god ? "^3" : "^1");
		else
			iLen += format(szJumpInfoMess[iLen], sizeof szJumpInfoMess - iLen, " (^3Backwards^1)");
	}

	new iPlayers[MAX_PLAYERS], iNum;
	get_players(iPlayers, iNum, "ch");

	for (new i = 0; i < iNum; i++) {
		new iPlayer = iPlayers[i];
		if (!g_eOnOff[iPlayer][of_bChatInfo])
			continue;

		switch(iColor) {
			case god:	client_print_color(iPlayer, print_team_red,	"^4%n^3 jumped ^4%.0f^3 with ^4%s%s^3 (pre:^4%.0f^3 str:^4%i^3 sync: ^4%i%%^3)%s", id, g_eJumpstats[id][js_flDistance], g_ePreStats[id][ptLadderBhop] ? "Ladder " : "", g_eNameJumps[g_eWhichJump[id]][info_chat], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_iStrafes[id], g_iStrAllEff[id], szJumpInfoMess);
			case leet:	client_print_color(iPlayer, print_team_red, "^3%n^1 jumped ^3%.0f^1 with ^3%s%s^1 (pre:^3%.0f^1 str:^3%i^1 sync: ^3%i%%^1)%s", id, g_eJumpstats[id][js_flDistance], g_ePreStats[id][ptLadderBhop] ? "Ladder " : "", g_eNameJumps[g_eWhichJump[id]][info_chat], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_iStrafes[id], g_iStrAllEff[id], szJumpInfoMess);
			case holy:	client_print_color(iPlayer, print_team_blue,"^4%n^1 jumped ^4%.0f^1 with ^4%s%s^1 (pre:^4%.0f^1 str:^4%i^1 sync: ^4%i%%^1)%s", id, g_eJumpstats[id][js_flDistance], g_ePreStats[id][ptLadderBhop] ? "Ladder " : "", g_eNameJumps[g_eWhichJump[id]][info_chat], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_iStrafes[id], g_iStrAllEff[id], szJumpInfoMess);
			case pro:	client_print_color(iPlayer, print_team_blue,"^3%n^1 jumped ^3%.0f^1 with ^3%s%s^1 (pre:^3%.0f^1 str:^3%i^1 sync: ^3%i%%^1)%s", id, g_eJumpstats[id][js_flDistance], g_ePreStats[id][ptLadderBhop] ? "Ladder " : "", g_eNameJumps[g_eWhichJump[id]][info_chat], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_iStrafes[id], g_iStrAllEff[id], szJumpInfoMess);
			case good:	client_print_color(iPlayer, print_team_grey,"^3%n^1 jumped ^3%.0f^1 with ^3%s%s^1 (pre:^3%.0f^1 str:^3%i^1 sync: ^3%i%%^1)%s", id, g_eJumpstats[id][js_flDistance], g_ePreStats[id][ptLadderBhop] ? "Ladder " : "", g_eNameJumps[g_eWhichJump[id]][info_chat], g_eJumpData[id][g_iJumps[id]][JUMP_PRE], g_iStrafes[id], g_iStrAllEff[id], szJumpInfoMess);

		}
	}
}

public sound_jumps(id) {
	new aDistance[ChatDist];
	get_colorchat_by_distance(id, aDistance, g_eWhichJump[id]);

	new iPlayers[MAX_PLAYERS], iNum;
	get_players(iPlayers, iNum, "ch");

	for (new i = 0; i < iNum; i++) {
		new iPlayer = iPlayers[i];
		if (!g_eOnOff[iPlayer][of_bSound])
			continue;

		if (g_eJumpstats[id][js_flDistance] >= aDistance[god]) {
			client_cmd(iPlayer, "spk %s", g_szSounds[god]);
		} else if (g_eJumpstats[id][js_flDistance] >= aDistance[leet]) {
			client_cmd(iPlayer, "spk %s", g_szSounds[leet]);
		} else if (g_eJumpstats[id][js_flDistance] >= aDistance[holy]) {
			client_cmd(iPlayer, "spk %s", g_szSounds[holy]);
		} else if (g_eJumpstats[id][js_flDistance] >= aDistance[pro]) {
			client_cmd(id, "spk %s", g_szSounds[pro]);
			break;
		} else if (g_eJumpstats[id][js_flDistance] >= aDistance[good]) {
			client_cmd(id, "spk %s", g_szSounds[good]);
			break;
		}
	}
}