# Ratio (Truth Graph System) - Raw Interview Brainstorm

Date: 2026-01-02

## Q&A Session

**Q: What's the feature you want to build? Give me the elevator pitch — what does it do and why does it matter to you?**
A: It's kind of a new layer to the system / widget type. I think I mentioned it in one of the scientia things but the idea is that you can have a UI for dealing with a DAG of syllogisms.

**Q: When you say 'syllogism' — are you thinking strictly classical Aristotelian form (major premise, minor premise, conclusion), or a looser notion of 'argument with premises and conclusion' that could include modern propositional/predicate logic, or even informal reasoning chains?**
A: I'm not sure the exact structure of each node but basically they are truth claims that can have dependencies on other truth claims and that other truth claims can depend on. There would probably be types based on the type of edge or other metadata so maybe things that you would consider to logically necessarily be true based on dependencies being true to be a particular sub type.

**Q: For the edge types between truth claims — what kinds of logical relationships are you imagining? For example: 'A necessarily implies B', 'A provides evidence for B', 'A contradicts B', 'A is a special case of B'?**
A: There would probably be a built in set of types. You would probably not let people define new ones because that seems like it could be abusable and it's not like people are necessarily inventing new kinds of reasoning. I think you would have a pretty rich library of edge types though. Part of the idea is that people can agree or disagree with certain truth claims or edges and turn off parts of the graph or see a "diff" of truth claims based on other people.

**Q: When a dependency (premise) is marked false or uncertain — what should happen to claims that depend on it?**
A: I think people would be able to see truth claims they currently consider to be true but also truth claims that they have marked as not true (with possibly even a trace / dump / summary as to why they don't consider that node to be true, or why they consider it to be true for that matter).

**Q: For the 'diff' between people's belief states — is this primarily for debate/dialogue (seeing where you disagree with someone), for education (understanding why an authority believes X), or for personal tracking (how your beliefs evolved over time)?**
A: It would kind of be a "rich data format" that you can use or link to in lots of appropriate contexts. Kind of like a data type.

**Q: When you 'turn off' a node — does this mean you're asserting it's false, suspending judgment, or something else?**
A: I think it means you are considering it as false but if there are other tools like confidence level that are useful you might be able to use those as well. There would be metadata connected to nodes as well like evidence, counter-evidence, etc.

**Q: For the evidence/counter-evidence attached to nodes — what form does this take? References to external sources (books, URLs), links to other entities in your graph database, free-form text notes, or structured arguments?**
A: Probably references to sources (like our deep links to paragraphs and sentences) links to the public service intelligence database, encyclopedia, your own notes, etc.

**Q: How does this relate to Scientia's philosophical content? Are truth-claim graphs embedded within science articles, or are they a separate layer that sciences can reference?**
A: I think of it as a lower level data type like a video file or xml. So I think you would definitely build things in the science layer using that data type but you would probably use it in other contexts as well.

**Q: If this is a fundamental data type like video/xml — what's the primitive unit? Is it a single truth claim (node), a single argument (node + its immediate premises), or an entire interconnected graph?**
A: Subgraph.

**Q: How do you envision creating new truth claims? Typing natural language that gets parsed, filling out structured forms, or drawing nodes and connecting them visually?**
A: Natural language, Structured forms, Visual graph editor, STML markup (all of the above).

**Q: For the visual graph editor — does this use the memory palace / Zelda-style spatial navigation you mentioned for Scientia, or is it more like a traditional node-and-wire editor (like Blender nodes, Unreal Blueprint)?**
A: Multiple views.

**Q: Given the 640x720, no-scrolling, pagination-only constraint — how do you imagine navigating a large truth graph? Drilling into subgraphs, or something else?**
A: Probably something like drill down, breadcrumb trail, and a "paginated" infinite canvas where you can go N/S/E/W to other pages if there is content in that direction. Search will be available but I am trying not to use search as a crutch, I don't want it to be the primary mode of interaction generally in the UI (not just in here but in general) and there should always be an alternative way to get to any piece of content beyond search.

**Q: For the paginated infinite canvas (N/S/E/W navigation) — what determines the spatial layout? Is it author-positioned (like canvas file explorer), auto-laid-out by algorithm, or derived from the logical structure (premises above conclusions, etc.)?**
A: I think you would have manual but if you can have algorithmic/automated heuristics that make good results you would want those options too.

**Q: When viewing someone else's truth graph (an authority, a debate opponent) — can you annotate it without modifying their version? Like glossing but for argument structure?**
A: Local annotations, Fork and modify. There would probably be some kind of "gloss" in the sense of adding evidence but what you want to avoid is having glosses that basically turn into a comment section on a particular node. You want any additions to be append only and to be structured and material. Let people express themselves through graph structure mutation instead of text where they can be rude or improper, etc.

**Q: For 'express through graph structure mutation instead of text' — does this mean disagreement is expressed by creating a counter-claim node with a 'contradicts' edge, rather than writing 'I disagree because...'?**
A: Well you would want to ground stuff as much as possible in reason and evidence. People get confused eventually thinking through the implications of reason and evidence and also in any contentions that are not grounded in them. Text is very semantically ambiguous and it has lots of unnecessary ways people can get themselves in trouble.

**Q: What happens when two people have graphs that directly contradict? Is there a resolution mechanism, or do parallel truths just coexist?**
A: I think it's a federated thing so different versions can co-exist at the same time. I don't think any graphs are as isolated as people think they are. Ultimately we all live in the same objective reality and the rules of reason are the same for everyone so I think everything is a part of the same tree structure, people just go in different directions because of reasoning mistakes or unequal distribution of evidence or maybe differences in prudence.

**Q: You mention 'the same tree structure' — is there a canonical root? Something like first principles, axioms, or self-evident truths that everything traces back to?**
A: Well wouldn't you say that any truth claim ultimately exists in a kind of tree? Even if people disagree on the conclusions we are all kind of starting from the same observations of reality and reason right?

**Q: For the 'reasoning mistakes' that cause divergence — should the system detect logical fallacies or invalid inference patterns automatically?**
A: I think there could be automated linting tools that are algorithmic or LLM assisted. The problem with LLMs is that they can have the same biases as people so in some ways the graph is more reliable than AI while I think it would be easy to assume things are the other way around.

**Q: For the algorithmic linting — what kinds of structural issues should it catch? Circular dependencies, unsupported claims (no premises), orphaned nodes, or something else?**
A: I think anything that is a clear win but it would be better to have no linting than false positives.

**Q: How should truth claims relate to the entity system? Is each claim its own entity, is a subgraph one entity, or is this a separate data structure entirely?**
A: Entity graph is our database. All data is stored as entities and relationships. I think that claims and subgraphs would both potentially be entities.

**Q: If both claims and subgraphs are entities — what's the relationship between them? Does a subgraph entity 'contain' claim entities, or does it just reference them via relationships?**
A: The whole database is just graphs of entities and different kinds of relationships. So widgets are entities, users are entities, pages with nodes arranged on them are entities, folders or groupings are entities. It just depends on the types of relationships and the genus of entities. Does that make sense?

**Q: For the claim entity itself — what properties does it need beyond the proposition text? Author, timestamp, confidence, sources are obvious. Anything else essential?**
A: Probably fairly rich metadata and then possibly arbitrary relationships to other entities.

**Q: For the proposition text itself — should it be constrained to a particular form? Natural language sentence, subject-predicate structure, or completely freeform?**
A: I think it will be freeform text in most cases but with the ability for people to make longer/richer/more structured content for certain nodes if necessary or desired. Honestly little "sub-graph" visualizers that are basically diagrams could be useful in some cases. The idea is that depending on the context the node could just be a short statement for a claim or it could be the claim plus a more in depth explanation, etc.

**Q: Should claims be able to contain STML markup, or should the proposition text be plain? For example, could a claim embed a link to an entity or reference a term definition?**
A: We don't have formatting markup through STML and we don't have scrolling. If we have a richer content it will be more like Acme text editor + HyperCard.

**Q: For the 'richer content' view of a claim — is this a drill-down (click claim to see expanded page with explanation), or is it inline expansion (claim grows to show more), or a split-pane (claim on left, elaboration on right)?**
A: Probably paginated and there might be inline links to bring up other content in current or facing pane.

**Q: You mentioned 'Acme text editor + HyperCard' — does this mean claims can have executable commands embedded in their text, like '$validate-premises' or '$show-counter-evidence'?**
A: Well potentially there could be embedded commands if they are useful. I haven't experimented enough with this yet to know if that is a good idea or not.

**Q: For the edge types between claims — you mentioned a built-in library. What are the most important ones to support first? Logical support, contradiction, evidence, something else?**
A: Start with all three (support/depends, contradicts, evidence).

**Q: Should edges themselves have metadata? For example: strength of support, who added this connection, confidence that this is the right relationship type?**
A: Well there will definitely be some metadata such as who created it and when, etc that is a part of all entities/relationships. I think there is a good chance there will be full metadata for relationships but not sure yet.

**Q: For integrating with the public service intelligence database — do truth claims link TO intelligence entities, or can intelligence data automatically generate claims? For example, 'Event X happened on date Y' becoming a claim node.**
A: Both directions.

**Q: How does this integrate with Smaragda simulations? Could you simulate 'what if this claim were false' and see downstream effects on the graph?**
A: You might be able to do something like this or use it for argument stress testing. Would have a more developed prototype and use it more to see if that makes sense.

**Q: For importing/exporting truth graphs — should there be a standard interchange format so people can share argument structures across internets, or is this purely internal?**
A: All mechanisms (internal, file export, and federation).

**Q: For the UI widget(s) — is this one widget with multiple view modes, or separate widgets for different purposes (graph editor, claim browser, diff viewer)?**
A: I'm not sure exactly but I would imagine there could be more than one widget. I'm still not sure if we will have embed because of limited screen real estate or something that pulls up content in facing panel. It also kind of depends on how you draw the boundary of widgets. Definitely will require multiple UI modes I think.

**Q: For the facing panel interaction pattern you've been developing — when you select a claim in the graph, what shows in the facing panel? The claim's detail/elaboration, its immediate premises, its evidence links, or something else?**
A: Probably it would usually be the claim detail, which itself might include information about dependencies, dependents, etc.

**Q: What's the relationship between this feature and the Inbox? Could unresolved contradictions, incomplete arguments, or disputed claims surface as tasks?**
A: There could definitely be an integration with the inbox, it might be somewhat user configurable.

**Q: For versioning/history — since everything is event-sourced, you can see how a claim changed over time. Should the UI expose this explicitly? Like 'view this claim as of last month' or 'see all edits to this argument'?**
A: I think this is a UI design issue but in a perfect world you would definitely have access to all of this information.

**Q: What should this feature/system be called? Do you have a name in mind?**
A: Ratio.

**Q: What's the first milestone for Ratio — the minimum slice that proves the concept and is actually useful?**
A: Visual graph.

**Q: What's the biggest risk or open question you have about Ratio?**
A: UX complexity.

**Q: For UX complexity — what would make it 'too complicated'? Is the concern about authoring (creating claims and connections), navigation (finding your way in large graphs), or understanding (grasping what the visualization means)?**
A: I think the UI is probably not going to be incredibly hard, a lot of it will be problems you would want to solve in other parts of the system as well. There is just a high surface area because there are lots of pieces of information you want to expose and you have to figure out tricks to do stuff without pagination in a nice way.

**Q: Who is the primary user for v1? Yourself exploring ideas, teaching others, Scientia philosophers, or general Lapideus users?**
A: Everyone and also for people/groups that are trying to discuss an issue.

**Q: For groups discussing an issue — is there a collaborative editing model? Multiple people editing the same graph simultaneously, or turn-based contributions, or each person has their own fork they reconcile?**
A: Well I don't think we will have anything like general purpose zoom calls and I think realtime collaborative editing is definitely not something you would do over the internet. Possibly in some cases in person with multiple physical devices on the same local network but not sure. I think for groups it would be more of an async type thing and a tool for having clarity about disagreements.

**Q: For the spatial layout on the paginated canvas — should there be semantic meaning to position? For example: premises on top, conclusions below; or earlier claims on left, later on right; or is position purely for user organization?**
A: There could be helpful conventions as we go but for now I would say purely organizational.

**Q: For async group use — when someone adds a claim or connection to a shared graph, how do others find out? Notification via Inbox, visible in a 'recent changes' view, or just discover when they next look at the graph?**
A: It would be through the internet system so part of your federated internets and on the batch. I think there would be multiple ways that you could be notified or browse all changes and/or changes that you are interested in.

**Q: Is there anything about Ratio I haven't asked about that's important to capture?**
A: All covered.
