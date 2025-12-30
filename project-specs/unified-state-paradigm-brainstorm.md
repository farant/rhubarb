# Unified State Paradigm - Raw Interview Brainstorm

Date: 2025-12-29

## Q&A Session

**Q: What's the feature you want to build? Give me the elevator pitch — what does it do and why does it matter to you?**
A: refactor and a new data paradigm for widget states and long term entities

**Q: When you say 'widget states' — are these UI widgets with transient visual state, or something more abstract like game entities with behavior?**
A: Hybrid

**Q: What's broken about how state is currently handled that made you want a new paradigm?**
A: Persistence mismatch, Update complexity, the api needs to be more unified, right now widgets are not handling state in a consistent way. this means it can't be introspected in the ui, and it is not always being saved on mutation which is basically what should happen. also we need to be able to "close the books" on all our event sourced entities sometimes based on some kind of entity or genera based policy. so some entities we want to keep a record of all changes forever, sometimes we want to have policies where we don't keep the records for as long. we might also need some kind of debounce policy with a wal of immediately preserved changes that then get aggregated into single event mutations in some cases. just trying to think through all the different scenarios

**Q: For 'closing the books' — when you compact events into a snapshot, do you need to preserve the ability to replay from that snapshot forward, or is it purely for storage savings?**
A: well i'm not 100% sure. for example at some point we will probably have screen recording and replay that is event based vs frame based. so from that perspective we might want to keep a full history of event state. i guess one concept might be that some ui state is derivable from other events based on ongoing calculation (ideally would like things to be fully deterministic, so like in eskill steenbergs dependable c that might mean only using integers, no floats or doubles which can be inconsistent across machines). i guess i am just thinking about the frame restore of an action game. there is state for 40 entities that might have been changing frame by frame wnd we don't necessarily need to preserve every crude update of all those things forever

**Q: The WAL + debounce scenario — is this for things like text input where you want crash safety but don't need 'keystroke-level' history?**
A: the idea is that if someone quits before the debounced timeout saves the event that those pending changes waiting to be aggregated and saved as a single event are not lost

**Q: For derivable state — where does the derivation logic live? Is it attached to the entity type, or is it a separate 'view' layer that watches events?**
A: in general i want to try to approach things based on a double buffered state model. so each entity is an actor that has access to the full state of the previous frame (across all entities/actors, global state) but then they can only write their own state for the next frame. entities/actors can send messages to each other to trigger state changes in each other. my understanding is that this model can be highly multi-threaded

**Q: How do you envision classifying entities into persistence tiers? Is this per-type at compile time, or can individual entities change tier at runtime?**
A: Hybrid

**Q: In this model, what IS an 'event' that gets persisted? Is it the delta between frame N and frame N+1 for an entity, or is it the messages that triggered the change?**
A: probably both. right now we are automatically doing event sourcing for crud events on entities, there are no user defined events sort of "domain specific" events. but at some point i can imagine having those. and then you would also probably still have crud events in a lot of cases. there would be messages between actors and, you know, probably ui events from user, outside environment, etc? one design consideration is whatever we need to do a "replay" or to stream changes that can be replicated on a different computer "watching" the activity

**Q: For message passing between actors — are messages processed immediately within the same frame, or queued for next frame?**
A: you would definitely need some deterministic order. so probably in the same frame? but i haven't built something like this before so i'm not 100% sure. i think this is based on idea that carmack mentioned at some point in the past 5 years

**Q: How do UI widgets fit into the actor model? Is each widget an actor, or do widgets observe/project from underlying actors?**
A: i think widgets are actors. we need to be able to have the ui basically do queries and create a view model for itself as a projection i guess? not sure what the best api is here. but for example the view model would be ephemeral but the state of the ui that is generating its view model (like "i have this page open and looking at this information") or whatever would probably be persisted as part of the widget actor so that it can be restored after reopening the app. but there would be stuff like animations that would be ephemeral i think

**Q: For the 'introspectable in UI' requirement you mentioned — what does introspection look like? Debug inspector? User-facing history?**
A: so i am kind of making something like plan 9, i have a ranger-like ui that lets you explore actors. my idea is that everything is basically a graph database that you can inspect and interact with, and then obviously you can use that to build more tailored inspection widgets in different ways. you will probably have the ability to define widgets in a data driven way using stml as a markup language and sputnik as your scripting language

**Q: For the persistent vs ephemeral split within a widget actor — how does the widget declare which fields are which? Annotation? Separate structs?**
A: i think there will be a little iteration here to see what works with the grain of usage best but i could see annotation or separate structs. another thing that is important is that kind of like distributed tracing i want semantic "intent" kind of like aristotles final cause or something like that to be an explicit part of the code. so you don't just have mechanical code you also have intent where the programmer is documenting "i'm adding this feature with the intent that it helps users budget grocery prices more easily" or something like that and that data mutations and ultimately all the code can be traced back to specific intents

**Q: For view model projection — when does it get computed? Every frame, on-demand when rendering, or when underlying data changes?**
A: i think there would be both? like some would be "event driven", maybe polling each frame for events like how a game engine handles input events, and some would be happening every frame also similar to a game engine

**Q: For intent tracking — where does intent get attached? At commit time, in code comments, as metadata on functions/events, or something else?**
A: at some point this will have its own vcs so not git. but probably somehow in code annotations and event meta data. the way i kind of imagine it is almost having a ui that asks you "what are you trying to do" and then annotate data you are creating as you go. obviously at the code level as well, but probably prompting you at key points in the ui "what is your intent here" as well to make sure it's happening

**Q: Can an intent have sub-intents? Like 'help users budget groceries' breaks down into 'add price comparison widget' breaks down into 'fetch prices from API'?**
A: yes either a dag or a graph, i think intents can be arbitrarily complex. but part of the key of the system is that it will be moderate realist instead of nominalist so we will have a real finite taxonomy of different kinds of things built in so intents will probably themselves be instances of genus inside a taxonomy of different kinds of intents, etc

**Q: For the taxonomy of genera — is this something you define upfront and extend carefully, or does it emerge and get refined as you use the system?**
A: Hybrid

**Q: How does the genera/taxonomy relate to the actor types? Is each actor type a species under some genus, or are they orthogonal concepts?**
A: everything is an actor/entity/species/instance. i am basically trying to have a graph database (there are relationships too!) where every node has some kind of event sourced history and is an actor that can potentially send and receive messages. but the idea is kind of like instead of plan9 everything is a file it's that everything is a node in the graph database.

**Q: Are relationships themselves actors/nodes with their own state and history, or are they simpler edges?**
A: relationships are a different "type" than nodes, so i don't know if you would necessarily be able to explore relationships in themselves as nodes separately, but they are named and directional and probably have an id/uuid and may be able to have additional properties themselves. but it could be as simple as relationship name and source/destination. there is also backlinking so a node can see its outgoing and incoming relationships

**Q: In the double-buffered frame model, when a relationship changes (created/deleted), which actor owns that change? The source, the target, or the relationship itself?**
A: in this context it might make sense to have the relationship be its own actor that receives messages

**Q: For querying the graph to build view models — do you envision a query language, traversal API, or something else?**
A: i think anything is a possibility? we have our own sputnik language with is similar to javascript but can diverge. that is probably the main "language" along with i guess just declarative stml to be working with. i could see there being stuff like "signals" in sputnik code similar to solid.js etc that re-reruns certains functions (that have an api for interacting with the entity repository) if certain input entities have changed

**Q: When a view model depends on multiple nodes across the graph, how does invalidation work? Does each node track its dependents?**
A: probably some combination of 2 and 3? i don't think nodes track who is watching them although they probably could introspect and figure it out if they wanted to. nodes have genera and tags so imagine sometimes the dependencies will be everything that matches a certain query and their dependents potentially

**Q: For memory layout of the double buffer — is this two full copies of all actor state, or something more granular like copy-on-write per actor?**
A: need to explore tradeoffs. one problem i need to solve right now is we have arenas (piscina) but i think we are basically using one arena for everything and it's never really being discarded so we have memory leaks. so i think one view of the full double buffer would be a discipline that actually creates a lifetime for certain things. i think it would be a little like memory mapped files where not necessarily the whole "entity database" is paged into state at any given time, its based on what is being affected and might lazily be allocated in ram as certain entities become "hot" (ie they are queried or receive a message or whatever). i'm sure there are lots of optimizations. i think all this would be opaque to the actor code that is querying "global state" it doesn't know what is in memory or not it just queries everything it needs to. but the intention of this discipline is definitely just so that you could have massive parallel threading where every actor could be its own thread, etc as you need to.

**Q: How does this connect to your existing arena allocator (piscina)? Is actor state arena-allocated, or does it need different lifetime management?**
A: mixed. i think there could be a per frame arena that is passed as context to actors and wiped every frame or reset and that could help with memory leaks. but there are arenas that should also be global potentially? like i am thinking of having a global string interning system but idk (obbviously that would persist beyond a frame to have stable pointers per string for the lifetime of the app. this could even be something where pointers or offsets / relative pointers are persisted to disk so the whole app has a stable id for strings that can be referenced across sessions)

**Q: For the on-disk representation of the entity graph — are you thinking custom format, SQLite, memory-mapped files, or something else?**
A: custom binary, we have implemented stuff using nuntium and entity repository i think already but is just a first version

**Q: For cross-session string interning — do strings need to be addable at runtime, or is the string table fixed at build/load time?**
A: Append-only

**Q: For the actor API — when an actor reads 'previous frame global state', what does that call look like? Function taking entity ID? Typed accessor?**
A: i could see 1 or 3. probably it would be like treating global state as a database that it would try to extract information from. i'm not sure if it would have the ability to read from global state as memory directly. maybe when it does query results it also can get pointers back into frame arena

**Q: For writing to next frame — does the actor mutate in place, return a new state, or call setter functions?**
A: i think there are advantages to being able to treat it like a pure function although inside the actor itself i could see imperative setter api being most convenient. i don't know exactly how it works but probably you would copy current frame memory for "new frame" and then do granular updates of that? or something. i'm sure there are lots of optimizations

**Q: For inter-actor messages — are messages typed (specific struct per message type) or generic (string name + payload)?**
A: probably something like a tagged union? since message sender and receiver have access to the same global state i think you could just reference global state that receiver could look up, doesn't necessarily need to be a part of the payload

**Q: Can an actor send a message to 'all actors matching query' or only to specific IDs?**
A: probably both. it's a little fuzzy events vs message bus. but i think you would want to support a lot of message bus patterns for the cases when they are logically appropriate

**Q: What exists today that this refactor touches? Is there already an entity system, event sourcing, widget system that needs to evolve?**
A: Partial exists

**Q: What's the first milestone you'd want working? Full system, or something narrower to prove the concept?**
A: Single widget type

**Q: Which widget type makes sense as the first conversion candidate? Something simple to prove concepts, or something complex that exercises more of the system?**
A: there are maybe 6-7 different widgets as well as a schirmata / multiple screen "widget" that basically functions like tmux screens. so you would want to migrate all this to consistent storage story / api to iron out current persistence across app run discrepencies for widget / schirmata state

**Q: What's the biggest risk or unknown you're worried about with this design?**
A: Persistence

**Q: What specifically is broken about current persistence? State not saving? Saving inconsistently? Corruption on load?**
A: Not saving, Inconsistent, we have to manually make sure state saves so sometimes it doesn't happen and the different schirmata views that might be showing the same widget on different screens with different states do not seem to always be saved/restored properly

**Q: For schirmata — is this managing multiple independent widget trees, or is it more like viewport switching over one tree?**
A: all widgets should have access to the same entity repository but each screen is its own widget state. technically the way it is implemented is there are widget singletons that are supposed to get hydrated to correct "screen state" if they are currently visible when screen switches

**Q: In the new model, should each screen have its own widget instance (actor), or should widgets be shared with screen-specific view state layered on top?**
A: logically each widget instance and each screen should be its own actor.

**Q: When should persistence happen — every mutation, periodic checkpoint, on explicit save, or on app close?**
A: immediate durability, just potentially some debouncing so you are not creating a new event for every keystroke?

**Q: What should this system be called? Does it have a name, or should we coin one for the spec?**
A: we already have some actor and message passing stuff. i don't know what exactly we will be adding but as we add new stuff we would give it latin names i think

**Q: Are there any hard constraints I haven't asked about? Memory limits, target platforms, dependencies to avoid?**
A: No dependencies

**Q: Which existing rhubarb libraries/files contain the current actor, entity, or widget code that this refactor will touch or extend?**
A: Explore together

**Q: Any final thoughts or requirements I haven't captured?**
A: All covered
