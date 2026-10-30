/* pictor_componentia.c - componere pictoris */

#include "pictor_componentia.h"
#include "communio_cliens.h"
#include "xar.h"
#include "dispositio.h"
#include "pictor_documentum.h"
#include "exemplaria.h"

#include <stdio.h>
#include <string.h>


/* ==================================================
 * Auxilia (lectio insularum)
 * ================================================== */

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior chorda
attributum_chorda (
    constans InsulaRamus* ramus,
      constans character* titulus)
{
    chorda* a;
    chorda  vacua;

    a = insula_ramus_attributum(ramus, titulus);
    si (a)
    {
        redde *a;
    }
    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    redde vacua;
}

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  w,
                    s32  h)
{
     Componens* c;
         Fines  f;

    c           = componens_creare(p, in, id, partes);
    f.x         = x;
    f.y         = y;
    f.latitudo  = w;
    f.altitudo  = h;
    componens_ponere_fines(c, f);
    redde c;
}

#define PINACES_PAGINAE  XII

/* pinacotheca D3 (spec): dialogus super prospectum - rete XII
 * pinacum (III x IV altum, IV x III latum: forma superficiei), infra
 * linea ‹ "p / n" › X (sagittae solum si paginae plures).
 * Dispositio in cellulis: columna (spatium I) = rete crescens +
 * linea fixa III;
 * rete = ordines crescentes, cellulae crescentes (intervallum I).
 * Pinax: "pinax:<lat>x<alt>:<seq>:<spatium>[:electum]" (imago intra
 * marginem II) */
/* titulus pinacis k (latitudo x altitudo imaginis) - rete commune
 * pinacothecae et communium (plan-5 T6) */
nomen chorda (*TitulusPinacis)(constans vacuum* ctx, s32 k,
    s32 latitudo, s32 altitudo, Piscina* piscina);

/* rete XII pinacum: dialogus 'nomen', nodi "<praefixum><k>", bullae
 * "<nomen>.prior|pagina|posterior|claudere", actio "<nomen>.agere";
 * numerus 0 et 'vacua' -> textus "<nomen>.vacua" in medio */
interior Componens*
rete_componere (
                        Piscina* piscina,
            InternamentumChorda* intern,
             constans character* titulus_dialogi,
             constans character* praefixum,
                            s32  numerus,
                            s32  pagina,
                 TitulusPinacis  titulus_pinacis,
                constans vacuum* ctx,
             constans character* vacua,
                          Fines  fp,
                            s32  latitudo_superficiei,
                            s32  altitudo_superficiei,
                            s32  cw,
                            s32  ch)
{
           Componens* dialogus;
           Componens* n;
          Dispositio* d;
     DispositioForma  forma;
                 s32  columnae;
                 s32  ordines;
                 s32  d_radix;
                 s32  d_rete;
                 s32  d_ordo;
                 s32  d_linea;
                 s32  d_cellula[PINACES_PAGINAE];
                 s32  d_prior;
                 s32  d_pagina;
                 s32  d_posterior;
                 s32  d_claudere;
                 s32  paginae;
                 s32  k;
                 s32  r;
                 s32  i;
               Fines  f;
           character  id[LXIV];
           character  t[CXXVIII];

    paginae  = numerus > ZEPHYRUM ? (numerus + XI) / PINACES_PAGINAE
                                  : I;
    si (pagina < ZEPHYRUM)
    {
        pagina = ZEPHYRUM;
    }
    si (pagina >= paginae)
    {
        pagina = paginae - I;
    }
    /* forma superficiei, non platforma (spec D3) */
    columnae  = latitudo_superficiei > altitudo_superficiei ? IV : III;
    ordines   = PINACES_PAGINAE / columnae;

    d = dispositio_creare(piscina);
    dispositio_formam_initiare(&forma);
    forma.directio           = DISPOSITIO_COLUMNA;
    forma.latitudo.genus     = DISPOSITIO_CRESCENS;
    forma.altitudo.genus     = DISPOSITIO_CRESCENS;
    forma.spatium_sinistrum  = I;
    forma.spatium_dextrum    = I;
    forma.spatium_superum    = I;
    forma.spatium_inferum    = I;
    forma.intervallum        = I;
    d_radix                  = dispositio_addere(d, -I, &forma);
    dispositio_formam_initiare(&forma);
    forma.directio        = DISPOSITIO_COLUMNA;
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_CRESCENS;
    forma.intervallum     = I;
    d_rete                = dispositio_addere(d, d_radix, &forma);
    per (r = ZEPHYRUM; r < ordines; r++)
    {
        dispositio_formam_initiare(&forma);
        forma.directio        = DISPOSITIO_LINEA;
        forma.latitudo.genus  = DISPOSITIO_CRESCENS;
        forma.altitudo.genus  = DISPOSITIO_CRESCENS;
        forma.intervallum     = I;
        d_ordo                = dispositio_addere(d, d_rete, &forma);
        per (k = ZEPHYRUM; k < columnae; k++)
        {
            dispositio_formam_initiare(&forma);
            forma.latitudo.genus = DISPOSITIO_CRESCENS;
            forma.altitudo.genus = DISPOSITIO_CRESCENS;
            d_cellula[r * columnae + k] = dispositio_addere(d, d_ordo,
                &forma);
        }
    }
    dispositio_formam_initiare(&forma);
    forma.directio        = DISPOSITIO_LINEA;
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_FIXA;
    forma.altitudo.valor  = III;
    forma.intervallum     = I;
    d_linea               = dispositio_addere(d, d_radix, &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus = DISPOSITIO_FIXA;
    forma.latitudo.valor = IV;
    forma.altitudo.genus = DISPOSITIO_FIXA;
    forma.altitudo.valor = III;
    d_prior      = paginae > I ? dispositio_addere(d, d_linea, &forma)
                               : -I;
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_FIXA;
    forma.altitudo.valor  = III;
    d_pagina              = dispositio_addere(d, d_linea, &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus = DISPOSITIO_FIXA;
    forma.latitudo.valor = IV;
    forma.altitudo.genus = DISPOSITIO_FIXA;
    forma.altitudo.valor = III;
    d_posterior  = paginae > I ? dispositio_addere(d, d_linea, &forma)
                               : -I;
    d_claudere   = dispositio_addere(d, d_linea, &forma);
    dispositio_computare(d, fp.latitudo / cw, fp.altitudo / ch, NIHIL,
        NIHIL);

    dialogus = nodus(piscina, intern, titulus_dialogi, PARTES_DIALOGUS,
        fp.x, fp.y, fp.latitudo, fp.altitudo);
    sprintf(t, "%s.agere", titulus_dialogi);
    per (i = ZEPHYRUM; i < PINACES_PAGINAE; i++)
    {
        k = pagina * PINACES_PAGINAE + i;
        si (k >= numerus)
        {
            frange;
        }
        f = dispositio_ad_pixela(dispositio_fines(d, d_cellula[i]),
            fp.latitudo / cw, fp.altitudo / ch, fp.latitudo,
            fp.altitudo, cw, ch);
        sprintf(id, "%s%lu", praefixum, (insignatus longus)i);
        n = nodus(piscina, intern, id, PARTES_BOTTONE, f.x, f.y,
            f.latitudo, f.altitudo);
        /* imago intra marginem II (figura quadrati) */
        componens_ponere_titulum(n, chorda_ut_cstr(titulus_pinacis(ctx,
            k,
            f.latitudo - IV, f.altitudo - IV, piscina), piscina));
        componens_ponere_actio(n, t);
        componens_addere_liberum(dialogus, n);
    }
    si (numerus == ZEPHYRUM && vacua)
    {
        f = dispositio_ad_pixela(dispositio_fines(d, d_rete),
            fp.latitudo / cw, fp.altitudo / ch, fp.latitudo,
            fp.altitudo, cw, ch);
        sprintf(id, "%s.vacua", titulus_dialogi);
        n = nodus(piscina, intern, id, PARTES_BOTTONE, f.x, f.y,
            f.latitudo, f.altitudo);
        componens_ponere_titulum(n, chorda_ut_cstr(chorda_concatenare(
            chorda_ex_literis("pagina:", piscina), chorda_ex_literis(
            vacua, piscina), piscina), piscina));
        componens_addere_liberum(dialogus, n);
    }
    {
        hic_manens constans character* ID[IV] = {
            "prior", "pagina", "posterior", "claudere"
        };
        s32 indices[IV];

        indices[ZEPHYRUM]  = d_prior;
        indices[I]         = d_pagina;
        indices[II]        = d_posterior;
        indices[III]       = d_claudere;
        per (i = ZEPHYRUM; i < IV; i++)
        {
            si (indices[i] < ZEPHYRUM)
            {
                perge;
            }
            f = dispositio_ad_pixela(dispositio_fines(d, indices[i]),
                fp.latitudo / cw, fp.altitudo / ch, fp.latitudo,
                fp.altitudo, cw, ch);
            sprintf(id, "%s.%s", titulus_dialogi, ID[i]);
            n = nodus(piscina, intern, id, PARTES_BOTTONE, f.x, f.y,
                f.latitudo, f.altitudo);
            si (i == I)
            {
                character pg[XXXII];

                sprintf(pg, "pagina:%ld / %ld", (longus)(pagina + I),
                    (longus)paginae);
                componens_ponere_titulum(n, pg);
            }
            alioquin
            {
                componens_ponere_titulum(n, i == ZEPHYRUM
                    ? "sagitta:sinistra" : i == II ? "sagitta:dextra"
                    : "textus:X");
                componens_ponere_actio(n, t);
            }
            componens_addere_liberum(dialogus, n);
        }
    }
    redde dialogus;
}

/* pinacotheca: pinax "pinax:<lat>x<alt>:<seq>:<spatium>[:electum]" */
nomen structura {
    constans PictorPinacotheca* pc;
                        chorda  aperta;
} TitulusPinacothecae;

interior chorda
_titulus_pinacothecae (
    constans vacuum* ctx,
                s32  k,
                s32  latitudo,
                s32  altitudo,
            Piscina* piscina)
{
    constans TitulusPinacothecae* tp =
        (constans TitulusPinacothecae*)ctx;
                   PictorPictura* p;
                       character  t[LXIV];

    p = (PictorPictura*)xar_obtinere(tp->pc->picturae, (i32)k);
    sprintf(t, "pinax:%ldx%ld:%ld:", (longus)latitudo, (longus)altitudo,
        (longus)p->seq);
    redde chorda_concatenare(chorda_concatenare(chorda_ex_literis(t,
        piscina), p->spatium, piscina), chorda_ex_literis(
        chorda_aequalis(p->spatium, tp->aperta) ? ":electum" : "",
        piscina), piscina);
}

interior Componens*
pinacothecam_componere (
                       Piscina* piscina,
           InternamentumChorda* intern,
    constans PictorPinacotheca* pc,
          constans InsulaRamus* ramus,
                         Fines  fp,
                           s32  latitudo_superficiei,
                           s32  altitudo_superficiei,
                           s32  cw,
                           s32  ch)
{
    TitulusPinacothecae tp;

    tp.pc      = pc;
    tp.aperta  = attributum_chorda(ramus, "pictura");
    redde rete_componere(piscina, intern, "pinacotheca", "pinax.",
        (pc
            && pc->picturae) ? (s32)xar_numerus(pc->picturae) : ZEPHYRUM,
        attributum_s32(ramus, "pagina_pinacothecae", ZEPHYRUM),
        _titulus_pinacothecae, &tp, NIHIL, fp, latitudo_superficiei,
        altitudo_superficiei, cw, ch);
}

/* communia: "alienus:<lat>x<alt>:<hora>:<replica>:<spatium>" */
interior chorda
_titulus_alieni (
    constans PictorPicturaAliena* a,
                             s32  latitudo,
                             s32  altitudo,
                         Piscina* piscina)
{
    character t[LXIV];

    sprintf(t, "alienus:%ldx%ld:%ld:", (longus)latitudo,
        (longus)altitudo, (longus)a->hora);
    redde chorda_concatenare(chorda_concatenare(chorda_concatenare(
        chorda_ex_literis(t, piscina), a->replica, piscina),
        chorda_ex_literis(":", piscina), piscina), a->spatium, piscina);
}

interior chorda
_titulus_communium (
    constans vacuum* ctx,
                s32  k,
                s32  latitudo,
                s32  altitudo,
            Piscina* piscina)
{
    constans PictorCommunia* pc = (constans PictorCommunia*)ctx;

    redde _titulus_alieni((PictorPicturaAliena*)xar_obtinere(
        pc->picturae, (i32)k), latitudo, altitudo, piscina);
}

/* "replica:spatium" -> partes (ULID sine ':') */
interior b32
_spectatam_partire (
    chorda  spectata,
    chorda* replica,
    chorda* spatium)
{
    i32 i;

    per (i = ZEPHYRUM; i < spectata.mensura && spectata.datum[i] != ':';
         i++)
    {
    }
    si (i == ZEPHYRUM || i >= spectata.mensura)
    {
        redde FALSUM;
    }
    replica->datum    = spectata.datum;
    replica->mensura  = i;
    spatium->datum    = spectata.datum + i + I;
    spatium->mensura  = spectata.mensura - i - I;
    redde VERUM;
}

/* spec IX.6: spectator - pictura aliena lectione sola, linea "by
 * Persona · Titulus", infra ‹ "Edit a copy" › X. Imago (nodus
 * "spectator.imago") tractus sinister/dexter fert (actio); dialogus
 * ipse ictum extra imaginem capit (claudit ad rete). NIHIL si
 * spectata abest (vocans rete componit). */
interior Componens*
spectatorem_componere (
                      Piscina* piscina,
          InternamentumChorda* intern,
    constans PictorCompositio* cfg,
         constans InsulaRamus* ramus,
                        Fines  fp,
                          s32  cw,
                          s32  ch)
{
                Componens* dialogus;
                Componens* n;
               Dispositio* d;
          DispositioForma  forma;
      PictorPicturaAliena* a;
                   chorda  replica;
                   chorda  spatium;
                   chorda  auctor;
                      s32  index;
                      s32  d_radix;
                      s32  d_imago;
                      s32  d_auctor;
                      s32  d_linea;
                      s32  d_bullae[IV];
                      s32  i;
                    Fines  f;

    si (   !cfg->communia
        || !_spectatam_partire(attributum_chorda(ramus,
            "spectata"), &replica, &spatium))
    {
        redde NIHIL;
    }
    index = pictor_communia_index(cfg->communia, replica, spatium);
    si (index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    a = (PictorPicturaAliena*)xar_obtinere(cfg->communia->picturae,
        (i32)index);
    d = dispositio_creare(piscina);
    dispositio_formam_initiare(&forma);
    forma.directio           = DISPOSITIO_COLUMNA;
    forma.latitudo.genus     = DISPOSITIO_CRESCENS;
    forma.altitudo.genus     = DISPOSITIO_CRESCENS;
    forma.spatium_sinistrum  = I;
    forma.spatium_dextrum    = I;
    forma.spatium_superum    = I;
    forma.spatium_inferum    = I;
    forma.intervallum        = I;
    d_radix                  = dispositio_addere(d, -I, &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_CRESCENS;
    d_imago               = dispositio_addere(d, d_radix, &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_FIXA;
    forma.altitudo.valor  = II;
    d_auctor              = dispositio_addere(d, d_radix, &forma);
    dispositio_formam_initiare(&forma);
    forma.directio        = DISPOSITIO_LINEA;
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_FIXA;
    forma.altitudo.valor  = III;
    forma.intervallum     = I;
    d_linea               = dispositio_addere(d, d_radix, &forma);
    per (i = ZEPHYRUM; i < IV; i++)
    {
        dispositio_formam_initiare(&forma);
        forma.altitudo.genus = DISPOSITIO_FIXA;
        forma.altitudo.valor = III;
        si (i == I)
        {
            forma.latitudo.genus = DISPOSITIO_CRESCENS;
        }
        alioquin
        {
            forma.latitudo.genus = DISPOSITIO_FIXA;
            forma.latitudo.valor = IV;
        }
        d_bullae[i] = dispositio_addere(d, d_linea, &forma);
    }
    dispositio_computare(d, fp.latitudo / cw, fp.altitudo / ch, NIHIL,
        NIHIL);
    dialogus = nodus(piscina, intern, "spectator", PARTES_DIALOGUS,
        fp.x,
        fp.y, fp.latitudo, fp.altitudo);
    componens_ponere_actio(dialogus, "spectator.agere");
    f = dispositio_ad_pixela(dispositio_fines(d, d_imago), fp.latitudo
        / cw, fp.altitudo / ch, fp.latitudo, fp.altitudo, cw, ch);
    n = nodus(piscina, intern, "spectator.imago", PARTES_BOTTONE, f.x,
        f.y,
        f.latitudo, f.altitudo);
    componens_ponere_titulum(n, chorda_ut_cstr(_titulus_alieni(a,
        f.latitudo - IV, f.altitudo - IV, piscina), piscina));
    componens_ponere_actio(n, "spectator.agere");
    componens_addere_liberum(dialogus, n);
    auctor = cfg->auctor ? cfg->auctor(cfg->auctor_ctx, a->replica,
        piscina) : chorda_ex_literis("", piscina);
    si (auctor.mensura > ZEPHYRUM)
    {
        f = dispositio_ad_pixela(dispositio_fines(d, d_auctor),
            fp.latitudo / cw, fp.altitudo / ch, fp.latitudo,
            fp.altitudo,
            cw, ch);
        n = nodus(piscina, intern, "spectator.auctor", PARTES_BOTTONE,
            f.x, f.y, f.latitudo, f.altitudo);
        componens_ponere_titulum(n, chorda_ut_cstr(chorda_concatenare(
            chorda_ex_literis("pagina:by ", piscina), auctor, piscina),
            piscina));
        componens_addere_liberum(dialogus, n);
    }
    {
        hic_manens constans character* ID[IV] = {
            "spectator.prior", "spectator.exemplum",
            "spectator.posterior", "spectator.claudere"
        };
        hic_manens constans character* TITULI[IV] = {
            "sagitta:sinistra", "textus:Edit a copy", "sagitta:dextra",
            "textus:X"
        };

        per (i = ZEPHYRUM; i < IV; i++)
        {
            f = dispositio_ad_pixela(dispositio_fines(d, d_bullae[i]),
                fp.latitudo / cw, fp.altitudo / ch, fp.latitudo,
                fp.altitudo, cw, ch);
            n = nodus(piscina, intern, ID[i], PARTES_BOTTONE, f.x, f.y,
                f.latitudo, f.altitudo);
            componens_ponere_titulum(n, TITULI[i]);
            componens_ponere_actio(n, "spectator.agere");
            componens_addere_liberum(dialogus, n);
        }
    }
    redde dialogus;
}

/* communia (spec IX.5): rete picturarum alienarum, vacuum -> textus */
interior Componens*
communia_componere (
                      Piscina* piscina,
          InternamentumChorda* intern,
    constans PictorCompositio* cfg,
         constans InsulaRamus* ramus,
                        Fines  fp,
                          s32  latitudo_superficiei,
                          s32  altitudo_superficiei,
                          s32  cw,
                          s32  ch)
{
    constans PictorCommunia* pc = cfg->communia;

    redde rete_componere(piscina, intern, "communia", "communia.pinax.",
        (pc
            && pc->picturae) ? (s32)xar_numerus(pc->picturae) : ZEPHYRUM,
        attributum_s32(ramus, "pagina_communium", ZEPHYRUM),
        _titulus_communium, pc, cfg->inscripta
        ? "No shared pictures yet." : "Sign in to see shared pictures.",
        fp, latitudo_superficiei, altitudo_superficiei, cw, ch);
}

/* plan-5 T7: dialogus formae columnae - lineae fixae (altitudines in
 * cellulis) deinde spatium crescens; nodi per ordinem in 'fines' */
#define LINEAE_DIALOGI  X

interior i32
_dialogum_disponere (
    constans s32* altitudines,
             i32  numerus,
           Fines  fp,
             s32  cw,
             s32  ch,
         Piscina* piscina,
           Fines* fines)
{
         Dispositio* d;
    DispositioForma  forma;
                s32  d_radix;
                s32  indices[LINEAE_DIALOGI];
                i32  i;

    d = dispositio_creare(piscina);
    dispositio_formam_initiare(&forma);
    forma.directio           = DISPOSITIO_COLUMNA;
    forma.latitudo.genus     = DISPOSITIO_CRESCENS;
    forma.altitudo.genus     = DISPOSITIO_CRESCENS;
    forma.spatium_sinistrum  = I;
    forma.spatium_dextrum    = I;
    forma.spatium_superum    = II;
    forma.spatium_inferum    = I;
    forma.intervallum        = I;
    d_radix                  = dispositio_addere(d, -I, &forma);
    per (i = ZEPHYRUM; i < numerus && i < LINEAE_DIALOGI; i++)
    {
        dispositio_formam_initiare(&forma);
        forma.latitudo.genus = DISPOSITIO_CRESCENS;
        si (altitudines[i] > ZEPHYRUM)
        {
            forma.altitudo.genus = DISPOSITIO_FIXA;
            forma.altitudo.valor = altitudines[i];
        }
        alioquin
        {
            forma.altitudo.genus = DISPOSITIO_CRESCENS;
        }
        indices[i] = dispositio_addere(d, d_radix, &forma);
    }
    dispositio_computare(d, fp.latitudo / cw, fp.altitudo / ch, NIHIL,
        NIHIL);
    per (i = ZEPHYRUM; i < numerus && i < LINEAE_DIALOGI; i++)
    {
        fines[i] = dispositio_ad_pixela(dispositio_fines(d, indices[i]),
            fp.latitudo / cw, fp.altitudo / ch, fp.latitudo,
            fp.altitudo,
            cw, ch);
    }
    redde numerus;
}

interior Componens*
_nodum_addere (
               Piscina* piscina,
   InternamentumChorda* intern,
             Componens* dialogus,
    constans character* id,
                 Fines  f,
    constans character* titulus,
    constans character* actio)
{
    Componens* n;

    n = nodus(piscina, intern, id, PARTES_BOTTONE, f.x, f.y, f.latitudo,
        f.altitudo);
    componens_ponere_titulum(n, titulus);
    si (actio)
    {
        componens_ponere_actio(n, actio);
    }
    componens_addere_liberum(dialogus, n);
    redde n;
}

/* linea bullarum: duae bullae dimidiae in f */
interior vacuum
_bullas_duas_addere (
               Piscina* piscina,
   InternamentumChorda* intern,
             Componens* dialogus,
                 Fines  f,
    constans character* id_a,
    constans character* titulus_a,
    constans character* id_b,
    constans character* titulus_b,
    constans character* actio,
                   s32  cw)
{
    Fines a;
    Fines b;

    a           = f;
    a.latitudo  = (f.latitudo - cw) / II;
    b           = a;
    b.x         = f.x + a.latitudo + cw;
    (vacuum)_nodum_addere(piscina, intern, dialogus, id_a, a, titulus_a,
        actio);
    (vacuum)_nodum_addere(piscina, intern, dialogus, id_b, b, titulus_b,
        actio);
}

/* spec IX.1: salutatio prima - "pictor", Sign in…, Draw without
 * signing in */
interior Componens*
salutationem_componere (
               Piscina* piscina,
   InternamentumChorda* intern,
                 Fines  fp,
                   s32  cw,
                   s32  ch)
{
    hic_manens constans s32 ALTITUDINES[V] = { III, ZEPHYRUM, III, III,
        ZEPHYRUM };
     Componens* dialogus;
         Fines  f[V];

    (vacuum)_dialogum_disponere(ALTITUDINES, V, fp, cw, ch, piscina, f);
    dialogus = nodus(piscina, intern, "salutatio", PARTES_DIALOGUS,
        fp.x,
        fp.y, fp.latitudo, fp.altitudo);
    (vacuum)_nodum_addere(piscina, intern, dialogus,
        "salutatio.titulus",
        f[ZEPHYRUM], "pagina:pictor", NIHIL);
    (vacuum)_nodum_addere(piscina, intern, dialogus,
        "salutatio.inscribere",
        f[II], "textus:Sign in\xE2\x80\xA6", "salutatio.agere");
    (vacuum)_nodum_addere(piscina, intern, dialogus,
        "salutatio.pingere",
        f[III], "textus:Draw without signing in", "salutatio.agere");
    redde dialogus;
}

/* spec IX.2: inscriptio - gradus ex hospite (-1 nulla): I epistula
 * (cum errore RETE/EXPIRATA), II codex (exspectans; codex falsus), III
 * "Signed in as ..." + OK. Campus "campus:<textus>" (inscriptio_textus);
 * error localis 'inscriptio_error' = "codex". */
interior Componens*
inscriptionem_componere (
                      Piscina* piscina,
          InternamentumChorda* intern,
    constans PictorCompositio* cfg,
         constans InsulaRamus* ramus,
                        Fines  fp,
                          s32  cw,
                          s32  ch)
{
    hic_manens constans s32 PRIMUS[VI] = { II, III, II, III, ZEPHYRUM,
        ZEPHYRUM };
    hic_manens constans s32 SECUNDUS[VIII] = { II, II, II, II, III, II,
        III, ZEPHYRUM };
    hic_manens constans s32  TERTIUS[III] = { II, III, ZEPHYRUM };
                  Componens* dialogus;
                      Fines  f[VIII];
                        s32  g;
                     chorda  textus;
                     chorda  error;
                     chorda  persona;
         constans character* nuntius;

    g = cfg->inscriptionis_gradus ? cfg->inscriptionis_gradus(
        cfg->auctor_ctx) : -I;
    textus  = attributum_chorda(ramus, "inscriptio_textus");
    error   = attributum_chorda(ramus, "inscriptio_error");
    dialogus = nodus(piscina, intern, "inscriptio", PARTES_DIALOGUS,
        fp.x,
        fp.y, fp.latitudo, fp.altitudo);
    si (g == (s32)INSCRIPTIO_INSCRIPTA)
    {
        (vacuum)_dialogum_disponere(TERTIUS, III, fp, cw, ch, piscina,
            f);
        persona = cfg->persona ? cfg->persona(cfg->auctor_ctx, piscina)
                               : chorda_ex_literis("", piscina);
        (vacuum)_nodum_addere(piscina, intern, dialogus,
            "inscriptio.titulus", f[ZEPHYRUM], chorda_ut_cstr(
            persona.mensura > ZEPHYRUM ? chorda_concatenare(
            chorda_ex_literis("pagina:Signed in as ", piscina), persona,
            piscina) : chorda_ex_literis("pagina:Signed in", piscina),
            piscina), NIHIL);
        (vacuum)_nodum_addere(piscina, intern, dialogus,
            "inscriptio.confirmata", f[I], "textus:OK",
            "inscriptio.agere");
        redde dialogus;
    }
    si (   g == (s32)INSCRIPTIO_PETIT || g == (s32)INSCRIPTIO_EXSPECTAT
        || g == (s32)INSCRIPTIO_CONFIRMAT
        || g == (s32)INSCRIPTIO_CODEX_FALSUS)
    {
        (vacuum)_dialogum_disponere(SECUNDUS, VIII, fp, cw, ch, piscina,
            f);
        (vacuum)_nodum_addere(piscina, intern, dialogus,
            "inscriptio.titulus", f[ZEPHYRUM],
            "pagina:Check your email",
            NIHIL);
        (vacuum)_nodum_addere(piscina, intern, dialogus,
            "inscriptio.nota",
            f[I], "pagina:Tap the link we sent,", NIHIL);
        (vacuum)_nodum_addere(piscina, intern, dialogus,
            "inscriptio.nota_altera", f[II], "pagina:or type the code"
            " here.", NIHIL);
        (vacuum)_nodum_addere(piscina, intern, dialogus,
            "inscriptio.signum", f[III], "pagina:Code", NIHIL);
        (vacuum)_nodum_addere(piscina, intern, dialogus,
            "inscriptio.campus", f[IV],
            chorda_ut_cstr(chorda_concatenare(
            chorda_ex_literis("campus:", piscina), textus, piscina),
            piscina), "inscriptio.agere");
        si (   g == (s32)INSCRIPTIO_CODEX_FALSUS
            || chorda_aequalis_literis(error, "codex"))
        {
            (vacuum)_nodum_addere(piscina, intern, dialogus,
                "inscriptio.error", f[V],
                "pagina:That code isn't right.",
                NIHIL);
        }
        _bullas_duas_addere(piscina, intern, dialogus, f[VI],
            "inscriptio.abicere", "textus:Cancel", "inscriptio.mittere",
            "textus:Send", "inscriptio.agere", cw);
        redde dialogus;
    }
    (vacuum)_dialogum_disponere(PRIMUS, VI, fp, cw, ch, piscina, f);
    (vacuum)_nodum_addere(piscina, intern, dialogus,
        "inscriptio.titulus",
        f[ZEPHYRUM], "pagina:Email address", NIHIL);
    (vacuum)_nodum_addere(piscina, intern, dialogus,
        "inscriptio.campus",
        f[I], chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(
        "campus:", piscina), textus, piscina), piscina),
        "inscriptio.agere");
    nuntius = g == (s32)INSCRIPTIO_RETE
            ? "pagina:Can't reach the server. Try again."
            : g == (s32)INSCRIPTIO_EXPIRATA
            ? "pagina:This sign-in expired. Start again." : NIHIL;
    si (nuntius)
    {
        (vacuum)_nodum_addere(piscina, intern, dialogus,
            "inscriptio.error",
            f[II], nuntius, NIHIL);
    }
    _bullas_duas_addere(piscina, intern, dialogus, f[III],
        "inscriptio.abicere", "textus:Cancel", "inscriptio.mittere",
        "textus:Send", "inscriptio.agere", cw);
    redde dialogus;
}

PictorElenchiCondiciones
pictor_elenchi_condiciones (
    constans PictorCompositio* cfg)
{
    PictorElenchiCondiciones k;

    memset(&k, ZEPHYRUM, magnitudo(k));
    si (cfg)
    {
        k.importare  = cfg->imago_eligibilis;
        k.communio   = cfg->communio_praesto;
        k.inscripta  = cfg->inscripta;
    }
    redde k;
}

vacuum
pictor_elenchum_parare (
     constans PictorElenchiCondiciones* c,
                           ElenchusRes  res[V],
                              Elenchus* e)
{
    i32 n;

    memset(res, ZEPHYRUM, magnitudo(ElenchusRes) * V);
    n                = ZEPHYRUM;
    res[n].titulus   = "pinacotheca.aperire";
    res[n++].textus  = "My pictures\xE2\x80\xA6";
    si (c && c->communio)
    {
        res[n].titulus   = "communia.aperire";
        res[n++].textus  = "Shared pictures\xE2\x80\xA6";
    }
    res[n].titulus   = "pictura.nova";
    res[n++].textus  = "New picture";
    si (c && c->importare)
    {
        res[n].titulus   = "imago.eligere";
        res[n++].textus  = "Import image\xE2\x80\xA6";
    }
    si (c && c->communio && !c->inscripta)
    {
        res[n].titulus   = "inscriptio.aperire";
        res[n++].textus  = "Sign in\xE2\x80\xA6";
    }
    per (e->numerus = ZEPHYRUM; e->numerus < (i32)n; e->numerus++)
    {
        res[e->numerus].clavis   = NIHIL;
        res[e->numerus].habilis  = VERUM;
        res[e->numerus].divisor  = FALSUM;
    }
    e->res     = res;
    e->electa  = -I;
}

constans character*
pictor_actio_instrumenti (
    chorda instrumentum)
{
    si (chorda_aequalis_literis(instrumentum, "penicillus"))
    {
        redde "penicillus.ictus";
    }
    si (chorda_aequalis_literis(instrumentum, "aspergillum"))
    {
        redde "aspergillum.ictus";
    }
    si (chorda_aequalis_literis(instrumentum, "spongia"))
    {
        redde "spongia.ictus";
    }
    si (chorda_aequalis_literis(instrumentum, "linea"))
    {
        redde "linea.ictus";
    }
    redde "";
}


/* ==================================================
 * Componere
 * ================================================== */

/* P1a: quadratum lineae status (PARTES_BOTTONE, XX x XX) filium
 * status addere */
interior Componens*
quadratum_addere (
                Piscina* piscina,
    InternamentumChorda* intern,
              Componens* status,
     constans character* id,
                    s32  x,
                    s32  y,
     constans character* titulus)
{
    Componens* q;

    q = nodus(piscina, intern, id, PARTES_BOTTONE, x, y, XX, XX);
    componens_ponere_titulum(q, titulus);
    /* P1b: ictus palettam suam aperit */
    componens_ponere_actio(q, "palette.aperire");
    componens_addere_liberum(status, q);
    redde q;
}

/* P4a: magnitudines penicilli (pixela) - eaedem ac
 * pictor_magnitudinem_ponere accipit */
hic_manens constans s32 magnitudines[X] = {
    I, II, III, IV, VI, VIII, XII, XVI, XXXII, LXIV
};

/* P4b: magnitudines aspergilli (multiplex radii) - eaedem */
hic_manens constans s32 magnitudines_aspergilli[V] = {
    I, II, IV, VIII, XVI
};

/* linea: latitudines (pixela) - eaedem */
hic_manens constans s32 magnitudines_lineae[V] = {
    I, II, IV, VIII, XVI
};

/* P1b: palette (PARTES_DIALOGUS) supra quadratum (x, y radicis):
 * optiones XX x XX, II inter, IV margo, VI per lineam (exemplaria X).
 * genus 'instrumentum', 'color_primus'/'color_secundus' aut
 * 'exemplar' (P3; optiones coloribus primo et secundo, ut
 * quadratum); electum = valor currens (titulus ':electum').
 * Palette intra [limes_x0, limes_x1) superficiei manet: angusta
 * (telephonum, CCXV) optiones pauciores per lineam habet, deinde
 * sinistrorsum movetur (Franus 2026-10-24, testa-ios T4) */
interior Componens*
palettam_componere (
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda  genus,
                    s32  x,
                    s32  y_quadrati,
                    s32  limes_x0,
                    s32  limes_x1,
                 chorda  instrumentum,
                    s32  color_currens,
                    s32  primus,
                    s32  secundus)
{
              Componens* palette;
              Componens* o;
                    b32  colores;
                    b32  exemplaria;
                    b32  mensurae;
                    b32  aspergilli;
                    s32  n;
                    s32  per_lineam;
                    s32  capacitas;
                    s32  k;
                    s32  latitudo;
                    s32  altitudo;
                    s32  valor;
              character  id[LXIV];
              character  titulus[LXIV];
     constans character* instrumenta[IV];
                    b32  lineae;

    instrumenta[ZEPHYRUM]  = "penicillus";
    instrumenta[I]         = "aspergillum";
    instrumenta[II]        = "spongia";
    instrumenta[III]       = "linea";
    exemplaria             = chorda_aequalis_literis(genus, "exemplar");
    mensurae = chorda_aequalis_literis(genus,
        "magnitudo");
    aspergilli  = mensurae && chorda_aequalis_literis(instrumentum,
        "aspergillum");
    lineae      = mensurae && chorda_aequalis_literis(instrumentum,
        "linea");
    colores     = !exemplaria && !mensurae
               && !chorda_aequalis_literis(genus, "instrumentum");
    n           = exemplaria ? (s32)EXEMPLAR_NUMERUS
                : (aspergilli || lineae) ? V
                : mensurae ? X
                : colores ? XVII : IV;
    /* P3: exemplaria (IV lineae) et magnitudines X per lineam, cetera
     * VI */
    per_lineam  = (exemplaria || mensurae) ? X : VI;
    capacitas   = (limes_x1 - limes_x0 - VIII + II) / (XX + II);
    si (capacitas >= I && per_lineam > capacitas)
    {
        per_lineam = capacitas;
    }
    latitudo = IV + (n < per_lineam ? n : per_lineam) * (XX + II) - II
             + IV;
    altitudo = IV + ((n + per_lineam - I) / per_lineam) * (XX + II) - II
             + IV;
    si (x + latitudo > limes_x1)
    {
        x = limes_x1 - latitudo;
    }
    si (x < limes_x0)
    {
        x = limes_x0;
    }
    palette   = nodus(piscina, intern, "palette", PARTES_DIALOGUS, x,
        y_quadrati - altitudo - II, latitudo, altitudo);
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (lineae)
        {
            sprintf(id, "optio.magnitudo.%d",
                (integer)magnitudines_lineae[k]);
            sprintf(titulus, "magnitudo:linea:%d%s",
                (integer)magnitudines_lineae[k],
                magnitudines_lineae[k]
                    == color_currens ? ":electum" : "");
        }
        alioquin si (mensurae && aspergilli)
        {
            sprintf(id, "optio.magnitudo.%d",
                (integer)magnitudines_aspergilli[k]);
            sprintf(titulus, "magnitudo:aspergillum:%d%s",
                (integer)magnitudines_aspergilli[k],
                magnitudines_aspergilli[k] == color_currens ? ":electum"
                                                            : "");
        }
        alioquin si (mensurae)
        {
            sprintf(id, "optio.magnitudo.%d", (integer)magnitudines[k]);
            sprintf(titulus, "magnitudo:%d%s", (integer)magnitudines[k],
                magnitudines[k] == color_currens ? ":electum" : "");
        }
        alioquin si (exemplaria)
        {
            /* Franus: optiones coloribus electis (ut quadratum) */
            sprintf(id, "optio.exemplar.%d", (integer)k);
            sprintf(titulus, "exemplar:%d:%d:%d%s", (integer)k,
                (integer)primus, (integer)secundus,
                k == color_currens ? ":electum" : "");
        }
        alioquin si (colores)
        {
            valor = k - I;   /* -1 nullus, deinde 0..15 */
            sprintf(id, "optio.%s.%d", chorda_ut_cstr(genus, piscina),
                (integer)valor);
            sprintf(titulus, "color:%d%s", (integer)valor,
                valor == color_currens ? ":electum" : "");
        }
        alioquin
        {
            sprintf(id, "optio.instrumentum.%s", instrumenta[k]);
            sprintf(titulus, "instrumentum:%s%s", instrumenta[k],
                chorda_aequalis_literis(instrumentum, instrumenta[k])
                    ? ":electum" : "");
        }
        o = nodus(piscina, intern, id, PARTES_BOTTONE,
            IV + (k % per_lineam) * (XX + II),
            IV + (k / per_lineam) * (XX + II), XX, XX);
        componens_ponere_titulum(o, titulus);
        /* actio = dominus attributi (domini.stml) */
        componens_ponere_actio(o, mensurae ? "magnitudo.ponere"
            : exemplaria ? "exemplar.ponere"
            : !colores ? "instrumentum.eligere"
            : chorda_aequalis_literis(genus, "color_primus")
            ? "color_primus.ponere" : "color_secundus.ponere");
        /* S5c: electio palettam claudit (dimissio post actionem) */
        o->claudit = VERUM;
        componens_addere_liberum(palette, o);
    }
    redde palette;
}

/* L3: stratum currens (ut in actionibus): ephemera stratum_activum si
 * in documento, aliter summum */
interior s32
stratum_currens_compositum (
           constans InsulaRamus* ramus,
      constans PictorDocumentum* doc)
{
    chorda* a;
       s32  id;
       i32  k;
       i32  n;

    n = pictor_documentum_numerus_stratorum(doc);
    a = insula_ramus_attributum(ramus,
        "stratum_activum");
    si (a && chorda_ut_s32(*a, &id))
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (pictor_documentum_stratum(doc, k)->id == id)
            {
                redde id;
            }
        }
    }
    redde n > ZEPHYRUM ? pictor_documentum_stratum(doc, n - I)->id : I;
}

/* L4: ordo destinatus tractus (summo primo) - eadem regula ac
 * ordo_destinatus actionum (pictor_actiones.c) */
interior s32
ordo_tractus (
    s32 ordo_tracti,
    s32 dy,
    i32 n)
{
    s32 gradus;
    s32 r;

    gradus  = dy >= ZEPHYRUM ? (dy + XI) / (XX + II)
                             : -((-dy + XI) / (XX + II));
    r = ordo_tracti + gradus;
    r = r < ZEPHYRUM ? ZEPHYRUM : r;
    r = r > (s32)n - I ? (s32)n - I : r;
    redde r;
}

/* L4: index insertionis ('strata.index', II altus, in rima inter
 * ordines) dum ordo trahitur: captura 'stratum.<id>', puncta II,
 * motus > III, ordo destinatus alius */
interior vacuum
indicem_tractus_addere (
                    Piscina* piscina,
        InternamentumChorda* intern,
                  Componens* palette,
             constans Motus* motus,
  constans PictorDocumentum* doc)
{
    Componens* o;
      Punctum  p0;
      Punctum  p1;
          s32  dx;
          s32  dy;
          s32  id;
          s32  r_tracti;
          s32  r;
          i32  n;
          i32  k;
       chorda  valor;

    si (   !motus || !motus->ictus_pendens
        || xar_numerus(motus->ictus_pendens) < II
        || motus->captura.mensura                         <= VIII
        || memcmp(motus->captura.datum, "stratum.", VIII) != ZEPHYRUM)
    {
        redde;
    }
    valor.datum = motus->captura.datum + VIII;
    valor.mensura = motus->captura.mensura - VIII;
    p0 = *(Punctum*)xar_obtinere(motus->ictus_pendens, ZEPHYRUM);
    p1 = *(Punctum*)xar_obtinere(motus->ictus_pendens, I);
    dx = p1.x - p0.x;
    dy = p1.y - p0.y;
    si (!chorda_ut_s32(valor, &id) || dx * dx + dy * dy <= IX)
    {
        redde;
    }
    n         = pictor_documentum_numerus_stratorum(doc);
    r_tracti  = -I;
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (pictor_documentum_stratum(doc, k)->id == id)
        {
            r_tracti = (s32)n - I - (s32)k;
        }
    }
    si (r_tracti < ZEPHYRUM)
    {
        redde;
    }
    r = ordo_tractus(r_tracti, dy, n);
    si (r == r_tracti)
    {
        redde;
    }
    o = nodus(piscina, intern, "strata.index", PARTES_BOTTONE,
        IV + XX + II, IV + r * (XX + II) + (r > r_tracti ? XX : -II), C,
        II);
    componens_ponere_titulum(o, "strata:index");
    componens_addere_liberum(palette, o);
}

/* L3: palette stratorum (PARTES_DIALOGUS 'palette') supra quadratum,
 * margine dextro ad dextrum eius (x_dextrum): ordines summo primo -
 * oculus (XX x XX, 'oculus.<id>', "oculus:<0|1>") et nomen (C x XX,
 * 'stratum.<id>', "stratum:<id>[:electum]"); infra '+' et '-'.
 * Intra [limes_x0, ...) manet (testa-ios T4: margo sinister) */
interior Componens*
strata_palettam_componere (
                    Piscina* piscina,
        InternamentumChorda* intern,
  constans PictorDocumentum* doc,
                        s32  currens,
                        s32  x_dextrum,
                        s32  y_quadrati,
                        s32  limes_x0)
{
               Componens* palette;
               Componens* o;
                     i32  n;
                     i32  k;
                     s32  r;
                     s32  latitudo;
                     s32  altitudo;
               character  id[XXXII];
               character  titulus[XXXII];
  constans PictorStratum* s;

    n         = pictor_documentum_numerus_stratorum(doc);
    latitudo  = IV + XX + II + C + IV;
    altitudo  = IV + ((s32)n + I) * (XX + II) - II + IV;
    palette   = nodus(piscina, intern, "palette", PARTES_DIALOGUS,
        x_dextrum - latitudo < limes_x0 ? limes_x0
                                        : x_dextrum - latitudo,
        y_quadrati - altitudo - II, latitudo, altitudo);
    per (k = ZEPHYRUM; k < n; k++)
    {
        /* summum primum */
        s = pictor_documentum_stratum(doc, n - I - k);
        r = IV + (s32)k * (XX + II);
        sprintf(id, "oculus.%d", (integer)s->id);
        sprintf(titulus, "oculus:%d", s->visibile ? I : ZEPHYRUM);
        o = nodus(piscina, intern, id, PARTES_BOTTONE, IV, r, XX, XX);
        componens_ponere_titulum(o, titulus);
        componens_ponere_actio(o, "strata.agere");
        componens_addere_liberum(palette, o);
        sprintf(id, "stratum.%d", (integer)s->id);
        sprintf(titulus, "stratum:%d%s", (integer)s->id,
            s->id == currens ? ":electum" : "");
        o = nodus(piscina, intern, id, PARTES_BOTTONE, IV + XX + II, r,
            C,
            XX);
        componens_ponere_titulum(o, titulus);
        componens_ponere_actio(o, "strata.agere");
        componens_addere_liberum(palette, o);
    }
    r = IV + (s32)n * (XX + II);
    o = nodus(piscina, intern, "strata.novum", PARTES_BOTTONE, IV, r,
        XX,
        XX);
    componens_ponere_titulum(o, "strata:novum");
    componens_ponere_actio(o, "strata.agere");
    componens_addere_liberum(palette, o);
    o = nodus(piscina, intern, "strata.deletum", PARTES_BOTTONE,
        IV + XX + II, r, XX, XX);
    componens_ponere_titulum(o, "strata:deletum");
    componens_ponere_actio(o, "strata.agere");
    componens_addere_liberum(palette, o);
    redde palette;
}

/* <componens/> <purus/> */
Componens*
pictor_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
             Componens* quadratum_stratorum;
             Componens* quadratum_elenchi;
           InsulaRamus  ramus;
      PictorCompositio* cfg;
             Componens* radix;
             Componens* prospectus;
             Componens* tabula;
             Componens* status;
                chorda  instrumentum;
                chorda  glutinatio;
                   b32  pendens;
                   s32  x_magnitudinis;   /* quadratum magnitudinis
                                           * lineae pendentis */
                   s32  doc_latitudo;
                   s32  doc_altitudo;
                   s32  zoom;
                   s32  latitudo;
                   s32  altitudo;
                   s32  cw;
                   s32  ch;
            Dispositio* dispositio;
       DispositioForma  forma;
                   s32  d_radix;
                   s32  d_prospectus;
                   s32  d_status;
                 Fines  fp;
                 Fines  fs;
                   i32  n;
                   i32  i;

    si (!repo || !piscina || !intern || !ctx)
    {
        redde NIHIL;
    }
    quadratum_stratorum  = NIHIL;
    quadratum_elenchi    = NIHIL;
    cfg                  = (PictorCompositio*)ctx;
    /* R3: status pictoris per ramum (sine eo radix repositorii) */
    ramus = cfg->ramus.repo ? cfg->ramus : insula_ramus_radix(repo);
    /* 013 B3: superficies STATUS est (dispensator scribit); ante
     * nuntium primum magnitudo configurata */
    latitudo = attributum_s32(&ramus,
        "superficies_latitudo",
        (s32)cfg->fenestra_latitudo);
    altitudo = attributum_s32(&ramus,
        "superficies_altitudo",
        (s32)cfg->fenestra_altitudo);
    cw = (cfg->cellula_latitudo > ZEPHYRUM) ? (s32)cfg->cellula_latitudo
                                            : I;
    ch = (cfg->cellula_altitudo > ZEPHYRUM) ? (s32)cfg->cellula_altitudo
                                            : I;

    /* dispositio in cellulis: columna = prospectus (crescens,
     * praecisus) super lineam status (fixa) - orae ad cellulas */
    dispositio = dispositio_creare(piscina);
    dispositio_formam_initiare(&forma);
    forma.directio        = DISPOSITIO_COLUMNA;
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_CRESCENS;
    d_radix               = dispositio_addere(dispositio, -I, &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_CRESCENS;
    forma.praecidere_x    = VERUM;
    forma.praecidere_y    = VERUM;
    d_prospectus = dispositio_addere(dispositio, d_radix,
        &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus = DISPOSITIO_CRESCENS;
    forma.altitudo.genus = DISPOSITIO_FIXA;
    forma.altitudo.valor = (s32)cfg->status_lineae;
    d_status = dispositio_addere(dispositio, d_radix,
        &forma);
    dispositio_computare(dispositio, latitudo / cw, altitudo / ch,
        NIHIL,
        NIHIL);
    fp = dispositio_ad_pixela(dispositio_fines(dispositio,
        d_prospectus), latitudo
        / cw,
        altitudo / ch, latitudo, altitudo, cw, ch);
    fs = dispositio_ad_pixela(dispositio_fines(dispositio, d_status),
        latitudo
        / cw,
        altitudo / ch, latitudo, altitudo, cw, ch);
    instrumentum = attributum_chorda(&ramus,
        "instrumentum");
    doc_latitudo = attributum_s32(&ramus, "latitudo",
        I);
    doc_altitudo = attributum_s32(&ramus, "altitudo",
        I);
    zoom = attributum_s32(&ramus, "zoom", I);
    si (zoom < I)
    {
        zoom = I;
    }
    /* glutinatio pendens (Franus 2026-10-13): campus et documentum
     * concordant - praevisio in tabula, linea status A / B / OK / X */
    glutinatio = attributum_chorda(&ramus, "glutinatio");
    pendens    = !chorda_vacua(glutinatio) && cfg->doc
              && cfg->doc->glutinatio.pendens;
    x_magnitudinis = ZEPHYRUM;
    /* pendens: tractus radit magnitudine PENICILLI (Franus 2026-10-13)
     * - palette magnitudinis ut penicilli, quidquid instrumentum */
    si (pendens)
    {
        instrumentum = chorda_ex_literis("penicillus", piscina);
    }

    radix = nodus(piscina, intern, "radix", PARTES_NULLUM, ZEPHYRUM,
                  ZEPHYRUM, latitudo, altitudo);
    componens_ponere_actio(radix, "instrumentum.eligere");

    prospectus = nodus(piscina, intern, "prospectus", PARTES_PROSPECTUS,
                       fp.x, fp.y, fp.latitudo, fp.altitudo);
    componens_ponere_sectio(prospectus, VERUM);
    /* indicium foci: figura prospectus exemplar addit */
    si (motus_ramus_focatus(motus, ramus))
    {
        componens_ponere_titulum(prospectus, "focatum");
    }
    componens_ponere_transformatio(prospectus,
        motus ? motus->pan.x : ZEPHYRUM,
        motus ? motus->pan.y : ZEPHYRUM,
        (i32)zoom);

    /* margo cellulae utrimque (ut folium scribae): margo paginae
     * (figura) in cellulis marginis visibilis, non extra prospectum */
    tabula = nodus(piscina, intern, "tabula", PARTES_TABULA, cw, ch,
                   doc_latitudo, doc_altitudo);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_focusabilis(tabula, VERUM);
    componens_ponere_actio(tabula, pendens ? "glutinatio.agere"
        : pictor_actio_instrumenti(instrumentum));
    si (pendens)
    {
        /* "glutinatio:<A|B>" - figura praevisionem ex documento
         * pingit */
        componens_ponere_titulum(tabula,
            chorda_aequalis_literis(glutinatio, "A") ? "glutinatio:A"
                                                     : "glutinatio:B");
    }
    /* puncta pendentia tabulae solae (L4: tractus ordinis stratorum
     * puncta quoque in motu fert - captura alia) */
    n = motus && motus->ictus_pendens
        && chorda_aequalis_literis(motus->captura, "tabula")
        ? xar_numerus(motus->ictus_pendens) : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        tabula->puncta = (Punctum*)piscina_allocare(piscina,
            (memoriae_index)n * magnitudo(Punctum));
        per (i = ZEPHYRUM; i < n; i++)
        {
            tabula->puncta[i] =
                *(Punctum*)xar_obtinere(motus->ictus_pendens,
                                                        i);
        }
        tabula->numerus_punctorum = n;
        /* aspergillum: praevisio guttarum - semen et radius in titulo
         * tabulae (data, ut puncta; figura ex eis guttas pingit) */
        si (chorda_aequalis_literis(instrumentum, "aspergillum"))
        {
            character titulus_guttarum[XLVIII];

            /* "semen radius color color_secundus exemplar" (Franus:
             * praevisio colore vero, non accentus; P3 exemplar) */
            sprintf(titulus_guttarum, "%ld %d %d %d %d",
                (longus)attributum_s32(&ramus, "semen",
                    ZEPHYRUM),
                (integer)(PICTOR_ASPERGILLI_RADIUS
                          * attributum_s32(&ramus,
                                "magnitudo_aspergilli", I)),
                (integer)attributum_s32(&ramus,
                    "color_primus", ZEPHYRUM),
                (integer)attributum_s32(&ramus,
                    "color_secundus", -I),
                (integer)attributum_s32(&ramus,
                    "exemplar", ZEPHYRUM));
            componens_ponere_titulum(tabula, titulus_guttarum);
        }
        /* spongia: latus quadrati in titulo (figura quadrata colore
         * fundi pingit) */
        si (chorda_aequalis_literis(instrumentum, "spongia"))
        {
            character titulus_spongiae[XVI];

            /* spongia: latus fixum (acta magnitudo="1") */
            sprintf(titulus_spongiae, "%d",
                (integer)PICTOR_SPONGIAE_LATUS);
            componens_ponere_titulum(tabula, titulus_spongiae);
        }
        /* P4a penicillus (et linea): "diametrus color" - color
         * praevisionis: primus, aut secundus si primus nullus sub
         * exemplari; -1 nihil */
        si (   chorda_aequalis_literis(instrumentum, "penicillus")
            || chorda_aequalis_literis(instrumentum, "linea"))
        {
            character titulus_penicilli[XXIV];
                  s32 primus;
                  s32 secundus;
                  s32 color;

            primus    = attributum_s32(&ramus,
                "color_primus", ZEPHYRUM);
            secundus  = attributum_s32(&ramus,
                "color_secundus", -I);
            color     = (primus >= ZEPHYRUM && primus < XVI) ? primus
                : (attributum_s32(&ramus, "exemplar",
                       ZEPHYRUM) != ZEPHYRUM
                   && secundus >= ZEPHYRUM && secundus < XVI) ? secundus
                : -I;
            sprintf(titulus_penicilli, "%d %d", (integer)attributum_s32(
                &ramus,
                chorda_aequalis_literis(instrumentum, "linea")
                ? "magnitudo_lineae" : "magnitudo_penicilli", I),
                (integer)color);
            componens_ponere_titulum(tabula, titulus_penicilli);
        }
    }

    status = nodus(piscina, intern, "status", PARTES_TITULUS, fs.x,
        fs.y,
                   fs.latitudo, fs.altitudo);
    /* testa-ios D6: nomen instrumenti non iam in linea - quadratum
     * instrumenti id dicit (linea angusta in telephono; Franus
     * 2026-10-23) */
    componens_ponere_titulum(status, "");

    /* glutinatio pendens: A / B / OK / X loco quadratorum */
    si (pendens)
    {
        hic_manens constans character* ID[IV] = {
            "glutinatio.A", "glutinatio.B", "glutinatio.confirmare",
            "glutinatio.abicere"
        };
        hic_manens constans character* TEXTUS[IV] = {"A", "B", "OK",
            "X"};
              hic_manens constans s32  LATITUDO[IV] = {XX, XX, XXX, XX};
                            character  t[XXIV];
                                  s32  y;
                                  s32  x;
                                  i32  k;
                            Componens* q;

        y = fs.altitudo > XX ? (fs.altitudo - XX) / II : ZEPHYRUM;
        x = cw;
        per (k = ZEPHYRUM; k < IV; k++)
        {
            b32 electum = (k == ZEPHYRUM
                && chorda_aequalis_literis(glutinatio, "A"))
                       || (k == I
                           && chorda_aequalis_literis(glutinatio, "B"));
            sprintf(t, "textus:%s%s", TEXTUS[k],
                electum ? ":electum" : "");
            q = nodus(piscina, intern, ID[k], PARTES_BOTTONE, x, y,
                LATITUDO[k], XX);
            componens_ponere_titulum(q, t);
            componens_ponere_actio(q, "glutinatio.agere");
            componens_addere_liberum(status, q);
            x += LATITUDO[k] + cw;
        }
        /* magnitudo radendi (penicilli): palettam suam aperit */
        sprintf(t, "magnitudo:%d", (integer)attributum_s32(&ramus,
            "magnitudo_penicilli", I));
        x_magnitudinis = x + cw;
        (vacuum)quadratum_addere(piscina, intern, status,
            "quadratum.magnitudo", x_magnitudinis, y, t);
        componens_ponere_titulum(status,
            "glutinatio: A Atkinson, B exemplaria");
    }
    /* P1a: quadrata (XX x XX, cellula inter se, centrata in linea):
     * instrumentum, color primus, color secundus (nullus ordinarie,
     * -1) - titulus dicit quid figura pingat */
    alioquin
    {
        character  t[XLVIII];
              s32  y;
        Componens* q;

        y = fs.altitudo > XX ? (fs.altitudo - XX) / II : ZEPHYRUM;
        sprintf(t, "instrumentum:%s", chorda_vacua(instrumentum)
            ? "nihil" : chorda_ut_cstr(instrumentum, piscina));
        quadratum_addere(piscina, intern, status,
            "quadratum.instrumentum", cw, y, t);
        sprintf(t, "color:%d", (integer)attributum_s32(&ramus,
            "color_primus", ZEPHYRUM));
        quadratum_addere(piscina, intern, status,
            "quadratum.color_primus", II * cw + XX, y, t);
        sprintf(t, "color:%d", (integer)attributum_s32(&ramus,
            "color_secundus", -I));
        quadratum_addere(piscina, intern, status,
            "quadratum.color_secundus", III * cw + II * XX, y, t);
        /* P3: exemplar coloribus veris - "exemplar:<n>:<primus>:
         * <secundus>" */
        sprintf(t, "exemplar:%d:%d:%d", (integer)attributum_s32(&ramus,
            "exemplar", ZEPHYRUM),
            (integer)attributum_s32(&ramus,
                "color_primus", ZEPHYRUM),
            (integer)attributum_s32(&ramus,
                "color_secundus", -I));
        quadratum_addere(piscina, intern, status, "quadratum.exemplar",
            IV * cw + III * XX, y, t);
        /* P4a/b: magnitudo instrumenti currentis; spongia fixa, sine
         * palette (actio nulla) */
        si (chorda_aequalis_literis(instrumentum, "aspergillum"))
        {
            sprintf(t, "magnitudo:aspergillum:%d", (integer)
                attributum_s32(&ramus,
                    "magnitudo_aspergilli", I));
        }
        alioquin si (chorda_aequalis_literis(instrumentum, "linea"))
        {
            sprintf(t, "magnitudo:linea:%d", (integer)attributum_s32(
                &ramus, "magnitudo_lineae", I));
        }
        alioquin si (chorda_aequalis_literis(instrumentum, "spongia"))
        {
            sprintf(t, "magnitudo:spongia:%d",
                (integer)PICTOR_SPONGIAE_LATUS);
        }
        alioquin
        {
            sprintf(t, "magnitudo:%d", (integer)attributum_s32(&ramus,
                "magnitudo_penicilli", I));
        }
        q = quadratum_addere(piscina, intern, status,
            "quadratum.magnitudo", V * cw + IV * XX, y, t);
        /* L3: quadratum stratorum ad dextrum lineae - testa-ios D6:
         * ICON (XX x XX, "strata:quadratum"), stratum currens in
         * palette (Franus 2026-10-23: linea in telephono angusta);
         * palettam 'strata' aperit. Filius RADICIS, non lineae
         * status */
        si (cfg->doc)
        {
            quadratum_stratorum = nodus(piscina, intern,
                "quadratum.strata", PARTES_BOTTONE,
                fs.x + fs.latitudo - cw - XX, fs.y + y, XX, XX);
            componens_ponere_titulum(quadratum_stratorum,
                "strata:quadratum");
            componens_ponere_actio(quadratum_stratorum,
                "palette.aperire");
        }
        /* pinacotheca D1 (olim C1b quadratum imaginis, eodem loco):
         * quadratum ⋯ ad sinistram stratorum (cw inter), solum cum
         * vexillo applicationis; menu suum (palette "elenchus")
         * aperit. Filius RADICIS */
        si (cfg->elenchus_praesto)
        {
            quadratum_elenchi = nodus(piscina, intern,
                "quadratum.elenchus", PARTES_BOTTONE,
                fs.x + fs.latitudo - II * cw - II * XX, fs.y + y, XX,
                XX);
            componens_ponere_titulum(quadratum_elenchi,
                "elenchus:quadratum");
            componens_ponere_actio(quadratum_elenchi,
                "palette.aperire");
        }
        si (chorda_aequalis_literis(instrumentum, "spongia"))
        {
            componens_ponere_actio(q, "");
        }
    }

    componens_addere_liberum(prospectus, tabula);
    componens_addere_liberum(radix, prospectus);
    componens_addere_liberum(radix, status);
    si (quadratum_stratorum)
    {
        componens_addere_liberum(radix, quadratum_stratorum);
    }
    si (quadratum_elenchi)
    {
        componens_addere_liberum(radix, quadratum_elenchi);
    }
    /* P1b: palette aperta - filius ULTIMUS radicis (super tabulam
     * pingitur, ictus primum capit) */
    {
        chorda palette;
           s32 qx;
           s32 qy;

        /* pendens: palette magnitudinis sola */
        palette = attributum_chorda(&ramus, "palette");
        si (pendens && !chorda_aequalis_literis(palette, "magnitudo"))
        {
            palette = chorda_ex_literis("", piscina);
        }
        qy = fs.y + (fs.altitudo > XX ? (fs.altitudo - XX) / II
                                      : ZEPHYRUM);
        qx = chorda_aequalis_literis(palette, "color_primus")
            ? II * cw + XX
            : chorda_aequalis_literis(palette, "color_secundus")
            ? III * cw + II * XX
            : chorda_aequalis_literis(palette, "exemplar")
            ? IV * cw + III * XX
            : chorda_aequalis_literis(palette, "magnitudo")
            ? (pendens ? x_magnitudinis : V * cw + IV * XX) : cw;
        /* pinacotheca D2: menu ⋯ (elenchus) supra quadratum suum -
         * area infra quadratum excluditur, ergo sursum reflectitur.
         * "Open picture…" sine 'claudit': actio ipsa palettam in
         * "pinacotheca" mutat, dimissio eam non claudat */
        si (   chorda_aequalis_literis(palette, "elenchus")
            && quadratum_elenchi)
        {
                    Elenchus  e;
                 ElenchusRes  res[V];
                     Punctum  ancora;
                       Fines  area;
                   Componens* menu;
    PictorElenchiCondiciones  k;

            k = pictor_elenchi_condiciones(cfg);
            pictor_elenchum_parare(&k, res, &e);
            ancora.x       = quadratum_elenchi->fines.x;
            ancora.y       = quadratum_elenchi->fines.y - II;
            area.x         = ZEPHYRUM;
            area.y         = ZEPHYRUM;
            area.latitudo  = latitudo;
            area.altitudo  = ancora.y;
            menu = elenchus_componere(&e, elenchus_fines(&e, ancora,
                area), "elenchus.agere", piscina, intern);
            /* res quae palettam aliam aperiunt (My pictures, Shared
             * pictures, Sign in) non claudunt: dimissio eam aliter post
             * actionem tolleret (plan-5 T6) */
            {
                i32 r;

                per (r = ZEPHYRUM; menu && menu->liberi
                     && r < xar_numerus(menu->liberi)
                     && r < e.numerus; r++)
                {
                    si (   strcmp(e.res[r].titulus,
                        "pinacotheca.aperire")
                           == ZEPHYRUM
                        || strcmp(e.res[r].titulus, "communia.aperire")
                           == ZEPHYRUM
                        || strcmp(e.res[r].titulus,
                        "inscriptio.aperire")
                           == ZEPHYRUM)
                    {
                        (*(Componens**)xar_obtinere(menu->liberi, r))
                            ->claudit = FALSUM;
                    }
                }
            }
            si (menu)
            {
                componens_addere_liberum(radix, menu);
            }
        }
        /* pinacotheca D3: rete pinacum super prospectum */
        si (   chorda_aequalis_literis(palette, "pinacotheca")
            && cfg->pinacotheca && cfg->pinacotheca->picturae)
        {
            componens_addere_liberum(radix, pinacothecam_componere(
                piscina, intern, cfg->pinacotheca, &ramus, fp, latitudo,
                altitudo, cw, ch));
        }
        /* plan-5 T6: Shared pictures; spectator (spectata absens ->
         * rete) */
        si (   (   chorda_aequalis_literis(palette, "communia")
                || chorda_aequalis_literis(palette, "spectator"))
            && cfg->communia)
        {
            Componens* sp;

            sp = chorda_aequalis_literis(palette, "spectator")
                ? spectatorem_componere(piscina, intern, cfg, &ramus,
                fp,
                cw,
                     ch) : NIHIL;
            componens_addere_liberum(radix,
                sp ? sp : communia_componere(
                piscina, intern, cfg, &ramus, fp, latitudo, altitudo,
                cw,
                ch));
        }
        /* plan-5 T7: salutatio et inscriptio */
        si (chorda_aequalis_literis(palette, "salutatio"))
        {
            componens_addere_liberum(radix, salutationem_componere(
                piscina, intern, fp, cw, ch));
        }
        si (chorda_aequalis_literis(palette, "inscriptio"))
        {
            componens_addere_liberum(radix, inscriptionem_componere(
                piscina, intern, cfg, &ramus, fp, cw, ch));
        }
        /* L3: palette stratorum (margine dextro ad quadratum) */
        si (chorda_aequalis_literis(palette, "strata") && cfg->doc)
        {
            Componens* ps;

            ps = strata_palettam_componere(piscina, intern, cfg->doc,
                stratum_currens_compositum(&ramus, cfg->doc),
                fs.x + fs.latitudo - cw, qy, ZEPHYRUM);
            indicem_tractus_addere(piscina, intern, ps, motus,
                cfg->doc);
            componens_addere_liberum(radix, ps);
        }
        si (   chorda_aequalis_literis(palette, "instrumentum")
            || chorda_aequalis_literis(palette, "color_primus")
            || chorda_aequalis_literis(palette, "color_secundus")
            || chorda_aequalis_literis(palette, "exemplar")
            || chorda_aequalis_literis(palette, "magnitudo"))
        {
            componens_addere_liberum(radix, palettam_componere(piscina,
                intern, palette, fs.x + qx, qy, ZEPHYRUM, latitudo,
                instrumentum,
                attributum_s32(&ramus,
                    chorda_aequalis_literis(palette, "magnitudo")
                    ? (chorda_aequalis_literis(instrumentum,
                    "aspergillum")
                       ? "magnitudo_aspergilli"
                       : chorda_aequalis_literis(instrumentum, "linea")
                       ? "magnitudo_lineae" : "magnitudo_penicilli")
                    : chorda_ut_cstr(palette, piscina),
                    chorda_aequalis_literis(palette, "color_secundus")
                    ? -I : chorda_aequalis_literis(palette, "magnitudo")
                    ? I : ZEPHYRUM),
                attributum_s32(&ramus, "color_primus",
                    ZEPHYRUM),
                attributum_s32(&ramus,
                "color_secundus",
                    -I)));
        }
    }
    redde radix;
}
