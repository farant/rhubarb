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
    primum = (SilvaQuaestioResultatum*)xar_obtinere(resultata,
        ZEPHYRUM);
    redde primum ? primum->nodus : NIHIL;
}

/* Nodum generis dati in valore quaerere - per RADICEM commissionis
 * (SilvaValor, non-constantem), ergo accessus mutabilis sine ullo
 * cast qualificatorem tollente (-Wcast-qual eum vetat) */
interior SilvaNodus*
_nodus_generis (
    SilvaValor valor,
           s32 genus)
{
    SilvaNodus* fructus;
           i32  numerus;
           i32  i;

    si (valor.genus == SILVA_VALOR_NODUS)
    {
        si (valor.datum.nodus == NIHIL)
        {
            redde NIHIL;
        }
        si (valor.datum.nodus->genus == genus)
        {
            redde valor.datum.nodus;
        }
        per (i = ZEPHYRUM; i < valor.datum.nodus->numerus_locorum; i++)
        {
            fructus = _nodus_generis(valor.datum.nodus->loci[i], genus);
            si (fructus != NIHIL)
            {
                redde fructus;
            }
        }
        redde NIHIL;
    }
    si (valor.genus == SILVA_VALOR_LISTA)
    {
        numerus = silva_valor_lista_numerus(valor);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaValor* elementum = silva_valor_lista_obtinere(valor,
                i);

            si (elementum != NIHIL)
            {
                fructus = _nodus_generis(*elementum, genus);
                si (fructus != NIHIL)
                {
                    redde fructus;
                }
            }
        }
    }
    redde NIHIL;
}

/* Primum lexema subarboris, ordine locorum */
interior SilvaToken*
_primum_lexema (
    SilvaNodus* nodus)
{
    SilvaToken* fructus;
           i32  numerus;
           i32  i;
           i32  j;

    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        SilvaValor valor = nodus->loci[i];

        si (valor.genus == SILVA_VALOR_TOKEN)
        {
            redde valor.datum.token;
        }
        si (valor.genus == SILVA_VALOR_NODUS)
        {
            fructus = _primum_lexema(valor.datum.nodus);
            si (fructus != NIHIL)
            {
                redde fructus;
            }
        }
        si (valor.genus == SILVA_VALOR_LISTA)
        {
            numerus = silva_valor_lista_numerus(valor);
            per (j = ZEPHYRUM; j < numerus; j++)
            {
                SilvaValor* elementum =
                    silva_valor_lista_obtinere(valor, j);

                si (elementum == NIHIL)
                {
                    perge;
                }
                si (elementum->genus == SILVA_VALOR_TOKEN)
                {
                    redde elementum->datum.token;
                }
                si (elementum->genus == SILVA_VALOR_NODUS)
                {
                    fructus = _primum_lexema(elementum->datum.nodus);
                    si (fructus != NIHIL)
                    {
                        redde fructus;
                    }
                }
            }
        }
    }
    redde NIHIL;
}

/* Primam occurrentiam acus 'novo' substituere (vitia in documento
 * plantare) */
interior chorda
_substituere (
                Piscina* piscina,
                 chorda  fenum,
     constans character* acus,
     constans character* novum)
{
    ChordaAedificator* aedificator;
                  i32  longitudo;
                  i32  i;
                  b32  factum;

    aedificator = chorda_aedificator_creare(piscina, fenum.mensura
        + 64);
    longitudo  = (i32)strlen(acus);
    factum     = FALSUM;

    per (i = ZEPHYRUM; i < fenum.mensura; i++)
    {
        si (   !factum
            && i + longitudo <= fenum.mensura
            && memcmp(fenum.datum + i, acus, (size_t)longitudo)
                   == ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aedificator, novum);
            i       += longitudo - I;
            factum  = VERUM;
            perge;
        }
        chorda_aedificator_appendere_character(aedificator,
            (character)fenum.datum[i]);
    }
    redde chorda_aedificator_finire(aedificator);
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
        si (memcmp(fenum.datum + i, acus, (size_t)longitudo)
            == ZEPHYRUM)
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

        c89_primum = silva_arbor_sigillum(piscina,
            &SILVA_C89_REGISTRUM);
        c89_iterum = silva_arbor_sigillum(piscina,
            &SILVA_C89_REGISTRUM);

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

        /* involucrum: grammatica + sigillum + ancora. Exemplaria
         * suturam titulus-attributum non transeunt: forma
         * multilinea attributorum (stml decretum quintum) tagum
         * intra frangit, ergo '<arbor' et attributum seorsum
         * quaeruntur - ambabus formis superstites */
        CREDO_VERUM (_quotiens(scriptura.textus, "<arbor") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "grammatica=\"c89\"") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "registrum-sigillum=\"") == I);

        /* genus = tag, locus = involucrum. Collapsus verticalis
         * (stml M2b) vincula unigena in captores vertit etiam cum
         * interius in lineam non cadat - '<specificatores(>' pro
         * '<specificatores>'; declaratio liberis pluribus blocus
         * manet */
        CREDO_VERUM (_quotiens(scriptura.textus, "<declaratio>") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<specificatores(>") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<typus-primitivus(>") == I);

        /* lexema orthographiae FIXAE: tag solus, NULLUS textus
         * (orthographia in genere ipso vivit). Collapsus spinae
         * (stml M2 T3c) vincula unigena in formam capturae vertit -
         * '<lex-int(>' pro '<lex-int>' */
        CREDO_VERUM (_quotiens(scriptura.textus, "<lex-int(>") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<lex-semicolon(>") == I);
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<lex-assignatio(>") == I);

        /* lexema orthographiae VARIAE: valor ut textus CRUDUS
         * (stml M3 - octeti codicis a re-involutione fluxus tuti;
         * crudus a spina et captura exclusus, clausura nominata) */
        CREDO_VERUM (_quotiens(scriptura.textus,
            "<lex-integer!>0</lex-integer>") == I);

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

            /* locus INDEX ut textus arabicus (collapsus: forma
             * capturae) */
            CREDO_VERUM (_quotiens(scriptura.textus,
                "<canonica(>") == I);

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


    /* ========================================================
     * PROBARE: comparator - vitium PLANTATUM per campum quemque
     *
     * Duae parsurae eiusdem fontis: arbores aequales, obiecta
     * lexematum INDEPENDENTIA. Campus unus mutatur, inaequalitas
     * exspectatur ET campus rectus nominari debet. Comparator qui
     * frangi non potest classis custodiae mortuae est - eadem
     * classis quam tabula orthographiae in T2 vitavit.
     * ======================================================== */

    {
           constans character* fons = "int n = 0;\n";
                 SilvaParsura* parsura_a;
                 SilvaParsura* parsura_b;
                   SilvaNodus* a;
                   SilvaNodus* b;
                   SilvaToken* lexema;
        SilvaArborDifferentia  differentia;

        imprimere("\n--- Probans comparatorem (vitia plantata) ---\n");

        parsura_a = silva_c89_parsare(piscina, "aequalitas.c", fons,
            (i32)strlen(fons), NIHIL);
        parsura_b = silva_c89_parsare(piscina, "aequalitas.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura_a);
        CREDO_NON_NIHIL (parsura_b);

        a = _nodus_generis(parsura_a->commissio->radix,
            (s32)SILVA_C89_GENUS_DECLARATIO);
        b = _nodus_generis(parsura_b->commissio->radix,
            (s32)SILVA_C89_GENUS_DECLARATIO);
        CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b);

        /* BASIS: parsurae duae eiusdem fontis aequales UTROQUE modo.
         * Sine hac assertione omnia infra vacue transirent */
        CREDO_VERUM (silva_arbor_aequalis(a, b,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        CREDO_NIHIL (differentia.campus);
        CREDO_VERUM (silva_arbor_aequalis(a, b,
            SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));

        /* differentia NIHIL licet */
        CREDO_VERUM (silva_arbor_aequalis(a, b,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, NIHIL));

        lexema = _primum_lexema(b);
        CREDO_NON_NIHIL (lexema);

        /* i. lexema/genus */
        {
            SilvaLexemaGenus servatum = lexema->genus;

            lexema->genus = SILVA_LEX_LONG;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/genus")
                    == ZEPHYRUM);
            CREDO_AEQUALIS_PTR (differentia.lexema_b, lexema);
            /* semita sedem nominat, non solum campum */
            CREDO_VERUM (differentia.via[0] != '\0');
            lexema->genus = servatum;
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        }

        /* ii. lexema/valor */
        {
            chorda servatus = lexema->valor;

            lexema->valor = chorda_ex_literis("zzz", piscina);
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/valor")
                    == ZEPHYRUM);
            lexema->valor = servatus;
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        }

        /* iii. lexema/standard */
        {
            i8 servatum = lexema->standard;

            lexema->standard = (i8)SILVA_STANDARD_C99;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/standard")
                    == ZEPHYRUM);
            lexema->standard = servatum;
        }

        /* iv. lexema/fons */
        {
            s32 servatum = lexema->fons_index;

            lexema->fons_index = servatum + I;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/fons")
                    == ZEPHYRUM);
            lexema->fons_index = servatum;
        }

        /* v. lexema/initium-lineae */
        {
            b32 servatum = lexema->initium_lineae;

            lexema->initium_lineae = servatum ? FALSUM : VERUM;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/initium-lineae")
                    == ZEPHYRUM);
            lexema->initium_lineae = servatum;
        }

        /* vi. MODUS vere aliquid facit: positio mutata FIDELITATE
         * capitur, STRUCTURA non. Sine ambabus assertionibus modus
         * qui nihil ageret idem videretur ac modus qui operatur */
        {
            i32 servata = lexema->linea;

            lexema->linea = servata + X;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/linea")
                    == ZEPHYRUM);
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));
            lexema->linea = servata;
        }

        /* vii. idem pro columna et offset */
        {
            i32 servata = lexema->columna;

            lexema->columna = servata + X;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));
            lexema->columna = servata;
        }
        {
            s32 servatum = lexema->byte_offset;

            lexema->byte_offset = servatum + X;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/offset")
                    == ZEPHYRUM);
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));
            lexema->byte_offset = servatum;
        }

        /* viii. PROVENIENTIA - UTROQUE modo capitur. Haec est
         * assertio quae custodiam 'positiones solum cum ambo sedem
         * habeant' mortuam esse probat: vitium derivationis omnia ad
         * -I ponens illic taceret, hic clamat */
        {
            s32 servatum = lexema->byte_offset;

            lexema->byte_offset = -I;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/provenientia")
                    == ZEPHYRUM);
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/provenientia")
                    == ZEPHYRUM);
            lexema->byte_offset = servatum;
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));
        }

        /* ix. trivia - series et numerus */
        {
            Xar* servata = lexema->spatia_post;

            lexema->spatia_post = NIHIL;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "trivia/post")
                    == ZEPHYRUM);
            lexema->spatia_post = servata;
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        }

        /* x. trivium IPSUM (valor triviorum, non solum numerus).
         * NB praesentia triviorum ASSERITUR, non si-clausula
         * custoditur: 'int' spatium sequens fert. Custodita, haec
         * probatio tacite transiret quandocumque trivia abessent -
         * ipsa classis custodiae mortuae quam hic venamur */
        {
            SilvaToken* trivium;
                chorda  servatus;

            CREDO_NON_NIHIL (lexema->spatia_post);
            CREDO_VERUM (xar_numerus(lexema->spatia_post) > ZEPHYRUM);

            trivium  = *(SilvaToken**)xar_obtinere(
                lexema->spatia_post, ZEPHYRUM);
            servatus = trivium->valor;

            trivium->valor = chorda_ex_literis("      ", piscina);
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/valor")
                    == ZEPHYRUM);
            trivium->valor = servatus;
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        }

        /* xi. nodus/genus */
        {
            s32 servatum = b->genus;

            b->genus = (s32)SILVA_C89_GENUS_CORPUS;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "nodus/genus")
                    == ZEPHYRUM);
            CREDO_AEQUALIS_PTR (differentia.nodus_b, b);
            b->genus = servatum;
        }

        /* xii. nodus/numerus-locorum */
        {
            i32 servatus = b->numerus_locorum;

            b->numerus_locorum = servatus - I;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "nodus/numerus-locorum")
                    == ZEPHYRUM);
            b->numerus_locorum = servatus;
        }

        /* xiii. nodus/pater-nullitas - captura 'lector patrem omnino
         * non fixit'. Comparatio monstratorum trans arbores nihil
         * significaret; nullitas significat */
        {
            SilvaNodus* servatus = b->pater;

            /* NODUS NIDIFICATUS adhibendus est, non 'declaratio'
             * ipsa: nodus summus in LISTA radicis sedet, ergo
             * patrem NON habet (commissio patrem inter nodos figit,
             * non a lista). Prima forma huius probationis id
             * si-clausula custodiebat et ideo NIHIL asserebat -
             * custodia clausulam vacuam CELABAT. Inventum eam
             * tollendo. */
            SilvaNodus* nidus_a = _nodus_generis(
                parsura_a->commissio->radix,
                (s32)SILVA_C89_GENUS_TYPUS_PRIMITIVUS);
            SilvaNodus* nidus_b = _nodus_generis(
                parsura_b->commissio->radix,
                (s32)SILVA_C89_GENUS_TYPUS_PRIMITIVUS);

            CREDO_NON_NIHIL (nidus_a);
            CREDO_NON_NIHIL (nidus_b);
            CREDO_NON_NIHIL (nidus_a->pater);
            CREDO_NON_NIHIL (nidus_b->pater);

            /* Vitium in nodo INTERIORE plantandum, et comparatio a
             * RADICIBUS incipienda: parentela radicum comparationis
             * extra comparationem iacet, ergo ibi CONSULTO non
             * confertur (T6: radices conferre CIX divergentias
             * falsas super corpus dedit). Prima forma huius
             * probationis vitium in radice IPSA plantabat et ideo
             * post emendationem T6 tacuit. */
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));

            servatus        = nidus_b->pater;
            nidus_b->pater  = NIHIL;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus,
                       "nodus/pater-nullitas") == ZEPHYRUM);
            nidus_b->pater = servatus;
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));

            /* et RADIX ipsa non confertur - custodia deliberata */
            servatus = b->pater;
            b->pater = b->pater ? NIHIL : a;
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            b->pater = servatus;
        }

        /* xiv. locus/genus-valoris - locus praesens contra NIHIL */
        {
            SilvaValor servatus  = b->loci[0];
            SilvaValor vacuus    = silva_valor_nihil();

            b->loci[0] = vacuus;
            CREDO_FALSUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "locus/genus-valoris")
                    == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (differentia.locus, (s32)ZEPHYRUM);
            b->loci[0] = servatus;
            CREDO_VERUM (silva_arbor_aequalis(a, b,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        }

        /* xv. arbor contra se ipsam semper aequalis */
        CREDO_VERUM (silva_arbor_aequalis(a, a,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));

        /* xvi. NIHIL */
        CREDO_VERUM (silva_arbor_aequalis(NIHIL, NIHIL,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        CREDO_FALSUM (silva_arbor_aequalis(a, NIHIL,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        CREDO_VERUM (differentia.campus != NIHIL
            && strcmp(differentia.campus, "nodus/nihil") == ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: lector T5a - constructio + validatio
     * ======================================================== */

    {
           constans character* fons = "int n = 0;\n";
                 SilvaParsura* parsura;
          constans SilvaNodus* origo;
                   SilvaNodus* lecta;
          SilvaArborScriptura  scriptura;
             SilvaArborVitium  vitium;
        SilvaArborDifferentia  differentia;
                       chorda  documentum;

        imprimere("\n--- Probans lectorem (T5a) ---\n");

        parsura = silva_c89_parsare(piscina, "lector.c", fons,
            (i32)strlen(fons), NIHIL);
        origo = _primus_congruens(piscina, parsura, "declaratio");
        CREDO_NON_NIHIL (origo);

        scriptura = silva_arbor_scribere_nodum(piscina, origo,
            &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
        CREDO_VERUM (scriptura.successus);
        documentum = scriptura.textus;

        /* i. CIRCUITUS: documentum relegi potest */
        lecta = silva_arbor_legere(piscina, NIHIL, documentum,
            &SILVA_C89_REGISTRUM, "c89", &vitium);
        si (lecta == NIHIL)
        {
            imprimere("  RECUSATUM: %s (linea %d)\n",
                vitium.causa ? vitium.causa : "(sine causa)",
                (integer)vitium.linea);
        }
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);

        /* forma servata */
        CREDO_AEQUALIS_S32 (lecta->genus, origo->genus);
        CREDO_AEQUALIS_I32 (lecta->numerus_locorum,
            origo->numerus_locorum);

        /* contentum servatum: valor primi lexematis + trivia */
        {
            SilvaToken* lexema_originis = _primum_lexema(
                _nodus_generis(parsura->commissio->radix,
                    (s32)SILVA_C89_GENUS_DECLARATIO));
            SilvaToken* lexema_lectum = _primum_lexema(lecta);

            CREDO_NON_NIHIL (lexema_originis);
            CREDO_NON_NIHIL (lexema_lectum);
            CREDO_VERUM (lexema_lectum->genus
                == lexema_originis->genus);
            /* orthographia FIXA ex tabula restituta - documentum eam
             * non fert, ergo hoc probat tabulam T2 vere adhiberi */
            CREDO_VERUM (chorda_aequalis_literis(lexema_lectum->valor,
                "int"));
            /* lens triviorum inversa: n="1" -> spatium unum VERUM */
            CREDO_NON_NIHIL (lexema_lectum->spatia_post);
            CREDO_AEQUALIS_I32 (
                xar_numerus(lexema_lectum->spatia_post), (i32)I);
            {
                SilvaToken* trivium = *(SilvaToken**)xar_obtinere(
                    lexema_lectum->spatia_post, ZEPHYRUM);

                CREDO_VERUM (trivium->genus == SILVA_LEX_SPATIA);
                CREDO_VERUM (chorda_aequalis_literis(trivium->valor,
                    " "));
            }
        }

        /* ii. COMPARATOR ARBOREM T5a AEQUALEM NON DICIT, ne modo
         * structurali quidem - lexemata lecta SYNTHETICA sunt
         * (byte_offset -I) donec T5b positiones derivet, et
         * PROVENIENTIA utroque modo confertur. Haec assertio probat
         * T5a circuitum plenum simulare NON posse: custodia id DICIT
         * potius quam celet, quod est ipsa ratio cur provenientia
         * modum non sequitur. */
        /* ORACULUM A (T5b): aequalitas PLENA modo FIDELITATIS.
         * Documentum positiones NON fert, ergo hoc derivationem
         * ipsam probat - id est ratio cur positiones in comparatorem
         * omnino inclusae sunt. In T5a haec assertio 'recusat,
         * patrem nominans' erat; nunc vertitur, ut consilium dixit. */
        si (!silva_arbor_aequalis(origo, lecta,
                 SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia))
        {
            imprimere("  [divergentia] %s @ %s\n",
                differentia.campus ? differentia.campus : "(nihil)",
                differentia.via);
        }
        CREDO_VERUM (silva_arbor_aequalis(origo, lecta,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        CREDO_VERUM (silva_arbor_aequalis(origo, lecta,
            SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));

        /* Aequalitas plena primo cursu suspecta est - ergo probemus
         * eam ex ANCORA vere pendere, non fortuito congruere. */

        /* ancora MOTA -> sedes omnes labuntur */
        {
            SilvaNodus* mota = silva_arbor_legere(piscina, NIHIL,
                _substituere(piscina, documentum, " b=\"0\"",
                    " b=\"500\""),
                &SILVA_C89_REGISTRUM, "c89", &vitium);

            CREDO_NON_NIHIL (mota);
            CREDO_FALSUM (silva_arbor_aequalis(origo, mota,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/offset")
                    == ZEPHYRUM);
            /* structura tamen intacta - modus rem suam agit */
            CREDO_VERUM (silva_arbor_aequalis(origo, mota,
                SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));
        }

        /* ancora ABSENS -> arbor AUCTORATA: sedes -I manent, et
         * comparator id per PROVENIENTIAM dicit (utroque modo).
         * Hic est casus arboris sine textu fontis */
        {
            SilvaNodus* nuda = silva_arbor_legere(piscina, NIHIL,
                _substituere(piscina, documentum, " b=\"0\"", ""),
                &SILVA_C89_REGISTRUM, "c89", &vitium);
            SilvaToken* lexema_nudum;

            CREDO_NON_NIHIL (nuda);
            lexema_nudum = _primum_lexema(nuda);
            CREDO_NON_NIHIL (lexema_nudum);
            CREDO_VERUM (lexema_nudum->byte_offset < ZEPHYRUM);
            /* structura LINEARUM tamen derivata est: ancora
             * ORDINATAS solas regit */
            CREDO_VERUM (lexema_nudum->initium_lineae);
            CREDO_AEQUALIS_I32 (lexema_nudum->longitudo,
                lexema_nudum->valor.mensura);

            CREDO_FALSUM (silva_arbor_aequalis(origo, nuda,
                SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/provenientia")
                    == ZEPHYRUM);
        }

        /* iii. REFUTATIONES - vitium in documento PLANTATUM,
         * quaeque causam SUAM et lineam nominans */

        /* sigillum: arbor vocabulario falso iudicata mendacium est */
        CREDO_NIHIL (silva_arbor_legere(piscina, NIHIL,
            _substituere(piscina, documentum, "registrum-sigillum=\"",
                "registrum-sigillum=\"ff"),
            &SILVA_C89_REGISTRUM, "c89", &vitium));
        CREDO_VERUM (vitium.causa != NIHIL
            && strcmp(vitium.causa, "sigillum registri non congruit")
                == ZEPHYRUM);
        CREDO_VERUM (vitium.linea > ZEPHYRUM);

        /* grammatica */
        CREDO_NIHIL (silva_arbor_legere(piscina, NIHIL, documentum,
            &SILVA_C89_REGISTRUM, "sceletum", &vitium));
        CREDO_VERUM (vitium.causa != NIHIL
            && strcmp(vitium.causa, "grammatica non congruit")
                == ZEPHYRUM);

        /* genus ignotum. NB elementum SINE tag claudente eligendum
         * est ('<declaratio>' tag claudentem NOMINATUM fert - tag
         * apertum solum substituere vitium PARSATIONIS daret, non
         * vocabularii). Forma capturae (M2b) tag claudentem omnino
         * caret - captor fictus tuto substituitur */
        CREDO_NIHIL (silva_arbor_legere(piscina, NIHIL,
            _substituere(piscina, documentum, "<typus-primitivus(>",
                "<genus-fictum(>"),
            &SILVA_C89_REGISTRUM, "c89", &vitium));
        CREDO_VERUM (vitium.causa != NIHIL
            && strcmp(vitium.causa, "genus registro ignotum")
                == ZEPHYRUM);
        CREDO_VERUM (vitium.linea > ZEPHYRUM);

        /* locus generi ignotus */
        CREDO_NIHIL (silva_arbor_legere(piscina, NIHIL,
            _substituere(piscina, documentum, "<specificatores(>",
                "<locus-fictus(>"),
            &SILVA_C89_REGISTRUM, "c89", &vitium));
        CREDO_VERUM (vitium.causa != NIHIL
            && strcmp(vitium.causa, "locus generi ignotus")
                == ZEPHYRUM);

        /* valor in genere orthographiae FIXAE - documentum quod
         * orthographiam portat fontem veritatis tertium faceret.
         * Corruptio in forma collapsa: '<lex-int(> xyz' textum
         * capit (regula capturae valorem purum dat). NB sine spatio
         * caudali: sarcinatio M2b lineam post '<lex-int(>' frangit */
        CREDO_NIHIL (silva_arbor_legere(piscina, NIHIL,
            _substituere(piscina, documentum, "<lex-int(>",
                "<lex-int(> xyz"),
            &SILVA_C89_REGISTRUM, "c89", &vitium));
        CREDO_VERUM (vitium.causa != NIHIL
            && strcmp(vitium.causa,
                   "valor in genere orthographiae fixae") == ZEPHYRUM);

        /* involucrum absens */
        CREDO_NIHIL (silva_arbor_legere(piscina, NIHIL,
            chorda_ex_literis("<aliud/>", piscina),
            &SILVA_C89_REGISTRUM, "c89", &vitium));
        CREDO_VERUM (vitium.causa != NIHIL
            && strcmp(vitium.causa, "involucrum <arbor> deest")
                == ZEPHYRUM);

        /* STML malformatum */
        CREDO_NIHIL (silva_arbor_legere(piscina, NIHIL,
            chorda_ex_literis("<arbor", piscina),
            &SILVA_C89_REGISTRUM, "c89", &vitium));
        CREDO_NON_NIHIL (vitium.causa);

        /* argumenta nihil */
        CREDO_NIHIL (silva_arbor_legere(piscina, NIHIL, documentum,
            &SILVA_C89_REGISTRUM, NIHIL, &vitium));
        CREDO_VERUM (vitium.causa != NIHIL
            && strcmp(vitium.causa, "argumenta nihil") == ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: lector - transclusio CLARE differtur (T5b)
     * ======================================================== */

    {
         constans character* fons = "i8 * t;\n";
               SilvaParsura* parsura;
        constans SilvaNodus* ambiguus;
        SilvaArborScriptura  scriptura;
           SilvaArborVitium  vitium;

        imprimere("\n--- Probans dilationem transclusionis ---\n");

        parsura  = silva_c89_parsare(piscina, "ambiguum.c", fons,
            (i32)strlen(fons), NIHIL);
        ambiguus = _primus_congruens(piscina, parsura, "ambiguus");
        CREDO_NON_NIHIL (ambiguus);

        scriptura = silva_arbor_scribere_nodum(piscina, ambiguus,
            &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
        CREDO_VERUM (scriptura.successus);

        /* T5b: documentum communicatum LEGITUR, et lexemata
         * communicata OBIECTUM IDEM sunt - non paria. Identitas res
         * est: bracchia GLR eosdem octetos fontis tegunt, et
         * duplicatio de arbore mentiretur. */
        {
            SilvaNodus* lectus;
            SilvaNodus* bracchium_a;
            SilvaNodus* bracchium_b;
            SilvaValor  interpretationes;
                   i32  i;

            lectus = silva_arbor_legere(piscina, NIHIL,
                scriptura.textus, &SILVA_C89_REGISTRUM, "c89",
                &vitium);
            si (lectus == NIHIL)
            {
                imprimere("  RECUSATUM: %s (linea %d)\n",
                    vitium.causa ? vitium.causa : "(sine causa)",
                    (integer)vitium.linea);
            }
            CREDO_NON_NIHIL (lectus);
            CREDO_NIHIL (vitium.causa);

            /* locum interpretationum invenire (lista bracchiorum) */
            interpretationes = silva_valor_nihil();
            per (i = ZEPHYRUM; i < lectus->numerus_locorum; i++)
            {
                si (lectus->loci[i].genus == SILVA_VALOR_LISTA)
                {
                    interpretationes = lectus->loci[i];
                    frange;
                }
            }
            CREDO_VERUM (silva_valor_lista_numerus(interpretationes)
                >= II);

            bracchium_a = silva_valor_lista_obtinere(
                interpretationes, ZEPHYRUM)->datum.nodus;
            bracchium_b = silva_valor_lista_obtinere(
                interpretationes, I)->datum.nodus;
            CREDO_NON_NIHIL (bracchium_a);
            CREDO_NON_NIHIL (bracchium_b);
            /* bracchia DIVERSA (genera diversa), lexemata EADEM */
            CREDO_INAEQUALITAS_PTR (bracchium_a, bracchium_b);
            CREDO_AEQUALIS_PTR (_primum_lexema(bracchium_a),
                _primum_lexema(bracchium_b));
        }

        /* transclusio ORBA (fragmentum ignotum) clare recusatur */
        CREDO_NIHIL (silva_arbor_legere(piscina, NIHIL,
            _substituere(piscina, scriptura.textus, "<<#lex",
                "<<#orphanum"),
            &SILVA_C89_REGISTRUM, "c89", &vitium));
        CREDO_VERUM (vitium.causa != NIHIL
            && strcmp(vitium.causa,
                   "transclusio ad fragmentum ignotum") == ZEPHYRUM);
        CREDO_VERUM (vitium.linea > ZEPHYRUM);
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
