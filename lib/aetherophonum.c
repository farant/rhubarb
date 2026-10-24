/* aetherophonum.c - theremin in latere vici (vide
 * include/aetherophonum.h)
 *
 * Arbor: radix "aetherophonum" (PARTES_TABULA, figura tota) et liberum
 * "campus" (area sonans, actio eadem) supra lineam status. Puncta muris
 * per destinatio_ad_locale in spatium campi: captura (botton pressus)
 * puncta extra campum dat - functiones notae et lucri ea coercent.
 */
#include "aetherophonum.h"
#include "membrum__praecepta.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define AETHEROPHONUM_LATITUDO   CCCLX     /* superficies ordinaria */
#define AETHEROPHONUM_ALTITUDO   CCXL
#define AETHEROPHONUM_LINEA      XII       /* linea status, pixela */
#define AETHEROPHONUM_NOTA_INFIMA   XLVIII    /* C3 */
#define AETHEROPHONUM_OCTAVAE    III
#define AETHEROPHONUM_OCTAVAE_MAXIMAE VI
#define AETHEROPHONUM_NOTA_SUMMA CXXVII
#define VISUS_AETHEROPHONI       I

hic_manens constans character actio_aetherophoni[] =
    "aetherophonum.agere";

/* praeparata tibiae per 'f', ordine circuli */
hic_manens constans character* tibiae[] = {
    "aether", "quadrans", "triangulum", "organum"
};

hic_manens constans character* nomina_notarum[XII] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};


/* ==================================================
 * Functiones purae
 * ================================================== */

f32
aetherophonum_nota (
    s32 x,
    s32 latitudo,
    s32 nota_infima,
    s32 octavae,
    b32 gradus)
{
    f64 ratio;
    f64 nota;

    si (latitudo <= ZEPHYRUM)
    {
        redde (f32)nota_infima;
    }
    ratio  = (f64)x / (f64)latitudo;
    ratio  = ratio < 0.0 ? 0.0 : ratio > 1.0 ? 1.0 : ratio;
    nota   = (f64)nota_infima + (f64)(octavae * XII) * ratio;
    si (gradus)
    {
        nota = floor(nota + 0.5);
    }
    redde (f32)nota;
}

f32
aetherophonum_lucrum (
    s32 y,
    s32 altitudo)
{
    f64 ratio;

    si (altitudo <= I)
    {
        redde 1.0f;
    }
    ratio = (f64)y / (f64)(altitudo - I);
    ratio = ratio < 0.0 ? 0.0 : ratio > 1.0 ? 1.0 : ratio;
    redde (f32)(1.0 - ratio);
}


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

interior b32
attributum_veritas (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     b32  praestitutum)
{
    chorda* a = insula_ramus_attributum(ramus, titulus);

    si (!a)
    {
        redde praestitutum;
    }
    redde chorda_aequalis_literis(*a, "verum");
}

/* tibia nota (ignota aut absens = aether) */
interior constans character*
tibia_currens (
    constans AetherophoniMontatio* m)
{
             chorda* a = insula_ramus_attributum(&m->ramus, "tibia");
     memoriae_index  k;

    per (k = ZEPHYRUM; a
        && k < magnitudo(tibiae) / magnitudo(tibiae[ZEPHYRUM]);
         k++)
    {
        si (chorda_aequalis_literis(*a, tibiae[k]))
        {
            redde tibiae[k];
        }
    }
    redde tibiae[ZEPHYRUM];
}

interior s32
nota_infima (
    constans AetherophoniMontatio* m)
{
    redde attributum_s32(&m->ramus, "nota_infima",
        AETHEROPHONUM_NOTA_INFIMA);
}

interior s32
octavae (
    constans AetherophoniMontatio* m)
{
    redde attributum_s32(&m->ramus, "octavae", AETHEROPHONUM_OCTAVAE);
}

interior b32
gradus (
    constans AetherophoniMontatio* m)
{
    redde attributum_veritas(&m->ramus, "gradus", FALSUM);
}

interior b32
vibratus (
    constans AetherophoniMontatio* m)
{
    redde attributum_veritas(&m->ramus, "vibratus", VERUM);
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

/* campum unum scriptore lateris (vocatur etiam extra actionem) */
interior b32
campum_scribere (
       AetherophoniMontatio* m,
         constans character* titulus,
         constans character* valor)
{
    ParCampi par;
      chorda prior;
         b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    prior        = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_aetherophoni, m->piscina));
    ok = mutare_ramum(&m->ramus, campum_ponere, &par);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior b32
numerum_scribere (
    AetherophoniMontatio* m,
      constans character* titulus,
                     s32  valor)
{
    character textus[XVI];

    sprintf(textus, "%d", (integer)valor);
    redde campum_scribere(m, titulus, textus);
}


/* ==================================================
 * Dispositio
 * ================================================== */

interior s32
superficies (
    constans AetherophoniMontatio* m,
                              b32  latitudo)
{
    s32 v = latitudo ? (s32)m->latitudo : (s32)m->altitudo;

    si (v <= ZEPHYRUM)
    {
        v = latitudo ? AETHEROPHONUM_LATITUDO : AETHEROPHONUM_ALTITUDO;
    }
    redde attributum_s32(&m->ramus, latitudo ? "superficies_latitudo"
        : "superficies_altitudo", v);
}

/* campus in spatio radicis: totum supra lineam status */
interior Fines
campi_fines (
    s32 latitudo,
    s32 altitudo)
{
    Fines f;

    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo > AETHEROPHONUM_LINEA
                ? altitudo - AETHEROPHONUM_LINEA : altitudo;
    redde f;
}

/* "C4", "F#3" (nota rotundata) */
interior vacuum
nomen_notae (
          f32  nota,
    character* alveus)
{
    s32 octava;
    s32 n = (s32)floor((f64)nota + 0.5);

    si (n < ZEPHYRUM)
    {
        n = ZEPHYRUM;
    }
    octava = n / XII - I;
    sprintf(alveus, "%s%d", nomina_notarum[n % XII], (integer)octava);
}


/* ==================================================
 * Vox (kinetica)
 * ================================================== */

interior vacuum
vocem_dimittere (
    AetherophoniMontatio* m)
{
    si (m->vox != ZEPHYRUM && m->porta.dimittere)
    {
        m->porta.dimittere(m->porta.ctx, m->vox);
    }
    m->vox      = ZEPHYRUM;
    m->tenetur  = FALSUM;
}

/* nota et lucrum puncti campi (spatium campi) */
interior vacuum
punctum_legere (
      AetherophoniMontatio* m,
            constans Fines* campus,
                   Punctum  p)
{
    m->nota    = aetherophonum_nota(p.x, campus->latitudo,
        nota_infima(m),
        octavae(m), gradus(m));
    m->lucrum  = aetherophonum_lucrum(p.y, campus->altitudo);
}


/* ==================================================
 * Praecepta
 * ================================================== */

interior b32
fac_tibiam (
    vacuum* ctx)
{
    AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;
      constans character* t = tibia_currens(m);
          memoriae_index  n = magnitudo(tibiae)
              / magnitudo(tibiae[ZEPHYRUM]);
          memoriae_index k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        si (tibiae[k] == t)
        {
            redde campum_scribere(m, "tibia", tibiae[(k + I) % n]);
        }
    }
    redde FALSUM;
}

interior b32
fac_gradus (
    vacuum* ctx)
{
    AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;

    redde campum_scribere(m, "gradus", gradus(m) ? "falsum" : "verum");
}

interior b32
fac_vibratum (
    vacuum* ctx)
{
    AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;

    redde campum_scribere(m, "vibratus",
        vibratus(m) ? "falsum" : "verum");
}

interior b32
fac_infra (
    vacuum* ctx)
{
    AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;
                     s32  n = nota_infima(m) - XII;

    si (n < ZEPHYRUM)
    {
        n = ZEPHYRUM;
    }
    redde numerum_scribere(m, "nota_infima", n);
}

interior b32
fac_supra (
    vacuum* ctx)
{
    AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;
                     s32  n = nota_infima(m) + XII;

    si (n + octavae(m) * XII > AETHEROPHONUM_NOTA_SUMMA)
    {
        redde VERUM;
    }
    redde numerum_scribere(m, "nota_infima", n);
}

interior b32
fac_angustius (
    vacuum* ctx)
{
    AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;
                     s32  o = octavae(m) - I;

    redde numerum_scribere(m, "octavae", o < I ? I : o);
}

interior b32
fac_latius (
    vacuum* ctx)
{
    AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;
                     s32  o = octavae(m) + I;

    si (o > AETHEROPHONUM_OCTAVAE_MAXIMAE)
    {
        o = AETHEROPHONUM_OCTAVAE_MAXIMAE;
    }
    redde numerum_scribere(m, "octavae", o);
}

/* Esc: vox tenta dimittitur; aliter latus prius (vicus) */
interior b32
fac_redire (
    vacuum* ctx)
{
    AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;

    si (m->tenetur)
    {
        vocem_dimittere(m);
        redde VERUM;
    }
    redde FALSUM;
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "aetherophonum.tibia", "Next instrument", "f", VISUS_AETHEROPHONI,
        ZEPHYRUM, fac_tibiam },
    { "aetherophonum.gradus", "Snap to semitones", "g",
        VISUS_AETHEROPHONI, ZEPHYRUM, fac_gradus },
    { "aetherophonum.vibratus", "Vibrato", "v", VISUS_AETHEROPHONI,
        ZEPHYRUM, fac_vibratum },
    { "aetherophonum.infra", "Octave down", "[", VISUS_AETHEROPHONI,
        ZEPHYRUM, fac_infra },
    { "aetherophonum.supra", "Octave up", "]", VISUS_AETHEROPHONI,
        ZEPHYRUM, fac_supra },
    { "aetherophonum.angustius", "Narrower range", "-",
        VISUS_AETHEROPHONI, ZEPHYRUM, fac_angustius },
    { "aetherophonum.latius", "Wider range", "=", VISUS_AETHEROPHONI,
        ZEPHYRUM, fac_latius },
    { "aetherophonum.redire", "Release / back", "Esc",
        VISUS_AETHEROPHONI, ZEPHYRUM, fac_redire },
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
     AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;
      constans Praeceptum* t;
                  Punctum  p;
                    Fines  campus;

    (vacuum)repo;
    (vacuum)des;
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, VISUS_AETHEROPHONI,
            ev);
        redde t && praecepta_status(t, VISUS_AETHEROPHONI,
            requisita_nulla, m) == PRAECEPTUM_PRAESTO && t->fn(m);
    }
    si (   !nodus
        || (   ev->genus != EVENTUS_MUS_DEPRESSUS
            && ev->genus != EVENTUS_MUS_MOTUS
            && ev->genus != EVENTUS_MUS_LIBERATUS))
    {
        redde FALSUM;
    }
    p.x     = (s32)ev->datum.mus.x;
    p.y     = (s32)ev->datum.mus.y;
    p       = destinatio_ad_locale(nodus, p);
    campus  = nodus->fines;
    commutatio (ev->genus)
    {
        casus EVENTUS_MUS_DEPRESSUS:
            si (   ev->datum.mus.botton != MUS_SINISTER
                || !chorda_aequalis_literis(nodus->id, "campus"))
            {
                redde FALSUM;
            }
            vocem_dimittere(m);
            motus_captura_ponere(motus, nodus->id);
            m->tenetur = VERUM;
            punctum_legere(m, &campus, p);
            m->vox = m->porta.incipere
                ? m->porta.incipere(m->porta.ctx, tibia_currens(m),
                      vibratus(m), m->nota, m->lucrum)
                : ZEPHYRUM;
            redde VERUM;
        casus EVENTUS_MUS_MOTUS:
            si (!m->tenetur)
            {
                redde FALSUM;
            }
            punctum_legere(m, &campus, p);
            si (m->vox != ZEPHYRUM && m->porta.ponere)
            {
                m->porta.ponere(m->porta.ctx, m->vox, m->nota,
                    m->lucrum,
                    gradus(m) ? AETHEROPHONUM_TRANSITUS_GRADUUM
                              : AETHEROPHONUM_TRANSITUS);
            }
            redde VERUM;
        ordinarius:
            /* solutio: porta clausa, captura tollitur */
            si (!m->tenetur)
            {
                motus_captura_tollere(motus);
                redde FALSUM;
            }
            vocem_dimittere(m);
            motus_captura_tollere(motus);
            redde VERUM;
    }
}

b32
aetherophonum_ictus_primus (
                 vacuum* ctx,
     InsulaRepositorium* repo,
                  Motus* motus,
              Componens* arbor,
       constans Eventus* ev)
{
     AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;
                Componens* campus;
                  Punctum  p;

    si (   !m || !arbor || ev->genus != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    campus = componens_invenire_in_spatio(arbor, m->ramus.via,
        chorda_ex_literis("campus", m->piscina));
    si (!campus)
    {
        redde FALSUM;
    }
    p.x  = (s32)ev->datum.mus.x;
    p.y  = (s32)ev->datum.mus.y;
    p    = destinatio_ad_locale(campus, p);
    si (   p.x < ZEPHYRUM || p.y < ZEPHYRUM
        || p.x >= campus->fines.latitudo
        || p.y >= campus->fines.altitudo)
    {
        redde FALSUM;
    }
    redde agere(repo, motus, NIHIL, campus, ev, m);
}


/* ==================================================
 * Figura
 * ================================================== */

interior vacuum
textum_pingere (
               Mandata* md,
                   s32  x,
                   s32  y,
    constans character* textus,
            ColorThema  color)
{
    mandata_textus(md, x, y, chorda_ex_literis(textus, md->piscina),
        FONS_PARVUS, figura_color_thematis(color));
}

interior vacuum
figura_aetherophoni (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
     AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;
                    Fines  f;
                    Fines  campus;
                  Punctum  a;
                  Punctum  b;
                      s32  infima;
                      s32  semitonia;
                      s32  n;
                      s32  x;
                character  appellatio[XVI];
                character  status[LXIV];
                character  summa[XVI];

    (vacuum)thema;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, f, chorda_aequalis_literis(c->titulus,
        "focatum"));
    si (!m)
    {
        redde;
    }
    campus     = campi_fines(f.latitudo, f.altitudo);
    infima     = nota_infima(m);
    semitonia  = octavae(m) * XII;
    /* semitonia: linea quaeque; C fortior et nominata */
    per (n = ZEPHYRUM; n <= semitonia && semitonia > ZEPHYRUM; n++)
    {
        x    = campus.x + campus.latitudo * n / semitonia;
        a.x  = x;
        a.y  = campus.y;
        b.x  = x;
        b.y  = campus.y + campus.altitudo - I;
        si ((infima + n) % XII == ZEPHYRUM)
        {
            mandata_linea(md, a, b, I, figura_color_thematis(
                COLOR_TEXT_DIM));
            nomen_notae((f32)(infima + n), appellatio);
            textum_pingere(md, x + II, campus.y + II, appellatio,
                COLOR_TEXT_DIM);
        }
        alioquin
        {
            mandata_linea(md, a, b, I, figura_color_thematis(
                COLOR_BORDER));
        }
    }
    /* nota currens dum tenetur */
    si (m->tenetur && semitonia > ZEPHYRUM)
    {
        x    = campus.x + (s32)((f64)campus.latitudo
             * (f64)(m->nota - (f32)infima) / (f64)semitonia);
        a.x = x;
        a.y = campus.y;
        b.x = x;
        b.y = campus.y + campus.altitudo - I;
        mandata_linea(md, a, b, II, figura_color_thematis(
            COLOR_ACCENT_PRIMARY));
        nomen_notae(m->nota, appellatio);
        textum_pingere(md, x + III, campus.y + campus.altitudo - XII,
            appellatio, COLOR_ACCENT_PRIMARY);
    }
    /* linea status: tibia, ambitus, vibratus, gradus */
    nomen_notae((f32)infima, appellatio);
    nomen_notae((f32)(infima + semitonia), summa);
    sprintf(status, "%s  %s-%s%s%s", tibia_currens(m), appellatio,
        summa,
        vibratus(m) ? "  vibratus" : "", gradus(m) ? "  gradus" : "");
    textum_pingere(md, II, campus.y + campus.altitudo + II, status,
        COLOR_TEXT);
}


/* ==================================================
 * Componere et montare
 * ================================================== */

Componens*
aetherophonum_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
     AetherophoniMontatio* m = (AetherophoniMontatio*)ctx;
                Componens* radix;
                Componens* campus;
                    Fines  f;

    (vacuum)repo;
    si (!m)
    {
        redde NIHIL;
    }
    radix = componens_creare(piscina, intern, "aetherophonum",
        PARTES_TABULA);
    campus = componens_creare(piscina, intern, "campus", PARTES_NULLUM);
    si (!radix || !campus)
    {
        redde radix;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = superficies(m, VERUM);
    f.altitudo  = superficies(m, FALSUM);
    componens_ponere_fines(radix, f);
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_aetherophoni);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    componens_ponere_fines(campus, campi_fines(f.latitudo, f.altitudo));
    componens_ponere_actio(campus, actio_aetherophoni);
    componens_addere_liberum(radix, campus);
    redde radix;
}

b32
aetherophonum_montare (
              AetherophoniMontatio* m,
                           Piscina* piscina,
               InternamentumChorda* intern,
                InsulaRepositorium* repo,
                constans character* id,
        constans AetherophoniPorta* porta,
                               i32  latitudo,
                               i32  altitudo,
                   StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(AetherophoniMontatio));
    m->piscina   = piscina;
    m->intern    = intern;
    m->latitudo  = latitudo;
    m->altitudo  = altitudo;
    si (porta)
    {
        m->porta = *porta;
    }
    m->ramus = id ? insula_ramus(repo, "aetherophonum", id)
                  : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "aetherophonum",
        "aetherophonum");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"aetherophonum\"", id ? id : "aetherophonum",
               piscina, intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    redde m->actiones && m->figurae
        && praecepta_verificare(TABULA_PRAECEPTORUM)
        && actio_registrare(m->actiones, actio_aetherophoni, agere, m)
        && figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_aetherophoni, m);
}
