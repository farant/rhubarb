/* sonda: ":memoria:" vere in memoria? */
#include "latina.h"
#include "piscina.h"
#include "scrinium.h"
#include <stdio.h>
#include <unistd.h>

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Scrinium* s;
    ScriniumEnuntiatum* e;
    FILE* f;
    integer gradus;

    si (chdir("build/sonda/scrinium_r1_dir") != 0) { imprimere("chdir fractum\n"); redde 1; }
    remove(":memoria:"); remove(":memoria:-wal"); remove(":memoria:-shm");
    s = scrinium_aperire(p, ":memoria:");
    imprimere("aperire: %s\n", s ? "bene" : "NIHIL");
    imprimere("create: %d\n", (integer)scrinium_exsequi(s, "CREATE TABLE t (x INTEGER)"));
    imprimere("insert: %d\n", (integer)scrinium_exsequi(s, "INSERT INTO t VALUES (7)"));
    scrinium_claudere(s);
    f = fopen(":memoria:", "rb");
    imprimere("plagula ':memoria:' in disco: %s\n", f ? "EXSTAT" : "nulla");
    si (f) fclose(f);
    s = scrinium_aperire(p, ":memoria:");
    e = scrinium_praeparare(s, "SELECT x FROM t");
    si (e == NIHIL) { imprimere("tabula t post reaperturam: nulla (in memoria)\n"); }
    alioquin {
        gradus = scrinium_gradi(e);
        imprimere("tabula t post reaperturam PERSISTIT: gradus %d x=%lld\n", gradus,
            (longus longus)scrinium_columna_numerus(e, 0));
        scrinium_finire(e);
    }
    scrinium_claudere(s);
    /* comparatio: ":memory:" */
    s = scrinium_aperire(p, ":memory:");
    (vacuum)scrinium_exsequi(s, "CREATE TABLE t (x INTEGER)");
    scrinium_claudere(s);
    f = fopen(":memory:", "rb");
    imprimere("plagula ':memory:' in disco: %s\n", f ? "EXSTAT" : "nulla");
    si (f) fclose(f);
    remove(":memoria:"); remove(":memoria:-wal"); remove(":memoria:-shm");
    piscina_destruere(p);
    redde 0;
}
