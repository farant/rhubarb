/* probatio_rivus_terminalis.c - pipeline fontis terminalis (eventus
 * B3a): quod proiectio tesserae non ostendit - morae poscendae, motus
 * NON coalescit (decodificatio pigra), glutinum trans traditiones
 * scissum, glutinum vacuum, capacitas, reliquiae muris post moram. */
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

s32 principale (vacuum)
{
            Piscina* piscina;
    RivusTerminalis* r;
            Eventus  e;
                 i8  magnum[CCC];

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
    dum (rivus_eventum(r, M, &e))
    {
    }
    CREDO_AEQUALIS_I32 (rivus_pendentes(r), ZEPHYRUM);

    imprimere("\n--- VI. reliquiae: mus SGR trans moram (H8) ---\n");
    _tradere(r, "\x1b[<0;20");
    CREDO_FALSUM (rivus_eventum(r, M, &e));
    rivus_moram(r, M);
    CREDO_FALSUM (rivus_eventum(r, M, &e));    /* nihil: servatur */
    _tradere(r, ";5M");
    CREDO_VERUM (rivus_eventum(r, M, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_DEPRESSUS
        && e.datum.mus.x == CXCV && e.datum.mus.y == XC);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
