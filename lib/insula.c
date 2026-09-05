/* insula.c - Insulae: tria genera, porta una per genus */

#include "insula.h"
#include "xar.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
genus_sanum (
    InsulaGenus genus)
{
    redde (i32)genus < (i32)INSULA_GENUS_NUMERUS;
}

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


/* ==================================================
 * Creatio et lectio
 * ================================================== */

InsulaRepositorium*
insula_repositorium_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* durabilis_stml,
     constans character* ephemera_stml)
{
    InsulaRepositorium* repo;
    constans character* fontes[INSULA_GENUS_NUMERUS];
               Piscina* p;
                   i32  g;

    si (!piscina || !intern || !durabilis_stml || !ephemera_stml)
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

    fontes[INSULA_DURABILIS]  = durabilis_stml;
    fontes[INSULA_EPHEMERA]   = ephemera_stml;
    per (g = ZEPHYRUM; g < (i32)INSULA_GENUS_NUMERUS; g++)
    {
        repo->piscinae[g][ZEPHYRUM] =
            piscina_generare_dynamicum("insula_a", XVI * M);
        repo->piscinae[g][I] =
            piscina_generare_dynamicum("insula_b", XVI * M);
        si (!repo->piscinae[g][ZEPHYRUM] || !repo->piscinae[g][I])
        {
            redde NIHIL;
        }
        p                = repo->piscinae[g][ZEPHYRUM];
        repo->activa[g]  = ZEPHYRUM;
        repo->domini[g] = xar_creare(piscina,
                                     (i32)magnitudo(InsulaDominus));
        repo->radices[g]  = legere_in(repo, p, fontes[g]);
        si (!repo->radices[g])
        {
            causam_ponere(repo, "fons insulae male formatus");
            redde NIHIL;
        }
        repo->textus_ultimus[g] =
            stml_scribere(repo->radices[g], p, FALSUM);
    }
    redde repo;
}

StmlNodus*
insula_radix (
    InsulaRepositorium* repo,
           InsulaGenus  genus)
{
    si (!repo || !genus_sanum(genus))
    {
        redde NIHIL;
    }
    redde repo->radices[genus];
}

chorda*
insula_attributum (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* titulus)
{
    si (!repo || !genus_sanum(genus))
    {
        redde NIHIL;
    }
    redde stml_attributum_capere(repo->radices[genus], titulus);
}

i32
insula_versio (
    constans InsulaRepositorium* repo,
                    InsulaGenus  genus)
{
    si (!repo || !genus_sanum(genus))
    {
        redde ZEPHYRUM;
    }
    redde repo->versio[genus];
}

chorda
insula_scribere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
               Piscina* piscina)
{
    chorda vacua;

    si (!repo || !genus_sanum(genus))
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    redde stml_scribere(repo->radices[genus], piscina, VERUM);
}


/* ==================================================
 * Domini (brainstorm XVI §2)
 * ================================================== */

interior chorda
chorda_nulla_insulae (vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

/* valor attributi tituli dati aut NIHIL */
interior chorda*
valor_attributi (
     StmlNodus* n,
        chorda  titulus)
{
    StmlAttributum* a;
               i32  i;
               i32  k;

    si (!n->attributa)
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

/* causa vacua = licet */
interior chorda
dominos_iudicare (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
             StmlNodus* ante,
             StmlNodus* post)
{
    InsulaDominus* d;
              i32  i;
              i32  n;
           chorda  causa;

    n = xar_numerus(repo->domini[genus]);
    per (i = ZEPHYRUM; i < n; i++)
    {
        d = (InsulaDominus*)xar_obtinere(repo->domini[genus], i);
        si (!attributum_mutatum(ante, post, d->attributum))
        {
            perge;
        }
        si (chorda_aequalis(d->dominus, repo->scriptor))
        {
            perge;
        }
        causa = chorda_ex_literis("dominus: ", repo->piscina);
        causa = chorda_concatenare(causa, d->attributum, repo->piscina);
        causa = chorda_concatenare(causa,
            chorda_ex_literis(" possidetur a ", repo->piscina),
            repo->piscina);
        causa = chorda_concatenare(causa, d->dominus, repo->piscina);
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
interior b32
mutare (
     InsulaRepositorium* repo,
            InsulaGenus  genus,
          InsulaMutator  fn,
                 vacuum* ctx)
{
            i32  alia;
        Piscina* p;
      StmlNodus* duplicatum;
         chorda  textus;
            Xar* vitia;
    CanonVitium* v;
         chorda  causa_dominorum;

    si (!repo || !fn || !genus_sanum(genus))
    {
        redde FALSUM;
    }
    alia  = I - repo->activa[genus];
    p     = repo->piscinae[genus][alia];
    piscina_vacare(p);
    textus      = stml_scribere(repo->radices[genus], p, FALSUM);
    duplicatum  = legere_in(repo, p, chorda_ut_cstr(textus, p));
    si (!duplicatum)
    {
        causam_ponere(repo, "duplicatio fracta: arbor non circuit");
        redde FALSUM;
    }

    fn(duplicatum, p, repo->intern, ctx);

    /* domini: attributa radicis mutata contra tabulam dominorum */
    causa_dominorum = dominos_iudicare(repo, genus,
        repo->radices[genus],
                                       duplicatum);
    si (!chorda_vacua(causa_dominorum))
    {
        repo->causa = causa_dominorum;
        redde FALSUM;
    }

    si (repo->canones[genus])
    {
        vitia = canon_iudicare(repo->canones[genus], duplicatum, p);
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

    repo->radices[genus]         = duplicatum;
    repo->activa[genus]          = alia;
    repo->textus_ultimus[genus]  = stml_scribere(duplicatum, p, FALSUM);
    repo->versio[genus]++;
    si (repo->actarius)
    {
        repo->actarius(genus, duplicatum, repo->actarius_ctx);
    }
    redde VERUM;
}

b32
mutare_durabile (
    InsulaRepositorium* repo,
         InsulaMutator  fn,
                vacuum* ctx)
{
    redde mutare(repo, INSULA_DURABILIS, fn, ctx);
}

b32
mutare_ephemera (
    InsulaRepositorium* repo,
         InsulaMutator  fn,
                vacuum* ctx)
{
    redde mutare(repo, INSULA_EPHEMERA, fn, ctx);
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
           i32  g;
           i32  alia;
       Piscina* p;
        chorda  nunc;
           b32  honestum;
     StmlNodus* refecta;

    si (!repo)
    {
        redde FALSUM;
    }
    honestum = VERUM;
    per (g = ZEPHYRUM; g < (i32)INSULA_GENUS_NUMERUS; g++)
    {
        alia  = I - repo->activa[g];
        p     = repo->piscinae[g][alia];
        piscina_vacare(p);
        nunc = stml_scribere(repo->radices[g], p, FALSUM);
        si (!chorda_aequalis(nunc, repo->textus_ultimus[g]))
        {
            honestum = FALSUM;
        }
        /* refacere EX TEXTU HONESTO solo */
        refecta = legere_in(repo, p,
            chorda_ut_cstr(repo->textus_ultimus[g], p));
        si (!refecta)
        {
            causam_ponere(repo,
                "restitutio fracta: textus honestus non circuit");
            redde FALSUM;
        }
        repo->radices[g]         = refecta;
        repo->textus_ultimus[g]  = stml_scribere(refecta, p, FALSUM);
        repo->activa[g]          = alia;
    }
    repo->mendacium = honestum ? FALSUM : VERUM;
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
           InsulaGenus  genus,
                 Canon* canon)
{
    si (!repo || !genus_sanum(genus))
    {
        redde;
    }
    repo->canones[genus] = canon;
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

b32
insula_dominum_ponere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* attributum,
    constans character* dominus)
{
    InsulaDominus* d;
           chorda* a;
           chorda* s;

    si (!repo || !genus_sanum(genus) || !attributum || !dominus)
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
    d              = (InsulaDominus*)xar_addere(repo->domini[genus]);
    d->attributum  = *a;
    d->dominus     = *s;
    redde VERUM;
}

i32
insula_dominos_legere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
             StmlNodus* domini)
{
    constans character* titulus_generis;
             StmlNodus* n;
                chorda* g;
                chorda* a;
                chorda* s;
                   i32  i;
                   i32  k;
                   i32  lecti;

    si (!repo || !domini || !genus_sanum(genus))
    {
        redde ZEPHYRUM;
    }
    titulus_generis = genus
        == INSULA_DURABILIS ? "durabilis" : "ephemera";
    lecti  = ZEPHYRUM;
    k      = stml_numerus_liberorum(domini);
    per (i = ZEPHYRUM; i < k; i++)
    {
        n = stml_liberum_ad_indicem(domini, i);
        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        g = stml_attributum_capere(n, "genus");
        a = stml_attributum_capere(n, "attributum");
        s = stml_attributum_capere(n, "scriptor");
        si (   !g || !a || !s
            || !chorda_aequalis_literis(*g, titulus_generis))
        {
            perge;
        }
        si (insula_dominum_ponere(repo, genus,
                chorda_ut_cstr(*a, repo->piscina),
                chorda_ut_cstr(*s, repo->piscina)))
        {
            lecti++;
        }
    }
    redde lecti;
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
