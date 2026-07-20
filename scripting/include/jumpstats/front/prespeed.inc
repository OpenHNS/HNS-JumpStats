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

enum STRUCT_PRE {
	SP_TYPE,
	SP_FOG,
	FOG_TYPE:SP_FOGTYPE,
	SP_KZRUSH_COLOR,
	Float:SP_PRE,
	Float:SP_POST
};

enum KZRUSH_PRE_COLOR {
	KZRUSH_PRE_DEFAULT = 0,
	KZRUSH_PRE_GOOD,
	KZRUSH_PRE_BAD,
	KZRUSH_PRE_MINLOSS
};

new g_ePreData[MAX_PLAYERS + 1][STRUCT_PRE];
new Float:g_flJof[MAX_PLAYERS + 1];

new bool:g_isPre[MAX_PLAYERS + 1];
new bool:g_isJof[MAX_PLAYERS + 1];

new bool:g_isTmpPre[MAX_PLAYERS + 1];

new Float:g_flHudPreTime[MAX_PLAYERS + 1];
new Float:g_flHudJofTime[MAX_PLAYERS + 1];

new Float:g_flHudTime[MAX_PLAYERS + 1];

const Float:FOG_COLOR_TIME = 0.8;
new Float:g_flPerfTime[MAX_PLAYERS + 1];

public show_prespeed(id) {
	new Float:g_flGameTime = get_gametime();

	if (!g_isTmpPre[id]) {
		g_isTmpPre[id] = g_isPre[id];
		if (g_isTmpPre[id]) {
			g_flHudPreTime[id] = g_flGameTime + 1.0;
			g_isPre[id] = false;
		}
	} else {
		if (g_isPre[id]) {
			g_flHudPreTime[id] = g_flGameTime + 1.0;
			g_isPre[id] = false;
		}
	}

	if (g_isJof[id]) {
		g_isJof[id] = false;
		
		g_flHudJofTime[id] = g_flGameTime + 1.0;
	}

	if (g_flHudPreTime[id] < g_flGameTime) {
		g_isTmpPre[id] = false;
	}

	if (g_flHudJofTime[id] < g_flGameTime) {
		g_flJof[id] = 0.0;
	}

	new iColors[3];

	switch (g_eSettings[id][S_PRESPEED_MODE]) {
		case PRESPEED_MODE_DYNAMIC: {
			new Float:val;
			val = convertToRange(floatmin(g_flHorSpeed[id], 285.0), 40.0, 285.0);

			FormatRGBHud(id, val, iColors);
		}
		case PRESPEED_MODE_KZRUSH: {
			FormatKzRushRGBHud(id, iColors);
		}
		default: {
			iColors[0] = g_eSettings[id][S_PRESPEED_R];
			iColors[1] = g_eSettings[id][S_PRESPEED_G];
			iColors[2] = g_eSettings[id][S_PRESPEED_B];
		}
	}

	set_hudmessage(
		iColors[0], iColors[1], iColors[2],
		g_eSettings[id][S_PRESPEED_X], g_eSettings[id][S_PRESPEED_Y], 0, 1.0, 0.12, 0.0, 0.0, g_pCvar[c_iPreSpeed]);
	
	new szJof[16];

	if (g_flJof[id]) {
		formatex(szJof, charsmax(szJof), "jof: %.0f", g_flJof[id]);
	}
	new szSpeed[64];

	if (g_eOnOff[id][of_bSpeed]) {
		switch (g_eSettings[id][S_SPEED_MODE]) {
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
		
	for (new i = 1; i < MaxClients; i++) {
		if (i == id || g_isUserSpec[i] == id) {
			if (g_isUserSpec[i] && i == id) {
				continue;
			}

			new iFrame = g_eSettings[i][S_PRESPEED_FRAME];
			if (iFrame < 1) {
				iFrame = g_pCvar[C_PRESPEED_FRAME];
			}

			g_iSkip[i]++;
			if (g_iSkip[i] > iFrame) {
				g_iSkip[i] = 0;
			}

			if (g_iSkip[i]) {
				continue;
			}

			if (g_eOnOff[id][of_bPre] && g_isTmpPre[id]) {
				new iType = g_ePreData[id][SP_TYPE];
				switch (iType) {
					case PRE_JUMP..PRE_SLIDE, PRE_FALL: {
						ShowSyncHudMsg(i, g_hudPreSpeed, "\
					 		%s^n\
							%s^n^n\
							%s^n\
							%.2f",
							g_flJof[i] ? szJof : "",
							g_eOnOff[i][of_bSpeed] ? szSpeed : "",
							g_szPreType[iType],
							g_ePreData[id][SP_PRE]);
					}
					case PRE_EDGEBUG, PRE_JUMPBUG: {
						ShowSyncHudMsg(i, g_hudPreSpeed, "\
							%s^n\
							%.2f Units",
							g_szPreType[iType],
							g_ePreData[id][SP_PRE]);
					}
					case PRE_BOOST: {
						ShowSyncHudMsg(i, g_hudPreSpeed, "\
							%s^n\
							%s^n^n\
							%s %d FOG:^n\
							%.2f^n\
							%.2f",
							g_flJof[i] ? szJof : "",
							g_eOnOff[i][of_bSpeed] ? szSpeed : "",
							g_szPreType[iType], g_ePreData[id][SP_FOG],
							g_ePreData[id][SP_PRE],
							g_ePreData[id][SP_POST]);
					}
					case PRE_BHOP..PRE_DD: {
						ShowSyncHudMsg(i, g_hudPreSpeed, "\
						%s^n\
						%s^n^n\
						%s %d %s^n\
						%.2f^n\
						%.2f", 
						g_flJof[i] ? szJof : "",
						g_eOnOff[i][of_bSpeed] ? szSpeed : "",
						g_szPreType[iType], g_ePreData[id][SP_FOG], g_szFogType[g_ePreData[id][SP_FOGTYPE]],
						g_ePreData[id][SP_PRE],
						g_ePreData[id][SP_POST]);
					}
				}
			} else { 
				ShowSyncHudMsg(i, g_hudPreSpeed, "%s^n%s", g_flJof[i] ? szJof : "", g_eOnOff[i][of_bSpeed] ? szSpeed : "");
			}
		}
	}

	g_flHudTime[id] = g_flGameTime;
}

stock show_pre(id, iType, Float:flPreSpeed, iFog = 0, Float:flPostSpeed = 0.0, FOG_TYPE:fogType = FOG_VERYBAD) {
	g_isPre[id] = true;

	g_ePreData[id][SP_TYPE] = iType;
	g_ePreData[id][SP_FOG] = iFog;
	g_ePreData[id][SP_PRE] = flPreSpeed;
	g_ePreData[id][SP_POST] = flPostSpeed;
	g_ePreData[id][SP_FOGTYPE] = fogType;
	g_ePreData[id][SP_KZRUSH_COLOR] = _:GetKzRushPreColor(id, iType, iFog);

	if (fogType == FOG_PERFECT || fogType == FOG_BAD) {
		g_flPerfTime[id] = get_gametime() + FOG_COLOR_TIME;
	} else {
		g_flPerfTime[id] = 0.0;
	}
}

stock KZRUSH_PRE_COLOR:GetKzRushPreColor(id, iType, iFog) {
	if (iType >= PRE_BHOP && iType <= PRE_DUCKBHOP) {
		new Float:flMaxHorSpeed;
		get_entvar(id, var_maxspeed, flMaxHorSpeed);
		flMaxHorSpeed = floatmul(1.2, flMaxHorSpeed);
		flMaxHorSpeed = floatsqroot(floatmul(flMaxHorSpeed, flMaxHorSpeed) - 16.0);

		if (g_flPrevHorSpeed[id] <= flMaxHorSpeed) {
			if (iFog == 1) {
				return KZRUSH_PRE_GOOD;
			}

			if (g_flOldHorSpeed[id] > flMaxHorSpeed) {
				return KZRUSH_PRE_MINLOSS;
			}
		}

		return KZRUSH_PRE_BAD;
	}

	if (iType == PRE_DD) {
		if (iFog <= 2) {
			return KZRUSH_PRE_GOOD;
		}

		if (iFog == 3 && g_bOldInDuck[1][id] && g_bOldInDuck[2][id]) {
			return KZRUSH_PRE_MINLOSS;
		}

		return KZRUSH_PRE_BAD;
	}

	return KZRUSH_PRE_DEFAULT;
}

FormatKzRushRGBHud(id, colors[3]) {
	colors[0] = g_eSettings[id][S_PRESPEED_R];
	colors[1] = g_eSettings[id][S_PRESPEED_G];
	colors[2] = g_eSettings[id][S_PRESPEED_B];

	if (!g_isTmpPre[id]) {
		return;
	}

	switch (g_ePreData[id][SP_KZRUSH_COLOR]) {
		case KZRUSH_PRE_GOOD: {
			colors[0] = g_eSettings[id][S_PRE_GOOD_R];
			colors[1] = g_eSettings[id][S_PRE_GOOD_G];
			colors[2] = g_eSettings[id][S_PRE_GOOD_B];
		}
		case KZRUSH_PRE_BAD: {
			colors[0] = g_eSettings[id][S_PRE_BAD_R];
			colors[1] = g_eSettings[id][S_PRE_BAD_G];
			colors[2] = g_eSettings[id][S_PRE_BAD_B];
		}
		case KZRUSH_PRE_MINLOSS: {
			colors[0] = g_eSettings[id][S_PRE_MINLOSS_R];
			colors[1] = g_eSettings[id][S_PRE_MINLOSS_G];
			colors[2] = g_eSettings[id][S_PRE_MINLOSS_B];
		}
	}
}

stock show_jof(id, Float:flJof) {
	if (!g_eOnOff[id][of_bJof]) {
		return;
	}

	g_isJof[id] = true;

	g_flJof[id] = flJof;
}

FormatRGBHud(id, const Float:val, colors[3]) {
	new iColorSpeedMin[3], iColorSpeedMax[3], iColorBase[3];

	iColorSpeedMin = {255, 255, 255};
	iColorSpeedMax = {0, 250, 0};

	iColorBase[0] = floatround(float(iColorSpeedMax[0] - iColorSpeedMin[0]) * val + iColorSpeedMin[0]);
	iColorBase[1] = floatround(float(iColorSpeedMax[1] - iColorSpeedMin[1]) * val + iColorSpeedMin[1]);
	iColorBase[2] = floatround(float(iColorSpeedMax[2] - iColorSpeedMin[2]) * val + iColorSpeedMin[2]);

	new Float:flNow = get_gametime();
	if (g_flPerfTime[id] > flNow) {
		new Float:fade = floatclamp((g_flPerfTime[id] - flNow) / FOG_COLOR_TIME, 0.0, 1.0);
		new iColorFog[3];

		if (g_ePreData[id][SP_FOGTYPE] == FOG_PERFECT) {
			iColorFog = {20, 255, 150};
		} else if (g_ePreData[id][SP_FOGTYPE] == FOG_BAD) {
			iColorFog = {255, 70, 120};
		} else {
			fade = 0.0;
		}

		colors[0] = floatround(float(iColorFog[0] - iColorBase[0]) * fade + iColorBase[0]);
		colors[1] = floatround(float(iColorFog[1] - iColorBase[1]) * fade + iColorBase[1]);
		colors[2] = floatround(float(iColorFog[2] - iColorBase[2]) * fade + iColorBase[2]);
	} else {
		colors[0] = iColorBase[0];
		colors[1] = iColorBase[1];
		colors[2] = iColorBase[2];
	}
}

stock Float: convertToRange(Float:value, Float:FromMin, Float:FromMax, Float:ToMin = 0.0, Float:ToMax = 1.0) {
	return floatclamp((value-FromMin) / (FromMax-FromMin) * (ToMax-ToMin + ToMin), ToMin, ToMax);
}
