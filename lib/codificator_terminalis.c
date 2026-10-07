/* codificator_terminalis.c - Vide codificator_terminalis.h
 *
 * Logica ex Ghostty (src/input/key_encode.zig, function_keys.zig,
 * kitty.zig; MIT, pin 12752b2) in vocabularium Eventus translata:
 * key = clavis (aut codex ubi clavis ignota, e.g. Insert), utf8 =
 * TEXTUS sequens (legacy: aut typus - interpres sub Ctrl/Alt textum
 * non emittit), unshifted_codepoint = runa. consumed_mods non habemus.
 */

#include "codificator_terminalis.h"
#include "interpres_terminalis.h"
#include "claves_physicae.h"
#include "utf8.h"
#include <string.h>

#define LIGANTES (MOD_SHIFT | MOD_ALT | MOD_IMPERIUM | MOD_SUPER)

/* Clavis functionalis kitty: numerus, finale ('u', '~', aut speciale
 * A-D H F P Q S), modificans */
nomen structura {
    s32 numerus;
     i8 finale;
    b32 modificans;
} Functionalis;

/* Textus clavis: TEXTUS sequens (si adest) */
nomen structura {
    constans i8* datum;
            i32  mensura;
} Textus;


/* ==================================================
 * Auxilia
 * ================================================== */

interior vacuum
_numerum (
    ChordaAedificator* a,
                  s32  n)
{
    chorda_aedificator_appendere_s32(a, n);
}

interior vacuum
_octetum (
    ChordaAedificator* a,
                   i8  b)
{
    chorda_aedificator_appendere_character(a, (character)b);
}

interior vacuum
_literas (
    ChordaAedificator* a,
   constans character* t)
{
    chorda_aedificator_appendere_literis(a, t);
}

interior vacuum
_textum (
    ChordaAedificator* a,
               Textus  t)
{
    i32 k;

    per (k = ZEPHYRUM; k < t.mensura; k++)
    {
        _octetum(a, t.datum[k]);
    }
}

/* Modificatores xterm/kitty: 1 + maiuscula I, alterum II, imperium IV,
 * super VIII (kitty etiam caps LXIV, num CXXVIII) */
interior s32
_modi_numerus (
    i32 modi,
    b32 serae)
{
    s32 n = ZEPHYRUM;

    si (modi & MOD_SHIFT)
    { n |= I;
    }
    si (modi & MOD_ALT)
    { n |= II;
    }
    si (modi & MOD_IMPERIUM)
    { n |= IV;
    }
    si (modi & MOD_SUPER)
    { n |= VIII;
    }
    si (serae && (modi & MOD_CAPS_LOCK))
    { n |= LXIV;
    }
    si (serae && (modi & MOD_NUM_LOCK))
    { n |= CXXVIII;
    }
    redde n + I;
}

interior b32
_regimen (
    s32 c)
{
    redde (b32)(c < XXXII || c == CXXVII);
}

/* Clavis functionalis (tabula kitty Ghostty, kitty.zig); FALSUM si
 * runa. Finale 'u', '~', aut speciale (A-D H F P Q: numerus I). */
interior b32
_functionalis (
    constans Eventus* e,
        Functionalis* f)
{
    hic_manens constans structura {
        clavis_t clavis;
             s32 numerus;
              i8 finale;
             b32 modificans;
    } TABULA[] = {
        { CLAVIS_EFFUGIUM,          XXVII,  'u', FALSUM },
        { CLAVIS_REDITUS,           XIII,   'u', FALSUM },
        { CLAVIS_TABULA,            IX,     'u', FALSUM },
        { CLAVIS_RETRORSUM,         CXXVII, 'u', FALSUM },
        { CLAVIS_DELERE,            III,    '~', FALSUM },
        { CLAVIS_SINISTER,          I,      'D', FALSUM },
        { CLAVIS_DEXTER,            I,      'C', FALSUM },
        { CLAVIS_SURSUM,            I,      'A', FALSUM },
        { CLAVIS_DEORSUM,           I,      'B', FALSUM },
        { CLAVIS_PAGINA_SURSUM,     V,      '~', FALSUM },
        { CLAVIS_PAGINA_DEORSUM,    VI,     '~', FALSUM },
        { CLAVIS_DOMUS,             I,      'H', FALSUM },
        { CLAVIS_FINIS,             I,      'F', FALSUM },
        { CLAVIS_F1,                I,      'P', FALSUM },
        { CLAVIS_F2,                I,      'Q', FALSUM },
        { CLAVIS_F3,                XIII,   '~', FALSUM },
        { CLAVIS_F4,                I,      'S', FALSUM },
        { CLAVIS_F5,                XV,     '~', FALSUM },
        { CLAVIS_F6,                XVII,   '~', FALSUM },
        { CLAVIS_F7,                XVIII,  '~', FALSUM },
        { CLAVIS_F8,                XIX,    '~', FALSUM },
        { CLAVIS_F9,                XX,     '~', FALSUM },
        { CLAVIS_F10,               XXI,    '~', FALSUM },
        { CLAVIS_F11,               XXIII,  '~', FALSUM },
        { CLAVIS_F12,               XXIV,   '~', FALSUM },
        { CLAVIS_CAPS_LOCK,         57358,  'u', VERUM  },
        { CLAVIS_NUM_LOCK,          57360,  'u', VERUM  },
        { CLAVIS_SINISTER_SHIFT,    57441,  'u', VERUM  },
        { CLAVIS_DEXTER_SHIFT,      57447,  'u', VERUM  },
        { CLAVIS_SINISTER_IMPERIUM, 57442,  'u', VERUM  },
        { CLAVIS_DEXTER_IMPERIUM,   57448,  'u', VERUM  },
        { CLAVIS_SINISTER_ALT,      57443,  'u', VERUM  },
        { CLAVIS_DEXTER_ALT,        57449,  'u', VERUM  },
        { CLAVIS_SINISTER_SUPER,    57444,  'u', VERUM  },
        { CLAVIS_DEXTER_SUPER,      57450,  'u', VERUM  }
    };
    s32 numerus = (s32)(magnitudo(TABULA) / magnitudo(TABULA[0]));
    s32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (TABULA[k].clavis == e->datum.clavis.clavis)
        {
            f->numerus     = TABULA[k].numerus;
            f->finale      = TABULA[k].finale;
            f->modificans  = TABULA[k].modificans;
            redde VERUM;
        }
    }
    /* Insert: clavis logica nulla in vocabulario, codex solus */
    si (e->datum.clavis.codex == EVENTUS_CODEX_INSERERE)
    {
        f->numerus     = II;
        f->finale      = '~';
        f->modificans  = FALSUM;
        redde VERUM;
    }
    redde FALSUM;
}

/* Forma specialis (finale A-D H F P Q S): numerus I */
interior b32
_specialis (
    i8 finale)
{
    redde (b32)(finale != 'u' && finale != '~');
}


/* ==================================================
 * kitty
 * ================================================== */

interior vacuum
_kitty (
    constans CodificatorModi* modi,
           constans Eventus* e,
                     Textus  textus,
          ChordaAedificator* a)
{
           i32 v      = modi->kitty_vexilla;
  EventusActio actio  = e->datum.clavis.actio;
           b32 solutio  = (b32)(e->genus == EVENTUS_CLAVIS_LIBERATUS
                             || actio == EVENTUS_ACTIO_SOLUTA);
           i32 ligantes  = e->datum.clavis.modificantes & LIGANTES;
      clavis_t c         = e->datum.clavis.clavis;
  Functionalis f;
           b32 habet;
           s32 mutata  = ZEPHYRUM;   /* clavis mutata (shift) */
           s32 basis   = ZEPHYRUM;   /* clavis dispositionis basicae */
           s32 m;
           s32 eventus  = ZEPHYRUM;   /* 0 nullum, 1 2 3 */
           b32 prior    = FALSUM;

    si (solutio)
    {
        si (!(v & INTERPRES_KITTY_GENERA))
        {
            redde;
        }
        si (   !(v & INTERPRES_KITTY_OMNES)
            && (c == CLAVIS_REDITUS || c == CLAVIS_RETRORSUM
                || c == CLAVIS_TABULA))
        {
            redde;
        }
        textus.mensura = ZEPHYRUM;   /* solutio textum non fert */
    }
    habet = _functionalis(e, &f);
    si (!habet && e->datum.clavis.runa > ZEPHYRUM)
    {
        f.numerus     = e->datum.clavis.runa;
        f.finale      = 'u';
        f.modificans  = FALSUM;
        habet         = VERUM;
    }

    /* praeparatio (sine OMNES): Enter/Tab/Backspace et textus planus ut
     * in legacy */
    si (!(v & INTERPRES_KITTY_OMNES))
    {
        si (ligantes == ZEPHYRUM)
        {
            si (c == CLAVIS_REDITUS)
            {
                _octetum(a, '\r');
                redde;
            }
            si (c == CLAVIS_TABULA)
            {
                _octetum(a, '\t');
                redde;
            }
            si (c == CLAVIS_RETRORSUM)
            {
                _octetum(a, (i8)0x7F);
                redde;
            }
        }
        si (   textus.mensura > ZEPHYRUM && ligantes == ZEPHYRUM
            && !solutio)
        {
            i32 k;
            b32 planus = VERUM;

            per (k = ZEPHYRUM; k < textus.mensura; k++)
            {
                si (_regimen(((s32)textus.datum[k]) & 0xFF))
                {
                    planus = FALSUM;
                }
            }
            si (planus)
            {
                _textum(a, textus);
                redde;
            }
        }
    }
    si (!habet)
    {
        si (!solutio && textus.mensura > ZEPHYRUM)
        {
            _textum(a, textus);
        }
        redde;
    }
    si (f.modificans && !(v & INTERPRES_KITTY_OMNES))
    {
        redde;
    }

    m = _modi_numerus(e->datum.clavis.modificantes, VERUM);
    si (v & INTERPRES_KITTY_GENERA)
    {
        eventus = solutio ? III
                : (actio == EVENTUS_ACTIO_ITERATA) ? II : I;
    }
    si ((v & INTERPRES_KITTY_ALTERNAE) && !_regimen(f.numerus))
    {
         constans i8* p       = textus.datum;
                 s32  primus  = -I;
                 b32  unus    = FALSUM;

        si (textus.mensura > ZEPHYRUM)
        {
            primus  = utf8_decodere(&p, textus.datum + textus.mensura);
            unus    = (b32)(p == textus.datum + textus.mensura);
        }
        basis = claves_littera_ex_codex(e->datum.clavis.codex);
        si (primus > ZEPHYRUM)
        {
            si (   primus != f.numerus
                && (e->datum.clavis.modificantes & MOD_SHIFT))
            {
                mutata = primus;
            }
            si (!(basis > ZEPHYRUM && basis != f.numerus
                  && primus != basis && unus))
            {
                basis = ZEPHYRUM;
            }
        }
        alioquin si (basis == f.numerus)
        {
            basis = ZEPHYRUM;
        }
    }
    /* textus associatus: non in solutione, non sub Ctrl/Alt/Super */
    si (   !(v & INTERPRES_KITTY_TEXTUS) || eventus == III
        || (e->datum.clavis.modificantes
            & (MOD_IMPERIUM | MOD_ALT | MOD_SUPER)))
    {
        textus.mensura = ZEPHYRUM;
    }

    _literas(a, "\033[");
    si (_specialis(f.finale))
    {
        si (eventus != ZEPHYRUM)
        {
            _literas(a, "1;");
            _numerum(a, m);
            _octetum(a, ':');
            _numerum(a, eventus);
        }
        alioquin si (m > I)
        {
            _literas(a, "1;");
            _numerum(a, m);
        }
        _octetum(a, f.finale);
        redde;
    }
    _numerum(a, f.numerus);
    si (mutata > ZEPHYRUM)
    {
        _octetum(a, ':');
        _numerum(a, mutata);
    }
    si (basis > ZEPHYRUM)
    {
        _literas(a, (mutata > ZEPHYRUM) ? ":" : "::");
        _numerum(a, basis);
    }
    si (eventus > I)
    {
        _octetum(a, ';');
        _numerum(a, m);
        _octetum(a, ':');
        _numerum(a, eventus);
        prior = VERUM;
    }
    alioquin si (m > I)
    {
        _octetum(a, ';');
        _numerum(a, m);
        prior = VERUM;
    }
    si (textus.mensura > ZEPHYRUM)
    {
         constans i8* p        = textus.datum;
         constans i8* finis    = textus.datum + textus.mensura;
                 i32  numerus  = ZEPHYRUM;

        dum (p < finis)
        {
            s32 cp = utf8_decodere(&p, finis);

            si (cp < ZEPHYRUM || _regimen(cp))
            {
                perge;
            }
            si (numerus == ZEPHYRUM)
            {
                _literas(a, prior ? ";" : ";;");
            }
            alioquin
            {
                _octetum(a, ':');
            }
            _numerum(a, cp);
            numerus++;
        }
    }
    _octetum(a, f.finale);
}


/* ==================================================
 * legacy (xterm 'PC-style', C0, alterum ut ESC)
 * ================================================== */

/* Ctrl + character -> C0 (tabula kitty per Ghostty); -1 nullum */
interior s32
_imperium_octetum (
    s32 c)
{
    si (c >= 'a' && c <= 'z' && c != 'i' && c != 'm')
    {
        redde c - 'a' + I;
    }
    commutatio (c)
    {
        casus ' ':  redde ZEPHYRUM;
        casus '/':  redde XXXI;
        casus '0':  redde XLVIII;
        casus '1':  redde XLIX;
        casus '2':  redde ZEPHYRUM;
        casus '3':  redde XXVII;
        casus '4':  redde XXVIII;
        casus '5':  redde XXIX;
        casus '6':  redde XXX;
        casus '7':  redde XXXI;
        casus '8':  redde CXXVII;
        casus '9':  redde LVII;
        casus '?':  redde CXXVII;
        casus '@':  redde ZEPHYRUM;
        casus '\\': redde XXVIII;
        casus ']':  redde XXIX;
        casus '^':  redde XXX;
        casus '_':  redde XXXI;
        casus '~':  redde XXX;
        ordinarius: redde -I;
    }
}

/* Ghostty ctrlSeq: -1 si non C0 */
interior s32
_seriem_imperii (
    constans Eventus* e,
              Textus  textus)
{
    i32 modi   = e->datum.clavis.modificantes;
    i32 reliqui;
    s32 c;

    si (!(modi & MOD_IMPERIUM))
    {
        redde -I;
    }
    reliqui = modi & (MOD_SHIFT | MOD_IMPERIUM | MOD_SUPER);
    si (textus.mensura == I)
    {
        c = ((s32)textus.datum[ZEPHYRUM]) & 0xFF;
    }
    alioquin si (   e->datum.clavis.runa > ZEPHYRUM
                 && e->datum.clavis.runa < CCLVI)
    {
        si (reliqui != MOD_IMPERIUM)
        {
            redde -I;
        }
        c = e->datum.clavis.runa;
    }
    alioquin
    {
        redde -I;
    }
    si ((reliqui & MOD_SHIFT) && (c < 'A' || c > 'Z') && c != '@')
    {
        reliqui &= ~(i32)MOD_SHIFT;
    }
    si (   c >= 'A' && c <= 'Z' && e->datum.clavis.runa > ZEPHYRUM
        && e->datum.clavis.runa < CCLVI)
    {
        c = e->datum.clavis.runa;
    }
    si (reliqui != MOD_IMPERIUM)
    {
        redde -I;
    }
    redde _imperium_octetum(c);
}

/* Claves PC-style (sagittae, Domus/Finis, '~', F1-F12). FALSUM si
 * clavis non talis. applicationis (DECCKM, D6): sagittae, Domus,
 * Finis sine modis SS3 (function_keys.zig cursorKey). */
interior b32
_pc (
     constans Eventus* e,
                  s32  m,
                  b32  applicationis,
    ChordaAedificator* a)
{
    Functionalis f;
        clavis_t c = e->datum.clavis.clavis;

    si (   c == CLAVIS_EFFUGIUM || c == CLAVIS_REDITUS
        || c == CLAVIS_TABULA
        || c == CLAVIS_RETRORSUM || !_functionalis(e, &f)
        || f.modificans)
    {
        redde FALSUM;
    }
    si (m <= I)
    {
        si (   f.finale == 'P' || f.finale == 'Q' || f.finale == 'S'
            || c        == CLAVIS_F3
            || (   applicationis
                && (   f.finale == 'A' || f.finale == 'B'
                    || f.finale == 'C' || f.finale == 'D'
                    || f.finale == 'H' || f.finale == 'F')))
        {
            _literas(a, "\033O");
            _octetum(a, (c == CLAVIS_F3) ? 'R' : f.finale);
            redde VERUM;
        }
        _literas(a, "\033[");
        si (f.finale == '~')
        {
            _numerum(a, f.numerus);
        }
        _octetum(a, f.finale);
        redde VERUM;
    }
    _literas(a, "\033[");
    _numerum(a, f.numerus);
    _octetum(a, ';');
    _numerum(a, m);
    _octetum(a, f.finale);
    redde VERUM;
}

/* xterm modifyOtherKeys: CSI 27 ; m ; c ~ */
interior vacuum
_alias (
    ChordaAedificator* a,
                  s32  m,
                  s32  c)
{
    _literas(a, "\033[27;");
    _numerum(a, m);
    _octetum(a, ';');
    _numerum(a, c);
    _octetum(a, '~');
}

interior vacuum
_vetustum (
    constans CodificatorModi* cm,
           constans Eventus* e,
                     Textus  textus,
          ChordaAedificator* a)
{
           i32 modi      = e->datum.clavis.modificantes;
           i32 ligantes  = modi & LIGANTES;
      clavis_t c         = e->datum.clavis.clavis;
           s32 m         = _modi_numerus(ligantes, FALSUM);
           b32 alterum   = (b32)((ligantes & MOD_ALT) != ZEPHYRUM);
           s32 octetus;
            i8 producta[IV];

    si (   e->genus              == EVENTUS_CLAVIS_LIBERATUS
        || e->datum.clavis.actio == EVENTUS_ACTIO_SOLUTA)
    {
        redde;      /* legacy solutiones non narrat */
    }
    /* textus: TEXTUS sequens, aut producta (sub Ctrl/Alt textus deest;
     * S3a: Unicode plena, olim typus ASCII) */
    si (   textus.mensura           == ZEPHYRUM
        && e->datum.clavis.producta >= 0x20
        && e->datum.clavis.producta != 0x7F)
    {
        textus.datum    = producta;
        textus.mensura  = (i32)utf8_codere(e->datum.clavis.producta,
            producta);
    }
    si (_pc(e, m, cm->sagittae_applicationis, a))
    {
        redde;
    }
    si (c == CLAVIS_TABULA)
    {
        si (ligantes == ZEPHYRUM)
        { _octetum(a, '\t');
        }
        alioquin si (ligantes == MOD_SHIFT)
        { _literas(a, "\033[Z");
        }
        alioquin si (ligantes == MOD_ALT)
        { _literas(a, "\033\t");
        }
        alioquin
        { _alias(a, m, IX);
        }
        redde;
    }
    si (c == CLAVIS_REDITUS)
    {
        si (ligantes == ZEPHYRUM)
        { _octetum(a, '\r');
        }
        alioquin si (ligantes == MOD_ALT)
        { _literas(a, "\033\r");
        }
        alioquin
        { _alias(a, m, XIII);
        }
        redde;
    }
    si (c == CLAVIS_EFFUGIUM)
    {
        si (ligantes == ZEPHYRUM)
        { _octetum(a, (i8)0x1B);
        }
        alioquin si (ligantes == MOD_ALT)
        { _literas(a, "\033\033");
        }
        alioquin
        { _alias(a, m, XXVII);
        }
        redde;
    }
    si (c == CLAVIS_RETRORSUM)
    {
        si (alterum)
        { _octetum(a, (i8)0x1B);
        }
        _octetum(a, (ligantes & MOD_IMPERIUM) ? (i8)0x08 : (i8)0x7F);
        redde;
    }
    octetus = _seriem_imperii(e, textus);
    si (octetus >= ZEPHYRUM)
    {
        si (alterum)
        { _octetum(a, (i8)0x1B);
        }
        _octetum(a, (i8)octetus);
        redde;
    }
    si (textus.mensura == ZEPHYRUM)
    {
        /* alterum sine textu: runa (macOS: unshifted) */
        si (alterum && e->datum.clavis.runa > ZEPHYRUM)
        {
            i8 b[IV];

            _octetum(a, (i8)0x1B);
            textus.datum    = b;
            textus.mensura  = (i32)utf8_codere(e->datum.clavis.runa, b);
            _textum(a, textus);
        }
        redde;
    }
    si (modi & MOD_IMPERIUM)
    {
        /* fixterms CSI u (Ctrl+I, Ctrl+M, ...): Shift solum si
         * character idem ac runa */
         constans i8* p = textus.datum;
                 s32  cp = utf8_decodere(&p, textus.datum
                     + textus.mensura);
                 i32 u = modi & (MOD_SHIFT | MOD_ALT | MOD_IMPERIUM);

        si (cp >= 'A' && cp <= 'Z' && (u & MOD_SHIFT))
        {
            cp = cp - 'A' + 'a';
        }
        si (e->datum.clavis.runa != cp)
        {
            u &= ~(i32)MOD_SHIFT;
        }
        _literas(a, "\033[");
        _numerum(a, cp);
        _octetum(a, ';');
        _numerum(a, _modi_numerus(u, FALSUM));
        _octetum(a, 'u');
        redde;
    }
    si (alterum)
    {
        _octetum(a, (i8)0x1B);
        si (textus.mensura == I || e->datum.clavis.runa <= ZEPHYRUM)
        {
            _textum(a, textus);
        }
        alioquin
        {
            i8 b[IV];

            textus.datum    = b;
            textus.mensura  = (i32)utf8_codere(e->datum.clavis.runa, b);
            _textum(a, textus);
        }
        redde;
    }
    si (modi & MOD_SUPER)
    {
        redde;      /* macOS: Command + clavis textum non mittit */
    }
    _textum(a, textus);
}


/* ==================================================
 * Mus SGR (mouse_encode.zig), focus, glutinum (paste.zig)
 * ================================================== */

/* Pixelum nostrum -> cellula 0-basata (PAVIMENTUM: Modulus) */
interior s32
_cellula (
    s32 pixelum,
    s32 magnitudo_cellulae)
{
    s32 m = (magnitudo_cellulae > ZEPHYRUM) ? magnitudo_cellulae : I;

    si (pixelum >= ZEPHYRUM)
    {
        redde pixelum / m;
    }
    redde -((-pixelum + m - I) / m);
}

/* Relatio una per formam (mouse_encode.zig encode): SGR CSI < codex ;
 * columna ; linea M|m; SGR-pixela idem cum pixelis; formae veteres
 * (X10, UTF-8, urxvt) solutionem ut botton III narrant; X10 cellulas
 * ultra CCXXII non fert (nihil) */
interior vacuum
_relatio (
    constans CodificatorModi* modi,
           ChordaAedificator* a,
                         s32  codex,
                         s32  columna,
                         s32  linea,
                         s32  x,
                         s32  y,
                         b32  solutio)
{
     i8 b[IV];
    i32 n;
    i32 k;

    si (   solutio && modi->mus_forma != CODIFICATOR_FORMA_SGR
        && modi->mus_forma != CODIFICATOR_FORMA_SGR_PIXELA)
    {
        codex = (codex & ~(s32)III) | III;
    }
    commutatio (modi->mus_forma)
    {
        casus CODIFICATOR_FORMA_X10:
            si (columna > CCXXII || linea > CCXXII)
            {
                redde;
            }
            _literas(a, "\033[M");
            _octetum(a, (i8)(XXXII + codex));
            _octetum(a, (i8)(XXXIII + columna));
            _octetum(a, (i8)(XXXIII + linea));
            redde;
        casus CODIFICATOR_FORMA_UTF8:
            _literas(a, "\033[M");
            _octetum(a, (i8)(XXXII + codex));
            n = (i32)utf8_codere(columna + XXXIII, b);
            per (k = ZEPHYRUM; k < n; k++)
            {
                _octetum(a, b[k]);
            }
            n = (i32)utf8_codere(linea + XXXIII, b);
            per (k = ZEPHYRUM; k < n; k++)
            {
                _octetum(a, b[k]);
            }
            redde;
        casus CODIFICATOR_FORMA_URXVT:
            _literas(a, "\033[");
            _numerum(a, XXXII + codex);
            _octetum(a, ';');
            _numerum(a, columna + I);
            _octetum(a, ';');
            _numerum(a, linea + I);
            _octetum(a, 'M');
            redde;
        casus CODIFICATOR_FORMA_SGR_PIXELA:
            columna  = x - I;
            linea    = y - I;
            frange;
        ordinarius:
            frange;
    }
    _literas(a, "\033[<");
    _numerum(a, codex);
    _octetum(a, ';');
    _numerum(a, columna + I);
    _octetum(a, ';');
    _numerum(a, linea + I);
    _octetum(a, solutio ? 'm' : 'M');
}

interior s32
_modi_muris (
    i32 modi)
{
    s32 n = ZEPHYRUM;

    si (modi & MOD_SHIFT)
    { n += IV;
    }
    si (modi & MOD_ALT)
    { n += VIII;
    }
    si (modi & MOD_IMPERIUM)
    { n += XVI;
    }
    redde n;
}

/* Positio una: extra fenestram (negativa) solum solutio aut tractus in
 * modo motus refertur, ad marginem; cellula priori aequalis
 * omittitur */
interior vacuum
_positionem (
    constans CodificatorModi* modi,
                         s32  x,
                         s32  y,
                         s32  codex,
                         b32  solutio,
                         b32  tractus,
                         s32* prior_c,
                         s32* prior_l,
           ChordaAedificator* a)
{
    s32 c = _cellula(x, modi->cellula_latitudo);
    s32 l = _cellula(y, modi->cellula_altitudo);
    b32 pixela = (b32)(modi->mus_forma == CODIFICATOR_FORMA_SGR_PIXELA);

    si ((c < ZEPHYRUM || l < ZEPHYRUM) && !solutio && !tractus)
    {
        redde;
    }
    si (c < ZEPHYRUM)
    { c = ZEPHYRUM;
    }
    si (l < ZEPHYRUM)
    { l = ZEPHYRUM;
    }
    /* repetitio omissa: cellula, aut pixelum in forma pixelorum */
    si (pixela ? (x == *prior_c && y == *prior_l)
               : (c == *prior_c && l == *prior_l))
    {
        redde;
    }
    *prior_c = pixela ? x : c;
    *prior_l = pixela ? y : l;
    _relatio(modi, a, codex, c, l, x, y, solutio);
}

interior vacuum
_murem (
    constans CodificatorModi* modi,
            constans Eventus* e,
           ChordaAedificator* a)
{
    b32 motus    = (b32)(e->genus == EVENTUS_MUS_MOTUS);
    b32 solutio  = (b32)(e->genus == EVENTUS_MUS_LIBERATUS);
    s32 codex;
    s32 prior_c = -I;
    s32 prior_l = -I;
    i32 k;

    si (modi->mus == CODIFICATOR_MUS_NULLUS)
    {
        redde;
    }
    /* X10 (?9): pressio sola sinistri, medii, dextri; sine modis */
    si (   modi->mus == CODIFICATOR_MUS_X10
        && e->genus  != EVENTUS_MUS_DEPRESSUS)
    {
        redde;
    }
    commutatio (e->datum.mus.botton)
    {
        casus MUS_SINISTER: codex = ZEPHYRUM;  frange;
        casus MUS_MEDIUS:   codex = I;         frange;
        casus MUS_DEXTER:   codex = II;        frange;
        ordinarius:
            si (!motus)
            {
                redde;      /* botton ignotus */
            }
            codex = III;    /* motus sine bottone */
            frange;
    }
    si (motus)
    {
        si (modi->mus == CODIFICATOR_MUS_PRESSIO)
        {
            redde;
        }
        si (modi->mus == CODIFICATOR_MUS_TRACTUS && codex == III)
        {
            redde;
        }
        codex += XXXII;
    }
    si (modi->mus != CODIFICATOR_MUS_X10)
    {
        codex += _modi_muris(e->datum.mus.modificantes);
    }
    si (motus)
    {
        /* motus coalitus: exempla, deinde positio ultima */
        per (k = ZEPHYRUM; k < e->datum.mus.numerus_exemplorum; k++)
        {
            _positionem(modi, e->datum.mus.exempla[k].x,
                e->datum.mus.exempla[k].y, codex, FALSUM,
                (b32)((codex & III) != III), &prior_c, &prior_l, a);
        }
    }
    _positionem(modi, e->datum.mus.x, e->datum.mus.y, codex, solutio,
        (b32)(motus && (codex & III) != III), &prior_c, &prior_l, a);
}

interior vacuum
_rotulam (
    constans CodificatorModi* modi,
            constans Eventus* e,
           ChordaAedificator* a)
{
    s32 gradus = (modi->cellula_altitudo > ZEPHYRUM)
        ? modi->cellula_altitudo : I;
    s32 modi_m = _modi_muris(e->datum.rotula.modificantes);
    s32 n;
    s32 k;
    s32 prior_c;
    s32 prior_l;

    si (   modi->mus == CODIFICATOR_MUS_NULLUS
        || modi->mus == CODIFICATOR_MUS_X10)
    {
        redde;
    }
    /* gradus integri (versus nihil); fractio non refertur */
    n = e->datum.rotula.dy / gradus;
    per (k = ZEPHYRUM; k < ((n < ZEPHYRUM) ? -n : n); k++)
    {
        prior_c = -I;
        prior_l = -I;
        _positionem(modi, e->datum.rotula.x, e->datum.rotula.y,
            ((n > ZEPHYRUM) ? LXIV : LXV) + modi_m, FALSUM, FALSUM,
            &prior_c, &prior_l, a);
    }
    n = e->datum.rotula.dx / gradus;
    per (k = ZEPHYRUM; k < ((n < ZEPHYRUM) ? -n : n); k++)
    {
        prior_c = -I;
        prior_l = -I;
        _positionem(modi, e->datum.rotula.x, e->datum.rotula.y,
            ((n > ZEPHYRUM) ? LXVI : LXVII) + modi_m, FALSUM, FALSUM,
            &prior_c, &prior_l, a);
    }
}

/* Octetus quem glutinum non fert (paste.zig: NUL BS ENQ EOT ESC DEL et
 * signa conchae) - in spatium; ergo terminus 201~ intra onus numquam */
interior b32
_infidus (
    i8 b)
{
    commutatio (((s32)b) & 0xFF)
    {
        casus 0x00: casus 0x08: casus 0x05: casus 0x04: casus 0x1B:
        casus 0x7F: casus 0x03: casus 0x1C: casus 0x15: casus 0x1A:
        casus 0x11: casus 0x13: casus 0x17: casus 0x16: casus 0x12:
        casus 0x0F:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* Octetus oneris glutini: infidus -> spatium; sine ?2004 '\n' fit
 * '\r' */
interior vacuum
_glutini_octetum (
    constans CodificatorModi* modi,
                          i8  b,
           ChordaAedificator* a)
{
    si (_infidus(b))
    {
        b = ' ';
    }
    alioquin si (b == '\n' && !modi->glutinum)
    {
        b = '\r';
    }
    _octetum(a, b);
}

interior vacuum
_glutinare (
     constans CodificatorModi* modi,
                       Textus  t,
            ChordaAedificator* a)
{
    i32 k;

    si (modi->glutinum)
    {
        _literas(a, "\033[200~");
    }
    per (k = ZEPHYRUM; k < t.mensura; k++)
    {
        _glutini_octetum(modi, t.datum[k], a);
    }
    si (modi->glutinum)
    {
        _literas(a, "\033[201~");
    }
}

/* Character conchae tutus sine effugio */
interior b32
_tutus_conchae (
    i8 b)
{
    s32 c = ((s32)b) & 0xFF;

    redde (b32)(   (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                || (c >= '0' && c <= '9') || c >= 0x80 || c == '/'
                || c == '.' || c == '_' || c == '-' || c == '+'
                || c == ',' || c == ':' || c == '@' || c == '%'
                || c == '=');
}

/* Depositio promota: viae more conchae effugitae, spatiis iunctae (ut
 * terminalis viam depositam glutinat) */
interior vacuum
_depositionem (
    constans CodificatorModi* modi,
            constans Eventus* e,
           ChordaAedificator* a)
{
    i32 k;

    si (modi->glutinum)
    {
        _literas(a, "\033[200~");
    }
    per (k = ZEPHYRUM; k < e->datum.depositio.viae.mensura; k++)
    {
        i8 b = e->datum.depositio.viae.datum[k];

        si (b == '\n')
        {
            _octetum(a, ' ');     /* viae spatiis iunctae */
            perge;
        }
        si (!_tutus_conchae(b))
        {
            _octetum(a, '\\');
        }
        _glutini_octetum(modi, b, a);
    }
    si (modi->glutinum)
    {
        _literas(a, "\033[201~");
    }
}


/* ==================================================
 * Publica
 * ================================================== */

/* Eventum unum (et TEXTUS sequens) codificare; numerus consumptus */
interior i32
_codificare (
    constans CodificatorModi* modi,
            constans Eventus* eventa,
                         i32  numerus,
           ChordaAedificator* aedificator)
{
     constans Eventus* e;
               Textus  textus;
                  i32  consumpta = I;

    si (numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    e = &eventa[ZEPHYRUM];
    commutatio (e->genus)
    {
        casus EVENTUS_CLAVIS_DEPRESSUS:
        casus EVENTUS_CLAVIS_LIBERATUS:
            frange;
        casus EVENTUS_MUS_DEPRESSUS:
        casus EVENTUS_MUS_LIBERATUS:
        casus EVENTUS_MUS_MOTUS:
            _murem(modi, e, aedificator);
            redde I;
        casus EVENTUS_MUS_ROTULA:
            _rotulam(modi, e, aedificator);
            redde I;
        casus EVENTUS_FOCUS:
        casus EVENTUS_DEFOCUS:
            si (modi->focus)
            {
                _literas(aedificator, (e->genus == EVENTUS_FOCUS)
                    ? "\033[I" : "\033[O");
            }
            redde I;
        casus EVENTUS_TEXTUS:
            textus.datum    = e->datum.textus.contentum.datum;
            textus.mensura  = e->datum.textus.contentum.mensura;
            si (e->datum.textus.origo == EVENTUS_ORIGO_GLUTINATA)
            {
                _glutinare(modi, textus, aedificator);
            }
            alioquin si (e->datum.textus.genus
                         == EVENTUS_TEXTUS_COMMISSUM)
            {
                _textum(aedificator, textus);   /* IME, sine clave */
            }
            redde I;
        casus EVENTUS_DEPOSITIO:
            si (e->datum.depositio.promota)
            {
                _depositionem(modi, e, aedificator);
            }
            redde I;
        ordinarius:
            redde I;    /* facultates, magnitudo, ...: non octeti */
    }
    textus.datum    = NIHIL;
    textus.mensura  = ZEPHYRUM;
    si (   e->genus == EVENTUS_CLAVIS_DEPRESSUS && numerus > I
        && eventa[I].genus == EVENTUS_TEXTUS
        && eventa[I].datum.textus.genus == EVENTUS_TEXTUS_COMMISSUM
        && eventa[I].datum.textus.origo == EVENTUS_ORIGO_SCRIPTA)
    {
        textus.datum    = eventa[I].datum.textus.contentum.datum;
        textus.mensura  = eventa[I].datum.textus.contentum.mensura;
        consumpta       = II;
    }
    si (modi->kitty_vexilla != ZEPHYRUM)
    {
        _kitty(modi, e, textus, aedificator);
    }
    alioquin
    {
        _vetustum(modi, e, textus, aedificator);
    }
    redde consumpta;
}

/* LNM (Ghostty Exec.queueWrite): omne CR ab initio emissum -> CR LF,
 * in loco (aedificator auctus, deinde retrorsum translatus) */
interior vacuum
_lineas_novas (
    ChordaAedificator* a,
                  i32  initium)
{
    chorda v;
       i32 finis;
       i32 numerus;
       i32 k;
       i32 d;

    v        = chorda_aedificator_spectare(a);
    finis    = v.mensura;
    numerus  = ZEPHYRUM;
    per (k = initium; k < finis; k++)
    {
        si (v.datum[k] == '\r')
        {
            numerus++;
        }
    }
    si (numerus == ZEPHYRUM)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        _octetum(a, '\n');
    }
    v = chorda_aedificator_spectare(a);
    d = finis + numerus;
    per (k = finis; k > initium; k--)
    {
        si (v.datum[k - I] == '\r')
        {
            v.datum[--d] = '\n';
        }
        v.datum[--d] = v.datum[k - I];
    }
}

i32
codificator_eventa (
    constans CodificatorModi* modi,
            constans Eventus* eventa,
                         i32  numerus,
           ChordaAedificator* aedificator)
{
    i32 initium;
    i32 consumpta;

    initium    = (i32)chorda_aedificator_longitudo(aedificator);
    consumpta  = _codificare(modi, eventa, numerus, aedificator);
    si (modi->lnm)
    {
        _lineas_novas(aedificator, initium);
    }
    redde consumpta;
}
