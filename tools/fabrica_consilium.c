/* fabrica_consilium.c - bin/fabrica consilium: CONSILIUM IUDICATUM et
 * ACTUM (spec project-specs/consilium-spec.md, gradus S3b, S5c1;
 * nucleus lib/consilium.c)
 *
 * Usus: bin/fabrica consilium [-machina] [-agere [-pergere]
 *       [-resumere]] VIA
 *   VIA = documentum consilium.canon (gradus, effectus, regulae).
 *   Effusio: undae (ordo), arcus cum causis, cyclus nominatus,
 *   recusationes, conflictus, iudicia (non LICET); -machina: TSV
 *   (contractus include/consilium.h) + 'SUMMA gradus N undae N
 *   conflictus N vetita N roganda N cyclus N recusationes N'.
 *   -agere (S5c1): gradus MANDATA sua exsequuntur, undis seriatim, per
 *   consilium_exsequi - observati (arbor, ~/.bin, directoria '~/...'
 *   effectuum): scriptura extra propositum gradum frangit. Diarium
 *   build/consilium/acta/<titulus>.tsv (appendendum solum), acta
 *   gradus build/consilium/acta/<titulus>/<gradus>.log. -pergere:
 *   independentes post fracturam pergunt. -resumere: gradus ACTI
 *   cursus prioris non iterantur, si consilium idem et arbor (HEAD +
 *   status fontium, build/ neglecto) non mutata post actum ultimum.
 * Exitus: 0 consilium exsequi licet / omnia acta · 1 non sine iudicio
 *   (conflictus, cyclus, recusatio, VETITUM aut ROGANDUM; -agere:
 *   nihil actum - gradus sine mandato, resumptio recusata) · 2 usus
 *   aut documentum illegibile (causae omnes in stderr) · 3 (-agere)
 *   actum, sed gradus fractus aut omissus.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "processus.h"
#include "sigillum.h"
#include "chorda_aedificator.h"
#include "consilium.h"
#include "fabrica_sutura.h"

#include <stdio.h>
#include <string.h>

/* diarium et acta ipsa sub hoc praefixo: exsecutor ea inter
 * photographias scribit - numquam scriptura gradus */
#define ACTA_CONSILII "build/consilium/acta/"

/* status exsecutoris (datum suturae) */
nomen structura {
                Piscina* piscina;
     constans character* diarium;      /* via diarii */
                 chorda  acta;         /* directorium actorum, '/'
                                        * finali */
                    Xar* directoria;   /* chorda '~/X' observata */
                 chorda  sigillum;
                    b32  resumptum;
                    b32  caput_scriptum;
} ConsiliumAgens;

/* VESTIGIUM ARBORIS (S5b, Franus 2026-10-17): HEAD + differentia
 * fontium sequendorum + plagulae novae non ignoratae (nomen et
 * contentum) - build/ et ignorata extra. Sine git: 'ignota' (resumptio
 * tum non tuta) */
interior chorda
_vestigium_arboris (
    Piscina* piscina)
{
    constans character* constans caput[] = {
        "git", "rev-parse", "HEAD", NIHIL
    };
    constans character* constans differentia[] = {
        "git", "diff", "HEAD", "--binary", NIHIL
    };
    constans character* constans novae[] = {
        "git", "ls-files", "-o", "--exclude-standard", "-z", NIHIL
    };
      SigillumContextus ctx;
      ProcessusResultus r;
               Sigillum s;
              character hex[SIGILLUM_HEX_MENSURA];
                    i32 initium;
                    i32 i;

    sigillum_incipere(&ctx);
    r = processus_exsequi(caput, LX * M, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde chorda_ex_literis("ignota", piscina);
    }
    sigillum_addere(&ctx, r.effusio.datum,
        (memoriae_index)r.effusio.mensura);
    r = processus_exsequi(differentia, LX * M, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde chorda_ex_literis("ignota", piscina);
    }
    sigillum_addere(&ctx, "D", I);
    sigillum_addere(&ctx, r.effusio.datum,
        (memoriae_index)r.effusio.mensura);
    r = processus_exsequi(novae, LX * M, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde chorda_ex_literis("ignota", piscina);
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i < r.effusio.mensura; i++)
    {
        chorda titulus;
        chorda contentum;

        si (r.effusio.datum[i] != '\0')
        {
            perge;
        }
        titulus = chorda_sectio(r.effusio, initium, i);
        initium = i + I;
        sigillum_addere(&ctx, "N", I);
        sigillum_addere(&ctx, titulus.datum,
            (memoriae_index)titulus.mensura
            + I);
        contentum = filum_legere_totum(chorda_ut_cstr(titulus, piscina),
            piscina);
        sigillum_addere(&ctx, contentum.datum,
            (memoriae_index)contentum.mensura);
    }
    s = sigillum_finire(&ctx);
    sigillum_hex(&s, hex);
    redde chorda_ex_literis(hex, piscina);
}

/* titulus gradus in nomen plagulae: [A-Za-z0-9._-], cetera '_' */
interior chorda
_nomen_plagulae (
      chorda  titulus,
     Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);
                  i32 i;

    per (i = ZEPHYRUM; i < titulus.mensura; i++)
    {
        character x = (character)titulus.datum[i];

        chorda_aedificator_appendere_character(a,
               (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')
            || (x >= '0' && x <= '9') || x == '.' || x == '-'
            ? x : '_');
    }
    redde chorda_aedificator_finire(a);
}

interior vacuum
_diario_appendere (
    ConsiliumAgens* a,
            chorda  linea)
{
    si (!a->caput_scriptum)
    {
        /* caput pigrum: consilium recusatum nihil in diario
         * relinquit */
        a->caput_scriptum = VERUM;
        (vacuum)filum_appendere_firmiter(a->diarium,
            consilium_diarium_caput(a->sigillum, _vestigium_arboris(
            a->piscina), a->resumptum, a->piscina));
    }
    si (!filum_appendere_firmiter(a->diarium, linea))
    {
        fprintf(stderr, "consilium -agere: diarium scribi nequit: %s\n",
            a->diarium);
    }
}

interior b32
_agere (
                      vacuum* datum,
    constans ConsiliumGradus* gradus,
                     Piscina* piscina,
              ConsiliumActum* actum)
{
            ConsiliumAgens*  a = (ConsiliumAgens*)datum;
        constans character** argv;
         ProcessusResultus   r;
                    chorda   acta_via;
                    chorda   omnia;
                       i32   i;

    argv = (constans character**)piscina_allocare(piscina,
        (memoriae_index)(xar_numerus(gradus->mandatum) + I)
        * (memoriae_index)magnitudo(character*));
    per (i = ZEPHYRUM; i < xar_numerus(gradus->mandatum); i++)
    {
        argv[i] =
            chorda_ut_cstr(*(chorda*)xar_obtinere(gradus->mandatum,
            i), piscina);
    }
    argv[i]   = NIHIL;
    acta_via  = chorda_concatenare(chorda_concatenare(a->acta,
        _nomen_plagulae(gradus->titulus, piscina), piscina),
        chorda_ex_literis(".log", piscina), piscina);
    r = processus_exsequi((constans character* constans*)argv,
        MORA_SANATIONIS_MS, piscina);
    actum->duratio_ms = r.mora_ms;
    omnia = chorda_concatenare(r.effusio, r.erratum, piscina);
    (vacuum)filum_scribere(chorda_ut_cstr(acta_via, piscina), omnia);
    si (!r.successus)
    {
        actum->codex  = -I;
        actum->cauda  = chorda_concatenare(chorda_ex_literis(
            processus_error_nomen(r.error), piscina),
            chorda_concatenare(chorda_ex_literis(": ", piscina),
            r.error_descriptio, piscina), piscina);
        redde FALSUM;
    }
    actum->codex = r.signum != ZEPHYRUM ? (s32)(CXXVIII + r.signum)
        : (s32)r.codex_exitus;
    si (actum->codex != ZEPHYRUM)
    {
        actum->cauda = chorda_concatenare(chorda_concatenare(
            suturae_ultimae_lineae(omnia, I), chorda_ex_literis(
            " (acta: ", piscina), piscina), chorda_concatenare(acta_via,
            chorda_ex_literis(")", piscina), piscina), piscina);
    }
    redde VERUM;
}

/* photographia per fabricam (arbor + directoria '~/X'), diarium et
 * acta exsecutoris ipsius exclusa */
interior b32
_observare (
     vacuum*  datum,
    Piscina*  piscina,
        Xar** out)
{
    ConsiliumAgens* a = (ConsiliumAgens*)datum;
               Xar* vestigia;
               Xar* observata;
            chorda  praefixum = chorda_ex_literis(ACTA_CONSILII,
                piscina);
               i32 i;

    si (!suturae_photographare(a->directoria, piscina, &vestigia))
    {
        redde FALSUM;
    }
    observata = xar_creare(piscina,
        (i32)magnitudo(ConsiliumObservatio));
    per (i = ZEPHYRUM; i < xar_numerus(vestigia); i++)
    {
        constans FabricaVestigium* v = (constans FabricaVestigium*)
            xar_obtinere(vestigia, i);
             ConsiliumObservatio* o;

        si (chorda_incipit(v->via, praefixum))
        {
            perge;
        }
        o             = (ConsiliumObservatio*)xar_addere(observata);
        o->via        = v->via;
        o->tempus_ns  = v->tempus_ns;
        o->mensura    = v->mensura;
    }
    *out = observata;
    redde VERUM;
}

interior vacuum
_incipit (
                      vacuum* datum,
    constans ConsiliumGradus* gradus,
                         i32  unda)
{
    ConsiliumAgens* a = (ConsiliumAgens*)datum;

    fprintf(stderr, "  agitur %.*s (unda %ld)\n",
        (integer)gradus->titulus.mensura,
        (constans character*)gradus->titulus.datum, (longus)unda);
    _diario_appendere(a, consilium_diarium_incepti(gradus, unda,
        a->piscina));
}

interior vacuum
_notare (
                     vacuum* datum,
    constans ConsiliumActus* actus)
{
    ConsiliumAgens* a = (ConsiliumAgens*)datum;

    _diario_appendere(a, consilium_diarium_actus(actus,
        _vestigium_arboris(a->piscina), a->piscina));
}

/* directoria '~/X' observanda: ~/.bin semper (politica institutionum),
 * et directoria rerum '~/...' quas effectus nominant */
interior Xar*
_directoria_observanda (
    constans Consilium* c,
               Piscina* piscina)
{
    Xar* d = xar_creare(piscina, (i32)magnitudo(chorda));
    i32  i;
    i32  j;
    i32  k;

    *(chorda*)xar_addere(d) = chorda_ex_literis("~/.bin", piscina);
    per (i = ZEPHYRUM; i < xar_numerus(consilium_gradus(c)); i++)
    {
        constans ConsiliumGradus* g = *(ConsiliumGradus**)xar_obtinere(
            consilium_gradus(c), i);

        per (j = ZEPHYRUM; j < xar_numerus(g->effectus); j++)
        {
            chorda res = ((constans ConsiliumEffectus*)xar_obtinere(
                g->effectus, j))->res;
               s32 f;
               b32 iam = FALSUM;

            si (   res.mensura < III || res.datum[ZEPHYRUM] != '~'
                || res.datum[I] != '/')
            {
                perge;
            }
            f = (s32)res.mensura - I;
            dum (f > I && res.datum[f] != '/')
            {
                f--;
            }
            res = chorda_sectio(res, ZEPHYRUM, (i32)f);
            per (k = ZEPHYRUM; k < xar_numerus(d); k++)
            {
                iam = iam
                    || chorda_aequalis(res, *(chorda*)xar_obtinere(
                    d, k));
            }
            si (!iam && res.mensura > II)
            {
                *(chorda*)xar_addere(d) = res;
            }
        }
    }
    redde d;
}

/* -agere: consilium exsequi (S5c1) */
interior s32
_agere_consilium (
     constans character* via,
              Consilium* c,
           constans Xar* regulae,
                    b32  machina,
                    b32  pergere,
                    b32  resumere,
                Piscina* piscina)
{
           ConsiliumAgens  a;
          ConsiliumSutura  s;
                   chorda  titulus;
                   chorda  causa;
                   chorda  effusio;
       constans character* basis;
       constans character* punctum;
                      Xar* facta = NIHIL;
                      Xar* acta;
                      Xar* sine;
                      i32  i;
                      i32  mala = ZEPHYRUM;

    sine = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(consilium_gradus(c)); i++)
    {
        constans ConsiliumGradus* g = *(ConsiliumGradus**)xar_obtinere(
            consilium_gradus(c), i);

        si (xar_numerus(g->mandatum) == ZEPHYRUM && !g->factum)
        {
            *(chorda*)xar_addere(sine) = g->titulus;
        }
    }
    si (xar_numerus(sine) > ZEPHYRUM)
    {
        fprintf(stderr, "consilium -agere: gradus sine mandato (non "
            "exsequibiles) - nihil actum:\n");
        per (i = ZEPHYRUM; i < xar_numerus(sine); i++)
        {
            chorda t = *(chorda*)xar_obtinere(sine, i);

            fprintf(stderr, "  %.*s\n", (integer)t.mensura,
                (constans character*)t.datum);
        }
        redde I;
    }
    basis    = strrchr(via, '/');
    basis    = basis != NIHIL ? basis + I : via;
    punctum  = strrchr(basis, '.');
    titulus  = chorda_ex_literis(basis, piscina);
    si (punctum != NIHIL && punctum != basis)
    {
        titulus = chorda_sectio(titulus, ZEPHYRUM,
            (i32)(punctum - basis));
    }
    memset(&a, ZEPHYRUM, magnitudo(a));
    a.piscina     = piscina;
    a.acta        =
        chorda_concatenare(chorda_concatenare(chorda_ex_literis(
        ACTA_CONSILII, piscina), titulus, piscina),
        chorda_ex_literis("/",
        piscina), piscina);
    a.diarium     =
        chorda_ut_cstr(chorda_concatenare(chorda_concatenare(
        chorda_ex_literis(ACTA_CONSILII, piscina), titulus, piscina),
        chorda_ex_literis(".tsv", piscina), piscina), piscina);
    a.directoria  = _directoria_observanda(c, piscina);
    a.sigillum    = consilium_sigillum(c, regulae, piscina);
    a.resumptum   = resumere;
    si (!filum_directorium_creare_cum_parentibus(chorda_ut_cstr(a.acta,
            piscina)))
    {
        fprintf(stderr, "consilium -agere: directorium actorum creari "
            "nequit: %.*s\n", (integer)a.acta.mensura,
            (constans character*)a.acta.datum);
        redde II;
    }
    si (resumere)
    {
        chorda arbor = _vestigium_arboris(piscina);

        si (chorda_aequalis_literis(arbor, "ignota"))
        {
            fprintf(stderr, "consilium -agere -resumere: RECUSATIO: "
                "vestigium arboris ignotum (git?) - resumptio non "
                "tuta, nihil actum\n");
            redde I;
        }
        si (!consilium_diarium_resumere(filum_existit(a.diarium)
                ? filum_legere_totum(a.diarium, piscina)
                : chorda_ex_literis("", piscina), a.sigillum, arbor,
                piscina, &facta, &causa))
        {
            fprintf(stderr,
                "consilium -agere -resumere: RECUSATIO: %.*s "
                "(%s) - nihil actum\n", (integer)causa.mensura,
                (constans character*)causa.datum, a.diarium);
            redde I;
        }
        fprintf(stderr, "consilium: resumptio - %ld gradus iam acti "
            "(%s)\n", (longus)xar_numerus(facta), a.diarium);
    }
    s.datum      = &a;
    s.agere      = _agere;
    s.observare  = _observare;
    s.incipit    = _incipit;
    s.notare     = _notare;
    acta = consilium_exsequi(c, regulae, &s, pergere ? CONSILIUM_PERGERE
        : CONSILIUM_SISTERE, facta, piscina, &causa);
    si (acta == NIHIL)
    {
        fprintf(stderr,
            "consilium -agere: RECUSATIO %s - nihil actum:\n"
            "%.*s", via, (integer)causa.mensura,
            (constans character*)causa.datum);
        redde I;
    }
    effusio = consilium_acta_scribere(acta, machina, piscina);
    fwrite(effusio.datum, I, (size_t)effusio.mensura, stdout);
    si (!machina)
    {
        printf("diarium: %s\n", a.diarium);
    }
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        si (((constans ConsiliumActus*)xar_obtinere(acta, i))->eventus
            != CONSILIUM_ACTUM)
        {
            mala++;
        }
    }
    redde mala > ZEPHYRUM ? III : ZEPHYRUM;
}

s32
suturae_consilium (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
    constans character* via       = NIHIL;
                   b32  machina   = FALSUM;
                   b32  agere     = FALSUM;
                   b32  pergere   = FALSUM;
                   b32  resumere  = FALSUM;
                   b32  malum     = FALSUM;
             Consilium* c;
                   Xar* regulae  = NIHIL;
                   Xar* conflictus;
                   Xar* iudicia;
         ConsiliumOrdo* ordo;
                chorda  causa;
                chorda  effusio;
                   i32  vetita   = ZEPHYRUM;
                   i32  roganda  = ZEPHYRUM;
                   s32  i;

    per (i = II; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-agere") == ZEPHYRUM)
        {
            agere = VERUM;
        }
        alioquin si (strcmp(argv[i], "-pergere") == ZEPHYRUM)
        {
            pergere = VERUM;
        }
        alioquin si (strcmp(argv[i], "-resumere") == ZEPHYRUM)
        {
            resumere = VERUM;
        }
        alioquin si (argv[i][ZEPHYRUM] != '-' && via == NIHIL)
        {
            via = argv[i];
        }
        alioquin
        {
            malum = VERUM;
        }
    }
    si (via == NIHIL || malum || ((pergere || resumere) && !agere))
    {
        fprintf(stderr,
            "usus: bin/fabrica consilium [-machina] [-agere "
            "[-pergere] [-resumere]] VIA\n");
        redde II;
    }
    si (!filum_existit(via))
    {
        fprintf(stderr, "consilium: documentum absens: %s\n", via);
        redde II;
    }
    c = consilium_legere(filum_legere_totum(via, piscina), piscina,
        &regulae, &causa);
    si (c == NIHIL)
    {
        fprintf(stderr, "consilium: RECUSATIO %s:\n%.*s\n", via,
            (integer)causa.mensura, (constans character*)causa.datum);
        redde II;
    }
    si (agere)
    {
        redde _agere_consilium(via, c, regulae, machina, pergere,
            resumere, piscina);
    }
    ordo        = consilium_ordinare(c, piscina);
    conflictus  = consilium_conflictus(c, ordo, piscina);
    iudicia     = consilium_iudicare(c, regulae, piscina);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(iudicia); i++)
    {
        constans ConsiliumIudicium* j = (constans ConsiliumIudicium*)
            xar_obtinere(iudicia, (i32)i);

        si (j->sententia == CONSILIUM_VETITUM)
        {
            vetita++;
        }
        alioquin si (j->sententia == CONSILIUM_ROGANDUM)
        {
            roganda++;
        }
    }
    effusio = consilium_scribere(c, ordo, conflictus, iudicia, machina,
        piscina);
    fwrite(effusio.datum, I, (size_t)effusio.mensura, stdout);
    printf(machina
        ? "SUMMA\tgradus\t%ld\tundae\t%ld\tconflictus\t%ld\tvetita\t%ld"
          "\troganda\t%ld\tcyclus\t%ld\trecusationes\t%ld\n"
        : "summa: %ld gradus, %ld undae, %ld conflictus, %ld vetita,"
          " %ld roganda, cyclus %ld, %ld recusationes\n",
        (longus)xar_numerus(consilium_gradus(c)),
        (longus)xar_numerus(ordo->undae),
        (longus)xar_numerus(conflictus),
        (longus)vetita, (longus)roganda,
        (longus)xar_numerus(ordo->cyclus),
        (longus)xar_numerus(ordo->recusationes));
    redde (   xar_numerus(conflictus) > ZEPHYRUM || vetita > ZEPHYRUM
           || roganda > ZEPHYRUM || xar_numerus(ordo->cyclus) > ZEPHYRUM
           || xar_numerus(ordo->recusationes)
               > ZEPHYRUM) ? I : ZEPHYRUM;
}
