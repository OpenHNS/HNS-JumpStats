stock in_fall_js(id, iFog, bool:isDuck, bool:isJump, Float:flFrameTime) {
	if (g_bPrevLadder[id] || !is_falling_pre_frame(id, flFrameTime)) {
		return;
	}

	new bool:bBoost = bool:(iFog == 1 && g_flPrevHorSpeed[id] > g_flOldHorSpeed[id] + 5.0);

	if (bBoost) {
		ready_boost_stats(id, g_flOldHorSpeed[id], g_flPrevHorSpeed[id]);
	}

	if (isDuck || isJump) {
		return;
	}

	if (iFog > 10) {
		g_isFalling[id] = true;
		g_eJumpType[id] = IS_JUMP;
		g_iJumps[id] = 0;
		g_eJumpData[id][g_iJumps[id]][JUMP_PRE] = g_flHorSpeed[id];
		show_pre(id, PRE_FALL, g_flHorSpeed[id]);
	} else if (bBoost) {
		show_pre(id, PRE_BOOST, g_flOldHorSpeed[id], iFog, g_flPrevHorSpeed[id]);
	} else { // TODO: Лучше сделать отдельно детекторы типа, и для фалл ран придумать цвета
		show_pre(id, PRE_FALLRUN, g_flPreHorSpeed[id], iFog, g_flPrevHorSpeed[id], get_dd_fog_type(iFog));
	}
}

stock bool:is_falling_pre_frame(id, Float:flFrameTime) {
	new Float:flThresholdFrameTime = g_flPrevFrameTime[id] > 0.0 ? g_flPrevFrameTime[id] : flFrameTime;
	if (g_bSlide[id] || flThresholdFrameTime <= 0.0 || g_flVelocity[id][2] >= 0.0) {
		return false;
	}

	new Float:flPlayerGravity;
	get_entvar(id, var_gravity, flPlayerGravity);
	if (flPlayerGravity <= 0.0) {
		flPlayerGravity = 1.0;
	}

	new Float:flFallThreshold = g_pCvar[c_iGravity] * flPlayerGravity * flThresholdFrameTime * 0.5;
	return bool:(g_flVelocity[id][2] <= -flFallThreshold);
}
