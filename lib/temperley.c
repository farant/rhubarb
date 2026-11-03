/* temperley.c - Moduli standard W_{n,j} algebrae Temperley-Lieb
 *
 * Status per enumerationem recursivam gignuntur, optionibus ordine
 * aperire, claudere, defectus - ordo lexicographicus formae textus.
 * Clavis status base IV, puncto 0 summo, ( = 0, ) = 1, | = 2: claves
 * ergo crescunt ordine enumerationis et quaestio binaria eas invenit
 * sine ordinatione (nullus qsort, nullus status universalis). Actio
 * e_i semel tabulatur: imago et genus per (i, status).
 *
 * Vide project-specs/temperley-lieb-plan.md.
 */
#include "temperley.h"

/* clavis 2 fila bitorum in i32 capere debet */
nomen character TemperleyClavisCapit[(II * TEMPERLEY_FILA_MAXIMA
    <= XXXII) ? I : -I];

structura TemperleyModulus {
     i32  fila;
     i32  defectus;
     i32  dimensio;
     i32* claves;      /* dimensio, stricte crescentes */
      s8* paria;       /* dimensio x fila */
     i32* imagines;    /* (fila - 1) x dimensio: ordine i, deinde status */
      i8* genera;      /* idem */
};

/* status enumerationis: localis, per recursionem traditus */
nomen structura {
     i32  fila;
     i32  defectus;
      s8  currens[TEMPERLEY_FILA_MAXIMA];
     i32  acervus[TEMPERLEY_FILA_MAXIMA];   /* arcus aperti */
     i32  numerus;
      s8* paria;                            /* NIHIL: numerare tantum */
} ContextusEnumerandi;


/* ==================================================
 * Auxilia
 * ================================================== */

interior vacuum
_enumerare (
    ContextusEnumerandi* ce,
                    i32  k,
                    i32  profunditas,
                    i32  facti)
{
    i32 a;
    i32 x;

    si (k == ce->fila)
    {
        si (profunditas == ZEPHYRUM && facti == ce->defectus)
        {
            si (ce->paria != NIHIL)
            {
                per (x = ZEPHYRUM; x < ce->fila; x++)
                {
                    ce->paria[ce->numerus * ce->fila + x] =
                        ce->currens[x];
                }
            }
            ce->numerus++;
        }
        redde;
    }
    /* ( : arcus aperitur si spatium claudendi manet */
    si (profunditas + I <= ce->fila - k - I)
    {
        ce->acervus[profunditas] = k;
        _enumerare(ce, k + I, profunditas + I, facti);
    }
    /* ) : arcus intimus clauditur */
    si (profunditas > ZEPHYRUM)
    {
        a               = ce->acervus[profunditas - I];
        ce->currens[a]  = (s8)k;
        ce->currens[k]  = (s8)a;
        _enumerare(ce, k + I, profunditas - I, facti);
        ce->acervus[profunditas - I] = a;
    }
    /* | : defectus extra omnes arcus */
    si (profunditas == ZEPHYRUM && facti < ce->defectus)
    {
        ce->currens[k] = (s8)TEMPERLEY_DEFECTUS;
        _enumerare(ce, k + I, profunditas, facti + I);
    }
}

/* clavis ex paribus; FALSUM si par extra ambitum */
interior b32
_clavis (
    constans s8* paria,
            i32  fila,
            i32* clavis)
{
    i32 c = ZEPHYRUM;
    i32 signum;
    i32 x;

    per (x = ZEPHYRUM; x < fila; x++)
    {
        si (paria[x] == (s8)TEMPERLEY_DEFECTUS)
        {
            signum = II;
        }
        alioquin si (paria[x] < ZEPHYRUM || (s32)paria[x] >= (s32)fila
                     || (s32)paria[x] == (s32)x)
        {
            redde FALSUM;
        }
        alioquin
        {
            signum = (s32)paria[x] > (s32)x ? ZEPHYRUM : I;
        }
        c = c * IV + signum;
    }
    *clavis = c;
    redde VERUM;
}

/* index clavis per quaestionem binariam; TEMPERLEY_ERRATUM si abest */
interior s32
_invenire (
    constans TemperleyModulus* m,
                          i32  clavis)
{
    i32 inferior = ZEPHYRUM;
    i32 superior = m->dimensio;
    i32 medius;

    dum (inferior < superior)
    {
        medius = inferior + (superior - inferior) / II;
        si (m->claves[medius] < clavis)
        {
            inferior = medius + I;
        }
        alioquin
        {
            superior = medius;
        }
    }
    si (inferior < m->dimensio && m->claves[inferior] == clavis)
    {
        redde (s32)inferior;
    }
    redde TEMPERLEY_ERRATUM;
}


/* ==================================================
 * Modulus
 * ================================================== */

TemperleyModulus*
temperley_modulus_creare (
         i32  fila,
         i32  defectus,
    Piscina*  piscina)
{
       TemperleyModulus* m;
    ContextusEnumerandi  ce;
                     s8  opus[TEMPERLEY_FILA_MAXIMA];
                    i32  loca;
                    i32  s;
                    i32  i;
                    i32  x;

    si (   fila == ZEPHYRUM || fila > TEMPERLEY_FILA_MAXIMA
        || defectus > fila || (fila - defectus) % II != ZEPHYRUM)
    {
        redde NIHIL;
    }
    ce.fila      = fila;
    ce.defectus  = defectus;
    ce.numerus   = ZEPHYRUM;
    ce.paria     = NIHIL;
    _enumerare(&ce, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);

    m = (TemperleyModulus*)piscina_allocare(piscina,
        magnitudo(TemperleyModulus));
    m->fila      = fila;
    m->defectus  = defectus;
    m->dimensio  = ce.numerus;
    m->paria     = (s8*)piscina_allocare(piscina,
        (memoriae_index)m->dimensio
        * (memoriae_index)fila * magnitudo(s8));
    ce.numerus  = ZEPHYRUM;
    ce.paria    = m->paria;
    _enumerare(&ce, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);

    m->claves = (i32*)piscina_allocare(piscina,
        (memoriae_index)m->dimensio
        * magnitudo(i32));
    per (s = ZEPHYRUM; s < m->dimensio; s++)
    {
        (vacuum)_clavis(m->paria + s * fila, fila, &m->claves[s]);
    }

    /* tabulae actionis: (fila - 1) x dimensio, una saltem cella */
    loca = (fila - I) * m->dimensio;
    m->imagines = (i32*)piscina_allocare(piscina, (memoriae_index)(loca
        > ZEPHYRUM
        ? loca : I) * magnitudo(i32));
    m->genera   = (i8*)piscina_allocare(piscina, (memoriae_index)(loca
        > ZEPHYRUM
        ? loca : I) * magnitudo(i8));
    per (i = ZEPHYRUM; i + I < fila; i++)
    {
        per (s = ZEPHYRUM; s < m->dimensio; s++)
        {
            i32 a = i;
            i32 b = i + I;
            s32 pa;
            s32 pb;
            s32 genus;
            s32 index = (s32)s;
            i32 clavis;

            per (x = ZEPHYRUM; x < fila; x++)
            {
                opus[x] = m->paria[s * fila + x];
            }
            pa = (s32)opus[a];
            pb = (s32)opus[b];
            si (pa == (s32)b)
            {
                genus = TEMPERLEY_DELTA;                    /* (a) */
            }
            alioquin si (pa >= ZEPHYRUM && pb >= ZEPHYRUM)
            {
                opus[a]        = (s8)b;                      /* (b) */
                opus[b]        = (s8)a;
                opus[(i32)pa]  = (s8)pb;
                opus[(i32)pb]  = (s8)pa;
                genus          = TEMPERLEY_UNUM;
            }
            alioquin si (pa < ZEPHYRUM && pb < ZEPHYRUM)
            {
                genus = TEMPERLEY_NIHIL;                    /* (d) */
            }
            alioquin
            {
                s32 alter = pa < ZEPHYRUM ? pb : pa;       /* (c) */

                opus[a]           = (s8)b;
                opus[b]           = (s8)a;
                opus[(i32)alter]  = (s8)TEMPERLEY_DEFECTUS;
                genus             = TEMPERLEY_UNUM;
            }
            si (genus == TEMPERLEY_UNUM)
            {
                /* imago semper status moduli est; ramus defensivus */
                si (!_clavis(opus, fila, &clavis))
                {
                    redde NIHIL;
                }
                index = _invenire(m, clavis);
                si (index < ZEPHYRUM)
                {
                    redde NIHIL;
                }
            }
            m->imagines[i * m->dimensio + s] = genus == TEMPERLEY_NIHIL
                ? ZEPHYRUM : (i32)index;
            m->genera[i * m->dimensio + s]   = (i8)genus;
        }
    }
    redde m;
}

i32
temperley_modulus_dimensio (
    constans TemperleyModulus* m)
{
    redde m->dimensio;
}

i32
temperley_modulus_fila (
    constans TemperleyModulus* m)
{
    redde m->fila;
}

i32
temperley_modulus_defectus (
    constans TemperleyModulus* m)
{
    redde m->defectus;
}


/* ==================================================
 * Actio et status
 * ================================================== */

s32
temperley_actio (
    constans TemperleyModulus* m,
                          i32  i,
                          i32  status,
                          i32* imago)
{
    i32 locus;

    si (i == ZEPHYRUM || i >= m->fila || status >= m->dimensio)
    {
        redde TEMPERLEY_ERRATUM;
    }
    locus = (i - I) * m->dimensio + status;
    si ((s32)m->genera[locus] != TEMPERLEY_NIHIL)
    {
        *imago = m->imagines[locus];
    }
    redde (s32)m->genera[locus];
}

b32
temperley_status_paria (
    constans TemperleyModulus* m,
                          i32  status,
                          s32* paria)
{
    i32 x;

    si (status >= m->dimensio)
    {
        redde FALSUM;
    }
    per (x = ZEPHYRUM; x < m->fila; x++)
    {
        paria[x] = (s32)m->paria[status * m->fila + x];
    }
    redde VERUM;
}

s32
temperley_status_quaerere (
    constans TemperleyModulus* m,
                 constans s32* paria)
{
     s8 opus[TEMPERLEY_FILA_MAXIMA];
    i32 clavis;
    s32 index;
    i32 x;

    per (x = ZEPHYRUM; x < m->fila; x++)
    {
        si (paria[x] < TEMPERLEY_DEFECTUS || paria[x] >= (s32)m->fila)
        {
            redde TEMPERLEY_ERRATUM;
        }
        opus[x] = (s8)paria[x];
    }
    si (!_clavis(opus, m->fila, &clavis))
    {
        redde TEMPERLEY_ERRATUM;
    }
    index = _invenire(m, clavis);
    si (index < ZEPHYRUM)
    {
        redde TEMPERLEY_ERRATUM;
    }
    /* clavis formam solam videt: paria transeuntia aut non reciproca
     * eandem formam habere possunt */
    per (x = ZEPHYRUM; x < m->fila; x++)
    {
        si (opus[x] != m->paria[(i32)index * m->fila + x])
        {
            redde TEMPERLEY_ERRATUM;
        }
    }
    redde index;
}

chorda
temperley_status_ad_chordam (
    constans TemperleyModulus* m,
                          i32  status,
                     Piscina*  piscina)
{
    chorda forma;
        s8 par;
       i32 x;

    forma.mensura  = ZEPHYRUM;
    forma.datum    = NIHIL;
    si (status >= m->dimensio)
    {
        redde forma;
    }
    forma.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)m->fila);
    per (x = ZEPHYRUM; x < m->fila; x++)
    {
        par = m->paria[status * m->fila + x];
        forma.datum[x] = (i8)(par == (s8)TEMPERLEY_DEFECTUS ? '|'
            : ((s32)par > (s32)x ? '(' : ')'));
    }
    forma.mensura = m->fila;
    redde forma;
}
