#include "actor.h"
#include "tempus.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* ==================================================
 * Parsing Helpers (Interior)
 * ================================================== */

/* Parsare chorda ad s32 */
interior b32
_actor_parsare_s32(
    chorda  valor,
    s32*    resultus)
{
    character buffer[LXIV];
    character* endptr;
    longus     val;
    i32        len;

    si (!valor.datum || valor.mensura == ZEPHYRUM || !resultus)
    {
        redde FALSUM;
    }

    len = valor.mensura;
    si (len > LXIII)
    {
        len = LXIII;
    }
    memcpy(buffer, valor.datum, (memoriae_index)len);
    buffer[len] = '\0';

    val = strtol(buffer, &endptr, X);

    si (endptr == buffer || *endptr != '\0')
    {
        redde FALSUM;
    }

    *resultus = (s32)val;
    redde VERUM;
}

/* Parsare chorda ad s64 */
interior b32
_actor_parsare_s64(
    chorda  valor,
    s64*    resultus)
{
    s64 val;
    s64 sign;
    i32 i;
    i32 start;

    si (!valor.datum || valor.mensura == ZEPHYRUM || !resultus)
    {
        redde FALSUM;
    }

    val  = 0;
    sign = 1;
    i    = 0;

    dum (i < valor.mensura &&
         (valor.datum[i] == ' ' || valor.datum[i] == '\t'))
    {
        i++;
    }

    si (i < valor.mensura && valor.datum[i] == '-')
    {
        sign = -1;
        i++;
    }
    alioquin si (i < valor.mensura && valor.datum[i] == '+')
    {
        i++;
    }

    start = i;
    si (i >= valor.mensura)
    {
        redde FALSUM;
    }

    dum (i < valor.mensura)
    {
        character c;
        c = (character)valor.datum[i];

        si (c >= '0' && c <= '9')
        {
            val = val * X + (s64)(c - '0');
            i++;
        }
        alioquin
        {
            frange;
        }
    }

    si (i == start)
    {
        redde FALSUM;
    }

    si (i < valor.mensura)
    {
        redde FALSUM;
    }

    *resultus = val * sign;
    redde VERUM;
}

/* Parsare chorda ad f64 */
interior b32
_actor_parsare_f64(
    chorda  valor,
    f64*    resultus)
{
    character  buffer[CXXVIII];
    character* endptr;
    f64        val;
    i32        len;

    si (!valor.datum || valor.mensura == ZEPHYRUM || !resultus)
    {
        redde FALSUM;
    }

    len = valor.mensura;
    si (len > CXXVII)
    {
        len = CXXVII;
    }
    memcpy(buffer, valor.datum, (memoriae_index)len);
    buffer[len] = '\0';

    val = strtod(buffer, &endptr);

    si (endptr == buffer || *endptr != '\0')
    {
        redde FALSUM;
    }

    *resultus = val;
    redde VERUM;
}

/* Parsare chorda ad b32 */
interior b32
_actor_parsare_b32(
    chorda  valor,
    b32*    resultus)
{
    si (!valor.datum || valor.mensura == ZEPHYRUM || !resultus)
    {
        redde FALSUM;
    }

    si (chorda_aequalis_literis(valor, "true") ||
        chorda_aequalis_literis(valor, "verum") ||
        chorda_aequalis_literis(valor, "1"))
    {
        *resultus = VERUM;
        redde VERUM;
    }

    si (chorda_aequalis_literis(valor, "false") ||
        chorda_aequalis_literis(valor, "falsum") ||
        chorda_aequalis_literis(valor, "0"))
    {
        *resultus = FALSUM;
        redde VERUM;
    }

    redde FALSUM;
}


/* ==================================================
 * Nuntius - Creatio
 * ================================================== */

Nuntius*
nuntius_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              mittens_id,
    constans character*  genus)
{
    Nuntius* nuntius;
    chorda   genus_chorda;

    si (!piscina || !intern || !genus)
    {
        redde NIHIL;
    }

    nuntius = (Nuntius*)piscina_allocare(piscina, magnitudo(Nuntius));
    si (!nuntius)
    {
        redde NIHIL;
    }

    /* Internare genus */
    genus_chorda = chorda_ex_literis(genus, piscina);

    nuntius->mittens_id     = mittens_id;
    nuntius->genus          = chorda_internare(intern, genus_chorda);
    nuntius->datum          = NIHIL;  /* Allocatur pigre */
    nuntius->tempus_creatus = tempus_nunc();
    nuntius->piscina        = piscina;
    nuntius->intern         = intern;

    si (!nuntius->genus)
    {
        redde NIHIL;
    }

    redde nuntius;
}


/* ==================================================
 * Nuntius - Datum (Payload)
 * ================================================== */

/* Allocare datum si necessarium */
interior b32
_nuntius_datum_allocare(
    Nuntius* nuntius)
{
    si (!nuntius)
    {
        redde FALSUM;
    }

    si (nuntius->datum)
    {
        redde VERUM;  /* Iam allocatum */
    }

    nuntius->datum = tabula_dispersa_creare_chorda(nuntius->piscina, XVI);
    redde nuntius->datum != NIHIL;
}

b32
nuntius_datum_ponere(
    Nuntius*            nuntius,
    constans character* clavis,
    chorda*             valor)
{
    chorda clavis_chorda;

    si (!nuntius || !clavis || !valor)
    {
        redde FALSUM;
    }

    si (!_nuntius_datum_allocare(nuntius))
    {
        redde FALSUM;
    }

    clavis_chorda = chorda_ex_literis(clavis, nuntius->piscina);

    redde tabula_dispersa_inserere(nuntius->datum, clavis_chorda, valor);
}

b32
nuntius_datum_ponere_literis(
    Nuntius*            nuntius,
    constans character* clavis,
    constans character* valor)
{
    chorda  valor_chorda;
    chorda* valor_internatum;

    si (!nuntius || !clavis || !valor)
    {
        redde FALSUM;
    }

    valor_chorda = chorda_ex_literis(valor, nuntius->piscina);

    valor_internatum = chorda_internare(nuntius->intern, valor_chorda);
    si (!valor_internatum)
    {
        redde FALSUM;
    }

    redde nuntius_datum_ponere(nuntius, clavis, valor_internatum);
}

b32
nuntius_datum_ponere_s32(
    Nuntius*            nuntius,
    constans character* clavis,
    s32                 valor)
{
    character buffer[XXXII];

    si (!nuntius || !clavis)
    {
        redde FALSUM;
    }

    sprintf(buffer, "%d", (integer)valor);
    redde nuntius_datum_ponere_literis(nuntius, clavis, buffer);
}

chorda*
nuntius_datum_capere(
    Nuntius*            nuntius,
    constans character* clavis)
{
    vacuum* valor;

    si (!nuntius || !clavis || !nuntius->datum)
    {
        redde NIHIL;
    }

    si (tabula_dispersa_invenire_literis(nuntius->datum, clavis, &valor))
    {
        redde (chorda*)valor;
    }

    redde NIHIL;
}

b32
nuntius_datum_capere_s32(
    Nuntius*            nuntius,
    constans character* clavis,
    s32*                valor)
{
    chorda* chorda_valor;

    si (!nuntius || !clavis || !valor)
    {
        redde FALSUM;
    }

    chorda_valor = nuntius_datum_capere(nuntius, clavis);
    si (!chorda_valor)
    {
        redde FALSUM;
    }

    redde _actor_parsare_s32(*chorda_valor, valor);
}

b32
nuntius_datum_capere_s64(
    Nuntius*            nuntius,
    constans character* clavis,
    s64*                valor)
{
    chorda* chorda_valor;

    si (!nuntius || !clavis || !valor)
    {
        redde FALSUM;
    }

    chorda_valor = nuntius_datum_capere(nuntius, clavis);
    si (!chorda_valor)
    {
        redde FALSUM;
    }

    redde _actor_parsare_s64(*chorda_valor, valor);
}

b32
nuntius_datum_capere_f64(
    Nuntius*            nuntius,
    constans character* clavis,
    f64*                valor)
{
    chorda* chorda_valor;

    si (!nuntius || !clavis || !valor)
    {
        redde FALSUM;
    }

    chorda_valor = nuntius_datum_capere(nuntius, clavis);
    si (!chorda_valor)
    {
        redde FALSUM;
    }

    redde _actor_parsare_f64(*chorda_valor, valor);
}

b32
nuntius_datum_capere_b32(
    Nuntius*            nuntius,
    constans character* clavis,
    b32*                valor)
{
    chorda* chorda_valor;

    si (!nuntius || !clavis || !valor)
    {
        redde FALSUM;
    }

    chorda_valor = nuntius_datum_capere(nuntius, clavis);
    si (!chorda_valor)
    {
        redde FALSUM;
    }

    redde _actor_parsare_b32(*chorda_valor, valor);
}

b32
nuntius_datum_habet(
    Nuntius*            nuntius,
    constans character* clavis)
{
    si (!nuntius || !clavis || !nuntius->datum)
    {
        redde FALSUM;
    }

    redde tabula_dispersa_continet_literis(nuntius->datum, clavis);
}


/* ==================================================
 * Nuntius - Utilitas
 * ================================================== */

vacuum
nuntius_imprimere(
    Nuntius* nuntius)
{
    TabulaIterator iter;
    chorda         clavis;
    vacuum*        valor;
    chorda*        valor_chorda;

    si (!nuntius)
    {
        imprimere("Nuntius: NIHIL\n");
        redde;
    }

    imprimere("Nuntius:\n");

    si (nuntius->mittens_id)
    {
        imprimere("  Mittens: %.*s\n",
                  nuntius->mittens_id->mensura,
                  nuntius->mittens_id->datum);
    }
    alioquin
    {
        imprimere("  Mittens: (systema)\n");
    }

    si (nuntius->genus)
    {
        imprimere("  Genus: %.*s\n",
                  nuntius->genus->mensura,
                  nuntius->genus->datum);
    }

    imprimere("  Tempus: %.3f\n", nuntius->tempus_creatus);

    si (nuntius->datum)
    {
        imprimere("  Datum (%d):\n",
                  tabula_dispersa_numerus(nuntius->datum));

        iter = tabula_dispersa_iterator_initium(nuntius->datum);
        dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
        {
            valor_chorda = (chorda*)valor;
            imprimere("    %.*s = %.*s\n",
                      clavis.mensura, clavis.datum,
                      valor_chorda->mensura, valor_chorda->datum);
        }
    }
    alioquin
    {
        imprimere("  Datum: (vacuum)\n");
    }
}


/* ==================================================
 * Capsa - Creatio
 * ================================================== */

Capsa*
capsa_creare(
    Piscina* piscina,
    chorda*  possessor_id)
{
    Capsa* capsa;

    si (!piscina || !possessor_id)
    {
        redde NIHIL;
    }

    capsa = (Capsa*)piscina_allocare(piscina, magnitudo(Capsa));
    si (!capsa)
    {
        redde NIHIL;
    }

    capsa->possessor_id = possessor_id;
    capsa->piscina      = piscina;

    capsa->nuntii = xar_creare(piscina, magnitudo(Nuntius*));
    si (!capsa->nuntii)
    {
        redde NIHIL;
    }

    redde capsa;
}


/* ==================================================
 * Capsa - Operationes
 * ================================================== */

b32
capsa_addere(
    Capsa*   capsa,
    Nuntius* nuntius)
{
    Nuntius** slot;

    si (!capsa || !nuntius)
    {
        redde FALSUM;
    }

    slot = (Nuntius**)xar_addere(capsa->nuntii);
    si (!slot)
    {
        redde FALSUM;
    }

    *slot = nuntius;
    redde VERUM;
}

Nuntius*
capsa_tollere(
    Capsa* capsa)
{
    Nuntius** primus;
    Nuntius*  nuntius;
    i32       numerus;
    i32       i;

    si (!capsa || xar_vacuum_est(capsa->nuntii))
    {
        redde NIHIL;
    }

    /* Capere primum */
    primus = (Nuntius**)xar_obtinere(capsa->nuntii, ZEPHYRUM);
    si (!primus)
    {
        redde NIHIL;
    }
    nuntius = *primus;

    /* Movere omnes alios unum locum */
    numerus = xar_numerus(capsa->nuntii);
    per (i = I; i < numerus; i++)
    {
        Nuntius** currens;
        Nuntius** prior;

        currens = (Nuntius**)xar_obtinere(capsa->nuntii, i);
        prior   = (Nuntius**)xar_obtinere(capsa->nuntii, i - I);

        si (currens && prior)
        {
            *prior = *currens;
        }
    }

    /* Truncare */
    xar_truncare(capsa->nuntii, numerus - I);

    redde nuntius;
}

Nuntius*
capsa_inspicere(
    Capsa* capsa)
{
    Nuntius** primus;

    si (!capsa || xar_vacuum_est(capsa->nuntii))
    {
        redde NIHIL;
    }

    primus = (Nuntius**)xar_obtinere(capsa->nuntii, ZEPHYRUM);
    si (!primus)
    {
        redde NIHIL;
    }

    redde *primus;
}

i32
capsa_numerus(
    Capsa* capsa)
{
    si (!capsa)
    {
        redde ZEPHYRUM;
    }

    redde xar_numerus(capsa->nuntii);
}

b32
capsa_vacua_est(
    Capsa* capsa)
{
    si (!capsa)
    {
        redde VERUM;
    }

    redde xar_vacuum_est(capsa->nuntii);
}

vacuum
capsa_vacare(
    Capsa* capsa)
{
    si (!capsa)
    {
        redde;
    }

    xar_vacare(capsa->nuntii);
}
