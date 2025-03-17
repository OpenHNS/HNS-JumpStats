public in_ladder(id, bool:isJump) {
	g_flFirstJump[id] = g_flPrevOrigin[id];

	g_eJumpType[id] = IS_JUMP;
	g_iJumps[id] = 0;
	g_eJumpData[id][g_iJumps[id]][JUMP_PRE] = g_flHorSpeed[id];
	g_flStartSpeed[id] = g_flHorSpeed[id];

	g_isLadderBhop[id] = true;

	g_ePreStats[id][ptBackWards] = false;

	if (isJump) {
		show_pre(id, pre_ladder_jump, g_flHorSpeed[id]);
	} else {
		g_eWhichJump[id] = jt_LadderJump;

		g_flFirstJump[id][2] = g_flLadderXYZ[id][2] + 35.031250;

		g_flLadderDist[id] = (g_flLadderSize[id][0] <= g_flLadderSize[id][1] ?  g_flLadderSize[id][0] : g_flLadderSize[id][1]) + 0.03125;

		g_flLadderDist[id] = g_flLadderDist[id] > 10 ? 4.0 : g_flLadderDist[id];
		
		show_pre(id, pre_ladder, g_flHorSpeed[id]);
	}
}