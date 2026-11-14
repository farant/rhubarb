/* gabc_visus.c - visus cantus gabc ex textu in memoria (vide
 * include/gabc_visus.h; ex lib/antiphonale.c extractum, psalterium
 * planum T2 - tabula VIEW/FILE in lib/gabc_visus.worklog.md)
 *
 * Versio et compositio in contextu visus: textus in
 * gabc_visus_textum_ponere legitur, compositio pigre (latitudo,
 * spatium mutata); pictura solum legit et mandata emittit.
 */
#include "gabc_visus.h"
#include "gabc.h"
#include "gabc_compositio.h"
#include "gabc_pictura.h"
#include "gabc_carmen.h"
#include "figura.h"
#include "destinatio.h"
#include "thema.h"
#include "fons.h"
#include <stdio.h>
#include <string.h>

#define VISUS_LATITUDO   CDLXXX    /* superficies ordinaria */
#define VISUS_ALTITUDO   CCCXX
#define VISUS_LINEA      XII       /* linea ima et vexillum */
#define VISUS_SAGITTA    XXIV      /* latitudo areae sagittae */
#define VISUS_LIMBUS_X   VI        /* margo texturae foci (ut */
#define VISUS_LIMBUS_Y   VIII      /* navigator, chronica) */
#define VISUS_SPATIUM    VIII
#define VISUS_VEXILLUM   CCLVI
#define VISUS_GLOBULUS   XVI       /* latitudo globuli canendi */
#define MORA_SONANS      XXXIII    /* pulsus dum sonat (P7) */
/* porta incipere recusat: vicus solum mutus aut sine apparatu soni */
#define VEXILLUM_MUTUM   "can't play: sound muted or unavailable"

/* versio bona: unitas una (G18) - textus, cantus, carmen et nexus;
 * compositio in piscina propria (relaxatio sine versione nova) */
nomen structura {
            Piscina* piscina;
             chorda  textus;
         GabcCantus* cantus;
             chorda  carmen;      /* textus libri <carmina>; vacuus =
                                   * non sonabile (porta legit) */
  GabcCarmenRelatio  relatio;     /* nexus ordine initii (G11) */
                s64  finis;       /* pulsus post nexum ultimum */
          character  causa[CXXVIII];  /* cur non sonabile */
            Piscina* piscina_compositionis;
     GabcCompositio* compositio;
                i32  latitudo;
                i32  altitudo;
                i32  spatium;
} Versio;

structura GabcVisus {
                 Piscina* piscina;
             InsulaRamus* ramus;
      constans character* actio;
                  chorda  scriptor;        /* actio semel copiata:
                                            * scriptura campi non
                                            * crescit piscinam */
     constans character* radix_id;
         GabcVisusPorta  porta;
                    i32  latitudo;
                    i32  altitudo;
                    i32  margo_supra;
                 Versio* versio;           /* NIHIL = nulla bona */
              character  vexillum[VISUS_VEXILLUM];   /* "" = nullum */
    /* sonus (P7): sonat = petitum nec sistitum; illuminata = syllaba
     * nexus currentis; electa = syllaba ictu electa sistente (-1) */
                    b32 sonat;
                    s32 illuminata;
                    s32 electa;
};


/* ==================================================
 * Status (ramus)
 * ================================================== */

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  valor;

    a = insula_ramus_attributum(ramus, titulus);
    si (!a || !chorda_ut_s32(*a, &valor))
    {
        redde praestitutum;
    }
    redde valor;
}

interior i32
spatium_currens (
    constans GabcVisus* visus)
{
    s32 s = attributum_s32(visus->ramus, "spatium", VISUS_SPATIUM);

    si (s < VI || s > XLVIII || s % II != ZEPHYRUM)
    {
        s = VISUS_SPATIUM;
    }
    redde (i32)s;
}

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

interior vacuum
campum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParCampi* par = (ParCampi*)ctx;

    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

interior b32
numerum_scribere (
             GabcVisus* visus,
    constans character* titulus,
                   s32  valor)
{
    ParCampi par;
      chorda prior;
   character textus[XVI];
         b32 ok;

    sprintf(textus, "%d", (integer)valor);
    par.titulus  = titulus;
    par.valor    = textus;
    prior        = visus->ramus->repo->scriptor;
    insula_scriptorem_ponere(visus->ramus->repo, visus->scriptor);
    ok = mutare_ramum(visus->ramus, campum_ponere, &par);
    insula_scriptorem_ponere(visus->ramus->repo, prior);
    redde ok;
}


/* ==================================================
 * Dispositio
 * ================================================== */

interior s32
superficies (
    constans GabcVisus* visus,
                   b32  latitudo)
{
    s32 valor = latitudo ? (s32)visus->latitudo : (s32)visus->altitudo;

    si (valor <= ZEPHYRUM)
    {
        valor = latitudo ? VISUS_LATITUDO : VISUS_ALTITUDO;
    }
    redde attributum_s32(visus->ramus, latitudo ? "superficies_latitudo"
        : "superficies_altitudo", valor);
}

/* area paginae: intra limbum, sub margine involucri, supra lineam
 * imam */
interior i32
altitudo_paginae (
    constans GabcVisus* visus)
{
    s32 a = superficies(visus, FALSUM) - II * VISUS_LIMBUS_Y
          - VISUS_LINEA - (s32)visus->margo_supra;

    redde a > ZEPHYRUM ? (i32)a : I;
}

interior i32
latitudo_paginae (
    constans GabcVisus* visus)
{
    s32 l = superficies(visus, VERUM) - II * VISUS_LIMBUS_X;

    redde l > ZEPHYRUM ? (i32)l : I;
}

interior vacuum
versionem_destruere (
    Versio* versio)
{
    si (versio == NIHIL)
    {
        redde;
    }
    si (versio->piscina_compositionis)
    {
        piscina_destruere(versio->piscina_compositionis);
    }
    piscina_destruere(versio->piscina);
}

/* compositio ad formam datam (pigre); NIHIL si forma mala aut
 * memoria */
interior GabcCompositio*
componere_ad (
    Versio* versio,
       i32  latitudo,
       i32  altitudo,
       i32  spatium)
{
    GabcCompositionisForma forma;

    si (versio == NIHIL)
    {
        redde NIHIL;
    }
    si (   versio->compositio && versio->latitudo == latitudo
        && versio->altitudo == altitudo && versio->spatium == spatium)
    {
        redde versio->compositio;
    }
    si (versio->piscina_compositionis)
    {
        piscina_destruere(versio->piscina_compositionis);
    }
    versio->compositio             = NIHIL;
    versio->piscina_compositionis  = piscina_generare_dynamicum(
        "gabc_visus_compositio", (memoriae_index)(CCLVI * MXXIV));
    si (versio->piscina_compositionis == NIHIL)
    {
        redde NIHIL;
    }
    gabc_compositionis_formam_ordinariam(&forma);
    forma.latitudo    = latitudo;
    forma.altitudo    = altitudo;
    forma.spatium     = spatium;
    versio->latitudo  = latitudo;
    versio->altitudo  = altitudo;
    versio->spatium   = spatium;
    versio->compositio  = gabc_componere(versio->cantus, &forma,
        versio->piscina_compositionis);
    redde versio->compositio;
}

interior GabcCompositio*
compositio_currens (
    GabcVisus* visus)
{
    redde componere_ad(visus->versio, latitudo_paginae(visus),
        altitudo_paginae(visus), spatium_currens(visus));
}

/* pagina visa, ad compositionem coercita */
interior s32
pagina_currens (
         constans GabcVisus* visus,
    constans GabcCompositio* c)
{
    s32 p = attributum_s32(visus->ramus, "pagina", ZEPHYRUM);

    si (c == NIHIL || p < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (p >= (s32)c->numerus_paginarum)
    {
        p = (s32)c->numerus_paginarum - I;
    }
    redde p;
}


/* ==================================================
 * Carmen et nexus (P7)
 * ================================================== */

/* conversor -> textus libri <carmina> in piscina versionis (porta
 * legit: visus carmen.h nescit); causa si non */
interior vacuum
carmen_aedificare (
    Versio* versio)
{
      GabcCarmenForma  forma;
    ChordaAedificator* a;
    ChordaAedificator* b;
                  i32  n;

    versio->carmen.datum     = NIHIL;
    versio->carmen.mensura   = ZEPHYRUM;
    versio->causa[ZEPHYRUM]  = '\0';
    gabc_carmen_formam_ordinariam(&forma);
    a = chorda_aedificator_creare(versio->piscina, MMM);
    b = chorda_aedificator_creare(versio->piscina, MMM);
    si (!a || !b)
    {
        sprintf(versio->causa, "out of memory");
        redde;
    }
    si (!gabc_carmen_scribere(versio->cantus, &forma, versio->piscina,
        a,
            &versio->relatio))
    {
        sprintf(versio->causa, "%s", versio->cantus->numerus_notarum
            == ZEPHYRUM ? "no notes" : "the converter refused it");
        redde;
    }
    chorda_aedificator_appendere_literis(b, "<carmina>\n");
    chorda_aedificator_appendere_chorda(b,
        chorda_aedificator_finire(a));
    chorda_aedificator_appendere_literis(b, "</carmina>\n");
    si (versio->relatio.numerus_nexuum == ZEPHYRUM)
    {
        sprintf(versio->causa, "no notes");
        redde;
    }
    versio->carmen  = chorda_aedificator_finire(b);
    n               = versio->relatio.numerus_nexuum;
    versio->finis   = versio->relatio.nexus[n - I].initium
                    + versio->relatio.nexus[n - I].longitudo;
}

/* nexus ultimus cuius initium <= pulsus (quaestio bipartita); -1 */
interior s32
nexus_ad_pulsum (
    constans Versio* versio,
                s64  pulsus)
{
    s32 imum;
    s32 summum;
    s32 medium;

    si (   versio == NIHIL || versio->carmen.mensura == ZEPHYRUM
        || pulsus < ZEPHYRUM)
    {
        redde -I;
    }
    imum    = ZEPHYRUM;
    summum  = (s32)versio->relatio.numerus_nexuum;
    dum (imum < summum)
    {
        medium = (imum + summum) / II;
        si (versio->relatio.nexus[medium].initium <= pulsus)
        {
            imum = medium + I;
        }
        alioquin
        {
            summum = medium;
        }
    }
    redde imum - I;
}

interior s32
syllaba_ad_pulsum (
    constans Versio* versio,
                s64  pulsus)
{
    s32 n = nexus_ad_pulsum(versio, pulsus);

    redde n < ZEPHYRUM ? -I : (s32)versio->relatio.nexus[n].syllaba;
}

/* nexus primus syllabae >= s (directio +I) aut ultimus syllabae <= s
 * (directio -I, eius syllabae nexus primus); -1 si nullus */
interior s32
nexus_syllabae (
    constans Versio* versio,
                s32  s,
                s32  directio)
{
    s32 k;
    s32 n;
    s32 inventa;

    si (versio == NIHIL || versio->carmen.mensura == ZEPHYRUM)
    {
        redde -I;
    }
    n = (s32)versio->relatio.numerus_nexuum;
    si (directio > ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            si ((s32)versio->relatio.nexus[k].syllaba >= s)
            {
                redde k;
            }
        }
        redde -I;
    }
    inventa = -I;
    per (k = n - I; k >= ZEPHYRUM; k--)
    {
        si ((s32)versio->relatio.nexus[k].syllaba <= s)
        {
            inventa = (s32)versio->relatio.nexus[k].syllaba;
            frange;
        }
    }
    redde inventa < ZEPHYRUM ? -I : nexus_syllabae(versio, inventa, I);
}


/* ==================================================
 * Creatio et textus
 * ================================================== */

GabcVisus*
gabc_visus_creare (
                     Piscina* piscina,
                 InsulaRamus* ramus,
          constans character* actio,
          constans character* radix_id,
     constans GabcVisusPorta* porta,
                         i32  latitudo,
                         i32  altitudo,
                         i32  margo_supra)
{
    GabcVisus* visus;

    si (!piscina || !ramus || !actio || !radix_id)
    {
        redde NIHIL;
    }
    visus = (GabcVisus*)piscina_allocare(piscina, magnitudo(GabcVisus));
    si (visus == NIHIL)
    {
        redde NIHIL;
    }
    memset(visus, ZEPHYRUM, magnitudo(GabcVisus));
    visus->piscina  = piscina;
    visus->ramus    = ramus;
    visus->actio        = chorda_ut_cstr(chorda_ex_literis(actio,
        piscina), piscina);
    visus->radix_id     = chorda_ut_cstr(chorda_ex_literis(radix_id,
        piscina), piscina);
    si (porta)
    {
        visus->porta = *porta;
    }
    visus->latitudo     = latitudo;
    visus->altitudo     = altitudo;
    visus->margo_supra  = margo_supra;
    visus->illuminata   = -I;
    visus->electa       = -I;
    si (!visus->actio || !visus->radix_id)
    {
        redde NIHIL;
    }
    visus->scriptor = chorda_ex_literis(visus->actio, piscina);
    redde visus;
}

/* G18: versio nova in locum veteris; sonans (continuare) ad pulsum
 * eundem transit, brevior aut non sonabilis sistit; sine continuare
 * sistit. Vetus solum post novam aedificatam et traditam */
interior vacuum
versionem_tradere (
     GabcVisus* visus,
        Versio* nova,
           b32  continuare)
{
    s64 pulsus;

    si (visus->sonat && !continuare)
    {
        si (visus->porta.sistere)
        {
            visus->porta.sistere(visus->porta.ctx);
        }
        visus->sonat       = FALSUM;
        visus->illuminata  = -I;
    }
    alioquin si (visus->sonat)
    {
        pulsus = visus->porta.pulsus
            ? visus->porta.pulsus(visus->porta.ctx) : -I;
        si (pulsus < ZEPHYRUM)
        {
            visus->sonat = FALSUM;
        }
        alioquin si (   nova->carmen.mensura == ZEPHYRUM
                     || pulsus               >= nova->finis
                     || !visus->porta.incipere)
        {
            si (visus->porta.sistere)
            {
                visus->porta.sistere(visus->porta.ctx);
            }
            visus->sonat = FALSUM;
            si (nova->carmen.mensura == ZEPHYRUM)
            {
                /* cur sistit dicitur (ut gabc_visus_canere) */
                sprintf(visus->vexillum, "not playable: %.200s",
                    nova->causa);
                visus->electa = visus->illuminata;
            }
        }
        alioquin si (!visus->porta.incipere(visus->porta.ctx,
                         nova->carmen, pulsus))
        {
            visus->sonat = FALSUM;
            sprintf(visus->vexillum, "%s", VEXILLUM_MUTUM);
            visus->electa = visus->illuminata;
        }
        si (!visus->sonat)
        {
            visus->illuminata = -I;
        }
    }
    versionem_destruere(visus->versio);
    visus->versio = nova;
    si (!continuare)
    {
        /* melodia alia: Spatium ab initio */
        visus->electa = -I;
    }
    alioquin si (visus->electa >= (s32)nova->cantus->numerus_syllabarum)
    {
        /* electa indicem servat (permutatio melodiam plerumque mutat,
         * syllabas non); versio brevior: ad syllabam ultimam */
        visus->electa = (s32)nova->cantus->numerus_syllabarum - I;
    }
}

b32
gabc_visus_textum_ponere (
     GabcVisus* visus,
        chorda  textus,
           b32  continuare)
{
     character  vexillum[VISUS_VEXILLUM];
       Piscina* p;
        chorda  copia;
    GabcCantus* cantus;
        Versio* nova;
           b32  mutatum;
           i32  k;

    si (visus == NIHIL)
    {
        redde FALSUM;
    }
    vexillum[ZEPHYRUM]  = '\0';
    nova                = NIHIL;
    p = piscina_generare_dynamicum("gabc_visus_versio",
        (memoriae_index)(LXIV * MXXIV));
    si (p == NIHIL)
    {
        redde FALSUM;
    }
    copia.mensura  = textus.mensura;
    copia.datum    = textus.mensura
        > ZEPHYRUM ? (i8*)piscina_allocare(p,
        (memoriae_index)textus.mensura) : NIHIL;
    cantus = NIHIL;
    si (textus.mensura == ZEPHYRUM)
    {
        sprintf(vexillum, "empty text");
    }
    alioquin si (copia.datum == NIHIL)
    {
        sprintf(vexillum, "out of memory");
    }
    alioquin
    {
        memcpy(copia.datum, textus.datum, (size_t)textus.mensura);
        cantus = gabc_legere(copia, p);
        si (cantus == NIHIL)
        {
            sprintf(vexillum, "out of memory");
        }
    }
    si (cantus && !gabc_successus(cantus))
    {
        per (k = ZEPHYRUM; k < cantus->numerus_vitiorum; k++)
        {
            si (cantus->vitia[k].gravitas == GABC_VITIUM)
            {
                sprintf(vexillum, "%u:%u: %.*s",
                    (unsigned)cantus->vitia[k].linea,
                    (unsigned)cantus->vitia[k].columna,
                    (integer)(cantus->vitia[k].causa.mensura > CC
                        ? CC : cantus->vitia[k].causa.mensura),
                    (constans character*)cantus->vitia[k].causa.datum);
                frange;
            }
        }
    }
    alioquin si (   cantus && visus->versio
                 && visus->versio->textus.mensura == copia.mensura
                 && memcmp(visus->versio->textus.datum, copia.datum,
                        (size_t)copia.mensura) == ZEPHYRUM)
    {
        /* idem textus (eventus speculae spurius): versio manet */
        nova = NIHIL;
    }
    alioquin si (cantus)
    {
        nova = (Versio*)piscina_allocare(p, magnitudo(Versio));
        si (nova)
        {
            memset(nova, ZEPHYRUM, magnitudo(Versio));
            nova->piscina  = p;
            nova->textus   = copia;
            nova->cantus   = cantus;
            carmen_aedificare(nova);
        }
    }
    si (nova == NIHIL)
    {
        piscina_destruere(p);
    }
    mutatum = strcmp(vexillum, visus->vexillum) != ZEPHYRUM;
    strcpy(visus->vexillum, vexillum);
    si (nova)
    {
        versionem_tradere(visus, nova, continuare);
        mutatum = VERUM;
    }
    redde mutatum;
}

vacuum
gabc_visus_vacuare (
    GabcVisus* visus)
{
    si (visus == NIHIL)
    {
        redde;
    }
    si (visus->sonat && visus->porta.sistere)
    {
        visus->porta.sistere(visus->porta.ctx);
    }
    visus->sonat       = FALSUM;
    visus->illuminata  = -I;
    visus->electa      = -I;
    versionem_destruere(visus->versio);
    visus->versio              = NIHIL;
    visus->vexillum[ZEPHYRUM]  = '\0';
}

vacuum
gabc_visus_vexillum_ponere (
              GabcVisus* visus,
     constans character* vexillum)
{
    si (visus == NIHIL)
    {
        redde;
    }
    visus->vexillum[ZEPHYRUM] = '\0';
    si (vexillum)
    {
        strncpy(visus->vexillum, vexillum, VISUS_VEXILLUM - I);
        visus->vexillum[VISUS_VEXILLUM - I] = '\0';
    }
}

constans character*
gabc_visus_vexillum (
    constans GabcVisus* visus)
{
    redde visus ? visus->vexillum : "";
}

chorda
gabc_visus_titulus (
    constans GabcVisus* visus)
{
    chorda t;

    si (   visus && visus->versio
        && visus->versio->cantus->titulus.mensura > ZEPHYRUM)
    {
        redde visus->versio->cantus->titulus;
    }
    t.datum    = NIHIL;
    t.mensura  = ZEPHYRUM;
    redde t;
}

b32
gabc_visus_habet_versionem (
    constans GabcVisus* visus)
{
    redde visus && visus->versio;
}


/* ==================================================
 * Actiones
 * ================================================== */

b32
gabc_visus_paginam_movere (
     GabcVisus* visus,
           s32  gradus)
{
    GabcCompositio* c;
               s32  p;

    c = compositio_currens(visus);
    si (c == NIHIL)
    {
        redde VERUM;
    }
    p = pagina_currens(visus, c) + gradus;
    si (p < ZEPHYRUM || p >= (s32)c->numerus_paginarum)
    {
        redde VERUM;
    }
    redde numerum_scribere(visus, "pagina", p);
}

b32
gabc_visus_paginam_ponere (
     GabcVisus* visus,
           s32  pagina)
{
    redde numerum_scribere(visus, "pagina", pagina);
}

/* spatium mutatum; syllaba prima visa in conspectu manet */
b32
gabc_visus_spatium_movere (
     GabcVisus* visus,
           s32  gradus)
{
    GabcCompositio* c;
               s32  spatium;
               s32  prima;
               s32  pagina;
               i32  s;

    spatium = (s32)spatium_currens(visus) + gradus;
    si (spatium < VI || spatium > XLVIII)
    {
        redde VERUM;
    }
    c      = compositio_currens(visus);
    prima  = -I;
    si (c)
    {
        pagina = pagina_currens(visus, c);
        per (s = ZEPHYRUM; s < c->numerus_syllabarum; s++)
        {
            si (c->syllabae[s].pagina == pagina)
            {
                prima = (s32)s;
                frange;
            }
        }
    }
    si (!numerum_scribere(visus, "spatium", spatium))
    {
        redde FALSUM;
    }
    c = componere_ad(visus->versio, latitudo_paginae(visus),
        altitudo_paginae(visus), (i32)spatium);
    si (c && prima >= ZEPHYRUM && c->syllabae[prima].pagina >= ZEPHYRUM)
    {
        redde numerum_scribere(visus, "pagina",
            c->syllabae[prima].pagina);
    }
    redde VERUM;
}

/* syllaba visa sonans aut electa */
interior s32
syllaba_currens (
    constans GabcVisus* visus)
{
    redde visus->sonat ? visus->illuminata : visus->electa;
}

/* sonans: quaerere ad nexum; sistens: eligere */
interior b32
ad_syllabam_ire (
     GabcVisus* visus,
           s32  nexus)
{
    s32 s;

    si (nexus < ZEPHYRUM || visus->versio == NIHIL)
    {
        redde VERUM;
    }
    s = (s32)visus->versio->relatio.nexus[nexus].syllaba;
    si (visus->sonat)
    {
        si (visus->porta.quaerere)
        {
            visus->porta.quaerere(visus->porta.ctx,
                visus->versio->relatio.nexus[nexus].initium);
        }
        visus->illuminata = s;
    }
    alioquin
    {
        visus->electa = s;
    }
    redde VERUM;
}

b32
gabc_visus_canere (
    GabcVisus* visus)
{
    s32 nexus;
    s64 pulsus;

    si (visus->sonat)
    {
        si (visus->porta.sistere)
        {
            visus->porta.sistere(visus->porta.ctx);
        }
        visus->sonat       = FALSUM;
        visus->electa      = visus->illuminata;
        visus->illuminata  = -I;
        redde VERUM;
    }
    si (visus->versio == NIHIL)
    {
        redde VERUM;
    }
    si (visus->versio->carmen.mensura == ZEPHYRUM)
    {
        sprintf(visus->vexillum, "not playable: %.200s",
            visus->versio->causa);
        redde VERUM;
    }
    si (!visus->porta.incipere)
    {
        redde VERUM;
    }
    nexus = visus->electa >= ZEPHYRUM ? nexus_syllabae(visus->versio,
        visus->electa, I) : ZEPHYRUM;
    si (nexus < ZEPHYRUM && visus->electa >= ZEPHYRUM)
    {
        /* electa post notam ultimam: a nota ultima, non ab initio */
        nexus = nexus_syllabae(visus->versio, visus->electa, -I);
    }
    pulsus = nexus >= ZEPHYRUM
        ? visus->versio->relatio.nexus[nexus].initium : ZEPHYRUM;
    si (visus->porta.incipere(visus->porta.ctx, visus->versio->carmen,
            pulsus))
    {
        visus->sonat       = VERUM;
        visus->illuminata  = syllaba_ad_pulsum(visus->versio, pulsus);
        visus->electa      = -I;
        si (strcmp(visus->vexillum, VEXILLUM_MUTUM) == ZEPHYRUM)
        {
            visus->vexillum[ZEPHYRUM] = '\0';
        }
    }
    alioquin
    {
        sprintf(visus->vexillum, "%s", VEXILLUM_MUTUM);
    }
    redde VERUM;
}

b32
gabc_visus_sistere (
    GabcVisus* visus)
{
    si (visus->sonat)
    {
        si (visus->porta.sistere)
        {
            visus->porta.sistere(visus->porta.ctx);
        }
        visus->sonat       = FALSUM;
        visus->illuminata  = -I;
    }
    redde VERUM;
}

b32
gabc_visus_syllabam_movere (
     GabcVisus* visus,
           s32  directio)
{
    s32 s = syllaba_currens(visus);

    si (directio < ZEPHYRUM)
    {
        si (s <= ZEPHYRUM)
        {
            redde VERUM;
        }
        redde ad_syllabam_ire(visus, nexus_syllabae(visus->versio, s
            - I,
            -I));
    }
    redde ad_syllabam_ire(visus, nexus_syllabae(visus->versio, s + I,
        I));
}

b32
gabc_visus_premere (
               GabcVisus* visus,
               Componens* nodus,
        constans Eventus* eventus)
{
    GabcCompositio* comp;
           Punctum  punctum;
               s32  s;

    si (   !visus || !nodus || eventus->genus != EVENTUS_MUS_DEPRESSUS
        || eventus->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_prior"))
    {
        redde gabc_visus_paginam_movere(visus, -I);
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_proxima"))
    {
        redde gabc_visus_paginam_movere(visus, I);
    }
    si (chorda_aequalis_literis(nodus->id, "globulus_canere"))
    {
        redde gabc_visus_canere(visus);
    }
    si (!chorda_aequalis_literis(nodus->id, visus->radix_id))
    {
        redde FALSUM;
    }
    comp = compositio_currens(visus);
    si (comp == NIHIL)
    {
        redde FALSUM;
    }
    punctum.x  = (s32)eventus->datum.mus.x;
    punctum.y  = (s32)eventus->datum.mus.y;
    punctum    = destinatio_ad_locale(nodus, punctum);
    s = gabc_compositio_syllaba_ad_punctum(comp,
        (i32)pagina_currens(visus, comp), punctum.x - VISUS_LIMBUS_X,
        punctum.y - VISUS_LIMBUS_Y - (s32)visus->margo_supra);
    si (s < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (visus->sonat)
    {
        redde ad_syllabam_ire(visus, nexus_syllabae(visus->versio, s,
            I));
    }
    visus->electa = s;
    redde VERUM;
}


/* ==================================================
 * Componere, pingere, pulsus
 * ================================================== */

/* fines radicis = superficies; compositio pigra; nodi lineae imae */
vacuum
gabc_visus_componere (
              GabcVisus* visus,
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    Componens* prior;
    Componens* proxima;
    Componens* globulus;
        Fines  f;
        Fines  s;

    si (!visus || !radix)
    {
        redde;
    }
    (vacuum)compositio_currens(visus);
    prior     = componens_creare(piscina, intern, "sagitta_prior",
        PARTES_NULLUM);
    proxima   = componens_creare(piscina, intern, "sagitta_proxima",
        PARTES_NULLUM);
    globulus  = componens_creare(piscina, intern, "globulus_canere",
        PARTES_NULLUM);
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = superficies(visus, VERUM);
    f.altitudo  = superficies(visus, FALSUM);
    componens_ponere_fines(radix, f);
    si (!prior || !proxima || !globulus)
    {
        redde;
    }
    s.x         = VISUS_LIMBUS_X;
    s.y         = f.altitudo - VISUS_LIMBUS_Y - VISUS_LINEA;
    s.latitudo  = VISUS_SAGITTA;
    s.altitudo  = VISUS_LINEA;
    componens_ponere_fines(prior, s);
    componens_ponere_actio(prior, visus->actio);
    s.x = f.latitudo - VISUS_LIMBUS_X - VISUS_SAGITTA;
    componens_ponere_fines(proxima, s);
    componens_ponere_actio(proxima, visus->actio);
    s.x         = VISUS_LIMBUS_X + VISUS_SAGITTA;
    s.latitudo  = VISUS_GLOBULUS;
    componens_ponere_fines(globulus, s);
    componens_ponere_actio(globulus, visus->actio);
    componens_addere_liberum(radix, prior);
    componens_addere_liberum(radix, proxima);
    componens_addere_liberum(radix, globulus);
}

interior vacuum
textum_pingere (
               Mandata* md,
                   s32  x,
                   s32  y,
                chorda  textus,
            ColorThema  color)
{
    mandata_textus(md, x, y, textus, FONS_PARVUS,
        figura_color_thematis(color));
}

vacuum
gabc_visus_pingere (
              GabcVisus* visus,
                Mandata* md,
                  Fines  totum,
                    b32  focatum,
                 chorda  titulus,
     constans character* nuntius_vacuus)
{
     constans GabcCompositio* comp;
                       Fines  f;
           GabcPicturaStatus  status;
                       Fines  fundus;
                       Fines  vexillum;
                   character  pes[CCLVI];
                         s32  pagina;
                         s32  y_pedis;
                         s32  y_paginae;

    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = totum.latitudo;
    f.altitudo  = totum.altitudo;
    figura_mensam_pingere(md, f, focatum);
    /* fundus intra limbum: textura foci solum in margine (ut
     * navigator, chronica) */
    fundus.x         = VISUS_LIMBUS_X;
    fundus.y         = VISUS_LIMBUS_Y;
    fundus.latitudo  = f.latitudo - II * VISUS_LIMBUS_X;
    fundus.altitudo  = f.altitudo - II * VISUS_LIMBUS_Y;
    mandata_rectangulum(md, fundus, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (visus == NIHIL)
    {
        redde;
    }
    y_paginae  = VISUS_LIMBUS_Y + (s32)visus->margo_supra;
    comp       = visus->versio ? visus->versio->compositio : NIHIL;
    si (nuntius_vacuus && visus->versio == NIHIL)
    {
        textum_pingere(md, VISUS_LIMBUS_X + VIII, y_paginae + VIII,
            chorda_ex_literis(nuntius_vacuus, md->piscina),
            COLOR_TEXT_DIM);
    }
    alioquin si (visus->versio && comp == NIHIL)
    {
        textum_pingere(md, VISUS_LIMBUS_X + VIII, y_paginae + XX,
            chorda_ex_literis("pane too narrow for this staff size",
            md->piscina), COLOR_TEXT_DIM);
    }
    pagina = pagina_currens(visus, comp);
    si (comp)
    {
        status.syllaba_illuminata  = syllaba_currens(visus);
        status.nota_illuminata     = -I;
        (vacuum)gabc_paginam_pingere(comp, (i32)pagina, VISUS_LIMBUS_X,
            y_paginae, &status, md);
    }
    /* linea ima "<  >/#  titulus  2/5  >": sagitta, globulus canendi
     * (sonans quadratum, aliter triangulum), titulus et pagina,
     * sagitta */
    y_pedis = f.altitudo - VISUS_LIMBUS_Y - VISUS_LINEA + II;
    textum_pingere(md, VISUS_LIMBUS_X + II, y_pedis,
        chorda_ex_literis("<", md->piscina), COLOR_TEXT);
    textum_pingere(md, VISUS_LIMBUS_X + VISUS_SAGITTA, y_pedis,
        chorda_ex_literis(visus->sonat ? "\xE2\x96\xA0" : "\xE2\x96\xB6",
            md->piscina), visus->sonat ? COLOR_ACCENT_PRIMARY
            : COLOR_TEXT);
    sprintf(pes, "%.*s  %d/%u", (integer)(titulus.mensura > C
        ? C : titulus.mensura), titulus.datum
        ? (constans character*)titulus.datum : "",
        (integer)(pagina + I),
        comp ? (unsigned)comp->numerus_paginarum : 0u);
    textum_pingere(md, VISUS_LIMBUS_X + VISUS_SAGITTA + VISUS_GLOBULUS,
        y_pedis, chorda_ex_literis(pes, md->piscina), COLOR_TEXT);
    textum_pingere(md, f.latitudo - VISUS_LIMBUS_X - VIII, y_pedis,
        chorda_ex_literis(">", md->piscina), COLOR_TEXT);
    /* vexillum supra paginam, SUB margine involucri (caput
     * psalterii visibile manet; antiphonale: margo 0) */
    si (visus->vexillum[ZEPHYRUM])
    {
        vexillum.x         = VISUS_LIMBUS_X;
        vexillum.y         = y_paginae;
        vexillum.latitudo  = fundus.latitudo;
        vexillum.altitudo  = VISUS_LINEA;
        mandata_rectangulum(md, vexillum, figura_color_thematis(
            COLOR_ERROR), VERUM);
        textum_pingere(md, VISUS_LIMBUS_X + II, y_paginae + II,
            chorda_ex_literis(visus->vexillum, md->piscina),
            COLOR_BACKGROUND);
    }
}

b32
gabc_visus_pulsare (
    GabcVisus* visus)
{
    GabcCompositio* comp;
               s64  pulsus;
               s32  s;
               b32  mutatum;

    mutatum = FALSUM;
    si (visus == NIHIL || !visus->sonat)
    {
        redde FALSUM;
    }
    /* sonus (P7): syllaba nexus currentis illuminatur, pagina eam
     * sequitur (etiam post relaxationem: zoom, permutatio).
     * Illuminatio sonum mora apparatus praecedit (v1, worklog
     * antiphonalis) */
    pulsus = visus->porta.pulsus ? visus->porta.pulsus(visus->porta.ctx)
        : -I;
    s      = pulsus < ZEPHYRUM ? -I : syllaba_ad_pulsum(visus->versio,
                 pulsus);
    si (pulsus < ZEPHYRUM)
    {
        visus->sonat = FALSUM;
    }
    si (s != visus->illuminata)
    {
        visus->illuminata  = s;
        mutatum            = VERUM;
    }
    comp = s >= ZEPHYRUM ? compositio_currens(visus) : NIHIL;
    si (   comp && s < (s32)comp->numerus_syllabarum
        && comp->syllabae[s].pagina >= ZEPHYRUM
        && comp->syllabae[s].pagina != pagina_currens(visus, comp))
    {
        (vacuum)numerum_scribere(visus, "pagina",
            comp->syllabae[s].pagina);
        mutatum = VERUM;
    }
    redde mutatum;
}

b32
gabc_visus_sonat (
    constans GabcVisus* visus)
{
    redde visus && visus->sonat;
}

Mora
gabc_visus_mora (
    constans GabcVisus* visus)
{
    redde visus && visus->sonat ? MORA_SONANS : -I;
}
