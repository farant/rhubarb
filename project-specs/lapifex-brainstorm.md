# Lapifex Brainstorm - Raw Q&A

## Motivation

**Q: When you say 'build a C compiler' — what is the thing that makes you want one? Is it about compiling your own rhubarb codebase with something you fully control, is it about having a compilation step that feeds into a larger toolchain you're building (like an OS), or is it more about the compiler being a forcing function that makes you deeply understand C semantics?**

A: I want to be able to compile and execute C code from within rhubarb, so basically be able to use C as a scripting language. Have full control to be able to do debugging stuff, all that kind of thing.

---

## Parser / Analysis Architecture

**Q: The brainstorms doc describes a sophisticated analysis tool (layers, roundtrip, trivia, error recovery, querying). A compiler typically throws most of that away — it just needs correct semantics. Do you see these as one parser serving two masters, or two separate consumers of a shared lexer/token infrastructure?**

A: Definitely still want to keep all the analysis/roundtrip stuff. It would be nice if that layer had access to type checking as well (we will want to do type checking). If it is dramatically easier to give the C compiler its own parser I would be open to that but it seems like maybe we would have one parser that serves a lot of functions.

---

## Execution Model

**Q: So you want to JIT or interpret C code at runtime within rhubarb — not just compile to a binary ahead of time. That's a very different beast from a traditional compiler. Are you thinking something like TCC (Tiny C Compiler) where you can compile C to machine code in-memory and call it, or more like an interpreter that walks the AST, or a bytecode VM approach?**

A: Probably JIT to native but it would maybe execute in a forked process for debugging purposes. Would be open to other interpretation modes for debugging/introspecting purposes however.

---

## Debugging UX

**Q: When you say 'debugging stuff' and 'use C as scripting language' — what does the debugging experience look like in your head? Is this stepping through your own rhubarb code with full source mapping, or is it more like 'run this C snippet and inspect what happened'?**

A: It would start simple but basically you want to make an ideal C programming environment in rhubarb. It would probably support literate programming so that it fit the no-scrolling + pagination rhubarb model. So you would want to be able to step, visualize memory, basically have open ended ability to make it as good a debugger / development experience as possible.

---

## UI Context

**Q: You mentioned literate programming and no-scrolling pagination. So the C development environment is embedded IN rhubarb's UI model, not a separate terminal tool. Does that mean rhubarb already has or will have its own rendering/UI layer, and the C environment is a 'mode' or 'view' within it?**

A: Yes, rhubarb has a GUI environment with some amount of development at this point. You can see what is there by using probatio_combinado as an entry point.

---

## Target Architecture

**Q: JIT to native in a forked process is interesting — that gives you isolation (crash safety) plus the ability to ptrace/inspect the child. For the JIT backend, are you thinking about targeting one architecture (ARM64 since you're on Apple Silicon), or do you want architecture abstraction from the start?**

A: Just ARM64 for now. I think the main target platforms are MacBooks and possibly Raspberry Pi someday. I don't anticipate porting to x86 any time super soon.

---

## Type Checking Strictness

**Q: Type checking in the analysis layer is a big deal. C's type system is deceptively complex (implicit conversions, integer promotion rules, compatible types, composite types across translation units). How complete does type checking need to be initially?**

A: I would like to be able to be very strict. So full C89 + optionally Dependable C (for example no floats) + arbitrary linting rules basically.

---

## Parser Architecture

**Q: arbor2 has a hand-written GLR parser with hand-written state tables. The specifier explosion we just hit shows this doesn't scale well for combinatorial grammar rules. For the compiler+analysis system, what parsing approach?**

A: I don't know to be honest, I think the reason we switched to GLR is because we want to handle ambiguity and errors. So I don't know if it's possible to meet all our goals approaching it another way or not. But I would say that code generation we control and write seems like it could make sense if we stick with GLR. Or restructure grammar if that is possible could be good too.

---

## Debug Bridge

**Q: The forked-process JIT model means the compiler and the running program are in separate address spaces. How do you envision the debugging/introspection bridge?**

A: It would probably be some combination of all three (OS debug APIs, instrumented code, shared memory). We would definitely be able to insert instrumentation, I think there could be shared memory in some cases, but I'm sure processes would have their own memory in other cases and we would want to have full transparency.

---

## Literate Programming Model

**Q: Does that mean rhubarb's C environment stores files in a literate format and the parser/compiler operates on tangled output, or does the parser itself understand literate documents natively?**

A: I think probably it would be up to a different layer to present the parser with a set of translation units. That being said, we may want syntax highlighting and go to definition and all that stuff so there probably would be some parsing layer that is aware of the format / structure we are storing things in which is not really file based. But this would be able to be projected into the filesystem as code based on user instructions.

---

## Code Storage Model

**Q: You said 'not really file based' for how code is stored. Are you thinking of something like a structured database/document store where code lives as semantic chunks rather than flat files?**

A: At the moment rhubarb is kind of like Acme, you can actually see a mobile app that is in the ../prunifex folder that is also following a similar UI model. There is a non-scrolling set of pages that can have various different things in them like $commands, XML, etc. So the C code would probably be on pages that have a special XML tag or embedded in tags and spread out across all the pages, text of which is ultimately our "source of truth."

---

## Parser Generator

**Q: A parser generator that outputs GLR tables from a grammar spec would give you the best of both: declarative grammar + GLR execution. How do you feel about that investment?**

A: I think it could definitely be worth it if that seems like something we can figure out and execute on.

---

## IR Design

**Q: What's your mental model for the intermediate representation?**

A: I have zero experience with this so I will be leaning on your expertise for now. I will say that we are not trying to make the most optimized compiler, to start having everything running will be success. I don't think we are ever going to prioritize being the most optimized compiler because that sounds like it could be a big investment that is not necessarily worth it for our use case. That being said a moderate amount of optimization that is low hanging fruit could definitely make sense. So three-address code seems like it would be a good middle ground. We can probably rule out SSA-based for now.

---

## JIT Granularity

**Q: Do you envision compiling a whole translation unit at once, or function-at-a-time?**

A: I could see having function-at-a-time support in some cases through some kind of hot reload mechanism. Otherwise I would kind of imagine creating a unity build translation unit on behalf of the literate code and compiling that.

---

## Error UX

**Q: How much do you care about error message quality?**

A: Good enough might be adequate, but there will be UI design challenges here that I think will take some hands on experimentation to figure out best approach. It will probably be pretty integrated and will want to stick to our "no scrolling" UI rules. So we will see what kind of affordances end up making sense like popovers or paginated error logs or what.

---

## Theory / Implementation

**Q: For the GLR table generator — are you comfortable with me designing this, or do you want to study the theory together?**

A: Probably you can design it and implement but I am always happy to learn, I'm sure the more I understand what we are doing the better I will be able to contribute with decisions and stuff.

---

## Optimization Scope

**Q: Should IR support optimization across function boundaries (interprocedural) or is each function optimized in isolation?**

A: Start per-function, design IR so interprocedural could be added later.

---

## Hot Reload

**Q: If you recompile a function and patch it into a running forked process, how strict should hot reload be?**

A: This is probably going to involve library functions and coding conventions (we will be able to lint for!) so it is hard to say exactly what it will look like. But probably since it is running in a separate process we will let the developer make a dumb decision and crash their process potentially. My best reference for this is however they did it in Handmade Hero.

---

## Standard Library

**Q: When JIT-compiling C in a forked process, your code will want to call printf, malloc, etc. How do you plan to handle libc?**

A: I think we will use system libc but we might have a layer of abstraction so we can do Latin names potentially and also provide mocks to have granular control over what code can do to host.

---

## Capability / Sandboxing

**Q: Is the libc wrapper about security (untrusted code can't do file I/O) or more about testing (mock malloc to simulate failure)?**

A: Both equally.

---

## First Milestone

**Q: What feels like the right first milestone?**

A: It seems like probably we would do something like parser → type checker → expression evaluator → hello world. But I am open to breaking it down in different ways. It seems like there are plenty of different milestones along the way.

---

## Naming

**Q: What should the compiler be called?**

A: Lapifex.

---

## Relationship to Arbor

**Q: Does lapifex depend on arbor, or does lapifex subsume arbor?**

A: I would say lapifex can be a consumer of arbor but if we lean towards a fresh start on the parser I would just call the whole thing lapifex potentially. But whatever structure makes most sense from an implementation perspective.

---

## Parser Generator Scope

**Q: Should the parser generator be general-purpose or C-specific?**

A: It is specifically designed for our use case, including stuff like roundtrip byte-for-byte identical input → parse → serialize, so whatever extra stuff we need for trivia, macro expansion layers, etc.

---

## 6-Month Goal

**Q: What does 'success' look like for lapifex in 6 months?**

A: End-to-end demo: write C in rhubarb pages, compile, run, see output, basic debugging.

---

## Self-Hosting

**Q: Do you envision lapifex eventually compiling its own generated parser tables?**

A: Not a priority.

---

## Library Decomposition

**Q: Where would you draw the library boundaries?**

A: Whatever seems to not get in the way of development and best facilitates unit tests.

---

## Test Compatibility

**Q: How important is it that the new system passes all existing arbor2 tests?**

A: I think we want to test the same C expressions and input files but it doesn't have to be like a 1-for-1 library drop-in. There are still even arbor v1 tests that we might want to pull in as we go. I would say that typedefs and macros in the depth of their potential complexity have been the stumbling block both times so it might be worth focusing on that early in the process as makes sense.

---

## Typedef / Macro Strategy

**Q: Do you want lapifex to tackle the typedef/macro problem head-on as a core design principle?**

A: I think two-pass makes sense, we just have to be able to handle arbitrary macros. And we also want to parse files where we don't have all the includes and don't necessarily have the file where macros or typedefs are included and still be able to heuristic what identifiers are likely types in most cases. So I would say the crux of the parser challenge is arbitrary macro expansion/layers, typedefs and byte-for-byte roundtrip. Those are good things to focus on testing and getting right as early as possible. It also seems like it is good to test on complete files early vs just expressions/statements.

---

## Semantic Predicates in Grammar

**Q: Should the grammar format be able to express semantic predicates like 'this identifier is a typedef name'?**

A: I would use the arbor2 GLR as an idea of what kind of code we want to be able to generate. It seems like in a lot of ways it was on the right track, just it was dealing with combinatorial explosion.

---

## Trivia in Grammar Format

**Q: Should the generator's grammar format have explicit trivia rules?**

A: I don't know if we need to have any explicit rules about it, more like if the code generated by the grammar needs to handle trivia or other concerns we would want to be able to support that.

---

## Demo Scope

**Q: What subset of C89 would make the 6-month demo feel real?**

A: I am comfortable either doing a layer at a time deeply or doing an integrated version end-to-end that we deepen as we go. I have had success in the past building modular libraries with lots of unit tests but we can certainly be building them out in tandem at the same time.

---

## Debug Minimum

**Q: What's the minimum debugging that would feel like 'yes, this is a debugger'?**

A: Probably breakpoints or step-through.

---

## Project Layout

**Q: Where does lapifex live in the rhubarb monorepo?**

A: In lib/.

---

## Summary of Key Decisions

| Topic | Decision |
|-------|----------|
| Name | Lapifex ("stone-worker/mason") |
| Motivation | Compile + execute C from within rhubarb as scripting language |
| Execution model | JIT to native ARM64, forked process for isolation |
| Parser approach | GLR with parser generator (eliminate hand-written state tables) |
| Grammar format | Custom format, possibly using STML parser |
| IR | Three-address code, per-function optimization initially |
| Register allocation | Linear scan |
| Type checking | Full C89 + Dependable C + custom lint rules, multiple passes |
| Typedef strategy | Two-pass (collect then parse) + GLR forking for unknown |
| Roundtrip | Byte-for-byte, core design requirement |
| Macro handling | Layered expansion model from arbor2 brainstorms |
| Libc | System libc with Latin wrapper + mock/sandbox layer |
| Hot reload | Function-level, developer-responsible, Handmade Hero style |
| Debug bridge | Combination: instrumentation + shared memory + OS APIs |
| Code storage | Page-based (rhubarb UI), projected to filesystem |
| Literate programming | Separate tangling layer + page-aware parsing for IDE features |
| Error messages | Integrated in no-scrolling UI, good enough quality |
| Target | ARM64 only (MacBook + future Raspberry Pi) |
| Project layout | lib/lapifex_*.c in rhubarb monorepo |
| Relationship to arbor | Lapifex may consume arbor or subsume it depending on architecture |
| Self-hosting | Not a priority |
| 6-month goal | End-to-end demo: write → compile → run → debug in rhubarb |
| Test strategy | Reuse arbor2 C input files, not 1:1 test port |
| Key challenge | Typedefs + macros + roundtrip — focus on these early |
