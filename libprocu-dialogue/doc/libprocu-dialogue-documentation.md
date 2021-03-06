```markdown
# @file   : libprocu-dialogue-documentation.md
# @version: 2020-06-03
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
- [Features](#features)
- [Examples](#Examples)
- [Configuration](#configuration)
- [Documentation](#documentation)
- [Appendices](#appendices)



**Introduction**
=====================================

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

Such a standard open format had yet to be established at the time of initiation of this project.

We are aiming at providing a library with the **standard data model** as outlined in the [**Dialogue Standard Exchange Format**](#dialogue-standard-exchange-format). This is a community work in progress and part of the [**Libre Gaming Manifest**](https://github.com/LibreGamingManifest), a larger effort aiming at enabling creative and community oriented collaborators to create interesting games by providing the necessary libre tools to do so.

This library is in development whenever a community member requires change of existing functionality. Some standard functions are implemented so that you are able to quickly implement dialogue into your application. Simple [**sample applications**](#sample-applications) are bundled with the library code.

When writing the code we are trying to avoid convoluted (if high-performant) code in favor of understandable code. Code lines are usually generously commented so that the learning curve effort is reduced to a bare minimum.



# Features

Features are listed separately for standardized features (have standard documented), supported features (have a library implementation), and feature requests (not yet incorporated into standard and library).

For detailed description of the features, please read the  [**Standard Dialogue Exchange System Specification**](https://github.com/LibreGamingManifest/libre-gaming-engines/blob/master/libprocu-dialogue/doc/dialogue-exchange-system-specification.md). 



### Implemented Features

- Engine agnostic international [**Standard Dialogue Exchange System Specification**](https://github.com/LibreGamingManifest/libre-gaming-engines/blob/master/libprocu-dialogue/doc/dialogue-exchange-system-specification.md).
- Fully **open** source (FDL for specification, GPL for code).
- Well-**documented** (specification document, library documentation, source code comments, commented samples) (run *sh/doxy* to generate *doc/libprocu-dialogue-api.pdf*.
- Dialogue source is **self-explanatory** due to use of key-value pairs that carry meaning and content together, though extensive documentation is also available.
- A simple one-header C++ **library** (*libprocu-dialogue*).
- **Branched** conversations (next node) with multiple choices (*component-selection*).
- **Non-linear** conversations, such as looping backwards in the conversation to a previous dialogue node (*next-node-id*).
- Common node **types**: start (*node-init*), end (*node-exit*), normal connected node (*node*) .
- Associate nodes to **actors** (with the *actor* element). As such multiple actor conversations are supported in one dialogue structure.
- **Random** components (*component-random*) that will randomly select the next node.
- **Chaining** of long phrases one after another.
- Selection **hints** (*hint*) to show different (shortened) text/media than the full one in the reply.
- Media **decorators** (*decorator*) to show the mood or influence of the choice when selected.
- ***Mood*** hints for voice acting or visual indication of such either through the user interface, heads-up display, or character animation.
- Dialogue **media** file elements (*text*, *image*, *sprite*, *audio*, *speech*, *sound*, *media*).
- **Multiple** synchronous dialogue media where additional media files can be used to decorate choices and dialogues (e.g. hover image).
- **Camera** control information (*camera*) can be stored per item.
- Passing **arguments** to dialogue to create dynamic conversations like an non-player character greeting the player by name.
- Variable definition of **parameter** **parsing prefix** (*variable-init*) and **suffix** (*variable-end*) to easily adapt to any converted syntax.
- Library support for release mode and **experimental** mode (via library options, e.g. *LIBDIALOGUE_COUT_ERROR*).



### Specified Features

Features specified in the standard but not yet implemented in the *libprocu* library. Those features are in addition to the ones that are  listed above as implemented, because all implemented features are specified.

- **Localization** support (with the *language* element).
- Text **styling** (*text-style*) format definition to allow your application extract and apply styles, for example text color, text formatting, any other styling required by the implementation.
- Node **groups** to create for example question groups (*group*).
- Nodes can **execute** (*execute*) arbitrary code, such as giving you an item or ability or performing any other action.
- Dialogue nodes can **store** additional arbitrary *data* allowing you to customize behavior.
- Nodes can define **actions** (*action*) to be performed with the node. Allows defining on-start, on-finish node actions.
- **Animation** decorators (*animate*, *emote*, *express*).
- **Conditions** (*condition*) are stored in nodes and can be checked by the game engine code.
- Nodes hold information on ***events*** and ***notifications*** that may be sent during node processing.
- **Script** component to define script functionality. Scripting functions can be used for example to determine the availability of responses.
- Scripted conditions (variable, named, scripted) for conditional flows in conversations. Allows to implements a way for NPCs to have different responses based on  different conditions, allowing them to greet you differently or say different things.



### Client Application Owned Features

Features that are not standardized in the specification (and may not be unless there is a good reason to do so) and should be rather implemented in client applications.

- Possibility for **breaking off** an unfinished conversation must be implemented by the game engine client as the library does not implement dialogue flow control.
- Dialogues should not store **rewards** or **achievements** but should be able to send out notifications or events (specified with the *event* and *notification* elements) so that the game engine can use those notifications to perform reward or achievement related functions.
- Neither the specification nor the code library is concerned with the presentation layer as this one will always be very specific to the game you are implementing, so your application will have to customize the **look** and **interaction** behavior of the dialogue however you like on the game client side.
- **Text splitting** between multiple boxes in case a long text is not fitting the given dialogue frame shall be performed by the game client application as the library does not provide a presentation layer functionality. The standard however supports multiple text based items or components that can be read out together for each node and processed accordingly for correct display and stepping forward through text.



### Feature Requests

- Custom callbacks, events, or notifications (example is to trigger achievements).
- Variables with scope, either local to a single script or global between all scripts (e.g. if node has been visited 2 times give a different response).
- Possibility to convert from other formats (Twine, Ink, DlgSystem, ...).
- Inventory application to list actors, dialogues, moods as a script for voice acting.
- A dialogue node editor.





# Examples

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



The full code for the above example is available in [*samples/demo1/src/dialogue-demo1.cpp*](../samples/demo1/src/dialogue-demo1.cpp)  together with [*samples/demo1/data/dialogue-demo1.json*](../samples/demo1/data/dialogue-demo1.json).

The length and complexity of your dialogue now mainly depends on the number of included nodes.





## Sample Applications

Sample demos can be run by compiling the dialogue app and executing the demo number

```bash
> sh/make
> bin/dialogue --demo 1
```

Help will show you the available demos

```bash
> bin/dialogue --help
--- dialogue | v0.202006.03 ---
--- usage:
  -h --help         : show this help
  -d --demo uint    : run defined demo
          --demo 1  : simple branching dialogue with choices
          --demo 2  : random node (last of choice 3), show actor, repeat selected text
          --demo 3  : display hints and decorators
          --demo 4  : variable substitution to greet player (you are Deucalion)
```





# Configuration

The library behavior can be altered by changing the following configuration values in your C++ code.



| type option = default value           | description                                                  |
| ------------------------------------- | ------------------------------------------------------------ |
| bool LIBDIALOGUE_SORT_SEQUENCE = true | Items will be sorted automatically by sequence when imported. This should avoid an additional step if the order of selections is important. |
| uint LIBDIALOGUE_JSON_INDENT = 4      | Default indent in space characters.                          |
| bool LIBDIALOGUE_COUT_ERROR = false   | We can show the error text before throwing the error, so if you don't catch it and segfault, you at least know where. |



A short code example: 

```cpp
#include "lib/libprocu-dialogue.hpp"
using namespace procu;
LIBDIALOGUE_COUT_ERROR = true;
```





# Documentation



Run the *sh/doxy* script from the library root folder to generate the API documentation.

```bash
> sh/doxy
```



This will generate

- [*doc/pdf/libprocu-dialogue-api.pdf*](pdf/libprocu-dialogue-api.pdf)
- [html documentation](html/index.html)



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

The command line tool source code is under *src/dialogue.cpp*

There are a few handy scripts included:

- *sh/make* - simplified gcc compile script for the command line generator
- *sh/doxy*    - api documentation create script
- *sh/clean*   - cleans all complied and generated files



**Documentation License**
-------------------------------------

This document is published under the [GNU Free Documentation License (FDL)](http://www.gnu.org/licenses/fdl-1.3.html) ([http://www.gnu.org/licenses/fdl-1.3.html](http://www.gnu.org/licenses/fdl-1.3.html)).


Besides this documentation file, all the library code is well commented and the *libprocu-dialogue.hpp* file contains a documentation section at the beginning of the file.





**Code License**
-------------------------------------

The source code of this library is published under  [The GNU General Public License version 3 or later](https://www.gnu.org/licenses/gpl.html)([https://www.gnu.org/licenses/gpl.html](https://www.gnu.org/licenses/gpl.html)).





```EOF```
