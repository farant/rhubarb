/*
 * plastica_motus.c - figura cum motu (plastica_motus.h)
 *
 * Tres gradus: plastica_legere corpus dat (nodi, articuli, quies);
 * plagula iterum legitur et positurae cyclique in tabulas colliguntur;
 * coctio eos in canales Statuae vertit. Statua nova valorem prioris
 * transcribit et tabulas eius mutuatur; cycli soli novi sunt.
 */
#include "plastica_motus.h"
#include "stml.h"
#include "internamentum.h"
#include "xar.h"
#include "stereometria.h"
#include <math.h>
#include <string.h>

#define ERRORIS_MENSURA  CCLVI
#define DURATIO_MAXIMA   60.0f
#define GRADUS_DIMIDII   180.0f
#define GRADUS_PLENI     360.0f
/* sutura: 180 modulo 360 ambigua est */
#define GRADUS_MARGO     1.0e-3f

#define TACTA_ROTATIO  I
#define TACTA_SCALA    II
#define TACTA_MOTUS    IV

/* linea nodi ex plagula ossium in arbore composita (plastica eam ut
 * datam reddit) */
#define LINEA_OSSIUM     ((i32)0x40000000)
#define QUIES_MARGO      1.0e-4f
#define QUIES_ROTATIO    0.999999f
#define TITULI_MENSURA   LXIV


/* quid positura de articulo uno dicat */
nomen structura {
    i32 tacta;          /* TACTA_*; 0 = quies */
    f32 rotatio[III];   /* gradus, ad quietem additi */
    f32 scala[III];
    f32 motus[III];
} Artus;

nomen structura {
    chorda  titulus;
     Artus* artus;      /* articulorum */
       b32  ossium;     /* ex plagula ossium: figura substituere
                         * potest */
} Positura;

nomen structura {
    i32 positura;
    f32 tempus;
    i32 linea;
    b32 sutura;         /* clavis addita quae circulum claudit */
} Clavis;

nomen structura {
    chorda titulus;
       f32 duratio;
       i32 linea;
       b32 ossium;
    Clavis claves[PLASTICA_MOTUS_CLAVES_MAXIMAE + I];
       i32 clavium;
    chorda momenta[PLASTICA_MOTUS_MOMENTA_MAXIMA];
       f32 tempora[PLASTICA_MOTUS_MOMENTA_MAXIMA];
       i32 momentorum;
} Cyclus;

nomen structura {
            Piscina* piscina;
    constans Statua* basis;
    constans chorda* partes;
       constans i32* articuli;
                i32  articulorum;
           Positura  positurae[PLASTICA_MOTUS_POSITURAE_MAXIMAE + I];
                i32  positurarum;   /* prima "quies" est */
             Cyclus* cycli;         /* PLASTICA_MOTUS_CYCLI_MAXIMI */
                i32  cyclorum;
                b32  fractum;
                i32  linea;
          character  error[ERRORIS_MENSURA];
             /* titulus ossium dum ossa leguntur; vacua in figura */
    chorda plagula;
    chorda plagula_erroris;
       b32 in_ossibus;
    /* figura posituram ossium substituit */
       b32 substitutum;
    /* motus positurae ossium per hunc multiplicatur */
       f32 factor[PLASTICA_ARTICULI_MAXIMI];

       i32 par[PLASTICA_ARTICULI_MAXIMI];
    /* articuli ossium, ordine arboris; et articulus figurae pro
     * quoque (NIHIL si non nominatus) */
    constans StmlNodus* ossa[PLASTICA_ARTICULI_MAXIMI];
    constans StmlNodus* solida[PLASTICA_ARTICULI_MAXIMI];
                   i32  ossium;
} Motor;


/* ==================================================
 * Recusatio
 * ================================================== */

interior vacuum
_appone (
                 Motor* m,
                   i32* longitudo,
    constans character* fons,
                   i32  mensura)
{
    i32 k;

    per (k = ZEPHYRUM;
         k < mensura && *longitudo + I < ERRORIS_MENSURA; k++)
    {
        character littera = fons[k];

        si (littera == '\n' || littera == '\r' || littera == '\t')
        {
            littera = ' ';
        }
        m->error[(*longitudo)++] = littera;
    }
    m->error[*longitudo] = '\0';
}

/* nuntius = ante + medium + post; prima recusatio manet */
interior b32
_recusare (
                 Motor* m,
                   i32  linea,
    constans character* ante,
       constans chorda* medium,
    constans character* post)
{
    i32 longitudo = ZEPHYRUM;

    si (m->fractum)
    {
        redde FALSUM;
    }
    m->fractum          = VERUM;
    m->linea            = linea;
    m->plagula_erroris  = m->plagula;
    si (ante)
    {
        _appone(m, &longitudo, ante, (i32)strlen(ante));
    }
    si (medium && medium->datum)
    {
        _appone(m, &longitudo, (constans character*)medium->datum,
            medium->mensura > LXIV ? LXIV : medium->mensura);
    }
    si (post)
    {
        _appone(m, &longitudo, post, (i32)strlen(post));
    }
    redde FALSUM;
}

/* "<elementum> attribute 'titulus' quid" */
interior b32
_recusare_attributum (
                 Motor* m,
    constans StmlNodus* nodus,
    constans character* titulus,
    constans character* quid)
{
    i32 longitudo = ZEPHYRUM;

    si (m->fractum)
    {
        redde FALSUM;
    }
    m->fractum          = VERUM;
    m->linea            = nodus->linea;
    m->plagula_erroris  = m->plagula;
    _appone(m, &longitudo, "<", I);
    si (nodus->titulus && nodus->titulus->datum)
    {
        _appone(m, &longitudo,
            (constans character*)nodus->titulus->datum,
            nodus->titulus->mensura > LXIV ? LXIV
                : nodus->titulus->mensura);
    }
    _appone(m, &longitudo, "> attribute '", XIII);
    _appone(m, &longitudo, titulus, (i32)strlen(titulus));
    _appone(m, &longitudo, "' ", II);
    _appone(m, &longitudo, quid, (i32)strlen(quid));
    redde FALSUM;
}


/* ==================================================
 * Valores
 * ================================================== */

interior b32
_est (
       constans chorda* titulus,
    constans character* literae)
{
    redde titulus && chorda_aequalis_literis(*titulus, literae);
}

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
        i32 initium;
        f64 x;

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
        si (   n >= numerus
            || !chorda_ut_f64(chorda_sectio(*valor, initium, k), &x)
            || !(x == x) || x > 1.0e6 || x < -1.0e6)
        {
            redde FALSUM;
        }
        exitus[n++] = (f32)x;
    }
    redde n == numerus;
}

interior b32
_attributa_licita (
                           Motor* m,
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
            si (_est(a->titulus, licita[j]))
            {
                notum = VERUM;
                frange;
            }
        }
        si (!notum)
        {
            redde _recusare(m, nodus->linea, "unknown attribute '",
                a ? a->titulus : NIHIL, "'");
        }
    }
    redde VERUM;
}

/* liberum k-esimum quod elementum est; *finis VERUM post ultimum.
 * Commenta et spatia praetereuntur; textus alius recusatur */
interior constans StmlNodus*
_liberum (
                 Motor* m,
    constans StmlNodus* nodus,
                   i32* cursor,
                   b32* finis)
{
    i32 numerus = nodus->liberi ? xar_numerus(nodus->liberi)
        : ZEPHYRUM;

    *finis = FALSUM;
    dum (*cursor < numerus)
    {
                StmlNodus** locus =
                    (StmlNodus**)xar_obtinere(nodus->liberi, *cursor);
        constans StmlNodus* liber = locus ? *locus : NIHIL;
                       i32  k;

        (*cursor)++;
        si (!liber || liber->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        si (liber->genus == STML_NODUS_ELEMENTUM)
        {
            redde liber;
        }
        per (k = ZEPHYRUM; liber->valor && k < liber->valor->mensura;
             k++)
        {
            si (!_spatium(liber->valor->datum[k]))
            {
                (vacuum)_recusare(m, nodus->linea, "<",
                    nodus->titulus,
                    "> has unexpected text between its elements");
                redde NIHIL;
            }
        }
    }
    *finis = VERUM;
    redde NIHIL;
}

/* elementum liberos elementa non habet */
interior b32
_sine_liberis (
                 Motor* m,
    constans StmlNodus* nodus)
{
    i32 cursor = ZEPHYRUM;
    b32 finis;

    si (_liberum(m, nodus, &cursor, &finis))
    {
        redde _recusare(m, nodus->linea, "<", nodus->titulus,
            "> cannot hold other elements");
    }
    redde finis;
}

/* index articuli (in tabula articulorum) titulo; -1 si nullus */
interior s32
_articulus (
       constans Motor* m,
      constans chorda* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->articulorum; k++)
    {
        si (chorda_aequalis(m->partes[m->articuli[k]], *titulus))
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* index positurae titulo; -1 si nulla */
interior s32
_positura (
       constans Motor* m,
      constans chorda* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->positurarum; k++)
    {
        si (chorda_aequalis(m->positurae[k].titulus, *titulus))
        {
            redde (s32)k;
        }
    }
    redde -I;
}


/* ==================================================
 * Positurae
 * ================================================== */

interior b32
_artum_legere (
                 Motor* m,
    constans StmlNodus* nodus,
                 Artus* artus,
                   b32* visi)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "rotatio", "scala", "motus", NIHIL
    };
    constans chorda* titulus =
        stml_attributum_capere(nodus, "titulus");
    constans chorda* rotatio =
        stml_attributum_capere(nodus, "rotatio");
    constans chorda* scala = stml_attributum_capere(nodus, "scala");
    constans chorda* motus = stml_attributum_capere(nodus, "motus");
              Artus* a;
                s32  k;

    si (   !_attributa_licita(m, nodus, LICITA)
        || !_sine_liberis(m, nodus))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed (the joint's title)");
    }
    k = _articulus(m, titulus);
    si (k < ZEPHYRUM)
    {
        redde _recusare(m, nodus->linea,
            "<art> names an unknown joint '",
            titulus, "'");
    }
    si (visi[k])
    {
        redde _recusare(m, nodus->linea, "joint '", titulus,
            "' appears twice in this pose");
    }
    visi[k] = VERUM;
    si (!rotatio && !scala && !motus)
    {
        redde _recusare(m, nodus->linea, "<art> needs at least one of"
            " 'rotatio', 'scala', 'motus'", NIHIL, NIHIL);
    }
    a = &artus[k];
    si (rotatio)
    {
        si (!_numeri(rotatio, a->rotatio, III))
        {
            redde _recusare_attributum(m, nodus, "rotatio",
                "needs 3 numbers (degrees about x y z)");
        }
        a->tacta |= TACTA_ROTATIO;
    }
    si (scala)
    {
        si (_numeri(scala, a->scala, I))
        {
            a->scala[I]   = a->scala[ZEPHYRUM];
            a->scala[II]  = a->scala[ZEPHYRUM];
        }
        alioquin si (!_numeri(scala, a->scala, III))
        {
            redde _recusare_attributum(m, nodus, "scala",
                "needs 1 or 3 numbers above 0");
        }
        si (   !(a->scala[ZEPHYRUM] > 0.0f) || !(a->scala[I] > 0.0f)
            || !(a->scala[II] > 0.0f))
        {
            redde _recusare_attributum(m, nodus, "scala",
                "needs 1 or 3 numbers above 0");
        }
        a->tacta |= TACTA_SCALA;
    }
    si (motus)
    {
        si (m->basis->nodi[m->articuli[k]].parens != ZEPHYRUM)
        {
            redde _recusare_attributum(m, nodus, "motus",
                "is only allowed on a joint directly under <figura>");
        }
        si (!_numeri(motus, a->motus, III))
        {
            redde _recusare_attributum(m, nodus, "motus",
                "needs 3 numbers (x y z)");
        }
        si (m->in_ossibus)
        {
            a->motus[ZEPHYRUM]  *= m->factor[k];
            a->motus[I]         *= m->factor[k];
            a->motus[II]        *= m->factor[k];
        }
        a->tacta |= TACTA_MOTUS;
    }
    redde VERUM;
}

/* titulus in suffixo desinit et praefixum non vacuum habet */
interior b32
_desinit (
    constans chorda* titulus,
 constans character* suffixum)
{
    i32 mensura = (i32)strlen(suffixum);

    redde titulus->mensura > mensura
        && memcmp(titulus->datum + titulus->mensura - mensura,
               suffixum, mensura) == ZEPHYRUM;
}

/* quies paris imago est: locus trans x = 0, rotatio reflexa */
interior b32
_quies_imago (
    constans StatuaNodus* a,
    constans StatuaNodus* b)
{
    f32 punctum =
        a->rotatio.x * b->rotatio.x - a->rotatio.y * b->rotatio.y
        - a->rotatio.z * b->rotatio.z + a->rotatio.w * b->rotatio.w;

    redde fabs((f64)(a->translatio.x + b->translatio.x)) <= QUIES_MARGO
        && fabs((f64)(a->translatio.y - b->translatio.y))
            <= QUIES_MARGO
        && fabs((f64)(a->translatio.z - b->translatio.z))
            <= QUIES_MARGO
        && fabs((f64)punctum) >= QUIES_ROTATIO;
}

/* par cuiusque articuli ("X_sin" cum "X_dex"; ceteri sibi ipsi), et
 * quies paris imago */
interior b32
_paria_parare (
    Motor* m,
      i32  linea)
{
    character post[TITULI_MENSURA + XXXII];
          i32 j;
          i32 k;

    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        constans chorda* titulus   = &m->partes[m->articuli[j]];
                    b32  sinister  = _desinit(titulus, "_sin");
                    i32  radix     = titulus->mensura - IV;

        m->par[j] = j;
        si (!sinister && !_desinit(titulus, "_dex"))
        {
            perge;
        }
        per (k = ZEPHYRUM; k < m->articulorum; k++)
        {
            constans chorda* alter = &m->partes[m->articuli[k]];

            si (   alter->mensura == titulus->mensura
                && _desinit(alter, sinister ? "_dex" : "_sin")
                && memcmp(alter->datum, titulus->datum, radix)
                    == ZEPHYRUM)
            {
                frange;
            }
        }
        si (k >= m->articulorum)
        {
            /* "' has no partner 'X_dex'" */
            i32 n = radix > TITULI_MENSURA ? TITULI_MENSURA : radix;

            memcpy(post, "' has no partner '", XVIII);
            memcpy(post + XVIII, titulus->datum, n);
            memcpy(post + XVIII + n, sinister ? "_dex'" : "_sin'",
                VI);
            redde _recusare(m, linea, "joint '", titulus, post);
        }
        m->par[j] = k;
    }
    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        constans StatuaNodus* a = &m->basis->nodi[m->articuli[j]];
        constans StatuaNodus* b =
            &m->basis->nodi[m->articuli[m->par[j]]];
                         s32 parens = ZEPHYRUM;

        si (m->par[j] == j)
        {
            perge;
        }
        /* parens paris par parentis sit (radix sibi ipsa) */
        per (k = ZEPHYRUM; k < m->articulorum; k++)
        {
            si ((s32)m->articuli[k] == a->parens)
            {
                parens = (s32)m->articuli[m->par[k]];
            }
        }
        si (b->parens != parens || !_quies_imago(a, b))
        {
            redde _recusare(m, linea, "joint '",
                &m->partes[m->articuli[j]],
                "' and its partner are not mirror images at rest"
                " (locus, turn or parent)");
        }
    }
    redde VERUM;
}

/* imago positurae trans x = 0 in 'exitus' */
interior vacuum
_speculare (
     constans Motor* m,
     constans Artus* fons,
              Artus* exitus)
{
    i32 j;

    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        Artus* a = &exitus[m->par[j]];

        *a                  = fons[j];
        a->rotatio[I]       = -a->rotatio[I];
        a->rotatio[II]      = -a->rotatio[II];
        a->motus[ZEPHYRUM]  = -a->motus[ZEPHYRUM];
    }
}

interior b32
_positura_legere (
                 Motor* m,
    constans StmlNodus* nodus)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "ex", "speculum", NIHIL
    };
       constans chorda* titulus =
           stml_attributum_capere(nodus, "titulus");
       constans chorda* origo = stml_attributum_capere(nodus, "ex");
       constans chorda* speculum =
           stml_attributum_capere(nodus, "speculum");
    constans StmlNodus* liber;
              Positura* p;
                 Artus* artus;
                   b32  visi[PLASTICA_ARTICULI_MAXIMI];
                   b32  finis;
                   i32  cursor = ZEPHYRUM;
                   s32  locus;

    si (!_attributa_licita(m, nodus, LICITA))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed and cannot be empty");
    }
    si (_est(titulus, "quies"))
    {
        redde _recusare(m, nodus->linea, "the pose 'quies' is built in"
            " (the rest pose) and cannot be written", NIHIL, NIHIL);
    }
    /* figura posituram ossium eiusdem tituli substituit */
    locus = _positura(m, titulus);
    si (   locus >= ZEPHYRUM
        && (m->in_ossibus || !m->positurae[locus].ossium))
    {
        redde _recusare(m, nodus->linea, "pose title '", titulus,
            "' is used twice");
    }
    si (   locus < ZEPHYRUM
        && m->positurarum > PLASTICA_MOTUS_POSITURAE_MAXIMAE)
    {
        redde _recusare(m, nodus->linea,
            "<positura> is one pose too many (at most 64)", NIHIL,
            NIHIL);
    }
    artus = (Artus*)piscina_allocare(m->piscina,
        (m->articulorum ? m->articulorum : I) * magnitudo(Artus));
    si (!artus)
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    memset(artus, ZEPHYRUM,
        (m->articulorum ? m->articulorum : I) * magnitudo(Artus));
    si (origo && speculum)
    {
        redde _recusare(m, nodus->linea, "<positura>: 'speculum' and"
            " 'ex' cannot be used together", NIHIL, NIHIL);
    }
    si (origo)
    {
        s32 k = _positura(m, origo);

        si (k < ZEPHYRUM)
        {
            redde _recusare_attributum(m, nodus, "ex",
                "names a pose that is not written above this one");
        }
        memcpy(artus, m->positurae[k].artus,
            m->articulorum * magnitudo(Artus));
    }
    si (speculum)
    {
        s32 k = _positura(m, speculum);

        si (k < ZEPHYRUM)
        {
            redde _recusare_attributum(m, nodus, "speculum",
                "names a pose that is not written above this one");
        }
        si (!_paria_parare(m, nodus->linea))
        {
            redde FALSUM;
        }
        _speculare(m, m->positurae[k].artus, artus);
    }
    memset(visi, ZEPHYRUM, magnitudo(visi));
    dum ((liber = _liberum(m, nodus, &cursor, &finis)) != NIHIL)
    {
        si (!_est(liber->titulus, "art"))
        {
            redde _recusare(m, liber->linea,
                "<positura> holds only <art>, got <", liber->titulus,
                ">");
        }
        si (!_artum_legere(m, liber, artus, visi))
        {
            redde FALSUM;
        }
    }
    si (!finis)
    {
        redde FALSUM;
    }
    si (locus < ZEPHYRUM)
    {
        locus = (s32)m->positurarum++;
    }
    alioquin
    {
        m->substitutum = VERUM;
    }
    p           = &m->positurae[locus];
    p->titulus  = *titulus;
    p->artus    = artus;
    p->ossium   = m->in_ossibus;
    redde VERUM;
}


/* ==================================================
 * Cycli
 * ================================================== */

/* differentia graduum inter claves vicinas CLXXX attingit? Sutura
 * modulo CCCLX confertur */
interior b32
_nimis_vertit (
    f32 prior,
    f32 sequens,
    b32 sutura)
{
    f32 d = sequens - prior;

    si (sutura)
    {
        d -= GRADUS_PLENI
            * (f32)floor((f64)((d + GRADUS_DIMIDII) / GRADUS_PLENI));
        redde d < -GRADUS_DIMIDII + GRADUS_MARGO;
    }
    redde d >= GRADUS_DIMIDII || d <= -GRADUS_DIMIDII;
}

/* claves vicinae: nullus articulus in ullo axe CLXXX gradus vertit */
interior b32
_vias_probare (
               Motor* m,
     constans Cyclus* c)
{
    i32 k;
    i32 j;
    i32 a;

    per (k = I; k < c->clavium; k++)
    {
        constans Artus* prior =
            m->positurae[c->claves[k - I].positura].artus;
        constans Artus* sequens =
            m->positurae[c->claves[k].positura].artus;

        per (j = ZEPHYRUM; j < m->articulorum; j++)
        {
            per (a = ZEPHYRUM; a < III; a++)
            {
                si (_nimis_vertit(prior[j].rotatio[a],
                    sequens[j].rotatio[a], c->claves[k].sutura))
                {
                    redde _recusare(m, c->claves[k].linea,
                        "joint '", &m->partes[m->articuli[j]],
                        "' turns 180 degrees or more since the"
                        " previous key: add a key between");
                }
            }
        }
    }
    redde VERUM;
}

interior b32
_clavis_legere (
                 Motor* m,
    constans StmlNodus* nodus,
                Cyclus* c)
{
    hic_manens constans character* constans LICITA[] = {
        "tempus", "positura", NIHIL
    };
    constans chorda* tempus = stml_attributum_capere(nodus, "tempus");
    constans chorda* positura =
        stml_attributum_capere(nodus, "positura");
             Clavis* clavis;
                f32  t;
                s32  k;

    si (   !_attributa_licita(m, nodus, LICITA)
        || !_sine_liberis(m, nodus))
    {
        redde FALSUM;
    }
    si (!tempus || !_numeri(tempus, &t, I))
    {
        redde _recusare_attributum(m, nodus, "tempus",
            "needs 1 number (seconds)");
    }
    si (!positura)
    {
        redde _recusare_attributum(m, nodus, "positura",
            "is needed (a pose title, or 'quies')");
    }
    k = _positura(m, positura);
    si (k < ZEPHYRUM)
    {
        redde _recusare(m, nodus->linea,
            "<clavis> names an unknown pose '", positura, "'");
    }
    si (c->clavium >= PLASTICA_MOTUS_CLAVES_MAXIMAE)
    {
        redde _recusare(m, nodus->linea,
            "<clavis> is one key too many (at most 64 in a cycle)",
            NIHIL, NIHIL);
    }
    si (c->clavium == ZEPHYRUM && t != 0.0f)
    {
        redde _recusare(m, nodus->linea,
            "the first key of a cycle must be at tempus 0", NIHIL,
            NIHIL);
    }
    si (c->clavium && !(t > c->claves[c->clavium - I].tempus))
    {
        redde _recusare(m, nodus->linea,
            "key times must be increasing", NIHIL, NIHIL);
    }
    si (t > c->duratio)
    {
        redde _recusare(m, nodus->linea,
            "<clavis> is later than the cycle's 'duratio'", NIHIL,
            NIHIL);
    }
    clavis            = &c->claves[c->clavium++];
    clavis->positura  = (i32)k;
    clavis->tempus    = t;
    clavis->linea     = nodus->linea;
    clavis->sutura    = FALSUM;
    redde VERUM;
}

interior b32
_momentum_legere (
                 Motor* m,
    constans StmlNodus* nodus,
                Cyclus* c)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "tempus", NIHIL
    };
    constans chorda* titulus =
        stml_attributum_capere(nodus, "titulus");
    constans chorda* tempus = stml_attributum_capere(nodus, "tempus");
                f32  t;
                i32  k;

    si (   !_attributa_licita(m, nodus, LICITA)
        || !_sine_liberis(m, nodus))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed and cannot be empty");
    }
    si (   !tempus || !_numeri(tempus, &t, I) || t < 0.0f
        || t > c->duratio)
    {
        redde _recusare_attributum(m, nodus, "tempus",
            "needs 1 number from 0 to the cycle's 'duratio'");
    }
    per (k = ZEPHYRUM; k < c->momentorum; k++)
    {
        si (chorda_aequalis(c->momenta[k], *titulus))
        {
            redde _recusare(m, nodus->linea, "moment title '",
                titulus, "' is used twice in this cycle");
        }
    }
    si (c->momentorum >= PLASTICA_MOTUS_MOMENTA_MAXIMA)
    {
        redde _recusare(m, nodus->linea, "<momentum> is one moment"
            " too many (at most 8 in a cycle)", NIHIL, NIHIL);
    }
    c->momenta[c->momentorum] = *titulus;
    c->tempora[c->momentorum] = t;
    c->momentorum++;
    redde VERUM;
}

interior b32
_cyclus_legere (
                 Motor* m,
    constans StmlNodus* nodus)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "duratio", "circulus", NIHIL
    };
       constans chorda* titulus =
           stml_attributum_capere(nodus, "titulus");
       constans chorda* duratio =
           stml_attributum_capere(nodus, "duratio");
       constans chorda* circulus =
           stml_attributum_capere(nodus, "circulus");
    constans StmlNodus* liber;
                Cyclus* c;
                Clavis* ultima;
                   b32  circularis = FALSUM;
                   b32  finis;
                   i32  cursor     = ZEPHYRUM;
                   i32  locus;


    si (!_attributa_licita(m, nodus, LICITA))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed and cannot be empty");
    }
    /* figura cyclum ossium eiusdem tituli substituit, loco servato */
    per (locus = ZEPHYRUM; locus < m->cyclorum; locus++)
    {
        si (chorda_aequalis(m->cycli[locus].titulus, *titulus))
        {
            si (m->in_ossibus || !m->cycli[locus].ossium)
            {
                redde _recusare(m, nodus->linea, "cycle title '",
                    titulus, "' is used twice");
            }
            frange;
        }
    }
    si (   locus       >= m->cyclorum
        && m->cyclorum >= PLASTICA_MOTUS_CYCLI_MAXIMI)
    {
        redde _recusare(m, nodus->linea,
            "<cyclus> is one cycle too many (at most 16)", NIHIL,
            NIHIL);
    }
    c = &m->cycli[locus];
    memset(c, ZEPHYRUM, magnitudo(*c));
    c->titulus  = *titulus;
    c->linea    = nodus->linea;
    c->ossium   = m->in_ossibus;
    si (   !duratio || !_numeri(duratio, &c->duratio, I)
        || !(c->duratio > 0.0f) || c->duratio > DURATIO_MAXIMA)
    {
        redde _recusare_attributum(m, nodus, "duratio",
            "needs 1 number above 0, at most 60 (seconds)");
    }
    si (circulus)
    {
        si (_est(circulus, "verum"))
        {
            circularis = VERUM;
        }
        alioquin si (!_est(circulus, "falsum"))
        {
            redde _recusare_attributum(m, nodus, "circulus",
                "must be 'verum' or 'falsum'");
        }
    }
    dum ((liber = _liberum(m, nodus, &cursor, &finis)) != NIHIL)
    {
        si (_est(liber->titulus, "clavis"))
        {
            si (!_clavis_legere(m, liber, c))
            {
                redde FALSUM;
            }
        }
        alioquin si (_est(liber->titulus, "momentum"))
        {
            si (!_momentum_legere(m, liber, c))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            redde _recusare(m, liber->linea, "<cyclus> holds only"
                " <clavis> and <momentum>, got <", liber->titulus,
                ">");
        }
    }
    si (!finis)
    {
        redde FALSUM;
    }
    si (c->clavium == ZEPHYRUM)
    {
        redde _recusare(m, nodus->linea, "<cyclus> has no keys",
            NIHIL, NIHIL);
    }
    /* clavis ad durationem: circulus primam iterat, aliter ultima
     * tenetur */
    ultima = &c->claves[c->clavium - I];
    si (circularis)
    {
        si (!(ultima->tempus < c->duratio))
        {
            redde _recusare(m, ultima->linea, "in a looping cycle the"
                " last key must come before 'duratio' (the first pose"
                " is repeated there)", NIHIL, NIHIL);
        }
        c->claves[c->clavium].positura =
            c->claves[ZEPHYRUM].positura;
    }
    alioquin
    {
        c->claves[c->clavium].positura = ultima->positura;
    }
    si (circularis || ultima->tempus < c->duratio)
    {
        c->claves[c->clavium].tempus  = c->duratio;
        c->claves[c->clavium].linea   = nodus->linea;
        c->claves[c->clavium].sutura  = circularis;
        c->clavium++;
    }
    si (!_vias_probare(m, c))
    {
        redde FALSUM;
    }
    si (locus >= m->cyclorum)
    {
        m->cyclorum++;
    }
    redde VERUM;
}


/* ==================================================
 * Coctio
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
                          gradus * STEREOMETRIA_PI / GRADUS_PLENI;
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

/* quies articuli, deinde gradus positurae in spatio eius: circa x,
 * deinde y, deinde z */
interior StereometriaQuaternio
_rotatio_articuli (
    StereometriaQuaternio  quies,
             constans f32* gradus)
{
    StereometriaQuaternio q =
        _circa_axem(1.0f, 0.0f, 0.0f, gradus[ZEPHYRUM]);

    q = _componere(_circa_axem(0.0f, 1.0f, 0.0f, gradus[I]), q);
    q = _componere(_circa_axem(0.0f, 0.0f, 1.0f, gradus[II]), q);
    redde stereometria_quaternio_normalizare(_componere(quies, q));
}

/* canalis unus: articulus j, proprietas, clavis ad quodque tempus */
interior b32
_canalem_coquere (
               Motor* m,
     constans Cyclus* c,
                 i32  j,
                 i32  tactum,
       StatuaCanalis* canalis)
{
    constans StatuaNodus* nodus  = &m->basis->nodi[m->articuli[j]];
                     i32  lata   = tactum == TACTA_ROTATIO ? IV : III;
                     i32  k;

    canalis->nodus       = (s32)m->articuli[j];
    canalis->proprietas  = tactum == TACTA_ROTATIO ? STATUA_ROTATIO
        : tactum == TACTA_SCALA ? STATUA_SCALA : STATUA_TRANSLATIO;
    canalis->gradatim  = FALSUM;
    canalis->clavium   = c->clavium;
    canalis->tempora     = (f32*)piscina_allocare(m->piscina,
        c->clavium * magnitudo(f32));
    canalis->valores     = (f32*)piscina_allocare(m->piscina,
        c->clavium * lata * magnitudo(f32));
    si (!canalis->tempora || !canalis->valores)
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    per (k = ZEPHYRUM; k < c->clavium; k++)
    {
        constans Artus* a =
            &m->positurae[c->claves[k].positura].artus[j];
                  f32* v = canalis->valores + k * lata;

        canalis->tempora[k] = c->claves[k].tempus;
        si (tactum == TACTA_ROTATIO)
        {
            StereometriaQuaternio q =
                _rotatio_articuli(nodus->rotatio, a->rotatio);

            v[ZEPHYRUM]  = q.x;
            v[I]         = q.y;
            v[II]        = q.z;
            v[III]       = q.w;
        }
        alioquin si (tactum == TACTA_SCALA)
        {
            b32 tacta = (a->tacta & TACTA_SCALA) != ZEPHYRUM;

            v[ZEPHYRUM]  = nodus->scala.x
                * (tacta ? a->scala[ZEPHYRUM] : 1.0f);
            v[I]         = nodus->scala.y
                * (tacta ? a->scala[I] : 1.0f);
            v[II]        = nodus->scala.z
                * (tacta ? a->scala[II] : 1.0f);
        }
        alioquin
        {
            v[ZEPHYRUM]  = nodus->translatio.x + a->motus[ZEPHYRUM];
            v[I]         = nodus->translatio.y + a->motus[I];
            v[II]        = nodus->translatio.z + a->motus[II];
        }
    }
    redde VERUM;
}

/* cyclum in canales: articulus quem positura cycli tangit canalem
 * pro quaque proprietate tacta accipit */
interior b32
_cyclum_coquere (
               Motor* m,
     constans Cyclus* c,
        StatuaCyclus* exitus)
{
    hic_manens constans i32 TACTA[] = {
        TACTA_ROTATIO, TACTA_SCALA, TACTA_MOTUS
    };
    i32 tacta[PLASTICA_ARTICULI_MAXIMI];
    i32 canalium  = ZEPHYRUM;
    i32 n         = ZEPHYRUM;
    i32 j;
    i32 k;
    i32 a;

    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        tacta[j] = ZEPHYRUM;
        per (k = ZEPHYRUM; k < c->clavium; k++)
        {
            tacta[j] |=
                m->positurae[c->claves[k].positura].artus[j].tacta;
        }
        per (a = ZEPHYRUM; a < III; a++)
        {
            si (tacta[j] & TACTA[a])
            {
                canalium++;
            }
        }
    }
    exitus->titulus   = chorda_transcribere(c->titulus, m->piscina);
    exitus->duratio   = c->duratio;
    exitus->canalium  = canalium;
    exitus->canales   = (StatuaCanalis*)piscina_allocare(m->piscina,
        (canalium ? canalium : I) * magnitudo(StatuaCanalis));
    si (!exitus->canales)
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        per (a = ZEPHYRUM; a < III; a++)
        {
            si (   (tacta[j] & TACTA[a])
                && !_canalem_coquere(m, c, j, TACTA[a],
                    &exitus->canales[n++]))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

interior PlasticaFiguraFructus
_recusatus (
    constans Motor* m,
           Piscina* piscina)
{
    PlasticaFiguraFructus f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.error = chorda_ex_literis(m->error, piscina);
    f.linea = m->linea;
    si (m->plagula_erroris.mensura)
    {
        f.plagula = chorda_transcribere(m->plagula_erroris, piscina);
    }
    redde f;
}

/* elementa motus sub radice legere */
interior b32
_motum_legere (
                 Motor* m,
    constans StmlNodus* radix)
{
    constans StmlNodus* liber;
                   b32  finis;
                   i32  cursor = ZEPHYRUM;

    dum ((liber = _liberum(m, radix, &cursor, &finis)) != NIHIL)
    {
        si (_est(liber->titulus, "positura"))
        {
            si (!_positura_legere(m, liber))
            {
                redde FALSUM;
            }
        }
        alioquin si (_est(liber->titulus, "cyclus"))
        {
            si (!_cyclus_legere(m, liber))
            {
                redde FALSUM;
            }
        }
        alioquin si (_est(liber->titulus, "membrum"))
        {
            redde _recusare(m, liber->linea,
                "<membrum>: limbs are not built yet", NIHIL, NIHIL);
        }
    }
    redde finis;
}


/* ==================================================
 * Ossa
 * ================================================== */

/* articulum ossium et liberos eius colligere: titulus, unicus, soli
 * articuli intus. Attributa cetera plastica iudicat */
interior b32
_os_probare (
                 Motor* m,
    constans StmlNodus* nodus)
{
       constans chorda* titulus =
           stml_attributum_capere(nodus, "titulus");
    constans StmlNodus* liber;
                   b32  finis;
                   i32  cursor = ZEPHYRUM;
                   i32  k;

    si (!titulus)
    {
        redde _recusare(m, nodus->linea,
            "<articulus> needs a non-empty attribute 'titulus'", NIHIL,
            NIHIL);
    }
    per (k = ZEPHYRUM; k < m->ossium; k++)
    {
        si (chorda_aequalis(
            *stml_attributum_capere(m->ossa[k], "titulus"), *titulus))
        {
            redde _recusare(m, nodus->linea, "joint title '", titulus,
                "' is used twice");
        }
    }
    si (m->ossium >= PLASTICA_ARTICULI_MAXIMI)
    {
        redde _recusare(m, nodus->linea,
            "<articulus> is one joint too many (at most 64)", NIHIL,
            NIHIL);
    }
    m->ossa[m->ossium++] = nodus;
    dum ((liber = _liberum(m, nodus, &cursor, &finis)) != NIHIL)
    {
        si (!_est(liber->titulus, "articulus"))
        {
            redde _recusare(m, liber->linea, "a skeleton joint holds"
                " only joints (solids belong to the figure), got <",
                liber->titulus, ">");
        }
        si (!_os_probare(m, liber))
        {
            redde FALSUM;
        }
    }
    redde finis;
}

/* plagula ossium: radix <ossa titulus>, articuli, positurae, cycli */
interior b32
_ossa_probare (
                 Motor* m,
    constans StmlNodus* radix)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", NIHIL
    };
    constans StmlNodus* liber;
                   b32  finis;
                   i32  cursor = ZEPHYRUM;

    si (!_est(radix->titulus, "ossa"))
    {
        redde _recusare(m, radix->linea,
            "a skeleton sheet needs the root element <ossa>, got <",
            radix->titulus, ">");
    }
    si (!_attributa_licita(m, radix, LICITA))
    {
        redde FALSUM;
    }
    si (!stml_attributum_capere(radix, "titulus"))
    {
        redde _recusare_attributum(m, radix, "titulus",
            "is needed and cannot be empty");
    }
    dum ((liber = _liberum(m, radix, &cursor, &finis)) != NIHIL)
    {
        si (_est(liber->titulus, "articulus"))
        {
            si (!_os_probare(m, liber))
            {
                redde FALSUM;
            }
        }
        alioquin si (   !_est(liber->titulus, "positura")
                     && !_est(liber->titulus, "cyclus")
                     && !_est(liber->titulus, "membrum"))
        {
            redde _recusare(m, liber->linea, "<ossa> holds only"
                " joints, poses and cycles (solids belong to the"
                " figure), got <", liber->titulus, ">");
        }
    }
    redde finis;
}

/* articuli plani figurae: quisque articulum ossium nominat */
interior b32
_figuram_probare (
                 Motor* m,
    constans StmlNodus* radix)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "locus", NIHIL
    };
    constans StmlNodus* liber;
                   b32  finis;
                   i32  cursor = ZEPHYRUM;

    dum ((liber = _liberum(m, radix, &cursor, &finis)) != NIHIL)
    {
           constans chorda* titulus;
           constans chorda* locus;
        constans StmlNodus* intus;
                       f32  numeri[III];
                       b32  finis_intus;
                       i32  cursor_intus = ZEPHYRUM;
                       i32  k;

        si (!_est(liber->titulus, "articulus"))
        {
            perge;
        }
        si (stml_attributum_capere(liber, "gyrus"))
        {
            redde _recusare_attributum(m, liber, "gyrus", "belongs to"
                " the skeleton (a figure may only give 'locus')");
        }
        si (stml_attributum_capere(liber, "rotatio"))
        {
            redde _recusare_attributum(m, liber, "rotatio", "belongs"
                " to the skeleton (a figure may only give 'locus')");
        }
        si (!_attributa_licita(m, liber, LICITA))
        {
            redde FALSUM;
        }
        titulus  = stml_attributum_capere(liber, "titulus");
        locus    = stml_attributum_capere(liber, "locus");
        si (!titulus)
        {
            redde _recusare_attributum(m, liber, "titulus",
                "is needed (a joint of the skeleton)");
        }
        per (k = ZEPHYRUM; k < m->ossium; k++)
        {
            si (chorda_aequalis(
                *stml_attributum_capere(m->ossa[k], "titulus"),
                *titulus))
            {
                frange;
            }
        }
        si (k >= m->ossium)
        {
            redde _recusare(m, liber->linea,
                "<articulus> names a joint the skeleton lacks: '",
                titulus, "'");
        }
        si (m->solida[k])
        {
            redde _recusare(m, liber->linea, "joint '", titulus,
                "' appears twice in this figure");
        }
        si (locus && !_numeri(locus, numeri, III))
        {
            redde _recusare_attributum(m, liber, "locus",
                "needs 3 numbers (x y z)");
        }
        m->solida[k] = liber;
        dum ((intus = _liberum(m, liber, &cursor_intus, &finis_intus))
            != NIHIL)
        {
            si (_est(intus->titulus, "articulus"))
            {
                redde _recusare(m, intus->linea, "joints are not"
                    " nested in a figure with a skeleton (the"
                    " skeleton holds the tree)", NIHIL, NIHIL);
            }
        }
        si (!finis_intus)
        {
            redde FALSUM;
        }
    }
    redde finis;
}

/* attributa 'ex' in 'ad' transcribere, praeter unum (NIHIL: omnia) */
interior b32
_attributa_transcribere (
                  Motor* m,
    InternamentumChorda* intern,
              StmlNodus* ad,
     constans StmlNodus* ex,
     constans character* praeter)
{
    i32 numerus = ex->attributa ? xar_numerus(ex->attributa)
        : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans StmlAttributum* a =
            (constans StmlAttributum*)xar_obtinere(ex->attributa, k);
                      character titulus[TITULI_MENSURA];
                         chorda valor;
                            i32 mensura;

        si (!a || !a->titulus || (praeter && _est(a->titulus, praeter)))
        {
            perge;
        }
        mensura = a->titulus->mensura < TITULI_MENSURA - I
            ? a->titulus->mensura : TITULI_MENSURA - I;
        memcpy(titulus, a->titulus->datum, mensura);
        titulus[mensura] = '\0';
        memset(&valor, ZEPHYRUM, magnitudo(valor));
        si (a->valor)
        {
            valor = *a->valor;
        }
        si (!stml_attributum_addere_chorda(ad, m->piscina, intern,
            titulus, valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* elementa libera 'ex' in 'ad' addere (nodi ipsi, non exemplaria).
 * 'radicis': articuli praetereuntur (ex ossibus veniunt); elementa
 * motus plastica ipsa praeterit */
interior b32
_elementa_addere (
             StmlNodus* ad,
    constans StmlNodus* ex,
                   b32  radicis)
{
    i32 numerus = ex->liberi ? xar_numerus(ex->liberi) : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        StmlNodus** locus =
            (StmlNodus**)xar_obtinere(ex->liberi, k);
         StmlNodus* liber = locus ? *locus : NIHIL;

        si (!liber || liber->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (radicis && _est(liber->titulus, "articulus"))
        {
            perge;
        }
        si (!stml_liberum_addere(ad, liber))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* articulus ossium in arborem compositam: attributa ossium ('locus'
 * figurae si datus), solida figurae, deinde articuli liberi. *index
 * per arborem ossium procedit, ordine quo collecta sunt */
interior StmlNodus*
_os_componere (
                  Motor* m,
    InternamentumChorda* intern,
     constans StmlNodus* os,
                    i32* index)
{
    constans StmlNodus* figurae = m->solida[*index];
       constans chorda* locus   = figurae
           ? stml_attributum_capere(figurae, "locus") : NIHIL;
             StmlNodus* novus   =
                 stml_elementum_creare(m->piscina, intern, "articulus");
                   i32 numerus = os->liberi ? xar_numerus(os->liberi)
                       : ZEPHYRUM;
                   i32 k;

    (*index)++;
    si (!novus)
    {
        redde NIHIL;
    }
    novus->linea = os->linea | LINEA_OSSIUM;
    si (   !_attributa_transcribere(m, intern, novus, os,
               locus ? "locus" : NIHIL)
        || (locus && !stml_attributum_addere_chorda(novus, m->piscina,
               intern, "locus", *locus))
        || (figurae && !_elementa_addere(novus, figurae, FALSUM)))
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
         StmlNodus** sedes = (StmlNodus**)xar_obtinere(os->liberi, k);
         StmlNodus*  liber = sedes ? *sedes : NIHIL;
         StmlNodus*  filius;

        si (!liber || liber->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        filius = _os_componere(m, intern, liber, index);
        si (!filius || !stml_liberum_addere(novus, filius))
        {
            redde NIHIL;
        }
    }
    redde novus;
}

/* corpus figurae cum ossibus: plagulam ossium a fonte petere, utramque
 * probare, arborem componere, plasticae tradere. *ossa = radix ossium
 * (positurae et cycli eius postea leguntur) */
interior b32
_corpus_ex_ossibus (
                   Motor* m,
               StmlNodus* figura,
         constans chorda* titulus,
            PlasticaFons  fons,
                  vacuum* contextus,
                     i32  semen,
     InternamentumChorda* intern,
         PlasticaFructus* corpus,
     constans StmlNodus** ossa)
{
    StmlResultus  r;
       StmlNodus* radix;
          chorda  textus;
             i32  numerus;
             i32  index = ZEPHYRUM;
             i32  k;

    si (!fons)
    {
        redde _recusare(m, figura->linea, "<figura> names the"
            " skeleton '", titulus,
            "': skeleton sheets need a source");
    }
    per (k = ZEPHYRUM; k < titulus->mensura; k++)
    {
        i8 littera = titulus->datum[k];

        si (   !(littera >= 'a' && littera <= 'z')
            && !(littera >= '0' && littera <= '9') && littera != '_')
        {
            redde _recusare_attributum(m, figura, "ossa",
                "may only hold a-z, 0-9 and _");
        }
    }
    textus = fons(*titulus, contextus);
    si (!textus.mensura)
    {
        redde _recusare(m, figura->linea, "unknown skeleton '",
            titulus, "'");
    }
    m->plagula  = chorda_transcribere(*titulus, m->piscina);
    r           = stml_legere(textus, m->piscina, intern);
    si (!r.successus || !r.elementum_radix)
    {
        redde _recusare(m, r.successus ? ZEPHYRUM : r.linea_erroris,
            "the skeleton sheet is not valid STML", NIHIL, NIHIL);
    }
    *ossa = r.elementum_radix;
    si (!_ossa_probare(m, *ossa))
    {
        redde FALSUM;
    }
    memset(&m->plagula, ZEPHYRUM, magnitudo(m->plagula));
    si (!_figuram_probare(m, figura))
    {
        redde FALSUM;
    }

    /* arbor composita: radix figurae (sine 'ossa'), elementa eius
     * quae articuli non sunt, deinde arbor ossium */
    radix = stml_elementum_creare(m->piscina, intern, "figura");
    si (   !radix
        || !_attributa_transcribere(m, intern, radix, figura, "ossa")
        || !_elementa_addere(radix, figura, VERUM))
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    radix->linea = figura->linea;
    numerus = (*ossa)->liberi ? xar_numerus((*ossa)->liberi)
        : ZEPHYRUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        StmlNodus** sedes =
            (StmlNodus**)xar_obtinere((*ossa)->liberi, k);
         StmlNodus* liber = sedes ? *sedes : NIHIL;
         StmlNodus* os;

        si (   !liber || liber->genus != STML_NODUS_ELEMENTUM
            || !_est(liber->titulus, "articulus"))
        {
            perge;
        }
        os = _os_componere(m, intern, liber, &index);
        si (!os || !stml_liberum_addere(radix, os))
        {
            redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL,
                NIHIL);
        }
    }
    *corpus = plastica_legere_arboris(radix, semen, m->piscina);
    si (!corpus->successus)
    {
        i32 longitudo = ZEPHYRUM;

        m->fractum  = VERUM;
        m->linea    = corpus->linea & ~LINEA_OSSIUM;
        si (corpus->linea & LINEA_OSSIUM)
        {
            m->plagula_erroris =
                chorda_transcribere(*titulus, m->piscina);
        }
        _appone(m, &longitudo,
            (constans character*)corpus->error.datum,
            corpus->error.mensura);
        redde FALSUM;
    }
    redde VERUM;
}

/* factor motus cuiusque articuli: altitudo in figura per altitudinem
 * in ossibus (ossium 0: I). Solus articulus radicalis motum fert */
interior vacuum
_factores_parare (
    Motor* m)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->articulorum; k++)
    {
        constans chorda* locus =
            stml_attributum_capere(m->ossa[k], "locus");
                    f32 numeri[III];

        m->factor[k] = 1.0f;
        si (   locus && _numeri(locus, numeri, III)
            && numeri[I] != 0.0f)
        {
            m->factor[k] =
                m->basis->nodi[m->articuli[k]].translatio.y
                / numeri[I];
        }
    }
}

PlasticaFiguraFructus
plastica_figura_legere (
     chorda  textus,
        i32  semen,
    Piscina* piscina)
{
    redde plastica_figura_legere_cum_fonte(textus, semen, NIHIL,
        NIHIL, piscina);
}

PlasticaFiguraFructus
plastica_figura_legere_cum_fonte (
          chorda  textus,
             i32  semen,
    PlasticaFons  fons,
          vacuum* contextus,
         Piscina* piscina)
{
    PlasticaFiguraFructus  f;
          PlasticaFructus  corpus;
                    Motor* m;
      InternamentumChorda* intern;
             StmlResultus  r;
       constans StmlNodus* ossa     = NIHIL;
          constans chorda* titulus  = NIHIL;
                   Statua* s;
                      i32  momentorum = ZEPHYRUM;
                      i32  k;
                      i32  j;
                      i32  n;

    memset(&f, ZEPHYRUM, magnitudo(f));
    m = (Motor*)piscina_allocare(piscina, magnitudo(Motor));
    si (!m)
    {
        f.error = chorda_ex_literis("out of memory", piscina);
        redde f;
    }
    memset(m, ZEPHYRUM, magnitudo(Motor));
    m->piscina  = piscina;
    intern      = internamentum_creare(piscina);
    r           = stml_legere(textus, piscina, intern);
    si (   r.successus && r.elementum_radix
        && _est(r.elementum_radix->titulus, "figura"))
    {
        titulus = stml_attributum_capere(r.elementum_radix, "ossa");
    }
    si (titulus)
    {
        si (!_corpus_ex_ossibus(m, r.elementum_radix, titulus, fons,
            contextus, semen, intern, &corpus, &ossa))
        {
            redde _recusatus(m, piscina);
        }
    }
    alioquin
    {
        corpus = plastica_legere_seminis(textus, semen, piscina);
        si (!corpus.successus)
        {
            f.error = corpus.error;
            f.linea = corpus.linea;
            redde f;
        }
        si (!r.successus || !r.elementum_radix)
        {
            (vacuum)_recusare(m, ZEPHYRUM,
                "the description is not valid STML", NIHIL, NIHIL);
            redde _recusatus(m, piscina);
        }
        si (!_est(r.elementum_radix->titulus, "figura"))
        {
            (vacuum)_recusare(m, r.elementum_radix->linea,
                "a figure needs the root element <figura>, got <",
                r.elementum_radix->titulus, ">");
            redde _recusatus(m, piscina);
        }
    }
    m->basis        = corpus.statua;
    m->partes       = corpus.partes;
    m->articuli     = corpus.articuli;
    m->articulorum  = corpus.articulorum;
    m->cycli        = (Cyclus*)piscina_allocare(piscina,
        PLASTICA_MOTUS_CYCLI_MAXIMI * magnitudo(Cyclus));
    /* positura "quies": nihil tangit */
    m->positurae[ZEPHYRUM].titulus =
        chorda_ex_literis("quies", piscina);
    m->positurae[ZEPHYRUM].artus = (Artus*)piscina_allocare(piscina,
        (m->articulorum ? m->articulorum : I) * magnitudo(Artus));
    s = (Statua*)piscina_allocare(piscina, magnitudo(Statua));
    si (!m->cycli || !m->positurae[ZEPHYRUM].artus || !s)
    {
        (vacuum)_recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
        redde _recusatus(m, piscina);
    }
    memset(m->positurae[ZEPHYRUM].artus, ZEPHYRUM,
        (m->articulorum ? m->articulorum : I) * magnitudo(Artus));
    m->positurarum = I;
    si (ossa)
    {
        /* motus ossium primum, in plagula sua */
        _factores_parare(m);
        m->plagula     = chorda_transcribere(*titulus, piscina);
        m->in_ossibus  = VERUM;
        si (!_motum_legere(m, ossa))
        {
            redde _recusatus(m, piscina);
        }
        m->in_ossibus = FALSUM;
        memset(&m->plagula, ZEPHYRUM, magnitudo(m->plagula));
    }
    si (!_motum_legere(m, r.elementum_radix))
    {
        redde _recusatus(m, piscina);
    }
    /* positura substituta: viae cyclorum iterum probantur (cyclus
     * ante substitutionem lectus posituram veterem vidit), quisque
     * in plagula sua */
    per (k = ZEPHYRUM; m->substitutum && k < m->cyclorum; k++)
    {
        memset(&m->plagula, ZEPHYRUM, magnitudo(m->plagula));
        si (m->cycli[k].ossium)
        {
            m->plagula = chorda_transcribere(*titulus, piscina);
        }
        si (!_vias_probare(m, &m->cycli[k]))
        {
            redde _recusatus(m, piscina);
        }
    }
    memset(&m->plagula, ZEPHYRUM, magnitudo(m->plagula));

    /* statua nova: valor prioris cum cyclis; tabulae mutuatae */
    *s           = *corpus.statua;
    s->cyclorum  = m->cyclorum;
    s->cycli      = (StatuaCyclus*)piscina_allocare(piscina,
        (m->cyclorum ? m->cyclorum : I) * magnitudo(StatuaCyclus));
    si (!s->cycli)
    {
        (vacuum)_recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
        redde _recusatus(m, piscina);
    }
    per (k = ZEPHYRUM; k < m->cyclorum; k++)
    {
        si (!_cyclum_coquere(m, &m->cycli[k], &s->cycli[k]))
        {
            redde _recusatus(m, piscina);
        }
        momentorum += m->cycli[k].momentorum;
    }
    si (!statua_finire(s, piscina))
    {
        (vacuum)_recusare(m, ZEPHYRUM,
            "the model could not be finished", NIHIL, NIHIL);
        redde _recusatus(m, piscina);
    }
    si (momentorum)
    {
        f.momenta = (PlasticaMomentum*)piscina_allocare(piscina,
            momentorum * magnitudo(PlasticaMomentum));
        si (!f.momenta)
        {
            (vacuum)_recusare(m, ZEPHYRUM, "out of memory", NIHIL,
                NIHIL);
            redde _recusatus(m, piscina);
        }
        n = ZEPHYRUM;
        per (k = ZEPHYRUM; k < m->cyclorum; k++)
        {
            per (j = ZEPHYRUM; j < m->cycli[k].momentorum; j++)
            {
                f.momenta[n].cyclus   = k;
                f.momenta[n].titulus  = chorda_transcribere(
                    m->cycli[k].momenta[j], piscina);
                f.momenta[n].tempus   = m->cycli[k].tempora[j];
                n++;
            }
        }
    }
    f.momentorum   = momentorum;
    f.successus    = VERUM;
    f.statua       = s;
    f.partes       = corpus.partes;
    f.articuli     = corpus.articuli;
    f.articulorum  = corpus.articulorum;
    redde f;
}
