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
    e[0].datum.clavis.clavis  = v->clavis;
    e[0].datum.clavis.codex   = v->codex;
    e[0].datum.clavis.runa    = v->runa;
    e[0].datum.clavis.producta =
        (s32)(insignatus character)v->typus;
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

/* Onera visus (textus, viae, exempla) in piscinam copiare: visus usque
 * ad lectionem proximam vivunt */
interior vacuum
_onera_copiare (
     Eventus* e,
     Piscina* piscina)
{
    si (e->genus == EVENTUS_TEXTUS && e->datum.textus.contentum.mensura
        > ZEPHYRUM)
    {
        i8* nova = (i8*)piscina_allocare(piscina,
            (memoriae_index)e->datum.textus.contentum.mensura);

        memcpy(nova, e->datum.textus.contentum.datum,
            (memoriae_index)e->datum.textus.contentum.mensura);
        e->datum.textus.contentum.datum = nova;
    }
    si (e->genus == EVENTUS_DEPOSITIO && e->datum.depositio.viae.mensura
        > ZEPHYRUM)
    {
        i8* nova = (i8*)piscina_allocare(piscina,
            (memoriae_index)e->datum.depositio.viae.mensura);

        memcpy(nova, e->datum.depositio.viae.datum,
            (memoriae_index)e->datum.depositio.viae.mensura);
        e->datum.depositio.viae.datum = nova;
    }
    si (   e->genus == EVENTUS_MUS_MOTUS
        && e->datum.mus.numerus_exemplorum > ZEPHYRUM)
    {
         memoriae_index  n;
        EventusExemplum* nova;

        n = (memoriae_index)e->datum.mus.numerus_exemplorum
            * magnitudo(EventusExemplum);
        nova = (EventusExemplum*)piscina_allocare_ordinatum(piscina, n,
            VIII);
        memcpy(nova, e->datum.mus.exempla, n);
        e->datum.mus.exempla = nova;
    }
}

/* Octeti -> rivus (modi ut programma ex modis codificatoris) -> Eventus
 * -> codificator (eidem modi) -> octeti */
interior chorda
_reditus (
                 constans i8* octeti,
                         i32  mensura,
    constans CodificatorModi* modi,
                     Piscina* piscina)
{
       RivusTerminalis* r = rivus_creare(piscina,
           modi->cellula_latitudo,
           modi->cellula_altitudo);
     ChordaAedificator* a = chorda_aedificator_creare(piscina, LXIV);
                   Xar* eventa = xar_creare(piscina,
                       (i32)magnitudo(Eventus));
                    i8 m[RIVUS_MODI_MAXIMUM];
                   i32 declarati = RIVUS_MODUS_MUS;
               Eventus e;
                   i32 k;
                   i32 gyrus;

    si (modi->mus == CODIFICATOR_MUS_OMNIS)
    {
        declarati |= RIVUS_MODUS_SUPER;
    }
    si (modi->glutinum)
    {
        declarati |= RIVUS_MODUS_GLUTINUM;
    }
    si (modi->focus)
    {
        declarati |= RIVUS_MODUS_FOCUS;
    }
    si (modi->kitty_vexilla != ZEPHYRUM)
    {
        declarati |= RIVUS_MODUS_KITTY;
    }
    (vacuum)rivus_modos_intrare(r, declarati, m, RIVUS_MODI_MAXIMUM);
    (vacuum)rivus_tradere(r, octeti, mensura);
    per (gyrus = ZEPHYRUM; gyrus < II; gyrus++)
    {
        si (gyrus == I)
        {
            rivus_moram(r, M);
        }
        dum (rivus_eventum_coalitum(r, M, &e))
        {
            _onera_copiare(&e, piscina);
            *(Eventus*)xar_addere(eventa) = e;
        }
    }
    /* Xar SEGMENTATA est (segmenta non contigua): codificator tabulam
     * contiguam poscit (clavis + TEXTUS sequens) - copiatur */
    {
            i32  n       = xar_numerus(eventa);
        Eventus* tabula  = (Eventus*)piscina_allocare_ordinatum(piscina,
            (memoriae_index)(n + I) * magnitudo(Eventus), VIII);

        per (k = ZEPHYRUM; k < n; k++)
        {
            tabula[k] = *(Eventus*)xar_obtinere(eventa, k);
        }
        k = ZEPHYRUM;
        dum (k < n)
        {
            k += codificator_eventa(modi, tabula + k, n - k, a);
        }
    }
    redde chorda_aedificator_finire(a);
}

/* Modi legacy aut kitty, cellula 1x1 (claves: positio nulla) */
interior CodificatorModi
_modi_clavium (
    i32 vexilla)
{
    CodificatorModi m;

    memset(&m, ZEPHYRUM, magnitudo(CodificatorModi));
    m.kitty_vexilla     = vexilla;
    m.mus               = CODIFICATOR_MUS_TRACTUS;
    m.glutinum          = VERUM;
    m.cellula_latitudo  = I;
    m.cellula_altitudo  = I;
    redde m;
}

/* Eventum unum codificare */
interior chorda
_codificare (
    constans CodificatorModi* modi,
            constans Eventus* e,
                     Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, LXIV);

    (vacuum)codificator_eventa(modi, e, I, a);
    redde chorda_aedificator_finire(a);
}

interior Eventus
_murem (
    eventus_genus_t genus,
                s32 x,
                s32 y,
       mus_botton_t botton,
                i32 modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                   = genus;
    e.datum.mus.x             = x;
    e.datum.mus.y             = y;
    e.datum.mus.botton        = botton;
    e.datum.mus.modificantes  = modificantes;
    redde e;
}

interior Eventus
_rotulam (
    s32 dx,
    s32 dy)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus               = EVENTUS_MUS_ROTULA;
    e.datum.rotula.x      = XXV;
    e.datum.rotula.y      = L;
    e.datum.rotula.dx     = dx;
    e.datum.rotula.dy     = dy;
    e.datum.rotula.genus  = EVENTUS_ROTULA_GRADATA;
    redde e;
}

interior Eventus
_glutinum (
       eventus_genus_t  genus,
    constans character* t)
{
    Eventus e;
    unio { constans character* l; i8* m; } u;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    u.l      = t;
    e.genus  = genus;
    si (genus == EVENTUS_DEPOSITIO)
    {
        e.datum.depositio.viae.datum    = u.m;
        e.datum.depositio.viae.mensura  = (i32)strlen(t);
        e.datum.depositio.promota       = VERUM;
    }
    alioquin
    {
        e.datum.textus.contentum.datum    = u.m;
        e.datum.textus.contentum.mensura  = (i32)strlen(t);
        e.datum.textus.origo              = EVENTUS_ORIGO_GLUTINATA;
    }
    redde e;
}

interior b32
_aequat (
                 chorda  c,
     constans character* t)
{
    redde (b32)(   c.mensura == (i32)strlen(t)
                && memcmp(c.datum, t, (memoriae_index)c.mensura)
                   == ZEPHYRUM);
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
        CodificatorModi  modi_r;

        si (n == ZEPHYRUM)
        {
            perge;      /* inexpressibile: nihil decodificandum */
        }
        modi_r = _modi_clavium(v->vexilla);
        c = _reditus((constans i8*)v->expectatum, n, &modi_r, piscina);
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
                      CodificatorModi modi_t;

                vexilla = chorda_aequalis_literis(t->profilum, "kitty")
                    ? OMNIA : ZEPHYRUM;
                modi_t  = _modi_clavium(vexilla);
                c = _reditus(t->octeti.datum, t->octeti.mensura,
                    &modi_t,
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
        /* VIII scaenae x II profila (B6a-ii: et mus) */
        CREDO_AEQUALIS_I32 (probati, XVI);
    }

    imprimere("\n--- V. mus SGR (cellula X x XX, Modulus) ---\n");
    {
        CodificatorModi modi_muris;
                Eventus e;
                Eventus fl[II];
        EventusExemplum ex[III];

        memset(&modi_muris, ZEPHYRUM, magnitudo(CodificatorModi));
        modi_muris.cellula_latitudo  = X;
        modi_muris.cellula_altitudo  = XX;
        modi_muris.mus               = CODIFICATOR_MUS_PRESSIO;
        e = _murem(EVENTUS_MUS_DEPRESSUS, XXV, L, MUS_SINISTER,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<0;3;3M"));
        e.genus = EVENTUS_MUS_LIBERATUS;   /* SGR: botton servatur */
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<0;3;3m"));
        e = _murem(EVENTUS_MUS_DEPRESSUS, XXV, L, MUS_DEXTER,
            MOD_SHIFT | MOD_IMPERIUM);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<22;3;3M"));
        e = _murem(EVENTUS_MUS_DEPRESSUS, XXV, L, MUS_MEDIUS, MOD_ALT);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<9;3;3M"));
        /* motus: ?1000 nullus; ?1002 cum bottone; ?1003 etiam super */
        e = _murem(EVENTUS_MUS_MOTUS, XXV, L, (mus_botton_t)ZEPHYRUM,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            ""));
        modi_muris.mus = CODIFICATOR_MUS_TRACTUS;
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            ""));
        modi_muris.mus = CODIFICATOR_MUS_OMNIS;
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<35;3;3M"));
        modi_muris.mus      = CODIFICATOR_MUS_TRACTUS;
        e.datum.mus.botton  = MUS_SINISTER;
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<32;3;3M"));
        /* motus coalitus: exempla deinde positio; cellula repetita
         * omissa */
        ex[0].x = X;      ex[0].y = X;
        ex[1].x = XII;    ex[1].y = X;
        ex[2].x = XXXV;   ex[2].y = X;
        e = _murem(EVENTUS_MUS_MOTUS, LV, X, MUS_SINISTER, ZEPHYRUM);
        e.datum.mus.exempla = ex;
        e.datum.mus.numerus_exemplorum = III;
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<32;2;1M\033[<32;4;1M\033[<32;6;1M"));
        /* rota: gradus = cellula altitudo; dimidius nihil */
        e = _rotulam(ZEPHYRUM, XX);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<64;3;3M"));
        e = _rotulam(ZEPHYRUM, -XL);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<65;3;3M\033[<65;3;3M"));
        e = _rotulam(XX, ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<66;3;3M"));
        e = _rotulam(-XX, ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<67;3;3M"));
        e = _rotulam(ZEPHYRUM, V);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            ""));
        /* extra fenestram: pressio non, solutio ad marginem */
        e = _murem(EVENTUS_MUS_DEPRESSUS, -V, X, MUS_SINISTER,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            ""));
        e.genus = EVENTUS_MUS_LIBERATUS;
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[<0;1;1m"));
        /* mus non petitus */
        modi_muris.mus = CODIFICATOR_MUS_NULLUS;
        e = _murem(EVENTUS_MUS_DEPRESSUS, XXV, L, MUS_SINISTER,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            ""));

        imprimere("\n--- VI. focus (?1004) ---\n");
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus = EVENTUS_FOCUS;
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            ""));
        modi_muris.focus = VERUM;
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[I"));
        e.genus = EVENTUS_DEFOCUS;
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[O"));

        imprimere("\n--- VII. glutinum (paste.zig) ---\n");
        e = _glutinum(EVENTUS_TEXTUS, "a\nb");
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "a\rb"));
        modi_muris.glutinum  = VERUM;
        e                    = _glutinum(EVENTUS_TEXTUS, "hello");
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[200~hello\033[201~"));
        /* terminus intra onus numquam: ESC in spatium */
        e = _glutinum(EVENTUS_TEXTUS, "x\033[201~rm -rf");
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[200~x [201~rm -rf\033[201~"));
        e = _glutinum(EVENTUS_TEXTUS, "\003\177z\n");
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[200~  z\n\033[201~"));
        e = _glutinum(EVENTUS_TEXTUS, "");
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[200~\033[201~"));
        /* depositio promota: viae more conchae, spatiis iunctae */
        e = _glutinum(EVENTUS_DEPOSITIO, "/a b\n/c");
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\033[200~/a\\ b /c\033[201~"));
        /* textus scriptus solus (IME): octeti ipsi */
        e                     = _glutinum(EVENTUS_TEXTUS, "\303\251");
        e.datum.textus.origo  = EVENTUS_ORIGO_COMPOSITA;
        CREDO_VERUM (_aequat(_codificare(&modi_muris, &e, piscina),
            "\303\251"));
        fl[0] = e;
        fl[1] = e;
        CREDO_AEQUALIS_I32 (codificator_eventa(&modi_muris, fl, II,
            chorda_aedificator_creare(piscina, VIII)), I);
    }

    imprimere("\n--- VIII. reditus: mus, focus, glutinum ---\n");
    {
        constans character* octeti[] = {
            "\033[<0;3;3M\033[<0;3;3m",
            "\033[<22;3;3M\033[<9;7;2M",
            "\033[<0;3;3M\033[<32;4;3M\033[<32;6;3M\033[<0;6;3m",
            "\033[<64;3;3M\033[<65;3;3M\033[<66;3;3M\033[<67;3;3M",
            "\033[I\033[O",
            "\033[200~hello\033[201~\033[200~\033[201~"
        };
        CodificatorModi modi_reditus;
                    i32 j;

        memset(&modi_reditus, ZEPHYRUM, magnitudo(CodificatorModi));
        modi_reditus.cellula_latitudo  = X;
        modi_reditus.cellula_altitudo  = XX;
        modi_reditus.mus               = CODIFICATOR_MUS_TRACTUS;
        modi_reditus.glutinum          = VERUM;
        modi_reditus.focus             = VERUM;
        per (j = ZEPHYRUM; j < VI; j++)
        {
            chorda c = _reditus((constans i8*)octeti[j],
                (i32)strlen(octeti[j]), &modi_reditus, piscina);

            si (!_aequat(c, octeti[j]))
            {
                _ostendere("REDITUS FRACTUS", octeti[j], c);
            }
            CREDO_VERUM (_aequat(c, octeti[j]));
        }
    }

    imprimere("\n--- IX. modi D6: X10, formae, DECCKM, LNM ---\n");
    {
        CodificatorModi m;
                Eventus e;
                Eventus k[II];
        EventusExemplum ex[II];
                 Vector v;
                    i32 n;
                 chorda sine;
                 chorda cum;

        /* mouse_encode.zig (cellula 1 x 1: pixelum = cellula) */
        memset(&m, ZEPHYRUM, magnitudo(CodificatorModi));
        m.cellula_latitudo  = I;
        m.cellula_altitudo  = I;
        m.mus               = CODIFICATOR_MUS_X10;
        m.mus_forma         = CODIFICATOR_FORMA_X10;
        /* "x10 press left": modi omittuntur */
        e = _murem(EVENTUS_MUS_DEPRESSUS, ZEPHYRUM, ZEPHYRUM,
            MUS_SINISTER,
            MOD_SHIFT | MOD_ALT | MOD_IMPERIUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[M !!"));
        /* "x10 ignores release"; motus et rotula quoque nihil */
        e.genus = EVENTUS_MUS_LIBERATUS;
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina), ""));
        e = _murem(EVENTUS_MUS_MOTUS, ZEPHYRUM, ZEPHYRUM, MUS_SINISTER,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina), ""));
        e = _rotulam(ZEPHYRUM, I);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina), ""));
        /* "x10 coordinate limit": cellula CCXXIII nihil, CCXXII \xff */
        e = _murem(EVENTUS_MUS_DEPRESSUS, CCXXIII, ZEPHYRUM,
            MUS_SINISTER,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina), ""));
        e = _murem(EVENTUS_MUS_DEPRESSUS, CCXXII, ZEPHYRUM,
            MUS_SINISTER,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[M \377!"));
        /* forma X10 sub ?1000: solutio = III; rota 64 + 32 */
        m.mus = CODIFICATOR_MUS_PRESSIO;
        e = _murem(EVENTUS_MUS_LIBERATUS, ZEPHYRUM, ZEPHYRUM,
            MUS_SINISTER, ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[M#!!"));
        e = _rotulam(ZEPHYRUM, I);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[M`:S"));
        /* "urxvt with modifiers", "urxvt release uses legacy
         * button 3 encoding" */
        m.mus        = CODIFICATOR_MUS_OMNIS;
        m.mus_forma  = CODIFICATOR_FORMA_URXVT;
        e = _murem(EVENTUS_MUS_DEPRESSUS, II, III, MUS_SINISTER,
            MOD_SHIFT | MOD_ALT | MOD_IMPERIUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[60;3;4M"));
        e = _murem(EVENTUS_MUS_LIBERATUS, II, III, MUS_DEXTER,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[35;3;4M"));
        /* "utf8 encodes large coordinates": CCCXXXIII, CDXXXIII */
        m.mus_forma = CODIFICATOR_FORMA_UTF8;
        e = _murem(EVENTUS_MUS_DEPRESSUS, CCC, CD, MUS_SINISTER,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[M \305\215\306\261"));
        /* "sgr pixels ..." (cellula X x XX: pixela, non cellulae) */
        m.mus_forma         = CODIFICATOR_FORMA_SGR_PIXELA;
        m.cellula_latitudo  = X;
        m.cellula_altitudo  = XX;
        e = _murem(EVENTUS_MUS_DEPRESSUS, X, XX, MUS_SINISTER,
            ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[<0;10;20M"));
        e = _murem(EVENTUS_MUS_LIBERATUS, X, XX, MUS_DEXTER, ZEPHYRUM);
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[<2;10;20m"));
        /* motus intra cellulam unam: pixela omnia; SGR unum */
        ex[0].x = I;
        ex[0].y = I;
        ex[1].x = II;
        ex[1].y = I;
        e = _murem(EVENTUS_MUS_MOTUS, III, I, MUS_SINISTER, ZEPHYRUM);
        e.datum.mus.exempla = ex;
        e.datum.mus.numerus_exemplorum = II;
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[<32;1;1M\033[<32;2;1M\033[<32;3;1M"));
        m.mus_forma = CODIFICATOR_FORMA_SGR;
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "\033[<32;1;1M"));

        /* DECCKM (function_keys.zig cursorKey): sine modis SS3 */
        memset(&m, ZEPHYRUM, magnitudo(CodificatorModi));
        m.sagittae_applicationis = VERUM;
        memset(&v, ZEPHYRUM, magnitudo(Vector));
        v.actio   = P;
        v.clavis  = CLAVIS_SURSUM;
        v.codex   = EVENTUS_CODEX_SAGITTA_SURSUM;
        n         = _eventa(&v, k);
        CREDO_VERUM (_aequat(_codificare(&m, k, piscina), "\033OA"));
        v.clavis  = CLAVIS_DOMUS;
        v.codex   = EVENTUS_CODEX_DOMUS;
        n         = _eventa(&v, k);
        CREDO_VERUM (_aequat(_codificare(&m, k, piscina), "\033OH"));
        v.clavis  = CLAVIS_FINIS;
        v.codex   = EVENTUS_CODEX_FINIS;
        n         = _eventa(&v, k);
        CREDO_VERUM (_aequat(_codificare(&m, k, piscina), "\033OF"));
        /* cum modis CSI 1;m; Pagina ~ et F1 immutatae */
        v.clavis  = CLAVIS_SURSUM;
        v.codex   = EVENTUS_CODEX_SAGITTA_SURSUM;
        v.modi    = MOD_SHIFT;
        n         = _eventa(&v, k);
        CREDO_VERUM (_aequat(_codificare(&m, k, piscina), "\033[1;2A"));
        v.modi    = ZEPHYRUM;
        v.clavis  = CLAVIS_PAGINA_SURSUM;
        v.codex   = EVENTUS_CODEX_PAGINA_SURSUM;
        n         = _eventa(&v, k);
        CREDO_VERUM (_aequat(_codificare(&m, k, piscina), "\033[5~"));
        /* sine DECCKM: CSI A */
        m.sagittae_applicationis  = FALSUM;
        v.clavis                  = CLAVIS_SURSUM;
        v.codex                   = EVENTUS_CODEX_SAGITTA_SURSUM;
        n                         = _eventa(&v, k);
        CREDO_VERUM (_aequat(_codificare(&m, k, piscina), "\033[A"));
        /* kitty: DECCKM nihil mutat (solum legacy) */
        m.kitty_vexilla           = I;
        sine                      = _codificare(&m, k, piscina);
        m.sagittae_applicationis  = VERUM;
        cum                       = _codificare(&m, k, piscina);
        CREDO_VERUM (chorda_aequalis(sine, cum));
        (vacuum)n;

        /* LNM (Ghostty Exec.queueWrite): omne CR -> CR LF */
        memset(&m, ZEPHYRUM, magnitudo(CodificatorModi));
        m.lnm     = VERUM;
        v.clavis  = CLAVIS_REDITUS;
        v.codex   = EVENTUS_CODEX_REDITUS;
        v.typus   = '\r';
        n         = _eventa(&v, k);
        CREDO_VERUM (_aequat(_codificare(&m, k, piscina), "\r\n"));
        /* aedificator communis: solum octeti novi vertuntur */
        {
            ChordaAedificator* ae = chorda_aedificator_creare(piscina,
                VIII);

            (vacuum)codificator_eventa(&m, k, n, ae);
            (vacuum)codificator_eventa(&m, k, n, ae);
            CREDO_VERUM (_aequat(chorda_aedificator_finire(ae),
                "\r\n\r\n"));
        }
        e = _glutinum(EVENTUS_TEXTUS, "a\nb\rc");
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina),
            "a\r\nb\r\nc"));
        m.lnm = FALSUM;
        CREDO_VERUM (_aequat(_codificare(&m, &e, piscina), "a\rb\rc"));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
