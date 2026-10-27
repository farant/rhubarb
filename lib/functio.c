/* functio.c - Functio harmonica (vide include/functio.h)
 *
 * PER FENESTRAM: clavis ad initium -> tonica T et scala (maior; minor
 * naturalis cum VI et VII elevatis). Radix in scala -> gradus; extra
 * scalam -> gradus supra eam deminutus ('b'). Applicata si acordum
 * extra scalam est et forma dominantis (maior, septima; diminuta /
 * diminuta septima ut viio) gradum diatonicum non tonicum, non
 * diminutum, tangit. Cadentia ex fenestra priore contigua. Clavis per
 * fenestram: _clavis_fenestrae (dominans praevia in clave proxima) -
 * gradus, clausulae, picardia eadem legunt.
 */

#include "functio.h"
#include "partitura.h"
#include <string.h>

/* gradus scalae a tonica */
hic_manens constans s32 MAIOR[VII] = { 0, 2, 4, 5, 7, 9, 11 };
hic_manens constans s32 MINOR[VII] = { 0, 2, 3, 5, 7, 8, 10 };

hic_manens constans character* ROMANI[VIII] = {
    "", "I", "II", "III", "IV", "V", "VI", "VII"
};

hic_manens constans character* CLAUSULAE[VI] = {
    NIHIL, "perfecta", "imperfecta", "dimidia", "plagalis", "deceptiva"
};

hic_manens constans character* ABBREVIATAE[VI] = {
    "", "perf.", "imperf.", "dimid.", "plag.", "decept."
};

interior s32
_modulus (
    s32 a,
    s32 m)
{
    redde ((a % m) + m) % m;
}

/* clavis ad pulsum */
nomen structura {
    s32 tonica;
    b32 minor;
} Clavis;

interior Clavis
_clavis (
    constans Carmen* carmen,
                s64  pulsus)
{
    Clavis c;
       s32 q = ZEPHYRUM;
       i32 i;

    c.minor = FALSUM;
    per (i = ZEPHYRUM; i < carmen->numerus_clavium; i++)
    {
        si (carmen->claves[i].pulsus <= pulsus)
        {
            q        = carmen->claves[i].quintae;
            c.minor  = carmen->claves[i].minor;
        }
    }
    c.tonica = _modulus(VII * q, XII);
    si (c.minor)
    {
        c.tonica = _modulus(c.tonica + IX, XII);
    }
    redde c;
}

/* forma dominantis: trias maior, septima, nonae, 13, 7#9, 7#11 (sine
 * dyade tertiae - vocantes eam addunt ubi licet) */
interior b32
_forma_dominantis (
    constans Acordum* h)
{
    redde h->genus == ACORDUM_MAIOR || h->genus == ACORDUM_SEPTIMA
        || h->genus == ACORDUM_NONA || h->genus == ACORDUM_NONA_MINOR
        || h->genus == ACORDUM_TERTIA_DECIMA
        || h->genus == ACORDUM_NONA_AUCTA
        || h->genus == ACORDUM_UNDECIMA_AUCTA;
}

/* acordum dominans clavis: forma dominantis in quinta tonicae, aut
 * diminuta semitonio infra eam */
interior b32
_dominans_clavis (
     constans Clavis* k,
    constans Acordum* h)
{
    s32 iv = _modulus((s32)h->radix - k->tonica, XII);

    si (_forma_dominantis(h) || h->genus == ACORDUM_TERTIA_MAIOR)
    {
        redde iv == VII;
    }
    si (   h->genus == ACORDUM_DIMINUTA
        || h->genus == ACORDUM_DIMINUTA_SEPTIMA
        || h->genus == ACORDUM_DIMIDIA_DIMINUTA)
    {
        redde iv == XI;
    }
    redde FALSUM;
}

/* fenestra sonans proxima post f (pausae transiliuntur);
 * numerus_fenestrarum si nulla */
interior i32
_proxima_sonans (
    constans Explicatio* e,
                    i32  f)
{
    i32 g = f + I;

    dum (   g < e->numerus_fenestrarum
         && e->fenestrae[g].harmonia.genus == ACORDUM_NULLA)
    {
        g++;
    }
    redde g;
}

/* clavis fenestrae f: ad initium eius, aut clavis fenestrae sonantis
 * proximae si alia est et acordum eius dominans (dominans praevia);
 * tonica clavis veteris numquam praevia */
interior Clavis
_clavis_fenestrae (
        constans Carmen* carmen,
    constans Explicatio* e,
                    i32  f,
                    b32* praevia)
{
    Clavis k = _clavis(carmen, e->fenestrae[f].initium);
    Clavis n;
       i32 g = _proxima_sonans(e, f);

    *praevia = FALSUM;
    si (   g < e->numerus_fenestrarum
        && (s32)e->fenestrae[f].harmonia.radix != k.tonica)
    {
        n = _clavis(carmen, e->fenestrae[g].initium);
        si (   (n.tonica != k.tonica || n.minor != k.minor)
            && _dominans_clavis(&n, &e->fenestrae[f].harmonia))
        {
            *praevia = VERUM;
            redde n;
        }
    }
    redde k;
}

/* gradus (1..7) intervalli a tonica; 0 si extra scalam. Minor: VI et
 * VII elevati (IX, XI) gradus sui sunt */
interior s32
_gradus (
    constans Clavis* k,
                s32  intervallum)
{
    s32 i;

    per (i = ZEPHYRUM; i < VII; i++)
    {
        si ((k->minor ? MINOR[i] : MAIOR[i]) == intervallum)
        {
            redde i + I;
        }
    }
    si (k->minor && intervallum == IX)
    {
        redde VI;
    }
    si (k->minor && intervallum == XI)
    {
        redde VII;
    }
    redde ZEPHYRUM;
}

/* classes omnes in scala (minor cum elevatis) */
interior b32
_diatonicum (
    constans Clavis* k,
                i32  classes)
{
    s32 c;

    per (c = ZEPHYRUM; c < XII; c++)
    {
        si (   (classes & ((i32)I << c))
            && _gradus(k, _modulus(c - k->tonica, XII)) == ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* qualitas triadis diatonicae in gradu: VERUM si maior aut minor (non
 * diminuta) - applicatae metae */
interior b32
_meta_idonea (
    constans Clavis* k,
                s32  gradus)
{
    constans s32* s = k->minor ? MINOR : MAIOR;
             s32  r = s[gradus - I];
             s32  q = s[(gradus + III) % VII];

    redde _modulus(q - r, XII) == VII;
}

/* triadis diatonicae in gradu qualitas minor (pro littera metae) */
interior b32
_meta_minor (
    constans Clavis* k,
                s32  gradus)
{
    constans s32* s = k->minor ? MINOR : MAIOR;

    redde _modulus(s[(gradus + I) % VII] - s[gradus - I], XII) == III;
}

/* scriptor numeralis in tabulam characterum (XVI) */
nomen structura {
    character* t;
          i32  n;
} Numeralis;

interior vacuum
_scribere (
              Numeralis* x,
     constans character* s)
{
    dum (*s != '\0' && x->n + I < XVI)
    {
        x->t[x->n++] = *s++;
    }
    x->t[x->n] = '\0';
}

/* romanus gradus, parvus si minor */
interior vacuum
_romanum (
    Numeralis* x,
          s32  gradus,
          b32  parvus)
{
    constans character* r = ROMANI[gradus];

    dum (*r != '\0' && x->n + I < XVI)
    {
        x->t[x->n++] = parvus ? (character)(*r - 'A' + 'a') : *r;
        r++;
    }
    x->t[x->n] = '\0';
}

/* figura inversionis: b = bassus - radix */
interior constans character*
_figura (
     AcordumGenus g,
              s32 b)
{
    b32 septima = acordum_proprietates(g)->septima >= ZEPHYRUM;

    si (b == ZEPHYRUM)
    {
        redde septima ? "7" : "";
    }
    si (b == III || b == IV)
    {
        redde septima ? "65" : "6";
    }
    si (b >= VI && b <= VIII)
    {
        redde septima ? "43" : "64";
    }
    redde septima ? "42" : "";
}

/* figura inversionis acordi: solum gradu III sine tonis additis */
interior constans character*
_figura_acordi (
    constans Acordum* h)
{
    redde (   acordum_proprietates(h->genus)->gradus == III
           && acordum_proprietates(h->genus)->additae == ZEPHYRUM)
        ? _figura(h->genus,
                  _modulus((s32)h->bassus - (s32)h->radix, XII))
        : "";
}

/* picardia: tertia Picardiae (functio_tertia_picardiae) - acordum
 * finalis maius ut I, non applicata */
interior vacuum
_gradum_scribere (
                constans Clavis* clavis,
    constans ExplicatioFenestra* w,
                  FunctioGradus* g,
                            b32  picardia)
{
      constans Acordum* h = &w->harmonia;
                Clavis  k = *clavis;
             Numeralis  x;
                   s32  iv;
                   s32  d;
                   b32  parvus;
                   b32  dominans;
                   b32  diminuta;
    constans character* qualitas = "";
    constans character* figura;

    x.t           = g->numeralis;
    x.n           = ZEPHYRUM;
    x.t[0]        = '\0';
    g->gradus     = ZEPHYRUM;
    g->alteratio  = ZEPHYRUM;
    g->applicata  = ZEPHYRUM;
    si (h->genus == ACORDUM_NULLA)
    {
        _scribere(&x, "-");
        redde;
    }
    si (h->genus == ACORDUM_IGNOTA)
    {
        _scribere(&x, "?");
        redde;
    }
    iv  = _modulus((s32)h->radix - k.tonica, XII);
    d   = _gradus(&k, iv);
    si (d == ZEPHYRUM)
    {
        g->alteratio  = -I;
        d             = _gradus(&k, _modulus(iv + I, XII));
    }
    g->gradus = d;
    /* proprietates generis (acordum.h): numeralis parva = tertia
     * minor; figura inversionis solum gradu III */
    parvus    = acordum_proprietates(h->genus)->tertia == III;
    qualitas  = acordum_proprietates(h->genus)->numeralis;
    figura    = _figura_acordi(h);
    si (h->genus == ACORDUM_CLASSIS_UNA)
    {
        _scribere(&x, "(");
        si (g->alteratio < ZEPHYRUM)
        {
            _scribere(&x, "b");
        }
        _romanum(&x, d, FALSUM);
        _scribere(&x, ")");
        redde;
    }
    /* applicata: extra scalam, forma dominantis aut septimae
     * diminutae */
    dominans  = _forma_dominantis(h);
    diminuta  = h->genus == ACORDUM_DIMINUTA
             || h->genus == ACORDUM_DIMINUTA_SEPTIMA;
    si (   !picardia && (dominans || diminuta)
        && !_diatonicum(&k, h->classes))
    {
        s32 meta = dominans ? _modulus(iv - VII, XII)
                            : _modulus(iv + I, XII);
        s32 dm   = _gradus(&k, meta);

        si (dm > I && _meta_idonea(&k, dm))
        {
            g->applicata = dm;
            g->alteratio = ZEPHYRUM;
            _romanum(&x, dominans ? V : VII, diminuta);
            _scribere(&x, qualitas);
            _scribere(&x, figura);
            _scribere(&x, "/");
            _romanum(&x, dm, _meta_minor(&k, dm));
            redde;
        }
    }
    si (g->alteratio < ZEPHYRUM)
    {
        _scribere(&x, "b");
    }
    _romanum(&x, d, parvus);
    _scribere(&x, qualitas);
    _scribere(&x, figura);
}

/* SUBSTITUTA (functio.h): forma dominantis cum septima extra clavem,
 * non applicata, radix semitonio supra radicem fenestrae sonantis
 * proximae; meta gradus diatonicus maior aut minor (etiam I) */
interior vacuum
_substitutam_legere (
              constans Carmen* carmen,
          constans Explicatio* e,
                          i32  f,
                FunctioGradus* g)
{
    constans Acordum* h = &e->fenestrae[f].harmonia;
    constans Acordum* m;
                 i32  p = _proxima_sonans(e, f);
              Clavis  k;
                 b32  praevia;
                 s32  dm;
           Numeralis  x;

    si (   p >= e->numerus_fenestrarum || g->applicata != ZEPHYRUM
        || !_forma_dominantis(h)
        || acordum_proprietates(h->genus)->septima < ZEPHYRUM)
    {
        redde;
    }
    m = &e->fenestrae[p].harmonia;
    k = _clavis_fenestrae(carmen, e, f, &praevia);
    si (   m->genus == ACORDUM_IGNOTA || _diatonicum(&k, h->classes)
        || _modulus((s32)h->radix - (s32)m->radix, XII) != I)
    {
        redde;
    }
    dm = _gradus(&k, _modulus((s32)m->radix - k.tonica, XII));
    si (dm == ZEPHYRUM || !_meta_idonea(&k, dm))
    {
        redde;
    }
    g->substituta  = VERUM;
    g->applicata   = dm == I ? ZEPHYRUM : dm;
    x.t            = g->numeralis;
    x.n            = ZEPHYRUM;
    x.t[0]         = '\0';
    _scribere(&x, "sub");
    _romanum(&x, V, FALSUM);
    _scribere(&x, acordum_proprietates(h->genus)->numeralis);
    _scribere(&x, _figura_acordi(h));
    si (dm != I)
    {
        _scribere(&x, "/");
        _romanum(&x, dm, _meta_minor(&k, dm));
    }
}

/* adventus: fenestra a linea mensurae ad finem eius; pars summa (notae
 * summae in initio) nullam notam in dimidia posteriore incipit.
 * *summa = altitudo notae summae in initio */
interior b32
_adventus (
         constans Carmen* carmen,
     constans Explicatio* e,
                     i32  f,
                     s32* summa)
{
    constans ExplicatioFenestra* w = &e->fenestrae[f];
                            s64  initium;
                            s64  finis;
                            s32  altitudo  = -I;
                            i32  pars      = ZEPHYRUM;
                            i32  m         = I;
                            i32  i;

    dum (carmen_pulsus_mensurae(carmen, m + I) <= w->initium)
    {
        m++;
    }
    initium  = carmen_pulsus_mensurae(carmen, m);
    finis    = carmen_pulsus_mensurae(carmen, m + I);
    finis    = finis < carmen->pulsus ? finis : carmen->pulsus;
    si (w->initium != initium || w->finis < finis)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < e->numerus_segmentorum; i++)
    {
        constans ExplicatioSegmentum* s = &e->segmenta[i];

        si (   (i32)s->fenestra == f && s->initium == initium
            && (s32)s->nota->nota > altitudo)
        {
            altitudo  = (s32)s->nota->nota;
            pars      = s->pars;
        }
    }
    *summa = altitudo;
    si (altitudo < ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < e->numerus_segmentorum; i++)
    {
        constans ExplicatioSegmentum* s = &e->segmenta[i];

        si (   (i32)s->fenestra == f && s->pars == pars
            && s->pulsus > initium + (finis - initium) / II)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* V (trias, septima, nona, 13, 7#9, 7#11, dyas tertiae) sine
 * applicatione */
interior b32
_dominans (
    constans FunctioGradus* g,
          constans Acordum* h)
{
    redde g->applicata == ZEPHYRUM && g->alteratio == ZEPHYRUM
        && g->gradus == V
        && (_forma_dominantis(h) || h->genus == ACORDUM_TERTIA_MAIOR);
}

/* I aut i (trias, sexta aut nona addita, 6/9 - operetta in Iadd6
 * desinit, iazz in I6/9; dyas, unisonus: clausulae quintam saepe
 * omittunt, duae voces in unisono desinunt) */
interior b32
_finalis (
    constans FunctioGradus* g,
          constans Acordum* h)
{
    redde g->applicata == ZEPHYRUM && g->alteratio == ZEPHYRUM
        && g->gradus == I
        && (   h->genus == ACORDUM_MAIOR || h->genus == ACORDUM_MINOR
            || h->genus == ACORDUM_SEXTA
            || h->genus == ACORDUM_SEXTA_MINOR
            || h->genus == ACORDUM_NONA_ADDITA
            || h->genus == ACORDUM_SEXTA_NONA
            || h->genus == ACORDUM_TERTIA_MAIOR
            || h->genus == ACORDUM_TERTIA_MINOR
            || h->genus == ACORDUM_QUINTA
            || h->genus == ACORDUM_CLASSIS_UNA);
}

/* dominans aut viio sine applicatione (duplex usus) */
interior b32
_ante_dominantem (
    constans FunctioGradus* q,
          constans Acordum* h)
{
    redde _dominans(q, h)
        || (   q->applicata == ZEPHYRUM && q->alteratio == ZEPHYRUM
            && q->gradus == VII
            && (   h->genus == ACORDUM_DIMINUTA
                || h->genus == ACORDUM_DIMINUTA_SEPTIMA
                || h->genus == ACORDUM_DIMIDIA_DIMINUTA));
}

b32
functio_carminis (
        constans Carmen* carmen,
    constans Explicatio* explicatio,
                Piscina* piscina,
                Functio* functio)
{
    constans Explicatio* e = explicatio;
                    i32  f;

    functio->explicatio  = e;
    functio->numerus     = ZEPHYRUM;
    functio->gradus      = (FunctioGradus*)piscina_allocare(piscina,
        (memoriae_index)(e->numerus_fenestrarum + I)
            * magnitudo(FunctioGradus));
    si (functio->gradus == NIHIL)
    {
        redde FALSUM;
    }
    per (f = ZEPHYRUM; f < e->numerus_fenestrarum; f++)
    {
        FunctioGradus* g = &functio->gradus[f];
               Clavis  k;
                  b32  praevia;

        memset(g, ZEPHYRUM, magnitudo(FunctioGradus));
        g->fenestra  = f;
        k            = _clavis_fenestrae(carmen, e, f, &praevia);
        g->praevia   = praevia;
        _gradum_scribere(&k, &e->fenestrae[f], g,
                         functio_tertia_picardiae(carmen, e, f));
    }
    /* duplex usus: sexta (6, m6) in IV ante dominantem ut septima
     * gradus II - F6 in C = Dm7/F = ii65; Dm6 in a minor = Bm7b5/D =
     * iih65 */
    per (f = ZEPHYRUM; f + I < e->numerus_fenestrarum; f++)
    {
                      FunctioGradus* g = &functio->gradus[f];
        constans ExplicatioFenestra* w = &e->fenestrae[f];

        si (   (   w->harmonia.genus == ACORDUM_SEXTA
                || w->harmonia.genus == ACORDUM_SEXTA_MINOR)
            && g->gradus    == IV && g->applicata == ZEPHYRUM
            && g->alteratio == ZEPHYRUM
            && _ante_dominantem(&functio->gradus[f + I],
                                &e->fenestrae[f + I].harmonia))
        {
            ExplicatioFenestra secunda = *w;
                        Clavis k;
                           b32 praevia;

            secunda.harmonia.radix  = (w->harmonia.radix + IX) % XII;
            secunda.harmonia.genus  =
                w->harmonia.genus == ACORDUM_SEXTA
                    ? ACORDUM_SEPTIMA_MINOR : ACORDUM_DIMIDIA_DIMINUTA;
            k = _clavis_fenestrae(carmen, e, f, &praevia);
            _gradum_scribere(&k, &secunda, g, FALSUM);
        }
    }
    /* substitutae: post lectionem primam (proxima nota) */
    per (f = ZEPHYRUM; f < e->numerus_fenestrarum; f++)
    {
        _substitutam_legere(carmen, e, f, &functio->gradus[f]);
    }
    /* clausulae: adventus et fenestra prior (fenestrae semper
     * contiguae) */
    per (f = I; f < e->numerus_fenestrarum; f++)
    {
        constans ExplicatioFenestra* w = &e->fenestrae[f];
        constans ExplicatioFenestra* p = &e->fenestrae[f - I];
                      FunctioGradus* g = &functio->gradus[f];
             constans FunctioGradus* q = &functio->gradus[f - I];
                             Clavis  k = _clavis(carmen,
                                 w->initium);
                                s32 summa;

        si (!_adventus(carmen, e, f, &summa))
        {
            perge;
        }
        si (_dominans(g, &w->harmonia))
        {
            g->clausula = FUNCTIO_DIMIDIA;
        }
        alioquin si (_finalis(g, &w->harmonia))
        {
            b32 a_v   = _dominans(q, &p->harmonia)
                || (q->substituta && q->applicata == ZEPHYRUM);
            b32 a_vii = q->applicata == ZEPHYRUM && q->gradus == VII
                && (   p->harmonia.genus == ACORDUM_DIMINUTA
                    || p->harmonia.genus == ACORDUM_DIMINUTA_SEPTIMA
                    || p->harmonia.genus == ACORDUM_DIMIDIA_DIMINUTA);

            si (   a_v && p->harmonia.bassus == p->harmonia.radix
                && w->harmonia.bassus   == w->harmonia.radix
                && _modulus(summa, XII) == k.tonica)
            {
                g->clausula = FUNCTIO_PERFECTA;
            }
            alioquin si (a_v || a_vii)
            {
                g->clausula = FUNCTIO_IMPERFECTA;
            }
            alioquin si (   q->applicata == ZEPHYRUM
                         && q->alteratio == ZEPHYRUM && q->gradus == IV)
            {
                g->clausula = FUNCTIO_PLAGALIS;
            }
        }
        alioquin si (   _dominans(q, &p->harmonia) && g->gradus == VI
                     && g->applicata == ZEPHYRUM
                     && g->alteratio == ZEPHYRUM)
        {
            g->clausula = FUNCTIO_FALLAX;
        }
    }
    functio->numerus = e->numerus_fenestrarum;
    redde VERUM;
}

constans character*
functio_clausulae_titulus (
    FunctioClausula clausula)
{
    redde (i32)clausula < VI ? CLAUSULAE[clausula] : NIHIL;
}

/* "G: " / "e: " - clavis carminis ad pulsum, maior littera magna */
interior b32
_clavem_scribere (
       constans Carmen* carmen,
                   s64  pulsus,
     ChordaAedificator* textus)
{
     constans CarmenClavis* k = NIHIL;
        constans character* t;
                       i32  i;
                       b32  bene;

    per (i = ZEPHYRUM; i < carmen->numerus_clavium; i++)
    {
        si (carmen->claves[i].pulsus <= pulsus)
        {
            k = &carmen->claves[i];
        }
    }
    si (k == NIHIL)
    {
        redde VERUM;
    }
    t     = partitura_clavis_littera(k);
    bene  = chorda_aedificator_appendere_character(textus,
        k->minor ? t[ZEPHYRUM] : (character)(t[ZEPHYRUM] - 'a' + 'A'));
    bene  = chorda_aedificator_appendere_literis(textus, t + I) && bene;
    redde chorda_aedificator_appendere_literis(textus, ": ") && bene;
}

b32
functio_lineam_scribere (
               vacuum* ctx,
      constans Carmen* carmen,
                  s64  initium,
                  s64  finis,
    ChordaAedificator* textus)
{
    constans Functio* fx      = (constans Functio*)ctx;
                 b32  bene    = VERUM;
                 b32  primum  = VERUM;
                 i32  f;

    per (f = ZEPHYRUM; f < fx->numerus; f++)
    {
         constans ExplicatioFenestra* w = &fx->explicatio->fenestrae[f];
              constans FunctioGradus* g = &fx->gradus[f];
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
        si (   g->praevia
            && _proxima_sonans(fx->explicatio, f) < fx->numerus)
        {
            bene = _clavem_scribere(carmen,
                fx->explicatio->fenestrae[
                    _proxima_sonans(fx->explicatio, f)].initium, textus)
                && bene;
        }
        bene    = chorda_aedificator_appendere_literis(textus,
            g->numeralis) && bene;
        bene = chorda_aedificator_appendere_character(textus, ':')
            && bene;
        bene = partitura_valorem_scribere(ad - w->initium, textus)
            && bene;
        si (g->clausula != FUNCTIO_NULLA)
        {
            bene = chorda_aedificator_appendere_literis(textus, " [")
                && bene;
            bene = chorda_aedificator_appendere_literis(textus,
                ABBREVIATAE[g->clausula]) && bene;
            bene = chorda_aedificator_appendere_character(textus, ']')
                && bene;
        }
    }
    redde bene;
}

b32
functio_tertia_picardiae (
        constans Carmen* carmen,
    constans Explicatio* explicatio,
                    i32  fenestra)
{
            constans Explicatio* e = explicatio;
    constans ExplicatioFenestra* w;
                         Clavis  k;
                            i32  ultima = ZEPHYRUM;
                            i32  i;
                            s32  summa;
                            b32  praevia;
                  FunctioGradus  prior;
                  FunctioGradus  proxima;

    si (fenestra >= e->numerus_fenestrarum)
    {
        redde FALSUM;
    }
    w = &e->fenestrae[fenestra];
    /* dominans praevia: clavis nova, radix eius non tonica - numquam
     * Picardia */
    k = _clavis_fenestrae(carmen, e, fenestra, &praevia);
    si (   !k.minor || (s32)w->harmonia.radix != k.tonica
        || (   w->harmonia.genus != ACORDUM_MAIOR
            && w->harmonia.genus != ACORDUM_SEXTA
            && w->harmonia.genus != ACORDUM_NONA_ADDITA
            && w->harmonia.genus != ACORDUM_TERTIA_MAIOR))
    {
        redde FALSUM;
    }
    /* ultima sonans carminis */
    per (i = ZEPHYRUM; i < e->numerus_fenestrarum; i++)
    {
        si (e->fenestrae[i].harmonia.genus != ACORDUM_NULLA)
        {
            ultima = i;
        }
    }
    si (fenestra == ultima)
    {
        redde VERUM;
    }
    /* adventus post dominantem, proxima non in gradu IV (V/iv) */
    si (fenestra == ZEPHYRUM || !_adventus(carmen, e, fenestra, &summa))
    {
        redde FALSUM;
    }
    memset(&prior, ZEPHYRUM, magnitudo(prior));
    memset(&proxima, ZEPHYRUM, magnitudo(proxima));
    /* prior et proxima in clave fenestrae: dominans EIUS, IV EIUS */
    _gradum_scribere(&k, &e->fenestrae[fenestra - I], &prior, FALSUM);
    si (!_dominans(&prior, &e->fenestrae[fenestra - I].harmonia))
    {
        redde FALSUM;
    }
    _gradum_scribere(&k, &e->fenestrae[fenestra + I], &proxima, FALSUM);
    redde proxima.gradus != IV;
}
