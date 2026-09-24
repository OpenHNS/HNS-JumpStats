enum _:PRE_TYPE {
	PRE_FOG = 0,
	PRE_JUMP,
	PRE_DUCK,
	PRE_LADDER,
	PRE_SLIDE,
	PRE_BOOST,
	PRE_FALL,
	PRE_EDGEBUG,
	PRE_BHOP,
	PRE_SBJ,
	PRE_BID,
	PRE_DUCKBHOP,
	PRE_DD,
	PRE_JUMPBUG
};

new g_szPreType[PRE_TYPE][] = {
	"",
	"jump",
	"duck",
	"ladder",
	"slide",
	"boost",
	"fall",
	"EdgeBug",
	"bhop",
	"sbj",
	"bid",
	"duckbhop",
	"dd",
	"JumpBug"
};

enum FOG_TYPE {
	FOG_VERYBAD = 0,
	FOG_PERFECT,
	FOG_GOOD,
	FOG_BAD
};

new g_szFogType[FOG_TYPE][] = {
	"[VB]",
	"[P]",
	"[G]",
	"[B]"
};

new Float:g_flJof[MAX_PLAYERS + 1];

new bool:g_isJof[MAX_PLAYERS + 1];

new Float:g_flHudJofTime[MAX_PLAYERS + 1];

const Float:PRE_HUD_TIME = 2.5;

public show_speed(id) {
	new Float:g_flGameTime = get_gametime();

	if (g_isJof[id]) {
		g_isJof[id] = false;
		
		g_flHudJofTime[id] = g_flGameTime + 1.0;
	}

	if (g_flHudJofTime[id] < g_flGameTime) {
		g_flJof[id] = 0.0;
	}

	new szJof[16];

	if (g_flJof[id]) {
		formatex(szJof, charsmax(szJof), "jof: %.0f", g_flJof[id]);
	}

	for (new i = 1; i <= MaxClients; i++) {
		if (i == id || g_isUserSpec[i] == id) {
			if (g_isUserSpec[i] && i == id) {
				continue;
			}
			if (!g_eOnOff[i][of_bSpeed] && !g_eOnOff[i][of_bJof]) {
				continue;
			}

			new iFrame = g_eSettings[i][S_SPEED_FRAME];
			if (iFrame < 1) {
				iFrame = g_pCvar[C_SPEED_FRAME];
			}

			g_iSkip[i]++;
			if (g_iSkip[i] > iFrame) {
				g_iSkip[i] = 0;
			}

			if (g_iSkip[i]) {
				continue;
			}

			new iColors[3];
			FormatSpeedRGBHud(id, i, iColors);

			new szSpeed[64];
			if (g_eOnOff[i][of_bSpeed]) {
				switch (g_eSettings[i][S_SPEED_MODE]) {
					case SPEED_MODE_QUAKE: {
						formatex(szSpeed, charsmax(szSpeed), "%.0f units/seconds^n%.0f velocity", g_flHorSpeed3D[id], g_flHorSpeed[id]);
					}
					case SPEED_MODE_NUMBER: {
						formatex(szSpeed, charsmax(szSpeed), "%.0f", g_flHorSpeed[id]);
					}
					default: {
						formatex(szSpeed, charsmax(szSpeed), "%.0f u/s", g_flHorSpeed[id]);
					}
				}
			}

			set_hudmessage(
				iColors[0], iColors[1], iColors[2],
				g_eSettings[i][S_SPEED_X], g_eSettings[i][S_SPEED_Y], 0, 1.0, 0.12, 0.0, 0.0, g_pCvar[c_iSpeed]);

			ShowSyncHudMsg(i, g_hudSpeed, "%s^n%s",
				g_eOnOff[i][of_bJof] && g_flJof[id] ? szJof : "",
				g_eOnOff[i][of_bSpeed] ? szSpeed : "");
		}
	}

}

stock show_pre(id, iType, Float:flPreSpeed, iFog = 0, Float:flPostSpeed = 0.0, FOG_TYPE:fogType = FOG_VERYBAD) {
	if (!g_pCvar[C_ENABLE_PRE]) {
		return;
	}

	new iColors[3];
	iColors[0] = g_eSettings[id][S_PRE_R];
	iColors[1] = g_eSettings[id][S_PRE_G];
	iColors[2] = g_eSettings[id][S_PRE_B];

	if (iType == PRE_BOOST || iType == PRE_JUMPBUG || iType == PRE_EDGEBUG) {
		iColors[0] = g_eSettings[id][S_PRE_GOOD_R];
		iColors[1] = g_eSettings[id][S_PRE_GOOD_G];
		iColors[2] = g_eSettings[id][S_PRE_GOOD_B];
	} else if (iType >= PRE_BHOP && iType <= PRE_DUCKBHOP) {
		new Float:flMaxHorSpeed;
		get_entvar(id, var_maxspeed, flMaxHorSpeed);
		flMaxHorSpeed = floatmul(1.2, flMaxHorSpeed);
		flMaxHorSpeed = floatsqroot(floatmul(flMaxHorSpeed, flMaxHorSpeed) - 16.0);

		if (g_flPrevHorSpeed[id] <= flMaxHorSpeed && iFog == 1) {
			iColors[0] = g_eSettings[id][S_PRE_GOOD_R];
			iColors[1] = g_eSettings[id][S_PRE_GOOD_G];
			iColors[2] = g_eSettings[id][S_PRE_GOOD_B];
		} else if (g_flPrevHorSpeed[id] <= flMaxHorSpeed && g_flOldHorSpeed[id] > flMaxHorSpeed) {
			iColors[0] = g_eSettings[id][S_PRE_MINLOSS_R];
			iColors[1] = g_eSettings[id][S_PRE_MINLOSS_G];
			iColors[2] = g_eSettings[id][S_PRE_MINLOSS_B];
		} else {
			iColors[0] = g_eSettings[id][S_PRE_BAD_R];
			iColors[1] = g_eSettings[id][S_PRE_BAD_G];
			iColors[2] = g_eSettings[id][S_PRE_BAD_B];
		}
	} else if (iType == PRE_DD) {
		if (iFog <= 2) {
			iColors[0] = g_eSettings[id][S_PRE_GOOD_R];
			iColors[1] = g_eSettings[id][S_PRE_GOOD_G];
			iColors[2] = g_eSettings[id][S_PRE_GOOD_B];
		} else if (iFog == 3 && g_bOldInDuck[1][id] && g_bOldInDuck[2][id]) {
			iColors[0] = g_eSettings[id][S_PRE_MINLOSS_R];
			iColors[1] = g_eSettings[id][S_PRE_MINLOSS_G];
			iColors[2] = g_eSettings[id][S_PRE_MINLOSS_B];
		} else {
			iColors[0] = g_eSettings[id][S_PRE_BAD_R];
			iColors[1] = g_eSettings[id][S_PRE_BAD_G];
			iColors[2] = g_eSettings[id][S_PRE_BAD_B];
		}
	}

	new szPre[96];

	switch (iType) {
		case PRE_JUMP..PRE_SLIDE, PRE_FALL: {
			formatex(szPre, charsmax(szPre), "%s^n%.2f",
				g_szPreType[iType],
				flPreSpeed);
		}
		case PRE_EDGEBUG, PRE_JUMPBUG: {
			formatex(szPre, charsmax(szPre), "%s^n%.2f Units",
				g_szPreType[iType],
				flPreSpeed);
		}
		case PRE_BOOST: {
			formatex(szPre, charsmax(szPre), "%s^n%.2f^n%.2f",
				g_szPreType[iType],
				flPreSpeed,
				flPostSpeed);
		}
		case PRE_BHOP..PRE_DD: {
			formatex(szPre, charsmax(szPre), "%s %d %s^n%.2f^n%.2f",
				g_szPreType[iType], iFog, g_szFogType[fogType],
				flPreSpeed,
				flPostSpeed);
		}
	}

	for (new i = 1; i <= MaxClients; i++) {
		if ((i != id && g_isUserSpec[i] != id) || !g_eOnOff[i][of_bPre]) {
			continue;
		}

		set_hudmessage(
			iColors[0], iColors[1], iColors[2],
			g_eSettings[i][S_PRE_X], g_eSettings[i][S_PRE_Y], 0, 6.0, PRE_HUD_TIME, 0.1, 0.3, g_pCvar[c_iPre]);

		ShowSyncHudMsg(i, g_hudPre, "%s", szPre);
	}

}

stock show_special_stats(id, iType, Float:flValue) {
	if (!g_pCvar[C_ENABLE_STATS]) {
		return;
	}

	for (new i = 1; i <= MaxClients; i++) {
		if ((i != id && g_isUserSpec[i] != id) || !g_eSettings[i][S_STATS]) {
			continue;
		}

		set_hudmessage(
			g_eSettings[i][S_STATS_R], g_eSettings[i][S_STATS_G], g_eSettings[i][S_STATS_B],
			g_eSettings[i][S_STATS_X], g_eSettings[i][S_STATS_Y], 0, 6.0, 2.5, 0.1, 0.3, g_pCvar[c_iStats]);

		switch (iType) {
			case PRE_JUMPBUG, PRE_EDGEBUG: {
				ShowSyncHudMsg(i, g_hudStats, "%s^ndistance: %.2f units", g_szPreType[iType], flValue);
			}
		}
	}
}

stock show_boost_chat(id, Float:flBefore, Float:flAfter) {
	if (!g_pCvar[C_ENABLE_CHATINFO] || !g_eSettings[id][S_CHATINFO]) {
		return;
	}

	new Float:flGainPercent;
	if (flBefore > 0.0) {
		flGainPercent = (flAfter - flBefore) * 100.0 / flBefore;
	}

	new iPlayers[MAX_PLAYERS], iNum;
	get_players(iPlayers, iNum, "ch");

	for (new i = 0; i < iNum; i++) {
		client_print_color(iPlayers[i], print_team_blue, "^3%n^1 performed ^3BOOST^1 (^3%.0f^1 => ^3%.0f^1 gain ^3%.0f%%^1)", id, flBefore, flAfter, flGainPercent);
	}
}

stock show_boost_stats(id, Float:flBefore, Float:flAfter) {
	if (!g_pCvar[C_ENABLE_STATS]) {
		return;
	}

	new Float:flGainPercent;
	if (flBefore > 0.0) {
		flGainPercent = (flAfter - flBefore) * 100.0 / flBefore;
	}

	for (new i = 1; i <= MaxClients; i++) {
		if ((i != id && g_isUserSpec[i] != id) || !g_eSettings[i][S_STATS]) {
			continue;
		}

		set_hudmessage(
			g_eSettings[i][S_STATS_R], g_eSettings[i][S_STATS_G], g_eSettings[i][S_STATS_B],
			g_eSettings[i][S_STATS_X], g_eSettings[i][S_STATS_Y], 0, 6.0, 2.5, 0.1, 0.3, g_pCvar[c_iStats]);

		ShowSyncHudMsg(i, g_hudStats, "BOOST^n%.0f => %.0f^ngain %.0f%%", flBefore, flAfter, flGainPercent);
	}
}

stock show_jof(id, Float:flJof) {
	if (!g_pCvar[C_ENABLE_SPEED]) {
		return;
	}

	g_isJof[id] = true;

	g_flJof[id] = flJof;
}

FormatSpeedRGBHud(id, iViewer, colors[3]) {
	new Float:val = convertToRange(floatmin(g_flHorSpeed[id], 285.0), 40.0, 285.0);
	FormatSpeedGradient(iViewer, val, colors);
}

FormatSpeedGradient(const id, const Float:val, colors[3]) {
	colors[0] = floatround(float(g_eSettings[id][S_SPEED_MAX_R] - g_eSettings[id][S_SPEED_MIN_R]) * val + g_eSettings[id][S_SPEED_MIN_R]);
	colors[1] = floatround(float(g_eSettings[id][S_SPEED_MAX_G] - g_eSettings[id][S_SPEED_MIN_G]) * val + g_eSettings[id][S_SPEED_MIN_G]);
	colors[2] = floatround(float(g_eSettings[id][S_SPEED_MAX_B] - g_eSettings[id][S_SPEED_MIN_B]) * val + g_eSettings[id][S_SPEED_MIN_B]);
}

stock Float: convertToRange(Float:value, Float:FromMin, Float:FromMax, Float:ToMin = 0.0, Float:ToMax = 1.0) {
	return floatclamp((value-FromMin) / (FromMax-FromMin) * (ToMax-ToMin + ToMin), ToMin, ToMax);
}
