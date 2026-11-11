/*
 * plastica.c - descriptio textualis in statuam (plastica.h)
 *
 * Arbor STML bis ambulatur: gradus 0 probat et numerat (omnis
 * recusatio hic nascitur), gradus I tabulas statuae implet. Inter
 * gradus nihil recusari potest nisi piscina deficit.
 */

#include "plastica.h"
#include "stml.h"
#include "internamentum.h"
#include "xar.h"
#include "stereometria.h"
#include <math.h>
#include <string.h>

#define ERRORIS_MENSURA  CCLVI
#define CAPSAE_VERTICES  XXIV
#define CAPSAE_INDICES   XXXVI

nomen structura {
      Piscina* piscina;
       Statua* statua;
       chorda* partes;
          i32  gradus;       /* 0 numerat, I implet */
          i32  nodorum;
          i32  solidorum;
          i32  verticum;
          i32  indicum;
          b32  fractum;
          i32  linea;
    character  error[ERRORIS_MENSURA];
} Plastes;


/* ==================================================
 * Recusatio
 * ================================================== */

interior vacuum
_appone (
             character* destinatio,
                   i32* longitudo,
    constans character* fons,
                   i32  mensura)
{
    i32 k;

    per (k = ZEPHYRUM;
         k < mensura && *longitudo + I < ERRORIS_MENSURA; k++)
    {
        character littera = fons[k];

        /* nuntius lineam novam numquam fert */
        si (littera == '\n' || littera == '\r' || littera == '\t')
        {
            littera = ' ';
        }
        destinatio[(*longitudo)++] = littera;
    }
    destinatio[*longitudo] = '\0';
}

/* nuntius = ante + medium + post; prima recusatio manet */
interior b32
_recusare (
               Plastes* p,
                   i32  linea,
    constans character* ante,
       constans chorda* medium,
    constans character* post)
{
    i32 longitudo = ZEPHYRUM;

    si (p->fractum)
    {
        redde FALSUM;
    }
    p->fractum  = VERUM;
    p->linea    = linea;
    _appone(p->error, &longitudo, ante, (i32)strlen(ante));
    si (medium && medium->datum)
    {
        _appone(p->error, &longitudo,
            (constans character*)medium->datum,
            medium->mensura > LXIV ? LXIV : medium->mensura);
    }
    si (post)
    {
        _appone(p->error, &longitudo, post, (i32)strlen(post));
    }
    redde FALSUM;
}


/* ==================================================
 * Valores attributorum
 * ================================================== */

interior b32
_spatium (
    i8 littera)
{
    redde littera == ' ' || littera == '\t' || littera == '\n'
        || littera == '\r';
}

/* 'numerus' numeri finiti spatiis separati, nec plus nec minus */
interior b32
_numeri (
    constans chorda* valor,
                f32* exitus,
                i32  numerus)
{
    i32 k = ZEPHYRUM;
    i32 n = ZEPHYRUM;

    dum (VERUM)
    {
        chorda verbum;
           f64 x;
           i32 initium;

        dum (k < valor->mensura && _spatium(valor->datum[k]))
        {
            k++;
        }
        si (k >= valor->mensura)
        {
            frange;
        }
        initium = k;
        dum (k < valor->mensura && !_spatium(valor->datum[k]))
        {
            k++;
        }
        si (n >= numerus)
        {
            redde FALSUM;
        }
        verbum = chorda_sectio(*valor, initium, k);
        si (   !chorda_ut_f64(verbum, &x) || !(x == x)
            || x > 1.0e6 || x < -1.0e6)
        {
            redde FALSUM;
        }
        exitus[n++] = (f32)x;
    }
    redde n == numerus;
}

interior s32
_digitus_hex (
    i8 littera)
{
    si (littera >= '0' && littera <= '9')
    {
        redde (s32)(littera - '0');
    }
    si (littera >= 'a' && littera <= 'f')
    {
        redde (s32)(littera - 'a') + X;
    }
    si (littera >= 'A' && littera <= 'F')
    {
        redde (s32)(littera - 'A') + X;
    }
    redde -I;
}

/* "#rgb" aut "#rrggbb" in tres f32 (0 .. 1) */
interior b32
_color (
    constans chorda* valor,
                f32* exitus)
{
    i32 k;

    si (   valor->mensura == ZEPHYRUM || valor->datum[ZEPHYRUM] != '#'
        || (valor->mensura != IV && valor->mensura != VII))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 a;
        s32 b;

        si (valor->mensura == IV)
        {
            a = _digitus_hex(valor->datum[I + k]);
            b = a;
        }
        alioquin
        {
            a = _digitus_hex(valor->datum[I + k * II]);
            b = _digitus_hex(valor->datum[II + k * II]);
        }
        si (a < ZEPHYRUM || b < ZEPHYRUM)
        {
            redde FALSUM;
        }
        exitus[k] = (f32)(a * XVI + b) / 255.0f;
    }
    redde VERUM;
}

interior b32
_titulus_est (
       constans chorda* titulus,
    constans character* literae)
{
    redde titulus && chorda_aequalis_literis(*titulus, literae);
}

/* omne attributum nodi in indice 'licita' (NIHIL terminato) est? */
interior b32
_attributa_licita (
                      Plastes* p,
             constans StmlNodus* nodus,
    constans character* constans* licita)
{
    i32 numerus = nodus->attributa ? xar_numerus(nodus->attributa)
        : ZEPHYRUM;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans StmlAttributum* a =
            (constans StmlAttributum*)xar_obtinere(nodus->attributa,
                k);
                            b32 notum = FALSUM;

        per (j = ZEPHYRUM; a && licita[j]; j++)
        {
            si (_titulus_est(a->titulus, licita[j]))
            {
                notum = VERUM;
                frange;
            }
        }
        si (!notum)
        {
            redde _recusare(p, nodus->linea, "unknown attribute '",
                a ? a->titulus : NIHIL, "'");
        }
    }
    redde VERUM;
}


/* ==================================================
 * Transformatio et proprietates communes
 * ================================================== */

interior StereometriaQuaternio
_circa_axem (
    f32 x,
    f32 y,
    f32 z,
    f32 gradus)
{
    StereometriaQuaternio q;
                      f32 dimidium =
                          gradus * STEREOMETRIA_PI / 360.0f;
                      f32 sinus    = (f32)sin((f64)dimidium);

    q.x = x * sinus;
    q.y = y * sinus;
    q.z = z * sinus;
    q.w = (f32)cos((f64)dimidium);
    redde q;
}

/* a * b: b primum applicatur */
interior StereometriaQuaternio
_componere (
    StereometriaQuaternio a,
    StereometriaQuaternio b)
{
    StereometriaQuaternio q;

    q.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    q.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    q.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    q.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    redde q;
}

/* locus, gyrus, rotatio nodi: gyrus primus, deinde rotatio circa x,
 * y, z hoc ordine */
interior b32
_transformatio (
                  Plastes* p,
       constans StmlNodus* nodus,
       StereometriaVector* translatio,
    StereometriaQuaternio* rotatio)
{
    constans chorda* valor;
                f32  n[III];

    *translatio  = stereometria_vector(0.0f, 0.0f, 0.0f);
    *rotatio     = stereometria_quaternio_identitas();
    valor        = stml_attributum_capere(nodus, "locus");
    si (valor)
    {
        si (!_numeri(valor, n, III))
        {
            redde _recusare(p, nodus->linea,
                "attribute 'locus' needs 3 numbers (x y z), got '",
                valor, "'");
        }
        *translatio = stereometria_vector(n[ZEPHYRUM], n[I], n[II]);
    }
    valor = stml_attributum_capere(nodus, "gyrus");
    si (valor)
    {
        si (!_numeri(valor, n, I))
        {
            redde _recusare(p, nodus->linea,
                "attribute 'gyrus' needs 1 number (degrees), got '",
                valor, "'");
        }
        *rotatio = _circa_axem(0.0f, 1.0f, 0.0f, n[ZEPHYRUM]);
    }
    valor = stml_attributum_capere(nodus, "rotatio");
    si (valor)
    {
        si (!_numeri(valor, n, III))
        {
            redde _recusare(p, nodus->linea,
                "attribute 'rotatio' needs 3 numbers (degrees about "
                "x y z), got '", valor, "'");
        }
        *rotatio = _componere(
            _circa_axem(1.0f, 0.0f, 0.0f, n[ZEPHYRUM]), *rotatio);
        *rotatio = _componere(_circa_axem(0.0f, 1.0f, 0.0f, n[I]),
            *rotatio);
        *rotatio = _componere(_circa_axem(0.0f, 0.0f, 1.0f, n[II]),
            *rotatio);
    }
    *rotatio = stereometria_quaternio_normalizare(*rotatio);
    redde VERUM;
}

/* nodum novum numerare aut implere; redde indicem eius */
interior i32
_nodus_novus (
                  Plastes* p,
       constans StmlNodus* nodus,
                      s32  parens,
       StereometriaVector  translatio,
    StereometriaQuaternio  rotatio)
{
    i32 n = p->nodorum++;

    si (p->gradus)
    {
        constans chorda* titulus =
            stml_attributum_capere(nodus, "titulus");
            StatuaNodus* novus = &p->statua->nodi[n];

        novus->parens      = parens;
        novus->rete        = -I;
        novus->signa       = ZEPHYRUM;
        novus->translatio  = translatio;
        novus->rotatio     = rotatio;
        novus->scala       = stereometria_vector(1.0f, 1.0f, 1.0f);
        si (titulus && titulus->mensura)
        {
            p->partes[n] = chorda_transcribere(*titulus, p->piscina);
        }
    }
    redde n;
}


/* ==================================================
 * Solida
 * ================================================== */

nomen structura {
    f32 color[IV];
    f32 limen_alpha;
    i32 signa;
} Proprietates;

/* color, signum, invisibilis solidi */
interior b32
_proprietates (
               Plastes* p,
    constans StmlNodus* nodus,
          Proprietates* q)
{
    constans chorda* valor;

    q->color[ZEPHYRUM]  = 1.0f;
    q->color[I]         = 1.0f;
    q->color[II]        = 1.0f;
    q->color[III]       = 1.0f;
    q->limen_alpha      = -1.0f;
    q->signa            = ZEPHYRUM;
    valor               = stml_attributum_capere(nodus, "color");
    si (valor && !_color(valor, q->color))
    {
        redde _recusare(p, nodus->linea,
            "attribute 'color' must be #rgb or #rrggbb, got '", valor,
            "'");
    }
    valor = stml_attributum_capere(nodus, "signum");
    si (valor)
    {
        si (_titulus_est(valor, "solidum"))
        {
            q->signa = STATUA_SIGNUM_SOLIDUM;
        }
        alioquin si (_titulus_est(valor, "gressibile"))
        {
            q->signa = STATUA_SIGNUM_GRESSIBILE;
        }
        alioquin
        {
            redde _recusare(p, nodus->linea,
                "attribute 'signum' must be 'solidum' or "
                "'gressibile', got '", valor, "'");
        }
    }
    valor = stml_attributum_capere(nodus, "invisibilis");
    si (valor)
    {
        si (!_titulus_est(valor, "verum"))
        {
            redde _recusare(p, nodus->linea,
                "attribute 'invisibilis' must be 'verum' or absent, "
                "got '", valor, "'");
        }
        q->color[III]   = 0.0f;
        q->limen_alpha  = 0.5f;
    }
    redde VERUM;
}

/* rete, partem et materiam solidi aperire: vertices et indices
 * ab offset currenti; redde indicem verticis primi */
interior i32
_solidum_aperire (
                   Plastes* p,
                       i32  nodus,
     constans Proprietates* q,
                       i32  verticum,
                       i32  indicum)
{
           Statua* s       = p->statua;
              i32  k       = p->solidorum;
              i32  primus  = p->verticum;
    StatuaMateria* materia;

    s->nodi[nodus].rete        = (s32)k;
    s->nodi[nodus].signa       = q->signa;
    s->retia[k].pars_prima     = k;
    s->retia[k].partium        = I;
    s->partes[k].index_primus  = p->indicum;
    s->partes[k].indicum       = indicum;
    s->partes[k].materia       = (s32)k;
    materia                    = &s->materiae[k];
    memset(materia, ZEPHYRUM, magnitudo(*materia));
    materia->textura      = -I;
    memcpy(materia->color, q->color, magnitudo(materia->color));
    materia->limen_alpha  = q->limen_alpha;
    materia->repetit_s    = VERUM;
    materia->repetit_t    = VERUM;
    (vacuum)verticum;
    redde primus;
}

/* nodus liberum elementum habet? (solidum liberos non fert) */
interior b32
_habet_elementa (
    constans StmlNodus* nodus)
{
    i32 numerus = nodus->liberi ? xar_numerus(nodus->liberi)
        : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        StmlNodus** locus =
            (StmlNodus**)xar_obtinere(nodus->liberi, k);

        si (   locus && *locus
            && (*locus)->genus == STML_NODUS_ELEMENTUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* facies capsae: normalis et duo axes tangentes (u x v = normalis),
 * ut anguli (-u -v) (+u -v) (+u +v) (-u +v) extrorsum visi contra
 * horologium eant */
hic_manens constans s32 CAPSAE_FACIES[VI][IX] = {
    {  1,  0,  0,   0, 1, 0,   0, 0, 1 },
    { -1,  0,  0,   0, 0, 1,   0, 1, 0 },
    {  0,  1,  0,   0, 0, 1,   1, 0, 0 },
    {  0, -1,  0,   1, 0, 0,   0, 0, 1 },
    {  0,  0,  1,   1, 0, 0,   0, 1, 0 },
    {  0,  0, -1,   0, 1, 0,   1, 0, 0 }
};

interior b32
_capsa (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "magnitudo", "locus", "gyrus", "rotatio", "color",
        "signum", "invisibilis", NIHIL
    };
    hic_manens constans s32 ANGULI[IV][II] = {
        { -1, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 }
    };
    hic_manens constans i32  ORDO[VI] = { 0, 1, 2, 0, 2, 3 };
         StereometriaVector  translatio;
      StereometriaQuaternio  rotatio;
               Proprietates  q;
            constans chorda* valor;
                        f32  dimidia[III];
                        i32  n;
                        i32  primus;
                        i32  f;
                        i32  a;
                        i32  k;

    si (!p->gradus)
    {
        si (!_attributa_licita(p, nodus, LICITA))
        {
            redde FALSUM;
        }
        si (_habet_elementa(nodus))
        {
            redde _recusare(p, nodus->linea,
                "<capsa> is a solid and cannot have children", NIHIL,
                NIHIL);
        }
    }
    valor = stml_attributum_capere(nodus, "magnitudo");
    si (   !valor || !_numeri(valor, dimidia, III)
        || !(dimidia[ZEPHYRUM] > 0.0f) || !(dimidia[I] > 0.0f)
        || !(dimidia[II] > 0.0f))
    {
        redde _recusare(p, nodus->linea,
            "<capsa> needs attribute 'magnitudo' with 3 positive "
            "numbers (x y z)", NIHIL, NIHIL);
    }
    si (   !_transformatio(p, nodus, &translatio, &rotatio)
        || !_proprietates(p, nodus, &q))
    {
        redde FALSUM;
    }
    n = _nodus_novus(p, nodus, parens, translatio, rotatio);
    si (p->gradus)
    {
        Statua* s = p->statua;

        primus = _solidum_aperire(p, n, &q, CAPSAE_VERTICES,
            CAPSAE_INDICES);
        per (a = ZEPHYRUM; a < III; a++)
        {
            dimidia[a] *= 0.5f;
        }
        per (f = ZEPHYRUM; f < VI; f++)
        {
            constans s32* facies = CAPSAE_FACIES[f];

            per (k = ZEPHYRUM; k < IV; k++)
            {
                i32  v         = primus + f * IV + k;
                f32* positio   = s->positiones + v * III;
                f32* normalis  = s->normales + v * III;

                per (a = ZEPHYRUM; a < III; a++)
                {
                    positio[a] = dimidia[a]
                        * (f32)(facies[a]
                            + ANGULI[k][ZEPHYRUM] * facies[III + a]
                            + ANGULI[k][I] * facies[VI + a]);
                    normalis[a] = (f32)facies[a];
                }
                /* basis in humo: centrum ad dimidiam altitudinem */
                positio[I] += dimidia[I];
            }
            per (k = ZEPHYRUM; k < VI; k++)
            {
                s->indices[p->indicum + f * VI + k] =
                    primus + f * IV + ORDO[k];
            }
        }
    }
    p->solidorum++;
    p->verticum  += CAPSAE_VERTICES;
    p->indicum   += CAPSAE_INDICES;
    redde VERUM;
}


/* ==================================================
 * Ambulatio
 * ================================================== */

interior b32
_liberi (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens);

interior b32
_grex (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "locus", "gyrus", "rotatio", NIHIL
    };
       StereometriaVector translatio;
    StereometriaQuaternio rotatio;
                      i32 n;

    si (   (!p->gradus && !_attributa_licita(p, nodus, LICITA))
        || !_transformatio(p, nodus, &translatio, &rotatio))
    {
        redde FALSUM;
    }
    n = _nodus_novus(p, nodus, parens, translatio, rotatio);
    redde _liberi(p, nodus, (s32)n);
}

interior b32
_textus_vacuus (
    constans chorda* valor)
{
    i32 k;

    si (!valor)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < valor->mensura; k++)
    {
        si (!_spatium(valor->datum[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_liberi (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    i32 numerus = nodus->liberi ? xar_numerus(nodus->liberi)
        : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        StmlNodus** locus =
            (StmlNodus**)xar_obtinere(nodus->liberi, k);
        constans StmlNodus* liber = locus ? *locus : NIHIL;

        si (!liber || liber->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        si (liber->genus == STML_NODUS_TEXTUS)
        {
            si (!_textus_vacuus(liber->valor))
            {
                redde _recusare(p, liber->linea,
                    "unexpected text between elements", NIHIL,
                    NIHIL);
            }
            perge;
        }
        si (liber->genus != STML_NODUS_ELEMENTUM)
        {
            redde _recusare(p, liber->linea,
                "only elements and comments are allowed here", NIHIL,
                NIHIL);
        }
        si (_titulus_est(liber->titulus, "grex"))
        {
            si (!_grex(p, liber, parens))
            {
                redde FALSUM;
            }
        }
        alioquin si (_titulus_est(liber->titulus, "capsa"))
        {
            si (!_capsa(p, liber, parens))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            redde _recusare(p, liber->linea, "unknown element <",
                liber->titulus, ">");
        }
    }
    redde VERUM;
}

interior b32
_radix (
               Plastes* p,
    constans StmlNodus* radix)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", NIHIL
    };
    i32 n;

    p->nodorum    = ZEPHYRUM;
    p->solidorum  = ZEPHYRUM;
    p->verticum   = ZEPHYRUM;
    p->indicum    = ZEPHYRUM;
    si (!p->gradus)
    {
        constans chorda* titulus;

        si (!_titulus_est(radix->titulus, "res"))
        {
            redde _recusare(p, radix->linea,
                "the root element must be <res>, got <",
                radix->titulus, ">");
        }
        si (!_attributa_licita(p, radix, LICITA))
        {
            redde FALSUM;
        }
        titulus = stml_attributum_capere(radix, "titulus");
        si (!titulus || titulus->mensura == ZEPHYRUM)
        {
            redde _recusare(p, radix->linea,
                "<res> needs a non-empty attribute 'titulus'", NIHIL,
                NIHIL);
        }
    }
    n = _nodus_novus(p, radix, -I,
        stereometria_vector(0.0f, 0.0f, 0.0f),
        stereometria_quaternio_identitas());
    si (!_liberi(p, radix, (s32)n))
    {
        redde FALSUM;
    }
    si (!p->gradus && p->solidorum == ZEPHYRUM)
    {
        redde _recusare(p, radix->linea, "<res> has no solids",
            NIHIL, NIHIL);
    }
    redde VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

interior vacuum*
_locus (
           Piscina* piscina,
    memoriae_index  mensura)
{
    vacuum* p = piscina_allocare(piscina, mensura ? mensura : I);

    si (p)
    {
        memset(p, ZEPHYRUM, mensura ? mensura : I);
    }
    redde p;
}

interior PlasticaFructus
_fructus_recusatus (
    constans Plastes* p,
             Piscina* piscina)
{
    PlasticaFructus f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.error = chorda_ex_literis(p->error, piscina);
    f.linea = p->linea;
    redde f;
}

PlasticaFructus
plastica_legere (
     chorda  textus,
    Piscina* piscina)
{
    PlasticaFructus  f;
            Plastes  p;
       StmlResultus  r;
             Statua* s;

    memset(&f, ZEPHYRUM, magnitudo(f));
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.piscina = piscina;
    r = stml_legere(textus, piscina, internamentum_creare(piscina));
    si (!r.successus || !r.elementum_radix)
    {
        chorda causa = r.error;

        (vacuum)_recusare(&p, r.successus ? ZEPHYRUM : r.linea_erroris,
            "the description is not valid STML: ",
            causa.mensura ? &causa : NIHIL,
            causa.mensura ? NIHIL : "no root element");
        redde _fructus_recusatus(&p, piscina);
    }
    p.gradus = ZEPHYRUM;
    si (!_radix(&p, r.elementum_radix))
    {
        redde _fructus_recusatus(&p, piscina);
    }

    s = (Statua*)_locus(piscina, magnitudo(Statua));
    si (s)
    {
        s->nodorum      = p.nodorum;
        s->nodi         = (StatuaNodus*)_locus(piscina,
            p.nodorum * magnitudo(StatuaNodus));
        s->retium       = p.solidorum;
        s->retia        = (StatuaRete*)_locus(piscina,
            p.solidorum * magnitudo(StatuaRete));
        s->partium      = p.solidorum;
        s->partes       = (StatuaPars*)_locus(piscina,
            p.solidorum * magnitudo(StatuaPars));
        s->materiarum   = p.solidorum;
        s->materiae     = (StatuaMateria*)_locus(piscina,
            p.solidorum * magnitudo(StatuaMateria));
        s->verticum     = p.verticum;
        s->positiones   = (f32*)_locus(piscina,
            p.verticum * III * magnitudo(f32));
        s->normales     = (f32*)_locus(piscina,
            p.verticum * III * magnitudo(f32));
        s->coordinatae  = (f32*)_locus(piscina,
            p.verticum * II * magnitudo(f32));
        s->indicum      = p.indicum;
        s->indices      = (i32*)_locus(piscina,
            p.indicum * magnitudo(i32));
        p.partes        = (chorda*)_locus(piscina,
            p.nodorum * magnitudo(chorda));
    }
    si (   !s || !s->nodi || !s->retia || !s->partes || !s->materiae
        || !s->positiones || !s->normales || !s->coordinatae
        || !s->indices || !p.partes)
    {
        (vacuum)_recusare(&p, ZEPHYRUM, "out of memory", NIHIL,
            NIHIL);
        redde _fructus_recusatus(&p, piscina);
    }
    p.statua = s;
    p.gradus = I;
    si (!_radix(&p, r.elementum_radix) || !statua_finire(s, piscina))
    {
        (vacuum)_recusare(&p, ZEPHYRUM,
            "the model could not be finished", NIHIL, NIHIL);
        redde _fructus_recusatus(&p, piscina);
    }
    f.successus  = VERUM;
    f.statua     = s;
    f.partes     = p.partes;
    redde f;
}
