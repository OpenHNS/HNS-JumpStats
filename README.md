## [README in English](https://github.com/WessTorn/HNS-JumpStats/blob/main/README_ENG.md)

# HNS-JumpStats

## Требование
| Название | Версия |
| :- | :- |
| [ReHLDS](https://github.com/rehlds/rehlds) | [![Download](https://img.shields.io/github/v/release/rehlds/rehlds?include_prereleases&style=flat-square)](https://github.com/rehlds/rehlds/releases) |
| [ReGameDLL_CS](https://github.com/rehlds/ReGameDLL_CS/releases) | [![Download](https://img.shields.io/github/v/release/s1lentq/ReGameDLL_CS?include_prereleases&style=flat-square)](https://github.com/rehlds/ReGameDLL_CS/releases) |
| [Metamod-R](https://github.com/rehlds/Metamod-R/releases) | [![Download](https://img.shields.io/github/v/release/rehlds/Metamod-R?include_prereleases&style=flat-square)](https://github.com/rehlds/Metamod-R/releases) |
| [AMXModX (v1.9 or v1.10)](https://www.amxmodx.org/downloads-new.php) | [![Download](https://img.shields.io/badge/AMXModX-%3E%3D1.9.0-blue?style=flat-square)](https://www.amxmodx.org/downloads-new.php) |
| [ReAPI](https://github.com/rehlds/reapi) | [![Download](https://img.shields.io/github/v/release/rehlds/reapi?include_prereleases&style=flat-square)](https://github.com/rehlds/reapi) |

## Прыжки
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

## Описание

### Основная статистика
![stats1](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/ef21fd88-a348-42d9-99ec-1588e196bdb4) ![stats2](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/b69af4f2-1ef6-4c19-934a-1f963b7750d9) ![stats5](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/3532d455-5ded-4d3c-8233-bc7b1be9b12d)

![stats3](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/97c7b379-874d-4adb-97a2-03164eac9bc6) ![stats4](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/df1b2208-9d1c-4863-add3-941b1c4f5114) ![stats6](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/fb2b9f98-790e-476e-86d1-3cdd3f67ad66)

- В первой строчке статистики указано название прыжка. Также показывает тип прыжка (Ladder / Drop / Up / SideWays / BackWards). И кол-во даков.
- Дистанция прыжка.
- Далее идут все элементы престрейфа (начиная с последнего).
- Конечная скорость - максимальная скорость в конце прыжка.
- Кол-во стрейфов

### Статистика стрейфов

![strafe](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/8591efa7-1a42-4612-abc9-228dc188ed00)

1. Зажатые клавиши.
2. Gain, то есть прирост в скорости, полученный на соответствующем стрейфе.
3. Loss, то есть потеря в скорости на соответствующем стрейфе.
4. Кол-во фреймов, которое длился стрейф.
5. Эффективность стрейфа, то есть отношение количества стрейфов, которые давали прирост скорости, к общему количеству фреймов для каждого стрейфа в процентах.
6. Общее кол-во Gain за все стрейфы.
7. Общая эффективность стрейфов (прыжка).
8. Информация о блоке, jof и land (подробнее смотрите ниже).

### Block Jof Land

![jumpoff_landing](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/d25817ff-0239-4864-904c-9a331d895cd5)

### Престрейфы (prespeed - jof, speed, speed)

![prespeed](https://github.com/WessTorn/HNS-JumpStats/assets/63194135/ee3d850a-7739-480d-b9c1-8ac023ad5666)

В prespeed (hud) отображаются jof, тип пресрейфа, speed, FOG и prestrafe pre - скорость до прыжка или дака (перед попаданием на землю) / post -  скорость после

FOG (frames on the ground) - в двух словах, это кол-во frame (кадров) на земле.
- Зачем нам показывают FOG и что мы должны знать?

  FOG нам показывают во время бхопов и даков (dd), тем самым мы понимаем сколько мы находимся на земле и сколько теряем скорость.

  Проще говоря, чем меньше FOG, тем меньше вы потеряете скорость. Скорость престрейфа (pre/post) нам показывает сколько мы потеряли скорости на земле.

  Но! Из-за особенности движка если во время бхопов у вас скорость будет выше 299.973 u/s, то вы потеряете много скорости в любом случае.

## Команды в чат
| Команда | Описание |
| :-: | :-: |
| ljsmenu / ljs | Главное меню |
| strafe / strafestats | Показать/скрыть статистику по стрейфам |
| stats / ljstats | Показать/скрыть основную статистику |
| chatinfo / ci | Показать/скрыть информацию о прыжках в чате |
| showspeed / speed | Показать/скрыть скорость (HUD) |
| showpre / pre | Показать/скрыть престрейфы (HUD) |
| jumpoff / jof | Показать/скрыть jof (HUD) |
| sound / snd | On/Off sounds for good jumps |

## Квары

| Cvar | Default | Описание |
| :--- | :---: | :--- |
| `js_prefix` | Jump | Префикс статистики |
| `js_enable_stats` | 1 | Включает HUD основной статистики |
| `js_enable_strafe` | 1 | Включает HUD статистики стрейфов |
| `js_enable_prespeed` | 1 | Включает HUD скорости, pre и JOF |
| `js_enable_console` | 1 | Включает статистику прыжков в консоли |
| `js_enable_chat` | 1 | Включает статистику лучших прыжков в чате |
| `js_enable_sound` | 1 | Включает звуки лучших прыжков |
| `js_stats_red` | 0 | Красный канал HUD основной статистики |
| `js_stats_green` | 200 | Зелёный канал HUD основной статистики |
| `js_stats_blue` | 60 | Синий канал HUD основной статистики |
| `js_failstats_red` | 200 | Красный канал HUD неудачного прыжка |
| `js_failstats_green` | 10 | Зелёный канал HUD неудачного прыжка |
| `js_failstats_blue` | 50 | Синий канал HUD неудачного прыжка |
| `js_prespeed_red` | 145 | Базовый красный канал prespeed HUD |
| `js_prespeed_green` | 145 | Базовый зелёный канал prespeed HUD |
| `js_prespeed_blue` | 145 | Базовый синий канал prespeed HUD |
| `js_goodpre_red` | 20 | Красный канал GOOD pre в режиме KZRush |
| `js_goodpre_green` | 255 | Зелёный канал GOOD pre в режиме KZRush |
| `js_goodpre_blue` | 150 | Синий канал GOOD pre в режиме KZRush |
| `js_badpre_red` | 255 | Красный канал BAD pre в режиме KZRush |
| `js_badpre_green` | 70 | Зелёный канал BAD pre в режиме KZRush |
| `js_badpre_blue` | 120 | Синий канал BAD pre в режиме KZRush |
| `js_minlosspre_red` | 30 | Красный канал MINLOSS pre в режиме KZRush |
| `js_minlosspre_green` | 135 | Зелёный канал MINLOSS pre в режиме KZRush |
| `js_minlosspre_blue` | 255 | Синий канал MINLOSS pre в режиме KZRush |
| `js_prespeed_mode` | 1 | Режим цвета: `0` — Fixed, `1` — Dynamic, `2` — KZRush |
| `js_speed_mode` | 0 | Стиль скорости: `0` — Default (`u/s`), `1` — Quake (`units/seconds` + `velocity`), `2` — Number |
| `js_prespeed_frame` | 3 | Количество пропускаемых кадров между обновлениями prespeed HUD |
| `js_stats_x` | -1.0 | Координата X HUD основной статистики |
| `js_stats_y` | 0.7 | Координата Y HUD основной статистики |
| `js_strafe_x` | 0.7 | Координата X HUD стрейфов |
| `js_strafe_y` | 0.35 | Координата Y HUD стрейфов |
| `js_prespeed_x` | -1.0 | Координата X prespeed HUD |
| `js_prespeed_y` | 0.55 | Координата Y prespeed HUD |
| `js_bhop_fogstats` | 0 | Включает сводную FOG-статистику серии bhop |
| `js_hud_stats` | 2 | HUD-канал основной статистики |
| `js_hud_strafe` | 3 | HUD-канал статистики стрейфов |
| `js_hud_prespeed` | 1 | HUD-канал prespeed |
| `js_console_fix` | 0 | Добавляет перевод строки для серверов со сломанным форматом консоли |
| `js_noslowdown` | 0 | Отключает проверку slowdown по `fuser2` |
| `js_minmode` | 0 | Включает компактный режим HUD-статистики |

## Звуки

Настроить звуки, путь к звуковым файлам в файле: `scripting/include/jumpstats/global.inc` (g_szSounds)

## Спасибки

[borjomi](https://forums.alliedmods.net/showthread.php?t=141586) - Надеюсь живой, использую алгоритмы uq_jumpstats

[ufame](https://github.com/ufame) - Мой ментор, наставил меня на верный путь.

[Kpoluk](https://github.com/Kpoluk) - Просто гений, вдохновлялся его kz-rush статистикой, использую его основные алгоритмы.

[Garey](https://github.com/Garey27) - Это бренд.
