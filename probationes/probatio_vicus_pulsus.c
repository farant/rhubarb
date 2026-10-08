/* probatio_vicus_pulsus.c - montationes vivae in vico
 * (vicus-latera S1c)
 *
 * Genera ficta (probatione registrata): 'vivus' in fundo vivit,
 * 'gelidus' pulsatur solum activus, 'quietus' pulsum non habet.
 * vicus_pulsare: activa semper, ceterae solum si vivunt in fundo;
 * mutatum fundi quadrum non petit; finitus -> finita, titulus
 * "[exitus]" in linea (et zona ictus eadem latitudine), non iam
 * pulsatur, index durabilis intactus. Deinde genus VERUM: compositio
 * communis (vicus_applicatio) cum terminali t1 - concha (/bin/sh) in
 * fundo legitur, exitus eius tabulam finit. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "color.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "figura.h"
#include "mandatum.h"
#include "tabula_pixelorum.h"
#include "delineare_mandata.h"
#include "aemulator.h"
#include "aemulator_hospes.h"
#include "terminale.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* montatio ficta: pulsus numerati, responsum proximum */
nomen structura {
    i32 pulsus;
    b32 mutatum;
    b32 finitus;
} Vivens;

interior b32
vivum_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* vol,
     InsulaRepositorium* r,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    (vacuum)sedes;
    (vacuum)p;
    (vacuum)in;
    (vacuum)vol;
    (vacuum)r;
    (vacuum)id;
    (vacuum)radix;
    (vacuum)latitudo;
    (vacuum)altitudo;
    redde VERUM;
}

interior VicusPulsus
vivum_pulsare (
    vacuum* ctx)
{
         Vivens* m;
    VicusPulsus  p;

    m          = (Vivens*)ctx;
    m->pulsus++;
    p.mutatum = m->mutatum;
    p.finitus = m->finitus;
    redde p;
}

interior vacuum
vivum_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    f->pulsare         = vivum_pulsare;
    f->pulsare_ctx     = montatio;
    f->vivit_in_fundo  = VERUM;
}

interior vacuum
gelidum_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    f->pulsare      = vivum_pulsare;
    f->pulsare_ctx  = montatio;
}

interior vacuum
quietum_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    (vacuum)montatio;
    (vacuum)f;
}

interior Vicus*
vicum_aperire (
    Volumen* vol)
{
    Vicus* v;

    v = vicus_creare(piscina, intern, vol, NIHIL, CDLXXX, CDLXXX);
    CREDO_NON_NIHIL(v);
    CREDO_VERUM(vicus_genus_addere(v, "vivus", magnitudo(Vivens),
        vivum_montare, vivum_describere));
    CREDO_VERUM(vicus_genus_addere(v, "gelidus", magnitudo(Vivens),
        vivum_montare, gelidum_describere));
    CREDO_VERUM(vicus_genus_addere(v, "quietus", magnitudo(Vivens),
        vivum_montare, quietum_describere));
    /* v2 ULTIMA: nihil post eam pingitur (V) */
    CREDO_VERUM(vicus_aperire(v,
        "<tabulae activa=\"v1\">"
        "<tabula id=\"v1\" genus=\"vivus\" titulus=\"v1\"/>"
        "<tabula id=\"g1\" genus=\"gelidus\" titulus=\"g1\"/>"
        "<tabula id=\"q1\" genus=\"quietus\" titulus=\"q1\"/>"
        "<tabula id=\"v2\" genus=\"vivus\" titulus=\"v2\"/>"
        "</tabulae>"));
    redde v;
}

interior Vivens*
vivens (
    Vicus* v,
      i32  index)
{
    redde (Vivens*)vicus_tabula(v, index)->montatio;
}

interior i32
color (
    ColorThema c)
{
    redde color_ad_pixelum(thema_color(c));
}

interior i32
numerare (
    TabulaPixelorum* t,
                s32  x,
                s32  y,
                s32  latitudo,
                s32  altitudo,
                i32  pix)
{
    i32 n;
    s32 i;
    s32 j;

    n = ZEPHYRUM;
    per (j = y; j < y + altitudo; j++)
    {
        per (i = x; i < x + latitudo; i++)
        {
            si (tabula_pixelorum_obtinere_pixelum(t, (i32)i, (i32)j)
                == pix)
            {
                n++;
            }
        }
    }
    redde n;
}

/* linea tabularum picta (arbor vici, nulla applicatio pingenda) */
interior TabulaPixelorum*
lineam_pingere (
    Vicus* v)
{
            Mandata* md;
    TabulaPixelorum* t;

    md = mandata_creare(piscina, intern);
    pingere(vicus_componere(v->repo, NIHIL, piscina, intern, v),
        vicus_figurae(v), ZEPHYRUM, md);
    t = tabula_pixelorum_creare_nuda(piscina, CDLXXX, VIII);
    tabula_pixelorum_vacare(t, color(COLOR_SUPERFICIES));
    delineare_mandata(md, t, vicus_imago_fons, v);
    redde t;
}

interior Componens*
zona_tabulae (
                  Vicus* v,
     constans character* id)
{
    redde componens_invenire_per_id(vicus_componere(v->repo, NIHIL,
        piscina, intern, v), chorda_ex_literis(id, piscina));
}

interior vacuum
dormire_ms (
    s32 ms)
{
    structura timespec ts;

    ts.tv_sec   = ZEPHYRUM;
    ts.tv_nsec  = (longus)ms * M * M;
    (vacuum)nanosleep(&ts, NIHIL);
}

/* textus in aemulatore terminalis t1 (pulsibus vici, non suis) */
interior b32
exspectare_textum (
                  Vicus* v,
    TerminaleApplicatio* ta,
     constans character* textus)
{
    i32 k;

    per (k = ZEPHYRUM; k < CD; k++)
    {
        (vacuum)vicus_pulsare(v);
        si (chorda_continet(aemulator_textum_effundere(
            aemulator_hospes_aemulator(ta->hospes), piscina),
            chorda_ex_literis(textus, piscina)))
        {
            redde VERUM;
        }
        dormire_ms(X);
    }
    redde FALSUM;
}

interior vacuum
genus_verum_probare (vacuum)
{
            Volumen* vol;
    VicusApplicatio  app;
        VicusTabula* t;
TerminaleApplicatio* ta;
                i32  k;
                b32  pingendum;

    imprimere("\n--- VII: terminale verum (index ordinarius) ---\n");
    (vacuum)setenv("SHELL", "/bin/sh", I);
    vol = volumen_temporarium(piscina, "probatio_vicus_pulsus_verum");
    CREDO_VERUM(vicus_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(app.vicus), III);
    t = vicus_tabula(app.vicus, II);
    CREDO_NON_NIHIL(t);
    si (!t)
    {
        volumen_claudere(vol);
        redde;
    }
    CREDO_CHORDA_AEQUALIS_LITERIS(t->id, "t1");
    CREDO_CHORDA_AEQUALIS_LITERIS(t->genus, "terminale");
    CREDO_VERUM(t->montata);
    CREDO_VERUM(t->facies.pulsare != NIHIL);
    CREDO_VERUM(t->facies.vivit_in_fundo);
    CREDO_VERUM(t->facies.componere == terminale_componere);
    ta = (TerminaleApplicatio*)t->montatio;
    /* activa s1: terminale in FUNDO legitur */
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_activa(app.vicus)->id, "s1");
    (vacuum)aemulator_hospes_scribere(ta->hospes,
        (constans i8*)"echo vivit_in_fundo\r", XX);
    CREDO_VERUM(exspectare_textum(app.vicus, ta, "vivit_in_fundo\n"));

    imprimere("\n--- VIII: concha exit - tabula finita ---\n");
    (vacuum)aemulator_hospes_scribere(ta->hospes,
        (constans i8*)"exit\r", V);
    pingendum = FALSUM;
    per (k = ZEPHYRUM; k < CD && !t->finita; k++)
    {
        pingendum = vicus_pulsare(app.vicus);
        dormire_ms(X);
    }
    CREDO_VERUM(t->finita);
    /* fundo finita: linea mutata, quadrum pingendum */
    CREDO_VERUM(pingendum);
    CREDO_VERUM(t->montata);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(app.vicus), III);
    terminale_claudere(ta);
    volumen_claudere(vol);
}

s32 principale (vacuum)
{
         Volumen* vol;
           Vicus* v;
       Componens* c;
 TabulaPixelorum* t;
          chorda  index;
             b32  inventum;
             s32  x;
             s32  lat;

    piscina = piscina_generare_dynamicum("probatio_vicus_pulsus",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_vicus_pulsus");

    imprimere("\n--- I: activa et viventes in fundo pulsantur ---\n");
    v = vicum_aperire(vol);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), IV);
    CREDO_FALSUM(vicus_pulsare(v));
    CREDO_AEQUALIS_I32(vivens(v, ZEPHYRUM)->pulsus, I);
    CREDO_AEQUALIS_I32(vivens(v, I)->pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(vivens(v, III)->pulsus, I);

    imprimere("\n--- II: mutatum activae pingit, fundi non ---\n");
    vivens(v, III)->mutatum = VERUM;
    CREDO_FALSUM(vicus_pulsare(v));
    vivens(v, ZEPHYRUM)->mutatum = VERUM;
    CREDO_VERUM(vicus_pulsare(v));
    vivens(v, ZEPHYRUM)->mutatum  = FALSUM;
    vivens(v, III)->mutatum       = FALSUM;

    imprimere("\n--- III: gelidus solum activus ---\n");
    CREDO_VERUM(vicus_activam_ponere(v, "g1"));
    CREDO_FALSUM(vicus_pulsare(v));
    CREDO_AEQUALIS_I32(vivens(v, I)->pulsus, I);
    CREDO_AEQUALIS_I32(vivens(v, ZEPHYRUM)->pulsus, IV);
    /* quietus activus: nullus pulsus, nihil frangitur */
    CREDO_VERUM(vicus_activam_ponere(v, "q1"));
    CREDO_FALSUM(vicus_pulsare(v));
    CREDO_AEQUALIS_I32(vivens(v, I)->pulsus, I);
    CREDO_AEQUALIS_I32(vivens(v, ZEPHYRUM)->pulsus, V);

    imprimere("\n--- IV: finitus in fundo - non iam pulsata ---\n");
    /* ante: post v2 nihil pingitur */
    c = zona_tabulae(v, "vicus.tabula.v2");
    CREDO_NON_NIHIL(c);
    x    = c ? c->fines.x : ZEPHYRUM;
    lat  = c ? c->fines.latitudo : ZEPHYRUM;
    CREDO_AEQUALIS_S32(lat, XXIV);
    t = lineam_pingere(v);
    CREDO_AEQUALIS_I32(numerare(t, x + XVIII, ZEPHYRUM, LIV, VIII,
        color(COLOR_TEXT)), ZEPHYRUM);
    vivens(v, III)->finitus = VERUM;
    CREDO_FALSUM(vicus_tabula(v, III)->finita);
    /* finita nunc: titulus mutatur, quadrum pingendum */
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_VERUM(vicus_tabula(v, III)->finita);
    CREDO_AEQUALIS_I32(vivens(v, III)->pulsus, VI);
    CREDO_FALSUM(vicus_pulsare(v));
    CREDO_AEQUALIS_I32(vivens(v, III)->pulsus, VI);
    /* activa facta: non pulsatur, sed montata manet */
    CREDO_VERUM(vicus_activam_ponere(v, "v2"));
    CREDO_FALSUM(vicus_pulsare(v));
    CREDO_AEQUALIS_I32(vivens(v, III)->pulsus, VI);
    CREDO_VERUM(vicus_tabula(v, III)->montata);
    CREDO_VERUM(vicus_activam_ponere(v, "q1"));

    imprimere("\n--- V: titulus \"[exitus]\" et zona ictus ---\n");
    /* " [exitus]" = IX cellulae post titulum */
    c = zona_tabulae(v, "vicus.tabula.v2");
    CREDO_NON_NIHIL(c);
    CREDO_AEQUALIS_S32(c ? c->fines.x : ZEPHYRUM, x);
    CREDO_AEQUALIS_S32(c ? c->fines.latitudo : ZEPHYRUM, lat + LIV);
    t = lineam_pingere(v);
    CREDO_VERUM(numerare(t, x + XVIII, ZEPHYRUM, LIV, VIII,
        color(COLOR_TEXT)) > ZEPHYRUM);
    /* titulus ipse non mutatus */
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_tabula(v, III)->titulus, "v2");

    imprimere("\n--- VI: index durabilis intactus ---\n");
    index = volumen_plagulam_promere(vol, chorda_ex_literis(
        "vicus/tabulae", piscina), piscina, &inventum);
    CREDO_VERUM(inventum);
    CREDO_FALSUM(chorda_continet(index, chorda_ex_literis("exitus",
        piscina)));
    v = vicum_aperire(vol);
    CREDO_FALSUM(vicus_tabula(v, III)->finita);
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_tabula(v, III)->titulus, "v2");
    volumen_claudere(vol);

    genus_verum_probare();

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
