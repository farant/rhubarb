/* canon_examen.c - plagulas STML contra canonem iudicare
 *
 * Usus:
 *   bin/canon_examen <plagula.stml> ...        canon per registrum
 *   bin/canon_examen -canon X.canon <plagula>  canon expressus
 *   bin/canon_examen -machina ...              TSV purum
 *
 * IUDICIA PER PLAGULAM (AMBO IUDICANT - decretum 2026-08-06):
 *   1. canon registri (radix vincit, extensio cadit) - pavimentum
 *      domus quod documentum deponere non potest
 *   2. si canon INFIXUS adest (liberum primum radicis <canon>):
 *      a. infixus IPSE contra canonem canonum   [fons: infixus-ipse]
 *      b. contentum contra infixum              [fons: infixus]
 * Machina: via, linea, genus, elementum, campus, nuntius, fons
 * iudicii. Plagula: 'via:linea:' (forma quam editor aperit);
 * linea 0 = nodus sine positione (fabricatus, non parsatus).
 *
 * Exitus: 0 = sanum; 1 = VITIA (etiam plagula quae parsari nequit -
 *   inventum est, non absentia); 2 = NIHIL IUDICATUM EST
 *   (disciplina domus: instrumentum quod nihil iudicavit successus
 *    non est - plagula sine canone TACERE non debet)
 */
#include "postulata_posix.h"

#include "latina.h"
#include "canon.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>

#define REGISTRUM "canones.registrum"

interior constans character* vitium_campus(CanonVitium* v,
                                           character* buffer,
                                           i32 tectum);
interior Canon* canonem_onerare(chorda via_canonis,
                                Piscina* piscina,
                                InternamentumChorda* intern);
interior i32 vitia_imprimere(constans character* via,
                             constans character* fons_iudicii,
                             Xar* vitia, b32 machina);
interior vacuum _campum(chorda* c, constans character* cadens);
interior b32 index_scribere(constans character* via,
                            Piscina* piscina,
                            InternamentumChorda* intern);

/* campus 'detail' cum limitibus cardinalitatis */
interior constans character*
vitium_campus(
    CanonVitium* v,
    character*   buffer,
    i32          tectum)
{
    si (v->genus == CANON_LIBERI_PAUCI ||
        v->genus == CANON_LIBERI_MULTI)
    {
        sprintf(buffer, "%.*s (%u, limes %u)",
                v->detail ? (integer)v->detail->mensura : 1,
                v->detail ? (constans character*)v->detail->datum
                          : "-",
                v->numerus, v->limes);
        redde buffer;
    }

    si (!v->detail)
    {
        redde "-";
    }
    {
        i32 m;

        m = v->detail->mensura < tectum - I ? v->detail->mensura
                                            : tectum - I;
        memcpy(buffer, v->detail->datum, (memoriae_index)m);
        buffer[m] = '\0';
    }

    redde buffer;
}

interior Canon*
canonem_onerare(
    chorda                via_canonis,
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    chorda  fons_canonis;
    chorda  causa;
    Canon*  canon;

    fons_canonis = filum_legere_totum(
        chorda_ut_cstr(via_canonis, piscina), piscina);
    si (fons_canonis.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "canon_examen: canon '%.*s' legi nequit\n",
                (integer)via_canonis.mensura,
                (constans character*)via_canonis.datum);
        redde NIHIL;
    }

    canon = canon_legere(fons_canonis, piscina, intern, &causa);
    si (!canon)
    {
        fprintf(stderr, "canon_examen: canon '%.*s' fractus: %.*s\n",
                (integer)via_canonis.mensura,
                (constans character*)via_canonis.datum,
                (integer)causa.mensura,
                (constans character*)causa.datum);
    }

    redde canon;
}

interior i32
vitia_imprimere(
    constans character*  via,
    constans character*  fons_iudicii,
    Xar*                 vitia,
    b32                  machina)
{
    i32 j;
    i32 numerus;

    numerus = (i32)xar_numerus(vitia);

    per (j = ZEPHYRUM; j < numerus; j++)
    {
        CanonVitium* v;
        character    buffer[DXII];
        i32          linea;

        v = (CanonVitium*)xar_obtinere(vitia, j);
        linea = v->nodus ? v->nodus->linea : ZEPHYRUM;

        si (machina)
        {
            imprimere("%s\t%u\t%u\t%.*s\t%s\t%s\t%s\n", via,
                linea,
                (i32)v->genus,
                v->elementum ? (integer)v->elementum->mensura : 1,
                v->elementum
                    ? (constans character*)v->elementum->datum
                    : "-",
                vitium_campus(v, buffer, (i32)magnitudo(buffer)),
                canon_nuntius(v->genus),
                fons_iudicii);
        }
        alioquin
        {
            /* via:linea: - forma universalis quam editor aperit */
            imprimere("%s:%u: <%.*s> %s: %s [%s]\n", via, linea,
                v->elementum ? (integer)v->elementum->mensura : 1,
                v->elementum
                    ? (constans character*)v->elementum->datum
                    : "-",
                canon_nuntius(v->genus),
                vitium_campus(v, buffer, (i32)magnitudo(buffer)),
                fons_iudicii);
        }
    }

    redde numerus;
}

/* campum TSV imprimere: chorda aut valor cadens; lineae novae et
 * tabulata in spatia vertuntur ne TSV rumpatur */
interior vacuum
_campum(
    chorda*              c,
    constans character*  cadens)
{
    i32 i;

    si (!c || c->mensura == ZEPHYRUM)
    {
        imprimere("%s", cadens);
        redde;
    }
    per (i = ZEPHYRUM; i < c->mensura; i++)
    {
        character ch;

        ch = (character)c->datum[i];
        si (ch == '\n' || ch == '\r' || ch == '\t')
        {
            ch = ' ';
        }
        putchar(ch);
    }
}

/* -index: inventarium canonis ut TSV - ex ARBORE parsata, non ex
 * Canone onerato (ordo documenti = ordo auctoris; tabula dispersa
 * eum perderet). Ordines:
 *   E  elem   intra  radix  textus  nota
 *   A  elem   attr   genus  nec     nota
 *   O  elem   attr   optio
 *   L  elem   liberum min   max     nota
 *   U  nomen  attr   super  nota
 * Consumptor primus: tools/natura_metamodulus_generare.sh
 * (catalogus METAMODULI e canone generatus, ne spec rancescat). */
interior b32
index_scribere(
    constans character*   via,
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    chorda        fons;
    StmlResultus  r;
    i32           n;
    i32           i;

    fons = filum_legere_totum(via, piscina);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "canon_examen: '%s' legi nequit\n", via);
        redde FALSUM;
    }
    r = stml_legere(fons, piscina, intern);
    si (!r.successus || !r.elementum_radix ||
        !chorda_aequalis_literis(*r.elementum_radix->titulus,
                                 "canon"))
    {
        fprintf(stderr,
            "canon_examen: '%s' non est canon parsabilis\n", via);
        redde FALSUM;
    }

    n = stml_numerus_liberorum(r.elementum_radix);
    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus* e;
        chorda*    e_nomen;

        e = stml_liberum_ad_indicem(r.elementum_radix, i);
        si (!e || e->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        e_nomen = stml_attributum_capere(e, "nomen");
        si (!e_nomen)
        {
            perge;
        }

        si (chorda_aequalis_literis(*e->titulus, "unicitas"))
        {
            imprimere("U\t");
            _campum(e_nomen, "-");                imprimere("\t");
            _campum(stml_attributum_capere(e, "attributum"), "-");
            imprimere("\t");
            _campum(stml_attributum_capere(e, "super"), "-");
            imprimere("\t");
            _campum(stml_attributum_capere(e, "intra"), "-");
            imprimere("\t");
            _campum(stml_attributum_capere(e, "nota"), "-");
            imprimere("\n");
            perge;
        }
        si (chorda_aequalis_literis(*e->titulus, "citatio"))
        {
            imprimere("C\t");
            _campum(e_nomen, "-");                imprimere("\t");
            _campum(stml_attributum_capere(e, "attributum"), "-");
            imprimere("\t");
            _campum(stml_attributum_capere(e, "ad"), "-");
            imprimere("\t");
            _campum(stml_attributum_capere(e, "super"), "-");
            imprimere("\t");
            _campum(stml_attributum_capere(e, "intra"), "-");
            imprimere("\t");
            _campum(stml_attributum_capere(e, "nota"), "-");
            imprimere("\n");
            perge;
        }
        si (!chorda_aequalis_literis(*e->titulus, "elementum"))
        {
            perge;
        }

        imprimere("E\t");
        _campum(e_nomen, "-");                          imprimere("\t");
        _campum(stml_attributum_capere(e, "intra"), "-");
        imprimere("\t");
        _campum(stml_attributum_capere(e, "radix"), "-");
        imprimere("\t");
        _campum(stml_attributum_capere(e, "textus"), "-");
        imprimere("\t");
        _campum(stml_attributum_capere(e, "nota"), "-");
        imprimere("\n");

        {
            i32 nl;
            i32 j;

            nl = stml_numerus_liberorum(e);
            per (j = ZEPHYRUM; j < nl; j++)
            {
                StmlNodus* l;
                chorda*    l_nomen;

                l = stml_liberum_ad_indicem(e, j);
                si (!l || l->genus != STML_NODUS_ELEMENTUM)
                {
                    perge;
                }
                l_nomen = stml_attributum_capere(l, "nomen");
                si (!l_nomen)
                {
                    perge;
                }

                si (chorda_aequalis_literis(*l->titulus,
                                            "attributum"))
                {
                    i32 no;
                    i32 k;

                    imprimere("A\t");
                    _campum(e_nomen, "-");     imprimere("\t");
                    _campum(l_nomen, "-");     imprimere("\t");
                    _campum(stml_attributum_capere(l, "genus"),
                            "textus");
                    imprimere("\t");
                    _campum(stml_attributum_capere(l,
                            "necessarium"), "-");
                    imprimere("\t");
                    _campum(stml_attributum_capere(l, "nota"), "-");
                    imprimere("\n");

                    no = stml_numerus_liberorum(l);
                    per (k = ZEPHYRUM; k < no; k++)
                    {
                        StmlNodus* o;
                        chorda     t;

                        o = stml_liberum_ad_indicem(l, k);
                        si (!o ||
                            o->genus != STML_NODUS_ELEMENTUM ||
                            !chorda_aequalis_literis(*o->titulus,
                                                     "optio"))
                        {
                            perge;
                        }
                        t = chorda_praecidere(
                            stml_textus_normalizatus(o, piscina));
                        imprimere("O\t");
                        _campum(e_nomen, "-"); imprimere("\t");
                        _campum(l_nomen, "-"); imprimere("\t");
                        _campum(&t, "-");
                        imprimere("\n");
                    }
                }
                alioquin si (chorda_aequalis_literis(*l->titulus,
                                                     "liberum"))
                {
                    imprimere("L\t");
                    _campum(e_nomen, "-");     imprimere("\t");
                    _campum(l_nomen, "-");     imprimere("\t");
                    _campum(stml_attributum_capere(l, "minimum"),
                            "0");
                    imprimere("\t");
                    _campum(stml_attributum_capere(l, "maximum"),
                            "-");
                    imprimere("\t");
                    _campum(stml_attributum_capere(l, "nota"), "-");
                    imprimere("\n");
                }
            }
        }
    }

    redde VERUM;
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    chorda               catalogus;
    constans character*  canon_expressus;
    b32                  machina;
    s32                  i;
    i32                  iudicatae;
    i32                  vitia_summa;

    i32 indexatae;

    canon_expressus = NIHIL;
    machina         = FALSUM;
    iudicatae       = ZEPHYRUM;
    vitia_summa     = ZEPHYRUM;
    indexatae       = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("canon_examen", 4194304);
    intern  = internamentum_creare(piscina);

    catalogus = filum_legere_totum(REGISTRUM, piscina);

    per (i = I; i < numerus; i++)
    {
        constans character* via;
        chorda              via_canonis;
        chorda              fons;
        chorda              causa;
        StmlResultus        r;
        StmlNodus*          infixus;
        b32                 iudicata;

        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
            perge;
        }
        si (strcmp(argumenta[i], "-canon") == ZEPHYRUM &&
            i + I < numerus)
        {
            canon_expressus = argumenta[++i];
            perge;
        }
        si (strcmp(argumenta[i], "-index") == ZEPHYRUM &&
            i + I < numerus)
        {
            si (index_scribere(argumenta[++i], piscina, intern))
            {
                indexatae++;
            }
            perge;
        }

        via = argumenta[i];

        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "canon_examen: '%s' legi nequit\n", via);
            perge;
        }
        r = stml_legere(fons, piscina, intern);
        si (!r.successus)
        {
            fprintf(stderr, "canon_examen: '%s' parsari nequit "
                            "(gradus I, ante canonem)\n", via);
            vitia_summa++;
            perge;
        }

        infixus = canon_infixum_invenire(r.elementum_radix);
        iudicata = FALSUM;

        /* canonem registri invenire: RADIX VINCIT, extensio cadit.
         * '.stml' quattuor dialectos fert, ergo extensio illis
         * nihil dicit; elementum radicis dialectum semper nominat */
        si (canon_expressus)
        {
            via_canonis = chorda_ex_literis(canon_expressus,
                                            piscina);
        }
        alioquin
        {
            via_canonis.datum   = NIHIL;
            via_canonis.mensura = ZEPHYRUM;
            si (r.elementum_radix && r.elementum_radix->titulus)
            {
                via_canonis = canon_registrum_quaerere_radice(
                    catalogus, r.elementum_radix->titulus, piscina);
            }
            si (via_canonis.mensura == ZEPHYRUM)
            {
                via_canonis = canon_registrum_quaerere(catalogus,
                                                       via, piscina);
            }
        }

        si (via_canonis.mensura > ZEPHYRUM)
        {
            Canon* canon;

            canon = canonem_onerare(via_canonis, piscina, intern);
            si (canon)
            {
                Xar* vitia;

                vitia = canon_iudicare(canon, r.radix, piscina);
                vitia_summa += vitia_imprimere(via,
                    chorda_ut_cstr(via_canonis, piscina),
                    vitia, machina);
                iudicata = VERUM;
            }
        }

        si (infixus)
        {
            chorda titulus_canonici;
            chorda via_cc;
            Canon* c_inf;

            /* a. infixus IPSE contra canonem canonum - infixus
             * fractus clamat, non tacite iners fit */
            titulus_canonici = chorda_ex_literis("canon", piscina);
            via_cc = canon_registrum_quaerere_radice(
                catalogus, &titulus_canonici, piscina);
            si (via_cc.mensura > ZEPHYRUM)
            {
                Canon* cc;

                cc = canonem_onerare(via_cc, piscina, intern);
                si (cc)
                {
                    Xar* vitia;

                    vitia = canon_iudicare(cc, infixus, piscina);
                    vitia_summa += vitia_imprimere(via,
                        "infixus-ipse", vitia, machina);
                    iudicata = VERUM;
                }
            }

            /* b. contentum contra infixum (ambo iudicant) */
            c_inf = canon_ex_nodo(infixus, piscina, intern, &causa);
            si (!c_inf)
            {
                fprintf(stderr,
                    "canon_examen: '%s' canon infixus fractus: "
                    "%.*s\n", via,
                    (integer)causa.mensura,
                    (constans character*)causa.datum);
                vitia_summa++;
            }
            alioquin
            {
                Xar* vitia;

                vitia = canon_iudicare(c_inf, r.radix, piscina);
                vitia_summa += vitia_imprimere(via, "infixus",
                                               vitia, machina);
                iudicata = VERUM;
            }
        }

        si (!iudicata)
        {
            fprintf(stderr,
                "canon_examen: '%s' canonem in %s non habet - "
                "NIHIL iudicatum\n", via, REGISTRUM);
            perge;
        }
        iudicatae++;
    }

    /* NIHIL IUDICATUM et NIHIL INVENTUM: non successus. Plagula
     * parsari nescia vitium est (exitus I), non absentia. */
    si (iudicatae == ZEPHYRUM && vitia_summa == ZEPHYRUM &&
        indexatae == ZEPHYRUM)
    {
        fprintf(stderr,
            "canon_examen: NULLA plagula iudicata est\n");
        redde II;
    }

    /* -index solum: TSV purum, sine summario */
    si (!machina && iudicatae > ZEPHYRUM)
    {
        imprimere("canon_examen: plagulae %u / VITIA %u\n",
                  iudicatae, vitia_summa);
    }

    redde vitia_summa > ZEPHYRUM ? I : ZEPHYRUM;
}
