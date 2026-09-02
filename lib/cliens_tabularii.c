/* cliens_tabularii.c - Cliens daemonis tabulariumd (implementatio)
 *
 * Extractum verbatim ex apps/forum/forum.c 2026-07-24 (villa V2);
 * mutationes solae: nomina publica, ClientTabularii pro ForumStatus,
 * 'genitus' ex hic_manens in structuram motus, praefixum stderr
 * configurabile.
 */

#include "cliens_tabularii.h"
#include "tcp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* legere CC rerum cum corporibus - laxe (numerus romanus deest) */
#define RESPONSUM_CAPACITAS 262144


/* litterae -> chorda (sine copia; unio contra cast-qual) */
interior chorda
_ch (
    constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l        = litterae;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(litterae);
    redde c;
}

interior b32
_chorda_est (
                chorda  c,
    constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (memoriae_index)c.mensura == m
        && (m == ZEPHYRUM
            || memcmp(c.datum, litterae, m) == ZEPHYRUM);
}


/* ==================================================
 * cliens daemonis (per-petitionem, ut fori)
 * ================================================== */

interior TcpConnexio*
_conectere_aut_gignere (
    ClientTabularii* cliens,
            Piscina* pn)
{
    TcpResultus rc = tcp_connectere("127.0.0.1", cliens->portus,
        pn);
    i32 k;   /* comparatur cum mora_excitationis_ms (i32) */

    si (rc.successus)
    {
        redde rc.connexio;
    }
    si (cliens->genitus)
    {
        /* iam genitum semel: defectus CITUS - anquisitio IS (2 s)
         * tractatorem obstruentem 5 s congelaret */
        redde NIHIL;
    }
    {
        character imperium[CXXVIII];

        cliens->genitus = VERUM;
        fprintf(stderr, "%s tabulariumd absens - gigno per"
            " launcher (portus %d)\n", cliens->praefixum,
            (int)cliens->portus);
        sprintf(imperium, "%s -portus %d > /dev/null 2>&1 &",
            cliens->launcher, (int)cliens->portus);
        (vacuum)system(imperium);
    }
    /* CC ms per gradum; gradus = mora_excitationis_ms / CC (V s
     * ordinarie = XXV gradus). Probatio launcheris falsi moram
     * brevem ponit ne V s dormiat. */
    per (k = ZEPHYRUM; k < cliens->mora_excitationis_ms / CC; k++)
    {
        (vacuum)system("sleep 0.2");
        rc = tcp_connectere("127.0.0.1", cliens->portus, pn);
        si (rc.successus)
        {
            redde rc.connexio;
        }
    }
    redde NIHIL;
}

/* lineam mittere, responsum ad novam lineam legere (obstruens);
 * chorda vacua = defectus (culpa posita) */
interior chorda
_daemon_petere (
    ClientTabularii* cliens,
            Piscina* pn,
             chorda  linea,
             chorda* culpa)
{
         chorda  vacua;
      character* buf;
            s32  pos = ZEPHYRUM;
    TcpConnexio* conn;

    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    conn           = _conectere_aut_gignere(cliens, pn);
    si (conn == NIHIL)
    {
        *culpa = _ch("tabulariumd absens -"
            " ./gesta/tabulariumd.sh incipe");
        redde vacua;
    }
    buf = (character*)piscina_allocare(pn,
        (memoriae_index)RESPONSUM_CAPACITAS);
    si (   buf == NIHIL
        || !tcp_mittere_omnia(conn, (constans i8*)linea.datum,
               linea.mensura)
        || !tcp_mittere_omnia(conn, (constans i8*)"\n", I))
    {
        tcp_claudere(conn);
        *culpa = _ch("missio ad daemonem fracta");
        redde vacua;
    }
    per (;;)
    {
        s32 n = tcp_recipere(conn, (i8*)(buf + pos),
            (i32)(RESPONSUM_CAPACITAS - I - pos));
        s32 j;
        b32 completum = FALSUM;

        si (n <= ZEPHYRUM)
        {
            frange;
        }
        per (j = pos; j < pos + n; j++)
        {
            si (buf[j] == '\n')
            {
                pos        = j;
                completum  = VERUM;
                frange;
            }
        }
        si (completum)
        {
            frange;
        }
        pos += n;
        si (pos >= (s32)(RESPONSUM_CAPACITAS - I))
        {
            frange;
        }
    }
    tcp_claudere(conn);
    si (pos == ZEPHYRUM)
    {
        *culpa = _ch("responsum daemonis vacuum");
        redde vacua;
    }
    redde chorda_ex_buffer((i8*)buf, (i32)pos);
}

/* textum instrumenti ex involucro MCP extrahere:
 * {"result":{"content":[{"type":"text","text":...}]}} aut
 * {"error":{"message":...}} -> *est_error */
interior chorda
_textus_ex_responso (
     chorda  responsum,
    Piscina* pn,
        b32* est_error)
{
    JsonResultus  r = json_legere(responsum, pn);
       JsonValor* v;
          chorda  vacua;

    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    *est_error     = FALSUM;
    si (!r.successus || !json_est_objectum(r.radix))
    {
        *est_error = VERUM;
        redde vacua;
    }
    v = json_objectum_capere(r.radix, "error");
    si (v != NIHIL)
    {
        *est_error = VERUM;
        redde json_ad_chorda(json_objectum_capere(v, "message"));
    }
    v = json_objectum_capere(r.radix, "result");
    si (v != NIHIL)
    {
        JsonValor* contentum  = json_objectum_capere(v, "content");
        JsonValor* isError    = json_objectum_capere(v, "isError");

        si (isError != NIHIL && json_ad_boolean(isError))
        {
            *est_error = VERUM;
        }
        si (   contentum != NIHIL && json_est_tabulatum(contentum)
            && json_tabulatum_numerus(contentum) > ZEPHYRUM)
        {
            redde json_ad_chorda(json_objectum_capere(
                json_tabulatum_obtinere(contentum, ZEPHYRUM),
                "text"));
        }
    }
    *est_error = VERUM;
    redde vacua;
}

/* chorda -> litterae NUL-terminatae in piscina */
constans character*
cliens_tabularii_litterae (
    Piscina* pn,
     chorda  c)
{
    character* l = (character*)piscina_allocare(pn,
        (memoriae_index)(c.mensura + I));

    si (l == NIHIL)
    {
        redde "";
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(l, c.datum, (memoriae_index)c.mensura);
    }
    l[c.mensura] = '\0';
    redde l;
}

/* "res <ID> creata" -> chorda ID (vacua si absens) */
chorda
cliens_tabularii_res_id (
     chorda  textus,
    Piscina* pn)
{
    constans character* t = cliens_tabularii_litterae(pn, textus);
    constans character* p = strstr(t, "res ");
                chorda  id_c;
                   i32  n = ZEPHYRUM;

    id_c.mensura  = ZEPHYRUM;
    id_c.datum    = NIHIL;
    si (p == NIHIL)
    {
        redde id_c;
    }
    p += IV;
    dum (p[n] != '\0' && p[n] != ' ')
    {
        n++;
    }
    {
        i8* copia = (i8*)piscina_allocare(pn, (memoriae_index)n);
        unio { constans character* l; constans i8* m; } u;

        si (copia == NIHIL || n == ZEPHYRUM)
        {
            redde id_c;
        }
        u.l = p;
        memcpy(copia, u.m, (memoriae_index)n);
        id_c.datum    = copia;
        id_c.mensura  = n;
    }
    redde id_c;
}

/* instrumentum daemonis vocare: petitio aedificata, textus
 * responsi redditus (vacuus + culpa in defectu) */
chorda
cliens_tabularii_vocare (
       ClientTabularii* cliens,
               Piscina* pn,
    constans character* nomen_instrumenti,
             JsonValor* arg_obj,
                chorda* culpa)
{
    JsonValor* petitio  = json_objectum_creare(pn);
    JsonValor* params   = json_objectum_creare(pn);
       chorda  responsum;
       chorda  textus;
          b32  est_error = FALSUM;
       chorda  vacua;

    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    cliens->petitio_index++;
    json_objectum_ponere(petitio, "jsonrpc",
        json_chorda_creare_literis(pn, "2.0"));
    json_objectum_ponere(petitio, "id",
        json_integer_creare(pn, cliens->petitio_index));
    json_objectum_ponere(petitio, "method",
        json_chorda_creare_literis(pn, "tools/call"));
    json_objectum_ponere(params, "name",
        json_chorda_creare_literis(pn, nomen_instrumenti));
    json_objectum_ponere(params, "arguments", arg_obj);
    json_objectum_ponere(petitio, "params", params);
    responsum = _daemon_petere(cliens, pn,
        json_scribere(petitio, pn), culpa);
    si (responsum.mensura == ZEPHYRUM)
    {
        redde vacua;   /* culpa iam posita */
    }
    textus = _textus_ex_responso(responsum, pn, &est_error);
    si (est_error)
    {
        *culpa = textus.mensura > ZEPHYRUM ? textus
            : _ch("responsum daemonis ininterpretabile");
        redde vacua;
    }
    redde textus;
}

/* res generis a daemone: tabulatum JSON parsatum (NIHIL = culpa) */
JsonValor*
cliens_tabularii_legere_cum (
    ClientTabularii* cliens,
            Piscina* pn,
          JsonValor* arg_obj,
             chorda* culpa)
{
          chorda textus;
    JsonResultus r;

    si (arg_obj == NIHIL)
    {
        *culpa = _ch("argumenta desunt");
        redde NIHIL;
    }
    textus = cliens_tabularii_vocare(cliens, pn, "legere", arg_obj,
        culpa);
    si (textus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    r = json_legere(textus, pn);
    si (!r.successus || !json_est_tabulatum(r.radix))
    {
        *culpa = _ch("tabulatum legere fracta");
        redde NIHIL;
    }
    redde r.radix;
}

/* Involucrum vetus: genus + quantum sola. */
JsonValor*
cliens_tabularii_legere (
    ClientTabularii* cliens,
            Piscina* pn,
             chorda  genus,
                i32  quantum,
             chorda* culpa)
{
    JsonValor* arg_obj = json_objectum_creare(pn);

    json_objectum_ponere(arg_obj, "genus",
        json_chorda_creare(pn, genus));
    json_objectum_ponere(arg_obj, "quantum",
        json_integer_creare(pn, (s64)quantum));
    redde cliens_tabularii_legere_cum(cliens, pn, arg_obj, culpa);
}


/* ==================================================
 * tractatores internuntii
 * ================================================== */

/* transmittere {instrumentum, argumenta} (genera G2): tractator
 * generalis UNUS - IS instrumenta daemonis directe adhibet
 * (addere cum dato, gerere nexus/denexus/mutatio, quaerere).
 * Allowlist quattuor; actor="fran" SEMPER iniectus (app
 * instrumentum Franis). Post hoc genera/campi novi C numquam
 * reaperiunt. Fructus {bene, textus, res_id?}. */
JsonValor*
cliens_tabularii_transmittere (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
    ClientTabularii* cliens = (ClientTabularii*)datum;
             chorda  instrumentum;
          JsonValor* arg_obj = NIHIL;
             chorda  textus;
             chorda  novum_id;
          JsonValor* fructus;

    instrumentum.mensura  = ZEPHYRUM;
    instrumentum.datum    = NIHIL;
    si (argumenta != NIHIL)
    {
        instrumentum = json_ad_chorda(json_objectum_capere(
            argumenta, "instrumentum"));
        arg_obj = json_objectum_capere(argumenta, "argumenta");
    }
    si (!(_chorda_est(instrumentum, "addere")
          || _chorda_est(instrumentum, "gerere")
          || _chorda_est(instrumentum, "legere")
          || _chorda_est(instrumentum, "quaerere")))
    {
        *culpa = _ch("instrumentum extra allowlist"
            " (addere|gerere|legere|quaerere)");
        redde NIHIL;
    }
    si (arg_obj == NIHIL || !json_est_objectum(arg_obj))
    {
        arg_obj = json_objectum_creare(piscina);
    }
    json_objectum_ponere(arg_obj, "actor",
        json_chorda_creare_literis(piscina, cliens->actor));
    textus = cliens_tabularii_vocare(cliens, piscina,
        cliens_tabularii_litterae(piscina, instrumentum), arg_obj,
        culpa);
    si (textus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "bene",
        json_boolean_creare(piscina, VERUM));
    json_objectum_ponere(fructus, "textus",
        json_chorda_creare(piscina, textus));
    novum_id = cliens_tabularii_res_id(textus, piscina);
    si (novum_id.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(fructus, "res_id",
            json_chorda_creare(piscina, novum_id));
    }
    redde fructus;
}


/* ========================================================================
 * INITIUM
 * ======================================================================== */

vacuum
cliens_tabularii_incipere (
    ClientTabularii* cliens,
                i32  portus)
{
    si (cliens == NIHIL)
    {
        redde;
    }
    cliens->portus                = portus;
    cliens->petitio_index         = (s64)I;
    cliens->actor                 = "fran";
    cliens->launcher              = "./gesta/tabulariumd.sh";
    cliens->praefixum             = "[cliens]";
    cliens->mora_excitationis_ms  = V * M;
    cliens->genitus               = FALSUM;
}
