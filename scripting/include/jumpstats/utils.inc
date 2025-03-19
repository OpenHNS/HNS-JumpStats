stock reset_stats(id) {
	g_eWhichJump[id] = jt_Not;

	arrayset(g_eJumpstats[id], 0, JUMPSTATS);
	g_eJumpstats[id][js_iJumpBlock] = 1000;

	arrayset(g_ePreStats[id], false, PRESTATS);

	reset_strafes(id);

	g_iVerInfo[id] = IS_MIDDLE;
	g_flDuckFirstZ[id] = 0.0;
	g_flJumpFirstZ[id] = 0.0;

	g_eDuckType[id] = IS_DUCK_NOT;
	for (new i = 0; i < NDUCKS; i++) {
   		arrayset(g_eDuckData[id][i], 0, STATS_DUCKS);
	}
	g_iDucks[id] = 0;

	g_eJumpType[id] = IS_JUMP_NOT;
	for (new i = 0; i < NJUMPS; i++) {
   		arrayset(g_eJumpData[id][i], 0, STATS_JUMPS);
	}
	g_iJumps[id] = 0;

	g_eFailJump[id] = fj_good;

	arrayset(g_flFirstJump[id], 0.0, 3);
	
	g_isLadderBhop[id] = false;
	g_isFalling[id] = false;
	g_isTouched[id] = false;

	g_iDetectHJ[id] = 0;
	g_flStartSpeed[id] = 0.0;

	g_bMoveOldSurfing[id] = false;
}

stock reset_strafes(id) {
	for (new i = 0; i < NSTRAFES; i++) {
		arrayset(g_eStrafeStats[id][i], 0, StrafeStats);
	}
	
	g_iStrafes[id] = 0;
	g_iStrAllEff[id] = 0;
	g_flStrGain[id] = 0.0;
}


stock Float:vector_hor_length(Float:flVel[3]) {
	new Float:flNorma = floatpower(flVel[0], 2.0) + floatpower(flVel[1], 2.0);
	if (flNorma > 0.0)
		return floatsqroot(flNorma);
		
	return 0.0;
}

stock detect_hj(id, Float:flOrigin[3], Float:VerOrigin) {
	new Float:flStarthj[3];
	flStarthj[0] = flOrigin[0];
	flStarthj[1] = flOrigin[1];
	flStarthj[2] = VerOrigin + 28.0;

	new Float:flStophj[3]
	flStophj[0] = flOrigin[0];
	flStophj[1] = flOrigin[1];
	flStophj[2] = flStarthj[2] - 133.0;

	engfunc(EngFunc_TraceLine, flStarthj, flStophj, IGNORE_MONSTERS, id, 0);

	new Float:flEndHJ[3];

	get_tr2(0, TR_vecEndPos, flEndHJ);

	if (flStarthj[2] - flEndHJ[2] < 133.0 && (flStarthj[2] - flEndHJ[2] - 64) != 0 && (flStarthj[2] - flEndHJ[2] - 64) > 0 && g_iDetectHJ[id] != 1) {
		g_iDetectHJ[id] = 2;
	}

	if (flStarthj[2] - flEndHJ[2] >= 133.0 && g_iDetectHJ[id] != 2) {
		g_iDetectHJ[id] = 1;
	}
}

public get_colorchat_by_distance(id, aDistance[ChatDist], JumpType:iJumpType) {
	new bool:isSotka = bool:(get_movevar(mv_airaccelerate) > 10.0);

	if (g_ePreStats[id][ptLadderBhop] && iJumpType == jt_BhopJump || iJumpType ==  jt_StandUpBhopJump || iJumpType ==  jt_BhopInDuck) {
		aDistance[god] = isSotka ? 270 : 260;
		aDistance[leet] = isSotka ? 265 : 255;
		aDistance[holy] = isSotka ? 260 : 250;
		aDistance[pro] = isSotka ? 255 : 245;
		aDistance[good] = isSotka ? 250 : 240;

		return PLUGIN_HANDLED;
	}

	switch (iJumpType) {
		case jt_LongJump, jt_HighJump: {
			aDistance[god] = isSotka ? 260 : 255;
			aDistance[leet] = isSotka ? 255 : 253;
			aDistance[holy] = isSotka ? 250 : 250;
			aDistance[pro] = 245
			aDistance[good] = 240;
		}
		case jt_CountJump, jt_StandUPCountJump, jt_WeirdJump: {
			aDistance[god] = 270;
			aDistance[leet] = 265;
			aDistance[holy] = 260;
			aDistance[pro] = 255;
			aDistance[good] = 250;
		}
		case jt_Multi_CountJump, jt_Multi_StandUPCountJump: {
			aDistance[god] = isSotka ? 270 : 260;
			aDistance[leet] = isSotka ? 265 : 255;
			aDistance[holy] = isSotka ? 260 : 250;
			aDistance[pro] = isSotka ? 255 : 245;
			aDistance[good] = isSotka ? 250 : 240;
		}
		case jt_LadderJump: {
			aDistance[god] = 190;
			aDistance[leet] = 180;
			aDistance[holy] = 170;
			aDistance[pro] = 160;
			aDistance[good] = 150;
		}
		case jt_BhopJump, jt_StandUpBhopJump: {
			aDistance[god] = 247;
			aDistance[leet] = 245;
			aDistance[holy] = 240;
			aDistance[pro] = 235;
			aDistance[good] = 230;
		}
		case jt_BhopInDuck, jt_DuckBhopJump: {
			aDistance[god] = 220;
			aDistance[leet] = 217;
			aDistance[holy] = 215;
			aDistance[pro] = 210;
			aDistance[good] = 205;
		}
	}

	return PLUGIN_HANDLED;
}

public calc_jof_block(id, Float:flJumpOff[3]) {
	new Float:flOrigin[3];
	new Float:tmpblock[3], tmpjblock[3], Float:tmpOrigin[3], Float:tmpOrigin2[3];
	flOrigin = flJumpOff;
	flOrigin[2] = flOrigin[2] - 36.1;

	new block_checking[3];

	for (new i = 0, j = -18; i < 3; i++, j = j + 18) {
		tmpOrigin = flOrigin;
		tmpOrigin2 = flOrigin;
		tmpblock = flOrigin;
		if (g_flVelocity[id][1] > 0 && floatabs(g_flVelocity[id][1]) > floatabs(g_flVelocity[id][0])) {
			tmpOrigin[1] = flOrigin[1] + 100;
			tmpOrigin2[1] = flOrigin[1] - 16;

			tmpOrigin[0] = tmpOrigin[0] + j;
			tmpOrigin2[0] = tmpOrigin2[0] + j;

			tmpblock[1] = flOrigin[1] + 290.0 + 1;
			tmpblock[0] = tmpblock[0] + j;
		} else if (g_flVelocity[id][1] < 0 && floatabs(g_flVelocity[id][1]) > floatabs(g_flVelocity[id][0])) {
			tmpOrigin[1] = flOrigin[1] - 100;
			tmpOrigin2[1] = flOrigin[1] + 16;

			tmpOrigin[0] = tmpOrigin[0] + j;
			tmpOrigin2[0] = tmpOrigin2[0] + j;

			tmpblock[1] = flOrigin[1] - 290.0 + 1;
			tmpblock[0] = tmpblock[0] + j;
		} else if (g_flVelocity[id][0] > 0 && floatabs(g_flVelocity[id][0]) > floatabs(g_flVelocity[id][1])) {
			tmpOrigin[0] = flOrigin[0] + 100;
			tmpOrigin2[0] = flOrigin[0] - 16;

			tmpOrigin[1] = tmpOrigin[1] + j;
			tmpOrigin2[1] = tmpOrigin2[1] + j;

			tmpblock[0] = flOrigin[0] + 290.0 + 1;
			tmpblock[1] = tmpblock[1] + j;
		} else if (g_flVelocity[id][0] < 0 && floatabs(g_flVelocity[id][0]) > floatabs(g_flVelocity[id][1])) {
			tmpOrigin[0] = flOrigin[0] - 100;
			tmpOrigin2[0] = flOrigin[0] + 16;

			tmpOrigin[1] = tmpOrigin[1] + j;
			tmpOrigin2[1] = tmpOrigin2[1] + j;

			tmpblock[0] = flOrigin[0] - 290.0 + 1;
			tmpblock[1] = tmpblock[1] + j;
		}

		new Float:tmpEdgeOrigin[3];

		engfunc(EngFunc_TraceLine, tmpOrigin, tmpOrigin2, IGNORE_GLASS, id, 0);
		get_tr2(0, TR_vecEndPos, tmpEdgeOrigin);

		if (get_distance_f(tmpEdgeOrigin, tmpOrigin2) != 0.0) {
			g_eJumpstats[id][js_flJof] = get_distance_f(tmpEdgeOrigin, tmpOrigin2) - 0.031250;
			show_jof(id, g_eJumpstats[id][js_flJof]);
		}

		new Float:tmpblockOrigin[3];

		engfunc(EngFunc_TraceLine, tmpEdgeOrigin, tmpblock, IGNORE_GLASS, id, 0);
		get_tr2(0, TR_vecEndPos, tmpblockOrigin);

		if (get_distance_f(tmpblockOrigin, tmpEdgeOrigin) != 0.0) {
			tmpjblock[i] = floatround(get_distance_f(tmpblockOrigin, tmpEdgeOrigin), floatround_floor) + 1;
		}

		new Float:checkblock1[3], Float:checkblock2[3];
		tmpblockOrigin[2] = tmpblockOrigin[2] - 1.0;

		checkblock1 = tmpblockOrigin;

		if (g_flVelocity[id][1] > 0 && floatabs(g_flVelocity[id][1]) > floatabs(g_flVelocity[id][0])) {
			checkblock1[1] = checkblock1[1] + 2.0;
		} else if (g_flVelocity[id][1] < 0 && floatabs(g_flVelocity[id][1]) > floatabs(g_flVelocity[id][0])) {
			checkblock1[1] = checkblock1[1] - 2.0;
		} else if (g_flVelocity[id][0] > 0 && floatabs(g_flVelocity[id][0]) > floatabs(g_flVelocity[id][1])) {
			checkblock1[0] = checkblock1[0] + 2.0;
		} else if (g_flVelocity[id][0] < 0 && floatabs(g_flVelocity[id][0]) > floatabs(g_flVelocity[id][1])) {
			checkblock1[0] = checkblock1[0] - 2.0;
		}

		checkblock2 = checkblock1;
		checkblock2[2] = checkblock2[2] + 18.0;

		new Float:tmpcheckblock[3];
		engfunc(EngFunc_TraceLine, checkblock2, checkblock1, IGNORE_GLASS, id, 0);
		get_tr2(0, TR_vecEndPos, tmpcheckblock);

		if (floatabs(tmpblockOrigin[2] - tmpcheckblock[2]) == 0.0) {
			block_checking[i] = 1;
		}
	}

	if (tmpjblock[0] != 0 && tmpjblock[0] <= tmpjblock[1] && tmpjblock[0] <= tmpjblock[2]) {
		if (!block_checking[0])
			g_eJumpstats[id][js_iJumpBlock] = tmpjblock[0];
	} else if (tmpjblock[1] != 0 && tmpjblock[1] <= tmpjblock[2] && tmpjblock[0] <= tmpjblock[0]) {
		if (!block_checking[1])
			g_eJumpstats[id][js_iJumpBlock] = tmpjblock[1];
	} else if (tmpjblock[2] != 0 && tmpjblock[2] <= tmpjblock[1] && tmpjblock[0] <= tmpjblock[0]) {
		if (!block_checking[2])
			g_eJumpstats[id][js_iJumpBlock] = tmpjblock[2];
	} else g_eJumpstats[id][js_iJumpBlock] = 0;
}

stock bool:isUserSurfing(id) {
	new Float:origin[3], Float:dest[3];
	get_entvar(id, var_origin, origin);
	
	dest[0] = origin[0];
	dest[1] = origin[1];
	dest[2] = origin[2] - 1.0;

	new Float:flFraction;

	engfunc(EngFunc_TraceHull, origin, dest, 0, 
		g_bInDuck[id] ? HULL_HEAD : HULL_HUMAN, id, 0);

	get_tr2(0, TR_flFraction, flFraction);

	if (flFraction >= 1.0) return false;
	
	get_tr2(0, TR_vecPlaneNormal, dest);

	return dest[2] <= 0.7;
}

stock Float:get_maxspeed(id) {
	new Float:flMaxSpeed;
	flMaxSpeed = get_entvar(id, var_maxspeed);
	
	return flMaxSpeed * 1.2;
}