/* pulsus_forma.c - formae notificationum domus (vide pulsus_forma.h) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "pulsus_forma.h"
#include <string.h>
#include <stdio.h>


/* ==================================================
 * Leges (C10)
 * ================================================== */

interior b32
_sine_moderantibus (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (s.datum[i] < (i8)XXXII || s.datum[i] == (i8)CXXVII)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* I..maximum signa ex [A-Za-z0-9._-] */
interior b32
_identitas_valida (
    chorda s,
       i32 maximum)
{
           i32 i;
     character x;

    si (s.mensura < I || s.mensura > maximum)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        x = (character)s.datum[i];
        si (!(   (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')
              || (x >= '0' && x <= '9') || x == '.' || x == '_'
              || x == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_id_validum (
    chorda s)
{
    redde s.mensura >= I && s.mensura <= PULSUS_ID_MAXIMUM
        && _sine_moderantibus(s);
}

/* locator ad positionem i: longitudo eius ("{persona}" aut
 * "{numerus}"), aliter 0 */
interior i32
_locator (
    chorda s,
       i32 i)
{
    hic_manens constans character PERSONA[] = "{persona}";
    hic_manens constans character NUMERUS[] = "{numerus}";
                              i32 n;

    n = (i32)strlen(PERSONA);
    si (   i + n <= s.mensura
        && (   memcmp(s.datum + i, PERSONA, (size_t)n) == ZEPHYRUM
            || memcmp(s.datum + i, NUMERUS, (size_t)n) == ZEPHYRUM))
    {
        redde n;
    }
    redde ZEPHYRUM;
}

interior b32
_forma_valida (
    chorda s)
{
    i32 i;

    si (   s.mensura < I || s.mensura > PULSUS_FORMA_MAXIMA
        || !_sine_moderantibus(s))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (s.datum[i] == '{' && _locator(s, i) == ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_hex_valida (
    chorda s)
{
           i32 i;
     character x;

    si (   s.mensura < II || s.mensura > PULSUS_TESSERA_MAXIMA
        || (s.mensura % II) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        x = (character)s.datum[i];
        si (!(   (x >= '0' && x <= '9') || (x >= 'a' && x <= 'f')
              || (x >= 'A' && x <= 'F')))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* campus chordae (valor JSON chorda) aut FALSUM */
interior b32
_chorda_campi (
              JsonValor* obiectum,
     constans character* clavis,
                 chorda* exitus)
{
    JsonValor* v;

    v = json_objectum_capere(obiectum, clavis);
    si (!v || !json_est_chorda(v))
    {
        redde FALSUM;
    }
    *exitus = json_ad_chorda(v);
    redde VERUM;
}

/* radix obiectum aut NIHIL */
interior JsonValor*
_radix (
     chorda  json,
    Piscina* piscina)
{
    JsonResultus r;

    r = json_legere(json, piscina);
    si (!r.successus || !r.radix || !json_est_objectum(r.radix))
    {
        redde NIHIL;
    }
    redde r.radix;
}

interior b32
_causa (
                chorda* causa,
    constans character* campus,
               Piscina* piscina)
{
    si (causa)
    {
        *causa = chorda_ex_literis(campus, piscina);
    }
    redde FALSUM;
}


/* ==================================================
 * Eventus
 * ================================================== */

b32
pulsus_eventum_legere (
           chorda  json,
          Piscina* piscina,
    PulsusEventus* exitus,
           chorda* causa)
{
    JsonValor* radix;
    JsonValor* v;
    JsonValor* id;
          i32  i;
          s64  mora;

    si (!piscina || !exitus)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    radix = _radix(json, piscina);
    si (!radix)
    {
        redde _causa(causa, "corpus (non obiectum JSON)", piscina);
    }
    si (   !_chorda_campi(radix, "clavis", &exitus->clavis)
        || !_identitas_valida(exitus->clavis, PULSUS_TITULUS_MAXIMUS))
    {
        redde _causa(causa, "clavis", piscina);
    }
    si (   !_chorda_campi(radix, "thema", &exitus->thema)
        || !_identitas_valida(exitus->thema, PULSUS_TITULUS_MAXIMUS))
    {
        redde _causa(causa, "thema", piscina);
    }
    exitus->res = xar_creare(piscina, magnitudo(chorda));
    si (!exitus->res)
    {
        redde _causa(causa, "memoria", piscina);
    }
    v = json_objectum_capere(radix, "res");
    si (v)
    {
        si (   !json_est_tabulatum(v)
            || json_tabulatum_numerus(v) > PULSUS_RES_PER_PETITIONEM)
        {
            redde _causa(causa, "res", piscina);
        }
        per (i = ZEPHYRUM; i < json_tabulatum_numerus(v); i++)
        {
            id = json_tabulatum_obtinere(v, i);
            si (   !id || !json_est_chorda(id)
                || !_id_validum(json_ad_chorda(id)))
            {
                redde _causa(causa, "res", piscina);
            }
            *(chorda*)xar_addere(exitus->res) = json_ad_chorda(id);
        }
    }
    exitus->mora_s  = PULSUS_MORA_ORDINARIA;
    v               = json_objectum_capere(radix, "mora");
    si (v)
    {
        mora = json_ad_integer(v);
        si (   !json_est_integer(v) || mora < ZEPHYRUM
            || mora > (s64)PULSUS_MORA_MAXIMA)
        {
            redde _causa(causa, "mora", piscina);
        }
        exitus->mora_s = (i32)mora;
    }
    si (   !_chorda_campi(radix, "unus", &exitus->unus)
        || !_forma_valida(exitus->unus))
    {
        redde _causa(causa, "unus", piscina);
    }
    si (   !_chorda_campi(radix, "plures", &exitus->plures)
        || !_forma_valida(exitus->plures))
    {
        redde _causa(causa, "plures", piscina);
    }
    v = json_objectum_capere(radix, "statim");
    si (v)
    {
        si (!json_est_boolean(v))
        {
            redde _causa(causa, "statim", piscina);
        }
        exitus->statim = json_ad_boolean(v);
    }
    redde VERUM;
}

chorda
pulsus_eventum_scribere (
    constans PulsusEventus* eventus,
                   Piscina* piscina)
{
    JsonValor* o;
    JsonValor* res;
       chorda  vacua;
          i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!eventus || !piscina)
    {
        redde vacua;
    }
    o    = json_objectum_creare(piscina);
    res  = json_tabulatum_creare(piscina);
    si (!o || !res)
    {
        redde vacua;
    }
    per (i = ZEPHYRUM; eventus->res
        && i < xar_numerus(eventus->res); i++)
    {
        json_tabulatum_addere(res, json_chorda_creare(piscina,
            *(chorda*)xar_obtinere(eventus->res, i)));
    }
    json_objectum_ponere(o, "clavis",
        json_chorda_creare(piscina, eventus->clavis));
    json_objectum_ponere(o, "thema",
        json_chorda_creare(piscina, eventus->thema));
    json_objectum_ponere(o, "res", res);
    json_objectum_ponere(o, "mora",
        json_integer_creare(piscina, (s64)eventus->mora_s));
    json_objectum_ponere(o, "unus",
        json_chorda_creare(piscina, eventus->unus));
    json_objectum_ponere(o, "plures",
        json_chorda_creare(piscina, eventus->plures));
    json_objectum_ponere(o, "statim",
        json_boolean_creare(piscina, eventus->statim));
    redde json_scribere(o, piscina);
}


/* ==================================================
 * Inscriptio
 * ================================================== */

b32
pulsus_inscriptionem_legere (
     chorda  json,
    Piscina* piscina,
     chorda* tessera,
     chorda* ambitus,
     chorda* thema,
     chorda* causa)
{
    JsonValor* radix;

    si (!piscina || !tessera || !ambitus || !thema)
    {
        redde FALSUM;
    }
    radix = _radix(json, piscina);
    si (!radix)
    {
        redde _causa(causa, "corpus (non obiectum JSON)", piscina);
    }
    si (   !_chorda_campi(radix, "tessera", tessera)
        || !_hex_valida(*tessera))
    {
        redde _causa(causa, "tessera", piscina);
    }
    si (   !_chorda_campi(radix, "ambitus", ambitus)
        || (   !chorda_aequalis_literis(*ambitus, "productio")
            && !chorda_aequalis_literis(*ambitus, "sandbox")))
    {
        redde _causa(causa, "ambitus", piscina);
    }
    si (   !_chorda_campi(radix, "thema", thema)
        || !_identitas_valida(*thema, PULSUS_TITULUS_MAXIMUS))
    {
        redde _causa(causa, "thema", piscina);
    }
    redde VERUM;
}


/* ==================================================
 * Textus et corpus
 * ================================================== */

chorda
pulsus_textum_facere (
     chorda  forma,
     chorda  persona,
        i32  numerus,
    Piscina* piscina)
{
       chorda  exitus;
    character  numeri[XXIV];
          i32  n_numeri;
          i32  capacitas;
          i32  i;
          i32  l;
          i32  n;
           i8* b;

    exitus.datum    = NIHIL;
    exitus.mensura  = ZEPHYRUM;
    si (!piscina)
    {
        redde exitus;
    }
    sprintf(numeri, "%u", (unsigned)numerus);
    n_numeri = (i32)strlen(numeri);
    capacitas = forma.mensura * (persona.mensura + n_numeri + I) + I;
    b = (i8*)piscina_allocare(piscina, (memoriae_index)capacitas);
    si (!b)
    {
        redde exitus;
    }
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < forma.mensura; )
    {
        l = _locator(forma, i);
        si (l > ZEPHYRUM && forma.datum[i + I] == 'p')
        {
            memcpy(b + n, persona.datum, (size_t)persona.mensura);
            n += persona.mensura;
            i += l;
        }
        alioquin si (l > ZEPHYRUM)
        {
            memcpy(b + n, numeri, (size_t)n_numeri);
            n += n_numeri;
            i += l;
        }
        alioquin
        {
            b[n++] = forma.datum[i++];
        }
    }
    exitus.datum    = b;
    exitus.mensura  = n;
    redde exitus;
}

/* corpus cum primis n rebus */
interior chorda
_corpus (
          chorda  titulus,
          chorda  textus,
          chorda  clavis,
          chorda  a,
    constans Xar* res,
             i32  n,
             b32  plures,
         Piscina* piscina)
{
    JsonValor* radix;
    JsonValor* systema;
    JsonValor* monitum;
    JsonValor* pulsus;
    JsonValor* tab;
          i32  i;

    radix    = json_objectum_creare(piscina);
    systema  = json_objectum_creare(piscina);
    monitum  = json_objectum_creare(piscina);
    pulsus   = json_objectum_creare(piscina);
    tab      = json_tabulatum_creare(piscina);
    json_objectum_ponere(monitum, "title", json_chorda_creare(piscina,
        titulus));
    json_objectum_ponere(monitum, "body", json_chorda_creare(piscina,
        textus));
    json_objectum_ponere(systema, "alert", monitum);
    json_objectum_ponere(systema, "sound",
        json_chorda_creare_literis(piscina,
        "default"));
    json_objectum_ponere(systema, "thread-id",
        json_chorda_creare(piscina,
        clavis));
    per (i = ZEPHYRUM; i < n; i++)
    {
        json_tabulatum_addere(tab, json_chorda_creare(piscina,
            *(chorda*)xar_obtinere(res, i)));
    }
    json_objectum_ponere(pulsus, "clavis", json_chorda_creare(piscina,
        clavis));
    json_objectum_ponere(pulsus, "res", tab);
    json_objectum_ponere(pulsus, "a", json_chorda_creare(piscina, a));
    si (plures)
    {
        json_objectum_ponere(pulsus, "plures",
            json_boolean_creare(piscina,
            VERUM));
    }
    json_objectum_ponere(radix, "aps", systema);
    json_objectum_ponere(radix, "pulsus", pulsus);
    redde json_scribere(radix, piscina);
}

chorda
pulsus_corpus_facere (
          chorda  titulus,
          chorda  textus,
          chorda  clavis,
          chorda  a,
    constans Xar* res,
         Piscina* piscina)
{
    chorda corpus;
       i32 n;
       i32 totum;

    corpus.datum    = NIHIL;
    corpus.mensura  = ZEPHYRUM;
    si (!piscina)
    {
        redde corpus;
    }
    totum  = res ? xar_numerus(res) : ZEPHYRUM;
    corpus = _corpus(titulus, textus, clavis, a, res, totum, FALSUM,
        piscina);
    /* truncatio: a fine, una res per gradum, plures notatur */
    per (n = totum; n > ZEPHYRUM
        && corpus.mensura > PULSUS_CORPUS_MAXIMUM; )
    {
        n--;
        corpus = _corpus(titulus, textus, clavis, a, res, n, VERUM,
            piscina);
    }
    redde corpus;
}


/* ==================================================
 * Tactus
 * ================================================== */

b32
pulsus_tactum_legere (
          chorda  json,
         Piscina* piscina,
    PulsusTactus* exitus)
{
    JsonValor* radix;
    JsonValor* pulsus;
    JsonValor* v;
    JsonValor* id;
          i32  i;

    si (!piscina || !exitus)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    radix   = _radix(json, piscina);
    pulsus  = radix ? json_objectum_capere(radix, "pulsus") : NIHIL;
    si (   !pulsus || !json_est_objectum(pulsus)
        || !_chorda_campi(pulsus, "clavis", &exitus->clavis))
    {
        redde FALSUM;
    }
    (vacuum)_chorda_campi(pulsus, "a", &exitus->a);
    exitus->res = xar_creare(piscina, magnitudo(chorda));
    si (!exitus->res)
    {
        redde FALSUM;
    }
    v = json_objectum_capere(pulsus, "res");
    per (i = ZEPHYRUM; v && json_est_tabulatum(v)
        && i < json_tabulatum_numerus(v); i++)
    {
        id = json_tabulatum_obtinere(v, i);
        si (id && json_est_chorda(id))
        {
            *(chorda*)xar_addere(exitus->res) = json_ad_chorda(id);
        }
    }
    exitus->plures = json_capere_boolean(pulsus, "plures", FALSUM);
    redde VERUM;
}
