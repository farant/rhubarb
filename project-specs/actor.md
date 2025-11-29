features for actors


- message handlers
- batch actors (particles and stuff)
- error state / crashed state for actor
- built in state machine
- services (singleton actors) that are responsible for certain state
- state is hierarchical, ie can be in alive.combat.attacking
- message has sender address
- every entity is an actor
- actor behavior is defined by genus
- actor can declare implementation of interfaces by having an "is-a" relationship to Genus
- every message handler gets a reference to entity itself
- message handlers can introspect actor properties and entities connected by relationship 
- mailbox per actor
- lazy allocation of mailboxes (don't need one until first message)
- all entities have address (uuid) and can receive messages. if genus does not have message handlers then is a no-op
- there will be a message bus
- there will be a service context to look up services by name, etc
- ability to see all actors that are in a crashed state and manually upd- ate state/help them recover
- actors "spawn" actors by persisting new entities
- entities can be mutated by sending messages or directly updating properties
- probably have a fallback message handler option so an entity can respond to any message and introspect message to decide how it responds
- genus/actor can have a message handlers for crud operations on entity
- message handlers are declarative, probably using a hash table and function pointers
- might also be able to have "transition handlers" that are called when transitioning out of or into certain states
- crud operations on actor are event sourced (we have an implementation of this for entity graph already in graphus_persistens.c)


