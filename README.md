# pseudoregalia-archipelago
This is a mod for Pseudoregalia that works with the [multi-game multi-world randomizer Archipelago](https://archipelago.gg/). It is currently in alpha and is being actively developed. Abilities, minor upgrades, major keys, small keys, and health pieces are shuffled.

You can get the latest release from [the releases page](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago/releases).

Currently the logic is mostly copied from [spuds' standalone randomizer](https://github.com/pseudoregalia-modding/rando), and may require [movement tricks such as solar ultrahops and cling climbing](https://www.youtube.com/watch?v=kZJjYdh6huk).

You'll spawn in Dungeon with Dream Breaker at its vanilla location, and you'll have to leave through the breakable wall to Underbelly by backflipping up to the first ledge. If you toss the breaker and bounce it off a wall, then pick it up, it'll continue to light your path.

[You can get the PopTracker pack for Pseudoregalia here.](https://github.com/Br00ty/pseudoregalia_brooty)


# Extra Features
- You can toggle Solar Wind once you've obtained it by pressing the top face button on controller or left ctrl on keyboard.
- Entering `/popups mute` into the console will mute the sound effects of send/receive popups.
- Entering `/popups hide` into the console will stop send/receive popups from appearing completely.


# Mod Installation
Note: If you are installing on Steam Deck, please read [the Steam Deck installation instructions](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago/blob/main/docs/steam_deck_installation.md) instead.
1. [Go to the Releases page](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago/releases/latest) and download the latest version of `pseudoregalia-archipelago.zip` and `Pseudoregalia.yaml`. You don't need the apworld or weighted yaml if you don't know what those are.
2. In Steam/steamapps/common, make a copy of your Pseudoregalia folder and rename it "Pseudoregalia_AP" (or whatever else you like).
3. Extract the contents of pseudoregalia-archipelago.zip directly into "Pseudoregalia_AP". You should see README.md appear in the folder.

![](https://i.imgur.com/SGPm9oq.jpg)

4. Create a shortcut to Pseudoregalia_AP/pseudoregalia.exe and rename it "Pseudoregalia_AP" (or whatever else you like). Place it on your desktop or in your games folder or whatever.
5. Launch the shortcut, which should open Pseudoregalia. Start a new file.
6. Once you've loaded into dungeon, press F10 to open the in-game console, and enter your connect message:

`/connect ip:port slotname password`


# Generating a Seed
1. [Install Archipelago,](https://archipelago.gg/tutorial/Archipelago/setup/en) including at least the generator.

![](https://i.imgur.com/9IedtY4.jpg)

2. Place pseudoregalia.apworld into Archipelago/lib/worlds.
3. Place all yamls for the seed into Archipelago/Players.
4. Run ArchipelagoGenerate.exe.
5. A zip file will be added to Archipelago/output, which can [hosted on the website](https://archipelago.gg/uploads) or hosted locally with ArchipelagoServer.exe.


# Known Issues
- The mod will not always fully load before starting the game, especially the first time. If the AP logo doesn't appear in the top right corner of the main menu, try restarting the game.
- If you're on top of an item when you first connect, it won't spawn until you reload the scene.


# Planned Updates
- Spawning in Castle instead of Dungeon
- Option to shuffle Dream Breaker
- Reworked logic to make movement tricks opt-in
- And maybe a bit more...
