/* comitatus.c - Figurae comitandi (vide include/comitatus.h)
 *
 * LECTIO: acorda et figura signum post signum, mensura post mensuram;
 * longitudo eadem regula ac cantus (divisor integrae 1..64, puncta ad
 * II) - lector MML eam interiorem tenet.
 *
 * COMPOSITIO: positio (bassus, quinta, voces superiores) cum acordo
 * solum mutatur; voces positionis clausae ex classibus acordi a radice
 * ascendentibus, inversio et octava motu minimo electae.
 */

#include "comitatus.h"
#include "cantus.h"
#include <string.h>

#define COMITATUS_VOX_BASSI    (-I)
#define COMITATUS_VOX_QUINTAE  (-II)

/* fractum: inter voces singulas sexagesima quarta */
#define COMITATUS_INTERVALLUM_FRACTI \
    ((s64)CANTUS_PULSUS_INTEGRAE / (s64)LXIV)

interior b32
_vitium (
       ComitatusVitium* vitium,
                   i32  offset,
    constans character* causa)
{
    vitium->offset  = offset;
    vitium->causa   = causa;
    redde FALSUM;
}

interior character
_signum (
    chorda textus,
       i32 i)
{
    redde i < textus.mensura ? (character)textus.datum[i] : '\0';
}

interior b32
_spatium (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior b32
_cifra (
    character c)
{
    redde c >= '0' && c <= '9';
}

interior s64
_absolutum (
    s64 x)
{
    redde x < ZEPHYRUM ? -x : x;
}

/* valor := divisor integrae {'.'} - ut cantus */
interior b32
_valorem_legere (
              chorda  textus,
                 i32* i,
                 s64* pulsus,
     ComitatusVitium* vitium)
{
    i32 initium  = *i;
    s32 n        = ZEPHYRUM;
    s64 basis;
    i32 puncta  = ZEPHYRUM;

    si (!_cifra(_signum(textus, *i)))
    {
        redde _vitium(vitium, *i, "longitudo exspectata");
    }
    dum (_cifra(_signum(textus, *i)) && n <= C)
    {
        n = n * X + (s32)(_signum(textus, *i) - '0');
        (*i)++;
    }
    si (   n < I || n > LXIV
        || (s32)CANTUS_PULSUS_INTEGRAE % n != ZEPHYRUM)
    {
        redde _vitium(vitium, initium,
                      "longitudo non divisor integrae (1..64)");
    }
    basis    = (s64)CANTUS_PULSUS_INTEGRAE / (s64)n;
    *pulsus  = basis;
    dum (_signum(textus, *i) == '.')
    {
        si (puncta == II)
        {
            redde _vitium(vitium, *i, "puncta plura quam II");
        }
        puncta++;
        si (basis % (puncta == I ? (s64)II : (s64)IV) != ZEPHYRUM)
        {
            redde _vitium(vitium, *i,
                "punctum longitudinem non dividit");
        }
        *pulsus += basis >> puncta;
        (*i)++;
    }
    redde VERUM;
}

i32
comitatus_mensurae (
    chorda textus)
{
    i32 n      = ZEPHYRUM;
    b32 plena  = FALSUM;
    i32 i;

    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        character c = (character)textus.datum[i];

        si (c == '|')
        {
            n++;
            plena = FALSUM;
        }
        alioquin si (!_spatium(c))
        {
            plena = VERUM;
        }
    }
    redde n + (plena ? (i32)I : (i32)ZEPHYRUM);
}

b32
comitatus_acorda_legere (
                chorda   textus,
          constans s64*  initia,
                   i32   numerus,
               Piscina*  piscina,
      ComitatusAcordum** acorda,
                   i32*  numerus_acordorum,
       ComitatusVitium*  vitium)
{
     ComitatusAcordum* a;
                  i32  n = ZEPHYRUM;
                  i32  i = ZEPHYRUM;
                  i32  m;

    a = (ComitatusAcordum*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(ComitatusAcordum)
                         * (memoriae_index)(textus.mensura + I)));
    si (a == NIHIL)
    {
        redde _vitium(vitium, ZEPHYRUM, "memoria deest");
    }
    per (m = ZEPHYRUM; ; m++)
    {
        i32 primum            = n;
        i32 initium_mensurae  = i;
        s64 summa             = ZEPHYRUM;
        i32 sine              = ZEPHYRUM;
        s64 longitudo;
        s64 pars;
        s64 reliquum;
        s64 t;
        i32 k;

        dum (i < textus.mensura && _signum(textus, i) != '|')
        {
            i32 j = i;

            si (_spatium(_signum(textus, i)))
            {
                i++;
                perge;
            }
            dum (   j < textus.mensura && !_spatium(_signum(textus, j))
                 && _signum(textus, j) != ':'
                 && _signum(textus, j) != '|')
            {
                j++;
            }
            memset(&a[n], ZEPHYRUM, magnitudo(a[n]));
            a[n].offset = i;
            a[n].nullum = j - i == I && _signum(textus, i) == 'r';
            a[n].acordum.genus = ACORDUM_NULLA;
            si (   !a[n].nullum
                && !acordum_legere(chorda_sectio(textus, i, j),
                                   &a[n].acordum))
            {
                redde _vitium(vitium, i, "acordum ignotum");
            }
            i = j;
            si (_signum(textus, i) == ':')
            {
                i++;
                si (!_valorem_legere(textus, &i, &a[n].longitudo,
                    vitium))
                {
                    redde FALSUM;
                }
                summa += a[n].longitudo;
            }
            alioquin
            {
                sine++;
            }
            n++;
        }
        si (n == primum)
        {
            si (i >= textus.mensura)
            {
                frange;
            }
            redde _vitium(vitium, initium_mensurae, "mensura vacua");
        }
        si (m >= numerus)
        {
            redde _vitium(vitium, initium_mensurae,
                          "mensurae plures quam initia");
        }
        longitudo  = initia[m + I] - initia[m];
        t          = ZEPHYRUM;
        per (k = primum; k < n; k++)
        {
            t += a[k].longitudo;
            si (t > longitudo)
            {
                redde _vitium(vitium, a[k].offset,
                              "valores mensuram excedunt");
            }
        }
        si (sine == ZEPHYRUM && summa < longitudo)
        {
            redde _vitium(vitium, a[n - I].offset,
                          "residuum mensurae sine acordo");
        }
        pars = sine > ZEPHYRUM ? (longitudo - summa) / (s64)sine
                               : ZEPHYRUM;
        si (sine > ZEPHYRUM && pars == ZEPHYRUM)
        {
            redde _vitium(vitium, a[primum].offset,
                          "residuum nimis breve");
        }
        /* residuum divisionis ultimo sine valore */
        reliquum = sine > ZEPHYRUM
            ? longitudo - summa - pars * (s64)(sine - I) : ZEPHYRUM;
        t = initia[m];
        per (k = primum; k < n; k++)
        {
            si (a[k].longitudo == ZEPHYRUM)
            {
                sine--;
                a[k].longitudo = sine == ZEPHYRUM ? reliquum : pars;
            }
            a[k].pulsus  = t;
            t            += a[k].longitudo;
        }
        si (i >= textus.mensura)
        {
            frange;
        }
        i++;
    }
    *acorda             = a;
    *numerus_acordorum  = n;
    redde VERUM;
}

/* vox: 'B' bassus, 'Q' quinta, 1..9 */
interior b32
_vocem (
    character  c,
         s32*  vox)
{
    si (c == 'B')
    {
        *vox = COMITATUS_VOX_BASSI;
        redde VERUM;
    }
    si (c == 'Q')
    {
        *vox = COMITATUS_VOX_QUINTAE;
        redde VERUM;
    }
    si (c >= '1' && c <= '9')
    {
        *vox = (s32)(c - '0');
        redde VERUM;
    }
    redde FALSUM;
}

b32
comitatus_figuram_legere (
              chorda  textus,
            Piscina*  piscina,
     ComitatusFigura* figura,
     ComitatusVitium* vitium)
{
                i32 capacitas  = textus.mensura + I;
                i32 mensurae   = comitatus_mensurae(textus) + II;
                s64 valor = (s64)CANTUS_PULSUS_INTEGRAE
                    / (s64)IV;
                f32  lucrum            = 1.0f;
                s64  t                 = ZEPHYRUM;
                i32  n                 = ZEPHYRUM;
                i32  nm                = ZEPHYRUM;
                i32  i                 = ZEPHYRUM;
                i32  initium_mensurae  = ZEPHYRUM;
    ComitatusGradus* g;

    memset(figura, ZEPHYRUM, magnitudo(*figura));
    g = (ComitatusGradus*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(ComitatusGradus)
                         * (memoriae_index)capacitas));
    figura->longitudines = (s64*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(s64) * (memoriae_index)mensurae));
    figura->primi = (i32*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(i32) * (memoriae_index)mensurae));
    si (   g             == NIHIL || figura->longitudines == NIHIL
        || figura->primi == NIHIL)
    {
        redde _vitium(vitium, ZEPHYRUM, "memoria deest");
    }
    figura->gradus    = g;
    figura->primi[0]  = ZEPHYRUM;
    dum (VERUM)
    {
              character  c = _signum(textus, i);
        ComitatusGradus* s;
                    s32  vox;

        si (i >= textus.mensura || c == '|')
        {
            si (n == figura->primi[nm])
            {
                si (i >= textus.mensura && nm > ZEPHYRUM)
                {
                    frange;
                }
                redde _vitium(vitium, initium_mensurae,
                              "mensura figurae vacua");
            }
            figura->longitudines[nm] = t;
            nm++;
            figura->primi[nm]  = n;
            t                  = ZEPHYRUM;
            lucrum             = 1.0f;
            si (i >= textus.mensura)
            {
                frange;
            }
            i++;
            initium_mensurae = i;
            perge;
        }
        si (_spatium(c))
        {
            i++;
            perge;
        }
        si (c == 'v')
        {
            i32 initium = i;
            s32 numerus = ZEPHYRUM;

            i++;
            dum (_spatium(_signum(textus, i)))
            {
                i++;
            }
            si (!_cifra(_signum(textus, i)))
            {
                redde _vitium(vitium, i, "numerus exspectatus");
            }
            dum (_cifra(_signum(textus, i)) && numerus <= M)
            {
                numerus = numerus * X + (s32)(_signum(textus, i) - '0');
                i++;
            }
            si (numerus > C)
            {
                redde _vitium(vitium, initium, "lucrum extra 0..100");
            }
            lucrum = (f32)numerus / 100.0f;
            perge;
        }
        s = &g[n];
        memset(s, ZEPHYRUM, magnitudo(*s));
        s->offset = i;
        si (c == 'r')
        {
            i++;
        }
        alioquin si (c == '[' || c == '{')
        {
            character claudens = c == '[' ? ']' : '}';

            s->fractum = c == '{';
            i++;
            dum (VERUM)
            {
                dum (_spatium(_signum(textus, i)))
                {
                    i++;
                }
                si (i >= textus.mensura)
                {
                    redde _vitium(vitium, s->offset,
                        "grex non clausus");
                }
                si (_signum(textus, i) == claudens)
                {
                    i++;
                    frange;
                }
                si (!_vocem(_signum(textus, i), &vox))
                {
                    redde _vitium(vitium, i, "vox ignota");
                }
                si (s->numerus_vocum == IX)
                {
                    redde _vitium(vitium, i, "voces plures quam IX");
                }
                s->voces[s->numerus_vocum++] = vox;
                i++;
            }
            si (s->numerus_vocum == ZEPHYRUM)
            {
                redde _vitium(vitium, s->offset, "grex vacuus");
            }
        }
        alioquin si (_vocem(c, &vox))
        {
            s->voces[0]       = vox;
            s->numerus_vocum  = I;
            i++;
        }
        alioquin
        {
            redde _vitium(vitium, i, "vox ignota");
        }
        si (_signum(textus, i) == ':')
        {
            i++;
            si (!_valorem_legere(textus, &i, &valor, vitium))
            {
                redde FALSUM;
            }
        }
        s->pulsus     = t;
        s->longitudo  = valor;
        s->lucrum     = lucrum;
        si (   s->fractum
            && (s64)(s->numerus_vocum
                - I) * COMITATUS_INTERVALLUM_FRACTI
                   >= valor)
        {
            redde _vitium(vitium, s->offset, "fractum gradu longius");
        }
        {
            i32 k;

            per (k = ZEPHYRUM; k < s->numerus_vocum; k++)
            {
                si (s->voces[k] > (s32)figura->vox_maxima)
                {
                    figura->vox_maxima = (i32)s->voces[k];
                }
                si (s->voces[k] == COMITATUS_VOX_BASSI)
                {
                    figura->bassus = VERUM;
                }
            }
        }
        t += valor;
        n++;
    }
    figura->numerus             = n;
    figura->numerus_mensurarum  = nm;
    redde VERUM;
}

/* positio sonans: acordum, bassus, quinta, voces clausae */
nomen structura {
        b32 sonat;
    Acordum acordum;
        s32 bassus;
        s32 quinta;
        s32 voces[XII];     /* positio clausa, ascendens */
        i32 numerus;        /* k: classes acordi */
        i32 adhibitae;      /* min(V, k) */
} Positio;

interior b32
_acorda_aequalia (
    constans Acordum* a,
    constans Acordum* b)
{
    redde a->genus == b->genus && a->radix == b->radix
        && a->bassus == b->bassus && a->classes == b->classes;
}

/* acordum ad pulsum; NIHIL in lacuna */
interior constans ComitatusAcordum*
_ad (
    constans ComitatusAcordum* acorda,
                          i32  numerus,
                          s64  pulsus)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (   acorda[k].pulsus <= pulsus
            && pulsus < acorda[k].pulsus + acorda[k].longitudo)
        {
            redde &acorda[k];
        }
    }
    redde NIHIL;
}

/* idem sonus: ambo nulla (aut lacunae) aut acorda aequalia */
interior b32
_aequalia (
    constans ComitatusAcordum* a,
    constans ComitatusAcordum* b)
{
    b32 nullum_a = a == NIHIL || a->nullum;
    b32 nullum_b = b == NIHIL || b->nullum;

    si (nullum_a || nullum_b)
    {
        redde nullum_a && nullum_b;
    }
    redde _acorda_aequalia(&a->acordum, &b->acordum);
}

/* nota classis in [infima, summa] proxima scopo, non 'vitanda';
 * aequales: inferior. -1 nulla */
interior s32
_proxima (
    i32 classis,
    s32 infima,
    s32 summa,
    s32 scopus,
    s32 vitanda)
{
    s32 optima = -I;
    s32 n;

    per (n = infima; n <= summa; n++)
    {
        si (   (i32)n % XII == classis && n != vitanda
            && (   optima < ZEPHYRUM
                || _absolutum((s64)(n - scopus))
                       < _absolutum((s64)(optima - scopus))))
        {
            optima = n;
        }
    }
    redde optima;
}

/* quinta acordi (acordum_proprietates, si in classibus); nulla: radix
 * + VII */
interior i32
_quintam (
    constans Acordum* a)
{
    s32 q = acordum_proprietates(a->genus)->quinta;

    si (q >= ZEPHYRUM && (a->classes & ((i32)I << ((a->radix + (i32)q)
                                                   % XII))))
    {
        redde (a->radix + (i32)q) % XII;
    }
    redde (a->radix + VII) % XII;
}

/* positio nova pro acordo: bassus, quinta, voces superiores motu
 * minimo */
interior vacuum
_positionem (
                      Positio* p,
             constans Acordum* a,
    constans ComitatusAmbitus* ambitus,
                          i32  vox_maxima,
                          b32  radix_licet)
{
    i32 ordo[XII];
    s32 omissa;
    s32 radix_omissa;
    b32 conversus;
    b32 quinta_mutata;
    s32 optima[XII];
    s64 pretium_optimum  = -I;
    i32 k                = ZEPHYRUM;
    i32 adhibitae;
    i32 r;
    i32 prima;
    s32 n0;

    /* bassus et quinta */
    p->bassus = _proxima(a->bassus, (s32)ambitus->bassi_infima,
        (s32)ambitus->bassi_summa,
        p->sonat ? p->bassus
                 : (s32)((ambitus->bassi_infima + ambitus->bassi_summa)
                         / II),
        -I);
    p->quinta = _proxima(_quintam(a), (s32)ambitus->bassi_infima,
        (s32)ambitus->bassi_summa, p->bassus, p->bassus);
    si (p->quinta < ZEPHYRUM)
    {
        p->quinta = p->bassus;
    }
    /* classes a radice ascendentes; voces pauciores quam classes:
     * quinta omittitur (sensibilis et septima manent) */
    omissa = -I;
    per (r = ZEPHYRUM; r < XII; r++)
    {
        si (a->classes & ((i32)I << ((a->radix + r) % XII)))
        {
            k++;
        }
    }
    /* quinta alterata (o, +, h7, o7, 7#5) acordum facit: numquam
     * omittitur (radix igitur sola cedit). Tonus additus sine septima
     * (6, m6, add9): quinta eos a cognatis distinguit (F6 A C D, non A
     * D F = Dm/F) - radix prius. Radix solum si bassus eam sonat
     * (radix_licet) */
    quinta_mutata = acordum_proprietates(a->genus) != NIHIL
        && acordum_proprietates(a->genus)->quinta >= ZEPHYRUM
        && acordum_proprietates(a->genus)->quinta != VII;
    conversus     = acordum_proprietates(a->genus) != NIHIL
        && acordum_proprietates(a->genus)->additae != ZEPHYRUM
        && acordum_proprietates(a->genus)->septima < ZEPHYRUM;
    radix_omissa  = -I;
    si (conversus && radix_licet && vox_maxima < k)
    {
        radix_omissa = (s32)a->radix;
        k--;
    }
    si (   !quinta_mutata && vox_maxima < k
        && (a->classes & ((i32)I << _quintam(a))))
    {
        omissa = (s32)_quintam(a);
        k--;
    }
    /* adhuc pauciores: radix quoque si bassus eam sonat - C7 duabus
     * vocibus E Bb, C9 tribus E Bb D */
    si (radix_licet && vox_maxima < k)
    {
        radix_omissa = (s32)a->radix;
    }
    k             = ZEPHYRUM;
    per (r = ZEPHYRUM; r < XII; r++)
    {
        i32 c = (a->radix + r) % XII;

        si (   (a->classes & ((i32)I << c)) && (s32)c != omissa
            && (s32)c != radix_omissa)
        {
            ordo[k++] = c;
        }
    }
    adhibitae = vox_maxima < k ? vox_maxima : k;
    per (prima = ZEPHYRUM; prima < (i32)k && adhibitae > ZEPHYRUM;
         prima++)
    {
        per (n0 = (s32)ambitus->infima; n0 <= (s32)ambitus->summa; n0++)
        {
            s32 v[XII];
            s64 pretium  = ZEPHYRUM;
            s64 summa    = ZEPHYRUM;
            i32 j;

            si ((i32)n0 % XII != ordo[prima])
            {
                perge;
            }
            /* radix non omissa inter voces adhibitas */
            si (radix_omissa < ZEPHYRUM && adhibitae < k)
            {
                b32 adest = FALSUM;

                per (j = ZEPHYRUM; j < adhibitae; j++)
                {
                    si (ordo[(prima + j) % k] == a->radix)
                    {
                        adest = VERUM;
                    }
                }
                si (!adest)
                {
                    perge;
                }
            }
            v[0] = n0;
            per (j = I; j < k; j++)
            {
                i32 c = ordo[(prima + j) % k];

                v[j] = v[j - I]
                     + (s32)((c + XII - (i32)v[j - I] % XII) % XII);
            }
            si (v[adhibitae - I] > (s32)ambitus->summa)
            {
                perge;
            }
            si (p->sonat && p->adhibitae > ZEPHYRUM)
            {
                i32 communes = adhibitae < p->adhibitae ? adhibitae
                                                        : p->adhibitae;

                per (j = ZEPHYRUM; j < communes; j++)
                {
                    pretium += _absolutum((s64)(v[j] - p->voces[j]));
                }
            }
            alioquin
            {
                per (j = ZEPHYRUM; j < adhibitae; j++)
                {
                    summa += (s64)v[j];
                }
                pretium = _absolutum(II * summa
                    - (s64)(ambitus->infima + ambitus->summa)
                      * (s64)adhibitae);
            }
            si (   pretium_optimum < ZEPHYRUM
                || pretium < pretium_optimum
                || (pretium == pretium_optimum && v[0] < optima[0]))
            {
                pretium_optimum = pretium;
                memcpy(optima, v, magnitudo(optima));
            }
        }
    }
    si (adhibitae > ZEPHYRUM)
    {
        memcpy(p->voces, optima, magnitudo(optima));
    }
    p->numerus    = k;
    p->adhibitae  = adhibitae;
    p->acordum    = *a;
    p->sonat      = VERUM;
}

b32
comitatus_componere (
      constans ComitatusFigura*  figura,
     constans ComitatusAcordum*  acorda,
                           i32   numerus_acordorum,
                  constans s64*  initia,
                           i32   numerus_mensurarum,
     constans ComitatusAmbitus*  ambitus,
                          b32   bassus_sonat,
                      Piscina*   piscina,
                ComitatusNota**  notae,
                          i32*   numerus,
               ComitatusVitium*  vitium)
{
          Positio  p;
    ComitatusNota* exitus;
              i32  maxima = ZEPHYRUM;
              i32  capacitas;
              i32  mensura_figurae  = ZEPHYRUM;
              i32  n                = ZEPHYRUM;
              i32  m;
              i32  k;

    si (   ambitus->summa > CXXVII || ambitus->bassi_summa > CXXVII
        || ambitus->infima > ambitus->summa
        || ambitus->bassi_infima > ambitus->bassi_summa)
    {
        redde _vitium(vitium, ZEPHYRUM, "ambitus extra MIDI");
    }
    si (   ambitus->summa - ambitus->infima < XII
        || ambitus->bassi_summa - ambitus->bassi_infima < XII)
    {
        redde _vitium(vitium, ZEPHYRUM, "ambitus angustior octava");
    }
    /* notae per mensuram figurae maximae */
    per (m = ZEPHYRUM; m < figura->numerus_mensurarum; m++)
    {
        i32 summa = ZEPHYRUM;

        per (k = figura->primi[m]; k < figura->primi[m + I]; k++)
        {
            summa += figura->gradus[k].numerus_vocum;
        }
        maxima = summa > maxima ? summa : maxima;
    }
    capacitas = numerus_mensurarum * maxima + I;
    exitus = (ComitatusNota*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(ComitatusNota)
                         * (memoriae_index)capacitas));
    si (exitus == NIHIL)
    {
        redde _vitium(vitium, ZEPHYRUM, "memoria deest");
    }
    memset(&p, ZEPHYRUM, magnitudo(p));
    per (m = ZEPHYRUM; m < numerus_mensurarum; m++)
    {
                              s64  longitudo;
        constans ComitatusAcordum* nunc;

        longitudo  = initia[m + I] - initia[m];
        nunc       = _ad(acorda, numerus_acordorum, initia[m]);

        /* acordum novum: aliud ac ad initium mensurae PRIORIS (mutatum
         * intra eam et tentum radicem accipit) */
        si (   m == ZEPHYRUM
            || !_aequalia(nunc,
                   _ad(acorda, numerus_acordorum, initia[m - I])))
        {
            mensura_figurae = ZEPHYRUM;
        }
        alioquin
        {
            mensura_figurae = (mensura_figurae + I)
                % figura->numerus_mensurarum;
        }
        si (figura->longitudines[mensura_figurae] != longitudo)
        {
            redde _vitium(vitium,
                figura->gradus[figura->primi[mensura_figurae]].offset,
                "mensura figurae mensurae carminis non congruit");
        }
        per (k = figura->primi[mensura_figurae]; k
            < figura->primi[mensura_figurae + I]; k++)
        {
             constans ComitatusGradus* g = &figura->gradus[k];
                                  s64  t = initia[m] + g->pulsus;
            constans ComitatusAcordum* a =
                _ad(acorda, numerus_acordorum, t);
                                 i32 j;

            si (g->numerus_vocum == ZEPHYRUM || a == NIHIL || a->nullum)
            {
                perge;
            }
            si (!p.sonat || !_acorda_aequalia(&p.acordum, &a->acordum))
            {
                _positionem(&p, &a->acordum, ambitus,
                    figura->vox_maxima,
                    (bassus_sonat || figura->bassus)
                        && a->acordum.bassus == a->acordum.radix);
            }
            per (j = ZEPHYRUM; j < g->numerus_vocum; j++)
            {
                s32 vox  = g->voces[j];
                s32 nota;
                s64 mora = g->fractum
                    ? (s64)j * COMITATUS_INTERVALLUM_FRACTI : ZEPHYRUM;
                ComitatusNota* e    = &exitus[n];

                si (vox == COMITATUS_VOX_BASSI)
                {
                    nota = p.bassus;
                }
                alioquin si (vox == COMITATUS_VOX_QUINTAE)
                {
                    nota = p.quinta;
                }
                alioquin
                {
                    i32 index = (i32)(vox - I);

                    nota = p.voces[index % p.numerus]
                         + (s32)(XII * (index / p.numerus));
                }
                si (nota > CXXVII)
                {
                    redde _vitium(vitium, g->offset, "nota extra MIDI");
                }
                e->pulsus     = t + mora;
                e->longitudo  = g->longitudo - mora;
                e->nota       = (i32)nota;
                e->lucrum     = g->lucrum;
                e->littera    = a->acordum.litterae[(i32)nota % XII];
                e->offset     = g->offset;
                e->vox        = (vox == COMITATUS_VOX_BASSI
                                 || vox == COMITATUS_VOX_QUINTAE)
                              ? ZEPHYRUM : (i32)vox;
                n++;
            }
        }
    }
    *notae    = exitus;
    *numerus  = n;
    redde VERUM;
}
