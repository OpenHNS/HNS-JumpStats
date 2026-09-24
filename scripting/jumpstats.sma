#include <jumpstats/index>

public plugin_init() {
	register_plugin("HNS JumpStats", "1.2.0.dev", "WessTorn");

	init_cvars();
	init_cmds();

	RegisterHookChain(RG_CBasePlayer_Spawn, "rgPlayerSpawn", true);
	RegisterHookChain(RG_PM_Move, "rgPM_Move_Pre");
	RegisterHookChain(RG_PM_Move, "rgPM_Move", true);
	RegisterHookChain(RG_PM_Jump, "rgPM_Jump_Pre");
	RegisterHookChain(RG_PM_Jump, "rgPM_Jump_Post", true);
	RegisterHookChain(RG_PM_AirMove, "rgPM_AirMove");
	RegisterHookChain(RG_PM_AirAccelerate, "rgPM_AirAccelerate_Pre");
	RegisterHookChain(RG_PM_AirAccelerate, "rgPM_AirAccelerate_Post", true);

	RegisterHookChain(RG_CBasePlayer_Observer_SetMode,"RG_CBasePlayerObserverSetMode_Pre", .post = true);
	RegisterHookChain(RG_CBasePlayer_Observer_FindNextPlayer,"RG_CBasePlayerObserverFindNextPlayer_Post", .post = true);

	g_hudStrafe = CreateHudSyncObj();
	g_hudStats = CreateHudSyncObj();
	g_hudPreSpeed = CreateHudSyncObj();

	g_bDebugMode = bool:(plugin_flags() & AMX_FLAG_DEBUG);

	init_menus();
}

// public plugin_precache() {
//     for(new i; i < sizeof(g_szSounds); i++)
//         precache_sound(g_szSounds[i]);
// }

public rgPlayerSpawn(id) {
	reset_stats(id);
	reset_pm_history(id);
	g_eFailJump[id] = fj_notshow;
	g_isUserSpec[id] = 0;
}

public rgPM_Move_Pre(id) {
	g_bPmActive[id] = false;
	g_bPmAirMove[id] = false;
	g_bPmAirAccelerated[id] = false;
	g_bPmJumpGround[id] = false;
	g_bPmJumped[id] = false;
	g_bPmJumpbugAttempt[id] = false;
	new moveType = get_pmove(pm_movetype);
	if (get_pmove(pm_dead) || get_pmove(pm_iuser1) || (moveType != MOVETYPE_WALK && moveType != MOVETYPE_FLY)) {
		if (g_bPmHistory[id]) {
			reset_stats(id);
			reset_pm_history(id);
		}
		return HC_CONTINUE;
	}

	g_bPmActive[id] = true;
	get_pmove(pm_origin, g_flPrevOrigin[id]);
	get_pmove(pm_velocity, g_flPrevVelocity[id]);
	g_bPrevInDuck[id] = bool:(get_pmove(pm_flags) & FL_DUCKING);
	g_bPrevLadder[id] = bool:(moveType == MOVETYPE_FLY);
	// pm_onground is categorized inside PM_Move; it is not initialized here.
	g_isOldGround[id] = bool:(get_pmove(pm_flags) & FL_ONGROUND) || g_bPrevLadder[id];

	new cmd = get_pmove(pm_cmd);
	g_iPmButtons[id] = get_ucmd(cmd, ucmd_buttons);
	get_ucmd(cmd, ucmd_viewangles, g_flPmAngles[id]);
	if (!g_bPmHistory[id]) {
		g_bPmHistory[id] = true;
		g_flOldVelocity[id] = g_flPrevVelocity[id];
		g_iPrevButtons[id] = g_iPmButtons[id];
	}

	g_bPmJumpbugAttempt[id] = g_bCheckJumpBug[id] && !g_bJumpbugDone[id]
		&& !g_isOldGround[id] && g_flPrevVelocity[id][2] < 0.0
		&& !(g_iPmButtons[id] & IN_DUCK) && (g_iPrevButtons[id] & IN_DUCK)
		&& (g_iPmButtons[id] & IN_JUMP) && !(g_iPrevButtons[id] & IN_JUMP);
	if (g_bPmJumpbugAttempt[id]) {
		g_flJumpbugStart[id] = g_flPrevOrigin[id];
	}
	return HC_CONTINUE;
}

public rgPM_Jump_Pre(id) {
	g_bPmJumpGround[id] = g_bPmActive[id] && get_pmove(pm_onground) != -1 && get_pmove(pm_waterlevel) < 2;
	if (g_bPmJumpGround[id]) {
		get_pmove(pm_origin, g_flPmJumpOrigin[id]);
		g_bPmJumpDuck[id] = bool:(get_pmove(pm_flags) & FL_DUCKING);
		new Float:velocity[3];
		get_pmove(pm_velocity, velocity);
		g_flPmJumpPreSpeed[id] = vector_hor_length(velocity);
	}
	return HC_CONTINUE;
}

public rgPM_Jump_Post(id) {
	if (g_bPmJumpGround[id] && get_pmove(pm_onground) == -1 && get_pmove(pm_waterlevel) < 2) {
		g_bPmJumped[id] = true;
		get_pmove(pm_velocity, g_flPmJumpVelocity[id]);
		g_flPmJumpPostSpeed[id] = vector_hor_length(g_flPmJumpVelocity[id]);
	}
	return HC_CONTINUE;
}

public rgPM_Move(id) {
	if (!g_bPmActive[id]) {
		return HC_CONTINUE;
	}

	if (g_isUserSpec[id] && is_user_alive(id)) {
		g_isUserSpec[id] = 0;
	}
	
	get_pmove(pm_origin, g_flOrigin[id]);
	get_pmove(pm_velocity, g_flVelocity[id]);

	g_flHorSpeed[id] = vector_hor_length(g_flVelocity[id]);
	g_flHorSpeed3D[id] = vector_length(g_flVelocity[id]);
	g_flPrevHorSpeed[id] = vector_hor_length(g_flPrevVelocity[id]);
	g_flOldHorSpeed[id] = vector_hor_length(g_flOldVelocity[id]);

	g_bInDuck[id] = bool:(get_pmove(pm_flags) & FL_DUCKING);

	new bool:isLadder = bool:(get_pmove(pm_movetype) == MOVETYPE_FLY);

	new bool:isGround = bool:(get_pmove(pm_onground) != -1);

	isGround = isGround || isLadder;

	new iButtons = g_iPmButtons[id];

	g_bSlide[id] = !isLadder && isPlayerSliding(id);
	new bool:bBlockBugChecks = bool:(g_bSlide[id] || get_pmove(pm_waterlevel) >= 2);

	// Classify takeoff before consuming this command's first air sample.
	if (g_isOldGround[id] && (!isGround || g_bPmJumped[id])) {
		start_pm_jump(id, iButtons);
	}
	if (g_bPmAirMove[id] && g_eWhichJump[id] != jt_Not) {
		collect_pm_air_stats(id, isGround);
	}

	if (!g_bSlide[id] && g_bPrevSlide[id]) {
		show_pre(id, PRE_SLIDE, g_flPrevHorSpeed[id]);
	}


	if (g_eSettings[id][S_PRESPEED] && (g_eOnOff[id][of_bSpeed] || g_eOnOff[id][of_bJof] || g_eOnOff[id][of_bPre])) {
		show_prespeed(id);
	}

	if (isGround) {
		if (g_bPmJumped[id]) {
			g_iFog[id] = 0;
		}
		g_iFog[id]++;

		if (!g_isOldGround[id] || g_bPmJumped[id]) {
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

		g_bCheckJumpBug[id] = !bBlockBugChecks;
		g_bJumpbugDone[id] = false;

		g_bCheckEdgeBug[id] = !bBlockBugChecks;
		g_bEdgebugDone[id] = false;
		g_iEdgeBugCount[id] = 0;
		g_flLastGroundZ[id] = g_flOrigin[id][2];

		if (!g_eJumpType[id]) {
			if (g_ePreStats[id][ptBackWards])
				g_ePreStats[id][ptBackWards] = !bool:(iButtons & IN_FORWARD);
			else
				g_ePreStats[id][ptBackWards] = bool:(iButtons & IN_BACK);
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
			g_bCheckJumpBug[id] = false;
			g_bCheckEdgeBug[id] = false;
		}

		if (g_eWhichJump[id] != jt_Not && !g_eFailJump[id]) {
			new Float:flCurrentZ = g_bInDuck[id] ? g_flOrigin[id][2] + 18.0 : g_flOrigin[id][2];

			if (g_eWhichJump[id] == jt_LadderJump) {
				new Float:flPreviousZ = g_bPrevInDuck[id] ? g_flPrevOrigin[id][2] + 18.0 : g_flPrevOrigin[id][2];

				if (!g_bLdjAboveStart[id] && flCurrentZ >= g_flFirstJump[id][2]) {
					g_bLdjAboveStart[id] = true;
				}

				if (g_bLdjAboveStart[id] && flPreviousZ >= g_flFirstJump[id][2] && flCurrentZ < g_flFirstJump[id][2]) {
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
			} else if (flCurrentZ < g_flFirstJump[id][2]) {
				g_eFailJump[id] = fj_fail;
				ready_jumps(id, g_flPrevOrigin[id]);
			}
		}

		if (!bBlockBugChecks && g_bPmJumpbugAttempt[id] && g_bPmJumped[id] && g_flVelocity[id][2] > 0.0) {
			new Float:flJumpbugLandingZ;
			if (find_jumpbug_landing(id, g_flJumpbugStart[id], flJumpbugLandingZ)) {
				new Float:flJumpbugDistance = float(floatround(g_flLastGroundZ[id])) - flJumpbugLandingZ;
				if (flJumpbugDistance > 0.0) {
					show_pre(id, PRE_JUMPBUG, flJumpbugDistance);
					show_special_stats(id, PRE_JUMPBUG, flJumpbugDistance);
					if (g_eWhichJump[id] != jt_Not) {
						reset_stats(id);
						g_eFailJump[id] = fj_notshow;
					}
					g_bJumpbugDone[id] = true;
					g_bCheckJumpBug[id] = false;
				}
			}
		}

		if (g_bCheckEdgeBug[id] && !g_bEdgebugDone[id] && g_flVelocity[id][2] < 0.0) {
			new Float:flPlayerGravity = Float:get_pmove(pm_gravity);
			
			if (flPlayerGravity <= 0.0) {
				flPlayerGravity = 1.0;
			}

			new Float:flFrameTime = Float:get_pmove(pm_frametime);
			new Float:flDelta = Float:get_movevar(mv_gravity) * flPlayerGravity * flFrameTime;
			new Float:flTarget = -flDelta * 0.5;
			new Float:flCurrentVz = g_flVelocity[id][2];
			new Float:flPrevVz = g_flPrevVelocity[id][2];
			if (floatabs(flCurrentVz - flTarget) <= 1.0 && flPrevVz < flTarget && isGoingToTouchGround(id, flFrameTime)) {
				new Float:flDistance = g_flLastGroundZ[id] - g_flOrigin[id][2];
				if (flDistance > 0.0) {
					g_bEdgebugDone[id] = true;
					g_bCheckEdgeBug[id] = false;
					g_iEdgeBugCount[id]++;

					show_pre(id, PRE_EDGEBUG, flDistance);
					show_special_stats(id, PRE_EDGEBUG, flDistance);
					if (g_eWhichJump[id] != jt_Not) {
						reset_stats(id);
						g_eFailJump[id] = fj_notshow;
					}
				}
			}
		}

		g_iFog[id] = 0;
	}
	
	g_iPrevButtons[id] = iButtons;
	g_bPmActive[id] = false;

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

	return HC_CONTINUE;
}

stock start_pm_jump(id, iButtons) {
	new bool:isJump = g_bPmJumped[id];
	new bool:isDuck = !isJump && !g_bInDuck[id] && !(iButtons & IN_JUMP) && bool:(g_iPrevButtons[id] & IN_DUCK);

	if (g_bPrevLadder[id]) {
		in_ladder(id, bool:(iButtons & IN_JUMP));
	} else if (isJump) {
		in_bhop_js(id, g_iFog[id]);
		calc_jof_block(id, g_flFirstJump[id], g_flPmJumpVelocity[id]);
	} else if (isDuck) {
		in_ducks(id, g_iFog[id]);
	} else if (g_flVelocity[id][2] <= -4.0 && g_iFog[id] > 10) {
		g_isFalling[id] = true;
		g_eJumpType[id] = IS_JUMP;
		g_iJumps[id] = 0;
		g_eJumpData[id][g_iJumps[id]][JUMP_PRE] = g_flHorSpeed[id];
		show_pre(id, PRE_FALL, g_flHorSpeed[id]);
	}

	if (g_eWhichJump[id] != jt_Not) {
		g_iOldStrButtons[id] = 0;
		// Compare the first air command with the view before this move.
		new Float:oldAngles[3];
		get_pmove(pm_oldangles, oldAngles);
		g_flStrOldAngle[id] = oldAngles[1];
	}
	if (is_boost_frame(id)) {
		show_pre(id, PRE_BOOST, g_flOldHorSpeed[id], g_iFog[id], g_flPrevHorSpeed[id]);
		show_boost_stats(id, g_flOldHorSpeed[id], g_flPrevHorSpeed[id]);
		show_boost_chat(id, g_flOldHorSpeed[id], g_flPrevHorSpeed[id]);
	}
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

stock bool:is_boost_frame(id) {
	return bool:(
		!g_bSlide[id]
		&& g_iFog[id] == 1
		&& g_flPrevHorSpeed[id] > g_flOldHorSpeed[id] + 5.0
		&& g_flVelocity[id][2] <= -4.0
	);
}

stock bool:isGoingToTouchGround(id, Float:flFrameTime) {
	if (!g_bPmAirAccelerated[id] || flFrameTime <= 0.0) {
		return false;
	}

	new Float:flVelocity[3], Float:flDestination[3];
	// AirAccelerate Post already includes the first half of gravity.
	flVelocity = g_flPmAirVelocity[id];

	for (new i = 0; i < 3; i++) {
		flDestination[i] = g_flPmAirOrigin[id][i] + (flVelocity[i] + g_flPmAirBaseVelocity[id][i]) * flFrameTime;
	}

	new iTrace = create_tr2();
	engfunc(EngFunc_TraceHull, g_flPmAirOrigin[id], flDestination, DONT_IGNORE_MONSTERS, g_iPmAirHull[id] ? HULL_HEAD : HULL_HUMAN, id, iTrace);

	new Float:flFraction;
	get_tr2(iTrace, TR_flFraction, flFraction);
	if (flFraction >= 1.0) {
		free_tr2(iTrace);
		return false;
	}

	new Float:flPlaneNormal[3];
	get_tr2(iTrace, TR_vecPlaneNormal, flPlaneNormal);
	free_tr2(iTrace);

	return flPlaneNormal[2] > 0.7;
}

stock bool:find_jumpbug_landing(id, Float:flStartOrigin[3], &Float:flLandingZ) {
	new Float:flProbe[3];
	flProbe = flStartOrigin;
	flProbe[2] = float(floatround(flProbe[2], floatround_floor));

	new iTrace = create_tr2();
	new iCounter = 18;

	while (iCounter > 0) {
		engfunc(EngFunc_TraceHull, flProbe, flProbe, DONT_IGNORE_MONSTERS, HULL_HUMAN, id, iTrace);

		if (get_tr2(iTrace, TR_StartSolid) || get_tr2(iTrace, TR_AllSolid) || !get_tr2(iTrace, TR_InOpen)) {
			flLandingZ = flProbe[2] + 1.0;
			free_tr2(iTrace);
			return true;
		}

		flProbe[2] -= 1.0;
		iCounter--;
	}

	free_tr2(iTrace);
	return false;
}

public rgPM_AirMove(id) {
	if (!g_bPmActive[id]) {
		return HC_CONTINUE;
	}
	// Buffer even when the jump has not been classified yet.
	g_bPmAirMove[id] = true;
	get_pmove(pm_origin, g_flPmAirOrigin[id]);
	get_pmove(pm_basevelocity, g_flPmAirBaseVelocity[id]);
	g_iPmAirHull[id] = get_pmove(pm_usehull);
	g_iPmAirTouches[id] = get_pmove(pm_numtouch);
	g_bPmAirSurf[id] = isUserSurfing(id);
	return HC_CONTINUE;
}

public rgPM_AirAccelerate_Pre(Float:wishdir[3], Float:wishspeed, Float:accel, id) {
	if (!g_bPmActive[id] || !g_bPmAirMove[id]) {
		return HC_CONTINUE;
	}
	new Float:velocity[3];
	get_pmove(pm_velocity, velocity);
	g_flPmAirPreSpeed[id] = vector_hor_length(velocity);
	new cmd = get_pmove(pm_cmd);
	g_iPmAirButtons[id] = get_ucmd(cmd, ucmd_buttons);
	get_ucmd(cmd, ucmd_viewangles, g_flPmAirAngles[id]);
	return HC_CONTINUE;
}

public rgPM_AirAccelerate_Post(Float:wishdir[3], Float:wishspeed, Float:accel, id) {
	if (g_bPmActive[id] && g_bPmAirMove[id]) {
		get_pmove(pm_velocity, g_flPmAirVelocity[id]);
		g_bPmAirAccelerated[id] = true;
	}
	return HC_CONTINUE;
}

stock collect_pm_air_stats(id, bool:isGround) {
	if (g_bPmAirSurf[id]) {
		reset_stats(id);
		return;
	}

	// A normal landing also adds a touch. Do not fail it solely for that.
	// Horizontal clipping still reveals a wall/slope hit on the landing step.
	if (get_pmove(pm_numtouch) > g_iPmAirTouches[id]) {
		if (!isGround || (g_bPmAirAccelerated[id]
			&& (floatabs(g_flVelocity[id][0] - g_flPmAirVelocity[id][0]) > 0.01
			|| floatabs(g_flVelocity[id][1] - g_flPmAirVelocity[id][1]) > 0.01))) {
			g_isTouched[id] = true;
		}
	}
	if (!g_bPmAirAccelerated[id]) {
		return;
	}

	if (g_eWhichJump[id] == jt_LongJump) {
		detect_hj(id, g_flPmAirOrigin[id], g_flFirstJump[id][2]);
	}

	if (g_iStrafes[id] >= NSTRAFES - 1) {
		return;
	}

	new iButtons = g_iPmAirButtons[id];

	new Float:flVelocity[3];
	flVelocity = g_flPmAirVelocity[id];

	new Float:flStrSpeed = vector_hor_length(flVelocity);
	new Float:flGain = flStrSpeed - g_flPmAirPreSpeed[id];

	new Float:flAngles[3];
	flAngles = g_flPmAirAngles[id];

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
	} else if (iButtons & IN_MOVELEFT && !(g_iOldStrButtons[id] & IN_MOVELEFT) && !(iButtons & (IN_MOVERIGHT|IN_BACK|IN_FORWARD)) && (isTiring)) {
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
		if (flGain > 0.0) {
			g_eStrafeStats[id][g_iStrafes[id]][st_iFrameGood] += 1;
		} else {
			g_eStrafeStats[id][g_iStrafes[id]][st_iFrameBad] += 1;
		}

	}

	if (flGain > 0.0) {
		g_eStrafeStats[id][g_iStrafes[id]][st_flSpeed] += flGain;
	} else if (flGain < 0.0) {
		g_eStrafeStats[id][g_iStrafes[id]][st_flSpeedFail] -= flGain;
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

stock reset_pm_history(id) {
	g_bPmActive[id] = false;
	g_bPmHistory[id] = false;
	g_bPmAirMove[id] = false;
	g_bPmAirAccelerated[id] = false;
	g_bPmJumped[id] = false;
	g_bPmJumpGround[id] = false;
	g_bPmJumpbugAttempt[id] = false;
	g_iPrevButtons[id] = 0;
	g_iFog[id] = 0;
	g_bOneReset[id] = true;
	g_isOldGround[id] = false;
	g_bPrevLadder[id] = false;
	g_bPrevInDuck[id] = false;
	g_bOldInDuck[0][id] = false;
	g_bOldInDuck[1][id] = false;
	g_bOldInDuck[2][id] = false;
	g_iOldStrButtons[id] = 0;
	g_flStrOldAngle[id] = 0.0;
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
	reset_pm_history(id);

	settings_player_connect(id)

	reset_stats(id);
}
