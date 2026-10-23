#!/bin/bash
# probare.sh - porta natalis pythonicae (stdlib python3 systematis)
#   probare.sh            pars PURA: probatio_silva.py (fictae, mundi
#                         temporarii; instrumenta domus ex fontibus suis)
#   probare.sh -arboris   pars ARBORIS VIVAE: probatio_silva_arboris.py
#                         (corpus, portae verae, bin/fabrica super arborem)
# Scissio ex auditu lectionum per greges (2026-10-23, desideratum
# …VXWK8): legit partis purae = fontes instrumentorum quae greges eius
# currunt (lectiones observatae), partis arboris = arbor tota.
# <porta titulus="pythonica" gradus="fusio" tegit="pythonica/*.py" legit="include/* lib/*.c *unitas.stml unitates.stml *aedificatio.stml aedilis.stml fabrica.stml silva/fontes/* silva/instrumenta/* silva/amalgama/* oratio/fontes/* oratio/instrumenta/* oratio/grammatica/* materia/fontes/* materia/grammatica/* md/fontes/* md/instrumenta/* toml/fontes/* toml/instrumenta/* css/fontes/* css/instrumenta/* crusta/effectus_mandata.stml project-specs/exhibita/md-html-b1.stml silva/probationes/fixa/formatoris/exclusiones.txt tools/aedilis_porta.sh tools/iudex_unitatum.c"/>
# <porta titulus="pythonica-arboris" gradus="fusio" tegit="pythonica/*.py" legit="*"/>
cd "$(dirname "$0")/.." || exit 2
if [ "${1:-}" = "-arboris" ]; then
    exec python3 pythonica/probatio_silva_arboris.py
fi
exec python3 pythonica/probatio_silva.py
