public in_bhop_js(id, iFog) {
	// PM_Jump snapshots exclude the first air acceleration and include
	// any hull/origin change performed by PM_Duck before the jump.
	g_flFirstJump[id] = g_flPmJumpOrigin[id];
	new Float:flPreHorSpeed = g_flPmJumpPreSpeed[id];
	
	if (iFog > 10) {
		//ready_moves(id);

		g_iJumps[id] = 0;
		g_eJumpData[id][g_iJumps[id]][JUMP_PRE] = flPreHorSpeed;
		g_eJumpData[id][g_iJumps[id]][JUMP_FOG] = iFog;
		g_iVerInfo[id] = IS_MIDDLE;
		
		if (g_bPmJumpDuck[id]) {
			g_eJumpType[id] = IS_DUCKBHOP;
			g_flFirstJump[id][2] = g_flFirstJump[id][2] + 18.0;
			g_flJumpFirstZ[id] = g_flPmJumpOrigin[id][2] + 18.0;
		} else {
			g_eJumpType[id] = IS_JUMP;
			g_eWhichJump[id] = jt_LongJump;
			g_flJumpFirstZ[id] = g_flPmJumpOrigin[id][2];
		}

		show_pre(id, PRE_JUMP, flPreHorSpeed);
		// ЖАМП
	} else {
		g_iJumps[id]++;

		if (!g_flJumpFirstZ[id])
			g_flJumpFirstZ[id] = g_bPmJumpDuck[id] ? g_flPmJumpOrigin[id][2] + 18.0 : g_flPmJumpOrigin[id][2];

		if (g_iJumps[id] >= NJUMPS) {
			g_iJumps[id] = 2;
			set_hudmessage(
				g_eSettings[id][S_STATS_R], g_eSettings[id][S_STATS_G], g_eSettings[id][S_STATS_B],
				g_eSettings[id][S_STATS_X], g_eSettings[id][S_STATS_Y], 0, 6.0, 2.5, 0.1, 0.3, g_pCvar[c_iStats]);
			ShowSyncHudMsg(id, g_hudStats, "Reset jumps!");
		}

		g_eJumpData[id][g_iJumps[id]][JUMP_PRE] = flPreHorSpeed;
		g_eJumpData[id][g_iJumps[id]][JUMP_FOG] = iFog;

		g_flFirstJump[id][2] = g_bPmJumpDuck[id] ? g_flFirstJump[id][2] + 18.0 : g_flFirstJump[id][2];

		new FOG_TYPE:iFogType;

		new Float:flMaxHorSpeed;
		flMaxHorSpeed = Float:get_pmove(pm_maxspeed);
		flMaxHorSpeed = floatmul(1.2, flMaxHorSpeed);
		flMaxHorSpeed = floatsqroot(floatmax(0.0, floatmul(flMaxHorSpeed, flMaxHorSpeed) - 16.0));

		new Float:flPostHorSpeed = g_flPmJumpPostSpeed[id];

		if (flPostHorSpeed < flPreHorSpeed - 0.01) {
			iFogType = FOG_BAD;
		} else if (iFog == 1) {
			iFogType = FOG_PERFECT;
		} else if (g_flOldHorSpeed[id] > flMaxHorSpeed) {
			iFogType = FOG_PERFECT;
		}

		if (!iFogType) {
			switch(iFog) {
				case 1..2: iFogType = FOG_GOOD;
				case 3: iFogType = FOG_BAD;
				default: iFogType = FOG_VERYBAD;
			}
		}


		if (g_eDuckType[id] != IS_DUCK_NOT && g_iDucks[id]) {
			if (g_iVerInfo[id] == IS_MIDDLE && !g_isFalling[id]) {
				new Float:flDuckZ = g_bPmJumpDuck[id] ? g_flPmJumpOrigin[id][2] + 18.0 : g_flPmJumpOrigin[id][2];

				if (flDuckZ - g_flDuckFirstZ[id] < -4.0) {
					g_iVerInfo[id] = IS_DROPPED;
				} else if (flDuckZ - g_flDuckFirstZ[id] > 4.0) {
					g_iVerInfo[id] = IS_UPPED;
				}
			}

			g_isFalling[id] =  false;

			if (g_eDuckType[id] == IS_SGS) {
				// Жамп после сгса
				g_eWhichJump[id] = g_iDucks[id] == 1 ? jt_StandUPCountJump : jt_Multi_StandUPCountJump;
				
				show_pre(id, g_eJumpType[id] == IS_SBJ ? PRE_SBJ : PRE_BHOP, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
			} else if (g_eDuckType[id] == IS_DD || g_eDuckType[id] == IS_DUCK) {
				// Жамп после ддрана
				g_eWhichJump[id] = g_iDucks[id] == 1 ? jt_CountJump : jt_Multi_CountJump;

				show_pre(id, g_eJumpType[id] == IS_SBJ ? PRE_SBJ : PRE_BHOP, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
			}

			g_eJumpType[id] = IS_BHJ;
		} else {
			if (g_eJumpType[id] != IS_JUMP_NOT) {
				if (g_iJumps[id] <= 1) {
					if (g_eJumpType[id] == IS_DUCKBHOP) {
						g_eWhichJump[id] = jt_DuckBhopJump;
						show_pre(id, PRE_DUCKBHOP, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
					} else if (g_bInDuck[id]) {
						g_eWhichJump[id] = jt_BhopInDuck;
						show_pre(id, PRE_BID, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
					} else if (g_eJumpType[id] == IS_SBJ) {
						g_eWhichJump[id] = jt_StandUpBhopJump;
						show_pre(id, PRE_SBJ, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
					} else if (g_eJumpType[id] == IS_JUMP) {
						g_eWhichJump[id] = jt_BhopJump;
						show_pre(id, PRE_BHOP, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
					}

					if (g_iVerInfo[id] == IS_MIDDLE && !g_isLadderBhop[id] && !g_isFalling[id]) {
						new Float:flJumpZ = g_bPmJumpDuck[id] ? g_flPmJumpOrigin[id][2] + 18.0 : g_flPmJumpOrigin[id][2];

						if (flJumpZ - g_flJumpFirstZ[id] < -4.0) {
							g_iVerInfo[id] = IS_DROPPED;
						} else if (flJumpZ - g_flJumpFirstZ[id] > 4.0) {
							g_iVerInfo[id] = IS_UPPED;
						}
					} else if (g_isLadderBhop[id]) {
						g_ePreStats[id][ptLadderBhop] = true;
						g_isLadderBhop[id] = false;
						g_iVerInfo[id] = IS_MIDDLE;
					}
				} else {
					if (g_eSettings[id][S_BHOP_FOGSTATS]) {
						g_eWhichJump[id] = jt_MultiBhopJump;
						if (g_bDebugMode) client_print(id, print_chat, "jump %d", g_iJumps[id])
					}

					if (g_eJumpType[id] == IS_DUCKBHOP) {
						show_pre(id, PRE_DUCKBHOP, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
					} else if (g_bInDuck[id]) {
						show_pre(id, PRE_BID, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
					} else if (g_eJumpType[id] == IS_SBJ) {
						show_pre(id, PRE_SBJ, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
					} else if (g_eJumpType[id] == IS_BHJ) {
						show_pre(id, PRE_BHOP, g_flPreHorSpeed[id], iFog, flPostHorSpeed, iFogType);
					}
				}
				g_eJumpType[id] = IS_BHJ;
			}
		}
		// БХОП
	}
}
