# Mercatura (Commerce Sphere) - Raw Interview Brainstorm

Date: 2025-12-30

## Q&A Session

**Q: What feature do you want to build?**
A: I think it would be good to talk about the business/marketplace side of the internet facet.

**Q: When you say 'business/marketplace' — are you thinking primarily about transactions between strangers (like eBay/Craigslist), or more about structured commercial relationships (like B2B contracts, recurring orders)?**
A: Both equally.

**Q: For the identity side — you mentioned in Lapideus that commerce parties must be non-anonymous. Does this mean full legal identity (name, address), or just 'accountable pseudonym' (license key traceable but not publicly exposed)?**
A: Full legal identity.

**Q: For payments — you mentioned payment being 'built in' but with plugin support. What's the relationship between the marketplace and actual money movement? Does the protocol escrow funds, or is it purely a coordination layer that records payment happened externally?**
A: I don't think there would necessarily be anything as complicated as escrow but I am not an expert on payment systems so it's possible that would be involved. The goal is to have people be able to sell digital and physical goods and ship/deliver them to a global market and collect payment (although I guess right now I am thinking of hard coding Lapideus as United States only) basically it would be about facilitating the kinds of transactions that are currently happening usefully on the internet.

**Q: For US-only scope — is this a technical/legal simplification (one jurisdiction, one currency, one tax regime), or is there a deeper philosophy about starting local before going global?**
A: Well there will only be a v1, like C89. In general I think that people would benefit from more national and lower internets vs global internet. I guess I could see the non-internet features of Lapideus being available elsewhere but the internet itself being USA only.

**Q: For 'digital goods' specifically — in Lapideus you mentioned books could be sold. What other digital goods do you envision? And how does ownership/licensing work — can a buyer resell, or is it non-transferable?**
A: Well people can sell digital things that are data usable in Lapideus or they can just sell arbitrary files probably.

**Q: For physical goods — how does shipping/fulfillment get coordinated? Is shipping info exchanged through the marketplace messaging, or is there structured 'order' data that includes address, tracking, etc?**
A: Well the idea is that you want to have as much digital infrastructure as possible to let small sellers give people good customer experience even if shipping and fulfillment, etc is not really their core competency.

**Q: For 'helping small sellers' — does this mean the protocol provides templates/workflows for common scenarios (order confirmation, shipping notification, delivery follow-up), or actual service integration (connect to USPS API, print labels)?**
A: Ok, so the big idea of this is that you are trying to fix the problems post industrialization of supply and demand. Ie when production is local you know customers so it is easy to calibrate supply but post industrialization customer and producer are often strangers. So the inefficiency we are trying to attack is the producers ability to understand demand. This is being fulfilled extremely inefficiently through advertising and analytics and stuff in a kind of intrusive way. Instead you would have more of a marketplace where people can broadcast demand and people can provide supply. Also probably want to help builders have a sort of transparent supply chain and you would potentially have AI assistants that can search the marketplace for goods and ideally people would be able to see the whole "bill of materials" for stuff they are buying without it requiring a huge amount of office overhead for any party. With the overarching goal to be breaking the post industrial feudalism where individuals need the "strong man" of the business owner to protect them and navigate the complexities of business on their behalf. Ie make it easy for individuals to be self employed and do a good job. So back to the shipping question you would provide people with an easy way to follow shipping/fulfillment best practices and handle all kinds of scenarios. Including potentially integrating with APIs although I think any integration with a 3rd party API will happen through a plugin architecture because obviously those are unstable. But you would also potentially have a marketplace of people presenting demand for their goods to be shipped for alternative shipping solutions to be able to be competitive on different levels of scale.

**Q: The 'broadcasting demand' concept is fascinating — does this look like explicit 'want ads' ("I need 50 wool blankets by March"), or more like aggregated signal ("100 people in NH have searched for wool blankets this month")?**
A: Probably "explicit want ads" but could be integrated with Salve and it would be very structured and semantic/taxonomized, not just some want text. Also the idea is to make it easier for people to ask for custom things and have people trying to engage in a craft find the person who wants to buy "a new wooden spoon" and potentially be able to bid and make it for them vs person going to Walmart.

**Q: For the craft/custom work matching — when multiple makers could fulfill a want ad, how does selection work? Buyer chooses from bids, automatic matching based on criteria, reputation scores, or geographic preference?**
A: Probably buyer chooses but with extra information shown to them of all the options.

**Q: For the 'extra information' shown to buyers — you mentioned transparent supply chains and bill of materials. Does a seller's profile include where they source materials, or is this only relevant for complex manufactured goods?**
A: Could show sourced materials information, if they are just starting, if they have not received a contract for awhile? Like almost a round robin of suppliers? There might also be an ability for suppliers to group together into some kind of guild and like explicitly do round robin of getting work and stuff like that.

**Q: The 'guild' concept is interesting — is this a formal entity in the system (with membership, rules, collective reputation), or more like a tag/group that suppliers opt into for coordination?**
A: I think there would be flexibility in structure but the idea is that there is infrastructure to build out and run these kinds of things very easily.

**Q: How does Salve (control plane) connect to marketplace activity? Would a seller's Salve instance manage their orders, or is Salve more for the guild/organizational level?**
A: All the parties involved might have Salve running since one of the core use cases of Salve is running a business/organization.

**Q: You mentioned AI assistants searching the marketplace — is this the buyer's Salve instance using LLM to find suppliers, or a marketplace-level AI that matches supply/demand across the whole system?**
A: Could be both layers. Obviously you want as much to be deterministic algorithms as possible if possible through taxonomy and all that but would integrate AI in places that might otherwise require a human employee or multiple human employees to create a good experience (and that would be too expensive to be realistic).

**Q: When transactions go wrong — item not delivered, not as described, payment disputed — how is resolution handled? Is there a built-in arbitration system, or does it fall back to external legal/payment processor dispute systems?**
A: Honestly this is the kind of thing that you would probably use Smaragda to try and have a comprehensive intelligent design for that handles all the edge cases well.

**Q: For communication between buyer and seller during a transaction — is this structured messages within the order entity (like eBay's message system), or more free-form with the marketplace just recording that communication happened?**
A: There would definitely be normal messages but also maybe some kind of wizard that helps people have more semantically structured messages where they can just fill in the blanks since that is more useful.

**Q: For pricing — is it fixed price (seller sets, buyer accepts), negotiable (bidding/offers), or more complex (quotes for custom work, bulk discounts)?**
A: I think you would have different normal pricing models. It would be a little more inverted because customer would be presenting a budget and a product they want more often ideally vs browsing goods (although it should still support that). If there is negotiation one advantage would be trying to give the buyer transparent information about costs and stuff like that and even to educate them on the trade/business so that they understand margins and they understand the costs/margin the seller is dealing with.

**Q: For the cost transparency / buyer education — where does the margin/cost information come from? Do sellers voluntarily disclose their cost structure, or is this aggregated from the taxonomized supply chain data?**
A: Well you would want to make it as easy for the seller as possible so ideally it is something that could be very automated without requiring them to take on extra overhead to track that kind of thing and disclose it.

**Q: Given the daily sync model of Lapideus — how does time-sensitive commerce work? If someone posts a want ad today, responses come in tomorrow's batch, acceptance the day after... does the latency matter?**
A: I think you would try to acclimate people to the latency but again after something like Smaragda fuzz testing you might have certain edge cases you would want to allow more real time.

**Q: You mentioned a 'science of running a dog walking company' style encyclopedia (Scientia). How does this connect to Mercatura — does the marketplace link to relevant knowledge for each trade, or are they separate spheres?**
A: They would be integrated somehow. So the idea is if you are 17 or 25 and you decide you want to learn how to make shoes or be a plumber there would be a very smooth progression you could follow of learning and starting to make income and then past that grow your business as much as you want to.

**Q: For the learning-to-earning pathway — does the system track progression explicitly (like skill levels, certifications, portfolio), or is it more organic (you learn, you start selling, reputation builds naturally)?**
A: Probably explicit tracking because people like certifications and badges but there would be the other things as well, organic reputation, guild certification and portfolio I imagine.

**Q: For discovery — you mentioned Yahoo-style directories over search. Does the marketplace have a browsable taxonomy of trades/products, or is it primarily want-ad driven where you post what you need?**
A: You would have both options. And there would also probably be a network of "town websites" that would basically be hubs for a particular town, browseable see all the local businesses of different kinds, restaurants, all their menus, deals or sales going on, etc. So it would be integrated with those kind of online town hubs. But you would also be able to browse like "catholic book publishers" and ideally see all the books that are being published by all the catholic book publishers and just browse through them.

**Q: The 'town website/hub' concept — is this a specific type of internet in the federation (a 'local internet'), or is it a view/aggregation that pulls from multiple seller profiles who claim that location?**
A: It would be a system, and it's possible this would be on the normal internet as well, of domains like desmoines.ia.golden.earth that are all using a consistent UI and the trick would be data entry/keeping it up to date but all the Lapideus tools would be making that easy or automated for businesses.

**Q: For keeping local business data up-to-date — who maintains it? The business owners themselves using Lapideus tools, local volunteers/moderators, or some automated scraping/verification?**
A: Depends on the maturity but it could be all three. One thing is that I think it could be cool to have nice videos/photos of business owners so I could see that being a business niche where all you do is add businesses, shoot and edit intro videos for them, do support if stuff isn't working, etc.

**Q: Wait — you mentioned nice videos/photos of business owners. But Lapideus has strict media constraints (16-color images, video only for politicians). Does commerce get an exception, or do these business profiles exist on the normal web side only?**
A: This would just be on the normal web version. golden.earth could potentially be a different product than Lapideus but it would be very integrated so if you are using Lapideus lots of stuff would be automated.

**Q: So Lapideus and golden.earth are related but distinct — is golden.earth more like a 'projection' of Lapideus data onto the normal web, or is it a separate system that shares data with Lapideus?**
A: I think of Lapideus as kind of a Microsoft Word tool that everyone would be able to use. golden.earth would be a business that does other things that uses Lapideus.

**Q: So golden.earth is a business using Lapideus tools. What parts of Mercatura are core protocol (built into Lapideus) vs services that businesses like golden.earth would provide on top?**
A: As relates to this golden.earth would just be the public websites, of which there would also probably be a Lapideus version that meets all the style constraints. Everything else we have discussed so far is part of Lapideus. golden.earth is more of a business that would be selling physical products and probably the company that sells Lapideus. The golden.earth domain name might be used somewhat as a bridge between normal web and Lapideus so you can still access some Lapideus information from normal web, etc.

**Q: For sellers managing their offerings — is inventory/catalog management built into Lapideus, or do sellers just post what's available when they want to sell?**
A: Basically we are trying to look at all the useful things people have SaaS companies for, ERP, whatever and within the constraints of Lapideus we want to have all that stuff built in instead of having something you can arbitrarily extend with apps. So inventory management is definitely a core functionality that you want to do on your computer and it would be built in somehow, surely integrated with Salve and all that stuff.

**Q: You mentioned accounting being 'baked in at a deep level' in Salve. For Mercatura specifically — does every transaction automatically create ledger entries, or is accounting a separate step the seller does?**
A: Yes, I mean it's a mutation of the database so it would be tracked. I think you would have strong defaults for everything that are intelligent for different kinds of businesses but you would probably let people configure things if they really needed to as well.

**Q: For US-only commerce — does the system help with tax compliance (sales tax collection, 1099 generation, tax reporting), or is that left to external tools?**
A: Yes, you would try to handle it. On a meta level basically there is a lot of intentional or unintentional asymmetrical warfare going by increasing bureaucratic overhead so that small businesses and self employment is too complicated. This can be leveraged as a weapon to make it too expensive for anyone but big companies who can afford a lot of employees to navigate and do compliance, etc. So the idea of all these tools on a meta level to some degree is to basically make a shield for people that will be crowdsourced or whatever it needs to be, based on moderate realist taxonomy etc, that takes away the asymmetry between for example a sole proprietor business and a 500-10000 employee business in their ability to field and successfully navigate that kind of stuff.

**Q: For ongoing business relationships — does Mercatura support recurring orders, subscriptions, or contracts with terms? Or is each transaction independent?**
A: This is a deep topic but obviously one of the practical use cases of networked computers is ERP stuff so ultimately to some degree it will be a complete ERP platform.

**Q: Given the scope we're discussing (full ERP, marketplace, want-ads, guilds, tax compliance, inventory) — is this all 'Mercatura' as one sphere, or should we break it into sub-features with separate specs?**
A: My experience is that the more hard problems you are trying to do simultaneously the easier they all get because there is so much crossover and often one hard problem is the solution to another and so on. So obviously there will be a progression of what sequence these things are developed in but ultimately everything will be in v1 which is the only version that will be released.

**Q: What's the first milestone for Mercatura — the minimum slice that proves the concept and is actually useful?**
A: That's tricky. Probably something like the golden earth town websites I was talking about? That's also kind of something I might use as a funnel to get businesses into Lapideus where first they use the normal web golden.earth and then over time they transition to Lapideus.

**Q: For the town website as first milestone — what minimum information does each business listing need to be useful? Name/address/hours, menu/catalog, or more?**
A: I think basic presence and a map organized in a Yahoo directory is a good start, I mean honestly kind of like the yellow pages but with much more information per business and a good consistent presentation. I think being able to see all the businesses in your town from your computer easily and browse through them is useful.

**Q: For the spec we're writing — should it focus on the Lapideus Mercatura protocol/features, or also cover golden.earth as an implementation/business that uses those features?**
A: We can focus on Mercatura which would probably have its own golden.earth equivalent anyway just following its constraints so no video for example like you were saying.

**Q: For the moderate realist taxonomy of commerce — are there a finite set of transaction types/patterns you have in mind, or does it emerge from the trades/industries taxonomy?**
A: Each industry defines its own stuff so there are variations but there is also a lot of overlap so it's not like every trade does everything totally different than every other trade.

**Q: For the spec name — should it be 'Mercatura' (commerce sphere) specifically, or something broader that encompasses the economic empowerment vision you described?**
A: Mercatura.

**Q: What existing systems does Mercatura replace or compete with? This helps clarify positioning.**
A: All of the above (eBay/Etsy/Amazon, Shopify/Square, ERP systems).

**Q: What's the biggest risk or open question you have about Mercatura that keeps you up at night?**
A: Probably just the governance model that structurally best protects buyers and sellers, balancing privacy and transparency and how to release a v1 that you don't ever update again that is complete.

**Q: Is there anything about Mercatura I haven't asked about that's important to capture in the spec?**
A: I think we touched on all the main things.
