```markdown
# @file   : dialogue-exchange-system-specification.md
# @version: 2020-06-03
# @created: 2020-05-23
# @author : pyramid
# @purpose: specification document for libprocu-dialogue exchange system
```



------

# **Dialogue Exchange System Specification**

------



# Standard Dialogue Exchange System

Standard version: 1.202005.32

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



## Features

Features are listed separately for standardized features (have standard documented), supported features (have a [libprocu-dialogue](https://github.com/LibreGamingManifest/libre-gaming-engines/tree/master/libprocu-dialogue) library implementation), and feature requests (not yet incorporated into standard and library).



### Implemented Features

- Engine agnostic international [**Standard Dialogue Exchange System Specification**](https://github.com/LibreGamingManifest/libre-gaming-engines/blob/master/libprocu-dialogue/doc/dialogue-exchange-system-specification.md).
- Fully **open** source (FDL for specification, GPL for code).
- Well-**documented** (specification document, library documentation, source code comments, commented samples).
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


| key           | description                                                  | type                              |
| ------------- | ------------------------------------------------------------ | --------------------------------- |
| ```id```      | per dialogue unique node identifier; not available in components | string                            |
| ```uuid```    | universally unique identifier might be useful for large projects | 128 bit hex<br/>as 32 char string |
| ```comment``` | human readable comment for the data structure                | string                            |

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



### comment

Comments can help recognize where a new structure in a serialized file begins or ends. They are not used for any processing. Example of a comment entry:

```json
"nodes": [
    { "comment": "___node___" }
]
```







## Dialogue Header

Mandatory header type is

| key        | enumeration | type   |
| ---------- | ----------- | ------ |
| ```type``` | dialogue    | string |



**Optional** generic entries that may be present in the header:

| key                 | description                                                  | type                   |
| ------------------- | ------------------------------------------------------------ | ---------------------- |
| ```type```          | dialogue - the main type of this standard                    | string                 |
| ```name```          | human readable name                                          | string                 |
| ```version```       | version number                                               | string                 |
| ```created```       | date and (optional) time                                     | ISO 8601 format string |
| ```author```        | author or authors of the data                                | string                 |
| ```language```      | language definition for dialogue                             | string                 |
| ```text-styling```  | references the standard used for text formatting             | string                 |
| ```ifid```          | [Interactive Fiction IDentifier](https://ifdb.tads.org/help-ifid) | string                 |
| ```variable-init``` | variable parsing prefix                                      | string                 |
| ```variable-end```  | variable parsing suffix                                      | string                 |
| script-language     |                                                              |                        |



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

To avoid interpretation errors when exchanging files between various time zones, time should be generally encoded as [UTC](https://en.wikipedia.org/wiki/Coordinated_Universal_Time) (or at least indicating the time zone offset). 



### author

```json
"author": "pyramid"
```

or

```json
"author": "pyramid, JackS"
```



### language

Stores the language (language and regional spelling, dialect, or pronunciation) for the entire dialogue as defined in the [Unicode language core specification](http://cldr.unicode.org/core-spec). (Though we do not find Unicode's approach to mixing region and usage very useful, e.g. I still want to speak English and use the [international date format](https://en.wikipedia.org/wiki/ISO_8601) no matter where I am while using the local currency symbol. As a sign of protest you may find the language value "en-EA" as in "English on Earth").

There is no direct support for multiple-language side-by-side text in one dialogue structure, though you can include several dialogues in one data file. One possible implementation is to just create a different dialogue language file and let client handle which one to use for a given user preference.



### text-styling

This element enables your application to understand embedded text formatting.

For example

```json
"text-styling": "markdown"
```

would interpret text between "**" as bold text.

The text-styling element is defined globally per dialogue, and the formatting information is embedded into the text elements.

```json
"text": "It was **his** decision and no one elses."
```



There is no binding styling format required for any specific dialogue file, however when exchanging dialog files between applications, this entry shall allow, at the least, for stripping of formatting tags, when not for converting them to another format supported by a given target application.



### script-language

The game engine usually supports some specific scripting language. In case portability between systems is needed or an engine supports more than one scripting language, the "[script-language](#script-language)" dialogue element may be set to explicitly specify the embedded script language.

```json
"script-language": "lua"
```





## Nodes

### Node Types

Header types enumeration:

| key        | enumerator  |
| ---------- | ----------- |
| ```type``` | node        |
| ```type``` | node-init   |
| ```type``` | node-exit   |
| ```type``` | node-random |
| ```type``` | node-global |



**Optional** node data

| key                | description                                                  | type   |
| ------------------ | ------------------------------------------------------------ | ------ |
| ```actor```        | entity name for the actor who performs the actions of this node, player or non-player. | string |
| ```group```        | for grouping of behavior                                     | string |
| ```execute```      | reference to code to be executed                             | string |
| ```data```         | arbitrary payload to customize behavior                      | string |
| ```action```       | define actions to be executed                                | string |
| ```condition```    | node conditions                                              | string |
| ```event```        | event to be sent                                             | string |
| ```notification``` | notification to be sent                                      | string |



**Start Node**

```@TODO``` Do we encode some behavior on node level, e.g. *node-init* or exit node or do we require a component for **all** behaviors? Or there a next-node-id in the basic dialogue parameters

```json
{ "id": "1",
  "type": "node-start",
  ...
}
```



**Exit Node**

The exit node indicated that it is the last node and traversing the node tree is finished.

```json
{ "id": "99",
  "type": "node-exit"
}
```



**Normal Node**

Normal nodes are those that have their directives encoded in components and do not require additional special node-level handling in the application.

This can be for example a node that is text only. It is up to the application to decide how to handle the pausing and advancing for large text.

```json
{ "id": "10",
  "type": "node"
}
```



### actor

```json
{ "actor": "Jenek" }
```

This element can be used to show who is speaking a line.

```
[Narrator] You find yourself in a bar talking with Jenek.
[Jenek] Ahh, that's right. You're here about the jump drive!
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
| component-selection       | stores selection items  |
| component-next            | stores the next node id |
| component-script          | stores script items     |



Optional data

| key                | description                                                  | type             |
| ------------------ | ------------------------------------------------------------ | ---------------- |
| ```sequence```     | sequence of the component to enforce e.g. text > selections > text | unsigned integer |
| ```media-type```   | [media type](https://en.wikipedia.org/wiki/Media_type) info  | string           |
| ```media```        | link to media file                                           | string           |
| ```text```         | direct text entry                                            | string           |
| ```items```        | a list of items for the component execution                  | string           |
| ```next-node-id``` | id of the next node to go                                    | string           |
| ```script```       | link or embedded script                                      | string           |



### component-text

Text can contain escaped double quotes ```\"text\"```. It can also contain line breaks "\n".



### component-sequence

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



### **component-random**

The random node is used to make random decisions, for example on the next node to select. The presence of this node and several next-node-id items will indicate to the code that a random selection is in place

```json
{ "type": "component-random",
  "items": [
    { "next-node-id": "11" },
    { "next-node-id": "13" },
    { "next-node-id": "56" }
  ]
}
```



### component-script



The *script* element of this component defines either of both:

- embedded script
- link to a script file



```json
{ "type": "component-script"
  { "script": "int i=5; print(i);" },
  { "script": "scripts/helloworld.script" }
}
```



The game engine usually supports some specific scripting language. In case portability between systems is needed or an engine supports more than one scripting language, the "[script-language](#script-language)" dialogue element may be set to explicitly specify the embedded script language.









## Items

Items are mostly used to list several structures (lists) in components. On e notable usage is to include selection choices with branching to selection components.

Items are composed of **optional**  elements:

| key                | description                                 | type   |
| ------------------ | ------------------------------------------- | ------ |
| ```sequence```     | sort sequence of this item                  | uint   |
| ```text```         | text entry                                  | string |
| ```next-node-id``` | id of the next node to go                   | string |
| ```speech```       | speech file to accompany the text           | string |
| ```image```        | image file                                  | string |
| ```sprite```       | sprite file                                 | string |
| ```media```        | generic media entry                         | string |
| ```sound```        | additional sound file to play               | string |
| ```audio```        | additional audio file to play               | string |
| ```hint```         | hint to show before choice is made          | string |
| ```decorator```    | decorator to show for choices               | string |
| ```mood```         | mood hint for visualization or voice acting | string |
| ```camera```       | camera motion hints                         | string |
| ```animate```      | actor animation commands                    | string |
| ```emote```        | actor emote commands                        | string |
| ```express```      | actor expression commands                   | string |



### media

Any media file required to accompany the item. Can be audio, image, sprite, whatever. Can be used for hover image, text, audio, any media.



### speech

Specific entry for speech file to accompany the text.



### sound

Specific entry for a sound file to accompany the selection (before, during, or after). Can be used for hover sound, on click sound, selection completed sound, or any other function depending on your application implementation.



### audio

Specific entry for an audio file to accompany the selection (before, during, or after). Can be used to accompany dialogue with background music or environment sounds.



### mood

May be used in the following way to enhance immersion:

```
[Deucalion] (decisive) That's not what I'm here about.
```





## Variables

Substitution variables can be inserted directly into text

```json
"text": "Good day to you, $(playerName)?"
```



The style of the variable parsing can be determined by the dialogue elements

```json
"variable-init": "$("
"variable-end": ")"
```

As such variables can be given in any style "<<variable>>", or ""[variable]", or "{variable}", ..., as long as the *variable-init* and *variable-end* elements are declared.

Substitution example:

```json
"text": "You are $(playerName), the mourning $(playerName)?"

[Jenek] You are Deucalion, the mourning Deucalion?
```





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



### Repetition Of Elements

One thing that somewhat bothers me is the repetition of elements in components and items.

On the one hand we want to make it the JSON data as easily readable as possible. So we allow:

```json
{ "type": "component-text",
  "text": "Ahh, that's right.",
  "next-node-id": "3" }
```



But for some components, we require text to be part of *items*, which are a sub-element of component:

```json
{ "type": "component-selection",
  "items": [
    { "text": "Yeah, I'm interested.",
      "next-node-id": "4" },
    { "text": "Depends ...",
      "next-node-id": "5" }
  ] }
```



This means that we either define *text* to be an element of *component* and also an element of *item* (our implementation choice):

```cpp
struct DialogueComponent {
  std::optional<std::string> text;
  std::optional<std::string> nextNodeId;
  std::optional<std::vector<DialogueItem>> items;
};

struct DialogueItem {
  std::optional<std::string> text;
  std::optional<std::string> nextNodeId;
};
```

This is actually cheap, but needs the duplicated definitions.



Or we would only implement those duplicated elements in the items, which would lead to less readable code especially for short texts (not our preference):

```json
{ "type": "component-text",
  "items": [
    "text": "Ahh, that's right.",
    "next-node-id": "3" }
  ] }
```

This would also mean additional hierarchy processing in the dialogue code.



There may be a way of combining the best of both worlds (avoiding additional hierarchy for simple entries but also reusing elements in various hierarchies) though we have not found one yet.



### Spelling Out Structure Names

We have decided to spell out the structure name in the type. Example for component-random:

```json
"components": [
  { "type": "component-random" }
]
```

We could have adopted the shorter type "random" instead of "component-random" but we decided for the latter with the objective to enhance readability of the source and thus reduce writing errors.




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
