enum JS_CVAR {
	c_szPrefix[24],
	c_iEnableStats,
	c_iEnableStrafe,
	c_iEnablePreSpeed,
	c_iEnableConsole,
	c_iEnableChat,
	c_iEnableSound,
	c_iStatsR,
	c_iStatsG,
	c_iStatsB,
	c_iFailR,
	c_iFailG,
	c_iFailB,
	Float:c_flStatsX,
	Float:c_flStatsY,
	Float:c_flStrafeX,
	Float:c_flStrafeY,
	c_iPreSpeedR,
	c_iPreSpeedG,
	c_iPreSpeedB,
	Float:c_iPreSpeedX,
	Float:c_iPreSpeedY,
	c_iStats,
	c_iStrafe,
	c_iPreSpeed,
	c_iConsoleBug,
	c_iBug,
	c_iNoslow,
	Float:c_iGravity
}

new g_pCvar[JS_CVAR];

init_cvars() {
	bind_pcvar_string(register_cvar("js_prefix", "Jump"),	g_pCvar[c_szPrefix], charsmax(g_pCvar[c_szPrefix]));

	bind_pcvar_num(register_cvar("js_enable_stats", "1"),		g_pCvar[c_iEnableStats]);
	bind_pcvar_num(register_cvar("js_enable_strafe", "1"),		g_pCvar[c_iEnableStrafe]);
	bind_pcvar_num(register_cvar("js_enable_prespeed", "1"),	g_pCvar[c_iEnablePreSpeed]);
	bind_pcvar_num(register_cvar("js_enable_console", "1"),		g_pCvar[c_iEnableConsole]);
	bind_pcvar_num(register_cvar("js_enable_chat", "1"),		g_pCvar[c_iEnableChat]);
	bind_pcvar_num(register_cvar("js_enable_sound", "1"),		g_pCvar[c_iEnableSound]);
	
	bind_pcvar_num(register_cvar("js_stats_red", "0"),		g_pCvar[c_iStatsR]);
	bind_pcvar_num(register_cvar("js_stats_green", "200"),	g_pCvar[c_iStatsG]);
	bind_pcvar_num(register_cvar("js_stats_blue", "60"),		g_pCvar[c_iStatsB]);

	bind_pcvar_num(register_cvar("js_failstats_red", "200"),	g_pCvar[c_iFailR]);
	bind_pcvar_num(register_cvar("js_failstats_green", "10"),	g_pCvar[c_iFailG]);
	bind_pcvar_num(register_cvar("js_failstats_blue", "50"),	g_pCvar[c_iFailB]);

	bind_pcvar_float(register_cvar("js_stats_x", "-1.0"),		g_pCvar[c_flStatsX]);
	bind_pcvar_float(register_cvar("js_stats_y", "0.7"),		g_pCvar[c_flStatsY]);

	bind_pcvar_float(register_cvar("js_strafe_x", "0.7"),	g_pCvar[c_flStrafeX]);
	bind_pcvar_float(register_cvar("js_strafe_y", "0.35"),	g_pCvar[c_flStrafeY]);

	bind_pcvar_num(register_cvar("js_prespeed_red", "145"),	g_pCvar[c_iPreSpeedR]);
	bind_pcvar_num(register_cvar("js_prespeed_green", "145"),g_pCvar[c_iPreSpeedG]);
	bind_pcvar_num(register_cvar("js_prespeed_blue", "145"),	g_pCvar[c_iPreSpeedB]);

	bind_pcvar_float(register_cvar("js_prespeed_x", "-1.0"),	g_pCvar[c_iPreSpeedX]);
	bind_pcvar_float(register_cvar("js_prespeed_y", "0.55"),	g_pCvar[c_iPreSpeedY]);

	bind_pcvar_num(register_cvar("js_hud_stats", "2"),		g_pCvar[c_iStats]);
	bind_pcvar_num(register_cvar("js_hud_strafe", "3"),		g_pCvar[c_iStrafe]);
	bind_pcvar_num(register_cvar("js_hud_prespeed", "1"),			g_pCvar[c_iPreSpeed]);

	bind_pcvar_num(register_cvar("js_console_fix", "0"),	g_pCvar[c_iConsoleBug]);

	bind_pcvar_num(register_cvar("js_bug_check", "1"),		g_pCvar[c_iBug]);
	bind_pcvar_num(register_cvar("js_noslowdown", "0"),		g_pCvar[c_iNoslow]);

	bind_pcvar_float(get_cvar_pointer("sv_gravity"),		g_pCvar[c_iGravity]);
}

public plugin_cfg() {
	new szConfig[64], szPath[128];
	get_localinfo("amxx_configsdir", szConfig, charsmax(szConfig));
	formatex(szPath, 99, "%s/jumpstats.cfg", szConfig);

	if (file_exists(szPath)) {
		server_cmd("exec %s", szPath);
		server_exec();
	}
}
