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

/* linea declarationis nominis publici, aut -I si non publicum */
interior s32
_nomen_publicum (
                   Xar* nomina,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; nomina != NIHIL && i < xar_numerus(nomina); i++)
    {
        constans BriarNomenPublicum* n = (constans BriarNomenPublicum*)
            xar_obtinere(nomina, i);

        si (chorda_aequalis_literis(n->titulus, titulus))
        {
            redde (s32)n->linea;
        }
    }
    redde -I;
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

    imprimere("\n--- Probans membra perfecta: nomina, derivatio, "
        "visibilia ---\n");
    si (membra != NIHIL && xar_numerus(membra) == III)
    {
        constans BriarMembrum* folium  = _membrum(membra, 0);
        constans BriarMembrum* media   = _membrum(membra, 1);
        constans BriarMembrum* ramus   = _membrum(membra, 2);
                          i32  k;
                          b32  derivatum = FALSUM;

        /* nomina publica cum linea declarationis; interior privatum.
         * silva 'declarans': typedef = DECLARATOR (linea nominis, XI),
         * functio = definitio tota (linea typi redditi, XIX) */
        CREDO_AEQUALIS_S32 (_nomen_publicum(folium->nomina,
            "FoliumRes"),
            XI);
        CREDO_AEQUALIS_S32 (_nomen_publicum(folium->nomina,
            "folium_duplicare"), XIX);
        CREDO_AEQUALIS_S32 (_nomen_publicum(folium->nomina,
            "folium_secretum"), -I);
        CREDO_VERUM (_nomen_publicum(media->nomina,
            "media_quadruplicare") > ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_nomen_publicum(media->nomina,
            "folium_duplicare"), -I);
        /* media folium_regiones.h derivavit (derivatio per membra) */
        per (k = ZEPHYRUM; media->derivata != NIHIL
            && k < xar_numerus(media->derivata); k++)
        {
            si (chorda_aequalis_literis(*(chorda*)xar_obtinere(
                    media->derivata, k), "folium_regiones.h"))
            {
                derivatum = VERUM;
            }
        }
        CREDO_VERUM (derivatum);
        /* caput parsurae PROPRIUM: sine folio, sine inclusione eius,
         * sine lineis, sine custode; caput proiecti: omnia manent */
        CREDO_FALSUM (_continet(piscina, media->caput_parsurae,
            "} FoliumRes;"));
        CREDO_VERUM (_continet(piscina, media->caput_parsurae,
            "media_quadruplicare (s32 x);"));
        CREDO_FALSUM (_continet(piscina, media->caput_parsurae,
            "#ifndef"));
        CREDO_VERUM (_continet(piscina, media->caput, "#ifndef"));
        CREDO_FALSUM (_continet(piscina, media->caput_parsurae,
            "#include \"folium_regiones.h\""));
        CREDO_FALSUM (_continet(piscina, media->caput_parsurae,
            "#line"));
        CREDO_VERUM (_continet(piscina, media->caput,
            "#include \"folium_regiones.h\""));
        CREDO_VERUM (_continet(piscina, media->caput, "#line"));
        CREDO_VERUM (_continet(piscina, media->corpus,
            "media_quadruplicare"));
        /* arbor TYPO-CORRECTA: parsura secunda media textum folium in
         * praeludio habet, ergo 'FoliumRes' typus notus - nulla
         * diagnosis typi ignoti manet (sine textu silva tolerat sed
         * typum nescit) */
        {
            i32 d;
            i32 ignoti = ZEPHYRUM;

            per (d = ZEPHYRUM; d < xar_numerus(media->nexus); d++)
            {
                constans BriarNexusRes* r = (constans BriarNexusRes*)
                    xar_obtinere(media->nexus, d);
                insignatus integer q;

                si (   !briar_nexus_regio_plana(r) || r->silva == NIHIL
                    || r->silva->semantica == NIHIL)
                {
                    perge;
                }
                per (q = ZEPHYRUM; q < silva_c89_diagnostica_numerus(
                    r->silva->semantica); q++)
                {
                    constans SemanticaDiagnosticum* dg =
                        silva_c89_diagnosticum_per_indicem(
                        r->silva->semantica, q);
                    integer mn = -I;
                    integer mx = ZEPHYRUM;

                    /* in textu PRINCIPALI solum (praeludium + regio);
                     * diagnoses capitum corporis aliena sunt */
                    si (   dg->codex
                        != (integer)EXAMEN_CODEX_TYPUS_NOMINATUS_IGNOTUS
                        || dg->nodus == NIHIL)
                    {
                        perge;
                    }
                    silva_nodus_extensionem(dg->nodus,
                        r->silva->parsura->fons_princeps, &mn, &mx);
                    si (mn >= ZEPHYRUM)
                    {
                        ignoti = ignoti + I;
                    }
                }
            }
            CREDO_AEQUALIS_I32 (ignoti, ZEPHYRUM);
        }
        CREDO_VERUM (_nomen_publicum(media->nomina, "media_res")
            > ZEPHYRUM);
        /* ramus solum folium videt (frater media non importatus) */
        CREDO_AEQUALIS_I32 (xar_numerus(ramus->visibilia), I);
        CREDO_VERUM (xar_numerus(ramus->visibilia) == I
            && strcmp((*(BriarMembrum**)xar_obtinere(ramus->visibilia,
                0))->titulus, "folium") == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(folium->visibilia), ZEPHYRUM);
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

    imprimere("\n--- Probans stampam clavis (membra) et statica ---\n");
    /* sine membris stampa IPSA: claves plagularum sine bibliotheca non
     * moventur */
    CREDO_VERUM (strcmp(briar_membra_stampa(piscina, "s", NIHIL), "s")
        == ZEPHYRUM);
    CREDO_VERUM (strcmp(briar_membra_stampa(piscina, "s",
        xar_creare(piscina, (i32)magnitudo(BriarMembrum))), "s")
        == ZEPHYRUM);
    si (membra != NIHIL && xar_numerus(membra) == III)
    {
               BriarMembrum* folium = (BriarMembrum*)xar_obtinere(
                   membra, 0);
               BriarMembrum* media = (BriarMembrum*)xar_obtinere(membra,
                   I);
                     chorda  octeti_folii   = folium->octeti;
                     chorda  octeti_mediae  = media->octeti;
         constans character* titulus_folii  = folium->titulus;
         constans character* ante = briar_membra_stampa(piscina, "s",
             membra);
        constans character* mutata;
        constans character* translata;

        CREDO_VERUM (strncmp(ante, "s membra ", (size_t)IX)
            == ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)strlen(ante), XXV);
        CREDO_VERUM (strcmp(ante, briar_membra_stampa(piscina, "s",
            membra)) == ZEPHYRUM);
        /* octetus unus additus in membro -> stampa alia */
        folium->octeti = chorda_concatenare(octeti_folii,
            chorda_ex_literis("\n", piscina), piscina);
        mutata          = briar_membra_stampa(piscina, "s", membra);
        folium->octeti  = octeti_folii;
        CREDO_VERUM (strcmp(ante, mutata) != ZEPHYRUM);
        /* titulus intrat (plagulae genitae eius nomen ferunt) */
        folium->titulus  = "folium_alterum";
        mutata           = briar_membra_stampa(piscina, "s", membra);
        folium->titulus  = titulus_folii;
        CREDO_VERUM (strcmp(ante, mutata) != ZEPHYRUM);
        /* mensura praefixa: sine ea 'X' + titulus 'media' + 'mediaZ'
         * et 'Xmedia' + 'media' + 'Z' fluxum EUNDEM darent (titulus
         * sequens separator non est cum in octetis stat - planta
         * prima, 'ab'/'c', muta fuit) */
        folium->octeti  = chorda_ex_literis("X", piscina);
        media->octeti   = chorda_ex_literis("mediaZ", piscina);
        mutata          = briar_membra_stampa(piscina, "s", membra);
        folium->octeti  = chorda_ex_literis("Xmedia", piscina);
        media->octeti   = chorda_ex_literis("Z", piscina);
        translata       = briar_membra_stampa(piscina, "s", membra);
        folium->octeti  = octeti_folii;
        media->octeti   = octeti_mediae;
        CREDO_VERUM (strcmp(mutata, translata) != ZEPHYRUM);
        CREDO_VERUM (strcmp(ante, briar_membra_stampa(piscina, "s",
            membra)) == ZEPHYRUM);
        /* statica membri (amalgama ea renominat): interior sola */
        CREDO_VERUM (_nomen_publicum(folium->statica,
            "folium_secretum") > ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_nomen_publicum(folium->statica,
            "folium_duplicare"), -I);
        CREDO_AEQUALIS_S32 (_nomen_publicum(folium->statica,
            "FoliumRes"), -I);
        CREDO_AEQUALIS_I32 (xar_numerus(media->statica), ZEPHYRUM);
    }
    {
        Xar* sa = _colligere(piscina, intern, fons,
            FIXA "statica_a.thistle", &causa);

        CREDO_NON_NIHIL (sa);
        si (sa != NIHIL && xar_numerus(sa) == II)
        {
            constans BriarMembrum* b = _membrum(sa, 0);

            /* functio et variabile interior */
            CREDO_VERUM (_nomen_publicum(b->statica, "adiutor")
                > ZEPHYRUM);
            CREDO_VERUM (_nomen_publicum(b->statica, "basis")
                > ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(b->statica), II);
        }
    }

    imprimere("\n--- Probans collectionem LEVEM (clavis ante parsuram)"
        " ---\n");
    {
        Xar* nexus_r = _nexum_texere(piscina, intern, fons,
            FIXA "radix.thistle");
                     Xar* levia   = NIHIL;
        BriarMembraCausa  cl;

        CREDO_NON_NIHIL (nexus_r);
        CREDO_VERUM (nexus_r != NIHIL && briar_membra_colligere_levia(
            piscina, FIXA "radix.thistle", nexus_r, intern, &levia,
            &cl));
        CREDO_NON_NIHIL (levia);
        si (   levia              != NIHIL && membra != NIHIL
            && xar_numerus(levia) == xar_numerus(membra))
        {
            i32 k;

            /* ordo idem (post-ordo), tituli idem, octeti iidem */
            per (k = ZEPHYRUM; k < xar_numerus(levia); k++)
            {
                CREDO_VERUM (strcmp(_membrum(levia, k)->titulus,
                    _membrum(membra, k)->titulus) == ZEPHYRUM);
                CREDO_VERUM (chorda_aequalis(_membrum(levia, k)->octeti,
                    _membrum(membra, k)->octeti));
                /* levis: silva nulla, partitio nulla */
                CREDO_NIHIL (_membrum(levia, k)->nomina);
            }
            /* clavis EADEM per constructionem */
            CREDO_VERUM (strcmp(briar_membra_stampa(piscina, "s",
                levia), briar_membra_stampa(piscina, "s", membra))
                == ZEPHYRUM);
        }
        alioquin
        {
            CREDO_VERUM (FALSUM);   /* numerus membrorum differt */
        }
        /* refutationes eaedem ante silvam: circulus */
        CREDO_FALSUM (briar_membra_colligere_levia(piscina,
            FIXA "circulus_a.thistle", _nexum_texere(piscina, intern,
            fons, FIXA "circulus_a.thistle"), intern, &levia, &cl));
        CREDO_VERUM (_continet(piscina, cl.causa,
            "partem communem in plagulam tertiam move"));
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
