```markdown
# @file   : dialogue-exchange-system-specification.md
# @version: 2020-05-29
# @created: 2020-05-23
# @author : pyramid
# @purpose: specification document for libprocu-dialogue exchange system
```



------

# **Dialogue Exchange System Specification**

------



# Standard Dialogue Exchange System

Standard version: 1.202005.28

Author(s): pyramid



## Rationale

The specification standardizes those elements that enable human understanding but also the machine exchange of data for conversation dialogues. It was developed with focus on game dialogues but is probably not limited due to it's flexibility (easy to modify), adaptability (can be modified for a new purpose), and extensibility (easy addition of new capabilities).

At the time of its initiation (in the year 2020) there were several specialized systems available but no standard specification documentation bundled with libre code that was open source, provided data structure and functional libraries, and was production proven. This standard is freely available under an [open license](#documentation-license).

This standard incorporates the specific use cases that were considered and included at the time of writing of the present version. However, due to it's open nature and extensible structure, it is easy to incorporate your own cases (and even revert them to become a standard, too).

We accomplish this by using a hierarchical data structure with most of the data being, by nature, declared *optional* which allows for small and easy to read source data as well as completely customizable implementation of just the data you require for your specific application.

The reason for allowing optional entries is that the standard should support minimum requirements but should also allow for flexible use in both, use cases that rely on the standard but incorporate optional elements partially (for example in some nodes only), but also for use cases that were not considered but can be supported by the standards' flexible approach (extensibility) and might be incorporated in a future version.



## Nomenclature



The encoding of the serialized **keys** follows the *markdown-anchor* writing style with all lower cases and dashes between words. This is for the sake of [improved human readability](#why-the-markdown-anchor-key-style).

However, because most programming languages do no allow hyphens in variables, those keys will be using a *camelCase* in the implemented code (so: *next-node-id* in the serialized file will be *nextNodeId* in the code.)



**Standardized enumerators** (*node-init*, *component-text*, ...) follow the same *markdown-anchor* writing style with all lower cases and dashes between words.

It is advisable to adopt this style for your **custom extensions** during your own implementation as it would allow for effortless adoption in case those extensions become a future standard.

All non-standard value entries follow your application specific standard as it only depends on your implementation to correctly parse them (for example *"actor": "Johnny"* or *"actor":"johnny_resource_character_2ae76bc89"*).

Supported value formats are.

- list
- string
- integer
- boolean



## Basic Structure

The basic structure of the dialog system is a data structure that can hold dialogues that are build up of different nodes connected in a tree structure that can be traversed.

```
root
|--- DIALOGUE
     |--- dialogue-header
     |--- NODE
          |--- node-header
          |--- COMPONENT
```



There can be multiple dialogues in one data structure object (in above chart with capital letters).

A dialogue consists of a header and multiple node objects.

Each node object consists of a header and one or multiple component object entries.

The basic structure with dialogues, nodes, and components looks like this in JSON in its minimal form:

```json
{ "dialogues": [
    { "type": "dialogue",
      "id": "0",
      "name": "Minimal Dialogue",
      "version": "1.20200525",
      "uuid": "CA66C1494A5208F97F7B908C24F9A8A6",

      "nodes": [
        { "type": "node-init",
          "id": "1",

          "components": [
            { "type": "component-text",
              "text: "Hello traveller"
            }
          ]

        } ]
    } ]
}
```



There are **mandatory** and **optional** entries for all data structures, so that libraries for data manipulation and persistence must allow for optional structures to not be present and may enforce checks on mandatory data structures.



A note on the terminology:

- data between brackets "[]" and "{}" is called a **structure**
- one line of data is called a **parameter** and also an **entry**, or **element**
- an entry consists of a **key** - **value** pair

In the previous example this would be

```json
"components": [                 << (multiple) components structure
  {                             << one component structure
    "type": "component-text",	<< one component parameter entry/element
    "text":                     << the entry key
        "Hello traveller"       << the entry value
  }
]
```

We **recommend** adaptation of this terminology for improved discussion efficiency.



Many **structures** have the following **mandatory** keys:

| key        | description | type   |
| ---------- | ----------- | ------ |
| ```type``` | object type | string |



**Optional** generic entries that may be present in some structures:


| key            | description                                                  | type                              |
| -------------- | ------------------------------------------------------------ | --------------------------------- |
| ```id```       | per dialogue unique node identifier; not available in components | string                            |
| ```uuid```     | universally unique identifier might be useful for large projects | 128 bit hex<br/>as 32 char string |
| ```name```     | human readable name                                          | string                            |
| ```version```  | version number                                               | string                            |
| ```created```  | date and (optional) time                                     | ISO 8601 format string            |
| ```comment```  | human readable comment for the data structure                | string                            |
| ```sequence``` | used to sort elements or components                          | uint                              |



This generic data may be present in any of the objects or components, so that in some cases we won't repeat those entries for the definitions included further in this document.



### uuid

The universally unique identifier is a 32 character long string build from a 128 bit integer converted to a hexagonal number.

```json
"uuid": "CA66C1494A5208F97F7B908C24F9A8A6"
```



### type

Every object will have it's own type enumerator, but it will need one.

See further at Node and Component for standard type enumerators.

Some examples

```json
"type": "dialogue"
"type": "node-question"
"type": "component-speech"
```



### name

The name of your dialogue, node, or component is optional because the standard requires the type to be parsed into (or from) the correct implementation component. The name is a human readable entry that allows for easy association and recognition.



### version

Version standards are plenty. This entry being an optional one leaves the doors open for using your own standard for versioning.

We recommend however the following options:

```cpp
// Standard 1: VERSION.ISO8601-DATE.REVISION
// version change when breaking change
// minor, maintenance, build, revision encoded as date
// can be extended to include hour,minute,second
// as per the flexible ISO8601 machine readable formatting
"1.202005.25"

// Standard 2: vMAJOR.MINOR.MAINTENANCE | ISO8601-DATE
// human readable, version components of equal length
// includes the date into version string
"v0.00.01 | 2020-05-25"
```



### created

Is an [international ISO 8601 date interchange format](https://en.wikipedia.org/wiki/ISO_8601) string in human readable form (separated with "-" for date entries and ":" for time entries and with " " between date and time). It contains the fill date and optionally the time with second as the smallest granularity entry.

Both entries are allowed

```json
"date": "2020-05-25"
"date": "2020-05-25 17:52"
```



### comment

Comments can help recognize where a new structure in a serialized file begins or ends. They are not used for any processing. Example of a comment entry:

```json
"nodes": [
    { "comment": "___node___" }
]
```



### sequence

When the order of execution is important, the sequence element may be included, especially because the JSON specification does not enforce any writing or reading sequence for objects of equal hierarchy level:

```json
"items": [
  { "text": "Answer 2",
    "sequence": 2 },
  { "text": "Answer 1",
    "sequence": 1 },
  { "text": "Answer 3",
    "sequence": 3 }
]
```





## Dialogue Header

Mandatory header types enumeration:

| key        | enumerator | description                    |
| ---------- | ---------- | ------------------------------ |
| ```type``` | dialogue   | the main type of this standard |



Optional standard header types are currently not implemented in this standard, however you are free to define your own.



## Nodes

### Types

Header types enumeration:

| key        | enumerator  |
| ---------- | ----------- |
| ```type``` | node        |
| ```type``` | node-init   |
| ```type``` | node-exit   |
| ```type``` | node-random |
| ```type``` | node-global |




Optional data

| key         | description                                                  | type   |
| ----------- | ------------------------------------------------------------ | ------ |
| ```actor``` | entity name for the actor who performs the actions of this node, player or non-player. | string |
|             |                                                              |        |
|             |                                                              |        |



### Start Node

```@TODO``` Do we encode some behavior on node level, e.g. *node-init* or exit node or do we require a component for **all** behaviors? Or there a next-node-id in the basic dialogue parameters

```json
{ "id": "1",
  "type": "node-start",
  ...
}
```



### Exit Node

The exit node indicated that it is the last node and traversing the node tree is finished.

```json
{ "id": "99",
  "type": "node-exit"
}
```



### Normal Node

Normal nodes are those that have their directives encoded in components and do not require additional special node-level handling in the application.

This can be for example a node that is text only. It is up to the application to decide how to handle the pausing and advancing for large text.

```json
{ "id": "10",
  "type": "node"
}
```






### Random Node

The random node is used to make random decisions, for example on the next node to select. The presence of this node and several next-node-id items will indicate to the code that a random selection is in place

```json
{ "id": "10",
  "type": "node-random",
  "components": [
    { "type": "component-selection",
      "items": [
        { "next-node-id": "11" },
        { "next-node-id": "13" },
        { "next-node-id": "56" }
      ]
    }
  ]
}
```







## Components

Components are blocks that describe specific functionality, for example a multimedia component that describes interaction between text and sound, or an animation component that describes motion of the actor.

The component follows the same reasoning as the component of the [Entity Component System](https://en.wikipedia.org/wiki/Entity_component_system) in the sense that several components can be attached to a node, and they will be executed one-be-one. The main difference is that we do not allow processing of all nodes that have, for example, the component type *component-text* as this would not make sense in a tree traversal approach.

Interaction between components is however allowed but should be handled by the implementing application.

The basic structure of a component is

```json
"components": [
    { "type": "component-text",
      "text": "some text" }
```



Component *type*s of the dialogue system

| component type enumerator | description             |
| ------------------------- | ----------------------- |
| component-text            | text only               |
| component-selection       |                         |
| component-next            | stores the next node id |
| component-reward          |                         |







Optional data

| key                | description                                                  | type    |
| ------------------ | ------------------------------------------------------------ | ------- |
| ```sequence```     | sequence of the component to enforce e.g. text > selections > text | integer |
| ```media-type```   | [media type](https://en.wikipedia.org/wiki/Media_type) info  | string  |
| ```media```        | link to media file                                           | string  |
| ```text```         | direct text entry                                            | string  |
| ```items```        | a list of items for the component execution                  | string  |
| ```next-node-id``` | id of the next node to go                                    | string  |



### text

Text can contain escaped double quotes ```\"text\"```. It can also contain line breaks "\n".





## Items



## Examples



A simple looping conversation can be:

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



The data for this conversation is:

```json
{ "type": "dialogue",
  "id": "0",
  "name": "Sample Dialogue",
  "version": "1.202005.29",
  "nodes": [
    { "id": "1",
      "type": "node-init",
      "actor": "Narrator",
      "next-node-id": "end",
      "components": [
        { "type": "component-text",
          "text": "The Aera have invaded ..." },
        { "type": "component-text",
          "text": "You are Deucalion, ..." },
        { "type": "component-text",
          "text": "You find yourself i..." },
        { "type": "component-text",
          "text": "[Jenek] Ahh, that's right. You're here about the jump drive!",
          "next-node-id": "3" }
      ]
    },

    { "id": "7",
      "type": "node",
      "components": [
        { "type": "component-text",
          "text": "So you're interested in the jump drive now?",
          "next-node-id": "3" }
      ]
    },

    { "id": "3",
      "type": "node",
      "components": [
        { "type": "component-selection",
          "items": [
            { "text": "Yeah, I'm interested.",
              "next-node-id": "4" },
            { "text": "Depends ...",
              "next-node-id": "5" },
            { "text": "That's not what I'm here about.",
              "next-node-id": "6" }
          ]
        }
      ]
    },

    { "id": "4",
      "type": "node",
      "components": [
        { "type": "component-text",
          "text": "Here you go. It's being installed as we speak.\nWell, what are you waiting for? Go and give it a spin!",
          "next-node-id": "end" }
      ]
    },

    { "id": "5",
      "type": "node",
      "components": [
        { "type": "component-text",
          "text": "I see ... Let me give you a very special price.",
          "next-node-id": "7" }
      ]
    },

    { "id": "6",
      "type": "node",
      "components": [
        { "type": "component-text",
          "text": "Dammit. I knew there would be no business to be done on this day!",
          "next-node-id": "end" }
      ]
    },

    { "id": "end",
      "type": "node-exit",
      "components": [
        { "type": "component-text",
          "text": "Thank you and good bye o/" }
      ]
    }
  ]
}
```



```End of Standard```

------



# Appendices



**Documentation License**
-------------------------------------

This document is published under the [GNU Free Documentation License (FDL)](http://www.gnu.org/licenses/fdl-1.3.html) ([http://www.gnu.org/licenses/fdl-1.3.html](http://www.gnu.org/licenses/fdl-1.3.html)).



## Appendix: Standard Development History

In this appendix chapter we are documenting some of the design decisions made in the past so that it would be easier to check back in case of a revisit of a given topic.


### Starting Node

We have considered this question: do we encode some behavior on node level, e.g. *node-init* or exit node or do we require a component for **all** behaviors? Or do we put a next-node-id in the basic dialogue parameters to serve as the starting node that points to the starting node.

Since dialogue parameters should provide generic description and the **nodes** are the actual constructs that provide **functionality**, we have decided to not make any exceptions for the starting node. Exceptions lead to if..then implementations, difficult to read and debug code, and clutter (both, mental and code) which should be avoided. There is nothing as useful as  **simplicity**.



### Why the markdown-anchor Key Style

Encoding of the serialized **keys** follows the *markdown-anchor* writing style with all lower cases and dashes between words. This is for the sake of improved human readability.

However, because most programming languages do no allow hyphens in variables, those keys will be using a *camelCase*.

Both allow for good human readability, especially in very large data sets) as well as machine processing.

Compare *markdown-anchor* style for human readability:

```json
{ "type": "component-selection",
  "items": [
    { "text": "Answer 1",
      "next-node-id": "2"
    },
    { "text": "Answer 2",
      "next-node-id": "2"
    },
    { "text": "Answer 3",
      "next-node-id": "2"
    }
  ]
}
```

With the *camelCase* style:

```json
{ "type": "componentSelection",
  "items": [
    { "text": "Answer 1",
      "nextNodeId": "2"
    },
    { "text": "Answer 2",
      "nextNodeId": "2"
    },
    { "text": "Answer 3",
      "nextNodeId": "2"
    }
  ]
}
```



### Why Use a Hierarchical Object Model

We have briefly analyzed feasibility of the approach devised in the [Entity Component System](https://en.wikipedia.org/wiki/Entity_component_system) (ECS) to extend it to both nodes and node components. This means that practically any type of node can be attached to the dialogues as long as your implementation supports this kind of node. In this system, the node functionality is described by the node components attached to it, and hence offer a second layer of flexibility and extensibility far beyond the present standard.

But then looking on the way nodes need to be processed, which is by traversing a linked tree, where one node points to the next node or nodes, and so on (including branching and revisits), we thought that this would be easier to accomplish with the hierarchical object model.

The ECS is a useful system when there are many entities with distinct  properties (that are grouped into property categories - the components) that require quick access and processing of their individual property classes. This might be the example in game model entities, where animation is called for those models having animation, and physics is called on those entities that have physical properties.

In our case, we better stick with the hierarchical class based object model.



For the sake of decision tracking, here are some unstructured considerations for using an entity component system, which were then deemed to unnecessarily add to the data model  complexity and consequently discarded.

```cpp
/**
 * @brief entity in-memory data handling.
 * This section provides the entity structures using the
 * EnTT library.
 * This shall allow for easy processing of structures as
 * the data is held modularly.
 * 
 * In the entity system, the data is structured like:
 * - registry is the dialogue
 * - nodes are inserted as entities
 * - dialogue components are entity components
 * 
 * @TODO As we want to decouple that data structs from the
 * internal links (vectors), we will need to make them all
 * optional in the structs and delete their content once
 * the have been carried over to the entity system.
 * 
 */
/** @TODO sync node id and entity id **/
/* uint32_t entityId = entt::to_integral(entity);
 * cout << "single entity id: " << entityId << std::endl;
 * emplace components to entity
 */
/** @TODO do we need to create specific components for
  * component-text, component-selection??
  * Functionally that would make a lot of sense, since
  * we must attach specific functionality to component
  * types anyway.
  * 
  **/
/** @TODO is this the right structure?
  * What about a list of components,
  * how would I attach that? Unless see above about
  * specific components.
  */
/*
void dialogueEntt(Dialogue &dlg, entt::registry &registry) {
  // now we need to make structures out of the dialogue
  // essentially we are copying node structs to entities
  // and removing internal links to hierarchical dependencies
  // from them
  // make entities from the nodes
  for (auto &obj : dlg.nodes) {
    cout << "node: " << obj.id << ", " << obj.type << "\n";
    // crate entity
    auto entity = registry.create();
    auto &position = registry.emplace<ComponentStruct>(entity, value);
  }
}
*/
```









## Appendix: Exchange on DlgSystem Standardization

First contact with DlgSystem developers and exchange of ideas on format standardization.

```
Ethollorn 2020-05-23 at 23:05
Your dialogue system seems to be well thought out. Good job. Would you also have a .dlg or .dlg.json file format specification available? It would help to make it available to a larger community and potentially establish your format as a standard format.

Dany 2020-05-24 at 02:25
hi @Ethollorn unfortunately :notyet~2: but the .dlg si deprecated and in the future we plan only to support the json one
even that is not up to date

as it needs to store graph nodes positions imo
and handle some extra data like the custom node data better
currently the export it is off by default because of this reason
because it is a json file the editor thinks you are trying to import some other asset type that needs to be properly handled
we did not have time to work on it to work properly with all the possible node data types :smile:

Ethollorn 2020-05-24 at 13:40
@Dany Thanks for the summary :wink:  I am asking 'cause there is a blatant lack of a standard exchange format for game dialogues in the open source world and your system seems to be the only one out there that is somewhat viable, so I thought I might spend some more time trying to adopt it to my needs (a standards document, and potentially an UE independent library) and hope that it may attract some attention outside of the UE developer community. Let's see where my time permits this idea to go... This development would proceed independently from your adaptations and customizations as your busy schedule probably won't permit a close collaboration. However, some exchange of ideas could still take place since nothing is more valuable than production experience which I assume you have plenty.
As for the technicalities: I believe JSON as the only exchange format is perfectly viable. Just yesterday when I looked at other formats (I think yarn) I thought storing node positions would be a great option, but should be made optional (this to be made part of the standard doc). As for the custom node data, in general definitely yes, but the way to do it should be somewhat generic to be able cover many use cases (need to think more about what and how). And honestly, haven't tried the system in UE yet (is still on my todo list with low prio as I am not developing with UE).

Dany 2020-05-24 at 16:14
the problem with a standard  like this is that it needs to be generic
not sure how can you do something like this for dialogues which are most of the time game genre specific

Ethollorn 2020-05-24 at 18:06
@Dany Probably you're right and I am looking way over my head. I'll just go along with my little project and see where it goes.
```

Then it went from there.



```EOF```
