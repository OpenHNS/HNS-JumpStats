#if defined _cmds_included
  #endinput
#endif

#define _cmds_included

enum _:JS_ONOFF {
	bool:of_bStrafe,
	bool:of_bStats,
	bool:of_bJof,
	bool:of_bSpeed,
	bool:of_bPre,
	bool:of_bChatInfo,
	bool:of_bSound
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
	if (!g_pCvar[c_iEnableChat])
		return PLUGIN_HANDLED;

	g_eOnOff[id][of_bChatInfo] = !g_eOnOff[id][of_bChatInfo];

	client_print_color(id, print_team_blue, "[^3%s^1] Chatinfo: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bChatInfo] ? "enabled" : "disabled");
	
	return PLUGIN_HANDLED;
}

public cmdShowStats(id) {
	if (!g_pCvar[c_iEnableStats])
		return PLUGIN_HANDLED;

	g_eOnOff[id][of_bStats] = !g_eOnOff[id][of_bStats];

	client_print_color(id, print_team_blue, "[^3%s^1] Stats: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bStats] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdShowStrafe(id) {
	if (!g_pCvar[c_iEnableStrafe])
		return PLUGIN_HANDLED;
		
	g_eOnOff[id][of_bStrafe] = !g_eOnOff[id][of_bStrafe];

	client_print_color(id, print_team_blue, "[^3%s^1] Strafe stats: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bStrafe] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdShowJof(id) {
	if (!g_pCvar[c_iEnablePreSpeed])
		return PLUGIN_HANDLED;

	g_eOnOff[id][of_bJof] = !g_eOnOff[id][of_bJof];

	client_print_color(id, print_team_blue, "[^3%s^1] JumpOff: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bJof] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdShowSpeed(id) {
	if (!g_pCvar[c_iEnablePreSpeed])
		return PLUGIN_HANDLED;

	g_eOnOff[id][of_bSpeed] = !g_eOnOff[id][of_bSpeed];

	client_print_color(id, print_team_blue, "[^3%s^1] Speed: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bSpeed] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdShowPre(id) {
	if (!g_pCvar[c_iEnablePreSpeed])
		return PLUGIN_HANDLED;

	g_eOnOff[id][of_bPre] = !g_eOnOff[id][of_bPre];

	client_print_color(id, print_team_blue, "[^3%s^1] Pre: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bPre] ? "enabled" : "disabled");

	return PLUGIN_HANDLED;
}

public cmdSound(id) {
	if (!g_pCvar[c_iEnableSound])
		return PLUGIN_HANDLED;

	g_eOnOff[id][of_bSound] = !g_eOnOff[id][of_bSound];

	client_print_color(id, print_team_blue, "[^3%s^1] Sound: ^3%s^1.", g_pCvar[c_szPrefix], g_eOnOff[id][of_bSound] ? "enabled" : "disabled");

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