/* probatio_codificator_terminalis.c - codificator clavium (eventus
 * B6a-i): vectores ex Ghostty (../ghostty @ 12752b2, key_encode.zig)
 * translati in Eventus, consumptio fluxus (clavis + TEXTUS), et
 * ORACULUM tabulae: octeti columnae terminalis (B4b) per rivum
 * decodificati, deinde codificati, octeti iidem.
 *
 * Translatio Ghostty -> Eventus: key = clavis + codex, utf8 = TEXTUS
 * sequens (aut typus ubi textus deest: interpres sub Ctrl/Alt textum
 * non emittit), unshifted_codepoint = runa. consumed_mods non habemus.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "internamentum.h"
#include "filum.h"
#include "eventus.h"
#include "eventus_conformitas.h"
#include "interpres_terminalis.h"
#include "rivus_terminalis.h"
#include "codificator_terminalis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define DIS INTERPRES_KITTY_DISCERNERE
#define G   INTERPRES_KITTY_GENERA
#define AL  INTERPRES_KITTY_ALTERNAE
#define O   INTERPRES_KITTY_OMNES
#define T   INTERPRES_KITTY_TEXTUS
#define OMNIA (DIS | G | AL | O | T)

#define P   EVENTUS_ACTIO_PRESSA
#define IT  EVENTUS_ACTIO_ITERATA
#define S   EVENTUS_ACTIO_SOLUTA

#define SH  MOD_SHIFT
#define CT  MOD_IMPERIUM
#define AT  MOD_ALT
#define SU  MOD_SUPER

#define LIT(c)  ((EventusCodex)(EVENTUS_CODEX_LITTERAE + ((c) - 'a')))
#define NUM(d)  ((EventusCodex)(EVENTUS_CODEX_NUMERI + (d)))
#define FUNCTIO(n)  ((EventusCodex)(EVENTUS_CODEX_FUNCTIONES + (n) - I))
#define CF(n)   ((clavis_t)((s32)CLAVIS_F1 + (n) - I))

nomen structura {
    constans character* titulus;
                   i32  vexilla;
          EventusActio  actio;
              clavis_t  clavis;
          EventusCodex  codex;
                   s32  runa;
             character  typus;
                   i32  modi;
    constans character* textus;        /* NIHIL: sine TEXTU */
    constans character* expectatum;    /* "" = inexpressibile */
} Vector;

interior constans Vector VECTORES[] = {
    /* --- kitty --- */
    { "k textus planus", DIS, P, (clavis_t)'A', LIT('a'), 'a', 'a', 0,
      "abcd", "abcd" },
    { "k iterata disambiguata", DIS, IT, (clavis_t)'A', LIT('a'), 'a',
        'a',
      0, "a", "a" },
    { "k reditus", DIS, P, CLAVIS_REDITUS, EVENTUS_CODEX_REDITUS, 0,
        '\r',
      0, NIHIL, "\r" },
    { "k retrorsum", DIS, P, CLAVIS_RETRORSUM, EVENTUS_CODEX_RETRORSUM,
        0,
      0, 0, NIHIL, "\177" },
    { "k tabula", DIS, P, CLAVIS_TABULA, EVENTUS_CODEX_TABULA, 0, '\t',
        0,
      NIHIL, "\t" },
    { "k solutio reditus sine omnibus", DIS | G, S, CLAVIS_REDITUS,
      EVENTUS_CODEX_REDITUS, 0, '\r', 0, NIHIL, "" },
    { "k solutio tabulae sine omnibus", DIS | G, S, CLAVIS_TABULA,
      EVENTUS_CODEX_TABULA, 0, '\t', 0, NIHIL, "" },
    { "k solutio reditus omnes", DIS | G | O, S, CLAVIS_REDITUS,
      EVENTUS_CODEX_REDITUS, 0, '\r', 0, NIHIL, "\033[13;1:3u" },
    { "k solutio retrorsum omnes", DIS | G | O, S, CLAVIS_RETRORSUM,
      EVENTUS_CODEX_RETRORSUM, 0, 0, 0, NIHIL, "\033[127;1:3u" },
    { "k solutio tabulae omnes", DIS | G | O, S, CLAVIS_TABULA,
      EVENTUS_CODEX_TABULA, 0, '\t', 0, NIHIL, "\033[9;1:3u" },
    { "k shift+retrorsum", DIS, P, CLAVIS_RETRORSUM,
      EVENTUS_CODEX_RETRORSUM, 0, 0, SH, NIHIL, "\033[127;2u" },
    { "k alt+retrorsum", DIS, P, CLAVIS_RETRORSUM,
        EVENTUS_CODEX_RETRORSUM,
      0, 0, AT, NIHIL, "\033[127;3u" },
    { "k shift+reditus", DIS, P, CLAVIS_REDITUS, EVENTUS_CODEX_REDITUS,
        0,
      '\r', SH, NIHIL, "\033[13;2u" },
    { "k shift+tabula", DIS, P, CLAVIS_TABULA, EVENTUS_CODEX_TABULA, 0,
      '\t', SH, NIHIL, "\033[9;2u" },
    { "k reditus vexillis omnibus", OMNIA, P, CLAVIS_REDITUS,
      EVENTUS_CODEX_REDITUS, 0, '\r', 0, NIHIL, "\033[13u" },
    { "k imperium sinistrum", OMNIA, P, CLAVIS_SINISTER_IMPERIUM,
      EVENTUS_CODEX_IMPERIUM_SINISTRUM, 0, 0, CT, NIHIL,
      "\033[57442;5u" },
    { "k imperium sinistrum solutum", OMNIA, S,
        CLAVIS_SINISTER_IMPERIUM,
      EVENTUS_CODEX_IMPERIUM_SINISTRUM, 0, 0, CT, NIHIL,
      "\033[57442;5:3u" },
    { "k delere", DIS, P, CLAVIS_DELERE, EVENTUS_CODEX_DELERE, 0, 0, 0,
      "\177", "\033[3~" },
    { "k shift+a omnia", OMNIA, P, (clavis_t)'A', LIT('a'), 'a', 'A',
        SH,
      "A", "\033[97:65;2;65u" },
    { "k shift+a soluta", OMNIA, S, (clavis_t)'A', LIT('a'), 'a', 'A',
        SH,
      NIHIL, "\033[97;2:3u" },
    { "k ctrl+i", OMNIA, P, (clavis_t)'I', LIT('i'), 'i', 'i', CT,
        NIHIL,
      "\033[105;5u" },
    { "k shift+sinistra", OMNIA, P, CLAVIS_SINISTER,
      EVENTUS_CODEX_SAGITTA_SINISTRA, 0, 0, SH, NIHIL, "\033[1;2:1D" },
    { "k shift+sinistra soluta", OMNIA, S, CLAVIS_SINISTER,
      EVENTUS_CODEX_SAGITTA_SINISTRA, 0, 0, SH, NIHIL, "\033[1;2:3D" },
    { "k sursum", OMNIA, P, CLAVIS_SURSUM, EVENTUS_CODEX_SAGITTA_SURSUM,
        0,
      0, 0, NIHIL, "\033[1;1:1A" },
    { "k sursum disambiguata", DIS, P, CLAVIS_SURSUM,
      EVENTUS_CODEX_SAGITTA_SURSUM, 0, 0, 0, NIHIL, "\033[A" },
    { "k effugium", OMNIA, P, CLAVIS_EFFUGIUM, EVENTUS_CODEX_EFFUGIUM,
        0,
      0, 0, NIHIL, "\033[27u" },
    { "k effugium solutum", OMNIA, S, CLAVIS_EFFUGIUM,
      EVENTUS_CODEX_EFFUGIUM, 0, 0, 0, NIHIL, "\033[27;1:3u" },
    { "k F1", DIS, P, CF(1), FUNCTIO(1), 0, 0, 0, NIHIL, "\033[P" },
    { "k F3", DIS, P, CF(3), FUNCTIO(3), 0, 0, 0, NIHIL, "\033[13~" },
    { "k shift+F5", DIS, P, CF(5), FUNCTIO(5), 0, 0, SH, NIHIL,
        "\033[15;2~" },
    { "k shift sinister omnia", OMNIA, P, CLAVIS_SINISTER_SHIFT,
      EVENTUS_CODEX_MAIUSCULA_SINISTRA, 0, 0, SH, NIHIL,
      "\033[57441;2u" },
    { "k shift sinister disambiguata", DIS, P, CLAVIS_SINISTER_SHIFT,
      EVENTUS_CODEX_MAIUSCULA_SINISTRA, 0, 0, SH, NIHIL, "" },
    { "k ctrl+a", DIS, P, (clavis_t)'A', LIT('a'), 'a', 'a', CT, NIHIL,
      "\033[97;5u" },
    { "k alt+a textus prohibitus", DIS | T, P, (clavis_t)'A', LIT('a'),
        'a',
      'a', AT, NIHIL, "\033[97;3u" },

    /* --- legacy --- */
    { "l ctrl+c", 0, P, (clavis_t)'C', LIT('c'), 'c', 'c', CT, NIHIL,
      "\003" },
    { "l ctrl+spatium", 0, P, CLAVIS_SPATIUM, EVENTUS_CODEX_SPATIUM,
        ' ',
      ' ', CT, NIHIL, "\000" },
    { "l alt+c", 0, P, (clavis_t)'C', LIT('c'), 'c', 'c', AT, NIHIL,
      "\033c" },
    { "l ctrl+alt+c", 0, P, (clavis_t)'C', LIT('c'), 'c', 'c', CT | AT,
      NIHIL, "\033\003" },
    { "l ctrl+shift+minus", 0, P, (clavis_t)'-', EVENTUS_CODEX_MINUS,
        '-',
      '_', CT | SH, NIHIL, "\037" },
    { "l F1", 0, P, CF(1), FUNCTIO(1), 0, 0, 0, NIHIL, "\033OP" },
    { "l shift+F1", 0, P, CF(1), FUNCTIO(1), 0, 0, SH, NIHIL,
        "\033[1;2P" },
    { "l F3", 0, P, CF(3), FUNCTIO(3), 0, 0, 0, NIHIL, "\033OR" },
    { "l ctrl+F3", 0, P, CF(3), FUNCTIO(3), 0, 0, CT, NIHIL,
        "\033[13;5~" },
    { "l F5", 0, P, CF(5), FUNCTIO(5), 0, 0, 0, NIHIL, "\033[15~" },
    { "l F12", 0, P, CF(12), FUNCTIO(12), 0, 0, 0, NIHIL, "\033[24~" },
    { "l shift+tabula", 0, P, CLAVIS_TABULA, EVENTUS_CODEX_TABULA, 0,
      '\t', SH, NIHIL, "\033[Z" },
    { "l ctrl+tabula", 0, P, CLAVIS_TABULA, EVENTUS_CODEX_TABULA, 0,
      '\t', CT, NIHIL, "\033[27;5;9~" },
    { "l alt+tabula", 0, P, CLAVIS_TABULA, EVENTUS_CODEX_TABULA, 0,
        '\t',
      AT, NIHIL, "\033\t" },
    { "l tabula", 0, P, CLAVIS_TABULA, EVENTUS_CODEX_TABULA, 0, '\t', 0,
      NIHIL, "\t" },
    { "l reditus", 0, P, CLAVIS_REDITUS, EVENTUS_CODEX_REDITUS, 0, '\r',
      0, NIHIL, "\r" },
    { "l alt+reditus", 0, P, CLAVIS_REDITUS, EVENTUS_CODEX_REDITUS, 0,
      '\r', AT, NIHIL, "\033\r" },
    { "l ctrl+reditus", 0, P, CLAVIS_REDITUS, EVENTUS_CODEX_REDITUS, 0,
      '\r', CT, NIHIL, "\033[27;5;13~" },
    { "l shift+reditus", 0, P, CLAVIS_REDITUS, EVENTUS_CODEX_REDITUS, 0,
      '\r', SH, NIHIL, "\033[27;2;13~" },
    { "l retrorsum", 0, P, CLAVIS_RETRORSUM, EVENTUS_CODEX_RETRORSUM, 0,
      0, 0, NIHIL, "\177" },
    { "l ctrl+retrorsum", 0, P, CLAVIS_RETRORSUM,
        EVENTUS_CODEX_RETRORSUM,
      0, 0, CT, NIHIL, "\010" },
    { "l alt+retrorsum", 0, P, CLAVIS_RETRORSUM,
        EVENTUS_CODEX_RETRORSUM,
      0, 0, AT, NIHIL, "\033\177" },
    { "l effugium", 0, P, CLAVIS_EFFUGIUM, EVENTUS_CODEX_EFFUGIUM, 0, 0,
      0, NIHIL, "\033" },
    { "l alt+effugium", 0, P, CLAVIS_EFFUGIUM, EVENTUS_CODEX_EFFUGIUM,
        0,
      0, AT, NIHIL, "\033\033" },
    { "l sursum", 0, P, CLAVIS_SURSUM, EVENTUS_CODEX_SAGITTA_SURSUM, 0,
        0,
      0, NIHIL, "\033[A" },
    { "l shift+sinistra", 0, P, CLAVIS_SINISTER,
      EVENTUS_CODEX_SAGITTA_SINISTRA, 0, 0, SH, NIHIL, "\033[1;2D" },
    { "l alt+dextra", 0, P, CLAVIS_DEXTER, EVENTUS_CODEX_SAGITTA_DEXTRA,
        0,
      0, AT, NIHIL, "\033[1;3C" },
    { "l ctrl+domus", 0, P, CLAVIS_DOMUS, EVENTUS_CODEX_DOMUS, 0, 0, CT,
      NIHIL, "\033[1;5H" },
    { "l insertio", 0, P, CLAVIS_IGNOTA, EVENTUS_CODEX_INSERERE, 0, 0,
        0,
      NIHIL, "\033[2~" },
    { "l shift+delere", 0, P, CLAVIS_DELERE, EVENTUS_CODEX_DELERE, 0, 0,
      SH, NIHIL, "\033[3;2~" },
    { "l pagina sursum", 0, P, CLAVIS_PAGINA_SURSUM,
      EVENTUS_CODEX_PAGINA_SURSUM, 0, 0, 0, NIHIL, "\033[5~" },
    { "l a", 0, P, (clavis_t)'A', LIT('a'), 'a', 'a', 0, "a", "a" },
    { "l shift+a", 0, P, (clavis_t)'A', LIT('a'), 'a', 'A', SH, "A",
        "A" },
    { "l iterata", 0, IT, (clavis_t)'A', LIT('a'), 'a', 'a', 0, "a",
        "a" },
    { "l soluta inexpressibilis", 0, S, (clavis_t)'A', LIT('a'), 'a',
        'a',
      0, NIHIL, "" },
    { "l super+a macOS", 0, P, (clavis_t)'A', LIT('a'), 'a', 'a', SU,
        "a",
      "" },
    { "l ctrl+1 (kitty: '1')", 0, P, (clavis_t)'1', NUM(1), '1', '1',
        CT,
      NIHIL, "1" },
    { "l ctrl+i fixterms", 0, P, (clavis_t)'I', LIT('i'), 'i', 'i', CT,
      NIHIL, "\033[105;5u" }
};

#define VECTORUM_NUMERUS \
    ((i32)(magnitudo(VECTORES) / magnitudo(VECTORES[0])))

/* Vector -> eventa (clavis, et TEXTUS si datur) */
interior i32
_eventa (
    constans Vector* v,
            Eventus* e)
{
    i32 n = I;
    unio { constans character* l; i8* m; } u;

    memset(e, ZEPHYRUM, II * magnitudo(Eventus));
    e[0].genus = (v->actio == S) ? EVENTUS_CLAVIS_LIBERATUS
                                 : EVENTUS_CLAVIS_DEPRESSUS;
    e[0].datum.clavis.clavis        = v->clavis;
    e[0].datum.clavis.codex         = v->codex;
    e[0].datum.clavis.runa          = v->runa;
    e[0].datum.clavis.typus         = v->typus;
    e[0].datum.clavis.modificantes  = v->modi;
    e[0].datum.clavis.actio         = v->actio;
    si (v->textus)
    {
        e[1].genus                           = EVENTUS_TEXTUS;
        u.l                                  = v->textus;
        e[1].datum.textus.contentum.datum    = u.m;
        e[1].datum.textus.contentum.mensura  = (i32)strlen(v->textus);
        n                                    = II;
    }
    redde n;
}

interior b32
_octeti_aequales (
                 chorda  c,
     constans character* t,
                    i32  mensura)
{
    redde (b32)(   c.mensura == mensura
                && (mensura == ZEPHYRUM
                    || memcmp(c.datum, t, (memoriae_index)mensura)
                       == ZEPHYRUM));
}

/* mensura expectati: "\000" unum octetum est (strlen falleret) */
interior i32
_mensura (
    constans character* titulus,
    constans character* t)
{
    si (strcmp(titulus, "l ctrl+spatium") == ZEPHYRUM)
    {
        redde I;
    }
    redde (i32)strlen(t);
}

#define TABULA "probationes/fixa/eventus/conformitas.stml"

interior vacuum
_ostendere (
     constans character* quid,
     constans character* titulus,
                 chorda  c)
{
    i32 k;

    imprimere("%s %s: [", quid, titulus);
    per (k = ZEPHYRUM; k < c.mensura; k++)
    {
        imprimere(" %02x", (unsigned)(unsigned char)c.datum[k]);
    }
    imprimere(" ]\n");
}

/* Octeti -> rivus (modi ut programma: kitty si vexilla) -> Eventus ->
 * codificator (eadem vexilla) -> octeti */
interior chorda
_reditus (
    constans i8* octeti,
            i32  mensura,
            i32  vexilla,
        Piscina* piscina)
{
       RivusTerminalis* r = rivus_creare(piscina, I, I);
     ChordaAedificator* a = chorda_aedificator_creare(piscina,
         LXIV);
                   Xar* eventa = xar_creare(piscina,
                       (i32)magnitudo(Eventus));
       CodificatorModi modi;
                    i8 m[RIVUS_MODI_MAXIMUM];
               Eventus e;
                   i32 k;

    (vacuum)rivus_modos_intrare(r, RIVUS_MODUS_MUS
        | RIVUS_MODUS_GLUTINUM
        | ((vexilla != ZEPHYRUM) ? RIVUS_MODUS_KITTY : ZEPHYRUM), m,
        RIVUS_MODI_MAXIMUM);
    (vacuum)rivus_tradere(r, octeti, mensura);
    dum (rivus_eventum_coalitum(r, M, &e))
    {
        *(Eventus*)xar_addere(eventa) = e;
    }
    rivus_moram(r, M);
    dum (rivus_eventum_coalitum(r, M, &e))
    {
        *(Eventus*)xar_addere(eventa) = e;
    }
    memset(&modi, ZEPHYRUM, magnitudo(CodificatorModi));
    modi.kitty_vexilla  = vexilla;
    k                   = ZEPHYRUM;
    dum (k < xar_numerus(eventa))
    {
        k += codificator_eventa(&modi, (constans Eventus*)xar_obtinere(
            eventa, k), xar_numerus(eventa) - k, a);
    }
    redde chorda_aedificator_finire(a);
}

/* mus SGR in octetis? */
interior b32
_mus_adest (
    chorda o)
{
    i32 k;

    per (k = ZEPHYRUM; k + II < o.mensura; k++)
    {
        si (   o.datum[k]      == (i8)0x1B && o.datum[k + I] == '['
            && o.datum[k + II] == '<')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

s32 principale (vacuum)
{
             Piscina* piscina;
 InternamentumChorda* intern;
     CodificatorModi  modi;
                 i32  i;

    piscina = piscina_generare_dynamicum(
        "probatio_codificator_terminalis", M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    memset(&modi, ZEPHYRUM, magnitudo(CodificatorModi));

    imprimere("\n--- I. vectores Ghostty (%d) ---\n",
        (int)VECTORUM_NUMERUS);
    per (i = ZEPHYRUM; i < VECTORUM_NUMERUS; i++)
    {
        constans Vector* v = &VECTORES[i];
                Eventus  e[II];
                    i32  n;
                    i32  consumpta;
      ChordaAedificator* a = chorda_aedificator_creare(piscina, LXIV);
                 chorda  c;

        n                   = _eventa(v, e);
        modi.kitty_vexilla  = v->vexilla;
        consumpta           = codificator_eventa(&modi, e, n, a);
        c                   = chorda_aedificator_finire(a);
        si (!_octeti_aequales(c, v->expectatum,
                _mensura(v->titulus, v->expectatum)))
        {
            i32 k;

            imprimere("FRACTUS %s: [", v->titulus);
            per (k = ZEPHYRUM; k < c.mensura; k++)
            {
                imprimere(" %02x", (unsigned)(unsigned char)c.datum[k]);
            }
            imprimere(" ]\n");
        }
        CREDO_VERUM (_octeti_aequales(c, v->expectatum,
            _mensura(v->titulus, v->expectatum)));
        CREDO_AEQUALIS_I32 (consumpta, n);
    }

    imprimere("\n--- II. consumptio: clavis sine TEXTU sequente ---\n");
    {
                  Eventus  e[II];
        ChordaAedificator* a = chorda_aedificator_creare(piscina, LXIV);

        (vacuum)_eventa(&VECTORES[ZEPHYRUM], e);
        /* clavis, clavis: textus non sequitur */
        e[1]                = e[0];
        modi.kitty_vexilla  = ZEPHYRUM;
        CREDO_AEQUALIS_I32 (codificator_eventa(&modi, e, II, a), I);
        CREDO_AEQUALIS_I32 (codificator_eventa(&modi, e, ZEPHYRUM, a),
            ZEPHYRUM);
    }

    imprimere("\n--- III. reditus: octeti, rivus, codificator ---\n");
    per (i = ZEPHYRUM; i < VECTORUM_NUMERUS; i++)
    {
        constans Vector* v = &VECTORES[i];
                    i32  n = _mensura(v->titulus, v->expectatum);
                 chorda  c;

        si (n == ZEPHYRUM)
        {
            perge;      /* inexpressibile: nihil decodificandum */
        }
        c = _reditus((constans i8*)v->expectatum, n, v->vexilla,
            piscina);
        si (!_octeti_aequales(c, v->expectatum, n))
        {
            _ostendere("REDITUS FRACTUS", v->titulus, c);
        }
        CREDO_VERUM (_octeti_aequales(c, v->expectatum, n));
    }

    imprimere("\n--- IV. oraculum tabulae: claves B4b iterum ---\n");
    {
        chorda  fons    = filum_legere_totum(TABULA, piscina);
           Xar* tabula;
           i32  probati = ZEPHYRUM;

        intern = internamentum_creare(piscina);
        tabula = (fons.mensura > ZEPHYRUM)
            ? eventus_conformitas_legere(chorda_ut_cstr(fons, piscina),
                  piscina, intern)
            : NIHIL;
        CREDO_NON_NIHIL (tabula);
        per (i = ZEPHYRUM; tabula && i < xar_numerus(tabula); i++)
        {
            ConformitasScaena* s =
                (ConformitasScaena*)xar_obtinere(tabula,
                i);
                          i32 k;

            per (k = ZEPHYRUM; k < xar_numerus(s->terminales); k++)
            {
                ConformitasTerminalis* t = (ConformitasTerminalis*)
                    xar_obtinere(s->terminales, k);
                               chorda c;
                                  i32 vexilla;

                si (_mus_adest(t->octeti))
                {
                    perge;      /* mus: B6a-ii */
                }
                vexilla = chorda_aequalis_literis(t->profilum, "kitty")
                    ? OMNIA : ZEPHYRUM;
                c = _reditus(t->octeti.datum, t->octeti.mensura,
                    vexilla,
                    piscina);
                si (!chorda_aequalis(c, t->octeti))
                {
                    _ostendere("TABULA FRACTA",
                        chorda_ut_cstr(s->titulus, piscina), c);
                }
                CREDO_VERUM (chorda_aequalis(c, t->octeti));
                probati++;
            }
        }
        /* V scaenae clavium x II profila */
        CREDO_AEQUALIS_I32 (probati, X);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
