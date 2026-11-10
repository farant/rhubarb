/* antiphonale.c - cantus gabc in latere vici (vide
 * include/antiphonale.h)
 *
 * Arbor: radix "antiphonale" (PARTES_TABULA, figura tota) et liberi
 * "sagitta_prior", "sagitta_proxima" in linea ima (ictus paginam
 * vertit). Versio et compositio in contextu privato (Antiphonale): legi
 * in pulsu, componi in componere (cum latitudo aut spatium mutatur);
 * figura solum legit et mandata emittit.
 */
#include "antiphonale.h"
#include "membrum__praecepta.h"
#include "gabc.h"
#include "gabc_compositio.h"
#include "gabc_pictura.h"
#include "gabc_carmen.h"
#include "specula.h"
#include "reactor.h"
#include "filum.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include <stdio.h>
#include <string.h>

#define ANTIPHONALE_LATITUDO   CDLXXX    /* superficies ordinaria */
#define ANTIPHONALE_ALTITUDO   CCCXX
#define ANTIPHONALE_LINEA      XII       /* linea ima et vexillum */
#define ANTIPHONALE_SAGITTA    XXIV      /* latitudo areae sagittae */
#define ANTIPHONALE_LIMBUS_X   VI        /* margo texturae foci (ut */
#define ANTIPHONALE_LIMBUS_Y   VIII      /* navigator, chronica) */
#define ANTIPHONALE_SPATIUM    VIII
#define ANTIPHONALE_VIA        DXII
#define ANTIPHONALE_VEXILLUM   CCLVI
/* sine specula (directorium absens, platforma): relegere et speculam
 * iterum temptare omni CCL ms */
#define MORA_SINE_SPECULA      CCL
#define MORA_SONANS            XXXIII    /* pulsus dum sonat (P7) */
#define ANTIPHONALE_GLOBULUS   XVI       /* latitudo globuli canendi */
#define VISUS_ANTIPHONALIS     I

hic_manens constans character actio_antiphonalis[] =
    "antiphonale.agere";

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

nomen structura {
     character  via[ANTIPHONALE_VIA];
       Specula* specula;
       Piscina* piscina_speculae; /* speculae solius: temptatio fracta
                                   * aut via mutata eam destruit */
        Versio* versio;           /* NIHIL = nulla bona */
     character  vexillum[ANTIPHONALE_VEXILLUM];   /* "" = nullum */
    /* sonus (P7): sonat = petitum nec sistitum; illuminata = syllaba
     * nexus currentis; electa = syllaba ictu electa sistente (-1) */
           b32 sonat;
           s32 illuminata;
           s32 electa;
} Antiphonale;


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
       s32  v;

    a = insula_ramus_attributum(ramus, titulus);
    si (!a || !chorda_ut_s32(*a, &v))
    {
        redde praestitutum;
    }
    redde v;
}

interior i32
spatium_currens (
    constans AntiphonaleMontatio* m)
{
    s32 s = attributum_s32(&m->ramus, "spatium", ANTIPHONALE_SPATIUM);

    si (s < VI || s > XLVIII || s % II != ZEPHYRUM)
    {
        s = ANTIPHONALE_SPATIUM;
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
    AntiphonaleMontatio* m,
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
    prior        = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_antiphonalis, m->piscina));
    ok = mutare_ramum(&m->ramus, campum_ponere, &par);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}


/* ==================================================
 * Dispositio
 * ================================================== */

interior s32
superficies (
    constans AntiphonaleMontatio* m,
                             b32  latitudo)
{
    s32 v = latitudo ? (s32)m->latitudo : (s32)m->altitudo;

    si (v <= ZEPHYRUM)
    {
        v = latitudo ? ANTIPHONALE_LATITUDO : ANTIPHONALE_ALTITUDO;
    }
    redde attributum_s32(&m->ramus, latitudo ? "superficies_latitudo"
        : "superficies_altitudo", v);
}

/* area paginae: intra limbum, supra lineam imam */
interior i32
altitudo_paginae (
    constans AntiphonaleMontatio* m)
{
    s32 a = superficies(m, FALSUM) - II * ANTIPHONALE_LIMBUS_Y
          - ANTIPHONALE_LINEA;

    redde a > ZEPHYRUM ? (i32)a : I;
}

interior i32
latitudo_paginae (
    constans AntiphonaleMontatio* m)
{
    s32 l = superficies(m, VERUM) - II * ANTIPHONALE_LIMBUS_X;

    redde l > ZEPHYRUM ? (i32)l : I;
}

interior vacuum
versionem_destruere (
    Versio* v)
{
    si (v == NIHIL)
    {
        redde;
    }
    si (v->piscina_compositionis)
    {
        piscina_destruere(v->piscina_compositionis);
    }
    piscina_destruere(v->piscina);
}

/* compositio ad formam datam (pigre); NIHIL si forma mala aut
 * memoria */
interior GabcCompositio*
componere_ad (
    Versio* v,
       i32  latitudo,
       i32  altitudo,
       i32  spatium)
{
    GabcCompositionisForma forma;

    si (v == NIHIL)
    {
        redde NIHIL;
    }
    si (   v->compositio && v->latitudo == latitudo
        && v->altitudo == altitudo && v->spatium == spatium)
    {
        redde v->compositio;
    }
    si (v->piscina_compositionis)
    {
        piscina_destruere(v->piscina_compositionis);
    }
    v->compositio             = NIHIL;
    v->piscina_compositionis  = piscina_generare_dynamicum(
        "antiphonale_compositio", (memoriae_index)(CCLVI * MXXIV));
    si (v->piscina_compositionis == NIHIL)
    {
        redde NIHIL;
    }
    gabc_compositionis_formam_ordinariam(&forma);
    forma.latitudo  = latitudo;
    forma.altitudo  = altitudo;
    forma.spatium   = spatium;
    v->latitudo     = latitudo;
    v->altitudo     = altitudo;
    v->spatium      = spatium;
    v->compositio   = gabc_componere(v->cantus, &forma,
        v->piscina_compositionis);
    redde v->compositio;
}

interior GabcCompositio*
compositio_currens (
    AntiphonaleMontatio* m)
{
    Antiphonale* a = (Antiphonale*)m->figurae_ctx;

    redde componere_ad(a->versio, latitudo_paginae(m),
        altitudo_paginae(m), spatium_currens(m));
}

/* pagina visa, ad compositionem coercita */
interior s32
pagina_currens (
       constans AntiphonaleMontatio* m,
            constans GabcCompositio* c)
{
    s32 p = attributum_s32(&m->ramus, "pagina", ZEPHYRUM);

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
 * legit: latus carmen.h nescit); causa si non */
interior vacuum
carmen_aedificare (
    Versio* v)
{
      GabcCarmenForma  forma;
    ChordaAedificator* a;
    ChordaAedificator* b;
                  i32  n;

    v->carmen.datum     = NIHIL;
    v->carmen.mensura   = ZEPHYRUM;
    v->causa[ZEPHYRUM]  = '\0';
    gabc_carmen_formam_ordinariam(&forma);
    a = chorda_aedificator_creare(v->piscina, MMM);
    b = chorda_aedificator_creare(v->piscina, MMM);
    si (!a || !b)
    {
        sprintf(v->causa, "out of memory");
        redde;
    }
    si (!gabc_carmen_scribere(v->cantus, &forma, v->piscina, a,
            &v->relatio))
    {
        sprintf(v->causa, "%s", v->cantus->numerus_notarum == ZEPHYRUM
            ? "no notes" : "the converter refused it");
        redde;
    }
    chorda_aedificator_appendere_literis(b, "<carmina>\n");
    chorda_aedificator_appendere_chorda(b,
        chorda_aedificator_finire(a));
    chorda_aedificator_appendere_literis(b, "</carmina>\n");
    si (v->relatio.numerus_nexuum == ZEPHYRUM)
    {
        sprintf(v->causa, "no notes");
        redde;
    }
    v->carmen  = chorda_aedificator_finire(b);
    n          = v->relatio.numerus_nexuum;
    v->finis   = v->relatio.nexus[n - I].initium
               + v->relatio.nexus[n - I].longitudo;
}

/* nexus ultimus cuius initium <= pulsus (quaestio bipartita); -1 */
interior s32
nexus_ad_pulsum (
    constans Versio* v,
                s64  pulsus)
{
    s32 imum;
    s32 summum;
    s32 medium;

    si (   v == NIHIL || v->carmen.mensura == ZEPHYRUM
        || pulsus < ZEPHYRUM)
    {
        redde -I;
    }
    imum    = ZEPHYRUM;
    summum  = (s32)v->relatio.numerus_nexuum;
    dum (imum < summum)
    {
        medium = (imum + summum) / II;
        si (v->relatio.nexus[medium].initium <= pulsus)
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
    constans Versio* v,
                s64  pulsus)
{
    s32 n = nexus_ad_pulsum(v, pulsus);

    redde n < ZEPHYRUM ? -I : (s32)v->relatio.nexus[n].syllaba;
}

/* nexus primus syllabae >= s (directio +I) aut ultimus syllabae <= s
 * (directio -I, eius syllabae nexus primus); -1 si nullus */
interior s32
nexus_syllabae (
    constans Versio* v,
                s32  s,
                s32  directio)
{
    s32 i;
    s32 n;
    s32 inventa;

    si (v == NIHIL || v->carmen.mensura == ZEPHYRUM)
    {
        redde -I;
    }
    n = (s32)v->relatio.numerus_nexuum;
    si (directio > ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i < n; i++)
        {
            si ((s32)v->relatio.nexus[i].syllaba >= s)
            {
                redde i;
            }
        }
        redde -I;
    }
    inventa = -I;
    per (i = n - I; i >= ZEPHYRUM; i--)
    {
        si ((s32)v->relatio.nexus[i].syllaba <= s)
        {
            inventa = (s32)v->relatio.nexus[i].syllaba;
            frange;
        }
    }
    redde inventa < ZEPHYRUM ? -I : nexus_syllabae(v, inventa, I);
}


/* ==================================================
 * Lectio (G13)
 * ================================================== */

/* plagulam legit; versio nova si bona (vetus post aedificationem
 * destruitur), aliter vexillum. VERUM si quid mutatum */
interior b32
relegere (
    AntiphonaleMontatio* m)
{
    Antiphonale* a = (Antiphonale*)m->figurae_ctx;
            s64  pulsus;
      character  vexillum[ANTIPHONALE_VEXILLUM];
        Piscina* p;
         chorda  textus;
     GabcCantus* cantus;
         Versio* nova;
            b32  mutatum;
            i32  i;

    si (a->via[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }
    vexillum[ZEPHYRUM]  = '\0';
    nova                = NIHIL;
    si (!filum_existit(a->via))
    {
        sprintf(vexillum, "missing: %.200s", a->via);
    }
    alioquin
    {
        p = piscina_generare_dynamicum("antiphonale_versio",
            (memoriae_index)(LXIV * MXXIV));
        si (p == NIHIL)
        {
            redde FALSUM;
        }
        textus = filum_legere_totum(a->via, p);
        cantus = textus.mensura > ZEPHYRUM ? gabc_legere(textus,
            p) : NIHIL;
        si (textus.mensura == ZEPHYRUM)
        {
            sprintf(vexillum, "empty file");
        }
        alioquin si (cantus == NIHIL)
        {
            sprintf(vexillum, "out of memory");
        }
        alioquin si (!gabc_successus(cantus))
        {
            per (i = ZEPHYRUM; i < cantus->numerus_vitiorum; i++)
            {
                si (cantus->vitia[i].gravitas == GABC_VITIUM)
                {
                    sprintf(vexillum, "%u:%u: %.*s",
                        (unsigned)cantus->vitia[i].linea,
                        (unsigned)cantus->vitia[i].columna,
                        (integer)(cantus->vitia[i].causa.mensura > CC
                            ? CC : cantus->vitia[i].causa.mensura),
                        (constans character*)
                            cantus->vitia[i].causa.datum);
                    frange;
                }
            }
        }
        alioquin si (   a->versio
                     && a->versio->textus.mensura == textus.mensura
                     && memcmp(a->versio->textus.datum, textus.datum,
                            (size_t)textus.mensura) == ZEPHYRUM)
        {
            /* idem textus (eventus speculae spurius): versio manet */
            nova = NIHIL;
        }
        alioquin
        {
            nova = (Versio*)piscina_allocare(p, magnitudo(Versio));
            si (nova)
            {
                memset(nova, ZEPHYRUM,
                    magnitudo(Versio));
                nova->piscina =
                                                           p;
                nova->textus = textus;
                nova->cantus = cantus;
                carmen_aedificare(nova);
            }
        }
        si (nova == NIHIL)
        {
            piscina_destruere(p);
        }
    }
    mutatum = strcmp(vexillum, a->vexillum) != ZEPHYRUM;
    strcpy(a->vexillum, vexillum);
    si (nova)
    {
        /* G18: sonans ad pulsum eundem in versionem novam transit
         * (cantor monstratorem solum tenet); brevior aut non sonabilis
         * sistit. Vetus solum post novam aedificatam et traditam */
        si (a->sonat)
        {
            pulsus =
                m->porta.pulsus ? m->porta.pulsus(m->porta.ctx) : -I;
            si (pulsus < ZEPHYRUM)
            {
                a->sonat = FALSUM;
            }
            alioquin si (   nova->carmen.mensura == ZEPHYRUM
                         || pulsus               >= nova->finis
                         || !m->porta.incipere)
            {
                si (m->porta.sistere)
                {
                    m->porta.sistere(m->porta.ctx);
                }
                a->sonat = FALSUM;
            }
            alioquin si (!m->porta.incipere(m->porta.ctx, nova->carmen,
                             pulsus))
            {
                a->sonat = FALSUM;
            }
            si (!a->sonat)
            {
                a->illuminata = -I;
            }
        }
        versionem_destruere(a->versio);
        a->versio  = nova;
        mutatum    = VERUM;
    }
    redde mutatum;
}

interior vacuum
speculam_claudere (
    Antiphonale* a)
{
    si (a->specula)
    {
        specula_claudere(a->specula);
        a->specula = NIHIL;
    }
    si (a->piscina_speculae)
    {
        piscina_destruere(a->piscina_speculae);
        a->piscina_speculae = NIHIL;
    }
}

/* specula in piscina sua: temptatio fracta (directorium absens) nihil
 * relinquit, ergo iterum temptari potest sine fine */
interior vacuum
speculam_aperire (
    Antiphonale* a)
{
    speculam_claudere(a);
    a->piscina_speculae = piscina_generare_dynamicum(
        "antiphonale_specula", (memoriae_index)M);
    si (a->piscina_speculae == NIHIL)
    {
        redde;
    }
    a->specula = specula_creare(a->piscina_speculae, a->via);
    si (a->specula == NIHIL)
    {
        speculam_claudere(a);
    }
}

interior vacuum
viam_ponere (
    AntiphonaleMontatio* m,
     constans character* via)
{
    Antiphonale* a = (Antiphonale*)m->figurae_ctx;

    speculam_claudere(a);
    si (a->sonat && m->porta.sistere)
    {
        m->porta.sistere(m->porta.ctx);
    }
    a->sonat       = FALSUM;
    a->illuminata  = -I;
    a->electa      = -I;
    versionem_destruere(a->versio);
    a->versio              = NIHIL;
    a->vexillum[ZEPHYRUM]  = '\0';
    a->via[ZEPHYRUM]       = '\0';
    si (via && via[ZEPHYRUM])
    {
        strncpy(a->via, via, ANTIPHONALE_VIA - I);
        a->via[ANTIPHONALE_VIA - I] = '\0';
        speculam_aperire(a);
    }
    (vacuum)relegere(m);
}


/* ==================================================
 * Praecepta
 * ================================================== */

interior b32
paginam_movere (
    AntiphonaleMontatio* m,
                    s32  gradus)
{
    GabcCompositio* c;
               s32  p;

    c = compositio_currens(m);
    si (c == NIHIL)
    {
        redde VERUM;
    }
    p = pagina_currens(m, c) + gradus;
    si (p < ZEPHYRUM || p >= (s32)c->numerus_paginarum)
    {
        redde VERUM;
    }
    redde numerum_scribere(m, "pagina", p);
}

/* spatium mutatum; syllaba prima visa in conspectu manet */
interior b32
spatium_movere (
    AntiphonaleMontatio* m,
                    s32  gradus)
{
       Antiphonale* a = (Antiphonale*)m->figurae_ctx;
    GabcCompositio* c;
               s32  spatium;
               s32  prima;
               s32  pagina;
               i32  s;

    spatium = (s32)spatium_currens(m) + gradus;
    si (spatium < VI || spatium > XLVIII)
    {
        redde VERUM;
    }
    c      = compositio_currens(m);
    prima  = -I;
    si (c)
    {
        pagina = pagina_currens(m, c);
        per (s = ZEPHYRUM; s < c->numerus_syllabarum; s++)
        {
            si (c->syllabae[s].pagina == pagina)
            {
                prima = (s32)s;
                frange;
            }
        }
    }
    si (!numerum_scribere(m, "spatium", spatium))
    {
        redde FALSUM;
    }
    c = componere_ad(a->versio, latitudo_paginae(m),
        altitudo_paginae(m),
        (i32)spatium);
    si (c && prima >= ZEPHYRUM && c->syllabae[prima].pagina >= ZEPHYRUM)
    {
        redde numerum_scribere(m, "pagina", c->syllabae[prima].pagina);
    }
    redde VERUM;
}

interior b32
fac_proximam (
    vacuum* ctx)
{
    redde paginam_movere((AntiphonaleMontatio*)ctx, I);
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    redde paginam_movere((AntiphonaleMontatio*)ctx, -I);
}

interior b32
fac_maius (
    vacuum* ctx)
{
    redde spatium_movere((AntiphonaleMontatio*)ctx, II);
}

interior b32
fac_minus (
    vacuum* ctx)
{
    redde spatium_movere((AntiphonaleMontatio*)ctx, -II);
}

/* syllaba visa sonans aut electa */
interior s32
syllaba_currens (
    constans Antiphonale* a)
{
    redde a->sonat ? a->illuminata : a->electa;
}

/* sonans: quaerere ad nexum; sistens: eligere */
interior b32
ad_syllabam_ire (
    AntiphonaleMontatio* m,
                    s32  nexus)
{
    Antiphonale* a = (Antiphonale*)m->figurae_ctx;
            s32  s;

    si (nexus < ZEPHYRUM || a->versio == NIHIL)
    {
        redde VERUM;
    }
    s = (s32)a->versio->relatio.nexus[nexus].syllaba;
    si (a->sonat)
    {
        si (m->porta.quaerere)
        {
            m->porta.quaerere(m->porta.ctx,
                a->versio->relatio.nexus[nexus].initium);
        }
        a->illuminata = s;
    }
    alioquin
    {
        a->electa = s;
    }
    redde VERUM;
}

interior b32
fac_canere (
    vacuum* ctx)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
            Antiphonale* a = (Antiphonale*)m->figurae_ctx;
                    s32  nexus;
                    s64  pulsus;

    si (a->sonat)
    {
        si (m->porta.sistere)
        {
            m->porta.sistere(m->porta.ctx);
        }
        a->sonat       = FALSUM;
        a->electa      = a->illuminata;
        a->illuminata  = -I;
        redde VERUM;
    }
    si (a->versio == NIHIL)
    {
        redde VERUM;
    }
    si (a->versio->carmen.mensura == ZEPHYRUM)
    {
        sprintf(a->vexillum, "not playable: %.200s", a->versio->causa);
        redde VERUM;
    }
    si (!m->porta.incipere)
    {
        redde VERUM;
    }
    nexus   = a->electa >= ZEPHYRUM ? nexus_syllabae(a->versio,
        a->electa,
                  I) : ZEPHYRUM;
    pulsus  = nexus
        >= ZEPHYRUM ? a->versio->relatio.nexus[nexus].initium
                                : ZEPHYRUM;
    si (m->porta.incipere(m->porta.ctx, a->versio->carmen, pulsus))
    {
        a->sonat       = VERUM;
        a->illuminata  = syllaba_ad_pulsum(a->versio, pulsus);
        a->electa      = -I;
    }
    redde VERUM;
}

interior b32
fac_priorem_syllabam (
    vacuum* ctx)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
            Antiphonale* a = (Antiphonale*)m->figurae_ctx;
                    s32  s = syllaba_currens(a);

    si (s <= ZEPHYRUM)
    {
        redde VERUM;
    }
    redde ad_syllabam_ire(m, nexus_syllabae(a->versio, s - I, -I));
}

interior b32
fac_proximam_syllabam (
    vacuum* ctx)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
            Antiphonale* a = (Antiphonale*)m->figurae_ctx;

    redde ad_syllabam_ire(m, nexus_syllabae(a->versio,
        syllaba_currens(a) + I, I));
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "antiphonale.canere", "Play / stop", " ", VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_canere },
    { "antiphonale.syllaba_prior", "Previous syllable", "Left",
        VISUS_ANTIPHONALIS, ZEPHYRUM, fac_priorem_syllabam },
    { "antiphonale.syllaba_proxima", "Next syllable", "Right",
        VISUS_ANTIPHONALIS, ZEPHYRUM, fac_proximam_syllabam },
    { "antiphonale.proxima", "Next page", "PageDown",
        VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_proximam },
    { "antiphonale.prior", "Previous page", "PageUp",
        VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_priorem },
    { "antiphonale.deorsum", "Next page", "j", VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_proximam },
    { "antiphonale.sursum", "Previous page", "k", VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_priorem },
    { "antiphonale.maius", "Zoom in", "+", VISUS_ANTIPHONALIS, ZEPHYRUM,
        fac_maius },
    { "antiphonale.maius_aequale", "Zoom in", "=", VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_maius },
    { "antiphonale.minus", "Zoom out", "-", VISUS_ANTIPHONALIS,
        ZEPHYRUM,
        fac_minus },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

interior b32
requisita_nulla (
        i32  requisita,
     vacuum* ctx)
{
    (vacuum)requisita;
    (vacuum)ctx;
    redde VERUM;
}


/* ==================================================
 * Actio
 * ================================================== */

interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
    constans Praeceptum* t;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, VISUS_ANTIPHONALIS,
            ev);
        redde t && praecepta_status(t, VISUS_ANTIPHONALIS,
            requisita_nulla, m) == PRAECEPTUM_PRAESTO && t->fn(m);
    }
    si (   nodus && ev->genus == EVENTUS_MUS_DEPRESSUS
        && ev->datum.mus.botton == MUS_SINISTER)
    {
        si (chorda_aequalis_literis(nodus->id, "sagitta_prior"))
        {
            redde paginam_movere(m, -I);
        }
        si (chorda_aequalis_literis(nodus->id,
                     "sagitta_proxima"))
        {
            redde paginam_movere(m, I);
        }
        si (chorda_aequalis_literis(nodus->id, "globulus_canere"))
        {
            redde fac_canere(m);
        }
        si (chorda_aequalis_literis(nodus->id, "antiphonale"))
        {
               Antiphonale* a     = (Antiphonale*)m->figurae_ctx;
            GabcCompositio* comp  = compositio_currens(m);
                   Punctum  p;
                       s32  s;

            si (comp == NIHIL)
            {
                redde FALSUM;
            }
            p.x  = (s32)ev->datum.mus.x;
            p.y  = (s32)ev->datum.mus.y;
            p    = destinatio_ad_locale(nodus, p);
            s    = gabc_compositio_syllaba_ad_punctum(comp,
                (i32)pagina_currens(m, comp), p.x
                    - ANTIPHONALE_LIMBUS_X,
                p.y - ANTIPHONALE_LIMBUS_Y);
            si (s < ZEPHYRUM)
            {
                redde FALSUM;
            }
            si (a->sonat)
            {
                redde ad_syllabam_ire(m, nexus_syllabae(a->versio, s,
                    I));
            }
            a->electa = s;
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Titulus
 * ================================================== */

/* name capitis aut nomen plagulae; vacuum si nulla */
interior chorda
titulum_capere (
    constans AntiphonaleMontatio* m)
{
    constans Antiphonale* a = (constans Antiphonale*)m->figurae_ctx;
      constans character* ultimum;
                  chorda  t;

    si (a->versio && a->versio->cantus->titulus.mensura > ZEPHYRUM)
    {
        redde a->versio->cantus->titulus;
    }
    ultimum    = strrchr(a->via, '/');
    ultimum    = ultimum ? ultimum + I : a->via;
    t.datum    = (i8*)NIHIL;
    t.mensura  = ZEPHYRUM;
    si (*ultimum)
    {
        t = chorda_ex_literis(ultimum, m->piscina);
    }
    redde t;
}

chorda
antiphonale_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
                 chorda  t;

    t = titulum_capere(m);
    si (t.mensura == ZEPHYRUM)
    {
        t = chorda_ex_literis("Antiphonale", piscina);
    }
    redde t;
}


/* ==================================================
 * Figura
 * ================================================== */

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

interior vacuum
figura_antiphonalis (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
        AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
       constans Antiphonale* a;
    constans GabcCompositio* comp;
                      Fines  f;
          GabcPicturaStatus  status;
                      Fines  fundus;
                      Fines  vexillum;
                     chorda  titulus;
                  character  pes[CCLVI];
                        s32  pagina;
                        s32  y_pedis;

    (vacuum)thema;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, f, chorda_aequalis_literis(c->titulus,
        "focatum"));
    /* fundus intra limbum: textura foci solum in margine (ut
     * navigator, chronica) */
    fundus.x         = ANTIPHONALE_LIMBUS_X;
    fundus.y         = ANTIPHONALE_LIMBUS_Y;
    fundus.latitudo  = f.latitudo - II * ANTIPHONALE_LIMBUS_X;
    fundus.altitudo  = f.altitudo - II * ANTIPHONALE_LIMBUS_Y;
    mandata_rectangulum(md, fundus, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!m || !m->figurae_ctx)
    {
        redde;
    }
    a     = (constans Antiphonale*)m->figurae_ctx;
    comp  = a->versio ? a->versio->compositio : NIHIL;
    si (a->via[ZEPHYRUM] == '\0')
    {
        textum_pingere(md, ANTIPHONALE_LIMBUS_X + VIII,
            ANTIPHONALE_LIMBUS_Y + VIII, chorda_ex_literis(
            "Open a chant with $antiphonale(path)", md->piscina),
            COLOR_TEXT_DIM);
    }
    alioquin si (a->versio && comp == NIHIL)
    {
        textum_pingere(md, ANTIPHONALE_LIMBUS_X + VIII,
            ANTIPHONALE_LIMBUS_Y + XX, chorda_ex_literis(
            "pane too narrow for this staff size", md->piscina),
            COLOR_TEXT_DIM);
    }
    pagina = pagina_currens(m, comp);
    si (comp)
    {
        status.syllaba_illuminata  = syllaba_currens(a);
        status.nota_illuminata     = -I;
        (vacuum)gabc_paginam_pingere(comp, (i32)pagina,
            ANTIPHONALE_LIMBUS_X, ANTIPHONALE_LIMBUS_Y, &status, md);
    }
    /* linea ima: "< titulus · 2/5 >" */
    titulus = titulum_capere(m);
    /* "<  >/#  titulus  2/5  >": sagitta, globulus canendi (sonans
     * quadratum, aliter triangulum), titulus et pagina, sagitta */
    y_pedis = f.altitudo - ANTIPHONALE_LIMBUS_Y - ANTIPHONALE_LINEA
        + II;
    textum_pingere(md, ANTIPHONALE_LIMBUS_X + II, y_pedis,
        chorda_ex_literis("<", md->piscina), COLOR_TEXT);
    textum_pingere(md, ANTIPHONALE_LIMBUS_X + ANTIPHONALE_SAGITTA,
        y_pedis,
        chorda_ex_literis(a->sonat ? "\xE2\x96\xA0" : "\xE2\x96\xB6",
            md->piscina), a->sonat ? COLOR_ACCENT_PRIMARY : COLOR_TEXT);
    sprintf(pes, "%.*s  %d/%u", (integer)(titulus.mensura > C
        ? C : titulus.mensura), titulus.datum
        ? (constans character*)titulus.datum : "",
        (integer)(pagina + I),
        comp ? (unsigned)comp->numerus_paginarum : 0u);
    textum_pingere(md, ANTIPHONALE_LIMBUS_X + ANTIPHONALE_SAGITTA
        + ANTIPHONALE_GLOBULUS, y_pedis, chorda_ex_literis(pes,
        md->piscina), COLOR_TEXT);
    textum_pingere(md, f.latitudo - ANTIPHONALE_LIMBUS_X - VIII,
        y_pedis,
        chorda_ex_literis(">", md->piscina), COLOR_TEXT);
    /* vexillum vitii supra paginam */
    si (a->vexillum[ZEPHYRUM])
    {
        vexillum.x         = ANTIPHONALE_LIMBUS_X;
        vexillum.y         = ANTIPHONALE_LIMBUS_Y;
        vexillum.latitudo  = fundus.latitudo;
        vexillum.altitudo  = ANTIPHONALE_LINEA;
        mandata_rectangulum(md, vexillum, figura_color_thematis(
            COLOR_ERROR), VERUM);
        textum_pingere(md, ANTIPHONALE_LIMBUS_X + II,
            ANTIPHONALE_LIMBUS_Y + II, chorda_ex_literis(a->vexillum,
            md->piscina), COLOR_BACKGROUND);
    }
}


/* ==================================================
 * Componere, montare, pulsus
 * ================================================== */

Componens*
antiphonale_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
              Componens* radix;
              Componens* prior;
              Componens* proxima;
              Componens* globulus;
                  Fines  f;
                  Fines  s;

    (vacuum)repo;
    si (!m)
    {
        redde NIHIL;
    }
    (vacuum)compositio_currens(m);
    radix    = componens_creare(piscina, intern, "antiphonale",
        PARTES_TABULA);
    prior    = componens_creare(piscina, intern, "sagitta_prior",
        PARTES_NULLUM);
    proxima  = componens_creare(piscina, intern,
                               "sagitta_proxima",
                               PARTES_NULLUM);
    globulus = componens_creare(piscina, intern, "globulus_canere",
        PARTES_NULLUM);
    si (!radix || !prior || !proxima || !globulus)
    {
        redde radix;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = superficies(m, VERUM);
    f.altitudo  = superficies(m, FALSUM);
    componens_ponere_fines(radix, f);
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_antiphonalis);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    s.x         = ANTIPHONALE_LIMBUS_X;
    s.y         = f.altitudo - ANTIPHONALE_LIMBUS_Y - ANTIPHONALE_LINEA;
    s.latitudo  = ANTIPHONALE_SAGITTA;
    s.altitudo  = ANTIPHONALE_LINEA;
    componens_ponere_fines(prior, s);
    componens_ponere_actio(prior, actio_antiphonalis);
    s.x = f.latitudo - ANTIPHONALE_LIMBUS_X - ANTIPHONALE_SAGITTA;
    componens_ponere_fines(proxima, s);
    componens_ponere_actio(proxima,
        actio_antiphonalis);
    s.x = ANTIPHONALE_LIMBUS_X
                                    + ANTIPHONALE_SAGITTA;
    s.latitudo = ANTIPHONALE_GLOBULUS;
    componens_ponere_fines(globulus, s);
    componens_ponere_actio(globulus, actio_antiphonalis);
    componens_addere_liberum(radix, prior);
    componens_addere_liberum(radix, proxima);
    componens_addere_liberum(radix, globulus);
    redde radix;
}

b32
antiphonale_montare (
          AntiphonaleMontatio* m,
                      Piscina* piscina,
          InternamentumChorda* intern,
           InsulaRepositorium* repo,
           constans character* id,
           constans character* via,
                          i32  latitudo,
                          i32  altitudo,
    constans AntiphonalePorta* porta,
              StatusSystemata* systemata)
{
        Organum* organum;
    Antiphonale* a;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(AntiphonaleMontatio));
    m->piscina   = piscina;
    m->intern    = intern;
    m->latitudo  = latitudo;
    m->altitudo  = altitudo;
    si (porta)
    {
        m->porta = *porta;
    }
    m->ramus = id ? insula_ramus(repo, "antiphonale", id)
                  : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "antiphonale",
        "antiphonale");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"antiphonale\"", id ? id : "antiphonale",
               piscina, intern))
    {
        redde FALSUM;
    }
    a = (Antiphonale*)piscina_allocare(piscina, magnitudo(Antiphonale));
    si (a == NIHIL)
    {
        redde FALSUM;
    }
    memset(a, ZEPHYRUM, magnitudo(Antiphonale));
    a->illuminata   = -I;
    a->electa       = -I;
    m->figurae_ctx  = a;
    m->actiones     = actio_registrum_creare(piscina, intern);
    m->figurae      = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_antiphonalis, agere, m)
        || !figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_antiphonalis, m))
    {
        redde FALSUM;
    }
    viam_ponere(m, via);
    redde VERUM;
}

b32
antiphonale_argumentum_ponere (
       AntiphonaleMontatio* m,
        constans character* via)
{
    Antiphonale* a;

    si (!m || !m->figurae_ctx || !via || via[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }
    a = (Antiphonale*)m->figurae_ctx;
    si (strcmp(a->via, via) != ZEPHYRUM)
    {
        viam_ponere(m, via);
        (vacuum)numerum_scribere(m, "pagina", ZEPHYRUM);
    }
    redde VERUM;
}

b32
antiphonale_pulsare (
    vacuum* ctx)
{
     AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
             Antiphonale* a;
          GabcCompositio* comp;
                     s64  pulsus;
                     s32  s;
                     b32  mutatum;

    si (!m || !m->figurae_ctx)
    {
        redde FALSUM;
    }
    a = (Antiphonale*)m->figurae_ctx;
    si (a->via[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }
    mutatum = FALSUM;
    si (a->specula == NIHIL)
    {
        /* platforma sine specula aut directorium absens: relegere et
         * iterum temptare omni pulsu (MORA_SINE_SPECULA) */
        speculam_aperire(a);
        mutatum = relegere(m);
    }
    alioquin si (specula_exhaurire(a->specula))
    {
        mutatum = relegere(m);
    }
    /* sonus (P7): syllaba nexus currentis illuminatur, pagina eam
     * sequitur (etiam post relaxationem: zoom, permutatio). Illuminatio
     * sonum mora apparatus praecedit (v1, worklog) */
    si (a->sonat)
    {
        pulsus = m->porta.pulsus ? m->porta.pulsus(m->porta.ctx) : -I;
        s      = pulsus < ZEPHYRUM ? -I : syllaba_ad_pulsum(a->versio,
                     pulsus);
        si (pulsus < ZEPHYRUM)
        {
            a->sonat = FALSUM;
        }
        si (s != a->illuminata)
        {
            a->illuminata  = s;
            mutatum        = VERUM;
        }
        comp = s >= ZEPHYRUM ? compositio_currens(m) : NIHIL;
        si (   comp && s < (s32)comp->numerus_syllabarum
            && comp->syllabae[s].pagina >= ZEPHYRUM
            && comp->syllabae[s].pagina != pagina_currens(m, comp))
        {
            (vacuum)numerum_scribere(m, "pagina",
                comp->syllabae[s].pagina);
            mutatum = VERUM;
        }
    }
    redde mutatum;
}

Mora
antiphonale_mora (
    vacuum* ctx)
{
     AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
    constans Antiphonale* a;

    si (!m || !m->figurae_ctx)
    {
        redde -I;
    }
    a = (constans Antiphonale*)m->figurae_ctx;
    si (a->via[ZEPHYRUM] == '\0')
    {
        redde -I;
    }
    si (a->sonat)
    {
        redde MORA_SONANS;
    }
    redde a->specula ? -I : MORA_SINE_SPECULA;
}

s32
antiphonale_fossa (
    vacuum* ctx,
       i32* eventus)
{
     AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
    constans Antiphonale* a;

    *eventus = REACTOR_LEGERE;
    si (!m || !m->figurae_ctx)
    {
        redde -I;
    }
    a = (constans Antiphonale*)m->figurae_ctx;
    redde a->specula ? specula_fossa(a->specula) : -I;
}
