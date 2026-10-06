/* pseudoterminale.c - pars portabilis: configuratio, nomina errorum,
 * pons memoriae (sutura probationum). Pons posix in
 * pseudoterminale_posix.c (per conventionem aedilis). */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "pseudoterminale.h"
#include <string.h>

/* decisio VIII: hospes TERM_PROGRAM ipse addit */
hic_manens constans character* constans ambitus_ordinarius[] = {
    "TERM=xterm-256color",
    "COLORTERM=truecolor",
    NIHIL
};

vacuum
pseudoterminale_configuratio_initiare (
    PseudoterminaleConfiguratio* cfg)
{
    si (!cfg)
    {
        redde;
    }
    cfg->argumenta    = NIHIL;
    cfg->ambitus      = ambitus_ordinarius;
    cfg->directorium  = NIHIL;
    cfg->latitudo     = LXXX;
    cfg->altitudo     = XXIV;
    cfg->px_latitudo  = ZEPHYRUM;
    cfg->px_altitudo  = ZEPHYRUM;
}

constans character*
pseudoterminale_error_nomen (
    PseudoterminaleError error)
{
    commutatio (error)
    {
        casus PSEUDOTERMINALE_OK:
            redde "nullus error";
        casus PSEUDOTERMINALE_ERROR_ARGUMENTA:
            redde "argumenta mala";
        casus PSEUDOTERMINALE_ERROR_APERIRE:
            redde "pseudo-terminale non apertum";
        casus PSEUDOTERMINALE_ERROR_GENERARE:
            redde "infans non generatus";
        casus PSEUDOTERMINALE_ERROR_EXEC:
            redde "exec fractum";
    }
    redde "error ignotus";
}


/* ==================================================
 * Pons memoriae
 * ================================================== */

nomen structura {
       Pseudoterminale  pons;       /* pons.datum = haec structura */
                    i8* effusio;    /* copiata */
                   i32  effusio_mensura;
                   i32  cursor;
                   i32  codex;
     ChordaAedificator* captum;
                   i32  latitudo;
                   i32  altitudo;
                   b32  clausum;
} PseudoterminaleMemoriae;

interior s32
memoria_legere (
    vacuum* datum,
        i8* buffer,
       i32  capacitas,
       s32  mora_ms)
{
    PseudoterminaleMemoriae* m;
                        i32  n;

    (vacuum)mora_ms;
    m = (PseudoterminaleMemoriae*)datum;
    si (m->clausum || m->cursor >= m->effusio_mensura)
    {
        redde -I;
    }
    n = m->effusio_mensura - m->cursor;
    si (n > capacitas)
    {
        n = capacitas;
    }
    memcpy(buffer, m->effusio + m->cursor, (memoriae_index)n);
    m->cursor += n;
    redde (s32)n;
}

interior s32
memoria_scribere (
          vacuum* datum,
     constans i8* octeti,
             i32  n)
{
    PseudoterminaleMemoriae* m;
                     chorda  c;
    unio {
         constans i8* c;
                  i8* m;
    } u;

    m = (PseudoterminaleMemoriae*)datum;
    si (m->clausum)
    {
        redde -I;
    }
    u.c        = octeti;
    c.datum    = u.m;
    c.mensura  = n;
    si (!chorda_aedificator_appendere_chorda(m->captum, c))
    {
        redde -I;
    }
    redde (s32)n;
}

interior b32
memoria_amplitudo (
    vacuum* datum,
       i32  latitudo,
       i32  altitudo,
       i32  px_latitudo,
       i32  px_altitudo)
{
    PseudoterminaleMemoriae* m;

    (vacuum)px_latitudo;
    (vacuum)px_altitudo;
    m = (PseudoterminaleMemoriae*)datum;
    si (m->clausum || latitudo < I || altitudo < I)
    {
        redde FALSUM;
    }
    m->latitudo = latitudo;
    m->altitudo = altitudo;
    redde VERUM;
}

/* infans fictus exit cum effusio tota lecta est */
interior b32
memoria_finitus (
                   vacuum* datum,
    PseudoterminaleExitus* exitus)
{
    PseudoterminaleMemoriae* m;

    m = (PseudoterminaleMemoriae*)datum;
    si (m->cursor < m->effusio_mensura)
    {
        redde FALSUM;
    }
    exitus->codex   = m->codex;
    exitus->signum  = ZEPHYRUM;
    redde VERUM;
}

interior s32
memoria_fossa (
    vacuum* datum)
{
    (vacuum)datum;
    redde -I;
}

interior vacuum
memoria_claudere (
    vacuum* datum)
{
    ((PseudoterminaleMemoriae*)datum)->clausum = VERUM;
}

Pseudoterminale*
pseudoterminale_memoriae_creare (
         Piscina* piscina,
     constans i8* effusio,
             i32  n,
             i32  codex)
{
    PseudoterminaleMemoriae* m;

    si (!piscina || (n > ZEPHYRUM && !effusio))
    {
        redde NIHIL;
    }
    m = (PseudoterminaleMemoriae*)piscina_allocare(piscina,
        magnitudo(PseudoterminaleMemoriae));
    memset(m, ZEPHYRUM, magnitudo(PseudoterminaleMemoriae));
    m->effusio = (i8*)piscina_allocare(piscina,
        (memoriae_index)(n + I));
    si (n > ZEPHYRUM)
    {
        memcpy(m->effusio, effusio, (memoriae_index)n);
    }
    m->effusio_mensura  = n;
    m->codex            = codex;
    m->latitudo         = LXXX;
    m->altitudo         = XXIV;
    m->captum           = chorda_aedificator_creare(piscina, CCLVI);
    si (!m->captum)
    {
        redde NIHIL;
    }
    m->pons.datum      = m;
    m->pons.legere     = memoria_legere;
    m->pons.scribere   = memoria_scribere;
    m->pons.amplitudo  = memoria_amplitudo;
    m->pons.finitus    = memoria_finitus;
    m->pons.fossa      = memoria_fossa;
    m->pons.claudere   = memoria_claudere;
    redde &m->pons;
}

chorda
pseudoterminale_memoriae_captum (
    constans Pseudoterminale* pt)
{
    redde chorda_aedificator_spectare(
        ((PseudoterminaleMemoriae*)pt->datum)->captum);
}

vacuum
pseudoterminale_memoriae_amplitudo (
    constans Pseudoterminale* pt,
                         i32* latitudo,
                         i32* altitudo)
{
    constans PseudoterminaleMemoriae* m;

    m          = (constans PseudoterminaleMemoriae*)pt->datum;
    *latitudo  = m->latitudo;
    *altitudo  = m->altitudo;
}
