/* insula.c - Insula: arbor una, porta una (C2: genera sublata) */

#include "insula.h"
#include "xar.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior StmlNodus*
legere_in (
    InsulaRepositorium* repo,
               Piscina* piscina,
    constans character* cstr)
{
    StmlResultus res;

    res = stml_legere_ex_literis(cstr, piscina, repo->intern);
    si (!res.successus)
    {
        redde NIHIL;
    }
    redde res.elementum_radix;
}

interior vacuum
causam_ponere (
    InsulaRepositorium* repo,
    constans character* litterae)
{
    repo->causa = chorda_ex_literis(litterae, repo->piscina);
}


/* canon rami (R1): nodus <elementum id> suo canone iudicatur */
nomen structura {
    chorda  elementum;
    chorda  id;
     Canon* canon;
} CanonRami;

interior chorda
chorda_nulla_insulae (vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

/* nodus rami in radice data: elementum vacuum = radix; aliter liberum
 * radicis titulo elementi et attributo id; NIHIL si abest */
interior StmlNodus*
ramum_invenire (
     StmlNodus* radix,
        chorda  elementum,
        chorda  id)
{
    StmlNodus* n;
       chorda* v;
          i32  i;
          i32  k;

    si (!radix)
    {
        redde NIHIL;
    }
    si (chorda_vacua(elementum))
    {
        redde radix;
    }
    k = stml_numerus_liberorum(radix);
    per (i = ZEPHYRUM; i < k; i++)
    {
        n = stml_liberum_ad_indicem(radix, i);
        si (   n->genus != STML_NODUS_ELEMENTUM || !n->titulus
            || !chorda_aequalis(*n->titulus, elementum))
        {
            perge;
        }
        v = stml_attributum_capere(n, "id");
        si (v && chorda_aequalis(*v, id))
        {
            redde n;
        }
    }
    redde NIHIL;
}


/* ==================================================
 * Creatio et lectio
 * ================================================== */

InsulaRepositorium*
insula_repositorium_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* stml)
{
    InsulaRepositorium* repo;

    si (!piscina || !intern || !stml)
    {
        redde NIHIL;
    }
    repo = (InsulaRepositorium*)piscina_allocare(piscina,
        magnitudo(InsulaRepositorium));
    si (!repo)
    {
        redde NIHIL;
    }
    memset(repo, ZEPHYRUM, magnitudo(InsulaRepositorium));
    repo->piscina  = piscina;
    repo->intern   = intern;
    repo->piscinae[ZEPHYRUM] = piscina_generare_dynamicum("insula_a",
        XVI * M);
    repo->piscinae[I] = piscina_generare_dynamicum("insula_b", XVI * M);
    si (!repo->piscinae[ZEPHYRUM] || !repo->piscinae[I])
    {
        redde NIHIL;
    }
    repo->activa           = ZEPHYRUM;
    repo->domini           = xar_creare(piscina,
        (i32)magnitudo(InsulaDominus));
    repo->canones_ramorum = xar_creare(piscina,
        (i32)magnitudo(CanonRami));
    repo->radix = legere_in(repo, repo->piscinae[ZEPHYRUM], stml);
    si (!repo->radix)
    {
        causam_ponere(repo, "fons insulae male formatus");
        redde NIHIL;
    }
    repo->textus_ultimus = stml_scribere(repo->radix,
        repo->piscinae[ZEPHYRUM], FALSUM);
    redde repo;
}

StmlNodus*
insula_radix (
    InsulaRepositorium* repo)
{
    si (!repo)
    {
        redde NIHIL;
    }
    redde repo->radix;
}

chorda*
insula_attributum (
    InsulaRepositorium* repo,
    constans character* titulus)
{
    si (!repo)
    {
        redde NIHIL;
    }
    redde stml_attributum_capere(repo->radix, titulus);
}

i32
insula_versio (
    constans InsulaRepositorium* repo)
{
    si (!repo)
    {
        redde ZEPHYRUM;
    }
    redde repo->versio;
}

chorda
insula_scribere (
    InsulaRepositorium* repo,
               Piscina* piscina)
{
    chorda vacua;

    si (!repo)
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    redde stml_scribere(repo->radix, piscina, VERUM);
}


/* ==================================================
 * Domini (brainstorm XVI §2)
 * ================================================== */

/* valor attributi tituli dati aut NIHIL (nodus NIHIL: NIHIL) */
interior chorda*
valor_attributi (
     StmlNodus* n,
        chorda  titulus)
{
    StmlAttributum* a;
               i32  i;
               i32  k;

    si (!n || !n->attributa)
    {
        redde NIHIL;
    }
    k = xar_numerus(n->attributa);
    per (i = ZEPHYRUM; i < k; i++)
    {
        a = (StmlAttributum*)xar_obtinere(n->attributa, i);
        si (a->titulus && chorda_aequalis(*a->titulus, titulus))
        {
            redde a->valor;
        }
    }
    redde NIHIL;
}

/* attributum mutatum? (additum, mutatum, sublatum) */
interior b32
attributum_mutatum (
     StmlNodus* ante,
     StmlNodus* post,
        chorda  titulus)
{
    chorda* a;
    chorda* b;

    a = valor_attributi(ante, titulus);
    b = valor_attributi(post, titulus);
    si (!a && !b)
    {
        redde FALSUM;
    }
    si (!a || !b)
    {
        redde VERUM;
    }
    redde !chorda_aequalis(*a, *b);
}

/* S5a: scriptor inter systemata (spatiis separata, verbum totum)? */
interior b32
inter_systemata (
    chorda systemata,
    chorda scriptor)
{
    i32 i;
    i32 initium;

    si (scriptor.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= systemata.mensura; i++)
    {
        chorda verbum;

        si (i < systemata.mensura && systemata.datum[i] != ' ')
        {
            perge;
        }
        verbum.datum    = systemata.datum + initium;
        verbum.mensura  = i - initium;
        si (chorda_aequalis(verbum, scriptor))
        {
            redde VERUM;
        }
        initium = i + I;
    }
    redde FALSUM;
}

/* causa vacua = licet */
interior chorda
dominos_iudicare (
    InsulaRepositorium* repo,
             StmlNodus* ante,
             StmlNodus* post)
{
    InsulaDominus* d;
              i32  i;
              i32  n;
           chorda  causa;

    n = xar_numerus(repo->domini);
    per (i = ZEPHYRUM; i < n; i++)
    {
        d = (InsulaDominus*)xar_obtinere(repo->domini, i);
        /* R1: attributum in nodo RAMI domini, ante et post */
        si (!attributum_mutatum(ramum_invenire(ante, d->elementum,
            d->id),
                                ramum_invenire(post, d->elementum,
                                d->id),
                                d->attributum))
        {
            perge;
        }
        si (   chorda_aequalis(d->dominus, repo->scriptor)
            || inter_systemata(d->systemata, repo->scriptor))
        {
            perge;
        }
        causa = chorda_ex_literis("dominus: ", repo->piscina);
        causa = chorda_concatenare(causa, d->attributum, repo->piscina);
        causa = chorda_concatenare(causa,
            chorda_ex_literis(" possidetur a ", repo->piscina),
            repo->piscina);
        causa = chorda_concatenare(causa, d->dominus, repo->piscina);
        si (d->systemata.mensura > ZEPHYRUM)
        {
            causa = chorda_concatenare(causa,
                chorda_ex_literis(" (et systematibus: ", repo->piscina),
                repo->piscina);
            causa = chorda_concatenare(causa, d->systemata,
                repo->piscina);
            causa = chorda_concatenare(causa,
                chorda_ex_literis(")", repo->piscina), repo->piscina);
        }
        causa = chorda_concatenare(causa,
            chorda_ex_literis("; scriptor '", repo->piscina),
            repo->piscina);
        causa = chorda_concatenare(causa, repo->scriptor,
            repo->piscina);
        causa = chorda_concatenare(causa,
            chorda_ex_literis("'", repo->piscina), repo->piscina);
        redde causa;
    }
    redde chorda_nulla_insulae();
}


/* ==================================================
 * Portae
 * ================================================== */

/* Scriptura per portam: duplicare in piscinam alteram (circuitus
 * per textum - ipsa disciplina rehydrationis), mutare, canone
 * iudicare, permutare. Recusatio relinquit duplicatum in piscina
 * altera, quae proxima scriptura vacatur. */
/* visio canonis radicis: copia superficialis radicis sine ramis qui
 * canonem proprium habent (R1) - arbor vera intacta */
interior StmlNodus*
visio_radicis (
    InsulaRepositorium* repo,
             StmlNodus* radix,
               Piscina* p)
{
    StmlNodus* visio;
    StmlNodus* n;
    CanonRami* cr;
          i32  i;
          i32  j;
          i32  k;
          b32  delegatus;

    si (xar_numerus(repo->canones_ramorum) == ZEPHYRUM)
    {
        redde radix;
    }
    visio = (StmlNodus*)piscina_allocare(p, magnitudo(StmlNodus));
    *visio = *radix;
    visio->liberi = xar_creare(p, (i32)magnitudo(StmlNodus*));
    k = stml_numerus_liberorum(radix);
    per (i = ZEPHYRUM; i < k; i++)
    {
        n          = stml_liberum_ad_indicem(radix, i);
        delegatus  = FALSUM;
        per (j = ZEPHYRUM; j
            < xar_numerus(repo->canones_ramorum);
             j++)
        {
            cr = (CanonRami*)xar_obtinere(repo->canones_ramorum,
                j);
            si (ramum_invenire(radix, cr->elementum, cr->id) == n)
            {
                delegatus = VERUM;
            }
        }
        si (!delegatus)
        {
            *(StmlNodus**)xar_addere(visio->liberi) = n;
        }
    }
    redde visio;
}

interior b32
mutare (
     InsulaRepositorium* repo,
                 chorda  elementum,
                 chorda  id,
          InsulaMutator  fn,
                 vacuum* ctx)
{
      StmlNodus* scopus;
      CanonRami* cr;
      StmlNodus* nodus_rami;
            i32  j;
            i32  alia;
        Piscina* p;
      StmlNodus* duplicatum;
         chorda  textus;
            Xar* vitia;
    CanonVitium* v;
         chorda  causa_dominorum;

    si (!repo || !fn)
    {
        redde FALSUM;
    }
    alia  = I - repo->activa;
    p     = repo->piscinae[alia];
    piscina_vacare(p);
    textus      = stml_scribere(repo->radix, p, FALSUM);
    duplicatum  = legere_in(repo, p, chorda_ut_cstr(textus, p));
    si (!duplicatum)
    {
        causam_ponere(repo, "duplicatio fracta: arbor non circuit");
        redde FALSUM;
    }

    scopus = ramum_invenire(duplicatum, elementum, id);
    si (!scopus)
    {
        causam_ponere(repo, "ramus abest: nullum liberum radicis"
                            " elemento et id datis");
        redde FALSUM;
    }
    fn(scopus, p, repo->intern, ctx);

    /* domini: attributa radicis mutata contra tabulam dominorum */
    causa_dominorum = dominos_iudicare(repo,
        repo->radix,
                                       duplicatum);
    si (!chorda_vacua(causa_dominorum))
    {
        repo->causa = causa_dominorum;
        redde FALSUM;
    }

    si (repo->canon)
    {
        vitia = canon_iudicare(repo->canon,
            visio_radicis(repo, duplicatum, p), p);
        si (vitia && xar_numerus(vitia) > ZEPHYRUM)
        {
            v = (CanonVitium*)xar_obtinere(vitia, ZEPHYRUM);
            repo->causa = chorda_concatenare(
                chorda_ex_literis("canon recusat scripturam: ",
                                  repo->piscina),
                chorda_ex_literis(canon_nuntius(v->genus),
                                  repo->piscina),
                repo->piscina);
            redde FALSUM;
        }
    }
    /* canones ramorum (R1): quisque nodum suum iudicat */
    per (j = ZEPHYRUM; j
        < xar_numerus(repo->canones_ramorum); j++)
    {
        cr = (CanonRami*)xar_obtinere(repo->canones_ramorum, j);
        nodus_rami = ramum_invenire(duplicatum, cr->elementum, cr->id);
        si (!nodus_rami || !cr->canon)
        {
            perge;
        }
        vitia = canon_iudicare(cr->canon, nodus_rami, p);
        si (vitia && xar_numerus(vitia) > ZEPHYRUM)
        {
            v = (CanonVitium*)xar_obtinere(vitia, ZEPHYRUM);
            repo->causa = chorda_concatenare(
                chorda_ex_literis("canon rami recusat scripturam: ",
                                  repo->piscina),
                chorda_ex_literis(canon_nuntius(v->genus),
                                  repo->piscina),
                repo->piscina);
            redde FALSUM;
        }
    }

    repo->radix           = duplicatum;
    repo->activa          = alia;
    repo->textus_ultimus  = stml_scribere(duplicatum, p, FALSUM);
    repo->versio++;
    si (repo->actarius)
    {
        repo->actarius(duplicatum, repo->actarius_ctx);
    }
    redde VERUM;
}

b32
insula_mutare (
    InsulaRepositorium* repo,
         InsulaMutator  fn,
                vacuum* ctx)
{
    redde mutare(repo, chorda_nulla_insulae(), chorda_nulla_insulae(),
                 fn, ctx);
}

b32
insula_attributum_ponere (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
     constans character* valor)
{
               i32  i;
               i32  numerus;
    StmlAttributum* attr;

    si (!nodus || !intern || !titulus || !valor)
    {
        redde FALSUM;
    }
    si (nodus->attributa)
    {
        numerus = xar_numerus(nodus->attributa);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
            si (   attr && attr->titulus
                && chorda_aequalis_literis(*attr->titulus, titulus))
            {
                attr->valor = chorda_internare_ex_literis(intern,
                    valor);
                redde VERUM;
            }
        }
    }
    redde stml_attributum_addere(nodus, piscina, intern, titulus,
                                 valor);
}


/* ==================================================
 * Restitutio et mendacium
 * ================================================== */

b32
insula_restituere (
    InsulaRepositorium* repo)
{
           i32  alia;
       Piscina* p;
        chorda  nunc;
           b32  honestum;
     StmlNodus* refecta;

    si (!repo)
    {
        redde FALSUM;
    }
    alia  = I - repo->activa;
    p     = repo->piscinae[alia];
    piscina_vacare(p);
    nunc      = stml_scribere(repo->radix, p, FALSUM);
    honestum  = chorda_aequalis(nunc, repo->textus_ultimus);
    /* refacere EX TEXTU HONESTO solo */
    refecta = legere_in(repo, p, chorda_ut_cstr(repo->textus_ultimus,
        p));
    si (!refecta)
    {
        causam_ponere(repo,
            "restitutio fracta: textus honestus non circuit");
        redde FALSUM;
    }
    repo->radix           = refecta;
    repo->textus_ultimus  = stml_scribere(refecta, p, FALSUM);
    repo->activa          = alia;
    repo->mendacium       = honestum ? FALSUM : VERUM;
    si (!honestum)
    {
        causam_ponere(repo, "mendacium: insula extra portam mutata");
    }
    redde honestum;
}

b32
insula_mendacium (
    constans InsulaRepositorium* repo)
{
    si (!repo)
    {
        redde FALSUM;
    }
    redde repo->mendacium;
}

chorda
insula_causa (
    constans InsulaRepositorium* repo)
{
    chorda vacua;

    si (!repo)
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    redde repo->causa;
}

vacuum
insula_ponere_canonem (
    InsulaRepositorium* repo,
                 Canon* canon)
{
    si (!repo)
    {
        redde;
    }
    repo->canon = canon;
}

vacuum
insula_ponere_actarium (
    InsulaRepositorium* repo,
        InsulaActarius  fn,
                vacuum* ctx)
{
    si (!repo)
    {
        redde;
    }
    repo->actarius      = fn;
    repo->actarius_ctx  = ctx;
}

vacuum
insula_scriptorem_ponere (
    InsulaRepositorium* repo,
                chorda  scriptor)
{
    si (!repo)
    {
        redde;
    }
    repo->scriptor = scriptor;
}

interior b32
dominum_addere (
    InsulaRepositorium* repo,
                chorda  elementum,
                chorda  id,
    constans character* attributum,
    constans character* dominus,
    constans character* systemata)
{
    InsulaDominus* d;
           chorda* a;
           chorda* s;
           chorda* y;

    si (!repo || !attributum || !dominus)
    {
        redde FALSUM;
    }
    /* internamentum vacuum = NIHIL: attributum vacuum recusatur */
    a = chorda_internare_ex_literis(repo->intern, attributum);
    s = chorda_internare_ex_literis(repo->intern, dominus);
    si (!a || !s)
    {
        redde FALSUM;
    }
    y = (systemata && systemata[ZEPHYRUM])
        ? chorda_internare_ex_literis(repo->intern, systemata) : NIHIL;
    d              = (InsulaDominus*)xar_addere(repo->domini);
    d->attributum  = *a;
    d->dominus     = *s;
    d->systemata   = y ? *y : chorda_nulla_insulae();
    d->elementum   = elementum;
    d->id          = id;
    redde VERUM;
}

b32
insula_dominum_ponere (
    InsulaRepositorium* repo,
    constans character* attributum,
    constans character* dominus)
{
    redde dominum_addere(repo, chorda_nulla_insulae(),
                         chorda_nulla_insulae(), attributum, dominus,
                         NIHIL);
}

interior i32
dominos_legere (
    InsulaRepositorium* repo,
                chorda  elementum,
                chorda  id,
             StmlNodus* domini)
{
             StmlNodus* n;
                chorda* a;
                chorda* s;
                chorda* y;
                   i32  i;
                   i32  k;
                   i32  lecti;

    si (!repo || !domini)
    {
        redde ZEPHYRUM;
    }
    lecti  = ZEPHYRUM;
    k      = stml_numerus_liberorum(domini);
    per (i = ZEPHYRUM; i < k; i++)
    {
        n = stml_liberum_ad_indicem(domini, i);
        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        /* 'genus' (forma vetus, ante C2) ignoratur */
        a = stml_attributum_capere(n, "attributum");
        s = stml_attributum_capere(n, "scriptor");
        y = stml_attributum_capere(n, "systemata");
        si (!a || !s)
        {
            perge;
        }
        si (dominum_addere(repo, elementum, id,
                chorda_ut_cstr(*a, repo->piscina),
                chorda_ut_cstr(*s, repo->piscina),
                y ? chorda_ut_cstr(*y, repo->piscina) : NIHIL))
        {
            lecti++;
        }
    }
    redde lecti;
}

i32
insula_dominos_legere (
    InsulaRepositorium* repo,
             StmlNodus* domini)
{
    redde dominos_legere(repo, chorda_nulla_insulae(),
                         chorda_nulla_insulae(), domini);
}

b32
insula_attributum_tollere (
              StmlNodus* nodus,
     constans character* titulus)
{
    StmlAttributum* attr;
               i32  i;
               i32  n;

    si (!nodus || !titulus || !nodus->attributa)
    {
        redde FALSUM;
    }
    n = xar_numerus(nodus->attributa);
    per (i = ZEPHYRUM; i < n; i++)
    {
        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
        si (   attr && attr->titulus
            && chorda_aequalis_literis(*attr->titulus, titulus))
        {
            xar_removere_cum_ultimo(nodus->attributa, i);
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Rami (R1)
 * ================================================== */

InsulaRamus
insula_ramus_radix (
    InsulaRepositorium* repo)
{
    InsulaRamus r;

    r.repo       = repo;
    r.elementum  = chorda_nulla_insulae();
    r.id         = chorda_nulla_insulae();
    redde r;
}

InsulaRamus
insula_ramus (
    InsulaRepositorium* repo,
    constans character* elementum,
    constans character* id)
{
    InsulaRamus  r;
         chorda* e;
         chorda* i;

    r = insula_ramus_radix(repo);
    si (!repo || !elementum || !id)
    {
        redde r;
    }
    e = chorda_internare_ex_literis(repo->intern, elementum);
    i = chorda_internare_ex_literis(repo->intern, id);
    si (e && i)
    {
        r.elementum  = *e;
        r.id         = *i;
    }
    redde r;
}

StmlNodus*
insula_ramus_nodus (
    constans InsulaRamus* ramus)
{
    si (!ramus || !ramus->repo)
    {
        redde NIHIL;
    }
    redde ramum_invenire(ramus->repo->radix, ramus->elementum,
                         ramus->id);
}

chorda*
insula_ramus_attributum (
    constans InsulaRamus* ramus,
      constans character* titulus)
{
    StmlNodus* n;

    n = insula_ramus_nodus(ramus);
    redde n ? stml_attributum_capere(n, titulus) : NIHIL;
}

b32
mutare_ramum (
    constans InsulaRamus* ramus,
           InsulaMutator  fn,
                  vacuum* ctx)
{
    si (!ramus)
    {
        redde FALSUM;
    }
    redde mutare(ramus->repo, ramus->elementum, ramus->id, fn,
                 ctx);
}

b32
insula_ramus_dominum_ponere (
    constans InsulaRamus* ramus,
      constans character* attributum,
      constans character* dominus)
{
    si (!ramus)
    {
        redde FALSUM;
    }
    redde dominum_addere(ramus->repo, ramus->elementum,
        ramus->id,
                         attributum, dominus, NIHIL);
}

i32
insula_ramus_dominos_legere (
    constans InsulaRamus* ramus,
               StmlNodus* domini)
{
    si (!ramus)
    {
        redde ZEPHYRUM;
    }
    redde dominos_legere(ramus->repo, ramus->elementum,
        ramus->id,
                         domini);
}

/* initiatio rami (T1a): elementum STML in piscina portae legitur */
nomen structura {
    constans InsulaRamus* ramus;
      constans character* stml;
                     b32  bene;
} InitiatioRami;

interior vacuum
initiatio_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
      InitiatioRami* x;
       StmlResultus  res;
          StmlNodus* e;
     StmlAttributum* attr;
                i32  i;

    x    = (InitiatioRami*)ctx;
    res  = stml_legere_ex_literis(x->stml, p, in);
    si (!res.successus || !res.elementum_radix)
    {
        x->bene = FALSUM;
        redde;
    }
    e = res.elementum_radix;
    si (chorda_vacua(x->ramus->elementum))
    {
        /* radix: attributa absentia sola */
        per (i = ZEPHYRUM; e->attributa
            && i < xar_numerus(e->attributa);
             i++)
        {
            attr = (StmlAttributum*)xar_obtinere(e->attributa, i);
            si (   attr->titulus
                && !valor_attributi(radix, *attr->titulus))
            {
                insula_attributum_ponere(radix, p, in,
                    chorda_ut_cstr(*attr->titulus, p),
                    attr->valor ? chorda_ut_cstr(*attr->valor, p) : "");
            }
        }
        redde;
    }
    si (!ramum_invenire(radix, x->ramus->elementum, x->ramus->id))
    {
        stml_liberum_addere(radix, e);
    }
}

b32
insula_ramum_initiare (
    constans InsulaRamus* ramus,
      constans character* elementum_stml)
{
    InitiatioRami x;

    si (   !ramus || !ramus->repo
        || !elementum_stml)
    {
        redde FALSUM;
    }
    si (   !chorda_vacua(ramus->elementum)
        && insula_ramus_nodus(ramus))
    {
        redde VERUM;   /* exstat: nihil mutatur */
    }
    x.ramus  = ramus;
    x.stml   = elementum_stml;
    x.bene   = VERUM;
    redde mutare(ramus->repo, chorda_nulla_insulae(),
                 chorda_nulla_insulae(), initiatio_mutator, &x)
        && x.bene;
}

vacuum
insula_ramus_canonem_ponere (
    constans InsulaRamus* ramus,
                   Canon* canon)
{
    CanonRami* cr;

    si (!ramus || !ramus->repo || !canon)
    {
        redde;
    }
    cr             = (CanonRami*)xar_addere(
        ramus->repo->canones_ramorum);
    cr->elementum  = ramus->elementum;
    cr->id         = ramus->id;
    cr->canon      = canon;
}
