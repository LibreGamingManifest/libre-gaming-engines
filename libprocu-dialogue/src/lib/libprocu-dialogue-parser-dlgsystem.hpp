//===================================
// @file   : libprocu-dialogue.hpp
// @version: 2020-05-24
// @created: 2020-05-23
// @author : pyramid
// @brief  : library for player dialogue
//===================================



//===================================
// Documentation
//===================================

/**
 * @brief ProcUDialogue Documentation\n
 * A dialogue system for games.
 * In future potentially a procedural dialogue generator.
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
// for holding seeds, colors
//#include <vector>
// for ordered collections
//#include <list>
// for holding unordered hierarchical objects
//#include <map>
// include for pristine random seed
//#include <random>
// performance measurement
// time, sleep, milliseconds
//#include <chrono>
// file in/output
#include <fstream>


//-----------------------------------
// includes: external libraries
//-----------------------------------

// include pcg random library
#include <pcg32.h>
// include json library, source: https://github.com/nlohmann/json
#include <json.hpp>


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

//-----------------------------------
// constants
//-----------------------------------

//===================================
// data structures
//===================================
/**
 * source
 * https://stackoverflow.com/a/9899621
 */
  
struct DlgCondition {
  std::string empty;
};

struct DlgTextArgument {
  std::string empty;
};

/**
 * @brief ...
 */
struct DlgChild {
  int TargetIndex;
  std::vector<DlgCondition> Conditions;
  std::string SpeakerState;
  bool bIncludeInAllOptionListIfUnsatisfied;
  std::string Text;
  std::vector<DlgTextArgument> TextArguments;
}; // end struct

struct DlgEnterEvent {
};

struct DlgEnterCondition {
  std::string Strength;
  std::string ConditionType;
  std::string ParticipantName;
  std::string CallbackName;
  std::string Operation;
  std::string CompareType;
  std::string OtherParticipantName;
  std::string OtherVariableName;
  int IntValue;
  float FloatValue;
  std::string NameValue;
  bool bBoolValue;
  bool bLongTermMemory;
  std::string CustomCondition;
};

struct DlgNode {
  int __index__;
  std::string __type__;
  std::string Text;
  std::string SpeakerState;
  std::string NodeData;
  std::string VoiceSoundWave;
  std::string VoiceDialogueWave;
  std::string GenericData;
  std::string OwnerName;
  bool bCheckChildrenOnEvaluation;
  std::vector<DlgTextArgument> TextArguments;
  std::vector<DlgChild> Children;
  // optional (check with .has_value() or if(var))
  // get value with .value()
  std::optional<bool> bIsVirtualParent;
};

struct DlgParticipantClass {
  std::string ParticipantName;
  std::string ParticipantClass;
};

struct Dialogue {
  std::string __type__;
  int DlgVersion;
  std::string DlgName;
  std::string DlgGuid;
  std::vector<DlgParticipantClass> DlgParticipantClasses;
  std::vector<DlgNode> Nodes;
};


//===================================
// data functions
//===================================

//-----------------------------------
// operators
//-----------------------------------

std::ostream &operator<<(std::ostream &os, procu::Dialogue dlg) {
  os << dlg.__type__ << " " << dlg.DlgVersion << " " << dlg.DlgName << " " << dlg.DlgGuid << "\n";
  return os;
  
}



//===================================
// json serialization
//===================================

//-----------------------------------
// json serialization
//-----------------------------------

/**
 * The nlohmann::json library supports all standard c++
 * variables and containery. Since we have our own structs
 * defined, we must also provide serialization functions
 * for them.
 * @TODO for convenience this is bundled into the one
 * header library, but could be separated into
 * >>> libprocu-dialogue-json
 */


/**
 * @brief JSON deserializers
 */
void from_json(const json& j, DlgCondition& condition) {
}
void from_json(const json& j, DlgTextArgument& textArgument) {
}

void from_json(const json& j, DlgNode& node) {
  node.__index__ = j["__index__"];
  node.__type__ = j["__type__"];
  //cout << j["bIsVirtualParent"] << "\n";
  //try { node.bIsVirtualParent = j["bIsVirtualParent"]; } catch(...){}
  //if (j.find("bIsVirtualParent") != node.end()) {
  //  cout << " element found\n";
  //};
  if (j.contains("bIsVirtualParent")) {
    node.bIsVirtualParent = j["bIsVirtualParent"];
  };

  //node.Text = j["Text"];
  //node.SpeakerState = j["SpeakerState"];
  //node.NodeData = j["NodeData"]; // null results in type error
  /*node.VoiceSoundWave = j["VoiceSoundWave"];
  node.VoiceDialogueWave = j["VoiceDialogueWave"];
  node.GenericData = j["GenericData"];
  node.OwnerName = j["OwnerName"];
  node.bCheckChildrenOnEvaluation = j["bCheckChildrenOnEvaluation"];
  */
}

void from_json(const json& j, procu::DlgParticipantClass &ParticipantClass) {
  ParticipantClass.ParticipantName = j["ParticipantName"];
  ParticipantClass.ParticipantClass = j["ParticipantClass"];
}

//void from_json(const json& j, procu::DlgParticipantClasses &ParticipantClasses) {
//  ParticipantClasses.ParticipantClasses = j["DlgParticipantClasses"];
//}

void from_json(const json& j, procu::Dialogue &dialogue) {
  dialogue.__type__ = j["__type__"];
  dialogue.DlgVersion = j["DlgVersion"];
  dialogue.DlgName = j["DlgName"];
  dialogue.DlgGuid = j["DlgGuid"];
  // need to use j.at().get_to for structs
  j.at("DlgParticipantClasses").get_to(dialogue.DlgParticipantClasses);
  j.at("Nodes").get_to(dialogue.Nodes);
  //j.at("StartNode").get_to(dialogue.Nodes);
}


// DlgChild
// DlgEnterEvent
// DlgEnterCondition


//===================================
// json serialization
//===================================


/**
 * @brief Loads dialogue data from json
 * @param dialogue struct that will be loaded
 * @return json data
 */
json loadDialogue(Dialogue &dialogue) {
  json data;

  // read a JSON file
  std::ifstream inFile("data/dialogue.json");
  inFile >> data;
  //cout << data << "\n";

  // deserialize from json to object
  //dialogue = data.get<procu::Dialogue>();
  dialogue = data;
  //dialogue.__type__ = data["__type__"];

  // deserialize from json to object
  //json dataFiletered = data["StartNode"]["Children"];
  //cout << dataFiletered;
  //return dataFiletered;
  return data;

}


/**
 * @brief Loads and deserializes universe data
 * TODO: add filename as param
 * TODO: add other objects
 */
void loadDialogueChildren(std::vector<procu::DlgChild> &children) {
  json data;

  // read a JSON file
  std::ifstream inFile("data/dialogue.json");
  inFile >> data;

  // deserialize from json to object
  //children = data["StartNode"]["Children"];

  // deserialize from json to object
  //json dataFiletered = data["StartNode"]["Children"];
  //cout << dataFiletered;
  //return dataFiletered;

}



//-----------------------------------

} // end namespace

#endif // end LIBPROCU_DIALOGUE_H header guards

