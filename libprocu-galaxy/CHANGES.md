```markdown
#====================================
# @file   : libprocu-galaxy/CHANGES.md
# @version: 2020-05-21
# @created: 2020-02-01
# @author : pyramid
# @purpose: version documentation for libprocu-galaxy
#====================================
```



=====================================
# libProcU::Galaxy
=====================================

=====================================
## version history
=====================================

v0.00.29 | 2020-05-21

- libprocu-galaxy: minor documentation changes
- README: minor documentation changes
- CHANGES: minor documentation changes

v0.00.28 | 2020-03-22

- doc: updated readme
- lib added json serializer for planet
- lib: updated comments

v0.00.27 | 2020-03-20
- gen: added demo 5: create whole galaxy
- gen: updated demo 4 with sector
- lib: removed vector3d include
- lib: added genSector
- lib: added sector serialization
- lib: added UniverseSector model
- lib: added stars json to system serializer

v0.00.26 | 2020-03-19
- gen: added demo 4
- vector3d: added constructor for std::vector<float>
- lib: added json serializer for system
- lib: added json serializer for star
- lib: changes system.pos to vector

v0.00.25 | 2020-03-18
- sh: updated clean
- gen: added demo 3 sae and load galaxy seed
- lib: added saveGalaxy, loadGalaxy
- lib: renamed uGalaxySeed to galaxySeed
- lib: added ext/json.hpp library

v0.00.24 | 2020-03-17
- doc: all code from (unpublished) SpaceExplorer Java source
  ported to c++
- gen: updated planet generator output
- lib: added hasPlanetsInHz for stars
- lib: added getHabitablePlanetsProbability fos stars
- lib: calc planet.mu
- lib: added calcPlanetHabitability
- lib: added planet isInHz flag
- lib: moved object helper functions out of galaxy class
- lib: updated documentation
- lib: added oxygen exists in composition check
- lib: atmosphereHabitability

v0.00.23 | 2020-03-16
- gen: atmosphere generator output
- lib: added atmosphere composition
- lib: added atmosphere.exists() check
- lib: atmosphere exists check: radius>0

v0.00.22 | 2020-03-15
- lib: calc planet atmosphere probability

v0.00.21 | 2020-03-14
- gen: updated star and planet log
- lib: added getPlanetTypeIndex
- lib: added star rotation
- lib: updated genPlanet function docu (comment)
- lib: moved generator code to genPlanet
- lib: refactored genPlanets code

v0.00.20 | 2020-03-12
- gen: improving output
- lib: improving planet code

v0.00.19 | 2020-03-12
- lib: generating planet data
- lib: planet generator with nebular hypothesis
- lib: updated system and star seed formulas

v0.00.18 | 2020-03-11
- gen: get frost limit
- lib: gen frost limit
- lib: added getStarMassDensity
- lib: added inverseExpDistribution
- lib: added normalDistribution
- lib: added calcFrostLimit

v0.00.17 | 2020-03-10
- gen: updated demo 2 text output
- lib: updated habitableZone description

v0.00.16 | 2020-03-09
- lib: added habitableZoneComplete

v0.00.15 | 2020-03-08
- lib: gen color
- lib: added getStarColor
- lib: gen radius, designation
- lib: gen temperatureSequence, stellarType
- lib: added genStarTemperatureSequence
- lib: renamed var to stellarType

v0.00.14 | 2020-03-07
- lib: cdf based star system multiplicity
- lib: new getRndCdfIdx

v0.00.13 | 2020-03-05
- lib: gen spectralType, mass, luminosity, temperature
- lib: cdf based star type
- lib: added getRndStarIdx
- lib: added cumulativeStarTypeProbability

v0.00.11 | 2020-03-03
- updated makefiles
- moved .hpp to lib directory
- updated vector3d.hpp 

v0.00.10 | 2020-03-02
- added showgalaxy.cpp

v0.00.09 | 2020-03-01
- gen: added stars and planets generation framework
- lib: added genPlanets
- lib: added genStars
- lib: fixed universe object scope

v0.00.08 | 2020-02-29
- gen: started demo 2: galaxy data
- gen: deleted temp function arg2int
- vector: added vector3.hpp
- lib: added data models
- lib: deleted SystemSeed struct

v0.00.07 | 2020-02-28
- gen: added planet seed sample
- gen: added star seed sample
- gen: improved demo descriptions
- gen: removed reference to system seed with coordinates
- lib: moved demo code to gen
- lib: added getStarSeeds
- lib: updated documentation
- lib: removed system seed with coordinates

v0.00.06 | 2020-02-27
- gen: added demo
- gen: added stoi converter to uint
- gen: cleaned obsolete demo code
- lib: updated code documentation
- lib: updated demo code to use functions
- lib: added getSystemSeeds
- lib: added getSectorSeed

v0.00.05 | 2020-02-26
- lib: seed functions
- remade make files

v0.00.04 | 2020-02-23
- added pristine seed code
- added galaxy generation code
- made single-header library
- renamed to libprocu-galaxy

v0.00.03 | 2020-02-02
- added modelUniversePlanet.hpp
- added modelUniverseStar.hpp
- added modelUniverseSystem.hpp
- added unitsUniverse.hpp
- moved genstellar cli tool to src

v0.00.02 | 2020-02-01
- project empty tempate remade

v0.00.00 | 2020-02-01
- project initiated

