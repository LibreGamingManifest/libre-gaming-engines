//===================================
// @file   : dialogue.cpp
// @version: see VERSION
// @created: 2020-05-23
// @author : pyramid
// @brief  : cli mini dialogue
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
//#include <entt.hpp>
// for pristine random seed
//#include <random>
// pcg random library
//#include <pcg32.h>
// json serialization
#include <json.hpp>

// project includes
//#include "lib/libprocu-character.hpp"
#include "lib/libprocu-dialogue.hpp"


//-----------------------------------
// using namespaces
//-----------------------------------

using namespace std;
using namespace procu;


//-----------------------------------
// constants
//-----------------------------------

void mainLoopDemo1() {
  procu::DialogueNode currNode;
  std::string nextNodeId;

  // load standard dialogue data
  procu::Dialogue dlg;
  json data = procu::loadDialogue("data/dialogue-demo1.json", dlg);
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
    for (auto &obj : currNode.components) {
      //cout << "  component type: " << obj.type << std::endl;

      // this is a generic component executor
      // you can build in text into every component,
      // but don't necessarily need to
      // when the text element is not present, this
      // function will simply do nothing
      execComponentText(obj);

      // process the special components, here text only
      if (obj.type=="component-text") {
        // you can just make the text specific to one
        // particular component and execute it here
        // instead of in the generic way as above
        //execComponentText(obj);
      }

      // process the text selector
      if (obj.type=="component-selection") {
        execComponentSelection(obj);

        // sort items by sequence, if not sorted on load
        // or in the function above
        /*
        sortItems(obj);
        for (auto& obj : obj.items.value()) { // debug
          cout << obj.sequence << ": " << obj.text << endl;
        }
        */

        // get input
        uint iInput;
        cout << "your selection: ";
        cin >> iInput;
        cout << "-----" << endl;
        //cout << "you chose: " << iInput << endl;

        // get the next node id from selected sequence
        // get the next node id and catch the data error
        // in case you forgot to include it but need it
        try { nextNodeId = getNextNodeIdFromSequence(obj, iInput); } catch(string e) {
          cout << e << std::endl; return; }
        //cout << "nextNodeId: " << nextNodeId << endl;

      } // selection

      if (obj.type=="component-next") {
        // you can build in the text but don't need to
        //execComponentText(obj);

        //try { nextNodeId = getNextNodeIdFromComponent(obj); } catch(string e) {
        //  cout << e << std::endl; return; }
      }

    // next node can come from any component
    // get the next node id and don't catch the data error
    // try removing "next-node-id" from "component-next"
    // it will segfault, but if you set
    // procu::LIBDIALOGUE_COUT_ERROR = true;
    // it may still show you the error before the
    // uncaught segfault
    if(!obj.nextNodeId) { continue; }
    nextNodeId = getNextNodeIdFromComponent(obj);
    //try { nextNodeId = getNextNodeIdFromComponent(obj); } catch(string e) {
    //  cout << e << std::endl; return; }

    //cout << "nextNodeId: " << nextNodeId << endl;

    } // components


    // exit if we have hit the exit node
    // this must come before setting a new
    // next node becasue we still want to process
    // the components of the exit node
    if(currNode.type=="node-exit") {
      bRun = false;
      //cout << "found exit node (id=" + nextNodeId + "). exiting dialogue." << endl;
    }

    // if not get next node and continue
    currNode = getNodeById(dlg, nextNodeId);

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
  mainLoopDemo1();
  cout << "program exited." << endl;

  return 0;
} // end main
 
