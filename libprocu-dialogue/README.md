```markdown
# @file   : README.md
# @version: 2020-05-29
# @created: 2020-05-23
# @author : pyramid
# @purpose: readme for libprocu-dialogue
```

------

# **A Procedural Dialogue Library**
## **LibProcU :: Dialogue** 
### ***libprocu-dialogue***

------



# Game Dialogue System

**LibProcU::Dialogue** is a single-header library for game dialogues that is composed of

- a standard specification document
- standard data structure library
- dialogue system with functions to manipulate the data
- JSON serialization and deserialization
- File persistence



Full library documentation is available in the [***LibProcU :: Dialogue Documentation***](doc/libprocu-dialogue-documentation.md).

Full specification is available in the [**Dialogue Exchange System Specification**](doc/dialogue-exchange-system-specification.md) document.

Sample application code is available in [samples](samples).



------



# Dialogue Sample Application

A short code can produce a simple branching and recursive dialogue for the following simple conversation (see [*samples/demo1*](samples/demo1) for source code and data).

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

This was implemented using just a few lines of code:

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

The full code is available in [*samples/demo1/src/dialogue.cpp*](samples/demo1/src/dialogue-demo1.cpp) together with the data in [*samples/demo1/data/dialogue-demo1.json*](samples/demo1/data/dialogue-demo1.json). See the demo [readme](samples/demo1/README.md) for compile instructions.



Full library documentation is available in the [***LibProcU :: Dialogue Documentation***](doc/libprocu-dialogue-documentation.md).

Full specification is available in the [**Dialogue Exchange System Specification**](doc/dialogue-exchange-system-specification.md) document.



------



# Standard Dialogue Exchange System

The files in this repository establish a **standard dialogue exchange system** that consists of a **specification** and **implementation**.

The system is extensible and can be easily customized to cater for cases that are not (or not yet) considered in the specification and implementation.

Full specification is available in the [**Dialogue Exchange System Specification**](doc/dialogue-exchange-system-specification.md) document.

Full library documentation is available in the [***LibProcU :: Dialogue Documentation***](doc/libprocu-dialogue-documentation.md).

Sample application code is available in [samples](samples).



------

```EOF```