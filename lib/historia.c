/* historia.c - cauda actorum -> proiectio (ex pictor_documentum) */

#include "historia.h"
#include "stml.h"
#include "xar.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

/* nullum chorda_ex_s64 in domo: per f64 sine decimalibus (exactum
 * infra 2^53), ut eventus_stml tempus scribit */
interior chorda
seq_chorda (
        s64  seq,
    Piscina* piscina)
{
    redde chorda_ex_f64((f64)seq, ZEPHYRUM, piscina);
}

interior vacuum
sigillum_renovare (
    Historia* h)
{
    h->sigillum = sigillum_computare(h->proiectio.memoria,
                                     h->proiectio.mensura);
}

interior s32
attributum_s32 (
             StmlNodus* n,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = stml_attributum_capere(n, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior vacuum
vacare (
    Historia* h)
{
    h->proiectio.vacare(h->proiectio.ctx);
}

interior vacuum
applicare (
    Historia* h,
      chorda  actum)
{
    h->proiectio.applicare(h->proiectio.ctx, actum);
}


/* ==================================================
 * Acta viva et checkpoints
 * ================================================== */

/* acta viva post 'post' usque ad 'ad' (inclusive): generis clientis
 * soli (volumen sua acta interserit - volumen-creatum,
 * plagula-condita - quae hic nihil sunt), rami honorati: reddit Xar
 * de VolumenActum ordine seq */
interior Xar*
acta_viva (
    Historia* h,
         s64  post,
         s64  ad)
{
             Xar* omnia;
             Xar* viva;
    VolumenActum* a;
    VolumenActum* sedes;
             i32  i;
             i32  n;
             i32  k;
             s64  ab;
    StmlResultus  res;

    omnia  = volumen_acta_legere(h->volumen, post, h->piscina);
    viva   = xar_creare(h->piscina, (i32)magnitudo(VolumenActum));
    n      = xar_numerus(omnia);
    per (i = ZEPHYRUM; i < n; i++)
    {
        a = (VolumenActum*)xar_obtinere(omnia, i);
        si (a->seq > ad)
        {
            frange;
        }
        si (chorda_aequalis_literis(a->genus, "ramus"))
        {
            res = stml_legere_ex_literis(chorda_ut_cstr(a->datum,
                h->piscina), h->piscina, h->intern);
            ab = res.successus
                ? (s64)attributum_s32(res.elementum_radix, "ab",
                ZEPHYRUM)
                : ZEPHYRUM;
            /* tollere viva cum seq > ab */
            k = xar_numerus(viva);
            dum (   k > ZEPHYRUM
                 && ((VolumenActum*)xar_obtinere(viva, k - I))->seq
                      > ab)
            {
                k--;
            }
            xar_truncare(viva, k);
            perge;
        }
        si (!chorda_aequalis_literis(a->genus, h->genus))
        {
            perge;
        }
        sedes   = (VolumenActum*)xar_addere(viva);
        *sedes  = *a;
    }
    redde viva;
}

/* seq in actis vivis? */
interior b32
seq_vivum (
    constans Xar* viva,
             s64  seq)
{
    i32 i;
    i32 n;

    n = xar_numerus(viva);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (((VolumenActum*)xar_obtinere(viva, i))->seq == seq)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* checkpoint proximus: plagula 'checkpoint/<seq>' cum seq maximo
 * <= ad ET vivo; redde seq (0 = nullus). Plagulae enumerantur -
 * seqs multipla intervalli non sunt (volumen acta sua interserit). */
interior s64
checkpoint_proximus (
         Historia* h,
              s64  ad,
     constans Xar* viva)
{
               Xar* plagulae;
    VolumenPlagula* pl;
            chorda  praefixum;
            chorda  cauda;
               s32  s;
               s64  optimum;
               i32  i;
               i32  n;

    praefixum  = chorda_ex_literis("checkpoint/", h->piscina);
    plagulae   = volumen_plagulas_enumerare(h->volumen, h->piscina);
    optimum    = ZEPHYRUM;
    n          = xar_numerus(plagulae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        pl = (VolumenPlagula*)xar_obtinere(plagulae, i);
        si (!chorda_incipit(pl->via, praefixum))
        {
            perge;
        }
        cauda.datum    = pl->via.datum + praefixum.mensura;
        cauda.mensura  = pl->via.mensura - praefixum.mensura;
        si (!chorda_ut_s32(cauda, &s))
        {
            perge;
        }
        si ((s64)s <= ad && (s64)s > optimum && seq_vivum(viva, (s64)s))
        {
            optimum = (s64)s;
        }
    }
    redde optimum;
}

/* proiectio ad seq 'ad': checkpoint proximus vivus <= ad, deinde
 * acta viva post eum; sine checkpoint ex vacuo */
interior vacuum
proicere_ad (
    Historia* h,
         s64  ad,
         b32  sine_checkpoint)
{
             Xar* viva;
    VolumenActum* a;
             i32  i;
             i32  n;
             s64  basis;
          chorda  hex;
          chorda  massa;
             b32  inventum;
          chorda  clavis;

    viva   = acta_viva(h, ZEPHYRUM, ad);
    basis  = ZEPHYRUM;
    si (!sine_checkpoint)
    {
        basis = checkpoint_proximus(h, ad, viva);
        si (basis > ZEPHYRUM)
        {
            clavis = chorda_concatenare(
                chorda_ex_literis("checkpoint/", h->piscina),
                seq_chorda(basis, h->piscina), h->piscina);
            hex = volumen_plagulam_promere(h->volumen, clavis,
                                           h->piscina, &inventum);
            massa = inventum
                  ? volumen_massam_promere(h->volumen, hex, h->piscina,
                                           &inventum)
                  : hex;
            si (   inventum
                && massa.mensura == (i32)h->proiectio.mensura)
            {
                memcpy(h->proiectio.memoria, massa.datum,
                       h->proiectio.mensura);
            }
            alioquin
            {
                basis = ZEPHYRUM;
            }
        }
    }
    si (basis == ZEPHYRUM)
    {
        vacare(h);
    }
    n = xar_numerus(viva);
    per (i = ZEPHYRUM; i < n; i++)
    {
        a = (VolumenActum*)xar_obtinere(viva, i);
        si (a->seq > basis)
        {
            applicare(h, a->datum);
        }
    }
    sigillum_renovare(h);
}

interior vacuum
checkpoint_condere (
    Historia* h,
         s64  seq)
{
    character hex[SIGILLUM_HEX_MENSURA];
       chorda contentum;
       chorda clavis;

    contentum.datum    = h->proiectio.memoria;
    contentum.mensura  = (i32)h->proiectio.mensura;
    si (!volumen_massam_condere(h->volumen, contentum, hex))
    {
        redde;
    }
    clavis = chorda_concatenare(chorda_ex_literis("checkpoint/",
        h->piscina),
                                seq_chorda(seq, h->piscina),
                                h->piscina);
    volumen_plagulam_condere(h->volumen, clavis,
                             chorda_ex_literis(hex, h->piscina),
                             h->origo_checkpoint);
}


/* ==================================================
 * Vita
 * ================================================== */

interior Historia*
historia_struere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* genus,
     constans character* origo_checkpoint,
                    i32  intervallum,
      HistoriaProiectio  proiectio)
{
    Historia* h;

    si (   !piscina || !intern || !volumen || !genus
        || !origo_checkpoint || !proiectio.memoria
        || proiectio.mensura == ZEPHYRUM || !proiectio.vacare
        || !proiectio.applicare)
    {
        redde NIHIL;
    }
    h = (Historia*)piscina_allocare(piscina, magnitudo(*h));
    si (!h)
    {
        redde NIHIL;
    }
    memset(h, ZEPHYRUM, magnitudo(Historia));
    h->volumen           = volumen;
    h->piscina           = piscina;
    h->intern            = intern;
    h->proiectio         = proiectio;
    h->genus             = genus;
    h->origo_checkpoint  = origo_checkpoint;
    h->intervallum       = intervallum > ZEPHYRUM ? intervallum : LXIV;
    redde h;
}

Historia*
historia_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* genus,
     constans character* origo_checkpoint,
                    i32  intervallum,
      HistoriaProiectio  proiectio)
{
    Historia* h;

    h = historia_struere(piscina, intern, volumen, genus,
                         origo_checkpoint, intervallum, proiectio);
    si (!h)
    {
        redde NIHIL;
    }
    vacare(h);
    sigillum_renovare(h);
    redde h;
}

Historia*
historia_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* genus,
     constans character* origo_checkpoint,
                    i32  intervallum,
      HistoriaProiectio  proiectio)
{
    Historia* h;
         Xar* viva;
         i32  n;

    h = historia_struere(piscina, intern, volumen, genus,
                         origo_checkpoint, intervallum, proiectio);
    si (!h)
    {
        redde NIHIL;
    }
    /* finis = seq ultimum vivum; cursor = finis */
    viva  = acta_viva(h, ZEPHYRUM, volumen_summa_actorum(volumen));
    n     = xar_numerus(viva);
    h->finis = n > ZEPHYRUM
             ? ((VolumenActum*)xar_obtinere(viva, n - I))->seq
             : ZEPHYRUM;
    h->cursor           = h->finis;
    h->numerus_vivorum  = n;
    proicere_ad(h, h->cursor, FALSUM);
    redde h;
}


/* ==================================================
 * Acta, revocare, reficere
 * ================================================== */

s64
historia_actum (
    Historia* h,
      chorda  actum)
{
       s64 seq;
    chorda ramus;

    si (!h || chorda_vacua(actum))
    {
        redde ZEPHYRUM;
    }
    si (h->cursor < h->finis)
    {
        h->numerus_vivorum =
            xar_numerus(acta_viva(h, ZEPHYRUM, h->cursor));
        ramus = chorda_ex_literis("<ramus ab=\"", h->piscina);
        ramus = chorda_concatenare(ramus,
                                   seq_chorda(h->cursor, h->piscina),
                                   h->piscina);
        ramus = chorda_concatenare(ramus,
                                   chorda_ex_literis("\"/>",
                                   h->piscina),
                                   h->piscina);
        volumen_actum_appendere(h->volumen, "ramus", ramus);
    }
    seq = volumen_actum_appendere(h->volumen, h->genus, actum);
    si (seq <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    applicare(h, actum);
    h->cursor  = seq;
    h->finis   = seq;
    h->numerus_vivorum++;
    sigillum_renovare(h);
    si (h->numerus_vivorum % h->intervallum == ZEPHYRUM)
    {
        checkpoint_condere(h, seq);
    }
    redde seq;
}

b32
historia_revocare (
    Historia* h)
{
    Xar* viva;
    i32  n;
    s64  ad;

    si (!h || h->cursor <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* actum vivum proximum infra cursor */
    viva  = acta_viva(h, ZEPHYRUM, h->cursor - I);
    n     = xar_numerus(viva);
    ad = n > ZEPHYRUM ? ((VolumenActum*)xar_obtinere(viva, n - I))->seq
                      : ZEPHYRUM;
    h->cursor = ad;
    h->numerus_vivorum--;
    proicere_ad(h, ad, FALSUM);
    redde VERUM;
}

b32
historia_reficere (
    Historia* h)
{
             Xar* viva;
    VolumenActum* a;
             i32  i;
             i32  n;

    si (!h || h->cursor >= h->finis)
    {
        redde FALSUM;
    }
    viva  = acta_viva(h, h->cursor, h->finis);
    n     = xar_numerus(viva);
    per (i = ZEPHYRUM; i < n; i++)
    {
        a = (VolumenActum*)xar_obtinere(viva, i);
        si (a->seq > h->cursor)
        {
            applicare(h, a->datum);
            h->cursor = a->seq;
            h->numerus_vivorum++;
            sigillum_renovare(h);
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Lectio et verificatio
 * ================================================== */

b32
historia_verificare (
    Historia* h)
{
    Sigillum ante;

    si (!h)
    {
        redde FALSUM;
    }
    ante = h->sigillum;
    proicere_ad(h, h->cursor, VERUM);
    redde sigillum_aequale(&ante, &h->sigillum);
}

chorda
historia_sigillum_hex (
    constans Historia* h,
              Piscina* piscina)
{
    character hex[SIGILLUM_HEX_MENSURA];
       chorda vacua;

    si (!h)
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    sigillum_hex(&h->sigillum, hex);
    redde chorda_ex_literis(hex, piscina);
}

s64
historia_cursor (
    constans Historia* h)
{
    redde h ? h->cursor : ZEPHYRUM;
}

s64
historia_finis (
    constans Historia* h)
{
    redde h ? h->finis : ZEPHYRUM;
}

i32
historia_numerus_vivorum (
    constans Historia* h)
{
    redde h ? h->numerus_vivorum : ZEPHYRUM;
}
