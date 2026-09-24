enum _:SPEED_MODE {
    SPEED_MODE_DEFAULT = 0,
    SPEED_MODE_QUAKE,
    SPEED_MODE_NUMBER
};

new const g_szSpeedMode[SPEED_MODE][] = {
    "Default",
    "Quake",
    "Number"
};

enum _:JS_ONOFF {
    bool:of_bJof,
    bool:of_bSpeed,
    bool:of_bPre
};

enum _:SETTINGS {
	bool:S_STATS,
    bool:S_CHATINFO,
    bool:S_SOUND,
    S_STATS_R,
    S_STATS_G,
    S_STATS_B,
    S_FAILSTATS_R,
    S_FAILSTATS_G,
    S_FAILSTATS_B,
    S_PRE_R,
    S_PRE_G,
    S_PRE_B,
    S_PRE_GOOD_R,
    S_PRE_GOOD_G,
    S_PRE_GOOD_B,
    S_PRE_BAD_R,
    S_PRE_BAD_G,
    S_PRE_BAD_B,
    S_PRE_MINLOSS_R,
    S_PRE_MINLOSS_G,
    S_PRE_MINLOSS_B,
    S_SPEED_MIN_R,
    S_SPEED_MIN_G,
    S_SPEED_MIN_B,
    S_SPEED_MAX_R,
    S_SPEED_MAX_G,
    S_SPEED_MAX_B,
    S_SPEED_MODE,
    S_SPEED_FRAME,
    Float:S_STATS_X,
    Float:S_STATS_Y,
    Float:S_PRE_X,
    Float:S_PRE_Y,
    Float:S_SPEED_X,
    Float:S_SPEED_Y,
    S_BHOP_FOGSTATS
};

new g_eSettings[MAX_PLAYERS + 1][SETTINGS];
new g_eOnOff[MAX_PLAYERS + 1][JS_ONOFF];

public settings_player_connect(id) {
    arrayset(g_eSettings[id], 0, SETTINGS);

    if (g_pCvar[C_ENABLE_STATS]) {
        g_eSettings[id][S_STATS] = true;
    }

    if (g_pCvar[C_ENABLE_CHATINFO]) {
        g_eSettings[id][S_CHATINFO] = true;
    }

    if (g_pCvar[C_ENABLE_SOUND]) {
        g_eSettings[id][S_SOUND] = true;
    }

    g_eSettings[id][S_STATS_R] = g_pCvar[C_STATS_R];
    g_eSettings[id][S_STATS_G] = g_pCvar[C_STATS_G];
    g_eSettings[id][S_STATS_B] = g_pCvar[C_STATS_B];

    g_eSettings[id][S_FAILSTATS_R] = g_pCvar[C_FAILSTATS_R];
    g_eSettings[id][S_FAILSTATS_G] = g_pCvar[C_FAILSTATS_G];
    g_eSettings[id][S_FAILSTATS_B] = g_pCvar[C_FAILSTATS_B];

    g_eSettings[id][S_PRE_R] = g_pCvar[C_PRE_R];
    g_eSettings[id][S_PRE_G] = g_pCvar[C_PRE_G];
    g_eSettings[id][S_PRE_B] = g_pCvar[C_PRE_B];

    g_eSettings[id][S_PRE_GOOD_R] = g_pCvar[C_PRE_GOOD_R];
    g_eSettings[id][S_PRE_GOOD_G] = g_pCvar[C_PRE_GOOD_G];
    g_eSettings[id][S_PRE_GOOD_B] = g_pCvar[C_PRE_GOOD_B];

    g_eSettings[id][S_PRE_BAD_R] = g_pCvar[C_PRE_BAD_R];
    g_eSettings[id][S_PRE_BAD_G] = g_pCvar[C_PRE_BAD_G];
    g_eSettings[id][S_PRE_BAD_B] = g_pCvar[C_PRE_BAD_B];

    g_eSettings[id][S_PRE_MINLOSS_R] = g_pCvar[C_PRE_MINLOSS_R];
    g_eSettings[id][S_PRE_MINLOSS_G] = g_pCvar[C_PRE_MINLOSS_G];
    g_eSettings[id][S_PRE_MINLOSS_B] = g_pCvar[C_PRE_MINLOSS_B];

    g_eSettings[id][S_SPEED_MIN_R] = g_pCvar[C_SPEED_MIN_R];
    g_eSettings[id][S_SPEED_MIN_G] = g_pCvar[C_SPEED_MIN_G];
    g_eSettings[id][S_SPEED_MIN_B] = g_pCvar[C_SPEED_MIN_B];

    g_eSettings[id][S_SPEED_MAX_R] = g_pCvar[C_SPEED_MAX_R];
    g_eSettings[id][S_SPEED_MAX_G] = g_pCvar[C_SPEED_MAX_G];
    g_eSettings[id][S_SPEED_MAX_B] = g_pCvar[C_SPEED_MAX_B];

    g_eSettings[id][S_SPEED_MODE] = g_pCvar[C_SPEED_MODE];
    if (g_eSettings[id][S_SPEED_MODE] < SPEED_MODE_DEFAULT || g_eSettings[id][S_SPEED_MODE] > SPEED_MODE_NUMBER) {
        g_eSettings[id][S_SPEED_MODE] = SPEED_MODE_DEFAULT;
    }

    g_eSettings[id][S_SPEED_FRAME] = g_pCvar[C_SPEED_FRAME];

    g_eSettings[id][S_STATS_X] = g_pCvar[C_STATS_X];
    g_eSettings[id][S_STATS_Y] = g_pCvar[C_STATS_Y];

    g_eSettings[id][S_PRE_X] = g_pCvar[C_PRE_X];
    g_eSettings[id][S_PRE_Y] = g_pCvar[C_PRE_Y];

    g_eSettings[id][S_SPEED_X] = g_pCvar[C_SPEED_X];
    g_eSettings[id][S_SPEED_Y] = g_pCvar[C_SPEED_Y];

    g_eSettings[id][S_BHOP_FOGSTATS] = g_pCvar[C_BHOP_FOGSTATS];
}

public settings_stats_set(id, bool:bStats) {
    if (g_pCvar[C_ENABLE_STATS]) {
        g_eSettings[id][S_STATS] = bStats;
    }
}

public settings_chatinfo_set(id, bool:bChatInfo) {
    if (g_pCvar[C_ENABLE_CHATINFO]) {
        g_eSettings[id][S_CHATINFO] = bChatInfo;
    }
}

public settings_sound_set(id, bool:bSound) {
    if (g_pCvar[C_ENABLE_SOUND]) {
        g_eSettings[id][S_SOUND] = bSound;
    }
}

public settings_stats_rgb_set(id, iRed, iGreen, iBlue) {
    g_eSettings[id][S_STATS_R] = iRed;
    g_eSettings[id][S_STATS_G] = iGreen;
    g_eSettings[id][S_STATS_B] = iBlue;
}

public settings_failstats_rgb_set(id, iRed, iGreen, iBlue) {
    g_eSettings[id][S_FAILSTATS_R] = iRed;
    g_eSettings[id][S_FAILSTATS_G] = iGreen;
    g_eSettings[id][S_FAILSTATS_B] = iBlue;
}

public settings_pre_rgb_set(id, iRed, iGreen, iBlue) {
    g_eSettings[id][S_PRE_R] = iRed;
    g_eSettings[id][S_PRE_G] = iGreen;
    g_eSettings[id][S_PRE_B] = iBlue;
}

public settings_pre_good_rgb_set(id, iRed, iGreen, iBlue) {
    g_eSettings[id][S_PRE_GOOD_R] = iRed;
    g_eSettings[id][S_PRE_GOOD_G] = iGreen;
    g_eSettings[id][S_PRE_GOOD_B] = iBlue;
}

public settings_pre_bad_rgb_set(id, iRed, iGreen, iBlue) {
    g_eSettings[id][S_PRE_BAD_R] = iRed;
    g_eSettings[id][S_PRE_BAD_G] = iGreen;
    g_eSettings[id][S_PRE_BAD_B] = iBlue;
}

public settings_pre_minloss_rgb_set(id, iRed, iGreen, iBlue) {
    g_eSettings[id][S_PRE_MINLOSS_R] = iRed;
    g_eSettings[id][S_PRE_MINLOSS_G] = iGreen;
    g_eSettings[id][S_PRE_MINLOSS_B] = iBlue;
}

public settings_speed_min_rgb_set(id, iRed, iGreen, iBlue) {
    g_eSettings[id][S_SPEED_MIN_R] = iRed;
    g_eSettings[id][S_SPEED_MIN_G] = iGreen;
    g_eSettings[id][S_SPEED_MIN_B] = iBlue;
}

public settings_speed_max_rgb_set(id, iRed, iGreen, iBlue) {
    g_eSettings[id][S_SPEED_MAX_R] = iRed;
    g_eSettings[id][S_SPEED_MAX_G] = iGreen;
    g_eSettings[id][S_SPEED_MAX_B] = iBlue;
}

public settings_speed_mode_set(id, iMode) {
    if (iMode < SPEED_MODE_DEFAULT || iMode > SPEED_MODE_NUMBER) {
        iMode = SPEED_MODE_DEFAULT;
    }

    g_eSettings[id][S_SPEED_MODE] = iMode;
}

public settings_speed_frame_set(id, iFrame) {
    g_eSettings[id][S_SPEED_FRAME] = iFrame;
}

public settings_stats_xy_set(id, Float:flX, Float:flY) {
    g_eSettings[id][S_STATS_X] = flX;
    g_eSettings[id][S_STATS_Y] = flY;
}

public settings_pre_xy_set(id, Float:flX, Float:flY) {
    g_eSettings[id][S_PRE_X] = flX;
    g_eSettings[id][S_PRE_Y] = flY;
}

public settings_speed_xy_set(id, Float:flX, Float:flY) {
    g_eSettings[id][S_SPEED_X] = flX;
    g_eSettings[id][S_SPEED_Y] = flY;
}

public settings_bhop_fogstats_set(id, bFogStats) {
    if (g_pCvar[C_BHOP_FOGSTATS]) {
        g_eSettings[id][S_BHOP_FOGSTATS] = bFogStats;
    }
}

const SETTINGS_STORAGE_VERSION = 1;
const SETTINGS_STORAGE_TOKENS = 1 + JS_ONOFF + SETTINGS;

new g_iSettingsVault = INVALID_HANDLE;

stock settings_storage_init() {
    g_iSettingsVault = nvault_open("jumpstats_settings");

    if (g_iSettingsVault == INVALID_HANDLE) {
        set_fail_state("Could not open jumpstats_settings nVault");
    }
}

stock settings_storage_close() {
    if (g_iSettingsVault != INVALID_HANDLE) {
        nvault_close(g_iSettingsVault);
        g_iSettingsVault = INVALID_HANDLE;
    }
}

stock settings_apply_defaults(const id) {
    arrayset(g_eOnOff[id], true, JS_ONOFF);
    settings_player_connect(id);
}

stock bool:settings_save_player(const id) {
    new szAuthId[35];

    if (!GetSettingsAuthId(id, szAuthId, charsmax(szAuthId))) {
        return false;
    }

    new szData[512];
    new iLen = formatex(szData, charsmax(szData), "%d", SETTINGS_STORAGE_VERSION);

    for (new i = 0; i < JS_ONOFF; i++) {
        iLen += formatex(szData[iLen], charsmax(szData) - iLen, " %d", g_eOnOff[id][i]);
    }

    for (new i = 0; i < SETTINGS; i++) {
        iLen += formatex(szData[iLen], charsmax(szData) - iLen, " %d", g_eSettings[id][i]);
    }

    nvault_set(g_iSettingsVault, szAuthId, szData);
    return true;
}

stock bool:settings_load_player(const id, const szAuthId[]) {
    if (g_iSettingsVault == INVALID_HANDLE || !IsValidSettingsAuthId(szAuthId)) {
        return false;
    }

    new szData[512];

    if (!nvault_get(g_iSettingsVault, szAuthId, szData, charsmax(szData))) {
        return false;
    }

    if (CountSettingsTokens(szData) != SETTINGS_STORAGE_TOKENS) {
        return false;
    }

    new szToken[16];
    new iPosition = argparse(szData, 0, szToken, charsmax(szToken));

    if (str_to_num(szToken) != SETTINGS_STORAGE_VERSION) {
        return false;
    }

    new iSavedOnOff[JS_ONOFF];
    new iSavedSettings[SETTINGS];

    for (new i = 0; i < JS_ONOFF; i++) {
        iPosition = argparse(szData, iPosition, szToken, charsmax(szToken));
        iSavedOnOff[i] = str_to_num(szToken);
    }

    for (new i = 0; i < SETTINGS; i++) {
        iPosition = argparse(szData, iPosition, szToken, charsmax(szToken));
        iSavedSettings[i] = str_to_num(szToken);
    }

    for (new i = 0; i < JS_ONOFF; i++) {
        g_eOnOff[id][i] = bool:(iSavedOnOff[i] != 0);
    }

    for (new i = 0; i < SETTINGS; i++) {
        g_eSettings[id][i] = iSavedSettings[i];
    }

    ValidateLoadedSettings(id);
    return true;
}

stock bool:settings_reset_player(const id) {
    settings_apply_defaults(id);

    new szAuthId[35];

    if (!GetSettingsAuthId(id, szAuthId, charsmax(szAuthId))) {
        return false;
    }

    nvault_remove(g_iSettingsVault, szAuthId);
    return true;
}

stock bool:GetSettingsAuthId(const id, szAuthId[], const iLen) {
    if (g_iSettingsVault == INVALID_HANDLE || is_user_bot(id) || is_user_hltv(id)) {
        return false;
    }

    get_user_authid(id, szAuthId, iLen);
    return IsValidSettingsAuthId(szAuthId);
}

stock bool:IsValidSettingsAuthId(const szAuthId[]) {
    return szAuthId[0]
        && !equal(szAuthId, "STEAM_ID_PENDING")
        && !equal(szAuthId, "STEAM_ID_LAN")
        && !equal(szAuthId, "VALVE_ID_LAN")
        && !equal(szAuthId, "BOT")
        && !equal(szAuthId, "HLTV");
}

stock CountSettingsTokens(const szData[]) {
    new szToken[16];
    new iPosition;
    new iCount;

    while ((iPosition = argparse(szData, iPosition, szToken, charsmax(szToken))) != -1) {
        iCount++;
    }

    return iCount;
}

stock ValidateLoadedSettings(const id) {
    g_eSettings[id][S_STATS] = bool:(g_eSettings[id][S_STATS] != false);
    g_eSettings[id][S_CHATINFO] = bool:(g_eSettings[id][S_CHATINFO] != false);
    g_eSettings[id][S_SOUND] = bool:(g_eSettings[id][S_SOUND] != false);

    for (new i = S_STATS_R; i <= S_SPEED_MAX_B; i++) {
        g_eSettings[id][i] = clamp(g_eSettings[id][i], 0, 255);
    }

    g_eSettings[id][S_SPEED_MODE] = clamp(g_eSettings[id][S_SPEED_MODE], SPEED_MODE_DEFAULT, SPEED_MODE_NUMBER);
    g_eSettings[id][S_SPEED_FRAME] = clamp(g_eSettings[id][S_SPEED_FRAME], 2, 5);

    g_eSettings[id][S_STATS_X] = floatclamp(g_eSettings[id][S_STATS_X], -1.0, 1.0);
    g_eSettings[id][S_STATS_Y] = floatclamp(g_eSettings[id][S_STATS_Y], 0.0, 1.0);
    g_eSettings[id][S_PRE_X] = floatclamp(g_eSettings[id][S_PRE_X], -1.0, 1.0);
    g_eSettings[id][S_PRE_Y] = floatclamp(g_eSettings[id][S_PRE_Y], 0.0, 1.0);
    g_eSettings[id][S_SPEED_X] = floatclamp(g_eSettings[id][S_SPEED_X], -1.0, 1.0);
    g_eSettings[id][S_SPEED_Y] = floatclamp(g_eSettings[id][S_SPEED_Y], 0.0, 1.0);

    g_eSettings[id][S_BHOP_FOGSTATS] = bool:(g_eSettings[id][S_BHOP_FOGSTATS] != 0);
}
