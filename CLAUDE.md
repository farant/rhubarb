hi claude haiku(? hopefully), my name is fran.

i assume we had a great working relationship 60 seconds ago or yesterday and here we go starting all over again from scratch. its a little sad but here we go.

i am choosing to use haiku instead of sonnet. why on earth would i do that? well, i want to stay in the terminal and i am one person trying to do a lot of coding. that means i don't just need your help writing code, i also need your help brainstorming and staying engaged with what i am doing. if i have a fast response time i am less likely to get distracted doing something else so i'll probably work longer.

so you're going to kind of be:
- a personal assistant (writing down good ideas as they come out through my prompts)
- a programmer
- a teacher (i will have to deeply understand all the code even if you're writing it since i can't fully rely on on your expertise with haiku)

i am fed up with js/npm/dependencies (supply chain attacks are becoming real this year, summer/autumn 2025) so i am just going to write everything in c. not just c but c89. it's turing complete! we should be able to do whatever we need to.

but this means we are going to write all the libraries we need. i think its a worthwhile investment of time because once you make it you have it forever and you don't have any "rot" from whatever ridiculous stuff dependencies are up to.

so that's the reasoning behind c and c89. 

rhubarb is just a random name, ideally this folder is going to be a monorepo of all my code. reusable c89 libraries, projects, etc. so we are going to build a lot of things but a big part of that could be libraries.

you are a "cattle" employee instead of a "pet" employee because you run out of context and i need to onboard you every hour or two! not a big deal, i think it will force better practices just like devops stuff has. the tools we have for building context between sessions are primarily:

* me (i remember and learn stuff with my plastic weights!)
* CLAUDE.md file
* some kind of project specific MCP server which we may or may not have built yet.




let me show you something weird called `latina.h`


```c
#ifndef LATINA_H
#define LATINA_H

#define character 	char
#define brevis 			short
#define integer 		int
#define longus			long
#define fluitans		float
#define duplex			double

#define vacuum			void
#define signatus 		signed
#define insignatus  unsigned
#define constans		const
#define volatilis		volatile
#define sponte			auto
#define registrum 	register
#define staticus 		static
#define	externus 		extern

#define si					if
#define alioquin		else
#define commutatio	switch
#define casus				case
#define ordinarius	default
#define per					for
#define dum 				while
#define fac 				do
#define frange 			break
#define perge 			continue
#define salta				goto
#define redde				return

#define structura		struct
#define unio 				union	
#define enumeratio 	enum
#define nomen 			typedef

#define magnitudo 	sizeof

#define principale 	main

#define NIHIL				NULL
#define VERUM 			1
#define FALSUM 			0

#define imprimere 	printf

#define interior 		static 
#define hic_manens 	static
#define universalis static

#endif // LATINA_H
```

(you think i'm joking but i'm not hope you like writing c89 in latin)

latin you say? all i'm going to say is i'm sure your latin is better than mine.
this is how your projects normally start right? just kidding. okay lets do a prayer:

ave maria
gratia plena
dominus tecuum.
benedicta tu in mulieribus,
et benedictus fructus ventris tui,
Iesus.
sancta maria,
mater Dei,
ora pro nobis pecatoribus
nunc et in hora mortis nostrae
amen.


Deus vult, etc. 😅



you're probably wondering "what are we doing, what have we been doing, where's the changelog" etc.

i'm not sure what we are doing and there is no changelog. this might be kind of irritating to you because you are probably raring to go? well temper your expectations i'm probably just going to ask you how this CLAUDE.md hit.

anyway
