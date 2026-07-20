enum JS_CVAR {
	c_szPrefix[24],
	C_ENABLE_STATS,
	C_ENABLE_STRAFE,
	C_ENABLE_PRESPEED,
	C_ENABLE_CONSOLE,
	C_ENABLE_CHATINFO,
	C_ENABLE_SOUND,
	C_STATS_R,
	C_STATS_G,
	C_STATS_B,
	C_FAILSTATS_R,
	C_FAILSTATS_G,
	C_FAILSTATS_B,
	C_PRESPEED_R,
	C_PRESPEED_G,
	C_PRESPEED_B,
	C_PRE_GOOD_R,
	C_PRE_GOOD_G,
	C_PRE_GOOD_B,
	C_PRE_BAD_R,
	C_PRE_BAD_G,
	C_PRE_BAD_B,
	C_PRE_MINLOSS_R,
	C_PRE_MINLOSS_G,
	C_PRE_MINLOSS_B,
	C_PRESPEED_MODE,
	C_SPEED_MODE,
	C_PRESPEED_FRAME,
	Float:C_STATS_X,
	Float:C_STATS_Y,
	Float:C_STRAFE_X,
	Float:C_STRAFE_Y,
	Float:C_PRESPEED_X,
	Float:C_PRESPEED_Y,
	C_BHOP_FOGSTATS,
	c_iStats,
	c_iStrafe,
	c_iPreSpeed,
	c_iConsoleBug,
	c_iNoslow,
	c_iMinmode,
	Float:c_iGravity
}

new g_pCvar[JS_CVAR];

init_cvars() {
	bind_pcvar_string(register_cvar("js_prefix", "Jump"),	g_pCvar[c_szPrefix], charsmax(g_pCvar[c_szPrefix]));

	bind_pcvar_num(register_cvar("js_enable_stats", "1"),		g_pCvar[C_ENABLE_STATS]);
	bind_pcvar_num(register_cvar("js_enable_strafe", "1"),		g_pCvar[C_ENABLE_STRAFE]);
	bind_pcvar_num(register_cvar("js_enable_prespeed", "1"),	g_pCvar[C_ENABLE_PRESPEED]);
	bind_pcvar_num(register_cvar("js_enable_console", "1"),		g_pCvar[C_ENABLE_CONSOLE]);
	bind_pcvar_num(register_cvar("js_enable_chat", "1"),		g_pCvar[C_ENABLE_CHATINFO]);
	bind_pcvar_num(register_cvar("js_enable_sound", "1"),		g_pCvar[C_ENABLE_SOUND]);
	
	bind_pcvar_num(register_cvar("js_stats_red", "20"),		g_pCvar[C_STATS_R]);
	bind_pcvar_num(register_cvar("js_stats_green", "255"),	g_pCvar[C_STATS_G]);
	bind_pcvar_num(register_cvar("js_stats_blue", "150"),		g_pCvar[C_STATS_B]);

	bind_pcvar_num(register_cvar("js_failstats_red", "255"),	g_pCvar[C_FAILSTATS_R]);
	bind_pcvar_num(register_cvar("js_failstats_green", "70"),	g_pCvar[C_FAILSTATS_G]);
	bind_pcvar_num(register_cvar("js_failstats_blue", "120"),	g_pCvar[C_FAILSTATS_B]);

	bind_pcvar_num(register_cvar("js_prespeed_red", "145"),	g_pCvar[C_PRESPEED_R]);
	bind_pcvar_num(register_cvar("js_prespeed_green", "145"),g_pCvar[C_PRESPEED_G]);
	bind_pcvar_num(register_cvar("js_prespeed_blue", "145"),	g_pCvar[C_PRESPEED_B]);

	bind_pcvar_num(register_cvar("js_goodpre_red", "20"),		g_pCvar[C_PRE_GOOD_R]);
	bind_pcvar_num(register_cvar("js_goodpre_green", "255"),	g_pCvar[C_PRE_GOOD_G]);
	bind_pcvar_num(register_cvar("js_goodpre_blue", "150"),	g_pCvar[C_PRE_GOOD_B]);

	bind_pcvar_num(register_cvar("js_badpre_red", "255"),		g_pCvar[C_PRE_BAD_R]);
	bind_pcvar_num(register_cvar("js_badpre_green", "70"),	g_pCvar[C_PRE_BAD_G]);
	bind_pcvar_num(register_cvar("js_badpre_blue", "120"),	g_pCvar[C_PRE_BAD_B]);

	bind_pcvar_num(register_cvar("js_minlosspre_red", "30"),	g_pCvar[C_PRE_MINLOSS_R]);
	bind_pcvar_num(register_cvar("js_minlosspre_green", "135"),g_pCvar[C_PRE_MINLOSS_G]);
	bind_pcvar_num(register_cvar("js_minlosspre_blue", "255"),	g_pCvar[C_PRE_MINLOSS_B]);

	bind_pcvar_num(register_cvar("js_prespeed_mode", "1"),	g_pCvar[C_PRESPEED_MODE]);
	bind_pcvar_num(register_cvar("js_speed_mode", "0"),		g_pCvar[C_SPEED_MODE]);
	
	bind_pcvar_num(register_cvar("js_prespeed_frame", "3"),	g_pCvar[C_PRESPEED_FRAME]);

	bind_pcvar_float(register_cvar("js_stats_x", "-1.0"),		g_pCvar[C_STATS_X]);
	bind_pcvar_float(register_cvar("js_stats_y", "0.7"),		g_pCvar[C_STATS_Y]);

	bind_pcvar_float(register_cvar("js_strafe_x", "0.7"),	g_pCvar[C_STRAFE_X]);
	bind_pcvar_float(register_cvar("js_strafe_y", "0.35"),	g_pCvar[C_STRAFE_Y]);

	bind_pcvar_float(register_cvar("js_prespeed_x", "-1.0"),	g_pCvar[C_PRESPEED_X]);
	bind_pcvar_float(register_cvar("js_prespeed_y", "0.55"),	g_pCvar[C_PRESPEED_Y]);

	bind_pcvar_num(register_cvar("js_bhop_fogstats", "1"),	g_pCvar[C_BHOP_FOGSTATS]);

	bind_pcvar_num(register_cvar("js_hud_stats", "2"),		g_pCvar[c_iStats]);
	bind_pcvar_num(register_cvar("js_hud_strafe", "3"),		g_pCvar[c_iStrafe]);
	bind_pcvar_num(register_cvar("js_hud_prespeed", "1"),			g_pCvar[c_iPreSpeed]);

	bind_pcvar_num(register_cvar("js_console_fix", "0"),	g_pCvar[c_iConsoleBug]);

	bind_pcvar_num(register_cvar("js_noslowdown", "0"),		g_pCvar[c_iNoslow]);

	bind_pcvar_num(register_cvar("js_minmode", "0"),		g_pCvar[c_iMinmode]);

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
