#include <jumpstats/index>

public plugin_init() {
	register_plugin("HNS JumpStats", "v1.1.0", "WessTorn");

	init_cvars();
	init_cmds();
	register_forward(FM_CmdStart, "fwdCmdStart");
	RegisterHookChain(RG_PM_Move, "rgPM_Move", true);
	RegisterHookChain(RG_PM_AirMove, "rgPM_AirMove");

	RegisterHookChain(RG_CBasePlayer_Observer_SetMode,"RG_CBasePlayerObserverSetMode_Pre", .post = true);
	RegisterHookChain(RG_CBasePlayer_Observer_FindNextPlayer,"RG_CBasePlayerObserverFindNextPlayer_Post", .post = true);

	g_hudStrafe = CreateHudSyncObj();
	g_hudStats = CreateHudSyncObj();
	g_hudPreSpeed = CreateHudSyncObj();

	//g_bDebugMode = bool:(plugin_flags() & AMX_FLAG_DEBUG);
}

public plugin_precache() {
    for(new i; i < sizeof(g_szSounds); i++)
        precache_sound(g_szSounds[i]);
}

public rgPlayerSpawn(id) {
	reset_stats(id);
	g_eFailJump[id] = fj_notshow;
}

public rgPM_Move(id) {
	if (is_user_bot(id) || is_user_hltv(id)) {
		return HC_CONTINUE;
	}
	
	g_iPrevButtons[id] = get_entvar(id, var_oldbuttons);

	get_entvar(id, var_origin, g_flOrigin[id]);
	get_entvar(id, var_velocity, g_flVelocity[id]);

	g_flMaxSpeed[id] = get_maxspeed(id);

	g_flHorSpeed[id] = vector_hor_length(g_flVelocity[id]);
	g_flPrevHorSpeed[id] = vector_hor_length(g_flPrevVelocity[id]);

	g_bInDuck[id] = bool:(get_entvar(id, var_flags) & FL_DUCKING);

	new bool:isLadder = bool:(get_entvar(id, var_movetype) == MOVETYPE_FLY);

	new bool:isGround = bool:(get_entvar(id, var_flags) & FL_ONGROUND);

	isGround = isGround || isLadder;

	g_isOldGround[id] = g_isOldGround[id] || g_bPrevLadder[id];

	if (g_pCvar[c_iEnablePreSpeed] && (g_eOnOff[id][of_bSpeed] || g_eOnOff[id][of_bJof] || g_eOnOff[id][of_bPre]))
		show_prespeed(id);

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
				g_eDuckType[id] = IS_SGS;
				g_eJumpType[id] = g_eJumpType[id] != IS_DUCKBHOP ? IS_SBJ : g_eJumpType[id];
			} else {
				g_eDuckType[id] = IS_DD;
			}
		}

		if (!g_eJumpType[id]) {
			if (g_ePreStats[id][ptBackWards])
				g_ePreStats[id][ptBackWards] = !bool:(g_iPrevButtons[id] & IN_FORWARD);
			else
				g_ePreStats[id][ptBackWards] = bool:(g_iPrevButtons[id] & IN_BACK);
		}

		if (g_iFog[id] <= 10) {
			g_bOneReset[id] = true;
		} else if (g_bOneReset[id]) {
			check_and_show_move(id);

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
			if (!isDuck && !isJump && g_flVelocity[id][2] <= -4.0) {
				if (g_iFog[id] > 10) {
					g_isFalling[id] = true;
					g_eJumpType[id] = IS_JUMP;
					g_iJumps[id] = 0;
					g_eJumpData[id][g_iJumps[id]][JUMP_PRE] = g_flHorSpeed[id];
					show_pre(id, pre_fall, g_flHorSpeed[id]);
					// ФАЛЛ
				}
			}
		}

		g_iFog[id] = 0;
	}
	
	g_iOldButtons[id] = g_iPrevButtons[id];

	g_flPrevVelocity[id] = g_flVelocity[id];
	g_flPrevOrigin[id] = g_flOrigin[id];

	g_isOldGround[id] = isGround;
	g_bPrevLadder[id] = isLadder;
	g_bPrevInDuck[id] = g_bInDuck[id];

	return HC_CONTINUE;
}

public rgPM_AirMove(id) {
	if (!is_user_alive(id) || is_user_bot(id)) {
		return HC_CONTINUE;
	}

	if (isUserSurfing(id)) {
		if (g_eWhichJump[id] == jt_Not) {
			reset_stats(id);
		}

		g_bMoveOldSurfing[id] = true;
	}
	
	if (g_eWhichJump[id] == jt_Not) {
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