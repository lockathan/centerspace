# Center Space

Center Space is a turn based science fiction video game targeting the PC platform written by me in 2012/2013. The game is written in C++ and uses Ogre, BulletPhysics, Lua and OpenAL.

The game was envisioned as a spiritual successor to MissionForce: Cyberstorm. Hence the initials being CS. In Center Space, you command a team of giant robots, or Mechanised Combat Units (MCUs), in battle against other teams of MCUs. Between missions, you earn credits and unlock new technology.

The game is played in turn based fashion, and the movement grid is square, rather than hexadecimal as in the original MissionForce: Cyberstorm. I chose a square grid because it was easier to implement and allowed for easy sloped terrain.

## State of the Code

The project is presented in the state that it was when I stopped working on it years ago. Therefore, expect some challenges in getting it to build on any platform. This should be easier on Windows than on other platforms, since the game originally targeted Windows.

I must also stress that this is not likely to be the best C++ code anyone has ever seen. Temper your expectations in terms of code quality!

## Dependencies

* Visual C++ Runtime. Likely an older version, such as [this one](https://www.microsoft.com/en-gb/download/details.aspx?id=40784)
* [DirectX June 2010 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
* Ogre3D (I recommend an older version, such as [1.9.1](https://sourceforge.net/projects/ogre/files/v1.9.1/))
* Ogre3D dependencies (Again, use an [older version](https://sourceforge.net/projects/ogre/files/ogre-dependencies-vc%2B%2B/1.7/))
* BulletPhysics (Try an older version, such as [2.82](https://github.com/bulletphysics/bullet3/releases/tag/2.82)
* [LibRocket](https://github.com/libRocket/libRocket)
* [OpenAL](http://www.openal.org/downloads/)
* [OgreAL](https://wiki.ogre3d.org/OgreAL)
* [LuaJIT](https://luajit.org/luajit.html)
* [tolua++](http://webserver2.tecgraf.puc-rio.br/~celes/tolua/tolua-3.2.html) for generating Lua interfaces

## Building on Windows

Obtain and install the above dependencies. You'll need compiled DLL files for everything except the C++ runtime because everything is dynamically linked. You'll also need the header files.

The Visual Studio .sln and .vcxproj files will have incorrect paths pointing to the header files - these need to be corrected.

## Building on Linux

Not officially supported - try this at your own risk.

## Support

No official support is provided for this repository, but feel free to ask any questions you like and I'll try to answer as best as I'm able to.

## Included

* The core game code written in C++ with some parts written in Lua (notably the higher level AI code and terrain generation code).
* User interface files, including images.
* Sound effect and music files.
* Robot models and textures

## Not included

During development of the game, I licensed a third party texture pack for use as the terrain. Obviously I don't have the license to redistribute these textures, so these are not included.

## Features

Some features implemented in the code may be useful to people wanting to implement similar features in their own games. This game contains implementations of:
* A-star pathfinding algorithm (see Managers/PathFindingManager.cpp)
* Isometric grid terrain generation with height variation. Generates a single mesh for the terrain with no duplicated vertices with correct texture mapping coordinates - quite proud of this one!
* Working shadows
* A minimap
* Fog of war

## Work required

My suggested roadmap for new features to implement would be:
* Switch to using a modern build system like CMake
* Upgrade to newer versions of all of the dependencies, most especially Ogre itself.
* Polish the combat experience, especially the missiles which don't work well.
* Implement progression in the game, including story but also including technology
* Polish the menu interface

## License

The source code, art assets and sound assets for Center Space are licensed under the MIT license as found in LICENSE. Some source code files are taken from third party sources and are marked as such by declarations at the top of the source file. These files are not covered by the Center Space license but by their own respective licenses.