# Scientia Simulation Follow-up - Raw Interview Brainstorm

Date: 2025-12-31

## Context

This interview follows a "simulated implementation" of Scientia Phase 1, where we walked through ~2.5 hours of pretend coding to surface complications and design questions. The simulation identified 19 complications ranging from addressing schemes to missing infrastructure.

## Q&A Session

**Q: For the addressing scheme (Science:Chapter:Paragraph:Sentence) — what happens when someone edits the canonical text and inserts a new paragraph between 3 and 4? Do all subsequent addresses shift, or does paragraph 3.5 get inserted, or does the new paragraph get a UUID-style address?**
A: Off the top of my head it seems like you might be forced to have versioned addresses. I guess this is actually a good reason to break content up into sub-sections because that limits the blast radius of insertion. Probably if you jump to an old version (which is available because everything is event sourced) you would be able to jump to the latest version. And there would be easy tools to bulk or granularly migrate glosses to latest version if underlying paragraph hash has changed (which would probably be used as a parallel form of ID). Giving paragraphs a UUID is actually a pretty good idea. You just have to have some kind of textual algorithms to understand "when is this just an update of a paragraph that got moved around, when has the paragraph been 'deleted' and replaced with a different paragraph" etc.

**Q: For paragraph identity across edits — would you use content hashing (same text = same paragraph even if moved), structural position (paragraph 3 in chapter 2 is always that slot), or semantic similarity (AI determines if rewritten paragraph is 'the same' paragraph)?**
A: I think you would use as many techniques as you could as well as having the ability for users to manually intervene and link continuity of paragraphs. I mean in theory if you are actually tracking like the granular widget events of text editing you would be able to see how a paragraph mutated over time based on people editing it and not necessarily persist all of that edit history forever but you might be able to figure out "oh the user moved the cursor to the middle of this paragraph, started typing and deleting" etc.

**Q: For print infrastructure — given there's no PDF/print code, what's the minimum viable path for physical card output? Browser print (generate HTML, user prints from browser), image export (PNG sheets user sends to print shop), or actual PDF generation?**
A: I think you are definitely going to need a PDF library at some point. One major functionality that will be built into the whole Lapideus app we haven't talked about yet is that it will have a whole suite of built in / native tools for writing books. Like snowflake method outlining, brainstorm notes, references, etc. everything you would want in a structured way. As well as having a structure that can be transformed into other formats in a consistent way such as a print version you can publish on Amazon Kindle digital or KDP, PDF, etc.

**Q: For the book writing tools you just mentioned — is this a separate sphere (like 'Scriptoria' for authoring), part of Scientia (authoring science content), or a general capability that multiple spheres use?**
A: Separate sphere.

**Q: For the card game mechanics — the simulation surfaced that 'free mixing' cards from different sciences need some interaction system. At minimum, what properties must every card have to participate in ANY game mode?**
A: So this is where the moderate realism would be, that you have types of objects that can interact with each other in a real way or in a more abstract way (like some kind of action that generates and consumes a resource). But I think a realistic or somewhat stylized version of this kind of "abstract discrete physics engine" is the direction you would need to pursue.

**Q: For the 'discrete physics engine' idea — are you thinking something like: Tools act on Materials to produce Products, Skills modify Tool effectiveness, People provide Skills, Locations constrain what Tools/Materials are available?**
A: Yes, definitely along those lines. Just figuring out the sweet spot of categories where every one is "earning its keep" so to speak would be the strength of the design.

**Q: For gloss display — the simulation noted 320px panel width is tight. Medieval manuscripts put glosses in margins. Modern options: inline expandable, footnote numbers, or separate panel. What feels right for the aesthetic?**
A: My guess is that the most coherent way to handle this would be taking advantage of the split view but the UI idioms for navigating content that has been displayed in split views still needs to be figured out. Could probably be as simple as having forward and back buttons though.

**Q: For split view navigation — when you're reading text on left and glosses on right, what happens when you move to next paragraph? Does the gloss panel update to show that paragraph's glosses, or do you navigate them independently?**
A: I think it would be a common pattern that changes to facing panel would be synced in some way to the interactions you are having with a panel.

**Q: For entity granularity — the simulation worried about thousands of sentence-level entities. Would you prefer: one entity per paragraph (sentences as array property), one entity per chapter (paragraphs as structured text), or accept the granularity for maximum addressability?**
A: I think you would probably accept the granularity but I haven't encountered technical challenges, if any, that would force revising this yet so who knows if that would end up being impractical or not.

**Q: For skill tree prerequisites forming a DAG — when visualizing without scrolling, the simulation proposed showing 'neighborhood' (selected node + its immediate connections). Does this feel right, or would you prefer tier-based pages (all skills at same depth level)?**
A: The best pattern I have so far for DAG is probably kind of a "folder structure" in that you have nodes you click on to go to a view of a subgraph? So the node kind of represents a subgraph of the tree or DAG. I think another pattern would be to have "infinite canvas" but to lay it out kind of like NES Zelda 1 where you essentially have a page you are looking at with "north south east west" (potentially, you might not have all the directions) on a given page. I know in one brainstorm I mentioned having something like Secret of Mana SNES/JRPG as a UI mode. The idea behind this is kind of an alternate view mode for hypergraphs where you almost have something like Stardew Valley or Minecraft (in the RPG style) that lets you build memory palaces as a way to make the graph tractable. Minecraft in the sense that you would be able to build out and expand your "base" or palace or whatever through the instrument of your character running around.

**Q: The memory palace / spatial navigation idea is fascinating — for Scientia skill trees specifically, would this be a literal 2D world (rooms representing skill clusters, walk between them) or more metaphorical (spatial layout but not a 'game world')?**
A: Could be both available. My idea is that this would be a ubiquitous UI affordance kind of like list vs icon view across all of Lapideus.

**Q: For quiz question sourcing — the simulation noted questions could be manually authored, generated from content, or AI-generated. For MVP, would you accept manual authoring only, or is some generation essential?**
A: I think there is a good chance that I will try to "seed" a lot of content that is hard coded / built into the app without requiring an internet using LLM generation. Again kind of taking the moderate realist bet that real things are finite and predominantly stable.

**Q: For LLM-seeded content — is this a one-time generation during development (baked into release), or would users be able to trigger generation for new sciences they want to explore?**
A: Probably both, people would provide their LLM integration through a plugin or something. Also would be able to get extra content from the internet or from files. I am imagining that bundling any amount of information as a file that you give to people would be a strongly supported pattern as well. So import from a file, all that stuff would be as easy as possible to do.

**Q: For the separate 'Scriptoria' authoring sphere you mentioned — would Scientia content be authored there and imported, or does Scientia have its own lighter authoring built in?**
A: I think they would be synergizing. Like you would use some "Scientia" features and widgets when writing and when you need to write, whether it's a full book or smaller piece of content you would have the "best" writing tools available to do it. Like the reality is that writing is not just a text editor based on the technical output (ie Word document vs .c file, etc). There are different reasons we write, like telos of the writing process itself. And different telos might themselves both "technically" result in a rich text document or ASCII file, or STML markup or HTML or whatever. The point is that editing modes should be organized around telos instead of output format. So there should be a really good mode for writing books/articles. Maybe a good format for writing letters. Good mode for writing code, etc. And the idea is that these modes are finite and especially through something like Smaragda you can surface the majority of them and then just include really good modes for all of these modalities.

**Q: For the 'telos-based editing modes' concept — for Scientia specifically, what are the distinct writing purposes? Writing a science book chapter, authoring a skill node description, creating card content, writing gloss commentary?**
A: So a science book chapter would probably be "book writing mode" although maybe with some Scientia specific affordances. Authoring a skill tree is a mode, for sure. Authoring cards is a mode. And glosses definitely a writing mode that is going to be available.

**Q: For the standard science categories (History, Principles, Techniques, Tools, Materials, Projects, Complications, Masters, Philosophy) — are all 9 required for every science, or can a science have a subset?**
A: I guess I would see it less as required categories and more similar in spirit to DDD "ubiquitous language" for a given domain where there are a finite number of core "types of things" that crop up across definitions of science that can be distilled, even if any given science does not have all of them.

**Q: For the 'discrete physics engine' card interactions — when a Tool acts on a Material, what determines the outcome? Fixed rules per tool-material pair, generic rules by category, or card-specific effect text?**
A: I think the way things like Marvel Champions or LOTR LCG handle this is that they end up adding tags to cards. So there might be rules written on a particular card (which can be any kind of rules/interaction describable) but the referents can be "tags" or categories vs specific cards so you can kind of have a variety of addressing techniques for defining what rules can apply to.

**Q: For card tags enabling rule targeting — would tags be hierarchical (Wood > Hardwood > Oak) so a rule targeting 'Wood' affects all subtypes, or flat (each card just has a set of independent tags)?**
A: Hierarchical tags make sense. Obviously you have limited space on the face of a card but I could see using that technique. But it would depend in practice if it was "earning its keep" as a mechanism that was providing a lot of value that wouldn't otherwise be possible.

**Q: For the roguelike/overworld generation you mentioned — in the Zelda 1 / memory palace model, what would the 'rooms' represent in Scientia context?**
A: Broadly in this kind of memory palace mode I think you would lean into the two pane mode, so you interact with objects in the world and they would pull up content in the facing panes. So from a certain perspective it's just a buildable Zelda/Stardew Valley world where the user (or author) can mark certain objects as "hyperlinks" to other content and place them around / build them. But I think you would have a lot of richness that could emerge from that.

**Q: For the simulation surfacing 'Scientia is really three systems' (Encyclopedia, Progression, Card Game) — does this framing feel right, or do you see them as more unified?**
A: I could see that making sense. If you really boil down a "mode" I think you would view any system as a bunch of extra widgets that are interacting with the same underlying graph database substrate everything is built on. But I guess with extra constraints and types of entities. I guess the idea of this highly integrated "OS" where you don't have different apps but everything is built in is that you can have a very tight integration between all these widgets where they are able to have a multiplicative effect on usability. Kind of like the flexibility you get from adding extra tags to a XML-ish markup language where you can then use that tag in all kinds of contexts.

**Q: For the Catholic philosophical grounding — the simulation noted pitfalls. What's the main risk you're thinking of?**
A: I think the main risk would be people having heretical ideas.

**Q: For preventing heretical content in philosophy sections — is this handled by curation (trusted editors/internets only), review process (someone with authority approves), or letting federated internets self-select?**
A: I'm not sure. Maybe forcing people to author through certain kinds of syllogisms so their expression is more rigorous and easier to talk about.

**Q: For structured syllogistic authoring — would this be a dedicated 'philosophy writing mode' that constrains how arguments are expressed, forcing premise-premise-conclusion structure?**
A: Syllogism DAG is probably going to be a core affordance that is a part of many systems.

**Q: For 'syllogism DAG' as a core affordance — is this a way of representing arguments where nodes are propositions and edges are inferential relationships, visualized and navigable like other DAGs in the system?**
A: Yes, basically. So people can identify basically dependency chain, where their "truth graph" diverges based on disagreeing with on a particular syllogism, experimental stuff where they can go "what if I turned on this one or off this one", etc. Probably will have richer metadata like evidence, objections, and whatever. I imagine ultimately you could have a case for any node if necessary. The idea is to have this as a module / UI that can be used by any system as easily as the graph database potentially.

**Q: Is there anything else from the simulation complications or Scientia design that I haven't asked about that's important to capture?**
A: Covered enough.

---

## Key Clarifications from Simulation Follow-up

### Addressing & Versioning
- Versioned addresses with content hash as parallel ID
- Multiple techniques for paragraph identity (hash, position, edit tracking, manual linking)
- Event sourcing enables jumping to old versions and migrating glosses

### Infrastructure
- PDF library needed (part of larger Scriptoria sphere for book authoring)
- Scriptoria = separate sphere for book writing tools with output format transformation
- Telos-based editing modes (book writing, skill tree authoring, card authoring, gloss writing)

### Card Game Mechanics
- "Discrete physics engine" model: Tools + Materials → Products, modified by Skills/People/Locations
- Tags enable rule targeting (hierarchical tags possible)
- Rules written on cards, referencing tags/categories

### Visualization
- DAG navigation: folder-like subgraph drilling OR Zelda 1 style room-to-room
- Memory palace mode: buildable 2D world as ubiquitous UI affordance across Lapideus
- Objects in world are hyperlinks to content, shown in facing pane

### Content & Authoring
- LLM-seeded content at development time, plus user-triggered generation via plugin
- File import as strongly supported pattern
- Standard categories are "ubiquitous language" not strict requirements

### Philosophical Content
- Main risk: heretical ideas
- Potential solution: syllogism DAG as core affordance
- Truth graphs with dependency chains, "what if" toggles, evidence, objections
- Syllogism DAG usable across all systems
