/* probatio_historia_observator.c - observatores historiae (status-plan
 * S5b): post actum, revocare, reficere vocantur; actum recusatum,
 * revocare ad initium, reficere ad finem eos non vocant; plures ordine
 * additionis. Proiectio ludicra: VIII numeratores, actum "k" auget. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "volumen.h"
#include "historia.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    i8 numeratores[VIII];
} Ludicra;

interior vacuum
ludicra_vacare (
    vacuum* ctx)
{
    memset(((Ludicra*)ctx)->numeratores, ZEPHYRUM, VIII);
}

interior vacuum
ludicra_applicare (
    vacuum* ctx,
    chorda  actum)
{
    s32 k;

    si (chorda_ut_s32(actum, &k) && k >= ZEPHYRUM && k < VIII)
    {
        ((Ludicra*)ctx)->numeratores[k]++;
    }
}

/* observator: vocationes numerat et ordinem notat */
nomen structura {
           i32  vocationes;
     character  ordo[XVI];
     character  signum;
       Ludicra* ludicra;
           i32  numerator_visus;   /* numerator 0 in vocatione ultima */
} Spectator;

interior vacuum
spectare (
    Historia* h,
      vacuum* ctx)
{
    Spectator* s;
          i32  n;

    (vacuum)h;
    s = (Spectator*)ctx;
    s->vocationes++;
    n = (i32)strlen(s->ordo);
    si (n + I < XVI)
    {
        s->ordo[n]      = s->signum;
        s->ordo[n + I]  = '\0';
    }
    s->numerator_visus = (i32)s->ludicra->numeratores[ZEPHYRUM];
}

interior vacuum
_probare_observatores (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
                Historia* h;
                 Ludicra  l;
       HistoriaProiectio  pr;
               Spectator  a;
               Spectator  b;

    in = internamentum_creare(c->piscina);
    vol = volumen_temporarium(c->piscina,
        "probatio_historia_observator");
    CREDO_NECESSE_NON_NIHIL(vol);
    memset(&l, ZEPHYRUM, magnitudo(l));
    memset(&pr, ZEPHYRUM, magnitudo(pr));
    pr.memoria    = l.numeratores;
    pr.mensura    = VIII;
    pr.vacare     = ludicra_vacare;
    pr.applicare  = ludicra_applicare;
    pr.ctx        = &l;
    h = historia_creare(c->piscina, in, vol, "", "ludus",
        "ludus:checkpoint", LXIV, pr);
    CREDO_NECESSE_NON_NIHIL(h);
    memset(&a, ZEPHYRUM, magnitudo(a));
    memset(&b, ZEPHYRUM, magnitudo(b));
    a.signum   = 'a';
    b.signum   = 'b';
    a.ludicra  = &l;
    b.ludicra  = &l;
    CREDO_FALSUM(historia_observatorem_addere(h, NIHIL, &a));
    CREDO_VERUM(historia_observatorem_addere(h, spectare, &a));
    CREDO_VERUM(historia_observatorem_addere(h, spectare, &b));

    /* actum: ambo, ordine; proiectio iam applicata */
    CREDO_VERUM(historia_actum(h, chorda_ex_literis("0", c->piscina))
        > ZEPHYRUM);
    CREDO_AEQUALIS_I32(a.vocationes, I);
    CREDO_AEQUALIS_I32(b.vocationes, I);
    CREDO_AEQUALIS_I32(a.numerator_visus, I);
    /* actum recusatum (vacuum): nulla vocatio */
    CREDO_AEQUALIS_S64(historia_actum(h, chorda_ex_literis("",
        c->piscina)), (s64)ZEPHYRUM);
    CREDO_AEQUALIS_I32(a.vocationes, I);
    /* revocare: vocatio, proiectio iam revocata */
    CREDO_VERUM(historia_revocare(h));
    CREDO_AEQUALIS_I32(a.vocationes, II);
    CREDO_AEQUALIS_I32(a.numerator_visus, ZEPHYRUM);
    /* revocare ad initium: FALSUM, nulla vocatio */
    CREDO_FALSUM(historia_revocare(h));
    CREDO_AEQUALIS_I32(a.vocationes, II);
    /* reficere: vocatio */
    CREDO_VERUM(historia_reficere(h));
    CREDO_AEQUALIS_I32(a.vocationes, III);
    CREDO_AEQUALIS_I32(a.numerator_visus, I);
    /* reficere ad finem: nulla vocatio */
    CREDO_FALSUM(historia_reficere(h));
    CREDO_AEQUALIS_I32(a.vocationes, III);
    /* actum coniunctum quoque */
    CREDO_VERUM(historia_actum_coniunctum(h, chorda_ex_literis("1",
        c->piscina)) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(b.vocationes, IV);
    CREDO_VERUM(strcmp(a.ordo, "aaaa") == ZEPHYRUM);
    CREDO_VERUM(strcmp(b.ordo, "bbbb") == ZEPHYRUM);
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "observatores", _probare_observatores,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("historia_observator", SECTIONES);
}
