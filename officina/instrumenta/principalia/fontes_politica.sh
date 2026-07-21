# fontes_politica.sh - politica manualis amalgamatis officinae
# (fontatur a tools/amalgama_fontes_generare.sh; LISTAE derivantur,
# haec plagula POLITICAM solam fert - quod derivatio scire nequit)

# radices derivationis = corpora propria officinae
radices() {
    ls officina/fontes/*.c
}

# derivata quae in listas NON intrant, cum causa:
#   silva/amalgama/silva.{h,c}: dependentia externa EX ARCHITECTURA
#   (PROOEMIUM #include "silva.h" + hospes duplex in amalgamare.sh),
#   non plagula amalgamatis
#   officina_indicium.h: possessio capitis publici - officina.h
#   API indicii declarat, caput internum non emittitur
exclusa_est() {
    case "$1" in
        silva/amalgama/silva.h|silva/amalgama/silva.c) return 0 ;;
        officina/fontes/officina_indicium.h) return 0 ;;
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
        *)                  echo "NIHIL" ;;
    esac
}
