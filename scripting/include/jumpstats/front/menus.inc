new const MENU_ID[] = "JUMPSTATS_MENU";
new const ENABLE_MENU_ID[] = "JUMPSTATS_ENABLE_MENU";
new const HUD_COLOR_MENU_ID[] = "JUMPSTATS_HUD_COLOR_MENU";
new const HUD_COLOR_EDIT_MENU_ID[] = "JUMPSTATS_HUD_COLOR_EDIT_MENU";
new const HUD_COLOR_INPUT_CMD[] = "JS_HUD_COLOR_INPUT";

enum _:HUD_COLOR_TARGET {
	HC_TARGET_NONE,
	HC_TARGET_STATS,
	HC_TARGET_FAILSTATS
};

new g_iHudColorTarget[MAX_PLAYERS + 1];

public init_menus() {
	register_menucmd(register_menuid(MENU_ID), 1023, "HandleJumpStatsMenu");
	register_menucmd(register_menuid(ENABLE_MENU_ID), 1023, "HandleEnableOptionsMenu");
	register_menucmd(register_menuid(HUD_COLOR_MENU_ID), 1023, "HandleCustomizeHudColorMenu");
	register_menucmd(register_menuid(HUD_COLOR_EDIT_MENU_ID), 1023, "HandleCustomizeHudColorEditMenu");
	register_clcmd(HUD_COLOR_INPUT_CMD, "CmdCustomizeHudColorInput");
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

	if (g_pCvar[C_ENABLE_PRESPEED]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wSpeed [%s]^n", g_eOnOff[id][of_bSpeed] ? "\yOn\w" : "\dOff\w");
		iKeys |= MENU_KEY_1;
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \dSpeed [server off]^n");
	}

	if (g_pCvar[C_ENABLE_PRESPEED]) {
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

	if (g_pCvar[C_ENABLE_STRAFE]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r4. \wStrafes stats [%s]^n", g_eSettings[id][S_STRAFE] ? "\yOn\w" : "\dOff\w");
		iKeys |= MENU_KEY_4;
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r4. \dStrafes stats [server off]^n");
	}

	if (g_pCvar[C_ENABLE_CHATINFO]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r5. \wChat info [%s]^n", g_eSettings[id][S_CHATINFO] ? "\yOn\w" : "\dOff\w");
		iKeys |= MENU_KEY_5;
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r5. \dChat info [server off]^n");
	}

	if (g_pCvar[C_ENABLE_PRESPEED]) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r6. \wJumpOff [%s]^n", g_eOnOff[id][of_bJof] ? "\yOn\w" : "\dOff\w");
		iKeys |= MENU_KEY_6;
	} else {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r6. \dJumpOff [server off]^n");
	}

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");
	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r8. \wBack^n");
	iKeys |= MENU_KEY_8;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r9. \wRefresh^n");
	iKeys |= MENU_KEY_9;

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
			cmdShowStrafe(id);
			ShowEnableOptionsMenu(id);
		}
		case 4: {
			cmdShowChatInfo(id);
			ShowEnableOptionsMenu(id);
		}
		case 5: {
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

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");
	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r8. \wBack^n");
	iKeys |= MENU_KEY_8;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r9. \wRefresh^n");
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
		case 7: {
			ShowJumpStatsMenu(id);
		}
		case 8: {
			ShowCustomizeHudColorMenu(id);
		}
		case 9: {
			return PLUGIN_HANDLED;
		}
	}

	return PLUGIN_HANDLED;
}

public ShowCustomizeHudColorEditMenu(const id) {
	new szBuffer[768];
	new iLen = 0;
	new iKeys = 0;

	if (g_iHudColorTarget[id] == HC_TARGET_STATS) {
		iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rGood stats HUD:^n\dCurrent: %d %d %d^n^n",
			g_eSettings[id][S_STATS_R],
			g_eSettings[id][S_STATS_G],
			g_eSettings[id][S_STATS_B]
		);
	} else {
		iLen += formatex(
			szBuffer[iLen], charsmax(szBuffer) - iLen, "\rFail stats HUD:^n\dCurrent: %d %d %d^n^n",
			g_eSettings[id][S_FAILSTATS_R],
			g_eSettings[id][S_FAILSTATS_G],
			g_eSettings[id][S_FAILSTATS_B]
		);
	}

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

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r9. \wRefresh^n");
	iKeys |= MENU_KEY_9;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r0. \wExit");
	iKeys |= MENU_KEY_0;

	show_menu(id, iKeys, szBuffer, -1, HUD_COLOR_EDIT_MENU_ID);
}

public HandleCustomizeHudColorEditMenu(const id, const key) {
	switch (key) {
		case 0: {
			if (g_iHudColorTarget[id] == HC_TARGET_STATS) {
				settings_stats_rgb_set(id, g_pCvar[C_STATS_R], g_pCvar[C_STATS_G], g_pCvar[C_STATS_B]);
			} else if (g_iHudColorTarget[id] == HC_TARGET_FAILSTATS) {
				settings_failstats_rgb_set(id, g_pCvar[C_FAILSTATS_R], g_pCvar[C_FAILSTATS_G], g_pCvar[C_FAILSTATS_B]);
			}

			ShowCustomizeHudColorEditMenu(id);
		}
		case 1: {
			client_print(id, print_chat, "Please enter your color in RGB format: 255 255 255");
			client_cmd(id, "messagemode %s", HUD_COLOR_INPUT_CMD);
		}
		case 2: {
			if (g_iHudColorTarget[id] == HC_TARGET_STATS) {
				settings_stats_rgb_set(id, 20, 255, 150);
			} else if (g_iHudColorTarget[id] == HC_TARGET_FAILSTATS) {
				settings_failstats_rgb_set(id, 20, 255, 150);
			}

			ShowCustomizeHudColorEditMenu(id);
		}
		case 3: {
			if (g_iHudColorTarget[id] == HC_TARGET_STATS) {
				settings_stats_rgb_set(id, 255, 70, 120);
			} else if (g_iHudColorTarget[id] == HC_TARGET_FAILSTATS) {
				settings_failstats_rgb_set(id, 255, 70, 120);
			}

			ShowCustomizeHudColorEditMenu(id);
		}
		case 4: {
			if (g_iHudColorTarget[id] == HC_TARGET_STATS) {
				settings_stats_rgb_set(id, 0, 170, 255);
			} else if (g_iHudColorTarget[id] == HC_TARGET_FAILSTATS) {
				settings_failstats_rgb_set(id, 0, 170, 255);
			}

			ShowCustomizeHudColorEditMenu(id);
		}
		case 5: {
			if (g_iHudColorTarget[id] == HC_TARGET_STATS) {
				settings_stats_rgb_set(id, 255, 255, 255);
			} else if (g_iHudColorTarget[id] == HC_TARGET_FAILSTATS) {
				settings_failstats_rgb_set(id, 255, 255, 255);
			}

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

	if (g_iHudColorTarget[id] == HC_TARGET_STATS) {
		settings_stats_rgb_set(id, iRed, iGreen, iBlue);
	} else if (g_iHudColorTarget[id] == HC_TARGET_FAILSTATS) {
		settings_failstats_rgb_set(id, iRed, iGreen, iBlue);
	}

	ShowCustomizeHudColorEditMenu(id);
	return PLUGIN_HANDLED;
}
