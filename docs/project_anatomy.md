**The following document is for developers! If you are trying to play this mod, please [read the instructions on the front page.](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago)**

The client mod uses [UE4SS](https://github.com/UE4SS-RE/RE-UE4SS) as an entry point and to modify and add game data.

There are three main parts of the client mod:
- A C++ mod which uses the (currently experimental) UE4SS C++ API. This mod is responsible for communicating with Archipelago and hooking into Blueprint functions in order to create callbacks. It often calls Blueprint functions when game data needs to be set.
- A Blueprint mod (AP_Randomizer.pak) which creates custom Unreal Engine assets directly. This mod is responsible for creating new assets as well as reading and writing directly to existing Pseudoregalia assets, such as variables for the GameInstance and player character.
  - The source files for this mod are in the `pseudoregalia-archipelago/pseudoregalia` folder.
- A second Blueprint mod (AP_Randomizer_p.pak) which replaces existing game assets. This includes:
  - the pickup assets so base game pickups don't appear
  - the main menu assets so the modded main menu is loaded
  - the notes so they are interactable
  - save points so they can be interacted with before you have a means of attacking

The reason the mod is split like this is for ease of development. Some things are easier to do in Blueprint than C++.

The Blueprint mod was also required early in development due to features not yet available in the C++ API, so I've chosen to keep using it as the alternative is a relatively expensive refactor and significant changes in program flow.
