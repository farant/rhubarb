/* nomenclator.c - index contactuum (include/nomenclator.h;
 * nomenclator-plan P2a). Forma calendarii: arbor dispositionis in
 * cellulis (LITTERA x LINEA), figura radicis omnia pingit (spatio
 * radicis), praecepta per membrum__praecepta, paginae per
 * membrum__album. */

#include "nomenclator.h"
#include "membrum__album.h"
#include "membrum__praecepta.h"
#include "membrum__prompta.h"
#include "membrum__elenchi.h"
#include "elenchus.h"
#include "rubrica.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"

#include <stdio.h>
#include <string.h>

#define NOMENCLATOR_LATITUDO   CCLXXX   /* superficies absens */
#define NOMENCLATOR_ALTITUDO   CCLVI
#define NOMENCLATOR_LITTERA    VI
#define NOMENCLATOR_LINEA      VIII
#define NOMENCLATOR_TITULI     III      /* lineae tituli */
#define NOMENCLATOR_CAPUT      II       /* lineae capitis indicis */
#define NOMENCLATOR_ORDO       II       /* lineae ordinis */

hic_manens constans character actio_nomenclatoris[] =
    "nomenclator.agere";

/* figura: contextus privatus */
nomen structura {
    NomenclatorMontatio* m;
                Piscina* scheda;
                    s32  latitudo_montationis;
                    s32  altitudo_montationis;
} NomenclatorFigurae;


/* ==================================================
 * Status (insula)
 * ================================================== */

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, titulus);
    si (a && chorda_ut_s32(*a, &v) && v >= ZEPHYRUM)
    {
        redde v;
    }
    redde praestitutum;
}

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

interior vacuum
campum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParCampi* par;

    par = (ParCampi*)ctx;
    si (!par->valor || !par->valor[ZEPHYRUM])
    {
        (vacuum)insula_attributum_tollere(radix, par->titulus);
        redde;
    }
    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

nomen structura {
    ParCampi* pares;
         i32  numerus;
} GrexCamporum;

interior vacuum
campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    GrexCamporum* grex;
             i32  i;

    grex = (GrexCamporum*)ctx;
    per (i = ZEPHYRUM; i < grex->numerus; i++)
    {
        campum_ponere(radix, p, in, (vacuum*)(grex->pares + i));
    }
}

/* duo campi actu uno, scriptore nomenclatoris (prior restituitur) */
interior b32
campos_scribere (
    NomenclatorMontatio* m,
     constans character* titulus_primus,
     constans character* valor_primus,
     constans character* titulus_secundus,
     constans character* valor_secundus)
{
        ParCampi pares[II];
    GrexCamporum grex;
          chorda prior;
             b32 ok;

    pares[ZEPHYRUM].titulus  = titulus_primus;
    pares[ZEPHYRUM].valor    = valor_primus;
    pares[I].titulus         = titulus_secundus;
    pares[I].valor           = valor_secundus;
    grex.pares               = pares;
    grex.numerus             = titulus_secundus ? II : I;
    prior                    = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_nomenclatoris, m->piscina));
    ok = mutare_ramum(&m->ramus, campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

/* superficies: attributum rami (vicus), aliter montationis, aliter
 * ordinaria */
interior s32
superficies (
    constans NomenclatorMontatio* m,
                             b32  latitudo)
{
    constans NomenclatorFigurae* f;
                            s32  v;

    f = (constans NomenclatorFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? f->latitudo_montationis
                                 : f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? NOMENCLATOR_LATITUDO : NOMENCLATOR_ALTITUDO;
    }
    redde attributum_s32(&m->ramus, latitudo ? "superficies_latitudo"
        : "superficies_altitudo", v);
}


/* ==================================================
 * Index (album)
 * ================================================== */

/* capacitas indicis in cellulis: altitudo minus margines (II), titulus,
 * caput, prompta (si aperta) */
interior s32
capacitas (
    constans NomenclatorMontatio* m)
{
    redde superficies(m, FALSUM) / NOMENCLATOR_LINEA - II
        - NOMENCLATOR_TITULI - NOMENCLATOR_CAPUT - (prompta_aperta(
        &m->ramus, NIHIL) ? NOMENCLATOR_ORDO : ZEPHYRUM);
}

/* P3: visus "rubricae"? */
interior b32
visus_rubricarum (
    constans NomenclatorMontatio* m)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, "visus");
    redde a && chorda_aequalis_literis(*a, "rubricae");
}

/* P3: rubrica filtri (NIHIL = nulla) */
interior chorda*
filtrum_currens (
    constans NomenclatorMontatio* m)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, "rubrica_filtrum");
    redde (a && a->mensura > ZEPHYRUM) ? a : NIHIL;
}

/* persona in filtro (nullo filtro: omnes) */
interior b32
in_filtro (
    constans NomenclatorMontatio* m,
                constans Persona* q)
{
    chorda* f;

    f = filtrum_currens(m);
    redde !f || rubricam_habet(q->rubricae, *f);
}

/* P3: greges visus rubricarum - rubricae (filtro: sola eius), deinde
 * "" = (none) (non sub filtro); Xar de chorda */
interior Xar*
greges (
    constans NomenclatorMontatio* m,
                         Piscina* p)
{
       Xar* g;
       Xar* r;
    chorda* f;
    chorda  vacua;
       i32  i;

    g = xar_creare(p, (i32)magnitudo(chorda));
    r = m->documentum ? personae_rubricae(m->documentum, p) : NIHIL;
    f = filtrum_currens(m);
    per (i = ZEPHYRUM; g && r && i < xar_numerus(r); i++)
    {
        si (!f || chorda_aequalis(*f, *(chorda*)xar_obtinere(r, i)))
        {
            *(chorda*)xar_addere(g) = *(chorda*)xar_obtinere(r, i);
        }
    }
    si (g && !f)
    {
        vacua.datum              = NIHIL;
        vacua.mensura            = ZEPHYRUM;
        *(chorda*)xar_addere(g)  = vacua;
    }
    redde g;
}

/* album: visus indicis - tituli litterarum "l<L>" ("l_" = '#'),
 * personae "p<id>" ordine clavis; visus rubricarum (P3) - tituli
 * gregum "g<k>", personae "p<id>-<k>" (in gregibus pluribus pluries);
 * filtrum in utroque */
interior Album*
ordines (
    constans NomenclatorMontatio* m,
                         Piscina* p)
{
                Album* l;
                  Xar* personae;
     constans Persona* q;
            character  littera;
            character  prior;
            character  id[XXIV];
                  i32  i;

    l = album_creare(p, capacitas(m));
    personae = m->documentum ? personae_ordinatae(m->documentum, p)
                             : NIHIL;
    si (l && personae && visus_rubricarum(m))
    {
           Xar* g;
        chorda  rubrica;
           b32  titulus;
           i32  k;

        g = greges(m, p);
        per (k = ZEPHYRUM; g && k < xar_numerus(g); k++)
        {
            rubrica = *(chorda*)xar_obtinere(g, k);
            titulus = FALSUM;
            per (i = ZEPHYRUM; i < xar_numerus(personae); i++)
            {
                q = *(constans Persona**)xar_obtinere(personae, i);
                si (  rubrica.mensura > ZEPHYRUM
                    ? !rubricam_habet(q->rubricae, rubrica)
                    : xar_numerus(q->rubricae) > ZEPHYRUM)
                {
                    perge;
                }
                si (!titulus)
                {
                    sprintf(id, "g%d", (integer)k);
                    (vacuum)album_addere(l, id, ALBUM_TITULUS,
                        NOMENCLATOR_ORDO, ZEPHYRUM);
                    titulus = VERUM;
                }
                sprintf(id, "p%d-%d", (integer)q->id, (integer)k);
                (vacuum)album_addere(l, id, ALBUM_RES, NOMENCLATOR_ORDO,
                    q->id);
            }
        }
        redde l;
    }
    prior = '\0';
    per (i = ZEPHYRUM; l && personae && i < xar_numerus(personae); i++)
    {
        q        = *(constans Persona**)xar_obtinere(personae, i);
        si (!in_filtro(m, q))
        {
            perge;
        }
        littera  = persona_littera(q);
        si (littera != prior)
        {
            sprintf(id, "l%c", littera == '#' ? '_' : littera);
            (vacuum)album_addere(l, id, ALBUM_TITULUS, NOMENCLATOR_ORDO,
                ZEPHYRUM);
            prior = littera;
        }
        sprintf(id, "p%d", (integer)q->id);
        (vacuum)album_addere(l, id, ALBUM_RES, NOMENCLATOR_ORDO, q->id);
    }
    redde l;
}

interior s32
pagina_currens (
    constans NomenclatorMontatio* m,
                           Album* l)
{
    redde album_pagina_coacta(l, attributum_s32(&m->ramus, "pagina",
        ZEPHYRUM));
}

/* id rei ordinis (RES) aut 0 */
interior s32
res_ordinis (
    constans AlbumOrdo* o)
{
    redde (o && o->genus == ALBUM_RES) ? o->res : ZEPHYRUM;
}

s32
nomenclator_persona_electa (
    constans NomenclatorMontatio* m)
{
       Album* l;
     Piscina* p;
         s32  id;

    si (!m || !m->documentum)
    {
        redde ZEPHYRUM;
    }
    id = attributum_s32(&m->ramus, "persona_electa", ZEPHYRUM);
    si (id > ZEPHYRUM && personae_persona(m->documentum, id))
    {
        redde id;
    }
    p = m->figurae_ctx ? ((NomenclatorFigurae*)m->figurae_ctx)->scheda
                       : m->piscina;
    /* P3: prima indicis (filtro et visu) */
    l = ordines(m, p);
    redde l ? album_res_prima(l, ZEPHYRUM) : ZEPHYRUM;
}

/* electio: persona_electa et pagina eius */
interior b32
electionem_scribere (
    NomenclatorMontatio* m,
                    s32  id,
                Piscina* p)
{
     Album* l;
       s32  k;
 character  numerus[XVI];
 character  pagina[XVI];

    l = ordines(m, p);
    k = l ? album_pagina_rei(l, id, pagina_currens(m, l)) : -I;
    sprintf(numerus, "%d", (integer)id);
    sprintf(pagina, "%d", (integer)(k < ZEPHYRUM ? ZEPHYRUM : k));
    redde campos_scribere(m, "persona_electa", numerus, "pagina",
        k > ZEPHYRUM ? pagina : "");
}

/* P3: filtrum ponere ("" tollit); pagina prima, electa prima indicis
 * novi */
interior b32
filtrum_ponere (
    NomenclatorMontatio* m,
     constans character* valor,
                Piscina* p)
{
       Album* l;
         s32  id;
   character  numerus[XVI];

    si (!campos_scribere(m, "rubrica_filtrum", valor, "pagina", ""))
    {
        redde FALSUM;
    }
    l   = ordines(m, p);
    id  = l ? album_res_prima(l, ZEPHYRUM) : ZEPHYRUM;
    sprintf(numerus, "%d", (integer)id);
    redde campos_scribere(m, "persona_electa", id > ZEPHYRUM ? numerus
        : "", NIHIL, NIHIL);
}

/* P3: visum ponere; electa manet, pagina eam sequitur */
interior b32
visum_ponere (
    NomenclatorMontatio* m,
     constans character* visus,
                Piscina* p)
{
    s32 electa;

    electa = nomenclator_persona_electa(m);
    si (!campos_scribere(m, "visus", visus, "pagina", ""))
    {
        redde FALSUM;
    }
    redde electa > ZEPHYRUM ? electionem_scribere(m, electa, p) : VERUM;
}

/* id rei ex ordine "p<id>" aut "p<id>-<k>"; 0 si alius */
interior s32
id_ordinis (
    chorda id)
{
    chorda n;
       s32 v;

    si (id.mensura < II || id.datum[ZEPHYRUM] != 'p')
    {
        redde ZEPHYRUM;
    }
    n.datum    = id.datum + I;
    n.mensura  = ZEPHYRUM;
    dum (   n.mensura < id.mensura - I
         && n.datum[n.mensura] != '-')
    {
        n.mensura++;
    }
    redde chorda_ut_s32(n, &v) ? v : ZEPHYRUM;
}

/* index gregis ex titulo "g<k>"; -1 si alius */
interior s32
gregis_index (
    chorda id)
{
    chorda n;
       s32 v;

    si (id.mensura < II || id.datum[ZEPHYRUM] != 'g')
    {
        redde -I;
    }
    n.datum    = id.datum + I;
    n.mensura  = id.mensura - I;
    redde (chorda_ut_s32(n, &v) && v >= ZEPHYRUM) ? v : -I;
}


/* ==================================================
 * Praecepta
 * ================================================== */

#define VISUS_INDICIS  I   /* P2a: visus unus */

nomen structura {
    NomenclatorMontatio* m;
                Piscina* p;
                  Album* l;
                    s32  electa;
} ContextusPraecepti;

/* gradus +1 / -1 inter res indicis */
interior b32
personam_movere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 i;
    s32 n;
    s32 index;
    s32 id;

    n      = (s32)album_numerus(cp->l);
    index  = -I;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (res_ordinis(album_ordo(cp->l, (i32)i)) == cp->electa)
        {
            index = i;
            frange;
        }
    }
    per (i = index + gradus; i >= ZEPHYRUM && i < n; i += gradus)
    {
        id = res_ordinis(album_ordo(cp->l, (i32)i));
        si (id > ZEPHYRUM)
        {
            redde electionem_scribere(cp->m, id, cp->p);
        }
    }
    redde VERUM;   /* in fine: consumpta, nihil */
}

interior b32
fac_proximam (
    vacuum* ctx)
{
    redde personam_movere((ContextusPraecepti*)ctx, I);
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    redde personam_movere((ContextusPraecepti*)ctx, -I);
}

/* pagina k: res prima electa */
interior b32
paginam_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
          s32 k;
          s32 id;
    character numerus[XVI];
    character pagina[XVI];

    k = pagina_currens(cp->m, cp->l) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    id = album_res_prima(cp->l, k);
    sprintf(pagina, "%d", (integer)k);
    sprintf(numerus, "%d", (integer)id);
    redde campos_scribere(cp->m, "pagina", pagina, "persona_electa",
        id > ZEPHYRUM ? numerus : "");
}

interior b32
fac_paginam_priorem (
    vacuum* ctx)
{
    redde paginam_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_paginam_proximam (
    vacuum* ctx)
{
    redde paginam_vertere((ContextusPraecepti*)ctx, I);
}

#define REQUIRIT_PERSONAM  I    /* persona electa */
#define REQUIRIT_FILTRUM   II   /* filtrum positum */

interior b32
requisita_nomenclatoris (
       i32  requisita,
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde (!(requisita & REQUIRIT_PERSONAM) || cp->electa > ZEPHYRUM)
        && (!(requisita & REQUIRIT_FILTRUM) || filtrum_currens(cp->m));
}

/* P3: filtrum (prompta, filtro currenti praeimpleta), tollere, visus */
interior b32
fac_filtrum (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
                chorda* f;
                chorda  textus;

    f               = filtrum_currens(cp->m);
    textus.datum    = NIHIL;
    textus.mensura  = ZEPHYRUM;
    si (f)
    {
        textus = *f;
    }
    redde prompta_aperire(&cp->m->ramus, actio_nomenclatoris, "filtrum",
        textus, cp->p);
}

interior b32
fac_filtrum_tollere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde filtrum_ponere(cp->m, "", cp->p);
}

interior b32
fac_visum_alternare (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde visum_ponere(cp->m, visus_rubricarum(cp->m) ? "index"
        : "rubricae", cp->p);
}

interior b32
fac_visum_indicis (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde visum_ponere(cp->m, "index", cp->p);
}

interior b32
fac_visum_rubricarum (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde visum_ponere(cp->m, "rubricae", cp->p);
}

interior b32
fac_novam_personam (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde prompta_aperire(&cp->m->ramus, actio_nomenclatoris, "homo",
        chorda_ex_literis("", cp->p), cp->p);
}

interior b32
fac_novam_societatem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde prompta_aperire(&cp->m->ramus, actio_nomenclatoris,
        "societas",
        chorda_ex_literis("", cp->p), cp->p);
}

/* delere: electa sequens in indice (ultima: prior) */
interior b32
fac_delere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
                   s32  n;
                   s32  i;
                   s32  index;
                   s32  successor;
                   s32  id;

    n          = (s32)album_numerus(cp->l);
    index      = -I;
    successor  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (res_ordinis(album_ordo(cp->l, (i32)i)) == cp->electa)
        {
            index = i;
            frange;
        }
    }
    per (i = index + I; index >= ZEPHYRUM && i < n && !successor; i++)
    {
        id = res_ordinis(album_ordo(cp->l, (i32)i));
        si (id > ZEPHYRUM && id != cp->electa)
        {
            successor = id;
        }
    }
    per (i = index - I; index >= ZEPHYRUM && i >= ZEPHYRUM
        && !successor;
         i--)
    {
        id = res_ordinis(album_ordo(cp->l, (i32)i));
        si (id > ZEPHYRUM && id != cp->electa)
        {
            successor = id;
        }
    }
    si (personae_delere(cp->m->documentum, cp->electa) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (successor > ZEPHYRUM)
    {
        redde electionem_scribere(cp->m, successor, cp->p);
    }
    redde campos_scribere(cp->m, "persona_electa", "", "pagina", "");
}

interior b32
fac_revocare (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    (vacuum)personae_revocare(cp->m->documentum);
    redde VERUM;
}

interior b32
fac_reficere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    (vacuum)personae_reficere(cp->m->documentum);
    redde VERUM;
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "nomenclator.proxima", "Next contact", "j", VISUS_INDICIS,
        ZEPHYRUM, fac_proximam },
    { "nomenclator.prior", "Previous contact", "k", VISUS_INDICIS,
        ZEPHYRUM, fac_priorem },
    { "nomenclator.pagina_prior", "Previous page", "[", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_priorem },
    { "nomenclator.pagina_proxima", "Next page", "]", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_proximam },
    { "nomenclator.nova_persona", "New person", "n", VISUS_INDICIS,
        ZEPHYRUM, fac_novam_personam },
    { "nomenclator.nova_societas", "New business", "N", VISUS_INDICIS,
        ZEPHYRUM, fac_novam_societatem },
    { "nomenclator.delere", "Delete contact", "X", VISUS_INDICIS,
        REQUIRIT_PERSONAM, fac_delere },
    { "nomenclator.revocare", "Undo", "u", VISUS_INDICIS, ZEPHYRUM,
        fac_revocare },
    { "nomenclator.reficere", "Redo", "C-r", VISUS_INDICIS, ZEPHYRUM,
        fac_reficere },
    { "nomenclator.filtrum", "Filter by tag", "f", VISUS_INDICIS,
        ZEPHYRUM, fac_filtrum },
    { "nomenclator.filtrum_tollere", "Clear filter", "Esc",
        VISUS_INDICIS, REQUIRIT_FILTRUM, fac_filtrum_tollere },
    { "nomenclator.visus_alternare", "Switch view", "g", VISUS_INDICIS,
        ZEPHYRUM, fac_visum_alternare },
    { "nomenclator.visus_indicis", "All contacts", NIHIL, VISUS_INDICIS,
        ZEPHYRUM, fac_visum_indicis },
    { "nomenclator.visus_rubricarum", "By tag", NIHIL, VISUS_INDICIS,
        ZEPHYRUM, fac_visum_rubricarum },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

/* P2b-2: menu globuli "+ New" */
#define ELENCHUS_NOVUS  I

hic_manens constans ElenchiIntroitus TABULA_ELENCHORUM[] = {
    { ELENCHUS_NOVUS, "nomenclator.nova_persona", FALSUM },
    { ELENCHUS_NOVUS, "nomenclator.nova_societas", FALSUM },
    { ZEPHYRUM, NIHIL, FALSUM }
};

interior vacuum
contextum_parare (
    NomenclatorMontatio* m,
                Piscina* p,
     ContextusPraecepti* cp)
{
    cp->m       = m;
    cp->p       = p;
    cp->l       = ordines(m, p);
    cp->electa  = nomenclator_persona_electa(m);
}

/* hospes menu: tabulae, visus, requisita, contextus paratus */
interior vacuum
hospitem_parare (
    NomenclatorMontatio* m,
                Piscina* p,
     ContextusPraecepti* cp,
          ElenchiHospes* h)
{
    contextum_parare(m, p, cp);
    h->introitus  = TABULA_ELENCHORUM;
    h->praecepta  = TABULA_PRAECEPTORUM;
    h->visus      = VISUS_INDICIS;
    h->requisita  = requisita_nomenclatoris;
    h->ctx        = cp;
}

/* Reditus promptae: homo "verba cognomen" (verbum ultimum cognomen,
 * cetera praenomen; verbum unum praenomen), societas titulus; nova
 * electa, prompta clausa. FALSUM = recusatum (prompta manet). */
interior b32
promptam_committere (
    NomenclatorMontatio* m,
                 chorda  genus,
                 chorda  textus,
                Piscina* p)
{
    chorda praenomen;
    chorda cognomen;
       s32 id;
       i32 i;

    /* P3: filtrum - vacuum tollit, aliter rubrica valida */
    si (chorda_aequalis_literis(genus, "filtrum"))
    {
        si (textus.mensura > ZEPHYRUM && !rubrica_valida(textus))
        {
            redde FALSUM;
        }
        (vacuum)prompta_claudere(&m->ramus, actio_nomenclatoris, p);
        redde filtrum_ponere(m, textus.mensura > ZEPHYRUM
            ? chorda_ut_cstr(textus, p) : "", p);
    }
    si (chorda_aequalis_literis(genus, "societas"))
    {
        id = personae_societatem_creare(m->documentum, textus);
    }
    alioquin
    {
        dum (textus.mensura > ZEPHYRUM && textus.datum[ZEPHYRUM] == ' ')
        {
            textus.datum++;
            textus.mensura--;
        }
        i = textus.mensura;
        dum (i > ZEPHYRUM && textus.datum[i - I] != ' ')
        {
            i--;
        }
        praenomen  = textus;
        cognomen   = textus;
        si (i == ZEPHYRUM)
        {
            cognomen.mensura = ZEPHYRUM;
        }
        alioquin
        {
            cognomen.datum     = textus.datum + i;
            cognomen.mensura   = textus.mensura - i;
            praenomen.mensura  = i;
            dum (   praenomen.mensura > ZEPHYRUM
                 && praenomen.datum[praenomen.mensura - I] == ' ')
            {
                praenomen.mensura--;
            }
        }
        id = personae_hominem_creare(m->documentum, praenomen,
            cognomen);
    }
    si (id <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    (vacuum)prompta_claudere(&m->ramus, actio_nomenclatoris, p);
    redde electionem_scribere(m, id, p);
}

/* <actio/> nomenclator.agere: claves per tabulam praeceptorum; ictus
 * sinister in ordine eligit, in sagitta paginam vertit */
interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
     NomenclatorMontatio* m;
      NomenclatorFigurae* privatum;
     constans Praeceptum* t;
      ContextusPraecepti  cp;
                     s32  id;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m         = (NomenclatorMontatio*)ctx;
    privatum  = (NomenclatorFigurae*)m->figurae_ctx;
    si (!privatum)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    /* P2b-2: menu apertum eventa omnia accipit */
    {
            Elenchus e;
             Punctum ancora;
       ElenchiHospes h;

        hospitem_parare(m, privatum->scheda, &cp, &h);
        si (elenchi_legere(&m->ramus, &h, privatum->scheda, &e,
            &ancora))
        {
            redde elenchi_agere(&m->ramus, actio_nomenclatoris, &h, &e,
                nodus, ev, privatum->scheda);
        }
    }
    /* P2b: prompta aperta claves omnes accipit */
    {
               chorda genus;
               chorda textus;
        PromptaExitus exitus;

        si (prompta_aperta(&m->ramus, &genus))
        {
            exitus = prompta_clavem_agere(&m->ramus,
                actio_nomenclatoris,
                ev, &textus, privatum->scheda);
            si (exitus == PROMPTA_REDITUS)
            {
                (vacuum)promptam_committere(m, genus, textus,
                    privatum->scheda);
                redde VERUM;
            }
            si (exitus != PROMPTA_NIHIL)
            {
                redde VERUM;
            }
        }
    }
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, VISUS_INDICIS, ev);
        si (!t)
        {
            redde FALSUM;
        }
        contextum_parare(m, privatum->scheda, &cp);
        redde praecepta_status(t, VISUS_INDICIS,
            requisita_nomenclatoris,
            &cp) == PRAECEPTUM_PRAESTO && t->fn(&cp);
    }
    si (   ev->genus            != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER || !nodus)
    {
        redde FALSUM;
    }
    id = nodus->partes
        == PARTES_ITEM ? id_ordinis(nodus->id) : ZEPHYRUM;
    si (id > ZEPHYRUM)
    {
        redde electionem_scribere(m, id, privatum->scheda);
    }
    /* P3: titulus gregis filtrat (iterum tollit); "(none)" nihil */
    si (   nodus->partes           == PARTES_INDEX
        && gregis_index(nodus->id) >= ZEPHYRUM)
    {
           Xar* g;
        chorda* f;
        chorda  rubrica;
           s32  k;

        g = greges(m, privatum->scheda);
        k = gregis_index(nodus->id);
        si (!g || k >= (s32)xar_numerus(g))
        {
            redde FALSUM;
        }
        rubrica = *(chorda*)xar_obtinere(g, (i32)k);
        si (rubrica.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
        f = filtrum_currens(m);
        redde filtrum_ponere(m, (f && chorda_aequalis(*f, rubrica)) ? ""
            : chorda_ut_cstr(rubrica, privatum->scheda),
            privatum->scheda);
    }
    /* P3: segmenta visus */
    si (chorda_aequalis_literis(nodus->id, "visus-indicis"))
    {
        redde visum_ponere(m, "index", privatum->scheda);
    }
    si (chorda_aequalis_literis(nodus->id, "visus-rubricarum"))
    {
        redde visum_ponere(m, "rubricae", privatum->scheda);
    }
    /* P2b-2: "+ New" - menu infra globulum (filius radicis: fines in
     * spatio radicis) */
    si (chorda_aequalis_literis(nodus->id, "nova"))
    {
        Punctum ancora;

        ancora.x = nodus->fines.x;
        ancora.y = nodus->fines.y + nodus->fines.altitudo;
        redde elenchi_aperire(&m->ramus, actio_nomenclatoris,
            ELENCHUS_NOVUS, ancora, privatum->scheda);
    }
    si (   chorda_aequalis_literis(nodus->id, "pagina-prior")
        || chorda_aequalis_literis(nodus->id, "pagina-proxima"))
    {
        contextum_parare(m, privatum->scheda, &cp);
        redde praecepta_invocare(TABULA_PRAECEPTORUM,
            chorda_aequalis_literis(nodus->id, "pagina-prior")
            ? "nomenclator.pagina_prior" : "nomenclator.pagina_proxima",
            VISUS_INDICIS, NIHIL, &cp);
    }
    redde FALSUM;
}


/* ==================================================
 * Figura
 * ================================================== */

interior chorda
praecidere (
    chorda textus,
       s32 latitudo)
{
    s32 n;

    n = latitudo / NOMENCLATOR_LITTERA;
    si (n < ZEPHYRUM)
    {
        n = ZEPHYRUM;
    }
    si (textus.mensura > (i32)n)
    {
        textus.mensura = (i32)n;
    }
    redde textus;
}

interior vacuum
textum_pingere (
         Mandata* md,
             s32  x,
             s32  y,
          chorda  textus,
             i32  fons,
      ColorThema  color)
{
    mandata_textus(md, x, y, chorda_transcribere(textus, md->piscina),
        fons, figura_color_thematis(color));
}

/* signum societatis: rhombus parvus (V x V) */
interior vacuum
rhombum_pingere (
    Mandata* md,
        s32  x,
        s32  y)
{
    Punctum p[IV];

    p[ZEPHYRUM].x  = x + II;
    p[ZEPHYRUM].y  = y;
    p[I].x         = x + IV;
    p[I].y         = y + II;
    p[II].x        = x + II;
    p[II].y        = y + IV;
    p[III].x       = x;
    p[III].y       = y + II;
    mandata_polygonum(md, p, IV, figura_color_thematis(COLOR_TEXT_DIM),
        VERUM);
}

/* ordo personae: electa fundo selectionis, ceteri linea infra; nomen,
 * signum societatis, rubricae obscurae ad dextrum */
interior vacuum
ordinem_pingere (
    NomenclatorMontatio* m,
                Mandata* md,
     constans Componens* l,
                    s32  id,
                    s32  electa)
{
     constans Persona* q;
               chorda  appellatio;
               chorda  rubricae;
                  s32  spatium;
                  s32  x;
              Punctum  a;
              Punctum  b;

    q = personae_persona(m->documentum, id);
    si (!q)
    {
        redde;
    }
    si (id == electa)
    {
        figura_ordinem_electum_pingere(md, l->fines);
    }
    alioquin
    {
        a.x = l->fines.x;
        a.y = l->fines.y + l->fines.altitudo - I;
        b.x = l->fines.x + l->fines.latitudo - I;
        b.y = a.y;
        mandata_linea(md, a, b, I, figura_color_thematis(
            COLOR_SUPERFICIES));
    }
    spatium   = l->fines.latitudo - XVI;
    rubricae  = rubricas_iungere(q->rubricae, md->piscina);
    si (rubricae.mensura > ZEPHYRUM)
    {
        rubricae  = praecidere(rubricae, spatium / II);
        spatium   -= (s32)(rubricae.mensura + II) * NOMENCLATOR_LITTERA;
        textum_pingere(md, l->fines.x + l->fines.latitudo - VI
            - (s32)rubricae.mensura * NOMENCLATOR_LITTERA, l->fines.y
            + IV, rubricae, ZEPHYRUM, id == electa ? COLOR_TEXT
            : COLOR_TEXT_DIM);
    }
    appellatio = praecidere(persona_nomen(q, md->piscina), spatium);
    textum_pingere(md, l->fines.x + X, l->fines.y + IV, appellatio,
        ZEPHYRUM,
        COLOR_TEXT);
    si (q->genus == PERSONA_SOCIETAS)
    {
        x = l->fines.x + X + (s32)(appellatio.mensura + I)
            * NOMENCLATOR_LITTERA;
        rhombum_pingere(md, x, l->fines.y + V);
    }
}

/* <figura/> <purus/> radix: mensa, titulus, caput, sagittae, tituli
 * litterarum, ordines - omnia spatio radicis */
interior vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
      NomenclatorFigurae* contextus;
     NomenclatorMontatio* m;
                   Album* l;
               Componens* f;
                   Fines  mensa;
                     s32  electa;
                     s32  k;
                     s32  n;
                     s32  id;
                     i32  i;
               character  textus[XXXII];

    (vacuum)thema;
    contextus       = (NomenclatorFigurae*)ctx;
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    mensa.x         = NOMENCLATOR_LITTERA;
    mensa.y         = NOMENCLATOR_LINEA;
    mensa.latitudo  = c->fines.latitudo - II * NOMENCLATOR_LITTERA;
    mensa.altitudo  = c->fines.altitudo - II * NOMENCLATOR_LINEA;
    mandata_rectangulum(md, mensa, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!contextus || !contextus->m || !contextus->m->documentum)
    {
        redde;
    }
    m       = contextus->m;
    l       = ordines(m, md->piscina);
    electa  = nomenclator_persona_electa(m);
    k       = pagina_currens(m, l);
    n       = album_paginae(l);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        si (chorda_aequalis_literis(f->id, "titulus"))
        {
            chorda* filtrum;

            textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Contacts", md->piscina), FONS_TITULI,
                COLOR_TEXT);
            /* P3: rubrica filtri obscura iuxta */
            filtrum = filtrum_currens(m);
            si (filtrum)
            {
                textum_pingere(md, f->fines.x + IV + IX * fons_latitudo(
                    FONS_TITULI), f->fines.y + II + NOMENCLATOR_LINEA
                        - I,
                    chorda_concatenare(chorda_ex_literis("#",
                    md->piscina),
                    *filtrum, md->piscina), ZEPHYRUM, COLOR_TEXT_DIM);
            }
        }
        alioquin si (chorda_aequalis_literis(f->id, "visus-indicis"))
        {
            figura_segmentum_pingere(md, f->fines, chorda_transcribere(
                chorda_ex_literis("All", md->piscina), md->piscina),
                FIGURA_SEGMENTUM_PRIMUM, visus_rubricarum(m) ? ZEPHYRUM
                : FIGURA_ELECTUM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "visus-rubricarum"))
        {
            figura_segmentum_pingere(md, f->fines, chorda_transcribere(
                chorda_ex_literis("Tags", md->piscina), md->piscina),
                FIGURA_SEGMENTUM_ULTIMUM, visus_rubricarum(m)
                ? FIGURA_ELECTUM : ZEPHYRUM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            s32 omnes;
            s32 visae;
            i32 j;

            omnes = (s32)personae_numerus(m->documentum);
            visae = ZEPHYRUM;
            per (j = ZEPHYRUM; j < (i32)omnes; j++)
            {
                si (in_filtro(m, personae_index(m->documentum, j)))
                {
                    visae++;
                }
            }
            si (filtrum_currens(m))
            {
                sprintf(textus, "%d of %d contact%s", (integer)visae,
                    (integer)omnes, omnes == I ? "" : "s");
            }
            alioquin
            {
                sprintf(textus, "%d contact%s", (integer)omnes,
                    omnes == I ? "" : "s");
            }
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(textus, md->piscina), ZEPHYRUM,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "index_paginae"))
        {
            si (n > I)
            {
                sprintf(textus, "%d/%d", (integer)(k + I), (integer)n);
                textum_pingere(md, f->fines.x + (f->fines.latitudo
                    - (s32)strlen(textus) * NOMENCLATOR_LITTERA) / II,
                    f->fines.y + IV, chorda_ex_literis(textus,
                    md->piscina), ZEPHYRUM, COLOR_TEXT_DIM);
            }
        }
        alioquin si (chorda_aequalis_literis(f->id, "prompta"))
        {
            chorda genus;

            si (prompta_aperta(&m->ramus, &genus))
            {
                prompta_pingere(md, f->fines,
                    chorda_aequalis_literis(genus,
                    "societas") ? "business:" : chorda_aequalis_literis(
                    genus, "filtrum") ? "filter:" : "name:", &m->ramus);
            }
        }
        alioquin si (chorda_aequalis_literis(f->id, "nova"))
        {
            figura_globulum_pingere(md, f->fines, chorda_transcribere(
                chorda_ex_literis("+ New", md->piscina), md->piscina),
                FIGURA_UMBRA);
        }
        alioquin si (chorda_aequalis_literis(f->id, "pagina-prior"))
        {
            figura_sagittam_pingere(md, f->fines, -I, k <= ZEPHYRUM
                ? FIGURA_OBSCURUM : ZEPHYRUM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "pagina-proxima"))
        {
            figura_sagittam_pingere(md, f->fines, I, k >= n - I
                ? FIGURA_OBSCURUM : ZEPHYRUM);
        }
        alioquin si (   f->partes == PARTES_INDEX && f->id.mensura >= II
                     && f->id.datum[ZEPHYRUM] == 'l')
        {
            character littera[II];

            littera[ZEPHYRUM]  = f->id.datum[I] == '_'
                               ? '#' : (character)f->id.datum[I];
            littera[I]         = '\0';
            figura_fasciam_pingere(md, f->fines);
            figura_textum_audacem(md, f->fines.x + VI, f->fines.y + IV,
                chorda_transcribere(chorda_ex_literis(littera,
                md->piscina), md->piscina), figura_color_thematis(
                COLOR_TEXT));
        }
        alioquin si (   f->partes           == PARTES_INDEX
                     && gregis_index(f->id) >= ZEPHYRUM)
        {
               Xar* g;
            chorda  rubrica;
               s32  q;

            g = greges(m, md->piscina);
            q = gregis_index(f->id);
            si (g && q < (s32)xar_numerus(g))
            {
                rubrica = *(chorda*)xar_obtinere(g, (i32)q);
                figura_fasciam_pingere(md, f->fines);
                figura_textum_audacem(md, f->fines.x + VI, f->fines.y
                    + IV, chorda_transcribere(rubrica.mensura > ZEPHYRUM
                    ? chorda_concatenare(chorda_ex_literis("#",
                    md->piscina), rubrica, md->piscina)
                    : chorda_ex_literis("(none)", md->piscina),
                    md->piscina), figura_color_thematis(COLOR_TEXT));
            }
        }
        alioquin si (f->partes == PARTES_ITEM && f->id.mensura > I)
        {
            id = id_ordinis(f->id);
            si (id > ZEPHYRUM)
            {
                ordinem_pingere(m, md, f, id, electa);
            }
        }
    }
}

/* P2b-2: menu - res ex statu, figura elenchi */
interior vacuum
figura_elenchi (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
     NomenclatorFigurae* contextus;
     ContextusPraecepti  cp;
          ElenchiHospes  h;
               Elenchus  e;
                Punctum  ancora;

    contextus = (NomenclatorFigurae*)ctx;
    si (!contextus || !contextus->m)
    {
        redde;
    }
    hospitem_parare(contextus->m, md->piscina, &cp, &h);
    si (elenchi_legere(&contextus->m->ramus, &h, md->piscina, &e,
            &ancora))
    {
        elenchus_figura(c, md, thema, &e);
    }
}

interior b32
figuras_registrare (
    NomenclatorMontatio* m,
                    i32  latitudo,
                    i32  altitudo)
{
    NomenclatorFigurae* contextus;

    contextus = (NomenclatorFigurae*)piscina_allocare(m->piscina,
        magnitudo(NomenclatorFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    contextus->m                     = m;
    contextus->scheda                = piscina_generare_dynamicum(
        "nomenclator_scheda", XVI * M);
    contextus->latitudo_montationis  = (s32)latitudo;
    contextus->altitudo_montationis  = (s32)altitudo;
    m->figurae_ctx                   = contextus;
    redde contextus->scheda && figura_registrare(m->figurae,
        PARTES_TABULA, ZEPHYRUM, figura_tabulae, contextus)
        && figura_registrare(m->figurae, PARTES_DIALOGUS, ZEPHYRUM,
        figura_elenchi, contextus);
}


/* ==================================================
 * Componere (dispositio in cellulis)
 * ================================================== */

/* componens in nodo dispositionis; fines post computationem (glutinum
 * idem ac kalendarium.c - candidatus bibliothecae membrum__) */
nomen structura {
           s32  index;
     Componens* c;
} LocusComponentis;

nomen structura {
             Dispositio* d;
                    Xar* loci;
                Piscina* piscina;
    InternamentumChorda* intern;
} Disponens;

interior s32
locum_addere (
              Disponens* dp,
                    s32  parens,
     DispositioDirectio  directio,
        DispositioGenus  genus_x,
                    s32  valor_x,
        DispositioGenus  genus_y,
                    s32  valor_y)
{
    DispositioForma forma;

    dispositio_formam_initiare(&forma);
    forma.directio        = directio;
    forma.latitudo.genus  = genus_x;
    forma.latitudo.valor  = valor_x;
    forma.altitudo.genus  = genus_y;
    forma.altitudo.valor  = valor_y;
    redde dispositio_addere(dp->d, parens, &forma);
}

/* folium cum componente (actio nomenclatoris) */
interior Componens*
componentem_locare (
              Disponens* dp,
                    s32  parens,
     constans character* id,
                 Partes  partes,
        DispositioGenus  genus_x,
                    s32  valor_x,
                    s32  lineae)
{
            Componens* c;
     LocusComponentis* l;
                  s32  index;

    index = locum_addere(dp, parens, DISPOSITIO_LINEA, genus_x, valor_x,
        DISPOSITIO_FIXA, lineae);
    c     = componens_creare(dp->piscina, dp->intern, id, partes);
    si (!c || index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    componens_ponere_actio(c, actio_nomenclatoris);
    l = (LocusComponentis*)xar_addere(dp->loci);
    si (l)
    {
        l->index  = index;
        l->c      = c;
    }
    redde c;
}

Componens*
nomenclator_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
     NomenclatorMontatio* m;
               Componens* radix;
               Disponens  dp;
         DispositioForma  forma;
                   Album* l;
      constans AlbumOrdo* o;
                     s32  latitudo;
                     s32  altitudo;
                     s32  columnae;
                     s32  lineae;
                     s32  radix_d;
                     s32  titulus_d;
                     s32  caput;
                     s32  regio;
                     s32  a;
                     s32  b;
                     s32  t;
                     s32  i;
                   Fines  f;

    (vacuum)repo;
    m = (NomenclatorMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo = superficies(m, VERUM);
    altitudo = superficies(m, FALSUM);
    radix     = componens_creare(piscina, intern, "nomenclator",
        PARTES_TABULA);
    si (!radix)
    {
        redde NIHIL;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    componens_ponere_fines(radix, f);
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_nomenclatoris);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    columnae    = latitudo / NOMENCLATOR_LITTERA;
    lineae      = altitudo / NOMENCLATOR_LINEA;
    dp.d        = dispositio_creare(piscina);
    dp.loci     = xar_creare(piscina, (i32)magnitudo(LocusComponentis));
    dp.piscina  = piscina;
    dp.intern   = intern;
    si (!dp.d || !dp.loci)
    {
        redde radix;
    }
    dispositio_formam_initiare(&forma);
    forma.directio           = DISPOSITIO_COLUMNA;
    forma.latitudo.genus     = DISPOSITIO_CRESCENS;
    forma.altitudo.genus     = DISPOSITIO_CRESCENS;
    forma.spatium_sinistrum  = I;
    forma.spatium_dextrum    = I;
    forma.spatium_superum    = I;
    forma.spatium_inferum    = I;
    radix_d                  = dispositio_addere(dp.d, -I, &forma);
    /* P3: titulus et segmenta visus (All | Tags) ad dextrum */
    titulus_d = locum_addere(&dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        NOMENCLATOR_TITULI);
    (vacuum)componentem_locare(&dp, titulus_d, "titulus",
        PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, NOMENCLATOR_TITULI);
    (vacuum)componentem_locare(&dp, titulus_d, "visus-indicis",
        PARTES_BOTTONE, DISPOSITIO_FIXA, V, II);
    (vacuum)componentem_locare(&dp, titulus_d, "visus-rubricarum",
        PARTES_BOTTONE, DISPOSITIO_FIXA, VI, II);
    caput = locum_addere(&dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(&dp, caput, "nova", PARTES_BOTTONE,
        DISPOSITIO_FIXA, VII, NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(&dp, caput, "caput", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(&dp, caput, "pagina-prior",
        PARTES_BOTTONE,
        DISPOSITIO_FIXA, III, NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(&dp, caput, "index_paginae",
        PARTES_INDEX,
        DISPOSITIO_FIXA, VII, NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(&dp, caput, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, NOMENCLATOR_CAPUT);
    regio = locum_addere(&dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    l = ordines(m, piscina);
    si (l)
    {
        album_pagina(l, pagina_currens(m, l), &a, &b, &t);
        /* pagina in medio litterae: titulus iteratus ("<id>-c") */
        si (t >= ZEPHYRUM)
        {
            o = album_ordo(l, (i32)t);
            (vacuum)componentem_locare(&dp, regio, chorda_ut_cstr(
                chorda_concatenare(o->id, chorda_ex_literis("-c",
                piscina), piscina), piscina), PARTES_INDEX,
                DISPOSITIO_CRESCENS, ZEPHYRUM, o->altitudo);
        }
        per (i = a; i < b; i++)
        {
            o = album_ordo(l, (i32)i);
            (vacuum)componentem_locare(&dp, regio, chorda_ut_cstr(o->id,
                piscina), o->genus == ALBUM_RES ? PARTES_ITEM
                : PARTES_INDEX, DISPOSITIO_CRESCENS, ZEPHYRUM,
                o->altitudo);
        }
    }
    /* P2b: prompta in linea ultima regionis indicis */
    si (prompta_aperta(&m->ramus, NIHIL))
    {
        (vacuum)locum_addere(&dp, regio, DISPOSITIO_LINEA,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS,
            ZEPHYRUM);
        (vacuum)componentem_locare(&dp, regio, "prompta", PARTES_CAMPUS,
            DISPOSITIO_CRESCENS, ZEPHYRUM, NOMENCLATOR_ORDO);
    }
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(dp.loci); i++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, (i32)i);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            latitudo, altitudo, NOMENCLATOR_LITTERA,
                NOMENCLATOR_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    /* P2b-2: menu filius ULTIMUS (super omnia) */
    {
     ContextusPraecepti  cp;
          ElenchiHospes  h;
               Elenchus  e;
                Punctum  ancora;
              Componens* menu;

        hospitem_parare(m, piscina, &cp, &h);
        si (elenchi_legere(&m->ramus, &h, piscina, &e, &ancora))
        {
            menu = elenchus_componere(&e, elenchus_fines(&e, ancora, f),
                actio_nomenclatoris, piscina, intern);
            si (menu)
            {
                componens_addere_liberum(radix, menu);
            }
        }
    }
    redde radix;
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
nomenclator_montare (
     NomenclatorMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
      PersonaeDocumentum* documentum,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(NomenclatorMontatio));
    m->piscina     = piscina;
    m->intern      = intern;
    m->documentum  = documentum;
    m->ramus       = id ? insula_ramus(repo, "nomenclator", id)
                        : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "nomenclator",
        "nomenclator");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"nomenclator\"", id ? id : "nomenclator",
               piscina, intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    redde m->actiones && m->figurae
        && praecepta_verificare(TABULA_PRAECEPTORUM)
        && elenchi_verificare(TABULA_ELENCHORUM, TABULA_PRAECEPTORUM)
        && actio_registrare(m->actiones, actio_nomenclatoris, agere, m)
        && figuras_registrare(m, latitudo, altitudo);
}
