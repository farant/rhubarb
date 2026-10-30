/* navigator.c - navigator in latere vici (include/navigator.h). Forma
 * lectoris (lib/lector.c): arbor dispositionis in cellulis (LITTERA x
 * LINEA), figura radicis omnia pingit, praecepta per
 * membrum__praecepta, index per membrum__album (titulus gregis =
 * hospes). Articulus in arca: HTML versionis -> adaptator situs ->
 * navigator_paginae -> lector_paginare, per (articulus, versio,
 * area). */

#include "navigator.h"
#include "membrum__album.h"
#include "membrum__praecepta.h"
#include "membrum__prompta.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "color.h"
#include "coloratio.h"
#include "motus.h"
#include "lector_paginae.h"
#include "navigator_paginae.h"
#include "vicipaedia.h"
#include "semita.h"

#include <stdio.h>
#include <string.h>

#define NAV_LATITUDO  CCC      /* superficies absens */
#define NAV_ALTITUDO  CCLVI
#define NAV_LITTERA   VI
#define NAV_LINEA     VIII
#define NAV_TITULI    III      /* lineae tituli */
#define NAV_CAPUT     II       /* lineae capitis (paginae indicis) */
#define NAV_ORDO      II       /* lineae ordinis et tituli gregis */

#define VISUS_INDICIS  I
#define VISUS_PAGINAE  II
#define VISUS_EVENTUUM IV       /* N3.3b: eventus quaestionis */
#define VISUS_RADICIS  VIII     /* N3.4: index sine dominio (radix) */
#define VISUS_SENTENTIAE XVI    /* N6.1: sententia sola */
#define REQUIRIT_ELECTUM  I

hic_manens constans character actio_navigatoris[] = "navigator.agere";

/* figura: contextus privatus */
nomen structura {
    NavigatorMontatio* m;
     constans Organum* organum;
              Piscina* scheda;
                  s32  latitudo_montationis;
                  s32  altitudo_montationis;
    /* cache articuli currentis: (articulus, versio, area) -> textus,
     * paginae (arca vacatur cum mutantur) */
              Piscina* arca;
               chorda  articulus_arcae;
               chorda  versio_arcae;
                  s32  latitudo_arcae;
                  s32  altitudo_arcae;
      NavigatorTextus* textus;
                  Xar* paginae;          /* LectorPagina: articulus,
                                          * deinde index nexuum */
                  i32 paginae_articuli; /* priores ex caput.textus */
               chorda titulus;          /* titulus versionis */
               chorda hospes;
    /* N3.3: petitio pendens (una); finitae pulsu proximo dimittuntur */
     NavigatorPetitio* petitio;
                  Xar* dimittendae;      /* NavigatorPetitio* */
                  b32  mutatum;          /* petitio finita: pingere */
    /* N3.3b: eventus quaestionis ultimae (copia, arca sua) */
              Piscina* arca_eventuum;
                  Xar* eventus;          /* VicipaediaEventus */
    /* N6.1: sententiae textus articuli (CodexSententia), in arca */
                  Xar* sententiae;
    /* N3.5: columnae laterales - paginae (Xar* LectorPagina) et pagina
     * possessoris (s32) per columnam textus->columnae, in arca */
                  Xar* columnae_paginae;
                  Xar* columnae_possessores;
} NavigatorFigurae;


/* ==================================================
 * Status (insula)
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

interior chorda
_campus (
    constans NavigatorMontatio* m,
            constans character* titulus)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, titulus);
    redde a ? *a : _vacua();
}

nomen structura {
    constans character* constans* tituli;
    constans character* constans* valores;
                             i32 numerus;
} GrexCamporum;

interior vacuum
_campos_ponere (
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
        si (!grex->valores[i] || !grex->valores[i][ZEPHYRUM])
        {
            (vacuum)insula_attributum_tollere(radix, grex->tituli[i]);
        }
        alioquin
        {
            insula_attributum_ponere(radix, p, in, grex->tituli[i],
                grex->valores[i]);
        }
    }
}

/* campi actu uno, scriptore navigatoris ("" aut NIHIL tollit) */
interior b32
_campos_scribere (
             NavigatorMontatio* m,
    constans character* constans* tituli,
    constans character* constans* valores,
                             i32  numerus)
{
    GrexCamporum grex;
          chorda prior;
             b32 ok;

    grex.tituli   = tituli;
    grex.valores  = valores;
    grex.numerus  = numerus;
    prior         = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_navigatoris, m->piscina));
    ok = mutare_ramum(&m->ramus, _campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior s32
_superficies (
    constans NavigatorMontatio* m,
                           b32  latitudo)
{
    constans NavigatorFigurae* f;
                          s32  v;
                       chorda  a;
                          s32  n;

    f = (constans NavigatorFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? f->latitudo_montationis
                                 : f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? NAV_LATITUDO : NAV_ALTITUDO;
    }
    a = _campus(m, latitudo ? "superficies_latitudo"
        : "superficies_altitudo");
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &n) && n > ZEPHYRUM)
        ? n : v;
}

interior i32
_visus (
    constans NavigatorMontatio* m)
{
    chorda v = _campus(m, "visus");

    redde chorda_aequalis_literis(v, "pagina") ? VISUS_PAGINAE
        : chorda_aequalis_literis(v, "eventus") ? VISUS_EVENTUUM
        : chorda_aequalis_literis(v, "sententia") ? VISUS_SENTENTIAE
        : _campus(m, "dominium").mensura == ZEPHYRUM ? VISUS_RADICIS
        : VISUS_INDICIS;
}

/* campus numerus (>= 0); -1 si abest aut malus */
interior s32
_campus_numerus (
    constans NavigatorMontatio* m,
            constans character* titulus)
{
    chorda a;
       s32 n;

    a = _campus(m, titulus);
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &n)
        && n >= ZEPHYRUM) ? n : -I;
}

/* margines paginae ut lector (cellulae): lateris X, supra et infra VI;
 * in latere angusto minuuntur (lateris et supra saltem I) */
interior vacuum
_margines (
    constans NavigatorMontatio* m,
                           s32* lateris,
                           s32* supra)
{
    s32 columnae;
    s32 lineae;

    columnae  = _superficies(m, VERUM) / NAV_LITTERA - II;
    lineae    = _superficies(m, FALSUM) / NAV_LINEA - II;
    *lateris  = (columnae - XL) / II;
    *supra    = (lineae - XX) / II;
    si (*lateris > X)
    {
        *lateris = X;
    }
    si (*lateris < I)
    {
        *lateris = I;
    }
    si (*supra > VI)
    {
        *supra = VI;
    }
    si (*supra < I)
    {
        *supra = I;
    }
}

/* area textus paginae (px) - eadem in dispositione et pictura */
interior vacuum
_area (
    constans NavigatorMontatio* m,
                           s32* latitudo,
                           s32* altitudo)
{
    s32 lateris;
    s32 supra;

    _margines(m, &lateris, &supra);
    *latitudo  = (_superficies(m, VERUM) / NAV_LITTERA - II
        - II * lateris) * NAV_LITTERA;
    *altitudo  = (_superficies(m, FALSUM) / NAV_LINEA - II
        - II * supra) * NAV_LINEA;
    si (*latitudo < NAV_LITTERA)
    {
        *latitudo = NAV_LITTERA;
    }
    si (*altitudo < NAV_LINEA)
    {
        *altitudo = NAV_LINEA;
    }
}


/* ==================================================
 * Articuli
 * ================================================== */

interior constans NavigatorArticulus*
_articulus_per_id (
    constans NavigatorMontatio* m,
                        chorda  identitas)
{
    i32 i;

    per (i = ZEPHYRUM; identitas.mensura > ZEPHYRUM
        && i < navigator_articulorum_numerus(m->bibliotheca); i++)
    {
        constans NavigatorArticulus* a = navigator_articulus(
            m->bibliotheca, i);

        si (chorda_aequalis(a->identitas, identitas))
        {
            redde a;
        }
    }
    redde NIHIL;
}

/* versio per identitatem; vacua = ultima; NIHIL si ignota */
interior constans NavigatorVersio*
_versio (
    constans NavigatorArticulus* a,
                         chorda  identitas)
{
    i32 i;
    i32 n;

    n = a ? xar_numerus(a->versiones) : ZEPHYRUM;
    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    si (identitas.mensura == ZEPHYRUM)
    {
        redde (constans NavigatorVersio*)xar_obtinere(a->versiones,
            n - I);
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans NavigatorVersio* v = (constans NavigatorVersio*)
            xar_obtinere(a->versiones, i);

        si (chorda_aequalis(v->identitas, identitas))
        {
            redde v;
        }
    }
    redde NIHIL;
}

/* chorda super litteras constantes (sine copia) */
interior chorda
_litterae (
    constans character* s)
{
    chorda c;

    c.datum    = (i8*)(vacuum*)(memoriae_index)s;
    c.mensura  = (i32)strlen(s);
    redde c;
}

/* hospes URL ("https://en.wikipedia.org/wiki/X": "en.wikipedia.org") */
interior chorda
_hospes (
    chorda url)
{
    s32 a;
    s32 b;

    a = chorda_invenire_index(url, _litterae("://"));
    si (a < ZEPHYRUM)
    {
        redde url;
    }
    url  = chorda_sectio(url, (i32)a + III, url.mensura);
    b    = chorda_invenire_index(url, _litterae("/"));
    redde b < ZEPHYRUM ? url : chorda_sectio(url, ZEPHYRUM, (i32)b);
}

/* dominium radicale: duo nomina ultima hospitis ("en.wikipedia.org"
 * -> "wikipedia.org"); co.uk etc. Public Suffix List postea */
interior chorda
_radix_dominii (
    chorda hospes)
{
    i32 i;
    i32 puncta;

    puncta = ZEPHYRUM;
    per (i = hospes.mensura; i > ZEPHYRUM; i--)
    {
        si (hospes.datum[i - I] == '.' && ++puncta == II)
        {
            redde chorda_sectio(hospes, i, hospes.mensura);
        }
    }
    redde hospes;
}

/* titulus articuli: versionis ultimae; vacuus -> URL */
interior chorda
_titulus_articuli (
    constans NavigatorArticulus* a)
{
    constans NavigatorVersio* v = _versio(a, _vacua());

    redde v && v->titulus.mensura > ZEPHYRUM ? v->titulus : a->url;
}

/* adaptator situs (v1: Vicipaedia sola) */
interior Articulus*
_extrahere (
                         chorda  situs,
                         chorda  html,
    constans NavigatorArticulus* a,
                        Piscina* p)
{
    si (chorda_aequalis_literis(situs, "vicipaedia"))
    {
        redde vicipaedia_articulus(html, chorda_ut_cstr(a->url, p), p);
    }
    redde NIHIL;
}

/* articulus currens paginatus (campi articulus, versio) in arca */
interior NavigatorFigurae*
_paginas_parare (
    NavigatorMontatio* m)
{
               NavigatorFigurae* f;
    constans NavigatorArticulus* a;
       constans NavigatorVersio* v;
                         chorda  html;
                      Articulus* articulus;
                            s32  latitudo;
                            s32  altitudo;

    f = (NavigatorFigurae*)m->figurae_ctx;
    a = _articulus_per_id(m, _campus(m, "articulus"));
    v = _versio(a, _campus(m, "versio"));
    si (!f || !a || !v)
    {
        redde NIHIL;
    }
    _area(m, &latitudo, &altitudo);
    si (   f->textus && f->latitudo_arcae == latitudo
        && f->altitudo_arcae == altitudo
        && chorda_aequalis(f->articulus_arcae, a->identitas)
        && chorda_aequalis(f->versio_arcae, v->identitas))
    {
        redde f;
    }
    piscina_vacare(f->arca);
    f->textus = NIHIL;
    f->paginae = NIHIL;
    f->sententiae = NIHIL;
    f->columnae_paginae = NIHIL;
    f->columnae_possessores = NIHIL;
    html = navigator_versionis_html(m->bibliotheca, v, f->arca);
    articulus = html.mensura > ZEPHYRUM ? _extrahere(a->situs, html, a,
        f->arca) : NIHIL;
    f->textus = articulus ? navigator_textum_facere(articulus, f->arca)
        : NIHIL;
    f->paginae = f->textus ? lector_paginare(&f->textus->caput,
        (i32)latitudo, (i32)altitudo, f->arca) : NIHIL;
    si (!f->paginae || xar_numerus(f->paginae) == ZEPHYRUM)
    {
        f->textus = NIHIL;
        redde NIHIL;
    }
    /* index nexuum (N3.1b): paginae novae post articulum */
    f->paginae_articuli = xar_numerus(f->paginae);
    /* N6.1: sententiae articuli (index nexuum non legitur) */
    f->sententiae = codex_sententiae(&f->textus->caput, NIHIL, ZEPHYRUM,
        f->arca);
    /* N3.5: columnae laterales eadem area paginatae; possessor =
     * pagina articuli quae ancoram continet */
    f->columnae_paginae = xar_creare(f->arca,
        (i32)magnitudo(Xar*));
    f->columnae_possessores = xar_creare(f->arca, (i32)magnitudo(s32));
    {
        i32 c;
        i32 q;

        per (c = ZEPHYRUM; f->textus->columnae && f->columnae_paginae
            && f->columnae_possessores
            && c < xar_numerus(f->textus->columnae); c++)
        {
            constans NavigatorColumna* col =
                (constans NavigatorColumna*)
                xar_obtinere(f->textus->columnae, c);
                                Xar** pg  = (Xar**)xar_addere(
                                    f->columnae_paginae);
                                 s32* pos = (s32*)xar_addere(
                                     f->columnae_possessores);

            si (!pg || !pos)
            {
                frange;
            }
            *pg   = lector_paginare(&col->caput, (i32)latitudo,
                (i32)altitudo, f->arca);
            *pos  = ZEPHYRUM;
            per (q = I; q < f->paginae_articuli; q++)
            {
                si (((constans LectorPagina*)xar_obtinere(f->paginae,
                    q))
                    ->initium > col->ancora)
                {
                    frange;
                }
                *pos = (s32)q;
            }
        }
    }
    si (f->textus->nexus.textus.mensura > ZEPHYRUM)
    {
        Xar* nexus = lector_paginare(&f->textus->nexus, (i32)latitudo,
            (i32)altitudo, f->arca);
        i32 i;

        per (i = ZEPHYRUM; nexus && i < xar_numerus(nexus); i++)
        {
            LectorPagina* g = (LectorPagina*)xar_addere(f->paginae);

            si (g)
            {
                *g = *(LectorPagina*)xar_obtinere(nexus, i);
            }
        }
    }
    f->articulus_arcae  = chorda_transcribere(a->identitas, f->arca);
    f->versio_arcae     = chorda_transcribere(v->identitas, f->arca);
    f->titulus         = chorda_transcribere(v->titulus.mensura
        > ZEPHYRUM ? v->titulus : a->url, f->arca);
    f->hospes          = chorda_transcribere(_hospes(a->url), f->arca);
    f->latitudo_arcae  = latitudo;
    f->altitudo_arcae  = altitudo;
    redde f;
}

/* pagina currens coacta ad [0, n-1] */
interior s32
_pagina_currens (
      constans NavigatorMontatio* m,
       constans NavigatorFigurae* f)
{
    s32 k;

    k = _campus_numerus(m, "pagina");
    si (k < ZEPHYRUM)
    {
        k = ZEPHYRUM;
    }
    si ((i32)k >= xar_numerus(f->paginae))
    {
        k = (s32)xar_numerus(f->paginae) - I;
    }
    redde k;
}

/* ===== positio lectionis (N3.4c) ===== */

/* positio paginae k articuli currentis servatur: versio, octetus
 * primus, in indice nexuum? */
interior vacuum
_positionem_servare (
            NavigatorMontatio* m,
    constans NavigatorFigurae* f,
                          s32  k)
{
    NavigatorPositio q;

    si (k < ZEPHYRUM || (i32)k >= xar_numerus(f->paginae))
    {
        redde;
    }
    q.versio      = f->versio_arcae;
    q.offset      = ((constans LectorPagina*)xar_obtinere(f->paginae,
        (i32)k))->initium;
    q.in_nexibus  = (i32)k >= f->paginae_articuli;
    (vacuum)navigator_positionem_scribere(m->bibliotheca,
        f->articulus_arcae, &q);
}

/* pagina positionem servatam continens (initium <= offset, in parte
 * sua: articulus aut index nexuum); versio alia aut nulla: 0 */
interior s32
_pagina_servata (
            NavigatorMontatio* m,
    constans NavigatorFigurae* f)
{
    NavigatorPositio q;
                 i32 a;
                 i32 b;
                 i32 i;
                 s32 k;

    si (   !navigator_positionem_legere(m->bibliotheca,
               f->articulus_arcae, &q, f->scheda)
        || !chorda_aequalis(q.versio, f->versio_arcae))
    {
        redde ZEPHYRUM;
    }
    a = q.in_nexibus ? f->paginae_articuli : ZEPHYRUM;
    b = q.in_nexibus ? xar_numerus(f->paginae) : f->paginae_articuli;
    si (a >= b)
    {
        redde ZEPHYRUM;
    }
    k = (s32)a;
    per (i = a + I; i < b; i++)
    {
        si (((constans LectorPagina*)xar_obtinere(f->paginae, i))
            ->initium > q.offset)
        {
            frange;
        }
        k = (s32)i;
    }
    redde k;
}


/* ==================================================
 * Index (ordo, album, electus)
 * ================================================== */

/* a ante b: hospes, deinde titulus (sine casu ASCII) */
interior b32
_praecedit (
    constans NavigatorArticulus* a,
    constans NavigatorArticulus* b)
{
    chorda x;
    chorda y;
       i32 i;
       s32 d;

    x = _hospes(a->url);
    y = _hospes(b->url);
    d = chorda_comparare(x, y);
    si (d != ZEPHYRUM)
    {
        redde d < ZEPHYRUM;
    }
    x = _titulus_articuli(a);
    y = _titulus_articuli(b);
    per (i = ZEPHYRUM; i < x.mensura && i < y.mensura; i++)
    {
        character p = (character)x.datum[i];
        character q = (character)y.datum[i];

        si (p >= 'A' && p <= 'Z')
        {
            p = (character)(p - 'A' + 'a');
        }
        si (q >= 'A' && q <= 'Z')
        {
            q = (character)(q - 'A' + 'a');
        }
        si (p != q)
        {
            redde (insignatus character)p < (insignatus character)q;
        }
    }
    redde x.mensura < y.mensura;
}

/* indices bibliothecae ordine indicis (Xar de i32) - N3.4: favores
 * dominii currentis soli */
interior Xar*
_ordo (
    constans NavigatorMontatio* m,
                       Piscina* p)
{
       Xar* o;
    chorda  dominium;
       i32  n;
       i32  i;
       i32  j;

    n         = navigator_articulorum_numerus(m->bibliotheca);
    o         = xar_creare(p, (i32)magnitudo(i32));
    dominium  = _campus(m, "dominium");
    per (i = ZEPHYRUM; o && i < n; i++)
    {
        constans NavigatorArticulus* a = navigator_articulus(
            m->bibliotheca, i);
                                i32* e;

        si (   !a->favor
            || !chorda_aequalis(_radix_dominii(_hospes(a->url)),
                   dominium))
        {
            perge;
        }
        e = (i32*)xar_addere(o);
        si (!e)
        {
            redde o;
        }
        *e = i;
        /* insertio: elementum novum retro movetur */
        per (j = xar_numerus(o) - I; j > ZEPHYRUM; j--)
        {
            i32* prior  = (i32*)xar_obtinere(o, j - I);
            i32* hic    = (i32*)xar_obtinere(o, j);
            i32  t;

            si (!_praecedit(navigator_articulus(m->bibliotheca, *hic),
                navigator_articulus(m->bibliotheca, *prior)))
            {
                frange;
            }
            t       = *prior;
            *prior  = *hic;
            *hic    = t;
        }
    }
    redde o;
}

interior i32
_index_ordinis (
    Xar* ordo,
    s32  positio)
{
    redde *(i32*)xar_obtinere(ordo, (i32)positio);
}

/* capacitas in cellulis: altitudo minus margines, titulus, caput,
 * linea notae et prompta si adsunt (N3.3) */
interior s32
_capacitas (
    constans NavigatorMontatio* m)
{
    redde _superficies(m, FALSUM) / NAV_LINEA - II - NAV_TITULI
        - NAV_CAPUT
        - (_campus(m, "nota").mensura > ZEPHYRUM ? NAV_CAPUT : ZEPHYRUM)
        - (prompta_aperta(&m->ramus, NIHIL) ? NAV_CAPUT : ZEPHYRUM);
}

/* album: grex per hospitem ("g<positio primi>"), res = index + 1
 * ("r<res>") */
interior Album*
_album (
    constans NavigatorMontatio* m,
                           Xar* ordo,
                       Piscina* p)
{
     Album* l;
    chorda  prior = _vacua();
       i32  k;
 character  id[XVI];

    l = album_creare(p, _capacitas(m));
    per (k = ZEPHYRUM; l && k < xar_numerus(ordo); k++)
    {
           i32 i = _index_ordinis(ordo, (s32)k);
        chorda h = _hospes(navigator_articulus(m->bibliotheca, i)->url);

        si (k == ZEPHYRUM || !chorda_aequalis(h, prior))
        {
            sprintf(id, "g%d", (integer)k);
            (vacuum)album_addere(l, id, ALBUM_TITULUS, NAV_ORDO,
                ZEPHYRUM);
            prior = h;
        }
        sprintf(id, "r%d", (integer)(i + I));
        (vacuum)album_addere(l, id, ALBUM_RES, NAV_ORDO, (s32)(i + I));
    }
    redde l;
}

/* positio electi in ordine; -1 si bibliotheca vacua */
interior s32
_electus (
    constans NavigatorMontatio* m,
                           Xar* ordo)
{
    chorda e;
       i32 k;

    si (xar_numerus(ordo) == ZEPHYRUM)
    {
        redde -I;
    }
    e = _campus(m, "electus_articulus");
    per (k = ZEPHYRUM; e.mensura > ZEPHYRUM && k < xar_numerus(ordo);
         k++)
    {
        si (chorda_aequalis(navigator_articulus(m->bibliotheca,
            _index_ordinis(ordo, (s32)k))->identitas, e))
        {
            redde (s32)k;
        }
    }
    redde ZEPHYRUM;
}

/* positio in ordine indicis bibliothecae i; -1 si abest */
interior s32
_positio_indicis (
    Xar* ordo,
    i32  index)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(ordo); k++)
    {
        si (_index_ordinis(ordo, (s32)k) == index)
        {
            redde (s32)k;
        }
    }
    redde -I;
}

interior s32
_pagina_indicis (
    Album* l,
      Xar* ordo,
      s32  electus)
{
    s32 k;

    k = electus >= ZEPHYRUM ? album_pagina_rei(l, (s32)_index_ordinis(
        ordo, electus) + I, ZEPHYRUM) : ZEPHYRUM;
    redde k < ZEPHYRUM ? ZEPHYRUM : k;
}

/* ===== radix (N3.4): dominia favorum ===== */

nomen structura {
    chorda titulus;     /* "wikipedia.org" */
       i32 numerus;     /* favores */
} DominiumFavorum;

/* dominia favorum ordine litterarum, cum numeris */
interior Xar*
_dominia (
    constans NavigatorMontatio* m,
                       Piscina* p)
{
                Xar* o;
    DominiumFavorum* d;
    DominiumFavorum  t;
             chorda  r;
                i32  i;
                i32  j;

    o = xar_creare(p, (i32)magnitudo(DominiumFavorum));
    per (i = ZEPHYRUM; o && i < navigator_articulorum_numerus(
        m->bibliotheca); i++)
    {
        constans NavigatorArticulus* a = navigator_articulus(
            m->bibliotheca, i);

        si (!a->favor)
        {
            perge;
        }
        r = _radix_dominii(_hospes(a->url));
        per (j = ZEPHYRUM; j < xar_numerus(o); j++)
        {
            d = (DominiumFavorum*)xar_obtinere(o, j);
            si (chorda_aequalis(d->titulus, r))
            {
                frange;
            }
        }
        si (j < xar_numerus(o))
        {
            d->numerus++;
            perge;
        }
        d = (DominiumFavorum*)xar_addere(o);
        si (!d)
        {
            redde o;
        }
        d->titulus = r;
        d->numerus = I;
        per (j = xar_numerus(o) - I; j > ZEPHYRUM; j--)
        {
            DominiumFavorum* prior = (DominiumFavorum*)xar_obtinere(o,
                j - I);
            DominiumFavorum* hic = (DominiumFavorum*)xar_obtinere(o, j);

            si (chorda_comparare(hic->titulus, prior->titulus)
                >= ZEPHYRUM)
            {
                frange;
            }
            t       = *prior;
            *prior  = *hic;
            *hic    = t;
        }
    }
    redde o;
}

interior constans DominiumFavorum*
_dominium (
    Xar* dominia,
    s32  k)
{
    redde (constans DominiumFavorum*)xar_obtinere(dominia, (i32)k);
}

/* radix (N3.4b): ordines = dominia favorum, deinde recentes; positio
 * e in [0, dominia) dominium, in [dominia, summa) recens */
nomen structura {
    Xar* dominia;    /* DominiumFavorum */
    Xar* recentes;   /* constans NavigatorArticulus* */
} RadixIndicis;

interior vacuum
_radicem_parare (
    constans NavigatorMontatio* m,
                       Piscina* p,
                  RadixIndicis* r)
{
    r->dominia   = _dominia(m, p);
    r->recentes  = navigator_recentes(m->bibliotheca, p);
}

interior i32
_radicis_numerus (
    constans RadixIndicis* r)
{
    redde (r->dominia ? xar_numerus(r->dominia) : ZEPHYRUM)
        + (r->recentes ? xar_numerus(r->recentes) : ZEPHYRUM);
}

/* recens k (ab 0) */
interior constans NavigatorArticulus*
_recens (
    constans RadixIndicis* r,
                      s32  k)
{
    redde *(constans NavigatorArticulus**)xar_obtinere(r->recentes,
        (i32)k);
}

/* album radicis: grex "Bookmarks" ("g0"; sine favoribus sed cum
 * recentibus nuntius "g2"), "d<k+1>"; grex "Recent" ("g1"),
 * "a<k+1>"; res = positio + 1 */
interior Album*
_radicis_album (
       constans NavigatorMontatio* m,
            constans RadixIndicis* r,
                          Piscina* p)
{
     Album* l;
       i32  k;
       i32  n;
 character  id[XVI];

    l = album_creare(p, _capacitas(m));
    n = xar_numerus(r->dominia);
    si (l && (n > ZEPHYRUM || xar_numerus(r->recentes) > ZEPHYRUM))
    {
        (vacuum)album_addere(l, "g0", ALBUM_TITULUS, NAV_ORDO,
            ZEPHYRUM);
    }
    si (l && n == ZEPHYRUM && xar_numerus(r->recentes) > ZEPHYRUM)
    {
        (vacuum)album_addere(l, "g2", ALBUM_TITULUS, NAV_ORDO,
            ZEPHYRUM);
    }
    per (k = ZEPHYRUM; l && k < n; k++)
    {
        sprintf(id, "d%d", (integer)(k + I));
        (vacuum)album_addere(l, id, ALBUM_RES, NAV_ORDO, (s32)(k + I));
    }
    si (l && xar_numerus(r->recentes) > ZEPHYRUM)
    {
        (vacuum)album_addere(l, "g1", ALBUM_TITULUS, NAV_ORDO,
            ZEPHYRUM);
    }
    per (k = ZEPHYRUM; l && k < xar_numerus(r->recentes); k++)
    {
        sprintf(id, "a%d", (integer)(k + I));
        (vacuum)album_addere(l, id, ALBUM_RES, NAV_ORDO, (s32)(n + k
            + I));
    }
    redde l;
}

/* positio electa (campus "electus" = "d:<dominium>" aut "a:<ULID>");
 * prima si abest; -1 si nulla */
interior s32
_electum_radicis (
       constans NavigatorMontatio* m,
            constans RadixIndicis* r)
{
    chorda e;
       i32 k;
       i32 n;

    si (_radicis_numerus(r) == ZEPHYRUM)
    {
        redde -I;
    }
    e = _campus(m, "electus");
    n = xar_numerus(r->dominia);
    si (e.mensura > II && e.datum[I] == ':' && e.datum[ZEPHYRUM] == 'd')
    {
        e = chorda_sectio(e, II, e.mensura);
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (chorda_aequalis(_dominium(r->dominia, (s32)k)->titulus,
                e))
            {
                redde (s32)k;
            }
        }
    }
    alioquin si (   e.mensura > II && e.datum[I] == ':'
                 && e.datum[ZEPHYRUM] == 'a')
    {
        e = chorda_sectio(e, II, e.mensura);
        per (k = ZEPHYRUM; k < xar_numerus(r->recentes); k++)
        {
            si (chorda_aequalis(_recens(r, (s32)k)->identitas, e))
            {
                redde (s32)(n + k);
            }
        }
    }
    redde ZEPHYRUM;
}

/* album indicis currentis (radix aut dominium); pagina electi et
 * numerus rerum */
interior Album*
_album_indicis (
    constans NavigatorMontatio* m,
                       Piscina* p,
                           s32* pagina,
                           i32* numerus)
{
    Album* l;
      Xar* o;
      s32  e;

    si (_visus(m) == VISUS_RADICIS)
    {
        RadixIndicis r;

        _radicem_parare(m, p, &r);
        l = _radicis_album(m, &r, p);
        e = _electum_radicis(m, &r);
        *pagina    = l && e >= ZEPHYRUM ? album_pagina_rei(l, e + I,
            ZEPHYRUM) : ZEPHYRUM;
        *numerus   = _radicis_numerus(&r);
    }
    alioquin
    {
        o = _ordo(m, p);
        l = _album(m, o, p);
        *pagina    = l ? _pagina_indicis(l, o, _electus(m,
            o)) : ZEPHYRUM;
        *numerus   = o ? xar_numerus(o) : ZEPHYRUM;
    }
    si (*pagina < ZEPHYRUM)
    {
        *pagina = ZEPHYRUM;
    }
    redde l;
}

/* id "r<n>" aut "g<n>" -> n (r: >0; g: >=0); -1 si aliud */
interior s32
_id_numerus (
         chorda id,
      character praefixum)
{
    s32 n;

    si (id.mensura < II || id.datum[ZEPHYRUM] != (i8)praefixum)
    {
        redde -I;
    }
    id.datum++;
    id.mensura--;
    redde chorda_ut_s32(id, &n) && n >= ZEPHYRUM ? n : -I;
}


/* ==================================================
 * Motus
 * ================================================== */

/* N3.4c: pagina servata articuli aperti scribitur et servatur
 * (apertio prima: pagina 0) */
interior b32
_paginam_restituere (
    NavigatorMontatio* m)
{
       NavigatorFigurae* f;
                    s32  k;
     constans character* tituli[I];
     constans character* valores[I];
              character  numerus[XVI];

    f = _paginas_parare(m);
    si (!f)
    {
        redde VERUM;
    }
    k = _pagina_servata(m, f);
    _positionem_servare(m, f, k);
    (vacuum)navigator_recentem_notare(m->bibliotheca,
        f->articulus_arcae);
    si (k == _campus_numerus(m, "pagina"))
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)k);
    tituli[ZEPHYRUM]   = "pagina";
    valores[ZEPHYRUM]  = numerus;
    redde _campos_scribere(m, tituli, valores, I);
}

interior b32
_articulum_aperire (
    NavigatorMontatio* m,
               chorda  identitas,
              Piscina* p)
{
    constans character* tituli[VII];
    constans character* valores[VII];

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "pagina";
    tituli[I]          = "articulus";
    valores[I]         = chorda_ut_cstr(identitas, p);
    /* versio ultima FIGITUR: locus (comes) versionis suae est, etiam
     * cum versio nova postea advenit (spec V.11, V.16) */
    tituli[II]         = "versio";
    valores[II]        = _versio(_articulus_per_id(m, identitas),
        _vacua()) ? chorda_ut_cstr(_versio(_articulus_per_id(m,
        identitas), _vacua())->identitas, p) : "";
    tituli[III]   = "pagina";
    valores[III]  = "0";
    tituli[IV]    = "electus_articulus";
    valores[IV]   = valores[I];
    tituli[V]     = "nota";
    valores[V]    = "";
    tituli[VI]    = "nexus";          /* N4.3: electio tollitur */
    valores[VI]   = "";
    si (!_campos_scribere(m, tituli, valores, VII))
    {
        redde FALSUM;
    }
    redde _paginam_restituere(m);
}

/* N3.5: definitur infra (columnae laterales) */
interior b32
_in_columna_movere (
    NavigatorMontatio* m,
                  s32  gradus,
                  b32* motum);

/* pagina articuli proxima (gradus I) aut prior (-I), coacta */
interior b32
_paginam_movere (
    NavigatorMontatio* m,
                  s32  gradus)
{
        NavigatorFigurae* f;
                     s32  k;
      constans character* tituli[III];
      constans character* valores[III];
               character  numerus[XVI];

    f = _paginas_parare(m);
    si (!f)
    {
        redde VERUM;
    }
    {
        /* N3.5: in columna laterali j k intra eam */
        b32 in_columna;
        b32 r = _in_columna_movere(m, gradus, &in_columna);

        si (in_columna)
        {
            redde r;
        }
    }
    k = _pagina_currens(m, f) + gradus;
    si (k < ZEPHYRUM || (i32)k >= xar_numerus(f->paginae))
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)k);
    tituli[ZEPHYRUM]   = "pagina";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "nexus";     /* N4.3: electio tollitur */
    valores[I]         = "";
    tituli[II]         = "columna";   /* N3.5: alterius paginae */
    valores[II]        = "";
    _positionem_servare(m, f, k);
    redde _campos_scribere(m, tituli, valores, III);
}

/* ===== sententiae (N6.1) ===== */

interior i32
_numerus_sententiarum (
    constans NavigatorFigurae* f)
{
    redde f && f->sententiae ? xar_numerus(f->sententiae) : ZEPHYRUM;
}

interior constans CodexSententia*
_sententia (
    constans NavigatorFigurae* f,
                          s32  k)
{
    redde (constans CodexSententia*)xar_obtinere(f->sententiae,
        (i32)k);
}

/* index sententiae offset continentis (ultima cuius initium <=
 * offset); 0 si ante primam; -1 si nullae */
interior s32
_sententiae_index (
    constans NavigatorFigurae* f,
                          s32  offset)
{
    i32 i;
    s32 k;

    si (_numerus_sententiarum(f) == ZEPHYRUM)
    {
        redde -I;
    }
    k = ZEPHYRUM;
    per (i = I; i < _numerus_sententiarum(f); i++)
    {
        si ((s32)_sententia(f, (s32)i)->initium > offset)
        {
            frange;
        }
        k = (s32)i;
    }
    redde k;
}

/* sententia currens (campus "sententia", coactus) */
interior s32
_sententia_currens (
     constans NavigatorMontatio* m,
      constans NavigatorFigurae* f)
{
    s32 offset = _campus_numerus(m, "sententia");

    redde _sententiae_index(f, offset < ZEPHYRUM ? ZEPHYRUM : offset);
}

/* sententia k scribitur (visus sententiae) et positio servatur */
interior b32
_sententiam_scribere (
            NavigatorMontatio* m,
    constans NavigatorFigurae* f,
                          s32  k)
{
     constans character* tituli[III];
     constans character* valores[III];
              character  numerus[XVI];
       NavigatorPositio  q;

    sprintf(numerus, "%u", (insignatus integer)_sententia(f, k)
        ->initium);
    q.versio      = f->versio_arcae;
    q.offset      = _sententia(f, k)->initium;
    q.in_nexibus  = FALSUM;
    (vacuum)navigator_positionem_scribere(m->bibliotheca,
        f->articulus_arcae, &q);
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "sententia";
    tituli[I]          = "sententia";
    valores[I]         = numerus;
    tituli[II]         = "nexus";
    valores[II]        = "";
    redde _campos_scribere(m, tituli, valores, III);
}

/* s in pagina articuli: sententia prima quae in pagina incipit,
 * aliter quae initium paginae continet; index nexuum: nihil */
interior b32
_sententiam_ex_pagina (
    NavigatorMontatio* m)
{
         NavigatorFigurae* f;
    constans LectorPagina* g;
                      s32  k;
                      s32  a;
                      s32  b;
                      i32  i;

    f = _paginas_parare(m);
    k = f ? _pagina_currens(m, f) : -I;
    si (   !f || (i32)k >= f->paginae_articuli
        || _numerus_sententiarum(f) == ZEPHYRUM)
    {
        redde VERUM;
    }
    g = (constans LectorPagina*)xar_obtinere(f->paginae, (i32)k);
    si (xar_numerus(g->lineae) == ZEPHYRUM)
    {
        redde VERUM;
    }
    a = (s32)((constans LectorLinea*)xar_obtinere(g->lineae, ZEPHYRUM))
        ->initium;
    b = (s32)((constans LectorLinea*)xar_obtinere(g->lineae,
        xar_numerus(g->lineae) - I))->finis;
    per (i = ZEPHYRUM; i < _numerus_sententiarum(f); i++)
    {
        si (   (s32)_sententia(f, (s32)i)->initium >= a
            && (s32)_sententia(f, (s32)i)->initium < b)
        {
            redde _sententiam_scribere(m, f, (s32)i);
        }
    }
    redde _sententiam_scribere(m, f, _sententiae_index(f, a));
}

/* N6.2: restitutio visus sententiae - sententia absens: positio
 * servata (eadem versio, non index nexuum), aliter prima; offset ad
 * initium sententiae continentis coactus; versio figitur */
interior b32
_sententiam_restituere (
            NavigatorMontatio* m,
    constans NavigatorFigurae* f)
{
       NavigatorPositio  q;
                    s32  k;
                    s32  offset;
     constans character* tituli[II];
     constans character* valores[II];
              character  numerus[XVI];

    si (_numerus_sententiarum(f) == ZEPHYRUM)
    {
        redde VERUM;
    }
    offset = _campus_numerus(m, "sententia");
    si (offset < ZEPHYRUM)
    {
        offset = ZEPHYRUM;
        si (   navigator_positionem_legere(m->bibliotheca,
                   f->articulus_arcae, &q, f->scheda)
            && chorda_aequalis(q.versio, f->versio_arcae)
            && !q.in_nexibus)
        {
            offset = (s32)q.offset;
        }
        k = _sententiae_index(f, offset);
        (vacuum)navigator_recentem_notare(m->bibliotheca,
            f->articulus_arcae);
        redde _sententiam_scribere(m, f, k);
    }
    k = _sententiae_index(f, offset);
    si (   (s32)_sententia(f, k)->initium == offset
        && _campus(m, "versio").mensura > ZEPHYRUM)
    {
        redde VERUM;
    }
    sprintf(numerus, "%u", (insignatus integer)_sententia(f, k)
        ->initium);
    tituli[ZEPHYRUM]   = "sententia";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "versio";
    valores[I]         = chorda_ut_cstr(f->versio_arcae, f->scheda);
    redde _campos_scribere(m, tituli, valores, II);
}

/* s / Esc in sententia: pagina articuli sententiam continens */
interior b32
_paginam_ex_sententia (
    NavigatorMontatio* m)
{
         NavigatorFigurae* f;
                      s32  offset;
                      s32  k;
                      i32  i;
       constans character* tituli[II];
       constans character* valores[II];
                character  numerus[XVI];

    f = _paginas_parare(m);
    si (!f || _numerus_sententiarum(f) == ZEPHYRUM)
    {
        redde VERUM;
    }
    offset  = (s32)_sententia(f, _sententia_currens(m, f))->initium;
    k       = ZEPHYRUM;
    per (i = I; i < f->paginae_articuli; i++)
    {
        si ((s32)((constans LectorPagina*)xar_obtinere(f->paginae, i))
            ->initium > offset)
        {
            frange;
        }
        k = (s32)i;
    }
    /* positio NON servatur: offset sententiae (iam servatum) subtilior
     * est quam initium paginae et paginam eandem invenit (N6.2) */
    sprintf(numerus, "%d", (integer)k);
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "pagina";
    tituli[I]          = "pagina";
    valores[I]         = numerus;
    redde _campos_scribere(m, tituli, valores, II);
}

/* j k in sententia: proxima / prior, coacta */
interior b32
_sententiam_movere (
    NavigatorMontatio* m,
                  s32  gradus)
{
    NavigatorFigurae* f;
                 s32  k;

    f = _paginas_parare(m);
    si (!f || _numerus_sententiarum(f) == ZEPHYRUM)
    {
        redde VERUM;
    }
    k = _sententia_currens(m, f) + gradus;
    si (k < ZEPHYRUM || (i32)k >= _numerus_sententiarum(f))
    {
        redde VERUM;
    }
    redde _sententiam_scribere(m, f, k);
}

/* ===== columnae laterales (N3.5) ===== */

/* columna possessa a pagina k (prima); -1 si nulla */
interior s32
_columna_paginae (
    constans NavigatorFigurae* f,
                          s32  k)
{
    i32 c;

    per (c = ZEPHYRUM; f->columnae_possessores
        && c < xar_numerus(f->columnae_possessores); c++)
    {
        si (*(constans s32*)xar_obtinere(f->columnae_possessores, c)
            == k)
        {
            redde (s32)c;
        }
    }
    redde -I;
}

interior Xar*
_columnae_paginae (
    constans NavigatorFigurae* f,
                          s32  c)
{
    redde *(Xar**)xar_obtinere(f->columnae_paginae, (i32)c);
}

interior constans NavigatorColumna*
_columna (
    constans NavigatorFigurae* f,
                          s32  c)
{
    redde (constans NavigatorColumna*)xar_obtinere(f->textus->columnae,
        (i32)c);
}

/* columna currens (campus "columna" et columna paginae currentis in
 * visu paginae); -1 si in columna principali */
interior s32
_columna_currens (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f)
{
    s32 c;

    si (   !f || _visus(m) != VISUS_PAGINAE
        || _campus_numerus(m, "columna") < ZEPHYRUM)
    {
        redde -I;
    }
    c = _columna_paginae(f, _pagina_currens(m, f));
    redde c >= ZEPHYRUM && _columnae_paginae(f, c) ? c : -I;
}

/* pagina in columna c coacta */
interior s32
_columnae_pagina (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f,
                           s32  c)
{
    s32 k = _campus_numerus(m, "columna");
    s32 n = (s32)xar_numerus(_columnae_paginae(f, c));

    redde k < ZEPHYRUM ? ZEPHYRUM : k >= n ? n - I : k;
}

/* notae columnae activae (~ celat ut textum articuli) */
interior Xar*
_columnae_notae (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f,
                           s32  c)
{
    redde _campus_numerus(m, "nexus_occulti") == I ? NIHIL
        : _columna(f, c)->notae;
}

/* columna intratur (pagina k) aut relinquitur (k < 0); electio nexus
 * tollitur */
interior b32
_columnam_scribere (
    NavigatorMontatio* m,
                  s32  k)
{
    constans character* tituli[II];
    constans character* valores[II];
             character  numerus[XVI];

    sprintf(numerus, "%d", (integer)k);
    tituli[ZEPHYRUM]   = "columna";
    valores[ZEPHYRUM]  = k < ZEPHYRUM ? "" : numerus;
    tituli[I]          = "nexus";
    valores[I]         = "";
    redde _campos_scribere(m, tituli, valores, II);
}

/* l: columnam paginae intrare (nulla: nihil) */
interior b32
_columnam_intrare (
    NavigatorMontatio* m)
{
    NavigatorFigurae* f = _paginas_parare(m);

    si (   !f || _columna_currens(m, f) >= ZEPHYRUM
        || _columna_paginae(f, _pagina_currens(m, f)) < ZEPHYRUM)
    {
        redde VERUM;
    }
    redde _columnam_scribere(m, ZEPHYRUM);
}

/* h / Esc: ad paginam possessoris (pagina non mutatur) */
interior b32
_columnam_relinquere (
    NavigatorMontatio* m)
{
    redde _columna_currens(m, _paginas_parare(m)) < ZEPHYRUM ? VERUM
        : _columnam_scribere(m, -I);
}

/* j k in columna: *motum VERUM si in columna (vocans aliter paginam
 * movet) */
interior b32
_in_columna_movere (
    NavigatorMontatio* m,
                  s32  gradus,
                  b32* motum)
{
    NavigatorFigurae* f = _paginas_parare(m);
                 s32  c;
                 s32  k;

    *motum  = FALSUM;
    c       = _columna_currens(m, f);
    si (c < ZEPHYRUM)
    {
        redde VERUM;
    }
    *motum  = VERUM;
    k       = _columnae_pagina(m, f, c) + gradus;
    si (k < ZEPHYRUM || (i32)k >= xar_numerus(_columnae_paginae(f, c)))
    {
        redde VERUM;
    }
    redde _columnam_scribere(m, k);
}

/* ===== nexus per claves (N4.3) ===== */

/* textus et notae paginae k (articulus aut index nexuum) */
interior chorda
_textus_paginae (
    constans NavigatorFigurae* f,
                          s32  k)
{
    redde (i32)k < f->paginae_articuli ? f->textus->caput.textus
                                       : f->textus->nexus.textus;
}

interior Xar*
_notae_paginae (
    constans NavigatorFigurae* f,
                          s32  k)
{
    redde (i32)k < f->paginae_articuli ? f->textus->notae
                                       : f->textus->notae_nexuum;
}

/* N4.5: notae nexuum ACTIVAE paginae k - nexus celati (~): paginae
 * articuli nullas habent (index nexuum semper) */
interior Xar*
_notae_activae (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f,
                           s32  k)
{
    redde _campus_numerus(m, "nexus_occulti") == I
        && (i32)k < f->paginae_articuli ? NIHIL : _notae_paginae(f, k);
}

/* N6.3: notae nexuum activae textus articuli (visus sententiae) */
interior Xar*
_notae_articuli_activae (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f)
{
    redde _campus_numerus(m, "nexus_occulti") == I ? NIHIL
        : f->textus->notae;
}

/* nexus qui in [a, b) INCIPIT: gradus I - initium minimum > ab (ab <
 * 0: primus), gradus -I - maximum < ab (ab < 0: ultimus); -1 si
 * nullus */
interior s32
_nexum_in_ambitu (
    Xar* notae,
    s32  a,
    s32  b,
    s32  ab,
    s32  gradus)
{
    constans ArticulusNota* n;
                       s32  r;
                       i32  i;

    r = -I;
    per (i = ZEPHYRUM; notae && i < xar_numerus(notae); i++)
    {
        n = (constans ArticulusNota*)xar_obtinere(notae, i);
        si (   n->genus        != ARTICULUS_NEXUS || (s32)n->initium < a
            || (s32)n->initium >= b)
        {
            perge;
        }
        si (gradus > ZEPHYRUM && (ab < ZEPHYRUM || (s32)n->initium > ab)
            && (r < ZEPHYRUM || (s32)n->initium < r))
        {
            r = (s32)n->initium;
        }
        si (gradus < ZEPHYRUM && (ab < ZEPHYRUM || (s32)n->initium < ab)
            && (r < ZEPHYRUM || (s32)n->initium > r))
        {
            r = (s32)n->initium;
        }
    }
    redde r;
}

/* N6.3: unitas nexuum = pagina (visus paginae) aut sententia (visus
 * sententiae) */
interior b32
_in_sententiis (
    constans NavigatorMontatio* m)
{
    redde _visus(m) == VISUS_SENTENTIAE;
}

interior s32
_unitas_currens (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f)
{
    s32 c = _columna_currens(m, f);

    /* N3.5b: in columna laterali unitas = pagina columnae */
    redde c >= ZEPHYRUM ? _columnae_pagina(m, f, c)
        : _in_sententiis(m) ? _sententia_currens(m, f)
        : _pagina_currens(m, f);
}

interior s32
_unitates_numerus (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f)
{
    s32 c = _columna_currens(m, f);

    redde c >= ZEPHYRUM ? (s32)xar_numerus(_columnae_paginae(f, c))
        : (s32)(_in_sententiis(m) ? _numerus_sententiarum(f)
                                  : xar_numerus(f->paginae));
}

interior Xar*
_notae_unitatis (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f,
                           s32  k)
{
    s32 c = _columna_currens(m, f);

    redde c >= ZEPHYRUM ? _columnae_notae(m, f, c)
        : _in_sententiis(m) ? _notae_articuli_activae(m, f)
        : _notae_activae(m, f, k);
}

/* nexus qui in unitate k incipit (vide _nexum_in_ambitu) */
interior s32
_nexum_in_unitate (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f,
                           s32  k,
                           s32  ab,
                           s32  gradus)
{
       constans LectorPagina* g;
     constans CodexSententia* t;

    si (_in_sententiis(m))
    {
        si (k < ZEPHYRUM || (i32)k >= _numerus_sententiarum(f))
        {
            redde -I;
        }
        t = _sententia(f, k);
        redde _nexum_in_ambitu(_notae_unitatis(m, f, k),
            (s32)t->initium,
            (s32)t->finis, ab, gradus);
    }
    g = _columna_currens(m, f) >= ZEPHYRUM
        ? (constans LectorPagina*)xar_obtinere(_columnae_paginae(f,
            _columna_currens(m, f)), (i32)k)
        : (constans LectorPagina*)xar_obtinere(f->paginae, (i32)k);
    si (!g || xar_numerus(g->lineae) == ZEPHYRUM)
    {
        redde -I;
    }
    redde _nexum_in_ambitu(_notae_unitatis(m, f, k), (s32)((constans
        LectorLinea*)xar_obtinere(g->lineae, ZEPHYRUM))->initium,
        (s32)((constans LectorLinea*)xar_obtinere(g->lineae,
        xar_numerus(g->lineae) - I))->finis, ab, gradus);
}

/* nota nexus electi (campus "nexus") in unitate currente; NIHIL si
 * nullus aut non iam in ea */
interior constans ArticulusNota*
_nexus_electus (
    constans NavigatorMontatio* m,
     constans NavigatorFigurae* f)
{
    constans ArticulusNota* n;
                       Xar* notae;
                       s32  e;
                       s32  k;
                       i32  i;

    e = _campus_numerus(m, "nexus");
    si (!f || e < ZEPHYRUM || _unitates_numerus(m, f) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    k = _unitas_currens(m, f);
    si (_nexum_in_unitate(m, f, k, e - I, I) != e)
    {
        redde NIHIL;
    }
    notae = _notae_unitatis(m, f, k);
    per (i = ZEPHYRUM; notae && i < xar_numerus(notae); i++)
    {
        n = (constans ArticulusNota*)xar_obtinere(notae, i);
        si (n->genus == ARTICULUS_NEXUS && (s32)n->initium == e)
        {
            redde n;
        }
    }
    redde NIHIL;
}

/* Tab (gradus I) / S-Tab (-I): nexus proximus in unitate (pagina aut
 * sententia), aliter primus (ultimus) unitatis sequentis (prioris)
 * quae nexum habet - unitas et electus actu uno, positio servatur.
 * Nullus: nihil */
interior b32
_nexum_eligere (
    NavigatorMontatio* m,
                  s32  gradus)
{
       NavigatorFigurae* f;
                    s32  k;
                    s32  r;
       NavigatorPositio  q;
     constans character* tituli[II];
     constans character* valores[II];
              character  unitas[XVI];
              character  nexus[XVI];

    f = _paginas_parare(m);
    si (!f || _unitates_numerus(m, f) == ZEPHYRUM)
    {
        redde VERUM;
    }
    k = _unitas_currens(m, f);
    r = _nexum_in_unitate(m, f, k, _nexus_electus(m, f)
        ? _campus_numerus(m, "nexus") : -I, gradus);
    dum (r < ZEPHYRUM)
    {
        k += gradus;
        si (k < ZEPHYRUM || k >= _unitates_numerus(m, f))
        {
            redde VERUM;
        }
        r = _nexum_in_unitate(m, f, k, -I, gradus);
    }
    sprintf(nexus, "%d", (integer)r);
    si (_columna_currens(m, f) >= ZEPHYRUM)
    {
        /* N3.5b: pagina columnae; positio non servatur */
        sprintf(unitas, "%d", (integer)k);
        tituli[ZEPHYRUM] = "columna";
    }
    alioquin si (_in_sententiis(m))
    {
        sprintf(unitas, "%u", (insignatus integer)_sententia(f, k)
            ->initium);
        tituli[ZEPHYRUM] = "sententia";
        si (k != _sententia_currens(m, f))
        {
            q.versio      = f->versio_arcae;
            q.offset      = _sententia(f, k)->initium;
            q.in_nexibus  = FALSUM;
            (vacuum)navigator_positionem_scribere(m->bibliotheca,
                f->articulus_arcae, &q);
        }
    }
    alioquin
    {
        sprintf(unitas, "%d", (integer)k);
        tituli[ZEPHYRUM] = "pagina";
        si (k != _pagina_currens(m, f))
        {
            _positionem_servare(m, f, k);
        }
    }
    valores[ZEPHYRUM]  = unitas;
    tituli[I]          = "nexus";
    valores[I]         = nexus;
    redde _campos_scribere(m, tituli, valores, II);
}


/* ==================================================
 * Aperire per URL (N3.3)
 * ================================================== */

interior b32
_notam_scribere (
    NavigatorMontatio* m,
               chorda  nota,
              Piscina* p)
{
    constans character* tituli[I];
    constans character* valores[I];

    tituli[ZEPHYRUM]   = "nota";
    valores[ZEPHYRUM]  = chorda_ut_cstr(nota, p);
    redde _campos_scribere(m, tituli, valores, I);
}

/* finis petitionis: articulus apertus aut nota; ABRUPTA = petitio
 * superata (vocans eam dimittendam iam posuit) */
interior vacuum
_petitio_finis (
                               vacuum* datum,
    constans NavigatorPetitioResultus* r)
{
    NavigatorMontatio*  m = (NavigatorMontatio*)datum;
     NavigatorFigurae*  f = (NavigatorFigurae*)m->figurae_ctx;
     NavigatorPetitio** d;
               chorda   nota;

    si (r->status == NAVIGATOR_PETITIO_ABRUPTA)
    {
        redde;
    }
    d = (NavigatorPetitio**)xar_addere(f->dimittendae);
    si (d)
    {
        *d = f->petitio;
    }
    f->petitio = NIHIL;
    f->mutatum = VERUM;
    si (r->status == NAVIGATOR_PETITIO_SERVATA && r->articulus)
    {
        (vacuum)_articulum_aperire(m, r->articulus->identitas,
            f->scheda);
        redde;
    }
    si (r->status == NAVIGATOR_PETITIO_NON_INVENTA)
    {
        nota = chorda_ex_literis("Not found", f->scheda);
    }
    alioquin si (r->status == NAVIGATOR_PETITIO_IGNOTA)
    {
        nota = chorda_ex_literis("Unsupported site (Wikipedia only for "
            "now)", f->scheda);
    }
    alioquin
    {
        nota = chorda_concatenare(chorda_ex_literis("Network: ",
            f->scheda), r->descriptio, f->scheda);
    }
    (vacuum)_notam_scribere(m, nota, f->scheda);
}


/* ==================================================
 * Quaestio (N3.3b)
 * ================================================== */

/* petitio pendens abrumpitur (finis ABRUPTA ignoratur) et dimittenda
 * ponitur */
interior vacuum
_pendentem_abrumpere (
    NavigatorFigurae* f)
{
     NavigatorPetitio*  pendens = f->petitio;
     NavigatorPetitio** d;

    f->petitio = NIHIL;
    si (pendens)
    {
        navigator_petitionem_abrumpere(pendens);
        d = (NavigatorPetitio**)xar_addere(f->dimittendae);
        si (d)
        {
            *d = pendens;
        }
    }
}

/* finis quaestionis: eventus in arca_eventuum copiantur (petitio
 * dimittetur), visus eventus; nulli: "No results" */
interior vacuum
_quaestio_finis (
                                vacuum* datum,
    constans NavigatorQuaestioResultus* r)
{
      NavigatorMontatio*  m = (NavigatorMontatio*)datum;
       NavigatorFigurae*  f = (NavigatorFigurae*)m->figurae_ctx;
       NavigatorPetitio** d;
     constans character*  tituli[III];
     constans character*  valores[III];
                    i32   i;

    si (r->status == NAVIGATOR_PETITIO_ABRUPTA)
    {
        redde;
    }
    d = (NavigatorPetitio**)xar_addere(f->dimittendae);
    si (d)
    {
        *d = f->petitio;
    }
    f->petitio = NIHIL;
    f->mutatum = VERUM;
    si (r->status != NAVIGATOR_PETITIO_SERVATA || !r->eventus)
    {
        (vacuum)_notam_scribere(m, r->status
            == NAVIGATOR_PETITIO_NON_INVENTA ? chorda_ex_literis(
            "Not found", f->scheda) : chorda_concatenare(
            chorda_ex_literis("Network: ", f->scheda), r->descriptio,
            f->scheda), f->scheda);
        redde;
    }
    piscina_vacare(f->arca_eventuum);
    f->eventus = xar_creare(f->arca_eventuum,
        (i32)magnitudo(VicipaediaEventus));
    per (i = ZEPHYRUM; f->eventus && i < xar_numerus(r->eventus); i++)
    {
        constans VicipaediaEventus* e = (constans VicipaediaEventus*)
            xar_obtinere(r->eventus, i);
                 VicipaediaEventus* c = (VicipaediaEventus*)xar_addere(
                     f->eventus);

        si (c)
        {
            c->titulus    = chorda_transcribere(e->titulus,
                f->arca_eventuum);
            c->clavis     = chorda_transcribere(e->clavis,
                f->arca_eventuum);
            c->descriptio = chorda_transcribere(e->descriptio,
                f->arca_eventuum);
        }
    }
    si (!f->eventus || xar_numerus(f->eventus) == ZEPHYRUM)
    {
        (vacuum)_notam_scribere(m, chorda_ex_literis("No results",
            f->scheda), f->scheda);
        redde;
    }
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "eventus";
    tituli[I]          = "electus_eventus";
    valores[I]         = "0";
    tituli[II]         = "nota";
    valores[II]        = "";
    (vacuum)_campos_scribere(m, tituli, valores, III);
}

/* quaestio in hospite dato incipit (pendens abrumpitur) */
interior b32
_quaestionem_incipere (
    NavigatorMontatio* m,
               chorda  hospes,
               chorda  verba,
              Piscina* p)
{
       NavigatorFigurae* f = (NavigatorFigurae*)m->figurae_ctx;
                Reactor* reactor;
     constans character* tituli[III];
     constans character* valores[III];

    reactor = m->reactor ? m->reactor(m->reactor_ctx) : NIHIL;
    si (!reactor)
    {
        redde _notam_scribere(m, chorda_ex_literis("Offline", p), p);
    }
    _pendentem_abrumpere(f);
    f->petitio = navigator_quaerere(reactor, hospes, verba,
        _quaestio_finis, m, m->optiones_petitionis);
    si (!f->petitio)
    {
        redde _notam_scribere(m, chorda_ex_literis("Cannot search", p),
            p);
    }
    tituli[ZEPHYRUM]   = "quaestio";
    valores[ZEPHYRUM]  = chorda_ut_cstr(verba, p);
    tituli[I]          = "quaestio_hospes";
    valores[I]         = chorda_ut_cstr(hospes, p);
    tituli[II]         = "nota";
    valores[II]        = chorda_ut_cstr(chorda_concatenare(
        chorda_concatenare(chorda_concatenare(chorda_ex_literis(
        "Searching ", p), hospes, p), chorda_ex_literis(" for '", p),
        p),
        chorda_concatenare(verba, chorda_ex_literis("'...", p), p), p),
        p);
    redde _campos_scribere(m, tituli, valores, III);
}

/* hospes ordinarius: articuli currentis (pagina), aliter en */
interior chorda
_hospes_currens (
    constans NavigatorMontatio* m,
                       Piscina* p)
{
    constans NavigatorArticulus* a;

    a = _visus(m) == VISUS_PAGINAE ? _articulus_per_id(m, _campus(m,
        "articulus")) : NIHIL;
    redde a ? chorda_transcribere(_hospes(a->url), p)
            : chorda_ex_literis("en.wikipedia.org", p);
}

/* "la: Roma" -> la.wikipedia.org, "Roma"; sine praefixo: hospes
 * currens */
interior b32
_quaerere (
    NavigatorMontatio* m,
               chorda  textus,
              Piscina* p)
{
    chorda hospes;
    chorda verba;
       i32 i;

    verba = textus;
    per (i = ZEPHYRUM; i < textus.mensura && i < XVI
        && ((textus.datum[i] >= 'a' && textus.datum[i] <= 'z')
            || textus.datum[i] == '-'); i++)
    {
    }
    si (i >= II && i < textus.mensura && textus.datum[i] == ':')
    {
        hospes = chorda_concatenare(chorda_sectio(textus, ZEPHYRUM, i),
            chorda_ex_literis(".wikipedia.org", p), p);
        verba  = chorda_praecidere(chorda_sectio(textus, i + I,
            textus.mensura));
    }
    alioquin
    {
        hospes = _hospes_currens(m, p);
    }
    si (verba.mensura == ZEPHYRUM)
    {
        redde _notam_scribere(m, chorda_ex_literis("Nothing to search",
            p), p);
    }
    redde _quaestionem_incipere(m, hospes, verba, p);
}

/* URL sine schemate ("https://a.org/x" -> "a.org/x") */
interior chorda
_sine_schemate (
    chorda url)
{
    s32 i = chorda_invenire_index(url, _litterae("://"));

    redde i < ZEPHYRUM ? url : chorda_sectio(url, (i32)i + III,
        url.mensura);
}

/* URL: servatus -> statim; aliter petitio in reactore hospitis */
interior b32
_url_aperire (
    NavigatorMontatio* m,
               chorda  url,
              Piscina* p)
{
               NavigatorFigurae*  f = (NavigatorFigurae*)m->figurae_ctx;
                VicipaediaSedes   sedes;
               NavigatorPetitio*  pendens;
               NavigatorPetitio** d;
                        Reactor*  reactor;
    constans NavigatorArticulus*  a;

    si (!chorda_continet(url, _litterae("://")))
    {
        redde _quaerere(m, url, p);   /* N3.3b: verba = quaestio */
    }
    si (!vicipaedia_sedem_legere(url, &sedes, p))
    {
        /* N4.2: URL sine schemate nominatur (nexus alienus) */
        redde _notam_scribere(m, chorda_concatenare(chorda_ex_literis(
            "Unsupported site: ", p), _sine_schemate(url), p), p);
    }
    a = navigator_articulum_invenire(m->bibliotheca,
        vicipaedia_url_articuli(&sedes, p));
    si (a)
    {
        redde _articulum_aperire(m, a->identitas, p);
    }
    reactor = m->reactor ? m->reactor(m->reactor_ctx) : NIHIL;
    si (!reactor)
    {
        redde _notam_scribere(m, chorda_ex_literis("Offline", p), p);
    }
    /* petitio nova pendentem abrumpit (finis ABRUPTA synchronus,
     * _petitio_finis eam ignorat; hic dimittenda ponitur) */
    pendens     = f->petitio;
    f->petitio  = NIHIL;
    si (pendens)
    {
        navigator_petitionem_abrumpere(pendens);
        d = (NavigatorPetitio**)xar_addere(f->dimittendae);
        si (d)
        {
            *d = pendens;
        }
    }
    f->petitio = navigator_petere(reactor, m->bibliotheca, url,
        _petitio_finis, m, m->optiones_petitionis);
    si (!f->petitio)
    {
        redde _notam_scribere(m, chorda_ex_literis("Cannot fetch", p),
            p);
    }
    redde _notam_scribere(m, chorda_concatenare(chorda_concatenare(
        chorda_ex_literis("Fetching ", p), sedes.hospes, p),
        chorda_concatenare(chorda_ex_literis("/wiki/", p),
        sedes.titulus,
        p), p), p);
}

b32
navigator_pulsare (
    vacuum* ctx)
{
    NavigatorMontatio* m = (NavigatorMontatio*)ctx;
     NavigatorFigurae* f;
                  b32  mutatum;
                  i32  i;

    f = m ? (NavigatorFigurae*)m->figurae_ctx : NIHIL;
    si (!f)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(f->dimittendae); i++)
    {
        navigator_petitionem_dimittere(*(NavigatorPetitio**)
            xar_obtinere(f->dimittendae, i));
    }
    xar_vacare(f->dimittendae);
    mutatum     = f->mutatum;
    f->mutatum  = FALSUM;
    redde mutatum;
}

s32
navigator_mora (
    vacuum* ctx)
{
    NavigatorMontatio* m = (NavigatorMontatio*)ctx;
     NavigatorFigurae* f;

    f = m ? (NavigatorFigurae*)m->figurae_ctx : NIHIL;
    redde f && (f->mutatum || xar_numerus(f->dimittendae) > ZEPHYRUM)
        ? ZEPHYRUM : -I;
}


/* ==================================================
 * Praecepta
 * ================================================== */

nomen structura {
    NavigatorMontatio* m;
              Piscina* p;
                Album* l;
                  Xar* ordo;
         RadixIndicis  radix;     /* N3.4: radix (dominia, recentes) */
                  s32  electus;     /* positio in ordine (dominiis) */
                  i32  visus;
} ContextusPraecepti;

interior b32
_requisita (
        i32  requisita,
     vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde !(requisita & REQUIRIT_ELECTUM) || cp->electus >= ZEPHYRUM;
}

interior vacuum
_contextum_parare (
     NavigatorMontatio* m,
               Piscina* p,
    ContextusPraecepti* cp)
{
    cp->m               = m;
    cp->p               = p;
    cp->visus           = _visus(m);
    cp->ordo            = _ordo(m, p);
    cp->radix.dominia   = NIHIL;
    cp->radix.recentes  = NIHIL;
    si (cp->visus == VISUS_RADICIS)
    {
        _radicem_parare(m, p, &cp->radix);
        cp->l        = _radicis_album(m, &cp->radix, p);
        cp->electus  = _electum_radicis(m, &cp->radix);
        redde;
    }
    cp->l        = _album(m, cp->ordo, p);
    cp->electus  = _electus(m, cp->ordo);
}

/* N3.3b: contextus visus eventuum (album et electus sui) */
interior vacuum
_contextum_eventuum_parare (
    ContextusPraecepti* cp);

interior b32
_electum_scribere (
    ContextusPraecepti* cp,
                   s32  positio)
{
    constans character* tituli[I];
    constans character* valores[I];

    si (positio < ZEPHYRUM || (i32)positio >= xar_numerus(cp->ordo))
    {
        redde VERUM;
    }
    tituli[ZEPHYRUM]   = "electus_articulus";
    valores[ZEPHYRUM]  = chorda_ut_cstr(navigator_articulus(
        cp->m->bibliotheca, _index_ordinis(cp->ordo,
        positio))->identitas,
        cp->p);
    redde _campos_scribere(cp->m, tituli, valores, I);
}

interior b32
fac_proximum (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _electum_scribere(cp, cp->electus + I);
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _electum_scribere(cp, cp->electus - I);
}

/* paginae indicis: electus = res prima paginae */
interior b32
_indicem_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;
    s32 res;

    k = _pagina_indicis(cp->l, cp->ordo, cp->electus) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    res = album_res_prima(cp->l, k);
    redde res > ZEPHYRUM ? _electum_scribere(cp, _positio_indicis(
        cp->ordo, (i32)(res - I))) : VERUM;
}

interior b32
fac_indicem_priorem (
    vacuum* ctx)
{
    redde _indicem_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_indicem_proximum (
    vacuum* ctx)
{
    redde _indicem_vertere((ContextusPraecepti*)ctx, I);
}

interior b32
fac_aperire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    si (cp->electus < ZEPHYRUM)
    {
        redde VERUM;
    }
    redde _articulum_aperire(cp->m, navigator_articulus(
        cp->m->bibliotheca, _index_ordinis(cp->ordo, cp->electus))
        ->identitas, cp->p);
}

interior b32
fac_paginam_proximam (
    vacuum* ctx)
{
    redde _paginam_movere(((ContextusPraecepti*)ctx)->m, I);
}

interior b32
fac_paginam_priorem (
    vacuum* ctx)
{
    redde _paginam_movere(((ContextusPraecepti*)ctx)->m, -I);
}

/* dominium radicale articuli aperti si favores habet, aliter ""
 * (radix) */
interior constans character*
_dominium_reditus (
    ContextusPraecepti* cp)
{
     constans NavigatorArticulus* a;
                          chorda  r;
                             i32  i;

    a = _articulus_per_id(cp->m, _campus(cp->m, "articulus"));
    si (!a)
    {
        redde "";
    }
    r = _radix_dominii(_hospes(a->url));
    per (i = ZEPHYRUM; i < navigator_articulorum_numerus(
        cp->m->bibliotheca); i++)
    {
        constans NavigatorArticulus* b = navigator_articulus(
            cp->m->bibliotheca, i);

        si (b->favor && chorda_aequalis(_radix_dominii(_hospes(b->url)),
            r))
        {
            redde chorda_ut_cstr(r, cp->p);
        }
    }
    redde "";
}

/* Esc in pagina: ad indicem (articulus e semita tollitur; electus
 * manet) - N3.4: in dominium articuli si favores habet */
interior b32
fac_ad_indicem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[VI];
    constans character* valores[VI];

    /* N4.3: nexus electus prius tollitur */
    si (_campus_numerus(cp->m, "nexus") >= ZEPHYRUM)
    {
        tituli[ZEPHYRUM]   = "nexus";
        valores[ZEPHYRUM]  = "";
        redde _campos_scribere(cp->m, tituli, valores, I);
    }
    /* N3.5: ex columna laterali ad paginam, non ad indicem */
    si (_columna_currens(cp->m, _paginas_parare(cp->m)) >= ZEPHYRUM)
    {
        redde _columnam_relinquere(cp->m);
    }
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "index";
    tituli[I]          = "articulus";
    valores[I]         = "";
    tituli[II]         = "versio";
    valores[II]        = "";
    tituli[III]        = "pagina";
    valores[III]       = "";
    tituli[IV]         = "nota";
    valores[IV]        = "";
    tituli[V]          = "dominium";
    valores[V]         = _dominium_reditus(cp);
    redde _campos_scribere(cp->m, tituli, valores, VI);
}

/* ===== radix et favores (N3.4) ===== */

/* positio e radicis eligitur: "d:<dominium>" aut "a:<ULID>" */
interior b32
_radicem_eligere (
    ContextusPraecepti* cp,
                   s32  e)
{
     constans character* tituli[I];
     constans character* valores[I];
                    i32  n;

    n = xar_numerus(cp->radix.dominia);
    si (e < ZEPHYRUM || (i32)e >= _radicis_numerus(&cp->radix))
    {
        redde VERUM;
    }
    tituli[ZEPHYRUM]   = "electus";
    valores[ZEPHYRUM]  = chorda_ut_cstr((i32)e < n
        ? chorda_concatenare(chorda_ex_literis("d:", cp->p), _dominium(
            cp->radix.dominia, e)->titulus, cp->p)
        : chorda_concatenare(chorda_ex_literis("a:", cp->p), _recens(
            &cp->radix, e - (s32)n)->identitas, cp->p), cp->p);
    redde _campos_scribere(cp->m, tituli, valores, I);
}

interior b32
fac_radicis_proximum (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _radicem_eligere(cp, cp->electus + I);
}

interior b32
fac_radicis_prius (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _radicem_eligere(cp, cp->electus - I);
}

interior b32
_radicem_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;
    s32 res;

    k = (cp->electus >= ZEPHYRUM ? album_pagina_rei(cp->l, cp->electus
        + I, ZEPHYRUM) : ZEPHYRUM) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    res = album_res_prima(cp->l, k);
    redde res > ZEPHYRUM ? _radicem_eligere(cp, res - I) : VERUM;
}

interior b32
fac_radicis_priora (
    vacuum* ctx)
{
    redde _radicem_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_radicis_proxima (
    vacuum* ctx)
{
    redde _radicem_vertere((ContextusPraecepti*)ctx, I);
}

/* Enter in radice: in dominium descendit, aut articulum recentem
 * aperit */
interior b32
fac_radicis_aperire (
    vacuum* ctx)
{
     ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
     constans character* tituli[I];
     constans character* valores[I];
                    i32  n;

    n = xar_numerus(cp->radix.dominia);
    si (cp->electus < ZEPHYRUM)
    {
        redde VERUM;
    }
    si ((i32)cp->electus >= n)
    {
        redde _articulum_aperire(cp->m, _recens(&cp->radix, cp->electus
            - (s32)n)->identitas, cp->p);
    }
    tituli[ZEPHYRUM]   = "dominium";
    valores[ZEPHYRUM]  = chorda_ut_cstr(_dominium(cp->radix.dominia,
        cp->electus)->titulus, cp->p);
    redde _campos_scribere(cp->m, tituli, valores, I);
}

/* Esc in dominio: ad radicem, dominium relictum electum */
interior b32
fac_ad_radicem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[III];
    constans character* valores[III];

    tituli[ZEPHYRUM]   = "electus";
    valores[ZEPHYRUM]  = chorda_ut_cstr(chorda_concatenare(
        chorda_ex_literis("d:", cp->p), _campus(cp->m, "dominium"),
        cp->p), cp->p);
    tituli[I]    = "dominium";
    valores[I]   = "";
    tituli[II]   = "nota";
    valores[II]  = "";
    redde _campos_scribere(cp->m, tituli, valores, III);
}

/* favor articuli vertitur (copia identitatis: acta relegantur);
 * latus pingendum */
interior b32
_favorem_vertere (
              NavigatorMontatio* m,
    constans NavigatorArticulus* a,
                        Piscina* p)
{
    si (!a)
    {
        redde VERUM;
    }
    ((NavigatorFigurae*)m->figurae_ctx)->mutatum = VERUM;
    redde navigator_favorem_ponere(m->bibliotheca, chorda_transcribere(
        a->identitas, p), !a->favor);
}

/* b: in pagina articulus apertus, in dominio electus */
interior b32
fac_favorem_vertere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    si (cp->visus == VISUS_PAGINAE)
    {
        redde _favorem_vertere(cp->m, _articulus_per_id(cp->m, _campus(
            cp->m, "articulus")), cp->p);
    }
    si (cp->visus == VISUS_RADICIS)
    {
        /* N3.4b: in ordine recenti solum (dominium non vertitur) */
        redde cp->electus < (s32)xar_numerus(cp->radix.dominia)
            || _favorem_vertere(cp->m, _recens(&cp->radix, cp->electus
            - (s32)xar_numerus(cp->radix.dominia)), cp->p);
    }
    redde cp->electus >= ZEPHYRUM && _favorem_vertere(cp->m,
        navigator_articulus(cp->m->bibliotheca, _index_ordinis(cp->ordo,
        cp->electus)), cp->p);
}

interior b32
fac_nexum_proximum (
    vacuum* ctx)
{
    redde _nexum_eligere(((ContextusPraecepti*)ctx)->m, I);
}

interior b32
fac_nexum_priorem (
    vacuum* ctx)
{
    redde _nexum_eligere(((ContextusPraecepti*)ctx)->m, -I);
}

interior b32
fac_sententiam_ex_pagina (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    /* N3.5: in columna laterali nihil */
    redde _columna_currens(cp->m, _paginas_parare(cp->m)) >= ZEPHYRUM
        || _sententiam_ex_pagina(cp->m);
}

interior b32
fac_columnam_intrare (
    vacuum* ctx)
{
    redde _columnam_intrare(((ContextusPraecepti*)ctx)->m);
}

interior b32
fac_columnam_relinquere (
    vacuum* ctx)
{
    redde _columnam_relinquere(((ContextusPraecepti*)ctx)->m);
}

interior b32
fac_paginam_ex_sententia (
    vacuum* ctx)
{
    redde _paginam_ex_sententia(((ContextusPraecepti*)ctx)->m);
}

/* Esc in sententia: nexus electus prius tollitur (N6.3), deinde ad
 * paginam */
interior b32
fac_sententia_redire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[I];
    constans character* valores[I];

    si (_campus_numerus(cp->m, "nexus") >= ZEPHYRUM)
    {
        tituli[ZEPHYRUM]   = "nexus";
        valores[ZEPHYRUM]  = "";
        redde _campos_scribere(cp->m, tituli, valores, I);
    }
    redde _paginam_ex_sententia(cp->m);
}

interior b32
fac_sententiam_proximam (
    vacuum* ctx)
{
    redde _sententiam_movere(((ContextusPraecepti*)ctx)->m, I);
}

interior b32
fac_sententiam_priorem (
    vacuum* ctx)
{
    redde _sententiam_movere(((ContextusPraecepti*)ctx)->m, -I);
}

/* N4.5: ~ nexus textus articuli celat / monstrat; electio tollitur */
interior b32
fac_nexus_vertere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[II];
    constans character* valores[II];

    tituli[ZEPHYRUM]   = "nexus_occulti";
    valores[ZEPHYRUM]  = _campus_numerus(cp->m, "nexus_occulti") == I
        ? "" : "1";
    tituli[I]   = "nexus";
    valores[I]  = "";
    redde _campos_scribere(cp->m, tituli, valores, II);
}

/* Enter in pagina: nexus electus aperitur (ut ictus); nullo: nihil */
interior b32
fac_nexum_aperire (
    vacuum* ctx)
{
          ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
      constans ArticulusNota* n;

    n = _nexus_electus(cp->m, _paginas_parare(cp->m));
    redde !n || _url_aperire(cp->m, chorda_transcribere(n->scopus,
        cp->p), cp->p);
}

/* o: prompta "url" aperitur (nota prior tollitur) */
interior b32
fac_promptam_aperire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    (vacuum)_notam_scribere(cp->m, chorda_ex_literis("", cp->p), cp->p);
    redde prompta_aperire(&cp->m->ramus, actio_navigatoris, "url",
        chorda_ex_literis("", cp->p), cp->p);
}

/* ===== eventus quaestionis (N3.3b) ===== */

interior i32
_numerus_eventuum (
    constans NavigatorMontatio* m)
{
    constans NavigatorFigurae* f = (constans NavigatorFigurae*)
        m->figurae_ctx;

    redde f && f->eventus ? xar_numerus(f->eventus) : ZEPHYRUM;
}

/* album eventuum: res = index + 1, id "r<res>" */
interior Album*
_eventuum_album (
    constans NavigatorMontatio* m,
                       Piscina* p)
{
        Album* l;
          i32  i;
    character  id[XVI];

    l = album_creare(p, _capacitas(m));
    per (i = ZEPHYRUM; l && i < _numerus_eventuum(m); i++)
    {
        sprintf(id, "r%d", (integer)(i + I));
        (vacuum)album_addere(l, id, ALBUM_RES, NAV_ORDO, (s32)(i + I));
    }
    redde l;
}

/* eventus electus (coactus); -1 si nulli */
interior s32
_electus_eventus (
    constans NavigatorMontatio* m)
{
    i32 n = _numerus_eventuum(m);
    s32 k;

    si (n == ZEPHYRUM)
    {
        redde -I;
    }
    k = _campus_numerus(m, "electus_eventus");
    si (k < ZEPHYRUM)
    {
        k = ZEPHYRUM;
    }
    si ((i32)k >= n)
    {
        k = (s32)n - I;
    }
    redde k;
}

interior constans VicipaediaEventus*
_eventus (
    constans NavigatorMontatio* m,
                           s32  k)
{
    redde (constans VicipaediaEventus*)xar_obtinere(
        ((constans NavigatorFigurae*)m->figurae_ctx)->eventus, (i32)k);
}

interior b32
_eventum_scribere (
    ContextusPraecepti* cp,
                   s32  k)
{
    constans character* tituli[I];
    constans character* valores[I];
             character  numerus[XVI];

    si (k < ZEPHYRUM || (i32)k >= _numerus_eventuum(cp->m))
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)k);
    tituli[ZEPHYRUM]   = "electus_eventus";
    valores[ZEPHYRUM]  = numerus;
    redde _campos_scribere(cp->m, tituli, valores, I);
}

interior b32
fac_eventum_proximum (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _eventum_scribere(cp, cp->electus + I);
}

interior b32
fac_eventum_priorem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _eventum_scribere(cp, cp->electus - I);
}

interior b32
_eventus_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;
    s32 res;

    k = (cp->electus >= ZEPHYRUM ? album_pagina_rei(cp->l, cp->electus
        + I, ZEPHYRUM) : ZEPHYRUM) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    res = album_res_prima(cp->l, k);
    redde res > ZEPHYRUM ? _eventum_scribere(cp, res - I) : VERUM;
}

interior b32
fac_eventus_priores (
    vacuum* ctx)
{
    redde _eventus_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_eventus_proximos (
    vacuum* ctx)
{
    redde _eventus_vertere((ContextusPraecepti*)ctx, I);
}

/* Enter: articulus eventus per viam URL (servatus statim) */
interior b32
fac_eventum_aperire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
       VicipaediaSedes  sedes;

    si (cp->electus < ZEPHYRUM)
    {
        redde VERUM;
    }
    sedes.hospes   = _campus(cp->m, "quaestio_hospes");
    sedes.titulus  = _eventus(cp->m, cp->electus)->clavis;
    sedes.ancora   = _vacua();
    si (sedes.hospes.mensura == ZEPHYRUM)
    {
        sedes.hospes = chorda_ex_literis("en.wikipedia.org", cp->p);
    }
    redde _url_aperire(cp->m, vicipaedia_url_articuli(&sedes, cp->p),
        cp->p);
}

/* Esc: ad paginam si articulus apertus, aliter ad indicem */
interior b32
fac_eventus_relinquere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[II];
    constans character* valores[II];

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = _campus(cp->m, "articulus").mensura > ZEPHYRUM
        ? "pagina" : "index";
    tituli[I]   = "nota";
    valores[I]  = "";
    redde _campos_scribere(cp->m, tituli, valores, II);
}

interior vacuum
_contextum_eventuum_parare (
    ContextusPraecepti* cp)
{
    si (cp->visus == VISUS_EVENTUUM)
    {
        cp->l        = _eventuum_album(cp->m, cp->p);
        cp->electus  = _electus_eventus(cp->m);
    }
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "navigator.url", "Open URL", "o", VISUS_INDICIS | VISUS_PAGINAE
        | VISUS_EVENTUUM | VISUS_RADICIS | VISUS_SENTENTIAE, ZEPHYRUM,
            fac_promptam_aperire },
    { "navigator.radix_proxima", "Next", "j", VISUS_RADICIS, ZEPHYRUM,
        fac_radicis_proximum },
    { "navigator.radix_prior", "Previous", "k", VISUS_RADICIS, ZEPHYRUM,
        fac_radicis_prius },
    { "navigator.radix_priora", "Previous page", "[", VISUS_RADICIS,
        ZEPHYRUM, fac_radicis_priora },
    { "navigator.radix_proxima_pagina", "Next page", "]", VISUS_RADICIS,
        ZEPHYRUM, fac_radicis_proxima },
    { "navigator.radix_aperire", "Open", "Enter", VISUS_RADICIS,
        REQUIRIT_ELECTUM, fac_radicis_aperire },
    { "navigator.ad_radicem", "Saved", "Esc", VISUS_INDICIS, ZEPHYRUM,
        fac_ad_radicem },
    { "navigator.favor", "Bookmark", "b", VISUS_INDICIS | VISUS_PAGINAE
        | VISUS_RADICIS, ZEPHYRUM, fac_favorem_vertere },
    { "navigator.eventus_proximus", "Next result", "j", VISUS_EVENTUUM,
        ZEPHYRUM, fac_eventum_proximum },
    { "navigator.eventus_prior", "Previous result", "k", VISUS_EVENTUUM,
        ZEPHYRUM, fac_eventum_priorem },
    { "navigator.eventus_priores", "Previous page", "[", VISUS_EVENTUUM,
        ZEPHYRUM, fac_eventus_priores },
    { "navigator.eventus_proximi", "Next page", "]", VISUS_EVENTUUM,
        ZEPHYRUM, fac_eventus_proximos },
    { "navigator.eventum_aperire", "Open", "Enter", VISUS_EVENTUUM,
        REQUIRIT_ELECTUM, fac_eventum_aperire },
    { "navigator.eventus_relinquere", "Back", "Esc", VISUS_EVENTUUM,
        ZEPHYRUM, fac_eventus_relinquere },
    { "navigator.proximus", "Next article", "j", VISUS_INDICIS,
        ZEPHYRUM, fac_proximum },
    { "navigator.prior", "Previous article", "k", VISUS_INDICIS,
        ZEPHYRUM, fac_priorem },
    { "navigator.index_prior", "Previous page", "[", VISUS_INDICIS,
        ZEPHYRUM, fac_indicem_priorem },
    { "navigator.index_proximus", "Next page", "]", VISUS_INDICIS,
        ZEPHYRUM, fac_indicem_proximum },
    { "navigator.aperire", "Read", "Enter", VISUS_INDICIS,
        REQUIRIT_ELECTUM, fac_aperire },
    { "navigator.nexus_proximus", "Next link", "Tab", VISUS_PAGINAE
        | VISUS_SENTENTIAE, ZEPHYRUM, fac_nexum_proximum },
    { "navigator.nexus_prior", "Previous link", "S-Tab", VISUS_PAGINAE
        | VISUS_SENTENTIAE, ZEPHYRUM, fac_nexum_priorem },
    { "navigator.nexum_aperire", "Open link", "Enter", VISUS_PAGINAE
        | VISUS_SENTENTIAE, ZEPHYRUM, fac_nexum_aperire },
    { "navigator.nexus_vertere", "Hide links", "~", VISUS_PAGINAE,
        ZEPHYRUM, fac_nexus_vertere },
    { "navigator.sententia", "Sentence view", "s", VISUS_PAGINAE,
        ZEPHYRUM, fac_sententiam_ex_pagina },
    { "navigator.columna_intrare", "Side column", "l", VISUS_PAGINAE,
        ZEPHYRUM, fac_columnam_intrare },
    { "navigator.columna_relinquere", "Back to page", "h",
        VISUS_PAGINAE, ZEPHYRUM, fac_columnam_relinquere },
    { "navigator.sententia_proxima", "Next sentence", "j",
        VISUS_SENTENTIAE, ZEPHYRUM, fac_sententiam_proximam },
    { "navigator.sententia_prior", "Previous sentence", "k",
        VISUS_SENTENTIAE, ZEPHYRUM, fac_sententiam_priorem },
    { "navigator.ad_paginam", "Page view", "s", VISUS_SENTENTIAE,
        ZEPHYRUM, fac_paginam_ex_sententia },
    { "navigator.sententia_redire", "Page view", "Esc",
        VISUS_SENTENTIAE, ZEPHYRUM, fac_sententia_redire },
    { "navigator.pagina_proxima", "Next page", "j", VISUS_PAGINAE,
        ZEPHYRUM, fac_paginam_proximam },
    { "navigator.pagina_prior", "Previous page", "k", VISUS_PAGINAE,
        ZEPHYRUM, fac_paginam_priorem },
    { "navigator.ad_indicem", "Saved", "Esc", VISUS_PAGINAE,
        ZEPHYRUM, fac_ad_indicem },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

/* N4.2: nota NEXUS sub puncto (x, y) LOCALI componentis "textus"
 * (ab origine eius - Destinatio.punctum_locale; latus in vico non in
 * origine schirmi) paginae currentis: linea per y, columna per x
 * (runae, fons lineae), offset, nota quae eum continet; NIHIL extra
 * nexum */
interior constans ArticulusNota*
_nexus_ad_punctum (
          NavigatorMontatio* m,
                        s32  x,
                        s32  y)
{
            NavigatorFigurae* f;
        constans LectorLinea* l;
      constans ArticulusNota* n;
                      chorda  t;
                         Xar* notae;
                         Xar* lineae;
                         s32  lateris;
                         s32  supra;
                         s32  k;
                         s32  lx;
                         s32  columna;
                         i32  offset;
                         i32  i;
                         i32  j;

    f = _paginas_parare(m);
    si (!f)
    {
        redde NIHIL;
    }
    _margines(m, &lateris, &supra);
    si (_in_sententiis(m))
    {
        /* N6.3: lineae dispositionis sententiae (eadem ac pictura) */
        s32 latitudo;
        s32 altitudo;
        b32 praecisa;

        si (_numerus_sententiarum(f) == ZEPHYRUM)
        {
            redde NIHIL;
        }
        _area(m, &latitudo, &altitudo);
        lineae = lector_sententiam_disponere(&f->textus->caput,
            _sententia(f, _sententia_currens(m, f)), (i32)latitudo,
            (i32)altitudo, &praecisa, f->scheda);
        t      = f->textus->caput.textus;
        notae  = _notae_articuli_activae(m, f);
    }
    alioquin si (_columna_currens(m, f) >= ZEPHYRUM)
    {
        /* N3.5: lineae paginae columnae lateralis */
        s32 c = _columna_currens(m, f);

        lineae  = ((constans LectorPagina*)xar_obtinere(
            _columnae_paginae(f, c), (i32)_columnae_pagina(m, f, c)))
            ->lineae;
        t      = _columna(f, c)->caput.textus;
        notae  = _columnae_notae(m, f, c);
    }
    alioquin
    {
        k       = _pagina_currens(m, f);
        lineae  = ((constans LectorPagina*)xar_obtinere(f->paginae,
            (i32)k))->lineae;
        t      = _textus_paginae(f, k);
        notae  = _notae_activae(m, f, k);
    }
    per (i = ZEPHYRUM; lineae && i < xar_numerus(lineae); i++)
    {
        l   = (constans LectorLinea*)xar_obtinere(lineae, i);
        lx  = lateris * NAV_LITTERA + l->x;
        si (   y < l->y || y >= l->y + fons_altitudo(l->fons)
            || x < lx)
        {
            perge;
        }
        columna  = (x - lx) / fons_latitudo(l->fons);
        offset   = l->initium;
        dum (offset < l->finis && columna > ZEPHYRUM)
        {
            offset++;
            si (offset >= l->finis || (t.datum[offset] & 0xC0) != 0x80)
            {
                columna--;
            }
        }
        si (offset >= l->finis)
        {
            redde NIHIL;
        }
        per (j = ZEPHYRUM; notae && j < xar_numerus(notae); j++)
        {
            n = (constans ArticulusNota*)xar_obtinere(notae, j);
            si (   n->genus == ARTICULUS_NEXUS && n->initium <= offset
                && offset < n->finis)
            {
                redde n;
            }
        }
        redde NIHIL;
    }
    redde NIHIL;
}

interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
      NavigatorMontatio* m;
       NavigatorFigurae* privatum;
    constans Praeceptum* t;
     ContextusPraecepti  cp;
                    s32  n;
                    s32  gradus;

    (vacuum)repo;
    (vacuum)motus;
    m         = (NavigatorMontatio*)ctx;
    privatum  = (NavigatorFigurae*)m->figurae_ctx;
    si (!privatum)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    si (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
        && prompta_aperta(&m->ramus, NIHIL))
    {
               chorda textus;
        PromptaExitus x;

        x = prompta_clavem_agere(&m->ramus, actio_navigatoris, ev,
            &textus, privatum->scheda);
        si (x == PROMPTA_REDITUS)
        {
            (vacuum)prompta_claudere(&m->ramus, actio_navigatoris,
                privatum->scheda);
            redde _url_aperire(m, chorda_praecidere(textus),
                privatum->scheda);
        }
        redde x != PROMPTA_NIHIL;
    }
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, _visus(m), ev);
        si (!t)
        {
            redde FALSUM;
        }
        _contextum_parare(m, privatum->scheda, &cp);
        _contextum_eventuum_parare(&cp);
        redde praecepta_status(t, cp.visus, _requisita, &cp)
            == PRAECEPTUM_PRAESTO && t->fn(&cp);
    }
    si (   !nodus || ev->datum.mus.botton != MUS_SINISTER
        || (   ev->genus != EVENTUS_MUS_DEPRESSUS
            && ev->genus != EVENTUS_MUS_DUPLEX))
    {
        redde FALSUM;
    }
    si (   chorda_aequalis_literis(nodus->id, "aperire-url")
        && ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        (vacuum)_notam_scribere(m, chorda_ex_literis("",
            privatum->scheda), privatum->scheda);
        redde prompta_aperire(&m->ramus, actio_navigatoris, "url",
            chorda_ex_literis("", privatum->scheda), privatum->scheda);
    }
    gradus = chorda_aequalis_literis(nodus->id, "pagina-prior") ? -I
        : chorda_aequalis_literis(nodus->id, "pagina-proxima") ? I
        : ZEPHYRUM;
    si (_visus(m) == VISUS_SENTENTIAE)
    {
        /* N6.1: sagittae pedis per sententias; stella ut in pagina */
        si (   chorda_aequalis_literis(nodus->id, "favor")
            && ev->genus == EVENTUS_MUS_DEPRESSUS)
        {
            redde _favorem_vertere(m, _articulus_per_id(m, _campus(m,
                "articulus")), privatum->scheda);
        }
        si (   chorda_aequalis_literis(nodus->id, "textus")
            && ev->genus == EVENTUS_MUS_DEPRESSUS)
        {
            /* N6.3: ictus in nexu sententiae (punctum locale) */
            constans ArticulusNota* n = _nexus_ad_punctum(m, des
                ? des->punctum_locale.x : ev->datum.mus.x
                - nodus->fines.x, des ? des->punctum_locale.y
                : ev->datum.mus.y - nodus->fines.y);

            redde n && _url_aperire(m, chorda_transcribere(n->scopus,
                privatum->scheda), privatum->scheda);
        }
        redde gradus != ZEPHYRUM && ev->genus == EVENTUS_MUS_DEPRESSUS
            && _sententiam_movere(m, gradus);
    }
    si (_visus(m) == VISUS_PAGINAE)
    {
        si (   chorda_aequalis_literis(nodus->id, "favor")
            && ev->genus == EVENTUS_MUS_DEPRESSUS)
        {
            redde _favorem_vertere(m, _articulus_per_id(m, _campus(m,
                "articulus")), privatum->scheda);
        }
        /* N3.5: signa columnae lateralis */
        si (   chorda_aequalis_literis(nodus->id, "columna-intrare")
            && ev->genus == EVENTUS_MUS_DEPRESSUS)
        {
            redde _columnam_intrare(m);
        }
        si (   chorda_aequalis_literis(nodus->id, "columna-redire")
            && ev->genus == EVENTUS_MUS_DEPRESSUS)
        {
            redde _columnam_relinquere(m);
        }
        si (   chorda_aequalis_literis(nodus->id, "textus")
            && ev->genus == EVENTUS_MUS_DEPRESSUS)
        {
            /* N4.2: ictus in nexu per viam URL aperit */
            /* punctum locale (des); aliter ex fine nodi */
            constans ArticulusNota* n = _nexus_ad_punctum(m, des
                ? des->punctum_locale.x : ev->datum.mus.x
                - nodus->fines.x, des ? des->punctum_locale.y
                : ev->datum.mus.y - nodus->fines.y);

            redde n && _url_aperire(m, chorda_transcribere(n->scopus,
                privatum->scheda), privatum->scheda);
        }
        redde gradus != ZEPHYRUM && ev->genus == EVENTUS_MUS_DEPRESSUS
            && _paginam_movere(m, gradus);
    }
    _contextum_parare(m, privatum->scheda, &cp);
    _contextum_eventuum_parare(&cp);
    si (cp.visus == VISUS_RADICIS)
    {
        /* N3.4: ictus eligit, duplex aperit (dominium aut recens),
         * sagittae paginas; "a<k>" post dominia */
        n = nodus->partes == PARTES_ITEM ? _id_numerus(nodus->id, 'd')
            : -I;
        si (nodus->partes == PARTES_ITEM && _id_numerus(nodus->id, 'a')
            > ZEPHYRUM)
        {
            n = _id_numerus(nodus->id, 'a') + (s32)xar_numerus(
                cp.radix.dominia);
        }
        si (n > ZEPHYRUM)
        {
            cp.electus = n - I;
            redde ev->genus == EVENTUS_MUS_DUPLEX
                ? fac_radicis_aperire(&cp) : _radicem_eligere(&cp, n
                    - I);
        }
        redde gradus != ZEPHYRUM && ev->genus == EVENTUS_MUS_DEPRESSUS
            && _radicem_vertere(&cp, gradus);
    }
    n = nodus->partes == PARTES_ITEM ? _id_numerus(nodus->id, 'r') : -I;
    si (cp.visus == VISUS_EVENTUUM)
    {
        /* N3.3b: ictus eligit, duplex aperit, sagittae paginas */
        si (n > ZEPHYRUM)
        {
            cp.electus = n - I;
            redde ev->genus == EVENTUS_MUS_DUPLEX
                ? fac_eventum_aperire(&cp) : _eventum_scribere(&cp, n
                    - I);
        }
        redde gradus != ZEPHYRUM && ev->genus == EVENTUS_MUS_DEPRESSUS
            && _eventus_vertere(&cp, gradus);
    }
    si (n > ZEPHYRUM)
    {
        cp.electus = _positio_indicis(cp.ordo, (i32)(n - I));
        si (ev->genus == EVENTUS_MUS_DUPLEX)
        {
            redde fac_aperire(&cp);
        }
        redde _electum_scribere(&cp, cp.electus);
    }
    si (gradus != ZEPHYRUM && ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        redde _indicem_vertere(&cp, gradus);
    }
    redde FALSUM;
}


/* ==================================================
 * Figura
 * ================================================== */

/* praecidere ad columnas (runae, non octeti) */
interior chorda
_praecidere (
    chorda textus,
       s32 columnae)
{
    i32 i;
    s32 runae;

    runae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si ((textus.datum[i] & 0xC0) != 0x80)
        {
            si (runae == columnae)
            {
                textus.mensura = i;
                redde textus;
            }
            runae++;
        }
    }
    redde textus;
}

interior s32
_runae (
    chorda textus)
{
    i32 i;
    s32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si ((textus.datum[i] & 0xC0) != 0x80)
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
_textum_pingere (
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

interior vacuum
_centrum_pingere (
               Mandata* md,
    constans Componens* c,
                chorda  textus,
            ColorThema  color)
{
    s32 columnae;

    columnae  = c->fines.latitudo / NAV_LITTERA - II;
    textus    = _praecidere(textus, columnae);
    _textum_pingere(md, c->fines.x + (c->fines.latitudo - _runae(textus)
        * NAV_LITTERA) / II, c->fines.y + (c->fines.altitudo
        - NAV_LINEA) / II, textus, FONS_PARVUS, color);
}

/* linea indicis: sinistrum, dextrum obscurum; electa ut lector */
interior vacuum
_lineam_ordinis_pingere (
               Mandata* md,
    constans Componens* c,
                chorda  sinistrum,
                chorda  dextrum,
                   b32  electus)
{
          s32 columnae;
      Punctum p;
      Punctum q;
   ColorThema obscurus;

    obscurus = electus ? COLOR_BACKGROUND : COLOR_TEXT_DIM;
    si (electus)
    {
        figura_ordinem_electum_pingere(md, c->fines);
    }
    alioquin
    {
        p.x = c->fines.x;
        p.y = c->fines.y + c->fines.altitudo - I;
        q.x = c->fines.x + c->fines.latitudo - I;
        q.y = p.y;
        mandata_linea(md, p, q, I, figura_color_thematis(
            COLOR_SUPERFICIES));
    }
    columnae = (c->fines.latitudo - VIII) / NAV_LITTERA
        - (s32)dextrum.mensura - II;
    _textum_pingere(md, c->fines.x + IV, c->fines.y + IV, _praecidere(
        sinistrum, columnae), FONS_PARVUS, electus ? COLOR_BACKGROUND
        : COLOR_TEXT);
    _textum_pingere(md, c->fines.x + c->fines.latitudo - IV
        - (s32)dextrum.mensura * NAV_LITTERA, c->fines.y + IV, dextrum,
        FONS_PARVUS, obscurus);
}

/* ordo indicis: titulus, "N versions" */
interior vacuum
_ordinem_pingere (
                        Mandata* md,
             constans Componens* c,
    constans NavigatorArticulus* a,
                            b32  electus)
{
    character textus[XXIV];
          i32 n;

    n = xar_numerus(a->versiones);
    sprintf(textus, n == I ? "1 version" : "%u versions",
        (insignatus integer)n);
    _lineam_ordinis_pingere(md, c, _titulus_articuli(a),
        chorda_ex_literis(textus, md->piscina), electus);
}

/* "1 bookmark" / "N bookmarks" */
interior chorda
_favores_numerati (
        i32  n,
    Piscina* p)
{
    character textus[XXIV];

    sprintf(textus, n == I ? "1 bookmark" : "%u bookmarks",
        (insignatus integer)n);
    redde chorda_ex_literis(textus, p);
}

/* sagittae et index paginarum (index; pes paginae) */
interior vacuum
_sagittam_pingere (
               Mandata* md,
    constans Componens* f,
                   s32  k,
                   s32  n)
{
    si (chorda_aequalis_literis(f->id, "pagina-prior"))
    {
        figura_sagittam_pingere(md, f->fines, -I, k <= ZEPHYRUM
            ? FIGURA_OBSCURUM : ZEPHYRUM);
    }
    alioquin si (chorda_aequalis_literis(f->id, "pagina-proxima"))
    {
        figura_sagittam_pingere(md, f->fines, I, k >= n - I
            ? FIGURA_OBSCURUM : ZEPHYRUM);
    }
}

/* petitio pendens? (nota "Fetching" obscura, non rubra) */
interior b32
_petens (
    constans NavigatorMontatio* m)
{
    redde m->figurae_ctx
        && ((constans NavigatorFigurae*)m->figurae_ctx)->petitio;
}

interior vacuum
_indicem_pingere (
     NavigatorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
        Album* l;
          Xar* ordo;
    Componens* f;
          s32  e;
          s32  k;
          s32  n;
          s32  r;
          i32  i;
    character  textus[XXXII];

    ordo  = _ordo(m, md->piscina);
    l     = _album(m, ordo, md->piscina);
    e     = _electus(m, ordo);
    k     = _pagina_indicis(l, ordo, e);
    n     = album_paginae(l);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        r = f->partes == PARTES_ITEM ? _id_numerus(f->id, 'r') : -I;
        si (r > ZEPHYRUM)
        {
            _ordinem_pingere(md, f, navigator_articulus(m->bibliotheca,
                (i32)(r - I)), e >= ZEPHYRUM && _index_ordinis(ordo, e)
                == (i32)(r - I));
        }
        alioquin si (_id_numerus(f->id, 'g') >= ZEPHYRUM)
        {
            r = _id_numerus(f->id, 'g');
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _hospes(navigator_articulus(m->bibliotheca,
                _index_ordinis(ordo, r))->url), FONS_PARVUS,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                _praecidere(_campus(m, "dominium"), f->fines.latitudo
                / NAV_LITTERA - I), FONS_TITULI, COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _favores_numerati(xar_numerus(ordo), md->piscina),
                FONS_PARVUS, COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nota"))
        {
            /* linea sua: petitio obscura, error rubra */
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _praecidere(_campus(m, "nota"), f->fines.latitudo
                / NAV_LITTERA - I), FONS_PARVUS, _petens(m)
                ? COLOR_TEXT_DIM : COLOR_ERROR);
        }
        alioquin si (chorda_aequalis_literis(f->id, "aperire-url"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("Open URL...", md->piscina),
                FONS_PARVUS, COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "prompta"))
        {
            prompta_pingere(md, f->fines, "url:", &m->ramus);
        }
        alioquin si (   chorda_aequalis_literis(f->id, "index_paginae")
                     && n > I)
        {
            sprintf(textus, "%d/%d", (integer)(k + I), (integer)n);
            _centrum_pingere(md, f, chorda_ex_literis(textus,
                md->piscina), COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nihil"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("No bookmarks yet", md->piscina),
                FONS_PARVUS, COLOR_TEXT_DIM);
        }
        alioquin
        {
            _sagittam_pingere(md, f, k, n);
        }
    }
}

/* radix (N3.4): "Saved", grex "Bookmarks", linea per dominium cum
 * numero favorum; ceterae partes ut index */
interior vacuum
_radicem_pingere (
     NavigatorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
           Album* l;
    RadixIndicis  radix;
       Componens* f;
             s32  e;
             s32  k;
             s32  n;
             s32  r;
             i32  numerus;
             i32  summa;
             i32  nd;
             i32  i;
       character  textus[XXXII];

    _radicem_parare(m, md->piscina, &radix);
    l      = _album_indicis(m, md->piscina, &k, &numerus);
    e      = _electum_radicis(m, &radix);
    n      = l ? album_paginae(l) : ZEPHYRUM;
    nd     = xar_numerus(radix.dominia);
    summa  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < nd; i++)
    {
        summa += _dominium(radix.dominia, (s32)i)->numerus;
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        r = f->partes == PARTES_ITEM ? _id_numerus(f->id, 'd') : -I;
        si (r > ZEPHYRUM && (i32)r <= nd)
        {
            _lineam_ordinis_pingere(md, f, _dominium(radix.dominia, r
                - I)->titulus, _favores_numerati(_dominium(
                radix.dominia, r - I)->numerus, md->piscina), r - I
                == e);
            perge;
        }
        r = f->partes == PARTES_ITEM ? _id_numerus(f->id, 'a') : -I;
        si (r > ZEPHYRUM && (i32)r <= xar_numerus(radix.recentes))
        {
            /* N3.4b: recens - titulus, hospes obscurus */
            _lineam_ordinis_pingere(md, f, _titulus_articuli(_recens(
                &radix, r - I)), _hospes(_recens(&radix, r - I)->url),
                r - I + (s32)nd == e);
        }
        alioquin si (   chorda_aequalis_literis(f->id, "g1")
                     || chorda_aequalis_literis(f->id, "g2"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(chorda_aequalis_literis(f->id, "g1")
                ? "Recent" : "No bookmarks yet", md->piscina),
                FONS_PARVUS, COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "g0"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("Bookmarks", md->piscina),
                FONS_PARVUS,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Saved", md->piscina), FONS_TITULI,
                COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _favores_numerati(summa, md->piscina), FONS_PARVUS,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nota"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _praecidere(_campus(m, "nota"), f->fines.latitudo
                / NAV_LITTERA - I), FONS_PARVUS, _petens(m)
                ? COLOR_TEXT_DIM : COLOR_ERROR);
        }
        alioquin si (chorda_aequalis_literis(f->id, "aperire-url"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("Open URL...", md->piscina),
                FONS_PARVUS, COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "prompta"))
        {
            prompta_pingere(md, f->fines, "url:", &m->ramus);
        }
        alioquin si (   chorda_aequalis_literis(f->id, "index_paginae")
                     && n > I)
        {
            sprintf(textus, "%d/%d", (integer)(k + I), (integer)n);
            _centrum_pingere(md, f, chorda_ex_literis(textus,
                md->piscina), COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nihil"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("No bookmarks yet", md->piscina),
                FONS_PARVUS, COLOR_TEXT_DIM);
        }
        alioquin
        {
            _sagittam_pingere(md, f, k, n);
        }
    }
}

/* N4.1: color nexuum vici (#rubricae in editore) */
interior ColorMandati
_color_nexus (vacuum)
{
    ColorMandati c;

    c.genus  = COLOR_MANDATI_RGBA;
    c.valor  = color_ad_pixelum(thema_color_ex_indice_colorationis(
        COLORATIO_LINK));
    redde c;
}

/* linea textus per segmenta: partes intra notam NEXUS colore nexuum,
 * ceterae COLOR_TEXT. notae ordine textus (navigator_paginae.h) */
interior vacuum
_lineam_textus_pingere (
                   Mandata* md,
                       s32  x,
                       s32  y,
                    chorda  textus,
      constans LectorLinea* l,
                       Xar* notae,
                       s32  electus)
{
    constans ArticulusNota* n;
                       i32  a;
                       i32  b;
                       i32  i;
                       s32  latitudo;
                     Fines  fundus;

    latitudo  = fons_latitudo(l->fons);
    a         = l->initium;
    per (i = ZEPHYRUM; notae && i < xar_numerus(notae) && a < l->finis;
         i++)
    {
        n = (constans ArticulusNota*)xar_obtinere(notae, i);
        si (   n->genus   != ARTICULUS_NEXUS || n->finis <= a
            || n->initium >= l->finis)
        {
            perge;
        }
        b = n->initium > a ? n->initium : a;
        si (b > a)
        {
            mandata_textus(md, x, y, chorda_transcribere(chorda_sectio(
                textus, a, b), md->piscina), l->fons,
                figura_color_thematis(COLOR_TEXT));
            x += _runae(chorda_sectio(textus, a, b)) * latitudo;
        }
        a = n->finis < l->finis ? n->finis : l->finis;
        si ((s32)n->initium == electus)
        {
            /* N4.3: electus inversus - fundus colore nexuum */
            fundus.x = x;
            fundus.y = y;
            fundus.latitudo  = _runae(chorda_sectio(textus, b, a))
                * latitudo;
            fundus.altitudo  = fons_altitudo(l->fons);
            mandata_rectangulum(md, fundus, _color_nexus(), VERUM);
        }
        mandata_textus(md, x, y, chorda_transcribere(chorda_sectio(
            textus, b, a), md->piscina), l->fons, (s32)n->initium
            == electus ? figura_color_thematis(COLOR_BACKGROUND)
            : _color_nexus());
        x += _runae(chorda_sectio(textus, b, a)) * latitudo;
    }
    si (a < l->finis)
    {
        mandata_textus(md, x, y, chorda_transcribere(chorda_sectio(
            textus, a, l->finis), md->piscina), l->fons,
            figura_color_thematis(COLOR_TEXT));
    }
}

/* L5 (ut lector): locus currens notas habet (hospes)? */
interior b32
_notae_loci (
    NavigatorMontatio* m,
              Piscina* piscina)
{
    redde m->notas_habet && m->notas_habet(m->notas_habet_ctx,
        navigator_locus(m, piscina));
}

/* pagina: lineae articuli; pes "titulus - hospes - k/n" inter sagittas
 * (nota: nuntius loco eius) */
interior vacuum
_paginam_pingere (
     NavigatorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
          NavigatorFigurae* f;
     constans LectorPagina* g;
      constans LectorLinea* l;
                 Componens* x;
                    chorda  nota;
                    chorda  pes;
                       s32  k;
                       s32  n;
                       s32  col;
                       s32  lateris;
                       s32  supra;
                       i32  i;
                       i32  j;
                 character  numeri[XXIV];

    f = _paginas_parare(m);
    si (!f)
    {
        redde;
    }
    k    = _pagina_currens(m, f);
    n    = (s32)xar_numerus(f->paginae);
    g    = (constans LectorPagina*)xar_obtinere(f->paginae, (i32)k);
    col  = _columna_currens(m, f);
    si (col >= ZEPHYRUM)
    {
        /* N3.5: columna lateralis - pagina eius, numeri eius */
        k = _columnae_pagina(m, f, col);
        n = (s32)xar_numerus(_columnae_paginae(f, col));
        g = (constans LectorPagina*)xar_obtinere(_columnae_paginae(f,
            col), (i32)k);
    }
    _margines(m, &lateris, &supra);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        x = componens_liberum(c, i);
        si (chorda_aequalis_literis(x->id, "textus") && col >= ZEPHYRUM)
        {
            per (j = ZEPHYRUM; j < xar_numerus(g->lineae); j++)
            {
                l = (constans LectorLinea*)xar_obtinere(g->lineae, j);
                _lineam_textus_pingere(md, x->fines.x + lateris
                    * NAV_LITTERA + l->x, x->fines.y + l->y,
                    _columna(f, col)->caput.textus, l,
                    _columnae_notae(m, f, col), _nexus_electus(m, f)
                    ? _campus_numerus(m, "nexus") : -I);
            }
        }
        alioquin si (chorda_aequalis_literis(x->id, "textus"))
        {
            per (j = ZEPHYRUM; j < xar_numerus(g->lineae); j++)
            {
                l = (constans LectorLinea*)xar_obtinere(g->lineae, j);
                _lineam_textus_pingere(md, x->fines.x + lateris
                    * NAV_LITTERA + l->x, x->fines.y + l->y,
                    _textus_paginae(f, k), l, _notae_activae(m, f, k),
                    _nexus_electus(m, f) ? _campus_numerus(m, "nexus")
                    : -I);
            }
        }
        alioquin si (chorda_aequalis_literis(x->id, "columna-intrare"))
        {
            figura_sagittam_pingere(md, x->fines, I, ZEPHYRUM);
        }
        alioquin si (chorda_aequalis_literis(x->id, "columna-redire"))
        {
            figura_sagittam_pingere(md, x->fines, -I, ZEPHYRUM);
        }
        alioquin si (   chorda_aequalis_literis(x->id, "pes")
                     && prompta_aperta(&m->ramus, NIHIL))
        {
            prompta_pingere(md, x->fines, "url:", &m->ramus);
        }
        alioquin si (chorda_aequalis_literis(x->id, "pes"))
        {
            nota = _campus(m, "nota");
            sprintf(numeri, " - %d/%d", (integer)(k + I), (integer)n);
            pes = chorda_concatenare(chorda_concatenare(f->titulus,
                chorda_concatenare(chorda_ex_literis(" - ",
                md->piscina),
                f->hospes, md->piscina), md->piscina),
                chorda_ex_literis(
                numeri, md->piscina), md->piscina);
            si (col >= ZEPHYRUM)
            {
                /* N3.5: "Rhubarb - side 1/2" */
                sprintf(numeri, " - side %d/%d", (integer)(k + I),
                    (integer)n);
                pes = chorda_concatenare(f->titulus, chorda_ex_literis(
                    numeri, md->piscina), md->piscina);
            }
            si (_notae_loci(m, md->piscina))
            {
                pes = chorda_concatenare(pes, chorda_ex_literis(
                    " \302\266", md->piscina), md->piscina);
            }
            si (nota.mensura == ZEPHYRUM && _nexus_electus(m, f))
            {
                /* N4.3: scopus nexus electi */
                pes = chorda_concatenare(chorda_ex_literis(
                    "\342\206\222 ", md->piscina), _sine_schemate(
                    _nexus_electus(m, f)->scopus), md->piscina);
            }
            _centrum_pingere(md, x, nota.mensura
                > ZEPHYRUM ? nota : pes,
                nota.mensura > ZEPHYRUM && !_petens(m) ? COLOR_ERROR
                : COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(x->id, "favor"))
        {
            /* N3.4: stella clara (flava) si favor, aliter obscura */
            constans NavigatorArticulus* a = _articulus_per_id(m,
                _campus(m, "articulus"));

            _centrum_pingere(md, x, chorda_ex_literis("\342\230\205",
                md->piscina), a && a->favor ? COLOR_WARNING
                : COLOR_TEXT_DIM);
        }
        alioquin
        {
            _sagittam_pingere(md, x, k, n);
        }
    }
}

/* N6.1 sententia: sola in medio areae (lector_sententiam_disponere);
 * titulus articuli supra; infra "sentence k/n" (" ..." si praecisa);
 * stella et sagittae ut pagina */
interior vacuum
_sententiam_pingere (
     NavigatorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
           NavigatorFigurae* f;
    constans CodexSententia* t;
       constans LectorLinea* l;
                  Componens* x;
                        Xar* lineae;
                     chorda  nota;
                        s32  k;
                        s32  n;
                        s32  lateris;
                        s32  supra;
                        s32  latitudo;
                        s32  altitudo;
                        b32  praecisa;
                        i32  i;
                        i32  j;
                  character  textus[XLVIII];

    f = _paginas_parare(m);
    si (!f || _numerus_sententiarum(f) == ZEPHYRUM)
    {
        redde;
    }
    k         = _sententia_currens(m, f);
    n         = (s32)_numerus_sententiarum(f);
    t         = _sententia(f, k);
    praecisa  = FALSUM;
    _margines(m, &lateris, &supra);
    _area(m, &latitudo, &altitudo);
    lineae = lector_sententiam_disponere(&f->textus->caput, t,
        (i32)latitudo, (i32)altitudo, &praecisa, md->piscina);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        x = componens_liberum(c, i);
        si (chorda_aequalis_literis(x->id, "textus"))
        {
            per (j = ZEPHYRUM; lineae && j < xar_numerus(lineae); j++)
            {
                l = (constans LectorLinea*)xar_obtinere(lineae, j);
                /* N6.3: nexus colorati, electus inversus */
                _lineam_textus_pingere(md, x->fines.x + lateris
                    * NAV_LITTERA + l->x, x->fines.y + l->y,
                    f->textus->caput.textus, l,
                    _notae_articuli_activae(m, f), _nexus_electus(m, f)
                    ? _campus_numerus(m, "nexus") : -I);
            }
        }
        alioquin si (chorda_aequalis_literis(x->id, "caput_paginae"))
        {
            _centrum_pingere(md, x, f->titulus, COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(x->id, "pes"))
        {
            nota = _campus(m, "nota");
            si (nota.mensura == ZEPHYRUM && _nexus_electus(m, f))
            {
                /* N6.3: scopus nexus electi */
                nota = chorda_concatenare(chorda_ex_literis(
                    "\342\206\222 ", md->piscina), _sine_schemate(
                    _nexus_electus(m, f)->scopus), md->piscina);
                _centrum_pingere(md, x, nota, COLOR_TEXT_DIM);
                perge;
            }
            sprintf(textus, "sentence %d/%d%s%s", (integer)(k + I),
                (integer)n, praecisa ? " ..." : "", _notae_loci(m,
                md->piscina) ? " \302\266" : "");
            _centrum_pingere(md, x, nota.mensura > ZEPHYRUM ? nota
                : chorda_ex_literis(textus, md->piscina),
                nota.mensura > ZEPHYRUM && !_petens(m) ? COLOR_ERROR
                : COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(x->id, "favor"))
        {
            constans NavigatorArticulus* a = _articulus_per_id(m,
                _campus(m, "articulus"));

            _centrum_pingere(md, x, chorda_ex_literis("\342\230\205",
                md->piscina), a && a->favor ? COLOR_WARNING
                : COLOR_TEXT_DIM);
        }
        alioquin
        {
            _sagittam_pingere(md, x, k, n);
        }
    }
}

/* ordo eventus: "titulus - descriptio" (descriptio obscura); electus
 * ut lector */
interior vacuum
_eventum_pingere (
                       Mandata* md,
            constans Componens* c,
    constans VicipaediaEventus* e,
                           b32  electus)
{
       chorda textus;
          s32 columnae;
          s32 n;
      Punctum p;
      Punctum q;

    si (electus)
    {
        figura_ordinem_electum_pingere(md, c->fines);
    }
    alioquin
    {
        p.x = c->fines.x;
        p.y = c->fines.y + c->fines.altitudo - I;
        q.x = c->fines.x + c->fines.latitudo - I;
        q.y = p.y;
        mandata_linea(md, p, q, I, figura_color_thematis(
            COLOR_SUPERFICIES));
    }
    columnae  = (c->fines.latitudo - VIII) / NAV_LITTERA;
    textus    = _praecidere(e->titulus, columnae);
    n         = _runae(textus);
    _textum_pingere(md, c->fines.x + IV, c->fines.y + IV, textus,
        FONS_PARVUS, electus ? COLOR_BACKGROUND : COLOR_TEXT);
    si (e->descriptio.mensura > ZEPHYRUM && columnae - n > IV)
    {
        _textum_pingere(md, c->fines.x + IV + n * NAV_LITTERA,
            c->fines.y + IV, _praecidere(chorda_concatenare(
            chorda_ex_literis(" - ", md->piscina), e->descriptio,
            md->piscina), columnae - n), FONS_PARVUS, electus
            ? COLOR_BACKGROUND : COLOR_TEXT_DIM);
    }
}

interior vacuum
_eventus_pingere (
     NavigatorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
        Album* l;
    Componens* f;
          s32  e;
          s32  k;
          s32  n;
          s32  r;
          i32  i;
    character  textus[XXXII];

    l = _eventuum_album(m, md->piscina);
    e = _electus_eventus(m);
    k = e >= ZEPHYRUM ? album_pagina_rei(l, e + I, ZEPHYRUM) : ZEPHYRUM;
    n = album_paginae(l);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        r = f->partes == PARTES_ITEM ? _id_numerus(f->id, 'r') : -I;
        si (r > ZEPHYRUM && (i32)r <= _numerus_eventuum(m))
        {
            _eventum_pingere(md, f, _eventus(m, r - I), r - I == e);
        }
        alioquin si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Results", md->piscina), FONS_TITULI,
                COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            sprintf(textus, _numerus_eventuum(m) == I ? "1 result: "
                : "%u results: ", (insignatus integer)_numerus_eventuum(
                m));
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _praecidere(chorda_concatenare(chorda_concatenare(
                chorda_ex_literis(textus, md->piscina), _campus(m,
                "quaestio"), md->piscina), chorda_concatenare(
                chorda_ex_literis(" (", md->piscina),
                chorda_concatenare(
                _campus(m, "quaestio_hospes"), chorda_ex_literis(")",
                md->piscina), md->piscina), md->piscina), md->piscina),
                f->fines.latitudo / NAV_LITTERA - I), FONS_PARVUS,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nota"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _praecidere(_campus(m, "nota"), f->fines.latitudo
                / NAV_LITTERA - I), FONS_PARVUS, _petens(m)
                ? COLOR_TEXT_DIM : COLOR_ERROR);
        }
        alioquin si (chorda_aequalis_literis(f->id, "prompta"))
        {
            prompta_pingere(md, f->fines, "url:", &m->ramus);
        }
        alioquin si (   chorda_aequalis_literis(f->id, "index_paginae")
                     && n > I)
        {
            sprintf(textus, "%d/%d", (integer)(k + I), (integer)n);
            _centrum_pingere(md, f, chorda_ex_literis(textus,
                md->piscina), COLOR_TEXT_DIM);
        }
        alioquin
        {
            _sagittam_pingere(md, f, k, n);
        }
    }
}

interior vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    NavigatorFigurae* contextus;
               Fines  mensa;

    (vacuum)thema;
    contextus       = (NavigatorFigurae*)ctx;
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    mensa.x         = NAV_LITTERA;
    mensa.y         = NAV_LINEA;
    mensa.latitudo  = c->fines.latitudo - II * NAV_LITTERA;
    mensa.altitudo  = c->fines.altitudo - II * NAV_LINEA;
    mandata_rectangulum(md, mensa, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!contextus || !contextus->m)
    {
        redde;
    }
    si (_visus(contextus->m) == VISUS_PAGINAE)
    {
        _paginam_pingere(contextus->m, c, md);
    }
    alioquin si (_visus(contextus->m) == VISUS_SENTENTIAE)
    {
        _sententiam_pingere(contextus->m, c, md);
    }
    alioquin si (_visus(contextus->m) == VISUS_EVENTUUM)
    {
        _eventus_pingere(contextus->m, c, md);
    }
    alioquin si (_visus(contextus->m) == VISUS_RADICIS)
    {
        _radicem_pingere(contextus->m, c, md);
    }
    alioquin
    {
        _indicem_pingere(contextus->m, c, md);
    }
}

interior b32
_figuras_registrare (
    NavigatorMontatio* m,
                  i32  latitudo,
                  i32  altitudo)
{
    NavigatorFigurae* contextus;

    contextus = (NavigatorFigurae*)piscina_allocare(m->piscina,
        magnitudo(NavigatorFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    memset(contextus, ZEPHYRUM, magnitudo(NavigatorFigurae));
    contextus->m                     = m;
    contextus->scheda                = piscina_generare_dynamicum(
        "navigator_scheda", LXIV * M);
    contextus->arca                  = piscina_generare_dynamicum(
        "navigator_arca", CCLVI * M);
    contextus->arca_eventuum         = piscina_generare_dynamicum(
        "navigator_eventus", LXIV * M);
    contextus->dimittendae           = xar_creare(m->piscina,
        (i32)magnitudo(NavigatorPetitio*));
    contextus->latitudo_montationis  = (s32)latitudo;
    contextus->altitudo_montationis  = (s32)altitudo;
    m->figurae_ctx                   = contextus;
    redde contextus->scheda && contextus->arca && contextus->dimittendae
        && figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
        figura_tabulae, contextus);
}


/* ==================================================
 * Componere (dispositio in cellulis)
 * ================================================== */

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
_locum_addere (
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

interior Componens*
_componentem_locare (
              Disponens* dp,
                    s32  parens,
     constans character* id,
                 Partes  partes,
        DispositioGenus  genus_x,
                    s32  valor_x,
        DispositioGenus  genus_y,
                    s32  lineae)
{
            Componens* c;
     LocusComponentis* l;
                  s32  index;

    index  = _locum_addere(dp, parens, DISPOSITIO_LINEA, genus_x,
        valor_x, genus_y, lineae);
    c      = componens_creare(dp->piscina, dp->intern, id, partes);
    si (!c || index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    componens_ponere_actio(c, actio_navigatoris);
    l = (LocusComponentis*)xar_addere(dp->loci);
    si (l)
    {
        l->index  = index;
        l->c      = c;
    }
    redde c;
}

/* index: titulus, caput (numerus, sagittae, index paginarum), regio
 * ordinum cum titulo gregis iterato */
interior vacuum
_indicem_disponere (
    NavigatorMontatio* m,
            Disponens* dp,
                  s32  radix_d,
              Piscina* piscina)
{
                  Album* l;
     constans AlbumOrdo* o;
                    s32  linea;
                    s32  regio;
                    s32  pagina;
                    i32  numerus;
                    s32  a;
                    s32  b;
                    s32  t;
                    s32  i;

    (vacuum)_componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_TITULI);
    linea = _locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "caput", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "aperire-url",
        PARTES_BOTTONE,
        DISPOSITIO_FIXA, XIII, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "index_paginae",
        PARTES_INDEX,
        DISPOSITIO_FIXA, VII, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        NAV_CAPUT);
    si (_campus(m, "nota").mensura > ZEPHYRUM)
    {
        /* N3.3: nuntius (petitio, error) in linea sua */
        (vacuum)_componentem_locare(dp, radix_d, "nota", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    }
    regio = _locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    si (prompta_aperta(&m->ramus, NIHIL))
    {
        (vacuum)_componentem_locare(dp, radix_d, "prompta",
            PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    }
    /* radix (dominia) aut dominium (favores) - N3.4 */
    l = _album_indicis(m, piscina, &pagina, &numerus);
    si (!l || numerus == ZEPHYRUM)
    {
        (vacuum)_componentem_locare(dp, regio, "nihil", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_ORDO);
        redde;
    }
    album_pagina(l, pagina, &a, &b, &t);
    si (t >= ZEPHYRUM)
    {
        /* pagina in medio gregis: hospes iteratur */
        o = album_ordo(l, (i32)t);
        (vacuum)_componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), PARTES_INDEX, DISPOSITIO_CRESCENS, ZEPHYRUM,
            DISPOSITIO_FIXA, o->altitudo);
    }
    per (i = a; i < b; i++)
    {
        o = album_ordo(l, (i32)i);
        (vacuum)_componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), o->genus
                == ALBUM_RES ? PARTES_ITEM : PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
            o->altitudo);
    }
}

/* eventus: titulus, caput (numerus, sagittae), nota, ordines,
 * prompta - ut index */
interior vacuum
_eventus_disponere (
    NavigatorMontatio* m,
            Disponens* dp,
                  s32  radix_d,
              Piscina* piscina)
{
                  Album* l;
     constans AlbumOrdo* o;
                    s32  linea;
                    s32  regio;
                    s32  a;
                    s32  b;
                    s32  t;
                    s32  e;
                    s32  i;

    (vacuum)_componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_TITULI);
    linea = _locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "caput", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "index_paginae",
        PARTES_INDEX,
        DISPOSITIO_FIXA, VII, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        NAV_CAPUT);
    si (_campus(m, "nota").mensura > ZEPHYRUM)
    {
        (vacuum)_componentem_locare(dp, radix_d, "nota", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    }
    regio = _locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    si (prompta_aperta(&m->ramus, NIHIL))
    {
        (vacuum)_componentem_locare(dp, radix_d, "prompta",
            PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    }
    l = _eventuum_album(m, piscina);
    si (!l || album_numerus(l) == ZEPHYRUM)
    {
        redde;
    }
    e = _electus_eventus(m);
    album_pagina(l, e >= ZEPHYRUM ? album_pagina_rei(l, e + I, ZEPHYRUM)
        : ZEPHYRUM, &a, &b, &t);
    per (i = a; i < b; i++)
    {
        o = album_ordo(l, (i32)i);
        (vacuum)_componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), PARTES_ITEM, DISPOSITIO_CRESCENS, ZEPHYRUM,
            DISPOSITIO_FIXA, o->altitudo);
    }
}

/* pagina: margo superus, area textus (cf. _area), pes cum sagittis */
interior vacuum
_paginam_disponere (
    NavigatorMontatio* m,
            Disponens* dp,
                  s32  radix_d)
{
                 s32  lateris;
                 s32  supra;
                 s32  linea;
    NavigatorFigurae* f;

    _margines(m, &lateris, &supra);
    /* margo superus: [<- columna] caput [columna ->] (N3.5) */
    linea = _locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, supra);
    f = _visus(m) == VISUS_PAGINAE ? _paginas_parare(m) : NIHIL;
    si (f && _columna_currens(m, f) >= ZEPHYRUM)
    {
        (vacuum)_componentem_locare(dp, linea, "columna-redire",
            PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
            supra);
    }
    (vacuum)_componentem_locare(dp, linea, "caput_paginae",
        PARTES_INDEX, DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        supra);
    si (   f && _columna_currens(m, f) < ZEPHYRUM
        && _columna_paginae(f, _pagina_currens(m, f)) >= ZEPHYRUM)
    {
        (vacuum)_componentem_locare(dp, linea, "columna-intrare",
            PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
            supra);
    }
    (vacuum)_componentem_locare(dp, radix_d, "textus", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    /* pes: [sagitta] pes [stella favoris] [sagitta] */
    linea = _locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, supra);
    (vacuum)_componentem_locare(dp, linea, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA, supra);
    (vacuum)_componentem_locare(dp, linea, "pes", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, supra);
    /* III cellulae ut sagittae: _centrum_pingere II marginis demit */
    (vacuum)_componentem_locare(dp, linea, "favor", PARTES_BOTTONE,
        DISPOSITIO_FIXA, III, DISPOSITIO_FIXA, supra);
    (vacuum)_componentem_locare(dp, linea, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA, supra);
}

Componens*
navigator_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    NavigatorMontatio* m;
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

    (vacuum)repo;
    m = (NavigatorMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo = _superficies(m, VERUM);
    altitudo = _superficies(m, FALSUM);
    radix    = componens_creare(piscina, intern, "navigator",
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
    componens_ponere_actio(radix, actio_navigatoris);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    columnae    = latitudo / NAV_LITTERA;
    lineae      = altitudo / NAV_LINEA;
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
    si (_visus(m) == VISUS_PAGINAE || _visus(m) == VISUS_SENTENTIAE)
    {
        _paginam_disponere(m, &dp, radix_d);
    }
    alioquin si (_visus(m) == VISUS_EVENTUUM)
    {
        _eventus_disponere(m, &dp, radix_d, piscina);
    }
    /* index et radix (N3.4) eandem formam habent */
    alioquin
    {
        _indicem_disponere(m, &dp, radix_d, piscina);
    }
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(dp.loci); i++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, (i32)i);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            latitudo, altitudo, NAV_LITTERA, NAV_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    redde radix;
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
navigator_montare (
       NavigatorMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
    NavigatorBibliotheca* bibliotheca,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo || !bibliotheca)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(NavigatorMontatio));
    m->piscina      = piscina;
    m->intern       = intern;
    m->bibliotheca  = bibliotheca;
    m->ramus        = id ? insula_ramus(repo, "navigator", id)
                         : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "navigator",
        "navigator");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"navigator\"", id ? id : "navigator", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_navigatoris, agere, m)
        || !_figuras_registrare(m, latitudo, altitudo))
    {
        redde FALSUM;
    }
    ((NavigatorFigurae*)m->figurae_ctx)->organum = organum;
    redde VERUM;
}


/* ==================================================
 * Semitae et locus
 * ================================================== */

/* SemitaOrganum lateris: manifestum suum; ctx = NavigatorMontatio* */
interior constans Organum*
_organa (
    vacuum* ctx,
    chorda  genus)
{
    NavigatorMontatio* m = (NavigatorMontatio*)ctx;

    si (   !m || !m->figurae_ctx
        || !chorda_aequalis_literis(genus, "navigator"))
    {
        redde NIHIL;
    }
    redde ((NavigatorFigurae*)m->figurae_ctx)->organum;
}

b32
navigator_argumentum_ponere (
      NavigatorMontatio* m,
     constans character* argumentum)
{
     StmlNodus* semita;
        chorda* articulus;
        chorda* versio;
        chorda  vitium;

    si (!m || !m->figurae_ctx)
    {
        redde FALSUM;
    }
    /* N3.3: $nav(URL) - servatus statim, aliter petitio */
    si (argumentum && strstr(argumentum, "://"))
    {
        Piscina* scheda = ((NavigatorFigurae*)m->figurae_ctx)->scheda;

        piscina_vacare(scheda);
        redde _url_aperire(m, chorda_ex_literis(argumentum, scheda),
            scheda);
    }
    /* URI aut cognomen manifesti ("" index, identitas nuda) */
    semita = semita_ex_argumento(_organa, m, "navigator", argumentum
        ? argumentum : "", m->piscina, m->intern, &vitium);
    si (!semita)
    {
        redde FALSUM;
    }
    /* articulus aut versio ignota: recusatur ante scripturam */
    articulus  = stml_attributum_capere(semita, "articulus");
    versio     = stml_attributum_capere(semita, "versio");
    si (   articulus && articulus->mensura > ZEPHYRUM
        && (!_articulus_per_id(m, *articulus)
            || (versio && versio->mensura > ZEPHYRUM
                && !_versio(_articulus_per_id(m, *articulus),
                *versio))))
    {
        redde FALSUM;
    }
    si (!semita_applicare(_organa, m, &m->ramus, semita, m->piscina,
            &vitium))
    {
        redde FALSUM;
    }
    /* N3.4c: articulus sine pagina nominata = ubi relictus - pagina
     * prior (status) tollitur, semitam_secuta servatam restituit */
    si (articulus && articulus->mensura > ZEPHYRUM)
    {
        /* N6.2: sententia quoque - nuda = ubi relictus */
         constans character* tituli[II];
         constans character* valores[II];
                        i32  n;

        n = ZEPHYRUM;
        si (!stml_attributum_capere(semita, "pagina"))
        {
            tituli[n]   = "pagina";
            valores[n]  = "";
            n++;
        }
        si (!stml_attributum_capere(semita, "sententia"))
        {
            tituli[n]   = "sententia";
            valores[n]  = "";
            n++;
        }
        si (n > ZEPHYRUM)
        {
            (vacuum)_campos_scribere(m, tituli, valores, n);
        }
    }
    redde navigator_semitam_secuta(m);
}

b32
navigator_semitam_secuta (
    vacuum* ctx)
{
     NavigatorMontatio* m;
      NavigatorFigurae* f;
                   s32  k;
    constans character* tituli[V];
    constans character* valores[V];
             character  numerus[XVI];

    m = (NavigatorMontatio*)ctx;
    si (   m && m->figurae_ctx && _visus(m) == VISUS_EVENTUUM
        && _numerus_eventuum(m) == ZEPHYRUM)
    {
        /* N3.3b: restitutio in eventibus - quaestio iteratur; sine
         * quaestione ad indicem */
        Piscina* scheda  = ((NavigatorFigurae*)m->figurae_ctx)->scheda;
         chorda  h       = _campus(m, "quaestio_hospes");

        piscina_vacare(scheda);
        si (_campus(m, "quaestio").mensura > ZEPHYRUM)
        {
            redde _quaestionem_incipere(m, h.mensura > ZEPHYRUM ? h
                : chorda_ex_literis("en.wikipedia.org", scheda),
                chorda_transcribere(_campus(m, "quaestio"), scheda),
                scheda);
        }
        tituli[ZEPHYRUM]   = "visus";
        valores[ZEPHYRUM]  = "index";
        redde _campos_scribere(m, tituli, valores, I);
    }
    si (   !m || !m->figurae_ctx
        || (_visus(m) != VISUS_PAGINAE
            && _visus(m) != VISUS_SENTENTIAE))
    {
        redde VERUM;
    }
    f = _paginas_parare(m);
    si (!f)
    {
        /* articulus amissus (restitutio): ad indicem cum nota */
        tituli[ZEPHYRUM]   = "visus";
        valores[ZEPHYRUM]  = "index";
        tituli[I]          = "articulus";
        valores[I]         = "";
        tituli[II]         = "versio";
        valores[II]        = "";
        tituli[III]        = "pagina";
        valores[III]       = "";
        tituli[IV]         = "nota";
        valores[IV]        = "article not found";
        redde _campos_scribere(m, tituli, valores, V);
    }
    si (_visus(m) == VISUS_SENTENTIAE)
    {
        redde _sententiam_restituere(m, f);
    }
    /* pagina coacta; absens (argumentum nudum): servata (N3.4c), et
     * servatur; versio absens figitur (versio in arca = ultima) */
    si (_campus_numerus(m, "pagina") < ZEPHYRUM)
    {
        k = _pagina_servata(m, f);
        _positionem_servare(m, f, k);
        (vacuum)navigator_recentem_notare(m->bibliotheca,
            f->articulus_arcae);
    }
    alioquin
    {
        k = _pagina_currens(m, f);
    }
    si (   k == _campus_numerus(m, "pagina")
        && _campus(m, "versio").mensura > ZEPHYRUM)
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)k);
    tituli[ZEPHYRUM]   = "pagina";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "versio";
    valores[I]         = chorda_ut_cstr(f->versio_arcae, f->scheda);
    redde _campos_scribere(m, tituli, valores, II);
}

chorda
navigator_locus (
     vacuum* ctx,
    Piscina* piscina)
{
     NavigatorMontatio* m;
             StmlNodus* semita;

    m = (NavigatorMontatio*)ctx;
    si (!m || !piscina || !m->figurae_ctx)
    {
        redde _vacua();
    }
    semita = semita_legere(_organa, m, &m->ramus, "navigator", piscina,
        m->intern);
    si (!semita)
    {
        redde _vacua();
    }
    /* index: articulus, versio, pagina iam absunt - omnis scriptor qui
     * ad indicem ducit eos tollit (fac_ad_indicem, semita indicis) */
    redde semita_ad_uri(_organa, m, semita, piscina);
}

chorda
navigator_locus_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
     NavigatorMontatio* m;
      NavigatorFigurae* f;
             character  numeri[XXIV];

    m = (NavigatorMontatio*)ctx;
    si (m && _visus(m) == VISUS_EVENTUUM)
    {
        chorda r;

        r = chorda_concatenare(chorda_ex_literis("Search: ", piscina),
            _campus(m, "quaestio"), piscina);
        r = chorda_concatenare(r, chorda_ex_literis(" (", piscina),
            piscina);
        r = chorda_concatenare(r, _campus(m, "quaestio_hospes"),
            piscina);
        redde chorda_concatenare(r, chorda_ex_literis(")", piscina),
            piscina);
    }
    si (m && _visus(m) == VISUS_SENTENTIAE)
    {
        /* N6.1: "Rhubarb - sentence 12/153" */
        f = _paginas_parare(m);
        si (f && _numerus_sententiarum(f) > ZEPHYRUM)
        {
            sprintf(numeri, " - sentence %d/%u", (integer)(
                _sententia_currens(m, f) + I), (insignatus integer)
                _numerus_sententiarum(f));
            redde chorda_concatenare(f->titulus, chorda_ex_literis(
                numeri, piscina), piscina);
        }
    }
    f = m && _visus(m) == VISUS_PAGINAE ? _paginas_parare(m) : NIHIL;
    si (!f)
    {
        /* N3.4: "Saved" (radix), "Saved - wikipedia.org" (dominium) */
        redde !m || _campus(m, "dominium").mensura == ZEPHYRUM
            ? chorda_ex_literis("Saved", piscina) : chorda_concatenare(
            chorda_ex_literis("Saved - ", piscina), _campus(m,
            "dominium"), piscina);
    }
    si (_columna_currens(m, f) >= ZEPHYRUM)
    {
        /* N3.5: "Rhubarb - side 1/2" */
        sprintf(numeri, " - side %d/%u", (integer)(_columnae_pagina(m,
            f,
            _columna_currens(m, f)) + I),
            (insignatus integer)xar_numerus(
            _columnae_paginae(f, _columna_currens(m, f))));
        redde chorda_concatenare(f->titulus, chorda_ex_literis(numeri,
            piscina), piscina);
    }
    sprintf(numeri, " - %d/%u", (integer)(_pagina_currens(m, f) + I),
        (insignatus integer)xar_numerus(f->paginae));
    redde chorda_concatenare(chorda_concatenare(f->titulus,
        chorda_concatenare(chorda_ex_literis(" - ", piscina), f->hospes,
        piscina), piscina), chorda_ex_literis(numeri, piscina),
        piscina);
}
