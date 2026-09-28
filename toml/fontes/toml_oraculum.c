/* toml_oraculum.c - Comparator toml-test (vide toml_oraculum.h)
 *
 * Differentia PRIMA redditur (ordine clavium exspectatarum): via
 * 'a.b[2]' et causa. Clavis exspectata absens: via = tabula, causa
 * nominat clavem; claves superfluae coctae: via = tabula.
 */

#include "toml_oraculum.h"
#include "toml_scalaris.h"
#include "toml_coctum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_CAPACITAS  MXXIV

nomen structura {
           Piscina* piscina;
         character  via[VIA_CAPACITAS];
               i32  mensura;
    TomlComparatio  exitus;
} Comparator;

hic_manens constans character* NOMINA_GENERUM[] = {
    "tabula", "series", "string", "integer", "float", "bool", "tempus"
};


/* ==================================================
 * Via et differentia
 * ================================================== */

interior i32
_via_addere (
            Comparator* c,
    constans character* forma,
    constans character* textus,
                   i32  mensura,
                   i32  index)
{
          i32 prior = c->mensura;
    character alveus[LXIV];
          i32 m;

    si (textus != NIHIL)
    {
        si (c->mensura > ZEPHYRUM && c->mensura < VIA_CAPACITAS - I)
        {
            c->via[c->mensura++] = '.';
        }
        m = mensura;
        si (c->mensura + m >= VIA_CAPACITAS - I)
        {
            m = VIA_CAPACITAS - I - c->mensura;
        }
        memcpy(c->via + c->mensura, textus, (size_t)m);
        c->mensura += m;
    }
    alioquin
    {
        sprintf(alveus, forma, (integer)index);
        m = (i32)strlen(alveus);
        si (c->mensura + m < VIA_CAPACITAS - I)
        {
            memcpy(c->via + c->mensura, alveus, (size_t)m);
            c->mensura += m;
        }
    }
    redde prior;
}

interior b32
_differt (
            Comparator* c,
    constans character* forma,
    constans character* primum,
    constans character* secundum)
{
    character* b = (character*)piscina_allocare(c->piscina, (i64)CCCLX);
    character* v = (character*)piscina_allocare(c->piscina,
                       (i64)c->mensura + I);

    si (b == NIHIL || v == NIHIL)
    {
        redde FALSUM;
    }
    sprintf(b, forma, primum != NIHIL ? primum : "",
        secundum != NIHIL ? secundum : "");
    memcpy(v, c->via, (size_t)c->mensura);
    v[c->mensura]                       = '\0';
    c->exitus.aequalis                  = FALSUM;
    c->exitus.via_differentiae.datum    = (i8*)v;
    c->exitus.via_differentiae.mensura  = c->mensura;
    c->exitus.causa.datum               = (i8*)b;
    c->exitus.causa.mensura             = (i32)strlen(b);
    redde FALSUM;
}


/* ==================================================
 * Scalaria
 * ================================================== */

interior b32
_aequat (
                 chorda  c,
     constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    redde c.mensura == n && memcmp(c.datum, litterae, (size_t)n)
        == ZEPHYRUM;
}

/* chorda exspectata -> copia nul-terminata (ad CXXVIII) */
interior vacuum
_copia (
         chorda  c,
      character* alveus)
{
    i32 m = c.mensura < CXXVII ? c.mensura : CXXVII;

    memcpy(alveus, c.datum, (size_t)m);
    alveus[m] = '\0';
}

/* integer decimalis signatus ex chorda (fines s64) */
interior b32
_integer_legere (
    constans character* s,
                   s64* exitus)
{
    b32 negativum  = FALSUM;
    i64 summa      = ZEPHYRUM;
    i64 limes;

    si (*s == '+' || *s == '-')
    {
        negativum = *s == '-';
        s++;
    }
    limes = negativum ? ((i64)I << LXIII) : ((i64)I << LXIII) - I;
    si (*s == '\0')
    {
        redde FALSUM;
    }
    dum (*s != '\0')
    {
        i64 d;

        si (*s < '0' || *s > '9')
        {
            redde FALSUM;
        }
        d = (i64)(*s - '0');
        si (summa > (limes - d) / X)
        {
            redde FALSUM;
        }
        summa = summa * X + d;
        s++;
    }
    *exitus = negativum
        ? (summa == ZEPHYRUM ? ZEPHYRUM : -(s64)(summa - I) - I)
        : (s64)summa;
    redde VERUM;
}

/* bitus signi f64 (etiam -0.0) */
interior b32
_signum_negativum (
    f64 x)
{
                     f64 unum = 1.0;
    insignatus character o[VIII];
    insignatus character u[VIII];
                     i32 b;
                     i32 summus = ZEPHYRUM;

    memcpy(o, &x, VIII);
    memcpy(u, &unum, VIII);
    per (b = ZEPHYRUM; b < VIII; b++)
    {
        si (u[b] == 0x3F)
        {
            summus = b;
        }
    }
    redde (o[summus] & 0x80) != ZEPHYRUM;
}

interior b32
_scalare (
             Comparator* c,
     constans TomlValor* v,
                 chorda  genus,
                 chorda  valor)
{
    character alveus[CXXVIII];
    character g[CXXVIII];

    _copia(valor, alveus);
    _copia(genus, g);

    si (_aequat(genus, "string"))
    {
        si (v->genus != TOML_VALOR_CHORDA)
        {
            redde _differt(c, "genus: %s != string%s",
                NOMINA_GENERUM[v->genus], NIHIL);
        }
        si (   v->datum.chorda_valor.mensura != valor.mensura
            || (valor.mensura > ZEPHYRUM
                && memcmp(v->datum.chorda_valor.datum, valor.datum,
                    (size_t)valor.mensura) != ZEPHYRUM))
        {
            redde _differt(c, "chorda: octeti differunt (exspectata "
                "'%s')%s", alveus, NIHIL);
        }
        redde VERUM;
    }
    si (_aequat(genus, "integer"))
    {
        s64 e;

        si (v->genus != TOML_VALOR_INTEGER)
        {
            redde _differt(c, "genus: %s != integer%s",
                NOMINA_GENERUM[v->genus], NIHIL);
        }
        si (!_integer_legere(alveus, &e))
        {
            redde _differt(c, "integer exspectatus illegibilis '%s'%s",
                alveus, NIHIL);
        }
        si (v->datum.integer_valor != e)
        {
            redde _differt(c, "integer: valor differt (exspectatus "
                "%s)%s", alveus, NIHIL);
        }
        redde VERUM;
    }
    si (_aequat(genus, "float"))
    {
                    f64  x;
                    f64  e;
     constans character* s = alveus;

        si (v->genus != TOML_VALOR_FLUITANS)
        {
            redde _differt(c, "genus: %s != float%s",
                NOMINA_GENERUM[v->genus], NIHIL);
        }
        x = v->datum.fluitans_valor;
        si (*s == '+' || *s == '-')
        {
            s++;
        }
        si (strcmp(s, "nan") == ZEPHYRUM)
        {
            redde x != x ? VERUM
                : _differt(c, "float: nan exspectatum%s%s", NIHIL,
                NIHIL);
        }
        si (strcmp(s, "inf") == ZEPHYRUM)
        {
            e = strtod("inf", NIHIL);
            si (alveus[ZEPHYRUM] == '-')
            {
                e = -e;
            }
            redde x == e ? VERUM
                : _differt(c, "float: %s exspectatum%s", alveus, NIHIL);
        }
        e = strtod(alveus, NIHIL);
        si (x != e)
        {
            redde _differt(c, "float: valor differt (exspectatus %s)%s",
                alveus, NIHIL);
        }
        si (   x                    == 0.0
            && _signum_negativum(x) != (alveus[ZEPHYRUM] == '-'))
        {
            redde _differt(c, "float: signum zephyri differt "
                "(exspectatus %s)%s", alveus, NIHIL);
        }
        redde VERUM;
    }
    si (_aequat(genus, "bool"))
    {
        si (v->genus != TOML_VALOR_BOOLEAN)
        {
            redde _differt(c, "genus: %s != bool%s",
                NOMINA_GENERUM[v->genus], NIHIL);
        }
        si ((v->datum.boolean_valor ? VERUM : FALSUM)
            != (_aequat(valor, "true") ? VERUM : FALSUM))
        {
            redde _differt(c, "bool: valor differt (exspectatus %s)%s",
                alveus, NIHIL);
        }
        redde VERUM;
    }
    si (   _aequat(genus, "datetime")
        || _aequat(genus, "datetime-local")
        || _aequat(genus, "date-local") || _aequat(genus, "time-local"))
    {
        TomlGenusTemporis gt = _aequat(genus, "datetime")
            ? TOML_TEMPUS_CUM_ZONA : _aequat(genus, "datetime-local")
            ? TOML_TEMPUS_LOCALE : _aequat(genus, "date-local")
            ? TOML_DIES_LOCALIS : TOML_HORA_LOCALIS;
                TomlTempus  e;
         TomlVitiumScalare  vit;
       constans TomlTempus* t = &v->datum.tempus_valor;

        si (v->genus != TOML_VALOR_TEMPUS)
        {
            redde _differt(c, "genus: %s != %s",
                NOMINA_GENERUM[v->genus],
                g);
        }
        si (!toml_tempus_legere(valor, &e, &vit))
        {
            redde _differt(c, "tempus exspectatum illegibile '%s'%s",
                alveus, NIHIL);
        }
        si (t->genus != gt || e.genus != gt)
        {
            redde _differt(c,
                "genus temporis differt (exspectatum %s)%s",
                g, NIHIL);
        }
        si (   t->annus       != e.annus || t->mensis != e.mensis
            || t->dies        != e.dies || t->hora != e.hora
            || t->minutum     != e.minutum || t->secundum != e.secundum
            || t->nanosecunda != e.nanosecunda
            || t->zona_minuta != e.zona_minuta)
        {
            redde _differt(c, "tempus: campi differunt (exspectatum "
                "%s)%s", alveus, NIHIL);
        }
        redde VERUM;
    }
    redde _differt(c, "genus exspectatum ignotum '%s'%s", g, NIHIL);
}


/* ==================================================
 * Structura
 * ================================================== */

/* objectum tagatum: EXACTE claves 'type' et 'value', ambae chordae */
interior b32
_notatum (
    JsonValor* j,
       chorda* genus,
       chorda* valor)
{
    JsonValor* t;
    JsonValor* v;

    si (!json_est_objectum(j) || json_objectum_numerus(j) != II)
    {
        redde FALSUM;
    }
    t = json_objectum_capere(j, "type");
    v = json_objectum_capere(j, "value");
    si (   t == NIHIL || v == NIHIL || !json_est_chorda(t)
        || !json_est_chorda(v))
    {
        redde FALSUM;
    }
    *genus = json_ad_chorda(t);
    *valor = json_ad_chorda(v);
    redde VERUM;
}

interior b32
_comparare (
             Comparator* c,
     constans TomlValor* v,
              JsonValor* j)
{
    chorda genus;
    chorda valor;

    si (v == NIHIL || j == NIHIL)
    {
        redde _differt(c, "valor absens%s%s", NIHIL, NIHIL);
    }
    si (_notatum(j, &genus, &valor))
    {
        redde _scalare(c, v, genus, valor);
    }
    si (json_est_objectum(j))
    {
        i32 k;
        i32 n = json_objectum_numerus(j);

        si (v->genus != TOML_VALOR_TABULA)
        {
            redde _differt(c, "genus: %s != tabula%s",
                NOMINA_GENERUM[v->genus], NIHIL);
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
                  JsonPar* p       = json_objectum_par_obtinere(j,
                      k);
       constans TomlValor* filius  = toml_tabulae_filius(v, *p->clavis);
                      i32  prior;

            si (filius == NIHIL)
            {
                character alveus[CXXVIII];

                _copia(*p->clavis, alveus);
                redde _differt(c, "clavis '%s' exspectata deest%s",
                    alveus,
                    NIHIL);
            }
            prior = _via_addere(c, NIHIL, (constans character*)
                p->clavis->datum, p->clavis->mensura, ZEPHYRUM);
            si (!_comparare(c, filius, p->valor))
            {
                redde FALSUM;
            }
            c->mensura = prior;
        }
        si (xar_numerus(v->datum.tabula.claves) != n)
        {
            character a[XXXII];
            character b[XXXII];

            sprintf(a, "%u", xar_numerus(v->datum.tabula.claves));
            sprintf(b, "%u", n);
            redde _differt(c, "claves superfluae: %s coctae, %s "
                "exspectatae", a, b);
        }
        redde VERUM;
    }
    si (json_est_tabulatum(j))
    {
        i32 k;
        i32 n = json_tabulatum_numerus(j);

        si (v->genus != TOML_VALOR_SERIES)
        {
            redde _differt(c, "genus: %s != series%s",
                NOMINA_GENERUM[v->genus], NIHIL);
        }
        si (xar_numerus(v->datum.series) != n)
        {
            character a[XXXII];
            character b[XXXII];

            sprintf(a, "%u", xar_numerus(v->datum.series));
            sprintf(b, "%u", n);
            redde _differt(c, "longitudo seriei: %s != %s", a, b);
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
            i32 prior = _via_addere(c, "[%d]", NIHIL, ZEPHYRUM, k);

            si (!_comparare(c,
                    *(TomlValor**)xar_obtinere(v->datum.series, k),
                    json_tabulatum_obtinere(j, k)))
            {
                redde FALSUM;
            }
            c->mensura = prior;
        }
        redde VERUM;
    }
    redde _differt(c, "exspectatum nec objectum nec tabulatum nec "
        "tagatum%s%s", NIHIL, NIHIL);
}

TomlComparatio
toml_oraculum_comparare (
                 Piscina* piscina,
      constans TomlValor* coctum,
               JsonValor* exspectatum)
{
    Comparator c;

    memset(&c, ZEPHYRUM, magnitudo(c));
    c.piscina = piscina;
    si (piscina != NIHIL && _comparare(&c, coctum, exspectatum))
    {
        c.exitus.aequalis = VERUM;
    }
    redde c.exitus;
}
