/* probatio_toml_scalaris.c - Coctio scalarium toml (Q7a)
 *
 * Casus inlinei: fons 'a = X' (aut clavis quotata) -> arbor ->
 * toml_scalarem_coquere: valor assertus (integer, fluitans, boolean,
 * chorda decodita, tempus per campos); vitia per
 * toml_scalaria_iudicare (dominus diagnosticorum): codex, OFFSET
 * vitii ipsius (non lexematis), numerus diagnosticorum (omne vitium
 * chordae, non primum), ordo octetorum.
 *
 * Corpora: toml-test valida omnia SINE diagnostico scalari; invalida
 * generum scalarium (control, encoding, string, integer, float,
 * datetime, local-*) omnia reiecta - syntaxi aut coctione; domus sine
 * diagnostico; silvestria numerantur. Verdictum BOM (Q1) pinnatum.
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_registrum.h"
#include "toml_scalaris.h"
#include "toml_corpus_ambulare.h"
#include "materia_diagnosticum.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S64_MAXIMUS ((s64)(((i64)I << LXIII) - I))
#define S64_MINIMUS (-S64_MAXIMUS - I)


/* ==================================================
 * Adiumenta
 * ================================================== */

interior MateriaNodus*
_radix (
               Piscina* piscina,
    constans character* litterae,
                   s32  mensura)
{
    character* fons = (character*)piscina_allocare(piscina,
                          (i64)mensura + I);

    memcpy(fons, litterae, (size_t)mensura);
    fons[mensura] = '\0';
    redde toml_arbor_parsare(piscina, fons, mensura, NIHIL);
}

/* nodus primus generis 'genus' in liberis documenti */
interior MateriaNodus*
_sententia (
    constans MateriaNodus* radix,
                      s32  genus)
{
    MateriaValor l;
             i32 k;

    si (radix == NIHIL)
    {
        redde NIHIL;
    }
    l = radix->loci[TOML_DOCUMENTUM_LIBERI];
    si (l.genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(l); k++)
    {
        MateriaValor* e = materia_valor_lista_obtinere(l, k);

        si (   e->genus              == MATERIA_VALOR_NODUS
            && e->datum.nodus->genus == genus)
        {
            redde e->datum.nodus;
        }
    }
    redde NIHIL;
}

/* valor paris primi coctus; diagnostica scalaria documenti in *dd */
interior b32
_coquere (
               Piscina*  piscina,
    constans character*  litterae,
             TomlValor*  v,
                   Xar** dd)
{
    MateriaNodus* radix = _radix(piscina, litterae,
                              (s32)strlen(litterae));
    MateriaNodus* par   = _sententia(radix, (s32)TOML_GENUS_PAR);

    memset(v, ZEPHYRUM, magnitudo(TomlValor));
    *dd = radix != NIHIL ? toml_scalaria_iudicare(piscina, radix)
        : NIHIL;
    si (   par == NIHIL || par->loci[TOML_PAR_VALOR].genus
        != MATERIA_VALOR_NODUS)
    {
        imprimere("    casus sine valore: %s\n", litterae);
        redde FALSUM;
    }
    redde toml_scalarem_coquere(piscina,
        par->loci[TOML_PAR_VALOR].datum.nodus, v, NIHIL);
}

interior i32
_numerus_diagnosticorum (
    Xar* dd)
{
    redde dd != NIHIL ? xar_numerus(dd) : ZEPHYRUM;
}

interior constans MateriaDiagnosticum*
_diagnosticum (
    Xar* dd,
    i32  k)
{
    redde (constans MateriaDiagnosticum*)xar_obtinere(dd, k);
}


/* ==================================================
 * Tabulae casuum
 * ================================================== */

nomen structura {
    constans character* fons;
                   s64  valor;
} CasusIntegri;

hic_manens constans CasusIntegri INTEGRI[] = {
    { "a = 0",                     ZEPHYRUM },
    { "a = +0",                    ZEPHYRUM },
    { "a = -0",                    ZEPHYRUM },
    { "a = 42",                    XLII },
    { "a = -17",                   -XVII },
    { "a = 1_000",                 M },
    { "a = 5_349_221",             (s64)5349221L },
    { "a = 0xDEADbeef",            (s64)3735928559UL },
    { "a = 0o755",                 CDXCIII },
    { "a = 0b1101",                XIII },
    { "a = 0o0",                   ZEPHYRUM },
    { "a = 9223372036854775807",   S64_MAXIMUS },
    { "a = -9223372036854775808",  S64_MINIMUS },
    { "a = 0x7FFFFFFFFFFFFFFF",    S64_MAXIMUS },
    { NIHIL,                       ZEPHYRUM }
};

nomen structura {
    constans character* fons;
                   f64  valor;
                   b32  negativum;   /* signum (etiam -0.0, -nan) */
                   i32  species;     /* 0 finitum, 1 inf, 2 nan */
} CasusFluitantis;

hic_manens constans CasusFluitantis FLUITANTES[] = {
    { "a = 1.5",          1.5,        FALSUM, ZEPHYRUM },
    { "a = -0.0",         0.0,        VERUM,  ZEPHYRUM },
    { "a = +0.0",         0.0,        FALSUM, ZEPHYRUM },
    { "a = 3.14159",      3.14159,    FALSUM, ZEPHYRUM },
    { "a = 6.02e+23",     6.02e+23,   FALSUM, ZEPHYRUM },
    { "a = 1e6",          1e6,        FALSUM, ZEPHYRUM },
    { "a = 1E-2",         1e-2,       FALSUM, ZEPHYRUM },
    { "a = 6.626e-34",    6.626e-34,  FALSUM, ZEPHYRUM },
    { "a = 224_617.445_991", 224617.445991, FALSUM, ZEPHYRUM },
    { "a = 0e0",          0.0,        FALSUM, ZEPHYRUM },
    { "a = -1_0.5e1_0",   -10.5e10,   VERUM,  ZEPHYRUM },
    { "a = inf",          0.0,        FALSUM, I },
    { "a = +inf",         0.0,        FALSUM, I },
    { "a = -inf",         0.0,        VERUM,  I },
    { "a = nan",          0.0,        FALSUM, II },
    { "a = +nan",         0.0,        FALSUM, II },
    { "a = -nan",         0.0,        VERUM,  II },
    { NIHIL,              0.0,        FALSUM, ZEPHYRUM }
};

nomen structura {
    constans character* fons;
    constans character* valor;       /* octeti decoditi */
                   i32  mensura;     /* -> strlen si 0 */
} CasusChordae;

hic_manens constans CasusChordae CHORDAE[] = {
    { "a = \"\"",                               "",          ZEPHYRUM },
    { "a = \"x\\ty\"",                          "x\ty",      ZEPHYRUM },
    { "a = \"\\b\\t\\n\\f\\r\\\"\\\\\"",        "\b\t\n\f\r\"\\",
        ZEPHYRUM },
    { "a = \"\\u00E9\"",                        "\303\251",  ZEPHYRUM },
    { "a = \"\\U0001F600\"",                    "\360\237\230\200",
        ZEPHYRUM },
    { "a = \"\\u0000\"",                        "",          I },
    { "a = \"tab\tliteral\"",                   "tab\tliteral",
        ZEPHYRUM },
    { "a = \"caf\303\251\"",                    "caf\303\251",
        ZEPHYRUM },
    { "a = 'C:\\Users\\x'",                     "C:\\Users\\x",
        ZEPHYRUM },
    { "a = '<\\i\\c*\\s*>'",                    "<\\i\\c*\\s*>",
        ZEPHYRUM },
    { "a = \"\"\"\nabc\"\"\"",                  "abc",       ZEPHYRUM },
    { "a = \"\"\"abc\n\"\"\"",                  "abc\n",     ZEPHYRUM },
    { "a = \"\"\"\nThe quick \\\n\n   brown\"\"\"", "The quick brown",
        ZEPHYRUM },
    { "a = \"\"\"x\\   \n  y\"\"\"",            "xy",        ZEPHYRUM },
    { "a = \"\"\"\r\nx\r\ny\"\"\"",             "x\ny",      ZEPHYRUM },
    { "a = \"\"\"x\\\r\n  y\"\"\"",             "xy",        ZEPHYRUM },
    { "a = \"\"\"a\"\"b\"\"\"\"\"",             "a\"\"b\"\"",
        ZEPHYRUM },
    { "a = '''\nraw\\n'''",                     "raw\\n",    ZEPHYRUM },
    { "a = '''''two'''''",                      "''two''",   ZEPHYRUM },
    { "a = '''x\r\ny'''",                       "x\ny",      ZEPHYRUM },
    { NIHIL,                                    NIHIL,       ZEPHYRUM }
};

nomen structura {
    constans character* fons;
     TomlGenusTemporis  genus;
                   s32  campi[VIII];  /* annus..nanosecunda, zona */
} CasusTemporis;

hic_manens constans CasusTemporis TEMPORA[] = {
    { "a = 1979-05-27T07:32:00Z", TOML_TEMPUS_CUM_ZONA,
      { MCMLXXIX, V, XXVII, VII, XXXII, ZEPHYRUM, ZEPHYRUM,
          ZEPHYRUM } },
    { "a = 1979-05-27T00:32:00.999999-07:00", TOML_TEMPUS_CUM_ZONA,
      { MCMLXXIX, V, XXVII, ZEPHYRUM, XXXII, ZEPHYRUM,
        (s32)999999000L, -CDXX } },
    { "a = 1979-05-27 07:32:00+05:30", TOML_TEMPUS_CUM_ZONA,
      { MCMLXXIX, V, XXVII, VII, XXXII, ZEPHYRUM, ZEPHYRUM, CCCXXX } },
    { "a = 1979-05-27t07:32:00z", TOML_TEMPUS_CUM_ZONA,
      { MCMLXXIX, V, XXVII, VII, XXXII, ZEPHYRUM, ZEPHYRUM,
          ZEPHYRUM } },
    { "a = 1979-05-27T07:32:00", TOML_TEMPUS_LOCALE,
      { MCMLXXIX, V, XXVII, VII, XXXII, ZEPHYRUM, ZEPHYRUM,
          ZEPHYRUM } },
    { "a = 1979-05-27", TOML_DIES_LOCALIS,
      { MCMLXXIX, V, XXVII, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
        ZEPHYRUM } },
    { "a = 07:32:00.1234567891", TOML_HORA_LOCALIS,
      { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, VII, XXXII, ZEPHYRUM,
        (s32)123456789L, ZEPHYRUM } },
    { "a = 2024-02-29", TOML_DIES_LOCALIS,
      { MMXXIV, II, XXIX, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
        ZEPHYRUM } },
    { "a = 2000-02-29", TOML_DIES_LOCALIS,
      { MM, II, XXIX, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
        ZEPHYRUM } },
    { "a = 23:59:60", TOML_HORA_LOCALIS,
      { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, XXIII, LIX, LX, ZEPHYRUM,
        ZEPHYRUM } },
    { NIHIL, TOML_TEMPUS_CUM_ZONA,
      { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
        ZEPHYRUM, ZEPHYRUM } }
};

/* vitia: codex primi, offset eius in fonte, numerus diagnosticorum */
nomen structura {
    constans character* fons;
    constans character* codex;
                   s32  initium;
                   i32  numerus;
} CasusVitii;

hic_manens constans CasusVitii VITIA[] = {
    /* integri */
    { "a = 007",                    TOML_CODEX_INTEGER,     IV,   I },
    { "a = +01",                    TOML_CODEX_INTEGER,     V,    I },
    { "a = 0X1F",                   TOML_CODEX_INTEGER,     V,    I },
    { "a = +0x1",                   TOML_CODEX_INTEGER,     IV,   I },
    { "a = -0o7",                   TOML_CODEX_INTEGER,     IV,   I },
    { "a = 0x",                     TOML_CODEX_INTEGER,     IV,   I },
    { "a = 0o8",                    TOML_CODEX_INTEGER,     VI,   I },
    { "a = 0b102",                  TOML_CODEX_INTEGER,     VIII, I },
    { "a = 0xg",                    TOML_CODEX_INTEGER,     VI,   I },
    { "a = 1__2",                   TOML_CODEX_INTEGER,     V,    I },
    { "a = 1_",                     TOML_CODEX_INTEGER,     V,    I },
    { "a = 0x_ff",                  TOML_CODEX_INTEGER,     VI,   I },
    { "a = --1",                    TOML_CODEX_INTEGER,     V,    I },
    { "a = +_1",                    TOML_CODEX_INTEGER,     V,    I },
    { "a = 1x",                     TOML_CODEX_INTEGER,     V,    I },
    { "a = 9223372036854775808",    TOML_CODEX_EXTRA_FINES, IV,   I },
    { "a = -9223372036854775809",   TOML_CODEX_EXTRA_FINES, IV,   I },
    { "a = 0x8000000000000000",     TOML_CODEX_EXTRA_FINES, IV,   I },
    { "a = 99999999999999999999999", TOML_CODEX_EXTRA_FINES, IV,  I },
    /* fluitantes */
    { "a = 1.",                     TOML_CODEX_FLUITANS,    V,    I },
    { "a = +.5",                    TOML_CODEX_FLUITANS,    V,    I },
    { "a = 1.e3",                   TOML_CODEX_FLUITANS,    V,    I },
    { "a = 1e",                     TOML_CODEX_FLUITANS,    V,    I },
    { "a = 1e+",                    TOML_CODEX_FLUITANS,    V,    I },
    { "a = 1.2.3",                  TOML_CODEX_FLUITANS,    VII,  I },
    { "a = 1e2.3",                  TOML_CODEX_FLUITANS,    VII,  I },
    { "a = 1ee2",                   TOML_CODEX_FLUITANS,    V,    I },
    { "a = 1._5",                   TOML_CODEX_FLUITANS,    VI,    I },
    { "a = 1_.5",                   TOML_CODEX_FLUITANS,    V,    I },
    { "a = 1.5_",                   TOML_CODEX_FLUITANS,    VII,  I },
    { "a = 1e_2",                   TOML_CODEX_FLUITANS,    VI,   I },
    { "a = 03.14",                  TOML_CODEX_FLUITANS,    IV,   I },
    { "a = -01.5",                  TOML_CODEX_FLUITANS,    V,    I },
    { "a = +in",                    TOML_CODEX_FLUITANS,    IV,   I },
    { "a = nanx",                   TOML_CODEX_FLUITANS,    IV,   I },
    { "a = infinity",               TOML_CODEX_FLUITANS,    IV,   I },
    /* tempora: limites */
    { "a = 2023-02-29",             TOML_CODEX_TEMPUS_LIMITES, XII, I },
    { "a = 1900-02-29",             TOML_CODEX_TEMPUS_LIMITES, XII, I },
    { "a = 2006-04-31",             TOML_CODEX_TEMPUS_LIMITES, XII, I },
    { "a = 2006-13-01",             TOML_CODEX_TEMPUS_LIMITES, IX,  I },
    { "a = 2006-00-01",             TOML_CODEX_TEMPUS_LIMITES, IX,  I },
    { "a = 2006-01-00",             TOML_CODEX_TEMPUS_LIMITES, XII, I },
    { "a = 24:00:00",               TOML_CODEX_TEMPUS_LIMITES, IV,  I },
    { "a = 12:60:00",               TOML_CODEX_TEMPUS_LIMITES, VII, I },
    { "a = 12:00:61",               TOML_CODEX_TEMPUS_LIMITES, X,   I },
    { "a = 1985-06-18T17:04:07+24:00", TOML_CODEX_TEMPUS_LIMITES,
      XXIV, I },
    { "a = 1985-06-18T17:04:07-00:60", TOML_CODEX_TEMPUS_LIMITES,
      XXVII, I },
    /* tempora: forma */
    { "a = 1987-7-05",              TOML_CODEX_TEMPUS_FORMA, IX,   I },
    { "a = 1987-07-5",              TOML_CODEX_TEMPUS_FORMA, XII,  I },
    { "a = 1987-07-05T17:45",       TOML_CODEX_TEMPUS_FORMA, XX,   I },
    { "a = 17:45",                  TOML_CODEX_TEMPUS_FORMA, IX,   I },
    { "a = 1987-07-0517:45:00Z",    TOML_CODEX_TEMPUS_FORMA, XIV,  I },
    { "a = 2006-01-30T",            TOML_CODEX_TEMPUS_FORMA, XIV,  I },
    { "a = 12:13:14.",              TOML_CODEX_TEMPUS_FORMA, XII,  I },
    { "a = 12:13:14..",             TOML_CODEX_TEMPUS_FORMA, XII,  I },
    { "a = 2016-09-09T09:09:09.Z",  TOML_CODEX_TEMPUS_FORMA, XXIII, I },
    { "a = 1979-05-27T07:32:00+5:00", TOML_CODEX_TEMPUS_FORMA,
      XXIV, I },
    { "a = 1979-05-27T07:32:00+05", TOML_CODEX_TEMPUS_FORMA, XXVI, I },
    { "a = 2020-01-01x",            TOML_CODEX_TEMPUS_FORMA, XIV,  I },
    { "a = 07:32:00Z",              TOML_CODEX_TEMPUS_FORMA, XII,  I },
    /* chordae: effugia (omnia, non primum) */
    { "a = \"\\x33\"",              TOML_CODEX_EFFUGIUM,    V,    I },
    { "a = \"\\/\"",                TOML_CODEX_EFFUGIUM,    V,    I },
    { "a = \"\\e\"",                TOML_CODEX_EFFUGIUM,    V,    I },
    { "a = \"\\q \\z\"",            TOML_CODEX_EFFUGIUM,    V,    II },
    { "a = \"\"\"\nfoo \\ x\"\"\"", TOML_CODEX_EFFUGIUM,    XII,  I },
    { "a = \"\"\"\ngee \\   \"\"\"", TOML_CODEX_EFFUGIUM,   XII,  I },
    { "a = \"\\u12\"",              TOML_CODEX_EFFUGIUM_MANCUM, V,
        I },
    { "a = \"\\U0001F6\"",          TOML_CODEX_EFFUGIUM_MANCUM, V,
        I },
    { "a = \"\\uD800\"",            TOML_CODEX_PUNCTUM_CODICIS, V, I },
    { "a = \"\\uDFFF\"",            TOML_CODEX_PUNCTUM_CODICIS, V, I },
    { "a = \"\\U00110000\"",        TOML_CODEX_PUNCTUM_CODICIS, V, I },
    /* chorda non clausa: punctum in fine lexematis (Q7b lacuna) */
    { "a = \"abc",                  TOML_CODEX_CHORDA_APERTA, VIII, I },
    { "a = '''abc",                TOML_CODEX_CHORDA_APERTA, X,  I },
    { "a = \"\"\"abc\n",            TOML_CODEX_CHORDA_APERTA, XI, I },
    { "\"abc = 1",                  TOML_CODEX_CHORDA_APERTA, VIII, I },
    { "\"\\q\" = 1",                TOML_CODEX_EFFUGIUM,    I,    I },
    /* octeti moderantes et UTF-8 */
    { "a = \"x\001y\"",             TOML_CODEX_OCTETUS_MODERANS, VI,
        I },
    { "a = 'x\177y'",               TOML_CODEX_OCTETUS_MODERANS, VI,
        I },
    { "a = \"\"\"x\ry\"\"\"",       TOML_CODEX_OCTETUS_MODERANS, VIII,
        I },
    { "a = '''x\010y'''",           TOML_CODEX_OCTETUS_MODERANS, VIII,
        I },
    { "a = \"x\ry\"",               TOML_CODEX_OCTETUS_MODERANS, VI,
        I },
    { "a = 1 # x\001",              TOML_CODEX_OCTETUS_MODERANS, IX,
        I },
    { "# \177\na = 1",              TOML_CODEX_OCTETUS_MODERANS, II,
        I },
    { "a = 1 # x\ry",               TOML_CODEX_OCTETUS_MODERANS, IX,
        I },
    { "# \303(",                    TOML_CODEX_UTF8,        II,   I },
    { "a = \"\377\"",               TOML_CODEX_UTF8,        V,    I },
    { "a = '\355\240\200'",         TOML_CODEX_UTF8,        V,    I },
    { "a = \"\300\257\"",           TOML_CODEX_UTF8,        V,    I },
    { "a = \"\360\237\230\"",       TOML_CODEX_UTF8,        V,    I },
    { NIHIL,                        NIHIL,                  ZEPHYRUM,
        ZEPHYRUM }
};


/* ==================================================
 * Corpora
 * ================================================== */

hic_manens constans character* GENERA_SCALARIA[] = {
    "invalid/control/", "invalid/encoding/", "invalid/string/",
    "invalid/integer/", "invalid/float/", "invalid/datetime/",
    "invalid/local-datetime/", "invalid/local-date/",
    "invalid/local-time/", NIHIL
};

nomen structura {
    i32 valida;
    i32 valida_cum_vitio;
    i32 invalida_scalaria;
    i32 invalida_scalaria_accepta;
    i32 invalida_syntaxi;
    i32 invalida_coctione;
    i32 domus;
    i32 domus_cum_vitio;
    i32 silvestria;
    i32 silvestria_cum_vitio;
    i32 ordo_ruptus;
    i32 nominati;
} Status;

interior b32
_scalare (
    constans character* via)
{
    i32 k;

    per (k = ZEPHYRUM; GENERA_SCALARIA[k] != NIHIL; k++)
    {
        si (strncmp(via, GENERA_SCALARIA[k],
                strlen(GENERA_SCALARIA[k])) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_nominare (
                Status* st,
    constans character* titulus,
    constans character* via,
                   Xar* dd)
{
    si (st->nominati >= X)
    {
        redde;
    }
    st->nominati++;
    imprimere("    %s: %s", titulus, via);
    si (_numerus_diagnosticorum(dd) > ZEPHYRUM)
    {
        constans MateriaDiagnosticum* d = _diagnosticum(dd, ZEPHYRUM);

        imprimere(" - %s @%d: %s", d->codex,
            (integer)d->tractus.initium,
            d->causa);
    }
    imprimere("\n");
}

interior vacuum
_visor (
                 vacuum* datum,
         TomlCorpusFons  fons,
     constans character* via,
                 chorda  textus,
                Piscina* opus)
{
          Status* st = (Status*)datum;
     TomlParsura  r;
    MateriaNodus* radix;
             Xar* dd;
             i32  n;
             i32  k;

    radix = toml_arbor_parsare(opus, (constans character*)textus.datum,
        (s32)textus.mensura, &r);
    si (radix == NIHIL)
    {
        redde;
    }
    dd  = toml_scalaria_iudicare(opus, radix);
    n   = _numerus_diagnosticorum(dd);
    per (k = I; k < n; k++)
    {
        si (   _diagnosticum(dd, k)->tractus.initium
            < _diagnosticum(dd, k - I)->tractus.initium)
        {
            st->ordo_ruptus++;
        }
    }
    si (fons == TOML_CORPUS_TOML_TEST)
    {
        si (strncmp(via, "valid/", VI) == ZEPHYRUM)
        {
            st->valida++;
            si (n > ZEPHYRUM)
            {
                st->valida_cum_vitio++;
                _nominare(st, "validum cum vitio", via, dd);
            }
        }
        alioquin si (_scalare(via))
        {
            st->invalida_scalaria++;
            si (!r.sana)
            {
                st->invalida_syntaxi++;
            }
            alioquin si (n > ZEPHYRUM)
            {
                st->invalida_coctione++;
            }
            alioquin
            {
                st->invalida_scalaria_accepta++;
                _nominare(st, "invalidum acceptum", via, dd);
            }
        }
    }
    alioquin si (fons == TOML_CORPUS_DOMUS)
    {
        st->domus++;
        si (n > ZEPHYRUM)
        {
            st->domus_cum_vitio++;
            _nominare(st, "domus cum vitio", via, dd);
        }
    }
    alioquin
    {
        st->silvestria++;
        si (n > ZEPHYRUM)
        {
            st->silvestria_cum_vitio++;
            _nominare(st, "silvestre cum vitio", via, dd);
        }
    }
}


/* ==================================================
 * Principale
 * ================================================== */

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
                Piscina* opus;
     constans character* radix = getenv("RHUBARB_RADIX");

    piscina = piscina_generare_dynamicum("probatio_toml_scalaris",
        262144);
    opus = piscina_generare_dynamicum("probatio_toml_scalaris_opus",
        1048576);
    si (!piscina || !opus)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * Integri
     * ================================================== */

    {
        i32 k;

        imprimere("\n--- Probans integros ---\n");
        per (k = ZEPHYRUM; INTEGRI[k].fons != NIHIL; k++)
        {
            TomlValor v;
                  Xar* dd;
                  b32 bene;

            bene = _coquere(opus, INTEGRI[k].fons, &v, &dd);
            si (   !bene || v.genus != TOML_VALOR_INTEGER
                || v.datum.integer_valor != INTEGRI[k].valor)
            {
                imprimere("    %s\n", INTEGRI[k].fons);
            }
            CREDO_VERUM (bene);
            CREDO_AEQUALIS_S32 ((s32)v.genus, (s32)TOML_VALOR_INTEGER);
            CREDO_AEQUALIS_S64 (v.datum.integer_valor,
                INTEGRI[k].valor);
            CREDO_AEQUALIS_I32 (_numerus_diagnosticorum(dd), ZEPHYRUM);
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * Fluitantes
     * ================================================== */

    {
        i32 k;

        imprimere("\n--- Probans fluitantes ---\n");
        per (k = ZEPHYRUM; FLUITANTES[k].fons != NIHIL; k++)
        {
             constans CasusFluitantis* c = &FLUITANTES[k];
                            TomlValor  v;
                                  Xar* dd;
                                  b32 bene;
                                  f64 x;
                                  b32 negativum;
                                  i32 species;

            bene  = _coquere(opus, c->fons, &v, &dd);
            x     = v.datum.fluitans_valor;
            species = x != x ? II
                : (x > 1e308 || x < -1e308) ? I : ZEPHYRUM;
            /* signum: etiam -0.0 et -nan (bitus summus) */
            {
                insignatus character o[VIII];
                                 f64 unum = 1.0;
                insignatus character u[VIII];
                                 i32 b;
                                 i32 summus = ZEPHYRUM;

                memcpy(o, &x, VIII);
                memcpy(u, &unum, VIII);
                /* octetus signi: ubi 1.0 octetum 0x3F habet */
                per (b = ZEPHYRUM; b < VIII; b++)
                {
                    si (u[b] == 0x3F)
                    {
                        summus = b;
                    }
                }
                negativum = (o[summus] & 0x80) != ZEPHYRUM;
            }
            si (   !bene || v.genus != TOML_VALOR_FLUITANS
                || species != c->species || negativum != c->negativum
                || (species == ZEPHYRUM && x != c->valor))
            {
                imprimere("    %s -> %g (species %u, neg %d)\n",
                    c->fons,
                    x, species, (integer)negativum);
            }
            CREDO_VERUM (bene);
            CREDO_AEQUALIS_S32 ((s32)v.genus, (s32)TOML_VALOR_FLUITANS);
            CREDO_AEQUALIS_I32 (species, c->species);
            CREDO_AEQUALIS_S32 ((s32)negativum, (s32)c->negativum);
            si (c->species == ZEPHYRUM)
            {
                CREDO_VERUM (x == c->valor);
            }
            CREDO_AEQUALIS_I32 (_numerus_diagnosticorum(dd), ZEPHYRUM);
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * Boolean
     * ================================================== */

    {
        TomlValor v;
              Xar* dd;

        imprimere("\n--- Probans boolean ---\n");
        CREDO_VERUM (_coquere(opus, "a = true", &v, &dd));
        CREDO_AEQUALIS_S32 ((s32)v.genus, (s32)TOML_VALOR_BOOLEAN);
        CREDO_VERUM (v.datum.boolean_valor);
        CREDO_VERUM (_coquere(opus, "a = false", &v, &dd));
        CREDO_FALSUM (v.datum.boolean_valor);
        CREDO_NON_NIHIL (v.nodus);
        piscina_vacare(opus);
    }


    /* ==================================================
     * Chordae
     * ================================================== */

    {
        i32 k;

        imprimere("\n--- Probans chordas ---\n");
        per (k = ZEPHYRUM; CHORDAE[k].fons != NIHIL; k++)
        {
             constans CasusChordae* c = &CHORDAE[k];
                         TomlValor  v;
                               Xar* dd;
                               b32 bene;
                               i32 m = c->mensura != ZEPHYRUM
                                   ? c->mensura
                                   : (i32)strlen(c->valor);
                               b32 idem;

            bene = _coquere(opus, c->fons, &v, &dd);
            idem = bene && v.genus == TOML_VALOR_CHORDA
                && v.datum.chorda_valor.mensura == m
                && (m == ZEPHYRUM
                    || memcmp(v.datum.chorda_valor.datum, c->valor,
                        (size_t)m) == ZEPHYRUM);
            si (!idem)
            {
                imprimere("    casus %u: '%.*s' (%u octeti)\n", k,
                    (integer)v.datum.chorda_valor.mensura,
                    (constans character*)v.datum.chorda_valor.datum,
                    v.datum.chorda_valor.mensura);
            }
            CREDO_VERUM (idem);
            CREDO_AEQUALIS_I32 (_numerus_diagnosticorum(dd), ZEPHYRUM);
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * Claves quotatae
     * ================================================== */

    {
        constans character* FONTES[] = { "\"k\\u0041\" = 1",
            "'lit\\n' = 1", "abc = 1", "\"\" = 1" };
        constans character* EXSPECTATA[] = { "kA", "lit\\n", "abc",
            "" };
                        i32 k;

        imprimere("\n--- Probans claves quotatas ---\n");
        per (k = ZEPHYRUM; k < IV; k++)
        {
            MateriaNodus* radix = _radix(opus, FONTES[k],
                                      (s32)strlen(FONTES[k]));
            MateriaNodus* par   = _sententia(radix,
                (s32)TOML_GENUS_PAR);
            MateriaNodus* clavis;
             MateriaValor* seg;
                   chorda  exitus;
                      i32  m = (i32)strlen(EXSPECTATA[k]);

            CREDO_NON_NIHIL (par);
            si (par == NIHIL)
            {
                perge;
            }
            clavis = par->loci[TOML_PAR_CLAVIS].datum.nodus;
            seg = materia_valor_lista_obtinere(
                clavis->loci[TOML_CLAVIS_PARTES], ZEPHYRUM);
            CREDO_VERUM (toml_chordam_coquere(opus, seg->datum.token,
                &exitus, NIHIL));
            CREDO_AEQUALIS_I32 (exitus.mensura, m);
            CREDO_VERUM (m == ZEPHYRUM
                || (exitus.mensura == m
                    && memcmp(exitus.datum, EXSPECTATA[k], (size_t)m)
                        == ZEPHYRUM));
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * Tempora
     * ================================================== */

    {
        i32 k;

        imprimere("\n--- Probans tempora ---\n");
        per (k = ZEPHYRUM; TEMPORA[k].fons != NIHIL; k++)
        {
             constans CasusTemporis* c = &TEMPORA[k];
                          TomlValor  v;
                                Xar* dd;
                                b32 bene;
                         TomlTempus* t = &v.datum.tempus_valor;
                                s32 campi[VIII];
                                i32 j;
                                b32 idem;

            bene             = _coquere(opus, c->fons, &v, &dd);
            campi[ZEPHYRUM]  = t->annus;
            campi[I]         = t->mensis;
            campi[II]        = t->dies;
            campi[III]       = t->hora;
            campi[IV]        = t->minutum;
            campi[V]         = t->secundum;
            campi[VI]        = t->nanosecunda;
            campi[VII]       = t->zona_minuta;
            idem = bene && v.genus == TOML_VALOR_TEMPUS
                && t->genus == c->genus;
            per (j = ZEPHYRUM; j < VIII; j++)
            {
                si (campi[j] != c->campi[j])
                {
                    idem = FALSUM;
                }
            }
            si (!idem)
            {
                imprimere("    %s -> genus %d: %d-%d-%d %d:%d:%d.%d "
                    "zona %d\n", c->fons, (integer)t->genus,
                    (integer)t->annus, (integer)t->mensis,
                    (integer)t->dies, (integer)t->hora,
                    (integer)t->minutum, (integer)t->secundum,
                    (integer)t->nanosecunda, (integer)t->zona_minuta);
            }
            CREDO_VERUM (idem);
            CREDO_AEQUALIS_I32 (_numerus_diagnosticorum(dd), ZEPHYRUM);
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * Vitia: codex, sedes, numerus
     * ================================================== */

    {
        i32 k;

        imprimere("\n--- Probans vitia ---\n");
        per (k = ZEPHYRUM; VITIA[k].fons != NIHIL; k++)
        {
            constans CasusVitii* c = &VITIA[k];
                   MateriaNodus* radix = _radix(opus, c->fons,
                                            (s32)strlen(c->fons));
                           Xar* dd = radix != NIHIL
                               ? toml_scalaria_iudicare(opus, radix)
                               : NIHIL;
                           i32 n  = _numerus_diagnosticorum(dd);
                           b32 bene;

            bene = n == c->numerus
                && strcmp(_diagnosticum(dd, ZEPHYRUM)->codex, c->codex)
                    == ZEPHYRUM
                && _diagnosticum(dd, ZEPHYRUM)->tractus.initium
                    == c->initium;
            si (!bene)
            {
                imprimere("    casus %u '%s': %u diagnostica", k,
                    c->fons,
                    n);
                si (n > ZEPHYRUM)
                {
                    imprimere(" - %s @%d: %s",
                        _diagnosticum(dd, ZEPHYRUM)->codex,
                        (integer)_diagnosticum(dd,
                            ZEPHYRUM)->tractus.initium,
                        _diagnosticum(dd, ZEPHYRUM)->causa);
                }
                imprimere("\n");
            }
            CREDO_VERUM (bene);
            si (n > ZEPHYRUM)
            {
                constans MateriaDiagnosticum* d = _diagnosticum(dd,
                                                    ZEPHYRUM);

                CREDO_NON_NIHIL (d->causa);
                CREDO_AEQUALIS_S32 (d->gravitas,
                    (s32)MATERIA_GRAVITAS_ERRATUM);
                CREDO_MAIOR_AUT_AEQUALIS_S32 (d->tractus.finis,
                    d->tractus.initium);
                CREDO_MAIOR_I32 (d->tractus.linea, ZEPHYRUM);
            }
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * Sedes linearis: vitium in linea altera chordae multae
     * ================================================== */

    {
        constans character* f = "a = \"\"\"\nab\n  \\q\"\"\"";
            MateriaNodus* radix;
                      Xar* dd;

        imprimere("\n--- Probans lineam et columnam vitii ---\n");
        radix  = _radix(opus, f, (s32)strlen(f));
        dd     = toml_scalaria_iudicare(opus, radix);
        CREDO_AEQUALIS_I32 (_numerus_diagnosticorum(dd), I);
        si (_numerus_diagnosticorum(dd) == I)
        {
            CREDO_AEQUALIS_I32 (_diagnosticum(dd,
                ZEPHYRUM)->tractus.linea,
                III);
            CREDO_AEQUALIS_I32 (
                _diagnosticum(dd, ZEPHYRUM)->tractus.columna, III);
            CREDO_AEQUALIS_S32 (_diagnosticum(dd,
                ZEPHYRUM)->tractus.finis,
                (s32)XV);
        }
        piscina_vacare(opus);
    }


    /* ==================================================
     * Malum non iudicatur; trivia eius iudicantur
     * ================================================== */

    {
        constans character* f = "\"\\q\" \"\\q\" = 1 # \001";
            MateriaNodus* radix;
                      Xar* dd;
               TomlParsura r;

        imprimere("\n--- Probans malum non iudicatum ---\n");
        radix = toml_arbor_parsare(opus, f, (s32)strlen(f), &r);
        CREDO_FALSUM (r.sana);
        dd = toml_scalaria_iudicare(opus, radix);
        CREDO_AEQUALIS_I32 (_numerus_diagnosticorum(dd), II);
        si (_numerus_diagnosticorum(dd) == II)
        {
            CREDO_AEQUALIS_S32 (_diagnosticum(dd,
                ZEPHYRUM)->tractus.initium,
                (s32)I);
            CREDO_AEQUALIS_S32 (_diagnosticum(dd, I)->tractus.initium,
                (s32)XVI);
        }
        piscina_vacare(opus);
    }


    /* ==================================================
     * Verdictum BOM (Q1): syntaxis recusat, coctio tacet
     * ================================================== */

    {
        constans character* f = "\357\273\277a = 1";
               TomlParsura  r;
            MateriaNodus* radix;

        imprimere("\n--- Probans BOM (verdictum Q1) ---\n");
        radix = toml_arbor_parsare(opus, f, (s32)strlen(f), &r);
        CREDO_NON_NIHIL (radix);
        CREDO_FALSUM (r.sana);
        piscina_vacare(opus);
    }


    /* ==================================================
     * Corpora
     * ================================================== */

    {
                  Status st;
        TomlCorpusNumeri nn;

        imprimere("\n--- Probans corpora ---\n");
        memset(&st, ZEPHYRUM, magnitudo(st));
        toml_corpus_ambulare(piscina, opus, radix
            != NIHIL ? radix : ".",
            _visor, &st, &nn);
        imprimere("  toml-test valida: %u, cum vitio scalari %u\n",
            st.valida, st.valida_cum_vitio);
        imprimere("  toml-test invalida scalaria: %u (syntaxi %u, "
            "coctione %u, accepta %u)\n", st.invalida_scalaria,
            st.invalida_syntaxi, st.invalida_coctione,
            st.invalida_scalaria_accepta);
        imprimere("  domus: %u, cum vitio %u; silvestria: %u, cum "
            "vitio %u; ordo ruptus %u\n", st.domus, st.domus_cum_vitio,
            st.silvestria, st.silvestria_cum_vitio, st.ordo_ruptus);
        CREDO_VERUM (nn.indices_lecti);
        CREDO_AEQUALIS_I32 (st.valida, (i32)CCV);
        CREDO_AEQUALIS_I32 (st.valida_cum_vitio, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (st.invalida_scalaria_accepta, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (st.invalida_coctione, (i32)CCI);
        CREDO_MAIOR_I32 (st.domus, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (st.domus_cum_vitio, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (st.ordo_ruptus, ZEPHYRUM);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(opus);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
