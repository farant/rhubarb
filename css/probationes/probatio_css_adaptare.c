/* probatio_css_adaptare.c - Adaptatio et ligatio triviorum
 *
 * PORTA PRINCIPALIS HIC EST TEGUMENTUM OCTETORUM (spec par. IX.7):
 * omnis octetus fontis EXACTE SEMEL apparet, sive in lexemate
 * significante sive in triviis eius. Non 'plerique', non 'summa
 * congruit' - tegumentum PER OCTETUM, offset post offset.
 *
 * Consilium eam portam T10 ponit, post parsatorem. Hic advenit
 * ANTE eum, quia iam agi POTEST: adaptatio et ligatio sunt ubi
 * octetus perdi potest, et parsator eas solum ADHIBET. Vitium hic
 * inventum vitium unius moduli est; idem post parsatorem inventum
 * inter duos moduli sederet.
 *
 * DUO REGIMINA, utrumque probatum (par. VI, D7):
 *   STRUCTURALE  spatia trivia sunt (inter regulas)
 *   CONTENTUM    spatia significantia (intra praeludia, valores)
 * Commentaria trivia sunt UTROQUE regimine - D7 SPATIA nominat,
 * non commentaria, et commentarium in strato quaestionum manere
 * debet ubicumque stat.
 */

#include "latina.h"
#include "credo.h"
#include "css_adaptare.h"
#include "css_lexicon.h"
#include "css_lexema.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

nomen enumeratio {
    REGIMEN_STRUCTURALE = 0,   /* spatia trivia */
    REGIMEN_CONTENTUM          /* spatia significantia (D7) */
} Regimen;


/* Lexare, adaptare, ligare. Seriem SIGNIFICANTIUM reddit (NIHIL
 * in fractura), et 'tokens' plana per punctatorem exitus. */
hic_manens Xar*
_parare (
                       Piscina* piscina,
    constans MateriaLexiconRatum* lexicon,
            constans character* fons,
                            i32  mensura,
                        Regimen  regimen,
                           Xar** plana_exitus)
{
    Xar* lexemata;
    Xar* plana;
    Xar* significantia;
    CssLigator ligator;
    i32 i;
    i32 numerus;

    lexemata = css_lexare(piscina, fons, mensura);
    si (lexemata == NIHIL) { redde NIHIL; }

    plana = css_adaptare(piscina, lexemata, &FORMA);
    si (plana == NIHIL) { redde NIHIL; }
    si (plana_exitus != NIHIL) { *plana_exitus = plana; }

    css_ligator_incipere(&ligator, piscina, lexicon);
    significantia = xar_creare(piscina, magnitudo(MateriaToken*));
    si (significantia == NIHIL) { redde NIHIL; }

    numerus = xar_numerus(plana);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        MateriaToken* token;
        b32 trivium_est;

        token = *(MateriaToken**)xar_obtinere(plana, i);
        trivium_est = materia_lexicon_trivium_est(lexicon, token->genus);

        /* REGIMEN CONTENTUM: spatia significantia fiunt, commentaria
         * trivia manent. Parsator id intra praeludia et valores agit. */
        si (regimen == REGIMEN_CONTENTUM
            && materia_lexicon_munus(lexicon, token->genus)
                   == MATERIA_MUNUS_SPATIUM)
        {
            trivium_est = FALSUM;
        }

        si (trivium_est)
        {
            si (!css_ligator_cumulare(&ligator, token)) { redde NIHIL; }
        }
        alioquin
        {
            MateriaToken** locus;
            si (!css_ligator_solvere(&ligator, token)) { redde NIHIL; }
            locus = (MateriaToken**)xar_addere(significantia);
            si (locus == NIHIL) { redde NIHIL; }
            *locus = token;
        }
    }

    redde significantia;
}

/* TEGUMENTUM: ante, valor, post - ordine fontis, offset post
 * offset, ab octeto ZEPHYRUM ad mensuram. */
hic_manens b32
_tegumentum_probare (
                       Piscina* piscina,
    constans MateriaLexiconRatum* lexicon,
            constans character* fons,
                            i32  mensura,
                        Regimen  regimen)
{
    Xar* significantia;
    s32  exspectatus = ZEPHYRUM;
    i32  i;
    i32  numerus;

    significantia = _parare(piscina, lexicon, fons, mensura, regimen,
        NIHIL);
    si (significantia == NIHIL) { redde FALSUM; }

    numerus = xar_numerus(significantia);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        MateriaToken* token;
        i32 j;

        token = *(MateriaToken**)xar_obtinere(significantia, i);

        per (j = ZEPHYRUM; j < token->numerus_ante; j++)
        {
            si (token->spatia_ante[j]->byte_offset != exspectatus)
            { redde FALSUM; }
            exspectatus += (s32)token->spatia_ante[j]->valor.mensura;
        }
        si (token->byte_offset != exspectatus) { redde FALSUM; }
        exspectatus += (s32)token->valor.mensura;
        per (j = ZEPHYRUM; j < token->numerus_post; j++)
        {
            si (token->spatia_post[j]->byte_offset != exspectatus)
            { redde FALSUM; }
            exspectatus += (s32)token->spatia_post[j]->valor.mensura;
        }
    }

    redde (b32)(exspectatus == (s32)mensura);
}

/* Plagulam ex disco legere; NIHIL si abest. */
hic_manens character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
    FILE* f;
    longus longitudo;
    character* memoria;
    size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL) { redde NIHIL; }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM) { fclose(f); redde NIHIL; }
    longitudo = ftell(f);
    si (longitudo < 0L) { fclose(f); redde NIHIL; }
    rewind(f);

    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo) { redde NIHIL; }

    *mensura = (i32)longitudo;
    redde memoria;
}


integer
principale (vacuum)
{
    Piscina* piscina;
    MateriaLexiconRatum ratum;
    MateriaLexIudicium iudicium;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_css_adaptare", 65536);
    credo_aperire(piscina);

    si (!materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON, &iudicium))
    {
        imprimere("LEXICON CSS RECUSATUM: %s\n",
            materia_lexicon_vitium_nomen((MateriaLexVitium)iudicium.vitium));
        redde I;
    }


    /* ========================================================
     * PROBARE: adaptatio UNUM PRO UNO
     * ======================================================== */
    {
        constans character* fons = "a { color : red }";
        i32 mensura = (i32)strlen(fons);
        Xar* lexemata;
        Xar* plana;
        i32 i;

        imprimere("\n--- Probans adaptationem unum-pro-uno ---\n");

        lexemata = css_lexare(piscina, fons, mensura);
        CREDO_NON_NIHIL (lexemata);
        plana = css_adaptare(piscina, lexemata, &FORMA);
        CREDO_NON_NIHIL (plana);

        /* INVARIATUM CENTRALE: nihil absorbetur, nihil omittitur.
         * Ex hoc 'dominus unus' STRUCTURALIS fit - octetus bis
         * converti non potest quia bis conversus non est. */
        CREDO_AEQUALIS_I32 (xar_numerus(plana), xar_numerus(lexemata));
        CREDO_MAIOR_I32 (xar_numerus(plana), (i32)X);

        per (i = ZEPHYRUM; i < xar_numerus(lexemata); i++)
        {
            constans CssLexema* lexema;
            MateriaToken* token;

            lexema = (constans CssLexema*)xar_obtinere(lexemata, i);
            token  = *(MateriaToken**)xar_obtinere(plana, i);

            CREDO_AEQUALIS_S32 (token->genus, (s32)lexema->genus);
            CREDO_AEQUALIS_S32 (token->byte_offset, (s32)lexema->offset);
            CREDO_AEQUALIS_I32 (token->linea, lexema->linea);
            CREDO_AEQUALIS_I32 (token->columna, lexema->columna);
            CREDO_AEQUALIS_I32 (token->valor.mensura,
                lexema->valor.mensura);
            /* Valor in FONTEM spectat - idem punctator, non copia */
            CREDO_AEQUALIS_PTR (token->valor.datum, lexema->valor.datum);
            /* fons unus (spec par. II) */
            CREDO_AEQUALIS_S32 (token->fons_index, ZEPHYRUM);

            /* VEXILLUM initium_lineae NUMQUAM PONITUR. Assertum
             * POSITIVE: CSS munus LINEA caret, ergo signum dicere
             * non potest, et si poneretur circuitum non superaret
             * (vide css_adaptare.c). 'Nemo id petivit' pro 'recte
             * agit' haberi non debet. */
            CREDO_FALSUM (materia_token_initium_lineae(token));
        }
    }


    /* ========================================================
     * PROBARE: regula divisionis, omnes rami
     * ======================================================== */
    {
        Xar* significantia;
        MateriaToken* t;

        imprimere("\n--- Probans divisionem: sine linea nova ---\n");
        {
            /* 'a { color : red }' uno in versu: nullum spatium
             * lineam novam fert, ergo OMNIA priori ut 'post'. */
            constans character* fons = "a { color : red }";
            significantia = _parare(piscina, &ratum, fons,
                (i32)strlen(fons), REGIMEN_STRUCTURALE, NIHIL);
            CREDO_NON_NIHIL (significantia);

            t = *(MateriaToken**)xar_obtinere(significantia, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (t->genus, (s32)CSS_LEX_IDENTIFICATOR);
            CREDO_AEQUALIS_S32 (t->byte_offset, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (t->numerus_post, I);   /* spatium post 'a' */
            CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);

            /* et sequens ergo NIHIL ante habet */
            t = *(MateriaToken**)xar_obtinere(significantia, I);
            CREDO_AEQUALIS_S32 (t->genus, (s32)CSS_LEX_BRACE_APERTA);
            CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);
        }

        imprimere("\n--- Probans divisionem: sine priore ---\n");
        {
            /* Trivia ducentia priorem non habent, ergo OMNIA
             * sequenti ut 'ante'. */
            constans character* fons = "\n\n  a{}";
            significantia = _parare(piscina, &ratum, fons,
                (i32)strlen(fons), REGIMEN_STRUCTURALE, NIHIL);
            CREDO_NON_NIHIL (significantia);

            t = *(MateriaToken**)xar_obtinere(significantia, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (t->genus, (s32)CSS_LEX_IDENTIFICATOR);
            CREDO_AEQUALIS_I32 (t->numerus_ante, I);
            CREDO_AEQUALIS_I32 (t->numerus_post, ZEPHYRUM);
        }

        imprimere("\n--- Probans divisionem: scissio vera ---\n");
        {
            /* 'a{} /' '* n *' '/\n  b{}' - pendentia TRIA:
             * SPATIA(" "), COMMENTARIUM, SPATIA("\n  ").
             * Divisor primum SPATIUM lineam novam ferens = index II.
             * Ergo duo priori, unum sequenti - commentarium apud
             * lineam SUAM manet, indentatio apud lexema SUUM. */
            constans character* fons = "a{} /* n */\n  b{}";
            significantia = _parare(piscina, &ratum, fons,
                (i32)strlen(fons), REGIMEN_STRUCTURALE, NIHIL);
            CREDO_NON_NIHIL (significantia);

            /* significantia: a { } b { } FINIS */
            CREDO_AEQUALIS_I32 (xar_numerus(significantia), VII);

            t = *(MateriaToken**)xar_obtinere(significantia, II);
            CREDO_AEQUALIS_S32 (t->genus, (s32)CSS_LEX_BRACE_CLAUSA);
            CREDO_AEQUALIS_I32 (t->numerus_post, II);
            CREDO_AEQUALIS_S32 (t->spatia_post[I]->genus,
                (s32)CSS_LEX_COMMENTARIUM);

            t = *(MateriaToken**)xar_obtinere(significantia, III);
            CREDO_AEQUALIS_S32 (t->genus, (s32)CSS_LEX_IDENTIFICATOR);
            CREDO_AEQUALIS_I32 (t->numerus_ante, I);
        }

        imprimere("\n--- Probans divisionem: commentarium multilineum NON divisor ---\n");
        {
            /* Silvae COMMENTUM_CLAUSUM lexema NOVA_LINEA non est,
             * ergo commentarium multilineum divisionem non facit et
             * apud lexema PRAECEDENS manet. Nos idem: divisor munus
             * SPATIUM ferre DEBET. Sine hac condicione commentarium
             * ad 'b' migraret - lectio FALSA, et octetim invisibilis. */
            constans character* fons = "a{} /* n\np */ b{}";
            significantia = _parare(piscina, &ratum, fons,
                (i32)strlen(fons), REGIMEN_STRUCTURALE, NIHIL);
            CREDO_NON_NIHIL (significantia);

            t = *(MateriaToken**)xar_obtinere(significantia, II);
            CREDO_AEQUALIS_S32 (t->genus, (s32)CSS_LEX_BRACE_CLAUSA);
            CREDO_AEQUALIS_I32 (t->numerus_post, III);

            t = *(MateriaToken**)xar_obtinere(significantia, III);
            CREDO_AEQUALIS_S32 (t->genus, (s32)CSS_LEX_IDENTIFICATOR);
            CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);
        }

        imprimere("\n--- Probans trivia trans SEGMENTA xar ---\n");
        {
            /* Xar SEGMENTATUS est, primo segmento XVI elementa
             * (xar.h:26). Series triviorum LONGIOR quam XVI ergo
             * segmenta TRANSIT, et xar_obtinere ut initium seriei
             * contiguae tractare memoriam ALIENAM legeret. Casus
             * hic ligatorem trans limitem illum trahit - sine eo
             * copia plana defensiva sola esset, non probata. */
            constans character* fons =
                "a /*a*/ /*b*/ /*c*/ /*d*/ /*e*/ /*f*/ /*g*/ /*h*/"
                " /*i*/ /*j*/ /*k*/ b{}";
            significantia = _parare(piscina, &ratum, fons,
                (i32)strlen(fons), REGIMEN_STRUCTURALE, NIHIL);
            CREDO_NON_NIHIL (significantia);

            /* nulla linea nova: OMNIA priori. XI commentaria +
             * XII spatia = XXIII trivia, super XVI segmenti primi. */
            t = *(MateriaToken**)xar_obtinere(significantia, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (t->genus, (s32)CSS_LEX_IDENTIFICATOR);
            CREDO_AEQUALIS_I32 (t->numerus_post, XXIII);
            CREDO_MAIOR_I32 (t->numerus_post, (i32)XVI);
            /* ultimum trivium legibile esse DEBET - hic copia
             * plana fallax memoriam alienam redderet */
            CREDO_AEQUALIS_S32 (t->spatia_post[XXII]->genus,
                (s32)CSS_LEX_SPATIA);
            CREDO_AEQUALIS_S32 (t->spatia_post[XXI]->genus,
                (s32)CSS_LEX_COMMENTARIUM);
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, fons,
                (i32)strlen(fons), REGIMEN_STRUCTURALE));
        }

        imprimere("\n--- Probans contractum: Xar CONTIGUUS NON EST ---\n");
        {
            /* CASUS SUPRA VITIUM PLANTATUM NON CEPIT, et causa
             * MENSURATA est: piscina segmentum secundum IMMEDIATE
             * post primum ponit (delta = CXXVIII = XVI * VIII),
             * ergo lectio ultra limitem in segmentum secundum
             * CASU cadit. Fortuna, non contractus - eadem forma
             * quam materia_token.h de ordinatione notat.
             *
             * Hic contractum ipsum probamus: inter cumulationes
             * ALIUD ex eadem piscina allocamus, ut segmenta
             * DISIUNGANTUR. Tunc series plana LOCUM TENET, et
             * vitium plantatum ardet. */
            constans character* fons =
                "a /*a*/ /*b*/ /*c*/ /*d*/ /*e*/ /*f*/ /*g*/ /*h*/"
                " /*i*/ /*j*/ /*k*/ b{}";
            Xar* plana;
            CssLigator ligator;
            MateriaToken* ultimum_trivium = NIHIL;
            MateriaToken* significans = NIHIL;
            i32 i;

            plana = css_adaptare(piscina,
                css_lexare(piscina, fons, (i32)strlen(fons)), &FORMA);
            CREDO_NON_NIHIL (plana);

            css_ligator_incipere(&ligator, piscina, &ratum);
            per (i = I; i < xar_numerus(plana); i++)
            {
                MateriaToken* token;
                token = *(MateriaToken**)xar_obtinere(plana, i);
                si (materia_lexicon_trivium_est(&ratum, token->genus))
                {
                    CREDO_VERUM (css_ligator_cumulare(&ligator, token));
                    ultimum_trivium = token;
                    /* DISIUNCTOR: allocatio aliena inter elementa */
                    (vacuum)piscina_allocare(piscina, (memoriae_index)LXIV);
                }
                alioquin { significans = token; frange; }
            }
            CREDO_NON_NIHIL (significans);
            CREDO_NON_NIHIL (ultimum_trivium);

            /* prior NIHIL est (a primo trivio incepimus), ergo OMNIA
             * sequenti ut 'ante' - et ultimum eorum idem OBIECTUM
             * esse debet quod cumulavimus. Lectio ultra segmentum
             * punctatorem ALIENUM redderet. */
            CREDO_VERUM (css_ligator_solvere(&ligator, significans));
            CREDO_AEQUALIS_I32 (significans->numerus_ante, XXIII);
            CREDO_AEQUALIS_PTR (
                significans->spatia_ante[significans->numerus_ante - I],
                ultimum_trivium);
        }

        imprimere("\n--- Probans regimen CONTENTUM (D7) ---\n");
        {
            /* Idem fons, regimine contenti: spatia significantia
             * fiunt, ergo in serie apparent et trivia nulla sunt.
             * Haec est via quam parsator intra praeludia adhibet. */
            constans character* fons = "1px 2px";
            Xar* structurale;
            Xar* contentum;

            structurale = _parare(piscina, &ratum, fons,
                (i32)strlen(fons), REGIMEN_STRUCTURALE, NIHIL);
            contentum = _parare(piscina, &ratum, fons,
                (i32)strlen(fons), REGIMEN_CONTENTUM, NIHIL);
            CREDO_NON_NIHIL (structurale);
            CREDO_NON_NIHIL (contentum);

            /* structurale: 1px 2px FINIS = III (spatium absorptum) */
            CREDO_AEQUALIS_I32 (xar_numerus(structurale), III);
            /* contentum: 1px SPATIUM 2px FINIS = IV */
            CREDO_AEQUALIS_I32 (xar_numerus(contentum), IV);

            t = *(MateriaToken**)xar_obtinere(contentum, I);
            CREDO_AEQUALIS_S32 (t->genus, (s32)CSS_LEX_SPATIA);
            /* et NIHIL absorbuit - spatium se ipsum est */
            CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (t->numerus_post, ZEPHYRUM);
        }
    }


    /* ========================================================
     * PORTA: TEGUMENTUM OCTETORUM (spec par. IX.7)
     * ======================================================== */
    {
        hic_manens constans character* CASUS[] = {
            "",
            " ",
            "\n\n  ",
            "a{}",
            "a { color : red }",
            "a{} /* n */\n  b{}",
            "a{} /* n\np */ b{}",
            "@media screen{a{color:red}}",
            "a{margin:1px 2px !important}",
            "}a{}",
            "a{;;color:red}",
            "a{",
            "a{b:\"interminata",
            "a{b:url(x.png)}",
            "/*",
            "a\r\nb{}",
            "a{}\f b{}"
        };
        i32 numerus_casuum = (i32)(magnitudo(CASUS) / magnitudo(CASUS[0]));
        i32 i;
        i32 probata = ZEPHYRUM;

        imprimere("\n--- PORTA: tegumentum octetorum, casus inlinei ---\n");

        per (i = ZEPHYRUM; i < numerus_casuum; i++)
        {
            i32 mensura = (i32)strlen(CASUS[i]);
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, CASUS[i],
                mensura, REGIMEN_STRUCTURALE));
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, CASUS[i],
                mensura, REGIMEN_CONTENTUM));
            probata = probata + I;
        }

        /* NUL inline - in probatione vivit, non in fixura (par. IX.6) */
        {
            hic_manens constans character NUL_CASUS[] =
                "a{b:\0c}";
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, NUL_CASUS,
                (i32)VII, REGIMEN_STRUCTURALE));
            probata = probata + I;
        }

        /* Custodia vacuitatis: ansa currisse DEBET. 'N/N mundum'
         * significare potest 'numquam apparuit'. */
        CREDO_MAIOR_S32 ((s32)probata, (s32)X);
    }


    /* ========================================================
     * PORTA: TEGUMENTUM super CORPUS VERUM
     * ======================================================== */
    {
        hic_manens constans character* PLAGULAE[] = {
            "probationes/fixa/css/componentia_2026-08-18.css",
            "probationes/fixa/css/thema_2026-08-18.css",
            "probationes/fixa/css/adversarius.css",
            "lib/speculum_assets/speculum.css",
            "knotapel/atlas/web/style.css"
        };
        i32 numerus_plagularum =
            (i32)(magnitudo(PLAGULAE) / magnitudo(PLAGULAE[0]));
        constans character* radix;
        i32 i;
        i32 lectae = ZEPHYRUM;

        imprimere("\n--- PORTA: tegumentum super corpus verum ---\n");

        radix = getenv("RHUBARB_RADIX");
        si (radix == NIHIL) { radix = "."; }

        per (i = ZEPHYRUM; i < numerus_plagularum; i++)
        {
            character via[DXII];
            character* textus;
            i32 mensura = ZEPHYRUM;

            sprintf(via, "%s/%s", radix, PLAGULAE[i]);
            textus = _plagulam_legere(piscina, via, &mensura);
            si (textus == NIHIL)
            {
                imprimere("  OMISSA (abest): %s\n", PLAGULAE[i]);
                perge;
            }
            imprimere("  %s (%d octeti)\n", PLAGULAE[i], (integer)mensura);
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, textus,
                mensura, REGIMEN_STRUCTURALE));
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, textus,
                mensura, REGIMEN_CONTENTUM));
            lectae = lectae + I;
        }

        /* Porta quae CORPUS SUUM non mensurat 'N/N mundum' dicere
         * potest cum nihil legerit. */
        CREDO_MAIOR_S32 ((s32)lectae, (s32)II);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
