# LEGATUS — registration (dev, --plugin-dir)

The plugin dir lives IN the repo, so `${CLAUDE_PLUGIN_ROOT}`-relative
paths stay valid (marketplace-cache portability = named non-goal v0).

## One-time setup

1. Prebuild (avoids cold-compile eating into startupTimeout):
   `./officina/legatus.sh </dev/null >/dev/null` — builds objects,
   exits 1 (EOF without exit is the honest code).
2. clangd-lsp is already disabled for this repo
   (.claude/settings.json, 2026-07-13).

## Per-session / permanent

- Dev: `claude --plugin-dir "$PWD/officina/legatus-plugin"`
- Verify: `claude --debug` shows LSP negotiation; Ctrl+O shows
  diagnostics received; /plugin lists the server.

## What it serves (v0)

- publishDiagnostics on didOpen/didChange (.c AND .h; examen
  judgment; pinned exclusions publish empty; INFRA dropped)
- hover (use-sites: `titulus : typus`, colloquium's voice)
- documentSymbol (file-scope symbols)
- .h didSave → warm-context rebuild + re-judge of open documents

Shakedown: the examen-custos hook stays live alongside legatus (a
deliberate differential) until the daily-driver bar, then the hook
is disabled (script kept for headless contexts).
