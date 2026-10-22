/* auditus_posix.c - exitus soni sine suggestu soni (vide
 * include/auditus.h)
 *
 * Suggestus sine exitu nostro (Linux, ceteri): aperire FALSUM - vocans
 * sine sono pergit, numquam fractum. Variantes aedilis (macos posix):
 * lib/auditus_macos.m in macOS.
 */
#include "auditus.h"

b32
auditus_aperire (
                  Mixtura* mixtura,
    constans AuditusForma* forma,
                  Piscina* piscina)
{
    (vacuum)mixtura;
    (vacuum)forma;
    (vacuum)piscina;
    redde FALSUM;
}

vacuum
auditus_claudere (
    vacuum)
{
}

b32
auditus_iubere (
    constans MixturaIussum* iussum)
{
    (vacuum)iussum;
    redde FALSUM;
}

b32
auditus_nuntium_legere (
    MixturaNuntius* nuntius)
{
    (vacuum)nuntius;
    redde FALSUM;
}

s64
auditus_momentum (
    vacuum)
{
    redde ZEPHYRUM;
}

i32
auditus_mora (
    vacuum)
{
    redde ZEPHYRUM;
}
