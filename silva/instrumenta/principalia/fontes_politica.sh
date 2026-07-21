# fontes_politica.sh - politica manualis amalgamatis silvae
# (fontatur a tools/amalgama_fontes_generare.sh; LISTAE derivantur,
# haec plagula POLITICAM solam fert - quod derivatio scire nequit)

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

# bases quarum excludenda COMPILATORE metuntur (messis: tools/
# amalgama_excludenda_generare.sh); friatio = servanda (politica
# manualis, numquam messa)
bases_excludendae() {
    echo "piscina chorda chorda_aedificator tabula_dispersa xar"
}
