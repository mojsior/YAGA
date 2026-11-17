# YAGA: Yet Another Gothic Accessibility Attempt

## What is YAGA?

**YAGA** is the **union plugin** which aims to make the game fully playable and accessible for the blind players. This goal is going to be achieved with both speech and sound in order to aid and guide the player throughout the game's world.

## Requirements

### For players

- The steam version of [Gothic 1](https://store.steampowered.com/app/65540/Gothic_1/) or [Gothic II: Night Of The Raven](https://store.steampowered.com/app/39510/Gothic_II_Gold_Edition/).  
Other versions of the game should work however that has not been tested and so is not recommended.
- A screen reader: **NVDA**, **JAWS**, the **Zhengdu Screenreader** (ZDSR) or another one supported by [Prism](https://github.com/ethindp/prism). Without one, YAGA speaks through the Windows voices (OneCore or SAPI).
- [yaga.vdf](https://github.com/Nuno69/YAGA/releases)

### For developers

Everything outlined above plus the following:

- [git lfs](https://git-lfs.com/) (required for fetching lfs files from repo)
- [CMake](https://cmake.org/) 3.28 or newer
- Visual Studio 2022 (or its Build Tools) with the **C++ ATL** component, which [Prism](https://github.com/ethindp/prism) needs
- An internet connection for the first configure: CMake downloads the Prism release source (pinned by its hash)
- [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html)

### Building

Before compiling the mod you need to clone this repository recursively by executing the following command: `git clone --recursive https://github.com/nuno69/yaga`

### building with Visual Studio

1. Open the repository in Visual Studio
2. Choose a build target that matches your game version
3. Build the project with `CTRL+SHIFT+B`

### Building with Command Line

1. Configure the build system with the following command: `cmake --preset G2-Release`
2. Build the mod with `cmake --build --preset G2-Release`
3. Optionally package the mod: `cmake --build out/build/G2-Release --target vdf`

## Settings

Here all the settings of the mod are going to be described. All settings are located in the **gothic.ini** file, in the `[YAGA]` section. If the setting is not in the file, its default value will be restored and written to the file.

### SpeechBackend

YAGA speaks through [Prism](https://github.com/ethindp/prism), which talks to the screen reader or to a speech engine. Possible values are:
- `auto` - the running screen reader; when none runs, a Windows voice (OneCore or SAPI). A screen reader started later takes over within ten seconds, and one that is closed hands over to a Windows voice.
- a Prism backend name, for example `NVDA`, `JAWS`, `ZDSR`, `SAPI` or `OneCore` (exact case). If it does not work, `auto` is used.

Text also goes to the braille display when the screen reader supports it.  
The default value is `auto`.

Prism uses the ZDSR library installed with the screen reader. If an older YAGA was copied into the game by hand, delete `ZDSRAPI.dll` and `nvdaControllerClient32.dll` from `Gothic\system\Autorun`: the old `ZDSRAPI.dll` is loaded by Union first and keeps ZDSR from working. YAGA says so when it finds it.

### SpeechRate

Rate of a Windows voice (screen readers keep their own settings), from `0` to `100`.  
The default value is `50`, the voice's normal rate.

### SpeechVolume

Volume of a Windows voice (screen readers keep their own settings), from `0` to `100`.  
The default value is `50`, the system's normal volume.

### CompassAngleOffset

Controls the global compass offset angle in degrees, when you press `SHIFT+LEFT_ARROW` or `SHIFT+RIGHT_ARROW` your world rotation will be set to next closest angle.  
The default value is `45`.

### NavigatorMode

Controls how the navigator walks the hero along a route:
- `0` - the navigator hands the hero one waypoint at a time to the engine's NPC movement and watches every step (recommended)
- `1` - the whole route is handed to the engine (`EV_GOROUTE`), the navigator only watches the result

The default value is `0`.

### NavigatorProgressEvery

The navigator speaks the remaining distance every this many waypoints. `0` speaks only at the start, on recalculation and on arrival.
The default value is `5`.

### Language

Language of the messages YAGA speaks itself (the game's own texts are always in the game's language):
- `auto` - the language of the game: Polish for the Polish release, English otherwise
- `pl` - Polish
- `en` - English

The default value is `auto`.

### AudioDescription

With `1`, the intro video of Gothic 1, played when a new game starts, is described through the screen reader. See [Audio description](#audio-description). `0` turns it off.
The default value is `1`.

### DevCommands

Development aid. With `1` the plugin reads commands from `yaga_cmd.txt` in the game folder and writes the results to `YAGA.log`: position, route planning, benchmarks, teleporting, an invulnerable hero with monsters removed around it (`god`, `peace`), way net and height map dumps (`dumpnet`, `heightmap`), a graph check of every sector (`routeall`) and a walk through a list of destinations (`tour`).
The default value is `0`.

## Navigation

Press `M` to open the sector map. Arrows move between 50 by 50 metre sectors, `ENTER` describes the selected sector and `SPACE` closes the map and walks the hero to it. `PAGE DOWN` and `PAGE UP` jump between named places (camps, mines, towers, the mountain fortress...) in alphabetical order. A sector with a named place says its name when you enter it with the arrows and on `ENTER`.

- Routes follow the game's own way net, the waypoints NPCs walk on. The goal is the named place of the sector, if it has one, otherwise the reachable waypoint nearest to the centre of the sector.
- The hero is walked by the engine's NPC movement, which steps around small obstacles. Where it gives up, the navigator runs the hero itself with the engine's animation controller and jumps or climbs when the hero stops moving. No key is ever pressed or held for the player: doors and ladders are used through the engine's own mob interaction.
- Ladders on the way are climbed, up or down. Closed doors on the way are opened; locked ones are reported, and so are closed gates. Places behind a gate the game opens by script (the monastery ruins, the fog tower, the Sleeper's temple) lead to the spot in front of it.
- Legs through deep water are swum. They count four times their length when the route is chosen, so a dry way round wins when there is one. The lake below the intro cliff, which kills a swimming hero, is left out.
- When the hero is stuck or pushed off the route, the navigator remembers the blocked leg and recalculates the route around it.
- Any movement key cancels the walk. The walk also stops when the hero takes damage.
- `J` speaks the navigation status: target, remaining distance and waypoint.
- Giving up inside the target sector, or a few metres from the goal, counts as arrival.

### Navigator waypoints

Some places the player can reach have no waypoints, or only a separate patch of them, because the way net was made for NPCs. YAGA adds its own waypoints and ways there. The navigator is the only one to see them: they are not inserted into the game's way net, so NPC routes and savegames do not change.

They come from a table built into the plugin and from `yaga_waynet_patch.txt` in the game folder, read when a world is loaded. One entry per line, `#` starts a comment:

```
world WORLD.ZEN        # following entries apply to this world only
node NAME x y z        # a new waypoint (world coordinates in cm, it is dropped onto the ground below)
link A B [jump]        # a way between two waypoints (new or the game's); "jump" marks a leg that needs a jump
unlink A B             # removes a link given earlier
avoid A B              # a way of the game's way net the hero cannot walk
```

With `DevCommands=1`, `patch` reloads the file and `rec start NAME` / `rec stop` record the hero's walk into `yaga_rec.txt` in this format.

Every spoken line and every navigator decision is written to `YAGA.log` in the game folder. Attach it to bug reports.

## Audio description

The Gothic 1 intro (`INTRO.BIK`) is described in short lines, in Polish or English (see [Language](#language)), through the screen reader.

- **When.** Each line is said in a pause of the narration and dialogue, so it does not cover anyone speaking. The lines follow the video's own frame counter, so they stay in time if the video stutters.
- **What.** The lines say what is on screen: the throne room, the battle with the orcs, the mines, the Barrier, the hero thrown from the cliff.
- **Names.** The people are named where the game names them: the narrator Xardas, King Rhobar II, Bullit and Diego.
- **Skipping.** `ESC` skips the video and silences the line being said.
- **Timing.** The pauses were measured on the Polish release. Another release of the intro with the same length gets the same timing. A video of another length, such as a mod's own intro, gets no description.

The description lines are in `src/Video/AudioDescription_G1.hpp`. The time of every line and the video time when it was said go to `YAGA.log` as `[video]`.

## TODO

- [ ] Perform code clean up. Refactor the code to adhere to single responsibility principle. Put input handling in one place
- [ ] Make the game automatically speak in menus and dialogs
- [ ] Make an accessible audio radar which will inform the player about their surroundings with both speech and sound
- [ ] Create a system which will allow the player to traverse the world from point A to point B (turn by turn navigation). Auto-walk to a map sector is done; turn by turn guidance is not.
- [ ] During combat, report adversary's weapon proficiency.
- [ ] Make all game screens accessible, namely trading, learning, ETC
- [ ] Make all game popups accessible
- [ ] Make the plugin localizable. YAGA's own messages are in Polish and English, picked by the game language; more languages need a translation table.
- [x] Make the plugin configurable

## Contributions

Please keep in mind that I am an inexperienced programmer, and YAGA is my first large-scale project.

Please forgive me any stupid errors I might have. I don't have the skill, I am just very determined to make it happen.

With that in mind if you have any suggestion about the code structure, functions or you want to write your own feature, please drop me a PR.
