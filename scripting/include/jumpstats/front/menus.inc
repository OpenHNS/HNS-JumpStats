public cmdLjsmenu(id) {
	if (!is_user_connected(id))
		return PLUGIN_HANDLED;

	new hMenu = menu_create("\rJump Stats Menu", "ljsmenuHandler");

	if (g_pCvar[c_iEnablePreSpeed] && g_eOnOff[id][of_bSpeed] == true) {
		menu_additem(hMenu, "Speed - \yOn", "1");
	} else {
		menu_additem(hMenu, "Speed - \dOff", "1");
	}

	if (g_pCvar[c_iEnablePreSpeed] && g_eOnOff[id][of_bPre] == true) {
		menu_additem(hMenu, "Show pre - \yOn", "2");
	} else {
		menu_additem(hMenu, "Show pre - \dOff", "2");
	}

	if (g_pCvar[c_iEnableStats] && g_eOnOff[id][of_bStats] == true) {
		menu_additem(hMenu, "Stats - \yOn", "3");
	} else {
		menu_additem(hMenu, "Stats - \dOff", "3");
	}

	if (g_pCvar[c_iEnableChat] && g_eOnOff[id][of_bChatInfo] == true) {
		menu_additem(hMenu, "Chat info - \yOn", "4");
	} else {
		menu_additem(hMenu, "Chat info - \dOff", "4");
	}

	if (g_pCvar[c_iEnableStrafe] && g_eOnOff[id][of_bStrafe] == true) {
		menu_additem(hMenu, "Strafes stats - \yOn", "5");
	} else {
		menu_additem(hMenu, "Strafes stats - \dOff", "5");
	}

	if (g_pCvar[c_iEnablePreSpeed] && g_eOnOff[id][of_bJof] == true) {
		menu_additem(hMenu, "JumpOff - \yOn", "6");
	} else {
		menu_additem(hMenu, "JumpOff - \dOff", "6");
	}

	menu_display(id, hMenu);

	return PLUGIN_HANDLED;
}

public ljsmenuHandler(id, hMenu, item) {
	if (!is_user_connected(id) || item == MENU_EXIT) {
		return PLUGIN_HANDLED;
	}

	switch (item) {
		case 0: {
			cmdShowSpeed(id);
			cmdLjsmenu(id);
		}
		case 1: {
			cmdShowPre(id);
			cmdLjsmenu(id);
		}
		case 2: {
			cmdShowStats(id);
			cmdLjsmenu(id);
		}
		case 3: {
			cmdShowChatInfo(id);
			cmdLjsmenu(id);
		}
		case 4: {
			cmdShowStrafe(id);
			cmdLjsmenu(id);
		}
		case 5: {
			cmdShowJof(id);
			cmdLjsmenu(id);
		}
	}

	menu_destroy(hMenu);
	return PLUGIN_HANDLED;
}
