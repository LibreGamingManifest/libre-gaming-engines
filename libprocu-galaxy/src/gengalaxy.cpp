//===================================
// @file   : gengalaxy.cpp
// @version: 2020-03-22
// @created: 2020-02-01
// @author : pyramid
// @brief  : cli generator frontend for libprocu-galaxy
//===================================


//-----------------------------------
// libraries headers
//-----------------------------------

// standard libraries
#include <iostream>
#include <string>
#include <string.h>
// setw
#include <iomanip>

// include for pristine random seed
#include <random>

// include pcg random library
#include "ext/pcg32.h"

// project includes
#include "lib/libprocu-galaxy.hpp"

// for json serialization
#include "ext/json.hpp"


//-----------------------------------
// using namespaces
//-----------------------------------

using namespace std;
using namespace procu;
using json = nlohmann::json;


//-----------------------------------
// demo 1: creating a galaxy seeds
//-----------------------------------

/**
 * @brief Create a galaxy with the given seed
 * @param seed for the galaxy 
 */
void createGalaxyFromSeed(const uint64_t seedGalaxy) {

    ProcUGalaxy galaxy;
    // create galaxy
    cout << "--- initiating galaxy generator\n";
    cout << "  got input seed: 0x" << hex << setw(16) << setfill('0') << hex << seedGalaxy 
      << dec << " ("<< seedGalaxy << ") ("<< sizeof(seedGalaxy) << " bytes)\n";

    cout << "initializing random generator\n";
    galaxy.setGalaxySeed(seedGalaxy);

    cout << "creating galaxy with cube grid (sector size xyz) 10 x 10 x 10 ly\n";
    cout << "  galaxy extension 100 kly x 100 kly x 100 ly\n";
    cout << "  galaxy center is at (0,0,0)\n";
    cout << "  galaxy extension 10 k x 10 k x 10 sectors\n";
    cout << "  sector sequence coordinates x [-5000:5000] y [-5:5] z [-5000:5000] \n";

    // create sector seeds
    // sector seeds are depending on galaxy seed, and sector x,y,z position
    // so that they are both, reproducible, and unique
    cout << "creating random seed for sample sectors\n";
    //std::cout << "  debug test signed multiplication: " << (int64_t)1e14 * (int64_t)1e14 << "\n";;

    cout << setfill(' ') << "  x,z,y:seed\n";
    for (int x=-1; x<1; ++x) {
      for (int z=-1; z<1; ++z) {
        for (int y=-1; y<1; ++y) {
            uint64_t seedSector = galaxy.getSectorSeed(x,y,z);
            cout << setw(3) << x << setw(3) << y << setw(3) << z << " : "
              << "0x" << setw(16) << setfill('0') << hex << seedSector
              << setw(3) << setfill(' ') << dec << " (" << seedSector
              << ") (" << sizeof(seedSector)<< " bytes)\n";
        }
      }
    }

    // create sector seeds
    cout << "creating seeds for systems in sector\n";
    cout << "  picking sector at coordinates x,y,z: (0,0,4)\n";

    uint64_t uSectorSeed = galaxy.getSectorSeed(0,0,4);
    cout << "  sector seed : "
      << "0x" << setw(16) << setfill('0') << hex << uSectorSeed << dec << " ("
      << uSectorSeed << ") (" << sizeof(uSectorSeed) << " bytes)\n";

    // create system seeds
    cout << "  system coordinates within the sector cube\n";
    cout << "  where (0,0,0) is cube's 'lower-left' coordinate\n";
    cout << "  and a cube has 10 ly extension in each coordinate\n";
    cout << "  generating systems coordinates and system seed\n";
    cout << "  system id : seed\n";

    vector<uint64_t> systemSeeds = galaxy.getSystemSeeds(uSectorSeed);
    for (vector<int>::size_type i = 0; i != systemSeeds.size(); i++) {
      cout << "  " << i << " : ";
      cout << "0x" << setw(16) << setfill('0') << hex << systemSeeds[i] << dec << " ("
        << systemSeeds[i] << ") (" << sizeof(systemSeeds[i]) << " bytes)\n";
    }

    // create star seeds
    cout << "creating seeds for stars\n";
    cout << "  picking star system 0 with seed : "
      << systemSeeds[0] << "\n";
    cout << "  generating star seeds\n";
    cout << "  star id : seed\n";
    vector<uint64_t> starSeeds = galaxy.getStarSeeds(systemSeeds[0], galaxy.MAX_STARS);
    for (vector<int>::size_type i = 0; i != starSeeds.size(); i++) {
      cout << "  " << i << " : ";
      cout << "0x" << setw(16) << setfill('0') << hex << starSeeds[i] << dec << " ("
        << starSeeds[i] << ") (" << sizeof(starSeeds[i]) << " bytes)\n";
    }

    // create planet seeds
    cout << "creating seeds for planets\n";
    cout << "  picking star system 0 with seed :"
      << systemSeeds[0] << "\n";
    cout << "  generating 4 planet seeds\n";
    cout << "  planet id : seed\n";
    vector<uint64_t> planetSeeds = galaxy.getPlanetSeeds(systemSeeds[0],4);
    for (vector<int>::size_type i = 0; i != planetSeeds.size(); i++) {
      cout << "  " << i << " : "
        << "0x" << setw(16) << setfill('0') << hex << planetSeeds[i] << dec << " ("
        << planetSeeds[i] << ") (" << sizeof(planetSeeds[i]) << " bytes)\n";
    }

} // end function


/**
 * @brief create a pristine seed for the galaxy
 */
void createPristineGalaxy() {
  cout << "--- running demo 1: creating seeds example\n";

  ProcUGalaxy galaxy;
  uint64_t uSeed = galaxy.createGalaxySeed();
  cout << "creating pristine seed " << uSeed << "\n";
  
  createGalaxyFromSeed(uSeed);

} // end function


//-----------------------------------
// demo 2: generate galaxy objects demo
//-----------------------------------

void createObjectsFromSeed(const uint64_t seedGalaxy) {
    ProcUGalaxy galaxy;

    // create galaxy
    cout << "  using galaxy seed: 0x" << hex << setw(16) << setfill('0') << hex << seedGalaxy 
      << dec << " ("<< seedGalaxy << ") (" << sizeof(seedGalaxy) << " bytes)\n";
    galaxy.setGalaxySeed(seedGalaxy);

    // pick a sector
    //cout << "using sector xyz: (-3678,4512,3)\n";
    //uint64_t seedSector = galaxy.getSectorSeed(-3678,4512,3);
    cout << "--- using example sector xyz: (0,0,4)\n";
    uint64_t seedSector = galaxy.getSectorSeed(0,0,4);
    cout << "  using sector seed: 0x" << hex << setw(16) << setfill('0') << hex << seedSector 
      << dec << " ("<< seedSector << ") (" << sizeof(seedSector) << " bytes)\n";

    // pick a system
    cout << "--- using example system nr 0 with system : seed\n";
    // create system seeds
    vector<uint64_t> systemSeeds = galaxy.getSystemSeeds(seedSector);
    // pick the n-th system
    int i = 0;
    cout << "  " << i << " : ";
    cout << "0x" << setw(16) << setfill('0') << hex << systemSeeds[i] << dec << " ("
      << systemSeeds[i] << ") (" << sizeof(systemSeeds[i]) << " bytes)\n";

    // generate system data (coordinates, objects)
    cout << "--- generating system data\n";
    /*
    UniverseSystem system = galaxy.genSystem(systemSeeds[i]);
    cout << "generated system data:\n";
    cout << "  position xyz : " << setfill(' ') << setprecision(4)
      << system.pos.x << " " << system.pos.y << " " << system.pos.z << "\n";
      */
    // test scope since copy should have been made into galaxy.systems
    galaxy.genSystem(systemSeeds[i]);
    UniverseSystem system = galaxy.systems[systemSeeds[i]];
    std::vector<double> systemPosition = system.position;
    cout << "  number of stars in system: " << system.multiplicity << "\n";
    //cout << "  position in sector xyz : " << setfill(' ') << setprecision(4)
    //  << system.position.x << " " << system.position.y << " " << system.position.z << "\n";
    cout << "  position in sector xyz : " << setfill(' ') << setprecision(4)
      << systemPosition[0] << " " << systemPosition[1] << " " << systemPosition[2] << "\n";

    // generate system stars
    cout << "------ generating system stars\n";
    galaxy.genStars(system.seed);
    // iterate and print stars
    // read system again since it was changed within the class
    // and not referenced from this client
    //system = galaxy.systems[system.seed];
    // Create a map iterator and point to beginning of map (not really required)
    //std::map<uint64_t, UniverseStar>::iterator it = system.stars.begin();
    // Iterate over the map using c++11 range based for loop
    //for (std::pair<uint64_t, UniverseStar> element : system.stars) {
    for (std::pair<uint64_t, UniverseStar> element : galaxy.systems[system.seed].stars) {
      // Accessing KEY from element
      uint64_t seed = element.first;
      // Accessing VALUE from element.
      UniverseStar star = element.second;
      cout << setfill(' ') << setprecision(6);
      cout << "  star seed : "
        << "0x" << setw(16) << setfill('0') << hex << seed << dec << " ("
        << seed << ")\n";

      cout << "    star type index = " << star.typeIndex << "\n";
      cout << "    star type: " << star.stellarType << "\n";
      cout << "    star designation: " << star.designation << "\n";
      cout << "    spectral class: " << star.spectralClass << "\n";
      cout << "    star luminosity [Lsol] = " << star.luminosity << "\n";
      cout << "    star temperature [K] = " << star.temperature << "\n";
      cout << "    star mass [Msol] = " << star.mass << "\n";
      cout << "    star radius [Rsol] = " << star.radius << "\n";
      //cout << "    star rotation [s] = " << star.axialRotation << "\n";
      cout << "    star rotation [d] = " << star.axialRotation/86400 << "\n";
      cout << "    star color RGB 24bit: " << (int)star.color[0] << " "
        << (int)star.color[1] << " " << (int)star.color[2] << " " << "\n";
      cout << dec << setprecision(4) << "    hzDistAu[1] = " <<
        star.hzDistAu[1] <<  " (" << HZDescription[1] << ")\n";
      cout << dec << setprecision(4) << "    hzDistAu[5] = " <<
        star.hzDistAu[5] << " (" << HZDescription[5] << ")\n";
      cout << dec << setprecision(4) << "    150K frost limit [au] = "
        << star.frostLimitAu << "\n";
      cout << dec << setprecision(4) << "    habitable planets probability = "
        << getHabitablePlanetsProbability(star) << "\n";
      cout << "    generated " << star.planetsCount << " planet"
        << (star.planetsCount>1?"s":"") << "\n";
    }

    // generate system star planets
    cout << "--- generating system planets for each star\n";
    for (std::pair<uint64_t, UniverseStar> starRec : galaxy.systems[system.seed].stars) {
      // Accessing KEY from element
      uint64_t starSeed = starRec.first;
      // Accessing VALUE from element.
      UniverseStar star = starRec.second;
      cout << "------ generating planets for star :: " << star.seed <<"\n";

      galaxy.genPlanets(system.seed, star.seed);
      int i = 0;
      for (std::pair<uint64_t, UniversePlanet> planetRec : galaxy.systems[system.seed].stars[starSeed].planets) {
        // Accessing KEY from element
        //uint64_t planetSeed = planetRec.first;
        // Accessing VALUE from element.
        UniversePlanet planet = planetRec.second;
        cout << "  planet " << i << ": "
          << "0x" << setw(16) << setfill('0') << hex << planet.seed
          << dec << " (" << planet.seed << ")\n";
        cout << "    star distance [au] = " << planet.starDistance << "\n";
        cout << "    in habitable zone : " << (planet.isInHz==true? "true":"false") << "\n";
        cout << "    planet type : " << planetType[planet.typeIndex] << "\n";
        cout << "    planet type index = " << planet.typeIndex << "\n";
        cout << "    planet radius [km] = " << planet.radius << "\n";
        cout << "    planet day [d] = " << planet.day/86400 << "\n";
        cout << "    planet year [a] = " << planet.year/yearEarth << "\n";
        cout << "    mass [Mearth] = " << planet.mass/Mearth << "\n";
        cout << "    gravitational param mu [km^3 s^-2] = " << planet.mu << "\n";
        cout << "    median temperature [K] = " << planet.temperature << "\n";
        cout << "    equator temperature [K] = " << planet.equatorTemperature << "\n";
        cout << "    pole temperature [K] = " << planet.poleTemperature << "\n";
        // check if planet has atmosphere
        // radius>0? and exists() is the same thing
        cout << "    planet has " << (planet.atmosphere.radius>0? "" : "no " ) << "atmosphere\n";
        if (planet.atmosphere.exists()) {
          cout << "      atmosphere radius = " << planet.atmosphere.radius << "\n";
          if ((planet.atmosphere.radius-planet.radius)==0) {
            cout << "      (gas giant radius = atmosphere radius)\n";
          } else {
            cout << "      atmosphere thickness = " << (planet.atmosphere.radius-planet.radius) << "\n";
          }
          cout << "      atmosphere composition : "
            << concatCompositionElements(planet.atmosphere.composition) << "\n";
          cout << "      atmosphere habitability : "
            << atmosphereHabitability(planet.atmosphere.composition) << "\n";
        } // end: planet has atmosphere
        cout << "      planet habitability = " << setprecision(4)
          << getPlanetHabitability(planet) << "\n";
        ++i;
      }
    }

    cout << "--- stars habitability summary\n";
    for (std::pair<uint64_t, UniverseStar> element : galaxy.systems[system.seed].stars) {
      // Accessing KEY from element
      //uint64_t seed = element.first;
      // Accessing VALUE from element.
      UniverseStar star = element.second;
      cout << "  star : "
        << "0x" << setw(16) << setfill('0') << hex << star.seed << dec << " ("
        << star.seed << ")\n";
      bool hasPlanetsInHZ = hasPlanetsInHz(star);
      cout << "    has planets in HZ : " << (hasPlanetsInHZ?"yes":"no") << "\n";
    }

} // createObjectsFromSeed aka demo 2


// create a pristine objects for the galaxy
void createPristineObjects() {
  cout << "--- running demo 2: creating objects example\n";

  ProcUGalaxy galaxy;
  uint64_t uSeed = galaxy.createGalaxySeed();
  cout << "--- creating pristine galaxy seed " << uSeed << "\n";
  
  createObjectsFromSeed(uSeed);

}


//-----------------------------------
// demo 3: create and save galaxy seed
//-----------------------------------

void  createAndSave() {
  cout << "--- running demo 3: create and save galaxy seed\n";

  ProcUGalaxy galaxy;
  galaxy.createGalaxySeed();
  cout << "--- creating pristine galaxy seed " << "\n";
  cout << "  0x" << hex << setw(16) << setfill('0') << hex << galaxy.galaxySeed 
    << dec << " ("<< galaxy.galaxySeed << ") ("<< sizeof(galaxy.galaxySeed) << " bytes)\n";

  cout << "--- saving galaxy seed to galaxy.json" << "\n";
  saveGalaxy(galaxy);
  cout << "saved.\n";

  cout << "--- setting a new seed " << "\n";
  galaxy.createGalaxySeed();
  cout << "  0x" << hex << setw(16) << setfill('0') << hex << galaxy.galaxySeed 
    << dec << " ("<< galaxy.galaxySeed << ") ("<< sizeof(galaxy.galaxySeed) << " bytes)\n";
  
  cout << "--- loading galaxy seed from galaxy.json" << "\n";
  loadGalaxy(galaxy);
  cout << "loaded.""\n";
  cout << "  0x" << hex << setw(16) << setfill('0') << hex << galaxy.galaxySeed 
    << dec << " ("<< galaxy.galaxySeed << ") ("<< sizeof(galaxy.galaxySeed) << " bytes)\n";

} // end demo 3


//-----------------------------------
// demo 4: save galaxy objects
//-----------------------------------

void saveGalaxyObjects() {
    cout << "--- running demo 4: create and save galaxy objects\n";
    cout << "  generating galaxy\n";
    ProcUGalaxy galaxy;
    galaxy.createGalaxySeed();

    cout << "  generating sector (0,0,4)\n";
    uint64_t seedSector = galaxy.getSectorSeed(0,0,4);
    UniverseSector sector = UniverseSector();
    sector.seed = seedSector;

    cout << "  generating systems\n";
    vector<uint64_t> systemSeeds = galaxy.getSystemSeeds(seedSector);
    // we only need one seed for this demo
    sector.systemSeeds = {systemSeeds[0]};
    sector.position = {0,0,4};
    galaxy.sectors[seedSector] = sector;

    cout << "  generating system data\n";
    // pick the n-th system
    int n = 0;
    galaxy.genSystem(systemSeeds[n]);

    cout << "  generating star data\n";
    galaxy.genStars(galaxy.systems[systemSeeds[n]].seed);

    // generate systems, stars, planets
    cout << "  generating planet data\n";
    UniverseSystem &system = galaxy.systems[systemSeeds[n]];
    system.sector = seedSector; // set parent sector
    for (auto& [starSeed, star] : system.stars) {
      galaxy.genPlanets(system.seed, star.seed);
    }

    // serialize data
    cout << "  serializing galaxy data\n";
    json data;
    data["galaxy"]["seed"] = galaxy.galaxySeed;
    cout << "  serializing sector data\n";
    data["sectors"] = galaxy.sectors;
    cout << "  serializing system data\n";
    json jSystem = system;
    data["systems"]["system"] = jSystem;

    // save prettified JSON to file
    cout << "  saving galaxy data to galaxy.json\n";
    std::ofstream outFile("galaxy.json");
    outFile << std::setw(2) << data << std::endl;

} // end demo 4


//-----------------------------------
// demo 5: generate complete galaxy objects
//-----------------------------------

void generateCompleteGalaxy(uint64_t seedGalaxy=0) {
  cout << "--- running demo 5: generating galaxy\n";

  ProcUGalaxy galaxy;
  if (seedGalaxy==0) {
    cout << "  creating pristine galaxy seed\n";
    galaxy.createGalaxySeed(); // pristine seed
  } else {
    cout << "  using parameter galaxy seed\n";
    galaxy.setGalaxySeed(seedGalaxy); // reuse previous seed
  }
  cout << "  0x" << hex << setw(16) << setfill('0') << hex << galaxy.galaxySeed 
    << dec << " ("<< galaxy.galaxySeed << ") ("<< sizeof(galaxy.galaxySeed) << " bytes)\n";

  // define galaxy size
  galaxy.GALAXY_SIZE_LY = {1000,10,1000};

  cout << "  galaxy extension x,y,z in [ly] = " << galaxy.GALAXY_SIZE_LY[0] << " "
    << galaxy.GALAXY_SIZE_LY[1] << " " << galaxy.GALAXY_SIZE_LY[2] << "\n";
  int sectorExtension = galaxy.GALAXY_SIZE_LY[0]/galaxy.SECTOR_SIZE_LY/2;
  cout << "  galaxy sectors position min/max = " << -sectorExtension << " / "
    << sectorExtension << "\n";

  cout << "  generating sectors\n";
  galaxy.genSectors();

  cout << "  generating systems\n";
  for (auto& [seedSector, sector] : galaxy.sectors) {
    galaxy.genSystems(seedSector);
    for (auto& systemSeed : sector.systemSeeds) {
      galaxy.genSystem(systemSeed);
    }
  }
  cout << "  unique system seeds = " << galaxy.systems.size() << "\n";

  cout << "  generating stars and planets\n";
  int countTotalStars = 0;
  int countTotalPlanets = 0;
  int countHabitablePlanets = 0;
  for (auto& [systemSeed, system] : galaxy.systems) {
      galaxy.genStars(systemSeed);
      for (auto& [starSeed, star] : system.stars) {
        ++countTotalStars;
        galaxy.genPlanets(system.seed, star.seed);
        for (auto& [planetSeed, planet] : star.planets) {
          ++countTotalPlanets;
          if (getPlanetHabitability(planet)>0) {
            ++countHabitablePlanets;
          }
        } // planets
      }
  }
  cout << "    total stars = " << countTotalStars << "\n";
  cout << "    total planets = " << countTotalPlanets << "\n";
  cout << "    probably habitable planets = " << countHabitablePlanets << "\n";

}


//===================================
// main program
//===================================

int main(int argc, char **argv) {
  uint16_t iDemo = 1; // demo number to run without parameter
  uint64_t uSeed = 0; // seed number to use

  cout << "--- gengalaxy | v0.00.28 | 2020-03-22 ---\n";

  //---------------------------------
  // parse input parameters
  //---------------------------------

  vector<string> args(argv, argv+argc);
  for (size_t i = 1; i < args.size(); ++i) {
    if (args[i] == "-h" or args[i] == "--help") {
      cout << "--- usage:\n";
      cout << "  -h --help         : show this help\n";
      cout << "  -s --seed uint    : generate with defined seed\n";
      cout << "  -d --demo uint    : run defined demo\n";
      cout << "          --demo 1  : (default) create seeds example\n";
      cout << "          --demo 2  : create objects example\n";
      cout << "          --demo 3  : save galaxy seed in json format\n";
      cout << "          --demo 4  : save objects in json format\n";
      cout << "          --demo 5  : generate whole galaxy and count objects\n";
      return 0;
    } else
    if (args[i] == "-s" or args[i] == "--seed") {
      uSeed = stoll(args[i+1]);
      cout << "param seed = 0x" << hex << setw(16) << setfill('0') << uSeed
        << dec << " (" << uSeed << ") ("<< sizeof(uSeed) << " bytes)\n";
    }
    if (args[i] == "-d" or args[i] == "--demo") {
      iDemo = (uint16_t)stoi(args[i+1]);
      cout << "param demo = 0x" << hex << setw(4) << setfill('0') << iDemo
        << dec << " (" << iDemo << ") ("<< sizeof(iDemo) << " bytes)\n";
    }
    if (args[i] == "-f" or args[i] == "--file") {
      string filename = args[i+1];
      cout << "filename: " + filename +  "\n";
    }
  }


  //---------------------------------
  // run tests
  //---------------------------------

  if (iDemo==1) {
    if (uSeed>0) {
      createGalaxyFromSeed(uSeed);
    } else {
      createPristineGalaxy();
    }
  } // demo 1

  if (iDemo==2) {
    if (uSeed>0) {
      createObjectsFromSeed(uSeed);
    } else {
      createPristineObjects();
    }
  } // demo 2

  if (iDemo==3) {
    if (uSeed>0) {
      cout << "this demo always creates a pristine seed\n";
    }
    createAndSave();
  } // demo 3

  if (iDemo==4) {
    if (uSeed>0) {
      cout << "this demo always creates a pristine seed\n";
    }
    saveGalaxyObjects();
  } // demo 4

  if (iDemo==5) {
    if (uSeed>0) {
      generateCompleteGalaxy(uSeed);
    } else {
      generateCompleteGalaxy();
    }
  } // demo 4

  return 0;
} // end main
