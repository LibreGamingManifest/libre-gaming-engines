```markdown
# @file   : libprocu-dialogue-documentation.md
# @version: 2020-05-29
# @created: 2020-05-23
# @author : pyramid
# @purpose: documentation for libprocu-dialogue
```



------

# **A Procedural Dialogue Library**

## **LibProcU :: Dialogue** 
### ***libprocu-dialogue***

------



**Table of Contents**
-------------------------------------

- [Introduction](#introduction)
- [Usage and Quick Guide](#Usage-and-Quick-Guide)
- [Appendices](#appendices)



**Introduction**
-------------------------------------

LibProcU stands for **Proc**edural **U**niverse **Lib**rary.

**LibProcU::Dialogue** is a single-header library for game dialogues that is composed of

- a standard specification document
- standard data structure library
- dialogue system with functions to manipulate the data
- JSON serialization and deserialization
- File persistence



In 2020, when I (pyramid, aka [pyramid3d](https://github.com/pyramid3d) was researching available open source technologies for game dialogue systems, nothing significant showed besides some dialogue editors (TreeLine, Yarn, Twine), and some data or markup formats (Ink, Twee, ClearDialogue JSON).

There was no standard and implementation that was open source, provided libraries, and was production proven. One system though, the [DlgSystem](https://gitlab.com/NotYetGames/DlgSystem) by [Not Yet Games](https://discord.gg/NotYet) had open source code available (albeit for an Unreal Engine plug-in), though the  serialized JSON data format looked seriously and production-ready to the extent that it seemed to be worth being inspired by it. My initial conversation with the developers that inspired this project is available in the [Appendix](#appendix-exchange-on-dlgsystem-standardization). Briefly after this conversation, I have started developing my own dialogue format standard as I found the one present in the DlgSystem to be very specific to the game type developed at that time.



**Conversation dialogue formats** must represent the conversation in such a way that it can be written, tested, debugged, and scripted. This means that in addition to text and the tree structure of the dialog object, it must include certain attributes that allow referencing audio, subtitles, and animation sequences, and other information required to build and use conversations comfortably by humans and machines.

Further, it should allow to contain not only implementation or usage specific information but also additional information like the position on screen for the editing tools or guidance on which mood to apply to a given conversation when delivering voice-over and acting, what are the conditions triggering the conversation, variables that allow the dialogue to reflect previous player choices, and many more features reflecting the requirements of modern gameplay.

@TODO Such a standard format has yet to be established. Should you have knowledge of such a format, please do not hesitate to contact the author.



We are aiming at providing a library with the **standard data model** as outlined in the [**Dialogue Standard Exchange Format**](#dialogue-standard-exchange-format). This is a community work in progress and part of the [**Libre Gaming Manifest**](https://github.com/LibreGamingManifest), a larger effort aiming at enabling creative and community oriented collaborators to create interesting games by providing the necessary libre tools to do so.

This library is in development whenever a community member requires change of existing functionality. Some standard functions are implemented so that you are able to quickly implement dialogue into your application. Simple [**sample applications***](#sample-applications) are bundled with the library code.

When writing the code we are trying to avoid high-performant code in favor of understandable code. Code lines are usually generously commented so that the learning curve effort is reduced to a bare minimum.



## Features

@TODO Features are currently only a collection of ideas and after initial basic development is done, should be separated into standardizes features (have standard documented), supported features (have a library implementation), and feature requests (not yet incorporated into standard and library).



### Feature requests

- Engine agnostic international standard format
- A sample C++ library
- Dynamic branched conversations with multiple choices.
- Complex conditions or logic without scripting
- Multiple actor conversations
- Triggers
- Conditions (variable, or named)
- Multiple synchronous dialogue media (text, image, audio, camera)
- Behavior trees
- Multiple selections
- Selection hints
- Mood hints (e.g. for voice acting)
- Dialogue media arguments

- Many node types: start, end, media, random nodes
- Questions and question groups
- Tasks (show text, wait for answer, move camera, close dialog, next node)
- Local and Global variables
- Custom event and event call nodes
- Retargetable Dialogue Graphs

- no localization
- associate dialogue to actor component
- camera information
- text color, formatting, styling per types or individual nodes
- media decorators
- animation decorators (tasks, emotes, expressions)
- hover image, text, audio, any media
- node groups
- tag to allow for waiting/pausing during dialogue
- Some sort of OnStart, OnFinish optional action
-  **variables with scope**, either local to a single script or global between all scripts
- Support for normal mode and Experimental Mode
- Allows non-linear conversations, such as looping backwards in the conversation to a previous dialogue node
- Allows conditional flow in conversations, giving you the power to script functions that determine the availability of responses
- Implements a way for NPCs to have different responses based on  different conditions, allowing them to greet you differently or say  different things
- Has a system to dynamically replace text in the dialogue by  utilizing a table of DynamicTextFunctions (which you can write your own  functions for). Now NPCs can greet you by name!
- Each dialogue node can execute arbitrary code upon being  encountered, such as giving you a tool or powerup, or teleporting you  somewhere else for a fee
- Each dialogue node can store arbitrary data which is passed into  DynamicTextFunctions, actions, and conditions, allowing you to customize behavior
- Has an abstract interface, allowing you to customize the look and behavior of the client-side however you like
- Comes with a default interface, redesigning a classic Roblox look and allowing you to jump start projects
- Allows NPC phrases to be chained into one another for such a purpose as a “next” button like in Zelda where text is split between multiple  boxes
- Fully open source and well-documented



## Configuration



| type option = default value           | description                                                  |
| ------------------------------------- | ------------------------------------------------------------ |
| bool LIBDIALOGUE_SORT_SEQUENCE = true | Items will be sorted automatically by sequence when imported. This should avoid an additional step if the order of selections is important. |
| uint LIBDIALOGUE_JSON_INDENT = 4      | Default indent in space characters.                          |
|                                       |                                                              |





## Examples

A simple branching and recursive dialogue can be implemented using just a few lines of code:

```cpp
  // load standard dialogue data
  json data = procu::loadDialogue("data/dialogue.json", dlg);

  // get the starting node
  currNode = getStartNode(dlg);

  // loop thorugh nodes until exit node found
  bool bRun = true;
  while (bRun) {

    //get components and execute them all
    for (auto &obj : currNode.components) {

      // display text from any component
      execComponentText(obj);

      // process the text selector
      if (obj.type=="component-selection") {
        execComponentSelection(obj);

        // get input
        uint iInput;
        cout << "your selection: ";
        cin >> iInput;

        // get the next node id from selected sequence
	    nextNodeId = getNextNodeIdFromSequence(obj, iInput);
      } // selection

      if (obj.type=="component-next") {
        nextNodeId = getNextNodeIdFromComponent(obj);
      }

    } // components


    // exit if exit node
    if(currNode.type=="node-exit") { bRun = false; }

    // if not get next node and continue
    currNode = getNodeById(dlg, nextNodeId);

  } // next node

```



This short code can produce the following simple conversation

```
You find yourself in the bar on the orbital station above Cephid 17 talking with Jenek, a ship dealer.
[Jenek] Ahh, that's right. You're here about the jump drive!
1: Depends ...
2: Yeah, I'm interested.
3: That's not what I'm here about.
your selection: 1
-----
I see ... Let me give you a very special price.
So you're interested in the jump drive now?
1: Depends ...
2: Yeah, I'm interested.
3: That's not what I'm here about.
your selection: 2
-----
Here you go. It's being installed as we speak.
Well, what are you waiting for? Go and give it a spin!
Thank you and good bye o/

```





The full code is available in [*src/dialogue.cpp*](src/dialogue.cpp) together with [*data/dialogue.json*](data/dialogue.json).

The complexity mainly depends on the number of nodes to be included.









## Sample Applications

Sample applications match program and data name, for example

- [*src/dialogue.cpp*](src/dialogue.cpp) together with [*data/dialogue.json*](data/dialogue.json)  implement a very simple branching dialogue with Q&A and different answer selections as well as recursive dialogue invocation.


------



# Appendices



**Appendix: Dependencies**
-------------------------------------

We are using the following libraries (or parts thereof).

For pseudo-random numbers, we use the [pcg random library](https://github.com/wjakob/pcg32).

JSON library for object serialization and deserialization we use [nlohmann::json](https://github.com/nlohmann/json).



**Appendix: File Structure**
-------------------------------------

The library code is in *src/lib/libprocu-dialogue.hpp*.

The command line tool source code is under *src/gendialogue.cpp*

There are some build scripts included:

- *sh/makelib* - simplified compile script for the shared library
- *sh/make* - simplified compile script for the command line generator
- *sh/doxy*    - api documentation create script
- *sh/clean*   - cleans all complied and generated files

You can also run *make* or *cmake* to build the demo code.



**Documentation License**
-------------------------------------

This document is published under the [GNU Free Documentation License (FDL)](http://www.gnu.org/licenses/fdl-1.3.html) ([http://www.gnu.org/licenses/fdl-1.3.html](http://www.gnu.org/licenses/fdl-1.3.html)).


Besides this documentation file, all the library code is well commented and the *libprocu-galaxy.hpp* file contains a documentation section at the beginning of the file.

The API is documented in *doc/libprocu-galaxy-api.pdf*. Run the *sh/doxy* script from the library root folder to generate it.



**Code License**
-------------------------------------

The source code of this library is published under  [The GNU General Public License version 3 or later](https://www.gnu.org/licenses/gpl.html)([https://www.gnu.org/licenses/gpl.html](https://www.gnu.org/licenses/gpl.html)).





```EOF```
