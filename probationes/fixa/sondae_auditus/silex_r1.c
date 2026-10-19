/* silex_r1.c - sonda: plagula vendicata consulto remota (condita)
 * a 'renovare' resuscitaturne, si caput eius in clausura manet? */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "xar.h"
#include "silex.h"
#include "volumen.h"

#include <stdio.h>

#define AREA "build/sonda/silex_r1_area"

interior constans character*
_classis (SilexRenovatioStatus s)
{
    commutatio (s)
    {
        casus SILEX_RENOVATIO_RENOVANDA:  redde "RENOVANDA";
        casus SILEX_RENOVATIO_ADDENDA:    redde "ADDENDA";
        casus SILEX_RENOVATIO_VULNUS:     redde "VULNUS";
        casus SILEX_RENOVATIO_CONFLICTUS: redde "CONFLICTUS";
        casus SILEX_RENOVATIO_DERELICTA:  redde "DERELICTA";
    }
    redde "?";
}

interior vacuum
_monstrare (constans character* titulus, SilexRenovatioFructus r)
{
    i32 i;

    imprimere("%s: successus=%d intactae=%d renovatae=%d additae=%d"
        " res=%d\n", titulus, (integer)r.successus, (integer)r.intactae,
        (integer)r.renovatae, (integer)r.additae,
        r.res == NIHIL ? -1 : (integer)xar_numerus(r.res));
    si (r.res == NIHIL) redde;
    per (i = 0; i < xar_numerus(r.res); i = i + 1)
    {
        SilexRenovatioRes* e = (SilexRenovatioRes*)xar_obtinere(r.res, i);
        imprimere("  %.*s -> %s\n", (integer)e->via.mensura,
            (constans character*)e->via.datum, _classis(e->status));
    }
}

integer
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("sonda", 65536);
    SilexFons* fons;
    Volumen* vol;
    SilexConditioFructus cond;
    SilexRenovatioFructus r;

    filum_arborem_delere(AREA);
    filum_directorium_creare_cum_parentibus(AREA "/ficta/include");
    filum_directorium_creare_cum_parentibus(AREA "/ficta/lib");
    filum_directorium_creare_cum_parentibus(AREA "/proj/include");
    filum_directorium_creare_cum_parentibus(AREA "/proj/lib");
    filum_directorium_creare_cum_parentibus(AREA "/proj/fontes");
    filum_scribere_literis(AREA "/ficta/include/minima.h",
        "/* minima v1 */\n");
    filum_scribere_literis(AREA "/ficta/lib/minima.c",
        "#include \"minima.h\"\n/* corpus v1 */\n");
    fons = silex_fons_disci(piscina, AREA "/ficta");
    si (fons == NIHIL) { imprimere("fons NIHIL\n"); redde 1; }

    vol = volumen_creare(piscina, AREA "/proj/proj.volumen");
    volumen_plagulam_condere(vol,
        chorda_ex_literis("include/minima.h", piscina),
        chorda_ex_literis("/* minima v1 */\n", piscina),
        "vendicata:include/minima.h");
    volumen_plagulam_condere(vol,
        chorda_ex_literis("lib/minima.c", piscina),
        chorda_ex_literis("#include \"minima.h\"\n/* corpus v1 */\n",
            piscina),
        "vendicata:lib/minima.c");
    volumen_plagulam_condere(vol,
        chorda_ex_literis("fontes/meum.c", piscina),
        chorda_ex_literis("/* meum est */\n", piscina), "genita");
    volumen_claudere(vol);
    filum_scribere_literis(AREA "/proj/include/minima.h",
        "/* minima v1 */\n");
    filum_scribere_literis(AREA "/proj/lib/minima.c",
        "#include \"minima.h\"\n/* corpus v1 */\n");
    filum_scribere_literis(AREA "/proj/fontes/meum.c",
        "/* meum est */\n");

    _monstrare("ante remotionem", silex_renovare(piscina, AREA "/proj",
        fons, FALSUM));

    /* consulto amoveo lib/minima.c et condo */
    filum_delere(AREA "/proj/lib/minima.c");
    cond = silex_condere(piscina, AREA "/proj", "amoveo minima.c");
    imprimere("condere: successus=%d conditae=%d remotae=%d err=%s\n",
        (integer)cond.successus, (integer)cond.conditae,
        (integer)cond.remotae, cond.erratum ? cond.erratum : "-");

    r = silex_renovare(piscina, AREA "/proj", fons, FALSUM);
    _monstrare("post remotionem (consilium)", r);
    r = silex_renovare(piscina, AREA "/proj", fons, VERUM);
    _monstrare("post remotionem (scribere)", r);
    imprimere("lib/minima.c in disco post scribere: %d\n",
        (integer)filum_existit(AREA "/proj/lib/minima.c"));
    piscina_destruere(piscina);
    redde 0;
}
