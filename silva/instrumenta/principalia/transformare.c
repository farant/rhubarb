/* transformare.c - TRANSFORMARE: formae vocationum super arbores silvae
 * (spec project-specs/transformare-spec.md, plan transformare-plan.md)
 *
 * Usus:  ./silva/transformare.sh -census <functio> <viae...> [-machina]
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
 * -machina: TSV, linea una per situm (ordine plagularum):
 *     situs  <via> <linea> <columna> <clavis> <scriptura>
 *     macro  <via> <linea> <columna> <macro>
 *     fracta <via> <causa>
 *
 * Exitus: 0 situs inventi | 1 nulli | 2 usus aut apparatus
 * Ex radice repositorii currendum (transformare.sh eo migrat).
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "iter_directoria.h"
#include "silva_parsare.h"
#include "silva_forma.h"
#include "silva_iudicium.h"
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
} CensusSitus;

nomen structura {
    chorda  clavis;      /* in_macro: 'macro|' + nomen */
    chorda  scriptura;   /* situs primi; in_macro: nomen macro */
       b32  in_macro;
       i32  aliter;      /* situs aliter scripti quam primus */
       Xar* situs;       /* i32: indices in censum */
} CensusForma;

hic_manens b32 machina_volo = FALSUM;


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

/* VERUM = plagula iudicata (situs fortasse nulli) */
interior b32
_plagulam_censere (
      IudiciumApparatus* app,
                Piscina* piscina,
     constans character* via,
                 chorda  functio,
                    Xar* census)
{
            Piscina* opus;
    IudiciumFructus  fr;
          character* fons;
                i32  mensura = ZEPHYRUM;
                Xar* situs;
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
    si (!silva_iudicium_iudicare(app, opus, via, fons, mensura, &fr))
    {
        _fractam_nuntiare(via, fr.recusatum
            ? "fines examinis tactae (RECUSO)"
            : "non iudicata (parsura cum erroribus aut apparatus)");
        piscina_destruere(opus);
        redde FALSUM;
    }
    situs = silva_situs_invenire(opus, fr.parsura, functio);
    si (situs == NIHIL)
    {
        _fractam_nuntiare(via, "situs non collecti (memoria)");
        piscina_destruere(opus);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(situs); k++)
    {
        constans SilvaSitus* s =
            (constans SilvaSitus*)xar_obtinere(situs,
            k);
             CensusSitus* r = (CensusSitus*)xar_addere(census);
                  chorda  clavis;
                  chorda  scriptura;

        si (r == NIHIL)
        {
            frange;
        }
        r->via        = via;
        r->linea      = s->linea;
        r->columna    = s->columna;
        r->in_macro   = s->in_macro;
        r->scriptura  = chorda_ex_literis("", piscina);
        si (s->in_macro)
        {
            r->clavis = chorda_transcribere(s->macro, piscina);
        }
        alioquin si (silva_situs_forma(opus, fr.parsura, s, fons,
                     mensura,
                     &clavis, &scriptura))
        {
            r->clavis     = chorda_transcribere(clavis, piscina);
            r->scriptura  = chorda_transcribere(scriptura, piscina);
        }
        alioquin
        {
            r->clavis = chorda_ex_literis("?|forma ignota",
                piscina);
            r->scriptura = chorda_ex_literis("(forma ignota)",
                piscina);
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
    si (nx != ny)
    {
        redde nx > ny ? -I : I;
    }
    redde px < py ? -I : (px > py ? I : ZEPHYRUM);
}

interior vacuum
_formam_imprimere (
    constans CensusForma* f,
                     Xar* census)
{
    i32 n = xar_numerus(f->situs);
    i32 k;
    i32 latitudo;

    imprimere("%5u  ", (insignatus integer)n);
    si (f->in_macro)
    {
        imprimere("in macro %.*s (non rescribitur)",
            (integer)f->scriptura.mensura,
            (constans character*)f->scriptura.datum);
        latitudo = f->scriptura.mensura + XXVII;
    }
    alioquin
    {
        imprimere("%.*s", (integer)f->scriptura.mensura,
            (constans character*)f->scriptura.datum);
        latitudo = f->scriptura.mensura;
    }
    dum (latitudo < XLIV)
    {
        fputc(' ', stdout);
        latitudo++;
    }
    per (k = ZEPHYRUM; k < n && k < III; k++)
    {
        constans CensusSitus* r = (constans CensusSitus*)xar_obtinere(
            census, *(i32*)xar_obtinere(f->situs, k));

        imprimere("%s%s:%u", k > ZEPHYRUM ? ", " : "  ", r->via,
            (insignatus integer)r->linea);
    }
    si (n > III)
    {
        imprimere(" (+%u)", (insignatus integer)(n - III));
    }
    si (f->aliter > ZEPHYRUM)
    {
        imprimere("  [aliter scripti: %u]",
            (insignatus integer)f->aliter);
    }
    imprimere("\n");
}

/* census gregatus per clavem; formae ordinatae imprimuntur */
interior vacuum
_censum_imprimere (
               Piscina* piscina,
                   Xar* census,
    constans character* functio,
                   i32  plagulae,
                   i32  fractae)
{
    TabulaDispersa* tabula = tabula_dispersa_creare_chorda(piscina,
        CCLVI);
               Xar* formae = xar_creare(piscina,
                   (i32)magnitudo(CensusForma*));
       CensusForma** ordo;
               i32   n;
               i32   k;
               i32   in_macro = ZEPHYRUM;

    si (tabula == NIHIL || formae == NIHIL)
    {
        fprintf(stderr, "transformare: memoria deest\n");
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(census); k++)
    {
        constans CensusSitus* r = (constans CensusSitus*)xar_obtinere(
            census, k);
                     chorda  clavis    = r->clavis;
                     vacuum* inventum  = NIHIL;
                CensusForma* f;

        si (r->in_macro)
        {
            clavis = chorda_concatenare(chorda_ex_literis("macro|",
                piscina), r->clavis, piscina);
            in_macro++;
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
                redde;
            }
            f->clavis     = clavis;
            f->scriptura  = r->in_macro ? r->clavis : r->scriptura;
            f->in_macro   = r->in_macro;
            f->aliter     = ZEPHYRUM;
            f->situs      = xar_creare(piscina, (i32)magnitudo(i32));
            si (   f->situs == NIHIL
                || !tabula_dispersa_inserere(tabula, clavis, f))
            {
                redde;
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
        redde;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        ordo[k] = *(CensusForma**)xar_obtinere(formae, k);
    }
    qsort(ordo, (memoriae_index)n, magnitudo(CensusForma*),
        _formas_comparare);
    imprimere("transformare -census %s: %u situs, %u formae, %u in"
        " macro (%u plagulae iudicatae, %u non iudicatae)\n", functio,
        (insignatus integer)xar_numerus(census), (insignatus integer)n,
        (insignatus integer)in_macro, (insignatus integer)plagulae,
        (insignatus integer)fractae);
    per (k = ZEPHYRUM; k < n; k++)
    {
        _formam_imprimere(ordo[k], census);
    }
}

interior vacuum
_censum_machinae (
    Xar* census)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(census); k++)
    {
        constans CensusSitus* r = (constans CensusSitus*)xar_obtinere(
            census, k);

        imprimere("%s\t%s\t%u\t%u\t", r->in_macro ? "macro" : "situs",
            r->via, (insignatus integer)r->linea,
            (insignatus integer)r->columna);
        _campum_scribere(r->clavis);
        si (!r->in_macro)
        {
            fputc('\t', stdout);
            _campum_scribere(r->scriptura);
        }
        fputc('\n', stdout);
    }
}

interior vacuum
_usum_monstrare (vacuum)
{
    fprintf(stderr, "usus: transformare -census <functio> <viae...>"
        " [-machina]  (ex radice repositorii)\n");
}

s32
principale (
      integer   argc,
    character** argv)
{
               Piscina*  piscina;
     IudiciumApparatus*  app;
    constans character*  functio  = NIHIL;
                   Xar*  viae;
                   Xar*  census;
             character** ordo;
                   i32   numerus      = ZEPHYRUM;
                   i32   plagulae     = ZEPHYRUM;
                   i32   fractae      = ZEPHYRUM;
                   b32   census_volo  = FALSUM;
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
        si (strcmp(argv[k], "-machina") == ZEPHYRUM)
        {
            machina_volo = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-census") == ZEPHYRUM
                     && k + I < argc)
        {
            census_volo  = VERUM;
            functio      = argv[++k];
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
    si (!census_volo || functio == NIHIL)
    {
        _usum_monstrare();
        redde II;
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
        si (_plagulam_censere(app, piscina, ordo[k],
                chorda_ex_literis(functio, piscina), census))
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
        _censum_machinae(census);
    }
    alioquin
    {
        _censum_imprimere(piscina, census, functio, plagulae, fractae);
    }
    redde xar_numerus(census) > ZEPHYRUM ? ZEPHYRUM : I;
}
