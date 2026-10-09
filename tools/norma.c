/* norma.c (instrumentum) - plagulae .norma (norma-spec-2 §III.3)
 *
 * Verba:
 *   norma c <x.norma> -praefixum P -caput <h> -corpus <c>
 *   norma iudicare <x.norma> <valor.json> [-norma <titulus>]
 *   norma inferre [-titulus T] [-status N] [-electio] [-fines]
 *                 [-sine_formis] [-clausum] [-sine_testimoniis] <via>...
 *     (norma-spec-3 §V; testimonia ut commenta, norma-spec-4 §III)
 *     via = plagula .json (exemplum unum) aut acervus herbarii
 *     (specimina status N, ordinarie CC); adumbratio .norma in stdout,
 *     'N exempla, M omissa' in stderr.
 * Exitus: 0 bene / validum; 1 invalidum (iudicare); 2 usus, lectio
 * fracta, recusatio, nullum exemplum (nuntius in stderr). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "json.h"
#include "norma.h"
#include "norma_stml.h"
#include "norma_ad_c.h"
#include "norma_inferre.h"
#include "herbarium.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior s32
_usus (vacuum)
{
    fputs("usus: norma c <x.norma> -praefixum P -caput <h> -corpus <c>\n"
          "      norma iudicare <x.norma> <valor.json> [-norma <titulus>]\n"
          "      norma inferre [-titulus T] [-status N] [-electio] [-fines]\n"
          "                    [-sine_formis] [-clausum] [-sine_testimoniis]"
          " <via>...\n",
          stderr);
    redde II;
}

interior b32
_legere (
    constans character* via,
       NormaStmlLectio* l,
               Piscina* p)
{
    chorda fons = filum_legere_totum(via, p);
       i32 i;

    si (fons.mensura == 0)
    {
        fprintf(stderr, "norma: %s legi non potuit\n", via);
        redde FALSUM;
    }
    *l = norma_stml_legere(fons, NIHIL, 0, p);
    per (i = 0; i < xar_numerus(l->vitia); i++)
    {
        NormaStmlVitium* v = (NormaStmlVitium*)xar_obtinere(l->vitia,
            i);

        fprintf(stderr, "%s:%u:%u: %.*s\n", via,
            (insignatus integer)v->linea,
            (insignatus integer)v->columna, (integer)v->nuntius.mensura,
            (constans character*)v->nuntius.datum);
    }
    redde l->successus;
}

interior NormaNominata*
_tabula (
        Xar* x,
    Piscina* p)
{
    NormaNominata* t = (NormaNominata*)piscina_allocare(p,
        (memoriae_index)((xar_numerus(x)
            + I) * magnitudo(NormaNominata)));

    xar_copiare_ad_tabulam(x, t, 0, xar_numerus(x));
    redde t;
}

interior s32
_c (
       integer   argc,
     character** argv,
       Piscina*  p)
{
     constans character* praefixum  = NIHIL;
     constans character* caput      = NIHIL;
     constans character* corpus     = NIHIL;
     constans character* nomen_capitis;
        NormaStmlLectio  l;
             NormaFonsC  fons;
                 chorda  causa;
                integer  i;

    per (i = III; i + I < argc; i += II)
    {
        si (strcmp(argv[i], "-praefixum") == 0) praefixum = argv[i + I];
        alioquin si (strcmp(argv[i], "-caput") == 0) caput = argv[i
                                                         + I];
        alioquin si (strcmp(argv[i], "-corpus") == 0) corpus = argv[i
                                                          + I];
        alioquin redde _usus();
    }
    si (argc < III || !praefixum || !caput || !corpus)
    {
        redde _usus();
    }
    si (!_legere(argv[II], &l, p))
    {
        redde II;
    }
    nomen_capitis = strrchr(caput, '/') ? strrchr(caput, '/')
        + I : caput;
    fons = norma_ad_c(_tabula(l.normae, p), xar_numerus(l.normae),
        praefixum, nomen_capitis, argv[II], p, &causa);
    si (fons.corpus.mensura == 0)
    {
        fprintf(stderr, "norma c: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde II;
    }
    si (   !filum_scribere(caput, fons.caput) || !filum_scribere(corpus,
            fons.corpus))
    {
        fprintf(stderr, "norma c: scribere non potuit\n");
        redde II;
    }
    redde 0;
}

interior s32
_iudicare (
       integer   argc,
     character** argv,
       Piscina*  p)
{
     constans character* titulus = NIHIL;
        NormaStmlLectio  l;
           JsonResultus  j;
                  Norma* n;
          NormaIudicium  iud;
                    i32  i;

    si (argc < IV)
    {
        redde _usus();
    }
    si (argc >= VI && strcmp(argv[IV], "-norma") == 0)
    {
        titulus = argv[V];
    }
    si (!_legere(argv[II], &l, p))
    {
        redde II;
    }
    n = titulus ? norma_stml_quaerere(&l, titulus)
        : (xar_numerus(l.normae) > 0
           ? ((NormaNominata*)xar_obtinere(l.normae,
           0))->norma : NIHIL);
    j = json_legere(filum_legere_totum(argv[III], p), p);
    si (!n || !j.successus)
    {
        fprintf(stderr, "norma iudicare: %s\n", !n ? "norma abest"
            : "valor JSON fractus");
        redde II;
    }
    iud = norma_iudicare(n, j.radix, p);
    per (i = 0; i < xar_numerus(iud.vitia)
        + xar_numerus(iud.notae); i++)
    {
                b32  vit = i < xar_numerus(iud.vitia);
        NormaVitium* v = (NormaVitium*)xar_obtinere(vit ? iud.vitia
            : iud.notae, vit ? i : i - xar_numerus(iud.vitia));

        printf("%s %.*s %s: %.*s\n", vit ? "VITIUM" : "NOTA",
            (integer)v->via.mensura, (constans character*)v->via.datum,
            norma_causa_descriptio(v->causa),
            (integer)v->nuntius.mensura,
            (constans character*)v->nuntius.datum);
    }
    redde iud.validum ? 0 : I;
}

/* exemplum (corpus JSON) addere; FALSUM si non JSON */
interior b32
_exemplum_addere (
     Inferentia* inf,
         chorda  corpus,
        Piscina* p)
{
    JsonResultus j = json_legere(corpus, p);

    si (!j.successus)
    {
        redde FALSUM;
    }
    inferentia_addere(inf, j.radix);
    redde VERUM;
}

interior s32
_inferre (
       integer   argc,
     character** argv,
       Piscina*  p)
{
     InferentiaOptiones  o        = inferentia_optiones_ordinariae();
     constans character* titulus  = "responsum";
                    i32  status   = CC;
             Inferentia* inf;
                  Norma* n;
          NormaNominata  nn;
                 chorda  causa;
                 chorda  scriptum;
                    i32  omissa             = 0;
                    b32  testimonia_rogata  = VERUM;
   InferentiaTestimonia* testimonia         = NIHIL;
                integer  i;

    per (i = II; i < argc && argv[i][0] == '-'; i++)
    {
        si (strcmp(argv[i], "-titulus") == 0 && i + I < argc)
        {
            titulus = argv[++i];
        }
        alioquin si (strcmp(argv[i], "-status") == 0 && i + I < argc)
        {
            status = (i32)atoi(argv[++i]);
        }
        alioquin si (strcmp(argv[i], "-electio") == 0)
        {
            o.electio = VERUM;
        }
        alioquin si (strcmp(argv[i], "-fines") == 0)
        {
            o.fines = VERUM;
        }
        alioquin si (strcmp(argv[i], "-sine_formis") == 0)
        {
            o.formae = FALSUM;
        }
        alioquin si (strcmp(argv[i], "-clausum") == 0)
        {
            o.modus = NORMA_CLAUSUM;
        }
        alioquin si (strcmp(argv[i], "-sine_testimoniis") == 0)
        {
            testimonia_rogata = FALSUM;
        }
        alioquin
        {
            redde _usus();
        }
    }
    si (i >= argc)
    {
        redde _usus();
    }
    inf = inferentia_creare(p, &o);
    per (; i < argc; i++)
    {
        Xar* sp = herbarium_enumerare(p, argv[i]);

        si (xar_numerus(sp) > 0)
        {
            i32 k;

            per (k = 0; k < xar_numerus(sp); k++)
            {
                HerbariumSpecimen* h =
                    (HerbariumSpecimen*)xar_obtinere(sp,
                    k);

                si (h->status != status)
                {
                    perge;
                }
                si (!_exemplum_addere(inf, h->corpus, p))
                {
                    omissa++;
                }
            }
        }
        alioquin
        {
            chorda corpus = filum_legere_totum(argv[i], p);

            si (   corpus.mensura == 0
                || !_exemplum_addere(inf, corpus, p))
            {
                fprintf(stderr,
                    "norma inferre: %s omissum (non JSON aut"
                    " legi non potuit)\n", argv[i]);
                omissa++;
            }
        }
    }
    fprintf(stderr, "norma inferre: %u exempla, %u omissa\n",
        (insignatus integer)inferentia_numerus(inf),
        (insignatus integer)omissa);
    n = inferentia_normam_testatam(inf, p, &testimonia);
    si (!n)
    {
        redde II;
    }
    nn.titulus  = chorda_ex_literis(titulus, p);
    nn.norma    = n;
    scriptum    = norma_stml_scribere_cum_commentis(&nn, I, p, &causa,
        testimonia_rogata ? inferentia_commentarius : NIHIL,
        testimonia);
    si (scriptum.mensura == 0)
    {
        fprintf(stderr, "norma inferre: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde II;
    }
    fwrite(scriptum.datum, I, (size_t)scriptum.mensura, stdout);
    fputc('\n', stdout);
    redde 0;
}

s32
principale (
       integer   argc,
     character** argv)
{
     Piscina* p = piscina_generare_dynamicum("norma", M * M);
         s32  exitus;

    si (argc < II)
    {
        exitus = _usus();
    }
    alioquin si (strcmp(argv[I], "c") == 0)
    {
        exitus = _c(argc, argv, p);
    }
    alioquin si (strcmp(argv[I], "iudicare") == 0)
    {
        exitus = _iudicare(argc, argv, p);
    }
    alioquin si (strcmp(argv[I], "inferre") == 0)
    {
        exitus = _inferre(argc, argv, p);
    }
    alioquin
    {
        exitus = _usus();
    }
    piscina_destruere(p);
    redde exitus;
}
