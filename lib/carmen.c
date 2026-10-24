/* carmen.c - Carmen (vide include/carmen.h)
 *
 * ORDO LECTIONIS carminis: attributa (tempus, metrum, clavis
 * initialia), deinde mutationes metri ORDINE (positio per mensuram ex
 * mappa metri iam lecta), deinde tempus et clavis (mappa metri plena),
 * deinde partes. Vitium ubicumque = liber vacuus.
 */
#include "carmen.h"
#include "xar.h"
#include "carmina_infixum.h"
#include "internamentum.h"
#include "stml.h"
#include <string.h>

#define CARMEN_TEMPUS_ORDINARIUM CXX


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_vacua (
    vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

interior b32
_vitium (
        CarminisVitium* vitium,
                chorda  carmen,
                chorda  pars,
                   s32  frustum,
                   i32  offset,
    constans character* causa)
{
    vitium->carmen   = carmen;
    vitium->pars     = pars;
    vitium->frustum  = frustum;
    vitium->offset   = offset;
    vitium->causa    = causa;
    redde FALSUM;
}

/* [a-z0-9_]+ */
interior b32
_signum_validum (
    chorda s)
{
    i32 i;

    si (s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        character x = (character)s.datum[i];

        si (!(   (x >= 'a' && x <= 'z') || (x >= '0' && x <= '9')
              || x == '_'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_elementum_est (
             StmlNodus* nodus,
    constans character* titulus)
{
    redde nodus->genus == STML_NODUS_ELEMENTUM
        && nodus->titulus != NIHIL
        && chorda_aequalis_literis(*nodus->titulus, titulus);
}

/* numerus integer attributi; FALSUM si absens aut non numerus */
interior b32
_s64_legere (
             StmlNodus* nodus,
    constans character* titulus,
                   s64* valor)
{
    chorda* a = stml_attributum_capere(nodus, titulus);

    redde a && chorda_ut_s64(*a, valor);
}

/* "N/D": N 1..64, D potentia II 1..64 */
interior b32
_metrum_legere (
          chorda  valor,
    CarmenMetrum* metrum)
{
    chorda n;
    chorda d;
       s32 numerator;
       s32 denominator;
       i32 i;

    per (i = ZEPHYRUM; i < valor.mensura && valor.datum[i] != '/'; i++)
    {
    }
    si (i == valor.mensura)
    {
        redde FALSUM;
    }
    n.datum    = valor.datum;
    n.mensura  = i;
    d.datum    = valor.datum + i + I;
    d.mensura  = valor.mensura - i - I;
    si (   !chorda_ut_s32(n, &numerator)
        || !chorda_ut_s32(d, &denominator)
        || numerator < I || numerator > LXIV || denominator < I
        || denominator > LXIV || (denominator & (denominator - I)) != 0)
    {
        redde FALSUM;
    }
    metrum->numerator    = (i32)numerator;
    metrum->denominator  = (i32)denominator;
    redde VERUM;
}

/* "a minor", "f# maior", "bb minor" -> quintae (circulus quintarum,
 * ut MusicXML 'fifths'): maior ex littera, '#' +VII, 'b' -VII, minor
 * -III; extra -VII..VII vitium */
interior b32
_clavem_legere (
          chorda  valor,
    CarmenClavis* clavis)
{
    hic_manens constans s32 quintae_litterarum[VII] = {
        III, V, ZEPHYRUM, II, IV, -I, I };   /* a b c d e f g */
       s32 q;
       i32 i = I;
    chorda modus;

    si (   valor.mensura < III || valor.datum[ZEPHYRUM] < 'a'
        || valor.datum[ZEPHYRUM] > 'g')
    {
        redde FALSUM;
    }
    q = quintae_litterarum[valor.datum[ZEPHYRUM] - 'a'];
    si (valor.datum[i] == '#')
    {
        q += VII;
        i++;
    }
    alioquin si (valor.datum[i] == 'b')
    {
        q -= VII;
        i++;
    }
    si (i >= valor.mensura || valor.datum[i] != ' ')
    {
        redde FALSUM;
    }
    modus.datum    = valor.datum + i + I;
    modus.mensura  = valor.mensura - i - I;
    si (chorda_aequalis_literis(modus, "minor"))
    {
        q              -= III;
        clavis->minor  = VERUM;
    }
    alioquin si (chorda_aequalis_literis(modus, "maior"))
    {
        clavis->minor = FALSUM;
    }
    alioquin
    {
        redde FALSUM;
    }
    si (q < -VII || q > VII)
    {
        redde FALSUM;
    }
    clavis->quintae = q;
    redde VERUM;
}

interior b32
_tempus_legere (
    chorda  valor,
       i32* tempus)
{
    s32 t;

    si (!chorda_ut_s32(valor, &t) || t < XX || t > CD)
    {
        redde FALSUM;
    }
    *tempus = (i32)t;
    redde VERUM;
}

/* pulsus mensurae metri dati */
interior s64
_longitudo_mensurae (
    constans CarmenMetrum* m)
{
    redde (s64)CANTUS_PULSUS_INTEGRAE * (s64)m->numerator
        / (s64)m->denominator;
}

/* initium mensurae ex metris [0, numerus) */
interior s64
_pulsus_mensurae (
    constans CarmenMetrum* metra,
                      i32  numerus,
                      i32  mensura)
{
    s64 pulsus = ZEPHYRUM;
    s64 numeri = I;   /* mensura cuius initium 'pulsus' est */
    i32 k;

    si (mensura <= I || numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        s64 l = _longitudo_mensurae(&metra[k]);
        s64 n;

        si (k + I < numerus)
        {
            n = (metra[k + I].pulsus - metra[k].pulsus) / l;
            si (numeri + n > (s64)mensura)
            {
                redde pulsus + ((s64)mensura - numeri) * l;
            }
            pulsus = metra[k + I].pulsus;
            numeri += n;
        }
        alioquin
        {
            redde pulsus + ((s64)mensura - numeri) * l;
        }
    }
    redde pulsus;
}

/* positio elementi: mensura="N" (>= 1) aut pulsus="N" (>= 0), altera
 * sola. FALSUM: causa in *causa */
interior b32
_positionem_legere (
             StmlNodus*  nodus,
       constans Carmen*  carmen,
                   b32   pulsus_admissus,
                   s64*  positio,
    constans character** causa)
{
    b32 m = stml_attributum_habet(nodus, "mensura");
    b32 p = stml_attributum_habet(nodus, "pulsus");
    s64 v;

    si (m == p || (p && !pulsus_admissus))
    {
        *causa =
            pulsus_admissus ? "positio: mensura aut pulsus, altera sola"
                                 : "mutatio metri per mensuram sola";
        redde FALSUM;
    }
    si (m)
    {
        si (   !_s64_legere(nodus, "mensura", &v) || v < I
            || v > CMXCIX * M)
        {
            *causa = "mensura non numerus >= 1";
            redde FALSUM;
        }
        *positio = _pulsus_mensurae(carmen->metra,
            carmen->numerus_metrorum,
            (i32)v);
        redde VERUM;
    }
    si (!_s64_legere(nodus, "pulsus", &v) || v < ZEPHYRUM)
    {
        *causa = "pulsus non numerus >= 0";
        redde FALSUM;
    }
    *positio = v;
    redde VERUM;
}

/* offset primi 't' in MML (extra '@titulus'); -1 si nullum */
interior s32
_tempus_in_textu (
    chorda textus)
{
    i32 i = ZEPHYRUM;

    dum (i < textus.mensura)
    {
        character x = (character)textus.datum[i];

        si (x == '@')
        {
            dum (i < textus.mensura && textus.datum[i] != ' ')
            {
                i++;
            }
            perge;
        }
        si (x == 't' || x == 'T')
        {
            redde (s32)i;
        }
        i++;
    }
    redde -I;
}


/* ==================================================
 * Partes
 * ================================================== */

interior b32
_frustum_legere (
               StmlNodus* nodus,
         constans Carmen* carmen,
                  chorda  titulus_partis,
                     s32  index,
                 Piscina* piscina,
           CarmenFrustum* frustum,
          CarminisVitium* vitium)
{
          StmlNodus* textus = NIHIL;
          Cantilena  k;
       CantusVitium  cv;
 constans character* causa = NIHIL;
                i32  i;
                s32  t;

    si (!_positionem_legere(nodus, carmen, VERUM, &frustum->pulsus,
                            &causa))
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, causa);
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (!_elementum_est(l, "textus") || textus != NIHIL)
        {
            redde _vitium(vitium, carmen->titulus, titulus_partis,
                index,
                ZEPHYRUM, "in frusto solum textus unus");
        }
        textus = l;
    }
    si (textus == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "frustum sine textu");
    }
    frustum->textus  = stml_textus_valor(textus, piscina);
    t                = _tempus_in_textu(frustum->textus);
    si (t >= ZEPHYRUM)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            (i32)t, "'t' in frusto: tempus carminis est");
    }
    si (!cantus_legere(frustum->textus, piscina, &k, &cv))
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            cv.offset, cv.causa);
    }
    si (k.numerus == ZEPHYRUM)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "frustum sine notis");
    }
    frustum->notae = (CarmenNota*)piscina_allocare(piscina,
        (memoriae_index)k.numerus * magnitudo(CarmenNota));
    si (frustum->notae == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "piscina exhausta");
    }
    per (i = ZEPHYRUM; i < k.numerus; i++)
    {
        CarmenNota* n = &frustum->notae[i];

        n->pulsus       = k.notae[i].pulsus;
        n->longitudo    = k.notae[i].pulsus_longitudo;
        n->nota         = k.notae[i].nota;
        n->lucrum       = k.notae[i].lucrum;
        n->littera      = k.notae[i].littera;
        n->praeparatum  = k.notae[i].praeparatum;
        n->offset       = k.notae[i].offset;
    }
    frustum->numerus           = k.numerus;
    frustum->longitudo         = k.pulsus;
    frustum->lineae            = k.lineae;
    frustum->numerus_linearum  = k.numerus_linearum;
    redde VERUM;
}

interior b32
_partem_legere (
               StmlNodus* nodus,
         constans Carmen* carmen,
                 Piscina* piscina,
              CarmenPars* pars,
          CarminisVitium* vitium)
{
    chorda* a;
       f64  lucrum;
       i32  numerus = ZEPHYRUM;
       i32  i;
       i32  j;

    a = stml_attributum_capere(nodus, "titulus");
    si (a == NIHIL || !_signum_validum(*a))
    {
        redde _vitium(vitium, carmen->titulus, _vacua(), -I, ZEPHYRUM,
            "pars sine titulo [a-z0-9_]+");
    }
    pars->titulus  = *a;
    a              = stml_attributum_capere(nodus, "instrumentum");
    si (a == NIHIL || !_signum_validum(*a))
    {
        redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
            ZEPHYRUM, "pars sine instrumento [a-z0-9_]+");
    }
    pars->instrumentum  = *a;
    pars->lucrum        = 1.0f;
    a                   = stml_attributum_capere(nodus, "lucrum");
    si (a != NIHIL)
    {
        si (!chorda_ut_f64(*a, &lucrum) || lucrum < 0.0 || lucrum > 1.0)
        {
            redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
                ZEPHYRUM, "lucrum extra 0..1");
        }
        pars->lucrum = (f32)lucrum;
    }
    pars->mutus = stml_attributum_habet(nodus, "mutus");
    pars->solus = stml_attributum_habet(nodus, "solus");
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (!_elementum_est(l, "frustum"))
        {
            redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
                ZEPHYRUM, "in parte solum frusta");
        }
        numerus++;
    }
    pars->frusta             = NIHIL;
    pars->numerus_frustorum  = ZEPHYRUM;
    si (numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    pars->frusta = (CarmenFrustum*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(CarmenFrustum));
    si (pars->frusta == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
            ZEPHYRUM, "piscina exhausta");
    }
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (!_frustum_legere(l, carmen, pars->titulus, (s32)j, piscina,
                             &pars->frusta[j], vitium))
        {
            redde FALSUM;
        }
        /* ordine positionis, non se tegentes */
        si (   j > ZEPHYRUM
            && pars->frusta[j].pulsus < pars->frusta[j - I].pulsus
                                      + pars->frusta[j - I].longitudo)
        {
            redde _vitium(vitium, carmen->titulus, pars->titulus,
                (s32)j,
                ZEPHYRUM,
                "frusta se tegunt aut ordine positionis non sunt");
        }
        j++;
    }
    pars->numerus_frustorum = numerus;
    redde VERUM;
}


/* ==================================================
 * Carmen
 * ================================================== */

/* liberi generis dati numerantur */
interior i32
_numerare (
             StmlNodus* nodus,
    constans character* titulus)
{
    i32 n = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        si (_elementum_est(stml_liberum_ad_indicem(nodus, i), titulus))
        {
            n++;
        }
    }
    redde n;
}

/* mutatio (tempus, metrum, clavis) ad positionem: eadem positione
 * ultimam substituit, ordine crescente aliter vitium. Reddit indicem
 * scribendum aut -1 */
interior s32
_locus_mutationis (
    s64 ultimus,
    i32 numerus,
    s64 positio)
{
    si (positio == ultimus)
    {
        redde (s32)numerus - I;
    }
    si (positio < ultimus)
    {
        redde -I;
    }
    redde (s32)numerus;
}


/* ==================================================
 * Instrumenta (libri et carminis)
 * ================================================== */

/* campus TibiaParametra per titulum attributi; fines inclusivi */
nomen structura {
     constans character* titulus;
                    f32  infima;
                    f32  summa;
} CampusInstrumenti;

hic_manens constans CampusInstrumenti CAMPI[XV] = {
    { "officium",    0.0f, 1.0f     },
    { "impetus",     0.0f, 60.0f    },
    { "lapsus",      0.0f, 60.0f    },
    { "tenor",       0.0f, 1.0f     },
    { "remissio",    0.0f, 60.0f    },
    { "vibratus",    0.0f, 12.0f    },
    { "celeritas",   0.0f, 50.0f    },
    { "gravia",      0.0f, 24000.0f },
    { "amplitudo",   0.0f, 1.0f     },
    { "decrementum", 0.0f, 60.0f    },
    { "claritas",    0.0f, 1.0f     },
    { "ratio",       0.0f, 32.0f    },
    { "index",       0.0f, 50.0f    },
    { "genus",       0.0f, 0.0f     },   /* verbum */
    { "forma",       0.0f, 0.0f     }    /* verbum */
};

hic_manens constans character* GENERA[III] = {
    "unda", "chorda", "modulatio"
};
hic_manens constans character* FORMAE[IV] = {
    "pulsus", "serra", "triangulum", "sinus"
};

/* campus k -> locus in parametris */
interior f32*
_campus (
    TibiaParametra* p,
               i32  k)
{
    f32* campi[XV];

    campi[0]   = &p->officium;
    campi[1]   = &p->involucrum.impetus;
    campi[2]   = &p->involucrum.lapsus;
    campi[3]   = &p->involucrum.tenor;
    campi[4]   = &p->involucrum.remissio;
    campi[5]   = &p->vibratus;
    campi[6]   = &p->vibratus_celeritas;
    campi[7]   = &p->gravia;
    campi[8]   = &p->amplitudo;
    campi[9]   = &p->decrementum;
    campi[10]  = &p->claritas;
    campi[11]  = &p->ratio;
    campi[12]  = &p->index;
    campi[13]  = &p->genus;
    campi[14]  = &p->forma;
    redde campi[k];
}

/* verbum in tabula -> index; -1 si ignotum */
interior s32
_verbum (
                  chorda   v,
      constans character** tabula,
                     i32   numerus)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (chorda_aequalis_literis(v, tabula[i]))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* "N..M" MIDI 0..127, N <= M */
interior b32
_ambitum_legere (
    chorda  v,
       i32* infima,
       i32* summa)
{
    i32 i;
    s64 a;
    s64 b;

    per (i = ZEPHYRUM; i + I < v.mensura; i++)
    {
        si (v.datum[i] == '.' && v.datum[i + I] == '.')
        {
            chorda x = chorda_sectio(v, ZEPHYRUM, i);
            chorda y = chorda_sectio(v, i + II, v.mensura);

            si (   !chorda_ut_s64(x, &a) || !chorda_ut_s64(y, &b)
                || a < ZEPHYRUM || b > CXXVII || a > b)
            {
                redde FALSUM;
            }
            *infima  = (i32)a;
            *summa   = (i32)b;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* instrumentum ex nodo; priora (eodem gradu et libri) = bases licitae.
 * vitium cum carmine (vacuo in libro) */
interior b32
_instrumentum_legere (
                      StmlNodus* nodus,
    constans CarmenInstrumentum* priora,
                            i32  numerus,
                         chorda  carmen,
             CarmenInstrumentum* x,
                 CarminisVitium* vitium)
{
    chorda* a      = stml_attributum_capere(nodus, "titulus");
    chorda* basis  = stml_attributum_capere(nodus, "basis");
       i32  i;
       i32  k;

    si (a == NIHIL || !_signum_validum(*a))
    {
        redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
            "instrumentum sine titulo [a-z0-9_]+");
    }
    x->titulus  = *a;
    x->infima   = ZEPHYRUM;
    x->summa    = CXXVII;
    memset(&x->parametra, ZEPHYRUM, magnitudo(TibiaParametra));
    x->parametra.forma             = (f32)SONUS_UNDA_SINUS;
    x->parametra.officium          = 0.5f;
    x->parametra.involucrum.tenor  = 1.0f;
    x->parametra.amplitudo         = 0.5f;
    si (basis != NIHIL)
    {
         constans CarmenInstrumentum* b = NIHIL;
                           character  t[LXIV];

        per (i = numerus; i > ZEPHYRUM && b == NIHIL; i--)
        {
            si (chorda_aequalis(priora[i - I].titulus, *basis))
            {
                b = &priora[i - I];
            }
        }
        si (b != NIHIL)
        {
            x->parametra  = b->parametra;
            x->infima     = b->infima;
            x->summa      = b->summa;
        }
        alioquin si (   basis->mensura > ZEPHYRUM
                     && basis->mensura < (i32)magnitudo(t))
        {
            memcpy(t, basis->datum, (size_t)basis->mensura);
            t[basis->mensura] = '\0';
            si (tibia_praeparatum(t) == NIHIL)
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "instrumentum: basis ignota");
            }
            x->parametra = *tibia_praeparatum(t);
        }
        alioquin
        {
            redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                "instrumentum: basis ignota");
        }
    }
    /* attributa omnia: nota aut vitium */
    per (i = ZEPHYRUM; nodus->attributa != NIHIL
                       && i < (i32)xar_numerus(nodus->attributa); i++)
    {
        StmlAttributum* t = (StmlAttributum*)xar_obtinere(
            nodus->attributa, i);
                    f64 valor;
                    s32 w;

        si (   chorda_aequalis_literis(*t->titulus, "titulus")
            || chorda_aequalis_literis(*t->titulus, "basis"))
        {
            perge;
        }
        si (chorda_aequalis_literis(*t->titulus, "ambitus"))
        {
            si (!_ambitum_legere(*t->valor, &x->infima, &x->summa))
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "instrumentum: valor malus aut extra fines");
            }
            perge;
        }
        per (k = ZEPHYRUM; k < XV; k++)
        {
            si (chorda_aequalis_literis(*t->titulus, CAMPI[k].titulus))
            {
                frange;
            }
        }
        si (k == XV)
        {
            redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                "instrumentum: attributum ignotum");
        }
        si (k >= XIII)
        {
            w = k == XIII ? _verbum(*t->valor, GENERA, III)
                          : _verbum(*t->valor, FORMAE, IV);
            si (w < ZEPHYRUM)
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "instrumentum: valor malus aut extra fines");
            }
            *_campus(&x->parametra, k) = (f32)w;
            perge;
        }
        si (   !chorda_ut_f64(*t->valor, &valor)
            || valor < (f64)CAMPI[k].infima
            || valor > (f64)CAMPI[k].summa)
        {
            redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                "instrumentum: valor malus aut extra fines");
        }
        *_campus(&x->parametra, k) = (f32)valor;
    }
    redde VERUM;
}

/* instrumenta nodi (filii 'instrumentum') in x[ab..]; bases: priora
 * eiusdem gradus et 'externa' (libri). Reddit numerum aut FALSUM per
 * vitium (numerus in *n) */
interior b32
_instrumenta_legere (
                      StmlNodus*  nodus,
    constans CarmenInstrumentum*  externa,
                            i32   numerus_externorum,
                         chorda   carmen,
                        Piscina*  piscina,
             CarmenInstrumentum** x,
                            i32*  n,
                 CarminisVitium*  vitium)
{
                   i32  numerus = _numerare(nodus, "instrumentum");
                   i32  i;
                   i32  j = ZEPHYRUM;
                   i32  m;
    CarmenInstrumentum* bases;

    *n = ZEPHYRUM;
    *x = (CarmenInstrumentum*)piscina_allocare(piscina,
        (memoriae_index)(numerus + numerus_externorum + I)
            * magnitudo(CarmenInstrumentum));
    bases = (CarmenInstrumentum*)piscina_allocare(piscina,
        (memoriae_index)(numerus + numerus_externorum + I)
            * magnitudo(CarmenInstrumentum));
    si (*x == NIHIL || bases == NIHIL)
    {
        redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    /* bases: externa, deinde propria ordine (supra) */
    per (i = ZEPHYRUM; i < numerus_externorum; i++)
    {
        bases[i] = externa[i];
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (!_elementum_est(l, "instrumentum"))
        {
            perge;
        }
        si (!_instrumentum_legere(l, bases, numerus_externorum + j,
                                  carmen, &(*x)[j], vitium))
        {
            redde FALSUM;
        }
        per (m = ZEPHYRUM; m < j; m++)
        {
            si (chorda_aequalis((*x)[m].titulus, (*x)[j].titulus))
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "instrumentum iteratum");
            }
        }
        bases[numerus_externorum + j] = (*x)[j];
        j++;
    }
    *n = j;
    redde VERUM;
}

interior b32
_carmen_legere (
                      StmlNodus* nodus,
    constans CarmenInstrumentum* libri,
                            i32  numerus_libri,
                        Piscina* piscina,
                         Carmen* k,
                 CarminisVitium* vitium)
{
                chorda* a;
                   i32  i;
                   i32  j;
                   s32  l;
                   s64  positio;
                   s64  finis;
    constans character* causa;

    memset(k, ZEPHYRUM, magnitudo(Carmen));
    a = stml_attributum_capere(nodus, "titulus");
    si (a == NIHIL || !_signum_validum(*a))
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "carmen sine titulo [a-z0-9_]+");
    }
    k->titulus  = *a;
    k->tempora  = (CantusTempus*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "tempus") + I)
        * magnitudo(CantusTempus));
    k->metra    = (CarmenMetrum*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "metrum") + I)
        * magnitudo(CarmenMetrum));
    k->claves   = (CarmenClavis*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "clavis") + I)
        * magnitudo(CarmenClavis));
    si (!k->tempora || !k->metra || !k->claves)
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    /* initialia */
    k->tempora[ZEPHYRUM].pulsus = ZEPHYRUM;
    k->tempora[ZEPHYRUM].tempus = CARMEN_TEMPUS_ORDINARIUM;
    k->metra[ZEPHYRUM].pulsus = ZEPHYRUM;
    k->metra[ZEPHYRUM].numerator = IV;
    k->metra[ZEPHYRUM].denominator = IV;
    k->claves[ZEPHYRUM].pulsus = ZEPHYRUM;
    k->claves[ZEPHYRUM].quintae = ZEPHYRUM;
    k->claves[ZEPHYRUM].minor = FALSUM;
    k->numerus_temporum = I;
    k->numerus_metrorum = I;
    k->numerus_clavium = I;
    a = stml_attributum_capere(nodus, "tempus");
    si (a && !_tempus_legere(*a, &k->tempora[ZEPHYRUM].tempus))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "tempus extra 20..400");
    }
    a = stml_attributum_capere(nodus, "metrum");
    si (a && !_metrum_legere(*a, &k->metra[ZEPHYRUM]))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "metrum non N/D (D potentia II)");
    }
    a = stml_attributum_capere(nodus, "clavis");
    si (a && !_clavem_legere(*a, &k->claves[ZEPHYRUM]))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "clavis non 'littera [#|b] maior|minor'");
    }
    /* elementa nota; mutationes metri ordine primae */
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(nodus, i);

        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (   !_elementum_est(n, "tempus")
            && !_elementum_est(n, "metrum")
            && !_elementum_est(n, "clavis")
            && !_elementum_est(n, "pars")
            && !_elementum_est(n, "instrumentum"))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "in carmine solum tempus, metrum, clavis, pars, "
                "instrumentum");
        }
        si (!_elementum_est(n, "metrum"))
        {
            perge;
        }
        a = stml_attributum_capere(n, "valor");
        si (!_positionem_legere(n, k, FALSUM, &positio, &causa))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                causa);
        }
        l = _locus_mutationis(k->metra[k->numerus_metrorum - I].pulsus,
            k->numerus_metrorum, positio);
        si (l < ZEPHYRUM || !a || !_metrum_legere(*a, &k->metra[l]))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "metrum: valor N/D, ordine mensurae");
        }
        k->metra[l].pulsus = positio;
        si (l == (s32)k->numerus_metrorum)
        {
            k->numerus_metrorum++;
        }
    }
    /* tempus et clavis (mappa metri plena) */
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
         StmlNodus* n       = stml_liberum_ad_indicem(nodus, i);
               b32  tempus  = _elementum_est(n, "tempus");

        si (!tempus && !_elementum_est(n, "clavis"))
        {
            perge;
        }
        a = stml_attributum_capere(n, "valor");
        si (!_positionem_legere(n, k, VERUM, &positio, &causa))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                causa);
        }
        si (tempus)
        {
            l = _locus_mutationis(
                k->tempora[k->numerus_temporum - I].pulsus,
                k->numerus_temporum, positio);
            si (   l < ZEPHYRUM || !a || !_tempus_legere(*a,
                    &k->tempora[l].tempus))
            {
                redde _vitium(vitium, k->titulus, _vacua(), -I,
                    ZEPHYRUM,
                    "tempus: valor 20..400, ordine positionis");
            }
            k->tempora[l].pulsus = positio;
            si (l == (s32)k->numerus_temporum)
            {
                k->numerus_temporum++;
            }
        }
        alioquin
        {
            l = _locus_mutationis(k->claves[k->numerus_clavium
                - I].pulsus,
                k->numerus_clavium, positio);
            si (   l < ZEPHYRUM || !a
                || !_clavem_legere(*a, &k->claves[l]))
            {
                redde _vitium(vitium, k->titulus, _vacua(), -I,
                    ZEPHYRUM,
                    "clavis: valor littera [#|b] maior|minor, ordine");
            }
            k->claves[l].pulsus = positio;
            si (l == (s32)k->numerus_clavium)
            {
                k->numerus_clavium++;
            }
        }
    }
    cantus_tempora_computare(k->tempora, k->numerus_temporum);
    /* instrumenta: carminis propria (bases: libri et supra), deinde
     * libri - quaerendo propria vincunt */
    {
        CarmenInstrumentum* propria;
                       i32  n;

        si (!_instrumenta_legere(nodus, libri, numerus_libri,
            k->titulus,
                                 piscina, &propria, &n, vitium))
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < numerus_libri; i++)
        {
            propria[n + i] = libri[i];
        }
        k->instrumenta             = propria;
        k->numerus_instrumentorum  = n + numerus_libri;
    }
    /* partes */
    k->numerus_partium = _numerare(nodus, "pars");
    si (k->numerus_partium > ZEPHYRUM)
    {
        k->partes = (CarmenPars*)piscina_allocare(piscina,
            (memoriae_index)k->numerus_partium * magnitudo(CarmenPars));
        si (!k->partes)
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "piscina exhausta");
        }
    }
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
         StmlNodus* n = stml_liberum_ad_indicem(nodus, i);
               i32  m;

        si (!_elementum_est(n, "pars"))
        {
            perge;
        }
        si (!_partem_legere(n, k, piscina, &k->partes[j], vitium))
        {
            redde FALSUM;
        }
        per (m = ZEPHYRUM; m < j; m++)
        {
            si (chorda_aequalis(k->partes[m].titulus,
                                k->partes[j].titulus))
            {
                redde _vitium(vitium, k->titulus, k->partes[j].titulus,
                    -I, ZEPHYRUM, "pars iterata");
            }
        }
        j++;
    }
    /* longitudo: finis frusti ultimi */
    k->pulsus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        CarmenPars* p = &k->partes[i];

        si (p->numerus_frustorum > ZEPHYRUM)
        {
            finis = p->frusta[p->numerus_frustorum - I].pulsus
                  + p->frusta[p->numerus_frustorum - I].longitudo;
            si (finis > k->pulsus)
            {
                k->pulsus = finis;
            }
        }
    }
    redde VERUM;
}

b32
carmina_legere (
            chorda  stml,
           Piscina* piscina,
           Carmina* liber,
    CarminisVitium* vitium)
{
          StmlResultus  r;
             StmlNodus* radix;
                Carmen* carmina;
                   i32  numerus = ZEPHYRUM;
                   i32  i;
                   i32  j;
                   i32  m;
    CarmenInstrumentum* libri;
                   i32  numerus_libri;

    liber->carmina = NIHIL;
    liber->numerus = ZEPHYRUM;
    r = stml_legere(stml, piscina, internamentum_creare(piscina));
    si (!r.successus)
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "STML fractum");
    }
    radix = r.elementum_radix;
    si (radix == NIHIL || !_elementum_est(radix, "carmina"))
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "radix non est 'carmina'");
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);

        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (_elementum_est(n, "instrumentum"))
        {
            perge;
        }
        si (!_elementum_est(n, "carmen"))
        {
            redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
                "in carminibus solum carmina et instrumenta");
        }
        numerus++;
    }
    /* instrumenta libri (ordine; basis supra) ante carmina */
    si (!_instrumenta_legere(radix, NIHIL, ZEPHYRUM, _vacua(), piscina,
                             &libri, &numerus_libri, vitium))
    {
        redde FALSUM;
    }
    si (numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    carmina = (Carmen*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Carmen));
    si (carmina == NIHIL)
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);

        si (   n->genus != STML_NODUS_ELEMENTUM
            || _elementum_est(n, "instrumentum"))
        {
            perge;
        }
        si (!_carmen_legere(n, libri, numerus_libri, piscina,
                            &carmina[j], vitium))
        {
            redde FALSUM;
        }
        per (m = ZEPHYRUM; m < j; m++)
        {
            si (chorda_aequalis(carmina[m].titulus, carmina[j].titulus))
            {
                redde _vitium(vitium, carmina[j].titulus, _vacua(), -I,
                    ZEPHYRUM, "carmen iteratum");
            }
        }
        j++;
    }
    liber->carmina = carmina;
    liber->numerus = numerus;
    redde VERUM;
}

b32
carmina_ordinaria (
           Piscina* piscina,
           Carmina* liber,
    CarminisVitium* vitium)
{
    redde carmina_legere(carmina_infixum_textus(piscina), piscina,
        liber,
        vitium);
}

constans Carmen*
carmina_invenire (
    constans Carmina* liber,
              chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < liber->numerus; i++)
    {
        si (chorda_aequalis(liber->carmina[i].titulus, titulus))
        {
            redde &liber->carmina[i];
        }
    }
    redde NIHIL;
}

s64
carmen_pulsus_mensurae (
    constans Carmen* carmen,
                i32  mensura)
{
    redde _pulsus_mensurae(carmen->metra, carmen->numerus_metrorum,
        mensura);
}

s64
carmen_tempus_mensurae (
    constans Carmen* carmen,
                s64  pulsus)
{
    constans CarmenMetrum* m = &carmen->metra[ZEPHYRUM];
                      i32  i;

    per (i = ZEPHYRUM; i < carmen->numerus_metrorum; i++)
    {
        si (carmen->metra[i].pulsus <= pulsus)
        {
            m = &carmen->metra[i];
        }
    }
    si (   m->denominator     == VIII && m->numerator > III
        && m->numerator % III == ZEPHYRUM)
    {
        redde (s64)III * CANTUS_PULSUS_INTEGRAE / VIII;
    }
    redde (s64)CANTUS_PULSUS_INTEGRAE / (s64)m->denominator;
}

constans CarmenInstrumentum*
carmen_instrumentum (
    constans Carmen* carmen,
             chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < carmen->numerus_instrumentorum; i++)
    {
        si (chorda_aequalis(carmen->instrumenta[i].titulus, titulus))
        {
            redde &carmen->instrumenta[i];
        }
    }
    redde NIHIL;
}
