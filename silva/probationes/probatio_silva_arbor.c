/* probatio_silva_arbor.c - stratum vocabularii dialecti 'arbor' (T2)
 *
 * Quattuor artificia probantur: sigillum registri, quaesitiones
 * nominum, tabula orthographiae, mangulatio tagorum.
 *
 * PORTA CENTRALIS hic vivit (§3 spec): tabula orthographiae fons
 * veritatis SECUNDUS est - silva tabulam inversam genus->littera
 * nusquam habet, ergo arbor eam AUCTORAVIT et divergere potest.
 * Porta: orthographia CUIUSQUE introitus lexatur et genus redditum
 * exspectatur. Praeterea genera quae orthographiam habere DEBENT
 * per fines enumerationis nominantur - aliter tabula perturbata
 * tacite 'orthographiam variam' redderet et porta nihil videret.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_tabulae_sceleti.h"
#include "silva_parsare.h"
#include "silva_quaestio.h"
#include "silva_c89_oraculum.h"
#include "silva_arbor.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


/* Primum nodum selectori congruentem; NIHIL si nullum */
interior constans SilvaNodus*
_primus_congruens (
                  Piscina* piscina,
    constans SilvaParsura* parsura,
       constans character* selector)
{
         constans character* causa = NIHIL;
              SilvaQuaestio* quaestio;
                        Xar* resultata;
    SilvaQuaestioResultatum* primum;

    quaestio = silva_quaestio_compilare(piscina, &SILVA_C89_REGISTRUM,
        selector, &causa);
    si (quaestio == NIHIL)
    {
        redde NIHIL;
    }
    resultata = silva_quaestio_exsequi(quaestio,
        parsura->commissio->radix, piscina);
    si (resultata == NIHIL || xar_numerus(resultata) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    primum = (SilvaQuaestioResultatum*)xar_obtinere(resultata, ZEPHYRUM);
    redde primum ? primum->nodus : NIHIL;
}

/* Quotiens acus in feno appareat */
interior i32
_quotiens (
                 chorda  fenum,
     constans character* acus)
{
    i32 numerus;
    i32 i;
    i32 longitudo;

    numerus    = ZEPHYRUM;
    longitudo  = (i32)strlen(acus);
    si (longitudo == ZEPHYRUM || fenum.mensura < longitudo)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i <= fenum.mensura - longitudo; i++)
    {
        si (memcmp(fenum.datum + i, acus, (size_t)longitudo) == ZEPHYRUM)
        {
            numerus++;
        }
    }
    redde numerus;
}

/* Sigillum registri synthetici - series mutabiles, ut sensibilitas
 * per campum probari possit */
interior chorda
_sigillum_synthetici (
             Piscina* piscina,
       SilvaTabGenus* genera,
                 i32  numerus_generum,
       SilvaTabLocus* loci,
                 i32  numerus_locorum)
{
    /* NB 'registrum' identificator esse NON potest - macro latinae
     * pro 'register'. Domus 'tabularium' adhibet. */
    SilvaRegistrumCoctum tabularium;

    tabularium.genera           = genera;
    tabularium.numerus_generum  = numerus_generum;
    tabularium.loci             = loci;
    tabularium.numerus_locorum  = numerus_locorum;

    redde silva_arbor_sigillum(piscina, &tabularium);
}

/* Genus lexematis UNICI in littera data; SILVA_LEX_NUMERUS_GENERUM
 * si lexemata non-EOF numero uno non sunt (orthographia quae in duo
 * lexemata frangitur porta capienda est, non tacite accipienda) */
interior SilvaLexemaGenus
_genus_lexatum (
               Piscina* piscina,
    constans character* littera)
{
                 Xar* cruda;
                 i32  numerus;
                 i32  i;
                 i32  reperta;
    SilvaLexemaGenus  inventum;

    reperta   = ZEPHYRUM;
    inventum  = SILVA_LEX_NUMERUS_GENERUM;

    cruda = silva_lexare_cruda(piscina, littera,
        (i32)strlen(littera), ZEPHYRUM);
    si (cruda == NIHIL)
    {
        redde SILVA_LEX_NUMERUS_GENERUM;
    }

    numerus = xar_numerus(cruda);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaToken* lexema = *(SilvaToken**)xar_obtinere(cruda, i);

        si (lexema == NIHIL || lexema->genus == SILVA_LEX_EOF)
        {
            perge;
        }
        inventum = lexema->genus;
        reperta++;
    }

    si (reperta != I)
    {
        redde SILVA_LEX_NUMERUS_GENERUM;
    }
    redde inventum;
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_arbor", 4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: sigillum registri
     * ======================================================== */

    {
        chorda c89_primum;
        chorda c89_iterum;
        chorda sceletum;
           i32 i;

        imprimere("\n--- Probans sigillum registri ---\n");

        c89_primum = silva_arbor_sigillum(piscina, &SILVA_C89_REGISTRUM);
        c89_iterum = silva_arbor_sigillum(piscina, &SILVA_C89_REGISTRUM);

        /* forma: VIII characteres, hexadecimales minusculi */
        CREDO_AEQUALIS_I32 (c89_primum.mensura,
            (i32)SILVA_ARBOR_SIGILLI_LONGITUDO);
        per (i = ZEPHYRUM; i < c89_primum.mensura; i++)
        {
            character c = (character)c89_primum.datum[i];

            CREDO_VERUM ((c >= '0' && c <= '9')
                || (c >= 'a' && c <= 'f'));
        }

        /* idem registrum -> idem sigillum */
        CREDO_CHORDA_AEQUALIS (c89_primum, c89_iterum);

        /* grammatica alia -> sigillum aliud */
        sceletum = silva_arbor_sigillum(piscina,
            &SILVA_SCELETUM_REGISTRUM);
        CREDO_AEQUALIS_I32 (sceletum.mensura,
            (i32)SILVA_ARBOR_SIGILLI_LONGITUDO);
        CREDO_FALSUM (chorda_aequalis(c89_primum, sceletum));

        /* argumenta NIHIL -> chorda vacua, non fractura */
        CREDO_AEQUALIS_I32 (
            silva_arbor_sigillum(NIHIL, &SILVA_C89_REGISTRUM).mensura,
            (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (
            silva_arbor_sigillum(piscina, NIHIL).mensura,
            (i32)ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: sensibilitas sigilli per CAMPUM QUEMQUE
     *
     * Registrum syntheticum minimum; campus unus mutatur, sigillum
     * mutari DEBET. Sigillum quod campum non videt custodia mortua
     * est - id est classis quam his probationibus quaerimus.
     * ======================================================== */

    {
        SilvaTabLocus loci[3];
        SilvaTabGenus genera[2];
               chorda basis;
               chorda mutatum;

        imprimere("\n--- Probans sensibilitatem sigilli ---\n");

        loci[0].titulus = "alpha";
        loci[0].species = SILVA_LOCUS_NODUS;
        loci[1].titulus = "beta";
        loci[1].species = SILVA_LOCUS_TOKEN;
        loci[2].titulus = "gamma";
        loci[2].species = SILVA_LOCUS_LISTA_NODUS;

        genera[0].titulus       = "primum";
        genera[0].loci_offset   = ZEPHYRUM;
        genera[0].loci_numerus  = II;
        genera[1].titulus       = "secundum";
        genera[1].loci_offset   = II;
        genera[1].loci_numerus  = I;

        basis = _sigillum_synthetici(piscina, genera, II, loci, III);
        CREDO_AEQUALIS_I32 (basis.mensura,
            (i32)SILVA_ARBOR_SIGILLI_LONGITUDO);

        /* i. titulus generis */
        genera[0].titulus = "primus";
        mutatum = _sigillum_synthetici(piscina, genera, II, loci, III);
        CREDO_FALSUM (chorda_aequalis(basis, mutatum));
        genera[0].titulus = "primum";

        /* ii. loci_offset */
        genera[1].loci_offset = I;
        mutatum = _sigillum_synthetici(piscina, genera, II, loci, III);
        CREDO_FALSUM (chorda_aequalis(basis, mutatum));
        genera[1].loci_offset = II;

        /* iii. loci_numerus */
        genera[0].loci_numerus = I;
        mutatum = _sigillum_synthetici(piscina, genera, II, loci, III);
        CREDO_FALSUM (chorda_aequalis(basis, mutatum));
        genera[0].loci_numerus = II;

        /* iv. titulus loci */
        loci[1].titulus = "beeta";
        mutatum = _sigillum_synthetici(piscina, genera, II, loci, III);
        CREDO_FALSUM (chorda_aequalis(basis, mutatum));
        loci[1].titulus = "beta";

        /* v. species loci */
        loci[1].species = SILVA_LOCUS_NODUS;
        mutatum = _sigillum_synthetici(piscina, genera, II, loci, III);
        CREDO_FALSUM (chorda_aequalis(basis, mutatum));
        loci[1].species = SILVA_LOCUS_TOKEN;

        /* vi. restitutio -> sigillum basis redit (mutationes supra
         * vere restitutae sunt, non tantum aliae) */
        mutatum = _sigillum_synthetici(piscina, genera, II, loci, III);
        CREDO_CHORDA_AEQUALIS (basis, mutatum);

        /* vii. separator: 'ab'+'c' et 'a'+'bc' materiam eandem
         * dare NON debent */
        loci[0].titulus = "alphab";
        loci[1].titulus = "eta";
        mutatum = _sigillum_synthetici(piscina, genera, II, loci, III);
        CREDO_FALSUM (chorda_aequalis(basis, mutatum));
        loci[0].titulus = "alpha";
        loci[1].titulus = "beta";
    }


    /* ========================================================
     * PROBARE: quaesitiones nominum registri
     * ======================================================== */

    {
        s32 genus_corpus;
        s32 genus_definitio;
        s32 locus;

        imprimere("\n--- Probans quaesitiones nominum ---\n");

        genus_definitio = silva_arbor_genus_index(&SILVA_C89_REGISTRUM,
            "definitio-functionis",
            (i32)strlen("definitio-functionis"));
        CREDO_AEQUALIS_S32 (genus_definitio,
            (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS);

        genus_corpus = silva_arbor_genus_index(&SILVA_C89_REGISTRUM,
            "corpus", (i32)strlen("corpus"));
        CREDO_AEQUALIS_S32 (genus_corpus,
            (s32)SILVA_C89_GENUS_CORPUS);

        /* ignotum */
        CREDO_AEQUALIS_S32 (silva_arbor_genus_index(
            &SILVA_C89_REGISTRUM, "genus-fictum",
            (i32)strlen("genus-fictum")), -I);

        /* praefixum NON congruit (mensura vera confertur, non
         * praefixum solum) */
        CREDO_AEQUALIS_S32 (silva_arbor_genus_index(
            &SILVA_C89_REGISTRUM, "corp", (i32)strlen("corp")), -I);

        /* argumenta NIHIL */
        CREDO_AEQUALIS_S32 (silva_arbor_genus_index(NIHIL, "corpus",
            (i32)strlen("corpus")), -I);
        CREDO_AEQUALIS_S32 (silva_arbor_genus_index(
            &SILVA_C89_REGISTRUM, NIHIL, ZEPHYRUM), -I);

        /* locus intra genus: 'corpus' locus definitionis-functionis */
        locus = silva_arbor_locus_index(&SILVA_C89_REGISTRUM,
            genus_definitio, "corpus", (i32)strlen("corpus"));
        CREDO_VERUM (locus >= ZEPHYRUM);
        CREDO_AEQUALIS_S32 (
            SILVA_C89_REGISTRUM.loci[locus].species,
            (s32)SILVA_LOCUS_NODUS);

        /* SCOPUS GENERIS - hoc est cor rei. 'corpus' locus VERUS est
         * definitionis-functionis sed NON generis 'corpus' (cuius
         * loci tok_aperta/elementa/tok_clausa sunt). Nomina locorum
         * non globaliter unica sunt: quaesitio quae genus ignoret
         * hic mentiretur. */
        CREDO_AEQUALIS_S32 (silva_arbor_locus_index(
            &SILVA_C89_REGISTRUM, genus_corpus, "corpus",
            (i32)strlen("corpus")), -I);

        /* locus verus generis 'corpus' */
        locus = silva_arbor_locus_index(&SILVA_C89_REGISTRUM,
            genus_corpus, "elementa", (i32)strlen("elementa"));
        CREDO_VERUM (locus >= ZEPHYRUM);
        CREDO_AEQUALIS_S32 (
            SILVA_C89_REGISTRUM.loci[locus].species,
            (s32)SILVA_LOCUS_LISTA_MIXTA);

        /* genus extra fines */
        CREDO_AEQUALIS_S32 (silva_arbor_locus_index(
            &SILVA_C89_REGISTRUM, -I, "corpus",
            (i32)strlen("corpus")), -I);
        CREDO_AEQUALIS_S32 (silva_arbor_locus_index(
            &SILVA_C89_REGISTRUM, (s32)9999, "corpus",
            (i32)strlen("corpus")), -I);
    }


    /* ========================================================
     * PROBARE: tabula orthographiae - PORTA
     *
     * Fines enumerationis nominantur EXPLICITE ut tabula perturbata
     * clamet. Sine his, tabula male ordinata NIHIL redderet et ansa
     * lexans introitum simpliciter praeteriret - custodia mortua.
     * ======================================================== */

    {
        i32 genus;
        i32 fixae;

        imprimere("\n--- Probans tabulam orthographiae (PORTA) ---\n");

        /* Canaria: genus lexematis NOVUM hanc probationem frangat,
         * ut quis id classificet (orthographia fixa an varia?).
         * Si hic frangitur, tabulam in silva_arbor.c extende. */
        CREDO_AEQUALIS_I32 ((i32)SILVA_LEX_NUMERUS_GENERUM, (i32)95);

        fixae = ZEPHYRUM;
        per (genus = ZEPHYRUM;
             genus < (i32)SILVA_LEX_NUMERUS_GENERUM;
             genus++)
        {
              SilvaLexemaGenus  g = (SilvaLexemaGenus)genus;
            constans character* orthographia;
                           b32  debet;

            orthographia = silva_arbor_orthographia(g);

            debet = ((genus >= (i32)SILVA_LEX_AUTO
                      && genus <= (i32)SILVA_LEX_WHILE)
                  || (genus >= (i32)SILVA_LEX_QUADRA_APERTA
                      && genus <= (i32)SILVA_LEX_CANCELLUM_CANCELLUM))
                ? VERUM : FALSUM;

            si (debet)
            {
                /* i. orthographia adesse DEBET */
                CREDO_NON_NIHIL (orthographia);
                si (orthographia == NIHIL)
                {
                    imprimere("  DEEST: %s\n",
                        silva_lexema_genus_nomen(g));
                    perge;
                }

                /* ii. PORTA: lexator eam relegat et genus reddat */
                si (_genus_lexatum(piscina, orthographia) != g)
                {
                    imprimere("  DIVERGENTIA: %s -> '%s' -> %s\n",
                        silva_lexema_genus_nomen(g), orthographia,
                        silva_lexema_genus_nomen(
                            _genus_lexatum(piscina, orthographia)));
                }
                CREDO_VERUM (
                    _genus_lexatum(piscina, orthographia) == g);

                /* iii. valor NON portandus (orthographia fixa) */
                CREDO_FALSUM (silva_arbor_valor_portandus(g));
                fixae++;
            }
            alioquin
            {
                /* orthographia VARIA - valor portandus, praeter EOF */
                CREDO_NIHIL (orthographia);
                si (g == SILVA_LEX_EOF)
                {
                    CREDO_FALSUM (silva_arbor_valor_portandus(g));
                }
                alioquin
                {
                    CREDO_VERUM (silva_arbor_valor_portandus(g));
                }
            }
        }

        /* XXXII verba clausa + XLVIII interpunctiones */
        CREDO_AEQUALIS_I32 (fixae, (i32)80);

        /* extra fines -> NIHIL, non fractura */
        CREDO_NIHIL (silva_arbor_orthographia(
            SILVA_LEX_NUMERUS_GENERUM));
        CREDO_FALSUM (silva_arbor_valor_portandus(
            SILVA_LEX_NUMERUS_GENERUM));

        /* exemplaria nominata (documentatio quae currit) */
        CREDO_VERUM (strcmp(silva_arbor_orthographia(
            SILVA_LEX_FLOAT_KW), "float") == ZEPHYRUM);
        CREDO_NIHIL (silva_arbor_orthographia(SILVA_LEX_FLOAT));
        CREDO_VERUM (strcmp(silva_arbor_orthographia(
            SILVA_LEX_SAGITTA), "->") == ZEPHYRUM);
        CREDO_VERUM (strcmp(silva_arbor_orthographia(
            SILVA_LEX_SINISTRORSUM_ASSIGNATIO), "<<=") == ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: mangulatio tagorum lexematum
     * ======================================================== */

    {
        character buffer[SILVA_ARBOR_TAG_CAPACITAS];
        character angustus[8];
              i32 longitudo;
              i32 genus;
              i32 longissimum;

        imprimere("\n--- Probans mangulationem tagorum ---\n");

        longitudo = silva_arbor_lexema_tag(SILVA_LEX_IDENTIFICATOR,
            buffer, (i32)SILVA_ARBOR_TAG_CAPACITAS);
        CREDO_VERUM (longitudo > ZEPHYRUM);
        CREDO_VERUM (strcmp(buffer, "lex-identificator") == ZEPHYRUM);

        /* '_' -> '-' */
        longitudo = silva_arbor_lexema_tag(SILVA_LEX_PAREN_APERTA,
            buffer, (i32)SILVA_ARBOR_TAG_CAPACITAS);
        CREDO_VERUM (strcmp(buffer, "lex-paren-aperta") == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (longitudo,
            (i32)strlen("lex-paren-aperta"));

        /* longissimum - custos capacitatis */
        longitudo = silva_arbor_lexema_tag(
            SILVA_LEX_SINISTRORSUM_ASSIGNATIO, buffer,
            (i32)SILVA_ARBOR_TAG_CAPACITAS);
        CREDO_VERUM (strcmp(buffer,
            "lex-sinistrorsum-assignatio") == ZEPHYRUM);

        /* COLLISIO MENSURATA (spec §0): 'assignatio' est et genus
         * nodi XXXV et genus lexematis. Praefixum ea separat. */
        longitudo = silva_arbor_lexema_tag(SILVA_LEX_ASSIGNATIO,
            buffer, (i32)SILVA_ARBOR_TAG_CAPACITAS);
        CREDO_VERUM (strcmp(buffer, "lex-assignatio") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (silva_arbor_genus_index(
            &SILVA_C89_REGISTRUM, "assignatio",
            (i32)strlen("assignatio")),
            (s32)SILVA_C89_GENUS_ASSIGNATIO);
        /* tag lexematis genus nodi NON est */
        CREDO_AEQUALIS_S32 (silva_arbor_genus_index(
            &SILVA_C89_REGISTRUM, "lex-assignatio",
            (i32)strlen("lex-assignatio")), -I);
        /* et genus nodi tag lexematis NON est */
        CREDO_VERUM (silva_arbor_lexema_ex_tag("assignatio",
            (i32)strlen("assignatio"))
            == SILVA_LEX_NUMERUS_GENERUM);

        /* ITER ROTUNDUM per genera OMNIA + custodia capacitatis */
        longissimum = ZEPHYRUM;
        per (genus = ZEPHYRUM;
             genus < (i32)SILVA_LEX_NUMERUS_GENERUM;
             genus++)
        {
            SilvaLexemaGenus g = (SilvaLexemaGenus)genus;

            longitudo = silva_arbor_lexema_tag(g, buffer,
                (i32)SILVA_ARBOR_TAG_CAPACITAS);
            CREDO_VERUM (longitudo > ZEPHYRUM);
            si (longitudo > longissimum)
            {
                longissimum = longitudo;
            }
            CREDO_VERUM (silva_arbor_lexema_ex_tag(buffer, longitudo)
                == g);
        }
        /* capacitas declarata vere sufficit (+ terminator) */
        CREDO_VERUM (longissimum + I
            <= (i32)SILVA_ARBOR_TAG_CAPACITAS);

        /* capacitas angusta -> ZEPHYRUM, non superfluxus */
        CREDO_AEQUALIS_I32 (silva_arbor_lexema_tag(
            SILVA_LEX_SINISTRORSUM_ASSIGNATIO, angustus,
            (i32)magnitudo(angustus)), (i32)ZEPHYRUM);

        /* argumenta mala */
        CREDO_AEQUALIS_I32 (silva_arbor_lexema_tag(
            SILVA_LEX_IDENTIFICATOR, NIHIL,
            (i32)SILVA_ARBOR_TAG_CAPACITAS), (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (silva_arbor_lexema_tag(
            SILVA_LEX_NUMERUS_GENERUM, buffer,
            (i32)SILVA_ARBOR_TAG_CAPACITAS), (i32)ZEPHYRUM);

        /* tag ignotum */
        CREDO_VERUM (silva_arbor_lexema_ex_tag("lex-fictum",
            (i32)strlen("lex-fictum")) == SILVA_LEX_NUMERUS_GENERUM);
        CREDO_VERUM (silva_arbor_lexema_ex_tag(NIHIL, ZEPHYRUM)
            == SILVA_LEX_NUMERUS_GENERUM);
    }


    /* ========================================================
     * PROBARE: scriptor - exhibitum I ('int n = 0;', purus)
     * ======================================================== */

    {
         constans character* fons = "int n = 0;\n";
               SilvaParsura* parsura;
        constans SilvaNodus* declaratio;
        SilvaArborScriptura  scriptura;

        imprimere("\n--- Probans scriptorem (exhibitum I) ---\n");

        parsura = silva_c89_parsare(piscina, "exhibitum.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        declaratio = _primus_congruens(piscina, parsura, "declaratio");
        CREDO_NON_NIHIL (declaratio);

        scriptura = silva_arbor_scribere_nodum(piscina, declaratio,
            &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
        si (!scriptura.successus)
        {
            imprimere("  FRACTA: %s\n",
                scriptura.causa ? scriptura.causa : "(sine causa)");
        }
        CREDO_VERUM (scriptura.successus);
        CREDO_VERUM (scriptura.textus.mensura > ZEPHYRUM);

        /* involucrum: grammatica + sigillum + ancora */
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<arbor grammatica=\"c89\"") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "registrum-sigillum=\"") == I);

        /* genus = tag, locus = involucrum */
        CREDO_VERUM (_quotiens(scriptura.textus, "<declaratio>") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<specificatores>") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<typus-primitivus>") == I);

        /* lexema orthographiae FIXAE: tag solus, NULLUS textus
         * (orthographia in genere ipso vivit). Clausurae tacitae
         * '</>' sunt - vide sanationem T0 */
        CREDO_VERUM (_quotiens(scriptura.textus, "<lex-int>") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<lex-semicolon>") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<lex-assignatio>") == I);

        /* lexema orthographiae VARIAE: valor ut TEXTUS */
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<lex-integer>0</>") == I);

        /* CONTENTUM MIXTUM (textus IUXTA elementa): forma quam
         * sanatio T0 possibilem fecit. Ante eam scriptor pulcher
         * lineas novas circa 'n' iniciebat et valorem CORRUMPEBAT,
         * cumulatim per cursum. Haec assertio illam sanationem
         * in situ vero custodit. */
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<lex-identificator>n<post><lex-spatia n=\"1\"/></post>")
            == I);

        /* trivia per LENTEM compactam - non ut textus (qui a
         * scriptore pulchro tacite praeteriretur) */
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<lex-spatia n=\"1\"/>") >= I);
        CREDO_VERUM (_quotiens(scriptura.textus, "<post>") >= I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<lex-nova-linea/>") == I);

        /* POSITIONES NON SCRIPTAE: nullum b=/linea=/columna= praeter
         * ancoram involucri (quae semel apparet) */
        CREDO_AEQUALIS_S32 ((s32)_quotiens(scriptura.textus, " b=\""),
            (s32)I);
        CREDO_AEQUALIS_S32 ((s32)_quotiens(scriptura.textus,
            " linea=\""), (s32)I);

        /* nullum fragmentum: nihil communicatur in arbore pura */
        CREDO_AEQUALIS_S32 ((s32)_quotiens(scriptura.textus, "<#"),
            (s32)ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: scriptor - exhibitum III (AMBIGUUS, communicatio)
     *
     * 'i8 * t;' sine lexico: duo bracchia GLR lexemata EADEM ferunt.
     * Arbor a silva_scribere consulto divergit - OMNIA bracchia
     * emittuntur, quod est ipsa captura prima recensionis
     * repraesentationalis (circuitus octetorum ambiguitatem servare
     * structuraliter nequit).
     * ======================================================== */

    {
         constans character* fons = "i8 * t;\n";
               SilvaParsura* parsura;
        constans SilvaNodus* ambiguus;
        SilvaArborScriptura  scriptura;

        imprimere("\n--- Probans scriptorem (exhibitum III) ---\n");

        parsura = silva_c89_parsare(piscina, "ambiguum.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);

        ambiguus = _primus_congruens(piscina, parsura, "ambiguus");
        si (ambiguus == NIHIL)
        {
            imprimere("  (nullum ambiguum - grammatica mutata?)\n");
        }
        CREDO_NON_NIHIL (ambiguus);

        si (ambiguus != NIHIL)
        {
            scriptura = silva_arbor_scribere_nodum(piscina, ambiguus,
                &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
            si (!scriptura.successus)
            {
                imprimere("  FRACTA: %s\n",
                    scriptura.causa ? scriptura.causa : "(sine causa)");
            }
            CREDO_VERUM (scriptura.successus);

            /* AMBO bracchia emissa - divergentia a scribere */
            CREDO_VERUM (_quotiens(scriptura.textus,
                "<interpretationes>") == I);
            CREDO_VERUM (_quotiens(scriptura.textus,
                "<declaratio>") >= I);
            CREDO_VERUM (_quotiens(scriptura.textus,
                "<sententia-expressionis>") >= I);

            /* locus INDEX ut textus arabicus */
            CREDO_VERUM (_quotiens(scriptura.textus,
                "<canonica>") == I);

            /* COMMUNICATIO: lexemata communicata fragmenta accipiunt,
             * usus sequentes transclusiones. Numeri congruere DEBENT -
             * fragmentum sine transclusione (aut contra) significaret
             * identitatem perisse */
            /* NB '<<#lexN>>' chordam '<#lex' CONTINET - ergo numerus
             * crudus fragmenta ET transclusiones simul numerat.
             * Fragmenta = crudum - transclusiones. */
            {
                i32 crudum = _quotiens(scriptura.textus, "<#lex");
                i32 transclusiones = _quotiens(scriptura.textus,
                    "<<#lex");
                i32 fragmenta = crudum - transclusiones;

                CREDO_VERUM (fragmenta >= I);
                /* lexema quodque communicatum bis adhibetur (bracchia
                 * duo), ergo fragmentum unum + transclusio una */
                CREDO_AEQUALIS_S32 ((s32)fragmenta,
                    (s32)transclusiones);
            }
        }
    }


    /* ========================================================
     * PROBARE: scriptor - refutationes clarae
     * ======================================================== */

    {
         constans character* fons = "int n = 0;\n";
               SilvaParsura* parsura;
        constans SilvaNodus* declaratio;
        SilvaArborScriptura  scriptura;

        imprimere("\n--- Probans refutationes scriptoris ---\n");

        parsura = silva_c89_parsare(piscina, "exhibitum.c", fons,
            (i32)strlen(fons), NIHIL);
        declaratio = _primus_congruens(piscina, parsura, "declaratio");
        CREDO_NON_NIHIL (declaratio);

        /* argumenta nihil */
        scriptura = silva_arbor_scribere_nodum(NIHIL, declaratio,
            &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
        CREDO_FALSUM (scriptura.successus);
        CREDO_NON_NIHIL (scriptura.causa);

        scriptura = silva_arbor_scribere_nodum(piscina, NIHIL,
            &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
        CREDO_FALSUM (scriptura.successus);

        /* grammatica innominata: registrum nomen SUUM non fert, ergo
         * scriptor eum fingere non potest */
        scriptura = silva_arbor_scribere_nodum(piscina, declaratio,
            &SILVA_C89_REGISTRUM, NIHIL, parsura->expansio, NIHIL);
        CREDO_FALSUM (scriptura.successus);
        CREDO_VERUM (scriptura.causa != NIHIL
            && strcmp(scriptura.causa, "grammatica innominata")
                == ZEPHYRUM);

        /* genus registro ignotum: nodus formae alienae */
        {
            SilvaNodus* fictus = silva_nodus_creare(piscina,
                (s32)9999, ZEPHYRUM);

            CREDO_NON_NIHIL (fictus);
            scriptura = silva_arbor_scribere_nodum(piscina, fictus,
                &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
            CREDO_FALSUM (scriptura.successus);
            CREDO_VERUM (scriptura.causa != NIHIL
                && strcmp(scriptura.causa, "genus registro ignotum")
                    == ZEPHYRUM);
            CREDO_AEQUALIS_PTR (scriptura.sedes, fictus);
        }

        /* forma nodi registro non congruens (loci nimis pauci) */
        {
            SilvaNodus* truncus = silva_nodus_creare(piscina,
                (s32)SILVA_C89_GENUS_CORPUS, I);

            CREDO_NON_NIHIL (truncus);
            scriptura = silva_arbor_scribere_nodum(piscina, truncus,
                &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
            CREDO_FALSUM (scriptura.successus);
            CREDO_VERUM (scriptura.causa != NIHIL
                && strcmp(scriptura.causa,
                       "forma nodi registro non congruit") == ZEPHYRUM);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
