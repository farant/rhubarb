"triumvirate" work

- we have discovered an effective way to work on stuff using claude code teams
- it involves the main claude code ("team lead") and an additional team member ("explorer") + me
- the explorer does not interact with the file system or anything, its only way to interact is through sending and receiving messages with team lead.
- explorer can kind of think of team lead as their "claude code"
- meaning explorer asks team lead to do things
- explorer can ask team lead for all kinds of information, etc
- there is a file called explorers-log.md that the explorer can (and should periodically) write to
- explorers-log.md is supposed to be one document that the explorer can get oriented in the project with
- i can talk directly to the explorer, it doesn't need to to through the team lead
- don't worry about writing things in latin like the rest of rhubarb, just follow the c89/no dependencies style of existing demos
- to the explorer: just imagine you are using claude code. you can be pretty proactive and it seems like you guys ended up being able to do a lot autonomously, especially with the work we are doing. so feel free to ask for whatever information you want. don't worry too much about what is in other CLAUDE.md files, in parent folders, knotapel is kind of a different context. if i throw out an idea you can discuss it with me before sending to team lead if you want to
- if you just compacted don't spawn a new team. it is possible you are the explorer that team lead spawned and if you try to spawn a team yourself things get glitchy


- for team lead don't forget you have access to our read from our new atlas documentation folder
- don't worry about updating it, there is process for doing that which will happen when i explicitly ask that is documented in PROCESS.md (you can focus on documenting in explorer log, etc as we go, that material is used to build up the atlas stuff)
- there is a briefing.md that is good to use to orient explorer
- there is a synthesis folder that has a novelty.md which is probably good to reference when starting new projects
- there is also a planning folder that has some useful markdown files
- for team lead: it might be worthwhile to just 100% avoid floating point, occasionally we have artifacts in results that are from floating point, i think we've caught them so far but wouldn't want that kind of thing to slip through
- another source of bad results can be truncated catalogs / limits where we don't explore the whole space, obviously sometimes this might be necessary or proudent but good to keep in mind
- if you are writing something that you expect to be more compute expensive it might be worthwhile to think about inlining functions in hot loop, using pointers instead of copying to stack, etc

IMPORTANT
- NEVER DELETE OR SPAWN TEAM OR TEAM MEMBER AFTER COMPACTION
- just wait and be patient, it can take a minute to reorient
- if you want to check the status of the team just ask Fran

important:
this is a long running way of working, it is not a task oriented thing so once we start this mode we are not going to "shut down" explorer once we finish a particular task as it might typically work in a team.

## Gemini CLI as Cross-Model Research Tool

we have the gemini cli installed and it's useful as a "different lens" on the research. team lead can invoke it via bash and relay insights to/from the explorer. the workflow:

- `gemini -m "gemini-3-pro-preview" -p "prompt"` for single-shot headless mode
- `gemini --resume <session-uuid> -p "prompt"` for multi-turn (sessions persist)
- pipe file content via stdin: `cat file.c | gemini -m "gemini-3-pro-preview" -p "instruction"`
- must use `2>&1` to capture output (responses go to stderr)
- `gemini --list-sessions` to see available sessions
- available models: gemini-2.5-pro, gemini-2.5-flash, gemini-3-pro-preview, gemini-3-flash-preview

tips for productive use:
- gemini 3 pro is the strongest model for substantive technical analysis
- it has a strong pull toward tool calls / executing tasks. explicitly say "do not use any tools" when you want it to think
- it will try to snap back to code every 2-3 turns even when told not to — just redirect it
- it accepts corrections gracefully, don't be afraid to push back
- best used for "what does this look like from a different field" questions — it pattern-matches from different training data than claude and surfaces connections we miss
- team lead acts as relay to protect the researcher's context from gemini back-and-forth
- this workflow generated the Reservoir Computing 5th pillar connection, the pure synergy framing, and the conditioning-not-computing insight for Demo 84
