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
        lectiones)          echo "EXCLUDENDA_LECTIONUM" ;;
        codificator_a64)    echo "EXCLUDENDA_CODIFICATORIS" ;;
        memoria_exsecutabilis_macos)
                            echo "EXCLUDENDA_MEMORIAE_EXSECUTABILIS" ;;
        *)                  echo "NIHIL" ;;
    esac
}

# bases quarum excludenda COMPILATORE metuntur (messis: tools/
# amalgama_excludenda_generare.sh); friatio = servanda (politica
# manualis, numquam messa)
bases_excludendae() {
    # lectiones (2026-10-27): machinula libc hospitis per librum
    # lectionum notat (fabrica-8) - lectiones_ambitus inusitatum
    # codificator_a64, memoria_exsecutabilis_macos (2026-11-12, M5 T6):
    # officina_effugium trampolinum codificat et aream exsecutabilem
    # reservat - formae codificatoris plures nondum consumuntur
    echo "piscina chorda chorda_aedificator tabula_dispersa xar lectiones codificator_a64 memoria_exsecutabilis_macos"
}
