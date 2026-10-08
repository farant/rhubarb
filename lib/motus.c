/* motus.c - genus mobile in memoria */

#include "motus.h"

#include <string.h>


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
    memset(&motus->ramus, ZEPHYRUM, magnitudo(InsulaRamus));
    motus->spatium.mensura  = ZEPHYRUM;
    motus->spatium.datum    = NIHIL;
    motus->gestus.status    = NIHIL;
    motus->gestus.effusor   = NIHIL;
    motus->gestus.ctx       = NIHIL;
    motus->gestus.quies_ms  = ZEPHYRUM;
    motus->gestus.tempus    = ZEPHYRUM;
    motus->gestus.sordidus  = FALSUM;
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
    InsulaRamus ramus;

    si (!motus || !repo)
    {
        redde FALSUM;
    }
    /* T3a: in ramum activum (nullus = radix) */
    ramus = motus->ramus.repo ? motus->ramus : insula_ramus_radix(repo);
        nulla.mensura = ZEPHYRUM;
    nulla.datum = NIHIL;
    insula_scriptorem_ponere(repo, chorda_ex_literis("motus",
        motus->piscina));
    ok = mutare_ramum(&ramus, INSULA_EPHEMERA, effusio_mutator, motus);
    insula_scriptorem_ponere(repo, nulla);
    si (ok)
    {
        motus->sordida = FALSUM;
    }
    redde ok;
}


/* ==================================================
 * Gestus (scriba-plan S1a)
 * ================================================== */

vacuum
motus_gestum_ponere (
                 Motus* motus,
                vacuum* gestus,
    MotusGestusEffusor  effusor,
                vacuum* ctx,
                   s64  quies_ms)
{
    si (!motus)
    {
        redde;
    }
    motus->gestus.status    = gestus;
    motus->gestus.effusor   = effusor;
    motus->gestus.ctx       = ctx;
    motus->gestus.quies_ms  = quies_ms;
    motus->gestus.sordidus  = FALSUM;
}

vacuum
mutare_gestum (
           Motus* motus,
    MotusMutator  fn,
          vacuum* ctx,
             s64  tempus)
{
    si (!motus || !fn || !motus->gestus.status)
    {
        redde;
    }
    fn(motus, ctx);
    motus->gestus.tempus    = tempus;
    motus->gestus.sordidus  = VERUM;
}

b32
motus_gestus_quies (
    constans Motus* motus,
               s64  nunc)
{
    si (!motus || !motus->gestus.status || !motus->gestus.sordidus)
    {
        redde FALSUM;
    }
    redde (nunc - motus->gestus.tempus) >= motus->gestus.quies_ms
        ? VERUM : FALSUM;
}

b32
motus_gestum_effundere (
                 Motus* motus,
    InsulaRepositorium* repo)
{
       b32 ok;
    chorda prior;

    si (   !motus || !repo || !motus->gestus.status
        || !motus->gestus.effusor
        || !motus->gestus.sordidus)
    {
        redde FALSUM;
    }
    /* scriptor prior restituitur: effusio etiam intra actionem
     * vocatur (scriba: Esc, dd), quae deinde sub nomine suo scribit */
    prior = repo->scriptor;
    insula_scriptorem_ponere(repo, chorda_ex_literis("gestus",
        motus->piscina));
    ok = motus->gestus.effusor(motus->gestus.status, repo,
                               motus->gestus.ctx);
    insula_scriptorem_ponere(repo, prior);
    si (ok)
    {
        motus->gestus.sordidus = FALSUM;
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
