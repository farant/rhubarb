/* gabc_carmen.c - conversor: cantus gabc -> textus carminis
 * (caput include/gabc_carmen.h; spec par. III, plan T7)
 *
 * ITER: (1) notae carminis per syllabas (_notas_colligere): longitudo
 * regulis RHYTHMUS in sextis decimis, unisoni ligati, fines incisorum
 * cum spiritu, verba syllabatim; (2) altitudo: sonus a do, do
 * absolutum, orthographia ex scala do; (3) clavis ex finali et
 * collectione; (4) commentarium, carmen, textus MML, nexus.
 * Strictus: approximatio prima conversionem sistit, exitus intactus.
 */

#include "gabc_carmen.h"
#include "utf8.h"
#include "xar.h"
#include <string.h>

#define PULSUS_SEXTAE  ((s64)(CMLX / IV))

/* spiritus post incisum, in sextis decimis */
#define SPIRITUS_MINOR  I
#define SPIRITUS_MAIOR  II


/* ==================================================
 * Notae carminis et conversor
 * ================================================== */

nomen structura {
    s32 sonus;          /* semitonia a do */
    s32 gradus;
    s32 alteratio;
    s32 longitudo;      /* sextae decimae */
    i32 nota;           /* nota gabc prima */
    i32 numerus;        /* notae gabc ligatae */
    i32 syllaba;
    i32 elementum;
    i32 linea;
    i32 columna;
    s32 spiritus;       /* post notam: 0, SPIRITUS_MINOR, _MAIOR */
    b32 finis;          /* incisum post notam finitur */
    s32 midi;
    s32 littera;        /* 0 c .. 6 b */
    s32 signum;         /* -1 0 +1 */
    s32 octava;
} NotaCarminis;

nomen structura {
                     Piscina* piscina;
         constans GabcCantus* cantus;
    constans GabcCarmenForma* forma;
                         Xar* approximationes;
                         Xar* structurae;
                         Xar* notae;           /* NotaCarminis */
                         i32  signa[GABC_CARMEN_SIGNORUM_GENERA];
                         b32  fractus;         /* strictus et
                                               * approximatio */
                        b32 memoria_defecit;
                        i32 syllabae_purgatae;
                        i32 enharmonicae;
} Conversor;

interior vacuum
_approximatio (
                Conversor* cv,
    GabcApproximatioGenus  genus,
                      s32  syllaba,
                      i32  linea,
                      i32  columna,
       constans character* descriptio)
{
    GabcApproximatio* a;

    si (cv->fractus)
    {
        redde;
    }
    a = (GabcApproximatio*)xar_addere(cv->approximationes);
    si (a == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde;
    }
    a->genus       = genus;
    a->syllaba     = syllaba;
    a->linea       = linea;
    a->columna     = columna;
    a->descriptio  = chorda_ex_literis(descriptio, cv->piscina);
    si (cv->forma->strictus)
    {
        cv->fractus = VERUM;
    }
}

interior vacuum
_structura (
              Conversor* cv,
     constans character* textus,
                 chorda  adiectum)
{
               chorda* s;
    ChordaAedificator* a;

    a = chorda_aedificator_creare(cv->piscina, CXXVIII);
    s = (chorda*)xar_addere(cv->structurae);
    si (a == NIHIL || s == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde;
    }
    chorda_aedificator_appendere_literis(a, textus);
    chorda_aedificator_appendere_chorda(a, adiectum);
    *s = chorda_aedificator_finire(a);
}

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior vacuum
_s32_scribere (
    ChordaAedificator* a,
                  s32  n)
{
    si (n < ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '-');
        n = -n;
    }
    chorda_aedificator_appendere_i32(a, (i32)n);
}

interior s32
_modulus (
    s32 n,
    s32 m)
{
    s32 r = n % m;

    redde r < ZEPHYRUM ? r + m : r;
}


/* ==================================================
 * Altitudo et orthographia
 * ================================================== */

/* scala maior: semitonia gradus 0..6 a do; classes toni litterarum c..b */
hic_manens constans s32 SCALA[VII] = { 0, 2, 4, 5, 7, 9, 11 };
hic_manens constans character LITTERAE[] = "cdefgab";

/* littera do per classem toni (0 c .. 6 b); bemolles praeferuntur (Gb,
 * non F#) - signum ex _orthographia */
hic_manens constans s32 DO_LITTERA[XII] = { 0, 1, 1, 2, 2, 3, 4, 4, 5,
    5, 6, 6 };

/* gradus a do -> semitonia a do (sine alteratione) */
interior s32
_sonus_gradus (
    s32 gradus)
{
    s32 octava = gradus >= ZEPHYRUM ? gradus / VII
                                    : -((-gradus + VI) / VII);

    redde XII * octava + SCALA[gradus - VII * octava];
}

/* littera, signum, octava MML (octava litterae SCRIPTAE) notae midi
 * quae gradum 'gradus' a do (do_midi) tenet. FALSUM si enharmonice
 * scripta (signum duplex aut octava extra o0..o8) */
interior b32
_orthographia (
    s32  do_midi,
    s32  gradus,
    s32  midi,
    s32* littera,
    s32* signum,
    s32* octava)
{
    s32 classis  = _modulus(do_midi, XII);
    s32 l        = _modulus(DO_LITTERA[classis] + gradus, VII);
    s32 s        = _modulus(midi - SCALA[l] + VI, XII) - VI;
    b32 recta    = VERUM;

    si (s > I)
    {
        l      = (l + I) % VII;
        recta  = FALSUM;
    }
    alioquin si (s < -I)
    {
        l      = (l + VI) % VII;
        recta  = FALSUM;
    }
    s        = _modulus(midi - SCALA[l] + VI, XII) - VI;
    *octava  = (midi - SCALA[l] - s) / XII - I;
    si (*octava < ZEPHYRUM || *octava > VIII)
    {
        /* Cb9, B#-1: scriptio simplex per classem toni */
        hic_manens constans s32 SIMPLEX_LITTERA[XII] = { 0, 0, 1, 1, 2,
            3,
            3, 4, 4, 5, 5, 6 };

        l        = SIMPLEX_LITTERA[_modulus(midi, XII)];
        s        = _modulus(midi, XII) - SCALA[l];
        *octava  = (midi - SCALA[l] - s) / XII - I;
        recta    = FALSUM;
    }
    *littera  = l;
    *signum   = s;
    redde recta;
}

/* "F4", "Bb3" */
interior vacuum
_nomen_toni_scribere (
    ChordaAedificator* a,
                  s32  littera,
                  s32  signum,
                  s32  octava)
{
    chorda_aedificator_appendere_character(a,
        (character)(LITTERAE[littera] - 'a' + 'A'));
    si (signum < ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, 'b');
    }
    alioquin si (signum > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '#');
    }
    _s32_scribere(a, octava);
}


/* ==================================================
 * Verba: purgatio syllabae
 * ================================================== */

nomen structura {
                 s32 initium;
                 s32 finis;
    constans character* textus;    /* minusculae */
                 b32 alternans;   /* par maiuscula, impar minuscula */
} Transcriptio;

/* litterae Latinae cum signis -> ASCII (ordine codicis) */
hic_manens constans Transcriptio TRANSCRIPTIONES[] = {
    { 0xC0, 0xC5, "A", FALSUM },   { 0xC6, 0xC6, "Ae", FALSUM },
    { 0xC7, 0xC7, "C", FALSUM },   { 0xC8, 0xCB, "E", FALSUM },
    { 0xCC, 0xCF, "I", FALSUM },   { 0xD1, 0xD1, "N", FALSUM },
    { 0xD2, 0xD6, "O", FALSUM },   { 0xD8, 0xD8, "O", FALSUM },
    { 0xD9, 0xDC, "U", FALSUM },   { 0xDD, 0xDD, "Y", FALSUM },
    { 0xE0, 0xE5, "a", FALSUM },   { 0xE6, 0xE6, "ae", FALSUM },
    { 0xE7, 0xE7, "c", FALSUM },   { 0xE8, 0xEB, "e", FALSUM },
    { 0xEC, 0xEF, "i", FALSUM },   { 0xF1, 0xF1, "n", FALSUM },
    { 0xF2, 0xF6, "o", FALSUM },   { 0xF8, 0xF8, "o", FALSUM },
    { 0xF9, 0xFC, "u", FALSUM },   { 0xFD, 0xFD, "y", FALSUM },
    { 0xFF, 0xFF, "y", FALSUM },   { 0x100, 0x105, "a", VERUM },
    { 0x112, 0x11B, "e", VERUM },  { 0x128, 0x12F, "i", VERUM },
    { 0x14C, 0x151, "o", VERUM },  { 0x152, 0x152, "Oe", FALSUM },
    { 0x153, 0x153, "oe", FALSUM }, { 0x168, 0x173, "u", VERUM },
    { 0x1FC, 0x1FC, "Ae", FALSUM }, { 0x1FD, 0x1FD, "ae", FALSUM },
    { 0x232, 0x233, "y", VERUM }
};

#define NUMERUS_TRANSCRIPTIONUM \
    ((i32)(magnitudo(TRANSCRIPTIONES) / magnitudo(TRANSCRIPTIONES[0])))

/* signa nota quae tacite tolluntur: interpunctio Latin-1, × ÷,
 * interpunctio generalis, litterae-signa (R/ V/), sagittae, dingbats
 * (cruces) */
interior b32
_signum_notum (
    s32 runa)
{
    redde (runa >= 0x80 && runa <= 0xBF) || runa == 0xD7 || runa == 0xF7
        || (runa >= 0x2000 && runa <= 0x206F)
        || (runa >= 0x2100 && runa <= 0x21FF)
        || (runa >= 0x2700 && runa <= 0x27BF);
}

/* textus cantatus syllabae (non elisio) purgatus in exitum: ASCII
 * litterae solae, prima ut scripta, ceterae minusculae. *mutata =
 * aliquid sublatum aut transcriptum. VERUM si littera ignota sublata */
interior b32
_syllabam_purgare (
    constans GabcSyllaba* syllaba,
       ChordaAedificator* exitus,
                     b32* mutata)
{
    i32 r;
    i32 initium  = (i32)chorda_aedificator_longitudo(exitus);
    b32 ignota   = FALSUM;

    per (r = ZEPHYRUM; r < syllaba->numerus_textuum; r++)
    {
        constans GabcTextus* t = &syllaba->textus[r];
                constans i8* p;
                constans i8* finis;

        si (   t->genus != GABC_TEXTUS_CANTATUS
            || (t->stilus & GABC_STILUS_ELISIO))
        {
            perge;
        }
        p      = t->textus.datum;
        finis  = p + t->textus.mensura;
        dum (p < finis)
        {
             constans i8* ante = p;
                     s32  runa = utf8_decodere(&p, finis);
                     i32  k;
                     b32  inventa = FALSUM;

            si (p == ante)
            {
                p++;
            }
            si (   (runa >= 'a' && runa <= 'z')
                || (runa >= 'A' && runa <= 'Z'))
            {
                chorda_aedificator_appendere_character(exitus,
                    (character)runa);
                perge;
            }
            *mutata = VERUM;
            si (runa >= ZEPHYRUM && runa < 0x80)
            {
                perge;
            }
            per (k = ZEPHYRUM; k < NUMERUS_TRANSCRIPTIONUM; k++)
            {
                constans Transcriptio* x = &TRANSCRIPTIONES[k];

                si (runa >= x->initium && runa <= x->finis)
                {
                    si (x->alternans && (runa % II) == ZEPHYRUM)
                    {
                        chorda_aedificator_appendere_character(exitus,
                            (character)(x->textus[ZEPHYRUM] - 'a'
                                + 'A'));
                    }
                    alioquin
                    {
                        chorda_aedificator_appendere_literis(exitus,
                            x->textus);
                    }
                    inventa = VERUM;
                    frange;
                }
            }
            si (!inventa && (runa < ZEPHYRUM || !_signum_notum(runa)))
            {
                ignota = VERUM;
            }
        }
    }
    /* littera prima ut scripta, ceterae minusculae */
    {
        chorda t = chorda_aedificator_spectare(exitus);
           i32 i;

        per (i = initium + I; i < t.mensura; i++)
        {
            si (t.datum[i] >= 'A' && t.datum[i] <= 'Z')
            {
                t.datum[i] = (i8)(t.datum[i] - 'A' + 'a');
            }
        }
    }
    redde ignota;
}


/* ==================================================
 * Notae carminis (rhythmus G8, incisa, verba)
 * ================================================== */

interior s32
_longitudo_propria (
     Conversor* cv,
           i32  k,
           i32  finis_neumae)
{
     constans GabcNota* n    = &cv->cantus->notae[k];
                   b32  mora = n->mora > ZEPHYRUM
                       || (k + I < finis_neumae
                           && cv->cantus->notae[k + I].mora == II);

    si (mora)
    {
        si (n->episema & GABC_EPISEMA_ADEST)
        {
            cv->signa[GABC_CARMEN_EPISEMA_CUM_MORA]++;
        }
        redde IV;
    }
    redde (n->episema & GABC_EPISEMA_ADEST) ? III : II;
}

interior vacuum
_signa_notae (
             Conversor* cv,
     constans GabcNota* n)
{
    si (n->ictus)
    {
        cv->signa[GABC_CARMEN_ICTUS]++;
    }
    si (n->signa & (GABC_SIGNUM_ACCENTUS | GABC_SIGNUM_ACCENTUS_REVERSUS
        | GABC_SIGNUM_CIRCULUS | GABC_SIGNUM_SEMICIRCULUS
        | GABC_SIGNUM_SEMICIRCULUS_REVERSUS))
    {
        cv->signa[GABC_CARMEN_SIGNUM_NOTAE]++;
    }
    si (n->formae & (GABC_FORMA_LIQUESCENS | GABC_FORMA_AUCTA_ASCENDENS
        | GABC_FORMA_AUCTA_DESCENDENS | GABC_FORMA_INITIO_DEBILIS))
    {
        cv->signa[GABC_CARMEN_LIQUESCENTIA]++;
    }
}

interior NotaCarminis*
_nota (
    Conversor* cv,
          s32  index)
{
    redde (NotaCarminis*)xar_obtinere(cv->notae, (i32)index);
}

/* syllaba post notas suas -> verba */
interior vacuum
_verba_syllabae (
                Conversor* cv,
     constans GabcSyllaba* syllaba,
                      s32  syllaba_index,
                      i32  notae,
        ChordaAedificator* verba,
                      b32* textus_ullus)
{
    ChordaAedificator* t = chorda_aedificator_creare(cv->piscina,
        XXXII);
                  b32 mutata = FALSUM;
                  i32 k;

    si (t == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde;
    }
    si (_syllabam_purgare(syllaba, t, &mutata))
    {
        _approximatio(cv, GABC_CARMEN_LITTERA_IGNOTA, syllaba_index,
            syllaba->linea, syllaba->columna,
            "littera non Latina e verbis sublata");
    }
    si (mutata)
    {
        cv->syllabae_purgatae++;
    }
    k = ZEPHYRUM;
    si (chorda_aedificator_longitudo(t) > ZEPHYRUM)
    {
        si (chorda_aedificator_longitudo(verba) > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(verba,
                syllaba->verbum_incipit ? ' ' : '-');
        }
        chorda_aedificator_appendere_chorda(verba,
            chorda_aedificator_finire(t));
        *textus_ullus  = VERUM;
        k              = I;
    }
    per (; k < notae; k++)
    {
        si (chorda_aedificator_longitudo(verba) > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(verba, ' ');
        }
        chorda_aedificator_appendere_character(verba, '_');
    }
}

interior vacuum
_notas_colligere (
             Conversor* cv,
     ChordaAedificator* verba,
                   b32* textus_ullus)
{
     constans GabcCantus* g = cv->cantus;
                     i32  s;

    per (s = ZEPHYRUM; s < g->numerus_syllabarum; s++)
    {
         constans GabcSyllaba* syllaba  = &g->syllabae[s];
                          s32  prior    = -I;   /* nota carminis prior in
                                               * syllaba, sine divisione */
                          i32 ante    = xar_numerus(cv->notae);
                          i32 e;

        per (e = ZEPHYRUM; e < syllaba->numerus_elementorum; e++)
        {
             constans GabcElementum* el = &syllaba->elementa[e];
                                i32  k;

            si (el->genus == GABC_EL_SIGNUM_OPACUM)
            {
                cv->signa[GABC_CARMEN_SIGNUM_OPACUM]++;
                perge;
            }
            si (el->genus == GABC_EL_DIVISIO)
            {
                s32 n = (s32)xar_numerus(cv->notae);

                prior = -I;
                si (n > ZEPHYRUM)
                {
                     NotaCarminis* u  = _nota(cv, n - I);
                              s32  sp = (   el->divisio
                                  == GABC_DIVISIO_MAIOR
                                  || el->divisio
                                      == GABC_DIVISIO_MAIOR_PUNCTATA
                                  || el->divisio
                                      == GABC_DIVISIO_FINALIS)
                                  ? SPIRITUS_MAIOR : SPIRITUS_MINOR;

                    u->finis = VERUM;
                    si (sp > u->spiritus)
                    {
                        u->spiritus = sp;
                    }
                }
                perge;
            }
            si (el->genus != GABC_EL_NEUMA)
            {
                perge;
            }
            per (k = el->prima; k < el->prima + el->numerus; k++)
            {
                 constans GabcNota* n  = &g->notae[k];
                               s32  longitudo_notae =
                                   _longitudo_propria(cv, k,
                                   el->prima + el->numerus);
                               s32 sonus = _sonus_gradus(n->gradus)
                                   + n->alteratio;
                     NotaCarminis* p  = prior >= ZEPHYRUM
                                   ? _nota(cv, prior) : NIHIL;

                _signa_notae(cv, n);
                si (   p != NIHIL
                    && (n->formae & (GABC_FORMA_QUILISMA
                        | GABC_FORMA_QUILISMA_QUADRATUM))
                    && p->longitudo < III)
                {
                    p->longitudo = III;
                }
                si (p != NIHIL && p->sonus == sonus)
                {
                    p->longitudo += longitudo_notae;
                    p->numerus++;
                    perge;
                }
                p = (NotaCarminis*)xar_addere(cv->notae);
                si (p == NIHIL)
                {
                    cv->memoria_defecit = VERUM;
                    redde;
                }
                memset(p, ZEPHYRUM, magnitudo(*p));
                p->sonus      = sonus;
                p->gradus     = n->gradus;
                p->alteratio  = n->alteratio;
                p->longitudo  = longitudo_notae;
                p->nota       = (i32)k;
                p->numerus    = I;
                p->syllaba    = (i32)s;
                p->elementum  = (i32)e;
                p->linea      = n->linea;
                p->columna    = n->columna;
                prior         = (s32)xar_numerus(cv->notae) - I;
            }
        }
        si (xar_numerus(cv->notae) > ante)
        {
            _verba_syllabae(cv, syllaba, (s32)s,
                xar_numerus(cv->notae) - ante, verba, textus_ullus);
        }
    }
}


/* ==================================================
 * Altitudo et clavis
 * ================================================== */

hic_manens constans character* MODI[VI] = { "ionius", "dorius",
    "phrygius", "lydius", "mixolydius", "aeolius" };

interior s32
_altitudinem_ponere (
    Conversor* cv)
{
    i32 n = xar_numerus(cv->notae);
    i32 i;
    s32 summus = -CMXCIX;
    s32 do_midi;

    per (i = ZEPHYRUM; i < n; i++)
    {
        si (_nota(cv, (s32)i)->sonus > summus)
        {
            summus = _nota(cv, (s32)i)->sonus;
        }
    }
    do_midi = cv->forma->do_fixum >= ZEPHYRUM ? cv->forma->do_fixum
                                              : cv->forma->summus
                                                  - summus;
    per (i = ZEPHYRUM; i < n; i++)
    {
         NotaCarminis* p = _nota(cv, (s32)i);
                  s32  m = do_midi + p->sonus;

        si (m < XII || m > CXIX)
        {
            dum (m < XII)
            {
                m += XII;
            }
            dum (m > CXIX)
            {
                m -= XII;
            }
            _approximatio(cv, GABC_CARMEN_EXTRA_AMBITUM,
                (s32)p->syllaba,
                p->linea, p->columna,
                "nota extra o0..o8: octava mota");
        }
        p->midi = m;
        si (!_orthographia(do_midi, p->gradus, m, &p->littera,
            &p->signum,
            &p->octava))
        {
            cv->enharmonicae++;
        }
    }
    redde do_midi;
}

/* clavis carminis: finalis + modus ex collectione (caput: CLAVIS) */
interior chorda
_clavem_eligere (
    Conversor* cv,
          s32  do_midi)
{
    constans GabcCantus* g = cv->cantus;
      ChordaAedificator* a = chorda_aedificator_creare(cv->piscina,
          XVI);
           NotaCarminis* f = _nota(cv, (s32)xar_numerus(cv->notae) - I);
                    i32  si_notae = ZEPHYRUM;
                    i32  si_bemolles = ZEPHYRUM;
                    i32  i;
                    b32  collectio_fa;
                    s32  gradus_finalis;
                    s32  gradus_in_collectione;
                    s32  alteratio_exspectata;
                    s32  littera;
                    s32  signum;
                    s32  octava;

    si (a == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde _vacua();
    }
    per (i = ZEPHYRUM; i < g->numerus_notarum; i++)
    {
        si (_modulus(g->notae[i].gradus, VII) == VI)
        {
            si_notae++;
            si (g->notae[i].alteratio < ZEPHYRUM)
            {
                si_bemolles++;
            }
        }
    }
    collectio_fa           = si_notae > ZEPHYRUM
                             && si_bemolles * II > si_notae;
    gradus_finalis         = _modulus(f->gradus, VII);
    gradus_in_collectione  = collectio_fa
                             ? _modulus(gradus_finalis - III, VII)
                             : gradus_finalis;
    alteratio_exspectata   = collectio_fa
        && gradus_finalis == VI ? -I : 0;
    si (   gradus_in_collectione == VI
        || f->alteratio          != alteratio_exspectata)
    {
        /* do collectionis maior */
        s32 gradus_do = collectio_fa ? III : ZEPHYRUM;

        _orthographia(do_midi, gradus_do, do_midi + SCALA[gradus_do],
            &littera, &signum, &octava);
        _approximatio(cv, GABC_CARMEN_CLAVIS_SINE_MODO, (s32)f->syllaba,
            f->linea, f->columna, gradus_in_collectione == VI
                ? "finalis in si collectionis: carmen modum non nominat"
                : "finalis alterata extra collectionem");
        gradus_in_collectione = -I;
    }
    alioquin
    {
        littera  = f->littera;
        signum   = f->signum;
    }
    chorda_aedificator_appendere_character(a, LITTERAE[littera]);
    si (signum < ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, 'b');
    }
    alioquin si (signum > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '#');
    }
    chorda_aedificator_appendere_character(a, ' ');
    chorda_aedificator_appendere_literis(a, gradus_in_collectione >= 0
        ? MODI[gradus_in_collectione] : "maior");
    _structura(cv, collectio_fa
        ? "clavis ex collectione fa (si bemolle plus quam dimidia parte)"
        : "clavis ex collectione do", _vacua());
    redde chorda_aedificator_finire(a);
}


/* ==================================================
 * Scriptio
 * ================================================== */

/* titulus [a-z0-9_]+ (ut musicxml_carmen) */
interior chorda
_plicare (
     Piscina* piscina,
      chorda  t)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, XXXII);
                  i32  i;
                  b32  linea = FALSUM;

    si (a == NIHIL)
    {
        redde _vacua();
    }
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c = (character)t.datum[i];

        si (c >= 'A' && c <= 'Z')
        {
            c = (character)(c - 'A' + 'a');
        }
        si ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        {
            si (linea && chorda_aedificator_longitudo(a) > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '_');
            }
            chorda_aedificator_appendere_character(a, c);
            linea = FALSUM;
        }
        alioquin
        {
            linea = VERUM;
        }
    }
    redde chorda_aedificator_finire(a);
}

/* textus commentarii: '--' -> '- -', '>' et lineae -> spatium */
interior vacuum
_commentarium_appendere (
    ChordaAedificator* a,
               chorda  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c = (character)t.datum[i];

        si (c == '>' || c == '\n' || c == '\r')
        {
            c = ' ';
        }
        chorda_aedificator_appendere_character(a, c);
        si (c == '-' && i + I < t.mensura && t.datum[i + I] == '-')
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
    }
}

interior chorda
_caput (
    constans GabcCantus* cantus,
     constans character* clavis)
{
    i32 i;

    per (i = ZEPHYRUM; i < cantus->numerus_capitum; i++)
    {
        si (chorda_aequalis_literis(cantus->capita[i].clavis, clavis))
        {
            redde cantus->capita[i].valor;
        }
    }
    redde _vacua();
}

interior vacuum
_partem_commentarii (
     ChordaAedificator* a,
                   b32* prima,
    constans character* praefixum,
                chorda  valor)
{
    si (valor.mensura == ZEPHYRUM)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(a, *prima ? " " : "; ");
    chorda_aedificator_appendere_literis(a, praefixum);
    _commentarium_appendere(a, valor);
    *prima = FALSUM;
}

/* longitudines MML (sextae decimae), maior prior */
hic_manens constans s32 LONGITUDINES[IX] = { 24, 16, 12, 8, 6, 4, 3, 2,
    1 };
hic_manens constans character* LONGITUDINES_MML[IX] = { "1.", "1", "2.",
    "2", "4.", "4", "8.", "8", "16" };

interior vacuum
_notam_mml (
        ChordaAedificator* a,
    constans NotaCarminis* p)
{
    s32 reliquum = p->longitudo;
    i32 k;
    b32 prima    = VERUM;

    per (k = ZEPHYRUM; k < IX && reliquum > ZEPHYRUM; k++)
    {
        dum (reliquum >= LONGITUDINES[k])
        {
            si (!prima)
            {
                chorda_aedificator_appendere_literis(a, " & ");
            }
            chorda_aedificator_appendere_character(a,
                LITTERAE[p->littera]);
            si (p->signum < ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '-');
            }
            alioquin si (p->signum > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '+');
            }
            chorda_aedificator_appendere_literis(a,
                LONGITUDINES_MML[k]);
            reliquum  -= LONGITUDINES[k];
            prima     = FALSUM;
        }
    }
}

/* textus MML et nexus (G11) */
interior b32
_textum_scribere (
             Conversor* cv,
    ChordaAedificator* a,
     GabcCarmenRelatio* relatio)
{
    i32 n       = xar_numerus(cv->notae);
    i32 i;
    s32 octava = -I;
    s64 pulsus = ZEPHYRUM;

    relatio->nexus = (GabcNexus*)piscina_allocare(cv->piscina,
        (memoriae_index)((n + I) * (i32)magnitudo(GabcNexus)));
    si (relatio->nexus == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        NotaCarminis* p = _nota(cv, (s32)i);
           GabcNexus* x = &relatio->nexus[i];

        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        si (p->octava != octava)
        {
            chorda_aedificator_appendere_character(a, 'o');
            _s32_scribere(a, p->octava);
            chorda_aedificator_appendere_character(a, ' ');
            octava = p->octava;
        }
        _notam_mml(a, p);
        x->initium    = pulsus;
        x->longitudo  = (s64)p->longitudo * PULSUS_SEXTAE;
        x->nota       = p->nota;
        x->numerus    = p->numerus;
        x->syllaba    = p->syllaba;
        x->elementum  = p->elementum;
        x->linea      = p->linea;
        x->columna    = p->columna;
        pulsus        += x->longitudo;
        si (p->finis && i + I < n)
        {
            si (p->spiritus > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(a,
                    p->spiritus == SPIRITUS_MAIOR ? " r8" : " r16");
                pulsus += (s64)p->spiritus * PULSUS_SEXTAE;
            }
            chorda_aedificator_appendere_literis(a, " |");
        }
    }
    relatio->numerus_nexuum = n;
    redde VERUM;
}


/* ==================================================
 * API
 * ================================================== */

vacuum
gabc_carmen_formam_ordinariam (
    GabcCarmenForma* forma)
{
    memset(forma, ZEPHYRUM, magnitudo(*forma));
    forma->strictus      = FALSUM;
    forma->summus        = LXV;
    forma->do_fixum      = -I;
    forma->tempus        = LXXX;
    forma->titulus       = _vacua();
    forma->instrumentum  = _vacua();
}

b32
gabc_carmen_scribere (
         constans GabcCantus* cantus,
    constans GabcCarmenForma* forma,
                     Piscina* piscina,
           ChordaAedificator* exitus,
           GabcCarmenRelatio* relatio)
{
              Conversor  cv;
      ChordaAedificator* a;
      ChordaAedificator* verba;
                    b32  textus_ullus  = FALSUM;
                    b32  bene          = FALSUM;
                    s32  do_midi       = -I;

    si (relatio == NIHIL)
    {
        redde FALSUM;
    }
    memset(relatio, ZEPHYRUM, magnitudo(*relatio));
    relatio->do_midi  = -I;
    relatio->clavis   = _vacua();
    si (   cantus == NIHIL || forma == NIHIL || piscina == NIHIL
        || exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(&cv, ZEPHYRUM, magnitudo(cv));
    cv.piscina  = piscina;
    cv.cantus   = cantus;
    cv.forma    = forma;
    cv.approximationes  = xar_creare(piscina,
        (i32)magnitudo(GabcApproximatio));
    cv.structurae  = xar_creare(piscina, (i32)magnitudo(chorda));
    cv.notae       = xar_creare(piscina,
        (i32)magnitudo(NotaCarminis));
    a      = chorda_aedificator_creare(piscina, MMMCMXCIX);
    verba  = chorda_aedificator_creare(piscina, CCLVI);
    si (   cv.approximationes == NIHIL || cv.structurae == NIHIL
        || cv.notae           == NIHIL || a == NIHIL || verba == NIHIL)
    {
        redde FALSUM;
    }

    si (!gabc_successus(cantus))
    {
        _structura(&cv, "cantus cum vitio: non conversus", _vacua());
        salta relatio_scribenda;
    }
    _notas_colligere(&cv, verba, &textus_ullus);
    si (cv.memoria_defecit || cv.fractus)
    {
        salta relatio_scribenda;
    }
    si (xar_numerus(cv.notae) == ZEPHYRUM)
    {
        _structura(&cv, "cantus sine notis: non conversus", _vacua());
        salta relatio_scribenda;
    }
    /* incisum ultimum: sine spiritu et sine '|' */
    _nota(&cv, (s32)xar_numerus(cv.notae) - I)->finis     = FALSUM;
    _nota(&cv, (s32)xar_numerus(cv.notae) - I)->spiritus  = ZEPHYRUM;

    do_midi           = _altitudinem_ponere(&cv);
    relatio->do_midi  = do_midi;
    relatio->clavis   = _clavem_eligere(&cv, do_midi);
    si (cv.memoria_defecit || cv.fractus)
    {
        salta relatio_scribenda;
    }
    {
        ChordaAedificator* d = chorda_aedificator_creare(piscina,
            XXXII);
                      s32 littera;
                      s32 signum;
                      s32 octava;

        si (d == NIHIL)
        {
            cv.memoria_defecit = VERUM;
            salta relatio_scribenda;
        }
        _orthographia(do_midi, ZEPHYRUM, do_midi, &littera, &signum,
            &octava);
        chorda_aedificator_appendere_literis(d, "do = ");
        _nomen_toni_scribere(d, littera, signum, octava);
        chorda_aedificator_appendere_literis(d, forma->do_fixum
            >= ZEPHYRUM
            ? " (fixum)" : " (nota altissima in summo)");
        _structura(&cv, "", chorda_aedificator_finire(d));

        /* commentarium */
        {
            b32 prima = VERUM;

            chorda_aedificator_appendere_literis(a, "  <!-- gabc:");
            _partem_commentarii(a, &prima, "", cantus->titulus);
            _partem_commentarii(a, &prima, "modus ", cantus->modus);
            si (cantus->differentia.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, ' ');
                _commentarium_appendere(a, cantus->differentia);
            }
            _partem_commentarii(a, &prima, "",
                _caput(cantus, "office-part"));
            _partem_commentarii(a, &prima, "liber ", _caput(cantus,
                "book"));
            _partem_commentarii(a, &prima, "transcriptor ",
                _caput(cantus, "transcriber"));
            chorda_aedificator_appendere_literis(a, prima ? " " : "; ");
            chorda_aedificator_appendere_literis(a, "do = ");
            _nomen_toni_scribere(a, littera, signum, octava);
            chorda_aedificator_appendere_literis(a, " -->\n");
        }
    }

    /* caput carminis */
    {
        chorda titulus = _plicare(piscina, forma->titulus.mensura
            > ZEPHYRUM
                             ? forma->titulus : cantus->titulus);

        si (titulus.mensura == ZEPHYRUM)
        {
            titulus = chorda_ex_literis("gabc", piscina);
        }
        chorda_aedificator_appendere_literis(a, "  <carmen titulus=\"");
        chorda_aedificator_appendere_chorda(a, titulus);
        chorda_aedificator_appendere_literis(a, "\" tempus=\"");
        chorda_aedificator_appendere_i32(a, forma->tempus);
        chorda_aedificator_appendere_literis(a,
            "\" metrum=\"liberum\" clavis=\"");
        chorda_aedificator_appendere_chorda(a, relatio->clavis);
        chorda_aedificator_appendere_literis(a, "\">\n"
            "    <reverberatio aula=\"0.9\" mollities=\"0.6\" "
            "latitudo=\"1\" lucrum=\"0.5\"/>\n"
            "    <pars titulus=\"cantus\" instrumentum=\"");
        si (forma->instrumentum.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(a, forma->instrumentum);
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a, "schola");
        }
        chorda_aedificator_appendere_literis(a, "\" lucrum=\"1\"\n"
            "          reverberatio=\"0.55\">\n"
            "      <frustum mensura=\"1\"><textus!>");
    }
    si (!_textum_scribere(&cv, a, relatio))
    {
        cv.memoria_defecit = VERUM;
        salta relatio_scribenda;
    }
    chorda_aedificator_appendere_literis(a, "</textus>");
    si (textus_ullus)
    {
        chorda_aedificator_appendere_literis(a, "\n        <verba!>");
        chorda_aedificator_appendere_chorda(a,
            chorda_aedificator_finire(verba));
        chorda_aedificator_appendere_literis(a, "</verba>");
    }
    chorda_aedificator_appendere_literis(a, "</frustum>\n"
        "    </pars>\n  </carmen>\n");

    si (cv.enharmonicae > ZEPHYRUM)
    {
        _structura(&cv, "notae enharmonice scriptae (signum duplex aut "
            "octava extra o0..o8)", _vacua());
    }
    si (cv.syllabae_purgatae > ZEPHYRUM)
    {
        _structura(&cv, "syllabae purgatae (accentus, signa, "
            "interpunctio sublata)", _vacua());
    }
    bene = VERUM;

relatio_scribenda:
    relatio->approximationes = (GabcApproximatio*)piscina_allocare(
        piscina, (memoriae_index)((xar_numerus(cv.approximationes) + I)
            * (i32)magnitudo(GabcApproximatio)));
    si (relatio->approximationes != NIHIL)
    {
        relatio->numerus_approximationum = xar_copiare_ad_tabulam(
            cv.approximationes, relatio->approximationes, ZEPHYRUM,
            xar_numerus(cv.approximationes));
    }
    relatio->structurae = (chorda*)piscina_allocare(piscina,
        (memoriae_index)((xar_numerus(cv.structurae) + I)
            * (i32)magnitudo(chorda)));
    si (relatio->structurae != NIHIL)
    {
        relatio->numerus_structurarum = xar_copiare_ad_tabulam(
            cv.structurae, relatio->structurae, ZEPHYRUM,
            xar_numerus(cv.structurae));
    }
    memcpy(relatio->signa, cv.signa, magnitudo(cv.signa));
    si (!bene || cv.fractus || cv.memoria_defecit)
    {
        redde FALSUM;
    }
    chorda_aedificator_appendere_chorda(exitus,
        chorda_aedificator_finire(a));
    redde VERUM;
}

constans character*
gabc_approximationis_titulus (
    GabcApproximatioGenus genus)
{
    commutatio (genus)
    {
        casus GABC_CARMEN_CLAVIS_SINE_MODO:
            redde "clavis sine modo";
        casus GABC_CARMEN_EXTRA_AMBITUM:
            redde "extra ambitum";
        casus GABC_CARMEN_LITTERA_IGNOTA:
            redde "littera ignota";
        ordinarius:
            redde "ignota";
    }
}

constans character*
gabc_signi_carminis_titulus (
    GabcSignumCarminisGenus genus)
{
    commutatio (genus)
    {
        casus GABC_CARMEN_ICTUS:
            redde "ictus";
        casus GABC_CARMEN_SIGNUM_NOTAE:
            redde "signa supra notam";
        casus GABC_CARMEN_LIQUESCENTIA:
            redde "liquescentia";
        casus GABC_CARMEN_EPISEMA_CUM_MORA:
            redde "episema cum mora";
        casus GABC_CARMEN_SIGNUM_OPACUM:
            redde "signa opaca";
        ordinarius:
            redde "ignotum";
    }
}

b32
gabc_relationem_scribere (
    constans GabcCarmenRelatio* relatio,
            constans character* via,
             ChordaAedificator* exitus)
{
    i32 i;

    si (relatio == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    via = via != NIHIL ? via : "";
    per (i = ZEPHYRUM; i < relatio->numerus_approximationum; i++)
    {
        constans GabcApproximatio* x = &relatio->approximationes[i];

        chorda_aedificator_appendere_literis(exitus, via);
        chorda_aedificator_appendere_character(exitus, ':');
        chorda_aedificator_appendere_i32(exitus, x->linea);
        chorda_aedificator_appendere_character(exitus, ':');
        chorda_aedificator_appendere_i32(exitus, x->columna);
        chorda_aedificator_appendere_literis(exitus, ": ");
        chorda_aedificator_appendere_literis(exitus,
            gabc_approximationis_titulus(x->genus));
        chorda_aedificator_appendere_literis(exitus, " - ");
        chorda_aedificator_appendere_chorda(exitus, x->descriptio);
        chorda_aedificator_appendere_character(exitus, '\n');
    }
    per (i = ZEPHYRUM; i < (i32)GABC_CARMEN_SIGNORUM_GENERA; i++)
    {
        si (relatio->signa[i] > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(exitus, via);
            chorda_aedificator_appendere_literis(exitus,
                ": signa non conversa: ");
            chorda_aedificator_appendere_literis(exitus,
                gabc_signi_carminis_titulus((GabcSignumCarminisGenus)i));
            chorda_aedificator_appendere_character(exitus, ' ');
            chorda_aedificator_appendere_i32(exitus, relatio->signa[i]);
            chorda_aedificator_appendere_character(exitus, '\n');
        }
    }
    per (i = ZEPHYRUM; i < relatio->numerus_structurarum; i++)
    {
        chorda_aedificator_appendere_literis(exitus, via);
        chorda_aedificator_appendere_literis(exitus,
            ": nota structurae: ");
        chorda_aedificator_appendere_chorda(exitus,
            relatio->structurae[i]);
        chorda_aedificator_appendere_character(exitus, '\n');
    }
    redde VERUM;
}
