# Brighton and Lapide: Fran's vision, verbatim

2026-09-23 · Fran

*Fran's own statement of where Lapide stands and what Brighton should become,
preserved WORD FOR WORD as a shared reference. Nothing here is decided or
planned yet: this is the source text that later design conversations pull
from. Commentary, decisions and regions live elsewhere (the tabularium and
future specs) and cite this file; do not edit the text below — add a dated
note at the end instead.*

---

At the moment the Lapide.org translation is complete but it's very much a first draft. The work is stalled but I have been reviewing all the translated English chapters and comparing against the original OCR as well as when needed the original PDF pages to look for issues. So far every chapter has discovered some issues and often the issues are significant. So it is definitely necessary work to do for the quality of the material.

Part of the reason I have stalled is because I have been busy with other programming projects. Another reason is because the long-running Claude conversation I was using to do review accidentally was terminated and now I am going to have to bring a new agent up-to-date with the reviewing process and make sure we are on the same page which can be a bit tricky because historically it is easy for agents to take license in cutting corners which is where most of the issues come from in the first place. 

All that being said I think another part of the reason it has stalled is because I need to expand the scope of what Lapide is doing and I'm not sure exactly how to approach it.

One thing that I need to do is to add images from the original PDFs. OCR is not perfect, it can misread words or drop sections. A floor of quality is to be able to compare the original PDF page image to the Latin version of the text (not necessarily raw OCR) and to the English version of the text.

This presents some challenges. An immediate concern is the hosting cost. Right now the lapide.org Github repo is very large but it's mostly all HTML. Once I have PNGs or JPEGs for every page of ~twenty 800 page PDFs the storage requirements are going to increase substantially and I'm not sure if Github will support hosting a repo of that size. This suggests that I need to figure out a more complicated hosting / deployment scenario. There are actually quite a few different options here. A related concern is that my standards for managing other hosting setups are influenced by gitops and are high.

The second consideration is UI. Right now the UI for lapide.org is very minimal and I am happy with it. It will be necessary to iterate on the UI to add the split view with the individual PDF pages. Probably the existing UI will stay as-is as a default mode, but figuring out the design of this more "paginated" mode is another open question with a breadth of different possibilities. 

The third consideration is that this will be an annotation heavy pass and there are open questions as to how heavy is practical and desired. At the very minimum there must be some mapping of the existing texts (in Latin and English) to the granularity of individual PDF pages. But there is also a question of spatial mapping of text to regions or columns of the image, mappings sentences or paragraphs in the Latin version of the document to the English version, etc. One of the factors here is that any pass that touches all of the text is time consuming and labor intensive. Where in a different situation you might choose to accomplish something through multiple iterative passes in this case there is an argument for batching as many changes as possible into a single pass. But it's a balancing act because if you make the overhead of work too high it can overwhelm the agent and harm the quality of the results (or slow down progress to such a crawl that no improvements land for the whole body of texts).

These are all specific to PDF pagination but there are a few other big issues to think through. I probably want to make my own Github-like platform for people to submit issues about the texts. I want to dogfood rhubarb for web servers. Lapide is not the only text, I want to have a similar standard of quality for many texts. I want to have a reader experience for people, potentially kind of like a social network for people reading scholastic texts (while avoiding what I perceive as the pitfalls of social networks). I also want to make a standalone desktop application people can use to do these kinds of translation projects on their own. 

Each of these involve a lot of design thinking but potentially benefit from being implemented in tandem.

Lets talk first about the Github idea. Right away there are some clear distinctions. I don't want to try to run Github. I don't want to try and host gigabytes of arbitrary PDF pages for people. So this might be a platform just for my own translation projects to start and then long-term it would probably have a more federated design where I am able to provide a list of servers I am interested in and then view all the projects across those servers in one user interface. So this means that I would be providing a server binary that other people might host and run on their own VPS. This seems like it could fall into the "1.0 and done" product philosophy. 

Why do I want to have my own Github project? Well it would be awfully nice to have issues that are specific to the structure of the text. It would also be nice for people to be able to see a history of issues that are connected to the text. 

Part of what I would like to implement for texts is an addressing scheme. The addresses would be something like:

c1.s4.pxyz7.abcd8.2

That is the address of an individual sentence. c1 stands for chapter 1. s4 stands for section four. pxyz7 is the identifier for a paragraph. I will have a special kind of nanoid that is based on a hash kind of like uuid5 (using the Crockford alphabet). For paragraphs the hashed value is a merkle tree of all its sentences. So if one sentence changes then the paragraph id also changes. Paragraphs ids would be linked with essentially redirects, so if pxyz7 becomes p123a a link to pxyz7 redirects to p123a or whatever the latest version is. Last is abcd8.2 which is an id built on hashing the sentence text. It is globally unique for the chapter. But chapters can have more than one sentence with the same text value. This is what the .2 disambiguator is for. Again if the sentence changes it gets a new ID but old versions of the sentence will link to new versions. The trailing .2, etc. disambiguator is globally unique across history and based on minting order not text order, so you could have sequential sentences with the ids abcef.10 then abcef.3.

Section and paragraph ids are optional in the address since sentences are globally unique for the chapter. c1.s4.pxyz7.abcd8.2 could be exchanged with c1.abcd8.2 if desired. But paragraphs and sections are addressable themselves for deep linking purposes so an ID might be c1.s4.pxyz7 or c1.s4 (or c1.pxyz7) and it will simply go the beginning of the paragraph or the section.

The kind of editor I'm imagining should probably be a web application first, even if I might use my "vitrea" tauri-style webkit wrapper to make it a desktop application as well. I'm thinking of naming this app/project "Brighton".

So for example you might be able to link to something like brighton.golden.earth or brighton.lapide.org/lapide.c4.abcde4 and view a particular sentence. This would also be federated ideally so if someone mirrors the text potentially it could either work directly or redirect to go to brighton-server.somesite.com/lapide.c4.abcde5 as well. This way you can have a distributed network of texts with links between each other.

The web app user interface would encompass a few different functionalities. 

First it would be a reader. It should let you highlight/favorite sections of text. It should let you read in an ebook-style paginated view. It should let you read paginated-by-sentence as well if that's what you prefer. I think it would let you create a "reading group" where you can share your highlights and notes with other people. It should let you write your own marginalia or glosses to the text. It should let you group together highlights and extracts and notes into an "essay" or anthology format. Obviously in this case it would have to support multi-tenancy, user accounts and all that stuff. The library of texts should have annotations so you can view commentary based on particular Bible verses, saints, etc.

Second is the "github" element. If you want to do a deeper dive into the text you should be able to see the transcribed Latin or the original PDF image of any part of the text you are looking at. If you see issues you should be able to submit them anchored to one or more sections of text. Ideally you would also be able to see the "rubric" or instructions that was given to AI for translating the text. This would be per language in general but also specific to certain terms like dulia or hyperdulia that you might want to handle in a particular way. Ideally all these issues would be public and you would be able to see what issues other people had submitted as well. Then there is the "text administrator" side where you would be able to see your submitted issues, manage or resolve them, etc.

Third is the publishing element. Translation is primarily through AI, that is what it would be catered towards, although obviously it would allow manual translation if desired. So this would probably involve creating tasks for the LLM, tracking if they are done, all that kind of stuff. Ideally the entire contents of a book including sentence metadata, PNG images, etc can be stored in a single file. Probably a sqlite database to start, maybe a .brighton file. A lot of the questions here have to do with editing ergonomics for LLMs. Probably you would be able to "mount" the .brighton file as a .git repo of many individual files that synced back into the single file so that an LLM would be able to more easily edit and read the contents. You might also publish it as a git repo as another way that people would be able to mirror the contents if desired, view the publishing history, etc.

All of these elements need to be multilingual long term. At a minimum you are going to tend to have English and Latin but Lapide for example already has some translation for 35 different languages. Every translated sentence of the text for a given language should have the translation rubrics that were used at the time the sentence was translated. So for example you would be able to search for text whose last version was translated in an outdated version of the rubric, etc. Translation rubrics cover things like how to handle specific terms, how to do Bible quotatations, how to handle saint names, translation register and all kinds of stuff like that.

Ideally this would also support indexing. So show indexing coverage, entries for particular people or places with links back into the library of texts where they occur, shared / deduplicated entities across works.

Another concern that goes along with indexing is essentially the "encyclopedia" element. For a given text as well a library of texts as a whole you might want to have AI generated and/or human edited encyclopedia entries for saints, places, historical events or references to other works. Multiple works in the system, especially since AI can create public domain translations of public domain works, can be included and correspond to the kind of stable sentence/paragraph etc. references that I mentioned earlier. So ideally over time if Lapide references a sentence from "the 10th sermon of St. Basil the Great" that can be a deep link into a translated version of the sermon itself. Over time this would also be multi-lingual, so if you are reading the Polish translation of Lapide it would link into the Polish translation of St. Basil the Great if that exists. Falling back to English or some other language preference if Polish was not yet available. Encyclopedia entries themselves would have translations and would want to keep track of their translation rubric, date of translation, model used, etc.

A fourth major element that is not strictly necessary but that I am personally interested in and that I think would add a lot is language learning. You would want to have a deeply integrated set of tools for someone who was studying one or more languages with the texts in the system as a starting point. So this might include things like flash cards, memorization games, quizzes or translation exercises graded by the user's personal AI subscription of choice.

Over time you would want to allow people to export and share their own authored language learning materials with others so you might have full blown "courses" to some degree that would develop.

To some degree this might be the right project focus for me for a product in the next year or so. It's the kind of thing that can prove a lot of the "private internet" technology and long term it might even be a more exciting focus for me because I'm interested in all these Catholic texts.

Long term this would not just be for Catholic texts, I think you would prove it in that domain but expand it to all scholarly domains. So you might have compatible resources for math, medical texts or any other science domain based on published books and papers. For some of these you can imagine an expansion of scope where some "Jupyter notebook" type functionality would be embedded to allow for interactive simulations and all that kind of thing. The base computational artifact for any domain would be a single compileable c89 file.

This would also expand the "courses" or interactive learning layer of language learning where you might have courses about a particular subject that would be based on walking through primary sources and papers on a particular subject. 

This expansion into new subjects obviously would increase the importance of the encyclopedia dramatically and the number of subjects it would be required to sustain. Eventually it actually would become something more like a proper encyclopedia with anchoring in the papers and texts also available to readers in the system as the sources for the content of its entries. 

I think at some point you would also introduce something like a syllogism or claim graph. Where you would try to show the linking of claims based on other claims and this would be particularly useful in open questions, it would allow you to see the point of departure where the basis of disagreement might be, to see the ripple effect if a particular claim was verified, disproven or qualified, etc.

So over time this is a very large system but I think it is definitely something that I want to make and I think it is also something that rationally should exist. And to some degree must be made as an integrated whole covering all these domains to be able to function the way it should be able to.

In some ways this is just an evolution of the internet. The internet as it is with how unplanned and emergent the structure is works very well all things considered. But if you had the resources and were designing it to be optimal for shared scholarship having the benefit of the past 30 years of usage to reflect on you would probably do some things differently or more intentionally in certain areas.

---

## Notes (append, dated)
