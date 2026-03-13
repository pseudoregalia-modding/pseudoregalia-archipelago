**The following document is for developers! If you are trying to play this mod, please [read the instructions on the front page.](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago)**

### Setting up tools

1. Link your GitHub account to your Epic Games account. This is required in order to gain access to the Unreal Engine source code used in UE4SS.

1. Add an [SSH key](https://docs.github.com/en/authentication/connecting-to-github-with-ssh) to your GitHub account for authentication. This is required in order to clone said Unreal Engine source code.

1. Install [Visual Studio 2022](https://learn.microsoft.com/en-us/visualstudio/releases/2022/release-history). You'll need the 17.14 Community version and may also need the 17.14 Build Tools.

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

    The solution file will be built to `Output\pseudoregalia-archipelago.sln`.

1. Open `pseudoregalia-archipelago.sln` in Visual Studio.

1. Right click `AP_Randomizer` in the Solution Explorer and select Properties.

1. Click the Configuration Manager button and set "Active solution configuration" to `Game__Shipping__Win64`.

1. Click Close, then OK.

1. Right click `AP_Randomizer` in the Solution Explorer and select Build.

    AP_Randomizer.dll will be written to `pseudoregalia-archipelago\Output\AP_Randomizer\Game__Shipping__Win64`. To use it in the mod, place it in the mod's `dlls` folder and rename it to `main.dll`.
