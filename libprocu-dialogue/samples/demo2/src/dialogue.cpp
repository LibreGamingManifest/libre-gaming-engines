//===================================
// @file   : dialogue.cpp
// @version: see VERSION
// @created: 2020-05-23
// @author : pyramid
// @brief  : cli mini dialogue
//===================================



#include <string> // standard library
std::string VERSION = "0.202005.30";


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
#include "lib/libprocu-dialogue.hpp"


//-----------------------------------
// using namespaces
//-----------------------------------

using namespace std;
using namespace procu;


//-----------------------------------
// demo 1
//-----------------------------------

/**
 * @brief demo 1
 * This demo walks you through the simplest
 * dialogue implementation without any shininess
 */
void mainLoopDemo1() {
  procu::DialogueNode currNode;
  std::string nextNodeId;

  // load standard dialogue data
  procu::Dialogue dlg;
  json data = procu::loadDialogue("data/dialogue-demo1shortest.json", dlg);
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


//-----------------------------------
// demo 2
//-----------------------------------

/**
 * @brief demo 2
 * To understand the basic program flow, first follow
 * the comments in demo 1. Those will not be repeated
 * in this demo. Only new additional features will be
 * commented.
 * [[new]] features as compared to demo 1:
 * - show actor if available
 * - repeat the selected text
 * - process the random component
 */

void mainLoopDemo2() {
  procu::DialogueNode currNode;
  std::string nextNodeId;
  bool bRun = true;
  uint iInput;

  // load standard dialogue data
  procu::Dialogue dlg;
  json data = procu::loadDialogue("data/dialogue-demo2.json", dlg);

  // get the starting node
  try { currNode = getStartNode(dlg); } catch(string e) {
    cout << e << std::endl; return;
  }

  // loop thorugh nodes until exit node found
  while (bRun) {

    //get components and execute them all
    for (auto &obj : currNode.components) {

      // [[new]] show actor if available
      if(currNode.actor) {
        execComponentTextActor(obj, currNode.actor.value());
      } else {
        execComponentText(obj);
      }

      // process the text selector
      if (obj.type=="component-selection") {
        execComponentSelection(obj);
        // get input
        cout << "your selection: ";
        cin >> iInput;
        cout << "-----" << endl;
        // get the next node id from selected sequence
        try { nextNodeId = getNextNodeIdFromSequence(obj, iInput);
        } catch(string e) { cout << e << endl; return; }

        // [[new]] repeat the selected text
        DialogueItem item = getItemFromSequence(obj, iInput);
        if(currNode.actor) {
          if(item.text) {
            cout << "[" << currNode.actor.value() << "] " << item.text.value() << endl;
          }
        } else {
          if(item.text) {
            cout << item.text.value() << endl;
          }
        }
      } // selection

      // [[new]] process the random component
      if (obj.type=="component-random") {
        DialogueItem item = getRandomItem(obj);
        if(currNode.actor) {
          if(item.text) {
            cout << "[" << currNode.actor.value() << "] " << item.text.value() << endl;
          }
        } else {
          if(item.text) {
            cout << item.text.value() << endl;
          }
        }
        // next node can come from item
        if(item.nextNodeId) {
          nextNodeId = item.nextNodeId.value();
        }
      } // component

      // next node can come from any component
      if(!obj.nextNodeId) { continue; }
      nextNodeId = getNextNodeIdFromComponent(obj);

    } // components

    // exit if we have hit the exit node
    if(currNode.type=="node-exit") { bRun = false; }

    // if not get next node and continue
    currNode = getNodeById(dlg, nextNodeId);

  }// next node

}


//===================================
// main program
//===================================

int main(int argc, char **argv) {

  // demo number to start by default without input parameter
  uint16_t iDemo = 2;

  // get program arguments
  std::vector<std::string> args(argv, argv+argc);

  // print header
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
      cout << "  -d --demo uint    : run defined demo\n";
      cout << "          --demo 1  : simple branching dialogue with choices\n";
      return 0;
    } else
    if (args[i] == "-f" or args[i] == "--file") {
      string filename = args[i+1];
      cout << "filename: " + filename +  "\n";
    }
    if (args[i] == "-d" or args[i] == "--demo") {
      iDemo = (uint16_t)stoi(args[i+1]);
      //cout << "param demo = 0x" << hex << setw(4) << setfill('0') << iDemo
      //  << dec << " (" << iDemo << ") ("<< sizeof(iDemo) << " bytes)\n";
    }
  } // args

  //---------------------------------
  // run program
  //---------------------------------

  cout << "--- demo " << iDemo << "\n";
  if (iDemo==1) { mainLoopDemo1(); }
  if (iDemo==2) { mainLoopDemo2(); }


  cout << "\ndialogue program exited." << endl;

  return 0;
} // end main
 
