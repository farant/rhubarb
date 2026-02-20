

- we should be able to compile syllogisms into dkc
- can think of execution as words -> tokens -> net -> tokens -> words. so words are our input format and our output format.
- one of the limitations of current llms is the finite token set. you cannot use words that are outside of the token set as input or output
- because we are compiling our net and have granular transparency of its execution we can introduce "passthrough tokens" or "opaque tokens" or "argument tokens", whatever we want to call them
- for example a syllogism (or any kind of function) does not necessarily need to the value of its arguments, just their logical nature. ie term of predicate, etc.
- execution could somehow track if opaque token makes it to output and then replace it with the initial input value.

- if you present syllogisms to net, for example "all cats are mammals", it should be able to compile that syllogism and somehow dynamically update and add that syllogism to its weights.
- this means presenting syllogisms to the net can be a training approach
- if you ask the llm a question like "are bears mortal?" and it does not have a "bears" subnet yet to answer that question, it can recognize that it does not know if that syllogism is true and bears can become an opaque token it uses in its reply to say "i don't know if bears are mortal" 

- if we can decompile traditional llms to knots, we should be able to wire together our compiled net with a traditional llm.
- this is not using them as some kind of pipeline where one outputs tokens that are then input to another
- they would be literally wired together into a single set of weights with connected pathways
- my theory is that having the compiled syllogisms as part of its weights will actually help the "traditional" llm regions become smarter much more rapidly on an extremely small training corpus

- part of designing the syllogism / compiled part is that we will want to build a framework for deterministic natural language parsing. 
- this can start simply with classic syllogism type grammer and formats like Aristotle is a man, etc.
- over time this should develop to the point where it can build a data structure ready to feed into the right tokens/opaque tokens structure for the compiled net from a paragraph of text.


