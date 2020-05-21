```markdown
# @file   : README.md
# @version: 2020-05-21
# @created: 2020-02-01
# @author : pyramid
# @purpose: documentation for libprocu-galaxy
```



**===========================================================================**
**===========================================================================**
**===========================================================================**



=====================================
# **A Procedural Universe Library**
# **LibProcU :: Galaxy** 
# ***libprocu :: galaxy***
=====================================


**Documentation License**
=====================================

This document is published under the [GNU Free Documentation License (FDL)](http://www.gnu.org/licenses/fdl-1.3.html) ([http://www.gnu.org/licenses/fdl-1.3.html](http://www.gnu.org/licenses/fdl-1.3.html)).


Besides this documentation file, all the library code is well commented and the *libprocu-galaxy.hpp* file contains a documentation section at the beginning of the file.

The API is documented in *doc/libprocu-galaxy-api.pdf*. Run the *sh/doxy* script from the library root folder to generate it.



**Code License**
=====================================

The source code of this libprocu-galaxy library is published under  [The GNU General Public License version 3 or later](https://www.gnu.org/licenses/gpl.html)([https://www.gnu.org/licenses/gpl.html](https://www.gnu.org/licenses/gpl.html)).



**Table of Contents**
=====================================

- [Introduction](#introduction)
- [Usage and Quick Guide](#Usage-and-Quick-Guide)
- [Appendices](#appendices)



**Introduction**
=====================================

LibProcU::Galaxy is a single-header library for cosmic galaxy objects.

It is a comprehensive library for procedural galaxy generation including stellar systems, stars, and planets.

LibProcU stands for **Proc**edural **U**niverse **Lib**rary.



**Usage and Quick Guide**
=====================================

You may bind the shared *libprocu-galaxy* library to your project.

The *gengalaxy* command line tool is generating new star systems and you may use it to generate your systems (default output file is galaxy.json) or use the source code as a reference for your own implementation.

Use *gengalaxy* help to list available demos.

Get help: \n
```
  > gengalaxy --help
```

For example, generate one random system and save the objects to json file:\n
```
  > gengalaxy --demo 4
```


Consider the following short code to generate your whole galaxy:

```cpp
  #include "lib/libprocu-galaxy.hpp"
  using namespace procu;

  // create galaxy
  ProcUGalaxy galaxy;
  if (seedGalaxy==0) {
    galaxy.createGalaxySeed(); // pristine seed
  } else {
    galaxy.setGalaxySeed(seedGalaxy); // reuse previous seed
  }

  // define galaxy size
  galaxy.GALAXY_SIZE_LY = {1000,10,1000};

  // generate sectors
  galaxy.genSectors();

  // generate systems
  for (auto& [seedSector, sector] : galaxy.sectors) {
    galaxy.genSystems(seedSector);
    for (auto& systemSeed : sector.systemSeeds) {
      galaxy.genSystem(systemSeed);
    }
  }

  //generate stars and planets
  for (auto& [systemSeed, system] : galaxy.systems) {
      galaxy.genStars(systemSeed);
      for (auto& [starSeed, star] : system.stars) {
        galaxy.genPlanets(system.seed, star.seed);
      }
  }

```

Saving and loading the galaxy seeds

```cpp

  // save galaxy seed
  saveGalaxy(galaxy);

  // load galaxy seed
  loadGalaxy(galaxy);

```

Look for the *gengalaxy* demo code to see how to save all galaxy objects.



**Appendix: Galaxy Model**
=====================================

We adopt the following hierarchical model:

- galaxy
  - sector
  - system
    - planets
      - orbital stations
    - asteroid belts
    - comets
    - space stations



**Appendix: Dependencies**
=====================================

We are using the following libraries (or parts thereof).

For pseudo-random numbers, we use the [pcg random library](https://github.com/wjakob/pcg32).

JSON library for object serialization and deserialization we use [nlohmann::json](https://github.com/nlohmann/json).

The [STB image library](https://github.com/nothings/stb).



**Appendix: File Structure**
=====================================

The library code is in *src/lib/libprocu-galaxy.hpp*.

The command line tool source code is under *src/gengalaxy.cpp*

There are four build scripts included:

- *sh/makelib* - simplified compile script for the shared library
- *sh/makecli* - simplified compile script for the command line generator
- *sh/doxy*    - api documentation create script
- *sh/clean*   - cleans all complied and generated files

You can also run *make* or *cmake* to build the demo code.



```EOF```
