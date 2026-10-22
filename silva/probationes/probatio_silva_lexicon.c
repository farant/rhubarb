/* probatio_silva_lexicon.c - derivatio sectionum lexici POSIX
 * (silva/instrumenta/silva_lexicon.h, functio I)
 *
 * Lexicon et plagulae SYNTHETICA (chordae breves) ut regula ipsa
 * probetur, non contentum systema_posix.h; sectio una realis
 * (postulata_atomica.h) contra plagulam realem. Regulae: semper
 * semper; caput= per '#include <...>'; 'poscit' transitive;
 * inclusio citata SOLUM 'postulata_*.h' evocat (sonus spec X.7,
 * 2026-10-21) - capita domus cetera per ambulationem repositorii
 * resolvuntur, non per lexicon.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "credo.h"
#include "silva_lexicon.h"
#include <string.h>

hic_manens constans character LEXICON[] =
    "/* <sectio semper=\"verum\"/> */\n"
    "typedef int SEMPER_TYPUS;\n"
    "/* <sectio caput=\"sys/a.h\"/> */\n"
    "int A_FUNCTIO(void);\n"
    "/* <sectio caput=\"sys/b.h\" poscit=\"sys/a.h\"/> */\n"
    "int B_FUNCTIO(void);\n"
    "/* <sectio caput=\"postulata_proba.h\"/> */\n"
    "#define PROBA_POSTULATA 7\n"
    "/* <sectio caput=\"domus.h\"/> */\n"
    "int DOMUS_FUNCTIO(void);\n";

/* VERUM si 'textus' in effusione (mensurae m) occurrit */
interior b32
_continet (
    constans character* effusio,
                   i32  m,
    constans character* textus)
{
    i32 n = (i32)strlen(textus);
    i32 k;

    si (effusio == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + n <= m; k++)
    {
        si (memcmp(effusio + k, textus, (memoriae_index)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior character*
_derivare (
    constans character* plagula,
               Piscina* piscina,
                   i32* m)
{
    redde silva_lexicon_posix_derivare(LEXICON, (i32)strlen(LEXICON),
                                       plagula, (i32)strlen(plagula),
                                       piscina, m);
}


    /* ==================================================
     * REGULAE: semper, caput angulatum, poscit
     * ================================================== */

interior vacuum
_sectio_regulae (
    CredoContextus* c)
{
    character* e;
          i32  m;

    e = _derivare("int x;\n", c->piscina, &m);
    CREDO_VERUM (_continet(e, m, "SEMPER_TYPUS"));
    CREDO_VERUM (!_continet(e, m, "A_FUNCTIO"));

    e = _derivare("#include <sys/a.h>\n", c->piscina, &m);
    CREDO_VERUM (_continet(e, m, "A_FUNCTIO"));
    CREDO_VERUM (!_continet(e, m, "B_FUNCTIO"));

    /* poscit: b trahit a */
    e = _derivare("#include <sys/b.h>\n", c->piscina, &m);
    CREDO_VERUM (_continet(e, m, "B_FUNCTIO"));
    CREDO_VERUM (_continet(e, m, "A_FUNCTIO"));
}


    /* ==================================================
     * POSTULATA: inclusio citata 'postulata_*.h' evocat, ceterae non
     * ================================================== */

interior vacuum
_sectio_postulata (
    CredoContextus* c)
{
    character* e;
          i32  m;

    e = _derivare("#include \"postulata_proba.h\"\n", c->piscina, &m);
    CREDO_VERUM (_continet(e, m, "PROBA_POSTULATA"));

    /* angulata quoque (forma nulla domus, sed regula una) */
    e = _derivare("#include <postulata_proba.h>\n", c->piscina, &m);
    CREDO_VERUM (_continet(e, m, "PROBA_POSTULATA"));

    /* caput domus citatum NON evocat sectionem cognominem */
    e = _derivare("#include \"domus.h\"\n", c->piscina, &m);
    CREDO_VERUM (!_continet(e, m, "DOMUS_FUNCTIO"));
    CREDO_VERUM (!_continet(e, m, "PROBA_POSTULATA"));

    /* sine inclusione: nihil */
    e = _derivare("int y;\n", c->piscina, &m);
    CREDO_VERUM (!_continet(e, m, "PROBA_POSTULATA"));
}


    /* ==================================================
     * REALIS: systema_posix.h, sectio postulata_atomica.h
     * ================================================== */

interior vacuum
_sectio_realis (
    CredoContextus* c)
{
         chorda lexicon =
             filum_legere_totum("silva/fontes/systema_posix.h",
                                             c->piscina);
             character* e;
                   i32  m;
    constans character* cum =
        "#include \"postulata_atomica.h\"\n#include \"latina.h\"\n";
    constans character* sine = "#include \"latina.h\"\n";

    CREDO_VERUM (lexicon.mensura > ZEPHYRUM);
    si (lexicon.mensura == ZEPHYRUM)
    {
        redde;
    }
    e = silva_lexicon_posix_derivare((constans character*)lexicon.datum,
                                     (i32)lexicon.mensura, cum,
                                     (i32)strlen(cum), c->piscina, &m);
    CREDO_VERUM (_continet(e, m, "__ATOMIC_ACQUIRE"));
    CREDO_VERUM (_continet(e, m, "__atomic_load_n"));
    e = silva_lexicon_posix_derivare((constans character*)lexicon.datum,
                                     (i32)lexicon.mensura, sine,
                                     (i32)strlen(sine), c->piscina, &m);
    CREDO_VERUM (!_continet(e, m, "__ATOMIC_ACQUIRE"));
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "regulae", _sectio_regulae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "postulata", _sectio_postulata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "realis", _sectio_realis, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("silva_lexicon", SECTIONES);
}
