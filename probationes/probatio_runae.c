/* probatio_runae.c - runae: latitudo (U2)
 *
 * Casus MANU ex regula Ghostty derivati (non ex tabula - vide
 * lib/runae.phase-log.md U2): regimina, signa componentia Mn/Me/Mc,
 * Default_Ignorable, iamo Hangul, East_Asian_Width W/F/A/H, Regional
 * Indicator, modificatores emoji, Prepend, surrogata, non-assignata,
 * runae invalidae. Casus mensurati tabularii (…VRTHANR): '広' = II,
 * U+0301 = ZEPHYRUM. Deinde totalitas per omnem codicem et corpus
 * Lapidis: linguae latae (ja/zh/ko) runas II habent, linguae signorum
 * (ar/hi/th/yo) runas ZEPHYRUM, en/la neutras.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "utf8.h"
#include "runae.h"
#include "credo.h"
#include <stdio.h>

nomen structura {
                   s32  runa;
                   i32  latitudo;
    constans character* causa;
} CasusLatitudinis;

hic_manens constans CasusLatitudinis CASUS[] = {
    { 0x0041, I, "A" },
    { 0x0020, I, "spatium" },
    { 0x0000, ZEPHYRUM, "NUL (Cc)" },
    { 0x001B, ZEPHYRUM, "ESC (Cc)" },
    { 0x007F, ZEPHYRUM, "DEL (Cc)" },
    { 0x0085, ZEPHYRUM, "NEL (Cc, C1)" },
    { 0x00A0, I, "NBSP (Zs)" },
    { 0x00AD, I, "hyphen mollis (DI, exceptio)" },
    { 0x00B1, I, "plus-minus (EAW A = I)" },
    { 0x00E9, I, "e acutum praecompositum" },
    { 0x0301, ZEPHYRUM, "acutum componens (Mn) - tabularium" },
    { 0x0378, I, "non assignatum graecum (N)" },
    { 0x0600, I, "signum numeri arabicum (Cf, Prepend)" },
    { 0x064E, ZEPHYRUM, "fatha arabica (Mn)" },
    { 0x093F, I, "matra i devanagari (Mc)" },
    { 0x094D, ZEPHYRUM, "virama devanagari (Mn)" },
    { 0x0E31, ZEPHYRUM, "mai han-akat thai (Mn)" },
    { 0x1100, II, "choseong kiyeok (W)" },
    { 0x115F, ZEPHYRUM, "choseong filler (DI ante W)" },
    { 0x1160, ZEPHYRUM, "jungseong filler (DI)" },
    { 0x1161, ZEPHYRUM, "jungseong a (GCB V)" },
    { 0x11A8, ZEPHYRUM, "jongseong kiyeok (GCB T)" },
    { 0x200B, ZEPHYRUM, "ZWSP (DI)" },
    { 0x200C, ZEPHYRUM, "ZWNJ (DI)" },
    { 0x200D, ZEPHYRUM, "ZWJ (DI)" },
    { 0x2028, ZEPHYRUM, "separator lineae (Zl)" },
    { 0x2029, ZEPHYRUM, "separator paragraphi (Zp)" },
    { 0x2060, ZEPHYRUM, "word joiner (DI)" },
    { 0x20DD, ZEPHYRUM, "circulus includens (Me)" },
    { 0x20E3, ZEPHYRUM, "keycap includens (Me)" },
    { 0x231A, II, "horologium (W, emoji)" },
    { 0x2764, I, "cor (N, textus)" },
    { 0x2E3A, II, "lineola duarum em" },
    { 0x2E3B, II, "lineola trium em (III -> II)" },
    { 0x3000, II, "spatium ideographicum (F)" },
    { 0x3042, II, "hiragana a (W)" },
    { 0x4E2D, II, "zhong (W)" },
    { 0x5E83, II, "hiro - tabularium" },
    { 0xAC00, II, "syllaba hangul ga (W)" },
    { 0xD800, ZEPHYRUM, "surrogatum (Cs)" },
    { 0xDFFF, ZEPHYRUM, "surrogatum (Cs)" },
    { 0xE000, I, "usus privatus (A = I)" },
    { 0xFE0F, ZEPHYRUM, "VS16 (DI)" },
    { 0xFEFF, ZEPHYRUM, "BOM (DI)" },
    { 0xFF21, II, "A plenae latitudinis (F)" },
    { 0xFF61, I, "punctum dimidiae latitudinis (H)" },
    { 0x1F1E6, II, "regional indicator A" },
    { 0x1F3FB, II, "modificator emoji (W, servatur)" },
    { 0x1F600, II, "facies ridens (W)" },
    { 0x2FFFD, II, "non assignatum planum II (W)" },
    { 0x3FFFD, II, "non assignatum planum III (W)" },
    { 0xE0001, ZEPHYRUM, "tag lingua (DI)" },
    { 0xE0100, ZEPHYRUM, "VS17 (DI)" },
    { 0x10FFFF, I, "non-character ultimum (N)" },
    { -I, I, "invalida: -1" },
    { 0x110000, I, "invalida: ultra U+10FFFF" },
    { 0x7FFFFFFF, I, "invalida: maxima" }
};

/* Linguae corporis et exspectationes */
nomen structura {
    constans character* lingua;
                   i32  latae_minimae;     /* runae latitudinis II */
                   i32  nullae_minimae;    /* runae latitudinis 0 */
                   b32  neutra;            /* solae latitudinis I */
} LinguaLatitudinis;

hic_manens constans LinguaLatitudinis LINGUAE[] = {
    { "ja", C, ZEPHYRUM, FALSUM },
    { "zh", C, ZEPHYRUM, FALSUM },
    { "ko", C, ZEPHYRUM, FALSUM },
    { "ar", ZEPHYRUM, XX, FALSUM },
    { "hi", ZEPHYRUM, XX, FALSUM },
    { "th", ZEPHYRUM, XX, FALSUM },
    { "yo", ZEPHYRUM, XX, FALSUM },
    { "en", ZEPHYRUM, ZEPHYRUM, VERUM },
    { "la", ZEPHYRUM, ZEPHYRUM, VERUM }
};

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
         i32  j;

    piscina = piscina_generare_dynamicum("probatio_runae", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Casus latitudinis (Unicode %s) ---\n",
        RUNAE_VERSIO);
    per (j = ZEPHYRUM; j < (i32)(magnitudo(CASUS)
        / magnitudo(CASUS[0]));
         j++)
    {
        i32 l = runae_latitudo(CASUS[j].runa);

        si (l != CASUS[j].latitudo)
        {
            imprimere("  FRACTA: U+%04X %s: %u, exspectata %u\n",
                (insignatus integer)CASUS[j].runa, CASUS[j].causa,
                (insignatus integer)l,
                (insignatus integer)CASUS[j].latitudo);
        }
        CREDO_AEQUALIS_I32 (l, CASUS[j].latitudo);
    }

    /* Totalitas: omnis codex 0..U+10FFFF latitudinem 0-II habet */
    {
        s32 runa;
        i32 numeri[IV]  = { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM };
        b32 totalis     = VERUM;

        imprimere("\n--- Totalitas ---\n");
        per (runa = ZEPHYRUM; runa <= 0x10FFFF; runa++)
        {
            i32 l = runae_latitudo(runa);

            si (l > II)
            {
                totalis = FALSUM;
                numeri[III]++;
            }
            alioquin
            {
                numeri[l]++;
            }
        }
        imprimere("  latitudo 0: %u, I: %u, II: %u, extra: %u\n",
            (insignatus integer)numeri[ZEPHYRUM],
            (insignatus integer)numeri[I],
            (insignatus integer)numeri[II],
            (insignatus integer)numeri[III]);
        CREDO_VERUM (totalis);
    }

    /* Corpus Lapidis */
    imprimere("\n--- Corpus Lapidis ---\n");
    per (j = ZEPHYRUM; j < (i32)(magnitudo(LINGUAE)
        / magnitudo(LINGUAE[0]));
         j++)
    {
        constans LinguaLatitudinis* l = &LINGUAE[j];
                         character  via[CXXVIII];
                            chorda  textus;
                       constans i8* cursor;
                       constans i8* finis;
                               i32  numeri[III] = { ZEPHYRUM, ZEPHYRUM,
                                   ZEPHYRUM };
                               i32 columnae = ZEPHYRUM;

        sprintf(via, "probationes/fixa/runae/corpus/%s.txt", l->lingua);
        textus = filum_legere_totum(via, piscina);
        CREDO_VERUM (textus.mensura > ZEPHYRUM);
        cursor  = textus.datum;
        finis   = textus.datum + textus.mensura;
        dum (cursor < finis)
        {
            s32 runa = utf8_decodere(&cursor, finis);
            i32 lat;

            si (runa < ZEPHYRUM || runa == 0x0A)
            {
                perge;
            }
            lat = runae_latitudo(runa);
            si (lat <= II)
            {
                numeri[lat]++;
            }
            columnae += lat;
        }
        imprimere("  %-3s runae latitudinis 0: %4u, I: %4u, II: %4u; "
            "columnae %u\n", l->lingua,
            (insignatus integer)numeri[ZEPHYRUM],
            (insignatus integer)numeri[I],
            (insignatus integer)numeri[II],
            (insignatus integer)columnae);
        CREDO_VERUM (numeri[II] >= l->latae_minimae);
        CREDO_VERUM (numeri[ZEPHYRUM] >= l->nullae_minimae);
        si (l->neutra)
        {
            CREDO_AEQUALIS_I32 (numeri[ZEPHYRUM] + numeri[II],
                ZEPHYRUM);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
