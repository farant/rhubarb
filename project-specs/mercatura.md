# Mercatura - Commerce Sphere Specification

Date: 2025-12-30
Status: Draft
Related: lapideus.md, salve.md, scientia.md, smaragda.md
Etymology: Latin "mercatura" = "trade, commerce" — the art of buying and selling

---

## Executive Summary

Mercatura is the commerce sphere of Lapideus, providing a complete platform for buying, selling, and economic coordination. Unlike traditional marketplaces that connect strangers for transactions, Mercatura inverts the model: buyers broadcast structured demand, suppliers respond with bids, and the system provides transparent cost/margin information to create informed exchanges.

**Core Mission**: Break post-industrial feudalism by making self-employment and small business operation as easy as working for a large company.

**Key Differentiators:**
- Demand-driven: Buyers post structured "want ads," suppliers respond
- Transparent costs: Automatic tracking enables margin/cost education
- Integrated ERP: Full business operations built-in (inventory, accounting, tax)
- Guild infrastructure: Easy collective organization for suppliers
- Learning-to-earning: Integrated with Scientia for skill development pathways
- Compliance shield: Tax, regulatory compliance handled automatically

**First Milestone**: Local business directory (modernized Yellow Pages) with browsable hierarchy.

---

## Core Philosophy

### Breaking Post-Industrial Feudalism

The problem: Post-industrialization separated producers from customers, making demand illegible. This created:
- Dependency on advertising/analytics (intrusive, inefficient)
- Bureaucratic complexity as asymmetric warfare against small businesses
- Requirement for "strong man" business owners to navigate complexity

The solution: Make the infrastructure that large companies use internally available to individuals, with:
- Transparent demand signals (structured want ads)
- Automated compliance (tax, regulatory)
- Shared knowledge (Scientia integration)
- Collective organization (guilds)

### Moderate Realism in Commerce

- Finite taxonomy of trades/industries
- Each industry has patterns, but significant overlap
- Transaction types emerge from taxonomy, not arbitrary
- Enables deterministic matching where possible, AI where needed

---

## Architecture Overview

### Marketplace Model

```
┌─────────────────────────────────────────────────────────────────┐
│                        MERCATURA                                 │
│                                                                  │
│  ┌────────────────┐              ┌────────────────┐             │
│  │    BUYERS      │              │    SELLERS     │             │
│  │                │              │                │             │
│  │  Post Want Ads │─────────────→│  Browse Wants  │             │
│  │  Set Budgets   │              │  Submit Bids   │             │
│  │  Choose Bids   │←─────────────│  Show Costs    │             │
│  └────────────────┘              └────────────────┘             │
│          │                              │                        │
│          └──────────┬───────────────────┘                        │
│                     ↓                                            │
│           ┌─────────────────┐                                    │
│           │   TRANSACTION   │                                    │
│           │    ENTITY       │                                    │
│           │                 │                                    │
│           │  Messages       │                                    │
│           │  Payment        │                                    │
│           │  Fulfillment    │                                    │
│           │  Ledger Entry   │                                    │
│           └─────────────────┘                                    │
│                     │                                            │
│          ┌──────────┼──────────┐                                 │
│          ↓          ↓          ↓                                 │
│   ┌──────────┐ ┌──────────┐ ┌──────────┐                        │
│   │  Salve   │ │ Scientia │ │  Guilds  │                        │
│   │ (Ops)    │ │ (Learn)  │ │ (Org)    │                        │
│   └──────────┘ └──────────┘ └──────────┘                        │
└─────────────────────────────────────────────────────────────────┘
```

### Data Flow

```
Buyer Intent                    Seller Capability
     │                               │
     ↓                               ↓
┌──────────┐                  ┌──────────┐
│ Want Ad  │                  │ Catalog  │
│ (struct) │                  │ (struct) │
└────┬─────┘                  └────┬─────┘
     │                               │
     └───────────┬───────────────────┘
                 ↓
          ┌──────────────┐
          │   Matching   │
          │  (taxonomy-  │
          │   based +    │
          │   AI assist) │
          └──────┬───────┘
                 ↓
          ┌──────────────┐
          │  Bid/Offer   │
          │  Negotiation │
          └──────┬───────┘
                 ↓
          ┌──────────────┐
          │  Transaction │
          │  Execution   │
          └──────────────┘
```

---

## Core Concepts

### 1. Want Ads (Demand Broadcasting)

Structured requests for goods/services:

```
Entitas: "WantAd::wooden_spoon_custom_123"
├─ genus: "WantAd"
├─ proprietates:
│   ├─ category: "Craft::Woodworking::Utensils"
│   ├─ description: "Custom wooden cooking spoon"
│   ├─ specifications:
│   │   ├─ material: "hardwood"
│   │   ├─ length: "12-14 inches"
│   │   └─ style: "traditional"
│   ├─ budget_range: ["25.00", "50.00"]
│   ├─ timeline: "2 weeks"
│   ├─ location_preference: "local" | "national"
│   └─ status: "open" | "bidding" | "accepted" | "fulfilled"
├─ relationes:
│   ├─ "posted_by" → User entity
│   ├─ "received_bids" → [Bid entities]
│   └─ "accepted_bid" → Bid entity (when chosen)
└─ notae: ["#custom", "#woodworking"]
```

### 2. Business Presence

Full legal identity required for sellers:

```
Entitas: "Business::smiths_woodcraft_456"
├─ genus: "Business"
├─ proprietates:
│   ├─ legal_name: "Smith's Woodcraft LLC"
│   ├─ owner_name: "John Smith"
│   ├─ address: { street, city, state, zip }
│   ├─ contact: { phone, email }
│   ├─ hours: { ... }
│   ├─ categories: ["Craft::Woodworking"]
│   ├─ established: "2020"
│   ├─ certifications: [...]
│   └─ tax_id: "XX-XXXXXXX" (encrypted)
├─ relationes:
│   ├─ "owned_by" → User entity (license holder)
│   ├─ "member_of" → [Guild entities]
│   ├─ "has_catalog" → [Product/Service entities]
│   ├─ "located_in" → Place::town entity
│   └─ "transactions" → [Transaction entities]
└─ notae: ["#woodworking", "#custom", "#local"]
```

### 3. Guilds

Flexible collective organization:

```
Entitas: "Guild::nh_woodworkers_coop"
├─ genus: "Guild"
├─ proprietates:
│   ├─ name: "New Hampshire Woodworkers Cooperative"
│   ├─ structure: "cooperative" | "association" | "informal"
│   ├─ rules:
│   │   ├─ membership_criteria: [...]
│   │   ├─ work_distribution: "round_robin" | "bid" | "assigned"
│   │   └─ revenue_sharing: { model }
│   ├─ certifications_offered: [...]
│   └─ founded: "2024"
├─ relationes:
│   ├─ "has_members" → [Business entities]
│   ├─ "certifies" → [User entities]
│   └─ "covers_region" → [Place entities]
└─ notae: ["#guild", "#woodworking", "#nh"]
```

### 4. Transactions

Complete record of exchange:

```
Entitas: "Transaction::txn_789"
├─ genus: "Transaction"
├─ proprietates:
│   ├─ type: "sale" | "service" | "custom_order"
│   ├─ status: "negotiating" | "agreed" | "paid" | "shipped" | "complete"
│   ├─ amount: "35.00"
│   ├─ currency: "USD"
│   ├─ cost_breakdown: { materials, labor, margin } (if disclosed)
│   ├─ shipping: { method, tracking, status }
│   └─ timestamps: { created, agreed, paid, shipped, delivered }
├─ relationes:
│   ├─ "buyer" → User entity
│   ├─ "seller" → Business entity
│   ├─ "originated_from" → WantAd | Catalog item
│   ├─ "messages" → [Message entities]
│   ├─ "creates_ledger_entry" → LedgerEntry entity
│   └─ "fulfilled_by" → Shipment entity (physical goods)
└─ notae: ["#transaction", "#complete"]
```

---

## Integrated Systems

### Salve Integration

Every marketplace participant can run Salve control planes:

| Actor | Salve Use Case |
|-------|---------------|
| Seller | Order management, inventory alerts, fulfillment workflows |
| Buyer | Procurement tracking, budget monitoring, recurring orders |
| Guild | Member coordination, work distribution, quality control |

### Scientia Integration

Learning-to-earning pathways:

```
Learning Path: "Woodworking"
├─ Beginner
│   ├─ Articles: basic tools, wood types, safety
│   ├─ Projects: cutting board, simple box
│   └─ Can sell: beginner-level items
├─ Intermediate
│   ├─ Articles: joinery, finishing, design
│   ├─ Projects: furniture, turned items
│   ├─ Certification: Guild apprentice
│   └─ Can sell: intermediate items, take custom orders
└─ Advanced
    ├─ Articles: business operations, pricing, scaling
    ├─ Certification: Guild master
    └─ Can sell: all items, mentor others
```

### Smaragda Integration

Use simulation to design:
- Dispute resolution workflows
- Edge cases in transaction flows
- Guild governance models
- Pricing strategy exploration

---

## Local Business Directory (Town Hubs)

### Structure

```
Town Hub: "concord.nh.lapideus"
├─ Businesses by Category (Yahoo-style)
│   ├─ Food & Dining
│   │   ├─ Restaurants
│   │   ├─ Bakeries
│   │   └─ Cafes
│   ├─ Services
│   │   ├─ Plumbers
│   │   ├─ Electricians
│   │   └─ Landscaping
│   └─ Retail
│       ├─ Clothing
│       ├─ Hardware
│       └─ Books
├─ Map View
├─ Current Deals/Sales
└─ Local Want Ads
```

### Data Maintenance

Three-layer approach:
1. **Business owners**: Primary responsibility, Lapideus tools make it easy
2. **Local moderators**: Fill gaps, verify accuracy
3. **Automated**: Sync from other sources, flag stale data

### golden.earth Bridge

- Normal web version with richer media (photos, video)
- Consistent UI across domains (desmoines.ia.golden.earth)
- Serves as funnel: normal web → Lapideus adoption
- Lapideus data auto-exports to web presence

---

## ERP Capabilities

### Inventory Management

```
Entitas: "Inventory::smiths_woodcraft_stock"
├─ genus: "Inventory"
├─ proprietates:
│   ├─ items: [
│   │   { sku, name, quantity, reorder_point, supplier }
│   │   ...
│   │ ]
│   └─ last_audit: "2025-12-15"
├─ relationes:
│   ├─ "belongs_to" → Business entity
│   └─ "triggers_alerts" → Salve workflow (low stock)
```

### Accounting Integration

Every transaction automatically:
- Creates ledger entries (double-entry)
- Categorizes by trade-specific defaults
- Tracks cost of goods sold
- Maintains audit trail

### Tax Compliance

Built-in support for:
- Sales tax calculation and collection
- 1099 generation for contractors
- Quarterly tax estimates
- Year-end reporting
- State-specific requirements (US only)

**Philosophy**: The system acts as a "shield" against bureaucratic complexity, leveling the playing field between sole proprietors and large corporations.

---

## Pricing and Transparency

### Inverted Model

Traditional: Seller sets price → Buyer accepts/rejects
Mercatura: Buyer sets budget → Sellers bid with cost breakdown

### Cost Transparency

When sellers bid, they can show:
- Material costs (from supply chain)
- Labor estimate
- Overhead allocation
- Margin

**Goal**: Educate buyers about trade economics, create informed negotiation.

### Automatic Cost Tracking

System tracks:
- Purchase prices of materials
- Time spent on orders
- Overhead categories

Sellers don't need extra work — Salve integration automates disclosure.

---

## Identity and Trust

### Full Legal Identity

Required for all commercial activity:
- Real name and contact info visible to counterparty
- Address verified (for physical goods shipping)
- License key tied to identity
- Revocable for violations

### Reputation Mechanisms

| Type | Description |
|------|-------------|
| Transaction history | Completed orders, ratings |
| Guild certification | Skill levels verified by peers |
| Portfolio | Past work examples |
| Badges/achievements | System-tracked progression |
| Time in business | Longevity indicator |

### Dispute Resolution

Designed via Smaragda simulation:
- Structured escalation paths
- Message-based mediation first
- Guild involvement for members
- License consequences for bad actors
- Merkle-chained evidence trail

---

## Communication

### Structured Messages

Wizards for common scenarios:
- Order confirmation
- Shipping notification
- Delivery follow-up
- Question about specifications
- Change request

Each message type has semantic fields, not just free text.

### Free-form Available

Normal messaging for exceptions, but:
- Attached to transaction entity
- Part of audit trail
- Searchable/filterable

---

## Timing and Sync

### Daily Sync Default

Mercatura follows Lapideus daily sync:
- Want ads posted today → visible tomorrow
- Bids submitted → arrive next batch
- Acceptance → confirmed next day

**Philosophy**: Commerce should be deliberate, not impulsive.

### Exceptions (TBD)

Smaragda testing may reveal edge cases requiring faster sync:
- Time-sensitive perishables?
- Urgent service needs?

Design exceptions carefully to avoid arms race.

---

## Constraints (Lapideus Media Rules)

### Allowed

- Text descriptions (monospace, structured)
- 16-color images (product photos, logos)
- Structured data (catalogs, specifications)

### Not Allowed

- Video (except political sphere)
- Rich media marketing
- Arbitrary file attachments (except for digital goods sale)

### Implication

Marketing through quality and reputation, not flashy media.

---

## First Milestone

### Scope: Local Business Directory

1. **Business registration** with full identity
2. **Category taxonomy** (Yahoo-style hierarchy)
3. **Town hub views** browsable by location
4. **Basic presence** (name, address, hours, contact, categories)
5. **Map integration** (16-color aesthetic)

### Out of Scope for V1 Milestone

- Want ads and bidding
- Inventory management
- Full accounting integration
- Payment processing
- Guild infrastructure

### Success Criteria

1. Business can register presence in their town
2. Users can browse all businesses in a town
3. Category navigation works
4. Basic contact/hours info visible
5. Data persists across app restarts
6. Family members can see each other's test businesses

---

## Future Phases

### Phase 2: Want Ads

- Structured demand broadcasting
- Bid submission and acceptance
- Basic transaction flow

### Phase 3: Full Transactions

- Payment integration (plugin architecture)
- Shipping workflow support
- Transaction history

### Phase 4: ERP Features

- Inventory management
- Accounting integration
- Tax compliance tools

### Phase 5: Guilds

- Collective organization
- Certification system
- Work distribution

### Phase 6: AI Integration

- Marketplace search assistance
- Demand/supply matching
- Cost estimation

---

## Dependencies

### Existing (from rhubarb)

- `entitas_repositorium.h/c` - Entity storage
- `persistentia.h/c` - Event log persistence
- `nuntium.h/c` - Wire format for sync
- Widget system for UI

### Required from Other Specs

- **Lapideus**: Network sync, identity/licensing, media constraints
- **Salve**: Control plane for business operations
- **Scientia**: Knowledge base for trade education
- **Smaragda**: Simulation for dispute resolution design

---

## Open Questions

1. **Payment processor integration**: Which processors? Plugin API design?
2. **Sales tax complexity**: State-by-state rules, nexus determination?
3. **Dispute resolution specifics**: What does Smaragda reveal?
4. **Guild governance models**: What patterns work best?
5. **golden.earth relationship**: Technical bridge implementation?
6. **Category taxonomy source**: Build from scratch or start from existing?
7. **Privacy vs transparency balance**: How much cost info is required vs optional?

---

## Appendix: Competitive Positioning

| Existing System | What Mercatura Replaces |
|-----------------|------------------------|
| eBay/Etsy/Amazon | Marketplace discovery and transactions |
| Shopify/Square | Small business commerce tools |
| QuickBooks | Accounting and tax compliance |
| ERP systems | Full business operations |
| Yellow Pages | Local business directory |
| Yelp | Business discovery (without review gaming) |

**Key difference**: All-in-one, no SaaS dependencies, v1 forever, aligned with user (not advertiser) interests.
