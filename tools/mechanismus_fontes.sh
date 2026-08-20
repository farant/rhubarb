# tools/mechanismus_fontes.sh - fontes MECHANISMI amalgamandi
#
# Fontatur (non exsecutus). Viae RADICI repositorii relativae.
#
# Mechanismus amalgamandi silvae PROPRIUS est sed a TRIBUS
# amalgamatoribus petitur (silva, tessera, officina): quisque
# manifestum SUUM fert, mechanismum COMMUNEM.
#
# UNUS FONS VERITATIS. Haec lista prius ter descripta erat - in
# silva/amalgamare.sh, tessera/amalgamare.sh, officina/amalgamare.sh
# - et cum silva_unitates.c ex amalgamatore extractum est (fed3e78),
# SOLA silva emendata est. Consequentia, per menses tacita et hodie
# mensurata (2026-08-20):
#   - tools/amalgama_excludenda_generare.sh pro silva OMNINO fractus
#     ('Undefined symbols: _silva_lineam_finire');
#   - tessera/amalgamare.sh et officina/amalgamare.sh eodem errore
#     fracti - id est, AMALGAMA HORUM DUORUM PROIECTORUM CONFICI NON
#     POTERAT.
# Nemo id sciebat quia nemo ea currebat. Extractio una, tres
# fracturae tacitae.
#
# LECTIO: lista aut exclusio UNIUS CASUS scripta, cuius ratio
# CLASSEM tegit, membrum secundum tacite fallit. Hic classis est.
mechanismus_silvae_fontes() {
    echo "silva/instrumenta/silva_amalgama.c"
    echo "silva/instrumenta/silva_unitates.c"
}
