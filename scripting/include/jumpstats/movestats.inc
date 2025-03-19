

public move_stats_counter(id, MOVE_TYPE:eMove, iFog) {
	if (g_eSessionMoveType[id] && (g_eSessionMoveType[id] != eMove)) {
		check_and_show_move(id);
	}

	g_eSessionMoveType[id] = eMove;

	g_eMoveStats[id][STATS_COUNT]++;

	if (g_eMoveStats[id][STATS_COUNT] >= 5) {
		isSessionMove[id] = true;
	}

	switch(eMove) {
		case MOVE_BHOP: {
			if (g_flHorSpeed[id] < g_flMaxSpeed[id] && (iFog == 1 || iFog >= 2 && g_flPrevHorSpeed[id] > g_flMaxSpeed[id])) {
				g_eMoveStats[id][STATS_FOG][FOG_PERFECT]++;
			} else {
				switch(iFog) {
					case 1..2: g_eMoveStats[id][STATS_FOG][FOG_GOOD]++;
					default: g_eMoveStats[id][STATS_FOG][FOG_BAD]++;
				}
			}
		}
		case MOVE_SGS: {
			switch(g_iFog[id]) {
				case 3: g_eMoveStats[id][STATS_FOG][FOG_PERFECT]++;
				case 4: g_eMoveStats[id][STATS_FOG][FOG_GOOD]++;
				default: g_eMoveStats[id][STATS_FOG][FOG_BAD]++;
			}

			if (g_bMoveOldSurfing[id]) {
				g_eMoveStats[id][SATS_ARTIFACT] = ARTIFACT_SURF;
			}
		}
		case MOVE_DDRUN: {
			switch(g_iFog[id]) {
				case 2: g_eMoveStats[id][STATS_FOG][FOG_PERFECT]++;
				case 3: g_eMoveStats[id][STATS_FOG][FOG_GOOD]++;
				default: g_eMoveStats[id][STATS_FOG][FOG_BAD]++;
			}

			if (g_bMoveOldSurfing[id]) {
				g_eMoveStats[id][SATS_ARTIFACT] = ARTIFACT_SURF;
			}
		}
	}
	

	g_eMoveStats[id][STATS_AVG_SPEED] += g_flHorSpeed[id];

	g_eMoveStats[id][STATS_PRECENT] = float(g_eMoveStats[id][STATS_FOG][FOG_PERFECT]) / float(g_eMoveStats[id][STATS_COUNT]) * 100.0;
}

public clear_move_stats(id) {
	isSessionMove[id] = false;
	g_eSessionMoveType[id] = MOVE_NOT;
	g_eMoveStats[id][STATS_COUNT] = 0;

	g_eMoveStats[id][STATS_FOG][FOG_PERFECT] = 0;
	g_eMoveStats[id][STATS_FOG][FOG_GOOD] = 0;
	g_eMoveStats[id][STATS_FOG][FOG_BAD] = 0;

	g_eMoveStats[id][STATS_PRECENT] = 0.0;

	g_eMoveStats[id][STATS_AVG_SPEED] = 0.0;

	g_eMoveStats[id][SATS_ARTIFACT] = ARTIFACT_SMESTA;
}