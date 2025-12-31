# Lapideus v2 - Implementation Specification

Date: 2025-12-30
Status: Draft
Based on: lapideus.md, codebase exploration
Etymology: Latin "lapideus" = "of stone" — solid, enduring, built to last

---

## Executive Summary

This document provides implementation-specific details for Lapideus, grounded in the existing rhubarb codebase. The infrastructure is remarkably complete — TCP server, HTTP parser, event reactor, entity repositories, and asset bundling are all production-ready.

**Key Finding**: Most of Lapideus can be built by composing existing components. The primary new work is the sync protocol, sphere UIs, and PWA client.

---

## Existing Infrastructure (Ready to Use)

### Networking Stack (Complete)

| Component | File | Capabilities |
|-----------|------|--------------|
| **TCP Client** | `tcp.h/tcp_posix.c` | Connect, send, receive, non-blocking I/O |
| **TCP Server** | `tcp.h/tcp_posix.c` | Listen, accept, IPv4/IPv6, configurable options |
| **HTTP Client** | `http.h/http.c` | GET/POST/etc, HTTPS, redirects, chunked encoding |
| **HTTP Parser** | `http.h/http.c` | Incremental parsing, headers, body, keep-alive |
| **TLS** | `tls.h` | macOS Security.framework, certificate verification |
| **Reactor** | `reactor.h/reactor.c` | poll()-based event loop, timers, 1000 FDs |
| **Router** | `router.h/router.c` | Path patterns, params, REST API routing |
| **URL/URI** | `url.h/uri.h` | Parsing, encoding, resolution |

**This means**: PWA serving, inter-node communication, and sync all have foundations.

### Entity & Persistence (Complete)

| Component | File | Capabilities |
|-----------|------|--------------|
| **Entity Repository** | `entitas_repositorium.h/impl.c` | Full CRUD, relationships, tags, queries |
| **Event Sourcing** | `persistentia.h` | 8 event types, append-only log |
| **File Backend** | `persistentia_nuntium.c` | Protobuf serialization, length-prefixed |
| **Memory Backend** | `persistentia_memoria.c` | In-memory for testing/ephemeral |
| **Nuntium** | `nuntium.h/c` | Protobuf-style wire format |

**Key Capability**: Multiple repositories can coexist in one process. Each "internet" gets its own `EntitasRepositorium` with separate `Persistentia` backend.

### Asset Bundling (Complete)

| Component | File | Capabilities |
|-----------|------|--------------|
| **Capsula** | `capsula.h/c` | Compile-time asset embedding |
| **Capsula Caudae** | `capsula_caudae.h/c` | Post-link asset appending (gigabytes) |
| **Compression** | `flatura.h` | gzip deflate/inflate |

**For Project Gutenberg**: Use `capsula_caudae` to append 1000+ books after compilation. No compiler limits.

### Widget/UI System (Complete)

| Component | File | Capabilities |
|-----------|------|--------------|
| **Widget** | `widget.h/c` | Vtable pattern, focus management |
| **Schirmata** | `schirmata.h/c` | 10 screens, mode switching |
| **Widget Registry** | `registrum_widget.h` | Factory pattern for widgets |
| **Drawing** | `delineare.h` | Canvas primitives, clipping |

**For Spheres**: Extend schirmata to support sphere collections (see sphere architecture below).

---

## Architecture

### Per-Internet Repository Isolation

```c
/*
 * Each "internet" is an isolated entity namespace
 * with its own persistentia backend.
 */
nomen structura {
    chorda*              id;           /* Internet identifier */
    chorda*              titulus;      /* Human name */
    chorda*              via_log;      /* Path to event log */

    Persistentia*        persistentia; /* File-based backend */
    EntitasRepositorium* repo;         /* Entity storage */

    /* Sync state */
    i64                  ultimum_sync; /* Last sync timestamp */
    i32                  batch_index;  /* Current batch number */

} Interrete;

/*
 * Main process manages multiple internets.
 */
nomen structura {
    Piscina*             piscina;
    InternamentumChorda* intern;       /* Global (enables cross-internet queries) */

    TabulaDispersa*      interretia;   /* id → Interrete* */
    Interrete*           interrete_proprium; /* User's private data */

    /* Server */
    TcpServus*           servus;
    Reactor*             reactor;
    Router*              router;

} LapideusProcessus;
```

### Creating an Internet

```c
Interrete* lapideus_interrete_creare(
    LapideusProcessus* proc,
    chorda*            id,
    chorda*            titulus,
    chorda*            via_log
) {
    Interrete* inter = piscina_allocare(proc->piscina, magnitudo(Interrete));

    /* Create file-based persistentia */
    inter->persistentia = persistentia_nuntium_creare(proc->piscina, via_log);

    /* Create repository (replays existing events) */
    inter->repo = entitas_repositorium_creare(proc->piscina, inter->persistentia);

    /* Register in process */
    tabula_dispersa_inserere(proc->interretia, *id, inter);

    redde inter;
}
```

### Internet Isolation

- Each internet has separate `Persistentia` → separate event log file
- Each internet has separate `EntitasRepositorium` → separate entity namespace
- Shared `InternamentumChorda` → enables cross-internet string comparison for aggregation
- User aggregates locally by querying multiple repos

---

## Sync Protocol

### Batch Format

Daily batches use the existing nuntium format with a wrapper:

```c
/*
 * Batch header tags
 */
#define BATCH_TAG_INTERNET_ID    1   /* Which internet */
#define BATCH_TAG_DATE           2   /* Batch date (YYYYMMDD as i32) */
#define BATCH_TAG_SEQUENCE       3   /* Batch sequence number */
#define BATCH_TAG_PREV_HASH      4   /* Hash of previous batch (Merkle chain) */
#define BATCH_TAG_EVENT_COUNT    5   /* Number of events in batch */
#define BATCH_TAG_EVENTS         6   /* Repeated: nested event messages */
#define BATCH_TAG_SIGNATURE      7   /* Server signature */

/*
 * Batch file structure:
 * [varint total_length][batch_header][event₁][event₂]...[signature]
 */
```

### Batch Creation (Server Side)

```c
chorda lapideus_batch_creare(
    Interrete*   inter,
    i32          date,         /* YYYYMMDD */
    Piscina*     piscina
) {
    NuntiumScriptor* scriptor = nuntium_scriptor_creare(piscina, MMMM);

    /* Write header */
    nuntium_scribere_chorda(scriptor, BATCH_TAG_INTERNET_ID, *inter->id);
    nuntium_scribere_varint(scriptor, BATCH_TAG_DATE, date);
    nuntium_scribere_varint(scriptor, BATCH_TAG_SEQUENCE, inter->batch_index);

    /* Get events since last batch */
    Xar* events = inter->repo->legere_eventus_post_indicem(
        inter->repo->datum, inter->last_batch_event_index);

    nuntium_scribere_varint(scriptor, BATCH_TAG_EVENT_COUNT, xar_longitudo(events));

    /* Write each event as nested message */
    per (i32 i = 0; i < xar_longitudo(events); i++) {
        Eventum* ev = *(Eventum**)xar_accedere(events, i);
        chorda ev_data = _serialize_eventum(ev, piscina);
        nuntium_scribere_nuntium(scriptor, BATCH_TAG_EVENTS, ev_data);
    }

    /* TODO: Add signature */

    inter->batch_index++;
    redde nuntium_scriptor_datum(scriptor);
}
```

### Batch Application (Client Side)

```c
SmaragdaStatus lapideus_batch_applicare(
    Interrete*   inter,
    chorda       batch_data,
    Piscina*     piscina
) {
    NuntiumLector* lector = nuntium_lector_creare(piscina, batch_data);

    i32 tag, wire_type;
    dum (nuntium_legere_field(lector, &tag, &wire_type)) {
        commutatio (tag) {
            casus BATCH_TAG_EVENTS: {
                chorda ev_data = nuntium_legere_nuntium(lector);
                Eventum* ev = _deserialize_eventum(ev_data, piscina);

                /* Apply to repository */
                _applicare_eventum(inter->repo, ev);
                frange;
            }
            ordinarius:
                nuntium_saltare_field(lector, wire_type);
        }
    }

    redde LAPIDEUS_SUCCESSUS;
}
```

### Sync Modes

| Mode | Timing | Use Case |
|------|--------|----------|
| **Inter-Internet** | Daily batches | Public internets, news updates |
| **Same-User** | On-demand | Phone ↔ laptop sync |
| **Thumb Drive** | Manual | Offline distribution |

```c
/*
 * Daily sync (client pulls from server)
 */
void lapideus_sync_quotidianus(LapideusProcessus* proc, Interrete* inter) {
    /* Connect to internet's server */
    TcpConnexio* conn = tcp_connectere(inter->server_host, inter->server_port, proc->piscina);

    /* Request batches since last sync */
    chorda request = _build_sync_request(inter->batch_index);
    tcp_mittere_omnia(conn, request);

    /* Receive and apply batches */
    dum (/* more batches */) {
        chorda batch = _receive_batch(conn, proc->piscina);
        lapideus_batch_applicare(inter, batch, proc->piscina);
    }

    tcp_claudere(conn);
}

/*
 * Same-user sync (bidirectional, on-demand)
 */
void lapideus_sync_proprius(LapideusProcessus* proc) {
    /* No daily batching - immediate sync */
    /* Includes PKM, Salve, non-internet data */
    /* Uses same batch format but different timing */
}
```

---

## Server Architecture

### HTTP Server for PWA

Leverage existing reactor and router:

```c
void lapideus_servus_initiare(LapideusProcessus* proc, i32 port) {
    /* Create TCP server */
    TcpServusResultus res = tcp_servus_creare(port, proc->piscina);
    proc->servus = res.servus;
    tcp_servus_auscultare(proc->servus, CXXVIII);

    /* Create reactor */
    proc->reactor = reactor_creare(proc->piscina);

    /* Create router for HTTP endpoints */
    proc->router = router_creare(proc->piscina);

    /* PWA static files */
    router_get(proc->router, "/", _handler_pwa_index);
    router_get(proc->router, "/app.js", _handler_pwa_js);
    router_get(proc->router, "/manifest.json", _handler_pwa_manifest);
    router_get(proc->router, "/sw.js", _handler_service_worker);

    /* Sync API */
    router_get(proc->router, "/api/sync/:internet", _handler_sync_pull);
    router_post(proc->router, "/api/sync/:internet", _handler_sync_push);
    router_get(proc->router, "/api/batch/:internet/:sequence", _handler_batch_get);

    /* Entity API (for PWA) */
    router_get(proc->router, "/api/entities/:internet", _handler_entities_list);
    router_get(proc->router, "/api/entity/:internet/:id", _handler_entity_get);
    router_post(proc->router, "/api/entity/:internet", _handler_entity_create);

    /* Register server in reactor */
    reactor_adicere(proc->reactor, tcp_servus_obtinere_fd(proc->servus),
                    REACTOR_LEGERE, _on_accept, proc);
}
```

### Connection Handling

```c
void _on_accept(i32 fd, ReactorEventus eventus, void* datum) {
    LapideusProcessus* proc = (LapideusProcessus*)datum;

    TcpConnexio* conn = tcp_servus_accipere(proc->servus, FALSUM, proc->piscina);
    si (conn == NIHIL) redde;

    /* Create connection context */
    ConnexioContext* ctx = piscina_allocare(proc->piscina, magnitudo(ConnexioContext));
    ctx->conn = conn;
    ctx->parser = http_parser_creare(proc->piscina);
    ctx->proc = proc;

    /* Register for read events */
    reactor_adicere(proc->reactor, tcp_obtinere_fd(conn),
                    REACTOR_LEGERE, _on_read, ctx);
}

void _on_read(i32 fd, ReactorEventus eventus, void* datum) {
    ConnexioContext* ctx = (ConnexioContext*)datum;

    /* Read data */
    i8 buffer[MMMM];
    i32 n = tcp_recipere(ctx->conn, buffer, MMMM);
    si (n <= 0) { /* handle close/error */ }

    /* Feed to HTTP parser */
    chorda data = { buffer, n };
    http_parser_adicere(ctx->parser, data);

    /* Check if request complete */
    si (http_parser_est_completa(ctx->parser)) {
        HttpPetitioServeri* req = http_parser_obtinere_petitio(ctx->parser);

        /* Route to handler */
        HttpResponsum resp = { 0 };
        RoutaParams params = { 0 };

        FunctioRutae handler = router_matching(ctx->proc->router, req, &params);
        si (handler) {
            handler(req, &resp, &params);
        } alioquin {
            resp.status = CCCCIV; /* 404 */
        }

        /* Send response */
        _send_response(ctx->conn, &resp);

        /* Reset parser for keep-alive */
        http_parser_reset(ctx->parser);
    }
}
```

---

## PWA Architecture

### Separate Codebase (TypeScript)

```
pwa/
├── src/
│   ├── index.ts           # Entry point
│   ├── sync.ts            # Sync engine (fetch batches)
│   ├── storage.ts         # IndexedDB wrapper
│   ├── entities.ts        # Entity CRUD operations
│   ├── spheres/
│   │   ├── libri.ts       # Books sphere
│   │   └── intelligentia.ts # Intelligence sphere
│   └── ui/
│       ├── canvas.ts      # Canvas rendering (rhubarb aesthetic)
│       ├── widgets.ts     # Widget abstractions
│       └── colors.ts      # 16-color palette
├── public/
│   ├── index.html
│   ├── manifest.json
│   └── sw.js              # Service worker for offline
├── package.json
└── tsconfig.json
```

### PWA Sync Engine

```typescript
// sync.ts
class SyncEngine {
    private db: IDBDatabase;
    private serverUrl: string;

    async sync(internetId: string): Promise<void> {
        // Get last sync index
        const lastIndex = await this.getLastBatchIndex(internetId);

        // Fetch new batches
        const response = await fetch(
            `${this.serverUrl}/api/sync/${internetId}?since=${lastIndex}`
        );
        const batches = await response.arrayBuffer();

        // Parse and apply batches (nuntium format)
        await this.applyBatches(internetId, batches);
    }

    async push(internetId: string, events: Event[]): Promise<void> {
        // For same-user sync, push local changes
        const body = this.serializeEvents(events);
        await fetch(`${this.serverUrl}/api/sync/${internetId}`, {
            method: 'POST',
            body
        });
    }
}
```

### PWA Canvas Rendering

```typescript
// canvas.ts - Match rhubarb visual vocabulary
const PALETTE = [
    '#000000', '#1a1a2e', '#16213e', '#0f3460',
    '#533483', '#e94560', '#f39422', '#ffd460',
    // ... 16 colors total
];

class CanvasRenderer {
    private ctx: CanvasRenderingContext2D;
    private scale: number = 2; // For retina

    drawText(x: number, y: number, text: string, color: number): void {
        this.ctx.fillStyle = PALETTE[color];
        this.ctx.font = `${8 * this.scale}px monospace`;
        this.ctx.fillText(text, x * this.scale, y * this.scale);
    }

    drawRect(x: number, y: number, w: number, h: number, color: number): void {
        this.ctx.fillStyle = PALETTE[color];
        this.ctx.fillRect(
            x * this.scale, y * this.scale,
            w * this.scale, h * this.scale
        );
    }
}
```

---

## Sphere Implementation

### Sphere as Widget Collection

```c
/*
 * A sphere is a cohesive UI domain with its own widgets.
 * User switches between spheres like switching apps.
 */
nomen structura {
    chorda*              titulus;       /* "Libri", "Intelligentia", etc */
    ManagerWidget*       manager;       /* Focus/event handling for sphere */
    Xar*                 widgets;       /* Xar<Widget*> */
    Entitas*             entitas;       /* Persistent state entity */
    b32                  activum;
} Sphaera;

/*
 * Each screen can have multiple spheres.
 */
nomen structura Schirma {
    /* ... existing fields ... */
    Sphaera              sphaerae[V];   /* 5 spheres: Libri, Intelligentia, Scientia, Politica, Mercatura */
    Sphaera*             sphaera_currens;
};
```

### Sphere Switching

```c
void schirma_activare_sphaeram(Schirma* schirma, i32 index) {
    schirma->sphaera_currens = &schirma->sphaerae[index];
    /* Focus goes to sphere's manager */
    /* Rendering switches to sphere's widgets */
}

/* Keyboard shortcut: Ctrl+1/2/3/4/5 */
b32 _tractare_sphere_switch(Eventus* ev, Schirma* schirma) {
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS && ev->datum.clavis.ctrl) {
        si (ev->datum.clavis.clavis >= CLAVIS_1 && ev->datum.clavis.clavis <= CLAVIS_5) {
            i32 index = ev->datum.clavis.clavis - CLAVIS_1;
            schirma_activare_sphaeram(schirma, index);
            redde VERUM;
        }
    }
    redde FALSUM;
}
```

---

## Book Bundling (Project Gutenberg)

### Using Capsula Caudae

```bash
# After compilation, append books to binary
./bin/capsula_caudae_adiungere libri_config.toml ./bin/lapideus
```

**libri_config.toml:**
```toml
[libri]
files = ["gutenberg/*.txt"]
compress = true
```

### Runtime Access

```c
void lapideus_libri_initiare(LapideusProcessus* proc, const char* argv0) {
    /* Open assets appended to binary */
    proc->libri_capsula = capsula_caudae_aperire(argv0, proc->piscina);

    si (proc->libri_capsula == NIHIL) {
        /* No books bundled - that's okay */
        redde;
    }

    /* Iterate available books */
    CapsulaIter iter;
    capsula_caudae_iter(proc->libri_capsula, &iter);

    dum (capsula_caudae_iter_proximum(&iter)) {
        chorda via = iter.via_currens;
        /* Register book in Libri sphere */
        _libri_registrare_textum(proc, via);
    }
}

chorda lapideus_libri_legere(LapideusProcessus* proc, chorda* via) {
    CapsulaFructus res = capsula_caudae_legere(proc->libri_capsula, via, proc->piscina);
    si (res.status != CAPSULA_SUCCESSUS) {
        redde (chorda){ NIHIL, 0 };
    }
    redde res.datum; /* Decompressed text */
}
```

---

## First Milestone: Family Sync

### Scope

1. Desktop server running on laptop
2. PWA installable on phones
3. One sphere working (Libri or Intelligentia)
4. Same-user sync (on-demand, not daily batched)
5. Offline capability on phone

### Components to Build

| Component | Effort | Dependencies |
|-----------|--------|--------------|
| `lapideus.h/c` - Core types | Small | None |
| `lapideus_sync.h/c` - Batch format | Medium | nuntium |
| `lapideus_servus.h/c` - HTTP server | Medium | tcp, reactor, router |
| PWA sync engine | Medium | fetch API |
| PWA IndexedDB storage | Small | browser APIs |
| PWA canvas rendering | Medium | None |
| Libri sphere (desktop) | Medium | widget system |
| Libri sphere (PWA) | Medium | PWA framework |

### Out of Scope for V1

- Public internets (multiple users)
- Daily batch timing enforcement
- Video streaming (Politica sphere)
- Commerce/payments (Mercatura sphere)
- AI content moderation
- License management
- Merkle-chain signatures

---

## Files to Create

### Core

| File | Purpose |
|------|---------|
| `include/lapideus.h` | Core types: Interrete, LapideusProcessus |
| `lib/lapideus.c` | Process lifecycle, internet management |
| `include/lapideus_sync.h` | Batch format, sync protocol |
| `lib/lapideus_sync.c` | Batch creation, application, sync |
| `include/lapideus_servus.h` | Server types, handlers |
| `lib/lapideus_servus.c` | HTTP server, PWA serving |

### Spheres

| File | Purpose |
|------|---------|
| `include/sphaera.h` | Sphere abstraction |
| `lib/sphaera.c` | Sphere management |
| `include/sphaera_libri.h` | Books sphere |
| `lib/sphaera_libri.c` | Book reading, glossing UI |

### PWA

| File | Purpose |
|------|---------|
| `pwa/src/index.ts` | Entry point |
| `pwa/src/sync.ts` | Sync engine |
| `pwa/src/storage.ts` | IndexedDB |
| `pwa/src/spheres/libri.ts` | Books sphere |

### Tests

| File | Purpose |
|------|---------|
| `probationes/probatio_lapideus.c` | Core tests |
| `probationes/probatio_lapideus_sync.c` | Sync protocol tests |

---

## Dependencies

### Existing (Ready)

- `tcp.h/c` - TCP client/server
- `http.h/c` - HTTP client, parser
- `reactor.h/c` - Event loop
- `router.h/c` - HTTP routing
- `entitas_repositorium.h/c` - Entity storage
- `persistentia.h/c` - Event sourcing
- `nuntium.h/c` - Wire format
- `capsula_caudae.h/c` - Asset bundling
- `widget.h/c` - UI widgets
- `schirmata.h/c` - Screen management

### New Required

- PWA build system (esbuild or similar)
- Service worker for offline

---

## Success Criteria (V1)

1. ✓ Desktop server starts and listens on port
2. ✓ PWA loads from desktop server
3. ✓ PWA installs as standalone app on phone
4. ✓ Create entity on desktop, see on phone after sync
5. ✓ Create entity on phone, see on desktop after sync
6. ✓ Phone works offline (cached data)
7. ✓ Libri sphere shows book text
8. ✓ Can add notes/highlights to book

---

## Open Questions

1. **PWA storage limits**: How much can IndexedDB hold? May need hybrid approach.
2. **Binary protocol in PWA**: Parse nuntium in JS or JSON API layer?
3. **Service worker sync**: Use Background Sync API or manual?
4. **Hot reload PWA**: How to update PWA when desktop app updates?
5. **Authentication**: How does phone prove it's same user?
