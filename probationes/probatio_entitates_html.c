/* probatio_entitates_html.c - Tabula entitatum HTML5 generata et
 * decoctor referentiarum
 *
 * I. TABULA: numerus MMCCXXXI pinnatus, CVI formae veteres, ordo
 *    octetorum strictus (quaestio binaria eo nititur), mensura ==
 *    strlen(octeti) in I..VI, tituli <= XXXIII.
 * II. QUAESTIO: exacta cum ';' et vetus sine; absentes NIHIL.
 * III. DECOCTOR HTML5: numericae (';' optionale, C1 -> Windows-1252,
 *    ZEPHYRUM/nimiae/surrogatae -> U+FFFD, '&#' nudum nulla),
 *    nominatae longissimae ('&notit;' -> U+00AC + 'it;'), formae
 *    veteres in textu decoctae sed in attributo ante '=' aut
 *    litteram NON (spec).
 * Recens? ./tools/entitates_html_generare.sh -probare
 */

#include "latina.h"
#include "credo.h"
#include "entitates_html.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

/* decoquere ab initio literarum; exspectatum NIHIL = FALSUM speratum */
hic_manens vacuum
_decoctio_probare (
    constans character* fons,
                   b32  in_attributo,
    constans character* exspectatum,
                   s32  post_exspectatus)
{
    character exitus[XVI];
          i32 longitudo  = ZEPHYRUM;
          s32 post       = ZEPHYRUM;
          b32 r;

    r = entitates_html_decoquere(fons, ZEPHYRUM, (s32)strlen(fons),
        in_attributo, exitus, &longitudo, &post);
    si (exspectatum == NIHIL)
    {
        si (r)
        {
            imprimere("  '%s': decoctum contra spem\n", fons);
        }
        CREDO_FALSUM (r);
        redde;
    }
    si (!r)
    {
        imprimere("  '%s': non decoctum\n", fons);
    }
    CREDO_VERUM (r);
    si (!r)
    {
        redde;
    }
    CREDO_AEQUALIS_I32 (longitudo, (i32)strlen(exspectatum));
    CREDO_VERUM (memcmp(exitus, exspectatum, (size_t)longitudo)
        == ZEPHYRUM);
    CREDO_AEQUALIS_S32 (post, post_exspectatus);
}

integer
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_entitates_html",
        65536);
    credo_aperire(piscina);

    imprimere("\n--- I. tabula generata ---\n");
    {
        i32 i;
        i32 inordinatae      = ZEPHYRUM;
        i32 mensurae_malae   = ZEPHYRUM;
        i32 veteres          = ZEPHYRUM;
        i32 titulus_maximus  = ZEPHYRUM;

        CREDO_AEQUALIS_I32 (ENTITATES_HTML_NUMERUS, (i32)2231);
        per (i = ZEPHYRUM; i < ENTITATES_HTML_NUMERUS; i++)
        {
            constans EntitasHtml* e = &ENTITATES_HTML[i];
                             i32  t = (i32)strlen(e->titulus);

            si (   i > ZEPHYRUM
                && strcmp(ENTITATES_HTML[i - I].titulus, e->titulus)
                    >= ZEPHYRUM)
            {
                inordinatae = inordinatae + I;
            }
            si (   e->mensura < I || e->mensura > VI
                || (i32)strlen(e->octeti) != e->mensura)
            {
                mensurae_malae = mensurae_malae + I;
            }
            si (t > ZEPHYRUM && e->titulus[t - I] != ';')
            {
                veteres = veteres + I;
            }
            si (t > titulus_maximus)
            {
                titulus_maximus = t;
            }
        }
        CREDO_AEQUALIS_I32 (inordinatae, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (mensurae_malae, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (veteres, (i32)106);
        CREDO_AEQUALIS_I32 (titulus_maximus, XXXII);
    }

    imprimere("\n--- II. quaestio ---\n");
    {
        constans EntitasHtml* e;

        e = entitates_html_quaerere("amp;", IV);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            CREDO_VERUM (strcmp(e->octeti, "&") == ZEPHYRUM);
        }
        e = entitates_html_quaerere("amp", III);
        CREDO_NON_NIHIL (e);
        e = entitates_html_quaerere("AElig;", VI);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            CREDO_VERUM (strcmp(e->octeti, "\xC3\x86") == ZEPHYRUM);
        }
        e = entitates_html_quaerere("zwnj;", V);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            CREDO_VERUM (strcmp(e->octeti, "\xE2\x80\x8C") == ZEPHYRUM);
        }
        /* puncta duo: U+226B U+20D2 */
        e = entitates_html_quaerere("nGt;", IV);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            CREDO_AEQUALIS_I32 (e->mensura, VI);
            CREDO_VERUM (strcmp(e->octeti,
                "\xE2\x89\xAB\xE2\x83\x92") == ZEPHYRUM);
        }
        CREDO_NIHIL (entitates_html_quaerere("abc;", IV));
        CREDO_NIHIL (entitates_html_quaerere("zwnj", IV));
        CREDO_NIHIL (entitates_html_quaerere("", ZEPHYRUM));
        /* clavis mensurata: 'amp;x' cum mensura IV = 'amp;' */
        CREDO_NON_NIHIL (entitates_html_quaerere("amp;x", IV));
    }

    imprimere("\n--- III. decoctor ---\n");
    _decoctio_probare("&amp;", FALSUM, "&", V);
    _decoctio_probare("&amp", FALSUM, "&", IV);
    _decoctio_probare("&ampx", FALSUM, "&", IV);
    _decoctio_probare("&notit;", FALSUM, "\xC2\xAC", IV);
    _decoctio_probare("&AElig;", FALSUM, "\xC3\x86", VII);
    _decoctio_probare("&nGt;", FALSUM, "\xE2\x89\xAB\xE2\x83\x92", V);
    /* attributum: forma vetus ante litteram aut '=' non decoquitur */
    _decoctio_probare("&ampx", VERUM, NIHIL, ZEPHYRUM);
    _decoctio_probare("&amp=", VERUM, NIHIL, ZEPHYRUM);
    _decoctio_probare("&amp;x", VERUM, "&", V);
    _decoctio_probare("&amp x", VERUM, "&", IV);
    _decoctio_probare("&notit;", VERUM, NIHIL, ZEPHYRUM);
    /* numericae */
    _decoctio_probare("&#65;", FALSUM, "A", V);
    _decoctio_probare("&#x41", FALSUM, "A", V);
    _decoctio_probare("&#X41;", FALSUM, "A", VI);
    _decoctio_probare("&#128;", FALSUM, "\xE2\x82\xAC", VI);
    _decoctio_probare("&#x81;", FALSUM, "\xC2\x81", VI);
    _decoctio_probare("&#0;", FALSUM, "\xEF\xBF\xBD", IV);
    _decoctio_probare("&#x110000;", FALSUM, "\xEF\xBF\xBD", X);
    _decoctio_probare("&#xD800;", FALSUM, "\xEF\xBF\xBD", VIII);
    _decoctio_probare("&#99999999999;", FALSUM, "\xEF\xBF\xBD", XIV);
    _decoctio_probare("&#x1F600;", FALSUM, "\xF0\x9F\x98\x80", IX);
    /* nullae referentiae */
    _decoctio_probare("&#;", FALSUM, NIHIL, ZEPHYRUM);
    _decoctio_probare("&#x;", FALSUM, NIHIL, ZEPHYRUM);
    _decoctio_probare("&;", FALSUM, NIHIL, ZEPHYRUM);
    _decoctio_probare("&foo;", FALSUM, NIHIL, ZEPHYRUM);
    _decoctio_probare("&", FALSUM, NIHIL, ZEPHYRUM);
    _decoctio_probare("x", FALSUM, NIHIL, ZEPHYRUM);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
