# Pseudoregalia


## Links

TODO: setup guide, options page, advanced options page, poptracker pack, github repo


## Items

All items that can be found "out in the open" in the base game are added to the item pool by default.
These include:

* Abilities like Dream Breaker and Slide
* Aspects, the pickups that look like green orbs
* Health pieces
* Major keys
* Small keys

The follow options will swap some default items out with replacements:

* Progressive Dream Breaker:
  swaps Dream Breaker, Strikebreak and Soul Cutter with three Progressive Dream Breaker items.
  This normalizes the order you get access to the swapped out items.
* Progressive Slide: swaps Slide and Solar Wind with two Progressive Slide items.
  This normalizes the order you get access to the swapped out items.
* Split Sun Greaves: swaps Sun Greaves and Heliacal Power with four Air Kick items.
  Each Air Kick acts like Heliacal Power, adding 1 kick.
* Split Cling Gem: swaps Cling Gem with three Cling Shard items.
  Each Cling Shard increases your cling limit by 2.
  (In the base game, you can only cling 6 times with Cling Gem before you touch the ground.)

The Randomize Time Trials option will also add the outfits to the item pool if turned on.
If turned off, the outfits are given to you at the start of the game instead.

Turning on certain options, such as Randomize Goats, will add filler items to balance the item pool.
These are the filler items used in balancing:

* Healing: restores one pip of health
* Major Power: gives one level of magic


## Locations

Any location where a default item listed above is found in the base game can hold a multiworld item.

The Split Sun Greaves and Split Cling Gem options each replace one item with three.
Turning these options on adds two additional locations where you would normally find Sun Greaves/Cling Gem.

If Randomize Time Trials is turned on, each time trial also guards an item.
You have to beat the bronze time to make the item spawn.
It is recommended to have some familiarity with all the time trials before playing with this option.

The following options will also add additional locations:

* Randomize Goats: talk to the goatling to get the item.
  Some goatlings are in easy-to-miss spots, so these videos can help you find them:
  * [Castle Sansa - Trapped Goatling](https://youtu.be/1xmK5C8Jf_Q)
  * [Empty Bailey - Alley Goatling](https://youtu.be/680X5O4OSCs)
  * [Twilight Theatre - Murderous Goatling](https://youtu.be/rB24sE1l-Ak)
* Randomize Chairs: sit down to get the item
* Randomize Books: read the book to get the item
* Randomize Notes: read the note to get the item


## Goal

Just like in the base game, the goal is to collect all 5 major keys to open the Great Door and defeat the Princess.

If you are playing in a multiworld with other people,
you may want to set your major keys to be local so you aren't waiting for someone else to get your MacGuffins.

To make your major keys local, add the following to your Local Items when configuring your options:

* Major Key - Empty Bailey
* Major Key - The Underbelly
* Major Key - Tower Remains
* Major Key - Sansa Keep
* Major Key - Twilight Theatre

If the Major Key Hints option is turned on,
reading the tombstones next to the Great Door will tell you where your major keys are located.
This will automatically create hints.


## Spawn Points

The Spawn Point option allows you to choose (directly or randomly) where you will start the game.
The following spawn points have special behaviors:

* `dungeon_mirror`: the vanilla starting point in Dilapidated Dungeon

    If this option is selected, Start With Breaker will be forced on
    and you will most likely be expected to leave through the dark rooms to The Underbelly.
    See [this video](https://youtu.be/Z_a9l2wzd8c) for a guide on how to do that.

* `library`: the save point at the start of Listless Library

    If this option is selected, either Start With Breaker or Randomize Books must also be turned on.
    If both options are off at the start of generation, Start With Breaker will be forced on.
    
    If just Randomize Books is turned on, your sphere one will have a lot of checks but will be very short,
    so keep that in mind if you are playing in a multiworld with other people.

* `theatre_main`: the save point in the auditorium in Twilight Theatre

    If this option is selected, your starting inventory will include Cling Gem/one Cling Shard on normal,
    or Heliacal Power/one Air Kick on hard+.
    You will be able to leave Twilight Theatre through the big pillar room to the south.

The following spawn points don't have any special behaviors:

* `castle_main`: the save point near Memento in Castle Sansa
* `castle_gazebo`: the save point in the gazebo in the Castle Sansa courtyard
* `underbelly_south`: the save point near the building at the south of The Underbelly
* `underbelly_big_room`: the save point in the big room in the middle of The Underbelly
* `bailey_main`: the save point in Empty Bailey
* `keep_main`: the save point in the main hallway in Sansa Keep
* `keep_north`: the north save point in Sansa Keep


## Item Appearance

TODO: describe how items appear and that this is configurable
