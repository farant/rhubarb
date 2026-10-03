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
        casus CLAVIS_SINISTER_SHIFT: redde "lshift";
        casus CLAVIS_DEXTER_SHIFT:   redde "rshift";
        casus CLAVIS_CAPS_LOCK:      redde "caps";
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
    si (m & MOD_CAPS_LOCK)
    { chorda_aedificator_appendere_literis(a, "+L");
    }
    si (m & MOD_NUM_LOCK)
    { chorda_aedificator_appendere_literis(a, "+N");
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
        casus EVENTUS_CLAVIS_LIBERATUS:
            /* K pressa, K* iterata, k soluta (B2b) */
            chorda_aedificator_appendere_character(a,
                (e->genus == EVENTUS_CLAVIS_LIBERATUS) ? 'k' : 'K');
            si (e->datum.clavis.actio == EVENTUS_ACTIO_ITERATA)
            {
                chorda_aedificator_appendere_character(a, '*');
            }
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
            /* B3a: positio et modificantes rotulae */
            chorda_aedificator_appendere_character(a, '@');
            chorda_aedificator_appendere_s32(a, e->datum.rotula.x);
            chorda_aedificator_appendere_character(a, ',');
            chorda_aedificator_appendere_s32(a, e->datum.rotula.y);
            _modos(a, e->datum.rotula.modificantes);
            frange;
        casus EVENTUS_FOCUS:
            chorda_aedificator_appendere_literis(a, "F+");
            frange;
        casus EVENTUS_FACULTATES:
            chorda_aedificator_appendere_literis(a, "Fac:");
            si (e->datum.facultates.liberationes)
            {
                chorda_aedificator_appendere_character(a, 'L');
            }
            si (e->datum.facultates.codex_physicus)
            {
                chorda_aedificator_appendere_character(a, 'C');
            }
            si (e->datum.facultates.tabula_distincta)
            {
                chorda_aedificator_appendere_character(a, 'T');
            }
            si (e->datum.facultates.modificantes_textus)
            {
                chorda_aedificator_appendere_character(a, 'M');
            }
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

/* producta eventus primi (clavis) ex octetis; -1 si nullus (S3a) */
interior s32
_productam (
                  Banca* b,
     constans character* fons)
{
      constans i8* ptr    = (constans i8*)fons;
      constans i8* finis  = ptr + strlen(fons);
     SeriesLexema  l;
          Eventus  e;
              s32  t;

    dum (series_lexema_proximum(b->lector, &ptr, finis, &l)
        != SERIES_NIHIL)
    {
        (vacuum)interpres_lexema(&b->interpres, &l, FALSUM, M,
            b->cauda);
    }
    /* eventus clavis PRIMUS (kitty prima FACULTATES praemittit) */
    t = -I;
    dum (eventus_caudae_extrahere(b->cauda, &e))
    {
        si (   t == -I
            && (e.genus == EVENTUS_CLAVIS_DEPRESSUS
                || e.genus == EVENTUS_CLAVIS_LIBERATUS))
        {
            t = e.datum.clavis.producta;
        }
    }
    eventus_cauda_lectio_incipit(b->cauda);
    redde t;
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

    /* B3a: typus = character VERUS (ut fenestra: characters[0]) -
     * proiectio alt+A ab alt+a discernit */
    CREDO_VERUM (_productam(&b, "\x1b" "A") == 'A');
    CREDO_VERUM (_productam(&b, "\x1b" "a") == 'a');
    CREDO_VERUM (_productam(&b, "A") == 'A');
    CREDO_VERUM (_productam(&b, "?") == '?');
    /* kitty: typus = clavis ipsa (minuscula), sub Shift maiuscula
     * (B3a) */
    CREDO_VERUM (_productam(&b, "\x1b[97u") == 'a');
    CREDO_VERUM (_productam(&b, "\x1b[97:65;2u") == 'A');
    /* S3a: alterum + e acutum: producta plena (typus ASCII perdebat) */
    CREDO_VERUM (_productam(&b, "\x1b\303\251") == 0xE9);

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
    CREDO_VERUM (_videre(&b, "\x1b[<64;1;1M", FALSUM, "W0,20g@5,10"));
    CREDO_VERUM (_videre(&b, "\x1b[<65;1;1M", FALSUM, "W0,-20g@5,10"));
    CREDO_VERUM (_videre(&b, "\x1b[<66;1;1M", FALSUM, "W20,0g@5,10"));
    CREDO_VERUM (_videre(&b, "\x1b[<67;1;1M", FALSUM, "W-20,0g@5,10"));
    /* B3a: shift+rota cum positione; motus + rota tacite (tessera) */
    CREDO_VERUM (_videre(&b, "\x1b[<68;11;6M", FALSUM,
        "W0,20g@105,110+S"));
    CREDO_VERUM (_videre(&b, "\x1b[<96;1;1M\x1b[<97;1;1M", FALSUM, ""));
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

    imprimere("\n--- VI-bis. modifyOtherKeys CSI 27;m;c~ (B6a) ---\n");
    /* codificator legacy Enter/Tab/Escape modificatos sic mittit */
    CREDO_VERUM (_videre(&b, "\x1b[27;5;13~", FALSUM, "Kret+C"));
    CREDO_VERUM (_videre(&b, "\x1b[27;2;13~", FALSUM, "Kret+S"));
    CREDO_VERUM (_videre(&b, "\x1b[27;5;9~", FALSUM, "Ktab+C"));
    CREDO_VERUM (_videre(&b, "\x1b[27;6;27~", FALSUM, "Kesc+S+C"));
    CREDO_VERUM (_videre(&b, "\x1b[27;5;127~", FALSUM, "Kbs+C"));
    CREDO_VERUM (_videre(&b, "\x1b[27;5;97~", FALSUM, "KA:a+C"));

    imprimere("\n--- VII. kitty (B2b): vexilla 1|2|4|8|16 ---\n");
    interpres_initiare(&b.interpres, X, XX);
    b.interpres.kitty_vexilla = 0x1F;
    /* prima series kitty: facultates discuntur, ante clavem */
    CREDO_VERUM (_videre(&b, "\x1b[97u", FALSUM, "Fac:LCTM KA:a#KeyA"));
    CREDO_VERUM (_videre(&b, "\x1b[97u", FALSUM, "KA:a#KeyA"));
    CREDO_VERUM (_videre(&b, "\x1b[97;;97u", FALSUM, "KA:a#KeyA Ta"));
    CREDO_VERUM (_videre(&b, "\x1b[97:65;2;65u", FALSUM,
        "KA:a+S#KeyA TA"));
    CREDO_VERUM (_videre(&b, "\x1b[97;1:2;97u", FALSUM,
        "K*A:a#KeyA Ta"));
    CREDO_VERUM (_videre(&b, "\x1b[97;1:3u", FALSUM, "kA:a#KeyA"));
    /* Ctrl+I != Tab */
    CREDO_VERUM (_videre(&b, "\x1b[105;5u", FALSUM, "KI:i+C#KeyI"));
    CREDO_VERUM (_videre(&b, "\x1b[9u", FALSUM, "Ktab#Tab"));
    /* AZERTY 'q' in positione US 'a'; Cyrillica cum textu */
    CREDO_VERUM (_videre(&b, "\x1b[113::97u", FALSUM, "KQ:q#KeyA"));
    CREDO_VERUM (_videre(&b, "\x1b[1092::97;1;1092u", FALSUM,
        "K?:U+444#KeyA T%209%132"));
    /* formae legacy cum genere */
    CREDO_VERUM (_videre(&b, "\x1b[1;1:3A", FALSUM, "kup#ArrowUp"));
    CREDO_VERUM (_videre(&b, "\x1b[1;5:2A", FALSUM, "K*up+C#ArrowUp"));
    CREDO_VERUM (_videre(&b, "\x1b[P\x1b[1;2Q\x1b[13~", FALSUM,
        "Kf1#F1 Kf2+S#F2 Kf3#F3"));
    CREDO_VERUM (_videre(&b, "\x1b[12;5R", FALSUM, ""));   /* CPR */
    CREDO_VERUM (_videre(&b, "\x1b[27u\x1b[13u\x1b[127u", FALSUM,
        "Kesc#Escape Kret#Enter Kbs#Backspace"));
    CREDO_VERUM (_videre(&b, "\x1b[57441;2u\x1b[57441;1:3u", FALSUM,
        "Klshift+S#ShiftLeft klshift#ShiftLeft"));
    CREDO_VERUM (_videre(&b, "\x1b[97;65u", FALSUM, "KA:a+L#KeyA"));
    CREDO_VERUM (_videre(&b, "\x1b[57400u", FALSUM, "K1:1"));
    CREDO_VERUM (_videre(&b, "\x1b[?31u", FALSUM, ""));
    /* legacy post kitty: idem */
    CREDO_VERUM (_videre(&b, "\x1b[1;5A", FALSUM, "Kup+C#ArrowUp"));
    /* sine ALTERNAE et sine basi: codex nescitur; sine OMNES
     * modificantes textus non narrantur (B4) */
    interpres_initiare(&b.interpres, X, XX);
    b.interpres.kitty_vexilla = INTERPRES_KITTY_DISCERNERE;
    CREDO_VERUM (_videre(&b, "\x1b[97u", FALSUM, "Fac:T KA:a"));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
