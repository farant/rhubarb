/* probatio_fori.c - F1 forum: fumus proxyis stdio<->TCP.
 *
 * Daemon VERUS mundo effimero (portu auto) + proxy VERUS per
 * canalem systematis (printf | fori > exitus) - probat: petitio
 * respondet trans proxy, NUNTIATIO non suspendit (nuntiatio inter
 * petitiones = responsa duo exacte), pipatum signatum, daemon
 * absens = error curae portu vero nominato (-sine-generatione:
 * nullus daemon lateralis in probationibus). */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VIA_DB   "gesta/build/probatio_fori.db"
#define VIA_AN   "gesta/build/probatio_fori.jsonl"
#define VIA_PORT "gesta/build/probatio_fori_portus.txt"
#define VIA_PID  "gesta/build/probatio_fori_pid.txt"
#define VIA_EX   "gesta/build/probatio_fori_exitus.txt"

/* mensura imperii canalis (MMXLVIII in latina.h deest) */
#define IMPERIUM_MENSURA 2048

interior vacuum
_purgare (vacuum)
{
    remove(VIA_DB);
    remove(VIA_DB "-wal");
    remove(VIA_DB "-shm");
    remove(VIA_AN);
    remove(VIA_PORT);
    remove(VIA_PID);
    remove(VIA_EX);
}

/* plagulam totam ut litterae (vacuae si abest) */
interior constans character*
_plagula_litterae (Piscina* piscina, constans character* via)
{
    FILE* pl = fopen(via, "rb");
    long mensura;
    character* textus;

    si (pl == NIHIL)
    {
        redde "";
    }
    fseek(pl, 0L, SEEK_END);
    mensura = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura > 0L ? mensura + 1L : I));
    si (textus == NIHIL
        || (mensura > 0L
            && fread(textus, I, (memoriae_index)mensura, pl)
                != (memoriae_index)mensura))
    {
        fclose(pl);
        redde "";
    }
    textus[mensura] = '\0';
    fclose(pl);
    redde textus;
}

/* numerus eventuum '\n' in textu */
interior s32
_lineas_numerare (constans character* textus)
{
    s32 n = ZEPHYRUM;
    constans character* p;

    per (p = textus; *p != '\0'; p++)
    {
        si (*p == '\n')
        {
            n++;
        }
    }
    redde n;
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;
    integer portus_int = 0;
    integer k;

    piscina = piscina_generare_dynamicum("probatio_fori",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    _purgare();

    /* aedificationes + genitus daemonis portu auto */
    CREDO_VERUM (system("./gesta/tabulariumd.sh -struere"
        " > /dev/null 2>&1") == ZEPHYRUM);
    CREDO_VERUM (system("./gesta/fori.sh -struere"
        " > /dev/null 2>&1") == ZEPHYRUM);
    CREDO_VERUM (system("gesta/build/tabulariumd -portus 0"
        " -scrinium " VIA_DB " -annales " VIA_AN
        " > " VIA_PORT " 2> /dev/null"
        " & echo $! > " VIA_PID) == ZEPHYRUM);
    per (k = ZEPHYRUM; k < XXV; k++)
    {
        FILE* pf = fopen(VIA_PORT, "r");

        si (pf != NIHIL)
        {
            si (fscanf(pf, "portus %d", &portus_int) == I
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
        character imperium[IMPERIUM_MENSURA];
        constans character* exitus;

        /* petitio + NUNTIATIO + petitio signata una serie:
         * responsa DUO exacte (nuntiatio nec respondetur nec
         * suspendit) */
        sprintf(imperium, "printf '%%s\\n%%s\\n%%s\\n'"
            " '{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":"
            "\"initialize\",\"params\":{}}'"
            " '{\"jsonrpc\":\"2.0\",\"method\":"
            "\"notifications/initialized\"}'"
            " '{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":"
            "\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"pipatum\",\"titulus\":"
            "\"pipatum per proxy\",\"corpus\":\"trans proxy.\","
            "\"signatura\":\"Fable 5\"}}}'"
            " | gesta/build/fori -mcp -portus %d"
            " -sine-generatione > " VIA_EX " 2>/dev/null",
            portus_int);
        CREDO_VERUM (system(imperium) == ZEPHYRUM);
        exitus = _plagula_litterae(piscina, VIA_EX);
        CREDO_VERUM (_lineas_numerare(exitus) == II);
        CREDO_VERUM (strstr(exitus, "protocolVersion") != NIHIL);
        CREDO_VERUM (strstr(exitus, "creata") != NIHIL);
        CREDO_VERUM (strstr(exitus, "pipatum per proxy")
            != NIHIL);

        /* signatura trans proxy in annales cecidit */
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_AN),
            "Fable 5") != NIHIL);

        /* daemon necatus -> error curae portu VERO nominato */
        (vacuum)system("kill $(cat " VIA_PID ") 2>/dev/null");
        (vacuum)system("sleep 0.3");
        sprintf(imperium, "printf '%%s\\n'"
            " '{\"jsonrpc\":\"2.0\",\"id\":9,\"method\":"
            "\"initialize\",\"params\":{}}'"
            " | gesta/build/fori -portus %d -sine-generatione"
            " > " VIA_EX " 2>/dev/null", portus_int);
        CREDO_VERUM (system(imperium) == ZEPHYRUM);
        exitus = _plagula_litterae(piscina, VIA_EX);
        CREDO_VERUM (strstr(exitus, "tabulariumd absens")
            != NIHIL);
        {
            character portus_textus[XXXII];

            sprintf(portus_textus, "portus %d", portus_int);
            CREDO_VERUM (strstr(exitus, portus_textus) != NIHIL);
        }
    }

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
