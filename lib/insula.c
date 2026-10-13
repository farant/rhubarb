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


/* canon rami (R1; S8 per viam): nodus rami suo canone iudicatur */
nomen structura {
    chorda  via;
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

/* S8: segmentum viae proximum ab indice *i ("elementum:id", '/'
 * sequens consumitur); FALSUM si finis aut segmentum sine ':' */
interior b32
segmentum_legere (
     chorda  via,
        i32* i,
     chorda* elementum,
     chorda* id)
{
    i32 initium;
    i32 colon;
    b32 colon_visum;

    si (*i >= via.mensura)
    {
        redde FALSUM;
    }
    initium      = *i;
    colon        = ZEPHYRUM;
    colon_visum  = FALSUM;
    dum (*i < via.mensura && via.datum[*i] != '/')
    {
        si (via.datum[*i] == ':' && !colon_visum)
        {
            colon        = *i;
            colon_visum  = VERUM;
        }
        (*i)++;
    }
    si (!colon_visum)
    {
        redde FALSUM;
    }
    elementum->datum    = via.datum + initium;
    elementum->mensura  = colon - initium;
    id->datum           = via.datum + colon + I;
    id->mensura         = *i - colon - I;
    si (*i < via.mensura)
    {
        (*i)++;
    }
    redde VERUM;
}

/* nodus est <elementum id="id">? */
interior b32
nodus_est (
     StmlNodus* n,
        chorda  elementum,
        chorda  id)
{
    chorda* v;

    si (   n->genus != STML_NODUS_ELEMENTUM || !n->titulus
        || !chorda_aequalis(*n->titulus, elementum))
    {
        redde FALSUM;
    }
    v = stml_attributum_capere(n, "id");
    redde v && chorda_aequalis(*v, id);
}

/* nodus rami in radice data per viam (S8: quavis profunditate; vacua
 * = radix); NIHIL si quodvis segmentum abest */
interior StmlNodus*
ramum_invenire (
     StmlNodus* radix,
        chorda  via)
{
    StmlNodus* n;
    StmlNodus* liber;
       chorda  elementum;
       chorda  id;
          i32  i;
          i32  j;
          i32  k;

    n = radix;
    i = ZEPHYRUM;
    dum (n && i < via.mensura)
    {
        si (!segmentum_legere(via, &i, &elementum, &id))
        {
            redde NIHIL;
        }
        liber  = NIHIL;
        k      = stml_numerus_liberorum(n);
        per (j = ZEPHYRUM; j < k && !liber; j++)
        {
            si (nodus_est(stml_liberum_ad_indicem(n, j), elementum, id))
            {
                liber = stml_liberum_ad_indicem(n, j);
            }
        }
        n = liber;
    }
    redde n;
}

/* S8: via inferior STRICTE sub superiore? (superior vacua = radix) */
interior b32
via_descendens (
    chorda superior,
    chorda inferior)
{
    si (chorda_vacua(superior))
    {
        redde !chorda_vacua(inferior);
    }
    redde inferior.mensura > superior.mensura + I
        && memcmp(inferior.datum, superior.datum,
               (size_t)superior.mensura) == ZEPHYRUM
        && inferior.datum[superior.mensura] == '/';
}

/* S8: via parentis (segmentum ultimum sublatum; vacua = radix) */
interior chorda
via_parentis (
    chorda via)
{
    chorda r;
       i32 k;

    k = via.mensura;
    dum (k > ZEPHYRUM && via.datum[k - I] != '/')
    {
        k--;
    }
    r.datum    = via.datum;
    r.mensura  = k > ZEPHYRUM ? k - I : ZEPHYRUM;
    redde r;
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
        si (!attributum_mutatum(ramum_invenire(ante, d->via),
                                ramum_invenire(post, d->via),
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
/* viae relativae (Xar de chorda, in p) ramorum cum canone proprio
 * STRICTE sub via data */
interior Xar*
reliqua_sub (
    InsulaRepositorium* repo,
                chorda  via,
               Piscina* p)
{
          Xar* reliqua;
    CanonRami* cr;
       chorda  r;
          i32  j;
          i32  initium;

    reliqua = xar_creare(p, (i32)magnitudo(chorda));
    initium = chorda_vacua(via) ? ZEPHYRUM : via.mensura + I;
    per (j = ZEPHYRUM; j < xar_numerus(repo->canones_ramorum); j++)
    {
        cr = (CanonRami*)xar_obtinere(repo->canones_ramorum, j);
        si (!via_descendens(via, cr->via))
        {
            perge;
        }
        r.datum                        = cr->via.datum + initium;
        r.mensura                      = cr->via.mensura - initium;
        *(chorda*)xar_addere(reliqua)  = r;
    }
    redde reliqua;
}

/* visio canonis (R1; S8 quavis profunditate): nodus sine ramis
 * descendentibus qui canonem proprium habent (reliqua = viae eorum
 * relativae). Copiae superficiales in via ad eos solum - arbor vera
 * intacta; sine reliquis nodus ipse. */
interior StmlNodus*
visio_nodi (
    StmlNodus* nodus,
          Xar* reliqua,
      Piscina* p)
{
    StmlNodus* visio;
    StmlNodus* n;
          Xar* sub;
       chorda* v;
       chorda  elementum;
       chorda  id;
       chorda  r;
          i32  i;
          i32  j;
          i32  k;
          i32  idx;
          b32  delegatus;

    si (!nodus || xar_numerus(reliqua) == ZEPHYRUM)
    {
        redde nodus;
    }
    visio = (StmlNodus*)piscina_allocare(p, magnitudo(StmlNodus));
    *visio = *nodus;
    visio->liberi = xar_creare(p, (i32)magnitudo(StmlNodus*));
    k = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < k; i++)
    {
        n          = stml_liberum_ad_indicem(nodus, i);
        sub        = NIHIL;
        delegatus  = FALSUM;
        per (j = ZEPHYRUM; j < xar_numerus(reliqua) && !delegatus; j++)
        {
            v    = (chorda*)xar_obtinere(reliqua, j);
            idx  = ZEPHYRUM;
            si (   !segmentum_legere(*v, &idx, &elementum, &id)
                || !nodus_est(n, elementum, id))
            {
                perge;
            }
            si (idx >= v->mensura)
            {
                delegatus = VERUM;   /* ramus ipse: suo canone */
                perge;
            }
            si (!sub)
            {
                sub = xar_creare(p, (i32)magnitudo(chorda));
            }
            r.datum                    = v->datum + idx;
            r.mensura                  = v->mensura - idx;
            *(chorda*)xar_addere(sub)  = r;
        }
        si (delegatus)
        {
            perge;
        }
        *(StmlNodus**)xar_addere(visio->liberi) = sub
            ? visio_nodi(n, sub, p) : n;
    }
    redde visio;
}

interior b32
mutare (
     InsulaRepositorium* repo,
                 chorda  via,
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

    scopus = ramum_invenire(duplicatum, via);
    si (!scopus)
    {
        causam_ponere(repo, "ramus abest: nullus nodus in via data");
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
            visio_nodi(duplicatum, reliqua_sub(repo,
                chorda_nulla_insulae(), p), p), p);
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
        nodus_rami = ramum_invenire(duplicatum, cr->via);
        si (!nodus_rami || !cr->canon)
        {
            perge;
        }
        vitia = canon_iudicare(cr->canon, visio_nodi(nodus_rami,
            reliqua_sub(repo, cr->via, p), p), p);
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
    redde mutare(repo, chorda_nulla_insulae(), fn, ctx);
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
                chorda  via,
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
    d->via         = via;
    redde VERUM;
}

b32
insula_dominum_ponere (
    InsulaRepositorium* repo,
    constans character* attributum,
    constans character* dominus)
{
    redde dominum_addere(repo, chorda_nulla_insulae(), attributum,
                         dominus, NIHIL);
}

interior i32
dominos_legere (
    InsulaRepositorium* repo,
                chorda  via,
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
        si (dominum_addere(repo, via,
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
    redde dominos_legere(repo, chorda_nulla_insulae(), domini);
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
    r.via        = chorda_nulla_insulae();
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
    InsulaRamus radix;

    radix = insula_ramus_radix(repo);
    redde insula_ramus_liberi(&radix, elementum, id);
}

InsulaRamus
insula_ramus_liberi (
    constans InsulaRamus* parens,
      constans character* elementum,
      constans character* id)
{
       InsulaRamus  r;
            chorda* e;
            chorda* i;
            chorda* v;
            chorda  t;
         character  memoria[CCLVI];
         character* b;
    memoriae_index  n;
    memoriae_index  le;
    memoriae_index  li;
    memoriae_index  k;

    si (!parens)
    {
        redde insula_ramus_radix(NIHIL);
    }
    r = *parens;
    si (!parens->repo || !elementum || !id)
    {
        redde r;
    }
    /* via = [via parentis '/'] elementum ':' id - internata */
    le  = (memoriae_index)strlen(elementum);
    li  = (memoriae_index)strlen(id);
    n   = (memoriae_index)parens->via.mensura + le + li + II;
    b   = n <= magnitudo(memoria) ? memoria
        : (character*)piscina_allocare(parens->repo->piscina, n);
    k   = ZEPHYRUM;
    si (!chorda_vacua(parens->via))
    {
        memcpy(b, parens->via.datum, (size_t)parens->via.mensura);
        k       = (memoriae_index)parens->via.mensura;
        b[k++]  = '/';
    }
    memcpy(b + k, elementum, (size_t)le);
    k       += le;
    b[k++]  = ':';
    memcpy(b + k, id, (size_t)li);
    k += li;
    t.datum = (i8*)b;
    t.mensura = (i32)k;
    v = chorda_internare(parens->repo->intern, t);
    e = chorda_internare_ex_literis(parens->repo->intern, elementum);
    i = chorda_internare_ex_literis(parens->repo->intern, id);
    si (v && e && i)
    {
        r.via        = *v;
        r.elementum  = *e;
        r.id         = *i;
    }
    redde r;
}

InsulaRamus
insula_ramus_ex_via (
    InsulaRepositorium* repo,
                chorda  via)
{
    InsulaRamus  r;
         chorda  elementum;
         chorda  id;
         chorda* v;
         chorda* e;
         chorda* d;
            i32  i;

    r = insula_ramus_radix(repo);
    si (!repo || chorda_vacua(via))
    {
        redde r;
    }
    /* omnia segmenta "elementum:id" plena; '/' finalis malus */
    i = ZEPHYRUM;
    dum (i < via.mensura)
    {
        si (   !segmentum_legere(via, &i, &elementum, &id)
            || chorda_vacua(elementum) || chorda_vacua(id)
            || (i == via.mensura && via.datum[i - I] == '/'))
        {
            redde insula_ramus_radix(NIHIL);
        }
    }
    v = chorda_internare(repo->intern, via);
    e = chorda_internare(repo->intern, elementum);
    d = chorda_internare(repo->intern, id);
    si (!v || !e || !d)
    {
        redde insula_ramus_radix(NIHIL);
    }
    r.via        = *v;
    r.elementum  = *e;
    r.id         = *d;
    redde r;
}

b32
insula_via_continet (
    chorda via,
    chorda descendens)
{
    si (chorda_vacua(via))
    {
        redde VERUM;
    }
    redde chorda_aequalis(via, descendens)
        || via_descendens(via, descendens);
}

StmlNodus*
insula_ramus_nodus (
    constans InsulaRamus* ramus)
{
    si (!ramus || !ramus->repo)
    {
        redde NIHIL;
    }
    redde ramum_invenire(ramus->repo->radix, ramus->via);
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
    redde mutare(ramus->repo, ramus->via, fn, ctx);
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
    redde dominum_addere(ramus->repo, ramus->via, attributum, dominus,
                         NIHIL);
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
    redde dominos_legere(ramus->repo, ramus->via, domini);
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
          StmlNodus* parens;
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
    si (chorda_vacua(x->ramus->via))
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
    /* S8: sub nodo parentis (exstantia ante portam probata) */
    parens = ramum_invenire(radix, via_parentis(x->ramus->via));
    si (!parens)
    {
        x->bene = FALSUM;
        redde;
    }
    si (!ramum_invenire(radix, x->ramus->via))
    {
        stml_liberum_addere(parens, e);
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
    si (   !chorda_vacua(ramus->via)
        && insula_ramus_nodus(ramus))
    {
        redde VERUM;   /* exstat: nihil mutatur */
    }
    /* S8: parens exstare debet (ante portam - nihil scribitur) */
    si (   !chorda_vacua(ramus->via)
        && !ramum_invenire(ramus->repo->radix,
               via_parentis(ramus->via)))
    {
        causam_ponere(ramus->repo, "ramus parens abest: initiatio"
                                   " sub nodo nullo");
        redde FALSUM;
    }
    x.ramus  = ramus;
    x.stml   = elementum_stml;
    x.bene   = VERUM;
    redde mutare(ramus->repo, chorda_nulla_insulae(), initiatio_mutator,
                 &x)
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
    cr->via    = ramus->via;
    cr->canon  = canon;
}
