# pseudoregalia-archipelago
This is a mod for Pseudoregalia that works with the [multi-game multi-world randomizer Archipelago](https://archipelago.gg/). It is currently in early alpha and is being actively developed. Abilities, minor upgrades, major keys, and small keys are shuffled.

You can get the latest release from [the releases page](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago/releases).

Currently the logic is copied from [spuds' standalone randomizer](https://github.com/pseudoregalia-modding/rando), and may require [movement tricks such as solar ultrahops and cling climbing](https://www.youtube.com/watch?v=kZJjYdh6huk).

You'll spawn in Dungeon with Dream Breaker at its vanilla location, and you'll have to leave through the breakable wall to Underbelly. If you toss the breaker and bounce it off a wall, then pick it up, it'll continue to light your path.


# Mod Installation
1. In Steam/steamapps/common, make a copy of your Pseudoregalia folder and rename it "Pseudoregalia_AP" (or whatever else you like).
2. Extract the contents of pseudoregalia-archipelago.zip directly into "Pseudoregalia_AP". You should notice no change to the root folder, but the mod files will be added to the subfolders.

![](https://i.imgur.com/SGPm9oq.jpg)

4. Create a shortcut to Pseudoregalia_AP/pseudoregalia.exe and rename it "Pseudoregalia_AP" (or whatever else you like). Place it on your desktop or in your games folder or whatever.
5. Launch the shortcut, which should open Pseudoregalia. Start a new file.
6. Once you've loaded into dungeon, press F10 to open the in-game console, and enter your connect message:

`/connect ip:port slotname password`


# Generating a Seed
1. [Install Archipelago](https://archipelago.gg/tutorial/Archipelago/setup/en), including at least the generator.

![](https://i.imgur.com/9IedtY4.jpg)

2. Place pseudoregalia.apworld into Archipelago/lib/worlds.
3. Place all yamls for the seed into Archipelago/Players.
4. Run ArchipelagoGenerate.exe.
5. A zip file will be added to Archipelago/output, which can [hosted on the website](https://archipelago.gg/uploads) or hosted locally with ArchipelagoServer.exe.

# Known Issues
- If you enter your connection info incorrectly, you have to restart the game to try again.
- Consumed small keys will be restored on scene transition.
- Messages printed to the screen will be deleted when passing through a loading zone even if you didn't have time to read them.
- If you get a lot of messages at once, such as from a released world, they'll extend off your screen.
- A crash can occur when entering incorrect connection info under certain conditions.

# Planned Updates
- Spawning in Castle instead of Dungeon
- Option to shuffle Dream Breaker
- Reworked logic to make movement tricks opt-in
- And maybe a bit more...