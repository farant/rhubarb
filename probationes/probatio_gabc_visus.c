/* probatio_gabc_visus.c - visus cantus gabc ex textu in memoria
 * (gabc_visus.h; psalterium planum T2)
 *
 * Quae probatio_antiphonale_latus per latus iam probat (pictura,
 * paginae, sonus, G18 per plagulam) hic non iterantur: solum quod
 * visus novum facit - textus in memoria (nulla plagula), vexillum ex
 * textu, et continuare FALSUM (sonus sistit, electa -1). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "gabc_visus.h"
#include "credo.h"
#include <string.h>

#define CANTUS_A   "name: A;\n%%\n(c4) A(g)men.(h) (::)\n"
#define CANTUS_B   "name: B;\n%%\n(c4) A(h)men.(g) (::)\n"
#define CANTUS_C   "name: C;\n%%\n(c4) A(f)men.(g) (::)\n"
#define CANTUS_MALUS "%%\n(c4) A(g\n"

nomen structura {
    GabcVisusPorta porta;
               b32 sonat;
               s64 pulsus;
               i32 incepta;
               s64 pulsus_incepti;
               i32 sistita;
} Porta;

interior b32
_incipere (
    vacuum* ctx,
    chorda  carmen,
       s64  pulsus)
{
    Porta* w = (Porta*)ctx;

    (vacuum)carmen;
    w->incepta++;
    w->pulsus_incepti  = pulsus;
    w->sonat           = VERUM;
    w->pulsus          = pulsus;
    redde VERUM;
}

interior vacuum
_quaerere (
    vacuum* ctx,
       s64  pulsus)
{
    ((Porta*)ctx)->pulsus = pulsus;
}

interior vacuum
_sistere (
    vacuum* ctx)
{
    Porta* w = (Porta*)ctx;

    w->sistita++;
    w->sonat = FALSUM;
}

interior s64
_pulsus (
    vacuum* ctx)
{
    Porta* w = (Porta*)ctx;

    redde w->sonat ? w->pulsus : -I;
}

interior GabcVisus*
_visum (
    CredoContextus* c,
             Porta* w)
{
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
            InsulaRamus* ramus;

    intern = internamentum_creare(c->piscina);
    repo = insula_repositorium_creare(c->piscina, intern,
        "<hospes/>");
    ramus   = (InsulaRamus*)piscina_allocare(c->piscina,
        magnitudo(InsulaRamus));
    si (!repo || !ramus)
    {
        redde NIHIL;
    }
    *ramus = insula_ramus_radix(repo);
    si (w)
    {
        memset(w, ZEPHYRUM, magnitudo(Porta));
        w->porta.incipere  = _incipere;
        w->porta.quaerere  = _quaerere;
        w->porta.sistere   = _sistere;
        w->porta.pulsus    = _pulsus;
        w->porta.ctx       = w;
    }
    redde gabc_visus_creare(c->piscina, ramus, "probatio.agere",
        "probatio", w ? &w->porta : NIHIL, ZEPHYRUM, ZEPHYRUM,
        ZEPHYRUM);
}

interior b32
_titulus_est (
             GabcVisus* visus,
    constans character* exspectatus)
{
    redde chorda_aequalis_literis(gabc_visus_titulus(visus),
        exspectatus);
}

interior vacuum
_textum_probare (
    CredoContextus* c)
{
    GabcVisus* visus = _visum(c, NIHIL);

    CREDO_NECESSE (visus != NIHIL);
    CREDO_FALSUM (gabc_visus_habet_versionem(visus));

    CREDO_GRADUS ("textus bonus: versio, titulus, nullum vexillum");
    CREDO_VERUM (gabc_visus_textum_ponere(visus,
        chorda_ex_literis(CANTUS_A, c->piscina), VERUM));
    CREDO_VERUM (gabc_visus_habet_versionem(visus));
    CREDO_VERUM (_titulus_est(visus, "A"));
    CREDO_LITERAE_AEQUALES (gabc_visus_vexillum(visus), "");

    CREDO_GRADUS ("idem textus: nihil mutatum");
    CREDO_FALSUM (gabc_visus_textum_ponere(visus,
        chorda_ex_literis(CANTUS_A, c->piscina), VERUM));

    CREDO_GRADUS ("textus malus: vexillum linea:columna, versio servata");
    CREDO_VERUM (gabc_visus_textum_ponere(visus,
        chorda_ex_literis(CANTUS_MALUS, c->piscina), VERUM));
    CREDO_VERUM (gabc_visus_vexillum(visus)[ZEPHYRUM] >= '1'
              && gabc_visus_vexillum(visus)[ZEPHYRUM] <= '9');
    CREDO_VERUM (strchr(gabc_visus_vexillum(visus), ':') != NIHIL);
    CREDO_VERUM (_titulus_est(visus, "A"));

    CREDO_GRADUS ("textus bonus idem post malum: vexillum tollitur");
    CREDO_VERUM (gabc_visus_textum_ponere(visus,
        chorda_ex_literis(CANTUS_A, c->piscina), VERUM));
    CREDO_LITERAE_AEQUALES (gabc_visus_vexillum(visus), "");

    CREDO_GRADUS ("textus vacuus: vexillum, versio servata");
    CREDO_VERUM (gabc_visus_textum_ponere(visus,
        chorda_ex_literis("", c->piscina), VERUM));
    CREDO_LITERAE_AEQUALES (gabc_visus_vexillum(visus), "empty text");
    CREDO_VERUM (_titulus_est(visus, "A"));

    CREDO_GRADUS ("vexillum involucri ponitur et tollitur");
    gabc_visus_vexillum_ponere(visus, "missing: x.gabc");
    CREDO_LITERAE_AEQUALES (gabc_visus_vexillum(visus),
        "missing: x.gabc");
    gabc_visus_vexillum_ponere(visus, NIHIL);
    CREDO_LITERAE_AEQUALES (gabc_visus_vexillum(visus), "");

    CREDO_GRADUS ("vacuare: versio nulla");
    gabc_visus_vacuare(visus);
    CREDO_FALSUM (gabc_visus_habet_versionem(visus));
}

interior vacuum
_continuare_probare (
    CredoContextus* c)
{
        Porta  w;
    GabcVisus* visus = _visum(c, &w);

    CREDO_NECESSE (visus != NIHIL);
    CREDO_NECESSE (gabc_visus_textum_ponere(visus,
        chorda_ex_literis(CANTUS_A, c->piscina), VERUM));
    CREDO_VERUM (gabc_visus_canere(visus));
    CREDO_AEQUALIS_I32 (w.incepta, I);
    CREDO_VERUM (gabc_visus_sonat(visus));

    CREDO_GRADUS ("continuare VERUM: versio nova ad pulsum eundem");
    w.pulsus = C;
    CREDO_VERUM (gabc_visus_textum_ponere(visus,
        chorda_ex_literis(CANTUS_B, c->piscina), VERUM));
    CREDO_AEQUALIS_I32 (w.incepta, II);
    CREDO_AEQUALIS_S64 (w.pulsus_incepti, (s64)C);
    CREDO_VERUM (gabc_visus_sonat(visus));
    CREDO_AEQUALIS_I32 (w.sistita, ZEPHYRUM);

    CREDO_GRADUS ("continuare FALSUM: sonus sistit, non iterum incipit");
    CREDO_VERUM (gabc_visus_textum_ponere(visus,
        chorda_ex_literis(CANTUS_C, c->piscina), FALSUM));
    CREDO_AEQUALIS_I32 (w.sistita, I);
    CREDO_AEQUALIS_I32 (w.incepta, II);
    CREDO_FALSUM (gabc_visus_sonat(visus));
    CREDO_VERUM (_titulus_est(visus, "C"));

    CREDO_GRADUS ("Spatium post FALSUM: ab initio (pulsus 0)");
    CREDO_VERUM (gabc_visus_canere(visus));
    CREDO_AEQUALIS_I32 (w.incepta, III);
    CREDO_AEQUALIS_S64 (w.pulsus_incepti, (s64)ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "textus in memoria", _textum_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "continuare", _continuare_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("gabc_visus", SECTIONES);
}
