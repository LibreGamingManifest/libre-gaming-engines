//===================================
// @file   : libprocu-dialogue.hpp
// @version: 2020-05-29
// @created: 2020-05-23
// @author : pyramid
// @brief  : library for player dialogue
//===================================



//===================================
// Documentation
//===================================

/**
 * @brief ProcU::Dialogue Documentation\n
 * A dialogue system for games.
 * In future, potentially a procedural dialogue generator.
 * 
 * **Introduction**
 * 
 * **Configuring**
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
#ifndef LIBPROCU_DIALOGUE_H
#define LIBPROCU_DIALOGUE_H


//-----------------------------------
// includes: standard libraries
//-----------------------------------

// terminal output stream
#include <iostream>
// standard string
#include <string>
// display formatting (setw)
#include <iomanip>
// for holding contiguous data
#include <vector>
// for ordered collections
#include <list>
// for holding key ordered hierarchical objects
#include <map>
// file in/output
#include <fstream>
  //for typeof to work 
#include <typeinfo>
// include for pristine random seed
//#include <random>
// performance measurement
// time, sleep, milliseconds
//#include <chrono>


//-----------------------------------
// includes: external libraries
//-----------------------------------

// include pcg random library
//#include <pcg32.h>
// include json library, source: https://github.com/nlohmann/json
#include <json.hpp>
// entity component systems
//#include <entt.hpp>


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

//-----------------------------------
// lib configuration
//-----------------------------------

/**
  * @brief Library behavior configuration with default
  * options preset.
  * @TODO implement usage of following options.
  */

bool LIBDIALOGUE_SORT_SEQUENCE = true;
uint LIBDIALOGUE_JSON_INDENT = 4;

// Do you want quick prototyping?
// We can show the error text before throw
// Therefore before catch or segfault :)
bool LIBDIALOGUE_COUT_ERROR = false;


//-----------------------------------
// constants
//-----------------------------------

//===================================
// data structures
//===================================

/**
 * @brief ...
 */
struct DialogueItem {
  // optional entries
  std::optional<std::string> text;
  std::optional<std::string> nextNodeId;
  std::optional<uint> sequence;
}; // end struct

struct DialogueComponent {
  // mandatory entries
  std::string type;
  // optional entries
  std::optional<std::string> name;
  std::optional<std::string> comment;
  std::optional<std::string> uuid;
  std::optional<std::string> sequence;
  std::optional<std::string> text;
  std::optional<std::string> nextNodeId;
  std::optional<std::vector<DialogueItem>> items;
};

struct DialogueNode {
  // mandatory entries
  std::string id;
  std::string type;
  std::vector<DialogueComponent> components;
  // optional entries
  std::optional<std::string> name;
  std::optional<std::string> comment;
  std::optional<std::string> uuid;
  std::optional<std::string> actor;
};

struct Dialogue {
  // mandatory entries
  std::string id;
  std::string type;
  std::vector<DialogueNode> nodes;
  // optional entries
  std::optional<std::string> comment;
  std::optional<std::string> name;
  std::optional<std::string> version;
  std::optional<std::string> uuid;
};

struct Dialogues {
  std::vector<Dialogue> dialogue;
};


//===================================
// data functions
//===================================

//-----------------------------------
// output stream operators
//-----------------------------------

/**
 * @brief Dialogue output stream
 */
std::ostream& operator<<(std::ostream& ostr, const procu::Dialogue& dlg) {
  ostr << dlg.id << " " << dlg.type; 
  if(dlg.name) { ostr << " " << dlg.name.value(); }
  if(dlg.version) { ostr << " " << dlg.version.value(); }
  return ostr;
}

/**
 * @brief DialogueItem output stream
 */
std::ostream& operator<<(std::ostream& ostr, const procu::DialogueItem& dlg) {
  if(dlg.sequence) { ostr << dlg.sequence.value(); }
  if(dlg.text) { ostr << " " << dlg.text.value(); }
  if(dlg.nextNodeId) { ostr << " " << dlg.nextNodeId.value(); }
  return ostr;
}


template<typename T>
std::ostream& operator<<(std::ostream& ostr, const std::optional<T>& ent) {
  ostr << ent.value();
  return ostr;
}

/**
 * @TODO implement templated type copy assignment for elements
 */
/*template<typename T>
T& operator=(T& out, const std::optional<T>& ent) {
  out = ent.value();
  return out;
}*/


/**
 * @TODO Implement missing output streams
 */


//===================================
// component functions (generic)
//===================================

/**
 * Compares two DialogueItem according to sequence.
 */
bool compareItemSequence(DialogueItem i1, DialogueItem i2) {
    return (i1.sequence < i2.sequence);
}

/**
 * @brief Sorts items by sequence
 * @param obj input component
 * Component will be checked for items,
 * for more than one item. Then items will be sorted
 * ascending by sequence number.
 * Items without sequence will be sorted
 * at the beginning with an arbitrary order
 * followed by items with sequence in ascending order.
 */
void sortItems(DialogueComponent& obj) {

    // no use continuing when there are no items
    if(!obj.items) { return; }

    // [[deprecated]] no use continuing if there is no sequence entry
    //if (!(obj.items.value()[0].sequence)) { cout << "no sequence found" << endl; return; }
    //if (!(obj.items.value()[0].sequence)) { return; }

    // auto update sequence entry if none given
    uint i = 0;
    for (auto &obj : obj.items.value()) {
      ++i;
      // set arbitrary sequence antry if not given
      if (!obj.sequence) {
        obj.sequence = std::optional<uint>{i};
      }
      //cout << " sequence: " << obj << endl;
    }

    // no use continuing to sort if there is only one item
    //cout << "size: " << obj.items.value().size() << endl;
    if (obj.items.value().size()==1) { return; }

    // sort items by sequence
    //for (auto &obj : obj.items.value()) { cout << " before: " << obj << endl; }
    sort(obj.items.value().begin(), obj.items.value().end(), compareItemSequence);
    //for (auto &obj : obj.items.value()) { cout << " after: " << obj << endl; }

}


/**
 * @brief Searches for and returns an address to the start node.
 * @param dlg dialogue reference
 * @return node address
 */
DialogueNode& getStartNode(Dialogue& dlg) {
  // empty node if not found
  for (auto& obj : dlg.nodes) {
    if(obj.type=="node-init") {
      //cout << obj.type << " (address: " << &obj << ")\n";
      return obj;
    }
  }
  // in c++ references cannot be empty
  // this is really stupid as I dunno how to solve a no return value
  // in vase the node was not found
  //DialogueNode* node = new DialogueNode(); // dereference the pointer to the new memory
  //return *node;
  // or throw an error
  //throw 1; // catch(int e)
  // catch(string e)
  throw string("libprocu-dialogue/getStartNode: node-init not found in dlg parameter");
}

/**
 * @brief Searches for and returns the node given by id.
 * @param dlg dialogue reference
 * @param nid node id
 * @return node object
 */
DialogueNode& getNodeById(Dialogue& dlg, std::string nid) {
  // empty node if not found
  for (auto& obj : dlg.nodes) {
    if(obj.id==nid) {
      //cout << obj.type << "  id: " << obj.id << endl;
      return obj;
    }
  }
  throw string("libprocu-dialogue/getNodeById: node for id " + nid + " not found in dlg parameter");
}


/**
 * @brief Searches for and returns the id of the next node.
 * @param obj component with items reference
 * @return node id
 */
std::string getNextNodeIdFromSequence(const DialogueComponent& obj, const uint seq) {
  std::string nextNodeId = "";
  // loop through items
  for (auto &obj : obj.items.value()) {
    if(!obj.sequence or !obj.nextNodeId) { break; }
    // get node id by sequence if exists
    if(obj.sequence.value()==seq) {
      nextNodeId = obj.nextNodeId.value();
    }
  }
  // error checking
  if (nextNodeId=="") {
    std::string err = string("libprocu-dialogue/getNextNodeIdFromSequence: node for sequence " + to_string(seq) + " not found in component parameter");
    if (LIBDIALOGUE_COUT_ERROR) { cout << err << endl; }
    throw err;
  }
  return nextNodeId;
}

/**
 * @brief Searches for and returns the id of the next node.
 * @param obj component reference
 * @return node id
 */
std::string getNextNodeIdFromComponent(const DialogueComponent& obj) {
  // error checking if no next node exists
  if(!obj.nextNodeId) {
    std::string err = string("libprocu-dialogue/getNextNodeIdFromComponent: next node not found in component parameter");
    if (LIBDIALOGUE_COUT_ERROR) { cout << err << endl; }
    throw err;
    return "";
  }
  // if exists return the next node id
  std::string nextNodeId = obj.nextNodeId.value();
  return nextNodeId;
}


//===================================
// component functions (text based nodes)
//===================================


/**
 * @brief Executes component.
 * @param obj component reference
 */
void execComponentText(const DialogueComponent& obj) {
    // check for optional entry
    if(obj.text) { cout << obj.text.value() << std::endl; }
}

/**
 * @brief Executes component.
 * @param obj component reference
 */
void execComponentSelection(DialogueComponent& obj) {
    // check for optional item list entry
    if(!obj.items) { return; }

    // sort items by sequence
    sortItems(obj);

    // list items of the component
    // item in optional items list
    for (auto &obj : obj.items.value()) {
      cout << obj.sequence << ": " << obj.text << endl;
    }
}


//===================================
// presentation functions
//===================================

void printDialogue(Dialogue &dlg) {

  //cout << "(JSON) nodes:\n" << std::setw(2) << data["nodes"] << std::endl; // debug

  // list dialogue
  cout << "--- dialogue header: " << dlg << "\n";
  //this is the visualization of the data headers
  for (auto &obj : dlg.nodes) {
    cout << "node: " << obj.id << ", " << obj.type << "\n";

    // execute components of the node
    for (auto &obj : obj.components) {
      cout << "  component: type: " << obj.type;
      // optional entry
      if(obj.uuid) { cout << " | uuid: " << obj.uuid.value() << ")"; }
      cout << "\n";


      // list items of the component
      // double optional: optional item in optional list
      if(obj.items) {
        //cout << "has items\n";
        for (auto &itm : obj.items.value()) {
          cout << "    item: ";
          if(itm.nextNodeId) { cout << "next-id: " << itm.nextNodeId.value(); }
          if(itm.text) { cout << " | text: " << itm.text.value(); }
          cout << "\n";
        }
      } // items

    } // components
  } // nodes

}


//===================================
// json deserialization
//===================================


/**
 * The nlohmann::json library supports all standard c++
 * variables and containers. Since we have our own structs
 * defined, we must also provide serialization functions
 * for them.
 * @TODO for convenience this is bundled into the one
 * header library, but could be separated into
 * >>> libprocu-dialogue-json
 */

/**
 * @brief JSON deserializer
 */
void from_json(const json& j, DialogueItem &obj) {
  if (j.contains("text")) { obj.text = j["text"]; };
  if (j.contains("sequence")) { obj.sequence = j["sequence"]; };
  if (j.contains("next-node-id")) { obj.nextNodeId = j["next-node-id"]; };
}

/**
 * @brief JSON deserializer
 */
void from_json(const json& j, DialogueComponent &obj) {
  j.at("type").get_to(obj.type);
  // optional entries cannot be loaded with j.at().get_to()
  if (j.contains("comment")) { obj.comment = j["comment"]; };
  if (j.contains("name")) { obj.name = j["name"]; };
  if (j.contains("uuid")) { obj.uuid = j["uuid"]; };
  if (j.contains("text")) { obj.text = j["text"]; };
  if (j.contains("sequence")) { obj.sequence = j["sequence"]; };
  if (j.contains("items")) { obj.items = j["items"]; };
  if (j.contains("next-node-id")) { obj.nextNodeId = j["next-node-id"]; };
}

/**
 * @brief JSON deserializer
 */
void from_json(const json& j, DialogueNode &obj) {
  j.at("type").get_to(obj.type);
  j.at("id").get_to(obj.id);
  j.at("components").get_to(obj.components);
  // optional entries cannot be loaded with j.at().get_to()
  if (j.contains("comment")) { obj.comment = j["comment"]; };
  if (j.contains("name")) { obj.name = j["name"]; };
  if (j.contains("uuid")) { obj.uuid = j["uuid"]; };
  if (j.contains("actor")) { obj.actor = j["actor"]; };
}

/**
 * @brief JSON deserializer
 */
void from_json(const json& j, procu::Dialogue &obj) {
  j.at("type").get_to(obj.type);
  j.at("id").get_to(obj.id);
  j.at("nodes").get_to(obj.nodes);
  // optional entries cannot be loaded with j.at().get_to()
  if (j.contains("comment")) { obj.comment = j["comment"]; };
  if (j.contains("name")) { obj.name = j["name"]; };
  if (j.contains("version")) { obj.version = j["version"]; };
  if (j.contains("uuid")) { obj.uuid = j["uuid"]; };
}


//-----------------------------------
// json deserialization persistence loader
//-----------------------------------


/**
 * @brief Loads dialogue data from json
 * @TODO we should always return dialogue**S**
 * or how will we even know when processing?
 * Unless we do return an ECS repository already.
 * @param dialogue struct that will be loaded
 * @return json data
 */
json loadDialogue(std::string filename, Dialogue &dialogue) {
  json data;

  // read a JSON file
  std::ifstream inFile(filename);
  inFile >> data;

  // deserialize from json to dialogue object
  if (data.contains("dialogues")) {
    //cout << " found a dialogues list in file" << std::endl; // debug
    //cout << data["dialogues"][0] << std::endl; // debug
    dialogue = data["dialogues"][0];
  } else {
    //cout << " found a single dialogue in file" << std::endl; // debug
    //cout << data << std::endl; // debug
    dialogue = data; // if it is a single dialogue
  }

  //cout << data["dialogues"][0]["nodes"] << "\n"; // debug
  
/**
  * @TODO implement usage of following option.
  */
  if (LIBDIALOGUE_SORT_SEQUENCE) {
  }

  return data["dialogues"][0];

}


//===================================
// json serialization
//===================================


/**
 * The nlohmann::json library supports all standard c++
 * variables and containers. Since we have our own structs
 * defined, we must also provide serialization functions
 * for them.
 * @TODO for convenience this is bundled into the one
 * header library, but could be separated into
 * >>> libprocu-dialogue-json
 * @TODO write the serialization code
 * @TODO write the file persistence code
 */


//-----------------------------------

} // end namespace

#endif // end LIBPROCU_DIALOGUE_H header guards

