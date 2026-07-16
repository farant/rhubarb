/* verificatio_sigilli.c - vectis sigilli II (K3): copia VIVA
 * (db + wal + shm + jsonl - lex K2 copiae WAL-plenae!) probatur:
 *   1. annales == acta (gesta_annales_verificare TENET)
 *   2. replicatio == tabulae stantes (octetim, res omnes)
 * Usus: verificatio_sigilli <via_db_copiae> <via_annalium_copiae>
 * Exitus 0 = AMBO TENENT. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "gesta.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    chorda res_id;
    chorda datum;
    chorda status;
} OrdoCaptus;

interior Xar*
_res_capere_omnes (GestaMundus* m, Piscina* piscina)
{
    Xar* fructus = xar_creare(piscina, (i32)magnitudo(OrdoCaptus));
    ScriniumEnuntiatum* e = scrinium_praeparare(gesta_scrinium(m),
        "SELECT res_id, datum, status FROM res ORDER BY res_id");

    si (fructus == NIHIL || e == NIHIL)
    {
        redde NIHIL;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        OrdoCaptus* o = (OrdoCaptus*)xar_addere(fructus);

        si (o != NIHIL)
        {
            o->res_id = scrinium_columna_textus(e, 0, piscina);
            o->datum = scrinium_columna_textus(e, I, piscina);
            o->status = scrinium_columna_textus(e, II, piscina);
        }
    }
    scrinium_finire(e);
    redde fructus;
}

interior b32
_pares (chorda a, chorda b)
{
    redde a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM
            || memcmp(a.datum, b.datum,
                   (memoriae_index)a.mensura) == ZEPHYRUM);
}

s32
principale (integer argc, character** argv)
{
    Piscina* piscina;
    GestaMundus* m;
    Xar* ante;
    Xar* post;
    i32 i;
    b32 sanum = VERUM;

    si (argc < III)
    {
        imprimere("usus: verificatio_sigilli <db> <jsonl>\n");
        redde I;
    }
    piscina = piscina_generare_dynamicum("verificatio", 268435456);
    si (piscina == NIHIL)
    {
        redde I;
    }
    m = gesta_aperire(piscina, argv[I], argv[II]);
    si (m == NIHIL)
    {
        imprimere("FRACTUM: mundus aperiri non potuit\n");
        redde I;
    }

    /* I. annales == acta */
    si (gesta_annales_verificare(m))
    {
        imprimere("I.  annales == acta: TENET (seq %d)\n",
            (int)gesta_seq_ultima(m));
    }
    alioquin
    {
        imprimere("I.  annales == acta: FRACTUM - %s\n",
            gesta_error(m));
        sanum = FALSUM;
    }

    /* II. replicatio == stantes */
    ante = _res_capere_omnes(m, piscina);
    si (ante == NIHIL || !gesta_replicare(m))
    {
        imprimere("II. replicatio FRACTA: %s\n", gesta_error(m));
        redde I;
    }
    post = _res_capere_omnes(m, piscina);
    si (post == NIHIL || xar_numerus(ante) != xar_numerus(post))
    {
        imprimere("II. replicatio == stantes: FRACTUM (numeri"
            " discrepant)\n");
        redde I;
    }
    per (i = ZEPHYRUM; i < xar_numerus(ante); i++)
    {
        OrdoCaptus* a = (OrdoCaptus*)xar_obtinere(ante, i);
        OrdoCaptus* p = (OrdoCaptus*)xar_obtinere(post, i);

        si (a == NIHIL || p == NIHIL
            || !_pares(a->res_id, p->res_id)
            || !_pares(a->datum, p->datum)
            || !_pares(a->status, p->status))
        {
            imprimere("II. discrepantia in ordine %d\n", (int)i);
            sanum = FALSUM;
            frange;
        }
    }
    si (sanum)
    {
        imprimere("II. replicatio == stantes: TENET (res %d"
            " octetim aequales)\n", (int)xar_numerus(ante));
    }
    redde sanum ? ZEPHYRUM : I;
}
