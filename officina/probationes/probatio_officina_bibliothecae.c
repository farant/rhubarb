/* probatio_officina_bibliothecae.c - bibliothecae: lineae #include
 * -> plagulae .c interpreti onerandae (officina-bibliothecae T2).
 *
 * Probat contra aedilem VERUM (radix repositorii, aedilis.stml):
 * clausura filum (V plagulae), ex_via (via ipsa prima), annotatio
 * corpus (calendarium -> sanctorale), varians posix (fenestra_tempus),
 * recusationes (caput solum macOS, magnitudo amalgamatis, caput
 * ignotum), systema nihil onerat, scopi temporarii nulli relicti;
 * sedes functionis contra indicem fixum (fixa/bibliothecae). */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "bibliothecae.h"
#include "iter_directoria.h"
#include "credo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

interior constans character*
_radix (vacuum)
{
    constans character* radix = getenv("RHUBARB_RADIX");

    redde radix != NIHIL ? radix : ".";
}

/* resolutor per sectionem (credo: status inter sectiones nullus) */
interior Bibliothecae*
_resolutor (
    Piscina* piscina)
{
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    redde bibliothecae_creare(piscina, _radix(), &causa);
}

interior Xar*
_ex_textu (
           Bibliothecae* b,
                Piscina* piscina,
     constans character* textus,
                 chorda* causa)
{
    causa->datum    = NIHIL;
    causa->mensura  = ZEPHYRUM;
    redde bibliothecae_ex_textu(b, chorda_ex_literis(textus, piscina),
        piscina, causa);
}

interior b32
_continet (
                   Xar* viae,
    constans character* via)
{
    i32 k;

    si (viae == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(viae); k++)
    {
        si (chorda_aequalis_literis(*(chorda*)xar_obtinere(viae, k),
                via))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_filum (
    CredoContextus* c)
{
          Bibliothecae* b;
                chorda  causa;
                   Xar* viae;

    b = _resolutor(c->piscina);
    CREDO_NECESSE(b != NIHIL);
    viae = _ex_textu(b, c->piscina, "#include \"filum.h\"\n", &causa);
    CREDO_NON_NIHIL(viae);
    CREDO_AEQUALIS_I32(xar_numerus(viae), V);
    CREDO_VERUM(_continet(viae, "lib/filum.c"));
    CREDO_VERUM(_continet(viae, "lib/lectiones.c"));
    CREDO_VERUM(_continet(viae, "lib/chorda.c"));
    CREDO_VERUM(_continet(viae, "lib/piscina.c"));
    CREDO_VERUM(_continet(viae, "lib/chorda_aedificator.c"));
}

interior vacuum
_sectio_via (
    CredoContextus* c)
{
          Bibliothecae* b;
                chorda  causa;
                   Xar* viae;

    b = _resolutor(c->piscina);
    CREDO_NECESSE(b != NIHIL);
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    viae = bibliothecae_ex_via(b, "lib/filum.c",
        c->piscina, &causa);
    CREDO_NON_NIHIL(viae);
    CREDO_AEQUALIS_I32(xar_numerus(viae), V);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(viae,
        ZEPHYRUM), "lib/filum.c");
    CREDO_VERUM(_continet(viae, "lib/lectiones.c"));
}

interior vacuum
_sectio_corpus (
    CredoContextus* c)
{
          Bibliothecae* b;
                chorda  causa;
                   Xar* viae;

    b = _resolutor(c->piscina);
    CREDO_NECESSE(b != NIHIL);
    viae = _ex_textu(b, c->piscina,
        "#include \"calendarium_liturgicum.h\"\n", &causa);
    CREDO_NON_NIHIL(viae);
    CREDO_VERUM(_continet(viae, "lib/sanctorale.c"));
}

interior vacuum
_sectio_varians (
    CredoContextus* c)
{
          Bibliothecae* b;
                chorda  causa;
                   Xar* viae;

    b = _resolutor(c->piscina);
    CREDO_NECESSE(b != NIHIL);
    /* _macos.c in detectione NON recusatio - sola .m */
    viae = _ex_textu(b, c->piscina, "#include \"fenestra_tempus.h\"\n",
        &causa);
    CREDO_NON_NIHIL(viae);
    CREDO_VERUM(_continet(viae, "lib/fenestra_tempus_posix.c"));
    CREDO_FALSUM(_continet(viae, "lib/fenestra_tempus_macos.c"));
}

interior vacuum
_sectio_macos (
    CredoContextus* c)
{
          Bibliothecae* b;
                chorda  causa;
                   Xar* viae;

    b = _resolutor(c->piscina);
    CREDO_NECESSE(b != NIHIL);
    viae = _ex_textu(b, c->piscina, "#include \"fenestra.h\"\n",
        &causa);
    CREDO_NIHIL(viae);
    CREDO_CHORDA_CONTINET(causa,
        chorda_ex_literis("lib/fenestra_macos.m", c->piscina));
    CREDO_CHORDA_CONTINET(causa,
        chorda_ex_literis("include/fenestra.h", c->piscina));
}

interior vacuum
_sectio_magnitudo (
    CredoContextus* c)
{
          Bibliothecae* b;
                chorda  causa;
                   Xar* viae;

    b = _resolutor(c->piscina);
    CREDO_NECESSE(b != NIHIL);
    viae = _ex_textu(b, c->piscina, "#include \"silva.h\"\n", &causa);
    CREDO_NIHIL(viae);
    CREDO_CHORDA_CONTINET(causa,
        chorda_ex_literis("silva/amalgama/silva.c", c->piscina));
    CREDO_CHORDA_CONTINET(causa,
        chorda_ex_literis("4194304", c->piscina));
}

interior vacuum
_sectio_ignotum (
    CredoContextus* c)
{
          Bibliothecae* b;
                chorda  causa;
                   Xar* viae;

    b = _resolutor(c->piscina);
    CREDO_NECESSE(b != NIHIL);
    viae = _ex_textu(b, c->piscina, "#include \"chorda_x.h\"\n",
        &causa);
    CREDO_NIHIL(viae);
    CREDO_CHORDA_CONTINET(causa,
        chorda_ex_literis("chorda_x.h", c->piscina));
}

interior vacuum
_sectio_systema (
    CredoContextus* c)
{
          Bibliothecae* b;
                chorda  causa;
                   Xar* viae;

    b = _resolutor(c->piscina);
    CREDO_NECESSE(b != NIHIL);
    viae = _ex_textu(b, c->piscina, "#include <stdio.h>\n", &causa);
    CREDO_NON_NIHIL(viae);
    CREDO_AEQUALIS_I32(xar_numerus(viae), ZEPHYRUM);
}

/* post sectiones priores (recusationes incluse): nullus scopus
 * HUIUS processus relictus (scopus_<pid>_*: cursus priores aut
 * plantae relicta sua non numerantur) */
interior vacuum
_sectio_vestigia (
    CredoContextus* c)
{
               character  via[1024];
               character  praefixum[LXIV];
     DirectoriumIterator* iter;
    DirectoriumIntroitus* e;
                     i32  numerus = ZEPHYRUM;

    sprintf(via, "%s/build/bibliothecae_scopi", _radix());
    sprintf(praefixum, "scopus_%ld_", (long)getpid());
    iter = directorium_iterator_aperire(via, c->piscina);
    si (iter != NIHIL)
    {
        dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
        {
            si (chorda_incipit(e->titulus,
                    chorda_ex_literis(praefixum, c->piscina)))
            {
                numerus++;
            }
        }
        directorium_iterator_claudere(iter);
    }
    CREDO_AEQUALIS_I32(numerus, ZEPHYRUM);
}

interior BibliothecaeSedes
_sedes (
               Piscina* piscina,
    constans character* index,
    constans character* functio,
                chorda* via,
                chorda* causa)
{
    character plena[1024];

    sprintf(plena, "%s/%s", _radix(), index);
    via->datum      = NIHIL;
    via->mensura    = ZEPHYRUM;
    causa->datum    = NIHIL;
    causa->mensura  = ZEPHYRUM;
    redde bibliothecae_sedem_quaerere(plena, functio, piscina, via,
        causa);
}

interior vacuum
_sectio_sedes (
    CredoContextus* c)
{
                 Piscina* p = c->piscina;
                  chorda  via;
                  chorda  causa;
      constans character* fixum =
          "officina/probationes/fixa/bibliothecae/nexus.tsv";

    /* caput + corpus = familia una, caput praefertur */
    CREDO_AEQUALIS_I32((i32)_sedes(p, fixum, "alpha_functio", &via,
        &causa), (i32)BIBLIOTHECAE_SEDES_INVENTA);
    CREDO_CHORDA_AEQUALIS_LITERIS(via, "include/alpha.h");
    /* corpus solum */
    CREDO_AEQUALIS_I32((i32)_sedes(p, fixum, "beta_functio", &via,
        &causa), (i32)BIBLIOTHECAE_SEDES_INVENTA);
    CREDO_CHORDA_AEQUALIS_LITERIS(via, "lib/beta.c");
    /* capita duo diversa */
    CREDO_AEQUALIS_I32((i32)_sedes(p, fixum, "gamma_functio", &via,
        &causa), (i32)BIBLIOTHECAE_SEDES_AMBIGUA);
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("include/gamma.h",
        p));
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("include/delta.h",
        p));
    /* sedes extra include/ et lib/ neglectae */
    CREDO_AEQUALIS_I32((i32)_sedes(p, fixum, "epsilon_functio", &via,
        &causa), (i32)BIBLIOTHECAE_SEDES_INVENTA);
    CREDO_CHORDA_AEQUALIS_LITERIS(via, "include/epsilon.h");
    CREDO_AEQUALIS_I32((i32)_sedes(p, fixum, "kappa_functio", &via,
        &causa), (i32)BIBLIOTHECAE_SEDES_IGNOTA);
    CREDO_AEQUALIS_I32((i32)_sedes(p, fixum, "nulla_functio", &via,
        &causa), (i32)BIBLIOTHECAE_SEDES_IGNOTA);
    /* index absens */
    CREDO_AEQUALIS_I32((i32)_sedes(p,
        "officina/probationes/fixa/bibliothecae/absens.tsv",
        "alpha_functio", &via, &causa),
        (i32)BIBLIOTHECAE_SEDES_IGNOTA);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "filum", _sectio_filum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "via", _sectio_via, NIHIL, NIHIL, NIHIL, NIHIL },
    { "corpus", _sectio_corpus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "varians posix", _sectio_varians, NIHIL, NIHIL, NIHIL, NIHIL },
    { "macOS recusatur", _sectio_macos, NIHIL, NIHIL, NIHIL, NIHIL },
    { "magnitudo recusatur", _sectio_magnitudo, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "caput ignotum", _sectio_ignotum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "systema", _sectio_systema, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nulla vestigia", _sectio_vestigia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sedes", _sectio_sedes, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_bibliothecae", SECTIONES);
}
