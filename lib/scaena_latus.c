/* scaena_latus.c - latus scaenae (vide include/scaena_latus.h)
 *
 * Arbor: radix "scaena" (PARTES_TABULA, figura tota) cum liberis qui
 * areae ictus sunt - "pictura", "sagitta_prior", "sagitta_proxima".
 *
 * Et pulsus et compositor statum curant: index (seq voluminis),
 * prospectus (sigillum inscriptionis ostensae), quadrum (mensura
 * partis visae), textus rei tactae (campus 'tactum'). Prospectus,
 * quadrum et textus in piscina_prospectus habitant, quae vacatur cum
 * prospectus alius legitur.
 *
 * HOSPES (latus aliud scaenam hospitat): functiones eaedem, tribus
 * locis divisae - locus_prospectus (ignotus non in primum cadit),
 * rem_tangere (exitus "mundus" m->foras ponit, nihil scribit),
 * eventum_tractare (claves [ ] omittuntur). Prospectus qui ostendi
 * non potest clavem "?<titulus>" in sigillum_lecti ponit, ne causa
 * quoque pulsu denuo fiat.
 */
#include "scaena_latus.h"
#include "membrum__praecepta.h"
#include "scaena_repositorium.h"
#include "graphis.h"
#include "dithering.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include "destinatio.h"
#include "mandatum.h"
#include <stdio.h>
#include <string.h>

#define LATUS_LATITUDO    CDLXXX  /* superficies ordinaria */
#define LATUS_ALTITUDO    DLXXVI
#define ORDO_ALTITUDO     XII     /* ordo capitis */
#define LIMBUS            II
#define SAGITTA_LATITUDO  XII
#define LITTERA_LATITUDO  VI
#define LITTERA_ALTITUDO  VIII
#define LINEA_ALTITUDO        (LITTERA_ALTITUDO + II)
/* margo lateris: textura foci solum hic apparet (ut negotia,
 * chronica); intra eum fundus planus */
#define MARGO_X           LITTERA_LATITUDO
#define MARGO_Y           LITTERA_ALTITUDO
/* textus: duae cellae a fundi lateribus, linea una sub pictura */
#define TEXTUS_LIMBUS     (MARGO_X + II * LITTERA_LATITUDO)
#define VISUS_SCAENAE     I
#define REI_TITULUS_VISUS D       /* litterae tituli rei in statu */

hic_manens constans character actio_scaenae[] = "scaena.agere";

/* dispositio lateris (spatium radicis) */
nomen structura {
    Fines prior;
    Fines proxima;
    Fines pictura;      /* pars picturae visa */
      s32 sectio_x;     /* columna picturae prima visa (latus
                         * angustius utrimque aequaliter secat) */
      s32 numerus_x;    /* textus "1/2" */
      s32 titulus_x;
      s32 textus_y;     /* linea prima partis textus */
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

/* campi scriptura, scriptore lateris; valor vacuus campum tollit */
interior b32
campum_scribere (
    ScaenaLatusMontatio* m,
     constans character* titulus,
     constans character* valor)
{
    ParCampi par;
      chorda prior = m->ramus.repo->scriptor;
         b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        m->scriptor ? m->scriptor : actio_scaenae, m->piscina));
    ok = mutare_ramum(&m->ramus, campum_ponere, &par);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

/* campus textus; chorda vacua si abest */
interior chorda
campus (
    constans ScaenaLatusMontatio* m,
              constans character* titulus)
{
    chorda* a = insula_ramus_attributum(&m->ramus, titulus);
    chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde a ? *a : vacua;
}

interior b32
chordae_pares (
    chorda a,
    chorda b)
{
    redde a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM || chorda_aequalis(a, b));
}


/* ==================================================
 * Index et prospectus
 * ================================================== */

interior i32
prospectuum_numerus (
    constans ScaenaLatusMontatio* m)
{
    redde m->index ? xar_numerus(m->index) : ZEPHYRUM;
}

interior ScaenaInscriptio*
inscriptio (
    constans ScaenaLatusMontatio* m,
                             i32  k)
{
    redde (ScaenaInscriptio*)xar_obtinere(m->index, k);
}

/* locus tituli in indice; -1 si abest */
interior s32
locus_tituli (
    constans ScaenaLatusMontatio* m,
                          chorda  titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < prospectuum_numerus(m); k++)
    {
        si (chordae_pares(inscriptio(m, k)->titulus, titulus))
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* locus prospectus ostensi: titulus status; aliter primus; -1 si
 * index vacuus. Hospes: titulus status aut -1 (numquam primus) */
interior s32
locus_prospectus (
    constans ScaenaLatusMontatio* m)
{
    s32 locus;

    si (prospectuum_numerus(m) == ZEPHYRUM)
    {
        redde -I;
    }
    locus = locus_tituli(m, campus(m, "prospectus"));
    si (m->hospes)
    {
        redde locus;
    }
    redde locus >= ZEPHYRUM ? locus : ZEPHYRUM;
}

interior vacuum
indicem_relegere (
    ScaenaLatusMontatio* m)
{
    piscina_vacare(m->piscina_indicis);
    m->index      = m->volumen
        ? scaena_enumerare(m->volumen, m->piscina_indicis) : NIHIL;
    m->seq_visum  = m->volumen ? volumen_seq_ultimum(m->volumen)
                               : ZEPHYRUM;
}

/* prospectum, quadrum et textum tactum ex memoria tollere */
interior vacuum
prospectum_tollere (
    ScaenaLatusMontatio* m)
{
    piscina_vacare(m->piscina_prospectus);
    memset(&m->prospectus, ZEPHYRUM, magnitudo(m->prospectus));
    m->lectus                  = FALSUM;
    m->sigillum_lecti.mensura  = ZEPHYRUM;
    m->causa.mensura           = ZEPHYRUM;
    m->quadrum.pixela          = NIHIL;
    m->quadrum.latitudo        = ZEPHYRUM;
    m->quadrum.altitudo        = ZEPHYRUM;
    m->tacti_textus.mensura    = ZEPHYRUM;
    m->tacti_clavis.mensura    = ZEPHYRUM;
    m->super                   = ZEPHYRUM;
}

/* hospes: prospectus status in volumine non est (aut campus vacat).
 * Causa semel fit: clavis "?<titulus>" in sigillo lecti. VERUM si
 * quid mutatum */
interior b32
prospectum_ignotum_curare (
    ScaenaLatusMontatio* m)
{
    character alveus[SCAENA_TITULUS_MAXIMUS + LXXX];
       chorda titulus = campus(m, "prospectus");
       chorda clavis;
          i32 mensura = titulus.mensura <= SCAENA_TITULUS_MAXIMUS
              ? titulus.mensura : SCAENA_TITULUS_MAXIMUS;

    sprintf(alveus, "?%.*s", (integer)mensura,
        titulus.datum ? (constans character*)titulus.datum : "");
    clavis = chorda_ex_literis(alveus, m->piscina_prospectus);
    si (chordae_pares(m->sigillum_lecti, clavis))
    {
        redde FALSUM;
    }
    prospectum_tollere(m);
    m->sigillum_lecti = chorda_ex_literis(alveus,
        m->piscina_prospectus);
    si (mensura == ZEPHYRUM)
    {
        m->causa = chorda_ex_literis("no scene is chosen",
            m->piscina_prospectus);
        redde VERUM;
    }
    sprintf(alveus, "there is no scene named '%.*s' in the volume",
        (integer)mensura, (constans character*)titulus.datum);
    m->causa = chorda_ex_literis(alveus, m->piscina_prospectus);
    redde VERUM;
}

/* prospectum legere si alius (aut nullus) nunc ostendendus est.
 * VERUM si quid mutatum */
interior b32
prospectum_curare (
    ScaenaLatusMontatio* m)
{
    ScaenaInscriptio* x;
              chorda  massa;
                 s32  locus     = locus_prospectus(m);
                 b32  inventum  = FALSUM;

    si (locus < ZEPHYRUM && m->hospes)
    {
        redde prospectum_ignotum_curare(m);
    }
    si (locus < ZEPHYRUM)
    {
        si (   !m->lectus && m->sigillum_lecti.mensura == ZEPHYRUM
            && m->causa.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
        prospectum_tollere(m);
        redde VERUM;
    }
    x = inscriptio(m, (i32)locus);
    si (   m->sigillum_lecti.mensura > ZEPHYRUM
        && chorda_aequalis(m->sigillum_lecti, x->sigillum_hex))
    {
        redde FALSUM;
    }
    prospectum_tollere(m);
    m->sigillum_lecti = chorda_transcribere(x->sigillum_hex,
        m->piscina_prospectus);
    massa = volumen_massam_promere(m->volumen, x->sigillum_hex,
        m->piscina_prospectus, &inventum);
    si (!inventum)
    {
        m->causa = chorda_ex_literis("the scene's data is missing from"
            " the volume", m->piscina_prospectus);
        redde VERUM;
    }
    si (!scaena_legere(massa, &m->prospectus, m->piscina_prospectus))
    {
        memset(&m->prospectus, ZEPHYRUM, magnitudo(m->prospectus));
        m->causa = chorda_ex_literis("the scene's data cannot be read"
            " (rebuild it: bin/scaena condere)", m->piscina_prospectus);
        redde VERUM;
    }
    m->lectus = VERUM;
    redde VERUM;
}


/* ==================================================
 * Dispositio et quadrum
 * ================================================== */

interior s32
superficies (
    constans ScaenaLatusMontatio* m,
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
    constans ScaenaLatusMontatio* m,
                       character* alveus)
{
    sprintf(alveus, "%d/%u", (integer)(locus_prospectus(m) + I),
        (insignatus)prospectuum_numerus(m));
}

interior vacuum
disponere (
    constans ScaenaLatusMontatio* m,
                      Dispositio* d)
{
    character numerus[XXXII];
          s32 latitudo  = superficies(m, VERUM);
          s32 altitudo  = superficies(m, FALSUM);
          s32 intra     = latitudo - II * MARGO_X;
          /* latus margine angustius: 'fines' latitudinem negativam
           * in nullam vertit */
          s32 visa_lata = intra < (s32)GRAPHIS_LATITUDO
              ? intra : (s32)GRAPHIS_LATITUDO;
          s32 caput     = MARGO_Y + ORDO_ALTITUDO;
          s32 visa_alta = altitudo - caput < (s32)GRAPHIS_ALTITUDO
              ? altitudo - caput : (s32)GRAPHIS_ALTITUDO;
          s32 x;

    /* pictura in medio lateris, INTRA marginem (limbus foci
     * integer manet); latus angustius partem MEDIAM summam ostendit
     * (utrimque aequaliter sectam) */
    d->pictura    = fines((latitudo - visa_lata) / II, caput, visa_lata,
        visa_alta);
    d->sectio_x = ((s32)GRAPHIS_LATITUDO - visa_lata) / II;
    d->textus_y = caput + (s32)GRAPHIS_ALTITUDO + LINEA_ALTITUDO;
    si (m->hospes)
    {
        /* sine sagittis et numero: titulus ab initio capitis */
        d->prior      = fines(MARGO_X + LIMBUS, MARGO_Y, ZEPHYRUM,
            ZEPHYRUM);
        d->proxima    = d->prior;
        d->numerus_x  = MARGO_X + LIMBUS;
        d->titulus_x  = MARGO_X + LIMBUS;
        redde;
    }
    numerum_formare(m, numerus);
    d->prior      = fines(MARGO_X + LIMBUS, MARGO_Y, SAGITTA_LATITUDO,
        ORDO_ALTITUDO);
    d->numerus_x  = MARGO_X + LIMBUS + SAGITTA_LATITUDO + LIMBUS;
    x             = d->numerus_x
        + (s32)strlen(numerus) * LITTERA_LATITUDO + LIMBUS;
    d->proxima    = fines(x, MARGO_Y, SAGITTA_LATITUDO, ORDO_ALTITUDO);
    d->titulus_x  = x + SAGITTA_LATITUDO + LIMBUS * II;
}

/* quadrum partis visae curare: indices prospectus per palettam.
 * VERUM si quid mutatum */
interior b32
quadrum_curare (
    ScaenaLatusMontatio* m)
{
    Dispositio  d;
            i8* pixela;
           i32  latitudo;
           i32  altitudo;
           i32  x;
           i32  y;

    disponere(m, &d);
    latitudo = (i32)d.pictura.latitudo;
    altitudo = (i32)d.pictura.altitudo;
    si (!m->lectus || latitudo == ZEPHYRUM || altitudo == ZEPHYRUM)
    {
        si (!m->quadrum.pixela)
        {
            redde FALSUM;
        }
        m->quadrum.pixela    = NIHIL;
        m->quadrum.latitudo  = ZEPHYRUM;
        m->quadrum.altitudo  = ZEPHYRUM;
        redde VERUM;
    }
    si (   m->quadrum.pixela && m->quadrum.latitudo == latitudo
        && m->quadrum.altitudo == altitudo)
    {
        redde FALSUM;
    }
    pixela = (i8*)piscina_allocare(m->piscina_prospectus,
        (memoriae_index)latitudo * altitudo * IV);
    m->quadrum.pixela    = pixela;
    m->quadrum.latitudo  = pixela ? latitudo : ZEPHYRUM;
    m->quadrum.altitudo  = pixela ? altitudo : ZEPHYRUM;
    si (!pixela)
    {
        redde VERUM;
    }
    per (y = ZEPHYRUM; y < altitudo; y++)
    {
        per (x = ZEPHYRUM; x < latitudo; x++)
        {
            /* lector indices supra XV recusat */
            i8 k = m->prospectus.indices[y * GRAPHIS_LATITUDO + x
                + (i32)d.sectio_x];
            i8* p = pixela + (y * latitudo + x) * IV;

            p[ZEPHYRUM]  = AQUINAS_PALETTE[k & 0x0F][ZEPHYRUM];
            p[I]         = AQUINAS_PALETTE[k & 0x0F][I];
            p[II]        = AQUINAS_PALETTE[k & 0x0F][II];
            p[III]       = 255;
        }
    }
    redde VERUM;
}

/* textum rei tactae curare: campus 'tactum' = "<prospectus>/<res>";
 * res in prospectu SUO quaeritur (post exitum non is qui ostenditur).
 * VERUM si quid mutatum */
interior b32
tactum_curare (
    ScaenaLatusMontatio* m)
{
             ScaenaProspectus  alius;
    constans ScaenaProspectus* p       = NIHIL;
                       chorda  tactum  = campus(m, "tactum");
                       chorda  titulus;
                       chorda  res;
                       chorda  massa;
                          i32  k;

    si (chordae_pares(tactum, m->tacti_clavis))
    {
        redde FALSUM;
    }
    m->tacti_textus.mensura = ZEPHYRUM;
    m->tacti_clavis.mensura = ZEPHYRUM;
    si (tactum.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    m->tacti_clavis = chorda_transcribere(tactum,
        m->piscina_prospectus);
    per (k = ZEPHYRUM; k < tactum.mensura && tactum.datum[k] != '/';
        k++)
    {
    }
    si (k >= tactum.mensura)
    {
        redde VERUM;
    }
    titulus.datum    = tactum.datum;
    titulus.mensura  = k;
    res.datum        = tactum.datum + k + I;
    res.mensura      = tactum.mensura - k - I;
    si (m->lectus && chordae_pares(m->prospectus.titulus, titulus))
    {
        p = &m->prospectus;
    }
    alioquin si (m->volumen && locus_tituli(m, titulus) >= ZEPHYRUM)
    {
        massa = scaena_promere(m->volumen, titulus,
            m->piscina_prospectus);
        si (scaena_legere(massa, &alius, m->piscina_prospectus))
        {
            p = &alius;
        }
    }
    per (k = ZEPHYRUM; p && k < p->rerum; k++)
    {
        si (chordae_pares(p->res[k].titulus, res))
        {
            m->tacti_textus = chorda_transcribere(p->res[k].textus,
                m->piscina_prospectus);
        }
    }
    redde VERUM;
}

/* omnia quae a statu et volumine pendent; VERUM si quid mutatum */
interior b32
curare (
    ScaenaLatusMontatio* m)
{
    b32 mutatum = prospectum_curare(m);

    si (quadrum_curare(m))
    {
        mutatum = VERUM;
    }
    si (tactum_curare(m))
    {
        mutatum = VERUM;
    }
    redde mutatum;
}

b32
scaena_latus_pulsare (
    vacuum* ctx)
{
    ScaenaLatusMontatio* m        = (ScaenaLatusMontatio*)ctx;
                    b32  mutatum  = FALSUM;

    si (!m)
    {
        redde FALSUM;
    }
    si (   m->volumen
        && volumen_seq_ultimum(m->volumen) != m->seq_visum)
    {
        indicem_relegere(m);
        mutatum = VERUM;
    }
    si (curare(m))
    {
        mutatum = VERUM;
    }
    redde mutatum;
}

Mora
scaena_latus_mora (
    vacuum* ctx)
{
    ScaenaLatusMontatio* m = (ScaenaLatusMontatio*)ctx;

    redde m && m->volumen ? SCAENA_LATUS_MORA_INSPECTIONIS : -I;
}

constans Imago*
scaena_latus_imago_fons (
    chorda  provenientia,
    vacuum* ctx)
{
    ScaenaLatusMontatio* m = (ScaenaLatusMontatio*)ctx;

    si (   !m || !m->quadrum.pixela
        || !chorda_aequalis(provenientia, m->quadri_titulus))
    {
        redde NIHIL;
    }
    redde &m->quadrum;
}

chorda
scaena_latus_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
    ScaenaLatusMontatio* m = (ScaenaLatusMontatio*)ctx;
                    s32  locus;

    si (!m || (locus = locus_prospectus(m)) < ZEPHYRUM)
    {
        redde chorda_ex_literis("", piscina);
    }
    redde chorda_transcribere(inscriptio(m, (i32)locus)->titulus,
        piscina);
}


/* ==================================================
 * Praecepta
 * ================================================== */

/* res 'n' prospectus ostensi (I .. rerum) tangere: textus eius, et
 * exitus si in prospectum voluminis ducit */
interior b32
rem_tangere (
    ScaenaLatusMontatio* m,
                    i32  n)
{
    constans ScaenaRes* r;
             character  tactum[SCAENA_TITULUS_MAXIMUS
                 + REI_TITULUS_VISUS + IV];
             character titulus[SCAENA_TITULUS_MAXIMUS + I];
                   b32 ok;

    si (!m->lectus || n < I || n > m->prospectus.rerum)
    {
        redde FALSUM;
    }
    r = &m->prospectus.res[n - I];
    si (   m->hospes
        && chorda_aequalis_literis(r->exitus, SCAENA_EXITUS_MUNDUS))
    {
        /* hospes scaenam claudit: nihil scribitur */
        m->foras = VERUM;
        redde VERUM;
    }
    sprintf(tactum, "%.*s/%.*s",
        (integer)(m->prospectus.titulus.mensura
            <= SCAENA_TITULUS_MAXIMUS ? m->prospectus.titulus.mensura
                                      : SCAENA_TITULUS_MAXIMUS),
        (constans character*)m->prospectus.titulus.datum,
        (integer)(r->titulus.mensura <= REI_TITULUS_VISUS
            ? r->titulus.mensura : REI_TITULUS_VISUS),
        (constans character*)r->titulus.datum);
    /* exitus ante scripturam legitur: r in piscina prospectus est */
    titulus[ZEPHYRUM] = '\0';
    si (   r->exitus.mensura > ZEPHYRUM
        && r->exitus.mensura          <= SCAENA_TITULUS_MAXIMUS
        && !chorda_aequalis_literis(r->exitus, SCAENA_EXITUS_MUNDUS)
        && locus_tituli(m, r->exitus) >= ZEPHYRUM)
    {
        sprintf(titulus, "%.*s", (integer)r->exitus.mensura,
            (constans character*)r->exitus.datum);
    }
    ok = campum_scribere(m, "tactum", tactum);
    si (ok && titulus[ZEPHYRUM])
    {
        ok        = campum_scribere(m, "prospectus", titulus);
        m->super  = ZEPHYRUM;
    }
    redde ok;
}

/* prospectus 'gradus' locis ab hoc (circulo); res tacta vacatur */
interior b32
prospectum_mutare (
    ScaenaLatusMontatio* m,
                    s32  gradus)
{
    character titulus[SCAENA_TITULUS_MAXIMUS + I];
       chorda t;
          s32 n = (s32)prospectuum_numerus(m);
          s32 locus;

    si (n < II)
    {
        redde n > ZEPHYRUM;
    }
    locus  = (locus_prospectus(m) + gradus + n) % n;
    t      = inscriptio(m, (i32)locus)->titulus;
    sprintf(titulus, "%.*s", (integer)(t.mensura
        <= SCAENA_TITULUS_MAXIMUS ? t.mensura
                                  : SCAENA_TITULUS_MAXIMUS),
        (constans character*)t.datum);
    m->super = ZEPHYRUM;
    redde campum_scribere(m, "prospectus", titulus)
        && campum_scribere(m, "tactum", "");
}

interior b32
fac_prospectum_priorem (
    vacuum* ctx)
{
    redde prospectum_mutare((ScaenaLatusMontatio*)ctx, -I);
}

interior b32
fac_prospectum_proximum (
    vacuum* ctx)
{
    redde prospectum_mutare((ScaenaLatusMontatio*)ctx, I);
}

/* rem 'gradus' locis ab electa eligere (circulo; ab nulla: prima aut
 * ultima) */
interior b32
rem_eligere (
    ScaenaLatusMontatio* m,
                    s32  gradus)
{
    s32 n = m->lectus ? (s32)m->prospectus.rerum : ZEPHYRUM;

    si (n < I)
    {
        redde FALSUM;
    }
    si (m->super < I || (s32)m->super > n)
    {
        m->super = gradus > ZEPHYRUM ? I : (i32)n;
        redde VERUM;
    }
    m->super = (i32)(((s32)m->super - I + gradus + n) % n) + I;
    redde VERUM;
}

interior b32
fac_rem_proximam (
    vacuum* ctx)
{
    redde rem_eligere((ScaenaLatusMontatio*)ctx, I);
}

interior b32
fac_rem_priorem (
    vacuum* ctx)
{
    redde rem_eligere((ScaenaLatusMontatio*)ctx, -I);
}

interior b32
fac_tangere (
    vacuum* ctx)
{
    ScaenaLatusMontatio* m = (ScaenaLatusMontatio*)ctx;

    redde rem_tangere(m, m->super);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "scaena.prior", "Previous view", "[", VISUS_SCAENAE, ZEPHYRUM,
        fac_prospectum_priorem },
    { "scaena.proximus", "Next view", "]", VISUS_SCAENAE, ZEPHYRUM,
        fac_prospectum_proximum },
    { "scaena.res_proxima", "Next thing", "j", VISUS_SCAENAE, ZEPHYRUM,
        fac_rem_proximam },
    { "scaena.res_prior", "Previous thing", "k", VISUS_SCAENAE,
        ZEPHYRUM, fac_rem_priorem },
    { "scaena.tangere", "Look at the chosen thing", "Enter",
        VISUS_SCAENAE, ZEPHYRUM, fac_tangere },
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

/* res in pixelo partis visae picturae; 0 si nulla aut extra */
interior i32
res_in_puncto (
    constans ScaenaLatusMontatio* m,
                             s32  x,
                             s32  y)
{
    Dispositio d;
            i8 dominus;

    disponere(m, &d);
    si (   !m->lectus || !m->quadrum.pixela || x < ZEPHYRUM
        || y < ZEPHYRUM || x >= (s32)m->quadrum.latitudo
        || y >= (s32)m->quadrum.altitudo)
    {
        redde ZEPHYRUM;
    }
    dominus = m->prospectus.domini[(i32)y * GRAPHIS_LATITUDO + (i32)x
        + (i32)d.sectio_x];
    redde dominus <= m->prospectus.rerum ? (i32)dominus : ZEPHYRUM;
}

/* eventus lateris (veri aut hospitati); VERUM si quid mutatum */
interior b32
eventum_tractare (
    ScaenaLatusMontatio* m,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev)
{
    constans Praeceptum* t;
                    b32  in_pictura;
                    i32  n;

    in_pictura = nodus && des
        && chorda_aequalis_literis(nodus->id, "pictura");
    /* res sub indice: motus in pictura eam ponit; motus alibi et
     * exitus ex pictura eam tollunt */
    si (ev->genus == EVENTUS_MUS_INTRAVIT)
    {
        redde FALSUM;
    }
    si (   ev->genus == EVENTUS_MUS_MOTUS
        || ev->genus == EVENTUS_MUS_EXIIT)
    {
        n = in_pictura && ev->genus == EVENTUS_MUS_MOTUS
            ? res_in_puncto(m, des->punctum_locale.x,
                  des->punctum_locale.y)
            : ZEPHYRUM;
        si (n == m->super)
        {
            redde FALSUM;
        }
        m->super = n;
        redde VERUM;
    }
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, VISUS_SCAENAE,
            ev);
        /* hospes: prospectus per exitus solos mutatur */
        si (   t && m->hospes
            && (   t->fn == fac_prospectum_priorem
                || t->fn == fac_prospectum_proximum))
        {
            redde FALSUM;
        }
        redde t && praecepta_status(t, VISUS_SCAENAE, requisita_nulla,
            m) == PRAECEPTUM_PRAESTO && t->fn(m);
    }
    si (   ev->genus            != EVENTUS_MUS_DEPRESSUS || !nodus
        || ev->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_prior"))
    {
        redde fac_prospectum_priorem(m);
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_proxima"))
    {
        redde fac_prospectum_proximum(m);
    }
    si (in_pictura)
    {
        n = res_in_puncto(m, des->punctum_locale.x,
            des->punctum_locale.y);
        si (n == ZEPHYRUM)
        {
            redde FALSUM;
        }
        m->super = n;
        redde rem_tangere(m, n);
    }
    redde FALSUM;
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
    (vacuum)repo;
    (vacuum)motus;
    redde eventum_tractare((ScaenaLatusMontatio*)ctx, des, nodus, ev);
}

ScaenaLatusEventum
scaena_latus_agere_hospitis (
    ScaenaLatusMontatio* m,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev)
{
    b32 mutatum;

    si (!m || !ev)
    {
        redde SCAENA_LATUS_NIHIL;
    }
    m->foras  = FALSUM;
    mutatum   = eventum_tractare(m, des, nodus, ev);
    si (m->foras)
    {
        m->foras = FALSUM;
        redde SCAENA_LATUS_FORAS;
    }
    redde mutatum ? SCAENA_LATUS_MUTATUM : SCAENA_LATUS_NIHIL;
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
 * longius linea scinditur); linea quae ultra 'limes' caderet et
 * sequentes non pinguntur. Redde: y lineae post ultimam */
interior s32
textum_fractum_pingere (
       Mandata* md,
           s32  x,
           s32  y,
           s32  latitudo,
           s32  limes,
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
        si (y + LITTERA_ALTITUDO > limes)
        {
            redde y;
        }
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
        y        += LINEA_ALTITUDO;
        initium  = finis;
        dum (initium < textus.mensura && textus.datum[initium] == ' ')
        {
            initium++;
        }
    }
    redde y;
}

/* nuntius duarum partium loco picturae. Redde: y lineae post
 * ultimam */
interior s32
nuntium_pingere (
               Mandata* md,
                   s32  latitudo,
                   s32  altitudo,
    constans character* caput,
                chorda  corpus)
{
    textum_pingere(md, TEXTUS_LIMBUS, MARGO_Y + ORDO_ALTITUDO + XVI,
        chorda_ex_literis(caput, md->piscina), COLOR_TEXT);
    redde textum_fractum_pingere(md, TEXTUS_LIMBUS,
        MARGO_Y + ORDO_ALTITUDO + XXXII, latitudo - TEXTUS_LIMBUS * II,
        altitudo - MARGO_Y, corpus, COLOR_TEXT_DIM);
}

/* latus totum (verum aut hospitatum) */
interior vacuum
latus_pingere (
    ScaenaLatusMontatio* m,
     constans Componens* c,
                Mandata* md)
{
             Dispositio d;
                  Fines f;
                 chorda titulus;
                 chorda res;
              character numerus[XXXII];
                    s32 latitudo;
                    s32 altitudo;
                    s32 y_textus = MARGO_Y
                        + (ORDO_ALTITUDO - LITTERA_ALTITUDO) / II;
                    s32 locus;
                    s32 x;
                    s32 y;
                    s32 spatium;
                    b32 plures;

    f = fines(ZEPHYRUM, ZEPHYRUM, c->fines.latitudo, c->fines.altitudo);
    figura_mensam_pingere(md, f, chorda_aequalis_literis(c->titulus,
        "focatum"));
    /* fundus intra marginem: textura foci solum in margine */
    mandata_rectangulum(md, fines(MARGO_X, MARGO_Y,
        (s32)c->fines.latitudo - II * MARGO_X,
        (s32)c->fines.altitudo - II * MARGO_Y),
        figura_color_thematis(COLOR_BACKGROUND), VERUM);
    si (!m)
    {
        redde;
    }
    disponere(m, &d);
    latitudo  = superficies(m, VERUM);
    altitudo  = superficies(m, FALSUM);
    locus     = locus_prospectus(m);
    plures    = prospectuum_numerus(m) > I;
    /* caput: sagittae, numerus (hospes: neutrum), titulus
     * prospectus, res sub indice */
    si (!m->hospes)
    {
        figura_sagittam_pingere(md, d.prior, -I,
            plures ? ZEPHYRUM : FIGURA_OBSCURUM);
        numerum_formare(m, numerus);
        textum_pingere(md, d.numerus_x, y_textus,
            chorda_ex_literis(numerus, md->piscina), COLOR_TEXT_DIM);
        figura_sagittam_pingere(md, d.proxima, I,
            plures ? ZEPHYRUM : FIGURA_OBSCURUM);
    }
    x = d.titulus_x;
    si (locus >= ZEPHYRUM)
    {
        titulus = inscriptio(m, (i32)locus)->titulus;
        spatium = latitudo - MARGO_X - LIMBUS - d.titulus_x;
        si (spatium < ZEPHYRUM)
        {
            spatium = ZEPHYRUM;
        }
        si ((s32)titulus.mensura * LITTERA_LATITUDO > spatium)
        {
            titulus.mensura = (i32)(spatium / LITTERA_LATITUDO);
        }
        textum_pingere(md, d.titulus_x, y_textus, titulus, COLOR_TEXT);
        x += (s32)titulus.mensura * LITTERA_LATITUDO;
    }
    si (   m->lectus && m->super >= I
        && m->super <= m->prospectus.rerum)
    {
        /* ad dextram; solum si post titulum capitur */
        res      = m->prospectus.res[m->super - I].titulus;
        spatium  = latitudo - MARGO_X - LIMBUS * II
            - (s32)res.mensura * LITTERA_LATITUDO;
        si (spatium >= x + LITTERA_LATITUDO)
        {
            textum_pingere(md, spatium, y_textus, res, COLOR_TEXT_DIM);
        }
    }
    /* fundus sub pictura */
    mandata_rectangulum(md, d.pictura,
        figura_color_thematis(COLOR_BACKGROUND), VERUM);
    si (m->causa.mensura > ZEPHYRUM)
    {
        y = nuntium_pingere(md, latitudo, altitudo,
            "This scene cannot be shown:", m->causa);
        si (   m->monitum && y + LINEA_ALTITUDO + LITTERA_ALTITUDO
            <= altitudo - MARGO_Y)
        {
            textum_pingere(md, TEXTUS_LIMBUS, y + LINEA_ALTITUDO,
                chorda_ex_literis(m->monitum, md->piscina),
                COLOR_TEXT);
        }
        redde;
    }
    si (locus < ZEPHYRUM)
    {
        (vacuum)nuntium_pingere(md, latitudo, altitudo,
            "No scenes yet", chorda_ex_literis(
                "bin/scaena condere -volumen <volume>", md->piscina));
        redde;
    }
    si (m->quadrum.pixela)
    {
        mandata_imago(md, m->quadri_titulus, fines(d.pictura.x,
            d.pictura.y, (s32)m->quadrum.latitudo,
            (s32)m->quadrum.altitudo));
    }
    si (!m->lectus)
    {
        redde;
    }
    /* textus: descriptio; linea vacua; res tacta (color idem: textus
     * obscurior in fundo lateris aegre legitur) */
    y = textum_fractum_pingere(md, TEXTUS_LIMBUS, d.textus_y,
        latitudo - TEXTUS_LIMBUS * II, altitudo - MARGO_Y,
        m->prospectus.descriptio, COLOR_TEXT);
    si (m->tacti_textus.mensura > ZEPHYRUM)
    {
        (vacuum)textum_fractum_pingere(md, TEXTUS_LIMBUS,
            y + LINEA_ALTITUDO, latitudo - TEXTUS_LIMBUS * II,
            altitudo - MARGO_Y, m->tacti_textus, COLOR_TEXT);
    }
}

interior vacuum
figura_scaenae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    (vacuum)thema;
    latus_pingere((ScaenaLatusMontatio*)ctx, c, md);
}

vacuum
scaena_latus_pingere_hospitis (
    ScaenaLatusMontatio* m,
     constans Componens* c,
                Mandata* md)
{
    si (c && md)
    {
        latus_pingere(m, c, md);
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
     constans character* actio,
                  Fines  f)
{
    Componens* c = componens_creare(piscina, intern, id, PARTES_NULLUM);

    si (!c)
    {
        redde;
    }
    componens_ponere_fines(c, f);
    componens_ponere_actio(c, actio);
    componens_addere_liberum(radix, c);
}

Componens*
scaena_latus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    ScaenaLatusMontatio* m = (ScaenaLatusMontatio*)ctx;
              Componens* radix;
             Dispositio  d;

    (vacuum)repo;
    si (!m)
    {
        redde NIHIL;
    }
    /* pictura et textus statum hunc sequantur (eventus prospectum aut
     * rem tactam modo mutavit) */
    (vacuum)curare(m);
    radix = componens_creare(piscina, intern, "scaena", PARTES_TABULA);
    si (!radix)
    {
        redde NIHIL;
    }
    disponere(m, &d);
    componens_ponere_fines(radix, fines(ZEPHYRUM, ZEPHYRUM,
        superficies(m, VERUM), superficies(m, FALSUM)));
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_scaenae);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    aream_addere(radix, piscina, intern, "pictura", actio_scaenae,
        d.pictura);
    aream_addere(radix, piscina, intern, "sagitta_prior",
        actio_scaenae, d.prior);
    aream_addere(radix, piscina, intern, "sagitta_proxima",
        actio_scaenae, d.proxima);
    redde radix;
}

vacuum
scaena_latus_areas_addere (
    ScaenaLatusMontatio* m,
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* actio)
{
    Dispositio d;

    si (!m || !radix || !piscina || !intern || !actio)
    {
        redde;
    }
    disponere(m, &d);
    aream_addere(radix, piscina, intern, "pictura", actio, d.pictura);
}

b32
scaena_latus_valet (
    constans ScaenaLatusMontatio* m)
{
    redde m && m->lectus;
}

vacuum
scaena_latus_dimittere (
    ScaenaLatusMontatio* m)
{
    si (!m)
    {
        redde;
    }
    si (m->piscina_indicis)
    {
        piscina_destruere(m->piscina_indicis);
    }
    si (m->piscina_prospectus)
    {
        piscina_destruere(m->piscina_prospectus);
    }
    m->piscina_indicis     = NIHIL;
    m->piscina_prospectus  = NIHIL;
    m->index               = NIHIL;
    memset(&m->prospectus, ZEPHYRUM, magnitudo(m->prospectus));
    m->lectus                  = FALSUM;
    m->sigillum_lecti.mensura  = ZEPHYRUM;
    m->causa.mensura           = ZEPHYRUM;
    m->quadrum.pixela          = NIHIL;
    m->tacti_textus.mensura    = ZEPHYRUM;
    m->tacti_clavis.mensura    = ZEPHYRUM;
    m->super                   = ZEPHYRUM;
}

b32
scaena_latus_montare (
    ScaenaLatusMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
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
    memset(m, ZEPHYRUM, magnitudo(ScaenaLatusMontatio));
    m->piscina   = piscina;
    m->intern    = intern;
    m->volumen   = volumen;
    m->latitudo  = latitudo;
    m->altitudo  = altitudo;
    m->scriptor  = actio_scaenae;
    m->ramus = id ? insula_ramus(repo, "scaena", id)
                  : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "scaena", "scaena");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"scaena\"", id ? id : "scaena", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->piscina_indicis     = piscina_generare_dynamicum("scaena_index",
        0x10000);
    m->piscina_prospectus  = piscina_generare_dynamicum(
        "scaena_prospectus", 0x100000);
    si (!m->piscina_indicis || !m->piscina_prospectus)
    {
        redde FALSUM;
    }
    sprintf(titulus, "scaena:%.100s", id ? id : "scaena");
    m->quadri_titulus = chorda_ex_literis(titulus, piscina);
    indicem_relegere(m);
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    redde m->actiones && m->figurae
        && praecepta_verificare(TABULA_PRAECEPTORUM)
        && actio_registrare(m->actiones, actio_scaenae, agere, m)
        && figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_scaenae, m);
}

b32
scaena_latus_montare_hospitem (
    ScaenaLatusMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
            InsulaRamus  ramus,
     constans character* scriptor,
     constans character* quadri_titulus,
     constans character* monitum,
                    i32  latitudo,
                    i32  altitudo)
{
    si (!m || !piscina || !intern || !scriptor || !quadri_titulus)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(ScaenaLatusMontatio));
    m->piscina   = piscina;
    m->intern    = intern;
    m->volumen   = volumen;
    m->latitudo  = latitudo;
    m->altitudo  = altitudo;
    m->ramus     = ramus;
    m->scriptor  = scriptor;
    m->hospes    = VERUM;
    m->monitum   = monitum;
    m->piscina_indicis     = piscina_generare_dynamicum(
        "scaena_hospitis_index", 0x10000);
    m->piscina_prospectus  = piscina_generare_dynamicum(
        "scaena_hospitis_prospectus", 0x100000);
    si (!m->piscina_indicis || !m->piscina_prospectus)
    {
        scaena_latus_dimittere(m);
        redde FALSUM;
    }
    m->quadri_titulus = chorda_ex_literis(quadri_titulus, piscina);
    indicem_relegere(m);
    redde praecepta_verificare(TABULA_PRAECEPTORUM);
}
