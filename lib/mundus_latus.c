/* mundus_latus.c - latus mundi (vide include/mundus_latus.h)
 *
 * Arbor: radix "mundus" (PARTES_TABULA, figura tota) cum liberis qui
 * areae ictus sunt - "pictura", "sagitta_prior", "sagitta_proxima".
 *
 * Pulsus: volumen inspicit (locus, personae), mundum tempore vero
 * promovet (intentum ex clavibus tentis), quadrum reficit cum clavis
 * eius mutatur. Clavis quadri = sigillum personae, locus et positura
 * corporis, mensura. Et pulsus et compositor quadrum curant; solus
 * pulsus mundum promovet.
 *
 * Ostia (ostium-plan T3): index in piscina_indicis cum indicibus
 * relegitur (ostia sola mutata locum non mutant). Ostium propinquum
 * solus pulsus computat, post gradum mundi; figura et compositor
 * mundus_latus_signum rogant, quae ex statu praesenti capsam dat -
 * area "ostium" et capsa picta semper congruunt.
 */
#include "mundus_latus.h"
#include "membrum__praecepta.h"
#include "statua_glb.h"
#include "statuae_repositorium.h"
#include "dithering.h"
#include "stereometria.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include "destinatio.h"
#include "mandatum.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define LATUS_LATITUDO    CCCLX   /* superficies ordinaria */
#define LATUS_ALTITUDO    CDL
#define ORDO_ALTITUDO     XII     /* ordo capitis */
#define LIMBUS            II
#define SAGITTA_LATITUDO  XII
/* globulus modi humi: "exemplaria" + limbi */
#define HUMUS_LATITUDO    LXVIII
#define HUMUS_EXEMPLARIA  "exemplaria"
#define HUMUS_ATKINSON    "atkinson"
#define LITTERA_LATITUDO  VI
#define LITTERA_ALTITUDO  VIII
#define VISUS_MUNDI       I
/* index quadri ubi locus non tegit: pixelum perlucidum (fundus
 * thematis sub pictura apparet) */
#define INDEX_FUNDI       0xFF
/* persona ordinaria: prima cuius titulus hoc continet */
#define PERSONA_ORDINARIA "mushroom"

hic_manens constans character actio_mundi[] = "mundus.agere";

/* dispositio lateris (spatium radicis) */
nomen structura {
    Fines prior;
    Fines proxima;
    Fines humus;        /* globulus modi humi, in fine capitis */
    Fines pictura;
      s32 numerus_x;    /* textus "2/5" */
      s32 titulus_x;
      s32 titulus_latitudo;
} Dispositio;


/* ==================================================
 * Status (ramus)
 * ================================================== */

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a = insula_ramus_attributum(ramus, titulus);
       s32  v;

    si (!a || !chorda_ut_s32(*a, &v))
    {
        redde praestitutum;
    }
    redde v;
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

    si (!par->valor || !par->valor[ZEPHYRUM])
    {
        (vacuum)insula_attributum_tollere(radix, par->titulus);
        redde;
    }
    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

/* campi scriptura, scriptore lateris (vocatur etiam extra actionem);
 * valor vacuus campum tollit */
interior b32
campum_scribere (
    MundusLatusMontatio* m,
     constans character* titulus,
     constans character* valor)
{
    ParCampi par;
      chorda prior = m->ramus.repo->scriptor;
         b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_mundi, m->piscina));
    ok = mutare_ramum(&m->ramus, campum_ponere, &par);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior s64
nunc (
    constans MundusLatusMontatio* m)
{
    redde m->porta.nunc ? m->porta.nunc(m->porta.ctx) : ZEPHYRUM;
}


/* ==================================================
 * Indices, locus, persona
 * ================================================== */

interior i32
personarum_numerus (
    constans MundusLatusMontatio* m)
{
    redde m->personae ? xar_numerus(m->personae) : ZEPHYRUM;
}

interior StatuaeInscriptio*
inscriptio (
    constans MundusLatusMontatio* m,
                             i32  k)
{
    redde (StatuaeInscriptio*)xar_obtinere(m->personae, k);
}

/* titulus fragmentum continet? (litterae ut scriptae) */
interior b32
titulus_continet (
                chorda  titulus,
    constans character* fragmentum)
{
    i32 longitudo = (i32)strlen(fragmentum);
    i32 k;

    per (k = ZEPHYRUM; k + longitudo <= titulus.mensura; k++)
    {
        si (memcmp(titulus.datum + k, fragmentum, longitudo)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* locus personae currentis in indice: titulus status; aliter prima
 * cuius titulus PERSONA_ORDINARIA continet; aliter prima; -1 si index
 * vacuus */
interior s32
locus_personae (
    constans MundusLatusMontatio* m)
{
    chorda* a = insula_ramus_attributum(&m->ramus, "persona");
       i32  k;

    si (personarum_numerus(m) == ZEPHYRUM)
    {
        redde -I;
    }
    per (k = ZEPHYRUM; a && k < personarum_numerus(m); k++)
    {
        si (chorda_aequalis(inscriptio(m, k)->titulus, *a))
        {
            redde (s32)k;
        }
    }
    per (k = ZEPHYRUM; k < personarum_numerus(m); k++)
    {
        si (titulus_continet(inscriptio(m, k)->titulus,
                PERSONA_ORDINARIA))
        {
            redde (s32)k;
        }
    }
    redde ZEPHYRUM;
}

interior vacuum
indices_relegere (
    MundusLatusMontatio* m)
{
    piscina_vacare(m->piscina_indicis);
    m->personae  = m->volumen
        ? statuae_enumerare(m->volumen, m->piscina_indicis) : NIHIL;
    m->loci      = m->volumen
        ? statuae_enumerare_generis(m->volumen, STATUAE_GENUS_LOCUS,
              m->piscina_indicis)
        : NIHIL;
    /* ostia loci ostensi (primi indicis) */
    m->ostia     = m->volumen && m->loci
        && xar_numerus(m->loci) > ZEPHYRUM
        ? mundus_ostia_legere(m->volumen,
              ((StatuaeInscriptio*)xar_obtinere(m->loci, ZEPHYRUM))
                  ->titulus, m->piscina_indicis)
        : NIHIL;
    m->ostium_propinquum = -I;
    m->seq_visum = m->volumen ? volumen_seq_ultimum(m->volumen)
                              : ZEPHYRUM;
}

/* locum ex memoria tollere */
interior vacuum
locum_tollere (
    MundusLatusMontatio* m)
{
    piscina_vacare(m->piscina_loci);
    piscina_vacare(m->piscina_humi);
    m->locus                  = NIHIL;
    m->solum                  = NIHIL;
    m->humus                  = NIHIL;
    m->sigillum_loci.mensura  = ZEPHYRUM;
    m->causa_loci.mensura     = ZEPHYRUM;
    m->positum                = FALSUM;
    m->mundus.solum           = NIHIL;
    m->mundus.corporum        = ZEPHYRUM;
    memset(m->tentae, ZEPHYRUM, magnitudo(m->tentae));
}

/* locum legere si alius (aut nullus) nunc videndus est: statua,
 * tabula gressus, humus; corpus in initio ponitur. VERUM si quid
 * mutatum */
interior b32
locum_curare (
    MundusLatusMontatio* m)
{
    StatuaeInscriptio* x;
        StatuaFructus  f;
               chorda  octeti;
                  f32  x_initii = 0.0f;
                  f32  z_initii = 0.0f;
                  b32  inventum = FALSUM;

    si (!m->loci || xar_numerus(m->loci) == ZEPHYRUM)
    {
        si (   !m->locus && m->sigillum_loci.mensura == ZEPHYRUM
            && m->causa_loci.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
        locum_tollere(m);
        redde VERUM;
    }
    x = (StatuaeInscriptio*)xar_obtinere(m->loci, ZEPHYRUM);
    si (   m->sigillum_loci.mensura > ZEPHYRUM
        && chorda_aequalis(m->sigillum_loci, x->sigillum_hex))
    {
        redde FALSUM;
    }
    locum_tollere(m);
    m->sigillum_loci = chorda_transcribere(x->sigillum_hex,
        m->piscina_loci);
    octeti = volumen_massam_promere(m->volumen, x->sigillum_hex,
        m->piscina_loci, &inventum);
    si (!inventum)
    {
        m->causa_loci = chorda_ex_literis("the place's data is missing "
            "from the volume", m->piscina_loci);
        redde VERUM;
    }
    f = statua_glb_legere(octeti, m->piscina_loci);
    si (!f.successus)
    {
        m->causa_loci = f.error;
        redde VERUM;
    }
    m->locus = f.statua;
    m->solum = mundus_solum_struere(m->locus, m->piscina_loci);
    si (!m->solum)
    {
        m->locus       = NIHIL;
        m->causa_loci  = chorda_ex_literis("the place is too large to "
            "walk in", m->piscina_loci);
        redde VERUM;
    }
    memset(&m->corpus, ZEPHYRUM, magnitudo(m->corpus));
    m->corpus.radius   = MUNDUS_CORPORIS_RADIUS;
    m->mundus.solum    = m->solum;
    m->mundus.corpora  = &m->corpus;
    si (   !mundus_solum_initium(m->solum, m->corpus.radius, &x_initii,
               &z_initii)
        || !mundus_corpus_ponere(&m->mundus, &m->corpus, x_initii,
               z_initii))
    {
        m->locus       = NIHIL;
        m->causa_loci  = chorda_ex_literis("the place has nowhere to "
            "walk", m->piscina_loci);
        redde VERUM;
    }
    m->mundus.corporum  = I;
    m->positum          = VERUM;
    m->hora_gradus      = m->hora_visa;
    redde VERUM;
}

/* modus humi quem status vult (ignotus aut absens = Atkinson) */
interior MundusHumiModus
humi_modus_status (
    constans MundusLatusMontatio* m)
{
    chorda* a = insula_ramus_attributum(&m->ramus, "humus");

    redde a && chorda_aequalis_literis(*a, HUMUS_EXEMPLARIA)
        ? MUNDUS_HUMUS_EXEMPLARIA : MUNDUS_HUMUS_ATKINSON;
}

/* humum loci curare: nulla dum locus nullus; nova (piscina vacata,
 * regiones veteres abiectae) cum locus novus est aut modus mutatus.
 * VERUM si quid mutatum */
interior b32
humum_curare (
    MundusLatusMontatio* m)
{
    MundusHumiModus modus = humi_modus_status(m);

    si (!m->locus)
    {
        redde FALSUM;
    }
    si (m->humus && m->humi_modus == modus)
    {
        redde FALSUM;
    }
    piscina_vacare(m->piscina_humi);
    m->humus       = mundus_humus_creare_modi(m->locus, m->activi,
        modus,
        m->piscina_humi);
    m->humi_modus  = modus;
    /* quadrum vetus ex humo vetere factum est */
    m->quadri_clavis[ZEPHYRUM] = '\0';
    redde VERUM;
}

/* personam legere si alia (aut nulla) nunc videnda est; VERUM si
 * quid mutatum */
interior b32
personam_curare (
    MundusLatusMontatio* m)
{
    StatuaeInscriptio* x;
        StatuaFructus  f;
               chorda  octeti;
                  s32  locus     = locus_personae(m);
                  b32  inventum  = FALSUM;

    si (locus < ZEPHYRUM)
    {
        si (   !m->persona && m->sigillum_personae.mensura == ZEPHYRUM
            && m->causa_personae.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
        piscina_vacare(m->piscina_personae);
        m->persona                    = NIHIL;
        m->sigillum_personae.mensura  = ZEPHYRUM;
        m->causa_personae.mensura     = ZEPHYRUM;
        memset(&m->forma, ZEPHYRUM, magnitudo(m->forma));
        redde VERUM;
    }
    x = inscriptio(m, (i32)locus);
    si (   m->sigillum_personae.mensura > ZEPHYRUM
        && chorda_aequalis(m->sigillum_personae, x->sigillum_hex))
    {
        redde FALSUM;
    }
    piscina_vacare(m->piscina_personae);
    m->persona            = NIHIL;
    m->sigillum_personae  = chorda_transcribere(x->sigillum_hex,
        m->piscina_personae);
    m->causa_personae.mensura = ZEPHYRUM;
    memset(&m->forma, ZEPHYRUM, magnitudo(m->forma));
    octeti = volumen_massam_promere(m->volumen, x->sigillum_hex,
        m->piscina_personae, &inventum);
    si (!inventum)
    {
        m->causa_personae = chorda_ex_literis("the character's data is "
            "missing from the volume", m->piscina_personae);
        redde VERUM;
    }
    f = statua_glb_legere(octeti, m->piscina_personae);
    si (!f.successus)
    {
        m->causa_personae = f.error;
        redde VERUM;
    }
    m->persona  = f.statua;
    m->forma    = mundus_forma(m->persona, m->piscina_personae);
    /* persona nova a passu primo incipit */
    m->corpus.tempus_motus = 0.0f;
    redde VERUM;
}


/* ==================================================
 * Dispositio
 * ================================================== */

interior s32
superficies (
    constans MundusLatusMontatio* m,
                             b32  latitudo)
{
    s32 v = latitudo ? (s32)m->latitudo : (s32)m->altitudo;

    si (v <= ZEPHYRUM)
    {
        v = latitudo ? LATUS_LATITUDO : LATUS_ALTITUDO;
    }
    v = attributum_s32(&m->ramus, latitudo ? "superficies_latitudo"
        : "superficies_altitudo", v);
    redde v < ZEPHYRUM ? ZEPHYRUM : v;
}

interior Fines
fines (
    s32 x,
    s32 y,
    s32 latitudo,
    s32 altitudo)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo < ZEPHYRUM ? ZEPHYRUM : latitudo;
    f.altitudo  = altitudo < ZEPHYRUM ? ZEPHYRUM : altitudo;
    redde f;
}

/* numerus "k/n" ("0/0" si index vacuus) */
interior vacuum
numerum_formare (
    constans MundusLatusMontatio* m,
                       character* alveus)
{
    sprintf(alveus, "%d/%u", (integer)(locus_personae(m) + I),
        (insignatus)personarum_numerus(m));
}

interior vacuum
disponere (
    constans MundusLatusMontatio* m,
                      Dispositio* d)
{
    character numerus[XXXII];
          s32 latitudo = superficies(m, VERUM);
          s32 altitudo = superficies(m, FALSUM);
          s32 x;

    numerum_formare(m, numerus);
    d->prior      = fines(LIMBUS, ZEPHYRUM, SAGITTA_LATITUDO,
        ORDO_ALTITUDO);
    d->numerus_x  = LIMBUS + SAGITTA_LATITUDO + LIMBUS;
    x             = d->numerus_x
        + (s32)strlen(numerus) * LITTERA_LATITUDO + LIMBUS;
    d->proxima    = fines(x, ZEPHYRUM, SAGITTA_LATITUDO, ORDO_ALTITUDO);
    d->titulus_x  = x + SAGITTA_LATITUDO + LIMBUS * II;
    d->humus = fines(latitudo - HUMUS_LATITUDO - LIMBUS, ZEPHYRUM,
        HUMUS_LATITUDO, ORDO_ALTITUDO);
    d->titulus_latitudo = d->humus.x - d->titulus_x - LIMBUS;
    si (d->titulus_latitudo < ZEPHYRUM)
    {
        d->titulus_latitudo = ZEPHYRUM;
    }
    d->pictura = fines(ZEPHYRUM, ORDO_ALTITUDO, latitudo,
        altitudo - ORDO_ALTITUDO);
}


/* ==================================================
 * Mundus et quadrum
 * ================================================== */

/* intentum corporis ex clavibus tentis: directio schirmi (dextrorsum,
 * sursum) per gyrum visus in mundum versa */
interior vacuum
intentum_ponere (
    MundusLatusMontatio* m)
{
    f32 angulus  = MUNDUS_GYRUS * STEREOMETRIA_PI / 180.0f;
    f32 cosinus  = (f32)cos((f64)angulus);
    f32 sinus    = (f32)sin((f64)angulus);
    f32 dextrorsum;
    f32 sursum;

    dextrorsum  = (m->tentae[MUNDUS_LATUS_DEXTRUM] ? 1.0f : 0.0f)
        - (m->tentae[MUNDUS_LATUS_SINISTRUM] ? 1.0f : 0.0f);
    sursum      = (m->tentae[MUNDUS_LATUS_SURSUM] ? 1.0f : 0.0f)
        - (m->tentae[MUNDUS_LATUS_DEORSUM] ? 1.0f : 0.0f);
    m->corpus.intendit    = dextrorsum != 0.0f || sursum != 0.0f;
    m->corpus.intentum_x  = dextrorsum * cosinus + sursum * sinus;
    m->corpus.intentum_z  = dextrorsum * sinus - sursum * cosinus;
}

interior b32
clavis_tenetur (
    constans MundusLatusMontatio* m)
{
    redde m->tentae[ZEPHYRUM] || m->tentae[I] || m->tentae[II]
        || m->tentae[III];
}

/* ostium propinquum computare: punctum a centro personae non plus
 * radio suo distans; ex pluribus proximum, ex paribus prius. VERUM
 * si mutatum */
interior b32
ostium_curare (
    MundusLatusMontatio* m)
{
    s32 novum    = -I;
    f32 optimum  = 0.0f;
    i32 numerus  = m->ostia ? xar_numerus(m->ostia) : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; m->positum && k < numerus; k++)
    {
        constans MundusOstium* o =
            (constans MundusOstium*)xar_obtinere(m->ostia, k);
                          f32 radius  =
                              (f32)o->radius_millesimae / 1000.0f;
                          f32 spatium_x  = m->corpus.x
                              - (f32)o->x_millesimae / 1000.0f;
                          f32 spatium_z  = m->corpus.z
                              - (f32)o->z_millesimae / 1000.0f;
                          f32 quadratum  = spatium_x * spatium_x
                              + spatium_z * spatium_z;

        si (   quadratum <= radius * radius
            && (novum < ZEPHYRUM || quadratum < optimum))
        {
            novum    = (s32)k;
            optimum  = quadratum;
        }
    }
    si (novum == m->ostium_propinquum)
    {
        redde FALSUM;
    }
    m->ostium_propinquum = novum;
    redde VERUM;
}

b32
mundus_latus_signum (
    constans MundusLatusMontatio* m,
                           Fines* exitus)
{
    constans MundusOstium* o;
               Dispositio  d;
                      f32  x;
                      f32  z;
                      f32  y         = 0.0f;
                      s32  puncti_x  = ZEPHYRUM;
                      s32  puncti_y  = ZEPHYRUM;
                      s32  origo_x   = ZEPHYRUM;
                      s32  origo_y   = ZEPHYRUM;
                      s32  latitudo;
                      s32  altitudo;
                      s32  capsa_x;
                      s32  capsa_y;

    si (   !m || !exitus || !m->ostia || !m->positum
        || !m->quadrum.pixela || m->ostium_propinquum < ZEPHYRUM
        || (i32)m->ostium_propinquum >= xar_numerus(m->ostia))
    {
        redde FALSUM;
    }
    o  = (constans MundusOstium*)xar_obtinere(m->ostia,
        (i32)m->ostium_propinquum);
    x = (f32)o->x_millesimae / 1000.0f;
    z = (f32)o->z_millesimae / 1000.0f;
    /* ostium in pariete solum gressibile non habet: altitudo
     * personae */
    si (!mundus_solum_altitudo(m->solum, x, z, &y))
    {
        y = m->corpus.y;
    }
    si (!mundus_quadri_origo(&m->mundus, ZEPHYRUM, m->quadrum.latitudo,
        m->quadrum.altitudo, &origo_x, &origo_y))
    {
        redde FALSUM;
    }
    mundus_pixelum_puncti(stereometria_vector(x,
        y + MUNDUS_LATUS_SIGNI_ALTITUDO, z), &puncti_x, &puncti_y);
    disponere(m, &d);
    latitudo  = (s32)(magnitudo(MUNDUS_LATUS_SIGNUM) - I)
        * LITTERA_LATITUDO + II * MUNDUS_LATUS_SIGNI_LIMBUS;
    altitudo  = LITTERA_ALTITUDO + II * MUNDUS_LATUS_SIGNI_LIMBUS;
    /* medio horizontali supra punctum, ima in puncto */
    capsa_x = d.pictura.x + (puncti_x - origo_x) - latitudo / II;
    capsa_y = d.pictura.y + (puncti_y - origo_y) - altitudo;
    /* ad marginem picturae coacta */
    si (capsa_x + latitudo > d.pictura.x + (s32)d.pictura.latitudo)
    {
        capsa_x = d.pictura.x + (s32)d.pictura.latitudo - latitudo;
    }
    si (capsa_y + altitudo > d.pictura.y + (s32)d.pictura.altitudo)
    {
        capsa_y = d.pictura.y + (s32)d.pictura.altitudo - altitudo;
    }
    si (capsa_x < d.pictura.x)
    {
        capsa_x = d.pictura.x;
    }
    si (capsa_y < d.pictura.y)
    {
        capsa_y = d.pictura.y;
    }
    *exitus = fines(capsa_x, capsa_y, latitudo, altitudo);
    redde VERUM;
}

/* quadrum tollere; VERUM si quid erat */
interior b32
quadrum_tollere (
    MundusLatusMontatio* m)
{
    si (!m->quadrum.pixela && m->quadri_clavis[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }
    piscina_vacare(m->piscina_quadri);
    m->quadrum.pixela           = NIHIL;
    m->quadrum.latitudo         = ZEPHYRUM;
    m->quadrum.altitudo         = ZEPHYRUM;
    m->quadri_clavis[ZEPHYRUM]  = '\0';
    redde VERUM;
}

/* quadrum reficere si clavis mutata; VERUM si mutatum */
interior b32
quadrum_curare (
    MundusLatusMontatio* m)
{
        Dispositio  d;
         character  clavis[CLX];
    memoriae_index  pixela;
    memoriae_index  k;
                i8* indices;
                i8* p;
               i32  latitudo;
               i32  altitudo;

    disponere(m, &d);
    latitudo = (i32)d.pictura.latitudo;
    altitudo = (i32)d.pictura.altitudo;
    si (   !m->locus || !m->humus || !m->positum || !m->persona
        || latitudo == ZEPHYRUM || altitudo == ZEPHYRUM)
    {
        redde quadrum_tollere(m);
    }
    sprintf(clavis, "%.64s|%d|%d|%d|%d|%d|%d|%d|%ux%u",
        m->sigillum_personae.mensura >= LXIV
            ? (constans character*)m->sigillum_personae.datum : "",
        (integer)m->humi_modus,
        (integer)(m->corpus.x * 1000.0f),
        (integer)(m->corpus.z * 1000.0f),
        (integer)(m->corpus.y * 1000.0f),
        (integer)(m->corpus.facies * 10.0f), (integer)m->corpus.movetur,
        (integer)(m->corpus.tempus_motus * 1000.0f),
        (insignatus)latitudo, (insignatus)altitudo);
    si (strcmp(clavis, m->quadri_clavis) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    strcpy(m->quadri_clavis, clavis);
    piscina_vacare(m->piscina_quadri);
    pixela   = (memoriae_index)latitudo * altitudo;
    indices  = (i8*)piscina_allocare(m->piscina_quadri, pixela);
    m->quadrum.pixela = (i8*)piscina_allocare(m->piscina_quadri,
        pixela * IV);
    m->corpus.forma = ZEPHYRUM;
    si (   !indices || !m->quadrum.pixela
        || !mundus_quadrum_pingere(m->humus, &m->mundus, &m->forma, I,
               ZEPHYRUM, m->activi, (i8)INDEX_FUNDI, latitudo, altitudo,
               indices, m->piscina_quadri))
    {
        m->quadrum.pixela    = NIHIL;
        m->quadrum.latitudo  = ZEPHYRUM;
        m->quadrum.altitudo  = ZEPHYRUM;
        redde VERUM;
    }
    m->quadrum.latitudo = latitudo;
    m->quadrum.altitudo = altitudo;
    per (k = ZEPHYRUM; k < pixela; k++)
    {
        p = m->quadrum.pixela + k * IV;
        si (indices[k] >= XVI)
        {
            memset(p, ZEPHYRUM, IV);
            perge;
        }
        p[ZEPHYRUM]  = AQUINAS_PALETTE[indices[k]][ZEPHYRUM];
        p[I]         = AQUINAS_PALETTE[indices[k]][I];
        p[II]        = AQUINAS_PALETTE[indices[k]][II];
        p[III]       = 255;
    }
    redde VERUM;
}

b32
mundus_latus_pulsare (
    vacuum* ctx)
{
    MundusLatusMontatio* m = (MundusLatusMontatio*)ctx;
                    s64  hora;
                    s64  spatium;
                    b32  mutatum = FALSUM;

    si (!m)
    {
        redde FALSUM;
    }
    /* horologium SEMEL per pulsum legitur */
    hora          = nunc(m);
    m->hora_visa  = hora;
    si (   m->volumen
        && (   !m->inspectum
            || hora - m->inspectio_ms
                >= MUNDUS_LATUS_MORA_INSPECTIONIS))
    {
        m->inspectum     = VERUM;
        m->inspectio_ms  = hora;
        si (volumen_seq_ultimum(m->volumen) != m->seq_visum)
        {
            indices_relegere(m);
            mutatum = VERUM;
        }
    }
    si (locum_curare(m))
    {
        mutatum = VERUM;
    }
    si (humum_curare(m))
    {
        mutatum = VERUM;
    }
    si (personam_curare(m))
    {
        mutatum = VERUM;
    }
    si (m->positum)
    {
        /* mundus tempore vero: spatium ab ultimo gradu (mundus_gradi
         * id ad 0.1 s cogit - mora longa personam non transfert) */
        spatium         = hora - m->hora_gradus;
        m->hora_gradus  = hora;
        intentum_ponere(m);
        si (spatium > ZEPHYRUM)
        {
            mundus_gradi(&m->mundus,
                spatium > M ? 1.0f : (f32)spatium / 1000.0f);
        }
    }
    si (ostium_curare(m))
    {
        mutatum = VERUM;
    }
    si (quadrum_curare(m))
    {
        mutatum = VERUM;
    }
    redde mutatum;
}

Mora
mundus_latus_mora (
    vacuum* ctx)
{
    MundusLatusMontatio* m = (MundusLatusMontatio*)ctx;

    si (!m)
    {
        redde -I;
    }
    si (m->positum && (clavis_tenetur(m) || m->corpus.movetur))
    {
        redde MUNDUS_LATUS_MORA_MOTUS;
    }
    redde m->volumen ? MUNDUS_LATUS_MORA_INSPECTIONIS : -I;
}

constans Imago*
mundus_latus_imago_fons (
    chorda  provenientia,
    vacuum* ctx)
{
    MundusLatusMontatio* m = (MundusLatusMontatio*)ctx;

    si (   !m || !m->quadrum.pixela
        || !chorda_aequalis(provenientia, m->quadri_titulus))
    {
        redde NIHIL;
    }
    redde &m->quadrum;
}

chorda
mundus_latus_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
    MundusLatusMontatio* m = (MundusLatusMontatio*)ctx;
                    s32  locus;

    si (!m || (locus = locus_personae(m)) < ZEPHYRUM)
    {
        redde chorda_ex_literis("", piscina);
    }
    redde chorda_transcribere(inscriptio(m, (i32)locus)->titulus,
        piscina);
}

vacuum
mundus_latus_focus_amissus (
    vacuum* ctx)
{
    MundusLatusMontatio* m = (MundusLatusMontatio*)ctx;

    si (!m)
    {
        redde;
    }
    memset(m->tentae, ZEPHYRUM, magnitudo(m->tentae));
    m->corpus.intendit = FALSUM;
}


/* ==================================================
 * Praecepta
 * ================================================== */

/* persona 'gradus' locis ab hac (circulo); corpus ubi stat manet */
interior b32
personam_mutare (
    MundusLatusMontatio* m,
                    s32  gradus)
{
    character titulus[STATUAE_TITULUS_MAXIMUS + I];
       chorda t;
          s32 n = (s32)personarum_numerus(m);
          s32 locus;

    si (n < II)
    {
        redde n > ZEPHYRUM;
    }
    locus  = (locus_personae(m) + gradus + n) % n;
    t      = inscriptio(m, (i32)locus)->titulus;
    sprintf(titulus, "%.*s", (integer)(t.mensura
        <= STATUAE_TITULUS_MAXIMUS ? t.mensura
                                   : STATUAE_TITULUS_MAXIMUS),
        (constans character*)t.datum);
    redde campum_scribere(m, "persona", titulus);
}

interior b32
fac_personam_priorem (
    vacuum* ctx)
{
    redde personam_mutare((MundusLatusMontatio*)ctx, -I);
}

interior b32
fac_personam_proximam (
    vacuum* ctx)
{
    redde personam_mutare((MundusLatusMontatio*)ctx, I);
}

/* dithering humi commutare: Atkinson <-> exemplaria */
interior b32
fac_humum (
    vacuum* ctx)
{
    MundusLatusMontatio* m = (MundusLatusMontatio*)ctx;

    redde campum_scribere(m, "humus",
        humi_modus_status(m) == MUNDUS_HUMUS_EXEMPLARIA
            ? "" : HUMUS_EXEMPLARIA);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "mundus.prior", "Previous character", "[", VISUS_MUNDI, ZEPHYRUM,
        fac_personam_priorem },
    { "mundus.proxima", "Next character", "]", VISUS_MUNDI, ZEPHYRUM,
        fac_personam_proximam },
    { "mundus.humus", "Ground dither: Atkinson / exemplaria", "g",
        VISUS_MUNDI, ZEPHYRUM, fac_humum },
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

/* clavis ambulandi eventus (w a s d sine modificante praeter Shift);
 * -1 si alia */
interior s32
clavis_ambulandi (
    constans Eventus* ev)
{
    si (ev->datum.clavis.modificantes
        & (MOD_SUPER | MOD_IMPERIUM | MOD_ALT))
    {
        redde -I;
    }
    commutatio (ev->datum.clavis.runa)
    {
        casus 'w':
            redde MUNDUS_LATUS_SURSUM;
        casus 'a':
            redde MUNDUS_LATUS_SINISTRUM;
        casus 's':
            redde MUNDUS_LATUS_DEORSUM;
        casus 'd':
            redde MUNDUS_LATUS_DEXTRUM;
        ordinarius:
            redde -I;
    }
}

interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
     MundusLatusMontatio* m = (MundusLatusMontatio*)ctx;
     constans Praeceptum* t;
                     s32  clavis;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    si (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
        || ev->genus == EVENTUS_CLAVIS_LIBERATUS)
    {
        clavis = clavis_ambulandi(ev);
        si (clavis >= ZEPHYRUM)
        {
            si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
            {
                /* passus primus post quietem ab hoc momento metitur,
                 * non ab inspectione ultima */
                si (!clavis_tenetur(m) && !m->corpus.movetur)
                {
                    m->hora_gradus = nunc(m);
                }
                m->tentae[clavis] = VERUM;
            }
            alioquin
            {
                m->tentae[clavis] = FALSUM;
            }
            redde VERUM;
        }
    }
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, VISUS_MUNDI, ev);
        redde t && praecepta_status(t, VISUS_MUNDI, requisita_nulla, m)
            == PRAECEPTUM_PRAESTO && t->fn(m);
    }
    si (   ev->genus            != EVENTUS_MUS_DEPRESSUS || !nodus
        || ev->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_prior"))
    {
        redde fac_personam_priorem(m);
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_proxima"))
    {
        redde fac_personam_proximam(m);
    }
    si (chorda_aequalis_literis(nodus->id, "humus"))
    {
        redde fac_humum(m);
    }
    redde FALSUM;
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

/* textus in lineas latitudinis datae fractus (ad spatia; verbum
 * longius linea scinditur) */
interior vacuum
textum_fractum_pingere (
       Mandata* md,
           s32  x,
           s32  y,
           s32  latitudo,
        chorda  textus,
    ColorThema  color)
{
    chorda linea;
       i32 per_lineam = latitudo / LITTERA_LATITUDO > ZEPHYRUM
           ? (i32)(latitudo / LITTERA_LATITUDO) : I;
       i32 initium    = ZEPHYRUM;
       i32 finis;
       i32 k;

    dum (initium < textus.mensura)
    {
        finis = initium + per_lineam;
        si (finis >= textus.mensura)
        {
            finis = textus.mensura;
        }
        alioquin
        {
            per (k = finis; k > initium; k--)
            {
                si (textus.datum[k] == ' ')
                {
                    finis = k;
                    frange;
                }
            }
        }
        linea.datum    = textus.datum + initium;
        linea.mensura  = finis - initium;
        textum_pingere(md, x, y, linea, color);
        y        += LITTERA_ALTITUDO + II;
        initium  = finis;
        dum (initium < textus.mensura && textus.datum[initium] == ' ')
        {
            initium++;
        }
    }
}

/* nuntius duarum partium loco picturae */
interior vacuum
nuntium_pingere (
                 Mandata* md,
     constans Dispositio* d,
      constans character* caput,
                  chorda  corpus)
{
    textum_pingere(md, LIMBUS * IV, d->pictura.y + XVI,
        chorda_ex_literis(caput, md->piscina), COLOR_TEXT);
    textum_fractum_pingere(md, LIMBUS * IV, d->pictura.y + XXXII,
        (s32)d->pictura.latitudo - LIMBUS * VIII, corpus,
        COLOR_TEXT_DIM);
}

interior vacuum
figura_mundi (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    MundusLatusMontatio* m = (MundusLatusMontatio*)ctx;
             Dispositio  d;
                  Fines  f;
                 chorda  titulus;
              character  numerus[XXXII];
                    s32  y_textus = (ORDO_ALTITUDO - LITTERA_ALTITUDO)
                        / II;
                    s32 locus;
                    b32 plures;

    (vacuum)thema;
    f = fines(ZEPHYRUM, ZEPHYRUM, c->fines.latitudo, c->fines.altitudo);
    figura_mensam_pingere(md, f, chorda_aequalis_literis(c->titulus,
        "focatum"));
    si (!m)
    {
        redde;
    }
    disponere(m, &d);
    locus   = locus_personae(m);
    plures  = personarum_numerus(m) > I;
    /* caput: sagittae, numerus, titulus personae */
    figura_sagittam_pingere(md, d.prior, -I,
        plures ? ZEPHYRUM : FIGURA_OBSCURUM);
    numerum_formare(m, numerus);
    textum_pingere(md, d.numerus_x, y_textus,
        chorda_ex_literis(numerus, md->piscina), COLOR_TEXT_DIM);
    figura_sagittam_pingere(md, d.proxima, I,
        plures ? ZEPHYRUM : FIGURA_OBSCURUM);
    si (locus >= ZEPHYRUM)
    {
        titulus = inscriptio(m, (i32)locus)->titulus;
        si ((s32)titulus.mensura * LITTERA_LATITUDO
            > d.titulus_latitudo)
        {
            titulus.mensura = (i32)(d.titulus_latitudo
                / LITTERA_LATITUDO);
        }
        textum_pingere(md, d.titulus_x, y_textus, titulus, COLOR_TEXT);
    }
    /* globulus modi humi */
    figura_globulum_pingere(md, d.humus, chorda_ex_literis(
        humi_modus_status(m) == MUNDUS_HUMUS_EXEMPLARIA
            ? HUMUS_EXEMPLARIA : HUMUS_ATKINSON, md->piscina),
        humi_modus_status(m) == MUNDUS_HUMUS_EXEMPLARIA
            ? FIGURA_ELECTUM : ZEPHYRUM);
    /* fundus sub pictura: color fundi thematis; quadrum extra locum
     * perlucidum est */
    mandata_rectangulum(md, d.pictura,
        figura_color_thematis(COLOR_BACKGROUND), VERUM);
    si (m->quadrum.pixela)
    {
        mandata_imago(md, m->quadri_titulus, fines(d.pictura.x,
            d.pictura.y, (s32)m->quadrum.latitudo,
            (s32)m->quadrum.altitudo));
        /* signum ostii propinqui: capsa plana, limbus, litterae */
        si (mundus_latus_signum(m, &f))
        {
            mandata_rectangulum(md, f,
                figura_color_thematis(COLOR_BACKGROUND), VERUM);
            mandata_rectangulum(md, f,
                figura_color_thematis(COLOR_TEXT), FALSUM);
            textum_pingere(md, f.x + MUNDUS_LATUS_SIGNI_LIMBUS,
                f.y + MUNDUS_LATUS_SIGNI_LIMBUS, chorda_ex_literis(
                    MUNDUS_LATUS_SIGNUM, md->piscina), COLOR_TEXT);
        }
        redde;
    }
    /* nuntii: locus primum, deinde persona */
    si (m->causa_loci.mensura > ZEPHYRUM)
    {
        nuntium_pingere(md, &d, "This place cannot be shown:",
            m->causa_loci);
        redde;
    }
    si (!m->locus)
    {
        nuntium_pingere(md, &d, "No place yet", chorda_ex_literis(
            "statuae addere <file.glb> -volumen <volume> -locus",
            md->piscina));
        redde;
    }
    si (locus < ZEPHYRUM)
    {
        nuntium_pingere(md, &d, "No characters yet", chorda_ex_literis(
            "statuae addere <file.glb> -volumen <volume>",
            md->piscina));
        redde;
    }
    si (m->causa_personae.mensura > ZEPHYRUM)
    {
        nuntium_pingere(md, &d, "This character cannot be shown:",
            m->causa_personae);
    }
}


/* ==================================================
 * Componere et montare
 * ================================================== */

interior vacuum
aream_addere (
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* id,
                  Fines  f)
{
    Componens* c = componens_creare(piscina, intern, id, PARTES_NULLUM);

    si (!c)
    {
        redde;
    }
    componens_ponere_fines(c, f);
    componens_ponere_actio(c, actio_mundi);
    componens_addere_liberum(radix, c);
}

Componens*
mundus_latus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    MundusLatusMontatio* m = (MundusLatusMontatio*)ctx;
              Componens* radix;
             Dispositio  d;
                  Fines  signum;

    (vacuum)repo;
    si (!m)
    {
        redde NIHIL;
    }
    /* pictura statum hunc sequatur (eventus personam modo mutavit) */
    (vacuum)personam_curare(m);
    (vacuum)humum_curare(m);
    (vacuum)quadrum_curare(m);
    radix = componens_creare(piscina, intern, "mundus", PARTES_TABULA);
    si (!radix)
    {
        redde NIHIL;
    }
    disponere(m, &d);
    componens_ponere_fines(radix, fines(ZEPHYRUM, ZEPHYRUM,
        superficies(m, VERUM), superficies(m, FALSUM)));
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_mundi);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    aream_addere(radix, piscina, intern, "pictura", d.pictura);
    aream_addere(radix, piscina, intern, "sagitta_prior", d.prior);
    aream_addere(radix, piscina, intern, "sagitta_proxima", d.proxima);
    aream_addere(radix, piscina, intern, "humus", d.humus);
    si (mundus_latus_signum(m, &signum))
    {
        aream_addere(radix, piscina, intern, "ostium", signum);
    }
    redde radix;
}

vacuum
mundus_latus_dimittere (
    MundusLatusMontatio* m)
{
    si (!m)
    {
        redde;
    }
    si (m->piscina_indicis)
    {
        piscina_destruere(m->piscina_indicis);
    }
    si (m->piscina_loci)
    {
        piscina_destruere(m->piscina_loci);
    }
    si (m->piscina_humi)
    {
        piscina_destruere(m->piscina_humi);
    }
    si (m->piscina_personae)
    {
        piscina_destruere(m->piscina_personae);
    }
    si (m->piscina_quadri)
    {
        piscina_destruere(m->piscina_quadri);
    }
    m->piscina_indicis    = NIHIL;
    m->piscina_loci       = NIHIL;
    m->piscina_humi       = NIHIL;
    m->piscina_personae   = NIHIL;
    m->piscina_quadri     = NIHIL;
    m->personae           = NIHIL;
    m->loci               = NIHIL;
    m->ostia              = NIHIL;
    m->ostium_propinquum  = -I;
    m->locus              = NIHIL;
    m->solum              = NIHIL;
    m->humus              = NIHIL;
    m->persona            = NIHIL;
    m->positum            = FALSUM;
    m->mundus.solum       = NIHIL;
    m->mundus.corporum    = ZEPHYRUM;
    m->quadrum.pixela     = NIHIL;
}

b32
mundus_latus_montare (
              MundusLatusMontatio* m,
                          Piscina* piscina,
              InternamentumChorda* intern,
                          Volumen* volumen,
               InsulaRepositorium* repo,
               constans character* id,
        constans MundusLatusPorta* porta,
                              i32  latitudo,
                              i32  altitudo,
                  StatusSystemata* systemata)
{
      Organum* organum;
    character  titulus[CXXVIII];

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(MundusLatusMontatio));
    m->piscina            = piscina;
    m->intern             = intern;
    m->volumen            = volumen;
    m->latitudo           = latitudo;
    m->altitudo           = altitudo;
    m->ostium_propinquum  = -I;
    si (porta)
    {
        m->porta = *porta;
    }
    dithering_praeparare_omnes(m->activi);
    m->ramus = id ? insula_ramus(repo, "mundus", id)
                  : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "mundus", "mundus");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"mundus\"", id ? id : "mundus", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->piscina_indicis   = piscina_generare_dynamicum("mundus_index",
        0x10000);
    m->piscina_loci      = piscina_generare_dynamicum("mundus_locus",
        0x400000);
    m->piscina_humi      = piscina_generare_dynamicum("mundus_humus",
        0x400000);
    m->piscina_personae  = piscina_generare_dynamicum("mundus_persona",
        0x100000);
    m->piscina_quadri    = piscina_generare_dynamicum("mundus_quadrum",
        0x400000);
    si (   !m->piscina_indicis || !m->piscina_loci || !m->piscina_humi
        || !m->piscina_personae || !m->piscina_quadri)
    {
        redde FALSUM;
    }
    sprintf(titulus, "mundus:%.100s", id ? id : "mundus");
    m->quadri_titulus  = chorda_ex_literis(titulus, piscina);
    m->hora_visa       = nunc(m);
    m->hora_gradus     = m->hora_visa;
    indices_relegere(m);
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    redde m->actiones && m->figurae
        && praecepta_verificare(TABULA_PRAECEPTORUM)
        && actio_registrare(m->actiones, actio_mundi, agere, m)
        && figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_mundi, m);
}
