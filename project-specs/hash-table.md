okay, i'm not an expert on hash table implementation but i'm trying to learn!


my basic understanding is that you have a key and a value. the key is hashed through some hash function that puts the key in a relatively unique bucket, although multiple keys might end up in the same bucket. within the bucket you have a linked list of values you walk to find the specific node you are looking for.

so one approach is
- hash table does not store data. instead it contains a node that users embed
  in their own data. it looks like the user supplied node has a link to the 
  hash table supplied node data structure, which then has a link to the next 
  element in the linked list?
  - that kind of makes sense in an inversion of control sense so that you can turn
    any data structure into the payload of a hash table?
  - if i understand correctly the hash table library is responsible for
    initializing the node on my data element after i have set the key. in the
    sense that the user sets the key of the node but then the table is responsible
    for doing appropriate linked list book keeping i assume. but technically
    the user is in control of where the node embedded in the user defined struct
    is allocated in memory.
  - then i get the node back by looking up by key. and then i can cast the pointer
    to the node back to my user supplied data structure because they are co-located
    in memory. and presumabely this does not rely on having the hash table 
    node struct as the first property in my user supplied hash table struct 
    because you can use the offsetof macro to do the math with any property.
- the hash table has an array of pointers to to nodes
  - it keeps track of number of buckets & how many things it has stored
  - it has a reference to its piscina
  - it has a function pointer for the hash function
  - it has a seed for the hash function
- when the hash table looks up a value
  - it hashes the key using the function it was initialized with
  - it looks like the hash is supposed to be an unsigned number
  - it modulos the hash with the table capacity to figure out the bucket index
  - then it uses the index to get a bucket from the bucket array, and it walks
    the nodes of the bucket to find the specific node needed.


- offsetof is part of stddef.h and it calculates the offset at compile time.
  - should be available since c89 so safe to use.


