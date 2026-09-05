/* motus.c - genus mobile in memoria */

#include "motus.h"


/* ==================================================
 * Vita et porta
 * ================================================== */

vacuum
motus_initiare (
      Motus* motus,
    Piscina* piscina)
{
    si (!motus)
    {
        redde;
    }
    motus->captura.mensura  = ZEPHYRUM;
    motus->captura.datum    = NIHIL;
    motus->ictus_pendens   = xar_creare(piscina,
                                        (i32)magnitudo(Punctum));
    motus->pan.x                      = ZEPHYRUM;
    motus->pan.y                      = ZEPHYRUM;
    motus->zoom                       = I;
    motus->tempus_ultimae_mutationis  = ZEPHYRUM;
    motus->sordida                    = FALSUM;
    motus->piscina                    = piscina;
}

vacuum
mutare_motum (
            Motus* motus,
     MotusMutator  fn,
           vacuum* ctx,
              s64  tempus)
{
    si (!motus || !fn)
    {
        redde;
    }
    fn(motus, ctx);
    motus->tempus_ultimae_mutationis  = tempus;
    motus->sordida                    = VERUM;
}

b32
motus_quies (
    constans Motus* motus,
               s64  nunc,
               s64  quies_ms)
{
    si (!motus || !motus->sordida)
    {
        redde FALSUM;
    }
    redde (nunc - motus->tempus_ultimae_mutationis) >= quies_ms
        ? VERUM : FALSUM;
}


/* ==================================================
 * Effusio
 * ================================================== */

interior constans character*
numerus_ut_cstr (
        s32  valor,
    Piscina* piscina)
{
    redde chorda_ut_cstr(chorda_ex_s32(valor, piscina), piscina);
}

/* Mutator effusionis: campi persistendi soli (pan, zoom). Ponere,
 * non addere - effusio altera substituit. */
interior vacuum
effusio_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans Motus* motus;

    motus = (constans Motus*)ctx;
    insula_attributum_ponere(radix, p, in, "pan_x",
                             numerus_ut_cstr(motus->pan.x, p));
    insula_attributum_ponere(radix, p, in, "pan_y",
                             numerus_ut_cstr(motus->pan.y, p));
    insula_attributum_ponere(radix, p, in, "zoom",
                             numerus_ut_cstr((s32)motus->zoom, p));
}

/* <quies/> */
b32
motus_effundere (
                 Motus* motus,
    InsulaRepositorium* repo)
{
        b32 ok;
     chorda nulla;

    si (!motus || !repo)
    {
        redde FALSUM;
    }
        nulla.mensura  = ZEPHYRUM;
    nulla.datum        = NIHIL;
    insula_scriptorem_ponere(repo, chorda_ex_literis("motus",
        motus->piscina));
    ok = mutare_ephemera(repo, effusio_mutator, motus);
    insula_scriptorem_ponere(repo, nulla);
    si (ok)
    {
        motus->sordida = FALSUM;
    }
    redde ok;
}


/* ==================================================
 * Captura
 * ================================================== */

vacuum
motus_captura_ponere (
     Motus* motus,
    chorda  id)
{
    si (!motus)
    {
        redde;
    }
    motus->captura = id;
}

vacuum
motus_captura_tollere (
    Motus* motus)
{
    si (!motus)
    {
        redde;
    }
    motus->captura.mensura  = ZEPHYRUM;
    motus->captura.datum    = NIHIL;
}
