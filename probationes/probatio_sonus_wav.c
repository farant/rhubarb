/* probatio_sonus_wav.c - Probationes scriptoris WAV (include/sonus_wav.h)
 *
 * Octeti aurei MANU computati ex specificatione RIFF/WAVE (caput XLIV
 * octetorum, PCM XVI bitorum little-endian), non ex codice exscripti:
 * XLVIII milia = 0xBB80, 0.5 x 32767 = 16383.5 -> 16384 = 0x4000
 * (rotundatio a zephyro, ergo -0.5 -> -16384 = 0xC000), 2.0 coercetur
 * ad 0x7FFF. Vide lib/sonus_wav.worklog.md.
 */
#include "latina.h"
#include "credo.h"
#include "filum.h"
#include "sonus.h"
#include "sonus_wav.h"

interior b32
_octeti_pares (
             chorda  c,
        constans i8* aurea,
                i32  n)
{
    i32 k;

    si (c.mensura != n || c.datum == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (c.datum[k] != aurea[k])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

hic_manens constans f32 EXEMPLA_UNICA[VI] = {
    0.0f, 0.5f, 1.0f, -1.0f, 2.0f, -0.5f };

hic_manens constans i8 AUREA_UNICA[XLIV + XII] = {
    'R', 'I', 'F', 'F', 0x30, 0x00, 0x00, 0x00,   /* XXXVI + XII */
    'W', 'A', 'V', 'E',
    'f', 'm', 't', ' ', 0x10, 0x00, 0x00, 0x00,   /* XVI */
    0x01, 0x00,                                   /* PCM */
    0x01, 0x00,                                   /* canalis I */
    0x80, 0xBB, 0x00, 0x00,                       /* XLVIII milia */
    0x00, 0x77, 0x01, 0x00,                       /* XCVI milia octeti/s */
    0x02, 0x00,                                   /* II octeti/quadrum */
    0x10, 0x00,                                   /* XVI bita */
    'd', 'a', 't', 'a', 0x0C, 0x00, 0x00, 0x00,   /* XII */
    0x00, 0x00,   /*  0.0 ->      0 */
    0x00, 0x40,   /*  0.5 ->  16384 */
    0xFF, 0x7F,   /*  1.0 ->  32767 */
    0x01, 0x80,   /* -1.0 -> -32767 */
    0xFF, 0x7F,   /*  2.0 ->  32767 (coercitum) */
    0x00, 0xC0    /* -0.5 -> -16384 */
};

hic_manens constans f32 EXEMPLA_BINA[IV] = {
    0.25f, -0.25f, 0.0f, 1.0f };

hic_manens constans i8 AUREA_BINA[XLIV + VIII] = {
    'R', 'I', 'F', 'F', 0x2C, 0x00, 0x00, 0x00,   /* XXXVI + VIII */
    'W', 'A', 'V', 'E',
    'f', 'm', 't', ' ', 0x10, 0x00, 0x00, 0x00,
    0x01, 0x00,
    0x02, 0x00,                                   /* canales II */
    0x80, 0xBB, 0x00, 0x00,
    0x00, 0xEE, 0x02, 0x00,                       /* CXCII milia */
    0x04, 0x00,                                   /* IV octeti/quadrum */
    0x10, 0x00,
    'd', 'a', 't', 'a', 0x08, 0x00, 0x00, 0x00,
    0x00, 0x20,   /* L  0.25 ->  8192 */
    0x00, 0xE0,   /* R -0.25 -> -8192 */
    0x00, 0x00,   /* L  0.0 */
    0xFF, 0x7F    /* R  1.0 */
};


    /* ==================================================
     * CODIFICARE: octeti aurei mono et stereo
     * ================================================== */

interior vacuum
_sectio_aurea (
    CredoContextus* c)
{
    chorda octeti;

    octeti = sonus_wav_codificare(EXEMPLA_UNICA, VI, I,
        SONUS_FREQUENTIA,
                                  c->piscina);
    CREDO_AEQUALIS_I32 (octeti.mensura, XLIV + XII);
    CREDO_VERUM (_octeti_pares(octeti, AUREA_UNICA, XLIV + XII));

    octeti = sonus_wav_codificare(EXEMPLA_BINA, II, II,
        SONUS_FREQUENTIA,
                                  c->piscina);
    CREDO_AEQUALIS_I32 (octeti.mensura, XLIV + VIII);
    CREDO_VERUM (_octeti_pares(octeti, AUREA_BINA, XLIV + VIII));

    /* quadra 0: caput solum, data mensurae 0 */
    octeti = sonus_wav_codificare(EXEMPLA_UNICA, ZEPHYRUM, I,
                                  SONUS_FREQUENTIA, c->piscina);
    CREDO_AEQUALIS_I32 (octeti.mensura, SONUS_WAV_CAPUT);
    CREDO_VERUM (octeti.datum != NIHIL && octeti.datum[XL] == 0x00
                 && octeti.datum[IV] == 0x24);
}


    /* ==================================================
     * REFUTATIO: canales et frequentia invalidae
     * ================================================== */

interior vacuum
_sectio_refutatio (
    CredoContextus* c)
{
    CREDO_AEQUALIS_I32 (sonus_wav_codificare(EXEMPLA_UNICA, VI,
        ZEPHYRUM,
                                             SONUS_FREQUENTIA,
                                             c->piscina).mensura,
                        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (sonus_wav_codificare(EXEMPLA_UNICA, II, III,
                                             SONUS_FREQUENTIA,
                                             c->piscina).mensura,
                        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (sonus_wav_codificare(EXEMPLA_UNICA, VI, I,
                                             ZEPHYRUM,
                                             c->piscina).mensura,
                        ZEPHYRUM);
    CREDO_VERUM (!sonus_wav_scribere("/nusquam/omnino/x.wav",
        EXEMPLA_UNICA,
                                     VI, I, SONUS_FREQUENTIA,
                                     c->piscina));
}


    /* ==================================================
     * SCRIBERE: plagula relecta octetim eadem
     * ================================================== */

interior vacuum
_sectio_scribere (
    CredoContextus* c)
{
     constans character* via = "/tmp/probatio_sonus_wav.wav";
                 chorda  relecta;

    CREDO_VERUM (sonus_wav_scribere(via, EXEMPLA_UNICA, VI, I,
                                    SONUS_FREQUENTIA, c->piscina));
    relecta = filum_legere_totum(via, c->piscina);
    CREDO_VERUM (_octeti_pares(relecta, AUREA_UNICA, XLIV + XII));
    (vacuum)filum_delere(via);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "aurea", _sectio_aurea, NIHIL, NIHIL, NIHIL, NIHIL },
    { "refutatio", _sectio_refutatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "scribere", _sectio_scribere, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sonus_wav", SECTIONES);
}
