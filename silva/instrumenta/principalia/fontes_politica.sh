# fontes_politica.sh - politica manualis amalgamatis silvae
# (fontatur a tools/amalgama_fontes_generare.sh; LISTAE derivantur,
# haec plagula POLITICAM solam fert - quod derivatio scire nequit)

# ------------------------------------------------------------------
# Fontes amalgamatoris IPSIUS (praeter obiecta dependentiarum, quae
# vocantes proprie diversa habent: vectis ea recentia struit,
# scriptor excludendorum calefacta sumit).
#
# UNUS FONS VERITATIS. Haec lista bis exstabat - in silva/
# amalgamare.sh et in tools/amalgama_excludenda_generare.sh - et
# DIVERGIT: silva_unitates.c illic additum est (fed3e78), hic
# numquam, unde scriptor excludendorum pro silva per menses fractus
# iacuit ('Undefined symbols: _silva_lineam_finire') et NEMO SCIEBAT,
# quia nemo eum curreret. Mensuratum 2026-08-20.
#
# Viae RADICI repositorii relativae; vocans praefixum suum addat.
# (tessera + officina amalgamatorem uno fonte struunt - lista unius
# plagulae divergere vix potest - ergo hanc formam nondum petunt;
# si crescant, idem sequantur.)
# ------------------------------------------------------------------
amalgamatoris_fontes() {
    echo "silva/instrumenta/principalia/amalgamator.c"
    echo "silva/instrumenta/silva_amalgama.c"
    echo "silva/instrumenta/silva_unitates.c"
}

# radices derivationis = corpora propria silvae; tabulae fixturarum
# (sceletum/imparilis = grammaticae probationum generatoris) numquam
# amalgamantur
radices() {
    for f in silva/fontes/*.c; do
        case "$f" in
            */silva_tabulae_sceleti.c|*/silva_tabulae_imparilis.c)
                continue ;;
        esac
        echo "$f"
    done
}

exclusa_est() {
    case "$1" in
        # selectio: subsystema mortuum in clausura silvae (solum
        # stml_proximus_maior - exclusum - eam vocat); statica
        # cache eius variabilia inusitata sub -Werror essent, quae
        # mechanismus (functiones solas demittens) delere nequit.
        # VIA RETRO (quaestiones selectorum annotationum, frustum
        # C+): hanc lineam dele, servanda friationis crescant.
        # Caput include/selectio.h MANET (typi pro prototypis).
        lib/selectio.c) return 0 ;;
    esac
    return 1
}

# politica per basem (vendicata; NIHIL = ordinarius)
servanda_pro() {
    case "$1" in
        friatio) echo "SERVANDA_FRIATIONIS" ;;
        *)       echo "NIHIL" ;;
    esac
}
excludenda_pro() {
    case "$1" in
        piscina)            echo "EXCLUDENDA_PISCINAE" ;;
        chorda)             echo "EXCLUDENDA_CHORDAE" ;;
        chorda_aedificator) echo "EXCLUDENDA_AEDIFICATORIS" ;;
        tabula_dispersa)    echo "EXCLUDENDA_TABULAE" ;;
        xar)                echo "EXCLUDENDA_XARIS" ;;
        internamentum)      echo "EXCLUDENDA_INTERNAMENTI" ;;
        selectio)           echo "EXCLUDENDA_SELECTIONIS" ;;
        stml)               echo "EXCLUDENDA_STML" ;;
        *)                  echo "NIHIL" ;;
    esac
}

# bases quarum excludenda COMPILATORE metuntur (messis: tools/
# amalgama_excludenda_generare.sh); friatio = servanda (politica
# manualis, numquam messa). stml + internamentum + selectio ab
# silva_annotationes vendicata (2026-07-21).
bases_excludendae() {
    echo "piscina chorda chorda_aedificator tabula_dispersa xar internamentum selectio stml"
}
