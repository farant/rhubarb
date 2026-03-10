---
id: PT0032
title: "Exploit Summa structural markers for syllogism parsing"
status: promoted_to_kit
promoted_to: KIT0013
source: ["Architecture Insight notes", "Inverse Knowledge Search paper"]
applicable_to: []
related: [PT0009, PT0033]
---

The Summa Theologica has highly consistent structural markers across 3,000+ articles that can be exploited for semi-automated syllogism extraction:

- "Videtur quod..." (It seems that...) = objection = premise to be refuted
- "Sed contra..." (On the contrary...) = counter-authority
- "Respondeo dicendum quod..." (I answer that...) = the actual argument
- "Ad primum/secundum/tertium..." = replies to objections

A parser that exploits these markers gets a huge head start on extracting syllogistic structure. The structural markers are the "curriculum scaffolding" that the Inverse Knowledge Search paper recommends -- except the Summa already provides it.

The Summa IS an "uncompressed reasoning corpus" -- it doesn't hide its derivations like most text. The respondeo sections are essentially Long Chains-of-Thought (LCoTs). This is the "dark matter" that the Inverse Knowledge Search paper says is missing from compressed scientific writing -- but it's already present in the Summa.

Implementation: deterministic parsing of structural markers in C89, followed by fluency-layer-assisted extraction of logical structure within each section.
