//===================================
// @file   : achi.cpp
// @version: see VERSION
// @created: 2020-05-23
// @author : pyramid
// @brief  : cli mini game
//===================================



#include <string> // standard library
std::string VERSION = "0.202005.28";


//-----------------------------------
// libraries headers
//-----------------------------------

// standard libraries
#include <iostream>
//#include <string.h>
//#include <sstream>
// setw
#include <iomanip>
// entity component systems
#include <entt.hpp>
// for pristine random seed
//#include <random>
// pcg random library
#include <pcg32.h>
// json serialization
#include <json.hpp>

// project includes
#include "lib/libprocu-character.hpp"
#include "lib/libprocu-dialogue.hpp"


//-----------------------------------
// using namespaces
//-----------------------------------

using namespace std;
using namespace procu;
using json = nlohmann::json;


//-----------------------------------
// constants
//-----------------------------------

const std::list<std::string> places = {
  "Valley", "City", "Province", "Field"
};

const std::list<std::string> npcs = {
  "Technician", "Philosopher", "Mathematician", "Teacher"
};


/**
 * @brief Companion character identifier component.
 */
struct Location {
  std::string name = "";
};

void loadPrintDialogue() {
  procu::Dialogue dlg;
  json data = procu::loadDialogue("data/dialogue.json", dlg);
  procu::printDialogue(dlg);
}


void mainLoop() {
  procu::DialogueNode currNode;
  std::string nextNodeId;
  
  procu::Dialogue dlg;
  json data = procu::loadDialogue("data/dialogue.json", dlg);
  //procu::printDialogue(dlg);

  // get the starting node
  try {
    currNode = getStartNode(dlg);
    //cout << currNode.type << " (address: " << &currNode << ")\n"; // debug
  } catch(string e) {
    cout << e << std::endl;
    return;
  }

  // loop thorugh nodes until exit node found
  bool bRun = true;
  while (bRun) {

    //get components and execute them all
    //cout << currNode.type << std::endl; // debug
    for (auto &obj : currNode.components) { // debug looping, no more
      //cout << "  component type: " << obj.type << std::endl;
      
      if (obj.type=="component-text") {
        execComponentText(obj);
      }
      if (obj.type=="component-selection") {
        execComponentSelection(obj);
        //std::map<int, std::pair<std::string, std::string>> sel;
        //sel = getComponentSelection(obj);
        //for (auto& [key, val] : sel) {
        //  cout << key << " " << val.first << " " << val.second << endl;
        //}

        // sort items by sequence
        /*
        sortItems(obj);
        for (auto& obj : obj.items.value()) {
          cout << obj.sequence << ": " << obj.text << endl;
        }*/

        // get input
        uint iInput;
        cout << "your selection: ";
        cin >> iInput;
        //cout << "you chose: " << iInput << endl;

        // get the next node
        nextNodeId = getNextNodeIdFromSequence(obj, iInput);
        //cout << "nextNodeId: " << nextNodeId << endl;

      } // if selection

    } // components

    // get next node and continue
    currNode = getNodeById(dlg, nextNodeId);

    // unless we have hit the exit node
    if(currNode.type=="node-exit") {
      bRun = false;
      cout << "found exit node (id=" + nextNodeId + "). exiting dialogue." << endl;
    }
  }// next node

}


//===================================
// main program
//===================================

int main(int argc, char **argv) {
  // seed number to use by default for this simulation
  uint64_t uSeed = 0;

  std::vector<std::string> args(argv, argv+argc);
  size_t pos = args[0].find("/", 0);
  size_t end = args[0].length();
  std::string progName = args[0].substr(pos+1,end+1-pos);
  cout << "--- " << progName << " | v" << VERSION << " ---\n";


  //---------------------------------
  // parse input parameters
  //---------------------------------

  for (size_t i = 1; i < args.size(); ++i) {
    if (args[i] == "-h" or args[i] == "--help") {
      cout << "--- usage:\n";
      cout << "  -h --help         : show this help\n";
      cout << "  -s --seed uint    : generate with defined seed\n";
      return 0;
    } else
    if (args[i] == "-s" or args[i] == "--seed") {
      uSeed = stoll(args[i+1]);
      cout << "param seed = 0x" << hex << setw(16) << setfill('0') << uSeed
        << dec << " (" << uSeed << ") ("<< sizeof(uSeed) << " bytes)\n";
    }
    if (args[i] == "-f" or args[i] == "--file") {
      string filename = args[i+1];
      cout << "filename: " + filename +  "\n";
    }
  }

  //---------------------------------
  // run program
  //---------------------------------

  //createLocations();
  //loadPrintDialogue();
  mainLoop();
  cout << "thank you and good bye." << endl;

  return 0;
} // end main
 
