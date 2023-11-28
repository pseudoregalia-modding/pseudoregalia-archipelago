**The following document is for developers! If you are trying to play this mod, please [read the instructions on the front page.](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago)**
<br>
<br>
The client mod uses [UE4SS](https://github.com/UE4SS-RE/RE-UE4SS) as an entry point and to modify and add game data.<br>
There are two main parts of the client mod:
- A C++ mod which uses the (currently experimental) UE4SS C++ API. This mod is responsible for communicating with Archipelago and hooking into Blueprint functions in order to create callbacks. It often calls Blueprint functions when game data needs to be set.
- A Blueprint mod which creates Unreal Engine assets directly. This mod is responsible for creating new assets as well as reading and writing directly to existing Pseudoregalia assets, such as variables for the GameInstance and player character.
  - The source files for this mod are in the `pseudoregalia-archipelago/pseudoregalia` folder.
  - The project configuration files for this side of the mod are currently not included in the repository. If you would like to make changes to the Blueprint code please contact me here on GitHub, or on the Archipelago Discord server `@littlemeowmeow0134`, and I will add them.

There is also an AP_Randomizer_p.pak file which should never need to be changed. This file simply erases existing item actors.

<br>
<br>
The reason the mod is split in half like this is for ease of development. Some things are easier to do in Blueprint than C++.

The Blueprint mod was also required early in development due to features not yet available in the C++ API, so I've chosen to keep using it as the alternative is a relatively expensive refactor and significant changes in program flow.
