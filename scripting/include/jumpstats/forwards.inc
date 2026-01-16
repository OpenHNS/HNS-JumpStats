/*
new g_hResetBugForward;

public plugin_init() {
	g_hResetBugForward = CreateMultiForward("fwResetBug", ET_IGNORE, FP_CELL);
}

public bug() { 
	new iReturn;
	ExecuteForward(g_hResetBugForward, iReturn, id); // Если нам нужно зафейлить жамп
}
*/

forward fwResetBug(id); // Get reset

public fwResetBug(id) {
	reset_stats(id);
	//g_eFailJump[id] = fj_notshow;
}