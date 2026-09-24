new const MENU_ID[] = "JUMPSTATS_MENU";
new const ENABLE_MENU_ID[] = "JUMPSTATS_ENABLE_MENU";
new const HUD_COLOR_MENU_ID[] = "JUMPSTATS_HUD_COLOR_MENU";
new const HUD_COLOR_EDIT_MENU_ID[] = "JUMPSTATS_HUD_COLOR_EDIT_MENU";
new const HUD_COLOR_INPUT_CMD[] = "JS_HUD_COLOR_INPUT";
new const HUD_POSITION_MENU_ID[] = "JUMPSTATS_HUD_POSITION_MENU";
new const HUD_POSITION_EDIT_MENU_ID[] = "JUMPSTATS_HUD_POSITION_EDIT_MENU";
new const HUD_POSITION_INPUT_CMD[] = "JS_HUD_POSITION_INPUT";

const Float:HUD_POSITION_DISPLAY_EPSILON = 0.0001;
new const SPEED_CONFIG_MENU_ID[] = "JUMPSTATS_SPEED_CONFIG_MENU";

enum _:HUD_COLOR_TARGET {
	HC_TARGET_NONE,
	HC_TARGET_STATS,
	HC_TARGET_FAILSTATS,
	HC_TARGET_PRE,
	HC_TARGET_PRE_GOOD,
	HC_TARGET_PRE_BAD,
	HC_TARGET_PRE_MINLOSS,
	HC_TARGET_SPEED_MIN,
	HC_TARGET_SPEED_MAX
};

enum _:HUD_POSITION_TARGET {
	HP_TARGET_NONE,
	HP_TARGET_STATS,
	HP_TARGET_PRE,
	HP_TARGET_SPEED
};

new g_iHudColorTarget[MAX_PLAYERS + 1];
new g_iHudPositionTarget[MAX_PLAYERS + 1];

public init_menus() {
	register_menucmd(register_menuid(MENU_ID), 1023, "HandleJumpStatsMenu");
	register_menucmd(register_menuid(ENABLE_MENU_ID), 1023, "HandleEnableOptionsMenu");
	register_menucmd(register_menuid(HUD_COLOR_MENU_ID), 1023, "HandleCustomizeHudColorMenu");
	register_menucmd(register_menuid(HUD_COLOR_EDIT_MENU_ID), 1023, "HandleCustomizeHudColorEditMenu");
	register_menucmd(register_menuid(HUD_POSITION_MENU_ID), 1023, "HandleCustomizeHudPositionMenu");
	register_menucmd(register_menuid(HUD_POSITION_EDIT_MENU_ID), 1023, "HandleCustomizeHudPositionEditMenu");
	register_menucmd(register_menuid(SPEED_CONFIG_MENU_ID), 1023, "HandleSpeedConfigurationsMenu");
	register_clcmd(HUD_COLOR_INPUT_CMD, "CmdCustomizeHudColorInput");
	register_clcmd(HUD_POSITION_INPUT_CMD, "CmdCustomizeHudPositionInput");
}

public ShowJumpStatsMenu(id) {
	new szBuffer[512];
	new iLen = 0;
	new iKeys = 0;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rJump Stats:^n");

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wEnable options^n");
	iKeys |= MENU_KEY_1;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r2. \wCustomize HUD Color^n");
	iKeys |= MENU_KEY_2;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r3. \wCustomize HUD Position^n");
	iKeys |= MENU_KEY_3;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r4. \wSpeed configurations^n");
	iKeys |= MENU_KEY_4;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");
	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r6. \wReset settings^n");
	iKeys |= MENU_KEY_6;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r7. \wSave settings^n");
	iKeys |= MENU_KEY_7;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r0. \wExit");
	iKeys |= MENU_KEY_0;

	show_menu(id, iKeys, szBuffer, -1, MENU_ID);
}

public HandleJumpStatsMenu(const id, const key) {
	switch (key) {
		case 0: {
			ShowEnableOptionsMenu(id);
		}
		case 1: {
			ShowCustomizeHudColorMenu(id);
		}
		case 2: {
			ShowCustomizeHudPositionMenu(id);
		}
		case 3: {
			ShowSpeedConfigurationsMenu(id);
		}
		case 5: {
			if (settings_reset_player(id)) {
				client_print_color(id, print_team_blue, "[^3%s^1] Settings reset to server defaults.", g_pCvar[c_szPrefix]);
			} else {
				client_print_color(id, print_team_blue, "[^3%s^1] Settings reset, but saved data could not be removed.", g_pCvar[c_szPrefix]);
			}

			ShowJumpStatsMenu(id);
		}
		case 6: {
			if (settings_save_player(id)) {
				client_print_color(id, print_team_blue, "[^3%s^1] Settings saved.", g_pCvar[c_szPrefix]);
			} else {
				client_print_color(id, print_team_blue, "[^3%s^1] Settings could not be saved.", g_pCvar[c_szPrefix]);
			}

			ShowJumpStatsMenu(id);
		}
		case 9: {
			return PLUGIN_HANDLED;
		}
	}

	return PLUGIN_HANDLED;
}

public ShowEnableOptionsMenu(const id) {
	new szBuffer[768];
	new iLen = 0;
	new iKeys = 0;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rEnable options:^n^n");

	if (g_pCvar[C_ENABLE_SPEED]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wSpeed [%s]^n", g_eOnOff[id][of_bSpeed] ? "\yOn\w" : "\dOff\w");
		iKeys |= MENU_KEY_1;
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \dSpeed [server off]^n");
	}

	if (g_pCvar[C_ENABLE_PRE]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r2. \wShow pre [%s]^n", g_eOnOff[id][of_bPre] ? "\yOn\w" : "\dOff\w");
		iKeys |= MENU_KEY_2;
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r2. \dShow pre [server off]^n");
	}

	if (g_pCvar[C_ENABLE_STATS]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r3. \wStats [%s]^n", g_eSettings[id][S_STATS] ? "\yOn\w" : "\dOff\w");
		iKeys |= MENU_KEY_3;
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r3. \dStats [server off]^n");
	}

	if (g_pCvar[C_ENABLE_CHATINFO]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r4. \wChat info [%s]^n", g_eSettings[id][S_CHATINFO] ? "\yOn\w" : "\dOff\w");
		iKeys |= MENU_KEY_4;
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r4. \dChat info [server off]^n");
	}

	if (g_pCvar[C_ENABLE_SPEED]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r5. \wJumpOff [%s]^n", g_eOnOff[id][of_bJof] ? "\yOn\w" : "\dOff\w");
		iKeys |= MENU_KEY_5;
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r5. \dJumpOff [server off]^n");
	}

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");
	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r8. \wBack^n");
	iKeys |= MENU_KEY_8;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r0. \wExit");
	iKeys |= MENU_KEY_0;

	show_menu(id, iKeys, szBuffer, -1, ENABLE_MENU_ID);
}

public HandleEnableOptionsMenu(const id, const key) {
	switch (key) {
		case 0: {
			cmdShowSpeed(id);
			ShowEnableOptionsMenu(id);
		}
		case 1: {
			cmdShowPre(id);
			ShowEnableOptionsMenu(id);
		}
		case 2: {
			cmdShowStats(id);
			ShowEnableOptionsMenu(id);
		}
		case 3: {
			cmdShowChatInfo(id);
			ShowEnableOptionsMenu(id);
		}
		case 4: {
			cmdShowJof(id);
			ShowEnableOptionsMenu(id);
		}
		case 7: {
			ShowJumpStatsMenu(id);
		}
		case 8: {
			ShowEnableOptionsMenu(id);
		}
		case 9: {
			return PLUGIN_HANDLED;
		}
	}

	return PLUGIN_HANDLED;
}


public ShowCustomizeHudColorMenu(const id) {
	new szBuffer[768];
	new iLen = 0;
	new iKeys = 0;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rCustomize HUD Color:^n^n");

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wGood stats HUD \d(%d %d %d)^n",
		g_eSettings[id][S_STATS_R],
		g_eSettings[id][S_STATS_G],
		g_eSettings[id][S_STATS_B]
	);
	iKeys |= MENU_KEY_1;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r2. \wFail stats HUD \d(%d %d %d)^n",
		g_eSettings[id][S_FAILSTATS_R],
		g_eSettings[id][S_FAILSTATS_G],
		g_eSettings[id][S_FAILSTATS_B]
	);
	iKeys |= MENU_KEY_2;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r3. \wPre: Normal \d(%d %d %d)^n",
		g_eSettings[id][S_PRE_R],
		g_eSettings[id][S_PRE_G],
		g_eSettings[id][S_PRE_B]
	);
	iKeys |= MENU_KEY_3;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r4. \wPre: Good \d(%d %d %d)^n",
		g_eSettings[id][S_PRE_GOOD_R],
		g_eSettings[id][S_PRE_GOOD_G],
		g_eSettings[id][S_PRE_GOOD_B]
	);
	iKeys |= MENU_KEY_4;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r5. \wPre: Bad \d(%d %d %d)^n",
		g_eSettings[id][S_PRE_BAD_R],
		g_eSettings[id][S_PRE_BAD_G],
		g_eSettings[id][S_PRE_BAD_B]
	);
	iKeys |= MENU_KEY_5;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r6. \wPre: Minloss \d(%d %d %d)^n",
		g_eSettings[id][S_PRE_MINLOSS_R],
		g_eSettings[id][S_PRE_MINLOSS_G],
		g_eSettings[id][S_PRE_MINLOSS_B]
	);
	iKeys |= MENU_KEY_6;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r7. \wSpeed: Minimum \d(%d %d %d)^n",
		g_eSettings[id][S_SPEED_MIN_R],
		g_eSettings[id][S_SPEED_MIN_G],
		g_eSettings[id][S_SPEED_MIN_B]
	);
	iKeys |= MENU_KEY_7;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r8. \wSpeed: Maximum \d(%d %d %d)^n",
		g_eSettings[id][S_SPEED_MAX_R],
		g_eSettings[id][S_SPEED_MAX_G],
		g_eSettings[id][S_SPEED_MAX_B]
	);
	iKeys |= MENU_KEY_8;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");
	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r9. \wBack^n");
	iKeys |= MENU_KEY_9;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r0. \wExit");
	iKeys |= MENU_KEY_0;

	show_menu(id, iKeys, szBuffer, -1, HUD_COLOR_MENU_ID);
}

public HandleCustomizeHudColorMenu(const id, const key) {
	switch (key) {
		case 0: {
			g_iHudColorTarget[id] = HC_TARGET_STATS;
			ShowCustomizeHudColorEditMenu(id);
		}
		case 1: {
			g_iHudColorTarget[id] = HC_TARGET_FAILSTATS;
			ShowCustomizeHudColorEditMenu(id);
		}
		case 2: {
			g_iHudColorTarget[id] = HC_TARGET_PRE;
			ShowCustomizeHudColorEditMenu(id);
		}
		case 3: {
			g_iHudColorTarget[id] = HC_TARGET_PRE_GOOD;
			ShowCustomizeHudColorEditMenu(id);
		}
		case 4: {
			g_iHudColorTarget[id] = HC_TARGET_PRE_BAD;
			ShowCustomizeHudColorEditMenu(id);
		}
		case 5: {
			g_iHudColorTarget[id] = HC_TARGET_PRE_MINLOSS;
			ShowCustomizeHudColorEditMenu(id);
		}
		case 6: {
			g_iHudColorTarget[id] = HC_TARGET_SPEED_MIN;
			ShowCustomizeHudColorEditMenu(id);
		}
		case 7: {
			g_iHudColorTarget[id] = HC_TARGET_SPEED_MAX;
			ShowCustomizeHudColorEditMenu(id);
		}
		case 8: {
			ShowJumpStatsMenu(id);
		}
		case 9: {
			return PLUGIN_HANDLED;
		}
	}

	return PLUGIN_HANDLED;
}

public ShowCustomizeHudPositionMenu(const id) {
	new szBuffer[768];
	new iLen = 0;
	new iKeys = 0;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rCustomize HUD Position:^n^n");

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wStats \d(%.2f %.2f)^n",
		HudPositionForDisplay(g_eSettings[id][S_STATS_X]),
		HudPositionForDisplay(g_eSettings[id][S_STATS_Y])
	);
	iKeys |= MENU_KEY_1;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r2. \wPre \d(%.2f %.2f)^n",
		HudPositionForDisplay(g_eSettings[id][S_PRE_X]),
		HudPositionForDisplay(g_eSettings[id][S_PRE_Y])
	);
	iKeys |= MENU_KEY_2;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r3. \wSpeed / JOF \d(%.2f %.2f)^n",
		HudPositionForDisplay(g_eSettings[id][S_SPEED_X]),
		HudPositionForDisplay(g_eSettings[id][S_SPEED_Y])
	);
	iKeys |= MENU_KEY_3;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");
	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r8. \wBack^n");
	iKeys |= MENU_KEY_8;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r0. \wExit");
	iKeys |= MENU_KEY_0;

	show_menu(id, iKeys, szBuffer, -1, HUD_POSITION_MENU_ID);
}

public HandleCustomizeHudPositionMenu(const id, const key) {
	switch (key) {
		case 0: {
			g_iHudPositionTarget[id] = HP_TARGET_STATS;
			ShowCustomizeHudPositionEditMenu(id);
		}
		case 1: {
			g_iHudPositionTarget[id] = HP_TARGET_PRE;
			ShowCustomizeHudPositionEditMenu(id);
		}
		case 2: {
			g_iHudPositionTarget[id] = HP_TARGET_SPEED;
			ShowCustomizeHudPositionEditMenu(id);
		}
		case 7: {
			ShowJumpStatsMenu(id);
		}
		case 8: {
			ShowCustomizeHudPositionMenu(id);
		}
		case 9: {
			return PLUGIN_HANDLED;
		}
	}

	return PLUGIN_HANDLED;
}

public ShowCustomizeHudPositionEditMenu(const id) {
	new szBuffer[768];
	new iLen = 0;
	new iKeys = 0;

	if (g_iHudPositionTarget[id] == HP_TARGET_STATS) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rStats HUD:^n\dCurrent: %.2f %.2f^n^n",
			HudPositionForDisplay(g_eSettings[id][S_STATS_X]),
			HudPositionForDisplay(g_eSettings[id][S_STATS_Y])
		);
	} else if (g_iHudPositionTarget[id] == HP_TARGET_PRE) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rPre HUD:^n\dCurrent: %.2f %.2f^n^n",
			HudPositionForDisplay(g_eSettings[id][S_PRE_X]),
			HudPositionForDisplay(g_eSettings[id][S_PRE_Y])
		);
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rSpeed / JOF HUD:^n\dCurrent: %.2f %.2f^n^n",
			HudPositionForDisplay(g_eSettings[id][S_SPEED_X]),
			HudPositionForDisplay(g_eSettings[id][S_SPEED_Y])
		);
	}

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wDefault \d(from cvar)^n");
	iKeys |= MENU_KEY_1;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r2. \wCustom position^n");
	iKeys |= MENU_KEY_2;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r3. \wUp^n");
	iKeys |= MENU_KEY_3;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r4. \wDown^n");
	iKeys |= MENU_KEY_4;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r5. \wLeft^n");
	iKeys |= MENU_KEY_5;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r6. \wRight^n");
	iKeys |= MENU_KEY_6;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");
	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r8. \wBack^n");
	iKeys |= MENU_KEY_8;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r0. \wExit");
	iKeys |= MENU_KEY_0;

	show_menu(id, iKeys, szBuffer, -1, HUD_POSITION_EDIT_MENU_ID);
}

public HandleCustomizeHudPositionEditMenu(const id, const key) {
	new Float:flX;
	new Float:flY;

	if (g_iHudPositionTarget[id] == HP_TARGET_STATS) {
		flX = g_eSettings[id][S_STATS_X];
		flY = g_eSettings[id][S_STATS_Y];
	} else if (g_iHudPositionTarget[id] == HP_TARGET_PRE) {
		flX = g_eSettings[id][S_PRE_X];
		flY = g_eSettings[id][S_PRE_Y];
	} else if (g_iHudPositionTarget[id] == HP_TARGET_SPEED) {
		flX = g_eSettings[id][S_SPEED_X];
		flY = g_eSettings[id][S_SPEED_Y];
	} else {
		return PLUGIN_HANDLED;
	}

	switch (key) {
		case 0: {
			if (g_iHudPositionTarget[id] == HP_TARGET_STATS) {
				settings_stats_xy_set(id, g_pCvar[C_STATS_X], g_pCvar[C_STATS_Y]);
			} else if (g_iHudPositionTarget[id] == HP_TARGET_PRE) {
				settings_pre_xy_set(id, g_pCvar[C_PRE_X], g_pCvar[C_PRE_Y]);
			} else {
				settings_speed_xy_set(id, g_pCvar[C_SPEED_X], g_pCvar[C_SPEED_Y]);
			}

			ShowCustomizeHudPositionEditMenu(id);
		}
		case 1: {
			client_print(id, print_chat, "Please enter your position in format X Y: %.2f %.2f",
				HudPositionForDisplay(flX), HudPositionForDisplay(flY));
			client_cmd(id, "messagemode %s", HUD_POSITION_INPUT_CMD);
		}
		case 2: {
			flY -= 0.02;

			if (flY < 0.0) {
				flY = 0.0;
			}

			ApplyHudPosition(id, flX, flY);
			ShowCustomizeHudPositionEditMenu(id);
		}
		case 3: {
			flY += 0.02;

			if (flY > 1.0) {
				flY = 1.0;
			}

			ApplyHudPosition(id, flX, flY);
			ShowCustomizeHudPositionEditMenu(id);
		}
		case 4: {
			flX -= 0.02;

			if (flX < -1.0) {
				flX = -1.0;
			}

			ApplyHudPosition(id, flX, flY);
			ShowCustomizeHudPositionEditMenu(id);
		}
		case 5: {
			flX += 0.02;

			if (flX > 1.0) {
				flX = 1.0;
			}

			ApplyHudPosition(id, flX, flY);
			ShowCustomizeHudPositionEditMenu(id);
		}
		case 7: {
			ShowCustomizeHudPositionMenu(id);
		}
		case 8: {
			ShowCustomizeHudPositionEditMenu(id);
		}
		case 9: {
			return PLUGIN_HANDLED;
		}
	}

	return PLUGIN_HANDLED;
}

public ShowCustomizeHudColorEditMenu(const id) {
	new szBuffer[768];
	new szTitle[32];
	new iLen = 0;
	new iKeys = 0;
	new iRed;
	new iGreen;
	new iBlue;

	if (!GetHudColor(id, szTitle, charsmax(szTitle), iRed, iGreen, iBlue)) {
		return;
	}

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r%s:^n\dCurrent: %d %d %d^n^n",
		szTitle, iRed, iGreen, iBlue);

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wDefault \d(from cvar)^n");
	iKeys |= MENU_KEY_1;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r2. \wCustom color^n");
	iKeys |= MENU_KEY_2;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r3. \wGreen \d(20 255 150)^n");
	iKeys |= MENU_KEY_3;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r4. \wRed \d(255 70 120)^n");
	iKeys |= MENU_KEY_4;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r5. \wBlue \d(0 170 255)^n");
	iKeys |= MENU_KEY_5;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r6. \wWhite \d(255 255 255)^n");
	iKeys |= MENU_KEY_6;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");
	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r8. \wBack^n");
	iKeys |= MENU_KEY_8;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r0. \wExit");
	iKeys |= MENU_KEY_0;

	show_menu(id, iKeys, szBuffer, -1, HUD_COLOR_EDIT_MENU_ID);
}

public HandleCustomizeHudColorEditMenu(const id, const key) {
	switch (key) {
		case 0: {
			ResetHudColor(id);
			ShowCustomizeHudColorEditMenu(id);
		}
		case 1: {
			client_print(id, print_chat, "Please enter your color in RGB format: 255 255 255");
			client_cmd(id, "messagemode %s", HUD_COLOR_INPUT_CMD);
		}
		case 2: {
			ApplyHudColor(id, 20, 255, 150);
			ShowCustomizeHudColorEditMenu(id);
		}
		case 3: {
			ApplyHudColor(id, 255, 70, 120);
			ShowCustomizeHudColorEditMenu(id);
		}
		case 4: {
			ApplyHudColor(id, 0, 170, 255);
			ShowCustomizeHudColorEditMenu(id);
		}
		case 5: {
			ApplyHudColor(id, 255, 255, 255);
			ShowCustomizeHudColorEditMenu(id);
		}
		case 7: {
			ShowCustomizeHudColorMenu(id);
		}
		case 8: {
			ShowCustomizeHudColorEditMenu(id);
		}
		case 9: {
			return PLUGIN_HANDLED;
		}
	}

	return PLUGIN_HANDLED;
}

public CmdCustomizeHudColorInput(const id) {
	new szArgs[64];
	new szRed[8], szGreen[8], szBlue[8];

	if (g_iHudColorTarget[id] == HC_TARGET_NONE) {
		return PLUGIN_HANDLED;
	}

	read_args(szArgs, charsmax(szArgs));
	remove_quotes(szArgs);
	trim(szArgs);

	if (!szArgs[0]) {
		ShowCustomizeHudColorEditMenu(id);
		return PLUGIN_HANDLED;
	}

	parse(szArgs, szRed, charsmax(szRed), szGreen, charsmax(szGreen), szBlue, charsmax(szBlue));

	if (!szRed[0] || !szGreen[0] || !szBlue[0]) {
		client_print(id, print_chat, "Invalid RGB format. Example: 255 255 255");
		ShowCustomizeHudColorEditMenu(id);
		return PLUGIN_HANDLED;
	}

	new iRed = str_to_num(szRed);
	new iGreen = str_to_num(szGreen);
	new iBlue = str_to_num(szBlue);

	if (iRed < 0 || iRed > 255 || iGreen < 0 || iGreen > 255 || iBlue < 0 || iBlue > 255) {
		client_print(id, print_chat, "RGB values must be between 0 and 255");
		ShowCustomizeHudColorEditMenu(id);
		return PLUGIN_HANDLED;
	}

	ApplyHudColor(id, iRed, iGreen, iBlue);

	ShowCustomizeHudColorEditMenu(id);
	return PLUGIN_HANDLED;
}

stock bool:GetHudColor(const id, szTitle[], const iTitleLen, &iRed, &iGreen, &iBlue) {
	switch (g_iHudColorTarget[id]) {
		case HC_TARGET_STATS: {
			copy(szTitle, iTitleLen, "Good stats HUD");
			iRed = g_eSettings[id][S_STATS_R];
			iGreen = g_eSettings[id][S_STATS_G];
			iBlue = g_eSettings[id][S_STATS_B];
		}
		case HC_TARGET_FAILSTATS: {
			copy(szTitle, iTitleLen, "Fail stats HUD");
			iRed = g_eSettings[id][S_FAILSTATS_R];
			iGreen = g_eSettings[id][S_FAILSTATS_G];
			iBlue = g_eSettings[id][S_FAILSTATS_B];
		}
		case HC_TARGET_PRE: {
			copy(szTitle, iTitleLen, "Pre: Normal");
			iRed = g_eSettings[id][S_PRE_R];
			iGreen = g_eSettings[id][S_PRE_G];
			iBlue = g_eSettings[id][S_PRE_B];
		}
		case HC_TARGET_PRE_GOOD: {
			copy(szTitle, iTitleLen, "Pre: Good");
			iRed = g_eSettings[id][S_PRE_GOOD_R];
			iGreen = g_eSettings[id][S_PRE_GOOD_G];
			iBlue = g_eSettings[id][S_PRE_GOOD_B];
		}
		case HC_TARGET_PRE_BAD: {
			copy(szTitle, iTitleLen, "Pre: Bad");
			iRed = g_eSettings[id][S_PRE_BAD_R];
			iGreen = g_eSettings[id][S_PRE_BAD_G];
			iBlue = g_eSettings[id][S_PRE_BAD_B];
		}
		case HC_TARGET_PRE_MINLOSS: {
			copy(szTitle, iTitleLen, "Pre: Minloss");
			iRed = g_eSettings[id][S_PRE_MINLOSS_R];
			iGreen = g_eSettings[id][S_PRE_MINLOSS_G];
			iBlue = g_eSettings[id][S_PRE_MINLOSS_B];
		}
		case HC_TARGET_SPEED_MIN: {
			copy(szTitle, iTitleLen, "Speed: Minimum");
			iRed = g_eSettings[id][S_SPEED_MIN_R];
			iGreen = g_eSettings[id][S_SPEED_MIN_G];
			iBlue = g_eSettings[id][S_SPEED_MIN_B];
		}
		case HC_TARGET_SPEED_MAX: {
			copy(szTitle, iTitleLen, "Speed: Maximum");
			iRed = g_eSettings[id][S_SPEED_MAX_R];
			iGreen = g_eSettings[id][S_SPEED_MAX_G];
			iBlue = g_eSettings[id][S_SPEED_MAX_B];
		}
		default: {
			return false;
		}
	}

	return true;
}

stock ApplyHudColor(const id, const iRed, const iGreen, const iBlue) {
	switch (g_iHudColorTarget[id]) {
		case HC_TARGET_STATS: settings_stats_rgb_set(id, iRed, iGreen, iBlue);
		case HC_TARGET_FAILSTATS: settings_failstats_rgb_set(id, iRed, iGreen, iBlue);
		case HC_TARGET_PRE: settings_pre_rgb_set(id, iRed, iGreen, iBlue);
		case HC_TARGET_PRE_GOOD: settings_pre_good_rgb_set(id, iRed, iGreen, iBlue);
		case HC_TARGET_PRE_BAD: settings_pre_bad_rgb_set(id, iRed, iGreen, iBlue);
		case HC_TARGET_PRE_MINLOSS: settings_pre_minloss_rgb_set(id, iRed, iGreen, iBlue);
		case HC_TARGET_SPEED_MIN: settings_speed_min_rgb_set(id, iRed, iGreen, iBlue);
		case HC_TARGET_SPEED_MAX: settings_speed_max_rgb_set(id, iRed, iGreen, iBlue);
	}
}

stock ResetHudColor(const id) {
	switch (g_iHudColorTarget[id]) {
		case HC_TARGET_STATS: settings_stats_rgb_set(id, g_pCvar[C_STATS_R], g_pCvar[C_STATS_G], g_pCvar[C_STATS_B]);
		case HC_TARGET_FAILSTATS: settings_failstats_rgb_set(id, g_pCvar[C_FAILSTATS_R], g_pCvar[C_FAILSTATS_G], g_pCvar[C_FAILSTATS_B]);
		case HC_TARGET_PRE: settings_pre_rgb_set(id, g_pCvar[C_PRE_R], g_pCvar[C_PRE_G], g_pCvar[C_PRE_B]);
		case HC_TARGET_PRE_GOOD: settings_pre_good_rgb_set(id, g_pCvar[C_PRE_GOOD_R], g_pCvar[C_PRE_GOOD_G], g_pCvar[C_PRE_GOOD_B]);
		case HC_TARGET_PRE_BAD: settings_pre_bad_rgb_set(id, g_pCvar[C_PRE_BAD_R], g_pCvar[C_PRE_BAD_G], g_pCvar[C_PRE_BAD_B]);
		case HC_TARGET_PRE_MINLOSS: settings_pre_minloss_rgb_set(id, g_pCvar[C_PRE_MINLOSS_R], g_pCvar[C_PRE_MINLOSS_G], g_pCvar[C_PRE_MINLOSS_B]);
		case HC_TARGET_SPEED_MIN: settings_speed_min_rgb_set(id, g_pCvar[C_SPEED_MIN_R], g_pCvar[C_SPEED_MIN_G], g_pCvar[C_SPEED_MIN_B]);
		case HC_TARGET_SPEED_MAX: settings_speed_max_rgb_set(id, g_pCvar[C_SPEED_MAX_R], g_pCvar[C_SPEED_MAX_G], g_pCvar[C_SPEED_MAX_B]);
	}
}

public CmdCustomizeHudPositionInput(const id) {
	new szArgs[64];
	new szX[16], szY[16];
	new Float:flX;
	new Float:flY;

	if (!GetHudPosition(id, flX, flY)) {
		return PLUGIN_HANDLED;
	}

	read_args(szArgs, charsmax(szArgs));
	remove_quotes(szArgs);
	trim(szArgs);

	if (!szArgs[0]) {
		ShowCustomizeHudPositionEditMenu(id);
		return PLUGIN_HANDLED;
	}

	parse(szArgs, szX, charsmax(szX), szY, charsmax(szY));

	if (!szX[0] || !szY[0]) {
		client_print(id, print_chat, "Invalid position format. Example: %.2f %.2f",
			HudPositionForDisplay(flX), HudPositionForDisplay(flY));
		ShowCustomizeHudPositionEditMenu(id);
		return PLUGIN_HANDLED;
	}

	flX = str_to_float(szX);
	flY = str_to_float(szY);

	if (flX < -1.0 || flX > 1.0 || flY < 0.0 || flY > 1.0) {
		client_print(id, print_chat, "X must be from -1.0 to 1.0 and Y must be from 0.0 to 1.0");
		ShowCustomizeHudPositionEditMenu(id);
		return PLUGIN_HANDLED;
	}

	ApplyHudPosition(id, flX, flY);
	ShowCustomizeHudPositionEditMenu(id);
	return PLUGIN_HANDLED;
}

ApplyHudPosition(const id, Float:flX, Float:flY) {
	if (g_iHudPositionTarget[id] == HP_TARGET_STATS) {
		settings_stats_xy_set(id, flX, flY);
	} else if (g_iHudPositionTarget[id] == HP_TARGET_PRE) {
		settings_pre_xy_set(id, flX, flY);
	} else if (g_iHudPositionTarget[id] == HP_TARGET_SPEED) {
		settings_speed_xy_set(id, flX, flY);
	}
}

stock bool:GetHudPosition(const id, &Float:flX, &Float:flY) {
	switch (g_iHudPositionTarget[id]) {
		case HP_TARGET_STATS: {
			flX = g_eSettings[id][S_STATS_X];
			flY = g_eSettings[id][S_STATS_Y];
		}
		case HP_TARGET_PRE: {
			flX = g_eSettings[id][S_PRE_X];
			flY = g_eSettings[id][S_PRE_Y];
		}
		case HP_TARGET_SPEED: {
			flX = g_eSettings[id][S_SPEED_X];
			flY = g_eSettings[id][S_SPEED_Y];
		}
		default: {
			return false;
		}
	}

	return true;
}

stock Float:HudPositionForDisplay(Float:flPosition) {
	return flPosition < 0.0
		? flPosition - HUD_POSITION_DISPLAY_EPSILON
		: flPosition + HUD_POSITION_DISPLAY_EPSILON;
}

public ShowSpeedConfigurationsMenu(const id) {
	new szBuffer[512];
	new iLen = 0;
	new iKeys = 0;
	new iFrame = g_eSettings[id][S_SPEED_FRAME];

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rSpeed configurations:^n^n");

	if (iFrame < 2 || iFrame > 5) {
		iFrame = g_pCvar[C_SPEED_FRAME];
	}

	if (iFrame < 2 || iFrame > 5) {
		iFrame = 3;
	}

	if (iFrame == g_pCvar[C_SPEED_FRAME]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wHUD update skip framerate: \y%d \d(default)^n", iFrame);
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wHUD update skip framerate: \y%d^n", iFrame);
	}
	iKeys |= MENU_KEY_1;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r2. \wSpeed style mode: \y%s^n",
		g_szSpeedMode[g_eSettings[id][S_SPEED_MODE]]
	);
	iKeys |= MENU_KEY_2;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");
	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r8. \wBack^n");
	iKeys |= MENU_KEY_8;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r0. \wExit");
	iKeys |= MENU_KEY_0;

	show_menu(id, iKeys, szBuffer, -1, SPEED_CONFIG_MENU_ID);
}

public HandleSpeedConfigurationsMenu(const id, const key) {
	switch (key) {
		case 0: {
			new iFrame = g_eSettings[id][S_SPEED_FRAME];

			if (iFrame < 2 || iFrame > 5) {
				iFrame = g_pCvar[C_SPEED_FRAME];
			}

			if (iFrame < 2 || iFrame > 5) {
				iFrame = 3;
			}

			switch (iFrame) {
				case 2: iFrame = 3;
				case 3: iFrame = 4;
				case 4: iFrame = 5;
				default: iFrame = 2;
			}

			settings_speed_frame_set(id, iFrame);
			ShowSpeedConfigurationsMenu(id);
		}
		case 1: {
			new iMode = g_eSettings[id][S_SPEED_MODE] + 1;
			if (iMode > SPEED_MODE_NUMBER) {
				iMode = SPEED_MODE_DEFAULT;
			}

			settings_speed_mode_set(id, iMode);
			ShowSpeedConfigurationsMenu(id);
		}
		case 7: {
			ShowJumpStatsMenu(id);
		}
		case 8: {
			ShowSpeedConfigurationsMenu(id);
		}
		case 9: {
			return PLUGIN_HANDLED;
		}
	}

	return PLUGIN_HANDLED;
}
