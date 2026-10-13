/* insula_mensura.c - pretium scripturae insulae (terminal-planning
 * park 010; status-plan S8d)
 *
 * Porta scripturae radicem TOTAM per textum copiat et iudicat. Hic
 * microsecundae per scripturam metiuntur:
 *   vicus     applicatio vera (vicus_applicatio), N tabulae (scriba |
 *             pictor in acervo; X summum), scriptura foci per
 *             dispensatorem (ut T5: MM scripturae per ordinem)
 *   gradus    organum ludicrum (fixa/organum/ludibrium.stml in
 *             probationes), catena d graduum, scriptura 'valor' in
 *             gradu ultimo
 *   ordines   ludibrium parens cum k liberis (ordines tabulae),
 *             scriptura 'valor' in liberum unum
 * Mensura octetorum = insula_scribere. Tempus per clock() (tempus
 * processus), minimum trium cursuum.
 *
 * Usus: ./bin/aedilis tools/insula_mensura.c && bash
 *   build/aedilis/insula_mensura/struere.sh &&
 *   ./build/aedilis/insula_mensura/insula_mensura
 * Profilum: ... insula_mensura -ansa vicus|gradus|ordines <n> & sample
 *   <pid> 10 (ansa XX secundas currit).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "volumen.h"
#include "insula.h"
#include "organum.h"
#include "organum_montatio.h"
#include "dispensator.h"
#include "vicus_applicatio.h"
#include "thema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VIA_LUDIBRII  "probationes/fixa/organum/ludibrium.stml"
#define ITERATIONES   MM
#define CURSUS        III

/* mundus unius ordinis: scriptura una per vocationem */
nomen structura {
               Piscina* piscina;
    InsulaRepositorium* repo;
           Dispensator* d;          /* vicus: focus per dispensatorem */
           InsulaRamus  scopus;     /* ludibrium: ramus scriptus */
               Volumen* volumen;
                   i32  numerus;    /* scripturae factae */
                   i32  rami;
} Mundus;

interior vacuum
incrementare (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    chorda* a;
       s32  v;

    (vacuum)ctx;
    a = stml_attributum_capere(radix, "valor");
    v = ZEPHYRUM;
    si (a)
    {
        (vacuum)chorda_ut_s32(*a, &v);
    }
    insula_attributum_ponere(radix, p, in, "valor",
        chorda_ut_cstr(chorda_ex_s32(v + I, p), p));
}

/* scriptura una; valor semper mutatur (domini mutata sola iudicant) */
interior b32
scribere (
    Mundus* m)
{
    m->numerus++;
    si (m->d)
    {
        dispensator_focus_ponere(m->d, chorda_ex_literis(
            (m->numerus & I) ? "a" : "b", m->piscina));
        redde VERUM;
    }
    insula_scriptorem_ponere(m->repo, chorda_ex_literis(
        "ludibrium.agere", m->piscina));
    redde mutare_ramum(&m->scopus, incrementare, NIHIL);
}

/* vicus vera cum n tabulis (scriba | pictor), focus in tabula prima */
interior b32
vicum_parare (
                 Mundus* m,
                    i32  n,
    InternamentumChorda* in)
{
       VicusApplicatio* app;
     ChordaAedificator* a;
                   i32  i;

    m->volumen = volumen_temporarium(m->piscina, "insula_mensura");
    si (!m->volumen)
    {
        redde FALSUM;
    }
    a = chorda_aedificator_creare(m->piscina, IV * M);
    chorda_aedificator_appendere_literis(a, "<tabulae activa=\"1\">");
    per (i = I; i <= n; i++)
    {
        chorda_aedificator_appendere_literis(a, "<tabula id=\"");
        chorda_aedificator_appendere_s32(a, (s32)i);
        chorda_aedificator_appendere_literis(a,
            "\"><latus genus=\"scriba\"/><acervus>"
            "<latus genus=\"pictor\"/></acervus></tabula>");
    }
    chorda_aedificator_appendere_literis(a, "</tabulae>");
    si (!volumen_plagulam_condere(m->volumen, chorda_ex_literis(
            "vicus/latera", m->piscina),
            chorda_aedificator_finire(a), "insula_mensura"))
    {
        redde FALSUM;
    }
    app = (VicusApplicatio*)piscina_allocare(m->piscina,
        magnitudo(VicusApplicatio));
    si (!vicus_applicatio_aedificare(app, m->piscina, in, m->volumen,
            CDLXXX, CDLXXX))
    {
        redde FALSUM;
    }
    m->repo  = app->vicus->repo;
    m->d     = app->d;
    /* tabulae VERE apertae (vicus suas numerat) */
    m->rami  = II * vicus_numerus_tabularum(app->vicus);
    redde VERUM;
}

/* ludibrium: catena d graduum (ordines FALSUM) aut parens cum d
 * liberis (ordines VERUM); scopus = imus / liberum ultimum */
interior b32
ludibrium_parare (
                 Mundus* m,
                    i32  n,
                    b32  ordines,
    InternamentumChorda* in)
{
        Organum* o;
    InsulaRamus  radix;
    InsulaRamus  parens;
    InsulaRamus  r;
      character  id[XVI];
            i32  i;

    o       = organum_legere_viam(m->piscina, in, NIHIL, VIA_LUDIBRII,
        "insula_mensura");
    m->repo = insula_repositorium_creare(m->piscina, in, "<hospes/>");
    si (!o || !m->repo)
    {
        redde FALSUM;
    }
    radix   = insula_ramus_radix(m->repo);
    parens  = radix;
    per (i = ZEPHYRUM; i < n + (ordines ? I : ZEPHYRUM); i++)
    {
        sprintf(id, "r%d", (integer)i);
        r = insula_ramus_liberi(&parens, "ludibrium", id);
        si (!organum_ramum_parare(o, &r, NIHIL, NIHIL,
                chorda_ut_cstr(r.via, m->piscina), m->piscina, in))
        {
            redde FALSUM;
        }
        /* ordines: primus parens, ceteri liberi eius */
        si (!ordines || i == ZEPHYRUM)
        {
            parens = r;
        }
        m->scopus = r;
    }
    m->rami = (i32)n + (ordines ? I : ZEPHYRUM);
    redde VERUM;
}

interior b32
parare (
                 Mundus* m,
     constans character* genus,
                    i32  n)
{
    InternamentumChorda* in;

    memset(m, ZEPHYRUM, magnitudo(*m));
    m->piscina = piscina_generare_dynamicum("insula_mensura",
        CCLVI * M);
    si (!m->piscina)
    {
        redde FALSUM;
    }
    in = internamentum_creare(m->piscina);
    si (strcmp(genus, "vicus") == ZEPHYRUM)
    {
        redde vicum_parare(m, n, in);
    }
    redde ludibrium_parare(m, n, strcmp(genus, "ordines") == ZEPHYRUM,
        in);
}

interior vacuum
dimittere (
    Mundus* m)
{
    si (m->volumen)
    {
        volumen_claudere(m->volumen);
    }
    piscina_destruere(m->piscina);
}

/* ordo tabulae: microsecundae per scripturam (minimum trium) */
interior vacuum
ordinem_metiri (
     constans character* genus,
                    i32  n)
{
      Mundus m;
     clock_t initium;
      duplex mora;
      duplex optimum;
         i32 c;
         i32 i;
         b32 ok;

    optimum  = -1.0;
    ok       = VERUM;
    per (c = ZEPHYRUM; c < CURSUS && ok; c++)
    {
        si (!parare(&m, genus, n))
        {
            imprimere("%-8s %4u  PARARE DEFECIT\n", genus, n);
            redde;
        }
        initium = clock();
        per (i = ZEPHYRUM; i < ITERATIONES && ok; i++)
        {
            ok = scribere(&m);
        }
        mora = (duplex)(clock() - initium) * 1.0e6 / CLOCKS_PER_SEC
            / ITERATIONES;
        si (c == ZEPHYRUM)
        {
            imprimere("%-8s %4u  rami %4u  octeti %7u  ", genus, n,
                m.rami, insula_scribere(m.repo, m.piscina).mensura);
        }
        si (optimum < 0.0 || mora < optimum)
        {
            optimum = mora;
        }
        dimittere(&m);
    }
    si (!ok)
    {
        imprimere("SCRIPTURA RECUSATA\n");
        redde;
    }
    imprimere("%8.1f us\n", optimum);
}

/* ansa pro 'sample': XX secundae scripturarum */
interior s32
ansa (
     constans character* genus,
                    i32  n)
{
      Mundus m;
     clock_t finis;

    si (!parare(&m, genus, n))
    {
        imprimere("parare defecit\n");
        redde I;
    }
    imprimere("ansa %s %u: XX s\n", genus, n);
    fflush(stdout);
    finis = clock() + (clock_t)(XX * CLOCKS_PER_SEC);
    dum (clock() < finis)
    {
        si (!scribere(&m))
        {
            imprimere("scriptura recusata\n");
            redde I;
        }
    }
    dimittere(&m);
    redde ZEPHYRUM;
}

s32
principale (
          s32   argc,
    character** argv)
{
    hic_manens constans i32 tabulae[]  = { II, III, VI, X };
    hic_manens constans i32 gradus[]   = { I, III, VI, XI, XXII };
    hic_manens constans i32 ordines[]  = { II, XI, L, C, CC };
                        i32 i;

    thema_initiare();
    si (argc == IV && strcmp(argv[I], "-ansa") == ZEPHYRUM)
    {
        redde ansa(argv[II], (i32)atoi(argv[III]));
    }
    imprimere("genus       n  rami        octeti         per scripturam"
        "\n");
    /* vicus X tabulas summum aperit (Cmd+1..0) */
    per (i = ZEPHYRUM; i < IV; i++)
    {
        ordinem_metiri("vicus", tabulae[i]);
    }
    per (i = ZEPHYRUM; i < V; i++)
    {
        ordinem_metiri("gradus", gradus[i]);
    }
    per (i = ZEPHYRUM; i < V; i++)
    {
        ordinem_metiri("ordines", ordines[i]);
    }
    redde ZEPHYRUM;
}
