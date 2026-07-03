/* hospes.c - The first true host of the tessera deliverable.
 *
 * VANILLA C89 ON PURPOSE: includes ONLY tessera.h + the standard
 * library. Proves (Phase C):
 *   1. HOST POLLUTION: variables named after latina keyword macros
 *      compile and WORK - one leaked define and this does not build.
 *   2. EQUIVALENCE SMOKE: the amalgam's emitter produces the exact
 *      golden bytes the modular suite pins, and the input parser
 *      yields the same events - through the memoria pons (no tty
 *      needed; amalgamare.sh runs this headless).
 */
#include <stdio.h>
#include <string.h>
#include "tessera.h"

/* --- latina keyword names as ordinary identifiers --- */
static int si = 1;
static double per = 2.0;
static char character = 'c';
static int nomen = 3;
static int magnitudo = 4;
static int redde = 5;
static int dum = 6;
static int vacuum = 7;

static int fideles = 0;
static int summa = 0;

static void proba(int conditio, const char* titulus)
{
    summa++;
    if (conditio) {
        fideles++;
    } else {
        fprintf(stderr, "hospes: INFIDELIS: %s\n", titulus);
    }
}

int main(void)
{
    TesseraPiscina* piscina;
    TesseraPonsMemoriae* pm;
    TesseraOpus* opus;
    TesseraLector* lector;

    /* the polluted names must actually work */
    si += nomen;
    per += (double)magnitudo;
    character = (char)(character + 1);
    redde += dum + vacuum;
    if (si != 4 || character != 'd' || redde != 18) {
        fprintf(stderr, "hospes: nomina latina corrupta?!\n");
        return 1;
    }

    piscina = tessera_piscina_generare_dynamicum("hospes", 8388608);
    if (piscina == NULL) {
        fprintf(stderr, "hospes: piscina deest\n");
        return 1;
    }
    pm = tessera_pons_memoriae_creare(piscina, 8, 3);
    opus = tessera_aperire(piscina, &pm->pons);
    lector = tessera_lector_creare(piscina, &pm->pons);
    proba(pm != NULL && opus != NULL && lector != NULL, "apertura");

    /* golden first frame (mirrors probatio_tessera_opus) */
    {
        TesseraChorda captum;
        const char* speratum = "\033[?25l\033[2J\033[1;1H\033[0mab";

        tessera_scribere_literis(opus, 0, 0, "ab",
            tessera_stilus_nativus());
        proba(tessera_praesentare(opus), "praesentatio");
        captum = tessera_pons_memoriae_captum(pm);
        proba(captum.mensura == (unsigned int)strlen(speratum)
            && memcmp(captum.datum, speratum, captum.mensura) == 0,
            "quadrum primum aureum");
    }

    /* zero-byte empty frame */
    {
        TesseraChorda captum;

        tessera_pons_memoriae_purgare(pm);
        proba(tessera_praesentare(opus), "praesentatio vacua");
        captum = tessera_pons_memoriae_captum(pm);
        proba(captum.mensura == 0, "quadrum vacuum = nihil");
    }

    /* UTF-8 cell through packed signum */
    {
        TesseraCellula cella;

        tessera_scribere_literis(opus, 0, 1, "\xE2\x94\x80",
            tessera_stilus_nativus());
        cella = tessera_cellulam_legere(opus, 0, 1);
        proba(cella.signum == TESSERA_SIGNUM_SIMPLEX_H,
            "signum compactum");
    }

    /* input: arrow key through the lector */
    {
        TesseraEventum ev;

        tessera_pons_memoriae_initum(pm,
            (const unsigned char*)"\033[A", 3);
        tessera_eventum_expectare(lector, &ev, 10);
        proba(ev.genus == TESSERA_EVENTUM_CLAVIS
            && ev.clavis == TESSERA_CLAVIS_SURSUM, "clavis sursum");
    }

    printf("hospes: %d/%d fideles (pollutio nulla)\n", fideles,
        summa);
    tessera_piscina_destruere(piscina);
    return (fideles == summa) ? 0 : 1;
}
