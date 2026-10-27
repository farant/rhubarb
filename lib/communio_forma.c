/* communio_forma.c - acta ut JSON, validatio in limine retis
 * (communio-plan-2 T2) */

#include "communio_forma.h"

#include <string.h>

#define SIGILLUM_HEX_LONGITUDO  LXIV
#define REPLICA_LONGITUDO       XXVI
#define DECIMALES_MAXIMI        XIX

/* causam ponit (si petita) et FALSUM reddit */
interior b32
causa_falsa (
                chorda* causa,
    constans character* textus,
               Piscina* piscina)
{
    si (causa)
    {
        *causa = chorda_ex_literis(textus, piscina);
    }
    redde FALSUM;
}

interior b32
hex_minuscula (
    i8 c)
{
    redde (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

/* Crockford base32 maiuscula, sine I L O U */
b32
communio_replica_valida (
    chorda replica)
{
    i32 i;

    si (replica.mensura != REPLICA_LONGITUDO || !replica.datum)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < REPLICA_LONGITUDO; i++)
    {
        i8 c = replica.datum[i];

        si (   !(c >= '0' && c <= '9') && !(c >= 'A' && c <= 'Z'))
        {
            redde FALSUM;
        }
        si (c == 'I' || c == 'L' || c == 'O' || c == 'U')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* numerus non negativus ut chorda decimalis */
interior JsonValor*
decimalis (
         s64  valor,
     Piscina* piscina)
{
    character inversa[XXIV];
    character recta[XXIV];
          i64 u;
          i32 n;
          i32 i;

    u = valor > ZEPHYRUM ? (i64)valor : (i64)ZEPHYRUM;
    n = ZEPHYRUM;
    fac
    {
        inversa[n++]  = (character)('0' + (integer)(u % (i64)X));
        u             /= (i64)X;
    } dum (u > (i64)ZEPHYRUM);
    per (i = ZEPHYRUM; i < n; i++)
    {
        recta[i] = inversa[n - I - i];
    }
    recta[n] = '\0';
    redde json_chorda_creare(piscina, chorda_ex_literis(recta,
        piscina));
}

/* chorda decimalis (solum signa 0-9, XIX ad plurimum) in s64 */
interior b32
decimalem_legere (
    chorda  textus,
       s64* exitus)
{
    i64 valor;
    i64 limes;
    i64 d;
    i32 i;

    si (textus.mensura == ZEPHYRUM || textus.mensura > DECIMALES_MAXIMI)
    {
        redde FALSUM;
    }
    limes = ((i64)~(i64)ZEPHYRUM) >> I;
    valor = (i64)ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (textus.datum[i] < '0' || textus.datum[i] > '9')
        {
            redde FALSUM;
        }
        d = (i64)(textus.datum[i] - '0');
        si (valor > (limes - d) / (i64)X)
        {
            redde FALSUM;
        }
        valor = valor * (i64)X + d;
    }
    *exitus = (s64)valor;
    redde VERUM;
}

/* campus chordae obiecti; FALSUM si deest aut non chorda */
interior b32
campus (
             JsonValor* obiectum,
    constans character* clavis,
                chorda* exitus)
{
    JsonValor* v;

    v = json_objectum_capere(obiectum, clavis);
    si (!v || v->genus != JSON_CHORDA)
    {
        redde FALSUM;
    }
    *exitus = v->datum.chorda_valor;
    redde VERUM;
}

JsonValor*
communio_actum_scribere (
    constans VolumenActum* actum,
                  Piscina* piscina)
{
    JsonValor* o;

    si (!actum)
    {
        redde NIHIL;
    }
    o = json_objectum_creare(piscina);
    si (!o)
    {
        redde NIHIL;
    }
    json_objectum_ponere(o, "hora", decimalis(actum->identitas.hora,
        piscina));
    json_objectum_ponere(o, "replica", json_chorda_creare(piscina,
        actum->identitas.replica));
    json_objectum_ponere(o, "machina", json_chorda_creare(piscina,
        actum->identitas.machina));
    json_objectum_ponere(o, "momentum", json_chorda_creare(piscina,
        actum->momentum));
    json_objectum_ponere(o, "genus", json_chorda_creare(piscina,
        actum->genus));
    json_objectum_ponere(o, "datum", json_chorda_creare(piscina,
        actum->datum));
    si (   actum->basis.hora != ZEPHYRUM
        || actum->basis.replica.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(o, "basis_hora", decimalis(
            actum->basis.hora, piscina));
        json_objectum_ponere(o, "basis_replica", json_chorda_creare(
            piscina, actum->basis.replica));
        json_objectum_ponere(o, "basis_machina", json_chorda_creare(
            piscina, actum->basis.machina));
    }
    json_objectum_ponere(o, "seq_originis", json_integer_creare(piscina,
        actum->seq_originis));
    redde o;
}

b32
communio_actum_legere (
       JsonValor* obiectum,
    VolumenActum* exitus,
          chorda* causa,
         Piscina* piscina)
{
     JsonValor* v;
        chorda  t;

    si (!obiectum || !exitus || obiectum->genus != JSON_OBJECTUM)
    {
        redde causa_falsa(causa, "actum non obiectum", piscina);
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    si (   !campus(obiectum, "hora", &t)
        || !decimalem_legere(t, &exitus->identitas.hora))
    {
        redde causa_falsa(causa, "hora mala", piscina);
    }
    si (   !campus(obiectum, "replica", &exitus->identitas.replica)
        || !campus(obiectum, "machina", &exitus->identitas.machina)
        || !campus(obiectum, "momentum", &exitus->momentum)
        || !campus(obiectum, "genus", &exitus->genus)
        || !campus(obiectum, "datum", &exitus->datum))
    {
        redde causa_falsa(causa, "campus deest", piscina);
    }
    si (json_objectum_capere(obiectum, "basis_hora"))
    {
        si (   !campus(obiectum, "basis_hora", &t)
            || !decimalem_legere(t, &exitus->basis.hora)
            || !campus(obiectum, "basis_replica",
                   &exitus->basis.replica)
            || !campus(obiectum, "basis_machina",
                   &exitus->basis.machina))
        {
            redde causa_falsa(causa, "basis mala", piscina);
        }
    }
    v = json_objectum_capere(obiectum, "seq_originis");
    si (!v || v->genus != JSON_INTEGER)
    {
        redde causa_falsa(causa, "seq_originis deest", piscina);
    }
    exitus->seq_originis  = v->datum.integer_valor;
    exitus->seq           = ZEPHYRUM;
    redde VERUM;
}

b32
communio_actum_validum (
    constans VolumenActum* actum,
                   chorda* causa,
                  Piscina* piscina)
{
    JsonResultus r;
          chorda vacua;
          chorda via;

    si (!actum)
    {
        redde causa_falsa(causa, "actum nullum", piscina);
    }
    si (!communio_replica_valida(actum->identitas.replica))
    {
        redde causa_falsa(causa, "replica mala", piscina);
    }
    si (actum->identitas.hora <= ZEPHYRUM)
    {
        redde causa_falsa(causa, "hora nulla", piscina);
    }
    si (actum->seq_originis <= ZEPHYRUM)
    {
        redde causa_falsa(causa, "seq_originis nullum", piscina);
    }
    si (actum->genus.mensura == ZEPHYRUM)
    {
        redde causa_falsa(causa, "genus vacuum", piscina);
    }
    si (   chorda_aequalis_literis(actum->genus, "plagula-condita")
        || chorda_aequalis_literis(actum->genus, "plagula-remota"))
    {
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        r              = json_legere(actum->datum, piscina);
        via = r.successus ? json_capere_chorda(r.radix, "via", vacua)
                          : vacua;
        si (via.mensura == ZEPHYRUM)
        {
            redde causa_falsa(causa, "plagula sine via", piscina);
        }
        si (   via.mensura                               >= VII
            && memcmp(via.datum, "aliena/", (size_t)VII) == ZEPHYRUM)
        {
            redde causa_falsa(causa, "via aliena reservata", piscina);
        }
    }
    redde VERUM;
}

chorda
communio_acta_scribere (
         Xar* acta,
     Piscina* piscina)
{
     JsonValor* o;
     JsonValor* t;
        chorda  vacua;
           i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    o              = json_objectum_creare(piscina);
    t              = json_tabulatum_creare(piscina);
    si (!o || !t)
    {
        redde vacua;
    }
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        JsonValor* a = communio_actum_scribere(
            (VolumenActum*)xar_obtinere(acta, i), piscina);

        si (!a)
        {
            redde vacua;
        }
        json_tabulatum_addere(t, a);
    }
    json_objectum_ponere(o, "acta", t);
    redde json_scribere(o, piscina);
}

Xar*
communio_acta_legere (
      chorda  corpus,
      chorda* causa,
     Piscina* piscina)
{
    JsonResultus  r;
       JsonValor* t;
             Xar* acta;
             i32  n;
             i32  i;

    r = json_legere(corpus, piscina);
    si (!r.successus || !r.radix || r.radix->genus != JSON_OBJECTUM)
    {
        (vacuum)causa_falsa(causa, "corpus non JSON", piscina);
        redde NIHIL;
    }
    t = json_objectum_capere(r.radix, "acta");
    si (!t || t->genus != JSON_TABULATUM)
    {
        (vacuum)causa_falsa(causa, "acta desunt", piscina);
        redde NIHIL;
    }
    n = json_tabulatum_numerus(t);
    si (n > COMMUNIO_ACTA_MAXIMA)
    {
        (vacuum)causa_falsa(causa, "acta nimia", piscina);
        redde NIHIL;
    }
    acta = xar_creare(piscina, (i32)magnitudo(VolumenActum));
    si (!acta)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        VolumenActum* a = (VolumenActum*)xar_addere(acta);

        si (   !a
            || !communio_actum_legere(json_tabulatum_obtinere(t, i), a,
                   causa, piscina)
            || !communio_actum_validum(a, causa, piscina))
        {
            redde NIHIL;
        }
    }
    redde acta;
}

Xar*
communio_massas_nominatas (
    constans VolumenActum* actum,
                  Piscina* piscina)
{
       Xar* l;
    chorda  d;
       i32  i;
       i32  j;

    l = xar_creare(piscina, (i32)magnitudo(chorda));
    si (!l || !actum)
    {
        redde l;
    }
    d = actum->datum;
    i = ZEPHYRUM;
    dum (i < d.mensura)
    {
        si (!hex_minuscula(d.datum[i]))
        {
            i++;
            perge;
        }
        j = i;
        dum (j < d.mensura && hex_minuscula(d.datum[j]))
        {
            j++;
        }
        si (j - i == SIGILLUM_HEX_LONGITUDO)
        {
            chorda* h = (chorda*)xar_addere(l);

            si (!h)
            {
                redde NIHIL;
            }
            h->datum    = d.datum + i;
            h->mensura  = SIGILLUM_HEX_LONGITUDO;
        }
        i = j;
    }
    redde l;
}
