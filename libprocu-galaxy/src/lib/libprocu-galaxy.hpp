//===================================
// @file   : libprocu-galaxy.hpp
// @version: 2020-05-21
// @created: 2020-02-01
// @author : pyramid
// @brief  : library for procedural galaxy generation
//===================================



//-----------------------------------
// Documentation
//-----------------------------------

/**
 * @brief ProcUGalaxy Documentation\n
 * A galaxy procedural generator.
 * 
 * **Introduction**
 * ProcUGalaxy uses the right-handed (OpenGL) coordinate system
 * with +X to the right, +Y up, and +Z into the screen.
 * For seed generation, the galaxy will be divided into sectors.
 * The galaxy is centered at origin xyz(0,0,0).
 * Sectors will have an average number of star systems generated
 * sequentially.
 * Star system will have one or several stars generated.
 * Each star will have planets generated.
 * Planets will have an atmosphere or not.
 * Gas giants always have an atmosphere with radius equal
 * to planet radius.
 * An atmosphere will have gaseous element components and
 * can sustain oxygen breathers or not.
 * Further expansions planned:
 * - generating planet orbital object placeholders (stations)
 * - generating planetary rings
 * - generating asteroid belts
 * - generating meteors and comets
 * 
 * **Configuring**
 * - GALAXY_TYPE : galaxy type
 *     default: SPIRAL
 * - GALAXY_SIZE_LY : galaxy size xyz vector in light years (ly)
 *     default: 100 000 ly across, 100 ly heights
 * - SECTOR_SIZE_LY : sector edge length in light years (ly)
 *     default: 10 ly across in all directions x,y,z
 * - MAX_SYSTEMS : maximum number of systems in a sector
 *    default: 10 with uniform distribution
 * 
 * **Generating Galaxy Pipeline**
 * - first create a galaxy seed (or reuse the one you have)
 * - create sector seed or seeds
 * - create system seed or seeds
 * - create system object seeds (stars, planets, ...)
 * - generate system data for each system
 * In general there is a separation between generating seeds
 * (in case lengthy data is not (yet) required), and
 * generating data content for any object.
 * 
 * **Creating Seeds (Functions)**
 * - createGalaxySeed
 * - getSectorSeed
 * - getSystemSeeds
 * - getStarSeeds
 * - getPlanetSeeds
 * 
 * **Generating Universe Data**
 * Galaxy data is generated in the ProcUUniverse class
 * while the Universe* structs ony hold the data, and
 * have no further functions of their own except the
 * constructor and destructor.
 * - genSystem
 * - genStars, genStar
 * - genPlanets, genPlanet
 * 
 * The **object hierarchy** is as follows:
 * - galaxy
 * -- sector
 * --- system
 * ---- star
 * ---- planet
 * ----- atmosphere
 * ----- moons
 * ----- orbitals
 * 
 * Generated data hierarchy is stored as maps of
 * - sectors in galaxy
 * - systems in galaxy
 * - stars in system
 * - planets in system
 * - atmosphere in planet
 * - element composition in atmosphere
 * 
 * There is no link between the sector and system data.
 * However for fast retrieval, sectors will hold the system
 * seed references.
 * Atmosphere and composition use planet seed to be generated.
 * 
 * **Serializing Objects**
 * TODO: description of object serialization and deserialization.
 * After careful consideration, we have selected JSON as
 * the choice serialization format as it is:
 * - Editable with a text editor
 * - Human readable
 * - Machine readable
 * - Compact even for large data sets
 * - Libraries available in popular programming languages
 * 
 * Serialization functions
 * - saveGalaxy
 * - loadGalaxy
 * 
 * **References**
 * For the creation of hierarchically dependent seeds
 * (lower hierarchy object seed is derived using a set expression
 * from the higher-level object), 
 * we adapt and extent a method introduced by Olof Elfwering in
 * "Procedurally Generating an Artificial Galaxy". 2016.
 *
 * **Code Style**
 * As we often develop on a mobile device and screen estate
 * is little, there are many indents with 2 spaces only.
 * Formatting (largely) follows the clean Stroustrup style.
 * 
 * @author pyramid
**/


//-----------------------------------
// headers
//-----------------------------------

// header include guards
#ifdef _MSC_VER
  #pragma once
#endif

#ifndef LIBPROCU_GALAXY_H
#define LIBPROCU_GALAXY_H


//-----------------------------------
// includes: standard libraries
//-----------------------------------

// terminal output stream
#include <iostream>
// standard string
#include <string>
// display formatting (setw)
#include <iomanip>
// for holding seeds, colors
#include <vector>
// for ordered collections
#include <list>
// for holding unordered hierarchical objects
#include <map>
// include for pristine random seed
#include <random>
// performance measurement
// time, sleep, milliseconds
#include <chrono>
// file in/output
#include <fstream>


//-----------------------------------
// includes: external libraries
//-----------------------------------

// include pcg random library
#include "pcg32.h"
// include json library
// https://github.com/nlohmann/json
#include "json.hpp"

// alternative vector library
//#include "vector3d.hpp"
// alternative star, planet color library
//#include "color.hpp"
// unused
//#ifdef __ANDROID__
//  #include "android.hpp"
//#endif


//-----------------------------------
// using namespace
//-----------------------------------

using namespace std;
using json = nlohmann::json;


//-----------------------------------
// namespace
//-----------------------------------

namespace procu {


//----------------------------------------------
// types
//----------------------------------------------

/**
 * @brief Define a platform independent byte.
 * Define uint8_t as byte to allow for
 * easy porting to c++17 std::byte
 * As far as tested, uint8_t is supported
 * on linux and android.
 * The main application is in RGBA color components.
 * Can be also used as a forward declaration.
 * @author pyramid
 */
typedef uint8_t byte;


//-----------------------------------
// lib configuration: debug levels
//-----------------------------------

/**
 * @brief Severity of the log message
 */
/*// currently unused
enum LOGLEVEL {
    DEBUG = 0,
    INFO = 1,
    ERROR = 2,
};*/


//-----------------------------------
// Constants of the Cosmic Universe
//-----------------------------------

// gravitation constant in [m^3 kg^-1 s^-2]
float G =  6.67384e-11f;
// Earth gravity in [m s^-2]
float gEarth = 9.81f;

// distance conversion factors
float au2km = 1.49597871e8f;    // au to km
float km2au = 6.68458712e-9f;   // km to au
float pc2km = 3.08567758e13f;   // parsec to km
float km2pc = 3.24077929e-14f;  // km to parsec
float c2kmps = 299792.458f;     // speed of light c to km per second
float m2au = 6.68458712e-12f;   // meter to au

// solar system reference constants
float Rsol = 696342.0f;         // Sun radius in km
float Msol = 1.989e30f;         // Sun mass in kg
float Rearth = 6371.0f;         // Earth radius in km
float Mearth = 5.972e24f;       // Earth mass in kg

// Sun luminosity constants in W
float Lsol = 3.84e26f;          // Sun luminosity in W
//blackbody constant 5.67E-008 [W*m^-2*K^-4]
float Lsigma = 5.67e-8f;

// time constants
// Earth year in [s] = 365.25636 d * 24 h * 60 min * 60 s
float yearEarth = 31558149.5f;  // Earth year in seconds

// pressure constants
float bar2Pa = 1e5f;            // 1 bar = 100 000 Pascal


//-----------------------------------
// libProcU procu::MathUtil
//-----------------------------------

/**
 * @brief Returns normal distribution value at point x, with
 * @param x - point to get distribution at
 * @param mu - median point, expected: frostLimit/2
 * @param sigma - standard deviation, expected: frostLimit/16
 * @return
 */
float normalDistribution(float x, float mu, float sigma) {
    return (1 / ( sigma * sqrt(2 * M_PI) )) * exp( - pow(x-mu,2) /  (2 * pow(sigma,2)) );
}

/**
  * @brief Returns inverse exponential distribution value at point x, with
  * @param x - point to get distribution at
  * @param skew - skewness of the function, expected: 0.5
  * @return
  */
float inverseExpDistribution(float x, float skew) {
    return exp(-pow(x, skew));
}


//---------------------------------
// random distribution functions
//---------------------------------

/**
  * Random index from non-linear
  * cumulative distribution function
  */
int getRndCdfIdx(float rn, std::list<float> cdf) {
  int idx = 0;
  for(std::list<float>::iterator iter=cdf.begin(); iter != cdf.end(); ++iter ) {
    float ub = *iter; // upper bound
    idx = std::distance(cdf.begin(), iter);
    //cout << idx << " ";
    if (rn<=ub) { break; }
  }
  //cout << "\n";
  //cout << "star type: " << idx << "\n";
  return idx;
}


//-----------------------------------
// Model of Universe Atmosphere
//-----------------------------------

//-----------------------------------
// atmosphere enumerators
//-----------------------------------

// more frequent elements on top
std::string componentOrder[10] = {
  "CO2", "H2", "N2", "O2", "He",
  "Ar", "CH4", "Ne", "Kr", "Xe"
};

// atmosphere element composition probability
std::map<std::string, float> elementProb = {
    {"CO2", 0.965f},  // run 0
    {"H2",  0.963f},
    {"N2",  0.780f},  // run 2
    {"O2",  0.210f},
    {"He",  0.102f},  // run 4
    {"Ar",  0.016f},
    {"CH4", 0.015f},  // run 6
    {"Ne",  0.0001f},
    {"Kr",  0.0001f}, // run 8
    {"Xe",  0.0001f}
};

// atmosphere gas pressure
std::map<std::string, float> ppMaxGas = {
    {"He", 2934.0f},
    {"Ne", 66.0f},
    {"H2", 16.5f},
    {"N2", 5.94f},
    {"O2", 1.6f},
    {"Ar", 1.12f},
    {"Kr", 0.12f},
    {"CO2", 0.015f},
    {"Xe", 0.009f},
    {"CH4", 0.001f}
};

// atmosphere element composition toxicity
std::map<std::string, float> toxicity = {
    {"He", 0.045f},
    {"Ne", 0.3f},
    {"H2", 0.6f},
    {"N2", 1.0f},
    {"O2", 1.7f},
    {"Ar", 2.3f},
    {"Kr", 7.1f},
    {"CO2", 20.0f},
    {"Xe", 25.6f},
    {"CH4", 20.0f}
};


//-----------------------------------
// atmosphere data structure
//-----------------------------------

/**
 * @brief Universe Atmosphere data structure.
 * Initialized to radius = 0
 * Check if radius has been set
 * (when atmosphere actually) exists for
 * the parent planet using the exists() function.
 */
struct UniverseAtmosphere {

    // radius in [km]
    float radius = 0;
    // pressure at surface in [atm]
    float pressure = 0;
    // element composition
    std::map<std::string, float> composition = {};


    //---------------------------
    // Constructor
    //---------------------------

    UniverseAtmosphere() { }

    // check if atmosphere exists
    bool exists() {
      return (radius>0);
    }

}; // end struct UniverseAtmosphere


//---------------------------
// UniverseAtmosphere functions
//---------------------------

/**
 * Calculate atmosphere habitability percentage based on
 * breathable component percentage, for oxygen breathers,
 * atmospheric pressure, and maximum component concentration.
 * TODO: adapt to non-carbon based life forms and
 * non-oxygen breathers as required.
 * @param composition - object holding the elements
 * @param pressure - atmospheric pressure in [bar]
 * @return probabAtmo - probability of habiltability
 */
float atmosphereHabitability(std::map<std::string, float> &composition, float pressure=1.0f) {
    float probabAtmo = 1.0f;

    int i = 0;
    for (std::pair<std::string, float> element : composition) {
      // Accessing KEY from record
      std::string gas = element.first;
      // Accessing VALUE from record.
      float val = element.second;
      //cout << i << ": " << gas << " :: " << val << " | ";
      float ppGas = (float)val * pressure; 
      if (ppGas>ppMaxGas[gas]) {
          probabAtmo = 0.0f;
      }
      // the atmosphere is not breathable
      if ((gas=="O2") & (ppGas<0.16f)) {
          probabAtmo = 0.0f;
      }
      // atmosphere has no oxygen
      if (composition.count("O2")==0) {
          probabAtmo = 0.0f;
      }
      ++i;
    }

    return probabAtmo;    
}

/**
 * Concatenates composition element names up to a given string length and using
 * the defined separator.
 * @param composition - object holding the elements
 * @param separator - separator string, e.g. "," or " "
 * @param bLong - return long or short Concatenated elements
 *   true will print elemnts and percentage ("H2:0.9553 N2:0.0470")
 *   false will print elements only ("H2 He O2")
 * @return
 */
std::string concatCompositionElements(std::map<std::string, float> &composition, std::string separator=" ", bool bLong=true) {
    std::string result = "";
    //uint maxLength = 10;

    //cout << "DEBUG: ";
    int i = 0;
    for (std::pair<std::string, float> element : composition) {
      // Accessing KEY from record
      std::string key = element.first;
      // Accessing VALUE from record.
      float val = element.second;
      //cout << i << ": " << key << " :: " << val << " | ";
      if (bLong) { // long
        result += key + ":" + to_string(val) + separator;
      } else { // short
        result += key + separator;
      }
      // we could implement maxLength checking;
      //if (result.length()>=maxLength) { break; }
      ++i;
    }
    //cout << "\n";

    return result;
}

/**
  * Creates atmosphere composition from typical elements.
  * @param composition - atmo map object storing the composition
  * @param rnd - random generator
  */
void createComposition(std::map<std::string, float> &composition, pcg32 rnd) {
    //cout << "DEBUG: --- createComposition\n";
    // initial composition total volume percentage
    float part = 0.0f;
    // which run this is
    int run = 0;
    // which element range to select. depends on the run (see below)
    int minCnt = 0;
    int maxCnt = elementProb.size() - 1;

    while (part < 1.0f) {
        //cout << "DEBUG: next part ... run = " << run << "\n";
        // which element range to select. depends on the run
        // one of the more frequent elements first
        // which of known composition elements to choose
        if (run==0) {
            minCnt = 0;
            maxCnt = 2;
        }
        if (run==1) {
            minCnt = 2;
            maxCnt = 4;
        }
        if (run>1) {
            minCnt = 4;
            maxCnt = elementProb.size() - 1;
        }
        //cout << "DEBUG: minCnt = " << minCnt << "\n";
        //cout << "DEBUG: maxCnt = " << maxCnt << "\n";

        // which element?
        int which = minCnt + (int)rnd.nextUInt(maxCnt-minCnt);
        //cout << "DEBUG: which element index = " << which << "\n";

        //std::string comp = keys[which];
        std::string comp = componentOrder[which];
        //cout << "DEBUG: selected component = " << comp << "\n";

        // get maximum volume percentage
        float maxPart = elementProb[comp];
        // get random volume percentage
        float variationPart = maxPart*0.6f + rnd.nextFloat() * maxPart*0.4f;
        // limit to the remaining volume percentage
        float partToAdd = min(variationPart, 1.0f-part);
        // add the new composition
        part += partToAdd;

        //cout << "DEBUG: element exists count = " << composition.count(comp) << "\n";
        if (composition.count(comp)) { //new
          composition[comp] = partToAdd;
        } else { // add
          composition[comp] += partToAdd;
        }

        //cout << "DEBUG: maxPart = " << maxPart << "\n";
        //cout << "DEBUG: variationPart = " << variationPart << "\n";
        //cout << "DEBUG: partToAdd = " << partToAdd << "\n";
        //cout << "DEBUG: total element part = " << composition[comp] << "\n";
        //cout << "DEBUG: total atmo = " << part << "\n";

        // next component run
        ++run;
    } // loop while not reached 100%

}


//-----------------------------------
// Model of Universe Planet
//-----------------------------------

//-----------------------------------
// planet enumerators
//-----------------------------------

/**
  * Periodic table of planets
  * arranged in a 6x3 matrix
  * by typeIndex in range [0..17]
  */

std::string planetType[18] = {
    "Hot Mercurian", "Hot Subterran", "Hot Terran", "Hot Superterran", "Hot Neptunian", "Hot Jovian", // 0-5
    "Warm Mercurian", "Warm Subterran", "Warm Terran", "Warm Superterran", "Warm Neptunian", "Warm Jovian", //6-11
    "Cold Mercurian", "Cold Subterran", "Cold Terran", "Cold Superterran", "Cold Neptunian", "Cold Jovian" //12-17
};

std::string planetFamily[18] = {
    "Mercurian", "Subterran", "Terran", "Superterran", "Neptunian", "Jovian",
    "Mercurian", "Subterran", "Terran", "Superterran", "Neptunian", "Jovian", 
    "Mercurian", "Subterran", "Terran", "Superterran", "Neptunian", "Jovian"
};

std::string planetClass[18] = {
    "Terrestial", "Terrestial", "Terrestial", "Terrestial", "Gas Giant", "Gas Giant",
    "Terrestial", "Terrestial", "Terrestial", "Terrestial", "Gas Giant", "Gas Giant",
    "Terrestial", "Terrestial", "Terrestial", "Terrestial", "Gas Giant", "Gas Giant"
};

std::string temperatureZone[18] = {
    "Hot Zone", "Hot Zone", "Hot Zone", "Hot Zone", "Hot Zone", "Hot Zone",
    "Warm Zone", "Warm Zone", "Warm Zone", "Warm Zone", "Warm Zone", "Warm Zone",
    "Cold Zone", "Cold Zone", "Cold Zone", "Cold Zone", "Cold Zone", "Cold Zone"
};

float Mearth_min[18] = {
    0.0f, 0.1f, 0.5f, 2.0f, 10.0f, 50.0f,
    0.0f, 0.1f, 0.5f, 2.0f, 10.0f, 50.0f,
    0.0f, 0.1f, 0.5f, 2.0f, 10.0f, 50.0f
};

float Mearth_max[18] = {
    0.1f, 0.5f, 2.0f, 10.0f, 50.0f, 1e3f, // in reality max jovian = 1e30f
    0.1f, 0.5f, 2.0f, 10.0f, 50.0f, 1e3f,
    0.1f, 0.5f, 2.0f, 10.0f, 50.0f, 1e3f
};

float Rearth_min[18] = {
    0.03f, 0.4f, 0.8f, 1.25f, 2.6f, 6.0f,
    0.03f, 0.4f, 0.8f, 1.25f, 2.6f, 6.0f,
    0.03f, 0.4f, 0.8f, 1.25f, 2.6f, 6.0f
};

float Rearth_max[18] = {
    0.4f, 0.8f, 1.25f, 2.6f, 6.0f, 1e3f, // in reality max jovian = 1e30f
    0.4f, 0.8f, 1.25f, 2.6f, 6.0f, 1e3f,
    0.4f, 0.8f, 1.25f, 2.6f, 6.0f, 1e3f
};


/**
 * gas giants always have a thick gas atmosphere
 */
float atmosphereProbabilityMax[18] = {
        0.0f, 0.001f, 0.001f, 0.001f, 1.0f, 1.0f,
        0.0f, 0.02f, 0.05f, 0.01f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
};

float atmospherePressureMin[18] = {
        0.0f, 0.1f, 0.5f, 0.5f, 10.0f, 1e2f,
        0.0f, 0.1f, 0.5f, 0.5f, 10.0f, 1e2f,
        0.0f, 0.1f, 0.5f, 0.5f, 10.0f, 1e2f
};

float atmospherePressureMax[18] = {
        0.001f, 0.5f, 2.0f, 3.0f, 1e3f, 2e3f,
        0.001f, 0.5f, 2.0f, 3.0f, 1e3f, 2e3f,
        0.001f, 0.5f, 2.0f, 3.0f, 1e3f, 2e3f
};

float planetHabitabilityPeriodicFactor[18] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};


//-----------------------------------
// planet data structure
//-----------------------------------

/**
 * @brief ProcU model for a planet.
 * It is pseudo-scientific and intended mainly for game
 * content generation.
 */
struct UniversePlanet {

  //---------------------------------
  // Variables
  //---------------------------------

  // the seed is the planet global unique identifier (GUID)
  uint64_t seed;
  // planet name
  std::string name = "";
  // planet position vector (length = distance) in [au]
  std::vector<double> position = {0,0,0};
  // distance from orbited star in [au]
  float starDistance = 0;
  // if planet is in habitable zone
  bool isInHz = false;
  // mass in [kg]
  float mass = 0;
  // Standard gravitational parameter mu (G*M) in [km^3 s^-2]
  float mu = 0;
  // median temperature in [K]
  float temperature = 0;
  // calculated from median planet temperature
  float equatorTemperature;
  float poleTemperature;
  // planet type index from periodic table of planets
  int typeIndex = -1;
  // radius in [km]
  float radius = 1000;
  // planet day (duration of one axis rotation in [s]
  float day = 0;
  // planet year (duration of one parent star orbital period in [s]
  float year = 0;
  // habitability probabilities in the range [0.0 ... 1.0]
  float probTemp = 0.0; // probability from temperature 
  float probGrav = 0.0; // probability from gravity
  float probAtmo = 0.0; // probability from atmosphere
  // atmosphere object
  // check if planet has atmosphere with
  // atmosphere.radius>0?
  UniverseAtmosphere atmosphere;


  // reserved for future use
  // initial orbital rotation in [rad] on 2000-01-01
  float rotation = 0;
  // base rgb color in the range [0..255]
  std::vector<byte> baseColor = {0,0,0};
  // base rgba color in the range [0.0 .. 1.0]
  //procu::Color baseColor = procu::Color(0.0f,0.0f,0.0f,0.0f);
  //std::vector<float> baseColor = {0.0f,0.0f,0.0f,0.0f};
  // percentage of water surface
  //float waterPercent;
  // water level in height value [0..255]
  // calculated from water percentage
  //int waterLevel;


  //---------------------------------
  // Constructor
  //---------------------------------

  /**
   * @brief constructor without parameters
   * @code  constructor: UniverseSystem (); @endcode
   */
  UniversePlanet() {};

}; // end struct


//---------------------------
// UniversePlanet functions
//---------------------------

/**
  * Calculates planet median surface temperature in [K]
  * Surface temparature expression is
  *   T = ((Aabs / Arad) * (Lsol * (1 – a) / (4 * pi * Lsigma * eta * D^2 ))) ^ 1/4
  * with
  *   Aabs - area of the planet that absorbs the power from the star
  *   Arad - the entire planet is not at the same temperature,
  *     it will radiate as if it had a temperature T over an area.
  *   Arad which is again some fraction of the total area of the planet.
  *   Aabs/Arad (Aabs_Arad) - 1/4 for ? Planets; 1/2 for ? Planets.
  *   a - albedo (radiation reflection). An albedo of 1 means that all
  *     the radiation is reflected, an albedo of 0 means all of it is absorbed.
  *     E.g Earth = 0.29
  *   Lsigma - blackbody constant 5.67E-008 [W*m^-2*K^-4] 
  *   eta - emissivity and represents atmospheric effects.
  *     eta ranges from 1 to 0 with 1 meaning the planet is a perfect blackbody
  *     and emits all the incident power. E.g. Earth =  0.96
  * @param Lstar - star luminosity * Lsol in Watt [W]
  * @param distAu - distance from sun center in [km]
  * @return planetTemperature in [K]
  */
float planetTemperature(float Lstar, float distAu) {
    float Aabs_Arad = 0.25f;
    float albedo = 0.0f; // Earth = 0.29f;
    float eta = 1.0f;    // Earth = 0.96f;
    float L = Lstar * Lsol;
    //float result = pow( Aabs_Arad * (L * (1.0f - albedo) / (4 * M_PI * Lsigma * eta * pow(distKm*1e3f, 2.0f)) ), 0.25f );
    // TODO change distance to be in AU
    float result = pow( Aabs_Arad * (L * (1.0f - albedo) / (4 * M_PI * Lsigma * eta * pow(distAu*au2km*1e3f, 2.0f)) ), 0.25f );
    return result;
}

/**
  * Sets the planet index from periodic table of planets.
  * The index is a number in the range of [0..17].
  * It is combined from the temperature zone [0,6,12] plus
  * mass index [0..5]
  * @param hzMinAu
  * @param hzMaxAu
  * @return
  */
int getPlanetTypeIndex(UniversePlanet &planet, float hzMinAu, float hzMaxAu) {
    
    // Warm Zone - default temperature zone
    int iZoneIdx = 6;
    // Hot Zone - water evaporates above evaporation point
    if (planet.starDistance < hzMinAu) {
        iZoneIdx = 0;
    }
    // Cold Zone - water can only exist in ice form below freezing point
    if (planet.starDistance > hzMaxAu) {
        iZoneIdx = 12;
    }

    // mass category
    // Mercurian, Subterran, Terran, Superterran, Neptunian, Jovian
    int iMassIdx = 0;
    for (int i=0; i<6; i++) {
        if ( (planet.mass/Mearth > Mearth_min[i]) & (planet.mass/Mearth < Mearth_max[i]) ) {
            iMassIdx = i;
            continue;
        }
    } // for

    int typeIndex = iZoneIdx+iMassIdx;

    return typeIndex;
}

std::string getPeriodicType(int typeIndex) {
    if (typeIndex>-1) {
        return planetType[typeIndex];
    }
    return "unknown";
}

int getPeriodicTypeColumn(int typeIndex) {
    if (typeIndex>-1) {
        return typeIndex % 6;
    }
    return -1;
}

/**
  * Creates atmosphere based on the probability
  * of each of the planet types.
  * Gas giants are always considered to have an atmosphere.
  * The atmosphere object is always initiated.
  * If a planet has no atmosphere, the atmo radius
  * will be set to zero.
  */
UniverseAtmosphere createAtmosphere(int typeIndex, float planetRadius, pcg32 &rnd) {

    float atmProb = rnd.nextFloat();

    // return empty struct if random probability out
    // of max range and check it with atmosphere.exists()
    if (atmProb>atmosphereProbabilityMax[typeIndex]) { return UniverseAtmosphere(); }

    //cout << "--- createAtmosphere planet\n";
    //cout << "typeIndex: " << typeIndex << "\n";
    //cout << "atmo max probability: " << atmosphereProbabilityMax[typeIndex] << "\n";
    //cout << "random atmProb: " << atmProb << "\n";
    //cout << "planet atmosphere is probable" << "\n";

    UniverseAtmosphere atmosphere = UniverseAtmosphere();

    // if mercurial, or terran, atmosphere radius must
    // be greater than surface radius
    if (getPeriodicTypeColumn(typeIndex) <= 3) {
        atmosphere.radius = planetRadius * (1.01f + rnd.nextFloat() * 0.09f);
    } else { // gas giant
        atmosphere.radius = planetRadius;
    }
    //cout << "planet radius = " << planetRadius << "\n";
    //cout << "atmosphere radius" << atmosphere.radius << "\n";

    atmosphere.pressure = atmospherePressureMin[typeIndex] + rnd.nextFloat()
        * (atmospherePressureMax[typeIndex] - atmospherePressureMin[typeIndex]);

    // create atmosphere components
    createComposition(atmosphere.composition, rnd);

    return atmosphere;
}

/**
 * @brief Estimates planet habitability probability
 * without technological aids.
 * Covered by periodic table of planets:
 *      . is in habitable zone (to sustain liquid water)?
 *      . is 0.3 - 10 Earth masses (without excess gravity)?
 * - has habitable atmosphere?
 *    . has breathable atmosphere?
 *    . has the right composition?
 *
 * Probability that the planet is habitable
 *   P = Pinhz * Ptemp * Pgrav * Patmo
 * with
 *   Pinhz - planet is in habitable zone
 *   Ptemp - has temperature range within human physiology
 *   Pgrav - has gravity within human physiology
 *   Patmo - has breathable atmosphere
 * 
 * TODO: Other parameters will be necessary for other
 * non-humanoid species.
 * TODO: Ptemp can be enhanced with checking for
 * equator or pole temperature to be in habitable range.
**/
void calcPlanetHabitability(UniversePlanet &planet) {
    //cout << "DEBUG: --- calcPlanetHabitability\n";

    // physiological limits -50C to 50C
    if ( (planet.temperature<223) | (planet.temperature>323) ) {
        planet.probTemp = 0.0f;
        //return 0f;
    } else {
        // well-being at 20C=293
        planet.probTemp = 1.0f - abs(293-planet.temperature)/70.0f;
    }
    //cout << "DEBUG: planet.probTemp = " << planet.probTemp << "\n";

    // relative surface gravity
    float grel = 0.0f;
    if ( (planet.mass!=0) & (planet.radius!=0) ) {
        grel = (G * planet.mass / pow(planet.radius*1e3f, 2.0f)) / gEarth;
    }
    //cout << "DEBUG: grel = " << grel << "\n";

    // physiological limits 0.2g to 3g
    if ( (grel<0.2f) | (grel>3.0f) ) {
        planet.probGrav = 0.0f;
    } else {
        // well-being at 20C=293K
        planet.probGrav = 1.0f - abs(1.0f-grel)/2.0f;
    }
    //cout << "DEBUG: planet.probGrav = " << planet.probGrav << "\n";

}

float getPlanetHabitability(UniversePlanet &planet) {
    calcPlanetHabitability(planet);
    if (!planet.isInHz) {
        return 0.0f;
    }
    if (planet.atmosphere.exists()) {
        return planet.probTemp * planet.probGrav * atmosphereHabitability(planet.atmosphere.composition);
    } else {
        return planet.probTemp * planet.probGrav;
    }

}


//-----------------------------------
// Model of Universe Star
//-----------------------------------

//-----------------------------------
// star enumerators
//-----------------------------------

/** comments
    spectralClass LuminosityClass Designation
    B   I   blue supergiant
    A   I   supergiant
    F   I   supergiant
    G   I   supergiant
    K   I   red supergiant
    M   I   red supergiant
    G   III regular giant
    K   III regular giant
    M   III regular giant
    O   V   main-sequence star
    B   V   main-sequence star
    A   V   main-sequence star
    F   V   main-sequence star
    G   V   main-sequence star
    K   V   orange dwarf
    M   V   red giant
    L   V   red dwarf
    T   V   methane dwarfs
    Y       brown dwarf
    D       white dwarf
    R       carbon-based star
    N       carbon-based star
    S       zirconium-monoxide-based star
    W       dying supergiant
*/

/**
  * spectralClass & LuminosityClass
  * unused; for reference only
  */
/*std::list<std::string> starClass = {
  "B I", "A I", "F I",  // B,A,F I
  "G I", "K I", "M I",  // G,K,M I
  "G III", "K III", "M III",  // G,K,M III
  "O V", "B V", "A V",  // O,B,A V
  "F V", "G V", "K V",  // F,G,K V
  "M V", "L V", "T V",  // M,L,T V
  "Y", "D", "R",        // Y, D, R
  "N", "S", "W"         // N, S, W 
};*/

/**
  * spectralClass
  */
std::list<std::string> spectralClass = {
  "B", "A", "F",  // B,A,F I
  "G", "K", "M",  // G,K,M I
  "G", "K", "M",  // G,K,M III
  "O", "B", "A",  // O,B,A V
  "F", "G", "K",  // F,G,K V
  "M", "L", "T",  // M,L,T V
  "Y", "D", "R",  // Y, D, R
  "N", "S", "W"   // N, S, W 
};

/**
  * LuminosityClass
  */
std::list<std::string> luminosityClass = {
  "I", "I", "I",        // B,A,F I
  "I", "I", "I",        // G,K,M I
  "III", "III", "III",  // G,K,M III
  "V", "V", "V",        // O,B,A V
  "V", "V", "V",        // F,G,K V
  "V", "V", "V",        // M,L,T V
  "", "", "",           // Y, D, R
  "", "", ""            // N, S, W 
};

std::list<string> starDesignation = {
  "blue supergiant", "supergiant", "supergiant",      // B,A,F I
  "supergiant", "red supergiant", "red supergiant",   // G,K,M I
  "regular giant", "regular giant", "regular giant",  // G, K, M III
  "main-sequence", "main-sequence", "main-sequence",  // O,B,A V
  "main-sequence", "main-sequence", "orange dwarf",   // F,G,K V
  "red dwarf", "red dwarf", "methane dwarf",          //M,L,T V
  "brown dwarf", "white dwarf", "carbon-based",       // Y,D,R
  "carbon-based", "zirconium-monoxide-based star", "dying supergiant" // N,S,W
};

/**
  * Non-normalized source probability of star classes
  * basis for calculating
  * normalized cumulative probability
  * density function starTypeProbability.
  * Data is kept for reference only
  * and not used in this library.
  *
std::list<float> starTypeProbabilitySource = {
  0.10f, 0.10f, 0.10f,   // B,A,F I
  0.10f, 0.10f, 0.10f,   // G,K,M I
  0.10f, 0.10f, 0.10f,   // G,K,M III
  0.20f, 0.50f, 0.90f,   // O,B,A V
  1.00f, 1.00f, 1.00f,   // F,G,K V
  0.60f, 0.30f, 0.10f,   // M,L,T V
  0.05f, 0.01f, 0.01f,   // Y, D, R
  0.01f, 0.01f, 0.01f    // N, S, W  
};*/

std::list<float> probabilityAge = {
  0.10f, 0.10f, 0.10f,   // B,A,F I
  0.10f, 0.10f, 0.10f,   // G,K,M I
  0.10f, 0.10f, 0.10f,   // G,K,M III
  0.20f, 0.50f, 0.90f,   // O,B,A V
  1.00f, 1.00f, 1.00f,   // F,G,K V
  0.60f, 0.30f, 0.10f,   // M,L,T V
  0.05f, 0.01f, 0.01f,   // Y, D, R
  0.01f, 0.01f, 0.01f    // N, S, W  
};

/**
 * starType probability
 * cumulative distribution function (cdf)
 */
std::list<float> starTypeProbability = {
  0.015152,  0.030303,  0.045455,   // B,A,F I
  0.060606,  0.075758,  0.090909,   // G,K,M I
  0.106061,  0.121212,  0.136364,   // G,K,M III
  0.166667,  0.242424,  0.378788,   // O,B,A V
  0.530303,  0.681818,  0.833333,   // F,G,K V
  0.924242,  0.969697,  0.984848,   // M,L,T V
  0.992424,  0.993939,  0.995454,   // Y, D, R
  0.996970,  0.998485,  1.000000    // N, S, W 
};

/**
  * minimum radius in [Rsol]
  */
list<float> minRadius = {
    30.0f, 30.0f, 30.0f, 30.0f, 25.0f, 11.0f, // I B, A, F, G, K, M
    20.0f, 15.0f, 10.0f, // III G, K, M
    6.6f, 1.8f, 1.4f, // V O, B, A
    1.15f, 0.96f, 0.70f, 0.08f, // V F, G, K, M
    0.08f, 0.008f, 0.08f, 0.08f, // L, T, Y, D
    0.01f, 0.01f, 0.01f, 0.01f // R, N, S, W
};

/**
 * maximum radius in [Rsol]
 */
list<float> maxRadius = {
    2000.0f, 1900.0f, 1800.0f, 1700.0f, 1600.0f, 1.0f, // I B, A, F, G, K, M
    200.0f, 50.0f, 30.0f, // III G, K, M
    30.0f, 6.6f, 1.8f, // V O, B, A
    1.40f, 1.15f, 0.96f, 0.62f, // V F, G, K, M
    0.15f, 0.1f, 0.14f, 0.1f, // L, T, Y, D
    0.1f, 0.1f, 0.1f, 0.1f // R, N, S, W
};

// we abandon random luminosity in favor of modeled luminosity
// but keep the model for reference and future investigation
// should it become more favorable for some at this moment
// unexperimented and unfathomed reason
/*
// minimum luminosity in [Lsol]
list<float> minLuminosity = {
    6100.0f, 36000.0f, 12000.0f, 9500.0f, 12000.0f, 52000.0f, // B, A, F, G, K, M I
    113.0f, 32.0f, 3.3f, // G, K, M III
    55000.0f, 42.0f, 8.8f, // O, B, A V
    1.4f, 0.51f, 0.11f, 0.00015f, // F, G, K, M V
    0.000032f, 3.6e-6f, 1.0e-6f, 1.5e-6f, // L, T, Y, D
    1.0e-6f, 1.0e-6f, 1.0e-6f, 1.0e-6f // R, N, S, W
};
// maximum luminosity in [Lsol]
list<float> maxLuminosity = {
    320000.0f, 50600.0f, 20000.0f, 11000.0f, 20000.0f, 270000.0f, // B, A, F, G, K, M I
    127.0f, 96.0f, 15.0f, // G, K, M III
    200000.0f, 24000.0f, 24.0f, // O, B, A V
    5.1f, 1.2f, 0.38f, 0.08f, // F, G, K, M V
    0.00029f, 5.6e-6f, 3.6e-6f, 2.0e-6f, // L, T, Y, D
    1.0e-6f, 1.0e-6f, 1.0e-6f, 1.0e-6f // R, N, S, W
};
*/

/**
 * minimum mass in [Msol]
 */
list<float> minMass = {
    10.0f, 5.0f, 4.0f, 3.0f, 2.0f, 7.0f, // B, A, F, G, K, M I
    30.0f, 20.0f, 3.0f, // G, K, M III
    16.0f, 2.1f, 1.4f, // O, B, A V
    1.04f, 0.8f, 0.08f, 0.075f, // F, G, K, M V
    0.005f, 0.005f, 0.0005f, 0.005f, // L, T, Y, D
    0.005f, 0.005f, 0.005f, 0.005f // R, N, S, W
};

/**
 * maximum mass in [Msol]
 */
list<float> maxMass = {
    100.0f, 30.0f, 20.0f, 11.0f, 40.0f, 40.0f, // B, A, F, G, K, M I
    100.0f, 70.0f, 15.0f, // G, K, M III
    200.0f, 24000.0f, 2.1f, // O, B, A V
    1.4f, 1.04f, 0.45f, 0.6f, // F, G, K, M V
    0.08f, 0.008f, 0.02f, 0.008f, // L, T, Y, D
    0.08f, 0.08f, 0.08f, 0.08f // R, N, S, W
};

/**
 * minimum effective temperature in [K]
 */
list<float> minTemperature = {
    9700.0f, 8300.0f, 6150.0f, 5050.0f, 3750.0f, 2950.0f, // B, A, F, G, K, M I
    4870.0f, 3780.0f, 2800.0f, // G, K, M III
    3780.0f, 11400.0f, 7920.0f, // O, B, A V
    6300.0f, 5440.0f, 4000.0f, 2600.0f, // F, G, K, M V
    1500.0f, 800.0f, 500.0f, 500.0f, // L, T, Y, D
    500.0f, 500.0f, 500.0f, 500.0f // R, N, S, W
};

/**
 * maximum effective temperature in [K]
 */
list<float> maxTemperature = {
    21000.0f, 9400.0f, 7500.0f, 5800.0f, 4900.0f, 3690.0f, // B, A, F, G, K, M I
    5010.0f, 4720.0f, 3660.0f, // G, K, M III
    54000.0f, 29200.0f, 9600.0f, // O, B, A V
    7350.0f, 6050.0f, 5240.0f, 3750.0f, // F, G, K, M V
    2600.0f, 1400.0f, 1000.0f, 800.0f, // L, T, Y, D
    800.0f, 800.0f, 800.0f, 800.0f // R, N, S, W
};


/**
 * Manually determined star colors.
 * In future to be replaced by an expression
 * like e.g. colot index = -2.5 log (temperature).
 * see also https://docs.kde.org/trunk5/en/extragear-edu/kstars/ai-colorandtemp.html
 */
list<vector<float>> apparentColors = {
  { 0.906f, 0.878f, 1.000f }, // B I
  { 0.792f, 0.749f, 0.929f }, // A I
  { 0.992f, 0.992f, 0.925f }, // F I
  { 1.000f, 1.000f, 1.000f }, // G I
  { 1.000f, 0.992f, 0.439f }, // K I
  { 0.965f, 0.800f, 0.298f }, // M I
  { 1.000f, 1.000f, 1.000f }, // G III
  { 1.000f, 0.992f, 0.439f }, // K III
  { 0.965f, 0.800f, 0.298f }, // M III
  { 0.973f, 0.561f, 0.380f }, // O V
  { 0.906f, 0.878f, 1.000f }, // B V
  { 0.792f, 0.749f, 0.929f }, // A V
  { 0.992f, 0.992f, 0.925f }, // F V
  { 1.000f, 1.000f, 1.000f }, // G V
  { 1.000f, 0.992f, 0.439f }, // K V
  { 0.965f, 0.800f, 0.298f }, // M V
  { 0.957f, 0.298f, 0.227f }, // L
  { 0.741f, 0.024f, 0.361f }, // T
  { 0.361f, 0.020f, 0.020f }, // Y
  { 1.0f, 1.0f, 1.0f },       // D
  { 1.0f, 1.0f, 1.0f },       // R
  { 1.0f, 1.0f, 1.0f },       // N
  { 1.0f, 1.0f, 1.0f },       // S
  { 1.0f, 1.0f, 1.0f }        // W
};

/**
 * @brief Habitable zone limit descriptors
 * as array.
 */
std::string HZDescription[8] = { 
  "unused", "Inner HZ 'Recent Venus' limit", // 0,1
  "'Runaway Greenhouse' limit", // 2
  "Inner HZ 'Moist Greenhouse' (waterloss) limit", // 3
  "Outer HZ 'Maximum Greenhouse' limit", // 4
  "Outer HZ 'Early Mars' limit", // 5
  "2 AU Cloud limit", "First CO2 Condensation limit" // 6,7
};


//-----------------------------------
// star data structure
//-----------------------------------

/**
 * @brief ProcU model for a star.
 * It is pseudo-scientific and intended mainly
 * for game content generation.
 *
 */
struct UniverseStar {

  //---------------------------------
  // Variables
  //---------------------------------

  // the seed is the star global unique identifier (GUID)
  uint64_t seed;
  // name of the star
  std::string name = "";

  // position within the sector cube
  std::vector<double> position = {0.0, 0.0, 0.0};

  // stellar type, e.g. "G2V", or "M5V"
  // sectral class ("M") + temperatur sequence ("5.5") + luminosity class ("V")
  uint typeIndex = 0;
  std::string spectralClass = "";
  std::string temperatureSequence = "";
  std::string luminosityClass = "";
  std::string stellarType = "";
  std::string designation = "";
  // mass in [Msol]
  float mass = 0;
  // luminosity in [Lsol]
  float luminosity = 0;
  // photosphere temperature [K]
  float temperature = 0;
  // float radius: radius in [Rsol]
  float radius = 0;
  // star (black body) color rgb [0..255]
  std::vector<byte> color = {0,0,0};

  // Habitable Zone (HZ) limits
  // hzDistAu[1] --> Recent Venus (inner HZ limit)
  // hzDistAu[5] --> Early Mars (outer HZ limit)
  // for info on other zone limits
  // see the generator function
  // habitableZoneComplete(...)
  float hzDistAu[8];

  // frost limit in [au] where temperature reaches 150K
  float frostLimitAu = 0;

  // a collection of planets in the system
  // identified by planet uint64_t seed, and planet object struct
  std::map<uint64_t, UniversePlanet> planets;

  // how many planets does this star host
  uint planetsCount;

  // axial rotation period in [s]
  float axialRotation = 0;

  // fluctuation of luminosity output in percent [%]
  float outputVariation = 0;

  // yet unused
  float magnitude = 0.0;
  // string position_ref: orbit pivot object ("system", gravity center, star name, or planet name) -->
  std::string positionReference;
  // Standard gravitational parameter mu in [km^3 s^-2]
  // float mu: standard gravitational parameter (G*M) in [km^3 s^-2] -->
  float mu = 0;
  // orbit period in [s]
  float orbitPeriod = 0;
  float metallicity = 0;
  // solar cycle between maximum intensities in earth years [y]
  float solarCycle = 0;
  

  //---------------------------------
  // Constructor
  //---------------------------------

  /**
   * @brief constructor without parameters
   * @code  constructor: UniverseSystem (); @endcode 
   */
  UniverseStar() {};

}; // end struct


//---------------------------
// UniverseStar functions
//---------------------------

/**
  * @brief Calculates the mass density for stars in sections
  * below and above the 150K frost limit.
  * Below the frost limit, a normal distribution is used.
  * Above the frost limit, an inverse exponential
  * distribution is used.
  * @param starMass - star mass in [Msol]
  * @param frostLimitAu - frost limit distance from star in [au]
  * @param posAu - distance from star to get mass density at in [au]
  * @return massDensity - mass density in [kg*au^-1]
  */
float getStarMassDensity(float starMass, float frostLimitAu, float posAu) {
    float massDensity = 0.0f;
    if (posAu<frostLimitAu) {
        // inside of frost limit
        massDensity = 4.2e24f * (starMass) * normalDistribution(posAu, frostLimitAu / 2.0f, frostLimitAu / 16.0f);
    } else {
        // outside of frost limit
        massDensity = 8.0e26f * (starMass) * inverseExpDistribution(posAu, 0.5f);
    }
    return massDensity;
}

/**
  * Frost limit is the distance from the star
  * where temperature reaches 150K.
  * It is a limit for the transition of the star system
  * mass density function.
  * D_FL = ( 0.25 * luminosity * 3.84e26 / (150K^4 * 4 * pi * sigma)) ^ 1/2
  */
float calcFrostLimit(float lumStar) {
    float frostLimitAu = sqrt(0.25f * lumStar * Lsol / (5.0625e8f * 4 * M_PI * Lsigma)) * m2au ;
    //cout << "    (debug) frostLimitAu = " << frostLimitAu << endl;
    return frostLimitAu;
}

/**
  * @brief Calculate the habitable zone parameters of a
  * star given the photospheric temperature tEff in
  * degrees Kelvin [K] and the stellar luminosity in
  * solar units [Lsol] for F, K, G, and M type stars
  * with temparature range from 2600K to 7200K.
  *
  * This code calculates habitable zone 'fluxes' using
  * the expression given in the Kopparapu et al. paper
  * "Habitable Zones Around Main-Sequence Stars".
  * The corresponding output file is 'HZ_fluxes.dat'.
  * It also generates a file 'HZ_coefficients.dat'
  * that gives the coefficients for the analytical expression.
  *
  * The result columns, i, are arranged according to
  * the HZ limits given in the paper plus two additional.
  * i = 0 --> (unused)
  * i = 1 --> Recent Venus (inner HZ limit)
  * i = 2 --> Runaway Greenhouse
  * i = 3 --> Moist Greenhouse
  * i = 4 --> Maximum Greenhouse
  * i = 5 --> Early Mars (outer HZ limit)
  * i = 6 --> 2 AU cloud limit (Note: This limit is not given in the paper)
  * i = 7 --> 1st CO2 condensation limit (Note: This limit is not given in the paper)
  * 
  * Usage: create a float [8] array and pass it to the function.
  * The HZ limits will be written to that array.
  * 
  * Results outside the temparature range [2600K to 7200K]
  * may return zero results, so you may want to check for 0.
  * 
  * @author pyramid (c++ port) (c) 2020
  * @author Ravi Kumar Kopparapu 2012-02-25
  */
void habitableZoneComplete(float (&hzDistAu)[8], float tEff, float lumStar) {
    /*
      * Coeffcients to be used in the analytical expression
      * to calculate habitable zone flux boundaries for each
      * habitable zone limit.
    **/

    float seffsun [8] = {
      seffsun[1] = 1.7763f,
      seffsun[2] = 1.0385f,
      seffsun[3] = 1.0146f,
      seffsun[4] = 0.3507f,
      seffsun[5] = 0.3207f,
      seffsun[6] = 0.2484f,
      seffsun[7] = 0.5408f
    };

    float a [8] = {
      a[1] = 1.4335e-4f,
      a[2] = 1.2456e-4f,
      a[3] = 8.1884e-5f,
      a[4] = 5.9578e-5f,
      a[5] = 5.4471e-5f,
      a[6] = 4.2588e-5f,
      a[7] = 4.4499e-5f
    };

    float b [8] = {
      b[1] = 3.3954e-9f,
      b[2] = 1.4612e-8f,
      b[3] = 1.9394e-9f,
      b[4] = 1.6707e-9f,
      b[5] = 1.5275e-9f,
      b[6] = 1.1963e-9f,
      b[7] = 1.4065e-10f
    };

    float c [8] = {
      c[1] = -7.6364e-12f,
      c[2] = -7.6345e-12f,
      c[3] = -4.3618e-12f,
      c[4] = -3.0058e-12f,
      c[5] = -2.7481e-12f,
      c[6] = -2.1709e-12f,
      c[7] = -2.2750e-12f
    };

    float d [8] = {
      d[1] = -1.1950e-15f,
      d[2] = -1.7511E-15f,
      d[3] = -6.8260e-16f,
      d[4] = -5.1925e-16f,
      d[5] = -4.7474e-16f,
      d[6] = -3.8282e-16f,
      d[7] = -3.3509e-16f
    };

    /**
      * Calculating HZ fluxes for stars with 2600 K < T_eff < 7200 K.
    **/
    // calculate the stellar fluxes
    float tStar = tEff - 5780.0f;
    float sEff [8];
    for (int i = 1; i < 8; i++) {
          sEff[i] = max((double)(seffsun[i] + a[i]*tStar + b[i]*pow(tStar,2) + c[i]*pow(tStar,3) + d[i]*pow(tStar,4)), 0.0);
    }

    // calculate the hz distances from star in [au]
    hzDistAu[0] = 0.0;
    for (int i = 1; i < 8; i++) {
        if (sEff[i]==0.0f) {
            hzDistAu[i] = 0.0f;
        } else {
            hzDistAu[i] = sqrt(lumStar/sEff[i]);
        }
        //cout << "hzDistAu[" << i << "] = " << hzDistAu[i] << "\n";
    }

}

/**
 * @brief Checks if star has planets in the habitable zone
 */
bool hasPlanetsInHz(UniverseStar &star) {
    //int i = 0;
    for (std::pair<uint64_t, UniversePlanet> element : star.planets) {
      // Accessing KEY from element
      //uint64_t planetSeed = element.first;
      // Accessing VALUE from element
      UniversePlanet planet = element.second;
      //cout << "DEBUG: " << i << ": " << planetSeed << " :: " << planet.isInHz << "\n";
      if (planet.isInHz) { return true; }
     // ++i;
    }
    return false;
}

/**
  * @brief Returns index number of a star from the
  * cumulated probability distribution function of
  * star occurences based on a uniform randim
  * input number.
  * @param float random uniform
  * @return int index
  */
int getRndStarIdx(float rn) {
  int idx = 0;
  for(std::list<float>::iterator iter=starTypeProbability.begin(),
      prev=starTypeProbability.end(); 
      iter != starTypeProbability.end(); prev=iter, ++iter ) {
    float ub = *iter; // upper bound
    idx = std::distance(starTypeProbability.begin(), iter);
    cout << idx << " ";
    if (rn<=ub) { break; }
    // compare with previous element
    //if (prev != probCdf.end()) {
      //prevEl = *prev }
  }
  //cout << "\n";
  //cout << "star type: " << idx << "\n";
  return idx;
}

/**
  * Calculates the star luminosity from its mass
  * Reference: http://en.wikipedia.org/wiki/Mass%E2%80%93luminosity_relation
  * for M < 0.43 Msol -> L/Lsol = .23 * M/Msol ^2.3
  * for .43 Msol < M < 2 Msol -> L/Lsol = M/Msol ^4
  * for 2 Msol < M < 20 Msol -> L/Lsol = 1.5 * M/Msol ^3.5
  * for 20 Msol < M -> L/Lsol = 3200 * M/Msol
  */
float calcLuminosity(float mass) {
    float luminosity = 0.0;
    //if (mass == NULL) { return -1.0f; }

    float massMSol = mass; //  / Msol;
    if (massMSol<0.43f) {
        luminosity = 0.23f * pow(massMSol, 2.3f);
    }
    if ( (massMSol>=0.43f) & (massMSol<2.0f) ) {
        luminosity = pow(massMSol, 4.0f);
    }
    if ( (massMSol>=2.0f) & (massMSol<20.0f) ) {
        luminosity = 1.5f * pow(massMSol, 3.5f);
    }
    if (massMSol>=20.0f) {
        luminosity = 3200.0f * massMSol;
    }
    
    return luminosity;
}

/**
  * @brief Calculates star colr rgb values
  * from temperature in Kelvin [K].
  * The algorithm is taken from
  * https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html
  * @param   star temperature in Kelvin [K]
  * @return  vector of red, green and blue color values
  * @author  pyramid
  * @author  Tanner Helland
  */
std::vector<byte> getStarColor(float starTemperatureK) {
  float temperature = starTemperatureK / 100.0;
  float red, green, blue;

  if (temperature <= 66.0) {
    red = 255;
  } else {
    red = temperature - 60.0;
    red = 329.698727446 * pow(red, -0.1332047592);
    if (red < 0) red = 0;
    if (red > 255) red = 255;
  }

  /* Calculate green */

  if (temperature <= 66.0) {
    green = temperature;
    green = 99.4708025861 * log(green) - 161.1195681661;
    if (green < 0) green = 0;
    if (green > 255) green = 255;
  } else {
    green = temperature - 60.0;
    green = 288.1221695283 * pow(green, -0.0755148492);
    if (green < 0) green = 0;
    if (green > 255) green = 255;
  }

  /* Calculate blue */

  if (temperature >= 66.0) {
    blue = 255;
  } else {

    if (temperature <= 19.0) {
      blue = 0;
    } else {
      blue = temperature - 10;
      blue = 138.5177312231 * log(blue) - 305.0447927307;
      if (blue < 0) blue = 0;
      if (blue > 255) blue = 255;
    }
  }

  return {(byte)red, (byte)green, (byte)blue};
}

/**
  * Stellar classification divides temperature classes into
  * subclasses named with arabic numerals and ranging from
  * the hottest stars within a class starting with 0 to their
  * coolest stars denoted with up to 9.9.
  * 
  * We have adopted a simple model with ten temperature
  * classes ranging between 0 and 9, and without
  * fractional numbers.
  **/
std::string genStarTemperatureSequence(int idx, float temperature) {
  std::string tempSeq = "";
  //std::cout << "  ... fx | genStarTemperatureSequence\n";
  float temperatureMin = *(std::next(minTemperature.begin(), idx));
  float temperatureMax = *(std::next(maxTemperature.begin(), idx));
  // step size for 10 steps
  float step = (temperatureMax-temperatureMin) / 10;
  // find step multiplier for input temp (0 - highest, 9 - lowest)
  float mult = (temperatureMax - temperature) / step;
  tempSeq = to_string((int)mult);

  //std::cout << "  star temperature " << temperature << "\n";
  //std::cout << "  temperatureMin " << temperatureMin << "\n";
  //std::cout << "  temperatureMax " << temperatureMax << "\n";
  //std::cout << "  step " << step << "\n";
  //std::cout << "  mult " << mult << "\n";
  //std::cout << "  mult (int) " << (int)mult << "\n";
  //std::cout << "  ... return : " << tempSeq << "\n";
  return tempSeq;
}

/**
  * @brief Calculates star system probability of hosting
  * habitable planets.
  * 
  * H = Pa * Pb
  *  - Pa: probability based on age for planet formation and creation of atmosphere
  *      as well as less risk of comet and asteroid impacts due to forming age of the system
  *      = 1.0 for age above 7e8 years (700 million)
  *  - Pb: Probability of habitable zone within 0.2 * distance between binary stars
  *      or around both binary stars
  *  - Fv: luminosity output variation (1-v)
  *  - Fu: hf uv radiation
  */
float getHabitablePlanetsProbability(UniverseStar &star) {
    float probAge = 0.0f; // star system age
    float probVar = 0.5f; // luminosity variation
    float probRad = 1.0f; // radiation (calculation unknown)

    //get age probability from star type index
    // or throw "undefined probabilityAge for spectralType " << spectralType;
    probAge = *(std::next(probabilityAge.begin(), star.typeIndex));
    // get output variation probability
    probVar = 1.0f - star.outputVariation;
    //std::cout << "DEBUG:  probAge " << probAge << "\n";
    //std::cout << "DEBUG:  probVar " << probVar << "\n";

    return probAge * probVar * probRad;
}


//-----------------------------------
// Model of Universe System
//-----------------------------------

//-----------------------------------
// system enumerators
//-----------------------------------

/**
 * @brief Predefined enums for system habitability.
 * It may be used for simplified galaxy map views to
 * inidcate system habitability by color code.
 * It is not defined here how this habitability will be established.
 * It may for example be the sum of the habitability status of all 
 * the systems' planets.
 * @author pyramid
 * @example SystemHabitabilityStatus sysHabitability = UNKNOWN;
 */
enum SystemHabitabilityStatus {
  UNKNOWN = 0,
  UNHABITABLE,
  HABITABLE
};

/**
 * @brief Predefined enums for per player system survey status.
 * The status is not part of the static system model, as it depends
 * on each player history interaction with the galaxy map.
 * In addition to the survey info, the UNDISCOVERED status is
 * added that may indicate that the system has not yet been discovered
 * by the player.
 * @author pyramid
 * @example SystemSurveyStatus sysStatus = UNDISCOVERED;
 */
enum SystemSurveyStatus {
  UNDISCOVERED = 0,
  UNSURVEYED,
  SURVEYED
};

/**
 * @brief Predefined enums for system multiplicity.
 * @author pyramid
 * @example SystemMultiplicity sysMultiplicity = Undefined;
 */
enum SystemMultiplicity {
  Undefined = 0,
  Unary,
  Binary,
  Trinary,
  Quaternary,
  Quintenary,
  Sextenary,
  Septenary
};

/**
 * @brief multiple star system probability
 * cumulative distribution function (cdf).
 * This data is invented without any scientific basis.
 * TODO: research scientific probabilities
 */
std::list<float> starSystemMultiProbability = {
  0.800,  // unary
  0.900,  // binary
  0.950,  // trinary
  0.975,  // quaternary
  0.988,  // quintenary
  0.996,  // sextanary 
  1.000   // septenary 
};

//-----------------------------------
// system data structure
//-----------------------------------

/**
 * @brief ProcU model for a star system.
 * It is pseudo-scientific and indended mainly for game
 * content generation.
 * Stores generated system coordinates and seed
 * as well as other relevant parameters.
 */
struct UniverseSystem {

  // the seed is the system global unique identifier (GUID)
  uint64_t seed;

  // the parent sector seed
  uint64_t sector;

  // system coordinates within the parent sector cube
  // where (0,0,0) is cube's 'lower-left' coordinate\n";
  //band a cube has SECTOR_SIZE_LY ly extension in each coordinate\n";
  std::vector<double> position;

  // system name
  std::string name = "";

  // a collection of stars in the system
  // identified by the star uint64_t seed, and star object struct
  std::map<uint64_t, UniverseStar> stars;

  // number of stars in the system
  int multiplicity = 0;

  /**
   * @brief constructor without parameters
   * @code  constructor: UniverseSystem(); @endcode 
   */
  UniverseSystem() {};

}; // end struct


//-----------------------------------
// Model of Universe Sector
//-----------------------------------

//-----------------------------------
// system data structure
//-----------------------------------

/**
 * @brief ProcU model for a universe sector.
 * Stores generated systems and sector coordinates.
 */
struct UniverseSector {

  // the seed is the global unique identifier (GUID)
  uint64_t seed;

  // sector coordinates within the parent galaxy
  std::vector<double> position;

  // sector name
  std::string name = "";

  // a collection of stars in the system
  // identified by the star uint64_t seed, and star object struct
  std::vector<uint64_t> systemSeeds;

  /**
   * @brief constructor without parameters
   * @code  constructor: UniverseSector(); @endcode 
   */
  UniverseSector() {};

}; // end struct


//-----------------------------------
// libProcU procu::ProcUGalaxy enum
//-----------------------------------

/**
 * Galaxy Types that can be generated
**/
enum GALAXY_TYPE {
    SPIRAL   = 0,
    GLOBULAR = 1
};


//-----------------------------------
// libProcU procu::ProcUGalaxy class
//-----------------------------------

/**
 * @brief ProcUGalaxy\n
 * A galaxy procedural generator.
 * See documentation at the top of this library file.
 * 
 * @author pyramid
**/

class ProcUGalaxy {

private:

public:
  /// configuration variables
  // galaxy size x, y, z
  int GALAXY_TYPE = GALAXY_TYPE::SPIRAL;
  std::vector<double> GALAXY_SIZE_LY{1.0e4, 100.0, 1.0e4};
  double SECTOR_SIZE_LY = 10.0;
  int MAX_SYSTEMS = 10;  // per sector
  int MAX_STARS = 3;     // per system
  int MAX_PLANETS = 10;  // per system

  // the galaxy seed is global
  uint64_t galaxySeed;

  // the rnd generator
  pcg32 rng;

  // galaxy data structure: holds sector data
  std::map<uint64_t, UniverseSector> sectors;

  // galaxy data structure: holds system data
  std::map<uint64_t, UniverseSystem> systems;

  /**
   * Class constructor
  **/
  ProcUGalaxy() {}

  /**
   * Class destructor
  **/
  ~ProcUGalaxy() {}


  //---------------------------------
  // create seeds
  //---------------------------------

  /**
   * @brief creates a random uint64_t seed from the
   * system's random device.
   * @param  none
   * @return uint64_t seed
   * @author pyramid
  **/
  uint64_t createGalaxySeed() {
    // create pristine seed
    //cout << "--- creating pristine random seed\n";
    random_device rd;  // rom random device
    galaxySeed = static_cast<uint64_t>(rd());
    //cout << "  seed from random device: 0x" << setw(16) << setfill('0') << hex << uSeed
    //  << dec << " (" << uSeed << ") ("<< sizeof(uSeed) << " bytes)\n";
    // initialize random generator and retuen seed
    rng.seed(galaxySeed);
    return galaxySeed;
  } // end function

  /**
   * @brief set and store new galaxy uint64_t seed
   * @param  uint64_t seed
   * @return none
   * @author pyramid
  **/
  void setGalaxySeed(uint64_t seed) {
    galaxySeed = seed;
    rng.seed(galaxySeed);
  } // end function

  /**
   * Returns seed for the sector specified by the sector
   * coordinate in xyz.
   * The galaxy seed needed for this function is obtained
   * from the global variable galaxySeed.
   */
  uint64_t getSectorSeed(const int x, const int y, const int z) {
    // here we modify the sector seed to be derived from the galaxy seed
    // we must do the multiplication as signed integers
    // to account for potential overflow
    // and then convert back to unsigned integer to keep the seed,
    // since seed cannot be negative
    // clang-9 does not handle overflow
    //uint64_t seedSector = (uint64_t)((int64_t)(galaxySeed) * ((int64_t)6e12 + (int64_t)x*(int64_t)1e9 + (int64_t)z*(int64_t)1e4 + (int64_t)y));
    //uint64_t seedSector = (uint64_t)((int64_t)(galaxySeed) + ((int64_t)6e13 + (int64_t)x*(int64_t)1e9 + (int64_t)z*(int64_t)1e5 + (int64_t)y));
    // clang-9 doesn't handle casting well
    // so the following works better
    uint64_t seedSector = galaxySeed + 6e14 + x*1e9 + z*1e5 + y;

    //cout << "  galaxy seed input: " << hex << setw(16) << setfill('0') << hex << galaxySeed 
    //  << dec << " ("<< galaxySeed << ") (" << dec << sizeof(galaxySeed) << " bytes)\n";
    //cout << "  getSectorSeed:" << setfill(' ') << setw(3) << x << setw(3) << y << setw(3) << z << " : "
    //  << hex << setw(16) << setfill('0') << hex << seedSector 
    //  << dec << " ("<< seedSector << ") (" << dec << sizeof(seedSector) << " bytes)\n";
    return seedSector;
  } // end function

  /**
   * @brief Creates system seeds
   * TODO: only create required number of seeds instead of MAX_SYSTEMS
  **/
  std::vector<uint64_t> getSystemSeeds(const uint64_t uSectorSeed) {
    std::vector<uint64_t> vSystemSeeds;
    for (int n=0; n<MAX_SYSTEMS; ++n) {
      uint64_t uSeedSystem = (uint64_t)((int64_t)uSectorSeed + 123 + (int64_t)1e11*(int64_t)n);
      vSystemSeeds.push_back(uSeedSystem);
      //cout.precision(4);
      //cout << "  " << n << " : "
      //  << "0x" << hex << uSeedSystem << dec << " (" << uSeedSystem
      //  << ") (" << sizeof(uSeedSystem)<< " bytes)\n";
    }

    return vSystemSeeds;
  } // end function

  /**
   * @brief Creates star seeds
   * @param uint8_t howMany - How many stars there are in the system
  **/
  std::vector<uint64_t> getStarSeeds(const uint64_t uSystemSeed, uint8_t howMany) {
    std::vector<uint64_t> vStarSeeds;

    // generate star seeds
    for (int n=0; n<howMany; ++n) {
      uint64_t uSeedStar = (uint64_t)((int64_t)uSystemSeed + 1.876e8 + 1e4*(int64_t)n);
      vStarSeeds.push_back(uSeedStar);
    }
    return vStarSeeds;
  } // end function

  /**
   * @brief Creates planet seeds
   * @param uint8_t howMany - How many planets there are for this star
  **/
  std::vector<uint64_t> getPlanetSeeds(const uint64_t uStarSeed, uint8_t howMany) {
    std::vector<uint64_t> planetSeeds;
    for (int n=0; n<howMany; ++n) {
      uint64_t uSeedPlanet = (uint64_t)((int64_t)uStarSeed + 5432 + (int64_t)n*1e4 + n);
      //uint64_t uSeedPlanet = (uint64_t)((int64_t)uStarSeed + n);
      planetSeeds.push_back(uSeedPlanet);
      //cout << uSeedPlanet << "\n";
    }
    return planetSeeds;
  } // end function


  //---------------------------------
  // generate universe system data
  //---------------------------------

  UniverseSector genSector(const int x, const int y, const int z) {
    // init system data container
    UniverseSector sector = UniverseSector();
    sector.seed = getSectorSeed(x,y,z);
    sector.position = {(double)x, (double)y, (double)z};
    return sector;
  }

  /**
   * @brief Generates all sectors in galaxy
   */
  void genSectors() {
    for (int x=-GALAXY_SIZE_LY[0]/SECTOR_SIZE_LY/2; x<GALAXY_SIZE_LY[0]/SECTOR_SIZE_LY/2; ++x) {
      for (int z=-GALAXY_SIZE_LY[2]/SECTOR_SIZE_LY/2; z<GALAXY_SIZE_LY[2]/SECTOR_SIZE_LY/2; ++z) {
        for (int y=-GALAXY_SIZE_LY[1]/SECTOR_SIZE_LY/2; y<GALAXY_SIZE_LY[1]/SECTOR_SIZE_LY/2; ++y) {
            UniverseSector sector = genSector(x,y,z);
            sectors[sector.seed] = sector;
            //cout << setfill(' ') << setw(3) << x << setw(3) << y << setw(3) << z << " : "
            //  << "0x" << setw(16) << setfill('0') << hex << sector.seed
            //  << setw(3) << setfill(' ') << dec << " (" << sector.seed
            //  << ") (" << sizeof(sector.seed)<< " bytes)\n";
        } // y
      } // z
    } // x
  } // end genSectors

  //---------------------------------
  // generate universe system data
  //---------------------------------

  UniverseSystem genSystem(const uint64_t systemSeed) {
    // init system data container
    UniverseSystem system = UniverseSystem();

    // init generator
    pcg32 rng(systemSeed);

    // generate data
    system.seed = systemSeed;
    // generate random system position
    system.position = {
      rng.nextDouble() * SECTOR_SIZE_LY,
      rng.nextDouble() * SECTOR_SIZE_LY,
      rng.nextDouble() * SECTOR_SIZE_LY
    };
    // generate random system multiplicity
    float rnum = rng.nextFloat();
    //cout << "    rand number for getting system multiplicity: " << rnum << "\n";
    system.multiplicity = getRndCdfIdx(rnum, starSystemMultiProbability) + 1;
    //cout << "  number of stars in system: " << system.multiplicity << "\n";

    // insert or update the system data in the galaxy model
    systems[system.seed] = system;

    return system;
  } // end function

  /**
   * @brief Generates systems and adds them to sector
   */
  void genSystems(const uint64_t sectorSeed) {
    // add seeds to sector
    sectors[sectorSeed].systemSeeds = getSystemSeeds(sectorSeed);
  }


  //---------------------------------
  // generate universe star data
  //---------------------------------

  UniverseStar genStar(uint64_t starSeed) {
    //cout << "  generating star "
    //  << "0x" << setw(16) << setfill('0') << hex << starSeed << dec << " ("
    //  << starSeed << ") (" << sizeof(starSeed) << " bytes)\n";

    // init star data container
    UniverseStar star = UniverseStar();

    // set object seed
    star.seed = starSeed;
    rng.seed(starSeed);
    
    // get probability index from starTypeProbability
    // density function
    int idx = getRndCdfIdx(rng.nextFloat(), starTypeProbability);
    star.typeIndex = (uint)idx;

    // generate object data

    // star mass in [Msol]
    float massMin = *(std::next(minMass.begin(), idx));
    float massMax = *(std::next(maxMass.begin(), idx));
    star.mass = massMin + rng.nextFloat()*(massMax-massMin);

    // star radius in [Rsol]
    float radiusMin = *(std::next(minRadius.begin(), idx));
    float radiusMax = *(std::next(maxRadius.begin(), idx));
    star.radius = radiusMin + rng.nextFloat()*(radiusMax-radiusMin);

    // luminosity in [Lsol]
    // we abandon random luminosity in favor of modeled luminosity
    //float luminosityMin = *(std::next(minLuminosity.begin(), idx));
    //float luminosityMax = *(std::next(maxLuminosity.begin(), idx));
    //star.luminosity = luminosityMin + rng.nextFloat()*(luminosityMax - luminosityMin);
    star.luminosity = calcLuminosity(star.mass);

    // photosphere temperature in Kelvin [K]
    float temperatureMin = *(std::next(minTemperature.begin(), idx));
    float temperatureMax = *(std::next(maxTemperature.begin(), idx));
    star.temperature = temperatureMin + rng.nextFloat()*(temperatureMax - temperatureMin);

    // stellar classification (needs temperature)
    star.spectralClass = *(std::next(spectralClass.begin(), idx));
    star.luminosityClass = *(std::next(luminosityClass.begin(), idx));
    star.temperatureSequence = genStarTemperatureSequence(idx, star.temperature);
    star.stellarType = star.spectralClass + star.temperatureSequence + star.luminosityClass;
    star.designation = *(std::next(starDesignation.begin(), idx));

    // determine star color
    star.color = getStarColor(star.temperature);

    // generate specific per star habitable zone
    // i = 1 --> Recent Venus !!! (inner HZ limit)
    // i = 5 --> Early Mars !!! (outer HZ limit)
    // for all limits, see comments of the function
    habitableZoneComplete(star.hzDistAu, star.temperature, star.luminosity);
    //for(int i=0; i<8; ++i) {
    //  cout << "hzDistAu[" << i << "] = " << star.hzDistAu[i] << "\n";
    //}

    // calculate frost limit
    star.frostLimitAu = calcFrostLimit(star.luminosity);

    // axial rotation approximation from star radius and mass
    // = pi * radius [Rsol] * RSol [km] / mass [Msol]
    star.axialRotation = M_PI * star.radius * Rsol / star.mass;

    // how many planets will this star host?
    // we determine (unscientifically)
    // that a star may have between zero
    // and eight planets
    star.planetsCount = rng.nextUInt(8);
    //cout << "   (debug) plnets count : " << star.planetsCount << endl;

    return star;
  }

  void genStars(uint64_t systemSeed) {
    //cout << "  genStars\n";
    // get star seeds for this system
    vector<uint64_t> starSeeds = getStarSeeds(systemSeed, systems[systemSeed].multiplicity);

    // generate a random number of stars
    for(int i=0; i<systems[systemSeed].multiplicity; ++i) {
      //cout << "  starting star id " << i << "\n";
      // create at least one star
      //cout << "  star " << i+1 << "\n";
      UniverseStar star = genStar(starSeeds[i]);
      systems[systemSeed].stars[starSeeds[i]] = star;
    }
    //cout << "  generated " << systems[systemSeed].multiplicity << " star"
    //  << (systems[systemSeed].multiplicity==1? "":"s") <<".\n";

    // TODO: generate orbitals for all stars

  } // end function


  //---------------------------------
  // generate universe planet data
  //---------------------------------

  /**
   * Generate a planet for a parent star
   * @param planetSeed - seed for generating further data
   * @param star - parent star mass is required for planet
   *   mas density calculaiton
   * @param planetDistanceAu - planet distanc from the
   *   parent star
   * @param lowerLimitAu - start of the mass accretion
   *   distance from parent star
   * @return planet - UniversePlanet object
   */
  UniversePlanet genPlanet(uint64_t planetSeed, UniverseStar &star, float planetDistanceAu, float &lowerLimitAu) {
    //cout << "generating planet : "
    //  << "0x" << setw(16) << setfill('0') << hex << planetSeed << dec << " ("
    //  << planetSeed << ") (" << sizeof(planetSeed) << " bytes)\n";

    // init planet data container
    UniversePlanet planet = UniversePlanet();

    // set object seed
    planet.seed = planetSeed;
    rng.seed(planetSeed);

    /// generate planet object data

    // generate position
    planet.position = {planetDistanceAu, 0, 0};
    planet.starDistance = planetDistanceAu;
    if ( (planet.starDistance>star.hzDistAu[1]) & (planet.starDistance<star.hzDistAu[5]) ) {
      planet.isInHz = true;
    }

    // get the mass density at the planet position
    // by accretion of the mass between the lower limit
    // and the upper limit
    float upperLimitAu = planetDistanceAu + planetDistanceAu - lowerLimitAu;
    // average mass density in [kg * au^⁻1] taken at the
    // planet distance
    float massDensity = getStarMassDensity(star.mass, star.frostLimitAu, planetDistanceAu);
    // linear interpolation of the mass between lower and upper limit
    planet.mass = massDensity * (upperLimitAu-lowerLimitAu);
    // standard gravitational parameter (G*M)
    planet.mu = G * planet.mass;

    // calculate planet temperature
    planet.temperature = planetTemperature(star.luminosity, planetDistanceAu);
    float deviation = rng.nextFloat() * 50.0f;
    planet.equatorTemperature = planet.temperature + deviation;
    deviation = rng.nextFloat() * 50.0f;
    planet.poleTemperature = planet.temperature - deviation;

    //debug output
    //cout << "  planet " << planet.seed << "\n";
    //cout << "    frost limit [au] = " << star.frostLimitAu << "\n";
    //cout << "    lowerLimitAu = " << lowerLimitAu << "\n";
    //cout << "    planetDistanceAu = " << planetDistanceAu << "\n";
    //cout << "    upperLimitAu = " << upperLimitAu << "\n";
    //cout << "    planetTemperature [K] = " << planet.temperature << "\n";
    //cout << "    massDensity [kg*au^⁻1] = " << massDensity << "\n";
    //cout << "    mass [kg] = " << planet.mass << "\n";
    //cout << "    mass [Mearth] = " << planet.mass/Mearth << "\n";

    // update limits for the next planet in loop
    lowerLimitAu = upperLimitAu;

    // generate planet periodic type
    planet.typeIndex = getPlanetTypeIndex(planet, star.hzDistAu[1], star.hzDistAu[5]);

    // generate planet radius in [km]
    planet.radius = Rearth_min[planet.typeIndex] + rng. nextFloat() * (Rearth_max[planet.typeIndex]-Rearth_min[planet.typeIndex]);
    planet.radius *= Rearth;

    // generate axial rotation (day)
    // planet day (duration of one axis rotation in [s]
    // TODO: can be improved by correlating planet type, mass, density, and radius
    planet.day = 2 * M_PI * planet.radius;

    // update orbital period (year) in [s]
    // we use a simple method, using the semi major axis in
    // [au], for orbital period P in []:
    // P = sqrt{a^3} where
    //   p, is the period in Earth years and a, is the semi
    //   major axis, in Astronomical Units.
    // TODO: can be improved by correlating planet type,
    // mass, density, and radius
    planet.year = sqrt(pow(planet.starDistance, 3)) * yearEarth;

    // TODO: generate atmosphere
    //planet.createAtmosphere(rng);
    planet.atmosphere = createAtmosphere(planet.typeIndex, planet.radius, rng);

    //TODO: enerate more planet parameters
    //TODO: generate moons

    return planet;
  }

  /**
   * Generates new planets in the system in accordance with the
   * nebular hypothesis.
   * Frost line (FL), where temperature is estimated to be about 150 K.
   * Small rocky planets form inward from the FL.
   * Larger gaseous planets form outward from the FL.
   * Rocky elements density ~4e24 kg/au, with
   * - min = 4e23 kg/au close to the sun (non-adhesion in melted state)
   * - max = 9e24 (accretion in semi-melted state)
   * - it follows a normal distribution between 0 au and the FL
   * Gas element density decreases exponentially
   * - from 2.5e26 at the frost line
   * - to 3e24 at 30 au
   * - it follows an inverse exponential function from the FL on
   */
  void genPlanets(uint64_t systemSeed, uint64_t starSeed) {
    //cout << "       generating planets : ";
    UniverseStar &star = systems[systemSeed].stars[starSeed];
    // set generator to star
    rng.seed(starSeed);

    // get planet seeds for this system
    vector<uint64_t> planetSeeds = getPlanetSeeds(starSeed, star.planetsCount);

    // init data for random planet position within the frost limit
    // TODO: how to assure that not too many small planets are generated?
    float lowerLimitAu = 0;
    //float upperLimitAu = 0;
    float planetDistanceAu = 0.0f;

    // generate all the planets for the star
    for(int i=0; i<(int)star.planetsCount; ++i) {

      // randomized planet distance from the star
      if (lowerLimitAu<star.frostLimitAu) {
          // inward of the frost limit
          planetDistanceAu = lowerLimitAu + 0.1f + rng.nextFloat() * (star.frostLimitAu - lowerLimitAu);
      } else  {
          // outward of the frost limit
          // max distance between planets
          // TODO: take into expression the possible system size based on the star mass
          planetDistanceAu *= 1.5f + rng.nextFloat();
          if (planetDistanceAu<=lowerLimitAu) {
              planetDistanceAu += lowerLimitAu;
          }
      }

      // refactored planet generation
      // generate the planet data
      UniversePlanet planet = genPlanet(planetSeeds[i], star, planetDistanceAu, lowerLimitAu);

      // finally add the planet to the star at the end
      // since it will be a copy of the local planet object
      systems[systemSeed].stars[starSeed].planets[planetSeeds[i]] = planet;

      // update star record in system
      //systems[systemSeed].stars[starSeed] = star;

    } // loop planet
    //cout << "DEBUG: planets in system = " << star.planets.size() << "\n";

  } // end genPlanets function

}; // end class ProcUGalaxy


//-----------------------------------
// libProcU Universe serialization
//-----------------------------------

/**
 * @brief Universe serialization must define
 * to_json and from_json functions for all
 * universe data structures.
 * TODO: extend with serialization for system objects.
 */

// de-/serializer for Universe Planet

/**
 * @brief JSON serializer for universe star
 */
void to_json(json& j, const UniversePlanet& planet) {
    j = json{
      {"seed", planet.seed},
      {"type", planet.typeIndex},
      {"mass", planet.mass},
      {"temperature", planet.temperature}
    };
}

/**
 * @brief JSON deserializer for universe star
 */
void from_json(const json& j, UniversePlanet& planet) {
    j.at("seed").get_to(planet.name);
    j.at("type").get_to(planet.typeIndex);
    j.at("mass").get_to(planet.mass);
    j.at("temperature").get_to(planet.temperature);
}

// de-/serializer for Universe Star

/**
 * @brief JSON serializer for universe star
 */
void to_json(json& j, const UniverseStar& star) {
    j = json{{"seed", star.seed}, {"type", star.typeIndex}, {"mass", star.mass}};

    // serialize planets
    json jPlanet;
    for (auto& [planetSeed, planet] : star.planets) {
      jPlanet.push_back(planet);
    }
    if (star.planetsCount>0) {
      j["planets"] = jPlanet;
    }
}

/**
 * @brief JSON deserializer for universe star
 */
void from_json(const json& j, UniverseStar& star) {
    j.at("seed").get_to(star.name);
    j.at("type").get_to(star.typeIndex);
    j.at("mass").get_to(star.mass);
}

// de-/serializer for Universe System

/**
 * @brief JSON serializer for universe system
 */
void to_json(json& j, const UniverseSystem& system) {
    //j = json{{"stars", system.stars}}; // cannot serialize maps directly
    j = json{{"sector", system.sector}, {"seed", system.seed}, {"position", system.position},
      {"multiplicity", system.multiplicity}};

    // The following insert a duplicated key record
    // which is the star seed... We keep it for archival purpose
    //json jStars(system.stars);
    //j["stars"] = jStars;

    // Therefore we prefer this iterative method
    json jStar;
    for (auto& [starSeed, star] : system.stars) {
      jStar.push_back(star);
    }
    j["stars"] = jStar;
}

/**
 * @brief JSON deserializer for universe system
 */
void from_json(const json& j, UniverseSystem& system) {
    j.at("seed").get_to(system.seed);
    j.at("position").get_to(system.position);
    //j.at("stars").get_to(system.stars);
}

// de-/serializer for Universe Sector

/**
 * @brief JSON serializer for universe sector
 */
void to_json(json& j, const UniverseSector& sector) {
    j = json{{"seed", sector.seed}, {"position", sector.position},
      {"name", sector.name}, {"systems", sector.systemSeeds}};
}

/**
 * @brief JSON deserializer for universe sector
 */
void from_json(const json& j, UniverseSector& sector) {
    j.at("seed").get_to(sector.seed);
}

// save/load functions for Universe Sector

/**
 * @brief Serializes and saves universe data
 * TODO: add filename as param
 * TODO: add other objects
 */
void saveGalaxy(ProcUGalaxy &galaxy) {
  json data;

  // serialize from object to json
  data["galaxy"]["seed"] = galaxy.galaxySeed;

  // write prettified JSON to file
  std::ofstream outFile("galaxy.json");
  outFile << std::setw(2) << data << std::endl;
}

/**
 * @brief Loads and deserializes universe data
 * TODO: add filename as param
 * TODO: add other objects
 */
void loadGalaxy(ProcUGalaxy &galaxy) {
  json data;

  // read a JSON file
  std::ifstream inFile("galaxy.json");
  inFile >> data;

  // deserialize from json to object
  galaxy.galaxySeed = data["galaxy"]["seed"];

}


} // end namespace

#endif // end LIBPROCU_GALAXY_H header guards

