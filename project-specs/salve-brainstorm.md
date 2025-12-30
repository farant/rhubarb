# Salve (Control Plane) - Raw Interview Brainstorm

Date: 2025-12-29

## Q&A Session

**Q: What's the feature you want to build? Give me the elevator pitch.**
A: Basically one of the features of having this graph stuff and the human inbox/cron jobs, and the rule engine, I guess it's built on a lot of this stuff, is to have stuff that can function like a control plane.

**Q: When you say 'control plane' — control plane for what? Your own rhubarb instance, external services/APIs, infrastructure (servers, containers), or something else?**
A: Just stuff in general, like businesses, whatever. The idea is that you have data flowing in (could even be at the prompting of the control plane to collect information) and have a complicated graph of real world things match some kind of explicit spec or spec implied by ontology of semantic graph.

**Q: So the control plane compares 'what is' (data flowing in) against 'what should be' (spec/ontology) and surfaces gaps. What happens when gaps are found?**
A: Configurable per gap type.

**Q: For data flowing in — what are the ingestion sources you're imagining?**
A: Manual + automated mix. Some stuff could be automated (especially if there is a whole "internet" that it is sharing data with), some would be manually entered but manually entered stuff could also be pull in that the control plane could send a task to the human inbox to answer certain questions as a way to collect certain information based on a strategy or schedule or whatever.

**Q: For the 'spec that reality should match' — is this defined as explicit rules, emergent from the taxonomy, or something like a target state graph?**
A: All three layers. The idea is that in a business there are many people whose job it is to act as a control plane so this would be a tool to automate a lot of that functionality at a much lower cost and with possibly with more consistent results plus easily sharing best practices for a domain.

**Q: You mentioned 'sharing best practices for a domain' — does this mean the rules/taxonomy themselves are shareable artifacts?**
A: Yes, packaged templates. Like you could publish a 'Restaurant Management' control plane template that others import.

**Q: When a gap is detected and automated action is configured — what kinds of actions can the system take?**
A: Graph mutations, Message dispatch, External calls, Workflow triggers. Send messages to human task inbox to ask them to take an action and then report back result or to collect some information.

**Q: For the human-in-the-loop tasks — when the control plane asks a human to 'report back result', how does it know the task is complete?**
A: Configurable per task but for example it might send a follow up task "did you finish this" after a certain amount of time if human doesn't report back. It would probably only consider the task "started" if the human "accepts the quest" so to speak. But I think it would generally require some kind of confirmation from the human that the task is completed and what the outcome was, I don't think it would just assume that once it is accepted it is done.

**Q: So tasks have states like 'pending', 'accepted', 'completed', 'timed out'. What about delegation?**
A: The state of the task would be in a state machine and the workflows would be determined by that state machine, so in theory it could do things in a lot of different ways.

**Q: For rule evaluation — when does the control plane check if reality matches spec?**
A: Probably mostly scheduled. Definitely not every frame. It would depend on how important the area was, how easy it was to measure, etc. But it would probably approach things on a similar order of magnitude as someone in a company approached dispatching tasks to their employees. This is more of a timeframe of days and weeks, sometimes monthly or quarterly stuff, not spamming someone with tasks every five minutes.

**Q: For rules that reference external data — does the rule pull fresh data at evaluation time, or does it only see what's already been ingested into the graph?**
A: Configurable per rule but in general it is making decisions based on what is in the graph.

**Q: What does the UI for building/managing control plane rules look like?**
A: Multiple views. Same rules, different editing interfaces.

**Q: For the 'target state graph' layer — is this like a reference architecture?**
A: Probably reference template, constraints. But think about it like a kubernetes controller, it would be able to do arbitrary checks / logic.

**Q: Speaking of Kubernetes controllers — the reconciliation loop pattern (observe → diff → act). Is that the mental model?**
A: Yes, reconciliation loops. Each controller owns a domain and reconciles it.

**Q: How do multiple controllers coordinate? If Controller A and Controller B both want to modify the same entity?**
A: Well they are both actors and they would send messages to the same entity. If an entity is being asked to sustain different contradictory states that would probably trigger some conflict flagging that sends a message to the human inbox.

**Q: For conflict detection — how does the system know two states are contradictory?**
A: I think that this goes into intent territory, the controllers would probably not be sending random mutations there would be some kind of semantic constraint entity (that might have a finite or indefinite lifecycle) that the entity is aware of it trying to conform to. So it would sort of be a little like ad hoc validation rules. That's another thing is that entities have a schema but runtime schema enforcement will not be super strict, so you can have entities that are invalid in any kind of genus. What happens instead is that entities that have validation errors or even anomalous data compared to the norm for that entity type (like it's the only one that has an extra field or has a certain value for an enum or whatever, various heuristics) will be surfaced through inbox tasks or something to users automatically and generically across entities for the most part I think.

**Q: So controllers don't mutate directly — they create 'constraint entities' that express desired state?**
A: They would probably do both in a moment, create the constraint entity as well as ask the entity to mutate. Creating the constraint entity should always succeed and then the response from the entity will let them know whether it succeeded at mutating or not or if there's a conflict (and the controller might just verify entity state after receiving a response to see if it's what it expects anyway).

**Q: For the 'anomalous data' detection you mentioned — is this statistical or structural?**
A: Probably all of these, I think you would have a set of heuristics (that you can always add to) that make sense to a person, including some of the things you are mentioning, vs having an extremely generic statistical approach. But obviously schema violations is a straightforward one.

**Q: Let's talk about observability. When the control plane is running reconciliation loops, how do you see what it's doing?**
A: Well everything would go through the event log and through intent (and author distributed tracing, for example you should be able to see which actions were initiated by a human vs an LLM vs an automated algorithm) it should be very auditable. Like all the steps and changes and states of things should be reconstructable along the whole way. Probably at some point going so far as to have some kind of merkle chain / blockchain-ish thing so that the history is not tamperable. For users I think they would probably be kept in the loop by being notified if something goes wrong and I think I imagine people having essentially subscriptions to certain entity queries so that if there is a particular kind of mutation that matches certain kinds of criteria they would be able to be notified / see a history or log of all those kinds of changes happening.

**Q: For the subscription model — is this push notifications, digest summaries, or pull-based?**
A: Probably not necessarily push notifications, I imagine configurable per subscription and tending more to digest summaries and pull based feeds. I think some of this will just take having the tools and iterating to see what actually works in practice and then paving the cowpaths, etc.

**Q: For the distributed tracing / author tracking — when an LLM or automated algorithm takes an action, how is that represented?**
A: Well similarly to intent you would want to sort of have "authorship" color or provenance to data that can be calculated. So basically all mutations are either triggered by a schedule (ie realtime or cron or whatever) or they are instigated by some kind of agent. I think you can boil agents down to humans, LLM or algorithm but I think you would obviously be interested in attaching additional metadata to any of those scenarios. The main idea is that it should be easy to quickly see what is human (and who) vs what is LLM instigated actions. I think this is also a bet on a security model to handle LLM and human bad actors as well through comprehensive auditability baked in at a pretty deep level.

**Q: For the LLM integration specifically — where do LLMs fit in?**
A: Well basically you have these process workflows that are kind of like state machines/decision flow charts or whatever. Some steps are easily automatable through traditional means, some steps really do require a human, but now there are steps that could be tractable with an LLM sort of filling in for a human employee. As a tangent another kind of provenance or trace data connected to things would be cost as appropriate. So accounting is probably going to be baked in at a pretty deep level and tracking LLM spend for example would be part of that.

**Q: So LLMs can substitute for humans in workflow steps. How do you decide which steps are LLM-eligible?**
A: I think this would be something that is decided as the system is created or that a business owner/individual would "program" where in a sense programming is configuring the workflow and control plane state machine vs programming as we think of it today. But I don't think typically you would be dynamically choosing between LLM and human, it would probably be clear what steps are human and what steps are LLM.

**Q: For cost tracking / accounting — is this just for LLM spend, or broader?**
A: Well part of the idea is that this is kind of a "Microsoft Word for operations" that can be used as a business or just human project control plane. So obviously ideally in any situation like that you would have the ability to be 100% transparent about all costs if you want to and the idea is that this system would make that trivial to do. So costs would not be just the costs of running the control plane it would be any costs that would fall into the domain of traditional accounting. The control plane / event graph would just be trying to facilitate this as fully as possible and it's probably worth baking in as appropriate since it would definitely be a requirement at some point. Obviously not everything happening in the control plane involves actual financial costs though.

**Q: For the accounting side, are you thinking standard double-entry ledger or something more graph-native?**
A: Probably hybrid, again the graph is the "data tensor" and the books would be kind of like a projection or coordinate system of that. It might be that all of this is just trackable on top of this generic infrastructure without any extra allowances and it might be that there are extra things that are helpful. I would say from an accounting perspective the entity graph is not just the entity graph at this moment in time but the event log so basically the entity graph at every point in time.

**Q: What should this feature be called?**
A: Maybe something like "salve".

**Q: 'Salve' as in 'healing ointment' or 'salve' as in the Latin greeting 'hail/be well'?**
A: Greeting/wellness. The system ensures things are well.

**Q: What's the first milestone for Salve?**
A: I think simple user defined workflows that are to help them with reminders/stay on track with some kind of project.

**Q: So the first milestone is essentially personal task management with scheduled reminders and workflow state?**
A: I would say like a user configurable state machine + cron + ability to push tasks to your inbox and then update state machine based on how you complete/respond to task (for example task might give you multiple choice options or collect some fields or whatever).

**Q: For the inbox tasks with multiple choice or field collection — is the task response schema defined in the state machine definition?**
A: The user defined state machine would define what message to send and then what to do with result as far as state changes (maybe with some sputnik pure functions for logic). So the message would be a little like inquirer.js almost? There would be different kinds of tasks/responses you can send (like checkboxes vs yes/no) and it would be easy to do any one of them.

**Q: For the state machine definition itself — where does a user create/edit one?**
A: Probably to start out with STML markup but it's possible to imagine there could be some kind of nice visualization for state machine editing and that would be a separate widget probably. But I think STML would give you everything you need to start.

**Q: For the cron/scheduling piece — does each state machine have its own schedule, or is there a central scheduler?**
A: State machine defines how and when it needs schedule stuff by interacting with some cron-ish API/schema in its definition for different states.

**Q: What does a state machine instance look like?**
A: It would be current state, history and a schema definition. (Which you would expect to evolve over time).

**Q: When the schema definition evolves — what happens to running instances?**
A: Depends on change but I could see a world where you would be able to see based on a schema definition whether it is in an "invalid state" after the change and a lot of it is basically callbacks so it would just use the new callbacks once the definition changes.

**Q: Is there anything about Salve I haven't asked about that's important?**
A: I think that's good!
