/* probatio_mandatum.c - Probationes mandatorum (sex primitiva) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "mandatum.h"
#include "chorda.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior ColorMandati
color_thema (
    i32 valor)
{
    ColorMandati c;
    c.genus = COLOR_MANDATI_THEMA;
    c.valor = valor;
    redde c;
}

/* Fixura suitae: m (coetus + III liberi, linea) et f semel aedificata
 * ut in sectione prima; sectiones ea LEGUNT, numquam mutant (quae
 * mutant copiam suam ex c->piscina faciunt). */
nomen structura {
     Mandata* m;
       Fines  f;
} Mundus;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                 Mundus* o;
                Punctum  puncta[III];
                    i32  coetus;
                 chorda  titulus;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    o        = (Mundus*)piscina_allocare(piscina, magnitudo(Mundus));
    o->m     = mandata_creare(piscina, intern);
    o->f.x   = X; o->f.y = XX; o->f.latitudo = C; o->f.altitudo = L;
    titulus = chorda_ex_literis("bottone_servare", piscina);
    coetus  = mandata_coetus_incipere(o->m, o->f, VERUM, ZEPHYRUM,
        ZEPHYRUM, I, titulus);
    mandata_rectangulum(o->m, o->f, color_thema(II), VERUM);
    puncta[0].x = ZEPHYRUM; puncta[0].y = ZEPHYRUM;
    puncta[1].x = X;        puncta[1].y = ZEPHYRUM;
    puncta[2].x = V;        puncta[2].y = V;
    mandata_polygonum(o->m, puncta, III, color_thema(III), FALSUM);
    mandata_textus(o->m, XII, XXII, chorda_ex_literis("Servare",
        piscina),
        ZEPHYRUM, color_thema(I));
    mandata_coetus_finire(o->m, coetus);
    mandata_linea(o->m, puncta[0], puncta[1], I, color_thema(I));
    c->datum = o;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Mandata* m;
                  Fines  f;
                Punctum  puncta[III];
                    i32  coetus;
                 chorda  titulus;
               Mandatum* md;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

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
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    constans Mundus* o = (constans Mundus*)c->suita;
            Punctum  puncta[III];

    puncta[0].x = XV; puncta[0].y = XXV;
    CREDO_VERUM  (fines_continet(o->f, puncta[0]));
    puncta[0].x = V;
    CREDO_FALSUM (fines_continet(o->f, puncta[0]));
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
         constans Mundus* o = (constans Mundus*)c->suita;
                 Piscina* piscina;
     InternamentumChorda* intern;
                 Mandata* m2;
                  chorda  textus;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    textus = mandata_scribere_stml(o->m, piscina, VERUM);
    CREDO_CHORDA_NON_VACUA (textus);
    m2 = mandata_legere_stml(chorda_ut_cstr(textus, piscina), piscina,
        intern);
    CREDO_NON_NIHIL (m2);
    CREDO_AEQUALIS_I32 (mandata_numerus(m2), V);
    CREDO_VERUM (mandata_aequalia(o->m, m2));
}

/* m2 = lectio scripturae m (ut in sectione tertia), copia propria */
interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
         constans Mundus* o = (constans Mundus*)c->suita;
                 Piscina* piscina;
     InternamentumChorda* intern;
                 Mandata* m2;
                  chorda  textus;
                Mandatum* md;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    textus   = mandata_scribere_stml(o->m, piscina, VERUM);
    m2      = mandata_legere_stml(chorda_ut_cstr(textus, piscina),
        piscina,
        intern);

    md           = mandata_obtinere(m2, I);
    md->fines.x  = md->fines.x + I;
    CREDO_FALSUM (mandata_aequalia(o->m, m2));
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
         constans Mundus* o = (constans Mundus*)c->suita;
                 Piscina* piscina;
     InternamentumChorda* intern;
                Mandatum* md;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        Mandata* me;
        Mandata* me_lecta;
         chorda  ts;

        me = mandata_creare(piscina, intern);
        mandata_rectangulum_exemplar(me, o->f, color_thema(II), XXI);
        mandata_rectangulum(me, o->f, color_thema(II), VERUM);
        md = mandata_obtinere(me, ZEPHYRUM);
        CREDO_VERUM(md->impletum);
        CREDO_AEQUALIS_I32(md->exemplar, XXI);
        CREDO_AEQUALIS_I32(mandata_obtinere(me, I)->exemplar, ZEPHYRUM);
        ts = mandata_scribere_stml(me, piscina, VERUM);
        CREDO_VERUM(chorda_continet(ts, chorda_ex_literis(
            "exemplar=\"21\"", piscina)));
        /* plenum attributum non scribit: unum tantum */
        CREDO_AEQUALIS_I32(chorda_numerare_occurrentia(ts,
            chorda_ex_literis(
            "exemplar=", piscina)), I);
        me_lecta = mandata_legere_stml(chorda_ut_cstr(ts, piscina),
            piscina,
            intern);
        CREDO_VERUM(mandata_aequalia(me, me_lecta));
        mandata_obtinere(me_lecta, ZEPHYRUM)->exemplar = XII;
        CREDO_FALSUM(mandata_aequalia(me, me_lecta));
    }
}

/* m suitae non vacatur: copia per scripturam et lectionem */
interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
         constans Mundus* o = (constans Mundus*)c->suita;
                 Piscina* piscina;
     InternamentumChorda* intern;
                 Mandata* m;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    m = mandata_legere_stml(chorda_ut_cstr(mandata_scribere_stml(o->m,
        piscina, VERUM), piscina), piscina, intern);

    mandata_vacare(m);
    CREDO_AEQUALIS_I32 (mandata_numerus(m), ZEPHYRUM);
}

/* textus et fons imaginis in piscinam mandatorum copiantur: vocans
 * memoriam suam statim reddere potest (buffer acervi, introitus
 * conspectus eiectus - kalendarium 10-13 et 10-23) */
interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
                 Mandata* m;
       constans Mandatum* y;
               character  buffer[XVI];
               character  fons[XVI];
                  chorda  textus;
                  chorda  via;
                   Fines  f;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    m        = mandata_creare(piscina, intern);
    memcpy(buffer, "Servare", VII);
    memcpy(fons, "imago.png", IX);
    textus.datum    = (i8*)buffer;
    textus.mensura  = VII;
    via.datum       = (i8*)fons;
    via.mensura     = IX;
    f.x             = ZEPHYRUM;
    f.y             = ZEPHYRUM;
    f.latitudo      = X;
    f.altitudo      = X;
    mandata_textus(m, I, I, textus, ZEPHYRUM, color_thema(I));
    mandata_imago(m, via, f);
    memset(buffer, 'x', magnitudo(buffer));
    memset(fons, 'x', magnitudo(fons));

    y = mandata_obtinere(m, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(y->textus, "Servare");
    CREDO_VERUM((constans character*)y->textus.datum != buffer);
    y = mandata_obtinere(m, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(y->textus, "imago.png");
    CREDO_VERUM((constans character*)y->textus.datum != fons);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "emissionem et spatia coetuum", _sectio_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "fines_continet", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "scribere/legere STML (circuitus)", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "inaequalitatem", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exemplar (indicium foci)", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vacare", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "possessio (textus copiatur)", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("mandatum", SECTIONES,
        _suitam_parare,
        NIHIL);
}
