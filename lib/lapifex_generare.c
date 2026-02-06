/* lapifex_generare.c - Implementatio Generatoris Grammaticae
 *
 * Legit grammaticam ex STML, computat FIRST coniuncta.
 */

#include "lapifex_generare.h"
#include "stml.h"
#include "tabula_dispersa.h"
#include <stdio.h>

/* ================================================
 * Functiones Auxiliares Internae
 * ================================================ */

/* Addere symbolum ad grammaticam, reddere indicem */
hic_manens s32
symbolum_addere(
    LapifexGrammatica*   grammatica,
    chorda*              titulus,
    chorda*              genus,
    b32                  est_terminale)
{
    LapifexSymbolum* sym;
    i32 index;

    index = (i32)xar_numerus(grammatica->symbola);
    sym = (LapifexSymbolum*)xar_addere(grammatica->symbola);
    si (!sym) redde -I;

    sym->titulus = titulus;
    sym->genus = genus;
    sym->index = (s32)index;
    sym->est_terminale = est_terminale;
    sym->first = xar_creare(grammatica->piscina, (i32)magnitudo(s32));
    sym->habet_epsilon = FALSUM;

    redde (s32)index;
}

/* Invenire symbolum per titulum in tabula dispersa, reddere indicem vel -1 */
hic_manens s32
symbolum_invenire(
    TabulaDispersa*      tabula_symbolorum,
    chorda*              titulus)
{
    vacuum* valor;

    si (tabula_dispersa_invenire(tabula_symbolorum, *titulus, &valor))
    {
        redde (s32)(s64)valor;
    }
    redde -I;
}

/* Verificare si coniunctum FIRST iam continet terminalem */
hic_manens b32
first_continet(
    Xar*  first,
    s32   terminalis_index)
{
    i32 i;
    i32 numerus;

    numerus = (i32)xar_numerus(first);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        s32* elem = (s32*)xar_obtinere(first, (i32)i);
        si (elem && *elem == terminalis_index) redde VERUM;
    }
    redde FALSUM;
}

/* Addere terminalem ad FIRST coniunctum si nondum praesens
 * Redde: VERUM si additum (mutatio facta)
 */
hic_manens b32
first_addere(
    Xar*  first,
    s32   terminalis_index)
{
    s32* novum;

    si (first_continet(first, terminalis_index)) redde FALSUM;

    novum = (s32*)xar_addere(first);
    si (novum)
    {
        *novum = terminalis_index;
        redde VERUM;
    }
    redde FALSUM;
}

/* ================================================
 * Legere Grammaticam ex STML
 * ================================================ */

LapifexGrammatica*
lapifex_grammaticam_legere(
    Piscina*                 piscina,
    InternamentumChorda*     intern,
    constans character*      stml_fons)
{
    LapifexGrammatica*  grammatica;
    StmlResultus        resultus;
    StmlNodus*          radix;
    StmlNodus*          terminalia_nodus;
    StmlNodus*          regulae_nodus;
    StmlNodus*          initium_nodus;
    Xar*                terminales;
    Xar*                regulae;
    TabulaDispersa*     tabula_symbolorum;
    i32                 i;

    si (!stml_fons || !piscina || !intern) redde NIHIL;

    /* Parsare STML */
    resultus = stml_legere_ex_literis(stml_fons, piscina, intern);
    si (!resultus.successus)
    {
        fprintf(stderr, "lapifex: error parsandi STML: linea %d\n",
                (int)resultus.linea_erroris);
        redde NIHIL;
    }

    radix = resultus.elementum_radix;
    si (!radix) redde NIHIL;

    /* Creare grammaticam */
    grammatica = (LapifexGrammatica*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(LapifexGrammatica));
    grammatica->symbola = xar_creare(piscina, (i32)magnitudo(LapifexSymbolum));
    grammatica->productiones = xar_creare(piscina, (i32)magnitudo(LapifexProductio));
    grammatica->initium_index = -I;
    grammatica->numerus_terminalium = ZEPHYRUM;
    grammatica->numerus_non_terminalium = ZEPHYRUM;
    grammatica->piscina = piscina;
    grammatica->intern = intern;

    /* Tabula symbolorum pro resolutione nominum */
    tabula_symbolorum = tabula_dispersa_creare_chorda(piscina, LXIV);

    /* ----------------------------------------
     * Pars I: Legere terminalia
     * ---------------------------------------- */
    terminalia_nodus = stml_invenire_liberum(radix, "terminalia");
    si (!terminalia_nodus)
    {
        fprintf(stderr, "lapifex: <terminalia> non inventum\n");
        redde NIHIL;
    }

    terminales = stml_invenire_omnes_liberos(terminalia_nodus, "terminalis", piscina);
    si (terminales)
    {
        per (i = ZEPHYRUM; i < (i32)xar_numerus(terminales); i++)
        {
            StmlNodus** nodus_ptr = (StmlNodus**)xar_obtinere(terminales, (i32)i);
            chorda*     tit;
            chorda*     gen;
            s32         idx;

            si (!nodus_ptr || !*nodus_ptr) perge;

            tit = stml_attributum_capere(*nodus_ptr, "titulus");
            gen = stml_attributum_capere(*nodus_ptr, "genus");

            si (!tit)
            {
                fprintf(stderr, "lapifex: <terminalis> sine titulo\n");
                redde NIHIL;
            }

            idx = symbolum_addere(grammatica, tit, gen, VERUM);
            si (idx < ZEPHYRUM) redde NIHIL;

            tabula_dispersa_inserere(tabula_symbolorum, *tit,
                (vacuum*)(s64)idx);

            grammatica->numerus_terminalium++;
        }
    }

    /* ----------------------------------------
     * Pars II: Legere regulae (non-terminalia)
     * Primo: creare symbola non-terminalia
     * ---------------------------------------- */
    regulae_nodus = stml_invenire_liberum(radix, "regulae");
    si (!regulae_nodus)
    {
        fprintf(stderr, "lapifex: <regulae> non inventum\n");
        redde NIHIL;
    }

    regulae = stml_invenire_omnes_liberos(regulae_nodus, "regula", piscina);
    si (regulae)
    {
        /* Primo passus: creare omnia symbola non-terminalia */
        per (i = ZEPHYRUM; i < (i32)xar_numerus(regulae); i++)
        {
            StmlNodus** nodus_ptr = (StmlNodus**)xar_obtinere(regulae, (i32)i);
            chorda*     tit;
            s32         idx;

            si (!nodus_ptr || !*nodus_ptr) perge;

            tit = stml_attributum_capere(*nodus_ptr, "titulus");
            si (!tit)
            {
                fprintf(stderr, "lapifex: <regula> sine titulo\n");
                redde NIHIL;
            }

            /* Si iam existit (e.g. iam definitum), praetere */
            si (tabula_dispersa_continet(tabula_symbolorum, *tit)) perge;

            idx = symbolum_addere(grammatica, tit, NIHIL, FALSUM);
            si (idx < ZEPHYRUM) redde NIHIL;

            tabula_dispersa_inserere(tabula_symbolorum, *tit,
                (vacuum*)(s64)idx);

            grammatica->numerus_non_terminalium++;
        }

        /* Secundus passus: creare productiones */
        per (i = ZEPHYRUM; i < (i32)xar_numerus(regulae); i++)
        {
            StmlNodus**  nodus_ptr = (StmlNodus**)xar_obtinere(regulae, (i32)i);
            chorda*      tit;
            s32          sinistrum_idx;
            Xar*         prod_nodi;
            i32          j;

            si (!nodus_ptr || !*nodus_ptr) perge;

            tit = stml_attributum_capere(*nodus_ptr, "titulus");
            sinistrum_idx = symbolum_invenire(tabula_symbolorum, tit);
            si (sinistrum_idx < ZEPHYRUM)
            {
                fprintf(stderr, "lapifex: symbolum '%.*s' non inventum\n",
                    (int)tit->mensura, (constans character*)tit->datum);
                redde NIHIL;
            }

            /* Iterare per <productio> liberos */
            prod_nodi = stml_invenire_omnes_liberos(*nodus_ptr, "productio", piscina);
            si (!prod_nodi) perge;

            per (j = ZEPHYRUM; j < (i32)xar_numerus(prod_nodi); j++)
            {
                StmlNodus**       prod_ptr = (StmlNodus**)xar_obtinere(prod_nodi, (i32)j);
                chorda            textus;
                chorda_fissio_fructus partes;
                LapifexProductio* prod;
                i32               k;

                si (!prod_ptr || !*prod_ptr) perge;

                /* Obtinere textum internum: "expr PLUS term" */
                textus = stml_textus_internus(*prod_ptr, piscina);
                textus = chorda_praecidere(textus);

                si (textus.mensura == ZEPHYRUM) perge;

                /* Dividere per spatia */
                partes = chorda_fissio(textus, ' ', piscina);

                /* Creare productionem */
                prod = (LapifexProductio*)xar_addere(grammatica->productiones);
                si (!prod) redde NIHIL;

                prod->sinistrum = sinistrum_idx;
                prod->dextrum = xar_creare(piscina, (i32)magnitudo(s32));
                prod->index = (s32)xar_numerus(grammatica->productiones) - I;

                /* Resolvere omnia symbola in dextro */
                per (k = ZEPHYRUM; k < partes.numerus; k++)
                {
                    chorda  pars = partes.elementa[k];
                    chorda  pars_praecisa = chorda_praecidere(pars);
                    s32     sym_idx;
                    s32*    novum;

                    si (pars_praecisa.mensura == ZEPHYRUM) perge;

                    /* Invenire symbolum in tabula */
                    sym_idx = symbolum_invenire(
                        tabula_symbolorum,
                        chorda_internare(intern, pars_praecisa));

                    si (sym_idx < ZEPHYRUM)
                    {
                        fprintf(stderr,
                            "lapifex: symbolum '%.*s' non inventum in productione\n",
                            (int)pars_praecisa.mensura,
                            (constans character*)pars_praecisa.datum);
                        redde NIHIL;
                    }

                    novum = (s32*)xar_addere(prod->dextrum);
                    si (novum) *novum = sym_idx;
                }
            }
        }
    }

    /* ----------------------------------------
     * Pars III: Initium symbolum
     * ---------------------------------------- */
    initium_nodus = stml_invenire_liberum(radix, "initium");
    si (initium_nodus)
    {
        chorda   textus = stml_textus_internus(initium_nodus, piscina);
        chorda   praecisa = chorda_praecidere(textus);
        chorda*  tit_internum;
        s32      initium_idx;

        tit_internum = chorda_internare(intern, praecisa);
        initium_idx = symbolum_invenire(tabula_symbolorum, tit_internum);

        si (initium_idx < ZEPHYRUM)
        {
            fprintf(stderr, "lapifex: initium symbolum '%.*s' non inventum\n",
                (int)praecisa.mensura, (constans character*)praecisa.datum);
            redde NIHIL;
        }

        grammatica->initium_index = initium_idx;
    }
    alioquin
    {
        fprintf(stderr, "lapifex: <initium> non inventum\n");
        redde NIHIL;
    }

    /* ----------------------------------------
     * Pars IV: Productio augmentata S' -> initium EOF
     * ---------------------------------------- */
    {
        chorda*          tit_augmentatum;
        s32              aug_idx;
        s32              eof_idx;
        LapifexProductio* prod;
        s32*             novum;

        tit_augmentatum = chorda_internare_ex_literis(intern, "__initium__");
        aug_idx = symbolum_addere(grammatica, tit_augmentatum, NIHIL, FALSUM);
        si (aug_idx < ZEPHYRUM) redde NIHIL;

        tabula_dispersa_inserere(tabula_symbolorum, *tit_augmentatum,
            (vacuum*)(s64)aug_idx);
        grammatica->numerus_non_terminalium++;

        /* Invenire EOF terminalem */
        eof_idx = symbolum_invenire(tabula_symbolorum,
            chorda_internare_ex_literis(intern, "EOF"));

        /* Creare S' -> initium EOF (vel solum initium si EOF non existit) */
        prod = (LapifexProductio*)xar_addere(grammatica->productiones);
        si (!prod) redde NIHIL;

        prod->sinistrum = aug_idx;
        prod->dextrum = xar_creare(piscina, (i32)magnitudo(s32));
        prod->index = (s32)xar_numerus(grammatica->productiones) - I;

        novum = (s32*)xar_addere(prod->dextrum);
        si (novum) *novum = grammatica->initium_index;

        si (eof_idx >= ZEPHYRUM)
        {
            novum = (s32*)xar_addere(prod->dextrum);
            si (novum) *novum = eof_idx;
        }

        /* Initium nunc est productio augmentata */
        grammatica->initium_index = aug_idx;
    }

    redde grammatica;
}

/* ================================================
 * FIRST Computatio
 * ================================================ */

b32
lapifex_first_computare(
    LapifexGrammatica*  grammatica)
{
    i32  numerus_symbolorum;
    i32  numerus_productionum;
    b32  mutatum;
    i32  i;

    si (!grammatica) redde FALSUM;

    numerus_symbolorum = (i32)xar_numerus(grammatica->symbola);
    numerus_productionum = (i32)xar_numerus(grammatica->productiones);

    /* Initializare: pro terminalibus, FIRST(t) = {t} */
    per (i = ZEPHYRUM; i < numerus_symbolorum; i++)
    {
        LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)i);
        si (sym && sym->est_terminale)
        {
            first_addere(sym->first, sym->index);
        }
    }

    /* Iterare usque dum nihil mutatur */
    fac
    {
        mutatum = FALSUM;

        per (i = ZEPHYRUM; i < numerus_productionum; i++)
        {
            LapifexProductio* prod = (LapifexProductio*)xar_obtinere(
                grammatica->productiones, (i32)i);
            LapifexSymbolum*  sinistrum;
            i32               numerus_dextrum;
            i32               j;
            b32               omnia_habent_epsilon;

            si (!prod) perge;

            sinistrum = (LapifexSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)prod->sinistrum);
            si (!sinistrum) perge;

            numerus_dextrum = (i32)xar_numerus(prod->dextrum);
            omnia_habent_epsilon = VERUM;

            /* Pro unaquaque A -> X1 X2 ... Xn */
            per (j = ZEPHYRUM; j < numerus_dextrum; j++)
            {
                s32*             idx_ptr = (s32*)xar_obtinere(prod->dextrum, (i32)j);
                LapifexSymbolum* xj;
                i32              k;
                i32              numerus_first;

                si (!idx_ptr) frange;

                xj = (LapifexSymbolum*)xar_obtinere(
                    grammatica->symbola, (i32)*idx_ptr);
                si (!xj) frange;

                /* Addere FIRST(Xj) - {epsilon} ad FIRST(A) */
                numerus_first = (i32)xar_numerus(xj->first);
                per (k = ZEPHYRUM; k < numerus_first; k++)
                {
                    s32* term_idx = (s32*)xar_obtinere(xj->first, (i32)k);
                    si (term_idx)
                    {
                        si (first_addere(sinistrum->first, *term_idx))
                        {
                            mutatum = VERUM;
                        }
                    }
                }

                /* Si Xj non habet epsilon, terminare */
                si (!xj->habet_epsilon)
                {
                    omnia_habent_epsilon = FALSUM;
                    frange;
                }
            }

            /* Si omnia Xj habent epsilon, addere epsilon ad FIRST(A) */
            si (omnia_habent_epsilon && numerus_dextrum > ZEPHYRUM)
            {
                si (!sinistrum->habet_epsilon)
                {
                    sinistrum->habet_epsilon = VERUM;
                    mutatum = VERUM;
                }
            }

            /* Productio vacua: A -> epsilon */
            si (numerus_dextrum == ZEPHYRUM)
            {
                si (!sinistrum->habet_epsilon)
                {
                    sinistrum->habet_epsilon = VERUM;
                    mutatum = VERUM;
                }
            }
        }
    } dum (mutatum);

    redde VERUM;
}

/* ================================================
 * Quaestio FIRST
 * ================================================ */

Xar*
lapifex_first_obtinere(
    LapifexGrammatica*  grammatica,
    i32                 symbolum_index)
{
    LapifexSymbolum* sym;

    si (!grammatica) redde NIHIL;

    sym = (LapifexSymbolum*)xar_obtinere(grammatica->symbola, symbolum_index);
    si (!sym) redde NIHIL;

    redde sym->first;
}

/* ================================================
 * Imprimere
 * ================================================ */

vacuum
lapifex_grammaticam_imprimere(
    LapifexGrammatica*  grammatica)
{
    i32 i;

    si (!grammatica) redde;

    imprimere("=== Grammatica ===\n");
    imprimere("Terminalia: %d\n", (int)grammatica->numerus_terminalium);
    imprimere("Non-terminalia: %d\n", (int)grammatica->numerus_non_terminalium);
    imprimere("Productiones: %d\n", (int)xar_numerus(grammatica->productiones));
    imprimere("Initium: %d\n\n", (int)grammatica->initium_index);

    /* Symbola */
    imprimere("--- Symbola ---\n");
    per (i = ZEPHYRUM; i < (i32)xar_numerus(grammatica->symbola); i++)
    {
        LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)i);
        si (sym)
        {
            imprimere("  [%d] %.*s (%s)\n",
                (int)sym->index,
                (int)sym->titulus->mensura,
                (constans character*)sym->titulus->datum,
                sym->est_terminale ? "terminale" : "non-terminale");
        }
    }

    /* Productiones */
    imprimere("\n--- Productiones ---\n");
    per (i = ZEPHYRUM; i < (i32)xar_numerus(grammatica->productiones); i++)
    {
        LapifexProductio* prod = (LapifexProductio*)xar_obtinere(
            grammatica->productiones, (i32)i);
        si (prod)
        {
            LapifexSymbolum* sinistrum = (LapifexSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)prod->sinistrum);
            i32 j;

            si (sinistrum)
            {
                imprimere("  P%d: %.*s ->",
                    (int)prod->index,
                    (int)sinistrum->titulus->mensura,
                    (constans character*)sinistrum->titulus->datum);
            }

            per (j = ZEPHYRUM; j < (i32)xar_numerus(prod->dextrum); j++)
            {
                s32* idx = (s32*)xar_obtinere(prod->dextrum, (i32)j);
                si (idx)
                {
                    LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                        grammatica->symbola, (i32)*idx);
                    si (sym)
                    {
                        imprimere(" %.*s",
                            (int)sym->titulus->mensura,
                            (constans character*)sym->titulus->datum);
                    }
                }
            }
            imprimere("\n");
        }
    }
}

vacuum
lapifex_first_imprimere(
    LapifexGrammatica*  grammatica)
{
    i32 i;

    si (!grammatica) redde;

    imprimere("\n=== FIRST Coniuncta ===\n");
    per (i = ZEPHYRUM; i < (i32)xar_numerus(grammatica->symbola); i++)
    {
        LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)i);
        si (sym && !sym->est_terminale)
        {
            i32 j;
            imprimere("  FIRST(%.*s) = {",
                (int)sym->titulus->mensura,
                (constans character*)sym->titulus->datum);

            per (j = ZEPHYRUM; j < (i32)xar_numerus(sym->first); j++)
            {
                s32* idx = (s32*)xar_obtinere(sym->first, (i32)j);
                si (idx)
                {
                    LapifexSymbolum* term = (LapifexSymbolum*)xar_obtinere(
                        grammatica->symbola, (i32)*idx);
                    si (term)
                    {
                        si (j > ZEPHYRUM) imprimere(", ");
                        imprimere("%.*s",
                            (int)term->titulus->mensura,
                            (constans character*)term->titulus->datum);
                    }
                }
            }

            si (sym->habet_epsilon) imprimere(", epsilon");
            imprimere("}\n");
        }
    }
}
