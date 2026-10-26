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
#include "membrum__charta.h"
#include "elenchus.h"
#include "rubrica.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include "fasti_fontes.h"
#include "similitudo.h"
#include "scriba_documentum.h"   /* scriba_effugere / scriba_solvere */
#include "semita.h"

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
                 Charta  notae;   /* P5b: liber "persona/<id>" */
       constans Organum* organum; /* semitae R4b: semita_legere */
       constans Organum* organum_scribae;  /* D5 S3c: liber notae */
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

/* D3: intus personae ordinalibus creationis (Persona.ordo = res albi)
 * designantur; id textus solum in limine (status, acta, liber).
 * Persona viva ordinis; NIHIL si nulla */
interior constans Persona*
persona_ordinis (
    constans NomenclatorMontatio* m,
                             s32  ordo)
{
    constans Persona* q;
                 i32  i;

    per (i = ZEPHYRUM; ordo > ZEPHYRUM && m->documentum
        && i < personae_numerus(m->documentum); i++)
    {
        q = personae_index(m->documentum, i);
        si (q && (s32)q->ordo == ordo)
        {
            redde q;
        }
    }
    redde NIHIL;
}

/* ordinalis personae vivae cuius id = textus; 0 si nulla */
interior s32
ordo_personae (
    constans NomenclatorMontatio* m,
                          chorda  id)
{
    constans Persona* q;

    q = m->documentum ? personae_persona(m->documentum, id) : NIHIL;
    redde q ? (s32)q->ordo : ZEPHYRUM;
}

/* id personae ordinis (in intern documenti); vacua si nulla */
interior chorda
id_personae (
    constans NomenclatorMontatio* m,
                             s32  ordo)
{
    constans Persona* q;
              chorda  vacua;

    q = persona_ordinis(m, ordo);
    si (q)
    {
        redde q->id;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* id personae ordinis ut literae in effusio (>= XXXII octeti); ""
 * si nulla */
interior constans character*
id_textus (
    constans NomenclatorMontatio* m,
                             s32  ordo,
                       character* effusio)
{
    chorda id;

    id = id_personae(m, ordo);
    memcpy(effusio, id.datum, (size_t)id.mensura);
    effusio[id.mensura] = '\0';
    redde effusio;
}

/* campus status (id textus) ut ordinalis; 0 si absens aut ignota */
interior s32
attributum_ordo (
    constans NomenclatorMontatio* m,
              constans character* titulus)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, titulus);
    redde a ? ordo_personae(m, *a) : ZEPHYRUM;
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

/* P4: quaestio currens in exitus[PROMPTA_MAXIMA] (soluta) - prompta
 * "quaestio" aperta: textus eius (vivus, dum typatur); aliter campus
 * quaestio (commissa); redde longitudinem, 0 = nulla */
interior i32
quaestio_currens (
    constans NomenclatorMontatio* m,
                       character* exitus)
{
     chorda  genus;
     chorda* a;
        s32  n;

    si (   prompta_aperta(&m->ramus, &genus)
        && chorda_aequalis_literis(genus, "quaestio"))
    {
        redde prompta_textus(&m->ramus, exitus);
    }
    a = insula_ramus_attributum(&m->ramus, "quaestio");
    n = (a && a->mensura > ZEPHYRUM) ? scriba_solvere(*a, exitus,
        PROMPTA_MAXIMA) : ZEPHYRUM;
    redde n > ZEPHYRUM ? (i32)n : ZEPHYRUM;
}

/* P4: quaestio commissa (campus) posita? */
interior b32
quaestio_commissa (
    constans NomenclatorMontatio* m)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, "quaestio");
    redde a && a->mensura > ZEPHYRUM;
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
            character  id[XLVIII];
                  i32  i;

    l = album_creare(p, capacitas(m));
    personae = m->documentum ? personae_ordinatae(m->documentum, p)
                             : NIHIL;
    /* P4: quaestio - ordo planus similitudinis nominum (filtro intra;
     * aequales: brevius, deinde ordine clavis), sine titulis */
    {
                  character   quaestio[PROMPTA_MAXIMA];
                     chorda   q_textus;
                     chorda*  nomina;
           constans Persona** candidati;
          SimilitudoFructus*  fructus;
                        i32   n;
                        i32   k;

        q_textus.mensura  = quaestio_currens(m, quaestio);
        q_textus.datum    = (i8*)quaestio;
        si (l && personae && q_textus.mensura > ZEPHYRUM)
        {
            nomina     = (chorda*)piscina_allocare(p, (memoriae_index)
                (xar_numerus(personae) + I) * magnitudo(chorda));
            candidati  = (constans Persona**)piscina_allocare(p,
                (memoriae_index)(xar_numerus(personae) + I)
                * magnitudo(constans Persona*));
            fructus    = (SimilitudoFructus*)piscina_allocare(p,
                (memoriae_index)(xar_numerus(personae) + I)
                * magnitudo(SimilitudoFructus));
            si (!nomina || !candidati || !fructus)
            {
                redde l;
            }
            n = ZEPHYRUM;
            per (i = ZEPHYRUM; i < xar_numerus(personae); i++)
            {
                q = *(constans Persona**)xar_obtinere(personae, i);
                si (in_filtro(m, q))
                {
                    candidati[n]  = q;
                    nomina[n]     = persona_nomen(q, p);
                    n++;
                }
            }
            n = similitudo_optima(q_textus, nomina, n, fructus, n);
            per (k = ZEPHYRUM; k < n; k++)
            {
                q = candidati[fructus[k].index];
                sprintf(id, "p%.*s", (integer)q->id.mensura,
                    (constans character*)q->id.datum);
                (vacuum)album_addere(l, id, ALBUM_RES, NOMENCLATOR_ORDO,
                    (s32)q->ordo);
            }
            redde l;
        }
    }
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
                sprintf(id, "p%.*s-%d", (integer)q->id.mensura,
                    (constans character*)q->id.datum, (integer)k);
                (vacuum)album_addere(l, id, ALBUM_RES, NOMENCLATOR_ORDO,
                    (s32)q->ordo);
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
        sprintf(id, "p%.*s", (integer)q->id.mensura,
            (constans character*)q->id.datum);
        (vacuum)album_addere(l, id, ALBUM_RES, NOMENCLATOR_ORDO,
            (s32)q->ordo);
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

/* ordinalis rei ordinis (RES) aut 0 */
interior s32
res_ordinis (
    constans AlbumOrdo* o)
{
    redde (o && o->genus == ALBUM_RES) ? o->res : ZEPHYRUM;
}

/* persona electa ut ordinalis (vide nomenclator_persona_electa); 0 si
 * nulla */
interior s32
electa_ordo (
    constans NomenclatorMontatio* m)
{
       Album* l;
     Piscina* p;
         s32  id;

    si (!m || !m->documentum)
    {
        redde ZEPHYRUM;
    }
    id = attributum_ordo(m, "persona_electa");
    p  = m->figurae_ctx ? ((NomenclatorFigurae*)m->figurae_ctx)->scheda
                        : m->piscina;
    /* P4: sub quaestione electa inter fructus esse debet (dum typatur
     * electa = prima) */
    {
        character quaestio[PROMPTA_MAXIMA];

        si (id > ZEPHYRUM && quaestio_currens(m, quaestio) > ZEPHYRUM)
        {
            l = ordines(m, p);
            redde (l
                && album_pagina_rei(l, id, ZEPHYRUM) >= ZEPHYRUM) ? id
                : (l ? album_res_prima(l, ZEPHYRUM) : ZEPHYRUM);
        }
    }
    si (id > ZEPHYRUM)
    {
        redde id;
    }
    /* P3: prima indicis (filtro et visu) */
    l = ordines(m, p);
    redde l ? album_res_prima(l, ZEPHYRUM) : ZEPHYRUM;
}

chorda
nomenclator_persona_electa (
    constans NomenclatorMontatio* m)
{
    redde id_personae(m, electa_ordo(m));
}

/* electio: persona_electa (id ordinis) et pagina eius */
interior b32
electionem_scribere (
    NomenclatorMontatio* m,
                    s32  id,
                Piscina* p)
{
     Album* l;
       s32  k;
 character  numerus[XXXII];
 character  pagina[XVI];

    l = ordines(m, p);
    k = l ? album_pagina_rei(l, id, pagina_currens(m, l)) : -I;
    (vacuum)id_textus(m, id, numerus);
    sprintf(pagina, "%d", (integer)(k < ZEPHYRUM ? ZEPHYRUM : k));
    redde campos_scribere(m, "persona_electa", numerus, "pagina",
        k > ZEPHYRUM ? pagina : "");
}

/* P5a: persona in visu singulari (viva, ordinalis) aut 0 */
interior s32
persona_aperta (
    constans NomenclatorMontatio* m)
{
    redde attributum_ordo(m, "persona_aperta");
}

/* P5a: ordines visus personae */
#define CAMPUS_NOMEN             ZEPHYRUM
#define CAMPUS_TELEPHONUM        I
#define CAMPUS_TELEPHONUM_NOVUM  II
#define CAMPUS_EPISTULA          III
#define CAMPUS_EPISTULA_NOVA     IV
#define CAMPUS_RUBRICAE          V
#define CAMPUS_NATALIS           VI

nomen structura {
    i32 genus;
    i32 index;   /* telephona / epistulae */
} CampusPersonae;

/* nomen, telephona, + phone, epistulae, + email, rubricae, natalis */
interior Xar*
campos_struere (
    constans Persona* q,
             Piscina* p)
{
               Xar* l;
    CampusPersonae* c;
               i32  i;

    l = xar_creare(p, (i32)magnitudo(CampusPersonae));
    si (!l || !q)
    {
        redde l;
    }
    c         = (CampusPersonae*)xar_addere(l);
    c->genus  = CAMPUS_NOMEN;
    c->index  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(q->telephona); i++)
    {
        c         = (CampusPersonae*)xar_addere(l);
        c->genus  = CAMPUS_TELEPHONUM;
        c->index  = i;
    }
    c         = (CampusPersonae*)xar_addere(l);
    c->genus  = CAMPUS_TELEPHONUM_NOVUM;
    c->index  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(q->epistulae); i++)
    {
        c         = (CampusPersonae*)xar_addere(l);
        c->genus  = CAMPUS_EPISTULA;
        c->index  = i;
    }
    c         = (CampusPersonae*)xar_addere(l);
    c->genus  = CAMPUS_EPISTULA_NOVA;
    c->index  = ZEPHYRUM;
    c         = (CampusPersonae*)xar_addere(l);
    c->genus  = CAMPUS_RUBRICAE;
    c->index  = ZEPHYRUM;
    c         = (CampusPersonae*)xar_addere(l);
    c->genus  = CAMPUS_NATALIS;
    c->index  = ZEPHYRUM;
    redde l;
}

/* campus electus ad [0, n - 1] coactus */
interior s32
campus_electus (
    constans NomenclatorMontatio* m,
                             s32  n)
{
    s32 k;

    k = attributum_s32(&m->ramus, "campus_electus", ZEPHYRUM);
    si (k >= n)
    {
        k = n - I;
    }
    redde k < ZEPHYRUM ? ZEPHYRUM : k;
}

hic_manens constans character* menses_anglice[XII] = {
    "January", "February", "March", "April", "May", "June", "July",
    "August", "September", "October", "November", "December"
};

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
   character  numerus[XXXII];

    si (!campos_scribere(m, "rubrica_filtrum", valor, "pagina", ""))
    {
        redde FALSUM;
    }
    l   = ordines(m, p);
    id  = l ? album_res_prima(l, ZEPHYRUM) : ZEPHYRUM;
    redde campos_scribere(m, "persona_electa", id_textus(m, id,
        numerus), NIHIL, NIHIL);
}

/* P4: quaestionem ponere (textus solutus; vacuus tollit); pagina
 * prima, electa prima fructuum */
interior b32
quaestionem_ponere (
    NomenclatorMontatio* m,
                 chorda  textus,
                Piscina* p)
{
       Album* l;
         s32  id;
   character  numerus[XXXII];
      chorda  effugitus;

    effugitus = textus.mensura > ZEPHYRUM ? scriba_effugere(
        (constans character*)textus.datum, textus.mensura, p)
        : chorda_ex_literis("", p);
    si (!campos_scribere(m, "quaestio", chorda_ut_cstr(effugitus, p),
            "pagina", ""))
    {
        redde FALSUM;
    }
    l   = ordines(m, p);
    id  = l ? album_res_prima(l, ZEPHYRUM) : ZEPHYRUM;
    redde campos_scribere(m, "persona_electa", id_textus(m, id,
        numerus), NIHIL, NIHIL);
}

/* P3: visum ponere; electa manet, pagina eam sequitur */
interior b32
visum_ponere (
    NomenclatorMontatio* m,
     constans character* visus,
                Piscina* p)
{
    s32 electa;

    electa = electa_ordo(m);
    si (!campos_scribere(m, "visus", visus, "pagina", ""))
    {
        redde FALSUM;
    }
    redde electa > ZEPHYRUM ? electionem_scribere(m, electa, p) : VERUM;
}

/* ordinalis personae ex ordine "p<id>" aut "p<id>-<k>"; 0 si alius
 * aut ignota */
interior s32
id_ordinis (
    constans NomenclatorMontatio* m,
                          chorda  id)
{
    chorda n;

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
    redde ordo_personae(m, n);
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

#define VISUS_INDICIS   I    /* index (P2a) */
#define VISUS_PERSONAE  II   /* persona aperta (P5a) */
#define VISUS_OMNES     (VISUS_INDICIS | VISUS_PERSONAE)

interior i32
visus_currens (
    constans NomenclatorMontatio* m)
{
    redde persona_aperta(m) > ZEPHYRUM ? VISUS_PERSONAE : VISUS_INDICIS;
}

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
    character numerus[XXXII];
    character pagina[XVI];

    k = pagina_currens(cp->m, cp->l) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    id = album_res_prima(cp->l, k);
    sprintf(pagina, "%d", (integer)k);
    redde campos_scribere(cp->m, "pagina", pagina, "persona_electa",
        id_textus(cp->m, id, numerus));
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
#define REQUIRIT_FILTRUM   II   /* filtrum aut quaestio (P4) positum */
#define REQUIRIT_NATALEM   IV   /* P6c: persona aperta cum natali, porta
                                 * diei et hodie */

/* P6c: natalis personae apertae in calendario aperiri potest? */
interior b32
natalis_aperibilis (
    constans NomenclatorMontatio* m)
{
    constans Persona* q;

    q = persona_ordinis(m, persona_aperta(m));
    redde q && q->natalis_positus && m->dies_aperire && m->hodie;
}

/* P6c: natalis proximus (hodie inclusa; 29 II -> 28 II anni communis)
 * portae diei redditur */
interior b32
fac_ad_natalem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
      constans Persona* q;
                  Dies  hodie;
                  Dies  ultima;
                  Dies  dies[II];
                   i32  n;

    si (   !natalis_aperibilis(cp->m)
        || !cp->m->hodie(cp->m->hodie_ctx, &hodie))
    {
        redde FALSUM;
    }
    q       = persona_ordinis(cp->m, persona_aperta(cp->m));
    ultima  = fasti_addere_annos(hodie, I);
    n       = fasti_annuum_expandere(q->natalis.mensis, q->natalis.dies,
        hodie, ultima, dies, II);
    si (n < I)
    {
        redde FALSUM;
    }
    redde cp->m->dies_aperire(cp->m->dies_aperire_ctx, dies[ZEPHYRUM]);
}

interior b32
requisita_nomenclatoris (
       i32  requisita,
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde (!(requisita & REQUIRIT_PERSONAM) || cp->electa > ZEPHYRUM)
        && (   !(requisita & REQUIRIT_FILTRUM) || filtrum_currens(cp->m)
            || quaestio_commissa(cp->m))
        && (!(requisita & REQUIRIT_NATALEM)
        || natalis_aperibilis(cp->m));
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

/* P4: Esc in indice - quaestio primum, deinde filtrum */
interior b32
fac_filtrum_tollere (
    vacuum* ctx)
{
     ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
                 chorda  vacua;

    si (quaestio_commissa(cp->m))
    {
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde quaestionem_ponere(cp->m, vacua, cp->p);
    }
    redde filtrum_ponere(cp->m, "", cp->p);
}

/* P4: quaestio (prompta "search:", quaestione commissa praeimpleta) */
interior b32
fac_quaestionem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
             character  quaestio[PROMPTA_MAXIMA];
                chorda  textus;

    textus.datum    = (i8*)quaestio;
    textus.mensura  = ZEPHYRUM;
    si (quaestio_commissa(cp->m))
    {
        textus.mensura = quaestio_currens(cp->m, quaestio);
    }
    redde prompta_aperire(&cp->m->ramus, actio_nomenclatoris,
        "quaestio", textus, cp->p);
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
    si (personae_delere(cp->m->documentum, id_personae(cp->m,
            cp->electa)) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* P5a: ex visu personae - ad indicem */
    (vacuum)campos_scribere(cp->m, "persona_aperta", "",
        "campus_electus",
        "");
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

/* P5a: visus personae */
interior b32
fac_aperire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
             character  numerus[XXXII];

    redde campos_scribere(cp->m, "persona_aperta", id_textus(cp->m,
        cp->electa, numerus), "campus_electus", "");
}

interior b32
fac_redire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde campos_scribere(cp->m, "persona_aperta", "", "campus_electus",
        "");
}

interior b32
campum_movere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    constans Persona* q;
                 Xar* campi;
                 s32  k;
           character  numerus[XVI];

    q      = persona_ordinis(cp->m, cp->electa);
    campi  = campos_struere(q, cp->p);
    si (!campi)
    {
        redde VERUM;
    }
    k = campus_electus(cp->m, (s32)xar_numerus(campi)) + gradus;
    si (k < ZEPHYRUM || k >= (s32)xar_numerus(campi))
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)k);
    redde campos_scribere(cp->m, "campus_electus", numerus, NIHIL,
        NIHIL);
}

interior b32
fac_campum_proximum (
    vacuum* ctx)
{
    redde campum_movere((ContextusPraecepti*)ctx, I);
}

interior b32
fac_campum_priorem (
    vacuum* ctx)
{
    redde campum_movere((ContextusPraecepti*)ctx, -I);
}

/* natalis in prompta: "YYYY-MM-DD" aut "MM-DD"; nullus "" */
interior chorda
natalem_textu (
    constans Persona* q,
             Piscina* p)
{
    character t[XVI];

    si (!q->natalis_positus)
    {
        redde chorda_ex_literis("", p);
    }
    si (q->natalis.annus == ZEPHYRUM)
    {
        sprintf(t, "%02d-%02d", (integer)q->natalis.mensis,
            (integer)q->natalis.dies);
    }
    alioquin
    {
        sprintf(t, "%04d-%02d-%02d", (integer)q->natalis.annus,
            (integer)q->natalis.mensis, (integer)q->natalis.dies);
    }
    redde chorda_ex_literis(t, p);
}

/* Enter in campo: prompta generis campi, valore praeimpleta */
interior b32
fac_campum_edere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
      constans Persona* q;
                   Xar* campi;
        CampusPersonae* campus;
                chorda  valor;
    constans character* genus;

    q      = persona_ordinis(cp->m, cp->electa);
    campi  = campos_struere(q, cp->p);
    si (!q || !campi || xar_numerus(campi) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    campus = (CampusPersonae*)xar_obtinere(campi, (i32)campus_electus(
        cp->m, (s32)xar_numerus(campi)));
    valor  = chorda_ex_literis("", cp->p);
    commutatio (campus->genus)
    {
        casus CAMPUS_NOMEN:
            genus = "nomen";
            valor = q->genus == PERSONA_SOCIETAS ? q->titulus
                  : persona_nomen(q, cp->p);
            frange;
        casus CAMPUS_TELEPHONUM:
            genus = "telephonum";
            valor = *(chorda*)xar_obtinere(q->telephona, campus->index);
            frange;
        casus CAMPUS_TELEPHONUM_NOVUM:
            genus = "telephonum";
            frange;
        casus CAMPUS_EPISTULA:
            genus = "epistula";
            valor = *(chorda*)xar_obtinere(q->epistulae, campus->index);
            frange;
        casus CAMPUS_EPISTULA_NOVA:
            genus = "epistula";
            frange;
        casus CAMPUS_RUBRICAE:
            genus = "rubricae";
            valor = rubricas_iungere(q->rubricae, cp->p);
            frange;
        ordinarius:
            genus = "natalis";
            valor = natalem_textu(q, cp->p);
            frange;
    }
    redde prompta_aperire(&cp->m->ramus, actio_nomenclatoris, genus,
        valor, cp->p);
}

interior b32
genus_ponere (
    ContextusPraecepti* cp,
          PersonaGenus  genus)
{
    redde personae_genus_ponere(cp->m->documentum, id_personae(cp->m,
        cp->electa), genus) > ZEPHYRUM;
}

interior b32
fac_genus_alternare (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
      constans Persona* q;

    q = persona_ordinis(cp->m, cp->electa);
    redde q && genus_ponere(cp, q->genus == PERSONA_HOMO
        ? PERSONA_SOCIETAS : PERSONA_HOMO);
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
    { "nomenclator.delere", "Delete contact", "X", VISUS_OMNES,
        REQUIRIT_PERSONAM, fac_delere },
    { "nomenclator.revocare", "Undo", "u", VISUS_OMNES, ZEPHYRUM,
        fac_revocare },
    { "nomenclator.reficere", "Redo", "C-r", VISUS_OMNES, ZEPHYRUM,
        fac_reficere },
    { "nomenclator.aperire", "Open contact", "Enter", VISUS_INDICIS,
        REQUIRIT_PERSONAM, fac_aperire },
    { "nomenclator.redire", "Back to list", "Esc", VISUS_PERSONAE,
        ZEPHYRUM, fac_redire },
    { "nomenclator.campus_proximus", "Next field", "j", VISUS_PERSONAE,
        ZEPHYRUM, fac_campum_proximum },
    { "nomenclator.campus_prior", "Previous field", "k", VISUS_PERSONAE,
        ZEPHYRUM, fac_campum_priorem },
    { "nomenclator.campum_edere", "Edit field", "Enter", VISUS_PERSONAE,
        ZEPHYRUM, fac_campum_edere },
    { "nomenclator.genus_alternare", "Person / business", "t",
        VISUS_PERSONAE, ZEPHYRUM, fac_genus_alternare },
    { "nomenclator.ad_natalem", "Birthday in calendar", "c",
        VISUS_PERSONAE, REQUIRIT_NATALEM, fac_ad_natalem },
    { "nomenclator.filtrum", "Filter by tag", "f", VISUS_INDICIS,
        ZEPHYRUM, fac_filtrum },
    { "nomenclator.filtrum_tollere", "Clear search / filter", "Esc",
        VISUS_INDICIS, REQUIRIT_FILTRUM, fac_filtrum_tollere },
    { "nomenclator.quaestio", "Search", "/", VISUS_INDICIS, ZEPHYRUM,
        fac_quaestionem },
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
    cp->m = m;
    cp->p = p;
    cp->l = ordines(m, p);
    cp->electa  = persona_aperta(m) > ZEPHYRUM ? persona_aperta(m)
                : electa_ordo(m);
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
    h->visus      = visus_currens(m);
    h->requisita  = requisita_nomenclatoris;
    h->ctx        = cp;
}

/* "verba cognomen": verbum ultimum cognomen, cetera praenomen */
interior vacuum
nomen_dividere (
     chorda  textus,
     chorda* praenomen,
     chorda* cognomen)
{
    i32 i;

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
    *praenomen  = textus;
    *cognomen   = textus;
    si (i == ZEPHYRUM)
    {
        cognomen->mensura = ZEPHYRUM;
        redde;
    }
    cognomen->datum     = textus.datum + i;
    cognomen->mensura   = textus.mensura - i;
    praenomen->mensura  = i;
    dum (   praenomen->mensura > ZEPHYRUM
         && praenomen->datum[praenomen->mensura - I] == ' ')
    {
        praenomen->mensura--;
    }
}

/* natalis forma americana (Franus 10-15): "M/D/YYYY" (mensis et dies
 * I aut II cifris, annus IV cifris > 0 - "86" ambiguum) aut "M/D" sine
 * anno; FALSUM si forma mala */
interior b32
natalem_americanum (
    chorda  t,
      Dies* d)
{
    s32 partes[III];
    i32 cifrae[III];
    i32 n;
    i32 i;

    n                 = ZEPHYRUM;
    partes[ZEPHYRUM]  = ZEPHYRUM;
    cifrae[ZEPHYRUM]  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        si (t.datum[i] == '/')
        {
            /* pars vacua (mensis/dies 0) a personis recusatur */
            si (n == II)
            {
                redde FALSUM;
            }
            n++;
            partes[n] = ZEPHYRUM;
            cifrae[n] = ZEPHYRUM;
            perge;
        }
        si (t.datum[i] < '0' || t.datum[i] > '9' || cifrae[n] == IV)
        {
            redde FALSUM;
        }
        partes[n] = partes[n] * X + (s32)(t.datum[i] - '0');
        cifrae[n]++;
    }
    si (   n == ZEPHYRUM || cifrae[n] == ZEPHYRUM
        || cifrae[ZEPHYRUM] > II || cifrae[I] > II
        || (n == II && (cifrae[II] != IV || partes[II] <= ZEPHYRUM)))
    {
        redde FALSUM;
    }
    d->annus   = n == II ? partes[II] : ZEPHYRUM;
    d->mensis  = partes[ZEPHYRUM];
    d->dies    = partes[I];
    redde VERUM;
}

/* natalis ex textu: "YYYY-MM-DD", "MM-DD", "--MM-DD", forma americana
 * ("M/D/YYYY", "M/D"); FALSUM si forma mala (validitas a personis
 * iudicatur) */
interior b32
natalem_ex_textu (
    chorda  t,
      Dies* d)
{
    s32 o;
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        si (t.datum[i] == '/')
        {
            redde natalem_americanum(t, d);
        }
    }
    si (t.mensura == X)
    {
        redde fasti_parsare_iso(t, d) && d->annus > ZEPHYRUM;
    }
    o = t.mensura == VII ? II : t.mensura == V ? ZEPHYRUM : -I;
    si (   o < ZEPHYRUM || (o == II && (t.datum[ZEPHYRUM] != '-'
            || t.datum[I] != '-')) || t.datum[o + II] != '-'
        || t.datum[o] < '0' || t.datum[o] > '9' || t.datum[o + I] < '0'
        || t.datum[o + I] > '9' || t.datum[o + III] < '0'
        || t.datum[o + III] > '9' || t.datum[o + IV] < '0'
        || t.datum[o + IV] > '9')
    {
        redde FALSUM;
    }
    d->annus   = ZEPHYRUM;
    d->mensis  = (s32)(t.datum[o] - '0') * X + (s32)(t.datum[o + I]
        - '0');
    d->dies    = (s32)(t.datum[o + III] - '0') * X + (s32)(t.datum[o
        + IV] - '0');
    redde VERUM;
}

/* index (telephona / epistulae) mutatus: index k substitutus (vacuum:
 * deletus), aut additus (k < 0) */
interior Xar*
indicem_mutare (
     constans Xar* vetus,
              s32  k,
           chorda  valor,
          Piscina* p)
{
    Xar* l;
    i32  i;

    l = xar_creare(p, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; l && vetus && i < xar_numerus(vetus); i++)
    {
        si ((s32)i == k)
        {
            si (valor.mensura > ZEPHYRUM)
            {
                *(chorda*)xar_addere(l) = valor;
            }
            perge;
        }
        *(chorda*)xar_addere(l) = *(chorda*)xar_obtinere(vetus, i);
    }
    si (l && k < ZEPHYRUM && valor.mensura > ZEPHYRUM)
    {
        *(chorda*)xar_addere(l) = valor;
    }
    redde l;
}

/* P5a: Reditus in campo personae apertae: VERUM actum, FALSUM
 * recusatum (prompta manet), -1 genus non campi */
interior s32
campum_committere (
    NomenclatorMontatio* m,
                 chorda  genus,
                 chorda  textus,
                Piscina* p)
{
     constans Persona* q;
                  Xar* campi;
       CampusPersonae* campus;
               chorda  id;
                 Dies  d;
               chorda  praenomen;
               chorda  cognomen;

    q      = persona_ordinis(m, persona_aperta(m));
    campi  = campos_struere(q, p);
    si (!q || !campi)
    {
        redde -I;
    }
    id = q->id;
    campus = (CampusPersonae*)xar_obtinere(campi, (i32)campus_electus(m,
        (s32)xar_numerus(campi)));
    si (chorda_aequalis_literis(genus, "nomen"))
    {
        si (q->genus == PERSONA_SOCIETAS)
        {
            redde personae_titulum_ponere(m->documentum, id, textus)
                > ZEPHYRUM;
        }
        nomen_dividere(textus, &praenomen, &cognomen);
        redde personae_nomen_ponere(m->documentum, id, praenomen,
            cognomen) > ZEPHYRUM;
    }
    si (chorda_aequalis_literis(genus, "telephonum"))
    {
        redde personae_telephona_ponere(m->documentum, id,
            indicem_mutare(q->telephona, campus->genus
            == CAMPUS_TELEPHONUM ? (s32)campus->index : -I, textus, p))
            > ZEPHYRUM;
    }
    si (chorda_aequalis_literis(genus, "epistula"))
    {
        redde personae_epistulas_ponere(m->documentum, id,
            indicem_mutare(q->epistulae, campus->genus
            == CAMPUS_EPISTULA ? (s32)campus->index : -I, textus, p))
            > ZEPHYRUM;
    }
    si (chorda_aequalis_literis(genus, "rubricae"))
    {
        redde personae_rubricas_ponere(m->documentum, id, textus)
            > ZEPHYRUM;
    }
    si (chorda_aequalis_literis(genus, "natalis"))
    {
        si (textus.mensura == ZEPHYRUM)
        {
            redde personae_natalem_ponere(m->documentum, id, NIHIL)
                > ZEPHYRUM;
        }
        redde natalem_ex_textu(textus, &d) && personae_natalem_ponere(
            m->documentum, id, &d) > ZEPHYRUM;
    }
    redde -I;
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
    chorda id;
       i32 i;

    /* P5a: campi personae apertae */
    si (persona_aperta(m) > ZEPHYRUM)
    {
        s32 r;

        r = campum_committere(m, genus, textus, p);
        si (r != -I)
        {
            si (r)
            {
                (vacuum)prompta_claudere(&m->ramus, actio_nomenclatoris,
                    p);
            }
            redde r != ZEPHYRUM;
        }
    }
    /* P4: quaestio - committitur (vacua tollit), prompta clauditur */
    si (chorda_aequalis_literis(genus, "quaestio"))
    {
        (vacuum)prompta_claudere(&m->ramus, actio_nomenclatoris, p);
        redde quaestionem_ponere(m, textus, p);
    }
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
    si (id.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    (vacuum)prompta_claudere(&m->ramus, actio_nomenclatoris, p);
    redde electionem_scribere(m, ordo_personae(m, id), p);
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
    (vacuum)des;
    m         = (NomenclatorMontatio*)ctx;
    privatum  = (NomenclatorFigurae*)m->figurae_ctx;
    si (!privatum)
    {
        redde FALSUM;
    }
    /* P5b: focus in notis - claves notis et dispensatori (Tab, Esc
     * gradus) relinquuntur */
    si (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
        && charta_focata(&privatum->notae, motus))
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
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, visus_currens(m),
            ev);
        si (!t)
        {
            redde FALSUM;
        }
        contextum_parare(m, privatum->scheda, &cp);
        redde praecepta_status(t, visus_currens(m),
            requisita_nomenclatoris,
            &cp) == PRAECEPTUM_PRAESTO && t->fn(&cp);
    }
    /* P5a: ictus duplex in ordine indicis personam aperit */
    si (   ev->genus            == EVENTUS_MUS_DUPLEX && nodus
        && ev->datum.mus.botton == MUS_SINISTER
        && nodus->partes        == PARTES_ITEM
        && id_ordinis(m, nodus->id)
        > ZEPHYRUM && persona_aperta(m) == ZEPHYRUM)
    {
        character numerus[XXXII];

        redde campos_scribere(m, "persona_aperta", id_textus(m,
            id_ordinis(m, nodus->id), numerus), "campus_electus", "");
    }
    si (   ev->genus            != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER || !nodus)
    {
        redde FALSUM;
    }
    /* P5a: visus personae - campus eligitur, redire, genus */
    si (persona_aperta(m) > ZEPHYRUM)
    {
        contextum_parare(m, privatum->scheda, &cp);
        si (   nodus->partes == PARTES_ITEM && nodus->id.mensura >= II
            && nodus->id.datum[ZEPHYRUM] == 'f')
        {
            chorda n;

            n.datum    = nodus->id.datum + I;
            n.mensura  = nodus->id.mensura - I;
            redde chorda_ut_s32(n, &id) && campos_scribere(m,
                "campus_electus", chorda_ut_cstr(n, privatum->scheda),
                NIHIL, NIHIL);
        }
        si (chorda_aequalis_literis(nodus->id, "redire"))
        {
            redde fac_redire(&cp);
        }
        /* P6c: sagitta ad natalem in calendario */
        si (chorda_aequalis_literis(nodus->id, "ad_natalem"))
        {
            redde fac_ad_natalem(&cp);
        }
        si (chorda_aequalis_literis(nodus->id, "genus-homo"))
        {
            redde genus_ponere(&cp, PERSONA_HOMO);
        }
        si (chorda_aequalis_literis(nodus->id, "genus-societas"))
        {
            redde genus_ponere(&cp, PERSONA_SOCIETAS);
        }
        redde FALSUM;
    }
    id = nodus->partes
        == PARTES_ITEM ? id_ordinis(m, nodus->id) : ZEPHYRUM;
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
        s32  y,
        i32  color)
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
    mandata_polygonum(md, p, IV, figura_color_thematis(color), VERUM);
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

    q = persona_ordinis(m, id);
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
        /* ordo electus: non obscurum (fundus selectionis) */
        rhombum_pingere(md, x, l->fines.y + V, id == electa ? COLOR_TEXT
            : COLOR_TEXT_DIM);
    }
}

/* titulus generis promptae */
interior constans character*
titulum_promptae (
    constans NomenclatorMontatio* m,
                          chorda  genus)
{
    constans Persona* q;

    si (chorda_aequalis_literis(genus, "societas"))
    {
        redde "business:";
    }
    si (chorda_aequalis_literis(genus, "filtrum"))
    {
        redde "filter:";
    }
    si (chorda_aequalis_literis(genus, "quaestio"))
    {
        redde "search:";
    }
    si (chorda_aequalis_literis(genus, "nomen"))
    {
        q = persona_ordinis(m, persona_aperta(m));
        redde (q && q->genus == PERSONA_SOCIETAS) ? "business:"
            : "name:";
    }
    si (chorda_aequalis_literis(genus, "telephonum"))
    {
        redde "phone:";
    }
    si (chorda_aequalis_literis(genus, "epistula"))
    {
        redde "email:";
    }
    si (chorda_aequalis_literis(genus, "rubricae"))
    {
        redde "tags:";
    }
    si (chorda_aequalis_literis(genus, "natalis"))
    {
        redde "birthday:";
    }
    redde "name:";
}

/* P5a: ordo campi personae: titulus (obscurus) et valor */
interior vacuum
campum_pingere (
        NomenclatorMontatio* m,
                    Mandata* md,
         constans Componens* f,
           constans Persona* q,
    constans CampusPersonae* campus,
                        b32  electus)
{
       chorda valor;
    character textus[LXIV];
         Dies hodie;
          s32 aetas;
          s32 x;
          i32 titulus_color;

    /* tituli obscuri in fundo selectionis evanescunt: colore fundi
     * (Franus 10-15) */
    titulus_color = electus ? COLOR_BACKGROUND : COLOR_TEXT_DIM;
    si (electus)
    {
        figura_ordinem_electum_pingere(md, f->fines);
    }
    x = f->fines.x + IV + XI * NOMENCLATOR_LITTERA;
    commutatio (campus->genus)
    {
        casus CAMPUS_NOMEN:
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                persona_nomen(q, md->piscina), FONS_TITULI, COLOR_TEXT);
            redde;
        casus CAMPUS_TELEPHONUM:
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("Phone", md->piscina), ZEPHYRUM,
                titulus_color);
            valor = *(chorda*)xar_obtinere(q->telephona, campus->index);
            frange;
        casus CAMPUS_TELEPHONUM_NOVUM:
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("+ phone", md->piscina), ZEPHYRUM,
                titulus_color);
            redde;
        casus CAMPUS_EPISTULA:
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("Email", md->piscina), ZEPHYRUM,
                titulus_color);
            valor = *(chorda*)xar_obtinere(q->epistulae, campus->index);
            frange;
        casus CAMPUS_EPISTULA_NOVA:
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("+ email", md->piscina), ZEPHYRUM,
                titulus_color);
            redde;
        casus CAMPUS_RUBRICAE:
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("Tags", md->piscina), ZEPHYRUM,
                titulus_color);
            /* "#a #b" */
            valor = chorda_ex_literis("", md->piscina);
            {
                i32 r;

                per (r = ZEPHYRUM; r < xar_numerus(q->rubricae); r++)
                {
                    valor = chorda_concatenare(valor, chorda_ex_literis(
                        r > ZEPHYRUM ? " #" : "#", md->piscina),
                        md->piscina);
                    valor = chorda_concatenare(valor, *(chorda*)
                        xar_obtinere(q->rubricae, r), md->piscina);
                }
            }
            frange;
        ordinarius:
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("Birthday", md->piscina), ZEPHYRUM,
                titulus_color);
            si (!q->natalis_positus)
            {
                valor = chorda_ex_literis("-", md->piscina);
                frange;
            }
            si (q->natalis.annus > ZEPHYRUM)
            {
                sprintf(textus, "%d %s %d", (integer)q->natalis.dies,
                    menses_anglice[q->natalis.mensis - I],
                    (integer)q->natalis.annus);
            }
            alioquin
            {
                sprintf(textus, "%d %s", (integer)q->natalis.dies,
                    menses_anglice[q->natalis.mensis - I]);
            }
            aetas = (m->hodie && m->hodie(m->hodie_ctx, &hodie))
                  ? persona_aetas(q, hodie) : -I;
            si (aetas >= ZEPHYRUM)
            {
                sprintf(textus + strlen(textus), "  %d",
                    (integer)aetas);
            }
            valor = chorda_ex_literis(textus, md->piscina);
            frange;
    }
    textum_pingere(md, x, f->fines.y + IV, praecidere(valor,
        f->fines.x + f->fines.latitudo - x), ZEPHYRUM, COLOR_TEXT);
}

/* P5a: figura visus personae (spatio radicis) */
interior vacuum
personam_pingere (
    NomenclatorMontatio* m,
                Mandata* md,
     constans Componens* c)
{
     constans Persona* q;
                  Xar* campi;
            Componens* f;
                  s32  electus;
                  s32  k;
                  i32  i;

    q      = persona_ordinis(m, persona_aperta(m));
    campi  = campos_struere(q, md->piscina);
    si (!q || !campi)
    {
        redde;
    }
    electus = campus_electus(m, (s32)xar_numerus(campi));
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        si (chorda_aequalis_literis(f->id, "redire"))
        {
            figura_globulum_pingere(md, f->fines, chorda_transcribere(
                chorda_ex_literis("< Contacts", md->piscina),
                md->piscina), FIGURA_UMBRA);
        }
        alioquin si (chorda_aequalis_literis(f->id, "genus-homo"))
        {
            figura_segmentum_pingere(md, f->fines, chorda_transcribere(
                chorda_ex_literis("Person", md->piscina), md->piscina),
                FIGURA_SEGMENTUM_PRIMUM, q->genus == PERSONA_HOMO
                ? FIGURA_ELECTUM : ZEPHYRUM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "genus-societas"))
        {
            figura_segmentum_pingere(md, f->fines, chorda_transcribere(
                chorda_ex_literis("Business", md->piscina),
                md->piscina),
                FIGURA_SEGMENTUM_ULTIMUM, q->genus == PERSONA_SOCIETAS
                ? FIGURA_ELECTUM : ZEPHYRUM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "ad_natalem"))
        {
            figura_sagittam_pingere(md, f->fines, I, ZEPHYRUM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "imago"))
        {
            /* P7: imago - locus servatus */
            mandata_rectangulum(md, f->fines, figura_color_thematis(
                COLOR_TEXT_DIM), FALSUM);
            textum_pingere(md, f->fines.x + (f->fines.latitudo - V
                * NOMENCLATOR_LITTERA) / II, f->fines.y
                + (f->fines.altitudo - NOMENCLATOR_LINEA) / II,
                chorda_ex_literis("photo", md->piscina), ZEPHYRUM,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "prompta"))
        {
            chorda genus;

            si (prompta_aperta(&m->ramus, &genus))
            {
                prompta_pingere(md, f->fines, titulum_promptae(m,
                    genus),
                    &m->ramus);
            }
        }
        alioquin si (   f->partes == PARTES_ITEM && f->id.mensura >= II
                     && f->id.datum[ZEPHYRUM] == 'f')
        {
            chorda n;

            n.datum    = f->id.datum + I;
            n.mensura  = f->id.mensura - I;
            si (   chorda_ut_s32(n, &k) && k >= ZEPHYRUM
                && k < (s32)xar_numerus(campi))
            {
                campum_pingere(m, md, f, q, (constans CampusPersonae*)
                    xar_obtinere(campi, (i32)k), k == electus);
            }
        }
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
    si (persona_aperta(m) > ZEPHYRUM)
    {
        /* P5b: umbra plana chartae notarum (scriba supra pingitur) */
        per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
        {
            f = componens_liberum(c, i);
            si (chorda_aequalis_literis(f->id, "notae"))
            {
                charta_umbram_pingere(&contextus->notae, md, f->fines);
            }
        }
        personam_pingere(m, md, c);
        redde;
    }
    l       = ordines(m, md->piscina);
    electa  = electa_ordo(m);
    k       = pagina_currens(m, l);
    n       = album_paginae(l);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        si (chorda_aequalis_literis(f->id, "titulus"))
        {
                chorda* filtrum;
                chorda  iuxta;
                chorda  quaestio;
             character  quaestio_textus[PROMPTA_MAXIMA];

            textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Contacts", md->piscina), FONS_TITULI,
                COLOR_TEXT);
            /* P3: rubrica filtri obscura iuxta; P4: "/quaestio" post */
            filtrum  = filtrum_currens(m);
            iuxta    = chorda_ex_literis("", md->piscina);
            si (filtrum)
            {
                iuxta = chorda_concatenare(chorda_ex_literis("#",
                    md->piscina), *filtrum, md->piscina);
            }
            quaestio.mensura  = quaestio_currens(m, quaestio_textus);
            quaestio.datum    = (i8*)quaestio_textus;
            si (quaestio.mensura > ZEPHYRUM)
            {
                iuxta = chorda_concatenare(iuxta, chorda_ex_literis(
                    filtrum ? " /" : "/", md->piscina), md->piscina);
                iuxta = chorda_concatenare(iuxta, quaestio,
                    md->piscina);
            }
            si (iuxta.mensura > ZEPHYRUM)
            {
                textum_pingere(md, f->fines.x + IV + IX * fons_latitudo(
                    FONS_TITULI), f->fines.y + II + NOMENCLATOR_LINEA
                    - I, iuxta, ZEPHYRUM, COLOR_TEXT_DIM);
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
            character quaestio_textus[PROMPTA_MAXIMA];

            omnes = (s32)personae_numerus(m->documentum);
            visae = ZEPHYRUM;
            per (j = ZEPHYRUM; j < (i32)omnes; j++)
            {
                si (in_filtro(m, personae_index(m->documentum, j)))
                {
                    visae++;
                }
            }
            /* P4: sub quaestione visae = fructus */
            si (quaestio_currens(m, quaestio_textus) > ZEPHYRUM)
            {
                visae = ZEPHYRUM;
                per (j = ZEPHYRUM; l && j < album_numerus(l); j++)
                {
                    si (album_ordo(l, j)->genus == ALBUM_RES)
                    {
                        visae++;
                    }
                }
            }
            si (   filtrum_currens(m)
                || quaestio_currens(m, quaestio_textus) > ZEPHYRUM)
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
                prompta_pingere(md, f->fines, titulum_promptae(m,
                    genus),
                    &m->ramus);
            }
        }
        alioquin si (chorda_aequalis_literis(f->id, "nihil_inventum"))
        {
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("No matches", md->piscina), ZEPHYRUM,
                COLOR_TEXT_DIM);
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
            id = id_ordinis(m, f->id);
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
    /* semitae R4b: zephyrum - notae.montata (librum_aptare) valida
     * etiam ante charta_montare (piscina non purgat) */
    memset(contextus, ZEPHYRUM, magnitudo(NomenclatorFigurae));
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

    /* lineae < 0: altitudo crescens */
    index = locum_addere(dp, parens, DISPOSITIO_LINEA, genus_x, valor_x,
        lineae >= ZEPHYRUM ? DISPOSITIO_FIXA : DISPOSITIO_CRESCENS,
        lineae >= ZEPHYRUM ? lineae : ZEPHYRUM);
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

/* P5a: visus personae - barra (redire, Person | Business), dimidium
 * superum (campi; imago ~1/3 latitudinis ad dextrum), dimidium inferum
 * notae (P5b) */
interior vacuum
personam_disponere (
    NomenclatorMontatio* m,
              Disponens* dp,
                    s32  radix_d,
                    s32  columnae,
                Piscina* piscina)
{
     constans Persona* q;
                  Xar* campi;
                  s32  barra;
                  s32  corpus;
                  s32  superum;
                  s32  campi_d;
                  i32  i;
            character  id[XVI];

    q      = persona_ordinis(m, persona_aperta(m));
    campi  = campos_struere(q, piscina);
    barra  = locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, III);
    (vacuum)componentem_locare(dp, barra, "redire", PARTES_BOTTONE,
        DISPOSITIO_FIXA, XII, II);
    (vacuum)locum_addere(dp, barra, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, II);
    (vacuum)componentem_locare(dp, barra, "genus-homo", PARTES_BOTTONE,
        DISPOSITIO_FIXA, VIII, II);
    (vacuum)componentem_locare(dp, barra, "genus-societas",
        PARTES_BOTTONE, DISPOSITIO_FIXA, X, II);
    corpus   = locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    superum  = locum_addere(dp, corpus, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_PARS, L);
    campi_d  = locum_addere(dp, superum, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    per (i = ZEPHYRUM; campi && i < xar_numerus(campi); i++)
    {
        sprintf(id, "f%d", (integer)i);
        /* P6c: natalis - linea cum sagitta "ad_natalem" ad dextrum */
        si (   ((CampusPersonae*)xar_obtinere(campi, i))->genus
               == CAMPUS_NATALIS && natalis_aperibilis(m))
        {
            s32 linea;

            linea = locum_addere(dp, campi_d, DISPOSITIO_LINEA,
                DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, II);
            (vacuum)componentem_locare(dp, linea, id, PARTES_ITEM,
                DISPOSITIO_CRESCENS, ZEPHYRUM, II);
            (vacuum)componentem_locare(dp, linea, "ad_natalem",
                PARTES_BOTTONE, DISPOSITIO_FIXA, III, II);
            perge;
        }
        (vacuum)componentem_locare(dp, campi_d, id, PARTES_ITEM,
            DISPOSITIO_CRESCENS, ZEPHYRUM, i == ZEPHYRUM ? III : II);
    }
    si (prompta_aperta(&m->ramus, NIHIL))
    {
        (vacuum)locum_addere(dp, campi_d, DISPOSITIO_LINEA,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS,
            ZEPHYRUM);
        (vacuum)componentem_locare(dp, campi_d, "prompta",
            PARTES_CAMPUS,
            DISPOSITIO_CRESCENS, ZEPHYRUM, NOMENCLATOR_ORDO);
    }
    /* P7: imago ~1/6 superficiei (tertia latitudinis, dimidium
     * altitudinis) */
    (vacuum)componentem_locare(dp, superum, "imago", PARTES_NULLUM,
        DISPOSITIO_FIXA, columnae / III, -I);
    /* P5b: notae (liber scribae) */
    (vacuum)componentem_locare(dp, corpus, "notae", PARTES_NULLUM,
        DISPOSITIO_CRESCENS, ZEPHYRUM, -I);
}

/* visus indicis: titulus cum segmentis, caput, regio ordinum,
 * prompta */
interior vacuum
indicem_disponere (
    NomenclatorMontatio* m,
              Disponens* dp,
                    s32  radix_d,
                Piscina* piscina)
{
                  Album* l;
     constans AlbumOrdo* o;
                    s32  titulus_d;
                    s32  caput;
                    s32  regio;
                    s32  a;
                    s32  b;
                    s32  t;
                    s32  i;

    /* P3: titulus et segmenta visus (All | Tags) ad dextrum */
    titulus_d = locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        NOMENCLATOR_TITULI);
    (vacuum)componentem_locare(dp, titulus_d, "titulus",
        PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, NOMENCLATOR_TITULI);
    (vacuum)componentem_locare(dp, titulus_d, "visus-indicis",
        PARTES_BOTTONE, DISPOSITIO_FIXA, V, II);
    (vacuum)componentem_locare(dp, titulus_d, "visus-rubricarum",
        PARTES_BOTTONE, DISPOSITIO_FIXA, VI, II);
    caput = locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(dp, caput, "nova", PARTES_BOTTONE,
        DISPOSITIO_FIXA, VII, NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(dp, caput, "caput", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(dp, caput, "pagina-prior",
        PARTES_BOTTONE,
        DISPOSITIO_FIXA, III, NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(dp, caput, "index_paginae",
        PARTES_INDEX,
        DISPOSITIO_FIXA, VII, NOMENCLATOR_CAPUT);
    (vacuum)componentem_locare(dp, caput, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, NOMENCLATOR_CAPUT);
    regio = locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    l = ordines(m, piscina);
    si (l)
    {
        album_pagina(l, pagina_currens(m, l), &a, &b, &t);
        /* pagina in medio litterae: titulus iteratus ("<id>-c") */
        si (t >= ZEPHYRUM)
        {
            o = album_ordo(l, (i32)t);
            (vacuum)componentem_locare(dp, regio, chorda_ut_cstr(
                chorda_concatenare(o->id, chorda_ex_literis("-c",
                piscina), piscina), piscina), PARTES_INDEX,
                DISPOSITIO_CRESCENS, ZEPHYRUM, o->altitudo);
        }
        per (i = a; i < b; i++)
        {
            o = album_ordo(l, (i32)i);
            (vacuum)componentem_locare(dp, regio, chorda_ut_cstr(o->id,
                piscina), o->genus == ALBUM_RES ? PARTES_ITEM
                : PARTES_INDEX, DISPOSITIO_CRESCENS, ZEPHYRUM,
                o->altitudo);
        }
    }
    /* P4: quaestio sine fructibus */
    {
        character quaestio[PROMPTA_MAXIMA];

        si (   l && album_numerus(l) == ZEPHYRUM
            && quaestio_currens(m, quaestio) > ZEPHYRUM)
        {
            (vacuum)componentem_locare(dp, regio, "nihil_inventum",
                PARTES_INDEX, DISPOSITIO_CRESCENS, ZEPHYRUM,
                NOMENCLATOR_ORDO);
        }
    }
    /* P2b: prompta in linea ultima regionis indicis */
    si (prompta_aperta(&m->ramus, NIHIL))
    {
        (vacuum)locum_addere(dp, regio, DISPOSITIO_LINEA,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS,
            ZEPHYRUM);
        (vacuum)componentem_locare(dp, regio, "prompta", PARTES_CAMPUS,
            DISPOSITIO_CRESCENS, ZEPHYRUM, NOMENCLATOR_ORDO);
    }
}

/* P5a */
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
                           s32  latitudo;
                           s32  altitudo;
                           s32  columnae;
                           s32  lineae;
                           s32  radix_d;
                           s32  i;
                         Fines  f;

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
    si (persona_aperta(m) > ZEPHYRUM)
    {
        personam_disponere(m, &dp, radix_d, columnae, piscina);
    }
    alioquin
    {
        indicem_disponere(m, &dp, radix_d, piscina);
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
    /* P5b: notae in loco "notae" (visus personae) */
    si (persona_aperta(m) > ZEPHYRUM && m->figurae_ctx)
    {
        Componens* locus;

        locus = componens_invenire_per_id(radix, chorda_ex_literis(
            "notae", piscina));
        si (locus)
        {
            charta_componere(&((NomenclatorFigurae*)m->figurae_ctx)
                ->notae, repo, motus, piscina, intern, radix,
                locus->fines);
        }
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

/* P5b: locus notarum ex superficie (ante dispositionem): dimidium
 * inferum corporis visus personae - radix (spatium I), barra (III),
 * superum PARS L (pavimentum) - ut personam_disponere */
interior vacuum
notarum_locus (
    constans NomenclatorMontatio* m,
                             s32* latitudo,
                             s32* altitudo)
{
    s32 corpus;

    corpus     = superficies(m, FALSUM) / NOMENCLATOR_LINEA - II - III;
    *latitudo  = (superficies(m, VERUM) / NOMENCLATOR_LITTERA - II)
               * NOMENCLATOR_LITTERA;
    *altitudo  = (corpus - corpus * L / C) * NOMENCLATOR_LINEA;
}

/* P5b: clavis libri personae: "persona/<id>" (buffer XXXII) */
interior vacuum
clavis_libri (
    constans NomenclatorMontatio* m,
                             s32  id,
                       character* clavis)
{
    character numerus[XXXII];

    sprintf(clavis, "persona/%s", id > ZEPHYRUM ? id_textus(m, id,
        numerus) : "0");
}

/* P5b: liber personam apertam sequitur (index librum servat); gestus
 * pendens prius effunditur */
interior vacuum
librum_aptare (
    NomenclatorMontatio* m,
                  Motus* motus)
{
    NomenclatorFigurae* ctx;
             character  clavis[XLVIII];
                   s32  lat;
                   s32  alt;

    ctx = m ? (NomenclatorFigurae*)m->figurae_ctx : NIHIL;
    si (!ctx || !ctx->notae.montata || persona_aperta(m) <= ZEPHYRUM)
    {
        redde;
    }
    clavis_libri(m, persona_aperta(m), clavis);
    notarum_locus(m, &lat, &alt);
    (vacuum)charta_clavem_ponere(&ctx->notae, motus, clavis, lat, alt);
}

vacuum
nomenclator_portam_diei_ponere (
       NomenclatorMontatio* m,
    NomenclatorDiemAperire  porta,
                    vacuum* ctx)
{
    si (m)
    {
        m->dies_aperire      = porta;
        m->dies_aperire_ctx  = ctx;
    }
}

/* D5 S3c: pagina notarum semitae (campus 'pagina' scribae
 * nidificatae) ante librum aptatum legitur, post eum ostenditur -
 * praevalet paginae visae ultimae */
interior vacuum
librum_aptare_cum_pagina (
    NomenclatorMontatio* m)
{
    NomenclatorFigurae* ctx;
                chorda* a;
                chorda  petita;

    ctx             = m ? (NomenclatorFigurae*)m->figurae_ctx : NIHIL;
    petita.datum    = NIHIL;
    petita.mensura  = ZEPHYRUM;
    si (ctx && ctx->notae.montata)
    {
        a = insula_ramus_attributum(&ctx->notae.scriba.ramus, "pagina");
        si (a && a->mensura > ZEPHYRUM)
        {
            petita = chorda_transcribere(*a, m->piscina);
        }
    }
    librum_aptare(m, NIHIL);
    si (ctx && ctx->notae.montata && petita.mensura > ZEPHYRUM)
    {
        (vacuum)scriba_paginam_ostendere(&ctx->notae.scriba, petita);
    }
}

/* semitae R4b: SemitaOrganum lateris - manifestum suum (liberi
 * nulli); ctx = NomenclatorMontatio* */
interior constans Organum*
organa_nomenclatoris (
     vacuum* ctx,
     chorda  genus)
{
    NomenclatorMontatio* m = (NomenclatorMontatio*)ctx;

    si (!m || !m->figurae_ctx)
    {
        redde NIHIL;
    }
    /* D5 S3c: montatio 'notae' (genus scriba) */
    si (chorda_aequalis_literis(genus, "scriba"))
    {
        redde ((NomenclatorFigurae*)m->figurae_ctx)->organum_scribae;
    }
    redde chorda_aequalis_literis(genus, "nomenclator")
        ? ((NomenclatorFigurae*)m->figurae_ctx)->organum : NIHIL;
}

b32
nomenclator_argumentum_ponere (
     NomenclatorMontatio* m,
      constans character* argumentum)
{
     StmlNodus* semita;
        chorda* aperta;
        chorda  vitium;

    si (!m)
    {
        redde FALSUM;
    }
    /* semitae R4e: URI aut cognomen manifesti ("" index, "<id>") */
    semita = semita_ex_argumento(organa_nomenclatoris, m, "nomenclator",
        argumentum ? argumentum : "", m->piscina, m->intern, &vitium);
    si (!semita)
    {
        redde FALSUM;
    }
    /* persona ignota: recusatur ante scripturam (typus nomen formam
     * solam iudicat) */
    aperta = stml_attributum_capere(semita, "persona_aperta");
    si (   aperta && aperta->mensura > ZEPHYRUM
        && !personae_persona(m->documentum, *aperta))
    {
        redde FALSUM;
    }
    si (!semita_applicare(organa_nomenclatoris, m, &m->ramus, semita,
            m->piscina, &vitium))
    {
        redde FALSUM;
    }
    librum_aptare_cum_pagina(m);
    redde VERUM;
}

b32
nomenclator_quaestionem_aperire (
    vacuum* ctx)
{
    ContextusPraecepti cp;

    memset(&cp, ZEPHYRUM, magnitudo(cp));
    cp.m = (NomenclatorMontatio*)ctx;
    cp.p = cp.m ? cp.m->piscina : NIHIL;
    redde cp.m && fac_quaestionem(&cp);
}

chorda
nomenclator_locus_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
     NomenclatorMontatio* m;
        constans Persona* p;
                  chorda* a;

    m = (NomenclatorMontatio*)ctx;
    a = m ? insula_ramus_attributum(&m->ramus,
        "persona_aperta") : NIHIL;
    p = (a && a->mensura > ZEPHYRUM && m->documentum)
        ? personae_persona(m->documentum, *a) : NIHIL;
    redde p ? persona_nomen(p, piscina)
            : chorda_ex_literis("Contacts", piscina);
}

b32
nomenclator_semitam_secuta (
    vacuum* ctx)
{
    NomenclatorMontatio* m = (NomenclatorMontatio*)ctx;

    si (!m)
    {
        redde FALSUM;
    }
    librum_aptare_cum_pagina(m);
    redde VERUM;
}

b32
nomenclator_reficere (
    NomenclatorMontatio* m)
{
    NomenclatorFigurae* ctx;

    ctx = m ? (NomenclatorFigurae*)m->figurae_ctx : NIHIL;
    redde ctx && charta_reficere(&ctx->notae);
}

vacuum
nomenclator_gestum_ponere (
     Motus* motus,
    vacuum* ctx)
{
    NomenclatorMontatio* m;

    m = (NomenclatorMontatio*)ctx;
    si (m && m->figurae_ctx)
    {
        charta_gestum_ponere(&((NomenclatorFigurae*)m->figurae_ctx)
            ->notae, motus);
    }
}

/* <actio/> agere et liber notarum post actum quemque (P5b) */
interior b32
agere_et_aptare (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    b32 r;

    r = agere(repo, motus, des, nodus, ev, ctx);
    librum_aptare((NomenclatorMontatio*)ctx, motus);
    redde r;
}

b32
nomenclator_montare (
     NomenclatorMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
      PersonaeDocumentum* documentum,
                 Volumen* volumen,
        NomenclatorHodie  hodie,
                  vacuum* hodie_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata)
{
               Organum* organum;
    NomenclatorFigurae* ctx;
             character  titulus[LXIV];
             character  clavis[XLVIII];
                   s32  lat;
                   s32  alt;
                   s32  persona;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(NomenclatorMontatio));
    m->piscina     = piscina;
    m->intern      = intern;
    m->documentum  = documentum;
    m->hodie       = hodie;
    m->hodie_ctx   = hodie_ctx;
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
    si (   !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !elenchi_verificare(TABULA_ELENCHORUM, TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_nomenclatoris,
               agere_et_aptare, m)
        || !figuras_registrare(m, latitudo, altitudo))
    {
        redde FALSUM;
    }
    ((NomenclatorFigurae*)m->figurae_ctx)->organum = organum;
    ((NomenclatorFigurae*)m->figurae_ctx)->organum_scribae =
        organum_legere_genus(piscina, intern, "scriba", "nomenclator");
    /* P5b: notae - liber personae apertae, aliter electae (index:
     * liber paratus ad Enter), aliter "persona/0" (index vacuus) */
    ctx      = (NomenclatorFigurae*)m->figurae_ctx;
    persona  = persona_aperta(m) > ZEPHYRUM ? persona_aperta(m)
             : electa_ordo(m);
    sprintf(titulus, "%s_notae", id ? id : "nomenclator");
    clavis_libri(m, persona, clavis);
    notarum_locus(m, &lat, &alt);
    redde charta_montare(&ctx->notae, piscina, intern, volumen,
        &m->ramus, m->actiones, m->figurae, titulus, clavis, lat, alt,
        systemata);
}
