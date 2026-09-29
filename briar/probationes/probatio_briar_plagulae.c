/* probatio_briar_plagulae.c - membra aedificationis (spec par. 3.5,
 * planum IX T2; lapide feature-requests/015): collectio transitiva,
 * post-ordo, quodque semel, via contra plagulam importantem (non
 * directorium currens), circulus et refutationes cum linea elementi. */
#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "via.h"
#include "filum.h"
#include "internamentum.h"
#include "silex.h"
#include "briar_arbor.h"
#include "briar_nexus.h"
#include "briar_contextus.h"
#include "briar_silva.h"
#include "briar_plagulae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FIXA "briar/probationes/fixa/bibliotheca/"

/* plagulam legere et nexum eius texere (ut tools/briar.c) */
interior Xar*
_nexum_texere (
               Piscina* piscina,
   InternamentumChorda* intern,
    constans SilexFons* fons,
    constans character* via)
{
    chorda textus = filum_legere_totum(via, piscina);
    MateriaNodus* doc;
    Xar*          nexus;

    si (textus.datum == NIHIL)
    {
        redde NIHIL;
    }
    doc   = briar_arbor_parsare(piscina,
        (constans character*)textus.datum, (i32)textus.mensura);
    nexus = briar_nexus_texere(piscina, doc, intern);
    (vacuum)briar_contexere(piscina, nexus, NIHIL);
    (vacuum)briar_silvam_texere(piscina, nexus, fons, via);
    redde nexus;
}

interior b32
_desinit (
    constans character* s,
    constans character* cauda)
{
    size_t a = s != NIHIL ? strlen(s) : ZEPHYRUM;
    size_t b = strlen(cauda);

    redde s != NIHIL && a >= b
        && strcmp(s + (a - b), cauda) == ZEPHYRUM;
}

interior b32
_continet (
               Piscina* piscina,
                chorda  c,
    constans character* acus)
{
    redde c.datum != NIHIL
        && chorda_continet(c, chorda_ex_literis(acus, piscina));
}

/* colligere pro radice data; *causa redditur */
interior Xar*
_colligere (
               Piscina* piscina,
   InternamentumChorda* intern,
    constans SilexFons* fons,
    constans character* via,
      BriarMembraCausa* causa)
{
    Xar* membra  = NIHIL;
    Xar* nexus   = _nexum_texere(piscina, intern, fons, via);

    CREDO_NON_NIHIL (nexus);
    si (   nexus == NIHIL
        || !briar_membra_colligere(piscina, via, nexus, intern, fons,
               &membra, causa))
    {
        redde NIHIL;
    }
    redde membra;
}

interior constans BriarMembrum*
_membrum (
    Xar* membra,
    i32  i)
{
    redde (constans BriarMembrum*)xar_obtinere(membra, i);
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
InternamentumChorda* intern;
          SilexFons* fons;
 constans character* radix;
   BriarMembraCausa  causa;
                Xar* membra;
          character  cwd[1024];

    piscina = piscina_generare_dynamicum("probatio_briar_plagulae",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    radix   = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    fons = silex_fons_disci(piscina, radix);
    CREDO_NON_NIHIL (fons);

    imprimere("\n--- Probans membra: rhombus, post-ordo, semel ---\n");
    membra = _colligere(piscina, intern, fons, FIXA "radix.thistle",
        &causa);
    CREDO_NON_NIHIL (membra);
    si (membra != NIHIL)
    {
        CREDO_AEQUALIS_I32 (xar_numerus(membra), III);
    }
    si (membra != NIHIL && xar_numerus(membra) == III)
    {
        CREDO_VERUM (strcmp(_membrum(membra, 0)->titulus, "folium")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(_membrum(membra, 1)->titulus, "media")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(_membrum(membra, 2)->titulus, "ramus")
            == ZEPHYRUM);
        /* folium primum per media adductum (ordo documenti) */
        CREDO_VERUM (_desinit(_membrum(membra, 0)->via_importantis,
            "/bibliotheca/media.thistle"));
        CREDO_VERUM (_desinit(_membrum(membra, 0)->via,
            "/bibliotheca/folium.thistle"));
        CREDO_VERUM (_desinit(_membrum(membra, 2)->via,
            "/bibliotheca/sub/ramus.thistle"));
        /* via normalizata et absoluta: nullum '..', initium '/' */
        CREDO_VERUM (strstr(_membrum(membra, 0)->via, "/..") == NIHIL);
        CREDO_VERUM (_membrum(membra, 0)->via[0] == '/');
        CREDO_VERUM (_membrum(membra, 0)->octeti.mensura > ZEPHYRUM);
        CREDO_NON_NIHIL (_membrum(membra, 1)->nexus);
        CREDO_AEQUALIS_I32 (_membrum(membra, 1)->linea_elementi, VIII);
    }

    imprimere("\n--- Probans viam contra plagulam, non cwd ---\n");
    CREDO_NON_NIHIL (getcwd(cwd, magnitudo(cwd)));
    {
        constans character* absoluta = chorda_ut_cstr(via_absoluta(
            chorda_ex_literis(FIXA "radix.thistle", piscina), piscina),
            piscina);
                                Xar* alia;
                   BriarMembraCausa  c2;

        CREDO_VERUM (chdir("/tmp") == ZEPHYRUM);
        alia = _colligere(piscina, intern, fons, absoluta, &c2);
        CREDO_VERUM (chdir(cwd) == ZEPHYRUM);
        CREDO_NON_NIHIL (alia);
        si (   alia              != NIHIL && membra != NIHIL
            && xar_numerus(alia) == III && xar_numerus(membra) == III)
        {
            CREDO_VERUM (strcmp(_membrum(alia, 0)->via,
                _membrum(membra, 0)->via) == ZEPHYRUM);
            CREDO_VERUM (strcmp(_membrum(alia, 2)->via,
                _membrum(membra, 2)->via) == ZEPHYRUM);
        }
    }

    imprimere("\n--- Probans circulum ---\n");
    CREDO_NIHIL (_colligere(piscina, intern, fons,
        FIXA "circulus_a.thistle", &causa));
    CREDO_VERUM (_continet(piscina, causa.causa,
        "circulus_a.thistle -> circulus_b.thistle -> "
        "circulus_a.thistle"));
    CREDO_VERUM (_continet(piscina, causa.causa,
        "partem communem in plagulam tertiam move"));
    CREDO_VERUM (_desinit(causa.via,
        "/bibliotheca/circulus_b.thistle"));
    CREDO_AEQUALIS_I32 (causa.linea, VI);

    imprimere("\n--- Probans refutationes ---\n");
    CREDO_NIHIL (_colligere(piscina, intern, fons,
        FIXA "radix_absens.thistle", &causa));
    CREDO_VERUM (_continet(piscina, causa.causa, "non exsistit"));
    CREDO_VERUM (_desinit(causa.via,
        "/bibliotheca/radix_absens.thistle"));
    CREDO_AEQUALIS_I32 (causa.linea, VI);

    CREDO_NIHIL (_colligere(piscina, intern, fons,
        FIXA "radix_non.thistle", &causa));
    CREDO_VERUM (_continet(piscina, causa.causa,
        "non est plagula .thistle"));

    CREDO_NIHIL (_colligere(piscina, intern, fons,
        FIXA "radix_sine_via.thistle", &causa));
    CREDO_VERUM (_continet(piscina, causa.causa, "sine via"));
    CREDO_AEQUALIS_I32 (causa.linea, VI);

    CREDO_NIHIL (_colligere(piscina, intern, fons,
        FIXA "radix_sine_c.thistle", &causa));
    CREDO_VERUM (_continet(piscina, causa.causa,
        "regio C plana nulla"));
    CREDO_VERUM (_desinit(causa.via,
        "/bibliotheca/radix_sine_c.thistle"));

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
