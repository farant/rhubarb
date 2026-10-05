#!/bin/bash
# docs/instrumenta/regenerare.sh - folia generata libri regenerat.
#
# Usus: ./docs/instrumenta/regenerare.sh   (ex radice arboris)
#
# 1. Folia visionum ex schedis (project-specs/visiones-extractae/*.md):
#    tabularium, textus, instrumenta. brighton.html NON generatur: prosa
#    et quaestiones manu scriptae sunt; inventarium eius in scheda vivit.
# 2. Notae status in visiones/opera.html ex inventario tabularii
#    'visiones operum (status)'.
#
# CAVE: inventarium ex arbore PRINCIPALI legitur (TABULARIUM_RADIX,
# ordinarie ../rhubarb). In arbore laboris (secunda, tertia) frigida.sh
# viam frigidam currit, quae tabularium.db vagum creare et annalibus
# addere potest (quaestio ...VSY50E): numquam hic currendum.
set -euo pipefail

RADIX=$(cd "$(dirname "$0")/../.." && pwd)
TABULARIUM_RADIX=${TABULARIUM_RADIX:-$RADIX/../rhubarb}
INSTR=$RADIX/docs/instrumenta
SCHEDAE=$RADIX/project-specs/visiones-extractae
VISIONES=$RADIX/docs/visiones

python3 "$INSTR/pagina_visionis.py" "$SCHEDAE/tabularium-visio.md" Tabularium tabularium-visio.md \
    'The source is a synthesis of a Fran + Claude conversation (2026-07-14), not Fran verbatim; claims it attributes to Fran are marked (Fran).' \
    > "$VISIONES/tabularium.html"
python3 "$INSTR/pagina_visionis.py" "$SCHEDAE/text-stack-visio.md" 'Text stack' text-stack-visio.md \
    'The source is a research brief dated 2026-09-21 (author given as "@Someone"), with an addendum that summarises Fran’s Brighton note; not Fran’s words.' \
    > "$VISIONES/textus.html"
python3 "$INSTR/pagina_visionis.py" "$SCHEDAE/silva-instrumenta-visio.md" 'Silva instrumenta' silva-instrumenta-visio.md \
    'The source is a synthesis of a Fran + Claude conversation (2026-07-10), not Fran verbatim; claims it attributes to Fran are marked (Fran).' \
    > "$VISIONES/instrumenta.html"
python3 "$INSTR/pagina_visionis.py" "$SCHEDAE/exemplar-programmandi.md" 'Exemplar programmandi' \
    'the ledger vision record 01M3XQVDSY87M80CKNDTFA71YH' \
    'The source is a ledger vision record of Fran’s ideas (2026-10-02, eight notes), written in Latin by Claude; the English is Claude’s translation. (c) is the core text, (n1)–(n8) the notes.' \
    > "$VISIONES/exemplar.html"
python3 "$INSTR/pagina_visionis.py" "$SCHEDAE/stml-visio.md" 'STML' stml-visio.md \
    'The source is a consolidation by Claude (2026-08-10, sections added to 2026-09-03) of Fran’s pre-STML notes (G1, G2) and design conversations; claims it attributes to Fran are marked (Fran), and its last section is recorded verbatim in substance from Fran.' \
    > "$VISIONES/stml.html"
python3 "$INSTR/pagina_visionis.py" "$SCHEDAE/knotapel-plexitas-visio.md" 'Knotapel: threads and plexity' knotapel-plexitas-visio.md \
    'The source is Fran’s own document (2026-10-05), a synthesis of Fran + Claude conversations from July 2025 to June 2026 that itself marks the earliest ones as ideas to test; the last section is Claude’s review, kept apart.' \
    > "$VISIONES/plexitas.html"
python3 "$INSTR/pagina_visionis.py" "$SCHEDAE/mathematica-bibliothecae-visio.md" 'Math library map' mathematica-bibliothecae-visio.md \
    'The source is Fran’s own document (2026-10-05); the last section is Claude’s review, kept apart.' \
    > "$VISIONES/mathematica.html"
echo "folia visionum: tabularium, textus, instrumenta, exemplar, stml, plexitas, mathematica"

TSV=$(mktemp)
trap 'rm -f "$TSV"' EXIT
(cd "$TABULARIUM_RADIX" && ./gesta/frigida.sh -inventarium "visiones operum (status)") > "$TSV" 2>/dev/null
python3 "$INSTR/status_visionum.py" "$TSV" "$VISIONES/opera.html"

# 3. Bibliothecae: index et folia brevia ex inventario 'bibliothecae (include)'
#    POST FUSIONEM main: lentes DERIVATAE (plagulae, mutatum) renovandae -
#      python3 docs/instrumenta/derivare_bibliothecas.py . > cellae.json
#    (capita nova in stderr; ordines adde, deinde cellae fons 'derivatum'
#    per instrumentum tabularii 'inventarium'); descriptiones capitum
#    novorum et mutatorum manu.
#    et greges.html ex inventario 'greges bibliothecarum'
TSV_B=$(mktemp)
TSV_G=$(mktemp)
(cd "$TABULARIUM_RADIX" && ./gesta/frigida.sh -inventarium "bibliothecae (include)") > "$TSV_B" 2>/dev/null
(cd "$TABULARIUM_RADIX" && ./gesta/frigida.sh -inventarium "greges bibliothecarum") > "$TSV_G" 2>/dev/null
python3 "$INSTR/pagina_bibliothecae.py" "$TSV_B" "$RADIX" "$TSV_G"
rm -f "$TSV_B" "$TSV_G"

# 4. Examen: codices ex fonte silvae + glossae Anglicae (examen_glossae.tsv)
python3 "$INSTR/pagina_examinis.py" "$RADIX"

# 5. Briar: facultates ex inventario 'briar facultates' + exempla ex fixis briar
TSV_F=$(mktemp)
(cd "$TABULARIUM_RADIX" && ./gesta/frigida.sh -inventarium "briar facultates") > "$TSV_F" 2>/dev/null
python3 "$INSTR/pagina_briar.py" "$TSV_F" "$RADIX"
rm -f "$TSV_F"

# 6. Fabrica: facultates ex inventario 'fabrica facultates' + exempla ex aedificatio.stml
TSV_FA=$(mktemp)
(cd "$TABULARIUM_RADIX" && ./gesta/frigida.sh -inventarium "fabrica facultates") > "$TSV_FA" 2>/dev/null
python3 "$INSTR/pagina_fabricae.py" "$TSV_FA" "$RADIX"
rm -f "$TSV_FA"

# 7. STML: itinerarium syntaxis (exempla per instrumenta vera currunt) + facultates
#    ex inventario 'stml facultates'. Instrumenta: STML, CANON_EXAMEN (vide pagina_stml.py).
TSV_S=$(mktemp)
(cd "$TABULARIUM_RADIX" && ./gesta/frigida.sh -inventarium "stml facultates") > "$TSV_S" 2>/dev/null
TABULARIUM_RADIX="$TABULARIUM_RADIX" python3 "$INSTR/pagina_stml.py" "$TSV_S" "$RADIX"
rm -f "$TSV_S"
