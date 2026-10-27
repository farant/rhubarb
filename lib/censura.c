/* censura.c - Carmen iudicatum (vide include/censura.h)
 *
 * Regulae ordine enumerationis currunt et inventa in tabulam vocantis
 * addunt (ultra capacitatem solum numerantur); deinde ordinatio
 * stabilis per (mensura, pulsus) - aequalia ordinem regularum servant.
 *
 * NOTA SONANS partis ad pulsum t: initium <= t < finis, sine
 * percussione; plures (acervus) -> summa (parallelae), omnes
 * (dissonantia).
 */

#include "censura.h"
#include "harmonia.h"
#include "partitura.h"
#include "explicatio.h"
#include "functio.h"
#include "piscina.h"
#include <string.h>

hic_manens constans character* TITULI[CENSURA_REGULAE] = {
    "lineae", "clavis", "ambitus", "dissonantia", "parallelae",
        "saltus",
    "tertia", "acorda", "evitanda", "occultae", "decussatio", "spatium"
};

nomen structura {
     constans character* instrumentum;
                    i32  infima;
                    i32  summa;
} AmbitusOrdinarius;

#define AMBITUS_NUMERUS XI

hic_manens constans AmbitusOrdinarius AMBITUS[AMBITUS_NUMERUS] = {
    { "aether",     XXXVI,  XCVI  },
    { "quadrans",   XLVIII, XCVI  },
    { "triangulum", XXIV,   LXXII },
    { "chorda",     XXVIII, LXXXVIII },
    { "campana",    XLVIII, XCVI  },
    { "organum",    XXXVI,  XCVI  },
    { "marimba",    XLV,    XCVI  },
    { "glockenspiel", LXXIX, CVIII },
    { "schola",     XLVIII, LXXIV },
    { "fistula",    LX,     XCVI  },
    { "nubes",      XXXVI,  LXXXIV }
};

/* gradus scalae maioris a tonica */
hic_manens constans s32 SCALA[VII] = {
    ZEPHYRUM, II, IV, V, VII, IX, XI
};

/* notae sonantes partis simul (dissonantia) maximae */
#define CENSURA_SIMUL XVI
/* notae sonantes omnium partium simul (tertia) maximae */
#define CENSURA_SONANTES LXIV

nomen structura {
             constans Carmen* carmen;
       constans CensuraForma* forma;
             CensuraInventum* inventa;
                         i32  capacitas;
                         i32  numerus;
         constans Explicatio* explicatio;   /* dissonantia; NIHIL =
                                             * omnes inventae */
} Iudex;

/* nota cum frusto suo */
nomen structura {
    constans CarmenNota* nota;
                    s32  frustum;
                    s64  initium;   /* absolutus */
} NotaLocata;

interior s32
_modulus (
    s32 a,
    s32 m)
{
    redde ((a % m) + m) % m;
}

vacuum
censura_ordinaria (
    CensuraForma* forma)
{
    memset(forma, ZEPHYRUM, magnitudo(CensuraForma));
    forma->activae[CENSURA_LINEAE]       = VERUM;
    forma->activae[CENSURA_CLAVIS]       = VERUM;
    forma->activae[CENSURA_AMBITUS]      = VERUM;
    forma->activae[CENSURA_DISSONANTIA]  = VERUM;
    forma->activae[CENSURA_ACORDA]       = VERUM;
}

constans character*
censura_titulus (
    CensuraRegula regula)
{
    si ((i32)regula < ZEPHYRUM || regula >= CENSURA_REGULAE)
    {
        redde NIHIL;
    }
    redde TITULI[regula];
}

b32
censura_regulam_invenire (
           chorda  titulus,
    CensuraRegula* regula)
{
    i32 i;

    per (i = ZEPHYRUM; i < (i32)CENSURA_REGULAE; i++)
    {
        si (chorda_aequalis_literis(titulus, TITULI[i]))
        {
            *regula = (CensuraRegula)i;
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
censura_regulas_legere (
          chorda  regulae,
    CensuraForma* forma,
          chorda* ignota)
{
    i32 p = ZEPHYRUM;

    si (   regulae.mensura > ZEPHYRUM && regulae.datum[ZEPHYRUM] != '+'
        && regulae.datum[ZEPHYRUM] != '-')
    {
        memset(forma->activae, ZEPHYRUM, magnitudo(forma->activae));
    }
    dum (p < regulae.mensura)
    {
                  b32 accendere = VERUM;
                  i32 q;
        CensuraRegula r;
               chorda t;

        si (regulae.datum[p] == '+' || regulae.datum[p] == '-')
        {
            accendere = regulae.datum[p] == '+';
            p++;
        }
        q = p;
        dum (q < regulae.mensura && regulae.datum[q] != ',')
        {
            q++;
        }
        t.datum    = regulae.datum + p;
        t.mensura  = q - p;
        si (!censura_regulam_invenire(t, &r))
        {
            si (ignota != NIHIL)
            {
                *ignota = t;
            }
            redde FALSUM;
        }
        forma->activae[r]  = accendere;
        p                  = q < regulae.mensura ? q + I : q;
    }
    redde VERUM;
}

b32
censura_carminis_forma (
    constans Carmen* carmen,
       CensuraForma* forma,
             chorda* ignota)
{
    s32 catena[XVI];
    i32 n = ZEPHYRUM;
    s32 x = -I;
    i32 i;

    censura_ordinaria(forma);
    /* catena a stylo carminis ad basim ultimam (carmen eam iudicavit:
     * nota, non circularis) */
    si (   carmen->stylus.mensura > ZEPHYRUM
        && !chorda_aequalis_literis(carmen->stylus, "classicus"))
    {
        per (i = ZEPHYRUM; i < carmen->numerus_stilorum; i++)
        {
            si (chorda_aequalis(carmen->stili[i].titulus,
                carmen->stylus))
            {
                x = (s32)i;
                frange;
            }
        }
    }
    dum (x >= ZEPHYRUM && n < XVI)
    {
        constans CarmenStylus* s = &carmen->stili[x];

        catena[n++]  = x;
        x            = -I;
        si (   s->basis.mensura > ZEPHYRUM
            && !chorda_aequalis_literis(s->basis, "classicus"))
        {
            per (i = ZEPHYRUM; i < carmen->numerus_stilorum; i++)
            {
                si (chorda_aequalis(carmen->stili[i].titulus, s->basis))
                {
                    x = (s32)i;
                    frange;
                }
            }
        }
    }
    /* basis prior: a fine catenae ad stylum carminis */
    per (i = n; i > ZEPHYRUM; i--)
    {
        constans CarmenStylus* s = &carmen->stili[catena[i - I]];

        si (   s->regulae.mensura > ZEPHYRUM
            && !censura_regulas_legere(s->regulae, forma, ignota))
        {
            redde FALSUM;
        }
        si (s->septimae >= ZEPHYRUM)
        {
            forma->septimae_liberae = s->septimae == I;
        }
        si (s->intentiones >= ZEPHYRUM)
        {
            forma->intentiones_liberae = s->intentiones == I;
        }
    }
    redde carmen->censura_regulae.mensura == ZEPHYRUM
        || censura_regulas_legere(carmen->censura_regulae, forma,
                                  ignota);
}

b32
censura_ambitus_ordinarius (
     vacuum* ctx,
     chorda  instrumentum,
        i32* infima,
        i32* summa)
{
    i32 i;

    (vacuum)ctx;
    per (i = ZEPHYRUM; i < AMBITUS_NUMERUS; i++)
    {
        si (chorda_aequalis_literis(instrumentum,
            AMBITUS[i].instrumentum))
        {
            *infima  = AMBITUS[i].infima;
            *summa   = AMBITUS[i].summa;
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Loca
 * ================================================== */

/* mensura (1 = prima) et pulsus intra eam */
interior vacuum
_locus (
    constans Carmen* carmen,
                s64  pulsus,
                i32* mensura,
                s64* intra)
{
    i32 m = I;

    dum (carmen_pulsus_mensurae(carmen, m + I) <= pulsus)
    {
        m++;
    }
    *mensura  = m;
    *intra    = pulsus - carmen_pulsus_mensurae(carmen, m);
}

interior i32
_mensurae (
    constans Carmen* carmen)
{
    i32 m = I;

    dum (carmen_pulsus_mensurae(carmen, m + I) < carmen->pulsus)
    {
        m++;
    }
    redde m;
}

interior constans CarmenMetrum*
_metrum (
    constans Carmen* carmen,
                s64  pulsus)
{
    i32 i;
    i32 electum = ZEPHYRUM;

    per (i = ZEPHYRUM; i < carmen->numerus_metrorum; i++)
    {
        si (carmen->metra[i].pulsus <= pulsus)
        {
            electum = i;
        }
    }
    redde &carmen->metra[electum];
}

interior constans CarmenClavis*
_clavis (
    constans Carmen* carmen,
                s64  pulsus)
{
    i32 i;
    i32 electa = ZEPHYRUM;

    per (i = ZEPHYRUM; i < carmen->numerus_clavium; i++)
    {
        si (carmen->claves[i].pulsus <= pulsus)
        {
            electa = i;
        }
    }
    redde &carmen->claves[electa];
}

interior b32
_percussio (
    constans CarmenNota* nota)
{
    redde nota->praeparatum.mensura > ZEPHYRUM;
}

interior vacuum
_addere (
                  Iudex* iudex,
          CensuraRegula  regula,
    constans CarmenPars* pars,
                    s32  frustum,
                    s64  pulsus,     /* absolutus */
                    i32  offset,
    constans CarmenNota* nota,
    constans CarmenPars* altera,
    constans CarmenNota* nota_altera,
    constans CarmenNota* nota_prior,
    constans CarmenNota* nota_altera_prior)
{
    CensuraInventum* x;

    si (iudex->numerus < iudex->capacitas)
    {
        x = &iudex->inventa[iudex->numerus];
        memset(x, ZEPHYRUM, magnitudo(CensuraInventum));
        x->regula  = regula;
        x->pars    = pars->titulus;
        si (altera != NIHIL)
        {
            x->pars_altera = altera->titulus;
        }
        _locus(iudex->carmen, pulsus, &x->mensura, &x->pulsus);
        x->frustum            = frustum;
        x->offset             = offset;
        x->nota               = nota;
        x->nota_altera        = nota_altera;
        x->nota_prior         = nota_prior;
        x->nota_altera_prior  = nota_altera_prior;
    }
    iudex->numerus++;
}


/* ==================================================
 * Regulae
 * ================================================== */

interior vacuum
_lineae (
    Iudex* iudex)
{
    constans Carmen* k = iudex->carmen;
                i32  i;
                i32  f;
                i32  j;

    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        constans CarmenPars* p = &k->partes[i];

        per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
        {
            constans CarmenFrustum* fr = &p->frusta[f];

            per (j = ZEPHYRUM; j < fr->numerus_linearum; j++)
            {
                s64 pulsus = fr->pulsus + fr->lineae[j].pulsus;
                i32 m;
                s64 intra;

                _locus(k, pulsus, &m, &intra);
                si (intra != ZEPHYRUM)
                {
                    _addere(iudex, CENSURA_LINEAE, p, (s32)f, pulsus,
                        fr->lineae[j].offset, NIHIL, NIHIL, NIHIL,
                        NIHIL, NIHIL);
                }
            }
        }
    }
}

/* classis in clave: scala maior tonicae relativae; minor etiam VI et
 * VII elevati; caerulea etiam b3 b5 b7 tonicae */
interior b32
_in_clave (
    constans CarmenClavis* clavis,
                      s32  nota)
{
    s32 basis    = _modulus(VII * clavis->quintae, XII);   /* maior */
    s32 classis  = _modulus(nota, XII);
    i32 g;

    per (g = ZEPHYRUM; g < VII; g++)
    {
        si (_modulus(basis + SCALA[g], XII) == classis)
        {
            redde VERUM;
        }
    }
    si (clavis->caerulea)
    {
        s32 t = clavis->minor ? _modulus(basis + IX, XII) : basis;

        si (   classis == _modulus(t + III, XII)
            || classis == _modulus(t + VI, XII)
            || classis == _modulus(t + X, XII))
        {
            redde VERUM;
        }
    }
    /* minor: tonica = basis + IX; VI elevatus +IX, VII elevatus +XI */
    redde clavis->minor
        && (   classis == _modulus(basis + IX + IX, XII)
            || classis == _modulus(basis + IX + XI, XII));
}

/* index fenestrae explicationis cui pulsus pertinet (initium <=
 * pulsus < finis); -I si nulla aut explicatio nulla */
interior s32
_fenestra_ad_pulsum (
    constans Iudex* iudex,
               s64  pulsus)
{
    i32 f;

    si (iudex->explicatio == NIHIL)
    {
        redde -I;
    }
    per (f = ZEPHYRUM; f < iudex->explicatio->numerus_fenestrarum; f++)
    {
        constans ExplicatioFenestra* w =
            &iudex->explicatio->fenestrae[f];

        si (w->initium <= pulsus && pulsus < w->finis)
        {
            redde (s32)f;
        }
    }
    redde -I;
}

/* tertia Picardiae (functio.h): tertia maior finalis in clave minore,
 * in fenestra quam functio Picardiae iudicat - licita */
interior b32
_picardiae_nota (
                  Iudex* iudex,
    constans CarmenNota* nota,
                    s64  pulsus)
{
     constans CarmenClavis* clavis = _clavis(iudex->carmen, pulsus);
                       s32  tertia = _modulus(VII * clavis->quintae
                                     + IX + IV, XII);
                       s32 f       = _fenestra_ad_pulsum(iudex,
                                                          pulsus);

    si (   f < ZEPHYRUM || !clavis->minor
        || _modulus((s32)nota->nota, XII) != tertia)
    {
        redde FALSUM;
    }
    redde functio_tertia_picardiae(iudex->carmen, iudex->explicatio,
                                   (i32)f);
}

/* bits a radice (intentiones, evitandae) ad classes rotati */
interior i32
_rotare (
    i32 bits,
    i32 radix)
{
    redde ((bits << radix) | (bits >> (XII - radix))) & 0xFFF;
}

/* classis c tonus tensionis licitae acordi h (acordum.h intentiones) */
interior b32
_intentio (
    constans Acordum* h,
                 i32  c)
{
    redde h->genus != ACORDUM_IGNOTA && h->genus != ACORDUM_NULLA
        && (_rotare(acordum_proprietates(h->genus)->intentiones,
                    h->radix) & ((i32)I << c));
}

/* acordum declaratum (Carmen.acorda) ad pulsum; NIHIL nullum ('r'
 * classes nullas habet: nihil admittit) */
interior constans ComitatusAcordum*
_declaratum (
    constans Carmen* k,
                s64  pulsus)
{
    i32 i;

    per (i = ZEPHYRUM; i < k->numerus_acordorum; i++)
    {
        constans ComitatusAcordum* a = &k->acorda[i];

        si (a->pulsus <= pulsus && pulsus < a->pulsus + a->longitudo)
        {
            redde a;
        }
    }
    redde NIHIL;
}

/* nota acordi declarati ad initium suum (dominans applicata scripta);
 * intentionibus liberis etiam intentio eius (F7: Ab) */
interior b32
_declarati_nota (
                  Iudex* iudex,
    constans CarmenNota* nota,
                    s64  pulsus)
{
    constans ComitatusAcordum* a = _declaratum(iudex->carmen, pulsus);
                          i32  c = nota->nota % XII;

    redde a != NIHIL
        && (   (a->acordum.classes & ((i32)I << c))
            || (   iudex->forma->intentiones_liberae
                && _intentio(&a->acordum, c)));
}

/* ORNAMENTUM CHROMATICUM (censura.h clavis): segmentum notae ad
 * initium eius transitus, vicinus aut appoggiatura (semper gradu
 * exit - explicatio.h), semitonio exiens */
interior b32
_ornamentum_chromaticum (
                  Iudex* iudex,
    constans CarmenNota* nota)
{
    i32 i;

    si (iudex->explicatio == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < iudex->explicatio->numerus_segmentorum; i++)
    {
        constans ExplicatioSegmentum* s =
            &iudex->explicatio->segmenta[i];

        si (s->nota == nota && s->initium == s->pulsus)
        {
            redde s->semitonium
                && (   s->genus == EXPLICATIO_TRANSITUS
                    || s->genus == EXPLICATIO_VICINUS
                    || s->genus == EXPLICATIO_INNIXA);
        }
    }
    redde FALSUM;
}

interior vacuum
_clavis_regula (
    Iudex* iudex)
{
    constans Carmen* k = iudex->carmen;
                i32  i;
                i32  f;
                i32  j;

    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        constans CarmenPars* p = &k->partes[i];

        per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
        {
            constans CarmenFrustum* fr = &p->frusta[f];

            per (j = ZEPHYRUM; j < fr->numerus; j++)
            {
                constans CarmenNota* n       = &fr->notae[j];
                                s64  pulsus  = fr->pulsus + n->pulsus;

                si (   !_percussio(n)
                    && !_in_clave(_clavis(k, pulsus), (s32)n->nota)
                    && !_picardiae_nota(iudex, n, pulsus)
                    && !_declarati_nota(iudex, n, pulsus)
                    && !_ornamentum_chromaticum(iudex, n))
                {
                    _addere(iudex, CENSURA_CLAVIS, p, (s32)f, pulsus,
                        n->offset, n, NIHIL, NIHIL, NIHIL, NIHIL);
                }
            }
        }
    }
}

interior vacuum
_ambitus (
    Iudex* iudex)
{
    constans Carmen* k = iudex->carmen;
                i32  i;
                i32  f;
                i32  j;

    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        constans CarmenPars* p = &k->partes[i];
                        i32  infima;
                        i32  summa;
                        b32  notus;

        constans CarmenInstrumentum* ci = carmen_instrumentum(k,
            p->instrumentum);

        /* forma ordinaria: ambitus instrumenti libri si datus, aliter
         * tabula (praeparatum redefinitum sine ambitu: tabula) */
        si (   iudex->forma->ambitus == NIHIL && ci != NIHIL
            && (ci->infima != ZEPHYRUM || ci->summa != CXXVII))
        {
            notus   = VERUM;
            infima  = ci->infima;
            summa   = ci->summa;
        }
        alioquin
        {
            notus = iudex->forma->ambitus != NIHIL
                ? iudex->forma->ambitus(iudex->forma->ctx,
                                        p->instrumentum, &infima,
                                        &summa)
                : censura_ambitus_ordinarius(NIHIL, p->instrumentum,
                                             &infima, &summa);
        }
        si (!notus)
        {
            perge;
        }
        per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
        {
            constans CarmenFrustum* fr = &p->frusta[f];

            per (j = ZEPHYRUM; j < fr->numerus; j++)
            {
                constans CarmenNota* n = &fr->notae[j];

                si (   !_percussio(n)
                    && (n->nota < infima || n->nota > summa))
                {
                    _addere(iudex, CENSURA_AMBITUS, p, (s32)f,
                        fr->pulsus + n->pulsus, n->offset, n, NIHIL,
                        NIHIL, NIHIL, NIHIL);
                }
            }
        }
    }
}

/* notae partis sonantes ad pulsum (sine percussione), ad maximum
 * 'capacitas'; numerus redditur */
interior i32
_sonantes (
    constans CarmenPars* pars,
                    s64  pulsus,
             NotaLocata* sonantes,
                    i32  capacitas)
{
    i32 n = ZEPHYRUM;
    i32 f;
    i32 j;

    per (f = ZEPHYRUM; f < pars->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &pars->frusta[f];

        per (j = ZEPHYRUM; j < fr->numerus && n < capacitas; j++)
        {
            constans CarmenNota* x   = &fr->notae[j];
                            s64  ab  = fr->pulsus + x->pulsus;

            si (   !_percussio(x) && ab <= pulsus
                && ab + x->longitudo > pulsus)
            {
                sonantes[n].nota     = x;
                sonantes[n].frustum  = (s32)f;
                sonantes[n].initium  = ab;
                n++;
            }
        }
    }
    redde n;
}

/* secunda, septima, tritonus (classis intervalli) */
interior b32
_asperum (
    i32 a,
    i32 b)
{
    s32 d = _modulus((s32)a - (s32)b, XII);

    redde d == I || d == II || d == VI || d == X || d == XI;
}

/* tempora fortia mensurae in 'fortia' (absoluti); numerus. Initium
 * semper; 4/x tempus tertium; metra composita (tempus carminis maius
 * quam 1/denominator: tres octavae) tempus quodque */
interior i32
_fortia (
    constans Carmen* carmen,
                s64  initium,
                s64* fortia)
{
    constans CarmenMetrum* m = _metrum(carmen, initium);
                      s64  unitas = (s64)CANTUS_PULSUS_INTEGRAE
                                    / (s64)m->denominator;
                      s64 tempus = carmen_tempus_mensurae(carmen,
                                                           initium);
                      s64 longitudo = (s64)m->numerator * unitas;
                      s64 t;
                      i32 n = ZEPHYRUM;

    fortia[n++] = initium;
    si (m->numerator == IV)
    {
        fortia[n++] = initium + II * tempus;
    }
    alioquin si (tempus > unitas)
    {
        per (t = tempus; t < longitudo && n < XXII; t += tempus)
        {
            fortia[n++] = initium + t;
        }
    }
    redde n;
}

/* segmentum notae quod ad pulsum sonat (ultimum incipiens <=) */
interior constans ExplicatioSegmentum*
_segmentum (
       constans Explicatio* e,
       constans CarmenNota* nota,
                       s64  pulsus)
{
    constans ExplicatioSegmentum* s = NIHIL;
                             i32  i;

    per (i = ZEPHYRUM; i < e->numerus_segmentorum; i++)
    {
        si (   e->segmenta[i].nota    == nota
            && e->segmenta[i].initium <= pulsus)
        {
            s = &e->segmenta[i];
        }
    }
    redde s;
}

/* nota dissonantiam explicat: aliena explicata, aut resoluta */
interior b32
_explicat (
    constans ExplicatioSegmentum* s)
{
    commutatio (s->genus)
    {
        casus EXPLICATIO_TRANSITUS:
        casus EXPLICATIO_VICINUS:
        casus EXPLICATIO_INNIXA:
        casus EXPLICATIO_ANTICIPATIO:
        casus EXPLICATIO_EFFUGIENS:
            redde VERUM;
        casus EXPLICATIO_SUSPENSIO:
        casus EXPLICATIO_RETARDATIO:
        casus EXPLICATIO_SEPTIMA:
            redde s->resoluta;
        ordinarius:
            redde FALSUM;
    }
}

/* culpa notae (statica); NIHIL si nulla */
interior constans character*
_culpa (
    constans ExplicatioSegmentum* s,
                             b32  septimae_liberae)
{
    commutatio (s->genus)
    {
        casus EXPLICATIO_IGNOTA:
            redde "nota sine explicatione";
        casus EXPLICATIO_SUSPENSIO:
            redde "suspensio non resoluta";
        casus EXPLICATIO_RETARDATIO:
            redde "retardatio non resoluta";
        casus EXPLICATIO_SEPTIMA:
            /* stylus popularis: septima tonus acordi est */
            redde septimae_liberae ? NIHIL : "septima non resoluta";
        ordinarius:
            redde NIHIL;
    }
}

/* VERUM: dissonantia referenda (causa in *causa; NIHIL sine
 * explicatione - ut ante N2); FALSUM: explicata */
/* culpa segmenti sub forma: tensio licita (intentiones_liberae) nulla
 * quacumque figura (nona suspensa non resolvenda), ceterum _culpa */
interior constans character*
_culpa_notae (
                           Iudex* iudex,
    constans ExplicatioSegmentum* s)
{
    constans Acordum* h = &iudex->explicatio->fenestrae[s->fenestra]
                                .harmonia;

    si (   iudex->forma->intentiones_liberae
        && _intentio(h, s->nota->nota % XII))
    {
        redde NIHIL;
    }
    redde _culpa(s, iudex->forma->septimae_liberae);
}

interior b32
_referenda (
                  Iudex*  iudex,
    constans CarmenNota*  a,
    constans CarmenNota*  b,
                    s64   pulsus,
     constans character** causa)
{
    constans ExplicatioSegmentum* sa;
    constans ExplicatioSegmentum* sb;
                    AcordumGenus  g;

    *causa = NIHIL;
    si (iudex->explicatio == NIHIL)
    {
        redde VERUM;
    }
    sa = _segmentum(iudex->explicatio, a, pulsus);
    sb = _segmentum(iudex->explicatio, b, pulsus);
    si (sa == NIHIL || sb == NIHIL)
    {
        redde VERUM;
    }
    si (_explicat(sa) || _explicat(sb))
    {
        redde FALSUM;
    }
    /* stylus iazzicus: tensio licita acordi fenestrae culpa caret */
    *causa = _culpa_notae(iudex, sa) != NIHIL ? _culpa_notae(iudex, sa)
                                              : _culpa_notae(iudex, sb);
    si (*causa != NIHIL)
    {
        redde VERUM;
    }
    g = iudex->explicatio->fenestrae[sa->fenestra].harmonia.genus;
    si (g != ACORDUM_IGNOTA && g != ACORDUM_NULLA)
    {
        redde FALSUM;
    }
    *causa = "acordum sine nomine";
    redde VERUM;
}

interior vacuum
_dissonantia (
    Iudex* iudex)
{
    constans Carmen* k = iudex->carmen;
         NotaLocata  a[CENSURA_SIMUL];
         NotaLocata  b[CENSURA_SIMUL];
                s64  fortia[XXII];
                i32  m;

    per (m = I; m <= _mensurae(k); m++)
    {
        i32 numerus = _fortia(k, carmen_pulsus_mensurae(k, m), fortia);
        i32 t;

        per (t = ZEPHYRUM; t < numerus; t++)
        {
            i32 i;
            i32 j;

            per (i = ZEPHYRUM; i < k->numerus_partium; i++)
            {
                i32 na = _sonantes(&k->partes[i], fortia[t], a,
                    CENSURA_SIMUL);

                per (j = i + I; j < k->numerus_partium; j++)
                {
                    i32 nb = _sonantes(&k->partes[j], fortia[t], b,
                        CENSURA_SIMUL);
                    i32 x;
                    i32 y;

                    per (x = ZEPHYRUM; x < na; x++)
                    {
                        per (y = ZEPHYRUM; y < nb; y++)
                        {
                            constans character* causa;
                                           i32  n = iudex->numerus;

                            si (   _asperum(a[x].nota->nota,
                                       b[y].nota->nota)
                                && _referenda(iudex, a[x].nota,
                                       b[y].nota, fortia[t], &causa))
                            {
                                _addere(iudex, CENSURA_DISSONANTIA,
                                    &k->partes[i], a[x].frustum,
                                    fortia[t],
                                    a[x].nota->offset, a[x].nota,
                                    &k->partes[j], b[y].nota, NIHIL,
                                    NIHIL);
                                si (n < iudex->capacitas)
                                {
                                    iudex->inventa[n].causa = causa;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/* notae sonantes partium omnium ad pulsum, ordine altitudinis
 * (insertio stabilis); *pars_infimae (si non NIHIL) = pars notae
 * infimae, aequalibus prior ordine partium; numerus (ad
 * CENSURA_SONANTES) */
interior i32
_omnes_sonantes (
    constans Carmen* carmen,
                s64  pulsus,
         NotaLocata* omnes,
                i32* pars_infimae)
{
    NotaLocata a[CENSURA_SIMUL];
           i32 n = ZEPHYRUM;
           i32 i;
           i32 x;
           i32 y;

    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        i32 na = _sonantes(&carmen->partes[i], pulsus, a,
            CENSURA_SIMUL);

        per (x = ZEPHYRUM; x < na && n < CENSURA_SONANTES; x++)
        {
            i32 altitudo = a[x].nota->nota;

            si (   pars_infimae != NIHIL
                && (   n == ZEPHYRUM
                    || altitudo < omnes[ZEPHYRUM].nota->nota))
            {
                *pars_infimae = i;
            }
            /* insertio stabilis per altitudinem */
            y = n;
            dum (y > ZEPHYRUM && omnes[y - I].nota->nota > altitudo)
            {
                omnes[y] = omnes[y - I];
                y--;
            }
            omnes[y] = a[x];
            n++;
        }
    }
    redde n;
}

/* fenestra temporis acordum cum tertia nominat (post notas
 * alienas remotas: arpeggium E2 E3 B3, G4 = Em). Sub sono >= III
 * notarum fenestra numquam vacua est: genus non inspicitur */
interior b32
_fenestra_cum_tertia (
    constans Iudex* iudex,
               s64  pulsus)
{
     constans Acordum* h;
                  s32  f = _fenestra_ad_pulsum(iudex, pulsus);
                  i32  radix;

    si (f < ZEPHYRUM)
    {
        redde FALSUM;
    }
    h = &iudex->explicatio->fenestrae[f].harmonia;
    redde !acordum_sine_tertia(h->classes, &radix);
}

interior vacuum
_tertia (
    Iudex* iudex)
{
    constans Carmen* k = iudex->carmen;
         NotaLocata  omnes[CENSURA_SONANTES];
                s64  fortia[XXII];
                s64  prius = -I;   /* tempus forte prius */
                i32  m;

    per (m = I; m <= _mensurae(k); m++)
    {
        i32 numerus = _fortia(k, carmen_pulsus_mensurae(k, m), fortia);
        i32 t;

        per (t = ZEPHYRUM; t < numerus; t++)
        {
            i32 pars     = ZEPHYRUM;
            i32 n        = _omnes_sonantes(k, fortia[t], omnes, &pars);
            i32 classes  = ZEPHYRUM;
            b32 novum    = FALSUM;
            i32 radix;
            i32 x;

            per (x = ZEPHYRUM; x < n; x++)
            {
                classes  |= (i32)I << (omnes[x].nota->nota % XII);
                novum    = novum || omnes[x].initium > prius;
            }
            prius = fortia[t];
            si (   n >= III && novum
                && acordum_sine_tertia(classes, &radix)
                && !_fenestra_cum_tertia(iudex, fortia[t]))
            {
                _addere(iudex, CENSURA_TERTIA, &k->partes[pars],
                    omnes[ZEPHYRUM].frustum, fortia[t],
                    omnes[ZEPHYRUM].nota->offset, omnes[ZEPHYRUM].nota,
                    NIHIL, NIHIL, NIHIL, NIHIL);
            }
        }
    }
}

/* nota summa sonans partis; nota NIHIL si nulla */
interior NotaLocata
_summa (
    constans CarmenPars* pars,
                    s64  pulsus)
{
    NotaLocata sonantes[CENSURA_SIMUL];
    NotaLocata electa;
           i32 n = _sonantes(pars, pulsus, sonantes, CENSURA_SIMUL);
           i32 i;

    electa.nota     = NIHIL;
    electa.frustum  = -I;
    electa.initium  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (   electa.nota == NIHIL || sonantes[i].nota->nota
            > electa.nota->nota)
        {
            electa = sonantes[i];
        }
    }
    redde electa;
}

/* initium proximum post 'post' in parte (sine percussione); -1 =
 * nullum */
interior s64
_initium_proximum (
    constans CarmenPars* pars,
                    s64  post)
{
    s64 proximum = -I;
    i32 f;
    i32 j;

    per (f = ZEPHYRUM; f < pars->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &pars->frusta[f];

        per (j = ZEPHYRUM; j < fr->numerus; j++)
        {
            s64 ab = fr->pulsus + fr->notae[j].pulsus;

            si (   !_percussio(&fr->notae[j]) && ab > post
                && (proximum < ZEPHYRUM || ab < proximum))
            {
                proximum = ab;
            }
        }
    }
    redde proximum;
}

interior vacuum
_parallelae (
    Iudex* iudex)
{
    constans Carmen* k = iudex->carmen;
                i32  i;
                i32  j;

    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        per (j = i + I; j < k->numerus_partium; j++)
        {
            constans CarmenPars* p = &k->partes[i];
            constans CarmenPars* q = &k->partes[j];
                     NotaLocata  a_prior;
                     NotaLocata  b_prior;
                            s64  t = -I;

            a_prior.nota = NIHIL;
            b_prior.nota = NIHIL;
            per (;;)
            {
                       s64 proximum_a = _initium_proximum(p, t);
                       s64 proximum_b = _initium_proximum(q, t);
                NotaLocata a;
                NotaLocata b;

                si (proximum_a < ZEPHYRUM && proximum_b < ZEPHYRUM)
                {
                    frange;
                }
                t = proximum_a < ZEPHYRUM ? proximum_b
                    : proximum_b < ZEPHYRUM ? proximum_a
                    : proximum_a < proximum_b ? proximum_a : proximum_b;
                a = _summa(p, t);
                b = _summa(q, t);
                /* ambae motae eodem sensu: nota tenta aut repetita
                 * (motus 0) non est parallela */
                si (   a.nota       != NIHIL && b.nota != NIHIL
                    && a_prior.nota != NIHIL && b_prior.nota != NIHIL)
                {
                    s32 da = (s32)a.nota->nota
                        - (s32)a_prior.nota->nota;
                    s32 db = (s32)b.nota->nota
                        - (s32)b_prior.nota->nota;
                    s32 iv = _modulus((s32)a.nota->nota
                        - (s32)b.nota->nota, XII);
                    s32 ip = _modulus((s32)a_prior.nota->nota
                        - (s32)b_prior.nota->nota, XII);

                    si (   ((da > ZEPHYRUM && db > ZEPHYRUM)
                            || (da < ZEPHYRUM && db < ZEPHYRUM))
                        && iv == ip && (iv == ZEPHYRUM || iv == VII))
                    {
                        _addere(iudex, CENSURA_PARALLELAE, p, a.frustum,
                            t,
                            a.nota->offset, a.nota, q, b.nota,
                            a_prior.nota, b_prior.nota);
                    }
                }
                a_prior = a;
                b_prior = b;
            }
        }
    }
}


/* ==================================================
 * Voces: occultae, decussatio, spatium
 * ================================================== */

/* nota ima sonans partis; nota NIHIL si nulla */
interior NotaLocata
_infima (
    constans CarmenPars* pars,
                    s64  pulsus)
{
    NotaLocata sonantes[CENSURA_SIMUL];
    NotaLocata electa;
           i32 n = _sonantes(pars, pulsus, sonantes, CENSURA_SIMUL);
           i32 i;

    electa.nota     = NIHIL;
    electa.frustum  = -I;
    electa.initium  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (   electa.nota == NIHIL || sonantes[i].nota->nota
            < electa.nota->nota)
        {
            electa = sonantes[i];
        }
    }
    redde electa;
}

/* vox: pars cum nota aliqua sine percussione */
interior b32
_vox_est (
    constans CarmenPars* pars)
{
    redde _initium_proximum(pars, -I) >= ZEPHYRUM;
}

/* vox proxima post partem i (ordine libri; -1: prima); -1 nulla */
interior s32
_vox_proxima (
    constans Carmen* carmen,
                s32  i)
{
    per (i = i + I; i < (s32)carmen->numerus_partium; i++)
    {
        si (_vox_est(&carmen->partes[i]))
        {
            redde i;
        }
    }
    redde -I;
}

/* initium proximum post 'post' in parte alterutra; -1 nullum */
interior s64
_initium_par (
    constans CarmenPars* p,
    constans CarmenPars* q,
                    s64  post)
{
    s64 a = _initium_proximum(p, post);
    s64 b = _initium_proximum(q, post);

    redde a < ZEPHYRUM ? b : b < ZEPHYRUM ? a : a < b ? a : b;
}

/* occultae: voces extremae ad quintam aut octavam eodem sensu, non ex
 * ea, superior saltu */
interior vacuum
_occultae (
    Iudex* iudex)
{
    constans Carmen* k       = iudex->carmen;
                s32  summa   = _vox_proxima(k, -I);
                s32  ultima  = summa;
                s32  i;
         NotaLocata  a_prior;
         NotaLocata  b_prior;
                s64  t      = -I;

    per (i = summa; i >= ZEPHYRUM; i = _vox_proxima(k, i))
    {
        ultima = i;
    }
    si (summa < ZEPHYRUM || ultima == summa)
    {
        redde;
    }
    a_prior.nota = NIHIL;
    b_prior.nota = NIHIL;
    dum ((t = _initium_par(&k->partes[summa], &k->partes[ultima], t))
         >= ZEPHYRUM)
    {
        NotaLocata a = _summa(&k->partes[summa], t);
        NotaLocata b = _infima(&k->partes[ultima], t);

        si (   a.nota       != NIHIL && b.nota != NIHIL
            && a_prior.nota != NIHIL && b_prior.nota != NIHIL)
        {
            s32 da = (s32)a.nota->nota - (s32)a_prior.nota->nota;
            s32 db = (s32)b.nota->nota - (s32)b_prior.nota->nota;
            s32 iv = _modulus((s32)a.nota->nota - (s32)b.nota->nota,
                              XII);
            s32 ip = _modulus((s32)a_prior.nota->nota
                              - (s32)b_prior.nota->nota, XII);

            si (   ((da > ZEPHYRUM && db > ZEPHYRUM)
                    || (da < ZEPHYRUM && db < ZEPHYRUM))
                && (iv == ZEPHYRUM || iv == VII) && iv != ip
                && (da > II || da < -II))
            {
                _addere(iudex, CENSURA_OCCULTAE, &k->partes[summa],
                    a.frustum, t, a.nota->offset, a.nota,
                    &k->partes[ultima], b.nota, a_prior.nota,
                    b_prior.nota);
            }
        }
        a_prior = a;
        b_prior = b;
    }
}

/* nota sonat usque ad pulsum t (finis >= t) */
interior b32
_sonat_usque (
    NotaLocata nota,
           s64 t)
{
    redde nota.nota != NIHIL
        && nota.initium + nota.nota->longitudo >= t;
}

/* par vocum proximarum per initia: decussatio (superioris infima
 * infra inferioris summam) aut spatium (plus quam XII); semel per
 * spatium temporis continuum quo valet - pausa vocis alterutrius id
 * finit */
interior vacuum
_vocum_par (
                  Iudex* iudex,
          CensuraRegula  regula,
    constans CarmenPars* p,
    constans CarmenPars* q)
{
           s64 t      = -I;
           b32 valet  = FALSUM;
    NotaLocata a_prior;
    NotaLocata b_prior;

    a_prior.nota = NIHIL;
    b_prior.nota = NIHIL;
    dum ((t = _initium_par(p, q, t)) >= ZEPHYRUM)
    {
        NotaLocata a = _infima(p, t);
        NotaLocata b = _summa(q, t);
               b32 nunc;

        nunc = a.nota != NIHIL && b.nota != NIHIL
            && (regula == CENSURA_DECUSSATIO
                ? a.nota->nota < b.nota->nota
                : (s32)a.nota->nota - (s32)b.nota->nota > (s32)XII);
        si (   nunc
            && !(   valet && _sonat_usque(a_prior, t)
                 && _sonat_usque(b_prior, t)))
        {
            _addere(iudex, regula, p, a.frustum, t, a.nota->offset,
                a.nota, q, b.nota, NIHIL, NIHIL);
        }
        valet    = nunc;
        a_prior  = a;
        b_prior  = b;
    }
}

/* decussatio: omnia paria proxima; spatium: paria sine voce ima */
interior vacuum
_voces_iudicare (
            Iudex* iudex,
    CensuraRegula  regula)
{
    constans Carmen* k = iudex->carmen;
                s32  i = _vox_proxima(k, -I);
                s32  j;

    dum (i >= ZEPHYRUM && (j = _vox_proxima(k, i)) >= ZEPHYRUM)
    {
        si (   regula             == CENSURA_DECUSSATIO
            || _vox_proxima(k, j) >= ZEPHYRUM)
        {
            _vocum_par(iudex, regula, &k->partes[i], &k->partes[j]);
        }
        i = j;
    }
}

/* notae sequentes partis (initio diverso; acervus: nota ultima eius),
 * sine percussione; trans pausas quoque */
interior vacuum
_saltus (
    Iudex* iudex)
{
    constans Carmen* k = iudex->carmen;
                i32  maximus = iudex->forma->saltus_maximus > ZEPHYRUM
                    ? iudex->forma->saltus_maximus : XII;
                i32 i;
                i32 f;
                i32 j;

    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        constans CarmenPars* p                = &k->partes[i];
        constans CarmenNota* prior            = NIHIL;
                        s64  initium_prioris  = -I;

        per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
        {
            constans CarmenFrustum* fr = &p->frusta[f];

            per (j = ZEPHYRUM; j < fr->numerus; j++)
            {
                constans CarmenNota* n   = &fr->notae[j];
                                s64  ab  = fr->pulsus + n->pulsus;
                                s32  d;

                si (_percussio(n))
                {
                    perge;
                }
                d = (s32)n->nota - (s32)(prior != NIHIL ? prior->nota
                                                        : n->nota);
                d = d < ZEPHYRUM ? -d : d;
                si (   prior != NIHIL && ab != initium_prioris
                    && d > (s32)maximus)
                {
                    _addere(iudex, CENSURA_SALTUS, p, (s32)f, ab,
                        n->offset, n, NIHIL, NIHIL, prior, NIHIL);
                }
                initium_prioris  = ab;
                prior            = n;
            }
        }
    }
}

/* index frusti partis quod notam continet; -I nullum */
interior s32
_frustum_notae (
    constans CarmenPars* p,
    constans CarmenNota* nota)
{
    i32 f;

    per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &p->frusta[f];

        si (   fr->numerus > ZEPHYRUM && nota >= fr->notae
            && nota < fr->notae + fr->numerus)
        {
            redde (s32)f;
        }
    }
    redde -I;
}

/* acorda: fenestra declarata cum nota sine forma extra acordum, aut
 * radice / tertia declarata nusquam sonante - semel per fenestram */
interior vacuum
_acorda_regula (
    Iudex* iudex)
{
        constans Carmen* k = iudex->carmen;
    constans Explicatio* e = iudex->explicatio;
                    i32  f;

    si (e == NIHIL)
    {
        redde;
    }
    per (f = ZEPHYRUM; f < e->numerus_fenestrarum; f++)
    {
          constans ExplicatioFenestra* w = &e->fenestrae[f];
         constans ExplicatioSegmentum* alienae[II];
         constans ExplicatioSegmentum* infima   = NIHIL;
         constans ExplicatioSegmentum* s;
                     constans Acordum* d;
                                  i32  classes = ZEPHYRUM;
                                  i32  numerus = ZEPHYRUM;
                                  i32  n;
                                  i32  i;
                   constans character* causa    = NIHIL;
                                  s32  tertia;

        si (!w->declaratum)
        {
            perge;
        }
        d = &w->harmonia;
        per (i = ZEPHYRUM; i < e->numerus_segmentorum; i++)
        {
            i32 c;

            s = &e->segmenta[i];
            si ((i32)s->fenestra != f)
            {
                perge;
            }
            c        = s->nota->nota % XII;
            classes  |= (i32)I << c;
            si (   infima == NIHIL
                || (   s->initium == w->initium
                    && (   infima->initium != w->initium
                        || s->nota->nota < infima->nota->nota)))
            {
                infima = s;
            }
            /* ignota = extra classes declaratas, sine forma; tensio
             * licita sub stylo iazzico non */
            si (   s->genus == EXPLICATIO_IGNOTA && numerus < II
                && !(   iudex->forma->intentiones_liberae
                     && _intentio(d, c)))
            {
                alienae[numerus++] = s;
            }
        }
        si (infima == NIHIL)
        {
            perge;
        }
        tertia = acordum_proprietates(d->genus) != NIHIL
               ? acordum_proprietates(d->genus)->tertia : -I;
        si (numerus == ZEPHYRUM)
        {
            si (!(classes & ((i32)I << d->radix)))
            {
                causa = "radix deest";
            }
            alioquin si (   tertia >= ZEPHYRUM
                         && !(classes & ((i32)I
                              << ((d->radix + (i32)tertia) % XII))))
            {
                causa = "tertia deest";
            }
            alioquin
            {
                perge;
            }
        }
        n = iudex->numerus;
        s = numerus > ZEPHYRUM ? alienae[ZEPHYRUM] : infima;
        _addere(iudex, CENSURA_ACORDA, &k->partes[s->pars],
            _frustum_notae(&k->partes[s->pars], s->nota), w->initium,
            s->nota->offset, s->nota, NIHIL,
            NIHIL, numerus > I ? alienae[I]->nota : NIHIL, NIHIL);
        si (n < iudex->capacitas)
        {
            iudex->inventa[n].causa   = causa;
            iudex->inventa[n].sonans  = acordum_ex_classibus(classes,
                infima->nota->nota % XII);
        }
    }
}

/* evitanda: nota evitanda acordi fenestrae tenta (sine forma) */
interior vacuum
_evitanda_regula (
    Iudex* iudex)
{
        constans Carmen* k = iudex->carmen;
    constans Explicatio* e = iudex->explicatio;
                    i32  i;

    si (e == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < e->numerus_segmentorum; i++)
    {
        constans ExplicatioSegmentum* s = &e->segmenta[i];
                    constans Acordum* h =
                        &e->fenestrae[s->fenestra].harmonia;
                                  i32 n = iudex->numerus;

        si (   s->genus != EXPLICATIO_IGNOTA
            || h->genus == ACORDUM_IGNOTA || h->genus == ACORDUM_NULLA
            || !(_rotare(acordum_proprietates(h->genus)->evitandae,
                         h->radix) & ((i32)I << (s->nota->nota % XII))))
        {
            perge;
        }
        _addere(iudex, CENSURA_EVITANDA, &k->partes[s->pars],
            _frustum_notae(&k->partes[s->pars], s->nota), s->initium,
            s->nota->offset, s->nota, NIHIL, NIHIL, NIHIL, NIHIL);
        si (n < iudex->capacitas)
        {
            iudex->inventa[n].sonans = *h;
        }
    }
}

i32
censura_iudicare (
           constans Carmen* carmen,
     constans CensuraForma* forma,
           CensuraInventum* inventa,
                       i32  capacitas)
{
    CensuraForma  ordinaria;
           Iudex  iudex;
      Explicatio  explicatio;
         Piscina* piscina = NIHIL;
             i32  x;
             i32  y;

    si (forma == NIHIL)
    {
        censura_ordinaria(&ordinaria);
        forma = &ordinaria;
    }
    iudex.carmen      = carmen;
    iudex.forma       = forma;
    iudex.inventa     = inventa;
    iudex.capacitas   = capacitas;
    iudex.numerus     = ZEPHYRUM;
    iudex.explicatio  = NIHIL;
    si (   forma->activae[CENSURA_DISSONANTIA]
        || forma->activae[CENSURA_CLAVIS]
        || forma->activae[CENSURA_TERTIA]
        || forma->activae[CENSURA_ACORDA]
        || forma->activae[CENSURA_EVITANDA])
    {
        /* explicatio in piscina propria: inventa notas carminis et
         * causas staticas tenent, nihil ex ea (clavis: tertia
         * Picardiae per fenestras; tertia: acordum fenestrae) */
        piscina = piscina_generare_dynamicum("censura", M * XVI);
        si (   piscina != NIHIL
            && explicatio_carminis(carmen, piscina, &explicatio))
        {
            iudex.explicatio = &explicatio;
        }
    }
    si (forma->activae[CENSURA_LINEAE])
    {
        _lineae(&iudex);
    }
    si (forma->activae[CENSURA_CLAVIS])
    {
        _clavis_regula(&iudex);
    }
    si (forma->activae[CENSURA_AMBITUS])
    {
        _ambitus(&iudex);
    }
    si (forma->activae[CENSURA_DISSONANTIA])
    {
        _dissonantia(&iudex);
    }
    si (forma->activae[CENSURA_PARALLELAE])
    {
        _parallelae(&iudex);
    }
    si (forma->activae[CENSURA_SALTUS])
    {
        _saltus(&iudex);
    }
    si (forma->activae[CENSURA_TERTIA])
    {
        _tertia(&iudex);
    }
    si (forma->activae[CENSURA_ACORDA])
    {
        _acorda_regula(&iudex);
    }
    si (forma->activae[CENSURA_EVITANDA])
    {
        _evitanda_regula(&iudex);
    }
    si (forma->activae[CENSURA_OCCULTAE])
    {
        _occultae(&iudex);
    }
    si (forma->activae[CENSURA_DECUSSATIO])
    {
        _voces_iudicare(&iudex, CENSURA_DECUSSATIO);
    }
    si (forma->activae[CENSURA_SPATIUM])
    {
        _voces_iudicare(&iudex, CENSURA_SPATIUM);
    }
    /* ordinatio stabilis (insertio) per mensuram, pulsum */
    x = iudex.numerus < capacitas ? iudex.numerus : capacitas;
    per (y = I; y < x; y++)
    {
        CensuraInventum t = inventa[y];
                    i32 z = y;

        dum (   z > ZEPHYRUM
             && (   inventa[z - I].mensura > t.mensura
                 || (   inventa[z - I].mensura == t.mensura
                     && inventa[z - I].pulsus > t.pulsus)))
        {
            inventa[z] = inventa[z - I];
            z--;
        }
        inventa[z] = t;
    }
    si (piscina != NIHIL)
    {
        piscina_destruere(piscina);
    }
    redde iudex.numerus;
}


/* ==================================================
 * Scriptio
 * ================================================== */

interior chorda
_instrumentum (
    constans Carmen* carmen,
             chorda  pars)
{
    i32 i;

    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        si (chorda_aequalis(carmen->partes[i].titulus, pars))
        {
            redde carmen->partes[i].instrumentum;
        }
    }
    redde pars;   /* non attingitur */
}

b32
censura_inventum_scribere (
             constans Carmen* carmen,
    constans CensuraInventum* inventum,
           ChordaAedificator* textus)
{
    constans CensuraInventum* x = inventum;
                         s64  pulsus;
                         b32  bene;

    pulsus = carmen_pulsus_mensurae(carmen, x->mensura) + x->pulsus;
    bene = chorda_aedificator_appendere_literis(textus, "mensura ");
    bene = chorda_aedificator_appendere_i32(textus, x->mensura) && bene;
    si (x->pulsus > ZEPHYRUM)
    {
        bene = chorda_aedificator_appendere_literis(textus, " +")
            && bene;
        bene = chorda_aedificator_appendere_s32(textus, (s32)x->pulsus)
            && bene;
    }
    bene = chorda_aedificator_appendere_character(textus, ' ') && bene;
    bene = chorda_aedificator_appendere_chorda(textus, x->pars) && bene;
    si (x->pars_altera.mensura > ZEPHYRUM)
    {
        bene = chorda_aedificator_appendere_character(textus, '/')
            && bene;
        bene = chorda_aedificator_appendere_chorda(textus,
            x->pars_altera)
            && bene;
    }
    bene = chorda_aedificator_appendere_literis(textus, ": ") && bene;
    commutatio (x->regula)
    {
        casus CENSURA_LINEAE:
            bene = chorda_aedificator_appendere_literis(textus,
                "'|' non in linea mensurae") && bene;
            frange;
        casus CENSURA_CLAVIS:
        {
            constans CarmenClavis* k = _clavis(carmen, pulsus);

            bene = partitura_notam_scribere(carmen, x->nota, pulsus,
                textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                " extra clavem ") && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                partitura_clavis_littera(k)) && bene;
            bene = chorda_aedificator_appendere_character(textus, ' ')
                && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                partitura_clavis_modus(k)) && bene;
            frange;
        }
        casus CENSURA_AMBITUS:
            bene = partitura_notam_scribere(carmen, x->nota, pulsus,
                textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                " extra ambitum ") && bene;
            bene = chorda_aedificator_appendere_chorda(textus,
                _instrumentum(carmen, x->pars)) && bene;
            frange;
        casus CENSURA_DISSONANTIA:
        {
            /* ab altitudine absoluta: pars inferior prima septimam in
             * 'secundam' verteret */
            s32 d = (s32)x->nota->nota - (s32)x->nota_altera->nota;

            d = _modulus(d < ZEPHYRUM ? -d : d, XII);

            bene = partitura_notam_scribere(carmen, x->nota, pulsus,
                textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                " contra ")
                && bene;
            bene = partitura_notam_scribere(carmen, x->nota_altera,
                pulsus, textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                d == VI ? ", tritonus" : d <= II ? ", secunda"
                                                 : ", septima") && bene;
            si (x->causa != NIHIL)
            {
                bene = chorda_aedificator_appendere_literis(textus,
                    " - ") && bene;
                bene = chorda_aedificator_appendere_literis(textus,
                    x->causa) && bene;
            }
            frange;
        }
        casus CENSURA_PARALLELAE:
        casus CENSURA_OCCULTAE:
        {
            s32 d = _modulus((s32)x->nota->nota
                - (s32)x->nota_altera->nota, XII);

            bene = chorda_aedificator_appendere_literis(textus,
                d == VII ? "quintae " : "octavae ") && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                x->regula == CENSURA_OCCULTAE ? "occultae "
                                              : "parallelae ") && bene;
            si (x->nota_prior != NIHIL && x->nota_altera_prior != NIHIL)
            {
                bene = partitura_notam_scribere(carmen, x->nota_prior,
                    pulsus, textus) && bene;
                bene = chorda_aedificator_appendere_character(textus,
                    '/')
                    && bene;
                bene = partitura_notam_scribere(carmen,
                    x->nota_altera_prior, pulsus, textus) && bene;
                bene = chorda_aedificator_appendere_literis(textus,
                    " -> ")
                    && bene;
            }
            bene = partitura_notam_scribere(carmen, x->nota, pulsus,
                textus) && bene;
            bene = chorda_aedificator_appendere_character(textus, '/')
                && bene;
            bene = partitura_notam_scribere(carmen, x->nota_altera,
                pulsus, textus) && bene;
            frange;
        }
        casus CENSURA_DECUSSATIO:
            bene = chorda_aedificator_appendere_literis(textus,
                "decussatio ") && bene;
            bene = partitura_notam_scribere(carmen, x->nota, pulsus,
                textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                " infra ") && bene;
            bene = partitura_notam_scribere(carmen, x->nota_altera,
                pulsus, textus) && bene;
            frange;
        casus CENSURA_SPATIUM:
            bene = chorda_aedificator_appendere_literis(textus,
                "spatium ") && bene;
            bene = partitura_notam_scribere(carmen, x->nota, pulsus,
                textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus, " / ")
                && bene;
            bene = partitura_notam_scribere(carmen, x->nota_altera,
                pulsus, textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus, ", ")
                && bene;
            bene = chorda_aedificator_appendere_s32(textus,
                (s32)x->nota->nota - (s32)x->nota_altera->nota) && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                " semitonia") && bene;
            frange;
        casus CENSURA_SALTUS:
        {
            s32 d = (s32)x->nota->nota - (s32)x->nota_prior->nota;

            bene = chorda_aedificator_appendere_literis(textus,
                "saltus ")
                && bene;
            bene = partitura_notam_scribere(carmen, x->nota_prior,
                pulsus, textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus, " -> ")
                && bene;
            bene = partitura_notam_scribere(carmen, x->nota, pulsus,
                textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus, ", ")
                && bene;
            bene = chorda_aedificator_appendere_s32(textus,
                d < ZEPHYRUM ? -d : d) && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                " semitonia") && bene;
            frange;
        }
        casus CENSURA_ACORDA:
        {
            constans ComitatusAcordum* a = _declaratum(carmen, pulsus);

            si (a != NIHIL)
            {
                bene = harmonia_scribere(carmen, &a->acordum, pulsus,
                    textus) && bene;
            }
            bene = chorda_aedificator_appendere_literis(textus,
                " declaratum, sonat ") && bene;
            bene = harmonia_scribere(carmen, &x->sonans, pulsus, textus)
                && bene;
            bene = chorda_aedificator_appendere_literis(textus, " (")
                && bene;
            si (x->causa != NIHIL)
            {
                bene = chorda_aedificator_appendere_literis(textus,
                    x->causa) && bene;
            }
            alioquin
            {
                bene = partitura_notam_scribere(carmen, x->nota, pulsus,
                    textus) && bene;
                si (x->nota_prior != NIHIL)
                {
                    bene = chorda_aedificator_appendere_character(
                        textus, ' ') && bene;
                    bene = partitura_notam_scribere(carmen,
                        x->nota_prior, pulsus, textus) && bene;
                }
            }
            bene = chorda_aedificator_appendere_character(textus, ')')
                && bene;
            frange;
        }
        casus CENSURA_EVITANDA:
        {
            s32 iv = _modulus((s32)(x->nota->nota % XII)
                              - (s32)x->sonans.radix, XII);

            bene = harmonia_scribere(carmen, &x->sonans, pulsus, textus)
                && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                ": nota evitanda ") && bene;
            bene = partitura_notam_scribere(carmen, x->nota, pulsus,
                textus) && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                iv == V ? " (11)" : iv == IV ? " (3)" : " (?)") && bene;
            frange;
        }
        casus CENSURA_TERTIA:
        {
            NotaLocata omnes[CENSURA_SONANTES];
                   i32 n = _omnes_sonantes(carmen, pulsus, omnes,
                       NIHIL);
                   i32 j;

            bene = chorda_aedificator_appendere_literis(textus,
                "voces ") && bene;
            bene = chorda_aedificator_appendere_i32(textus, n) && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                " sine tertia:") && bene;
            per (j = ZEPHYRUM; j < n; j++)
            {
                bene = chorda_aedificator_appendere_character(textus,
                    ' ')
                    && bene;
                bene = partitura_notam_scribere(carmen, omnes[j].nota,
                    pulsus, textus) && bene;
            }
            frange;
        }
        ordinarius:
            frange;
    }
    si (x->frustum >= ZEPHYRUM)
    {
        bene = chorda_aedificator_appendere_literis(textus,
            " (frustum ")
            && bene;
        bene = chorda_aedificator_appendere_s32(textus, x->frustum)
            && bene;
        bene = chorda_aedificator_appendere_literis(textus, ", offset ")
            && bene;
        bene = chorda_aedificator_appendere_i32(textus, x->offset)
            && bene;
        bene = chorda_aedificator_appendere_character(textus, ')')
            && bene;
    }
    redde chorda_aedificator_appendere_character(textus, '\n') && bene;
}
