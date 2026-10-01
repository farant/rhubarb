/* eventus_stml.c - Eventus[] <-> STML */

#include "eventus_stml.h"
#include "stml.h"
#include "chorda_aedificator.h"

#include <string.h>

/* Ordo = ordo enumerationis eventus_genus_t (eventus.h). Extende
 * UNA cum enumeratione (genera nova AD FINEM). */
hic_manens constans character* tituli[] = {
    "nihil", "claudere", "mutare_magnitudinem", "focus", "defocus",
    "exponere", "clavis_depressus", "clavis_liberatus", "mus_depressus",
    "mus_liberatus", "mus_motus", "mus_rotula", "mus_duplex",
    "mus_intravit", "mus_exiit", "focus_captus", "focus_amissus",
    "focus_petitus", "menu",
    /* eventus A2 */
    "textus", "depositio", "suspensio", "resumptio", "facultates"
};
#define TITULI_NUMERUS ((i32)(magnitudo(tituli) / magnitudo(tituli[0])))

/* Tituli W3C codicum physicorum; ordo = EventusCodex (eventus.h).
 * GENERATUM ex ordine enumerationis (eventus A2) - probatio circuitum
 * omnium codicum et EVENTUS_CODICES_NUMERUS == LXXXIV figit. */
hic_manens constans character* tituli_codicum[] = {
    "Unidentified", "KeyA", "KeyB", "KeyC", "KeyD", "KeyE", "KeyF",
        "KeyG",
    "KeyH", "KeyI", "KeyJ", "KeyK", "KeyL", "KeyM", "KeyN", "KeyO",
        "KeyP",
    "KeyQ", "KeyR", "KeyS", "KeyT", "KeyU", "KeyV", "KeyW", "KeyX",
        "KeyY",
    "KeyZ", "Digit0", "Digit1", "Digit2", "Digit3", "Digit4", "Digit5",
    "Digit6", "Digit7", "Digit8", "Digit9", "F1", "F2", "F3", "F4",
        "F5",
    "F6", "F7", "F8", "F9", "F10", "F11", "F12", "Space", "Enter",
        "Tab",
    "Backspace", "Escape", "ArrowLeft", "ArrowRight", "ArrowUp",
    "ArrowDown", "Home", "End", "PageUp", "PageDown", "Delete",
        "Insert",
    "Backquote", "Minus", "Equal", "BracketLeft", "BracketRight",
    "Backslash", "Semicolon", "Quote", "Comma", "Period", "Slash",
    "ShiftLeft", "ShiftRight", "ControlLeft", "ControlRight", "AltLeft",
    "AltRight", "MetaLeft", "MetaRight", "CapsLock"
};
#define CODICES_TITULORUM ((i32)(magnitudo(tituli_codicum) \
    / magnitudo(tituli_codicum[0])))

hic_manens constans character* tituli_actionum[] = {
    "pressa", "iterata", "soluta"
};
hic_manens constans character* tituli_indicatorum[] = {
    "mus", "stilus", "tactus"
};
hic_manens constans character* tituli_rotulae[] = {
    "ignota", "praecisa", "gradata"
};
hic_manens constans character* tituli_textuum[] = {
    "commissum", "componens"
};
hic_manens constans character* tituli_originum[] = {
    "scripta", "glutinata", "composita"
};


/* ==================================================
 * TITULI GENERUM ET CODICUM
 * ================================================== */

constans character*
eventus_genus_titulus (
    eventus_genus_t genus)
{
    si ((i32)genus >= TITULI_NUMERUS)
    { redde "ignotum";
    }
    redde tituli[genus];
}

eventus_genus_t
eventus_genus_ex_titulo (
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < TITULI_NUMERUS; i++)
    {
        si (strcmp(titulus, tituli[i]) == ZEPHYRUM)
        { redde (eventus_genus_t)i;
        }
    }
    redde EVENTUS_NIHIL;
}

constans character*
eventus_codex_titulus (
    EventusCodex codex)
{
    si ((i32)codex >= CODICES_TITULORUM)
    { redde tituli_codicum[ZEPHYRUM];
    }
    redde tituli_codicum[codex];
}

EventusCodex
eventus_codex_ex_titulo (
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < CODICES_TITULORUM; i++)
    {
        si (strcmp(titulus, tituli_codicum[i]) == ZEPHYRUM)
        { redde (EventusCodex)i;
        }
    }
    redde EVENTUS_CODEX_IGNOTUS;
}

/* Index tituli in tabula parva; -1 si abest */
interior s32
_index_tituli (
     constans character** tabula,
                    i32   numerus,
     constans character*  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (strcmp(titulus, tabula[i]) == ZEPHYRUM)
        { redde (s32)i;
        }
    }
    redde -I;
}


/* ==================================================
 * SCRIBERE
 * ================================================== */

interior vacuum
attr_s (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* t,
                    s32  v)
{
    stml_attributum_addere(n, p, in, t, chorda_ut_cstr(chorda_ex_s32(v,
        p), p));
}

interior vacuum
attr_longus (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* t,
                    s64  v)
{
    stml_attributum_addere(n, p, in, t,
        chorda_ut_cstr(chorda_ex_f64((f64)v, ZEPHYRUM, p), p));
}

interior vacuum
attr_f (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* t,
                    f32  v)
{
    stml_attributum_addere(n, p, in, t,
        chorda_ut_cstr(chorda_ex_f64((f64)v, III, p), p));
}

/* Textus in attributo: '%', '"' et octeti regiminis (< 0x20, 0x7F)
 * -> %XX (hex maiusculum); ceteri (UTF-8 quoque) verbatim. Exactum
 * (spatia, lineae novae) et legibile; quota in valore STML
 * irrepraesentabilis est (stml.h). */
interior vacuum
attr_textus (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* t,
                 chorda  v)
{
     constans character* hex = "0123456789ABCDEF";
      ChordaAedificator* aed;
                    i32  i;

    aed = chorda_aedificator_creare(p, (i32)(v.mensura * III + I));
    si (aed == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < (i32)v.mensura; i++)
    {
        i8 c = v.datum[i];

        si (   c == (i8)'%' || c == (i8)'"' || c < (i8)0x20
            || c == (i8)0x7F)
        {
            character tres[IV];

            tres[0] = '%';
            tres[1] = hex[(c >> IV) & 0x0F];
            tres[2] = hex[c & 0x0F];
            tres[3] = '\0';
            chorda_aedificator_appendere_literis(aed, tres);
        }
        alioquin
        {
            character unus[II];

            unus[0] = (character)c;
            unus[1] = '\0';
            chorda_aedificator_appendere_literis(aed, unus);
        }
    }
    stml_attributum_addere(n, p, in, t, chorda_ut_cstr(
        chorda_aedificator_finire(aed), p));
}

/* Attributa nova SOLUM si non ordinaria: plagulae veteres octetim
 * eaedem rescribuntur (probatio toy.eventus.stml). */
chorda
eventus_scribere_stml (
           constans Xar* eventus,
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  pulchrum)
{
    StmlNodus* radix;
    StmlNodus* n;
      Eventus* e;
          s32  codex_typi;
          i32  i;
          i32  k;
          i32  num;

    radix  = stml_elementum_creare(piscina, intern, "eventus_index");
    num    = xar_numerus(eventus);
    per (i = ZEPHYRUM; i < num; i++)
    {
        e = (Eventus*)xar_obtinere(eventus, i);
        n = stml_elementum_creare(piscina, intern, "eventus");
        stml_attributum_addere(n, piscina, intern, "genus",
                               eventus_genus_titulus(e->genus));
        attr_longus(n, piscina, intern, "tempus", e->tempus);
        commutatio (e->genus)
        {
            casus EVENTUS_MUS_DEPRESSUS:
            casus EVENTUS_MUS_LIBERATUS:
            casus EVENTUS_MUS_MOTUS:
            casus EVENTUS_MUS_DUPLEX:
                attr_s(n, piscina, intern, "x", (s32)e->datum.mus.x);
                attr_s(n, piscina, intern, "y", (s32)e->datum.mus.y);
                attr_s(n, piscina, intern, "botton",
                    (s32)e->datum.mus.botton);
                attr_s(n, piscina, intern, "modificantes",
                       (s32)e->datum.mus.modificantes);
                si (e->datum.mus.indicator != ZEPHYRUM)
                {
                    attr_s(n, piscina, intern, "indicator",
                        e->datum.mus.indicator);
                }
                si (   e->datum.mus.indicator_genus
                    != EVENTUS_INDICATOR_MUS
                    && (i32)e->datum.mus.indicator_genus < III)
                {
                    stml_attributum_addere(n, piscina, intern,
                        "indicator_genus",
                        tituli_indicatorum[e->datum.mus.indicator_genus]);
                }
                si (   e->datum.mus.pressio != EVENTUS_PRESSIO_IGNOTA
                    && e->datum.mus.pressio != ZEPHYRUM)
                {
                    attr_s(n, piscina, intern, "pressio",
                        e->datum.mus.pressio);
                }
                per (k = ZEPHYRUM; e->datum.mus.exempla != NIHIL
                     && k < e->datum.mus.numerus_exemplorum; k++)
                {
                    StmlNodus* x = stml_elementum_creare(piscina,
                        intern,
                        "exemplum");

                    attr_s(x, piscina, intern, "x",
                        e->datum.mus.exempla[k].x);
                    attr_s(x, piscina, intern, "y",
                        e->datum.mus.exempla[k].y);
                    attr_longus(x, piscina, intern, "tempus",
                        e->datum.mus.exempla[k].tempus);
                    stml_liberum_addere(n, x);
                }
                frange;
            casus EVENTUS_CLAVIS_DEPRESSUS:
            casus EVENTUS_CLAVIS_LIBERATUS:
                attr_s(n, piscina, intern, "clavis",
                    (s32)e->datum.clavis.clavis);
                codex_typi =
                    (s32)(insignatus character)e->datum.clavis.typus;
                attr_s(n, piscina, intern, "typus", codex_typi);
                attr_s(n, piscina, intern, "modificantes",
                       (s32)e->datum.clavis.modificantes);
                si (e->datum.clavis.runa != ZEPHYRUM)
                {
                    attr_s(n, piscina, intern, "runa",
                        e->datum.clavis.runa);
                }
                si (e->datum.clavis.codex != EVENTUS_CODEX_IGNOTUS)
                {
                    stml_attributum_addere(n, piscina, intern, "codex",
                        eventus_codex_titulus(e->datum.clavis.codex));
                }
                si (   e->datum.clavis.actio != EVENTUS_ACTIO_PRESSA
                    && (i32)e->datum.clavis.actio < III)
                {
                    stml_attributum_addere(n, piscina, intern, "actio",
                        tituli_actionum[e->datum.clavis.actio]);
                }
                frange;
            casus EVENTUS_MUS_ROTULA:
                attr_f(n, piscina, intern, "delta_x",
                    e->datum.rotula.delta_x);
                attr_f(n, piscina, intern, "delta_y",
                    e->datum.rotula.delta_y);
                si (   e->datum.rotula.dx    != ZEPHYRUM
                    || e->datum.rotula.dy    != ZEPHYRUM
                    || e->datum.rotula.genus != EVENTUS_ROTULA_IGNOTA)
                {
                    attr_s(n, piscina, intern, "dx",
                        e->datum.rotula.dx);
                    attr_s(n, piscina, intern, "dy",
                        e->datum.rotula.dy);
                    si ((i32)e->datum.rotula.genus < III)
                    {
                        stml_attributum_addere(n, piscina, intern,
                            "genus_rotulae",
                            tituli_rotulae[e->datum.rotula.genus]);
                    }
                }
                frange;
            casus EVENTUS_MUTARE_MAGNITUDINEM:
                attr_s(n, piscina, intern, "latitudo",
                       (s32)e->datum.mutare_magnitudinem.latitudo);
                attr_s(n, piscina, intern, "altitudo",
                       (s32)e->datum.mutare_magnitudinem.altitudo);
                frange;
            casus EVENTUS_TEXTUS:
                attr_textus(n, piscina, intern, "contentum",
                    e->datum.textus.contentum);
                si ((i32)e->datum.textus.genus < II)
                {
                    stml_attributum_addere(n, piscina, intern,
                        "genus_textus",
                        tituli_textuum[e->datum.textus.genus]);
                }
                attr_s(n, piscina, intern, "cursor",
                    e->datum.textus.cursor);
                si ((i32)e->datum.textus.origo < III)
                {
                    stml_attributum_addere(n, piscina, intern, "origo",
                        tituli_originum[e->datum.textus.origo]);
                }
                attr_s(n, piscina, intern, "truncatum",
                    (s32)(e->datum.textus.truncatum ? I : ZEPHYRUM));
                frange;
            casus EVENTUS_DEPOSITIO:
                attr_s(n, piscina, intern, "x",
                    (s32)e->datum.depositio.x);
                attr_s(n, piscina, intern, "y",
                    (s32)e->datum.depositio.y);
                attr_textus(n, piscina, intern, "viae",
                    e->datum.depositio.viae);
                attr_s(n, piscina, intern, "numerus",
                    (s32)e->datum.depositio.numerus);
                attr_s(n, piscina, intern, "promota",
                    (s32)(e->datum.depositio.promota ? I : ZEPHYRUM));
                frange;
            casus EVENTUS_FACULTATES:
            {
                constans EventusFacultates* f = &e->datum.facultates;

                attr_s(n, piscina, intern, "liberationes",
                    (s32)(f->liberationes ? I : ZEPHYRUM));
                attr_s(n, piscina, intern, "codex_physicus",
                    (s32)(f->codex_physicus ? I : ZEPHYRUM));
                attr_s(n, piscina, intern, "tabula_distincta",
                    (s32)(f->tabula_distincta ? I : ZEPHYRUM));
                attr_s(n, piscina, intern, "latera",
                    (s32)(f->latera ? I : ZEPHYRUM));
                attr_s(n, piscina, intern, "super",
                    (s32)(f->super ? I : ZEPHYRUM));
                attr_s(n, piscina, intern, "praeeditio",
                    (s32)(f->praeeditio ? I : ZEPHYRUM));
                attr_s(n, piscina, intern, "scriptura_copiae",
                    (s32)f->scriptura_copiae);
                attr_s(n, piscina, intern, "depositio",
                    (s32)f->depositio);
                attr_s(n, piscina, intern, "gradus_rotulae",
                    f->gradus_rotulae);
                attr_s(n, piscina, intern, "pressio",
                    (s32)(f->pressio ? I : ZEPHYRUM));
                frange;
            }
            ordinarius:
                frange;
        }
        stml_liberum_addere(radix, n);
    }
    redde stml_scribere(radix, piscina, pulchrum);
}


/* ==================================================
 * LEGERE
 * ================================================== */

interior s32
capere_s (
             StmlNodus* n,
    constans character* t)
{
    chorda* c;
       s32  v;

    c = stml_attributum_capere(n, t);
    v = ZEPHYRUM;
    si (c)
    { chorda_ut_s32(*c, &v);
    }
    redde v;
}

interior b32
habet (
             StmlNodus* n,
    constans character* t)
{
    redde (b32)(stml_attributum_capere(n, t) != NIHIL);
}

interior s64
capere_longus (
             StmlNodus* n,
    constans character* t)
{
    chorda* c;
       f64  v;

    c = stml_attributum_capere(n, t);
    v = 0.0;
    si (c)
    { chorda_ut_f64(*c, &v);
    }
    redde (s64)v;
}

interior f32
capere_f (
             StmlNodus* n,
    constans character* t)
{
    chorda* c;
       f64  v;

    c = stml_attributum_capere(n, t);
    v = 0.0;
    si (c)
    { chorda_ut_f64(*c, &v);
    }
    redde (f32)v;
}

/* Titulum attributi in tabula quaerere; 'si_abest' si abest (NB
 * 'ordinarius' macrum latinae est - default) */
interior s32
capere_titulum (
              StmlNodus*  n,
     constans character*  t,
     constans character** tabula,
                    i32   numerus,
                    s32   si_abest,
                Piscina*  p)
{
    chorda* c;
       s32  k;

    c = stml_attributum_capere(n, t);
    si (c == NIHIL)
    { redde si_abest;
    }
    k = _index_tituli(tabula, numerus, chorda_ut_cstr(*c, p));
    redde (k >= ZEPHYRUM) ? k : si_abest;
}

interior i32
_hex (
    i8 c)
{
    si (c >= (i8)'0' && c <= (i8)'9')
    { redde (i32)(c - (i8)'0');
    }
    si (c >= (i8)'A' && c <= (i8)'F')
    { redde (i32)(c - (i8)'A') + X;
    }
    si (c >= (i8)'a' && c <= (i8)'f')
    { redde (i32)(c - (i8)'a') + X;
    }
    redde XVI;
}

/* Inversum attr_textus: %XX -> octetus; in piscinam COPIATUM
 * (plagula possidet - spec Q16) */
interior chorda
capere_textum (
             StmlNodus* n,
    constans character* t,
               Piscina* p)
{
    chorda* c;
    chorda  r;
       i32  i;
       i32  m;

    r.datum    = NIHIL;
    r.mensura  = ZEPHYRUM;
    c          = stml_attributum_capere(n, t);
    si (c == NIHIL || c->mensura == ZEPHYRUM)
    { redde r;
    }
    r.datum = (i8*)piscina_allocare(p, (memoriae_index)c->mensura);
    si (r.datum == NIHIL)
    { redde r;
    }
    m = ZEPHYRUM;
    per (i = ZEPHYRUM; i < (i32)c->mensura; i++)
    {
        si (   c->datum[i] == (i8)'%' && i + II < (i32)c->mensura
            && _hex(c->datum[i + I]) < XVI
            && _hex(c->datum[i + II]) < XVI)
        {
            r.datum[m++] = (i8)(_hex(c->datum[i + I]) * XVI
                + _hex(c->datum[i + II]));
            i += II;
        }
        alioquin
        {
            r.datum[m++] = c->datum[i];
        }
    }
    r.mensura = m;
    redde r;
}

Xar*
eventus_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlResultus  res;
             Xar* index;
       StmlNodus* n;
         Eventus* e;
          chorda* g;

    res = stml_legere_ex_literis(cstr, piscina, intern);
    si (!res.successus || !res.elementum_radix)
    { redde NIHIL;
    }
    index  = xar_creare(piscina, (i32)magnitudo(Eventus));
    n      = stml_primus_liberum(res.elementum_radix);
    dum (n)
    {
        si (n->genus == STML_NODUS_ELEMENTUM)
        {
            e = (Eventus*)xar_addere(index);
            memset(e, ZEPHYRUM, magnitudo(Eventus));
            g = stml_attributum_capere(n, "genus");
            e->genus  = g ? eventus_genus_ex_titulo(chorda_ut_cstr(*g,
                piscina))
                          : EVENTUS_NIHIL;
            e->tempus = capere_longus(n, "tempus");
            si (   e->genus == EVENTUS_CLAVIS_DEPRESSUS
                || e->genus == EVENTUS_CLAVIS_LIBERATUS)
            {
                chorda* codex;

                e->datum.clavis.clavis = (clavis_t)capere_s(n,
                    "clavis");
                e->datum.clavis.typus = (character)capere_s(n,
                    "typus");
                e->datum.clavis.modificantes = (i32)capere_s(n,
                    "modificantes");
                e->datum.clavis.runa = capere_s(n, "runa");
                codex = stml_attributum_capere(n, "codex");
                e->datum.clavis.codex = codex
                    ? eventus_codex_ex_titulo(chorda_ut_cstr(*codex,
                        piscina))
                    : EVENTUS_CODEX_IGNOTUS;
                e->datum.clavis.actio = (EventusActio)capere_titulum(n,
                    "actio", tituli_actionum, III,
                    (s32)EVENTUS_ACTIO_PRESSA, piscina);
            }
            alioquin si (e->genus == EVENTUS_MUS_ROTULA)
            {
                e->datum.rotula.delta_x  = capere_f(n, "delta_x");
                e->datum.rotula.delta_y  = capere_f(n, "delta_y");
                e->datum.rotula.dx       = capere_s(n, "dx");
                e->datum.rotula.dy       = capere_s(n, "dy");
                e->datum.rotula.genus = (EventusRotulaGenus)
                    capere_titulum(n, "genus_rotulae", tituli_rotulae,
                        III, (s32)EVENTUS_ROTULA_IGNOTA, piscina);
            }
            alioquin si (e->genus == EVENTUS_MUTARE_MAGNITUDINEM)
            {
                e->datum.mutare_magnitudinem.latitudo = (i32)capere_s(n,
                    "latitudo");
                e->datum.mutare_magnitudinem.altitudo = (i32)capere_s(n,
                    "altitudo");
            }
            alioquin si (e->genus == EVENTUS_TEXTUS)
            {
                e->datum.textus.contentum = capere_textum(n,
                    "contentum",
                    piscina);
                e->datum.textus.genus = (EventusTextusGenus)
                    capere_titulum(n, "genus_textus", tituli_textuum,
                    II,
                        (s32)EVENTUS_TEXTUS_COMMISSUM, piscina);
                e->datum.textus.cursor = capere_s(n, "cursor");
                e->datum.textus.origo = (EventusOrigo)capere_titulum(n,
                    "origo", tituli_originum, III,
                    (s32)EVENTUS_ORIGO_SCRIPTA, piscina);
                e->datum.textus.truncatum = (b32)(capere_s(n,
                    "truncatum") != ZEPHYRUM);
            }
            alioquin si (e->genus == EVENTUS_DEPOSITIO)
            {
                e->datum.depositio.x = (i32)capere_s(n, "x");
                e->datum.depositio.y = (i32)capere_s(n, "y");
                e->datum.depositio.viae = capere_textum(n, "viae",
                    piscina);
                e->datum.depositio.numerus = (i32)capere_s(n,
                    "numerus");
                e->datum.depositio.promota = (b32)(capere_s(n,
                    "promota") != ZEPHYRUM);
            }
            alioquin si (e->genus == EVENTUS_FACULTATES)
            {
                EventusFacultates* f = &e->datum.facultates;

                f->liberationes      = (b32)(capere_s(n, "liberationes")
                    != ZEPHYRUM);
                f->codex_physicus    = (b32)(capere_s(n,
                    "codex_physicus") != ZEPHYRUM);
                f->tabula_distincta  = (b32)(capere_s(n,
                    "tabula_distincta") != ZEPHYRUM);
                f->latera            = (b32)(capere_s(n, "latera")
                    != ZEPHYRUM);
                f->super             = (b32)(capere_s(n, "super")
                    != ZEPHYRUM);
                f->praeeditio        = (b32)(capere_s(n, "praeeditio")
                    != ZEPHYRUM);
                f->scriptura_copiae  = (i32)capere_s(n,
                    "scriptura_copiae");
                f->depositio       = (i32)capere_s(n, "depositio");
                f->gradus_rotulae  = capere_s(n, "gradus_rotulae");
                f->pressio           = (b32)(capere_s(n, "pressio")
                    != ZEPHYRUM);
            }
            alioquin
            {
                 StmlNodus* x;
                       i32  numerus;
                       i32  k;

                e->datum.mus.x = (i32)capere_s(n, "x");
                e->datum.mus.y = (i32)capere_s(n, "y");
                e->datum.mus.botton = (mus_botton_t)capere_s(n,
                    "botton");
                e->datum.mus.modificantes = (i32)capere_s(n,
                    "modificantes");
                e->datum.mus.indicator = capere_s(n, "indicator");
                e->datum.mus.indicator_genus = (EventusIndicatorGenus)
                    capere_titulum(n, "indicator_genus",
                        tituli_indicatorum, III,
                        (s32)EVENTUS_INDICATOR_MUS, piscina);
                e->datum.mus.pressio = habet(n, "pressio")
                    ? capere_s(n, "pressio") : EVENTUS_PRESSIO_IGNOTA;
                /* exempla COPIATA in piscinam (plagula possidet) */
                numerus = ZEPHYRUM;
                per (x = stml_primus_liberum(n); x != NIHIL;
                     x = stml_frater_proximus(x))
                {
                    si (x->genus == STML_NODUS_ELEMENTUM)
                    { numerus++;
                    }
                }
                si (numerus > ZEPHYRUM)
                {
                    EventusExemplum* ex = (EventusExemplum*)
                        piscina_allocare(piscina,
                        (memoriae_index)numerus
                            * magnitudo(EventusExemplum));

                    k = ZEPHYRUM;
                    per (x = stml_primus_liberum(n); ex != NIHIL
                         && x != NIHIL; x = stml_frater_proximus(x))
                    {
                        si (x->genus != STML_NODUS_ELEMENTUM)
                        { perge;
                        }
                        ex[k].x       = capere_s(x, "x");
                        ex[k].y       = capere_s(x, "y");
                        ex[k].tempus  = capere_longus(x, "tempus");
                        k++;
                    }
                    e->datum.mus.exempla             = ex;
                    e->datum.mus.numerus_exemplorum  = (ex != NIHIL)
                        ? k : ZEPHYRUM;
                }
            }
        }
        n = stml_frater_proximus(n);
    }
    redde index;
}
