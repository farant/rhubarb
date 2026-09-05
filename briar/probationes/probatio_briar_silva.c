/* probatio_briar_silva.c - Genus interius quartum: regiones C per
 * silvam cum expansione (praeludium latina.h; capita e fonte silicis,
 * numquam e disco). Probat: 'principale' -> symbolum 'main' (parsura
 * nuda id non daret), regio probationis parsata, methodus cum
 * exemplari tractatoris compatibilis, linea erroris in lineam
 * .thistle translata.
 */

#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_contextus.h"
#include "briar_nexus.h"
#include "briar_silva.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* SALVE =
    "briar/probationes/fixa/thistle/salve.thistle";
hic_manens constans character* VITREUM =
    "briar/probationes/fixa/thistle/salve_vitreum.thistle";
hic_manens constans character* FRACTUM =
    "briar/probationes/fixa/thistle/adversa/c_fractum.thistle";
hic_manens constans character* DERIVATUM =
    "briar/probationes/fixa/thistle/derivatum.thistle";
hic_manens constans character* PUNCTUM =
    "briar/probationes/fixa/thistle/punctum.thistle";
hic_manens constans character* FRAGMENTA =
    "briar/probationes/fixa/thistle/fragmenta.thistle";
hic_manens constans character* FRAGMENTA_DERIVATA =
    "briar/probationes/fixa/thistle/fragmenta_derivata.thistle";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

/* linea (I-basata) in qua 'acus' primum incipit in columna 0 */
interior i32
_linea_ubi (
    constans character* fons,
                   i32  mensura,
    constans character* acus)
{
    i32 m      = (i32)strlen(acus);
    i32 linea  = I;
    i32 i;

    per (i = ZEPHYRUM; i + m <= mensura; i++)
    {
        si (   (i == ZEPHYRUM || fons[i - I] == '\n')
            && memcmp(fons + i, acus, (size_t)m) == ZEPHYRUM)
        {
            redde linea;
        }
        si (fons[i] == '\n')
        {
            linea = linea + I;
        }
    }
    redde ZEPHYRUM;
}

interior SilvaChorda
_silva_chorda (
               Piscina* piscina,
    constans character* literae)
{
    SilvaChorda s;
         chorda c = chorda_ex_literis(literae, piscina);

    s.mensura  = (insignatus integer)c.mensura;
    s.datum    = (insignatus character*)c.datum;
    redde s;
}

/* regio C n-ta (0-basata) inter partes nexus, aut NIHIL */
interior BriarNexusRes*
_regio_c (
    Xar* nexus,
    i32  quota)
{
    i32 i;
    i32 visa = ZEPHYRUM;

    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (   r->genus == BRIAR_NEXUS_REGIO
            && briar_nexus_titulus_est(r, "c"))
        {
            si (visa == quota)
            {
                redde r;
            }
            visa = visa + I;
        }
    }
    redde NIHIL;
}

interior Xar*
_texere_omnia (
               Piscina*  piscina,
   InternamentumChorda*  intern,
    constans SilexFons*  fons,
    constans character*  via,
             character** textus_out,
                   i32*  mensura_out)
{
        character* textus;
              i32  mensura = ZEPHYRUM;
    MateriaNodus* doc;
             Xar* nexus;

    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        redde NIHIL;
    }
        doc  = briar_arbor_parsare(piscina, textus, mensura);
    nexus    = briar_nexus_texere(piscina, doc, intern);
    si (nexus == NIHIL)
    {
        redde NIHIL;
    }
    si (briar_contexere(piscina, nexus, NIHIL) < ZEPHYRUM)
    {
        redde NIHIL;
    }
    si (briar_silvam_texere(piscina, nexus, fons) < ZEPHYRUM)
    {
        redde NIHIL;
    }
    *textus_out   = textus;
    *mensura_out  = mensura;
    redde nexus;
}

s32
principale (vacuum)
{
                  b32  praeteritus;
              Piscina* piscina;
  InternamentumChorda* intern;
   constans character* radix;
   constans SilexFons* fons;

    piscina = piscina_generare_dynamicum("probatio_briar_silva",
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

    imprimere("\n--- Probans salve: principale -> main ---\n");
    {
         character* textus;
               i32  mensura;
               Xar* nexus = _texere_omnia(piscina, intern, fons, SALVE,
                   &textus, &mensura);
        BriarNexusRes* r;
        SemanticaSymbolum* s;

        CREDO_NON_NIHIL (nexus);
        r = _regio_c(nexus, ZEPHYRUM);
        CREDO_NON_NIHIL (r);
        CREDO_NON_NIHIL (r->silva);
        CREDO_NON_NIHIL (r->silva->parsura);
        CREDO_NON_NIHIL (r->silva->semantica);
        CREDO_AEQUALIS_I32 (r->silva->parsura->numerus_errorum,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (r->linea_erroris, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (r->praeludium, IV);   /* latina + trias */
        CREDO_AEQUALIS_I32 (xar_numerus(r->silva->capita_derivata),
            ZEPHYRUM);
        /* expansio: 'principale' est macro -> symbolum 'main' */
        s = silva_c89_symbolum_invenire(r->silva->semantica,
            _silva_chorda(piscina, "main"));
        CREDO_NON_NIHIL (s);
        CREDO_AEQUALIS_S32 ((s32)s->genus, (s32)SYMBOLUM_FUNCTIO);
        CREDO_VERUM (silva_c89_symbolum_invenire(r->silva->semantica,
            _silva_chorda(piscina, "principale")) == NIHIL);
        /* linea silvae V (post praeludium IV) = linea prima contenti */
        CREDO_AEQUALIS_I32 (briar_nexus_linea_silvae(r, V),
            r->linea_initium);

        /* regio probationis etiam parsata (credo.h e fonte) */
        r = _regio_c(nexus, I);
        CREDO_NON_NIHIL (r);
        CREDO_NON_NIHIL (r->silva);
        CREDO_NON_NIHIL (r->silva->parsura);
        CREDO_AEQUALIS_I32 (r->silva->parsura->numerus_errorum,
            ZEPHYRUM);
        CREDO_NON_NIHIL (silva_c89_symbolum_invenire(
            r->silva->semantica, _silva_chorda(piscina, "main")));
        CREDO_NON_NIHIL (silva_c89_symbolum_invenire(
            r->silva->semantica,
            _silva_chorda(piscina, "credo_aperire")));
        briar_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans salve_vitreum: methodus + exemplar ---\n");
    {
            character* textus;
                  i32  mensura;
                  Xar* nexus;
        BriarNexusRes* r;
        SemanticaSymbolum* s;
        SemanticaSymbolum* exemplar;

        nexus = _texere_omnia(piscina, intern, fons, VITREUM, &textus,
            &mensura);
        CREDO_NON_NIHIL (nexus);
        r = _regio_c(nexus, ZEPHYRUM);
        CREDO_NON_NIHIL (r);
        CREDO_VERUM (briar_nexus_attributum_habet(r, "methodus"));
                /* + internuntius + exemplar */
        CREDO_AEQUALIS_I32 (r->praeludium, VI);
        CREDO_AEQUALIS_I32 (r->silva->parsura->numerus_errorum,
            ZEPHYRUM);
        s = silva_c89_symbolum_invenire(r->silva->semantica,
            _silva_chorda(piscina, "salve"));
        CREDO_NON_NIHIL (s);
        CREDO_AEQUALIS_S32 ((s32)s->genus, (s32)SYMBOLUM_FUNCTIO);
        exemplar = silva_c89_symbolum_invenire(r->silva->semantica,
            _silva_chorda(piscina, "briar_tractator_exemplar"));
        CREDO_NON_NIHIL (exemplar);
        CREDO_AEQUALIS_S32 ((s32)exemplar->typus->genus,
            (s32)TYPUS_C89_MONSTRATOR);
        CREDO_AEQUALIS_S32 (
            (s32)exemplar->typus->datum.monstrator.internum->genus,
            (s32)TYPUS_C89_FUNCTIO);
        CREDO_VERUM (silva_c89_typi_compatibiles(
            exemplar->typus->datum.monstrator.internum, s->typus));
        CREDO_AEQUALIS_I32 (briar_nexus_linea_silvae(r, VII),
            r->linea_initium);
        briar_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans c_fractum: linea erroris .thistle ---\n");
    {
            character* textus;
                  i32  mensura;
                  Xar* nexus;
        BriarNexusRes* r;
                  i32  linea;

        nexus = _texere_omnia(piscina, intern, fons, FRACTUM, &textus,
            &mensura);
        CREDO_NON_NIHIL (nexus);
        r = _regio_c(nexus, ZEPHYRUM);
        CREDO_NON_NIHIL (r);
        CREDO_NON_NIHIL (r->silva);
        CREDO_NON_NIHIL (r->silva->parsura);
        CREDO_VERUM (r->silva->parsura->numerus_errorum > ZEPHYRUM);
        linea = _linea_ubi(textus, mensura, "@@@");
        CREDO_VERUM (linea > ZEPHYRUM);
        CREDO_AEQUALIS_I32 (r->linea_erroris, linea);
        CREDO_VERUM (r->causa.mensura > ZEPHYRUM);
        briar_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans derivatum: capita ex usu ---\n");
    {
            character* textus;
                  i32  mensura;
                  Xar* nexus;
        BriarNexusRes* r;
        SemanticaSymbolum* s;

        nexus = _texere_omnia(piscina, intern, fons, DERIVATUM, &textus,
            &mensura);
        CREDO_NON_NIHIL (nexus);
        r = _regio_c(nexus, ZEPHYRUM);
        CREDO_NON_NIHIL (r);
        CREDO_NON_NIHIL (r->silva);
        CREDO_AEQUALIS_I32 (r->linea_erroris, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (r->silva->parsura->numerus_errorum,
            ZEPHYRUM);
        /* chorda.h (typus chorda + chorda_ex_literis), piscina.h
         * (typus Piscina + functiones): alphabetice */
        CREDO_AEQUALIS_I32 (xar_numerus(r->silva->capita_derivata), II);
        CREDO_VERUM (chorda_aequalis_literis(
            *(chorda*)xar_obtinere(r->silva->capita_derivata, ZEPHYRUM),
            "chorda.h"));
        CREDO_VERUM (chorda_aequalis_literis(
            *(chorda*)xar_obtinere(r->silva->capita_derivata, I),
            "piscina.h"));
        CREDO_AEQUALIS_I32 (r->praeludium, VI);   /* IV + capita II */
        /* post parsuram secundam symbolum resolutum, non implicitum */
        s = silva_c89_symbolum_invenire(r->silva->semantica,
            _silva_chorda(piscina, "chorda_ex_literis"));
        CREDO_NON_NIHIL (s);
        CREDO_FALSUM (s != NIHIL && s->est_implicitum);
        /* probatio: credo.h (credo_aperire + macro) + piscina.h */
        r = _regio_c(nexus, I);
        CREDO_NON_NIHIL (r);
        CREDO_AEQUALIS_I32 (xar_numerus(r->silva->capita_derivata), II);
        CREDO_VERUM (chorda_aequalis_literis(
            *(chorda*)xar_obtinere(r->silva->capita_derivata, ZEPHYRUM),
            "credo.h"));
        CREDO_VERUM (chorda_aequalis_literis(
            *(chorda*)xar_obtinere(r->silva->capita_derivata, I),
            "piscina.h"));
                CREDO_AEQUALIS_I32 (r->silva->parsura->numerus_errorum,
                    ZEPHYRUM);
        briar_silvam_solvere(nexus);
    }

        imprimere("\n--- Probans fragmenta: contextus parsatur, tabula"
            " linearum, fragmenta non parsantur ---\n");
    {
            character* textus;
                  i32  mensura;
                  Xar* nexus;
        BriarNexusRes* app;
        BriarNexusRes* prob;
        BriarNexusRes* frag;

        nexus = _texere_omnia(piscina, intern, fons, FRAGMENTA, &textus,
            &mensura);
        CREDO_NON_NIHIL (nexus);
        app   = _regio_c(nexus, V);
        prob  = _regio_c(nexus, VI);
        frag  = _regio_c(nexus, I);
        CREDO_NON_NIHIL (app);
        CREDO_NON_NIHIL (prob);
        CREDO_NON_NIHIL (frag);
        si (app != NIHIL && prob != NIHIL && frag != NIHIL)
        {
            /* radix app: chorda.h per fragmentum #capita CONTEXTA (et
             * chorda.h piscina.h trahit) - nihil derivatur: parsura
             * contextum videt, non octetos crudos cum '<<#capita>>' */
            CREDO_NON_NIHIL (app->silva);
            CREDO_AEQUALIS_I32 (app->linea_erroris, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (app->silva->parsura->numerus_errorum,
                ZEPHYRUM);
            CREDO_AEQUALIS_I32 (
                xar_numerus(app->silva->capita_derivata), ZEPHYRUM);
            /* lineae silvae per tabulam: index contextus 0 -> 20
             * (capita), 10 -> 9 (incrementum), 12 -> 41 (reditus) */
            CREDO_AEQUALIS_I32 (briar_nexus_linea_silvae(app,
                app->praeludium + ZEPHYRUM + I), XX);
            CREDO_AEQUALIS_I32 (briar_nexus_linea_silvae(app,
                app->praeludium + X + I), IX);
            CREDO_AEQUALIS_I32 (briar_nexus_linea_silvae(app,
                app->praeludium + XII + I), XLI);
            /* probatio: credo.h ipsa (chorda.h + piscina.h trahit) -
             * nihil derivatur; contextus parsatus sine erroribus */
            CREDO_NON_NIHIL (prob->silva);
            CREDO_AEQUALIS_I32 (prob->silva->parsura->numerus_errorum,
                ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(
                prob->silva->capita_derivata), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (briar_nexus_linea_silvae(prob,
                prob->praeludium + VII + I), XXIV);
            /* fragmentum: numquam parsatum */
            CREDO_VERUM (frag->est_fragmentum);
            CREDO_VERUM (frag->silva == NIHIL);
        }
        briar_silvam_solvere(nexus);
    }

        imprimere("\n--- Probans fragmenta_derivata: derivatio per texturam"
            " ---\n");
    {
            character* textus;
                  i32  mensura;
                  Xar* nexus;
        BriarNexusRes* r;
        SemanticaSymbolum* s;

        /* nullum #include: chorda_ex_literis in fragmento SOLO -
         * chorda.h et piscina.h derivata radici */
        nexus = _texere_omnia(piscina, intern, fons, FRAGMENTA_DERIVATA,
            &textus, &mensura);
        CREDO_NON_NIHIL (nexus);
        r = _regio_c(nexus, I);
        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            CREDO_FALSUM (r->est_fragmentum);
            CREDO_NON_NIHIL (r->silva);
            CREDO_AEQUALIS_I32 (r->silva->parsura->numerus_errorum,
                ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(r->silva->capita_derivata),
                II);
            CREDO_VERUM (chorda_aequalis_literis(
                *(chorda*)xar_obtinere(r->silva->capita_derivata,
                ZEPHYRUM), "chorda.h"));
            CREDO_VERUM (chorda_aequalis_literis(
                *(chorda*)xar_obtinere(r->silva->capita_derivata, I),
                "piscina.h"));
            s = silva_c89_symbolum_invenire(r->silva->semantica,
                _silva_chorda(piscina, "chorda_ex_literis"));
            CREDO_NON_NIHIL (s);
            CREDO_FALSUM (s != NIHIL && s->est_implicitum);
            /* linea fragmenti (8) per tabulam: index contextus IV */
            CREDO_AEQUALIS_I32 (briar_nexus_linea_silvae(r,
                r->praeludium + IV + I), VIII);
        }
        briar_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans punctum: typus regionis alterius numquam"
        " derivatur ---\n");
    {
            character* textus;
                  i32  mensura;
                  Xar* nexus;
        BriarNexusRes* r;
                  i32  k;

        /* 'Punctum' in regione app definitur ET in include/mandatum.h
         * (ludus, 2026-09-05) sedet: probatio eum per
         * <t>_regiones.h videt - mandatum.h derivatum caput genitum
         * duplicaret (typedef bis) */
        nexus = _texere_omnia(piscina, intern, fons, PUNCTUM, &textus,
            &mensura);
        CREDO_NON_NIHIL (nexus);
        r = _regio_c(nexus, I);
        CREDO_NON_NIHIL (r);
        CREDO_NON_NIHIL (r->silva);
        CREDO_AEQUALIS_I32 (r->silva->parsura->numerus_errorum,
            ZEPHYRUM);
        per (k = ZEPHYRUM; k < xar_numerus(r->silva->capita_derivata);
            k++)
        {
            chorda c = *(chorda*)xar_obtinere(r->silva->capita_derivata,
                k);

            imprimere("  derivatum: %.*s\n", (integer)c.mensura,
                (constans character*)c.datum);
            CREDO_FALSUM (chorda_aequalis_literis(c, "mandatum.h"));
        }
        briar_silvam_solvere(nexus);
    }


    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
