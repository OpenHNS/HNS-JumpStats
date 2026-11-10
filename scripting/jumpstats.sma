#include <jumpstats/index>

public plugin_init() {
	register_plugin("HNS JumpStats", "1.2.0.dev", "WessTorn");

	init_cvars();
	init_cmds();
	settings_storage_init();

	RegisterHookChain(RG_CBasePlayer_Spawn, "rgPlayerSpawn", true);
	RegisterHookChain(RG_PM_Move, "rgPM_Move", true);
	RegisterHookChain(RG_PM_AirMove, "rgPM_AirMove");
	register_forward(FM_CmdStart, "fwCmdStart_Post", true);
	register_forward(FM_PlayerPreThink, "fwPlayerPreThink_Post", true);

	RegisterHookChain(RG_CBasePlayer_Observer_SetMode,"RG_CBasePlayerObserverSetMode_Pre", .post = true);
	RegisterHookChain(RG_CBasePlayer_Observer_FindNextPlayer,"RG_CBasePlayerObserverFindNextPlayer_Post", .post = true);

	g_bDebugMode = bool:(plugin_flags() & AMX_FLAG_DEBUG);

	init_menus();
}

public plugin_end() {
	settings_storage_close();
}

// public plugin_precache() {
//     for(new i; i < sizeof(g_szSounds); i++)
//         precache_sound(g_szSounds[i]);
// }

public rgPlayerSpawn(id) {
	reset_stats(id);
	g_flPrevFrameTime[id] = 0.0;
	g_eFailJump[id] = fj_notshow;
	g_isUserSpec[id] = 0;
}

public rgPM_Move(id) {
	if (g_pCvar[c_iUseForwards]) {
		return HC_CONTINUE;
	}

	process_player_move(id, Float:get_pmove(pm_frametime));
	return HC_CONTINUE;
}

public fwCmdStart_Post(id, ucHandle, randomSeed) {
	if (g_pCvar[c_iUseForwards]) {
		g_flFrameTime[id] = float(get_uc(ucHandle, UC_Msec)) * 0.001;
	}

	return FMRES_IGNORED;
}

public fwPlayerPreThink_Post(id) {
	if (!g_pCvar[c_iUseForwards]) {
		return FMRES_IGNORED;
	}

	process_player_move(id, g_flFrameTime[id]);
	process_air_move(id, false);
	return FMRES_IGNORED;
}

stock process_player_move(id, Float:flFrameTime) {
	if (is_user_hltv(id)) {
		return;
	}

	if (g_isUserSpec[id] && is_user_alive(id)) {
		g_isUserSpec[id] = 0;
	}
	
	g_iPrevButtons[id] = get_entvar(id, var_oldbuttons);

	get_entvar(id, var_origin, g_flOrigin[id]);
	get_entvar(id, var_velocity, g_flVelocity[id]);

	g_flHorSpeed[id] = vector_hor_length(g_flVelocity[id]);
	g_flHorSpeed3D[id] = vector_length(g_flVelocity[id]);
	g_flPrevHorSpeed[id] = vector_hor_length(g_flPrevVelocity[id]);
	g_flOldHorSpeed[id] = vector_hor_length(g_flOldVelocity[id]);

	g_bInDuck[id] = bool:(get_entvar(id, var_flags) & FL_DUCKING);

	new bool:isLadder = bool:(get_entvar(id, var_movetype) == MOVETYPE_FLY);

	new bool:isGround = bool:(get_entvar(id, var_flags) & FL_ONGROUND);

	isGround = isGround || isLadder;

	g_isOldGround[id] = g_isOldGround[id] || g_bPrevLadder[id];

	new iButtons = get_entvar(id, var_button);

	g_bSlide[id] = !isLadder && isPlayerSliding(id);
	new bool:bBlockBugChecks = bool:(g_bSlide[id] || get_entvar(id, var_waterlevel) >= 2);

	if (!g_bSlide[id] && g_bPrevSlide[id]) {
		show_pre(id, PRE_SLIDE, g_flPrevHorSpeed[id]);
	}


	if (g_pCvar[C_ENABLE_SPEED]) {
		show_speed(id);
	}

	if (isGround) {
		g_iFog[id]++;

		if (!g_isOldGround[id]) {
			g_flPreHorSpeed[id] = g_flHorSpeed[id];
			if (g_eWhichJump[id] != jt_Not) {
				if (isLadder && g_eWhichJump[id] == jt_LadderJump) {
					reset_stats(id);
				} else {
					ready_jumps(id, g_flOrigin[id]);
				}
			}
		}

		if (g_iFog[id] == 1) {
			if (g_bInDuck[id]) {
				g_eDuckType[id] = g_eDuckType[id] != IS_DUCK_NOT ? IS_SGS : g_eDuckType[id];
				g_eJumpType[id] = g_eJumpType[id] != IS_DUCKBHOP ? IS_SBJ : g_eJumpType[id];
			}
		}

		g_eJumpBugData[id][JB_FRAMES] = 0;
		g_eJumpBugData[id][JB_CHECK] = !bBlockBugChecks;
		g_eJumpBugData[id][JB_LAST_GROUND_Z] = g_flOrigin[id][2];

		g_eEdgeBugData[id][EB_CHECK] = !bBlockBugChecks;
		g_eEdgeBugData[id][EB_LAST_GROUND_Z] = g_flOrigin[id][2];

		if (!g_eJumpType[id]) {
			if (g_ePreStats[id][ptBackWards])
				g_ePreStats[id][ptBackWards] = !bool:(g_iPrevButtons[id] & IN_FORWARD);
			else
				g_ePreStats[id][ptBackWards] = bool:(g_iPrevButtons[id] & IN_BACK);
		}

		if (g_iFog[id] <= 10) {
			g_bOneReset[id] = true;
		} else if (g_bOneReset[id]) {
			ready_moves(id);
			reset_stats(id);
			g_bOneReset[id] = false;
		}
	} else {
		if (bBlockBugChecks) {
			g_eJumpBugData[id][JB_CHECK] = false;
			g_eJumpBugData[id][JB_FRAMES] = 0;
			g_eEdgeBugData[id][EB_CHECK] = false;
		}

		check_failed_jump(id);

		in_jumpbug_js(id, iButtons);

		if (g_isOldGround[id]) {
			new bool:isDuck = !g_bPrevInDuck[id] && !(g_iPrevButtons[id] & IN_DUCK) && (g_iOldButtons[id] & IN_DUCK);
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
			in_fall_js(id, g_iFog[id], isDuck, isJump, flFrameTime);
		}

		in_edgebug_js(id, flFrameTime);

		g_iFog[id] = 0;
	}
	
	g_iOldButtons[id] = g_iPrevButtons[id];

	g_flOldVelocity[id] = g_flPrevVelocity[id];
	g_flPrevVelocity[id] = g_flVelocity[id];
	g_flPrevOrigin[id] = g_flOrigin[id];

	g_isOldGround[id] = isGround;
	g_bPrevLadder[id] = isLadder;
	g_bOldInDuck[2][id] = g_bOldInDuck[1][id];
	g_bOldInDuck[1][id] = g_bOldInDuck[0][id];
	g_bOldInDuck[0][id] = g_bPrevInDuck[id];
	g_bPrevInDuck[id] = g_bInDuck[id];
	g_bPrevSlide[id] = g_bSlide[id];
	g_flPrevFrameTime[id] = flFrameTime;

}

stock check_failed_jump(id) {
	if (g_eWhichJump[id] == jt_Not || g_eFailJump[id] != fj_good) {
		return;
	}

	new Float:flCurrentZ = g_bInDuck[id] ? g_flOrigin[id][2] + 18.0 : g_flOrigin[id][2];

	if (g_eWhichJump[id] != jt_LadderJump) {
		if (flCurrentZ < g_flFirstJump[id][2]) {
			g_eFailJump[id] = fj_fail;
			ready_jumps(id, g_flPrevOrigin[id]);
		}
		return;
	}

	new Float:flPreviousZ = g_bPrevInDuck[id] ? g_flPrevOrigin[id][2] + 18.0 : g_flPrevOrigin[id][2];

	if (!g_bLdjAboveStart[id] && flCurrentZ >= g_flFirstJump[id][2]) {
		g_bLdjAboveStart[id] = true;
	}

	if (!g_bLdjAboveStart[id] || flPreviousZ < g_flFirstJump[id][2] || flCurrentZ >= g_flFirstJump[id][2]) {
		return;
	}

	new Float:flDeltaZ = flPreviousZ - flCurrentZ;
	new Float:flRatio;
	if (flDeltaZ > 0.0) {
		flRatio = (flPreviousZ - g_flFirstJump[id][2]) / flDeltaZ;
	}

	if (flRatio < 0.0) {
		flRatio = 0.0;
	} else if (flRatio > 1.0) {
		flRatio = 1.0;
	}

	new Float:flCrossOrigin[3];
	flCrossOrigin[0] = g_flPrevOrigin[id][0] + (g_flOrigin[id][0] - g_flPrevOrigin[id][0]) * flRatio;
	flCrossOrigin[1] = g_flPrevOrigin[id][1] + (g_flOrigin[id][1] - g_flPrevOrigin[id][1]) * flRatio;
	flCrossOrigin[2] = g_flFirstJump[id][2] - (g_bInDuck[id] ? 18.0 : 0.0);

	g_eFailJump[id] = fj_fail;
	ready_jumps(id, flCrossOrigin);
}

public rgPM_AirMove(id) {
	if (g_pCvar[c_iUseForwards]) {
		return HC_CONTINUE;
	}

	process_air_move(id, true);
	return HC_CONTINUE;
}

stock process_air_move(id, bool:fromPmMove) {
	if (!is_user_alive(id)) {
		return;
	}
	
	if (g_eWhichJump[id] == jt_Not) {
		return;
	}

	if (!fromPmMove && (get_entvar(id, var_flags) & FL_ONGROUND
	|| get_entvar(id, var_movetype) != MOVETYPE_WALK
	|| get_entvar(id, var_waterlevel) >= 2)) {
		return;
	}

	if (isPlayerSliding(id)) {
		reset_stats(id);
		return;
	}

	if (fromPmMove) {
		g_isTouched[id] = get_pmove(pm_numtouch) ? true : g_isTouched[id];
	}

	if (g_eWhichJump[id] == jt_LongJump) {
		detect_hj(id, g_flOrigin[id], g_flFirstJump[id][2]);
	}

	if (g_iStrafes[id] >= NSTRAFES - 1) {
		return;
	}

	new iButtons = get_entvar(id, var_button);

	new Float:flVelocity[3]; get_entvar(id, var_velocity, flVelocity);

	new Float:flStrSpeed = vector_hor_length(flVelocity);

	new Float:flAngles[3]; get_entvar(id, var_v_angle, flAngles);

	if (g_eWhichJump[id] == jt_LadderJump) {
		g_flLdjEndCos[id] = velocity_view_cos(flVelocity, flAngles[1]);
	}

	g_eJumpstats[id][js_iFrames]++;

	new bool:isTiring = bool:(g_flStrOldAngle[id] != flAngles[1]);
	new iMoveButtons = iButtons & (IN_MOVELEFT|IN_MOVERIGHT|IN_BACK|IN_FORWARD);

	if (g_eWhichJump[id] == jt_LadderJump && g_eJumpstats[id][js_iFrames] == 1 && iMoveButtons == IN_MOVELEFT) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_A;
	} else if (g_eWhichJump[id] == jt_LadderJump && g_eJumpstats[id][js_iFrames] == 1 && iMoveButtons == IN_MOVERIGHT) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_D;
	} else if (g_eWhichJump[id] == jt_LadderJump && g_eJumpstats[id][js_iFrames] == 1 && iMoveButtons == IN_BACK) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_S;
	} else if (g_eWhichJump[id] == jt_LadderJump && g_eJumpstats[id][js_iFrames] == 1 && iMoveButtons == IN_FORWARD) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_W;
	} else if (iButtons & IN_MOVELEFT && (g_iStrafes[id] == 0 || !(g_iOldStrButtons[id] & IN_MOVELEFT)) && !(iButtons & (IN_MOVERIGHT|IN_BACK|IN_FORWARD)) && (g_iStrafes[id] == 0 || isTiring)) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_A;
	} else if (iButtons & IN_MOVERIGHT && (g_iStrafes[id] == 0 || !(g_iOldStrButtons[id] & IN_MOVERIGHT)) && !(iButtons & (IN_MOVELEFT|IN_BACK|IN_FORWARD)) && (g_iStrafes[id] == 0 || isTiring)) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_D;
	} else if (iButtons & IN_BACK && (g_iStrafes[id] == 0 || !(g_iOldStrButtons[id] & IN_BACK)) && !(iButtons & (IN_MOVELEFT|IN_MOVERIGHT|IN_FORWARD)) && (g_iStrafes[id] == 0 || isTiring)) {
		g_iStrafes[id]++;
		g_eStrafeStats[id][g_iStrafes[id]][st_iButton] = bi_S;
	} else if (iButtons & IN_FORWARD && (g_iStrafes[id] == 0 || !(g_iOldStrButtons[id] & IN_FORWARD)) && !(iButtons & (IN_MOVELEFT|IN_MOVERIGHT|IN_BACK)) && (g_iStrafes[id] == 0 || isTiring)) {
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
	settings_apply_defaults(id);
	g_bOldInDuck[0][id] = false;
	g_bOldInDuck[1][id] = false;
	g_bOldInDuck[2][id] = false;

	reset_stats(id);
}

public client_authorized(id, const authid[]) {
	settings_load_player(id, authid);
}
