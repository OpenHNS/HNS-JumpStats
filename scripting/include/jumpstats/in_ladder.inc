public in_ladder(id, bool:isJump) {
	g_flFirstJump[id] = g_flPrevOrigin[id];

	if (!isJump) {
		normalize_ladder_start_height(id, g_flFirstJump[id], g_flFirstJump[id][2]);
	}

	g_iOldStrButtons[id] = 0;

	g_eJumpType[id] = IS_JUMP;
	g_iJumps[id] = 0;
	g_eJumpData[id][g_iJumps[id]][JUMP_PRE] = isJump ? g_flHorSpeed[id] : g_flPrevHorSpeed[id];
	g_flStartSpeed[id] = g_eJumpData[id][g_iJumps[id]][JUMP_PRE];

	g_isLadderBhop[id] = true;

	g_ePreStats[id][ptBackWards] = false;

	new Float:flViewAngles[3];
	get_entvar(id, var_v_angle, flViewAngles);

	g_eWhichJump[id] = jt_LadderJump;
	g_flLdjStartCos[id] = velocity_view_cos(g_flPrevVelocity[id], flViewAngles[1]);
	g_flLdjEndCos[id] = g_flLdjStartCos[id];
	g_flStrOldAngle[id] = flViewAngles[1];
	g_bLdjAboveStart[id] = bool:((g_bInDuck[id] ? g_flOrigin[id][2] + 18.0 : g_flOrigin[id][2]) >= g_flFirstJump[id][2]);

	show_pre(id, PRE_LADDER, g_eJumpData[id][0][JUMP_PRE]);
}
