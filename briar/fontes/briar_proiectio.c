/* briar_proiectio.c - Vide briar_proiectio.h. */

#include "briar_proiectio.h"
#include "briar_lexicon.h"
#include "briar_stml.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"

chorda
briar_proiectionem_scribere (
                  Piscina*  piscina,
    constans MateriaNodus*  radix,
       constans character** causa)
{
      MateriaLexiconRatum ratum;
       MateriaLexIudicium iudicium;
    MateriaArborConsilium consilium;
    MateriaArborScriptura s;
                   chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    *causa         = NIHIL;
    si (!materia_lexicon_ratum_facere(&ratum, &BRIAR_LEXICON,
        &iudicium))
    {
        *causa = "lexicon recusatum";
        redde vacua;
    }
    briar_stml_consilium(&consilium, &ratum);
    s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
    si (!s.successus)
    {
        *causa = s.causa ? s.causa : "scriptura fracta";
        redde vacua;
    }
    redde s.textus;
}
