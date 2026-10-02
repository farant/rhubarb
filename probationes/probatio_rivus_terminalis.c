/* probatio_rivus_terminalis.c - pipeline fontis terminalis (eventus
 * B3a): quod proiectio tesserae non ostendit - morae poscendae, motus
 * NON coalescit (decodificatio pigra), glutinum trans traditiones
 * scissum, glutinum vacuum, capacitas, reliquiae muris post moram.
 * B3b: facultates primae, lectio coalita, modi declarati; glutinum
 * viarum -> DEPOSITIO (declaratum). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "eventus.h"
#include "rivus_terminalis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_tradere (
       RivusTerminalis* r,
    constans character* s)
{
    (vacuum)rivus_tradere(r, (constans i8*)s, (i32)strlen(s));
}

interior b32
_octeti_aequales (
           constans i8* octeti,
                   i32  mensura,
    constans character* literae)
{
    redde (b32)(   mensura == (i32)strlen(literae)
                && memcmp(octeti, literae, (memoriae_index)mensura)
                   == ZEPHYRUM);
}

s32 principale (vacuum)
{
            Piscina* piscina;
    RivusTerminalis* r;
    RivusTerminalis* novus;
            Eventus  e;
                 i8  magnum[CCC];
                 i8  modi[RIVUS_MODI_MAXIMUM];
                i32  numerus;

    piscina = piscina_generare_dynamicum("probatio_rivus_terminalis",
        M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    r = rivus_creare(piscina, X, XX);
    CREDO_NON_NIHIL (r);

    imprimere("\n--- 0. facultates PRIMAE (spec Q4) ---\n");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_FACULTATES);
    /* ?1003 non declaratum: super FALSUM */
    CREDO_FALSUM (e.datum.facultates.super);
    CREDO_AEQUALIS_S32 (e.datum.facultates.gradus_rotulae, XX);

    imprimere("\n--- I. morae poscendae ---\n");
    CREDO_AEQUALIS_S32 (rivus_mora_ms(r), ZEPHYRUM);
    CREDO_FALSUM (rivus_eventum(r, M, &e));
    _tradere(r, "\x1b");
    CREDO_FALSUM (rivus_eventum(r, M, &e));
    CREDO_AEQUALIS_S32 (rivus_mora_ms(r), RIVUS_MORA_FUGAE_MS);
    rivus_moram(r, M);
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_CLAVIS_DEPRESSUS
        && e.datum.clavis.clavis == CLAVIS_EFFUGIUM);
    CREDO_AEQUALIS_S32 (rivus_mora_ms(r), ZEPHYRUM);

    imprimere("\n--- II. motus non coalescit (pigra) ---\n");
    _tradere(r, "\x1b[<35;4;2M\x1b[<35;5;2M");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_MOTUS && e.datum.mus.x == XXXV);
    CREDO_AEQUALIS_I32 (e.datum.mus.numerus_exemplorum, ZEPHYRUM);
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_MOTUS && e.datum.mus.x == XLV);
    CREDO_FALSUM (rivus_eventum(r, M, &e));

    imprimere("\n--- III. glutinum trans traditiones scissum ---\n");
    _tradere(r, "\x1b[200~hel");
    CREDO_FALSUM (rivus_eventum(r, M, &e));
    CREDO_AEQUALIS_S32 (rivus_mora_ms(r), RIVUS_MORA_GLUTINI_MS);
    _tradere(r, "lo\x1b[20");
    CREDO_FALSUM (rivus_eventum(r, M, &e));
    _tradere(r, "1~x");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_TEXTUS
        && e.datum.textus.origo == EVENTUS_ORIGO_GLUTINATA);
    CREDO_VERUM (chorda_aequalis_literis(e.datum.textus.contentum,
        "hello"));
    CREDO_FALSUM (e.datum.textus.truncatum);
    CREDO_VERUM (rivus_eventum(r, M, &e));    /* 'x' post terminum */
    CREDO_VERUM (e.genus == EVENTUS_CLAVIS_DEPRESSUS
        && e.datum.clavis.runa == 'x');
    CREDO_VERUM (rivus_eventum(r, M, &e));    /* textus 'x' */
    CREDO_FALSUM (rivus_eventum(r, M, &e));

    imprimere("\n--- IV. glutinum vacuum; silentium truncat ---\n");
    _tradere(r, "\x1b[200~\x1b[201~");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_TEXTUS
        && e.datum.textus.contentum.mensura == ZEPHYRUM);
    _tradere(r, "\x1b[200~ab\x1b[2");
    CREDO_FALSUM (rivus_eventum(r, M, &e));
    rivus_moram(r, M);
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (chorda_aequalis_literis(e.datum.textus.contentum,
        "ab\x1b[2"));
    CREDO_VERUM (e.datum.textus.truncatum);

    imprimere("\n--- V. capacitas ---\n");
    memset(magnum, 'q', CCC);
    CREDO_AEQUALIS_I32 (rivus_spatium(r), RIVUS_BUFFER);
    CREDO_AEQUALIS_I32 (rivus_tradere(r, magnum, CCC), RIVUS_BUFFER);
    CREDO_AEQUALIS_I32 (rivus_spatium(r), ZEPHYRUM);
    numerus = ZEPHYRUM;
    dum (rivus_eventum(r, M, &e))
    {
        numerus++;
    }
    CREDO_AEQUALIS_I32 (rivus_pendentes(r), ZEPHYRUM);
    /* runa quaeque = CLAVIS + TEXTUS: nullum in cauda amissum */
    CREDO_AEQUALIS_I32 (numerus, II * RIVUS_BUFFER);

    imprimere("\n--- VI. reliquiae: mus SGR trans moram (H8) ---\n");
    _tradere(r, "\x1b[<0;20");
    CREDO_FALSUM (rivus_eventum(r, M, &e));
    rivus_moram(r, M);
    CREDO_FALSUM (rivus_eventum(r, M, &e));    /* nihil: servatur */
    _tradere(r, ";5M");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_DEPRESSUS
        && e.datum.mus.x == CXCV && e.datum.mus.y == XC);

    imprimere("\n--- VII. lectio coalita (spec Q10) ---\n");
    _tradere(r, "\x1b[<35;4;2M\x1b[<35;5;2M\x1b[<35;6;2Mz");
    CREDO_VERUM (rivus_eventum_coalitum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_MOTUS && e.datum.mus.x == LV);
    CREDO_AEQUALIS_I32 (e.datum.mus.numerus_exemplorum, II);
    CREDO_VERUM (rivus_eventum_coalitum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_CLAVIS_DEPRESSUS
        && e.datum.clavis.runa == 'z');
    CREDO_VERUM (rivus_eventum_coalitum(r, M, &e));     /* textus 'z' */
    CREDO_FALSUM (rivus_eventum_coalitum(r, M, &e));
    /* fluxus longus coalitus: nullum amissum */
    memset(magnum, 'q', CCC);
    CREDO_AEQUALIS_I32 (rivus_tradere(r, magnum, CCC), RIVUS_BUFFER);
    numerus = ZEPHYRUM;
    dum (rivus_eventum_coalitum(r, M, &e))
    {
        numerus++;
    }
    CREDO_AEQUALIS_I32 (numerus, II * RIVUS_BUFFER);

    imprimere("\n--- VIII. modi declarati: intrare, exire ---\n");
    numerus = rivus_modos_intrare(r, RIVUS_MODUS_MUS
        | RIVUS_MODUS_GLUTINUM
        | RIVUS_MODUS_FOCUS | RIVUS_MODUS_KITTY, modi,
        RIVUS_MODI_MAXIMUM);
    CREDO_VERUM (_octeti_aequales(modi, numerus,
        "\033[?1000h\033[?1002h\033[?1006h\033[?2004h\033[?1004h"
        "\033[>31u"));
    CREDO_AEQUALIS_I32 (rivus_interpres(r)->kitty_vexilla, 0x1F);
    CREDO_VERUM (rivus_eventum(r, M, &e));      /* facultates iterum */
    CREDO_VERUM (e.genus == EVENTUS_FACULTATES);
    CREDO_FALSUM (e.datum.facultates.super);
    /* iterum intrare sine exitu: nihil (kitty bis impulsum non) */
    CREDO_AEQUALIS_I32 (rivus_modos_intrare(r, RIVUS_MODUS_MUS, modi,
        RIVUS_MODI_MAXIMUM), ZEPHYRUM);
    numerus = rivus_modos_exire(r, modi, RIVUS_MODI_MAXIMUM);
    CREDO_VERUM (_octeti_aequales(modi, numerus,
        "\033[<u\033[?1004l\033[?2004l\033[?1006l\033[?1002l"
        "\033[?1000l"));
    CREDO_AEQUALIS_I32 (rivus_interpres(r)->kitty_vexilla, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (rivus_modos_exire(r, modi, RIVUS_MODI_MAXIMUM),
        ZEPHYRUM);
    /* super (?1003) solum declaratum: murem secum trahit */
    numerus = rivus_modos_intrare(r, RIVUS_MODUS_SUPER, modi,
        RIVUS_MODI_MAXIMUM);
    CREDO_VERUM (_octeti_aequales(modi, numerus,
        "\033[?1000h\033[?1002h\033[?1003h\033[?1006h"));
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_FACULTATES
        && e.datum.facultates.super);
    numerus = rivus_modos_exire(r, modi, RIVUS_MODI_MAXIMUM);
    CREDO_VERUM (_octeti_aequales(modi, numerus,
        "\033[?1006l\033[?1003l\033[?1002l\033[?1000l"));
    /* buffer angustus: nihil scriptum, status intactus */
    CREDO_AEQUALIS_I32 (rivus_modos_intrare(r, RIVUS_MODUS_KITTY, modi,
        IV), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (rivus_interpres(r)->kitty_vexilla, ZEPHYRUM);
    CREDO_FALSUM (rivus_eventum(r, M, &e));

    imprimere("\n--- IX. glutinum viarum -> DEPOSITIO ---\n");
    /* non declaratum: textus manet */
    _tradere(r, "\x1b[200~/tmp/a\x1b[201~");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_TEXTUS
        && chorda_aequalis_literis(e.datum.textus.contentum, "/tmp/a"));
    /* declaratum: ?2004 secum trahit, facultas HEURISTICA */
    numerus = rivus_modos_intrare(r, RIVUS_MODUS_DEPOSITIO, modi,
        RIVUS_MODI_MAXIMUM);
    CREDO_VERUM (_octeti_aequales(modi, numerus, "\033[?2004h"));
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_FACULTATES
        && e.datum.facultates.depositio
            == EVENTUS_DEPOSITIO_HEURISTICA);
    /* positio = indicator ultimus (cellula III,II -> 25,30) */
    _tradere(r, "\x1b[<0;3;2M\x1b[<0;3;2m");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (rivus_eventum(r, M, &e));
    _tradere(r, "\x1b[200~/Users/fran/a\\ b.txt /tmp/c\x1b[201~");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_DEPOSITIO);
    CREDO_VERUM (chorda_aequalis_literis(e.datum.depositio.viae,
        "/Users/fran/a b.txt\n/tmp/c"));
    CREDO_AEQUALIS_I32 (e.datum.depositio.numerus, II);
    CREDO_VERUM (e.datum.depositio.promota);
    CREDO_VERUM (e.datum.depositio.x == XXV
        && e.datum.depositio.y == XXX);
    /* citationes, file:// (localhost, %20) */
    _tradere(r, "\x1b[200~'/a b' \"/c\\\"d\"\nfile:///e%20f "
        "file://localhost/g\x1b[201~");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_DEPOSITIO);
    CREDO_VERUM (chorda_aequalis_literis(e.datum.depositio.viae,
        "/a b\n/c\"d\n/e f\n/g"));
    CREDO_AEQUALIS_I32 (e.datum.depositio.numerus, IV);
    /* non viae: textus manet */
    _tradere(r, "\x1b[200~/tmp/a and more\x1b[201~");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_TEXTUS && chorda_aequalis_literis(
        e.datum.textus.contentum, "/tmp/a and more"));
    _tradere(r, "\x1b[200~~/x\x1b[201~");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_TEXTUS);
    _tradere(r, "\x1b[200~'/a\x1b[201~");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_TEXTUS);
    _tradere(r, "\x1b[200~ \n \x1b[201~");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_TEXTUS);
    /* exitus: promotio cessat */
    numerus = rivus_modos_exire(r, modi, RIVUS_MODI_MAXIMUM);
    CREDO_VERUM (_octeti_aequales(modi, numerus, "\033[?2004l"));
    _tradere(r, "\x1b[200~/tmp/a\x1b[201~");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_TEXTUS);
    /* indicator numquam visus: positio 0,0 */
    novus = rivus_creare(piscina, X, XX);
    CREDO_NON_NIHIL (novus);
    CREDO_VERUM (rivus_eventum(novus, M, &e));          /* facultates */
    CREDO_VERUM (rivus_modos_intrare(novus, RIVUS_MODUS_DEPOSITIO, modi,
        RIVUS_MODI_MAXIMUM) > ZEPHYRUM);
    CREDO_VERUM (rivus_eventum(novus, M, &e));          /* facultates */
    _tradere(novus, "\x1b[200~/x\x1b[201~");
    CREDO_VERUM (rivus_eventum(novus, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_DEPOSITIO && e.datum.depositio.x
        == ZEPHYRUM && e.datum.depositio.y == ZEPHYRUM);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
