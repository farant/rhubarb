/* recusanda.c - fixum lanceatorum (probatio_officina_lanceatores,
 * 'interpretare -nativum'): functio quam dorsum nativum RECUSAT, ut
 * via recusationis lanceatoris (causa in stderr, cursus interpretatus,
 * fructus idem) exemplum stabile habeat. Recursio MUTUA: dorsum
 * functionem in opere vocatam recusat (limes notus M5; functiones
 * bibliothecarum scalares omnes iam nativae sunt - exemplum
 * color_srgb_ad_linearem a T9 gradu II compilatur).
 *
 * Usus: interpretare -fons <haec plagula> -nativum fixum_est_par 10 */

#include "latina.h"

s32
fixum_est_par (
    s32 numerus);

s32
fixum_est_impar (
    s32 numerus);

s32
fixum_est_par (
    s32 numerus)
{
    si (numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde fixum_est_impar(numerus - I);
}

s32
fixum_est_impar (
    s32 numerus)
{
    si (numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde fixum_est_par(numerus - I);
}
