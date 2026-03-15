# pseudoregalia-archipelago
This is a mod for Pseudoregalia that works with the [multi-game multi-world randomizer Archipelago](https://archipelago.gg/).


# Getting Started

* [Game page](/AP_Randomizer/apworld/docs/en_Pseudoregalia.md)
  with more info on Pseudoregalia's Archipelago implementation
  (the first few links don't work on github because this doc is intended for web host)
* [Setup guide](/AP_Randomizer/apworld/docs/setup_en.md)
* [Releases page](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago/releases)
* [General Archipelago guides](https://archipelago.gg/tutorial/)


# Generating a Seed

1. Install [Archipelago](https://archipelago.gg/tutorial/Archipelago/setup/en).
2. [Go to the Releases page](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago/releases/latest) and download the latest version of `pseudoregalia.apworld` and either double click to open the file with Archipelago, or manually place `pseudoregalia.apworld` into `Archipelago/custom_worlds` to install.
3. Place all yamls for the seed into Archipelago/Players.
   * You can get a template yaml file by clicking Generate Template Options after installing the apworld. If you just installed the apworld, you may have to restart the Archipelago Launcher before generating.
4. Run Generate from the Launcher.
5. A zip file will be added to Archipelago/output, which can [hosted on the website](https://archipelago.gg/uploads) or hosted locally by clicking Host.

For a more complete guide, [click here](https://archipelago.gg/tutorial/Archipelago/setup_en#generating-a-game).


# Mod and APWorld Compatibility

Starting in version `0.10.0`, the mod automatically checks the APWorld version used in generation to make sure it's compatible with the mod. Version `0.10.0` also introduced save management, so upgrading the mod might make it incompatible with existing saves. If you need to figure out which mod versions are compatible with a particular APWorld version, the rest of this section should give you the info you need.

In game, the mod version appears in the top right corner on the main menu. The APWorld version for an existing save file is shown on screen when the file is selected.

Version numbers are in the format `major.minor.patch` and a bump in each number has a different meaning:

* A major version bump (e.g. `1.3.2` to `2.0.0`) means a breaking change. This happens when changes are significant enough that the mod will not work with previous APWorld versions. In terms of compatibility, major versions must always match exactly.

  Major version `0` indicates a prerelease version. For example, version `0.10.0` is prerelease but version `1.0.0` is not.

* A minor version bump (e.g. `1.3.2` to `1.4.0`) means there is something new in the APWorld that requires an update in the mod. For non-prerelease versions, a mod is also compatible with an APWorld that has a lower minor version. For example, mod `1.3.2` is compatible with APWorlds `1.3.2` and `1.2.3`, but it isn't compatible with APWorld `1.4.0`.

  For prerelease versions, minor version bumps may also include breaking changes, so the minor version also has to match exactly. For example, mod `0.10.2` is compatible with APWorld `0.10.0`, but mod `0.11.0` is not.

* A patch version bump (e.g. `1.3.2` to `1.3.3`) means a change to either the mod or the APWorld that doesn't require updating the other. Examples include a bug fix in the mod or a small logic update in the APWorld. Patch versions don't matter for compatibility. All that matters is that the major and minor versions are compatible.

  Note: this means that the latest versions of the mod and the APWorld might be different. If the mod gets a patch update in release `1.3.1` but the APWorld is unchanged, that release will still have APWorld `1.3.0` to indicate that the APWorld is exactly the same as the previous release. The latest mod and APWorld will always be compatible with each other.

In summary:

* If a save file has APWorld version `a.b.c` where `a > 0`, only mod versions `a.y.z` where `y >= b` are compatible.
* If a save file has APWorld version `0.b.c` (i.e. is prerelease), only mod versions `0.b.z` are compatible.
