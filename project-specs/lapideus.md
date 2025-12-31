# Lapideus - Federated Internet Specification

Date: 2025-12-30
Status: Draft
Related: unified-state-paradigm.md, systema-paradigmata.md, salve.md
Etymology: Latin "lapideus" = "of stone" — solid, enduring, built to last

---

## Executive Summary

Lapideus is a replacement for HTTP and DNS built on TCP/IP. It provides federated "private internets" where each node acts as both browser and server. Instead of semantically neutral pages, Lapideus transmits structured entity graphs designed for specific use cases.

**Core principles:**
- Structured data, not pages
- Daily sync batching (intentional latency)
- Finite paradigms (5-6 specific applications, not general-purpose)
- Constrained media (text, 16-color images, ultra-low-res political video)
- Paid license ($50+) tied to identity, revocable
- Catholic values built in at protocol level
- One canonical app, source included, "1.0 and done" (C89 philosophy)

**First Milestone**: Book sphere or intelligence sphere, with PWA mobile client syncing to laptop server for family use.

---

## Architecture Overview

### The Internet Model

```
┌─────────────────────────────────────────────────────────────┐
│                    Lapideus Node                            │
│                                                             │
│  ┌─────────────┐    ┌──────────────┐    ┌───────────────┐  │
│  │   Browser   │ ←→ │ Entity Store │ ←→ │    Server     │  │
│  │  (Spheres)  │    │ (Per-Internet)│    │ (TCP Listener)│  │
│  └─────────────┘    └──────────────┘    └───────────────┘  │
│         ↓                  ↓                    ↓          │
│  ┌─────────────────────────────────────────────────────────┐│
│  │              Sync Engine (Daily Batches)                ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
         │                                        ↑
         ↓                                        │
┌─────────────────┐                    ┌─────────────────────┐
│  Other Nodes    │                    │  PWA Clients        │
│ (Daily Sync)    │                    │ (On-Demand Sync)    │
└─────────────────┘                    └─────────────────────┘
```

### Internet Topology

```
                    ┌───────────────────┐
                    │  Public Internet  │
                    │ (news, politics)  │
                    └─────────┬─────────┘
                              │ (daily sync)
        ┌─────────────────────┼─────────────────────┐
        │                     │                     │
        ↓                     ↓                     ↓
┌───────────────┐    ┌───────────────┐    ┌───────────────┐
│ Family        │    │ Local Area    │    │ Commerce      │
│ Internet      │    │ Internet      │    │ Internet      │
│ (invite only) │    │ (geographic)  │    │ (public)      │
└───────┬───────┘    └───────────────┘    └───────────────┘
        │
        │ (on-demand sync, same user)
        ↓
┌───────────────┐
│ Mobile PWA    │
│ (phone)       │
└───────────────┘
```

### Data Isolation

- Each internet is a separate namespace
- No automatic data flow between internets
- User can aggregate locally for viewing/querying
- Internet owners can configure aggregator behavior
- Strict isolation is the default

---

## Core Concepts

### 1. Structured Data, Not Pages

Instead of HTML documents at URLs, Lapideus transmits entity graphs:

```
Entity: "Event::2025_election_results"
├─ genus: "PoliticalEvent"
├─ proprietates:
│   ├─ title: "2025 Presidential Election Results"
│   ├─ date: "2025-11-04"
│   └─ summary: "..."
├─ relationes:
│   ├─ "involves" → Person::candidate_a
│   ├─ "involves" → Person::candidate_b
│   ├─ "location" → Place::united_states
│   └─ "sourced_by" → [Source entities with chain of custody]
└─ notae: ["#politics", "#election", "#2025"]
```

No CSS, no arbitrary layout, no semantic ambiguity. Data is structured for machine processing and human browsing through purpose-built interfaces.

### 2. Daily Sync Batching

- Changes collected throughout the day
- Published in daily batch at fixed time
- Clients sync when they connect
- Archive format supports "thumb drives in the mail"
- All batches preserved (offline for a week = get 7 batches)
- Universal rule: nobody publishes faster than daily

**Philosophy**: "Turn on the internet once a day, get the update, turn off."

### 3. Finite Paradigms (Spheres)

Not a general platform. Five or six specific applications with purpose-built UIs:

| Sphere | Description | Key Features |
|--------|-------------|--------------|
| **Libri** (Books) | Reading, glossing, book clubs | Public domain texts bundled, sentence-level addressing, physical book support |
| **Intelligentia** (News) | Public service intelligence agency | Entity/event graph, contestable sources, subscription to entities |
| **Scientia** (Encyclopedia) | Granular sciences hierarchically organized | Federated from chosen internets, "science of running a dog walking company" |
| **Politica** (Politics) | Low-res video streaming for candidates | 192x108 24fps, live + archive, screen sharing |
| **Mercatura** (Commerce) | Buying and selling | Built-in payment, non-anonymous messaging |

User switches between spheres like rhubarb widgets.

### 4. Constrained Media

| Media Type | Allowed | Restrictions |
|------------|---------|--------------|
| Text | Yes | Specific formats, monospace |
| Images | Limited | 16-color palette (rhubarb colors), specific contexts |
| Video | Very limited | 192x108 24fps, political candidates only |
| Audio | No | Not included |
| Arbitrary files | No | Not a file sharing platform |

**Philosophy**: Safe for 12-year-olds. No bombardment of gross content.

### 5. Identity and Licensing

- Identity tied to paid license ($50+ one-time)
- License key is the identity anchor
- Different transparency levels:
  - Public roles (business, admin): non-anonymous
  - Private activity: more privacy
  - Whistleblowers: TBD (anonymous hearsay is abusable)
- License revocable for rule violations
- Merkle-chain revocation log (tamper-evident)
- Revocation can be appealed, frivolous revocations have consequences

### 6. Global Entity IDs

Real-world entities have deterministic IDs:

```c
/* Example: UUID v5 from name + birth year */
Person::napoleon_bonaparte_1769
Place::paris_france
Book::summa_theologica_aquinas_1274
```

- Searchable across all connected internets
- Enables linking without coordination
- Browsable like Yahoo directories, not just search

---

## The Spheres

### Libri (Books)

**Vision**: Support the intellectual life centered on physical books.

**Features:**
- Project Gutenberg corpus bundled in binary
- Canonical IDs for editions (ISBN + internal scheme)
- Sentence-level addressing (Xanadu-style)
- Glossing and annotation
- Highlights and quotes from physical books (without full text)
- Book clubs / shared reading
- Sell digital copies through commerce sphere

**Addressing Example:**
```
libri://aquinas/summa_theologica/I/q2/a3/corpus/sent:5
```

### Intelligentia (News/Intelligence)

**Vision**: Public service intelligence agency, not opinion journalism.

**Features:**
- Entity/event graph with structured facts
- Sources with chain of custody
- Contestation is append-only (original stays, disputes linked)
- Subscribe to entity changes (geographic, industry, people)
- Feeds Salve control planes with real-world data
- Tamper-evident history via Merkle trees

**Trust Model:**
- Sources must be non-anonymous
- Chain of dependencies visible
- Competing claims coexist, user judges

### Scientia (Encyclopedia)

**Vision**: Granular, practical sciences organized hierarchically.

**Features:**
- "The science of running a dog walking company"
- Federated: aggregate from chosen internets
- Hierarchical browsing (like Yahoo directories)
- Expert knowledge distilled into structured format
- Links to Libri for source texts

### Politica (Politics)

**Vision**: Enable cheap political campaigns.

**Features:**
- Ultra-low-res video streaming: 192x108 @ 24fps
- Live streaming with call-ins
- Archive of past streams
- Screen sharing (rhubarb UI)
- Only for declared political candidates
- Exception to daily sync (live is real-time)

### Mercatura (Commerce)

**Vision**: Built-in buying and selling.

**Features:**
- Payment built into protocol (plugin for processors)
- Messaging between parties (non-anonymous)
- Transaction recorded in graph
- Reputation via tamper-evident history
- Digital goods transfer (ownership model from data-ownership spec)

---

## Protocol Design

### Wire Format

Same as rhubarb: nuntium (protobuf-style binary format)

```
[length_prefix][entity_event_1]
[length_prefix][entity_event_2]
...
```

### Batch Format

Daily batches are archive files containing:

```
batch_2025_12_30.lapideus
├─ metadata.nuntium       # Batch info, timestamp, sequence
├─ entities.nuntium       # New/updated entities
├─ events.nuntium         # Event log for the day
├─ signature.nuntium      # Server signature
└─ merkle_root.nuntium    # Tamper-evidence
```

### Sync Protocol

**Inter-Internet (Daily):**
1. Client connects to internet server
2. Requests batches since last sync
3. Server sends all missed batches
4. Client applies in sequence
5. Client acknowledges, disconnects

**Same-User (On-Demand):**
1. PWA connects to laptop server
2. Requests changes since last sync
3. Server sends delta immediately (no batch delay)
4. Bidirectional: phone changes sync to laptop too
5. Includes PKM/Salve data, not just internet data

### Encryption

- TLS-like transport encryption (not trying to hide from state actors)
- Signing more important than encryption
- Merkle trees for tamper-evident public history
- Private data encrypted at rest

---

## Identity System

### License Keys

```
License: LAPIDEUS-XXXX-XXXX-XXXX-XXXX
├─ Holder: Real name (non-anonymous)
├─ Issued: 2025-12-30
├─ Type: Personal | Business | Political
└─ Status: Active | Revoked | Suspended
```

### Public vs Private Activity

| Activity | Anonymity | Accountability |
|----------|-----------|----------------|
| Running a business | None | Full identity visible |
| Admin/moderator | None | Actions logged with ID |
| Publishing content | Limited | Pseudonym linked to license |
| Private browsing | Yes | License validates but not exposed |
| Commerce | None | Both parties identified |

### Revocation

```
Revocation Event:
├─ target_license: LAPIDEUS-XXXX-...
├─ reason: "Content policy violation"
├─ revoker: License of moderator/authority
├─ timestamp: (merkle-chained)
├─ appeal_window: 30 days
└─ status: pending_appeal | confirmed | overturned
```

Distributed Merkle-chain revocation log:
- All revocations publicly visible
- Revocation of revocation is possible
- Frivolous revocations have consequences for revoker
- Web of associations visible through intelligence sphere

---

## Governance

### Protocol-Level Rules

Built into the software, cannot be bypassed:
- Daily sync timing (no faster publishing)
- Media type restrictions (no arbitrary images/video)
- Identity tied to license
- Append-only for public sphere changes

### Per-Internet Rules

Set by internet owner:
- Who can join (invite-only vs public)
- Content policies beyond protocol minimum
- Moderation policies
- Aggregation settings

### Social Governance

- License issuance authority
- Revocation authority (distributed)
- Appeal process
- Consequences for abuse of power

**Philosophy**: "The internet is like a human society — needs governance structure and consequences."

---

## Client Architecture

### Desktop (Lapideus App)

- Written in C89 (rhubarb libraries)
- Acts as both browser and server
- Manages multiple internet connections
- Stores local entity graphs per internet
- Runs Salve, PKM, and other local features

### Mobile (PWA)

- Separate JS/TS codebase
- Same visual vocabulary (canvas, 16-color, monospace)
- Different widgets for mobile form factor
- Served by desktop app for installation
- Syncs on-demand with user's desktop (not daily)
- Offline capability via IndexedDB/etc

### Sync Scenarios

| Scenario | Timing | Data |
|----------|--------|------|
| Inter-internet | Daily batches | Internet data only |
| Same user, different devices | On-demand | Everything (PKM, Salve, etc) |
| PWA to desktop | On-demand | User's private data |

---

## First Milestone

### Scope

1. **Book Sphere (Libri)** OR **Intelligence Sphere (Intelligentia)**
2. Desktop app serving as server
3. PWA client for mobile
4. Family internet (me + wife, phones + computers)
5. On-demand sync for same user
6. Basic entity browsing UI

### Out of Scope for V1

- Commerce / payments
- Political video streaming
- Public internets with many users
- AI content moderation
- Merkle-chain revocation
- License management infrastructure

### Technical Deliverables

| Component | Description |
|-----------|-------------|
| Sync protocol | Binary format, batch/on-demand modes |
| Entity storage | Per-internet namespaces |
| Desktop server | TCP listener, serves PWA, syncs with clients |
| PWA client | Canvas rendering, offline storage, sync |
| One sphere UI | Either Libri or Intelligentia |

---

## Files to Create (V1)

| File | Purpose |
|------|---------|
| `include/lapideus.h` | Core types: Internet, Batch, License |
| `lib/lapideus.c` | Internet management, sync coordination |
| `include/lapideus_sync.h` | Sync protocol types |
| `lib/lapideus_sync.c` | Batch creation, delta sync, transport |
| `include/lapideus_server.h` | TCP server for clients |
| `lib/lapideus_server.c` | Connection handling, PWA serving |
| `pwa/` | TypeScript PWA codebase |

---

## Dependencies

### Existing (from rhubarb)
- `nuntium.h/c` - Wire format serialization
- `entitas_repositorium.h/c` - Entity storage
- `persistentia.h/c` - Event log persistence
- `piscina.h/c` - Memory management
- `chorda.h/c` - String handling

### New Required
- TCP server implementation
- PWA build system
- Canvas rendering for PWA (matching rhubarb aesthetic)

---

## Success Criteria (V1)

1. Desktop app runs server on local network
2. PWA installable on phone from desktop
3. Create entities on desktop, see on phone
4. Create entities on phone, see on desktop
5. Works offline, syncs when reconnected
6. One sphere (Libri or Intelligentia) usable end-to-end
7. Family can use it for intended purpose

---

## Future Phases

### Phase 2: Public Internets
- Multiple users per internet
- Daily batch publishing
- Basic moderation tools

### Phase 3: Commerce
- Payment integration
- Digital goods transfer
- Non-anonymous messaging

### Phase 4: Full Media
- 16-color image support
- Political video streaming
- Content moderation (AI or manual)

### Phase 5: Governance
- License management
- Merkle-chain revocation
- Appeal process
- Federation policies

### Phase 6: 1.0 Release
- Feature freeze
- "C89 philosophy" — stable forever
- Source code included with license
- Public launch

---

## Open Questions

1. **License authority**: Who issues initial licenses? Bootstrap problem.
2. **Payment processors**: Which to support? Plugin architecture?
3. **PWA limitations**: What can't be done in a PWA that affects sync?
4. **Merkle implementation**: Full blockchain or lighter Merkle DAG?
5. **Content moderation**: AI layer, manual review, or constrained media only?
6. **Book corpus**: How large is Project Gutenberg? Fits in binary?
7. **Sentence addressing**: Algorithm for canonical sentence IDs in any text?
