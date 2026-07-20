# HNS-JumpStats

Hide'n'Seek jump statistics for Counter-Strike 1.6

## Requirement
| Name | Version |
| :- | :- |
| [ReHLDS](https://github.com/rehlds/rehlds) | [![Download](https://img.shields.io/github/v/release/rehlds/rehlds?include_prereleases&style=flat-square)](https://github.com/rehlds/rehlds/releases) |
| [ReGameDLL_CS](https://github.com/rehlds/ReGameDLL_CS/releases) | [![Download](https://img.shields.io/github/v/release/s1lentq/ReGameDLL_CS?include_prereleases&style=flat-square)](https://github.com/rehlds/ReGameDLL_CS/releases) |
| [Metamod-R](https://github.com/rehlds/Metamod-R/releases) | [![Download](https://img.shields.io/github/v/release/rehlds/Metamod-R?include_prereleases&style=flat-square)](https://github.com/rehlds/Metamod-R/releases) |
| [AMXModX (v1.9 or v1.10)](https://www.amxmodx.org/downloads-new.php) | [![Download](https://img.shields.io/badge/AMXModX-%3E%3D1.9.0-blue?style=flat-square)](https://www.amxmodx.org/downloads-new.php) |
| [ReAPI](https://github.com/rehlds/reapi) | [![Download](https://img.shields.io/github/v/release/rehlds/reapi?include_prereleases&style=flat-square)](https://github.com/rehlds/reapi) |

## Jump
- LongJump
- HighJump
- WeirdJump
- CountJump (Drop / Up)
- Multi CountJump (Drop / Up)
- Stand-Up CountJump (Drop / Up)
- Multi Stand-Up CountJump (Drop / Up)
- BhopJump (Ladder / Drop / Up)
- Stand-Up BhopJump (Ladder / Drop / Up)
- BhopInDuck (Ladder / Drop / Up)
- DuckBhop (Ladder / Drop / Up)
- LadderJump

## Description

### Basic stats
![stats1](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/ef21fd88-a348-42d9-99ec-1588e196bdb4) ![stats2](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/b69af4f2-1ef6-4c19-934a-1f963b7750d9) ![stats5](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/3532d455-5ded-4d3c-8233-bc7b1be9b12d)

![stats3](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/97c7b379-874d-4adb-97a2-03164eac9bc6) ![stats4](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/df1b2208-9d1c-4863-add3-941b1c4f5114) ![stats6](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/fb2b9f98-790e-476e-86d1-3cdd3f67ad66)

- The first line of stats shows the name of the jump. Also shows the type of jump (Ladder / Drop / Up / SideWays / BackWards). And number of dacs.
- Distance of the jump.
- Then comes all elements of the pre-strike (starting with the last one).
- Final Speed - maximum speed at the end of the jump.
- Number of strafe

### Strife statistics

![strafe](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/8591efa7-1a42-4612-abc9-228dc188ed00)

1. Keys pressed.
2. Gain, i.e. the increase in speed gained in the corresponding strafe.
3. Loss, i.e. loss of velocity at the corresponding strafe.
4. Number of frames the strafe lasted.
5. the efficiency of a strafe, i.e. the ratio of the number of strafe that gave a speed gain to the total number of frames for each strafe, as a percentage.
6. The total number of Gain for all straights.
7. Total efficiency of the strafe (jump).
8. Information about block, jof and land (see below for details).

### Block Jof Land

![jumpoff_landing](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/d25817ff-0239-4864-904c-9a331d895cd5)

### prestreifs (prespeed - jof, speed, speed)

![prespeed](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/ee3d850a-7739-480d-b9c1-8ac023ad5666)

Prespeed (hud) displays jof, prespeed type, speed, FOG and prestrafe pre - speed before jump or duck (before hitting the ground) / post - speed after

FOG (frames on the ground) - in a nutshell, this is the number of frames (frames) on the ground.
- Why are we shown the FOG and what do we need to know?

  FOG we are shown during bhops and dhacks (dd), thereby we understand how much we are on the ground and how much speed we lose.

  Simply put, the lower the FOG, the less speed you lose. The pre/post speed shows us how much speed we have lost on the ground.

  But, due to the nature of the engine, if you have a speed higher than 299.973 u/s during bhops, then you will lose a lot of speed anyway.

## Commands to Chat
| Command | Description |
| :-: | :-: |
| ljsmenu / ljs | main menu
| strafe / strafestats | show/hide strafe statistics |
| stats / ljstats | show/hide the main stats |
| chatinfo / ci | show/hide info about jumps in the chat room |
| showpeed / speed | show/hide speed (HUD)
| showpre / pre | show/hide prestreifs (HUD) |
| jumpoff / jof | show/hide jof (HUD) |

## Cvar

| Cvar | Default | Description |
| :--- | :---: | :--- |
| `js_prefix` | Jump | Statistics prefix |
| `js_enable_stats` | 1 | Enables the main statistics HUD |
| `js_enable_strafe` | 1 | Enables the strafe statistics HUD |
| `js_enable_prespeed` | 1 | Enables the speed, pre and JOF HUD |
| `js_enable_console` | 1 | Enables jump statistics in the console |
| `js_enable_chat` | 1 | Enables announcements for the best jumps in chat |
| `js_enable_sound` | 1 | Enables sounds for the best jumps |
| `js_stats_red` | 0 | Red channel of the main statistics HUD |
| `js_stats_green` | 200 | Green channel of the main statistics HUD |
| `js_stats_blue` | 60 | Blue channel of the main statistics HUD |
| `js_failstats_red` | 200 | Red channel of the failed-jump HUD |
| `js_failstats_green` | 10 | Green channel of the failed-jump HUD |
| `js_failstats_blue` | 50 | Blue channel of the failed-jump HUD |
| `js_prespeed_red` | 145 | Base red channel of the prespeed HUD |
| `js_prespeed_green` | 145 | Base green channel of the prespeed HUD |
| `js_prespeed_blue` | 145 | Base blue channel of the prespeed HUD |
| `js_goodpre_red` | 20 | Red channel of GOOD pre in KZRush mode |
| `js_goodpre_green` | 255 | Green channel of GOOD pre in KZRush mode |
| `js_goodpre_blue` | 150 | Blue channel of GOOD pre in KZRush mode |
| `js_badpre_red` | 255 | Red channel of BAD pre in KZRush mode |
| `js_badpre_green` | 70 | Green channel of BAD pre in KZRush mode |
| `js_badpre_blue` | 120 | Blue channel of BAD pre in KZRush mode |
| `js_minlosspre_red` | 30 | Red channel of MINLOSS pre in KZRush mode |
| `js_minlosspre_green` | 135 | Green channel of MINLOSS pre in KZRush mode |
| `js_minlosspre_blue` | 255 | Blue channel of MINLOSS pre in KZRush mode |
| `js_prespeed_mode` | 1 | Color mode: `0` — Fixed, `1` — Dynamic, `2` — KZRush |
| `js_speed_mode` | 0 | Speed style: `0` — Default (`u/s`), `1` — Quake (`units/seconds` + `velocity`), `2` — Number |
| `js_prespeed_frame` | 3 | Number of skipped frames between prespeed HUD updates |
| `js_stats_x` | -1.0 | X coordinate of the main statistics HUD |
| `js_stats_y` | 0.7 | Y coordinate of the main statistics HUD |
| `js_strafe_x` | 0.7 | X coordinate of the strafe HUD |
| `js_strafe_y` | 0.35 | Y coordinate of the strafe HUD |
| `js_prespeed_x` | -1.0 | X coordinate of the prespeed HUD |
| `js_prespeed_y` | 0.55 | Y coordinate of the prespeed HUD |
| `js_bhop_fogstats` | 0 | Enables aggregate FOG statistics for a bhop sequence |
| `js_hud_stats` | 2 | HUD channel for the main statistics |
| `js_hud_strafe` | 3 | HUD channel for strafe statistics |
| `js_hud_prespeed` | 1 | HUD channel for prespeed |
| `js_console_fix` | 0 | Adds a line break for servers with broken console formatting |
| `js_noslowdown` | 0 | Disables the slowdown validation based on `fuser2` |
| `js_minmode` | 0 | Enables compact HUD statistics mode |

## Sounds

Customize sounds, path to sound files in file: `scripting/include/jumpstats/global.inc` (g_szSounds)

## Thank you

[borjomi](https://forums.alliedmods.net/showthread.php?t=141586) - Hopefully alive, using uq_jumpstats algorithms

[ufame](https://github.com/ufame) - My mentor, got me on the right track.

[Kpoluk](https://github.com/Kpoluk) - Just a genius, inspired by his kz-rush stats, use his basic algorithms.

[Garey](https://github.com/Garey27) - It's a brand.
