/* probatio_eventus_stml.c - Eventus[] <-> STML (plagulae replay) */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"
#include "eventus_stml.h"
#include "filum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Eventus*
addere (
                Xar* index,
    eventus_genus_t  genus,
                s64  tempus)
{
    Eventus* e;
    e = (Eventus*)xar_addere(index);
    memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus   = genus;
    e->tempus  = tempus;
    redde e;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                    Xar* index;
                    Xar* index2;
                Eventus* e;
                 chorda  textus;

    piscina = piscina_generare_dynamicum("probatio_eventus_stml",
        XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans tituli generum ---\n");
    CREDO_VERUM (eventus_genus_ex_titulo("mus_depressus")
        == EVENTUS_MUS_DEPRESSUS);
    CREDO_VERUM (strcmp(eventus_genus_titulus(EVENTUS_CLAVIS_LIBERATUS),
                        "clavis_liberatus") == ZEPHYRUM);
    CREDO_VERUM (eventus_genus_ex_titulo("ignotissimum")
        == EVENTUS_NIHIL);
    /* EVENTUS_MENU (plan 8): titulus ultimus, ordine enumerationis */
    CREDO_VERUM (strcmp(eventus_genus_titulus(EVENTUS_MENU), "menu")
        == ZEPHYRUM);
    CREDO_VERUM (eventus_genus_ex_titulo("menu") == EVENTUS_MENU);

    imprimere("\n--- Probans circuitum ---\n");
    index = xar_creare(piscina, (i32)magnitudo(Eventus));
    e = addere(index, EVENTUS_MUS_DEPRESSUS, M);
    e->datum.mus.x = X;
    e->datum.mus.y = XX;
    e->datum.mus.modificantes = II;
    e = addere(index, EVENTUS_CLAVIS_DEPRESSUS, M + L);
    e->datum.clavis.typus = 'p';
    e = addere(index, EVENTUS_MUS_ROTULA, M + C);
    e->datum.rotula.delta_y = -1.5f;
    e = addere(index, EVENTUS_MUTARE_MAGNITUDINEM, M + CC);
    e->datum.mutare_magnitudinem.latitudo = CDLXXX;
    e->datum.mutare_magnitudinem.altitudo = CCC;
    e = addere(index, EVENTUS_NIHIL, XL * M * M);   /* tempus > s32 */

    textus = eventus_scribere_stml(index, piscina, intern, VERUM);
    CREDO_CHORDA_NON_VACUA (textus);
    index2 = eventus_legere_stml(chorda_ut_cstr(textus, piscina),
        piscina, intern);
    CREDO_NON_NIHIL (index2);
    CREDO_AEQUALIS_I32 (xar_numerus(index2), V);
    e = (Eventus*)xar_obtinere(index2, ZEPHYRUM);
    CREDO_VERUM (e->genus == EVENTUS_MUS_DEPRESSUS);
    CREDO_VERUM (e->tempus == M);
    CREDO_AEQUALIS_S32 (e->datum.mus.x, X);
    CREDO_AEQUALIS_I32 (e->datum.mus.modificantes, II);
    e = (Eventus*)xar_obtinere(index2, I);
    CREDO_VERUM (e->genus == EVENTUS_CLAVIS_DEPRESSUS);
    CREDO_VERUM (e->datum.clavis.typus == 'p');
    e = (Eventus*)xar_obtinere(index2, II);
    CREDO_VERUM (e->datum.rotula.delta_y < -1.4f);
    CREDO_VERUM (e->datum.rotula.delta_y > -1.6f);
    e = (Eventus*)xar_obtinere(index2, III);
    CREDO_AEQUALIS_I32 (e->datum.mutare_magnitudinem.latitudo, CDLXXX);
    e = (Eventus*)xar_obtinere(index2, IV);
    CREDO_VERUM (e->tempus == XL * M * M);

    imprimere("\n--- A2: tituli generum novorum ---\n");
    CREDO_VERUM (strcmp(eventus_genus_titulus(EVENTUS_TEXTUS), "textus")
        == ZEPHYRUM);
    CREDO_VERUM (eventus_genus_ex_titulo("depositio")
        == EVENTUS_DEPOSITIO);
    CREDO_VERUM (eventus_genus_ex_titulo("suspensio")
        == EVENTUS_SUSPENSIO);
    CREDO_VERUM (eventus_genus_ex_titulo("resumptio")
        == EVENTUS_RESUMPTIO);
    CREDO_VERUM (strcmp(eventus_genus_titulus(EVENTUS_FACULTATES),
        "facultates") == ZEPHYRUM);

    imprimere("\n--- A2: codices physici (W3C) ---\n");
    {
        i32 k;
        b32 omnes = VERUM;

        per (k = ZEPHYRUM; k < (i32)EVENTUS_CODICES_NUMERUS; k++)
        {
            si (eventus_codex_ex_titulo(eventus_codex_titulus(
                    (EventusCodex)k)) != (EventusCodex)k)
            {
                imprimere("  codex %u non redit (%s)\n",
                    (insignatus integer)k,
                    eventus_codex_titulus((EventusCodex)k));
                omnes = FALSUM;
            }
        }
        CREDO_VERUM (omnes);
        CREDO_AEQUALIS_I32 ((i32)EVENTUS_CODICES_NUMERUS, LXXXIV);
        CREDO_VERUM (strcmp(eventus_codex_titulus(EVENTUS_CODEX_IGNOTUS),
            "Unidentified") == ZEPHYRUM);
        CREDO_VERUM (strcmp(eventus_codex_titulus(EVENTUS_CODEX_LITTERAE),
            "KeyA") == ZEPHYRUM);
        CREDO_VERUM (strcmp(eventus_codex_titulus((EventusCodex)
            (EVENTUS_CODEX_LITTERAE + XXV)), "KeyZ") == ZEPHYRUM);
        CREDO_VERUM (strcmp(eventus_codex_titulus(EVENTUS_CODEX_NUMERI),
            "Digit0") == ZEPHYRUM);
        CREDO_VERUM (strcmp(eventus_codex_titulus((EventusCodex)
            (EVENTUS_CODEX_FUNCTIONES + XI)), "F12") == ZEPHYRUM);
        CREDO_VERUM (strcmp(eventus_codex_titulus(
            EVENTUS_CODEX_SAGITTA_SINISTRA), "ArrowLeft") == ZEPHYRUM);
        CREDO_VERUM (strcmp(eventus_codex_titulus(
            EVENTUS_CODEX_SERA_MAIUSCULARUM), "CapsLock") == ZEPHYRUM);
        CREDO_VERUM (eventus_codex_ex_titulo("Semicolon")
            == EVENTUS_CODEX_PUNCTUM_VIRGULA);
        CREDO_VERUM (eventus_codex_ex_titulo("ControlRight")
            == EVENTUS_CODEX_IMPERIUM_DEXTRUM);
        CREDO_VERUM (eventus_codex_ex_titulo("NonExistent")
            == EVENTUS_CODEX_IGNOTUS);
    }

    imprimere("\n--- A2: circuitus campos novos omnes ---\n");
    {
        EventusExemplum  exempla[III];
          constans char* textus_difficilis =
              "  \"citatum\" 50% e\xCC\x81t\xC3\xA9\n\xE4\xB8\xAD ";
                    i32 k;

        exempla[0].x = I;   exempla[0].y = II;  exempla[0].tempus = M
                                                    + I;
        exempla[1].x = III; exempla[1].y = IV;  exempla[1].tempus = M
                                                    + II;
        exempla[2].x = V;   exempla[2].y = VI;  exempla[2].tempus = M
                                                    + III;
        index = xar_creare(piscina, (i32)magnitudo(Eventus));
        e = addere(index, EVENTUS_CLAVIS_DEPRESSUS, M);
        e->datum.clavis.clavis = CLAVIS_IGNOTA;
        e->datum.clavis.runa = 0xE9;            /* e acutum */
        e->datum.clavis.codex         = (EventusCodex)
            (EVENTUS_CODEX_LITTERAE + XVI);              /* KeyQ */
        e->datum.clavis.actio         = EVENTUS_ACTIO_ITERATA;
        e->datum.clavis.modificantes  = (i32)MOD_SHIFT
            | MOD_SHIFT_DEXTER;
        e = addere(index, EVENTUS_TEXTUS, M + I);
        e->datum.textus.contentum  =
            chorda_ex_literis(textus_difficilis,
            piscina);
        e->datum.textus.genus = EVENTUS_TEXTUS_COMPONENS;
        e->datum.textus.cursor = III;
        e->datum.textus.origo = EVENTUS_ORIGO_GLUTINATA;
        e->datum.textus.truncatum = VERUM;
        e = addere(index, EVENTUS_MUS_MOTUS, M + II);
        e->datum.mus.x = XL;
        e->datum.mus.y = L;
        e->datum.mus.indicator = II;
        e->datum.mus.indicator_genus = EVENTUS_INDICATOR_STILUS;
        e->datum.mus.pressio = DXII;
        e->datum.mus.exempla = exempla;
        e->datum.mus.numerus_exemplorum = III;
        e = addere(index, EVENTUS_MUS_ROTULA, M + III);
        e->datum.rotula.delta_y = -1.5f;
        e->datum.rotula.dx = -XII;
        e->datum.rotula.dy = XXX;
        e->datum.rotula.genus = EVENTUS_ROTULA_PRAECISA;
        e = addere(index, EVENTUS_DEPOSITIO, M + IV);
        e->datum.depositio.x = VII;
        e->datum.depositio.y = VIII;
        e->datum.depositio.viae     = chorda_ex_literis(
            "/a b/\"c\"\n/d%", piscina);
        e->datum.depositio.numerus = II;
        e->datum.depositio.promota = VERUM;
        e = addere(index, EVENTUS_FACULTATES, M + V);
        e->datum.facultates.liberationes = VERUM;
        e->datum.facultates.codex_physicus = VERUM;
        e->datum.facultates.tabula_distincta = FALSUM;
        e->datum.facultates.latera = VERUM;
        e->datum.facultates.super = VERUM;
        e->datum.facultates.praeeditio = FALSUM;
        e->datum.facultates.scriptura_copiae =
            EVENTUS_FACULTAS_FORTASSE;
        e->datum.facultates.depositio =
            EVENTUS_DEPOSITIO_HEURISTICA;
        e->datum.facultates.gradus_rotulae = XVI;
        e->datum.facultates.pressio = VERUM;
        e = addere(index, EVENTUS_SUSPENSIO, M + VI);
        e = addere(index, EVENTUS_RESUMPTIO, M + VII);

        textus = eventus_scribere_stml(index, piscina, intern, VERUM);
        index2 = eventus_legere_stml(chorda_ut_cstr(textus, piscina),
            piscina, intern);
        CREDO_NON_NIHIL (index2);
        CREDO_AEQUALIS_I32 (xar_numerus(index2), VIII);
        e = (Eventus*)xar_obtinere(index2, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (e->datum.clavis.runa, 0xE9);
        CREDO_VERUM (e->datum.clavis.codex
            == (EventusCodex)(EVENTUS_CODEX_LITTERAE + XVI));
        CREDO_VERUM (e->datum.clavis.actio == EVENTUS_ACTIO_ITERATA);
        CREDO_AEQUALIS_I32 (e->datum.clavis.modificantes,
            (i32)MOD_SHIFT | MOD_SHIFT_DEXTER);
        e = (Eventus*)xar_obtinere(index2, I);
        CREDO_VERUM (e->genus == EVENTUS_TEXTUS);
        CREDO_VERUM (chorda_aequalis_literis(e->datum.textus.contentum,
            textus_difficilis));
        CREDO_VERUM (e->datum.textus.genus == EVENTUS_TEXTUS_COMPONENS);
        CREDO_AEQUALIS_S32 (e->datum.textus.cursor, III);
        CREDO_VERUM (e->datum.textus.origo == EVENTUS_ORIGO_GLUTINATA);
        CREDO_VERUM (e->datum.textus.truncatum);
        e = (Eventus*)xar_obtinere(index2, II);
        CREDO_AEQUALIS_S32 (e->datum.mus.indicator, II);
        CREDO_VERUM (e->datum.mus.indicator_genus
            == EVENTUS_INDICATOR_STILUS);
        CREDO_AEQUALIS_S32 (e->datum.mus.pressio, DXII);
        CREDO_AEQUALIS_I32 (e->datum.mus.numerus_exemplorum, III);
        /* exempla COPIATA (plagula possidet): non ipsum vectorem */
        CREDO_VERUM (e->datum.mus.exempla != NIHIL
            && e->datum.mus.exempla != exempla);
        per (k = ZEPHYRUM; e->datum.mus.exempla != NIHIL
            && k < III; k++)
        {
            CREDO_AEQUALIS_S32 (e->datum.mus.exempla[k].x,
                exempla[k].x);
            CREDO_AEQUALIS_S32 (e->datum.mus.exempla[k].y,
                exempla[k].y);
            CREDO_VERUM (e->datum.mus.exempla[k].tempus
                == exempla[k].tempus);
        }
        e = (Eventus*)xar_obtinere(index2, III);
        CREDO_AEQUALIS_S32 (e->datum.rotula.dx, -XII);
        CREDO_AEQUALIS_S32 (e->datum.rotula.dy, XXX);
        CREDO_VERUM (e->datum.rotula.genus == EVENTUS_ROTULA_PRAECISA);
        CREDO_VERUM (e->datum.rotula.delta_y < -1.4f);
        e = (Eventus*)xar_obtinere(index2, IV);
        CREDO_AEQUALIS_S32 (e->datum.depositio.x, VII);
        CREDO_VERUM (chorda_aequalis_literis(e->datum.depositio.viae,
            "/a b/\"c\"\n/d%"));
        CREDO_AEQUALIS_I32 (e->datum.depositio.numerus, II);
        CREDO_VERUM (e->datum.depositio.promota);
        e = (Eventus*)xar_obtinere(index2, V);
        CREDO_VERUM (e->datum.facultates.liberationes
            && e->datum.facultates.codex_physicus
            && !e->datum.facultates.tabula_distincta
            && e->datum.facultates.latera && e->datum.facultates.super
            && !e->datum.facultates.praeeditio
            && e->datum.facultates.pressio);
        CREDO_AEQUALIS_I32 (e->datum.facultates.scriptura_copiae,
            EVENTUS_FACULTAS_FORTASSE);
        CREDO_AEQUALIS_I32 (e->datum.facultates.depositio,
            EVENTUS_DEPOSITIO_HEURISTICA);
        CREDO_AEQUALIS_S32 (e->datum.facultates.gradus_rotulae, XVI);
        CREDO_VERUM (((Eventus*)xar_obtinere(index2, VI))->genus
            == EVENTUS_SUSPENSIO);
        CREDO_VERUM (((Eventus*)xar_obtinere(index2, VII))->genus
            == EVENTUS_RESUMPTIO);
    }

    imprimere("\n--- A2: plagulae veteres - toy.eventus.stml ---\n");
    {
        chorda fons;
        chorda rescriptum;

        fons = filum_legere_totum("probationes/pictor/toy.eventus.stml",
            piscina);
        CREDO_CHORDA_NON_VACUA (fons);
        index = eventus_legere_stml(chorda_ut_cstr(fons, piscina),
            piscina, intern);
        CREDO_NON_NIHIL (index);
        CREDO_AEQUALIS_I32 (xar_numerus(index), X);
        /* mus veteris plagulae: pressio IGNOTA, non 0 */
        e = (Eventus*)xar_obtinere(index, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (e->datum.mus.pressio,
            EVENTUS_PRESSIO_IGNOTA);
        e = (Eventus*)xar_obtinere(index, VII);
        CREDO_VERUM (e->datum.clavis.codex == EVENTUS_CODEX_IGNOTUS);
        CREDO_VERUM (e->datum.clavis.typus == (character)XXVII);
        /* rescriptura OCTETIM eadem: attributa nova solum si non
         * ordinaria. toy.eventus.stml forma domus est (stml formare -
         * Franus 2026-10-01: forma compacta manu scripta erat; formatum
         * octetim = effusio scriptoris veteris, ergo plagula IPSA
         * oraculum est). */
        rescriptum = eventus_scribere_stml(index, piscina, intern,
            VERUM);
        si (!chorda_aequalis(rescriptum, fons))
        {
            imprimere("  rescriptum:\n%.*s\n", (int)rescriptum.mensura,
                (constans character*)rescriptum.datum);
        }
        CREDO_VERUM (chorda_aequalis(rescriptum, fons));
    }

    imprimere("\n--- A3c: positio extra fenestram (s32) ---\n");
    {
        chorda rescriptum;

        /* indicator sinistrorsum/sursum extra fenestram: x, y
         * NEGATIVA - consumens interrogare potest 'x < 0' (i32 hoc
         * numquam verum reddebat) */
        index = eventus_legere_stml(
            "<eventus_index>"
            "<eventus genus=\"mus_motus\" tempus=\"1\" x=\"-12\" y=\"-3\"/>"
            "<eventus genus=\"depositio\" tempus=\"2\" x=\"-7\" y=\"-8\" "
            "viae=\"/a\" numerus=\"1\"/>"
            "</eventus_index>",
            piscina, intern);
        CREDO_NON_NIHIL (index);
        CREDO_AEQUALIS_I32 (xar_numerus(index), II);
        e = (Eventus*)xar_obtinere(index, ZEPHYRUM);
        CREDO_VERUM (e->datum.mus.x < ZEPHYRUM);
        CREDO_VERUM (e->datum.mus.y < ZEPHYRUM);
        CREDO_AEQUALIS_S32 (e->datum.mus.x, -12);
        CREDO_AEQUALIS_S32 (e->datum.mus.y, -3);
        e = (Eventus*)xar_obtinere(index, I);
        CREDO_VERUM (e->datum.depositio.x < ZEPHYRUM);
        CREDO_AEQUALIS_S32 (e->datum.depositio.y, -8);
        rescriptum = eventus_scribere_stml(index, piscina, intern,
            FALSUM);
        CREDO_VERUM (chorda_continet(rescriptum,
            chorda_ex_literis("x=\"-12\"", piscina)));
        CREDO_VERUM (chorda_continet(rescriptum,
            chorda_ex_literis("y=\"-8\"", piscina)));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
