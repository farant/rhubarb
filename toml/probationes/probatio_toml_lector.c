/* probatio_toml_lector.c - Porta lectoris toml
 *
 * Quisque casus: fons, modi (character per petitionem: 'c' CLAVIS,
 * 'v' VALOR; ultimus repetitur), profunditas uncorum, lexemata
 * exspectata ut "TITULUS|octeti". Assertur genus et octeti cuiusque
 * lexematis, deinde TEGUMENTUM OCTETORUM: concatenatio omnium = fons,
 * FINIS ultimum. Deinde sedes (linea, columna) et situs reponendus.
 */

#include "latina.h"
#include "credo.h"
#include "toml_lector.h"
#include "toml_lexicon.h"
#include "materia_lexicon.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

#define EXSPECTATA_MAXIMA XXIV

nomen structura {
    constans character* titulus;
    constans character* fons;
    constans character* modi;
                   i32  profunditas;
    constans character* exspectata[EXSPECTATA_MAXIMA];
} Exemplum;

hic_manens constans Exemplum EXEMPLA[] = {
    /* claves */
    { "clavis-nuda", "a = 1", "cccvv", ZEPHYRUM,
      { "CLAVIS_NUDA|a", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "FINIS|", NIHIL } },
    { "clavis-gemina", "\"a b\" = 1", "cccvv", ZEPHYRUM,
      { "CLAVIS_GEMINA|\"a b\"", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "FINIS|", NIHIL } },
    { "clavis-simplex", "'c:\\x' = 1", "cccvv", ZEPHYRUM,
      { "CLAVIS_SIMPLEX|'c:\\x'", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "FINIS|", NIHIL } },
    { "clavis-punctata", "a.b.c = 1", "cccccccvv", ZEPHYRUM,
      { "CLAVIS_NUDA|a", "PUNCTUM|.", "CLAVIS_NUDA|b", "PUNCTUM|.",
        "CLAVIS_NUDA|c", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "FINIS|", NIHIL } },
    { "punctum-spatiatum", "a . b", "c", ZEPHYRUM,
      { "CLAVIS_NUDA|a", "SPATIUM| ", "PUNCTUM|.", "SPATIUM| ",
        "CLAVIS_NUDA|b", "FINIS|", NIHIL } },
    { "clavis-numeri", "1234 = 1", "cccvv", ZEPHYRUM,
      { "CLAVIS_NUDA|1234", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "FINIS|", NIHIL } },
    { "clavis-true", "true = true", "cccvv", ZEPHYRUM,
      { "CLAVIS_NUDA|true", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "VERUM|true", "FINIS|", NIHIL } },
    /* Review Focus I: clavis numero similis punctata */
    { "clavis-pi", "3.14159 = \"pi\"", "cccccvv", ZEPHYRUM,
      { "CLAVIS_NUDA|3", "PUNCTUM|.", "CLAVIS_NUDA|14159", "SPATIUM| ",
        "SIGNUM|=", "SPATIUM| ", "CHORDA_GEMINA|\"pi\"", "FINIS|",
        NIHIL } },
    { "clavis-dies", "1979-05-27 = 1", "cccvv", ZEPHYRUM,
      { "CLAVIS_NUDA|1979-05-27", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "FINIS|", NIHIL } },

    /* numeri: lexema unum quisque */
    { "numerus-signatus", "+99", "v", ZEPHYRUM,
      { "NUMERUS|+99", "FINIS|", NIHIL } },
    { "numerus-negativus", "-17", "v", ZEPHYRUM,
      { "NUMERUS|-17", "FINIS|", NIHIL } },
    { "numerus-sublineatus", "1_000", "v", ZEPHYRUM,
      { "NUMERUS|1_000", "FINIS|", NIHIL } },
    { "numerus-hex", "0xDEAD_beef", "v", ZEPHYRUM,
      { "NUMERUS|0xDEAD_beef", "FINIS|", NIHIL } },
    { "numerus-octalis", "0o755", "v", ZEPHYRUM,
      { "NUMERUS|0o755", "FINIS|", NIHIL } },
    { "numerus-binarius", "0b1101", "v", ZEPHYRUM,
      { "NUMERUS|0b1101", "FINIS|", NIHIL } },
    { "fluitans", "3.14", "v", ZEPHYRUM,
      { "NUMERUS|3.14", "FINIS|", NIHIL } },
    { "fluitans-negativus", "-0.01", "v", ZEPHYRUM,
      { "NUMERUS|-0.01", "FINIS|", NIHIL } },
    { "fluitans-exponens", "5e+22", "v", ZEPHYRUM,
      { "NUMERUS|5e+22", "FINIS|", NIHIL } },
    { "fluitans-exponens-zephyrum", "1e06", "v", ZEPHYRUM,
      { "NUMERUS|1e06", "FINIS|", NIHIL } },
    { "fluitans-parvus", "6.626e-34", "v", ZEPHYRUM,
      { "NUMERUS|6.626e-34", "FINIS|", NIHIL } },
    { "inf", "inf", "v", ZEPHYRUM,
      { "NUMERUS|inf", "FINIS|", NIHIL } },
    { "inf-signatum", "+inf", "v", ZEPHYRUM,
      { "NUMERUS|+inf", "FINIS|", NIHIL } },
    { "nan-negativum", "-nan", "v", ZEPHYRUM,
      { "NUMERUS|-nan", "FINIS|", NIHIL } },
    { "fluitans-longus", "9_224_617.445_991_228_313", "v", ZEPHYRUM,
      { "NUMERUS|9_224_617.445_991_228_313", "FINIS|", NIHIL } },
    /* pravi sed toti: coctio (Q7) recusat */
    { "pravus-sublineae", "1__2", "v", ZEPHYRUM,
      { "NUMERUS|1__2", "FINIS|", NIHIL } },
    { "pravus-hex-vacuus", "0x", "v", ZEPHYRUM,
      { "NUMERUS|0x", "FINIS|", NIHIL } },
    { "pravus-punctum", "1.", "v", ZEPHYRUM,
      { "NUMERUS|1.", "FINIS|", NIHIL } },

    /* tempora */
    { "tempus-zona", "1979-05-27T07:32:00Z", "v", ZEPHYRUM,
      { "TEMPUS|1979-05-27T07:32:00Z", "FINIS|", NIHIL } },
    { "tempus-fractio-zona", "1979-05-27T00:32:00.999999-07:00", "v",
      ZEPHYRUM,
      { "TEMPUS|1979-05-27T00:32:00.999999-07:00", "FINIS|", NIHIL } },
    { "tempus-spatium", "1979-05-27 07:32:00", "v", ZEPHYRUM,
      { "TEMPUS|1979-05-27 07:32:00", "FINIS|", NIHIL } },
    { "dies", "1979-05-27", "v", ZEPHYRUM,
      { "TEMPUS|1979-05-27", "FINIS|", NIHIL } },
    { "hora", "07:32:00", "v", ZEPHYRUM,
      { "TEMPUS|07:32:00", "FINIS|", NIHIL } },
    { "hora-fractio", "00:32:00.999999", "v", ZEPHYRUM,
      { "TEMPUS|00:32:00.999999", "FINIS|", NIHIL } },
    { "dies-commentum", "1979-05-27 # c", "v", ZEPHYRUM,
      { "TEMPUS|1979-05-27", "SPATIUM| ", "COMMENTUM|# c", "FINIS|",
        NIHIL } },
    { "tempus-sine-secundis", "1979-05-27T07:32", "v", ZEPHYRUM,
      { "TEMPUS|1979-05-27T07:32", "FINIS|", NIHIL } },
    /* spatium iungit SOLUM post diem integrum: decem characteres qui
     * dies non sunt non iunguntur */
    { "numerus-non-dies", "1234567890 12:00:00", "v", ZEPHYRUM,
      { "NUMERUS|1234567890", "SPATIUM| ", "TEMPUS|12:00:00", "FINIS|",
        NIHIL } },

    /* chordae */
    { "gemina-effugium", "\"a\\\"b\"", "v", ZEPHYRUM,
      { "CHORDA_GEMINA|\"a\\\"b\"", "FINIS|", NIHIL } },
    { "gemina-multa", "\"\"\"a\n\"\"b\"\"\"", "v", ZEPHYRUM,
      { "CHORDA_GEMINA_MULTA|\"\"\"a\n\"\"b\"\"\"", "FINIS|", NIHIL } },
    { "gemina-multa-quinque", "\"\"\"a\"\"\"\"\"", "v", ZEPHYRUM,
      { "CHORDA_GEMINA_MULTA|\"\"\"a\"\"\"\"\"", "FINIS|", NIHIL } },
    { "gemina-multa-aperta", "\"\"\"x", "v", ZEPHYRUM,
      { "CHORDA_GEMINA_MULTA|\"\"\"x", "FINIS|", NIHIL } },
    { "simplex-multa", "'''a'''", "v", ZEPHYRUM,
      { "CHORDA_SIMPLEX_MULTA|'''a'''", "FINIS|", NIHIL } },
    { "simplex-sine-effugio", "'a\\b'", "v", ZEPHYRUM,
      { "CHORDA_SIMPLEX|'a\\b'", "FINIS|", NIHIL } },
    { "gemina-aperta", "\"abc\nb", "vvc", ZEPHYRUM,
      { "CHORDA_GEMINA|\"abc", "LINEA_FINIS|\n", "CLAVIS_NUDA|b",
        "FINIS|", NIHIL } },

    /* lineae novae */
    { "linea-finis", "a = 1\n", "cccvvc", ZEPHYRUM,
      { "CLAVIS_NUDA|a", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "LINEA_FINIS|\n", "FINIS|", NIHIL } },
    { "linea-in-serie", "[1,\n2]", "v", I,
      { "SERIES_APERTURA|[", "NUMERUS|1", "COMMA|,", "LINEA|\n",
        "NUMERUS|2", "SERIES_CLAUSURA|]", "FINIS|", NIHIL } },
    { "crlf-finis", "a = 1\r\n", "cccvvc", ZEPHYRUM,
      { "CLAVIS_NUDA|a", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "LINEA_FINIS|\r\n", "FINIS|", NIHIL } },
    { "crlf-in-serie", "[1,\r\n2]", "v", I,
      { "SERIES_APERTURA|[", "NUMERUS|1", "COMMA|,", "LINEA|\r\n",
        "NUMERUS|2", "SERIES_CLAUSURA|]", "FINIS|", NIHIL } },
    /* Review Focus III: CR solus */
    { "cr-solus", "a = 1\rb", "cccvvvc", ZEPHYRUM,
      { "CLAVIS_NUDA|a", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "IGNOTUM|\r", "CLAVIS_NUDA|b", "FINIS|", NIHIL } },

    /* commentaria: octeti intra commentum manent (coctio iudicat) */
    { "commentum-caudale", "a = 1 # c", "cccvv", ZEPHYRUM,
      { "CLAVIS_NUDA|a", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "SPATIUM| ", "COMMENTUM|# c", "FINIS|", NIHIL } },
    { "commentum-tabula", "#\tx", "c", ZEPHYRUM,
      { "COMMENTUM|#\tx", "FINIS|", NIHIL } },
    { "commentum-del", "#\x7f x\n", "c", ZEPHYRUM,
      { "COMMENTUM|#\x7f x", "LINEA_FINIS|\n", "FINIS|", NIHIL } },
    { "commentum-utf8-pravum", "#\xc0\xaf\n", "c", ZEPHYRUM,
      { "COMMENTUM|#\xc0\xaf", "LINEA_FINIS|\n", "FINIS|", NIHIL } },

    /* ignota: extra commenta et chordas */
    { "utf8-pravum-clavis", "\xc0\xaf = 1", "cccvv", ZEPHYRUM,
      { "IGNOTUM|\xc0\xaf", "SPATIUM| ", "SIGNUM|=", "SPATIUM| ",
        "NUMERUS|1", "FINIS|", NIHIL } },
    /* Review Focus IV: BOM */
    { "bom", "\xef\xbb\xbf" "a = 1", "ccccvv", ZEPHYRUM,
      { "IGNOTUM|\xef\xbb\xbf", "CLAVIS_NUDA|a", "SPATIUM| ",
        "SIGNUM|=", "SPATIUM| ", "NUMERUS|1", "FINIS|", NIHIL } },
    { "verbum-in-valore", "hello", "v", ZEPHYRUM,
      { "IGNOTUM|hello", "FINIS|", NIHIL } },

    /* capita */
    { "caput-tabulae", "[a.b]", "c", ZEPHYRUM,
      { "TABULA_APERTURA|[", "CLAVIS_NUDA|a", "PUNCTUM|.",
        "CLAVIS_NUDA|b", "TABULA_CLAUSURA|]", "FINIS|", NIHIL } },
    { "caput-seriei", "[[p]]", "c", ZEPHYRUM,
      { "SERIES_TABULARUM_APERTURA|[[", "CLAVIS_NUDA|p",
        "SERIES_TABULARUM_CLAUSURA|]]", "FINIS|", NIHIL } },
    { "caput-spatiatum", "[ a ]", "c", ZEPHYRUM,
      { "TABULA_APERTURA|[", "SPATIUM| ", "CLAVIS_NUDA|a", "SPATIUM| ",
        "TABULA_CLAUSURA|]", "FINIS|", NIHIL } },

    /* tabula compacta: modi ut aedificator eos eliget */
    { "tabula-compacta", "{x = 1, y = 2}", "vcccvvcccccvvc", I,
      { "COMPACTA_APERTURA|{", "CLAVIS_NUDA|x", "SPATIUM| ", "SIGNUM|=",
        "SPATIUM| ", "NUMERUS|1", "COMMA|,", "SPATIUM| ",
            "CLAVIS_NUDA|y",
        "SPATIUM| ", "SIGNUM|=", "SPATIUM| ", "NUMERUS|2",
        "COMPACTA_CLAUSURA|}", "FINIS|", NIHIL } }
};

/* "TITULUS|octeti" contra lexema */
interior b32
_congruit (
    constans MateriaLexiconRatum* ratum,
                    MateriaToken* t,
              constans character* exspectatum)
{
    constans character* virgula = strchr(exspectatum, '|');
    constans character* titulus;
                   i32  lt;
                   i32  lv;

    si (virgula == NIHIL || t == NIHIL)
    {
        redde FALSUM;
    }
    lt       = (i32)(virgula - exspectatum);
    lv       = (i32)strlen(virgula + I);
    titulus  = materia_lexicon_titulus(ratum, t->genus);
    si (   titulus == NIHIL || (i32)strlen(titulus) != lt
        || strncmp(titulus, exspectatum, (size_t)lt) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde t->valor.mensura == lv
        && memcmp(t->valor.datum, virgula + I, (size_t)lv) == ZEPHYRUM;
}

/* fons in piscina (lexemata in eum monstrant - numquam acervus) */
interior constans character*
_fons (
               Piscina* piscina,
    constans character* textus)
{
          i32  n = (i32)strlen(textus);
    character* f = (character*)piscina_allocare(piscina, (i64)(n + I));

    memcpy(f, textus, (size_t)(n + I));
    redde f;
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;
                i32  octeti_tecti = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("probatio_toml_lector", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &TOML_LEXICON,
        &iudicium));


    /* ==================================================
     * PROBARE: exempla - genera, octeti, tegumentum
     * ================================================== */

    {
        i32 e;
        i32 numerus = (i32)(magnitudo(EXEMPLA) / magnitudo(EXEMPLA[0]));

        imprimere("\n--- Probans exempla lectoris (%u) ---\n", numerus);
        CREDO_VERUM (numerus >= XL);

        per (e = ZEPHYRUM; e < numerus; e++)
        {
             constans Exemplum* x        = &EXEMPLA[e];
            constans character* fons     = _fons(piscina, x->fons);
                           s32  mensura  = (s32)strlen(x->fons);
                    TomlLector  l;
                           i32  k;
                           i32  modi    = (i32)strlen(x->modi);
                           s32  tectum  = ZEPHYRUM;
                           b32  sanum   = VERUM;

            CREDO_VERUM (toml_lector_incipere(&l, piscina, fons,
                mensura));
            toml_lector_profunditatem_ponere(&l, x->profunditas);
            per (k = ZEPHYRUM; k < EXSPECTATA_MAXIMA; k++)
            {
                TomlModus m = (x->modi[k < modi ? k : modi - I] == 'v')
                    ? TOML_MODUS_VALOR : TOML_MODUS_CLAVIS;
                MateriaToken* t = toml_lector_proximum(&l, m);

                si (t == NIHIL || x->exspectata[k] == NIHIL)
                {
                    sanum = FALSUM;
                    frange;
                }
                si (!_congruit(&ratum, t, x->exspectata[k]))
                {
                    constans character* ti =
                        materia_lexicon_titulus(&ratum, t->genus);

                    imprimere("  %s [%u]: exspectatum '%s', redditum "
                        "'%s|%.*s'\n", x->titulus, k, x->exspectata[k],
                        ti != NIHIL ? ti : "?",
                        (integer)t->valor.mensura,
                        (constans character*)t->valor.datum);
                    sanum = FALSUM;
                    frange;
                }
                /* tegumentum: lexema incipit ubi prius desiit */
                si (t->byte_offset != tectum)
                {
                    imprimere("  %s [%u]: lacuna ad %d (lexema %d)\n",
                        x->titulus, k, (integer)tectum,
                        (integer)t->byte_offset);
                    sanum = FALSUM;
                    frange;
                }
                tectum = t->byte_offset + (s32)t->valor.mensura;
                si (t->genus == (s32)TOML_LEX_FINIS)
                {
                    si (x->exspectata[k + I] != NIHIL)
                    {
                        imprimere("  %s: FINIS praematurus\n",
                            x->titulus);
                        sanum = FALSUM;
                    }
                    frange;
                }
            }
            CREDO_VERUM (sanum);
            CREDO_AEQUALIS_S32 (tectum, mensura);
            octeti_tecti += (i32)tectum;
        }
        imprimere("  octeti tecti: %u\n", octeti_tecti);
    }


    /* ==================================================
     * PROBARE: sedes (linea, columna) et situs reponendus
     * ================================================== */

    {
        constans character* fons = _fons(piscina,
            "a = 1\nb = 2\n  c = 3");
                 TomlLector l;
                  TomlSitus s;
              MateriaToken* t;
              MateriaToken* u;
                       i32  k;
        /* lexema, linea, columna: omnia in modo CLAVIS/VALOR alterno */
        hic_manens constans i32 SEDES[][II] = {
            { I, I }, { I, II }, { I, III }, { I, IV }, { I, V },
            { I, VI }, { II, I }, { II, II }, { II, III }, { II, IV },
            { II, V }, { II, VI }, { III, I }, { III, III }, { III,
                IV },
            { III, V }, { III, VI }, { III, VII }
        };

        imprimere("\n--- Probans sedes et situm ---\n");
        CREDO_VERUM (toml_lector_incipere(&l, piscina, fons,
            (s32)strlen(fons)));
        per (k = ZEPHYRUM; k < (i32)(magnitudo(SEDES)
            / magnitudo(SEDES[0])); k++)
        {
            t = toml_lector_proximum(&l, TOML_MODUS_CLAVIS);
            CREDO_NON_NIHIL (t);
            si (t == NIHIL)
            {
                frange;
            }
            si (   t->linea   != SEDES[k][ZEPHYRUM]
                || t->columna != SEDES[k][I])
            {
                imprimere("  lexema %u '%.*s': %u:%u, "
                    "exspectatum %u:%u\n",
                    k, (integer)t->valor.mensura,
                    (constans character*)t->valor.datum, t->linea,
                    t->columna, SEDES[k][ZEPHYRUM], SEDES[k][I]);
            }
            CREDO_AEQUALIS_I32 (t->linea, SEDES[k][ZEPHYRUM]);
            CREDO_AEQUALIS_I32 (t->columna, SEDES[k][I]);
        }

        /* situs servatus -> reponitur -> idem lexema iterum */
        CREDO_VERUM (toml_lector_incipere(&l, piscina, fons,
            (s32)strlen(fons)));
        (vacuum)toml_lector_proximum(&l, TOML_MODUS_CLAVIS);
        s = toml_lector_situs(&l);
        t = toml_lector_proximum(&l, TOML_MODUS_CLAVIS);
        (vacuum)toml_lector_proximum(&l, TOML_MODUS_CLAVIS);
        toml_lector_situm_reponere(&l, s);
        u = toml_lector_proximum(&l, TOML_MODUS_CLAVIS);
        CREDO_NON_NIHIL (t);
        CREDO_NON_NIHIL (u);
        si (t != NIHIL && u != NIHIL)
        {
            CREDO_AEQUALIS_S32 (t->byte_offset, u->byte_offset);
            CREDO_AEQUALIS_S32 (t->genus, u->genus);
            CREDO_AEQUALIS_I32 (t->valor.mensura, u->valor.mensura);
        }
    }


    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
