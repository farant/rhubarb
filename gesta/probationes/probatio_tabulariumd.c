/* probatio_tabulariumd.c - F0 forum: fumus socketi daemonis.
 *
 * Daemon VERUS in mundo effimero (portu 0 auto-selecto) per
 * system() genitus; cliens = tcp domus (obstruens, per-petitionem
 * - contractus v1 spec-v2 par I). Probat trans filum: initialize
 * (bis, connexionibus separatis = idempotentia), addere pipatum
 * signatum, custodem CCXL, acta. Aedificatio daemonis per
 * ./gesta/tabulariumd.sh -struere (porta nativitatis launcheri). */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VIA_DB   "gesta/build/probatio_tabd.db"
#define VIA_AN   "gesta/build/probatio_tabd.jsonl"
#define VIA_PORT "gesta/build/probatio_tabd_portus.txt"
#define VIA_PID  "gesta/build/probatio_tabd_pid.txt"
#define VIA_ERR  "gesta/build/probatio_tabd_err.txt"

/* capacitas responsi (doctrina initialize ~2KB; laxe) - numerus
 * romanus 16384 in latina.h deest */
#define RESPONSUM_CAPACITAS 16384

interior vacuum
_purgare (vacuum)
{
    remove(VIA_DB);
    remove(VIA_DB "-wal");
    remove(VIA_DB "-shm");
    remove(VIA_AN);
    remove(VIA_PORT);
    remove(VIA_PID);
    remove(VIA_ERR);
}

/* lineam mittere, responsum ad novam lineam legere (obstruens) */
interior constans character*
_petere (
           TcpConnexio* conn,
               Piscina* pn,
    constans character* linea)
{
    character* buf = (character*)piscina_allocare(pn,
        (memoriae_index)RESPONSUM_CAPACITAS);
    s32 pos = ZEPHYRUM;

    si (buf == NIHIL)
    {
        redde "";
    }
    si (!tcp_mittere_omnia(conn, (constans i8*)linea,
            (i32)strlen(linea)))
    {
        redde "";
    }
    per (;;)
    {
        s32 n = tcp_recipere(conn, (i8*)(buf + pos),
            (i32)(RESPONSUM_CAPACITAS - I - pos));
        s32 j;

        si (n <= ZEPHYRUM)
        {
            frange;
        }
        per (j = pos; j < pos + n; j++)
        {
            si (buf[j] == '\n')
            {
                buf[j] = '\0';
                redde buf;
            }
        }
        pos += n;
        si (pos >= (s32)(RESPONSUM_CAPACITAS - I))
        {
            frange;
        }
    }
    buf[pos] = '\0';
    redde buf;
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;
    integer  portus_int = 0;
    integer  k;

    piscina = piscina_generare_dynamicum("probatio_tabd",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    _purgare();

    /* aedificatio + genitus daemonis (portus 0 = auto; portus
     * ligatus in stdout cadit) */
    CREDO_VERUM (system("./gesta/tabulariumd.sh -struere"
        " > /dev/null 2>&1") == ZEPHYRUM);
    CREDO_VERUM (system("gesta/build/tabulariumd -portus 0"
        " -scrinium " VIA_DB " -annales " VIA_AN
        " > " VIA_PORT " 2> " VIA_ERR
        " & echo $! > " VIA_PID) == ZEPHYRUM);

    /* portum anquirere (ad V secunda) */
    per (k = ZEPHYRUM; k < XXV; k++)
    {
        FILE* pf = fopen(VIA_PORT, "r");

        si (pf != NIHIL)
        {
            si (   fscanf(pf, "portus %d", &portus_int) == I
                && portus_int > 0)
            {
                fclose(pf);
                frange;
            }
            fclose(pf);
        }
        (vacuum)system("sleep 0.2");
    }
    CREDO_VERUM (portus_int > 0);

    si (portus_int > 0)
    {
               TcpResultus  rc;
        constans character* r;

        /* connexio prima: initialize + pipatum signatum + acta */
        rc = tcp_connectere("127.0.0.1", (i32)portus_int, piscina);
        per (k = ZEPHYRUM; k < X && !rc.successus; k++)
        {
            (vacuum)system("sleep 0.2");
            rc = tcp_connectere("127.0.0.1", (i32)portus_int,
                piscina);
        }
        CREDO_VERUM (rc.successus);
        si (rc.successus)
        {
            r = _petere(rc.connexio, piscina,
                "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":"
                "\"initialize\",\"params\":{}}\n");
            CREDO_VERUM (strstr(r, "\"result\"") != NIHIL);
            CREDO_VERUM (strstr(r, "protocolVersion") != NIHIL);
            r = _petere(rc.connexio, piscina,
                "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":"
                "\"tools/call\",\"params\":{\"name\":\"addere\","
                "\"arguments\":{\"genus\":\"pipatum\",\"titulus\":"
                "\"pipatum fumi\",\"corpus\":\"salve trans"
                " filum.\",\"signatura\":\"Fable 5\"}}}\n");
            CREDO_VERUM (strstr(r, "creata") != NIHIL);
            r = _petere(rc.connexio, piscina,
                "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":"
                "\"tools/call\",\"params\":{\"name\":\"acta\","
                "\"arguments\":{}}}\n");
            CREDO_VERUM (strstr(r, "pipatum fumi") != NIHIL);
            tcp_claudere(rc.connexio);
        }

        /* connexio secunda (contractus per-petitionem):
         * initialize ITERUM = idempotens trans filum + custos */
        rc = tcp_connectere("127.0.0.1", (i32)portus_int, piscina);
        CREDO_VERUM (rc.successus);
        si (rc.successus)
        {
            character linea[DXII];
            character longum[CCXLII];
                  s32 i;

            r = _petere(rc.connexio, piscina,
                "{\"jsonrpc\":\"2.0\",\"id\":4,\"method\":"
                "\"initialize\",\"params\":{}}\n");
            CREDO_VERUM (strstr(r, "\"result\"") != NIHIL);
            CREDO_VERUM (strstr(r, "iam initiatum") == NIHIL);
            per (i = ZEPHYRUM; i < CCXLI; i++)
            {
                longum[i] = 'a';
            }
            longum[CCXLI] = '\0';
            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":5,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"addere\",\"arguments\":{\"genus\":\"pipatum\","
                "\"titulus\":\"nimium\",\"corpus\":\"%s\"}}}\n",
                longum);
            r = _petere(rc.connexio, piscina, linea);
            CREDO_VERUM (strstr(r,
                "pipatum nimis longum (limes CCXL codicilli)")
                != NIHIL);
            tcp_claudere(rc.connexio);
        }
    }

    /* daemon necare (quietus si iam mortuus) */
    (vacuum)system("kill $(cat " VIA_PID ") 2>/dev/null");

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
