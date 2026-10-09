/* probatio_vicus_horologium.c - horologium in linea tabularum (S4)
 *
 * Genus fictum 'quietus' (sine pulsu); horologium fictum (hora,
 * minutum ex probatione). I: sine horologio nullus textus horae. II:
 * 19:52 -> "7:52 PM" ad dextrum (cellula marginis). III: 00:05 ->
 * "12:05 AM", 12:00 -> "12:00 PM". IV: vicus_pulsare VERUM solum cum
 * minutum mutatur. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "figura.h"
#include "mandatum.h"
#include "dispensator.h"
#include "vicus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* horologium fictum */
nomen structura {
    s32 hora;
    s32 minutum;
} HoraFicta;

interior vacuum
horologium_fictum (
    vacuum* ctx,
       s32* hora,
       s32* minutum)
{
    *hora     = ((HoraFicta*)ctx)->hora;
    *minutum  = ((HoraFicta*)ctx)->minutum;
}

interior b32
quietum_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* vol,
     InsulaRepositorium* r,
     constans character* id,
     constans character* argumentum,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)sedes;
    (vacuum)p;
    (vacuum)in;
    (vacuum)vol;
    (vacuum)r;
    (vacuum)id;
    (vacuum)argumentum;
    (vacuum)radix;
    (vacuum)latitudo;
    (vacuum)altitudo;
    (vacuum)ctx;
    redde VERUM;
}

interior vacuum
quietum_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    (vacuum)montatio;
    (vacuum)f;
}

/* textus in linea tabularum (y < VICUS_ALTITUDO_TABULARUM) aequalis
 * 'quaesitus': x eius, aut -1 */
interior s32
textus_x (
            Dispensator* d,
                  Vicus* v,
     constans character* quaesitus)
{
     Mandata* md;
    Mandatum* x;
         i32  i;

    dispensator_recomponere(d);
    md = mandata_creare(piscina, intern);
    pingere(dispensator_arbor(d), vicus_figurae(v), ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        x = mandata_obtinere(md, i);
        si (   x->genus == MANDATUM_TEXTUS
            && x->fines.y < VICUS_ALTITUDO_TABULARUM
            && chorda_aequalis_literis(x->textus, quaesitus))
        {
            redde x->fines.x;
        }
    }
    redde -I;
}

/* textus horae ullus (':' et "M" finale) in linea tabularum */
interior i32
horae_textus (
    Dispensator* d,
          Vicus* v)
{
     Mandata* md;
    Mandatum* x;
         i32  i;
         i32  n;

    dispensator_recomponere(d);
    md = mandata_creare(piscina, intern);
    pingere(dispensator_arbor(d), vicus_figurae(v), ZEPHYRUM, md);
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        x = mandata_obtinere(md, i);
        si (   x->genus == MANDATUM_TEXTUS && x->textus.mensura > II
            && chorda_continet(x->textus, chorda_ex_literis(":",
                   piscina))
            && x->textus.datum[x->textus.mensura - I] == 'M')
        {
            n++;
        }
    }
    redde n;
}

s32 principale (vacuum)
{
         Volumen* vol;
           Vicus* v;
     Dispensator* d;
       HoraFicta  ficta;

    piscina = piscina_generare_dynamicum("probatio_vicus_horologium",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol  = volumen_temporarium(piscina, "probatio_vicus_horologium");
    v    = vicus_creare(piscina, intern, vol, NIHIL, CDLXXX, CDLXXX);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_VERUM(vicus_genus_addere(v, "quietus", I, quietum_montare,
        quietum_describere, NIHIL));
    CREDO_VERUM(vicus_aperire(v,
        "<tabulae activa=\"A\"><tabula id=\"A\">"
        "<latus genus=\"quietus\"/><acervus>"
        "<latus genus=\"quietus\"/></acervus></tabula></tabulae>"));
    d = dispensator_creare(piscina, intern, v->repo, vicus_actiones(v),
        vicus_componere, v, CCC);
    CREDO_NECESSE_NON_NIHIL(d);

    imprimere("\n--- I: sine horologio ---\n");
    CREDO_AEQUALIS_I32(horae_textus(d, v), ZEPHYRUM);

    imprimere("\n--- II: 19:52 -> 7:52 PM ad dextrum ---\n");
    ficta.hora     = XIX;
    ficta.minutum  = LII;
    vicus_horologium_ponere(v, horologium_fictum, &ficta);
    /* CDLXXX - VI (cellula) - VII x VI */
    CREDO_AEQUALIS_S32(textus_x(d, v, "7:52 PM"), CDXXXII);
    CREDO_AEQUALIS_I32(horae_textus(d, v), I);

    imprimere("\n--- III: media nox et meridies ---\n");
    ficta.hora     = ZEPHYRUM;
    ficta.minutum  = V;
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_VERUM(textus_x(d, v, "12:05 AM") > ZEPHYRUM);
    ficta.hora     = XII;
    ficta.minutum  = ZEPHYRUM;
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_VERUM(textus_x(d, v, "12:00 PM") > ZEPHYRUM);

    imprimere("\n--- IV: pulsus solum cum minutum mutatur ---\n");
    CREDO_FALSUM(vicus_pulsare(v));
    ficta.minutum = I;
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_FALSUM(vicus_pulsare(v));
    CREDO_VERUM(textus_x(d, v, "12:01 PM") > ZEPHYRUM);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
