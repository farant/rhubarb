/* lapifex_generare.c - Implementatio Generatoris Grammaticae
 *
 * Legit grammaticam ex STML, computat FIRST coniuncta.
 */

#include "lapifex_generare.h"
#include "stml.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

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
    sym->exemplum = NIHIL;
    sym->index = (s32)index;
    sym->est_terminale = est_terminale;
    sym->first = xar_creare(grammatica->piscina, (i32)magnitudo(s32));
    sym->follow = xar_creare(grammatica->piscina, (i32)magnitudo(s32));
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

            /* Legere exemplum attributum pro lexere */
            {
                chorda* exemplum_attr;
                exemplum_attr = stml_attributum_capere(*nodus_ptr, "exemplum");
                si (exemplum_attr)
                {
                    LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                        grammatica->symbola, (i32)idx);
                    si (sym) sym->exemplum = exemplum_attr;
                }
            }

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

/* ================================================
 * FOLLOW Computatio
 * ================================================ */

/* Sentinella pro EOF/$: usamus -1 */
#define LAPIFEX_EOF_PROSPECTUS (-1)

/* Auxiliaris: addere terminalem ad FOLLOW coniunctum si nondum praesens
 * Redde: VERUM si additum (mutatio facta)
 */
hic_manens b32
follow_addere(
    Xar*  follow,
    s32   terminalis_index)
{
    redde first_addere(follow, terminalis_index);
}

/* Computare FIRST(beta a) — FIRST sequentiae symbolorum cum prospectu
 * symbola_indices: tabula de s32 indices symbolorum
 * numerus: longitudo tabulae
 * prospectus: terminalis prospectus ad appendere
 * fructus: Xar de s32 ubi scribimus resultus
 */
hic_manens vacuum
first_sequentiae_computare(
    LapifexGrammatica* grammatica,
    s32* symbola_indices, i32 numerus,
    s32 prospectus,
    Xar* fructus)
{
    i32 i;
    b32 omnia_habent_epsilon;

    omnia_habent_epsilon = VERUM;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)symbola_indices[i]);
        i32 j;
        i32 num_first;

        si (!sym) frange;

        /* Addere FIRST(sym) - {epsilon} ad fructus */
        num_first = (i32)xar_numerus(sym->first);
        per (j = ZEPHYRUM; j < num_first; j++)
        {
            s32* idx = (s32*)xar_obtinere(sym->first, j);
            si (idx)
            {
                first_addere(fructus, *idx);
            }
        }

        si (!sym->habet_epsilon)
        {
            omnia_habent_epsilon = FALSUM;
            frange;
        }
    }

    /* Si omnia habent epsilon (vel numerus == 0), addere prospectum */
    si (omnia_habent_epsilon)
    {
        first_addere(fructus, prospectus);
    }
}

b32
lapifex_follow_computare(
    LapifexGrammatica*  grammatica)
{
    i32  numerus_productionum;
    b32  mutatum;
    i32  i;

    si (!grammatica) redde FALSUM;

    numerus_productionum = (i32)xar_numerus(grammatica->productiones);

    /* Addere EOF ($) ad FOLLOW(symbolum initiale) */
    {
        LapifexSymbolum* initium = (LapifexSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)grammatica->initium_index);
        si (initium)
        {
            follow_addere(initium->follow, (s32)LAPIFEX_EOF_PROSPECTUS);
        }
    }

    /* Iterare usque dum nihil mutatur */
    fac
    {
        mutatum = FALSUM;

        per (i = ZEPHYRUM; i < numerus_productionum; i++)
        {
            LapifexProductio* prod = (LapifexProductio*)xar_obtinere(
                grammatica->productiones, i);
            LapifexSymbolum*  sinistrum;
            i32               num_dextrum;
            i32               j;

            si (!prod) perge;

            sinistrum = (LapifexSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)prod->sinistrum);
            si (!sinistrum) perge;

            num_dextrum = (i32)xar_numerus(prod->dextrum);

            /* Pro unoquoque symbolo B in dextro: A -> alpha B beta */
            per (j = ZEPHYRUM; j < num_dextrum; j++)
            {
                s32* b_idx_ptr = (s32*)xar_obtinere(prod->dextrum, j);
                LapifexSymbolum* b_sym;
                i32 k;
                b32 beta_habet_epsilon;

                si (!b_idx_ptr) perge;

                b_sym = (LapifexSymbolum*)xar_obtinere(
                    grammatica->symbola, (i32)*b_idx_ptr);
                si (!b_sym) perge;

                /* Solum non-terminalia habent FOLLOW significantem */
                si (b_sym->est_terminale) perge;

                /* Addere FIRST(beta) - {epsilon} ad FOLLOW(B) */
                beta_habet_epsilon = VERUM;
                per (k = j + I; k < num_dextrum; k++)
                {
                    s32* beta_idx_ptr = (s32*)xar_obtinere(prod->dextrum, k);
                    LapifexSymbolum* beta_sym;
                    i32 m;
                    i32 num_first;

                    si (!beta_idx_ptr) frange;

                    beta_sym = (LapifexSymbolum*)xar_obtinere(
                        grammatica->symbola, (i32)*beta_idx_ptr);
                    si (!beta_sym) frange;

                    num_first = (i32)xar_numerus(beta_sym->first);
                    per (m = ZEPHYRUM; m < num_first; m++)
                    {
                        s32* term_idx = (s32*)xar_obtinere(beta_sym->first, m);
                        si (term_idx)
                        {
                            si (follow_addere(b_sym->follow, *term_idx))
                            {
                                mutatum = VERUM;
                            }
                        }
                    }

                    si (!beta_sym->habet_epsilon)
                    {
                        beta_habet_epsilon = FALSUM;
                        frange;
                    }
                }

                /* Si beta est vacuum vel omnia in beta habent epsilon:
                 * addere FOLLOW(A) ad FOLLOW(B)
                 */
                si (beta_habet_epsilon)
                {
                    i32 num_follow_a = (i32)xar_numerus(sinistrum->follow);
                    i32 m;
                    per (m = ZEPHYRUM; m < num_follow_a; m++)
                    {
                        s32* f_idx = (s32*)xar_obtinere(sinistrum->follow, m);
                        si (f_idx)
                        {
                            si (follow_addere(b_sym->follow, *f_idx))
                            {
                                mutatum = VERUM;
                            }
                        }
                    }
                }
            }
        }
    } dum (mutatum);

    redde VERUM;
}

Xar*
lapifex_follow_obtinere(
    LapifexGrammatica*  grammatica,
    i32                 symbolum_index)
{
    LapifexSymbolum* sym;

    si (!grammatica) redde NIHIL;

    sym = (LapifexSymbolum*)xar_obtinere(grammatica->symbola, symbolum_index);
    si (!sym) redde NIHIL;

    redde sym->follow;
}

vacuum
lapifex_follow_imprimere(
    LapifexGrammatica*  grammatica)
{
    i32 i;

    si (!grammatica) redde;

    imprimere("\n=== FOLLOW Coniuncta ===\n");
    per (i = ZEPHYRUM; i < (i32)xar_numerus(grammatica->symbola); i++)
    {
        LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
            grammatica->symbola, i);
        si (sym && !sym->est_terminale)
        {
            i32 j;
            imprimere("  FOLLOW(%.*s) = {",
                (int)sym->titulus->mensura,
                (constans character*)sym->titulus->datum);

            per (j = ZEPHYRUM; j < (i32)xar_numerus(sym->follow); j++)
            {
                s32* idx = (s32*)xar_obtinere(sym->follow, j);
                si (idx)
                {
                    si (j > ZEPHYRUM) imprimere(", ");
                    si (*idx == (s32)LAPIFEX_EOF_PROSPECTUS)
                    {
                        imprimere("$");
                    }
                    alioquin
                    {
                        LapifexSymbolum* term = (LapifexSymbolum*)xar_obtinere(
                            grammatica->symbola, (i32)*idx);
                        si (term)
                        {
                            imprimere("%.*s",
                                (int)term->titulus->mensura,
                                (constans character*)term->titulus->datum);
                        }
                    }
                }
            }
            imprimere("}\n");
        }
    }
}

/* ================================================
 * LR(1) Collectio Canonica
 * ================================================ */

/* Auxiliaris: verificare si res iam in coniuncto */
hic_manens b32
res_in_coniuncto(
    Xar*         res_xar,
    LapifexRes*  res)
{
    i32 i;
    i32 numerus = (i32)xar_numerus(res_xar);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        LapifexRes* existens = (LapifexRes*)xar_obtinere(res_xar, i);
        si (existens &&
            existens->productio == res->productio &&
            existens->punctum == res->punctum &&
            existens->prospectus == res->prospectus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Clausura LR(1): expandere coniunctum rerum */
hic_manens vacuum
clausura_computare(
    LapifexGrammatica*  grammatica,
    Xar*                res_xar)
{
    b32 mutatum;
    i32 numerus_productionum;

    numerus_productionum = (i32)xar_numerus(grammatica->productiones);

    fac
    {
        i32 i;
        i32 numerus_rerum;

        mutatum = FALSUM;
        numerus_rerum = (i32)xar_numerus(res_xar);

        per (i = ZEPHYRUM; i < numerus_rerum; i++)
        {
            LapifexRes*       res_currens;
            LapifexProductio* prod;
            i32               num_dextrum;
            s32*              b_idx_ptr;
            LapifexSymbolum*  b_sym;
            i32               j;

            res_currens = (LapifexRes*)xar_obtinere(res_xar, i);
            si (!res_currens) perge;

            prod = (LapifexProductio*)xar_obtinere(
                grammatica->productiones, (i32)res_currens->productio);
            si (!prod) perge;

            num_dextrum = (i32)xar_numerus(prod->dextrum);

            /* Si punctum est ad finem, nihil expandere */
            si ((i32)res_currens->punctum >= num_dextrum) perge;

            /* Obtinere symbolum post punctum: B */
            b_idx_ptr = (s32*)xar_obtinere(prod->dextrum,
                (i32)res_currens->punctum);
            si (!b_idx_ptr) perge;

            b_sym = (LapifexSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)*b_idx_ptr);
            si (!b_sym || b_sym->est_terminale) perge;

            /* Pro [A -> alpha . B beta, a]:
             * beta = symbola post B (punctum+1 .. finem)
             * Computare FIRST(beta a)
             */
            {
                Xar*  first_beta_a;
                i32   num_beta;
                s32*  beta_indices;
                i32   k;

                num_beta = num_dextrum - (i32)res_currens->punctum - I;
                beta_indices = NIHIL;

                si (num_beta > ZEPHYRUM)
                {
                    beta_indices = (s32*)piscina_allocare(
                        grammatica->piscina,
                        (memoriae_index)((i32)magnitudo(s32) * num_beta));
                    per (k = ZEPHYRUM; k < num_beta; k++)
                    {
                        s32* sym_idx = (s32*)xar_obtinere(prod->dextrum,
                            (i32)res_currens->punctum + I + k);
                        si (sym_idx)
                        {
                            beta_indices[k] = *sym_idx;
                        }
                    }
                }

                first_beta_a = xar_creare(grammatica->piscina,
                    (i32)magnitudo(s32));

                first_sequentiae_computare(
                    grammatica,
                    beta_indices, num_beta,
                    res_currens->prospectus,
                    first_beta_a);

                /* Pro unaquaque productione B -> gamma */
                per (j = ZEPHYRUM; j < numerus_productionum; j++)
                {
                    LapifexProductio* prod_b;
                    i32 num_first_ba;
                    i32 m;

                    prod_b = (LapifexProductio*)xar_obtinere(
                        grammatica->productiones, j);
                    si (!prod_b || prod_b->sinistrum != *b_idx_ptr) perge;

                    /* Pro unoquoque terminale b in FIRST(beta a) */
                    num_first_ba = (i32)xar_numerus(first_beta_a);
                    per (m = ZEPHYRUM; m < num_first_ba; m++)
                    {
                        s32* b_term = (s32*)xar_obtinere(first_beta_a, m);
                        LapifexRes nova_res;

                        si (!b_term) perge;

                        nova_res.productio = prod_b->index;
                        nova_res.punctum = ZEPHYRUM;
                        nova_res.prospectus = *b_term;

                        si (!res_in_coniuncto(res_xar, &nova_res))
                        {
                            LapifexRes* addita = (LapifexRes*)xar_addere(res_xar);
                            si (addita)
                            {
                                *addita = nova_res;
                                mutatum = VERUM;
                            }
                        }
                    }
                }
            }
        }
    } dum (mutatum);
}

/* Goto LR(1): movere punctum trans symbolum */
hic_manens Xar*
goto_computare(
    LapifexGrammatica*  grammatica,
    Xar*                res_xar,
    s32                 symbolum)
{
    Xar* nova_res;
    i32  i;
    i32  numerus;

    nova_res = xar_creare(grammatica->piscina, (i32)magnitudo(LapifexRes));
    numerus = (i32)xar_numerus(res_xar);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        LapifexRes*       res_currens;
        LapifexProductio* prod;
        i32               num_dextrum;
        s32*              sym_idx_ptr;

        res_currens = (LapifexRes*)xar_obtinere(res_xar, i);
        si (!res_currens) perge;

        prod = (LapifexProductio*)xar_obtinere(
            grammatica->productiones, (i32)res_currens->productio);
        si (!prod) perge;

        num_dextrum = (i32)xar_numerus(prod->dextrum);
        si ((i32)res_currens->punctum >= num_dextrum) perge;

        sym_idx_ptr = (s32*)xar_obtinere(prod->dextrum,
            (i32)res_currens->punctum);
        si (!sym_idx_ptr || *sym_idx_ptr != symbolum) perge;

        /* Movere punctum */
        {
            LapifexRes* nova = (LapifexRes*)xar_addere(nova_res);
            si (nova)
            {
                nova->productio = res_currens->productio;
                nova->punctum = res_currens->punctum + I;
                nova->prospectus = res_currens->prospectus;
            }
        }
    }

    /* Clausura si non vacuum */
    si ((i32)xar_numerus(nova_res) > ZEPHYRUM)
    {
        clausura_computare(grammatica, nova_res);
    }

    redde nova_res;
}

/* Comparator pro ordinatione rerum: (productio, punctum, prospectus) */
hic_manens s32
res_comparator(constans vacuum* a, constans vacuum* b)
{
    constans LapifexRes* ra = (constans LapifexRes*)a;
    constans LapifexRes* rb = (constans LapifexRes*)b;

    si (ra->productio != rb->productio)
        redde (ra->productio < rb->productio) ? -I : I;
    si (ra->punctum != rb->punctum)
        redde (ra->punctum < rb->punctum) ? -I : I;
    si (ra->prospectus != rb->prospectus)
        redde (ra->prospectus < rb->prospectus) ? -I : I;
    redde ZEPHYRUM;
}

/* Verificare si duo coniuncta rerum sunt aequalia */
hic_manens b32
status_aequales(
    Xar*  res_a,
    Xar*  res_b)
{
    i32 num_a;
    i32 num_b;
    i32 i;

    num_a = (i32)xar_numerus(res_a);
    num_b = (i32)xar_numerus(res_b);

    si (num_a != num_b) redde FALSUM;

    /* Comparare elementum per elementum (assumimus ordinata) */
    per (i = ZEPHYRUM; i < num_a; i++)
    {
        LapifexRes* a = (LapifexRes*)xar_obtinere(res_a, i);
        LapifexRes* b = (LapifexRes*)xar_obtinere(res_b, i);

        si (!a || !b) redde FALSUM;
        si (a->productio != b->productio ||
            a->punctum != b->punctum ||
            a->prospectus != b->prospectus)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Invenire statum cum eodem coniuncto rerum, reddere indicem vel -1 */
hic_manens s32
statum_invenire(
    Xar*  status_omnes,
    Xar*  res_xar)
{
    i32 i;
    i32 numerus = (i32)xar_numerus(status_omnes);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        LapifexStatus* s = (LapifexStatus*)xar_obtinere(status_omnes, i);
        si (s && status_aequales(s->res, res_xar))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* Colligere omnia symbola quae apparent post punctum in coniuncto rerum */
hic_manens Xar*
symbola_post_punctum_colligere(
    LapifexGrammatica*  grammatica,
    Xar*                res_xar)
{
    Xar* symbola;
    i32  i;
    i32  numerus;

    symbola = xar_creare(grammatica->piscina, (i32)magnitudo(s32));
    numerus = (i32)xar_numerus(res_xar);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        LapifexRes*       res_currens;
        LapifexProductio* prod;
        s32*              sym_idx_ptr;

        res_currens = (LapifexRes*)xar_obtinere(res_xar, i);
        si (!res_currens) perge;

        prod = (LapifexProductio*)xar_obtinere(
            grammatica->productiones, (i32)res_currens->productio);
        si (!prod) perge;

        si ((i32)res_currens->punctum >= (i32)xar_numerus(prod->dextrum)) perge;

        sym_idx_ptr = (s32*)xar_obtinere(prod->dextrum,
            (i32)res_currens->punctum);
        si (!sym_idx_ptr) perge;

        /* Addere si nondum in lista */
        si (!first_continet(symbola, *sym_idx_ptr))
        {
            s32* novum = (s32*)xar_addere(symbola);
            si (novum) *novum = *sym_idx_ptr;
        }
    }

    redde symbola;
}

LapifexCollectio*
lapifex_collectio_construere(
    LapifexGrammatica*  grammatica)
{
    LapifexCollectio*   collectio;
    Xar*                opus;  /* worklist de s32 (indices statuum) */
    LapifexProductio*   prod_augmentata;
    i32                 num_prod;
    i32                 i;

    si (!grammatica) redde NIHIL;

    num_prod = (i32)xar_numerus(grammatica->productiones);

    /* Invenire productionem augmentatam: S' -> ... */
    prod_augmentata = NIHIL;
    per (i = ZEPHYRUM; i < num_prod; i++)
    {
        LapifexProductio* p = (LapifexProductio*)xar_obtinere(
            grammatica->productiones, i);
        si (p && p->sinistrum == grammatica->initium_index)
        {
            prod_augmentata = p;
            frange;
        }
    }

    si (!prod_augmentata)
    {
        fprintf(stderr, "lapifex: productio augmentata non inventa\n");
        redde NIHIL;
    }

    /* Creare collectionem */
    collectio = (LapifexCollectio*)piscina_allocare(
        grammatica->piscina,
        (memoriae_index)magnitudo(LapifexCollectio));
    collectio->status_omnes = xar_creare(grammatica->piscina,
        (i32)magnitudo(LapifexStatus));
    collectio->transitiones = xar_creare(grammatica->piscina,
        (i32)magnitudo(LapifexTransitio));
    collectio->grammatica = grammatica;

    opus = xar_creare(grammatica->piscina, (i32)magnitudo(s32));

    /* Status 0: clausura({[S' -> . initium, $]}) */
    {
        Xar*           res_initium;
        LapifexRes*    res_prima;
        LapifexStatus* status_0;
        s32*           opus_elem;

        res_initium = xar_creare(grammatica->piscina,
            (i32)magnitudo(LapifexRes));
        res_prima = (LapifexRes*)xar_addere(res_initium);
        res_prima->productio = prod_augmentata->index;
        res_prima->punctum = ZEPHYRUM;
        res_prima->prospectus = (s32)LAPIFEX_EOF_PROSPECTUS;

        clausura_computare(grammatica, res_initium);
        xar_ordinare(res_initium, res_comparator);

        status_0 = (LapifexStatus*)xar_addere(collectio->status_omnes);
        status_0->res = res_initium;
        status_0->index = ZEPHYRUM;

        opus_elem = (s32*)xar_addere(opus);
        *opus_elem = ZEPHYRUM;
    }

    /* Algorithmus worklist */
    {
        i32 opus_index = ZEPHYRUM;

        dum (opus_index < (i32)xar_numerus(opus))
        {
            s32*           stat_idx_ptr;
            LapifexStatus* status_currens;
            Xar*           symbola;
            i32            j;

            stat_idx_ptr = (s32*)xar_obtinere(opus, opus_index);
            opus_index++;

            si (!stat_idx_ptr) perge;

            status_currens = (LapifexStatus*)xar_obtinere(
                collectio->status_omnes, (i32)*stat_idx_ptr);
            si (!status_currens) perge;

            /* Colligere symbola post punctum */
            symbola = symbola_post_punctum_colligere(
                grammatica, status_currens->res);

            per (j = ZEPHYRUM; j < (i32)xar_numerus(symbola); j++)
            {
                s32* sym_ptr = (s32*)xar_obtinere(symbola, j);
                Xar* nova_res;
                s32  status_novus_idx;

                si (!sym_ptr) perge;

                nova_res = goto_computare(
                    grammatica, status_currens->res, *sym_ptr);

                si ((i32)xar_numerus(nova_res) == ZEPHYRUM) perge;

                /* Ordinare pro comparatione */
                xar_ordinare(nova_res, res_comparator);

                /* Verificare si status iam existit */
                status_novus_idx = statum_invenire(
                    collectio->status_omnes, nova_res);

                si (status_novus_idx < ZEPHYRUM)
                {
                    /* Novus status */
                    LapifexStatus* novus;
                    s32*           opus_elem;

                    status_novus_idx =
                        (s32)xar_numerus(collectio->status_omnes);
                    novus = (LapifexStatus*)xar_addere(
                        collectio->status_omnes);
                    novus->res = nova_res;
                    novus->index = status_novus_idx;

                    opus_elem = (s32*)xar_addere(opus);
                    *opus_elem = status_novus_idx;
                }

                /* Notare transitionem */
                {
                    LapifexTransitio* trans;
                    trans = (LapifexTransitio*)xar_addere(
                        collectio->transitiones);
                    trans->status = *stat_idx_ptr;
                    trans->symbolum = *sym_ptr;
                    trans->status_novus = status_novus_idx;
                }
            }
        }
    }

    redde collectio;
}

/* ================================================
 * Imprimere Collectionem
 * ================================================ */

/* ================================================
 * ACTION/GOTO Tabula
 * ================================================ */

/* Auxiliaris: verificare si actio iam existit in Xar */
hic_manens b32
actio_iam_existit(
    Xar*                   actiones,
    LapifexActioIntroitus* nova)
{
    i32 i;
    i32 numerus = (i32)xar_numerus(actiones);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        LapifexActioIntroitus* existens =
            (LapifexActioIntroitus*)xar_obtinere(actiones, i);
        si (existens &&
            existens->terminalis == nova->terminalis &&
            existens->actio == nova->actio &&
            existens->valor == nova->valor)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Auxiliaris: detegere conflictus in tabula */
hic_manens vacuum
conflictus_detegere(
    LapifexTabula* tabula)
{
    i32 s;
    i32 num_status;

    tabula->numerus_conflictuum = ZEPHYRUM;
    num_status = (i32)xar_numerus(tabula->status_tabulae);

    per (s = ZEPHYRUM; s < num_status; s++)
    {
        LapifexStatusTabula* st =
            (LapifexStatusTabula*)xar_obtinere(tabula->status_tabulae, s);
        i32 num_actiones;
        i32 i;
        i32 j;

        si (!st) perge;

        num_actiones = (i32)xar_numerus(st->actiones);
        st->habet_conflictum = FALSUM;

        per (i = ZEPHYRUM; i < num_actiones; i++)
        {
            LapifexActioIntroitus* ai =
                (LapifexActioIntroitus*)xar_obtinere(st->actiones, i);
            si (!ai) perge;

            per (j = i + I; j < num_actiones; j++)
            {
                LapifexActioIntroitus* aj =
                    (LapifexActioIntroitus*)xar_obtinere(st->actiones, j);
                si (!aj) perge;

                si (ai->terminalis == aj->terminalis)
                {
                    ai->conflictus_intentus = VERUM;
                    aj->conflictus_intentus = VERUM;
                    st->habet_conflictum = VERUM;
                    tabula->numerus_conflictuum++;
                }
            }
        }
    }
}

LapifexTabula*
lapifex_tabulam_construere(
    LapifexCollectio*  collectio)
{
    LapifexTabula*      tabula;
    LapifexGrammatica*  grammatica;
    Piscina*            piscina;
    i32                 num_status;
    i32                 num_transitiones;
    i32                 s;
    i32                 t;

    si (!collectio || !collectio->grammatica) redde NIHIL;

    grammatica = collectio->grammatica;
    piscina = grammatica->piscina;

    /* Creare tabulam */
    tabula = (LapifexTabula*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(LapifexTabula));
    tabula->grammatica = grammatica;
    tabula->collectio = collectio;
    tabula->numerus_conflictuum = ZEPHYRUM;

    num_status = (i32)xar_numerus(collectio->status_omnes);
    tabula->status_tabulae = xar_creare(piscina,
        (i32)magnitudo(LapifexStatusTabula));

    /* Initializare tabulam pro unoquoque statu */
    per (s = ZEPHYRUM; s < num_status; s++)
    {
        LapifexStatusTabula* st =
            (LapifexStatusTabula*)xar_addere(tabula->status_tabulae);
        st->actiones = xar_creare(piscina,
            (i32)magnitudo(LapifexActioIntroitus));
        st->goto_introitus = xar_creare(piscina,
            (i32)magnitudo(LapifexGotoIntroitus));
        st->index = (s32)s;
        st->habet_conflictum = FALSUM;
    }

    /* Passus 1: Transitiones -> SHIFT (terminalia) et GOTO (non-terminalia) */
    num_transitiones = (i32)xar_numerus(collectio->transitiones);

    per (t = ZEPHYRUM; t < num_transitiones; t++)
    {
        LapifexTransitio* trans =
            (LapifexTransitio*)xar_obtinere(collectio->transitiones, t);
        LapifexSymbolum*  sym;
        LapifexStatusTabula* st;

        si (!trans) perge;

        sym = (LapifexSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)trans->symbolum);
        si (!sym) perge;

        st = (LapifexStatusTabula*)xar_obtinere(
            tabula->status_tabulae, (i32)trans->status);
        si (!st) perge;

        si (sym->est_terminale)
        {
            /* SHIFT actio */
            LapifexActioIntroitus nova;
            nova.terminalis = trans->symbolum;
            nova.actio = LAPIFEX_ACTIO_TRANSPONERE;
            nova.valor = trans->status_novus;
            nova.conflictus_intentus = FALSUM;

            si (!actio_iam_existit(st->actiones, &nova))
            {
                LapifexActioIntroitus* addita =
                    (LapifexActioIntroitus*)xar_addere(st->actiones);
                si (addita) *addita = nova;
            }
        }
        alioquin
        {
            /* GOTO introitus */
            LapifexGotoIntroitus* gi =
                (LapifexGotoIntroitus*)xar_addere(st->goto_introitus);
            si (gi)
            {
                gi->non_terminalis = trans->symbolum;
                gi->status_novus = trans->status_novus;
            }
        }
    }

    /* Passus 2: Res cum puncto ad finem -> REDUCE vel ACCEPT */
    per (s = ZEPHYRUM; s < num_status; s++)
    {
        LapifexStatus* status_lr =
            (LapifexStatus*)xar_obtinere(collectio->status_omnes, s);
        LapifexStatusTabula* st;
        i32 num_rerum;
        i32 r;

        si (!status_lr) perge;

        st = (LapifexStatusTabula*)xar_obtinere(
            tabula->status_tabulae, s);
        si (!st) perge;

        num_rerum = (i32)xar_numerus(status_lr->res);

        per (r = ZEPHYRUM; r < num_rerum; r++)
        {
            LapifexRes* res =
                (LapifexRes*)xar_obtinere(status_lr->res, r);
            LapifexProductio* prod;
            i32 num_dextrum;

            si (!res) perge;

            prod = (LapifexProductio*)xar_obtinere(
                grammatica->productiones, (i32)res->productio);
            si (!prod) perge;

            num_dextrum = (i32)xar_numerus(prod->dextrum);

            /* Punctum ad finem? */
            si ((i32)res->punctum == num_dextrum)
            {
                LapifexActioIntroitus nova;
                nova.conflictus_intentus = FALSUM;

                si (prod->sinistrum == grammatica->initium_index &&
                    res->prospectus == (s32)LAPIFEX_EOF_PROSPECTUS)
                {
                    /* ACCEPT */
                    nova.terminalis = (s32)LAPIFEX_EOF_PROSPECTUS;
                    nova.actio = LAPIFEX_ACTIO_ACCIPERE;
                    nova.valor = ZEPHYRUM;
                }
                alioquin
                {
                    /* REDUCE */
                    nova.terminalis = res->prospectus;
                    nova.actio = LAPIFEX_ACTIO_REDUCERE;
                    nova.valor = prod->index;
                }

                si (!actio_iam_existit(st->actiones, &nova))
                {
                    LapifexActioIntroitus* addita =
                        (LapifexActioIntroitus*)xar_addere(st->actiones);
                    si (addita) *addita = nova;
                }
            }
        }
    }

    /* Passus 3: Detegere conflictus */
    conflictus_detegere(tabula);

    redde tabula;
}

Xar*
lapifex_actiones_quaerere(
    LapifexTabula*  tabula,
    s32             status,
    s32             terminalis)
{
    LapifexStatusTabula* st;
    Xar* fructus;
    i32 i;
    i32 numerus;

    si (!tabula) redde NIHIL;

    st = (LapifexStatusTabula*)xar_obtinere(
        tabula->status_tabulae, (i32)status);
    si (!st) redde NIHIL;

    fructus = xar_creare(tabula->grammatica->piscina,
        (i32)magnitudo(LapifexActioIntroitus));

    numerus = (i32)xar_numerus(st->actiones);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        LapifexActioIntroitus* ai =
            (LapifexActioIntroitus*)xar_obtinere(st->actiones, i);
        si (ai && ai->terminalis == terminalis)
        {
            LapifexActioIntroitus* copia =
                (LapifexActioIntroitus*)xar_addere(fructus);
            si (copia) *copia = *ai;
        }
    }

    redde fructus;
}

s32
lapifex_goto_quaerere(
    LapifexTabula*  tabula,
    s32             status,
    s32             non_terminalis)
{
    LapifexStatusTabula* st;
    i32 i;
    i32 numerus;

    si (!tabula) redde -I;

    st = (LapifexStatusTabula*)xar_obtinere(
        tabula->status_tabulae, (i32)status);
    si (!st) redde -I;

    numerus = (i32)xar_numerus(st->goto_introitus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        LapifexGotoIntroitus* gi =
            (LapifexGotoIntroitus*)xar_obtinere(st->goto_introitus, i);
        si (gi && gi->non_terminalis == non_terminalis)
        {
            redde gi->status_novus;
        }
    }

    redde -I;
}

vacuum
lapifex_tabulam_imprimere(
    LapifexTabula*  tabula)
{
    LapifexGrammatica* grammatica;
    i32 s;
    i32 num_status;

    si (!tabula) redde;

    grammatica = tabula->grammatica;
    num_status = (i32)xar_numerus(tabula->status_tabulae);

    imprimere("\n=== Tabula ACTION/GOTO ===\n");
    imprimere("Status: %d, Conflictus: %d\n\n",
        (int)num_status, (int)tabula->numerus_conflictuum);

    per (s = ZEPHYRUM; s < num_status; s++)
    {
        LapifexStatusTabula* st =
            (LapifexStatusTabula*)xar_obtinere(tabula->status_tabulae, s);
        i32 i;
        i32 num_actiones;
        i32 num_goto;

        si (!st) perge;

        num_actiones = (i32)xar_numerus(st->actiones);
        num_goto = (i32)xar_numerus(st->goto_introitus);

        imprimere("--- Status %d%s ---\n", (int)st->index,
            st->habet_conflictum ? " [CONFLICTUS]" : "");

        /* ACTION introitus */
        per (i = ZEPHYRUM; i < num_actiones; i++)
        {
            LapifexActioIntroitus* ai =
                (LapifexActioIntroitus*)xar_obtinere(st->actiones, i);
            si (!ai) perge;

            imprimere("  ACTION(");
            si (ai->terminalis == (s32)LAPIFEX_EOF_PROSPECTUS)
            {
                imprimere("$");
            }
            alioquin
            {
                LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                    grammatica->symbola, (i32)ai->terminalis);
                si (sym)
                {
                    imprimere("%.*s",
                        (int)sym->titulus->mensura,
                        (constans character*)sym->titulus->datum);
                }
            }

            imprimere(") = ");

            commutatio (ai->actio)
            {
                casus LAPIFEX_ACTIO_TRANSPONERE:
                    imprimere("SHIFT %d", (int)ai->valor);
                    frange;
                casus LAPIFEX_ACTIO_REDUCERE:
                {
                    LapifexProductio* prod = (LapifexProductio*)xar_obtinere(
                        grammatica->productiones, (i32)ai->valor);
                    si (prod)
                    {
                        LapifexSymbolum* lhs = (LapifexSymbolum*)xar_obtinere(
                            grammatica->symbola, (i32)prod->sinistrum);
                        si (lhs)
                        {
                            imprimere("REDUCE P%d (%.*s)",
                                (int)ai->valor,
                                (int)lhs->titulus->mensura,
                                (constans character*)lhs->titulus->datum);
                        }
                    }
                    frange;
                }
                casus LAPIFEX_ACTIO_ACCIPERE:
                    imprimere("ACCEPT");
                    frange;
                ordinarius:
                    imprimere("ERROR");
                    frange;
            }

            si (ai->conflictus_intentus)
            {
                imprimere(" [!]");
            }
            imprimere("\n");
        }

        /* GOTO introitus */
        per (i = ZEPHYRUM; i < num_goto; i++)
        {
            LapifexGotoIntroitus* gi =
                (LapifexGotoIntroitus*)xar_obtinere(st->goto_introitus, i);
            si (!gi) perge;

            {
                LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                    grammatica->symbola, (i32)gi->non_terminalis);
                si (sym)
                {
                    imprimere("  GOTO(%.*s) = %d\n",
                        (int)sym->titulus->mensura,
                        (constans character*)sym->titulus->datum,
                        (int)gi->status_novus);
                }
            }
        }

        imprimere("\n");
    }
}

vacuum
lapifex_conflictus_imprimere(
    LapifexTabula*  tabula)
{
    LapifexGrammatica* grammatica;
    i32 s;
    i32 num_status;

    si (!tabula) redde;

    grammatica = tabula->grammatica;
    num_status = (i32)xar_numerus(tabula->status_tabulae);

    si (tabula->numerus_conflictuum == ZEPHYRUM)
    {
        imprimere("\nNulli conflictus in tabula.\n");
        redde;
    }

    imprimere("\n=== Conflictus (%d) ===\n", (int)tabula->numerus_conflictuum);

    per (s = ZEPHYRUM; s < num_status; s++)
    {
        LapifexStatusTabula* st =
            (LapifexStatusTabula*)xar_obtinere(tabula->status_tabulae, s);
        i32 i;
        i32 num_actiones;

        si (!st || !st->habet_conflictum) perge;

        imprimere("  Status %d:\n", (int)st->index);

        num_actiones = (i32)xar_numerus(st->actiones);
        per (i = ZEPHYRUM; i < num_actiones; i++)
        {
            LapifexActioIntroitus* ai =
                (LapifexActioIntroitus*)xar_obtinere(st->actiones, i);
            si (!ai || !ai->conflictus_intentus) perge;

            imprimere("    ");
            si (ai->terminalis == (s32)LAPIFEX_EOF_PROSPECTUS)
            {
                imprimere("$");
            }
            alioquin
            {
                LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                    grammatica->symbola, (i32)ai->terminalis);
                si (sym)
                {
                    imprimere("%.*s",
                        (int)sym->titulus->mensura,
                        (constans character*)sym->titulus->datum);
                }
            }

            imprimere(": ");

            commutatio (ai->actio)
            {
                casus LAPIFEX_ACTIO_TRANSPONERE:
                    imprimere("SHIFT %d", (int)ai->valor);
                    frange;
                casus LAPIFEX_ACTIO_REDUCERE:
                    imprimere("REDUCE P%d", (int)ai->valor);
                    frange;
                casus LAPIFEX_ACTIO_ACCIPERE:
                    imprimere("ACCEPT");
                    frange;
                ordinarius:
                    imprimere("ERROR");
                    frange;
            }
            imprimere("\n");
        }
    }
}

/* ================================================
 * Imprimere Collectionem
 * ================================================ */

vacuum
lapifex_collectio_imprimere(
    LapifexCollectio*   collectio)
{
    LapifexGrammatica*  grammatica;
    i32 i;

    si (!collectio) redde;

    grammatica = collectio->grammatica;

    imprimere("\n=== Collectio Canonica LR(1) ===\n");
    imprimere("Status: %d, Transitiones: %d\n\n",
        (int)xar_numerus(collectio->status_omnes),
        (int)xar_numerus(collectio->transitiones));

    /* Imprimere unumquemque statum */
    per (i = ZEPHYRUM; i < (i32)xar_numerus(collectio->status_omnes); i++)
    {
        LapifexStatus* s = (LapifexStatus*)xar_obtinere(
            collectio->status_omnes, i);
        i32 j;

        si (!s) perge;

        imprimere("--- Status %d (%d res) ---\n",
            (int)s->index,
            (int)xar_numerus(s->res));

        per (j = ZEPHYRUM; j < (i32)xar_numerus(s->res); j++)
        {
            LapifexRes*       res = (LapifexRes*)xar_obtinere(s->res, j);
            LapifexProductio* prod;
            LapifexSymbolum*  sinistrum;
            i32               k;
            i32               num_dextrum;

            si (!res) perge;

            prod = (LapifexProductio*)xar_obtinere(
                grammatica->productiones, (i32)res->productio);
            si (!prod) perge;

            sinistrum = (LapifexSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)prod->sinistrum);
            si (!sinistrum) perge;

            num_dextrum = (i32)xar_numerus(prod->dextrum);

            imprimere("  [%.*s ->",
                (int)sinistrum->titulus->mensura,
                (constans character*)sinistrum->titulus->datum);

            per (k = ZEPHYRUM; k <= num_dextrum; k++)
            {
                si (k == (i32)res->punctum) imprimere(" .");

                si (k < num_dextrum)
                {
                    s32* sym_idx = (s32*)xar_obtinere(prod->dextrum, k);
                    si (sym_idx)
                    {
                        LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                            grammatica->symbola, (i32)*sym_idx);
                        si (sym)
                        {
                            imprimere(" %.*s",
                                (int)sym->titulus->mensura,
                                (constans character*)sym->titulus->datum);
                        }
                    }
                }
            }

            /* Prospectus */
            si (res->prospectus == (s32)LAPIFEX_EOF_PROSPECTUS)
            {
                imprimere(", $");
            }
            alioquin
            {
                LapifexSymbolum* look = (LapifexSymbolum*)xar_obtinere(
                    grammatica->symbola, (i32)res->prospectus);
                si (look)
                {
                    imprimere(", %.*s",
                        (int)look->titulus->mensura,
                        (constans character*)look->titulus->datum);
                }
            }
            imprimere("]\n");
        }

        /* Transitiones ex hoc statu */
        {
            i32 t;
            per (t = ZEPHYRUM; t < (i32)xar_numerus(collectio->transitiones); t++)
            {
                LapifexTransitio* trans = (LapifexTransitio*)xar_obtinere(
                    collectio->transitiones, t);
                si (trans && trans->status == s->index)
                {
                    LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                        grammatica->symbola, (i32)trans->symbolum);
                    si (sym)
                    {
                        imprimere("  -> %.*s -> Status %d\n",
                            (int)sym->titulus->mensura,
                            (constans character*)sym->titulus->datum,
                            (int)trans->status_novus);
                    }
                }
            }
        }
        imprimere("\n");
    }
}
