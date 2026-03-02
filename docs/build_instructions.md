**The following document is for developers! If you are trying to play this mod, please [read the instructions on the front page.](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago)**
<br>
<br>

### Setting up tools

1. Link your GitHub account to your Epic Games account. This is required in order to gain access to the Unreal Engine source code used in UE4SS.

1. Add an [SSH key](https://docs.github.com/en/authentication/connecting-to-github-with-ssh) to your GitHub account for authentication. This is required in order to clone said Unreal Engine source code.

1. Install [Visual Studio 2022](https://learn.microsoft.com/en-us/visualstudio/releases/2022/release-history). In order to build, you will need version 17.10 Build Tools, but you can also download the Community version to makes edits.

1. Install [CMake](https://cmake.org/download/).

1. Clone the repo recursively:

    ```
    > git clone https://github.com/pseudoregalia-modding/pseudoregalia-archipelago --recursive
    ```

### Building the project

1. Build the project with CMake:

    ```
    > cd pseudoregalia-archipelago
    > cmake -S . -B Output
    ```

    The solution file will be built to `Output\pseudoregalia-archipelago.sln`. You can open the solution in Visual Studio to make edits, but you will build with the build tools.

1. Launch Visual Studio Build Tools 17.10 from the Visual Studio Installer. This will open a new console.

1. In the build tools console, navigate to the AP_Randomizer project at `pseudoregalia-archipelago/Output/AP_Randomizer`.

1. Build with MSBuild:

    ```
    > msbuild AP_Randomizer.sln -p:Configuration=Game__Shipping__Win64 -p:Platform=x64
    ```

    AP_Randomizer.dll will be written to `pseudoregalia-archipelago\Output\AP_Randomizer\Game__Shipping__Win64`.

1. You can test changes by renaming the file to `main.dll` and replacing `pseudoregalia\Binaries\Win64\Mods\AP_Randomizer\dlls\main.dll` inside your Pseudoregalia Archipelago game installation.
