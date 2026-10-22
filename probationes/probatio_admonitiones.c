/* probatio_admonitiones.c - admonitiones (colloquia-plan T4a): ordo
 * recentissimum primum, id iteratum, maximum (vetustissimae eiciuntur),
 * lectae, chordae copiatae; ex eventu praeconii (licentia, Stop prima
 * linea, StopFailure; ceteri non). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "praeconium.h"
#include "admonitiones.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Admonitio
admonitio_ficta (
    constans character* id,
                   s64  tempus,
               Piscina* p)
{
    Admonitio x;

    memset(&x, ZEPHYRUM, magnitudo(x));
    x.id       = chorda_ex_literis(id, p);
    x.fons     = chorda_ex_literis("probatio", p);
    x.titulus  = chorda_ex_literis("titulus", p);
    x.corpus   = chorda_ex_literis("corpus", p);
    x.tempus   = tempus;
    redde x;
}

/* ordo, id iteratum, maximum, lectae */
interior vacuum
_probare_indicem (
    CredoContextus* c)
{
    Admonitiones* a;
       Admonitio  x;

    a = admonitiones_creare(c->piscina, III);
    CREDO_NECESSE(a != NIHIL);
    x = admonitio_ficta("a", (s64)XX, c->piscina);
    CREDO_VERUM(admonitiones_addere(a, &x));
    x = admonitio_ficta("b", (s64)X, c->piscina);
    CREDO_VERUM(admonitiones_addere(a, &x));
    x = admonitio_ficta("c", (s64)XXX, c->piscina);
    CREDO_VERUM(admonitiones_addere(a, &x));
    /* ordo: c (XXX), a (XX), b (X) */
    CREDO_AEQUALIS_I32(admonitiones_numerus(a), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(a, ZEPHYRUM)->id, "c");
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(a, I)->id, "a");
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(a, II)->id, "b");
    CREDO_VERUM(admonitio(a, III) == NIHIL);
    /* id iteratum ignoratur */
    x = admonitio_ficta("a", (s64)XL, c->piscina);
    CREDO_FALSUM(admonitiones_addere(a, &x));
    CREDO_AEQUALIS_I32(admonitiones_numerus(a), III);
    /* maximum III: d (XV) intrat, b (X) vetustissima eicitur */
    x = admonitio_ficta("d", (s64)XV, c->piscina);
    CREDO_VERUM(admonitiones_addere(a, &x));
    CREDO_AEQUALIS_I32(admonitiones_numerus(a), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(a, II)->id, "d");
    /* lectae */
    CREDO_AEQUALIS_I32(admonitiones_non_lectae(a), III);
    CREDO_VERUM(admonitionem_legere_notare(a, chorda_ex_literis("a",
        c->piscina)));
    CREDO_FALSUM(admonitionem_legere_notare(a, chorda_ex_literis("b",
        c->piscina)));
    CREDO_AEQUALIS_I32(admonitiones_non_lectae(a), II);
    CREDO_VERUM(admonitio(a, I)->lecta);
}

/* chordae copiantur: alveus fontis mutatus admonitionem non mutat */
interior vacuum
_probare_copiam (
    CredoContextus* c)
{
     Admonitiones* a;
        Admonitio  x;
        character  alveus[XVI];
        character  alveus_generis[XVI];

    a = admonitiones_creare(c->piscina, X);
    strcpy(alveus_generis, "finis");
    CREDO_NECESSE(a != NIHIL);
    strcpy(alveus, "originale");
    x                 = admonitio_ficta("k", (s64)I, c->piscina);
    x.corpus.datum    = (i8*)alveus;
    x.corpus.mensura  = (i32)strlen(alveus);
    x.genus.datum     = (i8*)alveus_generis;
    x.genus.mensura   = (i32)strlen(alveus_generis);
    CREDO_VERUM(admonitiones_addere(a, &x));
    strcpy(alveus, "MUTATUM!!");
    strcpy(alveus_generis, "MUTATUM");
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(a, ZEPHYRUM)->corpus,
        "originale");
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(a, ZEPHYRUM)->genus,
        "finis");
}

interior PraeconiumEventus
eventus_fictus (
    constans character* eventus,
    constans character* genus,
    constans character* nuntius,
               Piscina* p)
{
    PraeconiumEventus e;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.tempus    = (s64)M;
    e.sessio    = chorda_ex_literis("s-1", p);
    e.eventus   = chorda_ex_literis(eventus, p);
    e.panellus  = chorda_ex_literis("%17", p);
    e.servitor  = chorda_ex_literis("/private/tmp/tmux-501/default", p);
    e.via       = chorda_ex_literis("/w/alpha", p);
    e.genus     = chorda_ex_literis(genus, p);
    e.nuntius   = chorda_ex_literis(nuntius, p);
    redde e;
}

/* ex eventu: licentia ATTENTIO, Stop NUNTIUS (prima linea),
 * StopFailure ATTENTIO; PreToolUse, idle_prompt non */
interior vacuum
_probare_ex_eventu (
    CredoContextus* c)
{
    PraeconiumEventus e;
            Admonitio x;
               chorda t;

    t = chorda_ex_literis("secunda", c->piscina);
    e = eventus_fictus("Notification", "permission_prompt",
        "Claude needs your permission to use Bash", c->piscina);
    CREDO_VERUM(admonitio_ex_eventu(&e, t, &x, c->piscina));
    CREDO_AEQUALIS_S32((s32)x.gradus, (s32)ADMONITIO_ATTENTIO);
    CREDO_CHORDA_AEQUALIS_LITERIS(x.genus, "permissio");
    CREDO_CHORDA_AEQUALIS_LITERIS(x.titulus, "secunda");
    CREDO_CHORDA_AEQUALIS_LITERIS(x.corpus,
        "Claude needs your permission to use Bash");
    CREDO_CHORDA_AEQUALIS_LITERIS(x.fons, "colloquia");
    CREDO_CHORDA_AEQUALIS_LITERIS(x.id, "colloquia:s-1:1000");
    CREDO_CHORDA_AEQUALIS_LITERIS(x.actio, "panellus");
    CREDO_CHORDA_AEQUALIS_LITERIS(x.argumentum,
        "%17 /private/tmp/tmux-501/default");
    CREDO_AEQUALIS_S64(x.tempus, (s64)M);
    CREDO_FALSUM(x.lecta);

    e = eventus_fictus("Stop", "", "Done: built.\nDetails follow.",
        c->piscina);
    CREDO_VERUM(admonitio_ex_eventu(&e, t, &x, c->piscina));
    CREDO_AEQUALIS_S32((s32)x.gradus, (s32)ADMONITIO_NUNTIUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x.corpus, "Done: built.");
    CREDO_CHORDA_AEQUALIS_LITERIS(x.genus, "finis");

    e = eventus_fictus("StopFailure", "", "API error: overloaded",
        c->piscina);
    CREDO_VERUM(admonitio_ex_eventu(&e, t, &x, c->piscina));
    CREDO_AEQUALIS_S32((s32)x.gradus, (s32)ADMONITIO_ATTENTIO);
    CREDO_CHORDA_AEQUALIS_LITERIS(x.genus, "vitium");

    /* input et elicitatio: permissio quoque */
    e = eventus_fictus("Notification", "agent_needs_input", "input?",
        c->piscina);
    CREDO_VERUM(admonitio_ex_eventu(&e, t, &x, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(x.genus, "permissio");
    e = eventus_fictus("Notification", "elicitation_dialog", "choose",
        c->piscina);
    CREDO_VERUM(admonitio_ex_eventu(&e, t, &x, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(x.genus, "permissio");

    e = eventus_fictus("PreToolUse", "Bash", "ls", c->piscina);
    CREDO_FALSUM(admonitio_ex_eventu(&e, t, &x, c->piscina));
    e = eventus_fictus("Notification", "idle_prompt", "waiting",
        c->piscina);
    CREDO_FALSUM(admonitio_ex_eventu(&e, t, &x, c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "index", _probare_indicem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "copia", _probare_copiam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ex_eventu", _probare_ex_eventu, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("admonitiones", SECTIONES);
}
