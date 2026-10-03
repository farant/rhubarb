/* probatio_importatio_visus.c - zoom per rotulam integram (eventus S3b;
 * spec D2): importatio_visus olim delta_y f32 (scrollingDelta crudum)
 * x 0.5 legebat. Nunc dy + genus: GRADATA -> dy / gradus_rotulae x 0.5
 * (gradus ex FACULTATIBUS fontis; ordinarie fenestrae XVI), PRAECISA
 * -> dy x 0.5 (pixela nostra ~ puncta ad scalam I). Sensus idem. */
#include "latina.h"
#include "piscina.h"
#include "eventus.h"
#include "imago_typus.h"
#include "importatio_visus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Eventus
_rotula (
    EventusRotulaGenus genus,
                   s32 dy)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus               = EVENTUS_MUS_ROTULA;
    e.datum.rotula.genus  = genus;
    e.datum.rotula.dy     = dy;
    redde e;
}

interior b32
_prope (
    f32 a,
    f32 b)
{
    f32 d = a - b;

    redde (b32)(d < 0.001f && d > -0.001f);
}

s32 principale (vacuum)
{
          Piscina* piscina;
  ImportatioVisus* v;
            Imago  imago;
               i8  pixela[XVI * IV];
          Eventus  e;

    piscina = piscina_generare_dynamicum("probatio_importatio_visus",
        M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    memset(pixela, ZEPHYRUM, magnitudo(pixela));
    imago.pixela    = pixela;
    imago.latitudo  = IV;
    imago.altitudo  = IV;
    v               = importatio_visus_creare(piscina);
    CREDO_NON_NIHIL (v);
    importatio_visus_initiare_sessionem(v, &imago);
    CREDO_VERUM (_prope(v->zoom, 1.0f));

    imprimere("\n--- I. rota: gradus unus (XVI) = +0.5 ---\n");
    e = _rotula(EVENTUS_ROTULA_GRADATA, XVI);
    CREDO_VERUM (importatio_visus_tractare_eventum(v, &e));
    CREDO_VERUM (_prope(v->zoom, 1.5f));

    imprimere("\n--- II. trackpad: pixelum unum = +0.5 ---\n");
    e = _rotula(EVENTUS_ROTULA_PRAECISA, I);
    CREDO_VERUM (importatio_visus_tractare_eventum(v, &e));
    CREDO_VERUM (_prope(v->zoom, 2.0f));

    imprimere("\n--- III. gradus ex FACULTATIBUS (XX) ---\n");
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                            = EVENTUS_FACULTATES;
    e.datum.facultates.gradus_rotulae  = XX;
    (vacuum)importatio_visus_tractare_eventum(v, &e);
    e = _rotula(EVENTUS_ROTULA_GRADATA, -XX);
    CREDO_VERUM (importatio_visus_tractare_eventum(v, &e));
    CREDO_VERUM (_prope(v->zoom, 1.5f));

    imprimere("\n--- IV. dimidius gradus: +0.25 ---\n");
    e = _rotula(EVENTUS_ROTULA_GRADATA, X);
    CREDO_VERUM (importatio_visus_tractare_eventum(v, &e));
    CREDO_VERUM (_prope(v->zoom, 1.75f));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
