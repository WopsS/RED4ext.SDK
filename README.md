# RED4ext.SDK

A library to create mods for REDengine 4 ([Cyberpunk 2077](https://www.cyberpunk.net)), independently of [RED4ext](https://github.com/WopsS/RED4ext).

## Usage

### Header only version

Include the [header files](/include) in your project and use a C++17 compiler.

### Static library version

Add the [header files](/include) and the [source files](/src) to your project, define `RED4EXT_STATIC_LIB` and use a C++17 compiler.

## Build instructions

1. Download and install [Visual Studio 2019 Community Edition](https://www.visualstudio.com/) or a higher version.
2. Clone this repository.
3. Go to the `premake` directory and run `generate_project.bat`.
4. Open the solution (`RED4ext.SDK.sln`) located in `project` directory.
5. Build the project.
