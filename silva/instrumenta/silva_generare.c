/* silva_generare.c - Implementatio Generatoris Grammaticae
 *
 * Legit grammaticam ex STML, computat FIRST coniuncta.
 */

#include "silva_generare.h"
#include "silva_nodus.h"      /* SilvaLocusSpecies - enum commune */
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
    SilvaGenGrammatica*   grammatica,
    chorda*              titulus,
    chorda*              genus,
    b32                  est_terminale)
{
    SilvaGenSymbolum* sym;
    i32 index;

    index = (i32)xar_numerus(grammatica->symbola);
    sym = (SilvaGenSymbolum*)xar_addere(grammatica->symbola);
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

/* Normare albispatia: copia textus in qua \n \r \t fiunt spatia.
 * Sine hoc, productiones multilineares atomos amittunt TACITE:
 * stml_textus_internus lineam novam servat, chorda_fissio solum
 * per ' ' dividit, ergo "gamma@c\nDELTA@d" UNUS atomus fit -
 * symbolum DELTA e grammatica evanescit et locus corrumpitur
 * (inventum 2026-07-04: 85 conflictus spurii in c89.stml). */
hic_manens chorda
_albispatia_normare(
    Piscina* piscina,
    chorda   textus)
{
    chorda copia;
    i32 i;

    copia.mensura = textus.mensura;
    copia.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(textus.mensura > ZEPHYRUM
            ? textus.mensura : (i32)I));
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        i8 c = textus.datum[i];
        copia.datum[i] = (c == '\n' || c == '\r' || c == '\t')
            ? (i8)' ' : c;
    }
    redde copia;
}

/* Verificare atomum sanum: titulus et locus solum characteres
 * identificatorum ferunt ([A-Za-z0-9_-]). Locus cum '@' vel
 * albispatio interno = textus corruptus - clama, numquam tace. */
hic_manens b32
_atomi_pars_sana(
    chorda pars)
{
    i32 i;

    per (i = ZEPHYRUM; i < pars.mensura; i++)
    {
        i8 c = pars.datum[i];
        b32 sanus = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9') || c == '_' || c == '-';
        si (!sanus) redde FALSUM;
    }
    redde VERUM;
}

/* Scindere atomum productionis "symbolum@locus+" in partes.
 * titulus_out: nomen symboli; locus_out: nomen loci (mensura 0 = nullus);
 * appendere_out: VERUM si @locus+ (lista accumulans). */
hic_manens vacuum
atomum_scindere(
    chorda  atomum,
    chorda* titulus_out,
    chorda* locus_out,
    b32*    appendere_out)
{
    i32 i;

    *titulus_out = atomum;
    locus_out->datum = NIHIL;
    locus_out->mensura = ZEPHYRUM;
    *appendere_out = FALSUM;

    per (i = ZEPHYRUM; i < atomum.mensura; i++)
    {
        si (atomum.datum[i] == '@')
        {
            titulus_out->mensura = i;
            locus_out->datum = atomum.datum + i + I;
            locus_out->mensura = atomum.mensura - i - I;
            si (locus_out->mensura > ZEPHYRUM
                && locus_out->datum[locus_out->mensura - I] == '+')
            {
                *appendere_out = VERUM;
                locus_out->mensura--;
            }
            frange;
        }
    }
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

SilvaGenGrammatica*
silva_gen_grammaticam_legere(
    Piscina*                 piscina,
    InternamentumChorda*     intern,
    constans character*      stml_fons)
{
    SilvaGenGrammatica*  grammatica;
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
        fprintf(stderr, "silva_gen: error parsandi STML: linea %d\n",
                (int)resultus.linea_erroris);
        redde NIHIL;
    }

    radix = resultus.elementum_radix;
    si (!radix) redde NIHIL;

    /* Creare grammaticam */
    grammatica = (SilvaGenGrammatica*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaGenGrammatica));
    grammatica->symbola = xar_creare(piscina, (i32)magnitudo(SilvaGenSymbolum));
    grammatica->productiones = xar_creare(piscina, (i32)magnitudo(SilvaGenProductio));
    grammatica->genera_extra = xar_creare(piscina,
        (i32)magnitudo(SilvaGenGenusExtra));
    grammatica->praelationes = xar_creare(piscina,
        (i32)magnitudo(SilvaGenPraelatio));
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
        fprintf(stderr, "silva_gen: <terminalia> non inventum\n");
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
                fprintf(stderr, "silva_gen: <terminalis> sine titulo\n");
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
                    SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
     * Pars I.b: Legere praelationes (optionale) - resolutio
     * DECLARATA cellae conflictus in tabulis (M2c; genera-c89.md
     * decisiones 12). Sola actio nota: transponere. Terminalis
     * ignotus aut actio ignota = error clamans.
     * ---------------------------------------- */
    {
        StmlNodus* praelationes_nodus =
            stml_invenire_liberum(radix, "praelationes");

        si (praelationes_nodus != NIHIL)
        {
            Xar* praelationes_nodi = stml_invenire_omnes_liberos(
                praelationes_nodus, "praelatio", piscina);

            si (praelationes_nodi)
            {
                per (i = ZEPHYRUM;
                     i < (i32)xar_numerus(praelationes_nodi); i++)
                {
                    StmlNodus** nodus_ptr = (StmlNodus**)xar_obtinere(
                        praelationes_nodi, (i32)i);
                    chorda*            term;
                    chorda*            actio;
                    s32                idx;
                    SilvaGenPraelatio* nova;

                    si (!nodus_ptr || !*nodus_ptr) perge;

                    term = stml_attributum_capere(*nodus_ptr, "terminalis");
                    actio = stml_attributum_capere(*nodus_ptr, "actio");

                    si (!term || !actio)
                    {
                        fprintf(stderr, "silva_gen: <praelatio> sine "
                            "terminali aut actione\n");
                        redde NIHIL;
                    }
                    idx = symbolum_invenire(tabula_symbolorum, term);
                    si (idx < ZEPHYRUM)
                    {
                        fprintf(stderr, "silva_gen: praelatio: terminalis "
                            "'%.*s' ignotus\n",
                            (int)term->mensura,
                            (constans character*)term->datum);
                        redde NIHIL;
                    }
                    si (!chorda_aequalis_literis(*actio, "transponere"))
                    {
                        fprintf(stderr, "silva_gen: praelatio: actio "
                            "'%.*s' ignota (sola: transponere)\n",
                            (int)actio->mensura,
                            (constans character*)actio->datum);
                        redde NIHIL;
                    }

                    nova = (SilvaGenPraelatio*)xar_addere(
                        grammatica->praelationes);
                    nova->terminalis = idx;
                    nova->actio = (s32)SILVA_GEN_ACTIO_TRANSPONERE;
                }
            }
        }
    }

    /* ----------------------------------------
     * Pars II: Legere regulae (non-terminalia)
     * Primo: creare symbola non-terminalia
     * ---------------------------------------- */
    regulae_nodus = stml_invenire_liberum(radix, "regulae");
    si (!regulae_nodus)
    {
        fprintf(stderr, "silva_gen: <regulae> non inventum\n");
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
                fprintf(stderr, "silva_gen: <regula> sine titulo\n");
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
                fprintf(stderr, "silva_gen: symbolum '%.*s' non inventum\n",
                    (int)tit->mensura, (constans character*)tit->datum);
                redde NIHIL;
            }

            /* Validatio: LHS debet esse non-terminale (lapifex terminalem
             * tacite accipiebat) */
            {
                SilvaGenSymbolum* sin_sym = (SilvaGenSymbolum*)xar_obtinere(
                    grammatica->symbola, (i32)sinistrum_idx);
                si (sin_sym && sin_sym->est_terminale)
                {
                    fprintf(stderr,
                        "silva_gen: regula '%.*s' terminalem ut LHS habet\n",
                        (int)tit->mensura, (constans character*)tit->datum);
                    redde NIHIL;
                }
            }

            /* Iterare per <productio> liberos */
            prod_nodi = stml_invenire_omnes_liberos(*nodus_ptr, "productio", piscina);
            si (!prod_nodi) perge;

            per (j = ZEPHYRUM; j < (i32)xar_numerus(prod_nodi); j++)
            {
                StmlNodus**       prod_ptr = (StmlNodus**)xar_obtinere(prod_nodi, (i32)j);
                chorda            textus;
                chorda_fissio_fructus partes;
                SilvaGenProductio* prod;
                i32               k;

                si (!prod_ptr || !*prod_ptr) perge;

                /* Obtinere textum internum: "expr@sinister PLUS@tok term@dexter"
                 * (albispatia normata - productiones multilineares licitae) */
                textus = stml_textus_internus(*prod_ptr, piscina);
                textus = _albispatia_normare(piscina, textus);
                textus = chorda_praecidere(textus);

                /* Creare productionem (textus vacuus = EPSILON deliberata -
                 * silva additum; lapifex eam praeteribat) */
                prod = (SilvaGenProductio*)xar_addere(grammatica->productiones);
                si (!prod) redde NIHIL;

                prod->sinistrum = sinistrum_idx;
                prod->dextrum = xar_creare(piscina, (i32)magnitudo(s32));
                prod->index = (s32)xar_numerus(grammatica->productiones) - I;
                prod->genus = stml_attributum_capere(*prod_ptr, "genus");
                prod->id = stml_attributum_capere(*prod_ptr, "id");
                prod->modus = stml_attributum_capere(*prod_ptr, "modus");
                prod->manu = stml_attributum_capere(*prod_ptr, "manu");
                prod->loci = xar_creare(piscina,
                    (i32)magnitudo(SilvaGenLocusMappa));

                si (textus.mensura == ZEPHYRUM) perge;  /* epsilon */

                /* Dividere per spatia */
                partes = chorda_fissio(textus, ' ', piscina);

                /* Resolvere omnia symbola in dextro (cum @locis) */
                per (k = ZEPHYRUM; k < partes.numerus; k++)
                {
                    chorda  pars = partes.elementa[k];
                    chorda  pars_praecisa = chorda_praecidere(pars);
                    chorda  sym_titulus;
                    chorda  locus_titulus;
                    b32     locus_appendere;
                    s32     sym_idx;
                    s32*    novum;
                    SilvaGenLocusMappa* mappa;

                    si (pars_praecisa.mensura == ZEPHYRUM) perge;

                    atomum_scindere(pars_praecisa, &sym_titulus,
                        &locus_titulus, &locus_appendere);

                    si (!_atomi_pars_sana(sym_titulus)
                        || !_atomi_pars_sana(locus_titulus))
                    {
                        fprintf(stderr,
                            "silva_gen: atomum corruptum '%.*s' in "
                            "productione (character invalidus in "
                            "symbolo vel loco)\n",
                            (int)pars_praecisa.mensura,
                            (constans character*)pars_praecisa.datum);
                        redde NIHIL;
                    }

                    /* Invenire symbolum in tabula */
                    sym_idx = symbolum_invenire(
                        tabula_symbolorum,
                        chorda_internare(intern, sym_titulus));

                    si (sym_idx < ZEPHYRUM)
                    {
                        fprintf(stderr,
                            "silva_gen: symbolum '%.*s' non inventum in productione\n",
                            (int)sym_titulus.mensura,
                            (constans character*)sym_titulus.datum);
                        redde NIHIL;
                    }

                    novum = (s32*)xar_addere(prod->dextrum);
                    si (novum) *novum = sym_idx;

                    mappa = (SilvaGenLocusMappa*)xar_addere(prod->loci);
                    si (mappa)
                    {
                        mappa->titulus = (locus_titulus.mensura > ZEPHYRUM)
                            ? chorda_internare(intern, locus_titulus)
                            : NIHIL;
                        mappa->appendere = locus_appendere;
                    }
                }
            }
        }
    }

    /* ----------------------------------------
     * Pars II.5: Genera extra (structuralia) - unum registrum (S30)
     * ---------------------------------------- */
    {
        StmlNodus* extra_nodus;

        extra_nodus = stml_invenire_liberum(radix, "genera-extra");
        si (extra_nodus)
        {
            Xar* genera_nodi;

            genera_nodi = stml_invenire_omnes_liberos(extra_nodus, "genus",
                piscina);
            si (genera_nodi)
            {
                per (i = ZEPHYRUM; i < (i32)xar_numerus(genera_nodi); i++)
                {
                    StmlNodus** g_ptr = (StmlNodus**)xar_obtinere(
                        genera_nodi, (i32)i);
                    SilvaGenGenusExtra* extra;
                    chorda* g_tit;

                    si (!g_ptr || !*g_ptr) perge;

                    g_tit = stml_attributum_capere(*g_ptr, "titulus");
                    si (!g_tit)
                    {
                        fprintf(stderr,
                            "silva_gen: <genus> extra sine titulo\n");
                        redde NIHIL;
                    }

                    extra = (SilvaGenGenusExtra*)xar_addere(
                        grammatica->genera_extra);
                    si (extra)
                    {
                        extra->titulus = g_tit;
                        extra->loci_descriptio =
                            stml_attributum_capere(*g_ptr, "slots");
                    }
                }
            }
        }
    }

    /* ----------------------------------------
     * Pars II.6: Validatio annotationum (S19/S31 - generator est
     * schema, R4). manu = effugium: validatio locorum praetermittitur.
     * ---------------------------------------- */
    {
        TabulaDispersa* tabula_idorum;
        i32 p;
        i32 num_prod_val;

        tabula_idorum = tabula_dispersa_creare_chorda(piscina, XXXII);
        num_prod_val = (i32)xar_numerus(grammatica->productiones);

        per (p = ZEPHYRUM; p < num_prod_val; p++)
        {
            SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
                grammatica->productiones, p);
            i32 num_dex;
            i32 k;

            si (!prod) perge;

            num_dex = (i32)xar_numerus(prod->dextrum);

            /* id: mandatum cum genere, unicum */
            si (prod->genus != NIHIL && prod->id == NIHIL)
            {
                fprintf(stderr,
                    "silva_gen: productio P%d cum genere sine id\n",
                    (int)prod->index);
                redde NIHIL;
            }
            si (prod->id != NIHIL)
            {
                si (tabula_dispersa_continet(tabula_idorum, *prod->id))
                {
                    fprintf(stderr,
                        "silva_gen: id '%.*s' duplicatum\n",
                        (int)prod->id->mensura,
                        (constans character*)prod->id->datum);
                    redde NIHIL;
                }
                tabula_dispersa_inserere(tabula_idorum, *prod->id,
                    (vacuum*)(s64)prod->index);
            }

            si (prod->manu != NIHIL) perge;  /* effugium manuale */

            /* S19: productio multi-symbola sine genere = error.
             * Productiones modus (listae) exemptae - valores listarum
             * gignunt, non nodos; terminalia earum tamen possidenda. */
            si (num_dex > I && prod->genus == NIHIL && prod->modus == NIHIL)
            {
                fprintf(stderr,
                    "silva_gen: productio P%d (>1 symbola) sine genere\n",
                    (int)prod->index);
                redde NIHIL;
            }

            /* S19: in productione cum genere VEL modo, OMNE symbolum
             * locum habeat - terminalia pro invariato triviorum
             * unius-domini, non-terminalia pro TOTALITATE emissionis
             * (Phase 5: symbolum non mappatum = octeti subarboris
             * silenter perditi in scriptura). In listis separatores
             * mappati in listam interponuntur. */
            si (prod->genus != NIHIL || prod->modus != NIHIL)
            {
                per (k = ZEPHYRUM; k < num_dex; k++)
                {
                    s32* sym_idx = (s32*)xar_obtinere(prod->dextrum, k);
                    SilvaGenSymbolum* sym;
                    SilvaGenLocusMappa* mappa;

                    si (!sym_idx) perge;
                    sym = (SilvaGenSymbolum*)xar_obtinere(
                        grammatica->symbola, (i32)*sym_idx);
                    si (!sym) perge;

                    mappa = (SilvaGenLocusMappa*)xar_obtinere(prod->loci, k);
                    si (!mappa || mappa->titulus == NIHIL)
                    {
                        fprintf(stderr,
                            "silva_gen: productio P%d%s%.*s%s: %s '%.*s' "
                            "sine @loco (%s)\n",
                            (int)prod->index,
                            prod->id != NIHIL ? " '" : "",
                            prod->id != NIHIL ? (int)prod->id->mensura : 0,
                            prod->id != NIHIL
                                ? (constans character*)prod->id->datum : "",
                            prod->id != NIHIL ? "'" : "",
                            sym->est_terminale
                                ? "terminale" : "symbolum",
                            (int)sym->titulus->mensura,
                            (constans character*)sym->titulus->datum,
                            sym->est_terminale
                                ? "dominus triviorum deest"
                                : "totalitas emissionis frangeretur");
                        redde NIHIL;
                    }
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
            fprintf(stderr, "silva_gen: initium symbolum '%.*s' non inventum\n",
                (int)praecisa.mensura, (constans character*)praecisa.datum);
            redde NIHIL;
        }

        grammatica->initium_index = initium_idx;
    }
    alioquin
    {
        fprintf(stderr, "silva_gen: <initium> non inventum\n");
        redde NIHIL;
    }

    /* ----------------------------------------
     * Pars IV: Productio augmentata S' -> initium EOF
     * ---------------------------------------- */
    {
        chorda*          tit_augmentatum;
        s32              aug_idx;
        s32              eof_idx;
        SilvaGenProductio* prod;
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
        prod = (SilvaGenProductio*)xar_addere(grammatica->productiones);
        si (!prod) redde NIHIL;

        prod->sinistrum = aug_idx;
        prod->dextrum = xar_creare(piscina, (i32)magnitudo(s32));
        prod->index = (s32)xar_numerus(grammatica->productiones) - I;
        prod->genus = NIHIL;
        prod->id = NIHIL;
        prod->modus = NIHIL;
        prod->manu = NIHIL;
        prod->loci = xar_creare(piscina, (i32)magnitudo(SilvaGenLocusMappa));

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
 * Registrum generum (S20)
 * ================================================ */

/* Estne non-terminale lista-valens? (aliqua productio eius modus fert) */
hic_manens b32
_est_lista_valens_ad(SilvaGenGrammatica* g, s32 sym_idx, i32 profunditas)
{
    i32 i;

    si (profunditas > XXXII) redde FALSUM;  /* custodia cyclorum */

    per (i = ZEPHYRUM; i < (i32)xar_numerus(g->productiones); i++)
    {
        SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
            g->productiones, i);

        si (prod == NIHIL || prod->sinistrum != sym_idx) perge;
        si (prod->modus != NIHIL)
        {
            redde VERUM;
        }
        /* TRANSITIVUM (M2b): transitus unius symboli valorem
         * intactum fert - si symbolum eius lista-valens est,
         * hoc quoque est (specificatores-decl -> sd-*: lista
         * per duos transitus fluit; sine hoc species NODUS
         * falso computabatur -> S32 in tempore currendi) */
        si (prod->genus == NIHIL && prod->manu == NIHIL
            && (i32)xar_numerus(prod->dextrum) == I)
        {
            s32* sym = (s32*)xar_obtinere(prod->dextrum, ZEPHYRUM);
            SilvaGenSymbolum* s = (sym != NIHIL)
                ? (SilvaGenSymbolum*)xar_obtinere(g->symbola,
                      (i32)*sym)
                : NIHIL;

            si (s != NIHIL && !s->est_terminale
                && _est_lista_valens_ad(g, *sym, profunditas + I))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

hic_manens b32
_est_lista_valens(SilvaGenGrammatica* g, s32 sym_idx)
{
    redde _est_lista_valens_ad(g, sym_idx, ZEPHYRUM);
}

hic_manens b32
_chordae_pares(chorda* a, chorda* b)
{
    si (a == NIHIL || b == NIHIL) redde FALSUM;
    si (a->mensura != b->mensura) redde FALSUM;
    si (a->mensura == ZEPHYRUM) redde VERUM;
    redde (memcmp(a->datum, b->datum, (memoriae_index)a->mensura)
        == ZEPHYRUM) ? VERUM : FALSUM;
}

/* Species ex chorda ("nodus", "lista-token", ...) vel -1 */
hic_manens s32
_species_ex_chorda(chorda c)
{
    hic_manens constans character* NOMINA[] = {
        "nodus", "token", "lista-nodus", "lista-token",
        "lista-mixta", "index"
    };
    i32 i;

    per (i = ZEPHYRUM; i < VI; i++)
    {
        si (c.mensura == (i32)strlen(NOMINA[i])
            && memcmp(c.datum, NOMINA[i],
                   (memoriae_index)c.mensura) == ZEPHYRUM)
        {
            redde (s32)i;  /* ordo == SilvaLocusSpecies */
        }
    }
    redde -I;
}

/* Invenire vel creare genus in registro */
hic_manens SilvaGenGenusDef*
_genus_capere(SilvaGenGrammatica* g, Xar* genera, chorda* titulus)
{
    i32 i;
    SilvaGenGenusDef* def;

    per (i = ZEPHYRUM; i < (i32)xar_numerus(genera); i++)
    {
        def = (SilvaGenGenusDef*)xar_obtinere(genera, i);
        si (def != NIHIL && _chordae_pares(def->titulus, titulus))
        {
            redde def;
        }
    }
    def = (SilvaGenGenusDef*)xar_addere(genera);
    si (def == NIHIL) redde NIHIL;
    def->titulus = titulus;
    def->loci = xar_creare(g->piscina, (i32)magnitudo(SilvaGenLocusDef));
    def->ex_extra = FALSUM;
    redde def;
}

/* Unire locum in genus: species congruere debet (S20) */
hic_manens b32
_locum_unire(SilvaGenGenusDef* def, chorda* titulus, s32 species)
{
    i32 i;
    SilvaGenLocusDef* locus;

    per (i = ZEPHYRUM; i < (i32)xar_numerus(def->loci); i++)
    {
        locus = (SilvaGenLocusDef*)xar_obtinere(def->loci, i);
        si (locus != NIHIL && _chordae_pares(locus->titulus, titulus))
        {
            si (locus->species != species)
            {
                fprintf(stderr,
                    "silva_gen: conflictus specierum loci '%.*s' in genere "
                    "'%.*s' (S20)\n",
                    (int)titulus->mensura,
                    (constans character*)titulus->datum,
                    (int)def->titulus->mensura,
                    (constans character*)def->titulus->datum);
                redde FALSUM;
            }
            redde VERUM;
        }
    }
    locus = (SilvaGenLocusDef*)xar_addere(def->loci);
    si (locus == NIHIL) redde FALSUM;
    locus->titulus = titulus;
    locus->species = species;
    redde VERUM;
}

Xar*
silva_gen_registrum_computare(
    SilvaGenGrammatica*  grammatica)
{
    Xar* genera;
    i32 p;
    i32 i;

    si (!grammatica) redde NIHIL;

    genera = xar_creare(grammatica->piscina,
        (i32)magnitudo(SilvaGenGenusDef));

    /* Ex productionibus cum genere */
    per (p = ZEPHYRUM; p < (i32)xar_numerus(grammatica->productiones); p++)
    {
        SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, p);
        SilvaGenGenusDef* def;
        i32 k;

        si (prod == NIHIL || prod->genus == NIHIL) perge;

        def = _genus_capere(grammatica, genera, prod->genus);
        si (def == NIHIL) redde NIHIL;

        per (k = ZEPHYRUM; k < (i32)xar_numerus(prod->dextrum); k++)
        {
            SilvaGenLocusMappa* mappa = (SilvaGenLocusMappa*)xar_obtinere(
                prod->loci, k);
            s32* sym_idx = (s32*)xar_obtinere(prod->dextrum, k);
            SilvaGenSymbolum* sym;
            s32 species;

            si (mappa == NIHIL || mappa->titulus == NIHIL
                || sym_idx == NIHIL)
            {
                perge;
            }
            sym = (SilvaGenSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)*sym_idx);
            si (sym == NIHIL) perge;

            si (mappa->appendere)
            {
                species = sym->est_terminale
                    ? (s32)SILVA_LOCUS_LISTA_TOKEN
                    : (s32)SILVA_LOCUS_LISTA_MIXTA;
            }
            alioquin si (sym->est_terminale)
            {
                species = (s32)SILVA_LOCUS_TOKEN;
            }
            alioquin si (_est_lista_valens(grammatica, *sym_idx))
            {
                species = (s32)SILVA_LOCUS_LISTA_MIXTA;
            }
            alioquin
            {
                species = (s32)SILVA_LOCUS_NODUS;
            }

            si (!_locum_unire(def, mappa->titulus, species))
            {
                redde NIHIL;  /* conflictus S20 */
            }
        }
    }

    /* Ex genera-extra (collisio cum grammatica = error) */
    per (i = ZEPHYRUM; i < (i32)xar_numerus(grammatica->genera_extra); i++)
    {
        SilvaGenGenusExtra* extra = (SilvaGenGenusExtra*)xar_obtinere(
            grammatica->genera_extra, i);
        SilvaGenGenusDef* def;
        i32 j;

        si (extra == NIHIL) perge;

        /* collisio? */
        per (j = ZEPHYRUM; j < (i32)xar_numerus(genera); j++)
        {
            SilvaGenGenusDef* d = (SilvaGenGenusDef*)xar_obtinere(genera, j);

            si (d != NIHIL && _chordae_pares(d->titulus, extra->titulus))
            {
                fprintf(stderr,
                    "silva_gen: genus '%.*s' in grammatica ET genera-extra\n",
                    (int)extra->titulus->mensura,
                    (constans character*)extra->titulus->datum);
                redde NIHIL;
            }
        }

        def = _genus_capere(grammatica, genera, extra->titulus);
        si (def == NIHIL) redde NIHIL;
        def->ex_extra = VERUM;

        /* Parsare loci_descriptio: "nomen:species nomen:species ..." */
        si (extra->loci_descriptio != NIHIL)
        {
            chorda_fissio_fructus partes;
            i32 k;

            partes = chorda_fissio(*extra->loci_descriptio, ' ',
                grammatica->piscina);
            per (k = ZEPHYRUM; k < partes.numerus; k++)
            {
                chorda pars = chorda_praecidere(partes.elementa[k]);
                chorda titulus_loci;
                chorda species_chorda;
                s32 species;
                i32 m;
                s32 colon;

                si (pars.mensura == ZEPHYRUM) perge;

                colon = -I;
                per (m = ZEPHYRUM; m < pars.mensura; m++)
                {
                    si (pars.datum[m] == ':')
                    {
                        colon = (s32)m;
                        frange;
                    }
                }
                si (colon < ZEPHYRUM)
                {
                    fprintf(stderr,
                        "silva_gen: locus extra sine specie: '%.*s'\n",
                        (int)pars.mensura,
                        (constans character*)pars.datum);
                    redde NIHIL;
                }

                titulus_loci.datum = pars.datum;
                titulus_loci.mensura = (i32)colon;
                species_chorda.datum = pars.datum + colon + I;
                species_chorda.mensura = pars.mensura - (i32)colon - I;

                species = _species_ex_chorda(species_chorda);
                si (species < ZEPHYRUM)
                {
                    fprintf(stderr,
                        "silva_gen: species ignota: '%.*s'\n",
                        (int)species_chorda.mensura,
                        (constans character*)species_chorda.datum);
                    redde NIHIL;
                }

                si (!_locum_unire(def,
                        chorda_internare(grammatica->intern, titulus_loci),
                        species))
                {
                    redde NIHIL;
                }
            }
        }
    }

    /* Genera structuralia REQUISITA (spec-v2 par 12.2): motor GLR
     * fabricam ambigui praesumit; recuperatio ERROR, regiones
     * CONDITIONALIS. Absentia = error generationis - modus degradatus
     * sine compactione NON existit (quirk-limatus per constructionem
     * vetitus). */
    {
        hic_manens constans character* NECESSARIA[V] = {
            "ambiguus", "error", "conditionalis",
            "ramus-sumptus", "ramus-omissus"
        };
        SilvaGenGenusDef* defs[V];
        i32 n;

        per (n = ZEPHYRUM; n < V; n++)
        {
            SilvaGenGenusDef* inventum = NIHIL;
            i32 j;

            per (j = ZEPHYRUM; j < (i32)xar_numerus(genera); j++)
            {
                SilvaGenGenusDef* d =
                    (SilvaGenGenusDef*)xar_obtinere(genera, j);

                si (d != NIHIL && d->ex_extra && d->titulus != NIHIL
                    && chorda_aequalis_literis(*d->titulus, NECESSARIA[n]))
                {
                    inventum = d;
                    frange;
                }
            }
            si (inventum == NIHIL)
            {
                fprintf(stderr, "silva_gen: genus structurale '%s' in "
                    "genera-extra REQUIRITUR (spec-v2 par 12.2)\n",
                    NECESSARIA[n]);
                redde NIHIL;
            }
            defs[n] = inventum;
        }

        /* Formae generum structuralium (fabrica ambigui, Chunk C
         * recanonicalizatio, textura conditionalium Phase 7 - omnes
         * his nominibus/speciebus pendent). Species: II lista-nodus,
         * III lista-token, IV lista-mixta, V index. */
        {
            nomen structura {
                i32                 def_index;
                constans character* titulus;
                i32                 species_min;
                i32                 species_max;
            } FormaExigenda;
            hic_manens constans FormaExigenda FORMAE[X] = {
                { ZEPHYRUM, "interpretationes", II,  IV  },
                { ZEPHYRUM, "canonica",         V,   V   },
                { II,       "rami",             II,  II  },
                { II,       "finis",            III, III },
                { III,      "directiva",        III, III },
                { III,      "contentum",        II,  II  },
                { III,      "conditio_id",      V,   V   },
                { IV,       "directiva",        III, III },
                { IV,       "cruda",            III, III },
                { IV,       "conditio_id",      V,   V   }
            };
            i32 f;

            per (f = ZEPHYRUM; f < X; f++)
            {
                constans FormaExigenda* forma = &FORMAE[f];
                SilvaGenGenusDef* def = defs[forma->def_index];
                b32 inventum = FALSUM;
                i32 k;

                per (k = ZEPHYRUM; k < (i32)xar_numerus(def->loci); k++)
                {
                    SilvaGenLocusDef* locus =
                        (SilvaGenLocusDef*)xar_obtinere(def->loci, k);

                    si (locus == NIHIL || locus->titulus == NIHIL) perge;
                    si (chorda_aequalis_literis(*locus->titulus,
                            forma->titulus)
                        && (i32)locus->species >= forma->species_min
                        && (i32)locus->species <= forma->species_max)
                    {
                        inventum = VERUM;
                        frange;
                    }
                }
                si (!inventum)
                {
                    fprintf(stderr, "silva_gen: genus structurale '%s' "
                        "locum '%s' (species %d..%d) requirit\n",
                        NECESSARIA[forma->def_index], forma->titulus,
                        (int)forma->species_min,
                        (int)forma->species_max);
                    redde NIHIL;
                }
            }
        }
    }

    /* Validatio ordinis locorum (Phase 5, scribere): emissio generica
     * locos ordine layout ambulat, ergo QUAEQUE productio generis
     * locos suos ordine layout mappare debet - series indicum in
     * ordine dextri monotone non-decrescens (repetitio licita:
     * @locus+ accumulans). Ordo divergens = error generationis.
     * Via reversa nominata (INTENTIO Phase 5): index productionis
     * in nodis, si grammatica vera ordines divergentes petat. */
    per (p = ZEPHYRUM; p < (i32)xar_numerus(grammatica->productiones); p++)
    {
        SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, p);
        SilvaGenGenusDef* def = NIHIL;
        i32 j;
        i32 k;
        s32 prior;

        si (prod == NIHIL || prod->genus == NIHIL) perge;

        per (j = ZEPHYRUM; j < (i32)xar_numerus(genera); j++)
        {
            SilvaGenGenusDef* d = (SilvaGenGenusDef*)xar_obtinere(genera, j);

            si (d != NIHIL && _chordae_pares(d->titulus, prod->genus))
            {
                def = d;
                frange;
            }
        }
        si (def == NIHIL) perge;  /* non accidit - unitum supra */

        prior = -I;
        per (k = ZEPHYRUM; k < (i32)xar_numerus(prod->dextrum); k++)
        {
            SilvaGenLocusMappa* mappa = (SilvaGenLocusMappa*)xar_obtinere(
                prod->loci, k);
            s32 index_loci = -I;
            i32 m;

            si (mappa == NIHIL || mappa->titulus == NIHIL) perge;

            per (m = ZEPHYRUM; m < (i32)xar_numerus(def->loci); m++)
            {
                SilvaGenLocusDef* locus = (SilvaGenLocusDef*)xar_obtinere(
                    def->loci, m);

                si (locus != NIHIL
                    && _chordae_pares(locus->titulus, mappa->titulus))
                {
                    index_loci = (s32)m;
                    frange;
                }
            }
            si (index_loci < ZEPHYRUM) perge;  /* non accidit */

            si (index_loci < prior)
            {
                fprintf(stderr,
                    "silva_gen: productio '%.*s': locus '%.*s' ordinem "
                    "layout generis '%.*s' violat (emissio generica "
                    "ordinem dextri sequitur - loci omnium productionum "
                    "eiusdem generis ordine consentire debent)\n",
                    prod->id != NIHIL ? (int)prod->id->mensura : 0,
                    prod->id != NIHIL
                        ? (constans character*)prod->id->datum : "",
                    (int)mappa->titulus->mensura,
                    (constans character*)mappa->titulus->datum,
                    (int)prod->genus->mensura,
                    (constans character*)prod->genus->datum);
                redde NIHIL;
            }
            prior = index_loci;
        }
    }

    redde genera;
}

/* ================================================
 * FIRST Computatio
 * ================================================ */

b32
silva_gen_first_computare(
    SilvaGenGrammatica*  grammatica)
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
        SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
            SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
                grammatica->productiones, (i32)i);
            SilvaGenSymbolum*  sinistrum;
            i32               numerus_dextrum;
            i32               j;
            b32               omnia_habent_epsilon;

            si (!prod) perge;

            sinistrum = (SilvaGenSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)prod->sinistrum);
            si (!sinistrum) perge;

            numerus_dextrum = (i32)xar_numerus(prod->dextrum);
            omnia_habent_epsilon = VERUM;

            /* Pro unaquaque A -> X1 X2 ... Xn */
            per (j = ZEPHYRUM; j < numerus_dextrum; j++)
            {
                s32*             idx_ptr = (s32*)xar_obtinere(prod->dextrum, (i32)j);
                SilvaGenSymbolum* xj;
                i32              k;
                i32              numerus_first;

                si (!idx_ptr) frange;

                xj = (SilvaGenSymbolum*)xar_obtinere(
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
silva_gen_first_obtinere(
    SilvaGenGrammatica*  grammatica,
    i32                 symbolum_index)
{
    SilvaGenSymbolum* sym;

    si (!grammatica) redde NIHIL;

    sym = (SilvaGenSymbolum*)xar_obtinere(grammatica->symbola, symbolum_index);
    si (!sym) redde NIHIL;

    redde sym->first;
}

/* ================================================
 * Imprimere
 * ================================================ */

vacuum
silva_gen_grammaticam_imprimere(
    SilvaGenGrammatica*  grammatica)
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
        SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
        SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, (i32)i);
        si (prod)
        {
            SilvaGenSymbolum* sinistrum = (SilvaGenSymbolum*)xar_obtinere(
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
                    SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
silva_gen_first_imprimere(
    SilvaGenGrammatica*  grammatica)
{
    i32 i;

    si (!grammatica) redde;

    imprimere("\n=== FIRST Coniuncta ===\n");
    per (i = ZEPHYRUM; i < (i32)xar_numerus(grammatica->symbola); i++)
    {
        SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
                    SilvaGenSymbolum* term = (SilvaGenSymbolum*)xar_obtinere(
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
#define SILVA_GEN_EOF_PROSPECTUS (-1)

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
    SilvaGenGrammatica* grammatica,
    s32* symbola_indices, i32 numerus,
    s32 prospectus,
    Xar* fructus)
{
    i32 i;
    b32 omnia_habent_epsilon;

    omnia_habent_epsilon = VERUM;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
silva_gen_follow_computare(
    SilvaGenGrammatica*  grammatica)
{
    i32  numerus_productionum;
    b32  mutatum;
    i32  i;

    si (!grammatica) redde FALSUM;

    numerus_productionum = (i32)xar_numerus(grammatica->productiones);

    /* Addere EOF ($) ad FOLLOW(symbolum initiale) */
    {
        SilvaGenSymbolum* initium = (SilvaGenSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)grammatica->initium_index);
        si (initium)
        {
            follow_addere(initium->follow, (s32)SILVA_GEN_EOF_PROSPECTUS);
        }
    }

    /* Iterare usque dum nihil mutatur */
    fac
    {
        mutatum = FALSUM;

        per (i = ZEPHYRUM; i < numerus_productionum; i++)
        {
            SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
                grammatica->productiones, i);
            SilvaGenSymbolum*  sinistrum;
            i32               num_dextrum;
            i32               j;

            si (!prod) perge;

            sinistrum = (SilvaGenSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)prod->sinistrum);
            si (!sinistrum) perge;

            num_dextrum = (i32)xar_numerus(prod->dextrum);

            /* Pro unoquoque symbolo B in dextro: A -> alpha B beta */
            per (j = ZEPHYRUM; j < num_dextrum; j++)
            {
                s32* b_idx_ptr = (s32*)xar_obtinere(prod->dextrum, j);
                SilvaGenSymbolum* b_sym;
                i32 k;
                b32 beta_habet_epsilon;

                si (!b_idx_ptr) perge;

                b_sym = (SilvaGenSymbolum*)xar_obtinere(
                    grammatica->symbola, (i32)*b_idx_ptr);
                si (!b_sym) perge;

                /* Solum non-terminalia habent FOLLOW significantem */
                si (b_sym->est_terminale) perge;

                /* Addere FIRST(beta) - {epsilon} ad FOLLOW(B) */
                beta_habet_epsilon = VERUM;
                per (k = j + I; k < num_dextrum; k++)
                {
                    s32* beta_idx_ptr = (s32*)xar_obtinere(prod->dextrum, k);
                    SilvaGenSymbolum* beta_sym;
                    i32 m;
                    i32 num_first;

                    si (!beta_idx_ptr) frange;

                    beta_sym = (SilvaGenSymbolum*)xar_obtinere(
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
silva_gen_follow_obtinere(
    SilvaGenGrammatica*  grammatica,
    i32                 symbolum_index)
{
    SilvaGenSymbolum* sym;

    si (!grammatica) redde NIHIL;

    sym = (SilvaGenSymbolum*)xar_obtinere(grammatica->symbola, symbolum_index);
    si (!sym) redde NIHIL;

    redde sym->follow;
}

vacuum
silva_gen_follow_imprimere(
    SilvaGenGrammatica*  grammatica)
{
    i32 i;

    si (!grammatica) redde;

    imprimere("\n=== FOLLOW Coniuncta ===\n");
    per (i = ZEPHYRUM; i < (i32)xar_numerus(grammatica->symbola); i++)
    {
        SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
                    si (*idx == (s32)SILVA_GEN_EOF_PROSPECTUS)
                    {
                        imprimere("$");
                    }
                    alioquin
                    {
                        SilvaGenSymbolum* term = (SilvaGenSymbolum*)xar_obtinere(
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
    SilvaGenRes*  res)
{
    i32 i;
    i32 numerus = (i32)xar_numerus(res_xar);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenRes* existens = (SilvaGenRes*)xar_obtinere(res_xar, i);
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
    SilvaGenGrammatica*  grammatica,
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
            SilvaGenRes*       res_currens;
            SilvaGenProductio* prod;
            i32               num_dextrum;
            s32*              b_idx_ptr;
            SilvaGenSymbolum*  b_sym;
            i32               j;

            res_currens = (SilvaGenRes*)xar_obtinere(res_xar, i);
            si (!res_currens) perge;

            prod = (SilvaGenProductio*)xar_obtinere(
                grammatica->productiones, (i32)res_currens->productio);
            si (!prod) perge;

            num_dextrum = (i32)xar_numerus(prod->dextrum);

            /* Si punctum est ad finem, nihil expandere */
            si ((i32)res_currens->punctum >= num_dextrum) perge;

            /* Obtinere symbolum post punctum: B */
            b_idx_ptr = (s32*)xar_obtinere(prod->dextrum,
                (i32)res_currens->punctum);
            si (!b_idx_ptr) perge;

            b_sym = (SilvaGenSymbolum*)xar_obtinere(
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

                /* <tolera codex="SUBTRACTIO_COMPARATA" (>custos supra: punctum minor num_dextrum probatum (res completae praetermissae perge) */
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
                    SilvaGenProductio* prod_b;
                    i32 num_first_ba;
                    i32 m;

                    prod_b = (SilvaGenProductio*)xar_obtinere(
                        grammatica->productiones, j);
                    si (!prod_b || prod_b->sinistrum != *b_idx_ptr) perge;

                    /* Pro unoquoque terminale b in FIRST(beta a) */
                    num_first_ba = (i32)xar_numerus(first_beta_a);
                    per (m = ZEPHYRUM; m < num_first_ba; m++)
                    {
                        s32* b_term = (s32*)xar_obtinere(first_beta_a, m);
                        SilvaGenRes nova_res;

                        si (!b_term) perge;

                        nova_res.productio = prod_b->index;
                        nova_res.punctum = ZEPHYRUM;
                        nova_res.prospectus = *b_term;

                        si (!res_in_coniuncto(res_xar, &nova_res))
                        {
                            SilvaGenRes* addita = (SilvaGenRes*)xar_addere(res_xar);
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
    SilvaGenGrammatica*  grammatica,
    Xar*                res_xar,
    s32                 symbolum)
{
    Xar* nova_res;
    i32  i;
    i32  numerus;

    nova_res = xar_creare(grammatica->piscina, (i32)magnitudo(SilvaGenRes));
    numerus = (i32)xar_numerus(res_xar);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenRes*       res_currens;
        SilvaGenProductio* prod;
        i32               num_dextrum;
        s32*              sym_idx_ptr;

        res_currens = (SilvaGenRes*)xar_obtinere(res_xar, i);
        si (!res_currens) perge;

        prod = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, (i32)res_currens->productio);
        si (!prod) perge;

        num_dextrum = (i32)xar_numerus(prod->dextrum);
        si ((i32)res_currens->punctum >= num_dextrum) perge;

        sym_idx_ptr = (s32*)xar_obtinere(prod->dextrum,
            (i32)res_currens->punctum);
        si (!sym_idx_ptr || *sym_idx_ptr != symbolum) perge;

        /* Movere punctum */
        {
            SilvaGenRes* nova = (SilvaGenRes*)xar_addere(nova_res);
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
    constans SilvaGenRes* ra = (constans SilvaGenRes*)a;
    constans SilvaGenRes* rb = (constans SilvaGenRes*)b;

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
        SilvaGenRes* a = (SilvaGenRes*)xar_obtinere(res_a, i);
        SilvaGenRes* b = (SilvaGenRes*)xar_obtinere(res_b, i);

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
        SilvaGenStatus* s = (SilvaGenStatus*)xar_obtinere(status_omnes, i);
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
    SilvaGenGrammatica*  grammatica,
    Xar*                res_xar)
{
    Xar* symbola;
    i32  i;
    i32  numerus;

    symbola = xar_creare(grammatica->piscina, (i32)magnitudo(s32));
    numerus = (i32)xar_numerus(res_xar);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenRes*       res_currens;
        SilvaGenProductio* prod;
        s32*              sym_idx_ptr;

        res_currens = (SilvaGenRes*)xar_obtinere(res_xar, i);
        si (!res_currens) perge;

        prod = (SilvaGenProductio*)xar_obtinere(
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

SilvaGenCollectio*
silva_gen_collectio_construere(
    SilvaGenGrammatica*  grammatica)
{
    SilvaGenCollectio*   collectio;
    Xar*                opus;  /* worklist de s32 (indices statuum) */
    SilvaGenProductio*   prod_augmentata;
    i32                 num_prod;
    i32                 i;

    si (!grammatica) redde NIHIL;

    num_prod = (i32)xar_numerus(grammatica->productiones);

    /* Invenire productionem augmentatam: S' -> ... */
    prod_augmentata = NIHIL;
    per (i = ZEPHYRUM; i < num_prod; i++)
    {
        SilvaGenProductio* p = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, i);
        si (p && p->sinistrum == grammatica->initium_index)
        {
            prod_augmentata = p;
            frange;
        }
    }

    si (!prod_augmentata)
    {
        fprintf(stderr, "silva_gen: productio augmentata non inventa\n");
        redde NIHIL;
    }

    /* Creare collectionem */
    collectio = (SilvaGenCollectio*)piscina_allocare(
        grammatica->piscina,
        (memoriae_index)magnitudo(SilvaGenCollectio));
    collectio->status_omnes = xar_creare(grammatica->piscina,
        (i32)magnitudo(SilvaGenStatus));
    collectio->transitiones = xar_creare(grammatica->piscina,
        (i32)magnitudo(SilvaGenTransitio));
    collectio->grammatica = grammatica;

    opus = xar_creare(grammatica->piscina, (i32)magnitudo(s32));

    /* Status 0: clausura({[S' -> . initium, $]}) */
    {
        Xar*           res_initium;
        SilvaGenRes*    res_prima;
        SilvaGenStatus* status_0;
        s32*           opus_elem;

        res_initium = xar_creare(grammatica->piscina,
            (i32)magnitudo(SilvaGenRes));
        res_prima = (SilvaGenRes*)xar_addere(res_initium);
        res_prima->productio = prod_augmentata->index;
        res_prima->punctum = ZEPHYRUM;
        res_prima->prospectus = (s32)SILVA_GEN_EOF_PROSPECTUS;

        clausura_computare(grammatica, res_initium);
        xar_ordinare(res_initium, res_comparator);

        status_0 = (SilvaGenStatus*)xar_addere(collectio->status_omnes);
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
            SilvaGenStatus* status_currens;
            Xar*           symbola;
            i32            j;

            stat_idx_ptr = (s32*)xar_obtinere(opus, opus_index);
            opus_index++;

            si (!stat_idx_ptr) perge;

            status_currens = (SilvaGenStatus*)xar_obtinere(
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
                    SilvaGenStatus* novus;
                    s32*           opus_elem;

                    status_novus_idx =
                        (s32)xar_numerus(collectio->status_omnes);
                    novus = (SilvaGenStatus*)xar_addere(
                        collectio->status_omnes);
                    novus->res = nova_res;
                    novus->index = status_novus_idx;

                    opus_elem = (s32*)xar_addere(opus);
                    *opus_elem = status_novus_idx;
                }

                /* Notare transitionem */
                {
                    SilvaGenTransitio* trans;
                    trans = (SilvaGenTransitio*)xar_addere(
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
 * LALR(1) Constructio
 * ================================================ */

/* LR(0) res: productio + punctum (sine prospectu) */
nomen structura {
    s32  productio;
    s32  punctum;
} SilvaGenRes0;

/* Comparator pro LR(0) rebus */
hic_manens s32
res0_comparator(constans vacuum* a, constans vacuum* b)
{
    constans SilvaGenRes0* ra = (constans SilvaGenRes0*)a;
    constans SilvaGenRes0* rb = (constans SilvaGenRes0*)b;

    si (ra->productio != rb->productio)
        redde (ra->productio < rb->productio) ? -I : I;
    si (ra->punctum != rb->punctum)
        redde (ra->punctum < rb->punctum) ? -I : I;
    redde ZEPHYRUM;
}

/* Verificare si LR(0) res iam in coniuncto */
hic_manens b32
res0_in_coniuncto(
    Xar*          res_xar,
    SilvaGenRes0*  res)
{
    i32 i;
    i32 numerus = (i32)xar_numerus(res_xar);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenRes0* existens = (SilvaGenRes0*)xar_obtinere(res_xar, i);
        si (existens &&
            existens->productio == res->productio &&
            existens->punctum == res->punctum)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* LR(0) clausura: expandere sine prospectu */
hic_manens vacuum
clausura0_computare(
    SilvaGenGrammatica*  grammatica,
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
            SilvaGenRes0*      res_currens;
            SilvaGenProductio* prod;
            i32               num_dextrum;
            s32*              b_idx_ptr;
            SilvaGenSymbolum*  b_sym;
            i32               j;

            res_currens = (SilvaGenRes0*)xar_obtinere(res_xar, i);
            si (!res_currens) perge;

            prod = (SilvaGenProductio*)xar_obtinere(
                grammatica->productiones, (i32)res_currens->productio);
            si (!prod) perge;

            num_dextrum = (i32)xar_numerus(prod->dextrum);
            si ((i32)res_currens->punctum >= num_dextrum) perge;

            b_idx_ptr = (s32*)xar_obtinere(prod->dextrum,
                (i32)res_currens->punctum);
            si (!b_idx_ptr) perge;

            b_sym = (SilvaGenSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)*b_idx_ptr);
            si (!b_sym || b_sym->est_terminale) perge;

            /* Pro unaquaque productione B -> gamma, addere [B -> . gamma] */
            per (j = ZEPHYRUM; j < numerus_productionum; j++)
            {
                SilvaGenProductio* prod_b;
                SilvaGenRes0       nova_res;

                prod_b = (SilvaGenProductio*)xar_obtinere(
                    grammatica->productiones, j);
                si (!prod_b || prod_b->sinistrum != *b_idx_ptr) perge;

                nova_res.productio = prod_b->index;
                nova_res.punctum = ZEPHYRUM;

                si (!res0_in_coniuncto(res_xar, &nova_res))
                {
                    SilvaGenRes0* addita = (SilvaGenRes0*)xar_addere(res_xar);
                    si (addita)
                    {
                        *addita = nova_res;
                        mutatum = VERUM;
                    }
                }
            }
        }
    } dum (mutatum);
}

/* LR(0) goto: movere punctum trans symbolum */
hic_manens Xar*
goto0_computare(
    SilvaGenGrammatica*  grammatica,
    Xar*                res_xar,
    s32                 symbolum)
{
    Xar* nova_res;
    i32  i;
    i32  numerus;

    nova_res = xar_creare(grammatica->piscina, (i32)magnitudo(SilvaGenRes0));
    numerus = (i32)xar_numerus(res_xar);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenRes0*      res_currens;
        SilvaGenProductio* prod;
        i32               num_dextrum;
        s32*              sym_idx_ptr;

        res_currens = (SilvaGenRes0*)xar_obtinere(res_xar, i);
        si (!res_currens) perge;

        prod = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, (i32)res_currens->productio);
        si (!prod) perge;

        num_dextrum = (i32)xar_numerus(prod->dextrum);
        si ((i32)res_currens->punctum >= num_dextrum) perge;

        sym_idx_ptr = (s32*)xar_obtinere(prod->dextrum,
            (i32)res_currens->punctum);
        si (!sym_idx_ptr || *sym_idx_ptr != symbolum) perge;

        {
            SilvaGenRes0* nova = (SilvaGenRes0*)xar_addere(nova_res);
            si (nova)
            {
                nova->productio = res_currens->productio;
                nova->punctum = res_currens->punctum + I;
            }
        }
    }

    si ((i32)xar_numerus(nova_res) > ZEPHYRUM)
    {
        clausura0_computare(grammatica, nova_res);
    }

    redde nova_res;
}

/* Verificare si duo LR(0) coniuncta sunt aequalia */
hic_manens b32
status0_aequales(
    Xar*  res_a,
    Xar*  res_b)
{
    i32 num_a;
    i32 num_b;
    i32 i;

    num_a = (i32)xar_numerus(res_a);
    num_b = (i32)xar_numerus(res_b);

    si (num_a != num_b) redde FALSUM;

    per (i = ZEPHYRUM; i < num_a; i++)
    {
        SilvaGenRes0* a = (SilvaGenRes0*)xar_obtinere(res_a, i);
        SilvaGenRes0* b = (SilvaGenRes0*)xar_obtinere(res_b, i);

        si (!a || !b) redde FALSUM;
        si (a->productio != b->productio ||
            a->punctum != b->punctum)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Invenire LR(0) statum */
hic_manens s32
statum0_invenire(
    Xar*  status_omnes,
    Xar*  res_xar)
{
    i32 i;
    i32 numerus = (i32)xar_numerus(status_omnes);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenStatus* s = (SilvaGenStatus*)xar_obtinere(status_omnes, i);
        si (s && status0_aequales(s->res, res_xar))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* Colligere symbola post punctum in LR(0) rebus */
hic_manens Xar*
symbola_post_punctum0_colligere(
    SilvaGenGrammatica*  grammatica,
    Xar*                res_xar)
{
    Xar* symbola;
    i32  i;
    i32  numerus;

    symbola = xar_creare(grammatica->piscina, (i32)magnitudo(s32));
    numerus = (i32)xar_numerus(res_xar);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenRes0*      res_currens;
        SilvaGenProductio* prod;
        s32*              sym_idx_ptr;

        res_currens = (SilvaGenRes0*)xar_obtinere(res_xar, i);
        si (!res_currens) perge;

        prod = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, (i32)res_currens->productio);
        si (!prod) perge;

        si ((i32)res_currens->punctum >= (i32)xar_numerus(prod->dextrum)) perge;

        sym_idx_ptr = (s32*)xar_obtinere(prod->dextrum,
            (i32)res_currens->punctum);
        si (!sym_idx_ptr) perge;

        si (!first_continet(symbola, *sym_idx_ptr))
        {
            s32* novum = (s32*)xar_addere(symbola);
            si (novum) *novum = *sym_idx_ptr;
        }
    }

    redde symbola;
}

/* Tabula prospectuum: pro unoquoque statu et unaquaque re,
 * Xar de s32 (prospectus terminales) */
nomen structura {
    Xar** prospectus;   /* [numerus_rerum] Xar de s32 */
    s32   numerus_rerum;
} SilvaGenProspectusTabula;

/* ================================================
 * LALR(1) Constructio Principalis
 * ================================================ */

SilvaGenCollectio*
silva_gen_collectio_lalr_construere(
    SilvaGenGrammatica*  grammatica)
{
    /* Pars I: Construere LR(0) nucleos */
    Xar*                lr0_status;    /* Xar de SilvaGenStatus (res = Xar de SilvaGenRes0) */
    Xar*                lr0_trans;     /* Xar de SilvaGenTransitio */
    Xar*                opus;
    SilvaGenProductio*   prod_augmentata;
    i32                 num_prod;
    i32                 i;
    i32                 num_status;
    SilvaGenProspectusTabula* pt;
    SilvaGenCollectio*   collectio;

    si (!grammatica) redde NIHIL;

    num_prod = (i32)xar_numerus(grammatica->productiones);

    /* Invenire productionem augmentatam */
    prod_augmentata = NIHIL;
    per (i = ZEPHYRUM; i < num_prod; i++)
    {
        SilvaGenProductio* p = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, i);
        si (p && p->sinistrum == grammatica->initium_index)
        {
            prod_augmentata = p;
            frange;
        }
    }

    si (!prod_augmentata)
    {
        fprintf(stderr, "silva_gen lalr: productio augmentata non inventa\n");
        redde NIHIL;
    }

    /* --- Construere LR(0) automaton --- */
    lr0_status = xar_creare(grammatica->piscina, (i32)magnitudo(SilvaGenStatus));
    lr0_trans = xar_creare(grammatica->piscina, (i32)magnitudo(SilvaGenTransitio));
    opus = xar_creare(grammatica->piscina, (i32)magnitudo(s32));

    /* Status 0 */
    {
        Xar*           res_initium;
        SilvaGenRes0*   res_prima;
        SilvaGenStatus* status_0;
        s32*           opus_elem;

        res_initium = xar_creare(grammatica->piscina,
            (i32)magnitudo(SilvaGenRes0));
        res_prima = (SilvaGenRes0*)xar_addere(res_initium);
        res_prima->productio = prod_augmentata->index;
        res_prima->punctum = ZEPHYRUM;

        clausura0_computare(grammatica, res_initium);
        xar_ordinare(res_initium, res0_comparator);

        status_0 = (SilvaGenStatus*)xar_addere(lr0_status);
        status_0->res = res_initium;
        status_0->index = ZEPHYRUM;

        opus_elem = (s32*)xar_addere(opus);
        *opus_elem = ZEPHYRUM;
    }

    /* LR(0) worklist */
    {
        i32 opus_index = ZEPHYRUM;

        dum (opus_index < (i32)xar_numerus(opus))
        {
            s32*           stat_idx_ptr;
            SilvaGenStatus* status_currens;
            Xar*           symbola;
            i32            j;

            stat_idx_ptr = (s32*)xar_obtinere(opus, opus_index);
            opus_index++;

            si (!stat_idx_ptr) perge;

            status_currens = (SilvaGenStatus*)xar_obtinere(
                lr0_status, (i32)*stat_idx_ptr);
            si (!status_currens) perge;

            symbola = symbola_post_punctum0_colligere(
                grammatica, status_currens->res);

            per (j = ZEPHYRUM; j < (i32)xar_numerus(symbola); j++)
            {
                s32* sym_ptr = (s32*)xar_obtinere(symbola, j);
                Xar* nova_res;
                s32  status_novus_idx;

                si (!sym_ptr) perge;

                nova_res = goto0_computare(
                    grammatica, status_currens->res, *sym_ptr);

                si ((i32)xar_numerus(nova_res) == ZEPHYRUM) perge;

                xar_ordinare(nova_res, res0_comparator);

                status_novus_idx = statum0_invenire(lr0_status, nova_res);

                si (status_novus_idx < ZEPHYRUM)
                {
                    SilvaGenStatus* novus;
                    s32*           opus_elem;

                    status_novus_idx = (s32)xar_numerus(lr0_status);
                    novus = (SilvaGenStatus*)xar_addere(lr0_status);
                    novus->res = nova_res;
                    novus->index = status_novus_idx;

                    opus_elem = (s32*)xar_addere(opus);
                    *opus_elem = status_novus_idx;
                }

                /* Notare transitionem */
                {
                    SilvaGenTransitio* trans;
                    trans = (SilvaGenTransitio*)xar_addere(lr0_trans);
                    trans->status = *stat_idx_ptr;
                    trans->symbolum = *sym_ptr;
                    trans->status_novus = status_novus_idx;
                }
            }
        }
    }

    num_status = (i32)xar_numerus(lr0_status);

    /* --- Pars II: Allocare prospectus tabulas --- */
    pt = (SilvaGenProspectusTabula*)piscina_allocare(
        grammatica->piscina,
        (memoriae_index)((i32)magnitudo(SilvaGenProspectusTabula) * num_status));

    per (i = ZEPHYRUM; i < num_status; i++)
    {
        SilvaGenStatus* s = (SilvaGenStatus*)xar_obtinere(lr0_status, i);
        i32 nr = (i32)xar_numerus(s->res);
        i32 k;

        pt[i].numerus_rerum = (s32)nr;
        pt[i].prospectus = (Xar**)piscina_allocare(
            grammatica->piscina,
            (memoriae_index)((i32)magnitudo(Xar*) * nr));

        per (k = ZEPHYRUM; k < nr; k++)
        {
            pt[i].prospectus[k] = xar_creare(grammatica->piscina,
                (i32)magnitudo(s32));
        }
    }

    /* Semine: status 0, res augmentata habeat prospectum EOF */
    {
        s32   eof_val = (s32)SILVA_GEN_EOF_PROSPECTUS;
        s32*  p_eof;
        i32   aug_ri;
        SilvaGenStatus* s0_aug = (SilvaGenStatus*)xar_obtinere(lr0_status, ZEPHYRUM);
        i32   nr0_aug = (i32)xar_numerus(s0_aug->res);

        /* Invenire indicem rei augmentatae in statu 0 */
        aug_ri = ZEPHYRUM;
        per (aug_ri = ZEPHYRUM; aug_ri < nr0_aug; aug_ri++)
        {
            SilvaGenRes0* r0a = (SilvaGenRes0*)xar_obtinere(s0_aug->res, aug_ri);
            si (r0a && r0a->productio == prod_augmentata->index &&
                r0a->punctum == ZEPHYRUM)
            {
                frange;
            }
        }

        p_eof = (s32*)xar_addere(pt[0].prospectus[aug_ri]);
        *p_eof = eof_val;
    }

    /* --- Pars III: Propagare prospectus --- */
    /* Pro unoquoque statu, unaquaque re, unoquoque prospectu:
     * computare FIRST(beta a) et propagare ad status successorum */
    {
        b32 mutatum_global;

        fac
        {
            i32 si_idx; /* status index */

            mutatum_global = FALSUM;

            per (si_idx = ZEPHYRUM; si_idx < num_status; si_idx++)
            {
                SilvaGenStatus* status_lr0;
                i32            nr;
                i32            ri; /* res index */

                status_lr0 = (SilvaGenStatus*)xar_obtinere(lr0_status, si_idx);
                si (!status_lr0) perge;

                nr = (i32)xar_numerus(status_lr0->res);

                per (ri = ZEPHYRUM; ri < nr; ri++)
                {
                    SilvaGenRes0*      res0;
                    SilvaGenProductio* prod;
                    i32               num_dextrum;
                    SilvaGenSymbolum*  b_sym;
                    i32               num_prosp;
                    i32               pi; /* prospectus index */

                    res0 = (SilvaGenRes0*)xar_obtinere(status_lr0->res, ri);
                    si (!res0) perge;

                    prod = (SilvaGenProductio*)xar_obtinere(
                        grammatica->productiones, (i32)res0->productio);
                    si (!prod) perge;

                    num_dextrum = (i32)xar_numerus(prod->dextrum);

                    /* Casus 1: Punctum non ad finem - propagare trans GOTO */
                    si ((i32)res0->punctum < num_dextrum)
                    {
                        s32* sym_ptr;
                        i32  ti; /* transitio index */
                        i32  num_trans;

                        sym_ptr = (s32*)xar_obtinere(prod->dextrum,
                            (i32)res0->punctum);
                        si (!sym_ptr) perge;

                        /* Invenire transitionem ex hoc statu per hoc symbolum */
                        num_trans = (i32)xar_numerus(lr0_trans);
                        per (ti = ZEPHYRUM; ti < num_trans; ti++)
                        {
                            SilvaGenTransitio* trans;
                            SilvaGenStatus*    dest_status;
                            i32               dest_nr;
                            i32               di; /* dest res index */

                            trans = (SilvaGenTransitio*)xar_obtinere(
                                lr0_trans, ti);
                            si (!trans) perge;
                            si (trans->status != (s32)si_idx) perge;
                            si (trans->symbolum != *sym_ptr) perge;

                            /* Invenire rem correspondentem in statu destinationis */
                            dest_status = (SilvaGenStatus*)xar_obtinere(
                                lr0_status, (i32)trans->status_novus);
                            si (!dest_status) perge;

                            dest_nr = (i32)xar_numerus(dest_status->res);
                            per (di = ZEPHYRUM; di < dest_nr; di++)
                            {
                                SilvaGenRes0* dest_res;

                                dest_res = (SilvaGenRes0*)xar_obtinere(
                                    dest_status->res, di);
                                si (!dest_res) perge;

                                si (dest_res->productio == res0->productio &&
                                    dest_res->punctum == res0->punctum + I)
                                {
                                    /* Propagare omnes prospectus */
                                    num_prosp = (i32)xar_numerus(
                                        pt[si_idx].prospectus[ri]);

                                    per (pi = ZEPHYRUM; pi < num_prosp; pi++)
                                    {
                                        s32* prosp = (s32*)xar_obtinere(
                                            pt[si_idx].prospectus[ri], pi);
                                        si (!prosp) perge;

                                        si (!first_continet(
                                            pt[trans->status_novus].prospectus[di],
                                            *prosp))
                                        {
                                            s32* novum = (s32*)xar_addere(
                                                pt[trans->status_novus].prospectus[di]);
                                            *novum = *prosp;
                                            mutatum_global = VERUM;
                                        }
                                    }
                                    frange;
                                }
                            }
                            frange; /* una transitio per symbolum */
                        }

                        /* Casus 1b: Si symbolum post punctum est non-terminale B,
                         * generare prospectus pro productionibus B -> . gamma */
                        b_sym = (SilvaGenSymbolum*)xar_obtinere(
                            grammatica->symbola, (i32)*sym_ptr);
                        si (b_sym && !b_sym->est_terminale)
                        {
                            /* FIRST(beta a) ubi beta = symbola post B,
                             * a = prospectus currentes */
                            i32 num_beta;
                            s32* beta_indices;
                            i32 k;

                            num_beta = num_dextrum - (i32)res0->punctum - I;
                            beta_indices = NIHIL;

                            /* <tolera codex="SUBTRACTIO_COMPARATA" (>custos supra: blocus totus intra punctum minor num_dextrum stat */
                            si (num_beta > ZEPHYRUM)
                            {
                                beta_indices = (s32*)piscina_allocare(
                                    grammatica->piscina,
                                    (memoriae_index)((i32)magnitudo(s32) * num_beta));
                                per (k = ZEPHYRUM; k < num_beta; k++)
                                {
                                    s32* si2 = (s32*)xar_obtinere(prod->dextrum,
                                        (i32)res0->punctum + I + k);
                                    si (si2) beta_indices[k] = *si2;
                                }
                            }

                            /* Pro unoquoque prospectu currente a */
                            num_prosp = (i32)xar_numerus(
                                pt[si_idx].prospectus[ri]);

                            per (pi = ZEPHYRUM; pi < num_prosp; pi++)
                            {
                                s32* prosp_a;
                                Xar* first_beta_a;
                                i32  pj; /* prod index */

                                prosp_a = (s32*)xar_obtinere(
                                    pt[si_idx].prospectus[ri], pi);
                                si (!prosp_a) perge;

                                first_beta_a = xar_creare(
                                    grammatica->piscina, (i32)magnitudo(s32));

                                first_sequentiae_computare(
                                    grammatica,
                                    beta_indices, num_beta,
                                    *prosp_a,
                                    first_beta_a);

                                /* Pro unaquaque productione B -> gamma */
                                per (pj = ZEPHYRUM; pj < num_prod; pj++)
                                {
                                    SilvaGenProductio* prod_b;
                                    i32 qi; /* res index in statu currente */

                                    prod_b = (SilvaGenProductio*)xar_obtinere(
                                        grammatica->productiones, pj);
                                    si (!prod_b) perge;
                                    si (prod_b->sinistrum != *sym_ptr) perge;

                                    /* Invenire [B -> . gamma] in statu currente */
                                    per (qi = ZEPHYRUM; qi < nr; qi++)
                                    {
                                        SilvaGenRes0* qres;
                                        i32 fbi; /* first beta a index */
                                        i32 num_fba;

                                        qres = (SilvaGenRes0*)xar_obtinere(
                                            status_lr0->res, qi);
                                        si (!qres) perge;
                                        si (qres->productio != prod_b->index) perge;
                                        si (qres->punctum != ZEPHYRUM) perge;

                                        /* Addere FIRST(beta a) ad prospectus */
                                        num_fba = (i32)xar_numerus(first_beta_a);
                                        per (fbi = ZEPHYRUM; fbi < num_fba; fbi++)
                                        {
                                            s32* fb_term = (s32*)xar_obtinere(
                                                first_beta_a, fbi);
                                            si (!fb_term) perge;

                                            si (!first_continet(
                                                pt[si_idx].prospectus[qi],
                                                *fb_term))
                                            {
                                                s32* novum = (s32*)xar_addere(
                                                    pt[si_idx].prospectus[qi]);
                                                *novum = *fb_term;
                                                mutatum_global = VERUM;
                                            }
                                        }
                                        frange;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } dum (mutatum_global);
    }

    /* --- Pars IV: Convertere ad SilvaGenCollectio cum LR(1) rebus --- */
    collectio = (SilvaGenCollectio*)piscina_allocare(
        grammatica->piscina,
        (memoriae_index)magnitudo(SilvaGenCollectio));
    collectio->status_omnes = xar_creare(grammatica->piscina,
        (i32)magnitudo(SilvaGenStatus));
    collectio->transitiones = lr0_trans;
    collectio->grammatica = grammatica;

    per (i = ZEPHYRUM; i < num_status; i++)
    {
        SilvaGenStatus* lr0_s;
        SilvaGenStatus* lr1_s;
        Xar*           lr1_res;
        i32            nr;
        i32            ri;

        lr0_s = (SilvaGenStatus*)xar_obtinere(lr0_status, i);
        si (!lr0_s) perge;

        nr = (i32)xar_numerus(lr0_s->res);
        lr1_res = xar_creare(grammatica->piscina, (i32)magnitudo(SilvaGenRes));

        per (ri = ZEPHYRUM; ri < nr; ri++)
        {
            SilvaGenRes0* res0;
            i32          num_prosp;
            i32          pi;

            res0 = (SilvaGenRes0*)xar_obtinere(lr0_s->res, ri);
            si (!res0) perge;

            num_prosp = (i32)xar_numerus(pt[i].prospectus[ri]);

            per (pi = ZEPHYRUM; pi < num_prosp; pi++)
            {
                s32* prosp;
                SilvaGenRes* lr1_r;

                prosp = (s32*)xar_obtinere(pt[i].prospectus[ri], pi);
                si (!prosp) perge;

                lr1_r = (SilvaGenRes*)xar_addere(lr1_res);
                lr1_r->productio = res0->productio;
                lr1_r->punctum = res0->punctum;
                lr1_r->prospectus = *prosp;
            }
        }

        xar_ordinare(lr1_res, res_comparator);

        lr1_s = (SilvaGenStatus*)xar_addere(collectio->status_omnes);
        lr1_s->res = lr1_res;
        lr1_s->index = (s32)i;
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
    SilvaGenActioIntroitus* nova)
{
    i32 i;
    i32 numerus = (i32)xar_numerus(actiones);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenActioIntroitus* existens =
            (SilvaGenActioIntroitus*)xar_obtinere(actiones, i);
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

/* Auxiliaris: praelationes applicare (M2c) - ANTE detectionem
 * conflictuum. Cella (status x terminalis praelatus) quae ET
 * transpositionem ET reductionem fert: reductiones removentur,
 * transpositio sola manet, cella in cellas praelatas memoratur
 * (categoria census - figitur sicut cellae conflictuum).
 * Cella sine transpositione NON tangitur (praelatio actionem
 * NOMINATAM retinet, non caecam). */
hic_manens vacuum
praelationes_applicare(
    SilvaGenTabula* tabula)
{
    SilvaGenGrammatica* grammatica = tabula->grammatica;
    Piscina*            piscina = grammatica->piscina;
    i32                 num_status;
    i32                 num_praelationum;
    i32                 p;
    i32                 s;

    tabula->cellae_praelatae = xar_creare(piscina,
        (i32)magnitudo(SilvaGenCellaPraelata));
    tabula->numerus_praelatarum = ZEPHYRUM;

    num_praelationum = (i32)xar_numerus(grammatica->praelationes);
    si (num_praelationum == ZEPHYRUM) redde;

    num_status = (i32)xar_numerus(tabula->status_tabulae);

    per (p = ZEPHYRUM; p < num_praelationum; p++)
    {
        SilvaGenPraelatio* prael = (SilvaGenPraelatio*)xar_obtinere(
            grammatica->praelationes, p);

        si (!prael) perge;

        per (s = ZEPHYRUM; s < num_status; s++)
        {
            SilvaGenStatusTabula* st = (SilvaGenStatusTabula*)
                xar_obtinere(tabula->status_tabulae, s);
            b32 habet_trans = FALSUM;
            b32 habet_red = FALSUM;
            i32 num_actiones;
            i32 i;

            si (!st) perge;
            num_actiones = (i32)xar_numerus(st->actiones);

            per (i = ZEPHYRUM; i < num_actiones; i++)
            {
                SilvaGenActioIntroitus* ai = (SilvaGenActioIntroitus*)
                    xar_obtinere(st->actiones, i);

                si (!ai || ai->terminalis != prael->terminalis) perge;
                si (ai->actio == SILVA_GEN_ACTIO_TRANSPONERE)
                {
                    habet_trans = VERUM;
                }
                alioquin si (ai->actio == SILVA_GEN_ACTIO_REDUCERE)
                {
                    habet_red = VERUM;
                }
            }
            si (!habet_trans || !habet_red) perge;

            /* Cella conflictus in terminali praelato: reducere
             * novam listam actionum sine reductionibus eius */
            {
                Xar* novae = xar_creare(piscina,
                    (i32)magnitudo(SilvaGenActioIntroitus));

                per (i = ZEPHYRUM; i < num_actiones; i++)
                {
                    SilvaGenActioIntroitus* ai =
                        (SilvaGenActioIntroitus*)xar_obtinere(
                            st->actiones, i);

                    si (!ai) perge;
                    si (ai->terminalis == prael->terminalis
                        && ai->actio == SILVA_GEN_ACTIO_REDUCERE)
                    {
                        SilvaGenCellaPraelata* cella =
                            (SilvaGenCellaPraelata*)xar_addere(
                                tabula->cellae_praelatae);

                        cella->status = (s32)s;
                        cella->terminalis = prael->terminalis;
                        cella->actio_retenta = prael->actio;
                        cella->productio_remota = ai->valor;
                        tabula->numerus_praelatarum++;
                        perge;  /* remota - non copiatur */
                    }
                    *(SilvaGenActioIntroitus*)xar_addere(novae) = *ai;
                }
                st->actiones = novae;
            }
        }
    }
}

/* Auxiliaris: detegere conflictus in tabula */
hic_manens vacuum
conflictus_detegere(
    SilvaGenTabula* tabula)
{
    i32 s;
    i32 num_status;

    tabula->numerus_conflictuum = ZEPHYRUM;
    num_status = (i32)xar_numerus(tabula->status_tabulae);

    per (s = ZEPHYRUM; s < num_status; s++)
    {
        SilvaGenStatusTabula* st =
            (SilvaGenStatusTabula*)xar_obtinere(tabula->status_tabulae, s);
        i32 num_actiones;
        i32 i;
        i32 j;

        si (!st) perge;

        num_actiones = (i32)xar_numerus(st->actiones);
        st->habet_conflictum = FALSUM;

        per (i = ZEPHYRUM; i < num_actiones; i++)
        {
            SilvaGenActioIntroitus* ai =
                (SilvaGenActioIntroitus*)xar_obtinere(st->actiones, i);
            si (!ai) perge;

            per (j = i + I; j < num_actiones; j++)
            {
                SilvaGenActioIntroitus* aj =
                    (SilvaGenActioIntroitus*)xar_obtinere(st->actiones, j);
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

SilvaGenTabula*
silva_gen_tabulam_construere(
    SilvaGenCollectio*  collectio)
{
    SilvaGenTabula*      tabula;
    SilvaGenGrammatica*  grammatica;
    Piscina*            piscina;
    i32                 num_status;
    i32                 num_transitiones;
    i32                 s;
    i32                 t;

    si (!collectio || !collectio->grammatica) redde NIHIL;

    grammatica = collectio->grammatica;
    piscina = grammatica->piscina;

    /* Creare tabulam */
    tabula = (SilvaGenTabula*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaGenTabula));
    tabula->grammatica = grammatica;
    tabula->collectio = collectio;
    tabula->numerus_conflictuum = ZEPHYRUM;

    num_status = (i32)xar_numerus(collectio->status_omnes);
    tabula->status_tabulae = xar_creare(piscina,
        (i32)magnitudo(SilvaGenStatusTabula));

    /* Initializare tabulam pro unoquoque statu */
    per (s = ZEPHYRUM; s < num_status; s++)
    {
        SilvaGenStatusTabula* st =
            (SilvaGenStatusTabula*)xar_addere(tabula->status_tabulae);
        st->actiones = xar_creare(piscina,
            (i32)magnitudo(SilvaGenActioIntroitus));
        st->goto_introitus = xar_creare(piscina,
            (i32)magnitudo(SilvaGenGotoIntroitus));
        st->index = (s32)s;
        st->habet_conflictum = FALSUM;
    }

    /* Passus 1: Transitiones -> SHIFT (terminalia) et GOTO (non-terminalia) */
    num_transitiones = (i32)xar_numerus(collectio->transitiones);

    per (t = ZEPHYRUM; t < num_transitiones; t++)
    {
        SilvaGenTransitio* trans =
            (SilvaGenTransitio*)xar_obtinere(collectio->transitiones, t);
        SilvaGenSymbolum*  sym;
        SilvaGenStatusTabula* st;

        si (!trans) perge;

        sym = (SilvaGenSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)trans->symbolum);
        si (!sym) perge;

        st = (SilvaGenStatusTabula*)xar_obtinere(
            tabula->status_tabulae, (i32)trans->status);
        si (!st) perge;

        si (sym->est_terminale)
        {
            /* SHIFT actio */
            SilvaGenActioIntroitus nova;
            nova.terminalis = trans->symbolum;
            nova.actio = SILVA_GEN_ACTIO_TRANSPONERE;
            nova.valor = trans->status_novus;
            nova.conflictus_intentus = FALSUM;

            si (!actio_iam_existit(st->actiones, &nova))
            {
                SilvaGenActioIntroitus* addita =
                    (SilvaGenActioIntroitus*)xar_addere(st->actiones);
                si (addita) *addita = nova;
            }
        }
        alioquin
        {
            /* GOTO introitus */
            SilvaGenGotoIntroitus* gi =
                (SilvaGenGotoIntroitus*)xar_addere(st->goto_introitus);
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
        SilvaGenStatus* status_lr =
            (SilvaGenStatus*)xar_obtinere(collectio->status_omnes, s);
        SilvaGenStatusTabula* st;
        i32 num_rerum;
        i32 r;

        si (!status_lr) perge;

        st = (SilvaGenStatusTabula*)xar_obtinere(
            tabula->status_tabulae, s);
        si (!st) perge;

        num_rerum = (i32)xar_numerus(status_lr->res);

        per (r = ZEPHYRUM; r < num_rerum; r++)
        {
            SilvaGenRes* res =
                (SilvaGenRes*)xar_obtinere(status_lr->res, r);
            SilvaGenProductio* prod;
            i32 num_dextrum;

            si (!res) perge;

            prod = (SilvaGenProductio*)xar_obtinere(
                grammatica->productiones, (i32)res->productio);
            si (!prod) perge;

            num_dextrum = (i32)xar_numerus(prod->dextrum);

            /* Punctum ad finem? */
            si ((i32)res->punctum == num_dextrum)
            {
                SilvaGenActioIntroitus nova;
                nova.conflictus_intentus = FALSUM;

                si (prod->sinistrum == grammatica->initium_index &&
                    res->prospectus == (s32)SILVA_GEN_EOF_PROSPECTUS)
                {
                    /* ACCEPT */
                    nova.terminalis = (s32)SILVA_GEN_EOF_PROSPECTUS;
                    nova.actio = SILVA_GEN_ACTIO_ACCIPERE;
                    nova.valor = ZEPHYRUM;
                }
                alioquin
                {
                    /* REDUCE */
                    nova.terminalis = res->prospectus;
                    nova.actio = SILVA_GEN_ACTIO_REDUCERE;
                    nova.valor = prod->index;
                }

                si (!actio_iam_existit(st->actiones, &nova))
                {
                    SilvaGenActioIntroitus* addita =
                        (SilvaGenActioIntroitus*)xar_addere(st->actiones);
                    si (addita) *addita = nova;
                }
            }
        }
    }

    /* Passus 2.b: Praelationes applicare (resolutio declarata -
     * ANTE detectionem: cellae praelatae conflictus non sunt) */
    praelationes_applicare(tabula);

    /* Passus 3: Detegere conflictus */
    conflictus_detegere(tabula);

    redde tabula;
}

Xar*
silva_gen_actiones_quaerere(
    SilvaGenTabula*  tabula,
    s32             status,
    s32             terminalis)
{
    SilvaGenStatusTabula* st;
    Xar* fructus;
    i32 i;
    i32 numerus;

    si (!tabula) redde NIHIL;

    st = (SilvaGenStatusTabula*)xar_obtinere(
        tabula->status_tabulae, (i32)status);
    si (!st) redde NIHIL;

    fructus = xar_creare(tabula->grammatica->piscina,
        (i32)magnitudo(SilvaGenActioIntroitus));

    numerus = (i32)xar_numerus(st->actiones);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenActioIntroitus* ai =
            (SilvaGenActioIntroitus*)xar_obtinere(st->actiones, i);
        si (ai && ai->terminalis == terminalis)
        {
            SilvaGenActioIntroitus* copia =
                (SilvaGenActioIntroitus*)xar_addere(fructus);
            si (copia) *copia = *ai;
        }
    }

    redde fructus;
}

s32
silva_gen_goto_quaerere(
    SilvaGenTabula*  tabula,
    s32             status,
    s32             non_terminalis)
{
    SilvaGenStatusTabula* st;
    i32 i;
    i32 numerus;

    si (!tabula) redde -I;

    st = (SilvaGenStatusTabula*)xar_obtinere(
        tabula->status_tabulae, (i32)status);
    si (!st) redde -I;

    numerus = (i32)xar_numerus(st->goto_introitus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGenGotoIntroitus* gi =
            (SilvaGenGotoIntroitus*)xar_obtinere(st->goto_introitus, i);
        si (gi && gi->non_terminalis == non_terminalis)
        {
            redde gi->status_novus;
        }
    }

    redde -I;
}

vacuum
silva_gen_tabulam_imprimere(
    SilvaGenTabula*  tabula)
{
    SilvaGenGrammatica* grammatica;
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
        SilvaGenStatusTabula* st =
            (SilvaGenStatusTabula*)xar_obtinere(tabula->status_tabulae, s);
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
            SilvaGenActioIntroitus* ai =
                (SilvaGenActioIntroitus*)xar_obtinere(st->actiones, i);
            si (!ai) perge;

            imprimere("  ACTION(");
            si (ai->terminalis == (s32)SILVA_GEN_EOF_PROSPECTUS)
            {
                imprimere("$");
            }
            alioquin
            {
                SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
                casus SILVA_GEN_ACTIO_TRANSPONERE:
                    imprimere("SHIFT %d", (int)ai->valor);
                    frange;
                casus SILVA_GEN_ACTIO_REDUCERE:
                {
                    SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
                        grammatica->productiones, (i32)ai->valor);
                    si (prod)
                    {
                        SilvaGenSymbolum* lhs = (SilvaGenSymbolum*)xar_obtinere(
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
                casus SILVA_GEN_ACTIO_ACCIPERE:
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
            SilvaGenGotoIntroitus* gi =
                (SilvaGenGotoIntroitus*)xar_obtinere(st->goto_introitus, i);
            si (!gi) perge;

            {
                SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
silva_gen_conflictus_imprimere(
    SilvaGenTabula*  tabula)
{
    SilvaGenGrammatica* grammatica;
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
        SilvaGenStatusTabula* st =
            (SilvaGenStatusTabula*)xar_obtinere(tabula->status_tabulae, s);
        i32 i;
        i32 num_actiones;

        si (!st || !st->habet_conflictum) perge;

        imprimere("  Status %d:\n", (int)st->index);

        num_actiones = (i32)xar_numerus(st->actiones);
        per (i = ZEPHYRUM; i < num_actiones; i++)
        {
            SilvaGenActioIntroitus* ai =
                (SilvaGenActioIntroitus*)xar_obtinere(st->actiones, i);
            si (!ai || !ai->conflictus_intentus) perge;

            imprimere("    ");
            si (ai->terminalis == (s32)SILVA_GEN_EOF_PROSPECTUS)
            {
                imprimere("$");
            }
            alioquin
            {
                SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
                casus SILVA_GEN_ACTIO_TRANSPONERE:
                    imprimere("SHIFT %d", (int)ai->valor);
                    frange;
                casus SILVA_GEN_ACTIO_REDUCERE:
                    imprimere("REDUCE P%d", (int)ai->valor);
                    frange;
                casus SILVA_GEN_ACTIO_ACCIPERE:
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
silva_gen_collectio_imprimere(
    SilvaGenCollectio*   collectio)
{
    SilvaGenGrammatica*  grammatica;
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
        SilvaGenStatus* s = (SilvaGenStatus*)xar_obtinere(
            collectio->status_omnes, i);
        i32 j;

        si (!s) perge;

        imprimere("--- Status %d (%d res) ---\n",
            (int)s->index,
            (int)xar_numerus(s->res));

        per (j = ZEPHYRUM; j < (i32)xar_numerus(s->res); j++)
        {
            SilvaGenRes*       res = (SilvaGenRes*)xar_obtinere(s->res, j);
            SilvaGenProductio* prod;
            SilvaGenSymbolum*  sinistrum;
            i32               k;
            i32               num_dextrum;

            si (!res) perge;

            prod = (SilvaGenProductio*)xar_obtinere(
                grammatica->productiones, (i32)res->productio);
            si (!prod) perge;

            sinistrum = (SilvaGenSymbolum*)xar_obtinere(
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
                        SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
            si (res->prospectus == (s32)SILVA_GEN_EOF_PROSPECTUS)
            {
                imprimere(", $");
            }
            alioquin
            {
                SilvaGenSymbolum* look = (SilvaGenSymbolum*)xar_obtinere(
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
                SilvaGenTransitio* trans = (SilvaGenTransitio*)xar_obtinere(
                    collectio->transitiones, t);
                si (trans && trans->status == s->index)
                {
                    SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
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
