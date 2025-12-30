# Systema Paradigmata - Raw Interview Brainstorm

Date: 2025-12-29

## Q&A Session

**Q: What's the feature you want to build? Give me the elevator pitch.**
A: Let's look at the unified state paradigm spec/conversation and then go deeper on the sort of "OS like" cross widget paradigms that are available to apps (like global acme mode for text, global command registration, etc).

**Q: The spec mentions RegistrumCommandi for global command registration, and Acme-style text commands. Are there other cross-widget paradigms already in the codebase or in your head that should be unified together, or are we primarily systematizing what exists?**
A: Invent new paradigms

**Q: What's the mental model for how widgets relate to each other in this 'OS'? Are widgets more like independent apps that pass messages, like unix pipes where output flows between them, or like cooperating views onto shared data?**
A: Well so you have more detail might as well read the paradigm-brainstorm one as well. But I would say the idea more like shared data views? I would say part of the idea from a more applied intense practical perspective is that you have what I would maybe call filetree of strings? Basically addressable strings in a folder structure turning into a graph database and then into a new addressable string folder structure. So a lot of the power and automation would be the tools to enable you to do that.

**Q: So the graph database is the 'reasoning layer' between input strings and output strings. What kinds of OS-like paradigms enable this transformation?**
A: I mean that the graph database is the kind of "AST" or graph of the program. So what computers are really good at is transforming things from one format into another format, kind of like the graph is a data tensor and the string/filetrees are like different coordinate systems.

**Q: This is a profound framing. So rhubarb is essentially a graph-based 'intermediate representation' that can project to/from any filetree. What are the first cross-widget OS paradigms you want to build that enable this projection workflow?**
A: Well basically you want to give people [tools to] navigate these data tensors that are kind of hyper dimensional graphs. So you want to be able to see the underlying tensors from different coordinate systems / angles. And I think you want the nodes to be in a semantic taxonomy (moderate realism) that lets you almost do a keyhole pass based on semantic rules that lets you mark missing nodes and then present a task list to the user of questions/missing information that need to be answered/fleshed out that are implied in the graph. Also might be able to expand the graph automatically in some cases if there is enough information available to explicitly derive missing information/relationship/nodes based on known rules of relevant taxonomy.

**Q: For the 'keyhole pass' that detects missing nodes — where do the rules that define 'what SHOULD exist' come from?**
A: Genus schemas, User templates. You would potentially have external ontologies but you try to build as many real taxonomies in as possible because you know they are finite with moderate realism.

**Q: For the task list of 'missing information' presented to the user — is this the same concept as the 'Inbox' you mentioned for the canvas file explorer, or a different widget/paradigm?**
A: Same Inbox widget

**Q: So the Inbox is emerging as a major OS paradigm — the system-to-user communication channel. What kinds of tasks besides 'missing graph nodes' and 'external file changes' would flow through the Inbox?**
A: Scheduled reminders, Actor requests, Integrity violations, Import/export results. Yes, the idea is to have the user be an explicit actor within algorithms (AI agents might have an inbox as well) where you can take advantage of state machines or flow charts that have steps requiring human intervention.

**Q: For the 'user as explicit actor in algorithms' — when a workflow pauses for human input, how does the workflow maintain its state?**
A: State machines would be a first class concept I think. Ultimately it would be stored as entities but you would definitely have a workflow, associated state machine, current state, all that kind of stuff. Because it would be async in certain cases (certainly once user is involved) so it's almost like a saga or something.

**Q: For the state machines as first-class entities — where is the state machine definition stored?**
A: There would probably be multiple ways to edit state machines but a GUI for doing it, or at least for writing code and seeing a visualization would be nice. The graph / node interface like in music apps or 3D material generation, Blueprint in Unreal, etc would probably be a first class UI paradigm through which you could interact with certain things.

**Q: So node-based visual programming is another OS paradigm. Besides state machines, what else would you want to express in this node graph UI?**
A: State machines, Data flow/pipes, Query composition, Template composition. I think it could be usable for all kinds of things, it seems like that kind of workflow just works well for people to pipe together a lot of transformations to get sophisticated output.

**Q: Let's talk about the Acme-style global commands. How should commands relate to the other OS paradigms — can a command trigger a state machine, create an Inbox task, or start a data flow pipeline?**
A: All of the above. And the state machines would actually determine to some degree some of the commands that are available, in a certain sense the available commands are the valid states that you can transition to from current state.

**Q: So commands are state-machine aware — only valid transitions are available. Is there a global 'current state' of the whole system, or does each widget/actor have its own state machine that contributes available commands?**
A: There would be many states available, there are widget context based, actor specific, and then many different state machines.

**Q: For the command discovery UI — when user wants to see available commands, how do they access this?**
A: I so far think Acme inline and command palette for sure.

**Q: For the Acme inline commands — how does the system know text is a command vs regular text?**
A: Prefix sigil, Markup/tags. So far we have $command name (that can take arguments) and there would also probably STML/XML arguments as well that would be a rich argument structure (and output structure in some cases).

**Q: You mentioned STML for both arguments and output. Does this mean commands can produce structured output that renders inline?**
A: Mixed. Command declares its output mode, can be any of these (inline render, widget spawn, graph mutation).

**Q: Let's talk about the 'coordinate systems' for viewing the graph. Besides the canvas file explorer, what other 'views' into the same graph data should exist as OS paradigms?**
A: Timeline. By coordinate systems I mean outputting strings or "filetrees of strings" that can encode anything, a program, a visualization, etc. Some of this would be similar to Charles Simonyi intentional programming ideas.

**Q: So the key insight is that any view (canvas, tree, timeline) is just a 'projection' — a filetree of strings generated from graph state. Is there a universal 'projection definition' format?**
A: Well the idea is that you need powerful tools for turning a filetree of strings into a parsed AST, like parsers for lots of common formats like C files, XML, etc. And then you need to have lots of powerful tools for defining how to turn a graph into output strings.

**Q: For parsers — are you thinking of grammar-based parsing (like PEG), or format-specific parsers you build as needed?**
A: Hand-written

**Q: For generators — is the primary model template-based or traversal-based?**
A: I guess I would say it would be a little like Prolog where it is trying to solve an output for you based on a lot of rules. And to some degree you are trying to record intent based stuff first. So the idea is that you would have a graph that represents a whole lot of information and algorithms about your program. And then you could "render" it as JavaScript and HTML or C or documentation etc.

**Q: This is very powerful — the graph contains the semantic 'intent' and different renderers project it to different formats. How does a renderer know which nodes to process?**
A: I think the idea is that you have XML/STML versions of all programming languages, so you would have an XML version of C where all the operators and identifiers and stuff are tags. So you would have tag transformation rules that progressively transform tags from your higher level tags into more specific tags (kind of like keyhole optimization) until the thing is fully unfolded so to speak. And it would have awareness of apophatic or missing nodes, not sure what to call them, basically missing information, and it could present those as task questions to user if there is information needed to fully unfold app as JavaScript/HTML that was not missing in C unfolding to fully satisfy intent.

**Q: For the tag transformation rules — where do these rules live?**
A: Rules are entities, Rules in STML. Rules might be scaffolded in C code so that they are always available but ultimately I think the rules would be STML/entities.

**Q: For the 'apophatic nodes' / missing information — what triggers detection?**
A: Schema validation. Rules are more complicated than just genus schema, there would be rules that match a particular subgraph and say "by definition these other nodes with these relationships must exist."

**Q: So you have 'completeness rules' that pattern-match subgraphs and assert what else must exist. When missing nodes are detected, does the system just flag them (passive), or can it sometimes generate them automatically (active)?**
A: I think that the taxonomy would be rich enough where sometimes you would be able to infer the rules because basically you would have rules that are doing a combination of marking missing nodes as well as adding inferred missing information that is implied by the rules (like this particular model of truck has a particular number of wheels, whatever). So you might have an apophatic node that is identified by one rule that is also being populated by one or more rules. That's why I kind of think of it as similar to Prolog because you are almost defining constraints or facts or something.

**Q: Let's return to the OS paradigms level. We've covered: Inbox, Commands, State Machines, Node UI, Projections, Rules. What else is missing for a complete 'OS' layer?**
A: Networking/sync, Scheduling, Permissions/access

**Q: For networking/sync — is the model one canonical graph that replicates, or federated graphs that can reference each other?**
A: Long term this is going to be an internet alternative. So you will have an app that acts as your browser and a server. Instead of one public internet you will be a member of any number of private internets (or public internets, kind of like Discord servers) and internets might be able to federate with each other to some degree.

**Q: For permissions/access in this federated model — is access control at the entity level, relationship level, or capability-based?**
A: I think of it a little like GitHub. People will be able to fork data if they desire to, also the entity repository will be highly forkable kind of like git graphs, etc. People will be able to export/import collections of data from their graph as files. There will be shared data that people have different permissions to in a private network but the functionalities that are provided will be constrained. And probably for the most part internets will only publish a new version/update once per day and then changes will be queued for the batch update. Users will be able to browse all changes to all the internets they are connected to in aggregate if desired.

**Q: For scheduling — you mentioned batch updates once per day. What other time-based paradigms exist?**
A: Cron-style scheduling, Deadline-aware, Event-triggered, Human-calendar. I think rich scheduling/cron type stuff especially with state machines and workflows. One application might be something like n8n possibly? But basically you will want to have a big toolset for triggering things on schedules, or in an event driven way or whatever.

**Q: What's the relationship between this feature spec (OS paradigms) and the unified-state-paradigm spec?**
A: Builds on unified-state

**Q: What should we call this spec?**
A: Systema Paradigmata

**Q: What's the first milestone for Systema Paradigmata?**
A: Command palette

**Q: For the command palette as first milestone — what features must it have initially?**
A: Fuzzy search, Keyboard nav. Probably mostly just making sure that there is a global command registry for all widgets and making sure that commands can be introspected and then invoked through different means and their arguments are validated, etc.

**Q: For command argument validation — are arguments defined in a schema?**
A: STML schema. Probably an STML schema but you know, most arguments are just fields of some scalar type and then if the argument is STML or an entity or whatever there might be more complicated validation rules available.

**Q: Any final requirements I haven't captured?**
A: All covered. And just make sure to create a brainstorm file that is our raw conversation as well as your spec markdown file.
