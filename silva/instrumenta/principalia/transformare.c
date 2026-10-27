/* transformare.c - TRANSFORMARE: formae vocationum super arbores silvae
 * (spec project-specs/transformare-spec.md, plan transformare-plan.md)
 *
 * Usus:  ./silva/transformare.sh -census <functio> <viae...> [-machina]
 *        ./silva/transformare.sh -regulae <regulae.stml> <viae...>
 *                                [-machina]
 *
 * -census: omnes vocationes functionis in plagulis datis (directoria
 *   recursive: .c et .h; occulta, build, node_modules omissa), per
 *   FORMAM consumentis gregatae et numeratae cum sedibus (forma =
 *   silva_situs_forma: clavis post expansionem, ergo 'ZEPHYRUM' et
 *   '0' idem; scriptura situs primi ostenditur, aliter scripti
 *   numerantur). Vocationes a macro factae separatim ('in macro',
 *   numquam rescribuntur). Iudicium = recipe examinis (silva_iudicium:
 *   lexicon, capita repositorii, oraculum) - 'remove(v);' vocatio est,
 *   non declaratio.
 * -regulae: CONSILIUM (nihil scribitur). Regulae STML:
 *     <transformatio functio="remove">
 *       <regula>
 *         <si! (>remove($via) != ZEPHYRUM
 *         <fac! (>!filum_delere($via)
 *       </regula>
 *     </transformatio>
 *   per situm regula prima congruens (silva_rescriptiones_parare);
 *   differentiae unificatae in stdout (patch legit), summa et situs
 *   recusati per formam et causam in stderr.
 * -scribere: post consilium per plagulam CUSTODIA (silva_emendatio):
 *   iudicium examinis super textum novum (codex regularum, e.g.
 *   codex="97", exacte tot situs perdit quot rescripti; nihil aliud
 *   crescit; REICE non inducitur), clang -fsyntax-only ante (non
 *   compilans = non verificabilis, recusatur) et post; deinde scribit.
 *   Fractura = plagula intacta, causa nominata. Formator non currit
 *   (commissio plagulas totas format).
 * <inclusio>filum.h</inclusio> in regulis: plagula rescripta sine eo
 *   '#include "filum.h"' accipit (post '#include "..."' ultimam ante
 *   rescriptionem primam). Plagulae GENERATUM (linea prima) numquam
 *   rescribuntur - generator eiusve fontes.
 * -machina: TSV, linea una per situm (ordine plagularum):
 *     situs  <via> <linea> <columna> <clavis> <scriptura>     (census)
 *     macro  <via> <linea> <columna> <macro>
 *     facta|recusata <via> <linea> <columna> <regula> <causa> <clavis>
 *     fracta <via> <causa>
 *
 * Exitus census: 0 situs inventi | 1 nulli; consilium: 0 omnes situs
 * rescriberentur | 1 aliqui recusati; 2 usus aut apparatus.
 * Ex radice repositorii currendum (transformare.sh eo migrat).
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "iter_directoria.h"
#include "internamentum.h"
#include "stml.h"
#include "differentia.h"
#include "silva_parsare.h"
#include "silva_forma.h"
#include "silva_iudicium.h"
#include "silva_emendatio.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
    constans character* via;
                   i32  linea;
                   i32  columna;
                   b32  in_macro;
                chorda  clavis;      /* in_macro: nomen macro */
                chorda  scriptura;
                   s32  verdictum;   /* SilvaRescriptioVerdictum; -1 */
                   s32  regula;      /* index regulae; -1 nulla */
} CensusSitus;

nomen structura {
    chorda  clavis;      /* in_macro: 'macro|' + nomen */
    chorda  scriptura;   /* situs primi; in_macro: nomen macro */
       b32  in_macro;
       s32  verdictum;   /* -1 census */
       i32  aliter;      /* situs aliter scripti quam primus */
       Xar* situs;       /* i32: indices in censum */
} CensusForma;

hic_manens b32 machina_volo  = FALSUM;
hic_manens b32 scribere_volo = FALSUM;

/* -scribere: codex scopi (regulae codex="97"; -1 nullus) et vexilla
 * clang post '--' (transformare.sh vexilla domus et radices tradit) */
hic_manens s32 codex_scopi = -I;
hic_manens constans character* constans* vexilla_clang = NIHIL;

/* <inclusio>filum.h</inclusio>: caput quod substitutio postulat;
 * plagulae rescriptae sine eo linea '#include "caput"' additur */
hic_manens chorda inclusio_regularum;


/* ==================================================
 * Plagulae colligendae
 * ================================================== */

interior b32
_praetermittendum (
    chorda titulus)
{
    redde (titulus.mensura > ZEPHYRUM && titulus.datum[ZEPHYRUM] == '.')
        || chorda_aequalis_literis(titulus, "build")
        || chorda_aequalis_literis(titulus, "node_modules");
}

interior b32
_fons_c (
    chorda titulus)
{
    redde titulus.mensura >= II
        && titulus.datum[titulus.mensura - II] == '.'
        && (   titulus.datum[titulus.mensura - I] == 'c'
            || titulus.datum[titulus.mensura - I] == 'h');
}

/* via + '/' + titulus (NUL-terminata; './' praefixum omissum) */
interior character*
_viam_iungere (
                Piscina* piscina,
     constans character* via,
                 chorda  titulus)
{
    memoriae_index  m = strlen(via);
    memoriae_index  t = (memoriae_index)titulus.mensura;
         character* p;

    dum (m > I && via[m - I] == '/')
    {
        m--;
    }
    p = (character*)piscina_allocare(piscina, m + t + II);
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    si (m == I && via[ZEPHYRUM] == '.')
    {
        memcpy(p, titulus.datum, t);
        p[t] = '\0';
        redde p;
    }
    memcpy(p, via, m);
    p[m] = '/';
    memcpy(p + m + I, titulus.datum, t);
    p[m + I + t] = '\0';
    redde p;
}

interior vacuum
_directorium_colligere (
               Piscina* piscina,
    constans character* via,
                   Xar* viae,
                   i32  gradus)
{
     DirectoriumIterator* iter;
    DirectoriumIntroitus* e;

    si (gradus > DIRECTORIUM_PROFUNDITAS_MAXIMA)
    {
        redde;
    }
    iter = directorium_iterator_aperire(via, piscina);
    si (iter == NIHIL)
    {
        redde;
    }
    dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
    {
         character* plena;
               b32  directorium;

        si (_praetermittendum(e->titulus))
        {
            perge;
        }
        directorium = e->genus == INTROITUS_DIRECTORIUM
            && !e->est_nexus;
        si (   !directorium
            && !(e->genus == INTROITUS_FILUM && _fons_c(e->titulus)))
        {
            perge;
        }
        plena = _viam_iungere(piscina, via, e->titulus);
        si (plena == NIHIL)
        {
            perge;
        }
        si (directorium)
        {
            _directorium_colligere(piscina, plena, viae, gradus + I);
        }
        alioquin
        {
            character** locus = (character**)xar_addere(viae);

            si (locus != NIHIL)
            {
                *locus = plena;
            }
        }
    }
    directorium_iterator_claudere(iter);
}

interior integer
_vias_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde strcmp(*(constans character* constans*)a,
        *(constans character* constans*)b);
}

/* viae ordinatae, unicae (Xar -> tabula contigua) */
interior character**
_vias_ordinare (
    Piscina* piscina,
        Xar* viae,
        i32* numerus_out)
{
          i32   n = xar_numerus(viae);
    character** tabula;
          i32   k;
          i32   m = ZEPHYRUM;

    *numerus_out = ZEPHYRUM;
    tabula = (character**)piscina_allocare(piscina,
        (memoriae_index)(n > ZEPHYRUM ? n : I) * magnitudo(character*));
    si (tabula == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        tabula[k] = *(character**)xar_obtinere(viae, k);
    }
    qsort(tabula, (memoriae_index)n, magnitudo(character*),
        _vias_comparare);
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (   m                                == ZEPHYRUM
            || strcmp(tabula[m - I], tabula[k]) != ZEPHYRUM)
        {
            tabula[m++] = tabula[k];
        }
    }
    *numerus_out = m;
    redde tabula;
}


/* ==================================================
 * Census
 * ================================================== */

/* campus TSV: tabulae et lineae novae in spatia */
interior vacuum
_campum_scribere (
    chorda c)
{
    i32 k;

    per (k = ZEPHYRUM; k < c.mensura; k++)
    {
        character x = (character)c.datum[k];

        fputc((x == '\t' || x == '\n' || x == '\r') ? ' ' : x, stdout);
    }
}

interior vacuum
_fractam_nuntiare (
    constans character* via,
    constans character* causa)
{
    si (machina_volo)
    {
        imprimere("fracta\t%s\t%s\n", via, causa);
    }
    alioquin
    {
        fprintf(stderr, "transformare: %s: %s\n", via, causa);
    }
}

/* regulae ex plagula STML; NIHIL = fractura nuntiata in stderr */
interior Xar*
_regulas_legere (
                Piscina* piscina,
     constans character* via,
                 chorda* functio_out)
{
               character* textus;
                     i32  mensura = ZEPHYRUM;
            StmlResultus  res;
                     Xar* liberi;
                     Xar* regulae;
                  chorda* functio;
                     i32  k;

    textus = silva_iudicium_plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "transformare: regulae illegibiles: %s\n", via);
        redde NIHIL;
    }
    res = stml_legere(chorda_ex_buffer((i8*)textus, mensura), piscina,
        internamentum_creare(piscina));
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        fprintf(stderr, "transformare: %s:%u: STML fractum: %.*s\n",
            via,
            (insignatus integer)res.linea_erroris,
            (integer)res.error.mensura,
            (constans character*)res.error.datum);
        redde NIHIL;
    }
    si (   res.elementum_radix->titulus == NIHIL
        || !chorda_aequalis_literis(*res.elementum_radix->titulus,
               "transformatio"))
    {
        fprintf(stderr, "transformare: %s: radix <transformatio> esse"
            " debet\n", via);
        redde NIHIL;
    }
    {
        StmlNodus* inclusio = stml_invenire_liberum(res.elementum_radix,
            "inclusio");

        inclusio_regularum.mensura  = ZEPHYRUM;
        inclusio_regularum.datum    = NIHIL;
        si (inclusio != NIHIL)
        {
            inclusio_regularum = stml_textus_valor(inclusio, piscina);
        }
    }
    functio = stml_attributum_capere(res.elementum_radix, "functio");
    si (functio == NIHIL || functio->mensura == ZEPHYRUM)
    {
        fprintf(stderr, "transformare: %s: <transformatio functio=...>"
            " deest\n", via);
        redde NIHIL;
    }
    *functio_out  = *functio;
    {
        chorda* codex = stml_attributum_capere(res.elementum_radix,
            "codex");

        si (codex != NIHIL)
        {
            s32 numerus = ZEPHYRUM;
            i32 j;

            per (j = ZEPHYRUM; j < codex->mensura; j++)
            {
                si (codex->datum[j] < '0' || codex->datum[j] > '9')
                {
                    fprintf(stderr,
                        "transformare: %s: codex numerus esse"
                        " debet (e.g. codex=\"97\")\n", via);
                    redde NIHIL;
                }
                numerus = numerus * X + (s32)(codex->datum[j] - '0');
            }
            codex_scopi = numerus;
        }
    }
    liberi        = stml_invenire_omnes_liberos(res.elementum_radix,
        "regula", piscina);
    regulae       = xar_creare(piscina, (i32)magnitudo(SilvaRegula*));
    si (   liberi              == NIHIL || regulae == NIHIL
        || xar_numerus(liberi) == ZEPHYRUM)
    {
        fprintf(stderr, "transformare: %s: nulla <regula>\n", via);
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(liberi); k++)
    {
          StmlNodus* r      = *(StmlNodus**)xar_obtinere(liberi, k);
          StmlNodus* forma  = stml_invenire_liberum(r, "si");
          StmlNodus* subst  = stml_invenire_liberum(r, "fac");
             chorda  causa;
        SilvaRegula* regula;

        si (forma == NIHIL || subst == NIHIL)
        {
            fprintf(stderr, "transformare: %s: regula %u: <si> et <fac>"
                " requiruntur\n", via, (insignatus integer)k);
            redde NIHIL;
        }
        causa  = chorda_ex_literis("", piscina);
        regula = silva_regulam_parare(piscina,
            stml_textus_valor(forma, piscina),
            stml_textus_valor(subst, piscina), &causa);
        si (regula == NIHIL)
        {
            fprintf(stderr, "transformare: %s: regula %u: %.*s\n", via,
                (insignatus integer)k, (integer)causa.mensura,
                (constans character*)causa.datum);
            redde NIHIL;
        }
        *(SilvaRegula**)xar_addere(regulae) = regula;
    }
    redde regulae;
}

interior vacuum
_lineam_addere (
                      Xar* census,
                  Piscina* piscina,
                  Piscina* opus,
       constans character* via,
    constans SilvaParsura* parsura,
      constans SilvaSitus* s,
       constans character* fons,
                      i32  mensura,
                      s32  verdictum,
                      s32  regula)
{
    CensusSitus* r = (CensusSitus*)xar_addere(census);
         chorda  clavis;
         chorda  scriptura;

    si (r == NIHIL)
    {
        redde;
    }
    r->via        = via;
    r->linea      = s->linea;
    r->columna    = s->columna;
    r->in_macro   = s->in_macro;
    r->verdictum  = verdictum;
    r->regula     = regula;
    r->scriptura  = chorda_ex_literis("", piscina);
    si (s->in_macro)
    {
        r->clavis = chorda_transcribere(s->macro, piscina);
    }
    alioquin si (silva_situs_forma(opus, parsura, s, fons, mensura,
                 &clavis, &scriptura))
    {
        r->clavis     = chorda_transcribere(clavis, piscina);
        r->scriptura  = chorda_transcribere(scriptura, piscina);
    }
    alioquin
    {
        r->clavis     = chorda_ex_literis("?|forma ignota", piscina);
        r->scriptura  = chorda_ex_literis("(forma ignota)", piscina);
    }
}

/* initium rescriptionis FACTAE primae (ordine fontis) */
interior s32
_limes_primus (
    constans Xar* rescriptiones)
{
    s32 limes = -I;
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(rescriptiones); k++)
    {
        constans SilvaRescriptio* r = (constans SilvaRescriptio*)
            xar_obtinere(rescriptiones, k);

        si (   r->verdictum == SILVA_RESCRIPTIO_FACTA
            && (limes < ZEPHYRUM || r->initium < limes))
        {
            limes = r->initium;
        }
    }
    redde limes;
}

/* linea prima 'GENERATUM' fert? (plagula generata: generator eiusve
 * fontes rescribendi, numquam ipsa) */
interior b32
_generata_est (
    constans character* fons,
                   i32  mensura)
{
    i32 finis = ZEPHYRUM;
    i32 k;

    dum (finis < mensura && fons[finis] != '\n')
    {
        finis++;
    }
    per (k = ZEPHYRUM; k + IX <= finis; k++)
    {
        si (memcmp(fons + k, "GENERATUM", IX) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* finis directivae a 'locus' (initium lineae suae): octetus post '\n'
 * lineae ubi directiva cum commentariis suis finitur. Commentarium
 * in lineas sequentes iens transilitur (gesta/fontes/tabularium.c:
 * '#include "moneta.h"   /' + '* ...' duabus lineis - linea nova intra
 * commentarium caput numquam includebat); "..." et <...> nominis
 * capitis commentarium non aperiunt. */
interior s32
_directivae_finis (
    chorda textus,
       s32 locus)
{
          s32 n         = (s32)textus.mensura;
          s32 k         = locus;
    character claudens  = '\0';

    dum (k < n)
    {
        character c = (character)textus.datum[k];

        si (claudens != '\0')
        {
            si (c == claudens)
            {
                claudens = '\0';
            }
            alioquin si (c == '\n')
            {
                redde k + I;
            }
            k++;
            perge;
        }
        si (c == '"')
        {
            claudens = '"';
        }
        alioquin si (c == '<')
        {
            claudens = '>';
        }
        alioquin si (   c                   == '/' && k + I < n
                     && textus.datum[k + I] == '*')
        {
            k += II;
            dum (   k + I < n
                 && !(textus.datum[k] == '*'
                      && textus.datum[k + I] == '/'))
            {
                k++;
            }
            k = (k + I < n) ? k + II : n;
            perge;
        }
        alioquin si (c == '\n')
        {
            redde k + I;
        }
        k++;
    }
    redde n;
}

/* '#include "caput"' addere si deest: post lineam '#include "..."'
 * ultimam ante limitem (rescriptionem primam), aliter post '#include
 * <...>' ultimam, aliter in capite - post finem commentarii lineae
 * (_directivae_finis) */
interior chorda
_caput_addere (
         Piscina* piscina,
          chorda  textus,
          chorda  caput,
             s32  limes)
{
     ChordaAedificator* a;
                chorda  linea;
                   s32  post_quotam   = -I;
                   s32  post_angulum  = -I;
                   s32  locus;
                   s32  k;

    si (caput.mensura == ZEPHYRUM)
    {
        redde textus;
    }
    a = chorda_aedificator_creare(piscina,
        (memoriae_index)caput.mensura + XXIV);
    si (a == NIHIL)
    {
        redde textus;
    }
    (vacuum)chorda_aedificator_appendere_literis(a, "#include \"");
    (vacuum)chorda_aedificator_appendere_chorda(a, caput);
    (vacuum)chorda_aedificator_appendere_literis(a, "\"\n");
    linea = chorda_aedificator_finire(a);
    si (chorda_continet(textus, chorda_sectio(linea, ZEPHYRUM,
            linea.mensura - I)))
    {
        redde textus;
    }
    per (k = ZEPHYRUM; k < limes && k < (s32)textus.mensura; k++)
    {
        si (k > ZEPHYRUM && textus.datum[k - I] != '\n')
        {
            perge;
        }
        si (   k + X <= (s32)textus.mensura
            && memcmp(textus.datum + k, "#include \"", X) == ZEPHYRUM)
        {
            post_quotam = k;
        }
        alioquin si (   k + X <= (s32)textus.mensura
                     && memcmp(textus.datum + k, "#include <", X)
                         == ZEPHYRUM)
        {
            post_angulum = k;
        }
    }
    locus = post_quotam >= ZEPHYRUM ? post_quotam : post_angulum;
    si (locus >= ZEPHYRUM)
    {
        locus = _directivae_finis(textus, locus);
    }
    alioquin
    {
        locus = ZEPHYRUM;
    }
    redde chorda_concatenare(chorda_concatenare(chorda_sectio(textus,
        ZEPHYRUM, (i32)locus), linea, piscina), chorda_sectio(textus,
        (i32)locus, textus.mensura), piscina);
}

/* custodia et scriptio unius plagulae; nuntius in stderr */
interior b32
_plagulam_scribere (
           IudiciumApparatus* app,
                     Piscina* opus,
          constans character* via,
    constans IudiciumFructus* ante,
          constans character* fons,
                         i32  mensura,
                      chorda  post,
                constans Xar* rescriptiones)
{
    EmendatioVerdictum  v;
                chorda  erratum;
             character* textus      = chorda_ut_cstr(post, opus);
                   i32  applicatae  = ZEPHYRUM;
                   i32  k;

    per (k = ZEPHYRUM; k < xar_numerus(rescriptiones); k++)
    {
        constans SilvaRescriptio* r = (constans SilvaRescriptio*)
            xar_obtinere(rescriptiones, k);

        si (r->verdictum == SILVA_RESCRIPTIO_FACTA && !r->iterata)
        {
            applicatae++;
        }
    }
    si (textus == NIHIL)
    {
        fprintf(stderr, "%s: NON SCRIPTA (memoria)\n", via);
        redde FALSUM;
    }
    v = silva_emendationem_iudicare(app, opus, via, ante, textus,
        post.mensura, codex_scopi, (s32)applicatae);
    si (!v.bene)
    {
        fprintf(stderr, "%s: NON SCRIPTA (%.*s)\n", via,
            (integer)v.causa.mensura,
            (constans character*)v.causa.datum);
        redde FALSUM;
    }
    si (!silva_emendationem_compilare(opus, via, fons, mensura,
            vexilla_clang, &erratum))
    {
        fprintf(stderr, "%s: NON SCRIPTA (clang ANTE iam fractum - non"
            " verificabilis):\n%.*s\n", via, (integer)erratum.mensura,
            (constans character*)erratum.datum);
        redde FALSUM;
    }
    si (!silva_emendationem_compilare(opus, via, textus, post.mensura,
            vexilla_clang, &erratum))
    {
        fprintf(stderr, "%s: NON SCRIPTA (clang post):\n%.*s\n", via,
            (integer)erratum.mensura,
            (constans character*)erratum.datum);
        redde FALSUM;
    }
    si (!filum_scribere(via, post))
    {
        fprintf(stderr, "%s: NON SCRIPTA (scriptio fracta)\n", via);
        redde FALSUM;
    }
    si (codex_scopi >= ZEPHYRUM)
    {
        fprintf(stderr,
            "%s: SCRIPTA (%u rescriptiones; codex %d %u -> %u,"
            " nihil crevit, clang tacet)\n", via,
            (insignatus integer)applicatae, (integer)codex_scopi,
            (insignatus integer)v.ante, (insignatus integer)v.post);
    }
    alioquin
    {
        fprintf(stderr, "%s: SCRIPTA (%u rescriptiones; nihil crevit,"
            " clang tacet)\n", via, (insignatus integer)applicatae);
    }
    redde VERUM;
}

/* VERUM = plagula iudicata (situs fortasse nulli). regulae NIHIL =
 * census; aliter consilium: differentia in stdout (non -machina) */
interior b32
_plagulam_tractare (
       IudiciumApparatus* app,
                 Piscina* piscina,
      constans character* via,
                  chorda  functio,
            constans Xar* regulae,
                     Xar* census,
                     i32* mutatae,
                     i32* scriptae,
                     i32* non_scriptae)
{
            Piscina* opus;
    IudiciumFructus  fr;
          character* fons;
                i32  mensura = ZEPHYRUM;
                i32  k;

    opus = piscina_generare_dynamicum("transformare_opus", 16777216);
    si (opus == NIHIL)
    {
        _fractam_nuntiare(via, "piscina deest");
        redde FALSUM;
    }
    fons = silva_iudicium_plagulam_legere(opus, via, &mensura);
    si (fons == NIHIL)
    {
        _fractam_nuntiare(via, "plagula illegibilis");
        piscina_destruere(opus);
        redde FALSUM;
    }
    si (regulae != NIHIL && _generata_est(fons, mensura))
    {
        fprintf(stderr, "transformare: %s: GENERATUM - non rescribitur"
            " (generatorem eiusve fontes rescribe)\n", via);
        piscina_destruere(opus);
        redde VERUM;
    }
    si (!silva_iudicium_iudicare(app, opus, via, fons, mensura, &fr))
    {
        _fractam_nuntiare(via, fr.recusatum
            ? "fines examinis tactae (RECUSO)"
            : "non iudicata (parsura cum erroribus aut apparatus)");
        piscina_destruere(opus);
        redde FALSUM;
    }
    si (regulae == NIHIL)
    {
        Xar* situs = silva_situs_invenire(opus, fr.parsura, functio);

        si (situs == NIHIL)
        {
            _fractam_nuntiare(via, "situs non collecti (memoria)");
            piscina_destruere(opus);
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < xar_numerus(situs); k++)
        {
            _lineam_addere(census, piscina, opus, via, fr.parsura,
                (constans SilvaSitus*)xar_obtinere(situs, k), fons,
                mensura, -I, -I);
        }
    }
    alioquin
    {
        Xar* rescriptiones = silva_rescriptiones_parare(opus,
            fr.parsura,
            fons, mensura, functio, regulae);
        b32 facta = FALSUM;

        si (rescriptiones == NIHIL)
        {
            _fractam_nuntiare(via,
                "rescriptiones non paratae (memoria)");
            piscina_destruere(opus);
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < xar_numerus(rescriptiones); k++)
        {
            constans SilvaRescriptio* r = (constans SilvaRescriptio*)
                xar_obtinere(rescriptiones, k);

            _lineam_addere(census, piscina, opus, via, fr.parsura,
                &r->situs, fons, mensura, (s32)r->verdictum, r->regula);
            si (r->verdictum == SILVA_RESCRIPTIO_FACTA)
            {
                facta = VERUM;
            }
        }
        si (facta)
        {
                         chorda  ante;
                         chorda  post;
            DifferentiaLinearum* d;

            (*mutatae)++;
            ante  = chorda_ex_buffer((i8*)fons, mensura);
            post  = silva_rescriptiones_applicare(opus, fons, mensura,
                rescriptiones);
            post  = _caput_addere(opus, post, inclusio_regularum,
                _limes_primus(rescriptiones));
            si (scribere_volo)
            {
                si (_plagulam_scribere(app, opus, via, &fr, fons,
                    mensura,
                        post, rescriptiones))
                {
                    (*scriptae)++;
                }
                alioquin
                {
                    (*non_scriptae)++;
                }
            }
            d     = differentia_linearum(opus, ante, post);
            si (d != NIHIL && !machina_volo)
            {
                character* a = (character*)piscina_allocare(opus,
                    strlen(via) + V);
                character* b = (character*)piscina_allocare(opus,
                    strlen(via) + V);
                    chorda u;

                si (a != NIHIL && b != NIHIL)
                {
                    sprintf(a, "a/%s", via);
                    sprintf(b, "b/%s", via);
                    u = differentia_unificata(opus, d, a, b, III);
                    fwrite(u.datum, I, (memoriae_index)u.mensura,
                        stdout);
                }
            }
        }
    }
    piscina_destruere(opus);
    redde VERUM;
}

interior integer
_formas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans CensusForma* x   = *(constans CensusForma* constans*)a;
    constans CensusForma* y   = *(constans CensusForma* constans*)b;
                     i32  nx  = xar_numerus(x->situs);
                     i32  ny  = xar_numerus(y->situs);
                     i32  px  = *(i32*)xar_obtinere(x->situs, ZEPHYRUM);
                     i32  py  = *(i32*)xar_obtinere(y->situs, ZEPHYRUM);

    si (x->in_macro != y->in_macro)
    {
        redde x->in_macro ? I : -I;
    }
    si (x->verdictum != y->verdictum)
    {
        redde x->verdictum < y->verdictum ? -I : I;
    }
    si (nx != ny)
    {
        redde nx > ny ? -I : I;
    }
    redde px < py ? -I : (px > py ? I : ZEPHYRUM);
}

interior vacuum
_formam_imprimere (
                    FILE* exitus,
    constans CensusForma* f,
                     Xar* census)
{
    i32 n = xar_numerus(f->situs);
    i32 k;
    i32 latitudo;

    fprintf(exitus, "%5u  ", (insignatus integer)n);
    si (f->in_macro)
    {
        fprintf(exitus, "in macro %.*s (non rescribitur)",
            (integer)f->scriptura.mensura,
            (constans character*)f->scriptura.datum);
        latitudo = f->scriptura.mensura + XXVII;
    }
    alioquin
    {
        fprintf(exitus, "%.*s", (integer)f->scriptura.mensura,
            (constans character*)f->scriptura.datum);
        latitudo = f->scriptura.mensura;
    }
    dum (latitudo < XLIV)
    {
        fputc(' ', exitus);
        latitudo++;
    }
    per (k = ZEPHYRUM; k < n && k < III; k++)
    {
        constans CensusSitus* r = (constans CensusSitus*)xar_obtinere(
            census, *(i32*)xar_obtinere(f->situs, k));

        fprintf(exitus, "%s%s:%u", k > ZEPHYRUM ? ", " : "  ", r->via,
            (insignatus integer)r->linea);
    }
    si (n > III)
    {
        fprintf(exitus, " (+%u)", (insignatus integer)(n - III));
    }
    si (f->aliter > ZEPHYRUM)
    {
        fprintf(exitus, "  [aliter scripti: %u]",
            (insignatus integer)f->aliter);
    }
    si (f->verdictum >= ZEPHYRUM && !f->in_macro)
    {
        fprintf(exitus, "  [%s]", silva_rescriptionis_causa(
            (SilvaRescriptioVerdictum)f->verdictum));
    }
    fputc('\n', exitus);
}

/* lineae gregatae per clavem (et verdictum); solum_recusatae =
 * consilium: lineae FACTAE omissae. Numerus formarum redditur. */
interior i32
_formas_imprimere (
                FILE* exitus,
             Piscina* piscina,
                 Xar* census,
                 b32  solum_recusatae)
{
    TabulaDispersa* tabula = tabula_dispersa_creare_chorda(piscina,
        CCLVI);
               Xar* formae = xar_creare(piscina,
                   (i32)magnitudo(CensusForma*));
       CensusForma** ordo;
               i32   n;
               i32   k;

    si (tabula == NIHIL || formae == NIHIL)
    {
        fprintf(stderr, "transformare: memoria deest\n");
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(census); k++)
    {
        constans CensusSitus* r = (constans CensusSitus*)xar_obtinere(
            census, k);
                     chorda  clavis    = r->clavis;
                     vacuum* inventum  = NIHIL;
                CensusForma* f;
                  character  verdictum[XVI];

        si (solum_recusatae && r->verdictum == SILVA_RESCRIPTIO_FACTA)
        {
            perge;
        }
        si (r->in_macro)
        {
            clavis = chorda_concatenare(chorda_ex_literis("macro|",
                piscina), r->clavis, piscina);
        }
        alioquin si (r->verdictum >= ZEPHYRUM)
        {
            sprintf(verdictum, "%u|", (insignatus integer)r->verdictum);
            clavis = chorda_concatenare(chorda_ex_literis(verdictum,
                piscina), r->clavis, piscina);
        }
        si (tabula_dispersa_invenire(tabula, clavis, &inventum))
        {
            f = (CensusForma*)inventum;
            si (   !r->in_macro
                && !chorda_aequalis(r->scriptura, f->scriptura))
            {
                f->aliter++;
            }
        }
        alioquin
        {
            f = (CensusForma*)piscina_allocare(piscina,
                magnitudo(CensusForma));
            si (f == NIHIL)
            {
                redde ZEPHYRUM;
            }
            f->clavis     = clavis;
            f->scriptura  = r->in_macro ? r->clavis : r->scriptura;
            f->in_macro   = r->in_macro;
            f->verdictum  = r->verdictum;
            f->aliter     = ZEPHYRUM;
            f->situs      = xar_creare(piscina, (i32)magnitudo(i32));
            si (   f->situs == NIHIL
                || !tabula_dispersa_inserere(tabula, clavis, f))
            {
                redde ZEPHYRUM;
            }
            *(CensusForma**)xar_addere(formae) = f;
        }
        *(i32*)xar_addere(f->situs) = k;
    }
    n     = xar_numerus(formae);
    ordo  = (CensusForma**)piscina_allocare(piscina,
        (memoriae_index)(n
            > ZEPHYRUM ? n : I) * magnitudo(CensusForma*));
    si (ordo == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        ordo[k] = *(CensusForma**)xar_obtinere(formae, k);
    }
    qsort(ordo, (memoriae_index)n, magnitudo(CensusForma*),
        _formas_comparare);
    per (k = ZEPHYRUM; k < n; k++)
    {
        _formam_imprimere(exitus, ordo[k], census);
    }
    redde n;
}

interior vacuum
_censum_imprimere (
               Piscina* piscina,
                   Xar* census,
    constans character* functio,
                   i32  plagulae,
                   i32  fractae)
{
    i32 in_macro = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(census); k++)
    {
        si (((constans CensusSitus*)xar_obtinere(census, k))->in_macro)
        {
            in_macro++;
        }
    }
    imprimere("transformare -census %s: %u situs, %u in macro (%u"
        " plagulae iudicatae, %u non iudicatae)\n", functio,
        (insignatus integer)xar_numerus(census),
        (insignatus integer)in_macro, (insignatus integer)plagulae,
        (insignatus integer)fractae);
    (vacuum)_formas_imprimere(stdout, piscina, census, FALSUM);
}

/* summa consilii in stderr: regulae (numquam congruentes nominatae),
 * deinde situs recusati per formam et causam. Reddit recusatos. */
interior i32
_consilium_imprimere (
               Piscina* piscina,
                   Xar* census,
          constans Xar* regulae,
    constans character* via_regularum,
                   i32  plagulae,
                   i32  fractae,
                   i32  mutatae,
                   i32  scriptae,
                   i32  non_scriptae)
{
    i32* usus = (i32*)piscina_allocare(piscina,
        (memoriae_index)(xar_numerus(regulae) + I) * magnitudo(i32));
    i32 factae     = ZEPHYRUM;
    i32 recusatae  = ZEPHYRUM;
    i32 k;

    si (usus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(regulae); k++)
    {
        usus[k] = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(census); k++)
    {
        constans CensusSitus* r = (constans CensusSitus*)xar_obtinere(
            census, k);

        si (r->verdictum == SILVA_RESCRIPTIO_FACTA)
        {
            factae++;
        }
        alioquin
        {
            recusatae++;
        }
        si (   r->regula >= ZEPHYRUM
            && (i32)r->regula < xar_numerus(regulae))
        {
            usus[r->regula]++;
        }
    }
    fprintf(stderr, "transformare -regulae %s: %u situs, %u"
        " rescriberentur in %u plagulis, %u recusati (%u plagulae"
        " iudicatae, %u non iudicatae)\n", via_regularum,
        (insignatus integer)xar_numerus(census),
        (insignatus integer)factae,
        (insignatus integer)mutatae, (insignatus integer)recusatae,
        (insignatus integer)plagulae, (insignatus integer)fractae);
    si (scribere_volo)
    {
        fprintf(stderr, "  scriptae %u, non scriptae %u (custodia)\n",
            (insignatus integer)scriptae,
            (insignatus integer)non_scriptae);
    }
    alioquin
    {
        fprintf(stderr, "  consilium - nihil scriptum (-scribere"
            " applicat)\n");
    }
    per (k = ZEPHYRUM; k < xar_numerus(regulae); k++)
    {
        constans SilvaRegula* g = *(SilvaRegula* constans*)xar_obtinere(
            regulae, k);

        fprintf(stderr, "  regula %u: %5u  %.*s%s\n",
            (insignatus integer)k, (insignatus integer)usus[k],
            (integer)g->titulus.mensura,
            (constans character*)g->titulus.datum,
            usus[k] == ZEPHYRUM ? "   [numquam congruit]" : "");
    }
    si (recusatae > ZEPHYRUM)
    {
        fprintf(stderr, "recusati:\n");
        (vacuum)_formas_imprimere(stderr, piscina, census, VERUM);
    }
    redde recusatae;
}

interior vacuum
_lineas_machinae (
    Xar* census)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(census); k++)
    {
        constans CensusSitus* r = (constans CensusSitus*)xar_obtinere(
            census, k);

        si (r->verdictum < ZEPHYRUM)
        {
            imprimere("%s\t%s\t%u\t%u\t",
                r->in_macro ? "macro" : "situs",
                r->via, (insignatus integer)r->linea,
                (insignatus integer)r->columna);
            _campum_scribere(r->clavis);
            si (!r->in_macro)
            {
                fputc('\t', stdout);
                _campum_scribere(r->scriptura);
            }
            fputc('\n', stdout);
            perge;
        }
        imprimere("%s\t%s\t%u\t%u\t%d\t%s\t",
            r->verdictum
                == SILVA_RESCRIPTIO_FACTA ? "facta" : "recusata",
            r->via, (insignatus integer)r->linea,
            (insignatus integer)r->columna, (integer)r->regula,
            silva_rescriptionis_causa((SilvaRescriptioVerdictum)
                r->verdictum));
        _campum_scribere(r->clavis);
        fputc('\n', stdout);
    }
}

interior vacuum
_usum_monstrare (vacuum)
{
    fprintf(stderr, "usus: transformare -census <functio> <viae...>"
        " [-machina]\n       transformare -regulae <regulae.stml>"
        " <viae...> [-machina] [-scribere -- <vexilla clang>]\n  (ex"
        " radice repositorii; sine -scribere nihil scribitur)\n");
}

s32
principale (
      integer   argc,
    character** argv)
{
               Piscina*  piscina;
     IudiciumApparatus*  app;
    constans character*  functio        = NIHIL;
    constans character*  via_regularum  = NIHIL;
                   Xar*  regulae        = NIHIL;
                   Xar*  viae;
                   Xar*  census;
             character** ordo;
                chorda   titulus_functionis;
                   i32   numerus       = ZEPHYRUM;
                   i32   plagulae      = ZEPHYRUM;
                   i32   fractae       = ZEPHYRUM;
                   i32   mutatae       = ZEPHYRUM;
                   i32   scriptae      = ZEPHYRUM;
                   i32   non_scriptae  = ZEPHYRUM;
               integer   k;

    piscina = piscina_generare_dynamicum("transformare", 16777216);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "transformare: piscina deest\n");
        redde II;
    }
    viae    = xar_creare(piscina, (i32)magnitudo(character*));
    census  = xar_creare(piscina, (i32)magnitudo(CensusSitus));
    si (viae == NIHIL || census == NIHIL)
    {
        redde II;
    }
    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "--") == ZEPHYRUM)
        {
            vexilla_clang = (constans character* constans*)(argv + k
                + I);
            frange;
        }
        si (strcmp(argv[k], "-machina") == ZEPHYRUM)
        {
            machina_volo = VERUM;
        }
        alioquin si (strcmp(argv[k], "-scribere") == ZEPHYRUM)
        {
            scribere_volo = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-census") == ZEPHYRUM
                     && k + I < argc)
        {
            functio = argv[++k];
        }
        alioquin si (   strcmp(argv[k], "-regulae") == ZEPHYRUM
                     && k + I < argc)
        {
            via_regularum = argv[++k];
        }
        alioquin si (argv[k][ZEPHYRUM] == '-')
        {
            fprintf(stderr, "transformare: optio ignota: %s\n",
                argv[k]);
            _usum_monstrare();
            redde II;
        }
        alioquin si (directorium_existit(argv[k]))
        {
            _directorium_colligere(piscina, argv[k], viae, ZEPHYRUM);
        }
        alioquin
        {
            character** locus = (character**)xar_addere(viae);

            si (locus != NIHIL)
            {
                *locus = argv[k];
            }
        }
    }
    si (   (functio == NIHIL) == (via_regularum == NIHIL)
        || (scribere_volo && (via_regularum == NIHIL
            || vexilla_clang == NIHIL)))
    {
        _usum_monstrare();
        redde II;
    }
    si (via_regularum != NIHIL)
    {
        regulae = _regulas_legere(piscina, via_regularum,
            &titulus_functionis);
        si (regulae == NIHIL)
        {
            redde II;
        }
    }
    alioquin
    {
        titulus_functionis = chorda_ex_literis(functio, piscina);
    }
    si (xar_numerus(viae) == ZEPHYRUM)
    {
        fprintf(stderr,
            "transformare: nullae plagulae (.c/.h) datae\n");
        redde II;
    }
    ordo  = _vias_ordinare(piscina, viae, &numerus);
    app   = silva_iudicium_praeparare(piscina);
    si (ordo == NIHIL || app == NIHIL)
    {
        fprintf(stderr, "transformare: apparatus deest (ex radice"
            " repositorii curre)\n");
        redde II;
    }
    per (k = ZEPHYRUM; k < (integer)numerus; k++)
    {
        si (_plagulam_tractare(app, piscina, ordo[k],
                titulus_functionis, regulae, census, &mutatae,
                &scriptae,
                &non_scriptae))
        {
            plagulae++;
        }
        alioquin
        {
            fractae++;
        }
    }
    si (machina_volo)
    {
        _lineas_machinae(census);
    }
    si (regulae != NIHIL)
    {
        i32 recusatae = _consilium_imprimere(piscina, census, regulae,
            via_regularum, plagulae, fractae, mutatae, scriptae,
            non_scriptae);

        redde (recusatae > ZEPHYRUM || non_scriptae > ZEPHYRUM)
            ? I : ZEPHYRUM;
    }
    si (!machina_volo)
    {
        _censum_imprimere(piscina, census, functio, plagulae, fractae);
    }
    redde xar_numerus(census) > ZEPHYRUM ? ZEPHYRUM : I;
}
