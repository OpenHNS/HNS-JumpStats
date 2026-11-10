stock in_jumpbug_js(id, iButtons) {
	if (!g_eJumpBugData[id][JB_CHECK]) {
		return;
	}

	new bool:isDuckRelease = !(iButtons & IN_DUCK) && (g_iPrevButtons[id] & IN_DUCK);
	new bool:isJumpPress = (iButtons & IN_JUMP) && !(g_iPrevButtons[id] & IN_JUMP);

	if (!g_eJumpBugData[id][JB_FRAMES]) {
		if (isDuckRelease && isJumpPress && g_flVelocity[id][2] < 0.0) {
			g_eJumpBugData[id][JB_START_ORIGIN] = g_flOrigin[id];
			g_eJumpBugData[id][JB_FRAMES] = 2;
		}
		return;
	}

	g_eJumpBugData[id][JB_FRAMES]--;
	if (g_eJumpBugData[id][JB_FRAMES] || g_flVelocity[id][2] <= 0.0) {
		return;
	}

	new Float:flJumpbugLandingZ;
	new Float:flJumpbugStart[3];
	flJumpbugStart[0] = g_eJumpBugData[id][JB_START_ORIGIN][0];
	flJumpbugStart[1] = g_eJumpBugData[id][JB_START_ORIGIN][1];
	flJumpbugStart[2] = g_eJumpBugData[id][JB_START_ORIGIN][2];

	if (!find_jumpbug_landing(id, flJumpbugStart, flJumpbugLandingZ)) {
		return;
	}

	new Float:flJumpbugDistance = float(floatround(g_eJumpBugData[id][JB_LAST_GROUND_Z])) - flJumpbugLandingZ;
	if (flJumpbugDistance <= 0.0) {
		return;
	}

	g_eJumpBugData[id][JB_CHECK] = false;
	show_pre(id, PRE_JUMPBUG, flJumpbugDistance);
	ready_bug_stats(id, bt_JumpBug, flJumpbugDistance);

	if (g_eWhichJump[id] != jt_Not) {
		reset_stats(id);
		g_eFailJump[id] = fj_notshow;
	}
}

stock in_edgebug_js(id, Float:flFrameTime) {
	if (!g_eEdgeBugData[id][EB_CHECK] || g_flVelocity[id][2] >= 0.0) {
		return;
	}

	new Float:flPlayerGravity;
	get_entvar(id, var_gravity, flPlayerGravity);

	if (flPlayerGravity <= 0.0) {
		flPlayerGravity = 1.0;
	}

	new Float:flDelta = g_pCvar[c_iGravity] * flPlayerGravity * flFrameTime;
	new Float:flTarget = -flDelta * 0.5;
	new Float:flCurrentVz = g_flVelocity[id][2];
	new Float:flPrevVz = g_flPrevVelocity[id][2];

	if (floatabs(flCurrentVz - flTarget) > 1.0 || flPrevVz >= flTarget || !isGoingToTouchGround(id, flFrameTime, flPlayerGravity)) {
		return;
	}

	new Float:flDistance = g_eEdgeBugData[id][EB_LAST_GROUND_Z] - g_flOrigin[id][2];
	if (flDistance <= 0.0) {
		return;
	}

	g_eEdgeBugData[id][EB_CHECK] = false;

	show_pre(id, PRE_EDGEBUG, flDistance);
	ready_bug_stats(id, bt_EdgeBug, flDistance);

	if (g_eWhichJump[id] != jt_Not) {
		reset_stats(id);
		g_eFailJump[id] = fj_notshow;
	}
}


stock bool:isGoingToTouchGround(id, Float:flFrameTime, Float:flPlayerGravity) {
	if (flFrameTime <= 0.0) {
		return false;
	}

	new Float:flVelocity[3], Float:flDestination[3];
	flVelocity = g_flPrevVelocity[id];
	flVelocity[2] -= g_pCvar[c_iGravity] * flPlayerGravity * flFrameTime;

	for (new i = 0; i < 3; i++) {
		flDestination[i] = g_flPrevOrigin[id][i] + flVelocity[i] * flFrameTime;
	}

	new iTrace = create_tr2();
	engfunc(EngFunc_TraceHull, g_flPrevOrigin[id], flDestination, DONT_IGNORE_MONSTERS, g_bInDuck[id] ? HULL_HEAD : HULL_HUMAN, id, iTrace);

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
