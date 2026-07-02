#if defined _cmds_included
  #endinput
#endif

#define _cmds_included

enum _:JS_ONOFF {
	bool:of_bJof,
	bool:of_bSpeed,
	bool:of_bPre,
};

new g_eOnOff[MAX_PLAYERS + 1][JS_ONOFF];

public init_cmds() {
	RegisterSayCmd("ljsmenu", "ljs",		"cmdLjsmenu",		ADMIN_ALL, "Jump stats menu");
	RegisterSayCmd("strafe", "strafestats",	"cmdShowStrafe",	ADMIN_ALL, "Show strafe stats");
	RegisterSayCmd("stats", "ljstats",		"cmdShowStats",		ADMIN_ALL, "Show main stats");
	RegisterSayCmd("chatinfo", "ci",		"cmdShowChatInfo",	ADMIN_ALL, "Show jump chat onfo");
	RegisterSayCmd("showspeed", "speed",	"cmdShowSpeed",		ADMIN_ALL, "Show speed");
	RegisterSayCmd("showpre", "pre",		"cmdShowPre",		ADMIN_ALL, "Show pre");
	RegisterSayCmd("jumpoff", "jof",		"cmdShowJof",		ADMIN_ALL, "Show jof");
	RegisterSayCmd("sound", "snd",			"cmdSound",			ADMIN_ALL, "Sound jumps");
}

public cmdShowChatInfo(id) {
	if (!g_pCvar[C_ENABLE_CHATINFO])
		return PLUGIN_HANDLED;

	if (g_eSettings[id][S_CHATINFO]) {
		settings_chatinfo_set(id, false);
	} else {
		settings_chatinfo_set(id, true);
	}

	client_print_color(id, print_team_blue, "[^3%s^1] Chatinfo: ^3%s^1.", g_pCvar[c_szPrefix], g_eSettings[id][S_CHATINFO] ? "enabled" : "disabled");
	
	return PLUGIN_HANDLED;
}

public cmdShowStats(id) {
	if (!g_pCvar[C_ENABLE_STATS])
		return PLUGIN_HANDLED;

	if (g_eSettings[id][S_STATS]) {
		settings_stats_set(id, false);
	} else {
		settings_stats_set(id, true);
	}

	client_print_color(id, print_team_blue, "[^3%s^1] Stats: ^3%s^1.", g_pCvar[c_szPrefix], g_eSettings[id][S_STATS] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdShowStrafe(id) {
	if (!g_pCvar[C_ENABLE_STRAFE])
		return PLUGIN_HANDLED;
		
	if (g_eSettings[id][S_STRAFE]) {
		settings_strafe_set(id, false);
	} else {
		settings_strafe_set(id, true);
	}

	client_print_color(id, print_team_blue, "[^3%s^1] Strafe stats: ^3%s^1.", g_pCvar[c_szPrefix], g_eSettings[id][S_STRAFE] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdShowJof(id) {
	if (!g_pCvar[C_ENABLE_PRESPEED])
		return PLUGIN_HANDLED;

	g_eOnOff[id][of_bJof] = !g_eOnOff[id][of_bJof];

	client_print_color(id, print_team_blue, "[^3%s^1] JumpOff: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bJof] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdShowSpeed(id) {
	if (!g_pCvar[C_ENABLE_PRESPEED])
		return PLUGIN_HANDLED;

	g_eOnOff[id][of_bSpeed] = !g_eOnOff[id][of_bSpeed];

	client_print_color(id, print_team_blue, "[^3%s^1] Speed: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bSpeed] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdShowPre(id) {
	if (!g_pCvar[C_ENABLE_PRESPEED])
		return PLUGIN_HANDLED;

	g_eOnOff[id][of_bPre] = !g_eOnOff[id][of_bPre];

	client_print_color(id, print_team_blue, "[^3%s^1] Pre: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bPre] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdSound(id) {
	if (!g_pCvar[C_ENABLE_SOUND])
		return PLUGIN_HANDLED;

	if (g_eSettings[id][S_SOUND]) {
		settings_sound_set(id, false);
	} else {
		settings_sound_set(id, true);
	}

	client_print_color(id, print_team_blue, "[^3%s^1] Sound: ^3%s^1.", g_pCvar[c_szPrefix], g_eSettings[id][S_SOUND] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

stock RegisterSayCmd(const szCmd[], const szShort[], const szFunc[], flags = -1, szInfo[] = "") {
	new szTemp[65], szInfoLang[65];
	format(szInfoLang, 64, "%L", LANG_SERVER, szInfo);

	format(szTemp, 64, "say /%s", szCmd);
	register_clcmd(szTemp, szFunc, flags, szInfoLang);

	format(szTemp, 64, "say .%s", szCmd);
	register_clcmd(szTemp, szFunc, flags, szInfoLang);

	format(szTemp, 64, "/%s", szCmd);
	register_clcmd(szTemp, szFunc, flags, szInfoLang);

	format(szTemp, 64, "%s", szCmd);
	register_clcmd(szTemp, szFunc, flags, szInfoLang);

	format(szTemp, 64, "say /%s", szShort);
	register_clcmd(szTemp, szFunc, flags, szInfoLang);

	format(szTemp, 64, "say .%s", szShort);
	register_clcmd(szTemp, szFunc, flags, szInfoLang);

	format(szTemp, 64, "/%s", szShort);
	register_clcmd(szTemp, szFunc, flags, szInfoLang);

	format(szTemp, 64, "%s", szShort);
	register_clcmd(szTemp, szFunc, flags, szInfoLang);

	return 1;
}