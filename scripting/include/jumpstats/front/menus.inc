new const MENU_ID[] = "JUMPSTATS_MENU";
new const ENABLE_MENU_ID[] = "JUMPSTATS_ENABLE_MENU";

public init_menus() {
	register_menucmd(register_menuid(MENU_ID), 1023, "HandleJumpStatsMenu");
	register_menucmd(register_menuid(ENABLE_MENU_ID), 1023, "HandleEnableOptionsMenu");
}

public ShowJumpStatsMenu(id) {
	new szBuffer[512];
	new iLen = 0;
	new iKeys = 0;

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\rJump Stats:^n");

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "^n");

	iLen += formatex(szBuffer[iLen], charsmax(szBuffer) - iLen, "\r1. \wEnable options^n");
	iKeys |= MENU_KEY_1;

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
