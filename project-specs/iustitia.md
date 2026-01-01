# Iustitia - Justice/Governance Specification

Date: 2025-12-31
Status: Draft
Related: lapideus.md, mercatura.md, scientia.md
Etymology: Latin "iustitia" = "justice, righteousness" — the virtue of giving each their due

---

## Executive Summary

Iustitia is the governance sphere of Lapideus, providing a natural law-based justice system for moderating behavior, resolving disputes, and maintaining accountability across the federated internet. Unlike modern positive law systems where rules are created by whoever holds power, Iustitia is grounded in the pre-Justinian understanding that authority discovers and applies objective justice rather than creating it.

**Core Mission**: Provide a governance structure that achieves natural law justice through transparent, accountable processes with real consequences.

**Key Differentiators:**
- Natural law foundation: Justice is objective reality to be discerned, not rules to be followed
- Permanent transparent record: All actions Merkle-chained, with semantic expungement
- Subsidiarity model: Authority flows from internet ownership, HRE-style overlapping jurisdictions
- Rehabilitation focus: Right to rehabilitation, no indefinite punishment for willing penitents
- AI-assisted arbitration: Plugin architecture for neutral first-pass evaluation
- Defederation as ultimate sanction: Social/economic pressure rather than centralized enforcement

**First Milestone**: Public complaint log (minimal: submitter, accused, description).

---

## Core Philosophy

### Natural Law vs Positive Law

The governing principle is **Lex Naturalis** (natural law) — the understanding that justice exists as objective reality, independent of human legislation. This contrasts with:

- **Positive law**: Rules are whatever those in power declare them to be
- **Precedent-heavy systems**: Can be gamed to create unjust outcomes through technical manipulation
- **Legalism**: Strict rule application that produces injustice through procedural correctness

Instead, the question is always: "Was there / is there an injustice being perpetrated here?"

### Anti-Nominalist Perspective

Justice is not merely a name we apply to outcomes we prefer. It is discoverable through right reason. Authorities don't create justice — they discern and apply it. This was the Christian understanding of law prior to the rediscovery of the Code of Justinian (c. 1070) and represents a return to that tradition.

### Subsidiarity (HRE Model)

Inspired by the Holy Roman Empire's governance structure:
- Decisions made at lowest appropriate level
- Overlapping authorities rather than clean hierarchies
- Organic authority structures emerging from community choice
- Multiple tiers of appeal
- Priests, bishops, app maintainers, or whoever people choose as ultimate authority

---

## Architecture Overview

### Authority Model

```
┌─────────────────────────────────────────────────────────────────┐
│                         IUSTITIA                                 │
│                                                                  │
│  ┌────────────────────────────────────────────────────────────┐ │
│  │                  Ultimate Authority                         │ │
│  │         (App maintainer / Bishops / People's choice)        │ │
│  └──────────────────────────┬─────────────────────────────────┘ │
│                             │                                    │
│         ┌───────────────────┼───────────────────┐               │
│         ↓                   ↓                   ↓               │
│  ┌──────────────┐   ┌──────────────┐   ┌──────────────┐        │
│  │   Regional   │   │    Guild     │   │  Aggregator  │        │
│  │   Internets  │   │  Authorities │   │   Networks   │        │
│  └──────┬───────┘   └──────┬───────┘   └──────┬───────┘        │
│         │                   │                   │                │
│         └───────────────────┼───────────────────┘                │
│                             ↓                                    │
│  ┌────────────────────────────────────────────────────────────┐ │
│  │                    Internet Owners                          │ │
│  │    (Family / Town / Community / Business internets)         │ │
│  │                                                              │ │
│  │    Powers:                                                   │ │
│  │    - Eject users from their internet                        │ │
│  │    - Add revocations to blockchain                          │ │
│  │    - Set penalties (voluntary, escalate to revocation)      │ │
│  │    - Grant expungement                                       │ │
│  └────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

### Data Flow

```
Complaint Submitted                    Evidence Sources
       │                                     │
       ↓                                     ↓
┌──────────────┐                   ┌──────────────────┐
│  Complaint   │                   │  Entity Graph    │
│  Log         │                   │  (Intelligentia) │
│  (Public)    │                   │                  │
└──────┬───────┘                   │  - Messages      │
       │                           │  - Transactions  │
       │ (escalation)              │  - Public Acts   │
       ↓                           └────────┬─────────┘
┌──────────────┐                            │
│    Case      │←───────────────────────────┘
│    Entity    │     (discovery process)
│              │
│  - Parties   │
│  - Claims    │
│  - Evidence  │
│  - Ruling    │
└──────┬───────┘
       │
       ↓
┌──────────────┐
│  Revocation  │
│  Blockchain  │
│              │
│  - Key       │
│  - Revoker   │
│  - Reason    │
│  - Case ref  │
│  - Intent    │
└──────────────┘
```

---

## Core Concepts

### 1. Complaint Log (Querela)

Public record of potential injustices — "bug reports for society":

```
Entitas: "Querela::complaint_2025_001"
├─ genus: "Querela"
├─ proprietates:
│   ├─ submitter: License key of submitter
│   ├─ accused: License key or entity ID of accused
│   ├─ description: "What happened" (free text)
│   ├─ submitted: timestamp
│   └─ status: "open" | "under_review" | "case_opened" | "dismissed"
├─ relationes:
│   ├─ "submitted_by" → User entity
│   ├─ "against" → User/Business entity
│   └─ "became_case" → Case entity (if escalated)
└─ notae: [#querela, #pending]
```

**Features:**
- Anyone can submit (public interest standing)
- Public by default (transparency deters corruption)
- Pattern detection surfaces repeated complaints against same party
- Frivolous submissions tracked, consequences for abuse
- Material for authorities to plan and prioritize

### 2. Case (Causa)

Formal dispute proceeding:

```
Entitas: "Causa::case_2025_001"
├─ genus: "Causa"
├─ proprietates:
│   ├─ status: "discovery" | "hearing" | "ruling" | "appeal" | "closed"
│   ├─ type: "dispute" | "violation" | "appeal"
│   ├─ jurisdiction: Internet ID where offense occurred
│   ├─ opened: timestamp
│   └─ ruling: { decision, reasoning, sanctions }
├─ relationes:
│   ├─ "plaintiff" → User entity
│   ├─ "defendant" → User entity
│   ├─ "arbiter" → Authority entity (human or AI)
│   ├─ "originated_from" → Querela entity
│   ├─ "evidence" → [Entity references discovered]
│   └─ "appealed_to" → Causa entity (if appealed)
└─ notae: [#causa, #active]
```

**Discovery Process:**
- Algorithmic evidence gathering from entity graph
- Messages between parties become discoverable
- Defendant-driven disclosure for private information
- Default: discovered evidence becomes permanently public
- Exception: limited disclosure when justice requires it

### 3. Revocation Blockchain (Catena Revocationis)

Merkle-chained public record of license revocations:

```
Revocation Event:
├─ target_key: License key being revoked
├─ revoker_key: License key of authority
├─ reason: Category + description
├─ case_reference: Link to Causa entity
├─ intent: Documented intent (required)
├─ timestamp: Merkle-chained
├─ status: "active" | "appealed" | "unrevoked" | "expunged"
└─ signature: Revoker's cryptographic signature
```

**Properties:**
- Anyone can add revocations (but actions are tied to identity)
- Anyone can unrevoke (with same accountability)
- Documented intent required (fraud prevention)
- High cost: permanent record attached to real identity
- Coordinated attacks leave trail for later prosecution

### 4. Expungement (Expunctio)

Semantic layer preventing prejudicial use of past offenses:

```
Expungement Event:
├─ target_record: Reference to revocation or ruling
├─ granter_key: Authority who granted expungement
├─ reason: "Rehabilitation completed" | "Unjust original ruling"
├─ case_reference: Link to rehabilitation case
├─ timestamp: Merkle-chained
└─ signature: Granter's cryptographic signature
```

**Rules:**
- Record remains for integrity, but marked as expunged
- Using expunged records against someone is itself an actionable offense
- Authorities at certain tiers can grant expungement
- Abuse of expungement power has consequences

---

## Punishment and Rehabilitation

### Sanction Spectrum

Penalties are "voluntary" — refusal escalates toward revocation:

| Level | Sanction | Description |
|-------|----------|-------------|
| 1 | Warning | Formal documentation of offense |
| 2 | Probation | Time-based good behavior period |
| 3 | Restricted access | Limited features/capabilities |
| 4 | Temporary suspension | Loss of access for fixed period |
| 5 | License revocation | Complete loss of Lapideus access |

Internet owners determine sanctions within their jurisdiction.

### Rehabilitation Paths

Right to rehabilitation — no indefinite punishment for willing penitents:

| Path | Description | Integration |
|------|-------------|-------------|
| Education/Formation | Complete relevant Scientia courses | Scientia integration |
| Time-based probation | Period of good behavior | Salve workflow tracking |
| Restitution | Make wronged party whole | Mercatura payment |
| Vouching/Sponsorship | Authority attests to rehabilitation | Entity relationship |
| Catholic penitence | Prayer, rosaries, spiritual exercises | Optional tracker in app |

**Penitence Tracking:**
- System can include prayer/devotion tracker
- Honor-based — not perfect confirmation
- Faking rehabilitation is itself a punishable offense

---

## AI Arbitration

### Plugin Architecture

Models change over time, so AI judges are pluggable:

```
AI Judge Interface:
├─ evaluate_case(case_entity) → { ruling, confidence, reasoning }
├─ screen_complaint(querela) → { priority, flags, similar_cases }
├─ detect_patterns(complaints[]) → { clusters, systemic_issues }
└─ validate_revocation(revocation) → { approval, concerns, coordinated_attack_risk }
```

### AI Role

- First-pass neutral evaluation to increase throughput
- Always appealable to human authority
- Pattern detection for coordinated attacks
- Screening for frivolous filings
- Not final authority — discernment aid

---

## Defederation

### Mechanism

The ultimate social sanction — internet owners can refuse to federate:

```
Federation Status:
├─ source_internet: ID
├─ target_internet: ID
├─ status: "federated" | "defederated" | "pending"
├─ reason: (if defederated)
└─ changed: timestamp
```

### Aggregator Role

Federation aggregators can act as gatekeepers:
- If defederated from major aggregator, lose access to downstream internets
- Creates natural reputation pressure
- No centralized enforcement — organic social consequence

### Criteria

Each internet owner decides independently, but common triggers:
- Documented history of unrepentant unjust behavior
- Pattern of frivolous revocations/complaints
- Refusal to participate in appeals process
- Harboring revoked users

---

## Integration with Other Spheres

### Mercatura (Commerce)

Same court system handles commerce disputes:
- Fraud claims
- Non-payment
- Non-delivery
- Product/service quality
- Seller/buyer misconduct

Messages between buyer/seller are discoverable for disputes.

### Intelligentia (Intelligence)

Evidence source for cases:
- Public figures' messages available untampered
- Chain of custody for facts
- Entity relationships visible
- Web of associations documented

### Scientia (Encyclopedia)

Rehabilitation education:
- Courses on ethics, trade practices
- Required learning paths for certain offenses
- Certification of completion

### Salve (Control Plane)

Workflow support:
- Probation tracking
- Rehabilitation task management
- Prayer/devotion tracker
- Deadline monitoring for appeals

---

## Timing and Sync

### Revocation Timing

Options depending on situation:
- **Immediate**: For urgent cases (credible threat, fraud in progress)
- **Review period**: Default, allows appeal before effect
- **Configurable**: Authority can choose based on severity

### Daily Sync Integration

- Revocation may cause immediate loss of sync
- Data ownership (DRM-like) may prevent access to already-synced data
- Grace period for appeal in non-urgent cases

---

## Constraints and Abuse Prevention

### Non-Anonymity

Key protection against abuse:
- All actions tied to real identity (license key)
- Coordinated attacks leave permanent trail
- Abuse of power is documentable
- Web of associations visible

### Penalties for Abuse

| Abuse | Consequence |
|-------|-------------|
| Frivolous complaints | Warning → probation → revocation |
| Lost appeals | Escalating penalties |
| False revocations | Own revocation risk |
| Abuse of authority | Loss of authority + possible revocation |
| Faking rehabilitation | Penalty for fraud |
| Using expunged records | Actionable offense |

---

## First Milestone

### Scope: Public Complaint Log

1. **Complaint submission** with minimal fields
2. **Public viewing** of all complaints
3. **Pattern detection** for same accused
4. **Status tracking** (open, dismissed, escalated)

### Out of Scope for V1

- Full case management
- AI arbitration
- Revocation blockchain
- Expungement system
- Rehabilitation tracking
- Defederation management

### Success Criteria

1. User can submit complaint (submitter, accused, description)
2. All complaints publicly visible and browsable
3. Complaints grouped by accused party
4. Basic status transitions work
5. Data persists across app restarts
6. Family can use it to log household disputes (test case)

---

## Future Phases

### Phase 2: Case Management

- Formal case filing
- Arbiter assignment
- Ruling recording
- Basic appeals

### Phase 3: Revocation Blockchain

- Merkle-chained revocation log
- Add/unrevoke capabilities
- Intent documentation
- Signature verification

### Phase 4: AI Arbitration

- Plugin architecture for AI judges
- Pattern detection
- Coordinated attack screening
- First-pass rulings

### Phase 5: Rehabilitation

- Scientia course integration
- Probation tracking via Salve
- Restitution via Mercatura
- Penitence tracker

### Phase 6: Full Governance

- Expungement system
- Defederation management
- Aggregator integration
- HRE-style overlapping jurisdictions

---

## Dependencies

### Existing (from rhubarb)

- `entitas_repositorium.h/c` - Entity storage
- `persistentia.h/c` - Event log persistence
- `nuntium.h/c` - Wire format for sync

### Required from Other Specs

- **Lapideus**: Network sync, identity/licensing
- **Mercatura**: Commerce dispute integration
- **Intelligentia**: Evidence graph access
- **Scientia**: Rehabilitation courses
- **Salve**: Workflow tracking

### New Required

- Merkle tree implementation
- Cryptographic signing
- AI plugin interface

---

## Open Questions

1. **Discovery visibility**: When should disclosed evidence be restricted vs public?
2. **Jurisdiction conflicts**: Detailed rules for overlapping authority resolution?
3. **AI model selection**: Criteria for approving AI judges for the plugin system?
4. **International scope**: How does this work if Lapideus expands beyond US?
5. **Appeal limits**: Maximum number of appeals? Escalating costs?
6. **Penitence verification**: How much tracking for spiritual exercises?
7. **Defederation cascade**: How to prevent network fragmentation?

---

## Appendix: Natural Law Principles (Lex Naturalis)

Foundational principles for discerning justice (not exhaustive, not legalistic):

| Principle | Description |
|-----------|-------------|
| Do good, avoid evil | The first principle of practical reason |
| Give each their due | Justice as proper distribution |
| Do not harm | Protection from injury |
| Keep agreements | Honoring contracts and promises |
| Proportionality | Punishment proportional to offense |
| Right to defense | Accused may respond to claims |
| Rehabilitation | Path back for the repentant |
| Subsidiarity | Decisions at lowest appropriate level |
| Transparency | Public accountability for public acts |
| Intent matters | Consideration of motive and circumstance |

These guide discernment but do not replace it. The question remains: "Was justice served?"
