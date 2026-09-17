/* probatio_excerptum.c - Excerptum fontis cum signo (planum
 * materia-sedes B5)
 *
 * Contractus: margo numeri lineae (cifrae, minimum IV) + " | ",
 * linea fontis (CR sublato, moderatores praeter tabulam ut spatium),
 * deinde margo vacuus + '^' ad initium et '~' per CHARACTEREM UTF-8
 * usque ad finem tractus aut lineae. Tabula in margine sub linea
 * SERVATUR, ergo signum columnam veram tenet etiam in fonte
 * tabulato.
 *
 * Chordae expectatae hic CONTRACTUS sunt: codicem ad eas emenda,
 * numquam eas ad codicem.
 */

#include "latina.h"
#include "credo.h"
#include "chorda_aedificator.h"
#include "excerptum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_probare (
                Piscina* piscina,
     constans character* fons,
                    s32  initium,
                    s32  finis,
                    i32  linea,
     constans character* expectatum)
{
     ChordaAedificator* aedificator;
                chorda  fructus;

    aedificator = chorda_aedificator_creare(piscina,
        (memoriae_index)CXXVIII);
    si (aedificator == NIHIL)
    {
        CREDO_CULPA ("aedificator");
        redde;
    }
    CREDO_VERUM (excerptum_scribere(aedificator, fons,
        (i32)strlen(fons), initium, finis, linea));
    fructus = chorda_aedificator_finire(aedificator);
    CREDO_CHORDA_AEQUALIS_LITERIS (fructus, expectatum);
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
  ChordaAedificator* aedificator;

    piscina = piscina_generare_dynamicum("probatio_excerptum",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans excerptum ---\n");

    /* signum sub charactere uno */
    _probare(piscina, "int x = 1;\n", (s32)IV, (s32)V, I,
        "   1 | int x = 1;\n     |     ^\n");
    /* tabula in margine SERVATUR: columna vera manet */
    _probare(piscina, "\tab\n", (s32)I, (s32)III, I,
        "   1 | \tab\n     | \t^~\n");
    /* UTF-8: e acutum duo octeti, character unus */
    _probare(piscina, "\xC3\xA9 = x\n", (s32)III, (s32)IV, I,
        "   1 | \xC3\xA9 = x\n     |   ^\n");
    /* TRES octeti NECESSARII sunt: casus supra plantam in larva
     * continuationis (0x80 -> 0xC0) NON videt, quia in sequentia
     * BINA numerus octetorum 'characterem incipientium' idem manet
     * (I recte, I planta); in sequentia TERNA differt (I contra II).
     * Planta viridis mensura PROBATIONIS est, non codicis. */
    _probare(piscina, "\xE2\x82\xAC = x\n", (s32)IV, (s32)V, I,
        "   1 | \xE2\x82\xAC = x\n     |   ^\n");
    /* tilde per CHARACTEREM, non per octetum: 'a', euro, 'b'.
     * Litterae SEIUNCTAE: '\xAC' 'b' effugium hexadecimale AVIDUM
     * est ('\xACb' extra limites) - iunctura litterarum eum claudit */
    _probare(piscina, "a\xE2\x82\xAC" "b\n", ZEPHYRUM, (s32)V, I,
        "   1 | a\xE2\x82\xAC" "b\n     | ^~~\n");
    /* linea sine nova-linea ad finem */
    _probare(piscina, "abc", (s32)III, (s32)III, I,
        "   1 | abc\n     |    ^\n");
    /* tractus ultra finem lineae: ad lineam solam truncatur */
    _probare(piscina, "abc\nd", (s32)I, (s32)V, I,
        "   1 | abc\n     |  ^~\n");
    /* CR ante nova-linea sublatum */
    _probare(piscina, "ab\r\n", ZEPHYRUM, (s32)II, I,
        "   1 | ab\n     | ^~\n");
    /* margo crescit cum numero lineae */
    _probare(piscina, "q\n", ZEPHYRUM, (s32)I, (i32)12345,
        "12345 | q\n      | ^\n");

    /* initium extra fontem RECUSATUR */
    aedificator = chorda_aedificator_creare(piscina,
        (memoriae_index)XXXII);
    CREDO_NON_NIHIL (aedificator);
    CREDO_FALSUM (excerptum_scribere(aedificator, "ab", (i32)II,
        (s32)III, (s32)III, I));
    /* linea ZEPHYRUM RECUSATUR */
    CREDO_FALSUM (excerptum_scribere(aedificator, "ab", (i32)II,
        ZEPHYRUM, (s32)I, ZEPHYRUM));

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
