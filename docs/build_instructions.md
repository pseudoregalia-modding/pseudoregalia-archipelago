**The following document is for developers! If you are trying to play this mod, please [read the instructions on the front page.](https://github.com/pseudoregalia-modding/pseudoregalia-archipelago)**
<br>
<br>
### Setting up tools
1. Link your GitHub account to your Epic Games account. This is required in order to gain access to the Unreal Engine source code used in UE4SS.
2. [Install Visual Studio 2022,](https://visualstudio.microsoft.com/downloads/) including C++ build tools.
3. [Install CMake.](https://cmake.org/download/)
4. Clone the repo recursively:

    `git clone https://github.com/pseudoregalia-modding/pseudoregalia-archipelago --recursive`

### Building the project
1. Build the project with CMake:

    `cd pseudoregalia-archipelago`
  
    `cmake -S . -B Output`
  
    The solution file will be built to Output/pseudoregalia-archipelago.sln.

2. Open pseudoregalia-archipelago.sln in Visual Studio.
3. Right-click on the AP_Randomizer in the Solution Explorer (usually on the left sidebar), and select Properties.
4. Click the Configuration Manager button on the top right, and set "Active solution configuration" to `Game__Shipping__Win64`.
5. Click Close, then OK.
6. Right-click on AP_Randomizer in the Solution Explorer and select Build.
7. AP_Randomizer.dll will be written to `pseudoregalia-archipelago\Output\AP_Randomizer\Game__Shipping__Win64`.
