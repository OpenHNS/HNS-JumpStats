enum _:PRESPEED_MODE {
    PRESPEED_MODE_FIXED = 0,
    PRESPEED_MODE_DYNAMIC,
    PRESPEED_MODE_KZRUSH
};

new const g_szPrespeedMode[PRESPEED_MODE][] = {
    "Fixed",
    "Dynamic",
    "KZRush"
};

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

enum _:SETTINGS {
	bool:S_STATS,
    bool:S_STRAFE,
    bool:S_PRESPEED,
    bool:S_CHATINFO,
    bool:S_SOUND,
    S_STATS_R,
    S_STATS_G,
    S_STATS_B,
    S_FAILSTATS_R,
    S_FAILSTATS_G,
    S_FAILSTATS_B,
    S_PRESPEED_R,
    S_PRESPEED_G,
    S_PRESPEED_B,
    S_PRE_GOOD_R,
    S_PRE_GOOD_G,
    S_PRE_GOOD_B,
    S_PRE_BAD_R,
    S_PRE_BAD_G,
    S_PRE_BAD_B,
    S_PRE_MINLOSS_R,
    S_PRE_MINLOSS_G,
    S_PRE_MINLOSS_B,
    S_PRESPEED_MODE,
    S_SPEED_MODE,
    S_PRESPEED_FRAME,
    Float:S_STATS_X,
    Float:S_STATS_Y,
    Float:S_STRAFE_X,
    Float:S_STRAFE_Y,
    Float:S_PRESPEED_X,
    Float:S_PRESPEED_Y,
    S_BHOP_FOGSTATS
};

new g_eSettings[MAX_PLAYERS + 1][SETTINGS];

public settings_player_connect(id) {
    if (g_pCvar[C_ENABLE_STATS]) {
        g_eSettings[id][S_STATS] = true;
    }

    if (g_pCvar[C_ENABLE_STRAFE]) {
        g_eSettings[id][S_STRAFE] = true;
    }

    if (g_pCvar[C_ENABLE_PRESPEED]) {
        g_eSettings[id][S_PRESPEED] = true;
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

    g_eSettings[id][S_PRESPEED_R] = g_pCvar[C_PRESPEED_R];
    g_eSettings[id][S_PRESPEED_G] = g_pCvar[C_PRESPEED_G];
    g_eSettings[id][S_PRESPEED_B] = g_pCvar[C_PRESPEED_B];

    g_eSettings[id][S_PRE_GOOD_R] = g_pCvar[C_PRE_GOOD_R];
    g_eSettings[id][S_PRE_GOOD_G] = g_pCvar[C_PRE_GOOD_G];
    g_eSettings[id][S_PRE_GOOD_B] = g_pCvar[C_PRE_GOOD_B];

    g_eSettings[id][S_PRE_BAD_R] = g_pCvar[C_PRE_BAD_R];
    g_eSettings[id][S_PRE_BAD_G] = g_pCvar[C_PRE_BAD_G];
    g_eSettings[id][S_PRE_BAD_B] = g_pCvar[C_PRE_BAD_B];

    g_eSettings[id][S_PRE_MINLOSS_R] = g_pCvar[C_PRE_MINLOSS_R];
    g_eSettings[id][S_PRE_MINLOSS_G] = g_pCvar[C_PRE_MINLOSS_G];
    g_eSettings[id][S_PRE_MINLOSS_B] = g_pCvar[C_PRE_MINLOSS_B];

    g_eSettings[id][S_PRESPEED_MODE] = g_pCvar[C_PRESPEED_MODE];
    if (g_eSettings[id][S_PRESPEED_MODE] < PRESPEED_MODE_FIXED || g_eSettings[id][S_PRESPEED_MODE] > PRESPEED_MODE_KZRUSH) {
        g_eSettings[id][S_PRESPEED_MODE] = PRESPEED_MODE_DYNAMIC;
    }

    g_eSettings[id][S_SPEED_MODE] = g_pCvar[C_SPEED_MODE];
    if (g_eSettings[id][S_SPEED_MODE] < SPEED_MODE_DEFAULT || g_eSettings[id][S_SPEED_MODE] > SPEED_MODE_NUMBER) {
        g_eSettings[id][S_SPEED_MODE] = SPEED_MODE_DEFAULT;
    }

    g_eSettings[id][S_PRESPEED_FRAME] = g_pCvar[C_PRESPEED_FRAME];

    g_eSettings[id][S_STATS_X] = g_pCvar[C_STATS_X];
    g_eSettings[id][S_STATS_Y] = g_pCvar[C_STATS_Y];

    g_eSettings[id][S_STRAFE_X] = g_pCvar[C_STRAFE_X];
    g_eSettings[id][S_STRAFE_Y] = g_pCvar[C_STRAFE_Y];

    g_eSettings[id][S_PRESPEED_X] = g_pCvar[C_PRESPEED_X];
    g_eSettings[id][S_PRESPEED_Y] = g_pCvar[C_PRESPEED_Y];

    g_eSettings[id][S_BHOP_FOGSTATS] = g_pCvar[C_BHOP_FOGSTATS];
}

public settings_stats_set(id, bool:bStats) {
    if (g_pCvar[C_ENABLE_STATS]) {
        g_eSettings[id][S_STATS] = bStats;
    }
}

public settings_strafe_set(id, bool:bStrafe) {
    if (g_pCvar[C_ENABLE_STRAFE]) {
        g_eSettings[id][S_STRAFE] = bStrafe;
    }
}

public settings_prespeed_set(id, bool:bPreSpeed) {
    if (g_pCvar[C_ENABLE_PRESPEED]) {
        g_eSettings[id][S_PRESPEED] = bPreSpeed;
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

public settings_prespeed_rgb_set(id, iRed, iGreen, iBlue) {
    g_eSettings[id][S_PRESPEED_R] = iRed;
    g_eSettings[id][S_PRESPEED_G] = iGreen;
    g_eSettings[id][S_PRESPEED_B] = iBlue;
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

public settings_prespeed_mode_set(id, iMode) {
    if (iMode < PRESPEED_MODE_FIXED || iMode > PRESPEED_MODE_KZRUSH) {
        iMode = PRESPEED_MODE_DYNAMIC;
    }

    g_eSettings[id][S_PRESPEED_MODE] = iMode;
}

public settings_speed_mode_set(id, iMode) {
    if (iMode < SPEED_MODE_DEFAULT || iMode > SPEED_MODE_NUMBER) {
        iMode = SPEED_MODE_DEFAULT;
    }

    g_eSettings[id][S_SPEED_MODE] = iMode;
}

public settings_prespeed_frame_set(id, iFrame) {
    g_eSettings[id][S_PRESPEED_FRAME] = iFrame;
}

public settings_stats_xy_set(id, Float:flX, Float:flY) {
    g_eSettings[id][S_STATS_X] = flX;
    g_eSettings[id][S_STATS_Y] = flY;
}

public settings_strafe_xy_set(id, Float:flX, Float:flY) {
    g_eSettings[id][S_STRAFE_X] = flX;
    g_eSettings[id][S_STRAFE_Y] = flY;
}

public settings_prespeed_xy_set(id, Float:flX, Float:flY) {
    g_eSettings[id][S_PRESPEED_X] = flX;
    g_eSettings[id][S_PRESPEED_Y] = flY;
}

public settings_bhop_fogstats_set(id, bFogStats) {
    if (g_pCvar[C_BHOP_FOGSTATS]) {
        g_eSettings[id][S_BHOP_FOGSTATS] = bFogStats;
    }
}
