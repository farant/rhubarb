# fontes_politica.sh - politica manualis amalgamatis tesserae
# (fontatur a tools/amalgama_fontes_generare.sh; LISTAE derivantur,
# haec plagula POLITICAM solam fert - quod derivatio scire nequit)

# radices derivationis = corpora propria tesserae
radices() {
    ls tessera/fontes/*.c
}

# derivata quae in listas NON intrant, cum causa:
#   chorda.{h,c}: tessera TYPUM solum adhibet (TesseraChorda in
#   tessera.h possidetur), corpus pondus mortuum - exclusio
#   consulta manifesti originalis
exclusa_est() {
    case "$1" in
        include/chorda.h|lib/chorda.c) return 0 ;;
    esac
    return 1
}

# politica per basem (vendicata; NIHIL = ordinarius)
servanda_pro() {
    case "$1" in
        *) echo "NIHIL" ;;
    esac
}
excludenda_pro() {
    case "$1" in
        piscina)            echo "EXCLUDENDA_PISCINAE" ;;
        chorda_aedificator) echo "EXCLUDENDA_AEDIFICATORIS" ;;
        utf8)               echo "EXCLUDENDA_UTF8" ;;
        *)                  echo "NIHIL" ;;
    esac
}
