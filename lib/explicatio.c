/* explicatio.c - Harmonia explicata (vide include/explicatio.h)
 *
 * TRANSITUS DUO: (1) fenestrae ex punctis eventuum (initia, fines
 * notarum, initia mensurarum) - intervallum quodque classem infimam
 * sonantem habet; aequalia contigua intra mensuram iunguntur. Acordum
 * fenestrae: candidatae (forma: gradus aut altitudo eadem ab altero
 * latere) per classes removentur, subsets ordine magnitudinis.
 * (2) segmenta ordine temporis genus accipiunt - suspensio genus
 * segmenti prioris legit (iam iudicatum), anticipatio acordum
 * fenestrae proximae (transitu primo notum).
 */

#include "explicatio.h"
#include "partitura.h"
#include <stdlib.h>
#include <string.h>

/* nota sonans in parte sua; prior/proxima contiguae (pausa frangit) */
nomen structura {
    constans CarmenNota* nota;
                    i32  pars;
                    s64  initium;
                    s64  finis;
                    s32  prior;      /* index; -I nulla */
                    s32  proxima;
                    i32  segmentum;  /* primum (post ordinationem) */
                    i32  segmenta;   /* numerus */
} Locata;

/* classes chordarum fenestrae (transitu primo) */
nomen structura {
    i32 classes;      /* acordi (bits) */
    s32 septima;      /* classis septimae; -I nulla */
} AcordumFenestrae;

hic_manens constans character* TITULI[X] = {
    "acordi", "septima", "transitus", "vicinus", "appoggiatura",
    "suspensio", "retardatio", "anticipatio", "escapata", "ignota"
};

hic_manens constans character* ABBREVIATA[X] = {
    "", "sept.", "tr.", "vic.", "app.", "susp.", "ret.", "ant.", "esc.",
    "?"
};

/* gradus maioris a finali (tonica) */
hic_manens constans s32 SCALA[VII] = { 0, 2, 4, 5, 7, 9, 11 };

interior s32
_modulus (
    s32 a,
    s32 m)
{
    redde ((a % m) + m) % m;
}

interior i32
_bit (
    i32 classis)
{
    redde (i32)I << classis;
}

interior s32
_absolutum (
    s32 d)
{
    redde d < ZEPHYRUM ? -d : d;
}

interior b32
_gradus (
    s32 d)
{
    redde _absolutum(d) == I || _absolutum(d) == II;
}

interior i32
_numerus_bitorum (
    i32 b)
{
    i32 n = ZEPHYRUM;

    dum (b != ZEPHYRUM)
    {
        b &= b - I;
        n++;
    }
    redde n;
}

interior integer
_ordo_punctorum (
    constans vacuum* a,
    constans vacuum* b)
{
    s64 x = *(constans s64*)a;
    s64 y = *(constans s64*)b;

    redde x < y ? -I : (x > y ? I : ZEPHYRUM);
}

interior integer
_ordo_segmentorum (
    constans vacuum* a,
    constans vacuum* b)
{
    constans ExplicatioSegmentum* x = (constans ExplicatioSegmentum*)a;
    constans ExplicatioSegmentum* y = (constans ExplicatioSegmentum*)b;

    si (x->initium != y->initium)
    {
        redde x->initium < y->initium ? -I : I;
    }
    si (x->pars != y->pars)
    {
        redde x->pars < y->pars ? -I : I;
    }
    si (x->pulsus != y->pulsus)
    {
        redde x->pulsus < y->pulsus ? -I : I;
    }
    redde ZEPHYRUM;
}

/* septima acordi nominati (acordum_proprietates); -I si genus
 * septimam non habet */
interior s32
_septima (
    constans Acordum* h)
{
    s32 intervallum = acordum_proprietates(h->genus)->septima;

    si (intervallum < ZEPHYRUM)
    {
        redde -I;
    }
    redde _modulus((s32)h->radix + intervallum, XII);
}

/* gradus generis (acordum_proprietates): trias/septima III, suspensa
 * II (suspensio ante acordum suspensum: G B D + C, non Gsus4 + B),
 * dyas I, classis una 0 */
interior i32
_gradus_generis (
    AcordumGenus g)
{
    redde acordum_proprietates(g)->gradus;
}

interior b32
_nominatum (
    constans Acordum* h)
{
    redde h->genus != ACORDUM_IGNOTA && h->genus != ACORDUM_NULLA;
}

/* NOTA ADDITA (acordum.h): toni additi (sexta, nona; 7sus4 omnes
 * praeter radicem) acordi pars solum si TENENTUR (tentae: classes
 * notarum sine forma alienae aut fenestram totam sonantium) */
interior b32
_additae_tenentur (
    constans Acordum* h,
                 i32  tentae)
{
    i32 a = acordum_proprietates(h->genus)->additae;
    i32 r = h->radix;

    a = ((a << r) | (a >> (XII - r))) & 0xFFF;
    redde (a & h->classes & ~tentae) == ZEPHYRUM;
}

/* NUCLEUS: si notae tentae (cum basso) iam acordum plenum nominant
 * (trias aut septima), acordum MAIUS (septima aut toni additi) cuius
 * radix aut tertia solum a notis formae alienae venit reicitur - D F#
 * A tentae cum G B transeuntibus = D, non Gmaj9/D. Tentae non plenae
 * (G# B sub C D E: E7/G#) melodiam radicem dare sinunt; triades et
 * dyades non iudicantur */
interior b32
_nucleus_tenetur (
    constans Acordum* h,
                 i32  tentae,
                 i32  bassus,
                 i32  classes)
{
        s32 tertia  = acordum_proprietates(h->genus)->tertia;
        i32 r       = h->radix;
    Acordum t      = acordum_ex_classibus((classes & tentae)
                                          | _bit(bassus), bassus);

    /* tentae non plenae: nihil iudicatur (triades eo ipso: notae tentae
     * removeri nequeunt, trias tenta est lectio sola) */
    si (!_nominatum(&t) || acordum_proprietates(t.genus)->gradus < III)
    {
        redde VERUM;
    }
    si (r != bassus && !(tentae & _bit(r)))
    {
        redde FALSUM;
    }
    si (tertia >= ZEPHYRUM)
    {
        i32 t = (r + (i32)tertia) % XII;

        si (t != bassus && !(tentae & _bit(t)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* classis in scala clavis ad pulsum (signatura; minor = relativa) */
interior b32
_diatonica (
    constans Carmen* carmen,
                s64  pulsus,
                s32  classis,
                s32* finalis)
{
    s32 q      = ZEPHYRUM;
    b32 minor  = FALSUM;
    s32 t;
    i32 i;

    per (i = ZEPHYRUM; i < carmen->numerus_clavium; i++)
    {
        si (carmen->claves[i].pulsus <= pulsus)
        {
            q      = carmen->claves[i].quintae;
            minor  = carmen->claves[i].minor;
        }
    }
    t         = _modulus(VII * q, XII);
    *finalis  = minor ? _modulus(t + IX, XII) : t;
    per (i = ZEPHYRUM; i < VII; i++)
    {
        si (_modulus(t + SCALA[i], XII) == classis)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* tritonus solus -> septima dominantis implicita: radix tertia maior
 * infra classem alteram; diatonica in clave praefertur, deinde
 * dominans clavis, deinde classis minor */
interior b32
_implicitum (
    constans Carmen* carmen,
                s64  pulsus,
                i32  classes,
                i32  bassus,
            Acordum* h)
{
    s32 x = -I;
    s32 y;
    s32 r1;
    s32 r2;
    s32 finalis;
    b32 d1;
    b32 d2;
    i32 c;

    si (_numerus_bitorum(classes) != II)
    {
        redde FALSUM;
    }
    per (c = ZEPHYRUM; c < XII && x < ZEPHYRUM; c++)
    {
        si (classes & _bit(c))
        {
            x = (s32)c;
        }
    }
    y = x + VI;
    si (y >= XII || !(classes & _bit((i32)y)))
    {
        redde FALSUM;
    }
    r1          = _modulus(x - IV, XII);
    r2          = _modulus(y - IV, XII);
    d1          = _diatonica(carmen, pulsus, r1, &finalis);
    d2          = _diatonica(carmen, pulsus, r2, &finalis);
    h->genus    = ACORDUM_SEPTIMA;
    h->classes  = classes;
    h->bassus   = bassus;
    si (d1 != d2)
    {
        h->radix = (i32)(d1 ? r1 : r2);
    }
    alioquin si (r2 == _modulus(finalis + VII, XII))
    {
        h->radix = (i32)r2;
    }
    alioquin
    {
        h->radix = (i32)(r1 == _modulus(finalis + VII, XII) || r1 < r2
                         ? r1 : r2);
    }
    redde VERUM;
}


/* ==================================================
 * Forma melodica
 * ================================================== */

nomen structura {
    b32 prior;        /* motus ab altitudine priore notus */
    s32 d1;           /* altitudo - prior */
    b32 praeparata;   /* altitudo eadem, segmentum prius acordi */
    b32 proxima;
    s32 d2;           /* proxima - altitudo */
} Motus;

/* segmentum ultimum notae x ante segmentum s (segmenta ordine
 * (initium, pars) - notae eiusdem segmenta non contigua; campus
 * fenestra iam fenestram tenet, non locatam: per notam, quae in parte
 * sua unica est); -I nullum */
interior s32
_segmentum_prius (
    constans ExplicatioSegmentum* segmenta,
                             i32  s,
                 constans Locata* x)
{
    s32 j;

    per (j = (s32)s - I; j >= ZEPHYRUM; j--)
    {
        si (segmenta[j].nota == x->nota)
        {
            redde j;
        }
    }
    redde -I;
}

/* motus segmenti; praeparatio ex generibus iam datis (segmenta
 * priora ordine temporis) */
interior Motus
_motus (
                 constans Locata* locatae,
    constans ExplicatioSegmentum* segmenta,
                             i32  s,
                             i32  l)
{
    constans Locata* n = &locatae[l];
              Motus  m;
                s32  a = (s32)n->nota->nota;

    m.prior       = FALSUM;
    m.d1          = ZEPHYRUM;
    m.praeparata  = FALSUM;
    m.proxima     = n->proxima >= ZEPHYRUM;
    m.d2          = m.proxima ? (s32)locatae[n->proxima].nota->nota - a
                              : ZEPHYRUM;
    si (segmenta[s].initium > n->initium)
    {
        /* nota tenta: segmentum prius EIUSDEM notae */
        s32 j = _segmentum_prius(segmenta, s, n);

        m.prior       = VERUM;
        m.praeparata  = j >= ZEPHYRUM
            && (   segmenta[j].genus == EXPLICATIO_ACORDI
                || segmenta[j].genus == EXPLICATIO_SEPTIMA);
    }
    alioquin si (n->prior >= ZEPHYRUM)
    {
        constans Locata* p = &locatae[n->prior];
                    s32  j = _segmentum_prius(segmenta, s, p);

        m.prior  = VERUM;
        m.d1     = a - (s32)p->nota->nota;
        m.praeparata  = m.d1 == ZEPHYRUM && j >= ZEPHYRUM
                     && (   segmenta[j].genus == EXPLICATIO_ACORDI
                         || segmenta[j].genus == EXPLICATIO_SEPTIMA);
    }
    redde m;
}

/* candidata: gradus aut altitudo eadem ab altero latere */
interior b32
_candidata (
    constans Locata* locatae,
                i32  l,
                b32  tenta)
{
    constans Locata* n = &locatae[l];
                s32  a = (s32)n->nota->nota;

    si (tenta)
    {
        redde VERUM;
    }
    si (n->prior >= ZEPHYRUM)
    {
        s32 d = a - (s32)locatae[n->prior].nota->nota;

        si (d == ZEPHYRUM || _gradus(d))
        {
            redde VERUM;
        }
    }
    si (n->proxima >= ZEPHYRUM)
    {
        s32 d = (s32)locatae[n->proxima].nota->nota - a;

        si (d == ZEPHYRUM || _gradus(d))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Explicatio carminis
 * ================================================== */

interior b32
_sonans (
    constans CarmenPars* p,
                    b32  solus)
{
    redde !p->mutus && (!solus || p->solus);
}

/* litterae notarum fenestrae in acordum (orthographia auctoris) */
interior vacuum
_litteras_ponere (
                           Acordum* h,
      constans ExplicatioSegmentum* segmenta,
                               i32  primum,
                               i32  numerus)
{
    i32 i;

    per (i = primum; i < primum + numerus; i++)
    {
        harmonia_litteram_notare(h, segmenta[i].nota);
    }
}

/* acordum fenestrae: remotio minima candidatarum */
interior vacuum
_acordum (
                            constans Carmen* carmen,
                            constans Locata* locatae,
               constans ExplicatioSegmentum* segmenta,
                                        i32  primum,
                                        i32  numerus,
                         ExplicatioFenestra* f,
                           AcordumFenestrae* a)
{
         i32 classes     = ZEPHYRUM;
         i32 removendae;
         i32 non_cand  = ZEPHYRUM;
         i32 tentae    = ZEPHYRUM;
         i32 bassus    = f->harmonia.bassus;
         i32 optimum   = ZEPHYRUM;
         b32 inventum  = FALSUM;
         i32 magnitudo_s;
         i32 s;
         i32 i;
     Acordum h;
     Acordum electa;
         i32 meritum_electum = ZEPHYRUM;

    per (i = primum; i < primum + numerus; i++)
    {
        i32 c = (i32)segmenta[i].nota->nota % XII;
        i32 l = (i32)segmenta[i].fenestra;   /* index locatae */

        classes |= _bit(c);
        /* nota generata (acordo declarato sumpta) numquam candidata */
        si (   segmenta[i].nota->acordi
            || !_candidata(locatae, l,
                           segmenta[i].initium > segmenta[i].pulsus))
        {
            non_cand |= _bit(c);
        }
        /* cum fenestra incipiens et eam totam sonans: tenta (toni
         * additi); praeparata (suspensio) non */
        si (   segmenta[i].pulsus == f->initium
            && segmenta[i].pulsus + segmenta[i].nota->longitudo
               >= f->finis)
        {
            tentae |= _bit(c);
        }
    }
    tentae         |= non_cand;
    removendae     = classes & ~non_cand & ~_bit(bassus);
    f->implicitum  = FALSUM;
    electa         = acordum_ex_classibus(classes, bassus);
    a->classes     = classes;
    si (classes == ZEPHYRUM)
    {
        f->harmonia  = electa;
        a->septima   = -I;
        _litteras_ponere(&f->harmonia, segmenta, primum, numerus);
        redde;
    }
    /* subsets ordine magnitudinis; prima magnitudo cum nomine vincit */
    per (magnitudo_s = ZEPHYRUM;
         magnitudo_s <= _numerus_bitorum(removendae) && !inventum;
         magnitudo_s++)
    {
        per (s = removendae; ; s = (s - I) & removendae)
        {
            si (_numerus_bitorum(s) == magnitudo_s)
            {
                i32 manentes = ZEPHYRUM;
                i32 meritum;

                b32 implicata = FALSUM;

                h = acordum_ex_classibus(classes & ~s, bassus);
                si (   _nominatum(&h)
                    && (   !_additae_tenentur(&h, tentae)
                        || !_nucleus_tenetur(&h, tentae, bassus,
                                             classes)))
                {
                    h.genus = ACORDUM_IGNOTA;
                }
                si (   !_nominatum(&h)
                    && _implicitum(carmen, f->initium, classes & ~s,
                                   bassus, &h))
                {
                    implicata = VERUM;
                }
                si (_nominatum(&h))
                {
                    per (i = primum; i < primum + numerus; i++)
                    {
                        si (!(s & _bit((i32)segmenta[i].nota->nota
                            % XII)))
                        {
                            manentes++;
                        }
                    }
                    meritum = manentes * XVI
                            + _gradus_generis(h.genus) * II
                            + (h.radix == bassus ? I : ZEPHYRUM);
                    si (!inventum || meritum > meritum_electum)
                    {
                        inventum         = VERUM;
                        optimum          = s;
                        electa           = h;
                        meritum_electum  = meritum;
                        f->implicitum    = implicata;
                    }
                }
            }
            si (s == ZEPHYRUM)
            {
                frange;
            }
        }
    }
    si (inventum)
    {
        f->harmonia  = electa;
        a->classes   = classes & ~optimum;
        a->septima   = _septima(&electa);
        _litteras_ponere(&f->harmonia, segmenta, primum, numerus);
        redde;
    }
    /* ignotum: classes praeter bassum alienae; nomen a tono addito non
     * tento reiectum non redit */
    f->harmonia  = acordum_ex_classibus(classes, bassus);
    si (   _nominatum(&f->harmonia)
        && !_additae_tenentur(&f->harmonia, tentae))
    {
        f->harmonia.genus = ACORDUM_IGNOTA;
    }
    a->classes = _bit(bassus);
    a->septima = -I;
    _litteras_ponere(&f->harmonia, segmenta, primum, numerus);
}

/* bassus transiens: nota infima fenestrae gradu intrat et exit,
 * superiores omnes in acordo fenestrae prioris contiguae nominatae ->
 * acordum prius cum basso hoc. VERUM si sic factum */
interior b32
_bassus_transiens (
                          constans Locata* locatae,
             constans ExplicatioSegmentum* intra,
                                      i32  numerus,
                                      i32  f,
                       ExplicatioFenestra* fenestrae,
                         AcordumFenestrae* acorda)
{
     constans Locata* b           = NIHIL;
                 i32  superiores  = ZEPHYRUM;
                 i32  bassus      = fenestrae[f].harmonia.bassus;
                 i32  i;
                 s32  d1;
                 s32  d2;

    si (   f                      == ZEPHYRUM
        || fenestrae[f - I].finis != fenestrae[f].initium
        || !_nominatum(&fenestrae[f - I].harmonia))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        constans Locata* l = &locatae[intra[i].fenestra];

        si (   l->initium               == fenestrae[f].initium
            && (i32)l->nota->nota % XII == bassus
            && (b == NIHIL || l->nota->nota < b->nota->nota))
        {
            b = l;
        }
    }
    si (b == NIHIL || b->prior < ZEPHYRUM || b->proxima < ZEPHYRUM)
    {
        redde FALSUM;
    }
    d1 = (s32)b->nota->nota - (s32)locatae[b->prior].nota->nota;
    d2 = (s32)locatae[b->proxima].nota->nota - (s32)b->nota->nota;
    si (!_gradus(d1) || !_gradus(d2))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (&locatae[intra[i].fenestra] != b)
        {
            superiores |= _bit((i32)intra[i].nota->nota % XII);
        }
    }
    si ((superiores & ~acorda[f - I].classes) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    fenestrae[f].harmonia         = fenestrae[f - I].harmonia;
    fenestrae[f].harmonia.bassus  = bassus;
    fenestrae[f].implicitum       = fenestrae[f - I].implicitum;
    acorda[f]                     = acorda[f - I];
    redde VERUM;
}

/* genus segmenti alieni ex motu */
interior ExplicatioGenus
_genus_alienum (
    constans Motus* m,
               b32  anticipat,
               b32* resoluta)
{
    *resoluta = FALSUM;
    si (m->prior && m->d1 == ZEPHYRUM && m->praeparata)
    {
        si (m->proxima && _gradus(m->d2) && m->d2 > ZEPHYRUM)
        {
            *resoluta = VERUM;
            redde EXPLICATIO_RETARDATIO;
        }
        *resoluta = m->proxima && _gradus(m->d2) && m->d2 < ZEPHYRUM;
        redde EXPLICATIO_SUSPENSIO;
    }
    si (m->prior && m->proxima && _gradus(m->d1) && _gradus(m->d2))
    {
        redde (m->d1 > ZEPHYRUM) == (m->d2 > ZEPHYRUM)
            ? EXPLICATIO_TRANSITUS : EXPLICATIO_VICINUS;
    }
    si (   m->prior && m->proxima && _absolutum(m->d1) >= III
        && _gradus(m->d2))
    {
        *resoluta = VERUM;
        redde EXPLICATIO_INNIXA;
    }
    si (   m->prior && m->proxima && _gradus(m->d1)
        && _absolutum(m->d2)  >= III
        && (m->d1 > ZEPHYRUM) != (m->d2 > ZEPHYRUM))
    {
        redde EXPLICATIO_EFFUGIENS;
    }
    si (anticipat)
    {
        redde EXPLICATIO_ANTICIPATIO;
    }
    redde EXPLICATIO_IGNOTA;
}

/* BASSUS ARPEGGIANS: bassus novus (altitudo infima, classis b) ad
 * pulsum t SALTU a priore (gradu = transiens), classis quae in
 * fenestra [a, t) iam sonat, nulla alia nota ad t incipiente (bassus
 * ipse solus) */
interior b32
_bassus_fractus (
    constans Locata* locatae,
                i32  n,
                s64  a,
                s64  t,
                i32  b,
                s32  infima,
                s32  saltus)
{
    b32 sonat = FALSUM;
    i32 j;

    si (b >= XII || (saltus < III && saltus > -III))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < n; j++)
    {
        constans Locata* l = &locatae[j];
                    i32  c = (i32)l->nota->nota % XII;

        si (l->initium < t && l->finis > a && c == b)
        {
            sonat = VERUM;
        }
        si (l->initium == t && (s32)l->nota->nota != infima)
        {
            redde FALSUM;
        }
    }
    redde sonat;
}

/* acordum declaratum (Carmen.acorda, non 'r') sonans ad pulsum t:
 * index; -I nullum */
interior s32
_declaratum (
    constans Carmen* carmen,
                s64  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < carmen->numerus_acordorum; i++)
    {
        constans ComitatusAcordum* a = &carmen->acorda[i];

        si (   !a->nullum && a->pulsus <= t
            && t < a->pulsus + a->longitudo)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

b32
explicatio_carminis (
    constans Carmen* carmen,
            Piscina* piscina,
         Explicatio* explicatio)
{
                          Locata* locatae;
                             s64* puncta;
                             i32* infimae;
                             s32* declarata;
                             s32  infima_prior = -I;
              ExplicatioFenestra* fenestrae;
             ExplicatioSegmentum* segmenta;
                AcordumFenestrae* acorda;
                             i32  n      = ZEPHYRUM;
                             i32  np     = ZEPHYRUM;
                             i32  nf     = ZEPHYRUM;
                             i32  ns     = ZEPHYRUM;
                             b32  solus  = FALSUM;
                             i32  i;
                             i32  j;
                             i32  f;
                             i32  m;

    explicatio->fenestrae            = NIHIL;
    explicatio->numerus_fenestrarum  = ZEPHYRUM;
    explicatio->segmenta             = NIHIL;
    explicatio->numerus_segmentorum  = ZEPHYRUM;

    /* notae sonantes */
    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        solus = solus || carmen->partes[i].solus;
    }
    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        si (!_sonans(&carmen->partes[i], solus))
        {
            perge;
        }
        per (f = ZEPHYRUM; f < carmen->partes[i].numerus_frustorum; f++)
        {
            constans CarmenFrustum* fr = &carmen->partes[i].frusta[f];

            per (j = ZEPHYRUM; j < fr->numerus; j++)
            {
                si (fr->notae[j].praeparatum.mensura == ZEPHYRUM)
                {
                    n++;
                }
            }
        }
    }
    m = ZEPHYRUM;
    dum (carmen_pulsus_mensurae(carmen, m + I) < carmen->pulsus)
    {
        m++;
    }
    locatae  = (Locata*)piscina_allocare(piscina,
        (memoriae_index)(n + I) * magnitudo(Locata));
    puncta   = (s64*)piscina_allocare(piscina,
        (memoriae_index)(II * n + m + II
                         + II * carmen->numerus_acordorum)
        * magnitudo(s64));
    si (locatae == NIHIL || puncta == NIHIL)
    {
        redde FALSUM;
    }
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        /* nota prior per vocem partis (comitatus voces dat; textus
         * scriptus vox 0) */
        s32 prior[X];

        per (j = ZEPHYRUM; j < X; j++)
        {
            prior[j] = -I;
        }
        si (!_sonans(&carmen->partes[i], solus))
        {
            perge;
        }
        per (f = ZEPHYRUM; f < carmen->partes[i].numerus_frustorum; f++)
        {
            constans CarmenFrustum* fr = &carmen->partes[i].frusta[f];

            per (j = ZEPHYRUM; j < fr->numerus; j++)
            {
                 constans CarmenNota* nota  = &fr->notae[j];
                              Locata* l     = &locatae[n];
                                 i32  v     = nota->vox < X ? nota->vox
                                                           : IX;

                si (nota->praeparatum.mensura > ZEPHYRUM)
                {
                    prior[v] = -I;
                    perge;
                }
                l->nota     = nota;
                l->pars     = i;
                l->initium  = fr->pulsus + nota->pulsus;
                l->finis    = l->initium + nota->longitudo;
                l->prior    = prior[v] >= ZEPHYRUM
                           && locatae[prior[v]].finis == l->initium
                           ? prior[v] : -I;
                l->proxima  = -I;
                si (l->prior >= ZEPHYRUM)
                {
                    locatae[l->prior].proxima = (s32)n;
                }
                prior[v]      = (s32)n;
                puncta[np++]  = l->initium;
                puncta[np++]  = l->finis;
                n++;
            }
        }
    }
    per (i = ZEPHYRUM; i <= m; i++)
    {
        puncta[np++] = carmen_pulsus_mensurae(carmen, (i32)(i + I));
    }
    /* mutationes harmoniae declaratae fenestras secant */
    per (i = ZEPHYRUM; i < carmen->numerus_acordorum; i++)
    {
        puncta[np++] = carmen->acorda[i].pulsus;
        puncta[np++] = carmen->acorda[i].pulsus
                     + carmen->acorda[i].longitudo;
    }
    puncta[np++] = ZEPHYRUM;
    qsort(puncta, (size_t)np, magnitudo(s64), _ordo_punctorum);
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < np; i++)
    {
        si (j == ZEPHYRUM || puncta[i] != puncta[j - I])
        {
            puncta[j++] = puncta[i];
        }
    }
    np = j;
    si (np > ZEPHYRUM && puncta[np - I] > carmen->pulsus)
    {
        /* initia mensurarum ultra finem (ultima ad finem) */
        dum (np > I && puncta[np - II] >= carmen->pulsus)
        {
            np--;
        }
    }

    /* fenestrae: classis infima per intervallum, iunctae intra
     * mensuram */
    infimae    = (i32*)piscina_allocare(piscina,
        (memoriae_index)(np + I) * magnitudo(i32));
    declarata  = (s32*)piscina_allocare(piscina,
        (memoriae_index)(np + I) * magnitudo(s32));
    fenestrae  = (ExplicatioFenestra*)piscina_allocare(piscina,
        (memoriae_index)(np + I) * magnitudo(ExplicatioFenestra));
    si (infimae == NIHIL || declarata == NIHIL || fenestrae == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i + I < np; i++)
    {
        s32 infima = -I;
        s32 mensura_i;

        per (j = ZEPHYRUM; j < n; j++)
        {
            si (   locatae[j].initium <= puncta[i]
                && puncta[i] < locatae[j].finis
                && (   infima < ZEPHYRUM
                    || (s32)locatae[j].nota->nota < infima))
            {
                infima = (s32)locatae[j].nota->nota;
            }
        }
        infimae[i]  = infima < ZEPHYRUM ? XII : (i32)(infima % XII);
        mensura_i   = (s32)I;
        dum (carmen_pulsus_mensurae(carmen, (i32)mensura_i + I)
             <= puncta[i])
        {
            mensura_i++;
        }
        /* iunctio: intra mensuram, acordo declarato eodem (bassus
         * quilibet) aut, sine declarato, basso eodem */
        si (   nf > ZEPHYRUM
            && carmen_pulsus_mensurae(carmen, (i32)mensura_i)
                   <= fenestrae[nf - I].initium
            && _declaratum(carmen, puncta[i]) == declarata[nf - I]
            && (   declarata[nf - I] >= ZEPHYRUM
                || (i32)fenestrae[nf - I].harmonia.bassus
                   == infimae[i]
                || _bassus_fractus(locatae, n, fenestrae[nf
                    - I].initium,
                               puncta[i], infimae[i], infima,
                               infima - infima_prior)))
        {
            fenestrae[nf - I].finis = puncta[i + I];
        }
        alioquin
        {
            fenestrae[nf].initium           = puncta[i];
            fenestrae[nf].finis             = puncta[i + I];
            fenestrae[nf].harmonia.bassus   = infimae[i];
            fenestrae[nf].harmonia.genus    = ACORDUM_NULLA;
            fenestrae[nf].harmonia.radix    = infimae[i];
            fenestrae[nf].harmonia.classes  = ZEPHYRUM;
            memset(fenestrae[nf].harmonia.litterae, ZEPHYRUM,
                   magnitudo(fenestrae[nf].harmonia.litterae));
            fenestrae[nf].implicitum = FALSUM;
            fenestrae[nf].declaratum = FALSUM;
            declarata[nf]                   = _declaratum(carmen,
                                                          puncta[i]);
            nf++;
        }
        infima_prior = infima;
    }

    /* segmenta: nota x fenestra; 'fenestra' ad tempus index locatae
     * tenet, deinde fenestram */
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (f = ZEPHYRUM; f < nf; f++)
        {
            si (   fenestrae[f].initium < locatae[i].finis
                && locatae[i].initium < fenestrae[f].finis)
            {
                ns++;
            }
        }
    }
    segmenta = (ExplicatioSegmentum*)piscina_allocare(piscina,
        (memoriae_index)(ns + I) * magnitudo(ExplicatioSegmentum));
    acorda   = (AcordumFenestrae*)piscina_allocare(piscina,
        (memoriae_index)(nf + I) * magnitudo(AcordumFenestrae));
    si (segmenta == NIHIL || acorda == NIHIL)
    {
        redde FALSUM;
    }
    ns = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (f = ZEPHYRUM; f < nf; f++)
        {
            si (   fenestrae[f].initium < locatae[i].finis
                && locatae[i].initium < fenestrae[f].finis)
            {
                ExplicatioSegmentum* s = &segmenta[ns++];

                s->nota     = locatae[i].nota;
                s->pars     = locatae[i].pars;
                s->pulsus   = locatae[i].initium;
                s->initium  = locatae[i].initium;
                si (fenestrae[f].initium > s->initium)
                {
                    s->initium = fenestrae[f].initium;
                }
                s->fenestra    = i;   /* locata ad tempus */
                s->genus       = EXPLICATIO_ACORDI;
                s->resoluta    = FALSUM;
                s->semitonium  = FALSUM;
            }
        }
    }
    qsort(segmenta, (size_t)ns, magnitudo(ExplicatioSegmentum),
          _ordo_segmentorum);
    /* locatae: segmentum primum et numerus (segmenta notae non
     * contigua: ordo (initium, pars) - _segmentum_prius ea quaerit) */
    per (i = ZEPHYRUM; i < n; i++)
    {
        locatae[i].segmenta = ZEPHYRUM;
    }
    per (i = ns; i > ZEPHYRUM; i--)
    {
        Locata* l = &locatae[segmenta[i - I].fenestra];

        l->segmentum = i - I;
        l->segmenta++;
    }

    /* transitus primus: acordum fenestrae (segmenta fenestrae
     * contigua non sunt ordine temporis - per initium colliguntur) */
    {
        ExplicatioSegmentum* intra = (ExplicatioSegmentum*)
            piscina_allocare(piscina,
                (memoriae_index)(ns
                    + I) * magnitudo(ExplicatioSegmentum));

        si (intra == NIHIL)
        {
            redde FALSUM;
        }
        per (f = ZEPHYRUM; f < nf; f++)
        {
            i32 k = ZEPHYRUM;

            per (i = ZEPHYRUM; i < ns; i++)
            {
                si (   segmenta[i].initium >= fenestrae[f].initium
                    && segmenta[i].initium < fenestrae[f].finis)
                {
                    intra[k++] = segmenta[i];
                }
            }
            si (fenestrae[f].harmonia.bassus >= XII)
            {
                acorda[f].classes             = ZEPHYRUM;
                acorda[f].septima             = -I;
                fenestrae[f].harmonia.bassus  = ZEPHYRUM;
                fenestrae[f].harmonia.radix   = ZEPHYRUM;
                perge;
            }
            /* acordum declaratum: id nominatur, notae ad id iudicantur;
             * bassus = infima ad initium (extra classes: declaratus) */
            si (declarata[f] >= ZEPHYRUM)
            {
                Acordum h = carmen->acorda[declarata[f]].acordum;
                    i32 b = fenestrae[f].harmonia.bassus;

                si (h.classes & _bit(b))
                {
                    h.bassus = b;
                }
                fenestrae[f].harmonia    = h;
                fenestrae[f].declaratum  = VERUM;
                acorda[f].classes        = h.classes;
                acorda[f].septima        = _septima(&h);
                perge;
            }
            si (_bassus_transiens(locatae, intra, k, f, fenestrae,
                acorda))
            {
                perge;
            }
            _acordum(carmen, locatae, intra, ZEPHYRUM, k, &fenestrae[f],
                     &acorda[f]);
        }
    }

    /* transitus secundus: genera ordine temporis */
    per (i = ZEPHYRUM; i < ns; i++)
    {
        ExplicatioSegmentum* s = &segmenta[i];
                        i32  l = s->fenestra;
                        i32  c = (i32)s->nota->nota % XII;

        per (f = ZEPHYRUM; f + I < nf
            && fenestrae[f].finis <= s->initium;
             f++)
        {
        }
        s->fenestra = f;
        {
            Motus motus = _motus(locatae, segmenta, i, l);

            s->semitonium = motus.proxima && _absolutum(motus.d2) == I;
        }
        si (acorda[f].classes & _bit(c))
        {
            si ((s32)c == acorda[f].septima)
            {
                Motus m_ = _motus(locatae, segmenta, i, l);

                /* gradu descendit, aut transiens (C D E super E7) */
                s->genus     = EXPLICATIO_SEPTIMA;
                s->resoluta  = m_.proxima && _gradus(m_.d2)
                    && (   m_.d2 < ZEPHYRUM
                        || (   m_.prior && _gradus(m_.d1)
                            && m_.d1 > ZEPHYRUM));
            }
            alioquin
            {
                s->genus = EXPLICATIO_ACORDI;
            }
        }
        alioquin
        {
            Motus m_         = _motus(locatae, segmenta, i, l);
              b32 anticipat  = FALSUM;
              b32 resoluta;

            si (m_.proxima && m_.d2 == ZEPHYRUM)
            {
                 constans Locata* q = &locatae[locatae[l].proxima];
                             i32  g;

                per (g = f; g + I < nf
                    && fenestrae[g].finis <= q->initium;
                     g++)
                {
                }
                anticipat = g != f
                    && (acorda[g].classes & _bit(c)) != ZEPHYRUM;
            }
            s->genus     = _genus_alienum(&m_, anticipat, &resoluta);
            s->resoluta  = resoluta;
        }
    }
    explicatio->fenestrae            = fenestrae;
    explicatio->numerus_fenestrarum  = nf;
    explicatio->segmenta             = segmenta;
    explicatio->numerus_segmentorum  = ns;
    redde VERUM;
}

constans character*
explicatio_titulus (
    ExplicatioGenus genus)
{
    redde (i32)genus < X ? TITULI[genus] : "?";
}

b32
explicatio_lineam_scribere (
               vacuum* ctx,
      constans Carmen* carmen,
                  s64  initium,
                  s64  finis,
    ChordaAedificator* textus)
{
    constans Explicatio* e       = (constans Explicatio*)ctx;
                    b32  bene    = VERUM;
                    b32  primum  = VERUM;
                    i32  f;
                    i32  i;

    per (f = ZEPHYRUM; f < e->numerus_fenestrarum; f++)
    {
        constans ExplicatioFenestra* w        = &e->fenestrae[f];
                                b32  apertum  = FALSUM;
                                s64  ad;

        si (w->initium < initium || w->initium >= finis)
        {
            perge;
        }
        ad = w->finis < finis ? w->finis : finis;
        si (!primum)
        {
            bene = chorda_aedificator_appendere_character(textus, ' ')
                && bene;
        }
        primum  = FALSUM;
        bene    = harmonia_scribere(carmen, &w->harmonia, w->initium,
                                    textus) && bene;
        si (w->implicitum)
        {
            bene = chorda_aedificator_appendere_character(textus, '*')
                && bene;
        }
        bene = chorda_aedificator_appendere_character(textus, ':')
            && bene;
        bene = partitura_valorem_scribere(ad - w->initium, textus)
            && bene;
        per (i = ZEPHYRUM; i < e->numerus_segmentorum; i++)
        {
            constans ExplicatioSegmentum* s = &e->segmenta[i];
                                     b32  irresoluta;

            si (   s->fenestra != f || s->genus == EXPLICATIO_ACORDI
                || (s->genus == EXPLICATIO_SEPTIMA && s->resoluta))
            {
                perge;
            }
            irresoluta = !s->resoluta
                && (   s->genus == EXPLICATIO_SEPTIMA
                    || s->genus == EXPLICATIO_SUSPENSIO
                    || s->genus == EXPLICATIO_RETARDATIO
                    || s->genus == EXPLICATIO_INNIXA);
            bene = chorda_aedificator_appendere_literis(textus,
                apertum ? ", " : " (") && bene;
            apertum = VERUM;
            bene = partitura_notam_scribere(carmen, s->nota, s->pulsus,
                                            textus) && bene;
            bene = chorda_aedificator_appendere_character(textus, ' ')
                && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                ABBREVIATA[s->genus]) && bene;
            si (irresoluta)
            {
                bene = chorda_aedificator_appendere_character(textus,
                    '!') && bene;
            }
        }
        si (apertum)
        {
            bene = chorda_aedificator_appendere_character(textus, ')')
                && bene;
        }
    }
    redde bene;
}
