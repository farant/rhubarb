/* probatio_mandatum.c - Probationes mandatorum (sex primitiva) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "mandatum.h"
#include "credo.h"
#include <stdio.h>

interior ColorMandati
color_thema (
    i32 valor)
{
    ColorMandati c;
    c.genus = COLOR_MANDATI_THEMA;
    c.valor = valor;
    redde c;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Mandata* m;
                Mandata* m2;
                  Fines  f;
                Punctum  puncta[III];
                    i32  coetus;
                 chorda  textus;
                 chorda  titulus;
               Mandatum* md;

    piscina = piscina_generare_dynamicum("probatio_mandatum", LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans emissionem et spatia coetuum ---\n");
    m = mandata_creare(piscina, intern);
    CREDO_NON_NIHIL (m);
    CREDO_AEQUALIS_I32 (mandata_numerus(m), ZEPHYRUM);

    f.x = X; f.y = XX; f.latitudo = C; f.altitudo = L;
    titulus = chorda_ex_literis("bottone_servare", piscina);
    coetus  = mandata_coetus_incipere(m, f, VERUM, ZEPHYRUM, ZEPHYRUM,
        I,
                                      titulus);
    CREDO_AEQUALIS_I32 (coetus, ZEPHYRUM);
    mandata_rectangulum(m, f, color_thema(II), VERUM);
    puncta[0].x = ZEPHYRUM; puncta[0].y = ZEPHYRUM;
    puncta[1].x = X;        puncta[1].y = ZEPHYRUM;
    puncta[2].x = V;        puncta[2].y = V;
    mandata_polygonum(m, puncta, III, color_thema(III), FALSUM);
    mandata_textus(m, XII, XXII, chorda_ex_literis("Servare", piscina),
        ZEPHYRUM, color_thema(I));
    mandata_coetus_finire(m, coetus);
    mandata_linea(m, puncta[0], puncta[1], I, color_thema(I));

    CREDO_AEQUALIS_I32 (mandata_numerus(m), V);
    md = mandata_obtinere(m, ZEPHYRUM);
    CREDO_VERUM (md->genus == MANDATUM_COETUS);
    /* coetus + III liberi */
    CREDO_AEQUALIS_I32 (md->magnitudo_arboris, IV);
    md = mandata_obtinere(m, IV);
    CREDO_VERUM (md->genus == MANDATUM_LINEA);
    CREDO_AEQUALIS_I32 (md->magnitudo_arboris, I);

    imprimere("\n--- Probans fines_continet ---\n");
    puncta[0].x = XV; puncta[0].y = XXV;
    CREDO_VERUM  (fines_continet(f, puncta[0]));
    puncta[0].x = V;
    CREDO_FALSUM (fines_continet(f, puncta[0]));

    imprimere("\n--- Probans scribere/legere STML (circuitus) ---\n");
    textus = mandata_scribere_stml(m, piscina, VERUM);
    CREDO_CHORDA_NON_VACUA (textus);
    m2 = mandata_legere_stml(chorda_ut_cstr(textus, piscina), piscina,
        intern);
    CREDO_NON_NIHIL (m2);
    CREDO_AEQUALIS_I32 (mandata_numerus(m2), V);
    CREDO_VERUM (mandata_aequalia(m, m2));

    imprimere("\n--- Probans inaequalitatem ---\n");
    md           = mandata_obtinere(m2, I);
    md->fines.x  = md->fines.x + I;
    CREDO_FALSUM (mandata_aequalia(m, m2));

    imprimere("\n--- Probans vacare ---\n");
    mandata_vacare(m);
    CREDO_AEQUALIS_I32 (mandata_numerus(m), ZEPHYRUM);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
