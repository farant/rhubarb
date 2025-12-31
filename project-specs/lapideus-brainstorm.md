# Lapideus (New Internet) - Raw Interview Brainstorm

Date: 2025-12-30

## Q&A Session

**Q: What feature do you want to build?**
A: Yes, I think the networking stuff. Basically I want to make a new version of the internet, keep TCP/IP but replace HTTP and DNS. Have our single app that acts as a browser and a server, federated private internets.

**Q: What's wrong with HTTP that you want to solve? Is it the request-response model, the statelessness, the lack of built-in identity, or something else?**
A: I think the model of semantically neutral "pages" with addresses is not very useful. We don't want everyone to have a newspaper, the internet should have structured data built in. So basically the opportunity is to make strong decisions on certain limitations (for example probably nothing like CSS, all monospace fonts through rhubarb, internet only updates once a day, etc.) and the opportunity to build a lot of things in at the protocol level that we now know are a part of the internet as it should be used and should be just built in.

**Q: What's wrong with DNS that you want to solve?**
A: Probably centralization but more than anything you just want to have something that is a clean break from current internet gatekeeping. And again if you are not trying to make semantically neutral addressable articles/pages then I don't know if something exactly like domain names as an addressing scheme even makes sense.

**Q: If not pages with URLs, what IS the addressable unit? An entity? A query? A person's published graph?**
A: Well it depends on what the internet is for. So again it's about the moderate realist trick of not designing for any unknown usage but saying, these are reasonable use cases and then designing specifically for those. For example public forums are very argumentative and probably not good for people spiritually so I don't think they will be included. This will basically be a catholic internet. So you know, censorship built in on a foundational level (I've considered no images at all, still seems possible) so that you have something that is safe for a 12 year old to use or a normal person to use every day without being bombarded by gross stuff. Valid use cases are "journalism" transparency which I think would be more like a "public service intelligence agency" that is updating an entity/event graph in a structured way with sources that can be contested, chain of dependencies for sources, all that kind of stuff. People can subscribe to changes for certain kinds of real entities (like geographical, related to industry, certain public people, etc). There would be globally unique IDs for real people. So the internet would have an ID for Napoleon and you would be able to search all the internets you are connected to for things related to Napoleon. Also to have the information be organized in a browsable way more like Yahoo directories or an encyclopedia instead of everything being search based. The other big focuses will be reading and glossing physical books, taking notes on that, book clubs maybe, stuff like that. Business related stuff for buying and selling. And probably a low resolution video streaming infrastructure so people can do political campaigns cheaply without paying thousands of dollars a month to do video streaming with an archive of videos and call in show that is being live streamed (like extremely low resolution).

**Q: The 'internet only updates once a day' — is this sync batching, rate limiting, or something about how freshness works?**
A: Sync batching.

**Q: For the 'federated private internets' — how do you discover and connect to other internets? Is it invitation-only, directory-based, or something else?**
A: Ideally you create your own internets for family etc, you learn about internets related to your real life area that you join, and then there are publicly available internets that anyone can join that are focused on a certain thing. So some would be invite only some would not.

**Q: When you're connected to multiple internets, how does data flow? Can internet A see data from internet B through you?**
A: Ideally strict isolation but you can see combined data or do operations on combined data locally on your computer. Some internets might act like aggregators and have internet from multiple sources. But there is a difference between what the owner of the internet and the users can configure obviously.

**Q: Identity — is it one global identity per person, or different identities per internet, or both?**
A: There would be differences in identity between a person doing a public activity like running a business, acting as an admin etc, and someone doing private activities. So for public things where someone is in a position of power I don't think they would be able to be anonymous (also anonymous sources might be allowed for whistleblower type stuff but maybe not because anonymous hearsay can be abused obviously). Your identity would be tied to buying the app browser/server for a one time like $50+ price. So you would have a license key that is key to your individual usage and access and in certain cases the license key might be revoked/invalidated.

**Q: The 'globally unique IDs for real people' like Napoleon — who creates/maintains these? Is there an authority?**
A: It would be built into the protocol somehow. Maybe you would use something like a UUID v5 with a hash of the person's name and year of birth or something. There are a lot of factors to balance but I think there's a reasonable way to achieve it.

**Q: For the daily sync batching — what's the mechanism? Does your node push to a central server, pull from peers, or something else?**
A: Well each internet is centralized and has one or more authoritative servers to some degree. Part of the idea is that this could also be delivered via thumb drives in the mail so you could have updates every week or month as well. I think there would be an archive format for updates and the server would only release them on the schedule / clients would only read things before the publish date. But this would probably not be configurable. Once you let some servers publish faster it is just an arms race. It only works if everyone has to play by the same rules.

**Q: What happens if you're offline for a week? Do you get all the daily batches, just the latest state, or something else?**
A: All batches.

**Q: What IS the data format? Entity graphs like rhubarb uses, or something different for network transport?**
A: Probably the same data format as rhubarb but I don't know what kind of technical constraints might require additional changes as we go along. Definitely a binary format, you don't necessarily want to have all the overhead of HTTP where everything has to be text based and parsed. On the lowest level nuntium the rhubarb serializer/deserializer was just an attempt at a protobuf-ish implementation.

**Q: For the 'public service intelligence agency' journalism use case — how does contestation work? If I dispute a source?**
A: Append-only.

**Q: The 'censorship built in' — who decides what's allowed? Per-internet owner, protocol-level rules, or both?**
A: Some things at a protocol level and some things per internet. Like you wouldn't necessarily 100% force every user to be Christian even though it would probably be explicitly a Catholic app with some honoring of Christ/Mary/Saints, etc built in. But you would not have video at all except for the video streaming for people running for office. And you might have an AI content censorship layer or like I said no images at all so that nothing indecent can be posted. Images would only be allowed in very specific contexts if they were allowed and someone who tries to get around these rules would get their license revoked and their content removed/blacklisted somehow whatever internet it was on.

**Q: You mentioned 'no images at all' as possible — what media types ARE allowed?**
A: Well text in particular formats, low res 16 color palette images (everything would be using the rhubarb color palette and widgets, etc). Very low res video streaming for people running for political office, like 192x108 24fps which could include rhubarb screen sharing.

**Q: For commerce — is payment built into the protocol, or do you use external payment and just record the transaction?**
A: Somehow it would have payment built in, in the sense that it's not something 3rd parties would have to implement or provide. There might be a plugin system for integrating with different payment processors. One of the limitations of this app is that probably after some period of private beta there would be a 1.0 release and that's it. There would not be further versions, taking inspiration from C89 you want a stable base for people to build on.

**Q: For books and glosses — are the book texts themselves on the network, or just references/annotations?**
A: I would try to bundle as many public domain texts as possible with the app itself in the binary, so for example it might include the whole Project Gutenberg corpus. People would be able to sell digital copies of texts and people would be able to work with books they own physical copies of but don't have a digital copy of the whole text (like add quotes they like, highlights, etc.) The idea is that books will always be around a part of our life and they are an important part of the intellectual life of culture so just try to make something that catalyzes and supports working with physical books. So books would have canonical IDs for each edition, ideally would have numbering systems for referencing particular sentences in any public domain text and tools to add that kind of canonical addressing to newly published books as well, given a particular edition. So a little like Xanadu in that sense.

**Q: At the TCP level — is there a persistent connection between your node and the internet's server, or connect-on-sync?**
A: Not a strong opinion, probably depends on practical details. Although it would be nice if you could turn on the internet once a day, get the whole update, and then turn off the internet for the rest of the day.

**Q: For the low-res video streaming (political) — is this live or pre-recorded, and how does it fit with daily sync?**
A: Live + archive.

**Q: For encryption — is all traffic encrypted, and how are keys managed?**
A: I mean you probably would want some kind of encryption but I don't think the goal is to make something unreadable by state actors, etc or present it to people as something unreadable to state actors. You probably would encrypt it just similar to how everyone uses HTTPS now so people on the TCP path are not super easily snooping but I think encryption would be more important with signing / validation. Probably will be Merkle trees involved at some point because you want to have an untamperable history of anything that is in the public sphere for sure.

**Q: If someone's license gets revoked — how does the network know to reject them?**
A: I'm not sure how this will work. Honestly the internet is just like a human society where it needs a governance structure and people with the authority to punish (in this case the power is revoking key). There need to be consequences for abusing authority. So this is almost a "government" problem as much as a technical problem, although anything that can be cleanly solved 100% technically is great. You might have a distributed blockchain / Merkle tree event log of revoked keys (and the revoking can itself be revoked). So basically mods or the app itself could add people to that, people would have the ability to appeal, there would be consequences (like key revocation) for frivolously putting people on the list. You want to have as much untamperable public history as possible so if people do stuff like this they can't be anonymous and you can see a web of people they are associated with (through intelligence agency) so you can't just use kamikaze individuals tactically without eventually there being consequences for the organizer.

**Q: Creating a new internet — is this something any license holder can do, or restricted?**
A: Anyone.

**Q: What's the relationship between 'the app' and 'the internets'? Is there one canonical app, or can others build clients?**
A: You would try to have one canonical app, but purchasing the app would probably include the source code so people could be technical consultants, etc. You would make some effort to have there be one canonical app but ultimately I think what would preserve that long term would be higher quality (like C89) and network effect.

**Q: For browsing/navigating data — what does the interface look like? Tree navigation, search, feeds?**
A: Well here's where you have the finite paradigms. So it depends on what you are using it for. There is the book sphere stuff, there is the news/public intelligence agency stuff (basically also providing data for all the Salve control planes potentially), you would probably have something that is kind of like a science encyclopedia with very granular sciences like "the science of running a dog walking company" organized hierarchically, you have the political live streaming stuff, and then you have business marketplace things. That is like 90% of what it would consist of. There might be 1 or 2 extra applications I'm not thinking about or am forgetting but that is the order of magnitude we're talking about. So you would have logical and ergonomic UIs for all those activities.

**Q: For private internets (family, local) — is there a concept of 'chat' or real-time messaging?**
A: I would say probably no real time. Lean towards saying people use other things for messaging, but there probably would be messaging at least for business stuff (and neither party are anonymous in these cases).

**Q: For the 'science encyclopedia' with granular sciences — who creates/curates this? Is it crowdsourced, authoritative, or something else?**
A: It is federated. So you have an encyclopedia based on the internets that you want it to be aggregated from.

**Q: How does the app know which 'sphere' to show? Is it per-internet, per-data-type, or user-selected mode?**
A: User mode, it would be like switching between widgets in rhubarb.

**Q: What's the first milestone for this? What's the minimum to prove the concept works?**
A: Probably the book or intelligence stuff. Probably a mobile progressive web app with offline capability syncing with laptop where server is running. Users being like me and my wife on our phones and on our computers.

**Q: For the name — do you have something in mind, or should I suggest options?**
A: Probably the whole rhubarb/internet app (same thing) will be called Lapideus. Rhubarb being the collection of C libraries it is built with.

**Q: The PWA syncing with laptop server — is the laptop always the authoritative 'internet' server, or can mobile also serve?**
A: Laptop is server.

**Q: For the PWA — is this a separate codebase, or rhubarb compiled to WASM, or something else?**
A: Probably separate JS/TS that is bundled and the binary can serve to install on the phone but drawing to canvas and keeping the same low resolution/visual vocabulary as desktop app but different widgets, etc because of mobile form factor.

**Q: For the sync protocol between PWA and server — is this the same as inter-internet sync, or different?**
A: Might be the same, it is basically solving the same problem. Difference would be that it's running on a phone and using whatever storage is available to a PWA to sync. There's also the difference between different people using the internet and me using the internet on different devices. Like I would want stuff to sync between my devices on demand not following the publishing schedule. Plus there is extra non-internet like PKM, Salve, etc functionality built into the app that would be syncing between the phone and the computer for the same user.

**Q: Is there anything about Lapideus I haven't asked about that's important to capture?**
A: Covered everything.
