/* probatio_interpres_terminalis.c - decodificator lexematum terminalis
 * in Eventus (eventus B2a, legacy): octeti per series_terminalis (modo
 * initus) et interpretem in caudam; eventa compendiose redduntur
 * ("KA:a" clavis A runa a, "Ta" textus, "Kup+C#ArrowUp", "DL@25,30"
 * pressio sinistra ad pixelum, "W0,20" rotula). Cellula X x XX. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "eventus.h"
#include "eventus_cauda.h"
#include "eventus_stml.h"
#include "series_terminalis.h"
#include "interpres_terminalis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_runam (
    ChordaAedificator* a,
                  s32  r)
{
    hic_manens constans character notae[] = "0123456789abcdef";
    /* s32: 'k >= 0' cum i32 (insignato) numquam falsum */
    s32 k;

    si (r > 0x20 && r < 0x7F)
    {
        chorda_aedificator_appendere_character(a, (character)r);
        redde;
    }
    chorda_aedificator_appendere_literis(a, "U+");
    per (k = XX; k >= ZEPHYRUM; k -= IV)
    {
        si ((r >> k) != ZEPHYRUM || k < VIII)
        {
            chorda_aedificator_appendere_character(a,
                notae[(r >> k) & 0xF]);
        }
    }
}

interior constans character*
_clavis_titulus (
    clavis_t c)
{
    commutatio (c)
    {
        casus CLAVIS_REDITUS:        redde "ret";
        casus CLAVIS_TABULA:         redde "tab";
        casus CLAVIS_RETRORSUM:      redde "bs";
        casus CLAVIS_EFFUGIUM:       redde "esc";
        casus CLAVIS_DELERE:         redde "del";
        casus CLAVIS_SPATIUM:        redde "spc";
        casus CLAVIS_SINISTER:       redde "left";
        casus CLAVIS_DEXTER:         redde "right";
        casus CLAVIS_SURSUM:         redde "up";
        casus CLAVIS_DEORSUM:        redde "down";
        casus CLAVIS_DOMUS:          redde "home";
        casus CLAVIS_FINIS:          redde "end";
        casus CLAVIS_PAGINA_SURSUM:  redde "pgup";
        casus CLAVIS_PAGINA_DEORSUM: redde "pgdn";
        casus CLAVIS_IGNOTA:         redde "?";
        ordinarius:                  redde NIHIL;
    }
}

interior vacuum
_modos (
    ChordaAedificator* a,
                  i32  m)
{
    si (m & MOD_SHIFT)
    { chorda_aedificator_appendere_literis(a, "+S");
    }
    si (m & MOD_ALT)
    { chorda_aedificator_appendere_literis(a, "+A");
    }
    si (m & MOD_IMPERIUM)
    { chorda_aedificator_appendere_literis(a, "+C");
    }
    si (m & MOD_SUPER)
    { chorda_aedificator_appendere_literis(a, "+M");
    }
}

interior vacuum
_eventum_reddere (
         ChordaAedificator* a,
          constans Eventus* e)
{
    constans character* t;
             character  botton;
                   i32  k;

    commutatio (e->genus)
    {
        casus EVENTUS_CLAVIS_DEPRESSUS:
            chorda_aedificator_appendere_character(a, 'K');
            t = _clavis_titulus(e->datum.clavis.clavis);
            si (t)
            {
                chorda_aedificator_appendere_literis(a, t);
            }
            alioquin si (   e->datum.clavis.clavis >= CLAVIS_F1
                         && e->datum.clavis.clavis <= CLAVIS_F12)
            {
                chorda_aedificator_appendere_character(a, 'f');
                chorda_aedificator_appendere_s32(a,
                    (s32)(e->datum.clavis.clavis - CLAVIS_F1) + I);
            }
            alioquin
            {
                _runam(a, (s32)e->datum.clavis.clavis);
            }
            si (e->datum.clavis.runa != ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, ':');
                _runam(a, e->datum.clavis.runa);
            }
            _modos(a, e->datum.clavis.modificantes);
            si (e->datum.clavis.codex != EVENTUS_CODEX_IGNOTUS)
            {
                chorda_aedificator_appendere_character(a, '#');
                chorda_aedificator_appendere_literis(a,
                    eventus_codex_titulus(e->datum.clavis.codex));
            }
            frange;
        casus EVENTUS_TEXTUS:
            chorda_aedificator_appendere_character(a,
                (e->datum.textus.origo == EVENTUS_ORIGO_GLUTINATA)
                    ? 'P' : 'T');
            per (k = ZEPHYRUM; k
                < e->datum.textus.contentum.mensura; k++)
            {
                i32 o =
                    ((i32)e->datum.textus.contentum.datum[k]) & 0xFF;

                si (o > 0x20 && o < 0x7F)
                {
                    chorda_aedificator_appendere_character(a,
                        (character)o);
                }
                alioquin
                {
                    chorda_aedificator_appendere_character(a, '%');
                    chorda_aedificator_appendere_s32(a, (s32)o);
                }
            }
            frange;
        casus EVENTUS_MUS_DEPRESSUS:
        casus EVENTUS_MUS_LIBERATUS:
        casus EVENTUS_MUS_MOTUS:
            chorda_aedificator_appendere_character(a,
                  (e->genus == EVENTUS_MUS_DEPRESSUS) ? 'D'
                : (e->genus == EVENTUS_MUS_LIBERATUS) ? 'U' : 'M');
            botton = (e->datum.mus.botton == MUS_SINISTER) ? 'L'
                   : (e->datum.mus.botton == MUS_DEXTER)   ? 'R'
                   : (e->datum.mus.botton == MUS_MEDIUS)   ? 'C' : '0';
            chorda_aedificator_appendere_character(a, botton);
            chorda_aedificator_appendere_character(a, '@');
            chorda_aedificator_appendere_s32(a, e->datum.mus.x);
            chorda_aedificator_appendere_character(a, ',');
            chorda_aedificator_appendere_s32(a, e->datum.mus.y);
            _modos(a, e->datum.mus.modificantes);
            frange;
        casus EVENTUS_MUS_ROTULA:
            chorda_aedificator_appendere_character(a, 'W');
            chorda_aedificator_appendere_s32(a, e->datum.rotula.dx);
            chorda_aedificator_appendere_character(a, ',');
            chorda_aedificator_appendere_s32(a, e->datum.rotula.dy);
            si (e->datum.rotula.genus == EVENTUS_ROTULA_GRADATA)
            {
                chorda_aedificator_appendere_character(a, 'g');
            }
            frange;
        casus EVENTUS_FOCUS:
            chorda_aedificator_appendere_literis(a, "F+");
            frange;
        casus EVENTUS_DEFOCUS:
            chorda_aedificator_appendere_literis(a, "F-");
            frange;
        ordinarius:
            chorda_aedificator_appendere_literis(a, "?");
            chorda_aedificator_appendere_literis(a,
                eventus_genus_titulus(e->genus));
            frange;
    }
}

nomen structura {
                Piscina* piscina;
           SeriesLector* lector;
    InterpresTerminalis  interpres;
           EventusCauda* cauda;
} Banca;

interior chorda
_cauda_reddere (
    Banca* b)
{
     ChordaAedificator* a;
               Eventus  e;
                   b32  primus = VERUM;

    a = chorda_aedificator_creare(b->piscina, CCLVI);
    dum (eventus_caudae_extrahere(b->cauda, &e))
    {
        si (!primus)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        primus = FALSUM;
        _eventum_reddere(a, &e);
    }
    eventus_cauda_lectio_incipit(b->cauda);
    redde chorda_aedificator_finire(a);
}

/* octeti per lexematorem et interpretem; mora: in fine series pendens
 * evacuatur (ut fons post moram) */
interior chorda
_decodere (
                  Banca* b,
     constans character* fons,
                    i32  mensura,
                    b32  mora)
{
       constans i8* ptr    = (constans i8*)fons;
       constans i8* finis  = ptr + mensura;
      SeriesLexema  l;
               s64  t   = M;

    dum (series_lexema_proximum(b->lector, &ptr, finis, &l)
        != SERIES_NIHIL)
    {
        (vacuum)interpres_lexema(&b->interpres, &l, FALSUM, t++,
            b->cauda);
    }
    si (mora && series_lectorem_evacuare(b->lector, &l))
    {
        (vacuum)interpres_lexema(&b->interpres, &l, VERUM, t++,
            b->cauda);
    }
    redde _cauda_reddere(b);
}

interior b32
_videre (
                  Banca* b,
     constans character* fons,
                    b32  mora,
     constans character* exspectatum)
{
    chorda r = _decodere(b, fons, (i32)strlen(fons), mora);

    si (!chorda_aequalis_literis(r, exspectatum))
    {
        imprimere("  exspectatum: %s\n  actuale:     %.*s\n",
            exspectatum,
            (int)r.mensura, (constans character*)r.datum);
        redde FALSUM;
    }
    redde VERUM;
}

s32 principale (vacuum)
{
    Piscina* piscina;
      Banca  b;

    piscina =
        piscina_generare_dynamicum("probatio_interpres_terminalis",
        M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    b.piscina  = piscina;
    b.lector   = series_lectorem_creare(piscina);
    series_lectorem_initus_ponere(b.lector, VERUM);
    interpres_initiare(&b.interpres, X, XX);
    b.cauda = (EventusCauda*)piscina_allocare_ordinatum(piscina,
        magnitudo(EventusCauda), VIII);
    CREDO_NON_NIHIL (b.cauda);
    eventus_caudam_initiare(b.cauda);

    imprimere("\n--- I. imprimibilia: clavis + textus ---\n");
    CREDO_VERUM (_videre(&b, "a", FALSUM, "KA:a Ta"));
    CREDO_VERUM (_videre(&b, "A", FALSUM, "KA:a TA"));    /* sine +S */
    CREDO_VERUM (_videre(&b, "ab", FALSUM, "KA:a Ta KB:b Tb"));
    CREDO_VERUM (_videre(&b, "5!", FALSUM, "K5:5 T5 K!:! T!"));
    CREDO_VERUM (_videre(&b, " ", FALSUM, "Kspc:U+20 T%32"));
    CREDO_VERUM (_videre(&b, "\xc3\xa9", FALSUM, "K?:U+e9 T%195%169"));

    imprimere("\n--- II. regimina honesta ---\n");
    CREDO_VERUM (_videre(&b, "\r", FALSUM, "Kret"));
    CREDO_VERUM (_videre(&b, "\t", FALSUM, "Ktab"));
    CREDO_VERUM (_videre(&b, "\x7f", FALSUM, "Kbs"));
    CREDO_VERUM (_videre(&b, "\n", FALSUM, "KJ:j+C"));
    CREDO_VERUM (_videre(&b, "\x08", FALSUM, "KH:h+C"));
    CREDO_VERUM (_videre(&b, "\x01\x1a", FALSUM, "KA:a+C KZ:z+C"));
    CREDO_VERUM (_videre(&b, "\x1c\x1f", FALSUM, "K\\:\\+C K_:_+C"));

    imprimere("\n--- III. alterum ---\n");
    CREDO_VERUM (_videre(&b, "\x1b" "a", FALSUM, "KA:a+A"));
    CREDO_VERUM (_videre(&b, "\x1b\r", FALSUM, "Kret+A"));
    CREDO_VERUM (_videre(&b, "\x1b\x7f", FALSUM, "Kbs+A"));
    CREDO_VERUM (_videre(&b, "\x1b\x01", FALSUM, "KA:a+A+C"));
    CREDO_VERUM (_videre(&b, "\x1b\xc3\xa9", FALSUM, "K?:U+e9+A"));
    CREDO_VERUM (_videre(&b, "\x1b ", FALSUM, "Kspc:U+20+A"));
    /* alterum non manet post clavem */
    CREDO_VERUM (_videre(&b, "\x1b\rx", FALSUM, "Kret+A KX:x Tx"));
    CREDO_VERUM (_videre(&b, "\x1b", VERUM, "Kesc"));
    CREDO_VERUM (_videre(&b, "\x1b\x1b", VERUM, "Kesc Kesc"));
    CREDO_VERUM (_videre(&b, "\x1b[", VERUM, "K[:[+A"));
    CREDO_VERUM (_videre(&b, "\x1bO", VERUM, "KO:o+A"));
    CREDO_VERUM (_videre(&b, "\x1b[1;5", VERUM, ""));

    imprimere("\n--- IV. claves nominatae (CSI, SS3) ---\n");
    CREDO_VERUM (_videre(&b, "\x1b[A", FALSUM, "Kup#ArrowUp"));
    CREDO_VERUM (_videre(&b, "\x1b[1;5A", FALSUM, "Kup+C#ArrowUp"));
    CREDO_VERUM (_videre(&b, "\x1b[1;2D", FALSUM, "Kleft+S#ArrowLeft"));
    CREDO_VERUM (_videre(&b, "\x1b[1;9C", FALSUM,
        "Kright+M#ArrowRight"));
    CREDO_VERUM (_videre(&b, "\x1b[H\x1b[F", FALSUM,
        "Khome#Home Kend#End"));
    CREDO_VERUM (_videre(&b, "\x1b[5~\x1b[6~", FALSUM,
        "Kpgup#PageUp Kpgdn#PageDown"));
    CREDO_VERUM (_videre(&b, "\x1b[3~", FALSUM, "Kdel#Delete"));
    CREDO_VERUM (_videre(&b, "\x1b[2~", FALSUM, "K?#Insert"));
    CREDO_VERUM (_videre(&b, "\x1b[15~", FALSUM, "Kf5#F5"));
    CREDO_VERUM (_videre(&b, "\x1b[24;3~", FALSUM, "Kf12+A#F12"));
    CREDO_VERUM (_videre(&b, "\x1b[Z", FALSUM, "Ktab+S#Tab"));
    CREDO_VERUM (_videre(&b, "\x1bOA\x1bOP", FALSUM,
        "Kup#ArrowUp Kf1#F1"));
    CREDO_VERUM (_videre(&b, "\x1bO2P", FALSUM, "Kf1+S#F1"));
    CREDO_VERUM (_videre(&b, "\x1b\x1b[A", FALSUM, "Kup+A#ArrowUp"));
    /* responsa et glutini termini: nihil */
    CREDO_VERUM (_videre(&b, "\x1b]11;rgb:0/0/0\x07x", FALSUM,
        "KX:x Tx"));
    CREDO_VERUM (_videre(&b, "\x1b[200~", FALSUM, ""));
    CREDO_VERUM (_videre(&b, "\x1b[?1;2c", FALSUM, ""));

    imprimere("\n--- V. mus SGR: centrum cellulae ---\n");
    CREDO_VERUM (_videre(&b, "\x1b[<0;3;2M", FALSUM, "DL@25,30"));
    CREDO_VERUM (_videre(&b, "\x1b[<0;3;2m", FALSUM, "UL@25,30"));
    CREDO_VERUM (_videre(&b, "\x1b[<2;1;1M", FALSUM, "DR@5,10"));
    CREDO_VERUM (_videre(&b, "\x1b[<1;1;1M", FALSUM, "DC@5,10"));
    CREDO_VERUM (_videre(&b, "\x1b[<32;4;2M", FALSUM, "ML@35,30"));
    CREDO_VERUM (_videre(&b, "\x1b[<35;4;2M", FALSUM, "M0@35,30"));
    /* motus coalitus in cauda (exempla) */
    CREDO_VERUM (_videre(&b, "\x1b[<35;4;2M\x1b[<35;5;2M", FALSUM,
        "M0@45,30"));
    CREDO_VERUM (_videre(&b, "\x1b[<64;1;1M", FALSUM, "W0,20g"));
    CREDO_VERUM (_videre(&b, "\x1b[<65;1;1M", FALSUM, "W0,-20g"));
    CREDO_VERUM (_videre(&b, "\x1b[<66;1;1M", FALSUM, "W20,0g"));
    CREDO_VERUM (_videre(&b, "\x1b[<67;1;1M", FALSUM, "W-20,0g"));
    CREDO_VERUM (_videre(&b, "\x1b[<4;1;1M\x1b[<8;1;1M\x1b[<16;1;1M",
        FALSUM, "DL@5,10+S DL@5,10+A DL@5,10+C"));

    imprimere("\n--- VI. focus, glutinum, X10 ---\n");
    CREDO_VERUM (_videre(&b, "\x1b[I\x1b[O", FALSUM, "F+ F-"));
    CREDO_AEQUALIS_I32 (interpres_glutinum(&b.interpres,
        (constans i8*)"hi\nthere", VIII, M, b.cauda), I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_cauda_reddere(&b), "Phi%10there");
    CREDO_AEQUALIS_I32 (interpres_x10(&b.interpres, XXXII, XXXIII + II,
        XXXIII + I, M, b.cauda), I);
    CREDO_AEQUALIS_I32 (interpres_x10(&b.interpres, XXXII + III,
        XXXIII + II, XXXIII + I, M, b.cauda), I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_cauda_reddere(&b),
        "DL@25,30 U0@25,30");

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
