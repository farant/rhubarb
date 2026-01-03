okay, we are kind of making a new "browser"/os/repl environment


the environment will have a scripting language called sputnik

- sputnik is based on javascript, but has a number of changes
- kebab-case is allowable in identifiers
- allow for underscores in numeric literals
- it has normal javascript template strings with interpolation
- there will be a "markup" (or maybe later stml) value type that can be defined using 
  essentially jsx syntax.
- it will have a built in currency type with exact math with a $ suffix. so like 9.99$
- normal javascript types otherwise
- probably no prototypical inheritance
- arrow functions
- destructuring
- spread operator
- array functions like map, filter, find
- default parameters
- probably await and async
- pure annotation marks functions as pure
- impure function calls inside a pure function automatically deferred as effects
- effects are returned as an inspectable data structure, captured separately from normal return value
- non-pure functions in pure function cannot have a return value
- parent function can batch and execute effects
- there is an "entity" data type
- changes to entity data types are transparently persisted to disk, etc
- changes to entity data type automatically generate events. so all CRUD mutations
- (inside a pure function writing these events are automatically deferred)
- entities automatically get a uuidv7 id
- there is a projection and event-query type that automatically updates based on events
    - persistence is left to discretion of runtime
- there are tags. tags can be namespaced with :: separator
- any value can have tags
- tags literals can also be free standing as statements
- syntax is like let a = 123 #Number::inches #client-request
- there is a standard library that provides a taxonomy of tags
- tags are runtime inspectable and changeable
- you can have a #requires() and #forbids() tag for runtime enforcement of tags
- editor lets you navigate based on tags
- can be used for security policies, all kinds of things
- there is a module type
- there is a command type
- there is a worker type
- module names are globally unique
- don't have to import a module to use its methods, just do Module.method
- can manipulate modules at run time
- command creates a command available from the command line
- if a command value has command type properties then they act as sub-commands
- works are like background processes, can listen for entitie events or be trigger by cron jobs or messages or whatever.
- probably all code will be event sourced so you can always see old versions
- there are string substitution macros with variadic argument support (like c)
- but macro is actually a pure function that returns a string so can do fancy stuff
- probably a built in global message bus / event log


so here are some things that need to happen

- get a 480p drawing buffer to do ui in. basically we will match the aspect ratio of the monitor but vertical height we will simulate 480 pixels and then whatever corresponding width with that pixel dimension
- need to be able to have a text area type input, keyboard events, mouse events
- will be a little like tmux where have the screen will be text editor other half will possibly be a drawing area used by sputnik script (kind of like prototype?)
- need sputnik interpreter/etc to have this environment function as a repl


lots of other steps inbetween i'm sure
