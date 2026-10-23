/* partitura.c - Carmen ut textus (vide include/partitura.h)
 *
 * PER MENSURAM, PER PARTEM: notae partis quae mensuram [initium, finis)
 * tangunt (frusta ordine pulsus, notae ordine intra frustum - ergo
 * ordine initii); cursor ab initio mensurae: spatium ante notam =
 * pausae, nota ante initium = residuum ('~:'), nota ultra finem = '~'.
 * Notae se tegentes sine acervo (initio aut longitudine diversis)
 * ordine scribuntur, sine pausa (cursor non regreditur).
 */

#include "partitura.h"
#include <string.h>

/* gradus C D E F G A B */
hic_manens constans character LITTERAE[VII] = {
    'C', 'D', 'E', 'F', 'G', 'A', 'B'
};
hic_manens constans s32 SEMITONIA[VII] = {
    ZEPHYRUM, II, IV, V, VII, IX, XI
};
/* ordo diesium in signatura: F C G D A E B (bemolles retro) */
hic_manens constans i32 ORDO_DIESIUM[VII] = {
    III, ZEPHYRUM, IV, I, V, II, VI
};
/* claves per quintas -7..7 (ut carmen legit) */
hic_manens constans character* CLAVES_MAIORES[XV] = {
    "cb", "gb", "db", "ab", "eb", "bb", "f", "c",
    "g", "d", "a", "e", "b", "f#", "c#"
};
hic_manens constans character* CLAVES_MINORES[XV] = {
    "ab", "eb", "bb", "f", "c", "g", "d", "a",
    "e", "b", "f#", "c#", "g#", "d#", "a#"
};
/* pausae: valores maximi primum (punctati inclusi) */
hic_manens constans s64 PAUSAE[XIV] = {
    5760, 3840, 2880, 1920, 1440, 960, 720, 480, 360, 240, 180, 120, 90,
    60
};
hic_manens constans i32 VALORES_PAUSARUM[XIV] = {
    I, I, II, II, IV, IV, VIII, VIII, XVI, XVI, XXXII, XXXII, LXIV, LXIV
};
/* notae simul maximae in acervo uno */
#define PARTITURA_ACERVUS XVI

interior s32
_modulus (
    s32 a,
    s32 m)
{
    redde ((a % m) + m) % m;
}


/* ==================================================
 * Orthographia
 * ================================================== */

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

/* alteratio gradus in signatura clavis (quintae -7..7) */
interior s32
_signatura (
    s32 quintae,
    i32 gradus)
{
    s32 i;

    per (i = ZEPHYRUM; i < quintae; i++)
    {
        si (ORDO_DIESIUM[i] == gradus)
        {
            redde I;
        }
    }
    per (i = ZEPHYRUM; i < -quintae; i++)
    {
        si (ORDO_DIESIUM[VI - i] == gradus)
        {
            redde -I;
        }
    }
    redde ZEPHYRUM;
}

interior PartituraNota
_notam_facere (
    s32 nota,
    i32 gradus,
    s32 alteratio)
{
    PartituraNota n;
              s32 basis = nota - alteratio;   /* littera naturalis */

    n.littera    = LITTERAE[gradus];
    n.alteratio  = alteratio;
    n.octava     = (basis - _modulus(basis, XII)) / XII - I;
    redde n;
}

PartituraNota
partitura_orthographia (
        constans Carmen* carmen,
    constans CarmenNota* nota,
                    s64  pulsus)
{
    s32 n        = (s32)nota->nota;
    s32 q        = _clavis(carmen, pulsus)->quintae;
    s32 classis  = _modulus(n, XII);
    s32 a;
    i32 g;

    /* littera auctoris (MML eam semper scribit) */
    si (nota->littera >= 'a' && nota->littera <= 'g')
    {
        g = (i32)((nota->littera - 'a' + V) % VII);   /* a -> 5 */
        a = _modulus(n - SEMITONIA[g], XII);
        a = a > VI ? a - XII : a;
        si (a >= -II && a <= II)
        {
            redde _notam_facere(n, g, a);
        }
    }
    /* gradus scalae clavis */
    per (g = ZEPHYRUM; g < VII; g++)
    {
        si (_modulus(SEMITONIA[g] + _signatura(q, g), XII) == classis)
        {
            redde _notam_facere(n, g, _signatura(q, g));
        }
    }
    /* extra scalam: naturalis, aliter '#' (sine b) aut 'b' */
    per (g = ZEPHYRUM; g < VII; g++)
    {
        si (SEMITONIA[g] == classis)
        {
            redde _notam_facere(n, g, ZEPHYRUM);
        }
    }
    a = q < ZEPHYRUM ? -I : I;
    per (g = ZEPHYRUM; g < VII; g++)
    {
        si (_modulus(SEMITONIA[g] + a, XII) == classis)
        {
            redde _notam_facere(n, g, a);
        }
    }
    redde _notam_facere(n, ZEPHYRUM, ZEPHYRUM);   /* non attingitur */
}


/* ==================================================
 * Scriptio
 * ================================================== */

nomen structura {
    ChordaAedificator* a;
                  b32  bene;   /* FALSUM: memoria deest */
} Scriptor;

interior vacuum
_litterae (
              Scriptor* s,
    constans character* t)
{
    s->bene = chorda_aedificator_appendere_literis(s->a, t) && s->bene;
}

interior vacuum
_chorda (
    Scriptor* s,
      chorda  t)
{
    s->bene = chorda_aedificator_appendere_chorda(s->a, t) && s->bene;
}

interior vacuum
_numerus (
    Scriptor* s,
         s64  n)
{
    s->bene = chorda_aedificator_appendere_s32(s->a, (s32)n) && s->bene;
}

interior vacuum
_character (
     Scriptor* s,
    character  c)
{
    s->bene = chorda_aedificator_appendere_character(s->a, c)
        && s->bene;
}

constans character*
partitura_clavis_littera (
    constans CarmenClavis* clavis)
{
    redde clavis->minor ? CLAVES_MINORES[clavis->quintae + VII]
                        : CLAVES_MAIORES[clavis->quintae + VII];
}

interior vacuum
_clavem_scribere (
                 Scriptor* s,
    constans CarmenClavis* clavis)
{
    _litterae(s, partitura_clavis_littera(clavis));
    _litterae(s, clavis->minor ? " minor" : " maior");
}

/* ':valor' - 1..64, '.' punctata, 't' trioli, aliter '~pulsus' */
interior vacuum
_valorem_scribere (
    Scriptor* s,
         s64  l)
{
    i32 v;

    per (v = I; v <= LXIV; v *= II)
    {
        s64 basis     = (s64)CANTUS_PULSUS_INTEGRAE / (s64)v;
        b32 punctata  = l * II == basis * III;
        b32 ternaria  = l * III == basis * II;

        si (l == basis || punctata || ternaria)
        {
            _numerus(s, (s64)v);
            si (punctata)
            {
                _character(s, '.');
            }
            alioquin si (ternaria)
            {
                _character(s, 't');
            }
            redde;
        }
    }
    _character(s, '~');
    _numerus(s, l);
}

b32
partitura_valorem_scribere (
                   s64  pulsus,
     ChordaAedificator* textus)
{
    Scriptor s;

    s.a     = textus;
    s.bene  = VERUM;
    _valorem_scribere(&s, pulsus);
    redde s.bene;
}

/* pausae ab 'ab' ad 'ad', valoribus maximis primum */
interior vacuum
_pausas_scribere (
    Scriptor* s,
         s64  ab,
         s64  ad,
         b32* primum)
{
    i32 k = ZEPHYRUM;

    dum (ab < ad)
    {
        _litterae(s, *primum ? "r:" : " r:");
        *primum = FALSUM;
        dum (k < XIV && PAUSAE[k] > ad - ab)
        {
            k++;
        }
        si (k == XIV)
        {
            _character(s, '~');
            _numerus(s, ad - ab);
            redde;
        }
        _numerus(s, (s64)VALORES_PAUSARUM[k]);
        si (k % II == ZEPHYRUM && k != I)
        {
            _character(s, '.');
        }
        ab += PAUSAE[k];
    }
}

interior vacuum
_notam_scribere (
               Scriptor* s,
        constans Carmen* carmen,
    constans CarmenNota* nota,
                    s64  pulsus)
{
    PartituraNota o;
              s32 i;

    si (nota->praeparatum.mensura > ZEPHYRUM)
    {
        _character(s, '@');
        _chorda(s, nota->praeparatum);
        redde;
    }
    o = partitura_orthographia(carmen, nota, pulsus);
    _character(s, o.littera);
    per (i = ZEPHYRUM; i < o.alteratio; i++)
    {
        _character(s, '#');
    }
    per (i = ZEPHYRUM; i < -o.alteratio; i++)
    {
        _character(s, 'b');
    }
    _numerus(s, (s64)o.octava);
}

b32
partitura_notam_scribere (
        constans Carmen* carmen,
    constans CarmenNota* nota,
                    s64  pulsus,
      ChordaAedificator* textus)
{
    Scriptor s;

    s.a     = textus;
    s.bene  = VERUM;
    _notam_scribere(&s, carmen, nota, pulsus);
    redde s.bene;
}

PartituraAmbitus
partitura_ambitus (
    constans CarmenPars* pars)
{
    PartituraAmbitus a;
                 i32 f;
                 i32 j;

    memset(&a, ZEPHYRUM, magnitudo(a));
    per (f = ZEPHYRUM; f < pars->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &pars->frusta[f];

        per (j = ZEPHYRUM; j < fr->numerus; j++)
        {
            constans CarmenNota* n = &fr->notae[j];

            si (n->praeparatum.mensura > ZEPHYRUM)
            {
                perge;
            }
            a.numerus++;
            si (a.infima == NIHIL || n->nota < a.infima->nota)
            {
                a.infima          = n;
                a.pulsus_infimae  = fr->pulsus + n->pulsus;
            }
            si (a.summa == NIHIL || n->nota > a.summa->nota)
            {
                a.summa          = n;
                a.pulsus_summae  = fr->pulsus + n->pulsus;
            }
        }
    }
    redde a;
}

/* notae partis in mensura [initium, finis), una linea sine titulo */
interior vacuum
_partem_scribere (
               Scriptor* s,
        constans Carmen* carmen,
    constans CarmenPars* pars,
                    s64  initium,
                    s64  finis)
{
    constans CarmenNota* acervus[PARTITURA_ACERVUS];
                    s64  cursor = initium;
                    b32  primum = VERUM;
                    i32  f;
                    i32  j;

    per (f = ZEPHYRUM; f < pars->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &pars->frusta[f];

        j = ZEPHYRUM;
        dum (j < fr->numerus)
        {
            constans CarmenNota* n   = &fr->notae[j];
                            s64  ab  = fr->pulsus + n->pulsus;
                            s64  ad  = ab + n->longitudo;
                            s64  visum;
                            i32  numerus = ZEPHYRUM;
                            i32  x;
                            i32  y;

            /* acervus: notae sequentes eodem initio et longitudine */
            dum (   j < fr->numerus && numerus < PARTITURA_ACERVUS
                 && fr->notae[j].pulsus    == n->pulsus
                 && fr->notae[j].longitudo == n->longitudo)
            {
                acervus[numerus++] = &fr->notae[j];
                j++;
            }
            si (ad <= initium || ab >= finis)
            {
                perge;
            }
            visum = ab > initium ? ab : initium;
            si (visum > cursor)
            {
                _pausas_scribere(s, cursor, visum, &primum);
            }
            si (!primum)
            {
                _character(s, ' ');
            }
            primum = FALSUM;
            si (ab < initium)
            {
                _character(s, '~');
            }
            alioquin si (numerus == I)
            {
                _notam_scribere(s, carmen, n, ab);
            }
            alioquin
            {
                /* gradu ordinatae (insertio) */
                per (x = I; x < numerus; x++)
                {
                    constans CarmenNota* t = acervus[x];

                    y = x;
                    dum (y > ZEPHYRUM && acervus[y - I]->nota > t->nota)
                    {
                        acervus[y] = acervus[y - I];
                        y--;
                    }
                    acervus[y] = t;
                }
                _character(s, '[');
                per (x = ZEPHYRUM; x < numerus; x++)
                {
                    si (x > ZEPHYRUM)
                    {
                        _character(s, ' ');
                    }
                    _notam_scribere(s, carmen, acervus[x], ab);
                }
                _character(s, ']');
            }
            _character(s, ':');
            _valorem_scribere(s, (ad < finis ? ad : finis) - visum);
            si (ad > finis)
            {
                _character(s, '~');
            }
            si ((ad < finis ? ad : finis) > cursor)
            {
                cursor = ad < finis ? ad : finis;
            }
        }
    }
    si (cursor < finis)
    {
        _pausas_scribere(s, cursor, finis, &primum);
    }
}

/* mutatio (metrum, clavis, tempus) in mensura: '(+d)' si non initio */
interior vacuum
_positionem_scribere (
    Scriptor* s,
         s64  pulsus,
         s64  initium)
{
    si (pulsus > initium)
    {
        _litterae(s, " (+");
        _numerus(s, pulsus - initium);
        _character(s, ')');
    }
}

interior vacuum
_titulum_mensurae (
           Scriptor* s,
    constans Carmen* carmen,
                i32  mensura,
                s64  initium,
                s64  finis)
{
    i32 i;

    _litterae(s, "mensura ");
    _numerus(s, (s64)mensura);
    per (i = I; i < carmen->numerus_metrorum; i++)
    {
        constans CarmenMetrum* m = &carmen->metra[i];

        si (m->pulsus >= initium && m->pulsus < finis)
        {
            _litterae(s, "  metrum ");
            _numerus(s, (s64)m->numerator);
            _character(s, '/');
            _numerus(s, (s64)m->denominator);
            _positionem_scribere(s, m->pulsus, initium);
        }
    }
    per (i = I; i < carmen->numerus_clavium; i++)
    {
        si (   carmen->claves[i].pulsus >= initium
            && carmen->claves[i].pulsus < finis)
        {
            _litterae(s, "  clavis ");
            _clavem_scribere(s, &carmen->claves[i]);
            _positionem_scribere(s, carmen->claves[i].pulsus, initium);
        }
    }
    per (i = I; i < carmen->numerus_temporum; i++)
    {
        constans CantusTempus* t = &carmen->tempora[i];

        si (t->pulsus >= initium && t->pulsus < finis)
        {
            _litterae(s, "  tempus ");
            _numerus(s, (s64)t->tempus);
            _positionem_scribere(s, t->pulsus, initium);
        }
    }
    _character(s, '\n');
}

/* mensurae carminis: ultima quae ante finem incipit (I saltem) */
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

interior b32
_additam_habet (
    constans PartituraForma* forma)
{
    redde forma != NIHIL && forma->addita != NIHIL
        && forma->additae_titulus != NIHIL;
}

interior b32
_monstranda (
        constans CarmenPars* pars,
    constans PartituraForma* forma)
{
    redde forma == NIHIL || forma->pars.mensura == ZEPHYRUM
        || chorda_aequalis(pars->titulus, forma->pars);
}

b32
partitura_scribere (
             constans Carmen* carmen,
     constans PartituraForma* forma,
           ChordaAedificator* textus)
{
    Scriptor s;
         i32 prima     = I;
         i32 ultima    = _mensurae(carmen);
         i32 latitudo  = ZEPHYRUM;
         i32 inventae  = ZEPHYRUM;
         i32 m;
         i32 i;

    si (forma != NIHIL)
    {
        si (forma->prima > ZEPHYRUM)
        {
            prima = forma->prima;
        }
        si (forma->ultima > ZEPHYRUM)
        {
            si (forma->ultima > ultima)
            {
                redde FALSUM;
            }
            ultima = forma->ultima;
        }
    }
    si (prima > ultima)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        si (_monstranda(&carmen->partes[i], forma))
        {
            inventae++;
            si (carmen->partes[i].titulus.mensura > latitudo)
            {
                latitudo = carmen->partes[i].titulus.mensura;
            }
        }
    }
    si (   _additam_habet(forma)
        && (i32)strlen(forma->additae_titulus) > latitudo)
    {
        latitudo = (i32)strlen(forma->additae_titulus);
    }
    si (   inventae == ZEPHYRUM && forma != NIHIL
        && forma->pars.mensura > ZEPHYRUM)
    {
        redde FALSUM;
    }

    s.a     = textus;
    s.bene  = VERUM;
    _litterae(&s, "carmen ");
    _chorda(&s, carmen->titulus);
    _litterae(&s, ": ");
    _clavem_scribere(&s, &carmen->claves[ZEPHYRUM]);
    _litterae(&s, ", ");
    _numerus(&s, (s64)carmen->metra[ZEPHYRUM].numerator);
    _character(&s, '/');
    _numerus(&s, (s64)carmen->metra[ZEPHYRUM].denominator);
    _litterae(&s, ", ");
    _numerus(&s, (s64)carmen->tempora[ZEPHYRUM].tempus);
    _character(&s, '\n');
    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        constans CarmenPars* p = &carmen->partes[i];

        si (!_monstranda(p, forma))
        {
            perge;
        }
        _litterae(&s, "  pars ");
        _chorda(&s, p->titulus);
        _litterae(&s, ": ");
        _chorda(&s, p->instrumentum);
        _litterae(&s, ", lucrum ");
        s.bene = chorda_aedificator_appendere_f64(textus,
            (f64)p->lucrum,
            II) && s.bene;
        si (p->mutus)
        {
            _litterae(&s, " [mutus]");
        }
        si (p->solus)
        {
            _litterae(&s, " [solus]");
        }
        _character(&s, '\n');
    }

    per (m = prima; m <= ultima; m++)
    {
        s64 initium  = carmen_pulsus_mensurae(carmen, m);
        s64 finis    = carmen_pulsus_mensurae(carmen, m + I);

        _titulum_mensurae(&s, carmen, m, initium, finis);
        per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
        {
            constans CarmenPars* p = &carmen->partes[i];

            si (!_monstranda(p, forma))
            {
                perge;
            }
            _litterae(&s, "  ");
            _chorda(&s, p->titulus);
            s.bene = chorda_aedificator_appendere_repetita(textus, ' ',
                latitudo - p->titulus.mensura + II) && s.bene;
            _partem_scribere(&s, carmen, p, initium, finis);
            _character(&s, '\n');
        }
        si (_additam_habet(forma))
        {
            _litterae(&s, "  ");
            _litterae(&s, forma->additae_titulus);
            s.bene = chorda_aedificator_appendere_repetita(textus, ' ',
                latitudo - (i32)strlen(forma->additae_titulus) + II)
                && s.bene;
            s.bene = forma->addita(forma->additae_ctx, carmen, initium,
                finis, textus) && s.bene;
            _character(&s, '\n');
        }
    }
    redde s.bene;
}
