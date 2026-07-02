#include <jumpstats/index>

public plugin_init() {
	register_plugin("HNS JumpStats", "1.2.0.dev", "WessTorn");

	init_cvars();
	init_cmds();

	RegisterHookChain(RG_CBasePlayer_Spawn, "rgPlayerSpawn", true);
	RegisterHookChain(RG_PM_Move, "rgPM_Move", true);
	RegisterHookChain(RG_PM_AirMove, "rgPM_AirMove");

	RegisterHookChain(RG_CBasePlayer_Observer_SetMode,"RG_CBasePlayerObserverSetMode_Pre", .post = true);
	RegisterHookChain(RG_CBasePlayer_Observer_FindNextPlayer,"RG_CBasePlayerObserverFindNextPlayer_Post", .post = true);

	g_hudStrafe = CreateHudSyncObj();
	g_hudStats = CreateHudSyncObj();
	g_hudPreSpeed = CreateHudSyncObj();

	//g_bDebugMode = bool:(plugin_flags() & AMX_FLAG_DEBUG);
}

// public plugin_precache() {
//     for(new i; i < sizeof(g_szSounds); i++)
//         precache_sound(g_szSounds[i]);
// }

public rgPlayerSpawn(id) {
	reset_stats(id);
	g_eFailJump[id] = fj_notshow;
	g_isUserSpec[id] = 0;
}

public rgPM_Move(id) {
	if (is_user_hltv(id)) {
		return HC_CONTINUE;
	}

	if (g_isUserSpec[id] && is_user_alive(id)) {
		g_isUserSpec[id] = 0;
	}
	
	g_iPrevButtons[id] = get_entvar(id, var_oldbuttons);

	get_entvar(id, var_origin, g_flOrigin[id]);
	get_entvar(id, var_velocity, g_flVelocity[id]);

	g_flHorSpeed[id] = vector_hor_length(g_flVelocity[id]);
	g_flPrevHorSpeed[id] = vector_hor_length(g_flPrevVelocity[id]);
	g_flOldHorSpeed[id] = vector_hor_length(g_flOldVelocity[id]);

	g_bInDuck[id] = bool:(get_entvar(id, var_flags) & FL_DUCKING);

	new bool:isLadder = bool:(get_entvar(id, var_movetype) == MOVETYPE_FLY);

	new bool:isGround = bool:(get_entvar(id, var_flags) & FL_ONGROUND);

	isGround = isGround || isLadder;

	g_isOldGround[id] = g_isOldGround[id] || g_bPrevLadder[id];

	new iButtons = get_entvar(id, var_button);

	g_bSlide[id] = !isLadder && isPlayerSliding(id);

	if (!g_bSlide[id] && g_bPrevSlide[id]) {
		show_pre(id, PRE_SLIDE, g_flPrevHorSpeed[id]);
	}


	if (g_eSettings[id][S_PRESPEED] && (g_eOnOff[id][of_bSpeed] || g_eOnOff[id][of_bJof] || g_eOnOff[id][of_bPre])) {
		show_prespeed(id);
	}

	if (isGround) {
		g_iFog[id]++;

		if (isLadder) {
			new iEnt[1];
			find_sphere_class(id, "func_ladder", 18.0, iEnt, 1);

			if (iEnt[0] != 0) {
				get_entvar(iEnt[0], var_maxs, g_flLadderXYZ[id]);
				get_entvar(iEnt[0], var_size, g_flLadderSize[id]);
			}

		}

		if (!g_isOldGround[id]) {
			g_flPreHorSpeed[id] = g_flHorSpeed[id];
			if (g_eWhichJump[id] != jt_Not) {
				ready_jumps(id, g_flOrigin[id]);
			}
		}

		if (g_iFog[id] == 1) {
			if (g_bInDuck[id]) {
				g_eDuckType[id] = g_eDuckType[id] != IS_DUCK_NOT ? IS_SGS : g_eDuckType[id];
				g_eJumpType[id] = g_eJumpType[id] != IS_DUCKBHOP ? IS_SBJ : g_eJumpType[id];
			}
		}

		g_flJumpbugGroundZ[id] = g_flOrigin[id][2];
		g_iJumpBug[id] = 0;
		g_bJumpbugDone[id] = false;

		g_bCheckEdgeBug[id] = true;
		g_bEdgebugDone[id] = false;
		g_iEdgeBugCount[id] = 0;
		g_flLastGroundZ[id] = g_flOrigin[id][2];

		if (!g_eJumpType[id]) {
			if (g_ePreStats[id][ptBackWards])
				g_ePreStats[id][ptBackWards] = !bool:(g_iPrevButtons[id] & IN_FORWARD);
			else
				g_ePreStats[id][ptBackWards] = bool:(g_iPrevButtons[id] & IN_BACK);
		}

		if (g_iFog[id] <= 10) {
			g_bOneReset[id] = true;
		} else if (g_bOneReset[id]) {
			reset_stats(id);
			g_bOneReset[id] = false;
		}
	} else {
		if (g_eWhichJump[id] != jt_Not && !g_eFailJump[id]) {
			if ((g_bInDuck[id] ? (g_flOrigin[id][2] + 18.0) : g_flOrigin[id][2]) - g_flFirstJump[id][2] < 0) {
				g_eFailJump[id] = g_eWhichJump[id] == jt_LadderJump ? fj_notshow : fj_fail;
				ready_jumps(id, g_flPrevOrigin[id]);
			}
		}

		if (!g_bJumpbugDone[id]) {
			new bool:isDuckRelease = !(iButtons & IN_DUCK) && (g_iPrevButtons[id] & IN_DUCK);
			new bool:isJumpPress = (iButtons & IN_JUMP) && !(g_iPrevButtons[id] & IN_JUMP);

			if (!g_iJumpBug[id]) {
				if (isDuckRelease && isJumpPress && g_flVelocity[id][2] < 0.0) {
					g_iJumpBug[id] = 2;
				}
			} else {
				g_iJumpBug[id]--;
				if (!g_iJumpBug[id] && g_flVelocity[id][2] > 0.0) {
					new Float:flJumpbugDistance = g_flJumpbugGroundZ[id];
					if (flJumpbugDistance <= 0.0) {
						flJumpbugDistance = g_flLastGroundZ[id] - g_flOrigin[id][2];
					}

					g_bJumpbugDone[id] = true;
					show_pre(id, PRE_JUMPBUG, flJumpbugDistance);
					if (g_eWhichJump[id] != jt_Not) {
						reset_stats(id);
						g_eFailJump[id] = fj_notshow;
					}
				}
			}
		}

		if (g_isOldGround[id]) {
			new bool:isDuck = !g_bInDuck[id] && !(g_iPrevButtons[id] & IN_JUMP) && g_iOldButtons[id] & IN_DUCK;
			new bool:isJump = !isDuck && g_iPrevButtons[id] & IN_JUMP && !(g_iOldButtons[id] & IN_JUMP);

			if (g_bPrevLadder[id]) {
				in_ladder(id, isJump);
			} else {
				if (isDuck) {
					in_ducks(id, g_iFog[id]);
				}
				if (isJump) {
					in_bhop_js(id, g_iFog[id]);

					calc_jof_block(id, g_flFirstJump[id]);
				}
			}
			if (!isDuck && !isJump) {
				if (g_flVelocity[id][2] <= -4.0 && g_iFog[id] > 10) {
					g_isFalling[id] = true;
					g_eJumpType[id] = IS_JUMP;
					g_iJumps[id] = 0;
					g_eJumpData[id][g_iJumps[id]][JUMP_PRE] = g_flHorSpeed[id];
					show_pre(id, PRE_FALL, g_flHorSpeed[id]);
				}
				if (g_flVelocity[id][2] <= -4.0 && g_flPrevHorSpeed[id] > g_flOldHorSpeed[id] + 5.0 && g_iFog[id] == 1) {
					show_pre(id, PRE_BOOST, g_flOldHorSpeed[id], g_iFog[id], g_flPrevHorSpeed[id]);
				}
			}
		}

		if (g_bCheckEdgeBug[id] && !g_bEdgebugDone[id] && g_flVelocity[id][2] < 0.0) {
			new Float:flPlayerGravity;
			get_entvar(id, var_gravity, flPlayerGravity);
			
			if (flPlayerGravity <= 0.0) {
				flPlayerGravity = 1.0;
			}

			new Float:flDelta = g_pCvar[c_iGravity] * flPlayerGravity * Float:get_pmove(pm_frametime);
			new Float:flTarget = -flDelta * 0.5;
			new Float:flCurrentVz = g_flVelocity[id][2];
			new Float:flPrevVz = g_flPrevVelocity[id][2];
			if (floatabs(flCurrentVz - flTarget) <= 1.0 && flPrevVz < flTarget && isGoingToTouchGround(id)) {
				new Float:flDistance = g_flLastGroundZ[id] - g_flOrigin[id][2];
				if (flDistance > 0.0) {
					g_bEdgebugDone[id] = true;
					g_bCheckEdgeBug[id] = false;
					g_iEdgeBugCount[id]++;

					show_pre(id, PRE_EDGEBUG, flDistance);
				}
			}
		}

		g_iFog[id] = 0;
	}
	
	g_iOldButtons[id] = g_iPrevButtons[id];

	g_flOldVelocity[id] = g_flPrevVelocity[id];
	g_flPrevVelocity[id] = g_flVelocity[id];
	g_flPrevOrigin[id] = g_flOrigin[id];

	g_isOldGround[id] = isGround;
	g_bPrevLadder[id] = isLadder;
	g_bPrevInDuck[id] = g_bInDuck[id];
	g_bPrevSlide[id] = g_bSlide[id];

	return HC_CONTINUE;
}

stock bool:isPlayerSliding(id) {
	new Float:origin[3], Float:dest[3];
	origin = g_flOrigin[id];
	dest = origin;
	dest[2] -= 2.0;

	engfunc(EngFunc_TraceHull, origin, dest, IGNORE_MONSTERS, g_bInDuck[id] ? HULL_HEAD : HULL_HUMAN, id, 0);

	new Float:flFraction;
	get_tr2(0, TR_flFraction, flFraction);
	if (flFraction >= 1.0) {
		return false;
	}

	new Float:flPlaneNormal[3];
	get_tr2(0, TR_vecPlaneNormal, flPlaneNormal);

	return flPlaneNormal[2] > 0.0 && flPlaneNormal[2] <= 0.7;
}

stock bool:isGoingToTouchGround(id) {
	new Float:origin[3], Float:dest[3];
	origin = g_flOrigin[id];
	dest = origin;
	dest[2] -= 16.0;

	engfunc(EngFunc_TraceHull, origin, dest, IGNORE_MONSTERS, g_bInDuck[id] ? HULL_HEAD : HULL_HUMAN, id, 0);

	new Float:flFraction;
	get_tr2(0, TR_flFraction, flFraction);
	if (flFraction >= 1.0) {
		return false;
	}

	new Float:flPlaneNormal[3];
	get_tr2(0, TR_vecPlaneNormal, flPlaneNormal);

	return flPlaneNormal[2] > 0.7;
}

public rgPM_AirMove(id) {
	if (!is_user_alive(id)) {
		return HC_CONTINUE;
	}
	
	if (g_eWhichJump[id] == jt_Not) {
		return HC_CONTINUE;
	}

	if (isUserSurfing(id)) {
		reset_stats(id);
		return HC_CONTINUE;
	}

	g_isTouched[id] = get_pmove(pm_numtouch) ? true : g_isTouched[id];	

	if (g_eWhichJump[id] == jt_LongJump) {
		detect_hj(id, g_flOrigin[id], g_flFirstJump[id][2]);
	}

	if (g_iStrafes[id] >= NSTRAFES - 1) {
		return HC_CONTINUE;
	}

	new iButtons = get_entvar(id, var_button);

	new Float:flVelocity[3]; get_entvar(id, var_velocity, flVelocity);

	new Float:flStrSpeed = vector_hor_length(flVelocity);

	new Float:flAngles[3]; get_entvar(id, var_angles, flAngles);

	g_eJumpstats[id][js_iFrames]++;

	new bool:isTiring = bool:(g_flStrOldAngle[id] != flAngles[1]);

	if (iButtons & IN_MOVELEFT && !(g_iOldStrButtons[id] & IN_MOVELEFT) && !(iButtons & (IN_MOVERIGHT|IN_BACK|IN_FORWARD)) && (isTiring)) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_A;
	} else if (iButtons & IN_MOVERIGHT && !(g_iOldStrButtons[id] & IN_MOVERIGHT) && !(iButtons & (IN_MOVELEFT|IN_BACK|IN_FORWARD)) && (isTiring)) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_D;
	} else if (iButtons & IN_BACK && !(g_iOldStrButtons[id] & IN_BACK) && !(iButtons & (IN_MOVELEFT|IN_MOVERIGHT|IN_FORWARD)) && (isTiring)) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_S;
	} else if (iButtons & IN_FORWARD && !(g_iOldStrButtons[id] & IN_FORWARD) && !(iButtons & (IN_MOVELEFT|IN_MOVERIGHT|IN_BACK)) && (isTiring)) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_W;
	}

	if (iButtons & (IN_MOVERIGHT|IN_MOVELEFT|IN_FORWARD|IN_BACK)) {
		if (flStrSpeed > g_flStartSpeed[id]) {
			g_eStrafeStats[id][g_iStrafes[id]][st_iFrameGood] += 1;
		} else {
			g_eStrafeStats[id][g_iStrafes[id]][st_iFrameBad] += 1;
		}

	}

	if (flStrSpeed > g_flStartSpeed[id]) {
		g_eStrafeStats[id][g_iStrafes[id]][st_flSpeed] += flStrSpeed - g_flStartSpeed[id];
		g_flStartSpeed[id] = flStrSpeed;
	} else if (flStrSpeed < g_flStartSpeed[id]) {
		g_eStrafeStats[id][g_iStrafes[id]][st_flSpeedFail] += g_flStartSpeed[id] - flStrSpeed;
		g_flStartSpeed[id] = flStrSpeed;
	}

	g_eStrafeStats[id][g_iStrafes[id]][st_iFrame] += 1;

	g_eJumpstats[id][js_flEndSpeed] = flStrSpeed;

	g_flStrOldAngle[id] = flAngles[1];

	if ((iButtons & IN_MOVERIGHT && iButtons & (IN_MOVELEFT|IN_FORWARD|IN_BACK))
	|| (iButtons & IN_MOVELEFT && iButtons & (IN_FORWARD|IN_BACK|IN_MOVERIGHT))
	|| (iButtons & IN_FORWARD && iButtons & (IN_BACK|IN_MOVERIGHT|IN_MOVELEFT))
	|| (iButtons & IN_BACK && iButtons & (IN_MOVERIGHT|IN_MOVELEFT|IN_FORWARD)))
		g_iOldStrButtons[id] = 0;
	else if (isTiring)
		g_iOldStrButtons[id] = iButtons;

	return HC_CONTINUE;
}

public RG_CBasePlayerObserverSetMode_Pre(const id, iMode) {
	new iLastMode = get_member(id, m_iObserverLastMode);
	if (iLastMode != OBS_CHASE_FREE && iLastMode != OBS_IN_EYE) {
		g_isUserSpec[id] = 0;
		return HC_CONTINUE;
	}

	new iTarget = get_member(id, m_hObserverTarget);

	g_isUserSpec[id] = iTarget;
	
	return HC_CONTINUE;
}

public RG_CBasePlayerObserverFindNextPlayer_Post(const id) {
	new iTarget = get_member(id, m_hObserverTarget);

	g_isUserSpec[id] = iTarget;
}


public client_connect(id) {
	arrayset(g_eOnOff[id], true, JS_ONOFF); // Ну потом

	reset_stats(id);
}
