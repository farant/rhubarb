/* auspex.c - certificatio conversionum arithmeticarum (M0b Chunk B)
 *
 * Avium spectator, haruspicis frater. Pro quoque pari primitivorum
 * et operatore expressio "(T1)1 OP (T2)1" per silvam parsatur et
 * typatur; deinde TU assertionum temporis compilationis emittitur -
 * EADEM expressione verbatim: magnitudo per sizeof (in fine aciei),
 * signum per "(expr) * 0 - 1 > 0" (expressio constans integralis:
 * 1 pro insignato, 0 pro signato). clang -fsyntax-only = iudicium.
 *
 * Fluitantia: magnitudo sola (non constantia integralia; double vs
 * long double in Darwin arm64 indistinguibilia per sizeof - hiatus
 * notatus in worklog). Monstratores/variabiles: magnitudo sola.
 *
 * TU ut C99 compilatur: extensio longus longus ordines C99 sequitur
 * (INTENTIO DECISUS 4) - ipsa electio hic certificatur.
 *
 * Usus: ./silva/auspex.sh   (curre ex radice repositorii)
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define AUSPEX_VIA_TU "silva/build/auspicium.c"

nomen structura {
    constans character* titulus;
    b32                 integrale;
} AuspexTypus;

interior constans AuspexTypus TYPI[] = {
    { "char",               VERUM },
    { "signed char",        VERUM },
    { "unsigned char",      VERUM },
    { "short",              VERUM },
    { "unsigned short",     VERUM },
    { "int",                VERUM },
    { "unsigned int",       VERUM },
    { "long",               VERUM },
    { "unsigned long",      VERUM },
    { "long long",          VERUM },
    { "unsigned long long", VERUM },
    { "float",              FALSUM },
    { "double",             FALSUM },
    { "long double",        FALSUM }
};
#define TYPI_NUMERUS ((i32)(magnitudo(TYPI) / magnitudo(TYPI[0])))

nomen structura {
    constans character* textus;
    b32                 integrale_solum;
} AuspexOperator;

interior constans AuspexOperator OPERATORES[] = {
    { "+",  FALSUM }, { "-",  FALSUM }, { "*", FALSUM },
    { "/",  FALSUM }, { "<",  FALSUM }, { "==", FALSUM },
    { "&&", FALSUM },
    { "%",  VERUM },  { "<<", VERUM },  { ">>", VERUM },
    { "&",  VERUM },  { "^",  VERUM },  { "|",  VERUM }
};
#define OPERATORES_NUMERUS \
    ((i32)(magnitudo(OPERATORES) / magnitudo(OPERATORES[0])))

/* expressiones mixtae (variabiles globales - magnitudo sola) */
interior constans character* constans MIXTA[] = {
    "gp + 1", "1 + gp", "gp - 1", "gp - gq", "ga + 1",
    "1 ? gp : 0", "1 ? gvp : gp", "1 ? ga : gp",
    "gp == 0", "gp < gq",
    NIHIL
};

interior constans character* constans GLOBALIA =
    "int* gp;\n"
    "int* gq;\n"
    "int ga[4];\n"
    "void* gvp;\n";

nomen structura {
    constans character* textus;   /* expressio verbatim */
    b32                 signi;    /* assertio signi (constans
                                   * integralis) licet */
} AuspexCombo;

interior b32
_primitivum_insignatum (s32 p)
{
    commutatio (p)
    {
        casus PRIMITIVUM_CHARACTER_INSIGNATUM:
        casus PRIMITIVUM_BREVIS_INSIGNATUM:
        casus PRIMITIVUM_INTEGER_INSIGNATUM:
        casus PRIMITIVUM_LONGUS_INSIGNATUM:
        casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

interior vacuum
_combo_addere (Piscina* piscina, Xar* combi, constans character* textus,
    b32 signi)
{
    AuspexCombo* c = (AuspexCombo*)xar_addere(combi);

    si (c == NIHIL)
    {
        redde;
    }
    c->textus = textus;
    c->signi = signi;
    (vacuum)piscina;
}

/* Textum expressionis binariae in piscinam formare */
interior constans character*
_binarium_formare (Piscina* piscina, constans character* t1,
    constans character* op, constans character* t2)
{
    character* textus = (character*)piscina_allocare(piscina, CXXVIII);

    si (textus == NIHIL)
    {
        redde "";
    }
    sprintf(textus, "(%s)1 %s (%s)1", t1, op, t2);
    redde textus;
}

interior constans character*
_unarium_formare (Piscina* piscina, constans character* op,
    constans character* t)
{
    character* textus = (character*)piscina_allocare(piscina, CXXVIII);

    si (textus == NIHIL)
    {
        redde "";
    }
    sprintf(textus, "%s(%s)1", op, t);
    redde textus;
}

/* Fontem probae struere: globalia + functio cum sententiis */
interior constans character*
_probam_struere (Piscina* piscina, Xar* combi, i32* mensura_out)
{
    character* fons = (character*)piscina_allocare(piscina,
        (memoriae_index)(DXII * MMXXV));   /* ~1MB */
    i32 cursor = ZEPHYRUM;
    i32 i;
    i32 j;
    i32 k;

    si (fons == NIHIL)
    {
        redde NIHIL;
    }
    cursor += (i32)sprintf(fons + cursor, "%s", GLOBALIA);
    cursor += (i32)sprintf(fons + cursor, "void _auspicium(void)\n{\n");

    /* binaria */
    per (k = ZEPHYRUM; k < OPERATORES_NUMERUS; k++)
    {
        per (i = ZEPHYRUM; i < TYPI_NUMERUS; i++)
        {
            per (j = ZEPHYRUM; j < TYPI_NUMERUS; j++)
            {
                constans character* textus;
                b32 ambo_integralia = TYPI[i].integrale
                    && TYPI[j].integrale;

                si (OPERATORES[k].integrale_solum
                    && !ambo_integralia)
                {
                    perge;
                }
                textus = _binarium_formare(piscina,
                    TYPI[i].titulus, OPERATORES[k].textus,
                    TYPI[j].titulus);
                _combo_addere(piscina, combi, textus,
                    ambo_integralia);
                cursor += (i32)sprintf(fons + cursor, "    %s;\n",
                    textus);
            }
        }
    }
    /* unaria: - (arithmetica), ~ (integralia), ! (omnia) */
    per (i = ZEPHYRUM; i < TYPI_NUMERUS; i++)
    {
        constans character* textus = _unarium_formare(piscina, "-",
            TYPI[i].titulus);

        _combo_addere(piscina, combi, textus, TYPI[i].integrale);
        cursor += (i32)sprintf(fons + cursor, "    %s;\n", textus);
    }
    per (i = ZEPHYRUM; i < TYPI_NUMERUS; i++)
    {
        constans character* textus;

        si (!TYPI[i].integrale)
        {
            perge;
        }
        textus = _unarium_formare(piscina, "~", TYPI[i].titulus);
        _combo_addere(piscina, combi, textus, VERUM);
        cursor += (i32)sprintf(fons + cursor, "    %s;\n", textus);
    }
    per (i = ZEPHYRUM; i < TYPI_NUMERUS; i++)
    {
        constans character* textus = _unarium_formare(piscina, "!",
            TYPI[i].titulus);

        _combo_addere(piscina, combi, textus, TYPI[i].integrale);
        cursor += (i32)sprintf(fons + cursor, "    %s;\n", textus);
    }
    /* mixta (variabiles - magnitudo sola) */
    per (i = ZEPHYRUM; MIXTA[i] != NIHIL; i++)
    {
        _combo_addere(piscina, combi, MIXTA[i], FALSUM);
        cursor += (i32)sprintf(fons + cursor, "    %s;\n", MIXTA[i]);
    }
    cursor += (i32)sprintf(fons + cursor, "}\n");
    *mensura_out = cursor;
    redde fons;
}

/* Elementa corporis definitionis (nodi soli) in xar colligere */
interior b32
_sententias_colligere (constans SilvaParsura* parsura,
    i32 index_definitionis, Xar* sententiae)
{
    SilvaValor* e = silva_valor_lista_obtinere(
        parsura->commissio->radix, index_definitionis);
    constans SilvaNodus* def;
    SilvaValor corpus_v;
    SilvaValor elementa;
    i32 i;
    i32 m;

    si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
    {
        redde FALSUM;
    }
    def = e->datum.nodus;
    si (def->genus != (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
    {
        redde FALSUM;
    }
    corpus_v = silva_c89_definitio_functionis_corpus(def);
    si (corpus_v.genus != SILVA_VALOR_NODUS)
    {
        redde FALSUM;
    }
    elementa = silva_c89_corpus_elementa(corpus_v.datum.nodus);
    m = (i32)silva_valor_lista_numerus(elementa);
    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(elementa, i);

        si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
        {
            constans SilvaNodus** locus =
                (constans SilvaNodus**)xar_addere(sententiae);

            si (locus != NIHIL)
            {
                *locus = v->datum.nodus;
            }
        }
    }
    redde VERUM;
}

s32
principale (vacuum)
{
    Piscina* piscina;
    Xar* combi;
    Xar* sententiae;
    constans character* fons;
    i32 mensura = ZEPHYRUM;
    SilvaParsura* parsura;
    SilvaSemantica* sem;
    FILE* pl;
    i32 k;
    i32 assertiones = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("auspex", 536870912);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "auspex: piscina deest\n");
        redde I;
    }
    combi = xar_creare(piscina, (i32)magnitudo(AuspexCombo));
    sententiae = xar_creare(piscina,
        (i32)magnitudo(constans SilvaNodus*));
    si (combi == NIHIL || sententiae == NIHIL)
    {
        fprintf(stderr, "auspex: xar deest\n");
        redde I;
    }

    fons = _probam_struere(piscina, combi, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "auspex: proba non structa\n");
        redde I;
    }
    parsura = silva_c89_parsare(piscina, "auspicium_probe.c", fons,
        mensura, NIHIL);
    si (parsura == NIHIL || parsura->numerus_errorum > ZEPHYRUM)
    {
        fprintf(stderr, "auspex: proba non parsata\n");
        redde I;
    }
    sem = silva_c89_semantica_analysare(piscina, parsura);
    si (sem == NIHIL)
    {
        fprintf(stderr, "auspex: analysis fracta\n");
        redde I;
    }
    si (silva_c89_diagnostica_numerus(sem) != ZEPHYRUM)
    {
        fprintf(stderr, "auspex: diagnostica inexspectata (%d)\n",
            (int)silva_c89_diagnostica_numerus(sem));
        redde I;
    }
    /* globalia 4 -> definitio in indice 4 */
    si (!_sententias_colligere(parsura, IV, sententiae))
    {
        fprintf(stderr, "auspex: corpus non inventum\n");
        redde I;
    }
    si (xar_numerus(sententiae) != xar_numerus(combi))
    {
        fprintf(stderr, "auspex: %d sententiae, %d combi\n",
            (int)xar_numerus(sententiae), (int)xar_numerus(combi));
        redde I;
    }

    pl = fopen(AUSPEX_VIA_TU, "wb");
    si (pl == NIHIL)
    {
        fprintf(stderr, "auspex: %s non apertum (curre ex radice"
            " repositorii)\n", AUSPEX_VIA_TU);
        redde I;
    }
    fprintf(pl, "/* auspicium.c - GENERATUM ab auspice (M0b B)"
        " - noli manu tangere.\n"
        " * Compilatio est iudicium: clang -std=c99 -fsyntax-only.\n"
        " */\n\n");
    fprintf(pl, "%s\n", GLOBALIA);

    per (k = ZEPHYRUM; k < xar_numerus(combi); k++)
    {
        AuspexCombo* combo = (AuspexCombo*)xar_obtinere(combi, k);
        constans SilvaNodus** sen =
            (constans SilvaNodus**)xar_obtinere(sententiae, k);
        constans SilvaNodus* sententia;
        SilvaValor expr_v;
        constans TypusC89* typus;

        sententia = *sen;
        si (sententia->genus
            != (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS)
        {
            fprintf(stderr, "auspex: sententia %d generis %d\n",
                (int)k, (int)sententia->genus);
            redde I;
        }
        expr_v = silva_c89_sententia_expressionis_expressio(
            sententia);
        si (expr_v.genus != SILVA_VALOR_NODUS)
        {
            fprintf(stderr, "auspex: expressio %d deest\n", (int)k);
            redde I;
        }
        typus = silva_c89_typus_expressionis(sem, expr_v.datum.nodus);
        si (typus == NIHIL)
        {
            fprintf(stderr, "auspex: INOPS - expressio %d sine"
                " typo: %s\n", (int)k, combo->textus);
            redde I;
        }
        /* forma monstratorum pigra: internamentum eundem
         * monstratorem NON-constantem reddit (aequalitas
         * monstratorum EST identitas) - via legitima sine
         * exuitione qualium */
        si (typus->genus == TYPUS_C89_MONSTRATOR)
        {
            TypusC89* idem = silva_c89_typus_monstrator(sem,
                typus->datum.monstrator.internum);

            si (idem != typus
                || !silva_c89_formam_computare(sem, idem))
            {
                fprintf(stderr, "auspex: forma expressionis %d"
                    " deest: %s\n", (int)k, combo->textus);
                redde I;
            }
        }
        alioquin si (typus->genus != TYPUS_C89_PRIMITIVUS)
        {
            fprintf(stderr, "auspex: genus %d inexspectatum: %s\n",
                (int)typus->genus, combo->textus);
            redde I;
        }
        si (combo->signi)
        {
            si (typus->genus != TYPUS_C89_PRIMITIVUS)
            {
                fprintf(stderr, "auspex: %d non primitivum: %s\n",
                    (int)k, combo->textus);
                redde I;
            }
            fprintf(pl, "typedef char probatio_%d[((sizeof(%s)"
                " == %d) && ((((%s) * 0 - 1) > 0) == %d))"
                " ? 1 : -1];\n",
                (int)k, combo->textus,
                (int)typus->magnitudo_octetorum, combo->textus,
                _primitivum_insignatum(typus->datum.primitivum)
                    ? 1 : 0);
            assertiones += II;
        }
        alioquin
        {
            fprintf(pl, "typedef char probatio_%d[(sizeof(%s)"
                " == %d) ? 1 : -1];\n",
                (int)k, combo->textus,
                (int)typus->magnitudo_octetorum);
            assertiones += I;
        }
    }
    fclose(pl);
    imprimere("auspex: %d expressiones, %d assertiones -> %s\n",
        (int)xar_numerus(combi), (int)assertiones, AUSPEX_VIA_TU);
    redde ZEPHYRUM;
}
