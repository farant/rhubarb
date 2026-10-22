/* auditus_fumus.c - fumus exitus soni vivi (sonus S3b)
 *
 * Exitum aperit (auditus), sonitus praeparatos ad momenta ORDINATA
 * iubet (momentum + mora + margo), horologium exspectat, nuntios legit
 * (rector sedes liberat), censum petit, claudit. Auribus iudicatur:
 * punctum, radius, fragor sinistra, passus quater sinistra/dextra,
 * crepitus brevis. Numeri: quadra provecta, mora, voces finitae,
 * iussa tarda, apex.
 *
 * Usus: ./bin/aedilis tools/auditus_fumus.c && bash
 *   build/aedilis/auditus_fumus/struere.sh &&
 *   ./build/aedilis/auditus_fumus/auditus_fumus
 *
 * Exitus: 0 omnes voces finitae et horologium provectum; 1 sine sono
 * (exitus non apertus) aut numeri falsi.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "sonus.h"
#include "sonitus.h"
#include "mixtura.h"
#include "auditus.h"
#include <stdio.h>
#include <time.h>

#define FUMUS_MARGO     (II * M + CD)      /* L ms ultra moram */
#define FUMUS_SONITUS   VIII
#define FUMUS_TERMINUS  (CXX * M)          /* II.V s ultimi soni */

interior vacuum
_dormire_ms (
    i32 ms)
{
    struct timespec t;

    t.tv_sec   = (time_t)(ms / M);
    t.tv_nsec  = (long)(ms % M) * 1000000L;
    (vacuum)nanosleep(&t, NIHIL);
}

/* sonitum praeparatum ad momentum iubere; ansa vocis aut nulla */
interior MixturaAnsa
_iubere (
         MixturaRector* rector,
    constans character* titulus,
                   s64  momentum,
                   f32  positio,
                   i64  semen)
{
    MixturaIussum i;

    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SONITUM, momentum);
    i.ansa = mixtura_rector_capere(rector, I);
    si (   !mixtura_ansa_nulla(i.ansa)
        && sonitus_praeparatum(titulus) != NIHIL)
    {
        i.parametra  = *sonitus_praeparatum(titulus);
        i.semen      = semen;
        i.positio    = positio;
        si (auditus_iubere(&i))
        {
            redde i.ansa;
        }
    }
    i.ansa.generatio = ZEPHYRUM;
    redde i.ansa;
}

interior vacuum
_numerare (
    MixturaAnsa  ansa,
            i32* iussa)
{
    si (!mixtura_ansa_nulla(ansa))
    {
        (*iussa)++;
    }
}

integer
principale (
    vacuum)
{
         Piscina* piscina = piscina_generare_dynamicum("auditus_fumus",
             M * M);
           Mixtura* mixtura;
     MixturaRector* rector;
      MixturaForma  forma_mixturae;
      AuditusForma  forma;
    MixturaNuntius  n;
     MixturaCensus  census;
               s64  initium;
               s64  nunc;
               s64  primum;
       MixturaAnsa  crepitus;
               i32  finitae        = ZEPHYRUM;
               i32  iussa          = ZEPHYRUM;
               b32  census_lectus  = FALSUM;
               i32  k;

    si (piscina == NIHIL)
    {
        redde I;
    }
    forma_mixturae.voces = XVI;
    forma_mixturae.iussa = LXIV;
    forma_mixturae.nuntii = LXIV;
    forma.iussa = LXIV;
    forma.nuntii = LXIV;
    mixtura = mixtura_creare(piscina, &forma_mixturae);
    rector = mixtura_rectorem_creare(piscina, forma_mixturae.voces);
    si (   mixtura == NIHIL || rector == NIHIL
        || !auditus_aperire(mixtura, &forma, piscina))
    {
        fprintf(stderr,
            "auditus_fumus: exitus soni non apertus (sine sono)\n");
        piscina_destruere(piscina);
        redde I;
    }
    _dormire_ms(C);
    primum   = auditus_momentum();
    initium  = primum + (s64)auditus_mora() + FUMUS_MARGO;
    imprimere("auditus_fumus: apertum - mora %u quadra (%.1f ms), "
              "horologium %ld\n", auditus_mora(),
              (duplex)auditus_mora() * 1000.0
                  / (duplex)SONUS_FREQUENTIA,
              (longus)primum);

    _numerare(_iubere(rector, "punctum", initium, 0.0f, I), &iussa);
    _numerare(_iubere(rector, "radius", initium + XIX * M + CC, 0.0f,
        II),
              &iussa);
    _numerare(_iubere(rector, "fragor", initium + XXXVIII * M + CD,
        -0.7f,
                      III), &iussa);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        _numerare(_iubere(rector, "passus",
                          initium + LVII * M + DC + (s64)k * VII * M
                              + CC,
                          (k % II) ? 0.5f : -0.5f, (i64)(X + k)),
                          &iussa);
    }
    crepitus = _iubere(rector, "crepitus", initium + LXXXVI * M + CD,
        0.3f,
                       IV);
    _numerare(crepitus, &iussa);

    /* exspectare: ultimum sonum (crepitus III s) non - eum ad II.V s
     * sistimus, ut fumus brevis maneat */
    dum ((nunc = auditus_momentum()) < initium + FUMUS_TERMINUS)
    {
        dum (auditus_nuntium_legere(&n))
        {
            si (n.genus == MIXTURA_NUNTIUS_VOX_FINITA)
            {
                finitae++;
            }
            mixtura_rector_tractare(rector, &n);
        }
        _dormire_ms(X);
    }
    {
        MixturaIussum i;
                  s64 ultima = auditus_momentum() + (s64)auditus_mora();
                  s64 finis;

        /* crepitus (adhuc sonans) evanescit per C ms */
        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SISTERE, ultima);
        i.ansa    = crepitus;
        i.clivus  = IV * M + DCCC;
        (vacuum)auditus_iubere(&i);
        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_CENSUM_PETERE,
                              ultima + V * M);
        (vacuum)auditus_iubere(&i);
        finis = ultima + X * M;
        dum (auditus_momentum() < finis || !census_lectus)
        {
            dum (auditus_nuntium_legere(&n))
            {
                si (n.genus == MIXTURA_NUNTIUS_VOX_FINITA)
                {
                    finitae++;
                }
                alioquin si (n.genus == MIXTURA_NUNTIUS_CENSUS)
                {
                    census         = n.census;
                    census_lectus  = VERUM;
                }
                mixtura_rector_tractare(rector, &n);
            }
            _dormire_ms(X);
            si (auditus_momentum() > finis + XLVIII * M)
            {
                frange;   /* census numquam venit: fractum infra */
            }
        }
    }
    nunc = auditus_momentum();
    auditus_claudere();

    imprimere("auditus_fumus: horologium %ld -> %ld (%.2f s provecta)\n",
              (longus)primum, (longus)nunc,
              (duplex)(nunc - primum) / (duplex)SONUS_FREQUENTIA);
    imprimere("auditus_fumus: iussa %u, voces finitae %u/%u\n", iussa,
              finitae, (i32)FUMUS_SONITUS);
    si (census_lectus)
    {
        imprimere("auditus_fumus: census - tarda %u, irrita %u, amissa %u, "
                  "nuntii amissi %u, apex %.3f\n", census.iussa_tarda,
                  census.iussa_irrita, census.iussa_amissa,
                  census.nuntii_amissi, (duplex)census.apex);
    }
    piscina_destruere(piscina);
    si (   !census_lectus || iussa != FUMUS_SONITUS
        || finitae != FUMUS_SONITUS || nunc <= primum)
    {
        fprintf(stderr, "auditus_fumus: FRACTUM\n");
        redde I;
    }
    imprimere("auditus_fumus: BENE\n");
    redde ZEPHYRUM;
}
