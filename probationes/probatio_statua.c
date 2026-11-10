/* probatio_statua.c - statua et positura (statua.h)
 *
 * Statua manu facta: nodus parens (radix) et liber ad (0,1,0), cyclus
 * unus secundae unius in parente. quies: sine cyclo. linearis: claves
 * miscentur. gradatim: clavis prior tenetur. involutio: tempus per
 * durationem redit (negativum quoque), duratio 0 = clavis prima.
 * rotatio: slerp. sine canali: nodus quietem tenet. finire: ordo
 * parentes ante liberos; circulus, parens et indices extra fines,
 * claves decrescentes recusantur. sphaera: quietem tegit, non cyclos.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "stereometria.h"
#include "statua.h"
#include "credo.h"
#include <string.h>

#define EPSILON 0.0001f

#define CREDO_VECTOR(v, ex, ey, ez) \
    fac { \
        CREDO_F32_PROXIMUS ((v).x, (ex), EPSILON); \
        CREDO_F32_PROXIMUS ((v).y, (ey), EPSILON); \
        CREDO_F32_PROXIMUS ((v).z, (ez), EPSILON); \
    } dum (FALSUM)

interior vacuum*
_locus (
    CredoContextus* c,
    memoriae_index  mensura)
{
    vacuum* p = piscina_allocare(c->piscina, mensura);

    memset(p, ZEPHYRUM, mensura);
    redde p;
}

/* nodus 0 parens (radix), nodus 1 liber ad (0,1,0); sine retibus */
interior Statua*
_statuam_parare (
    CredoContextus* c)
{
    Statua* s = (Statua*)_locus(c, magnitudo(Statua));

    s->nodorum = II;
    s->nodi = (StatuaNodus*)_locus(c, II * magnitudo(StatuaNodus));
    s->nodi[ZEPHYRUM].parens = -I;
    s->nodi[ZEPHYRUM].rete = -I;
    s->nodi[ZEPHYRUM].translatio  = stereometria_vector(0.0f, 0.0f,
        0.0f);
    s->nodi[ZEPHYRUM].rotatio     = stereometria_quaternio_identitas();
    s->nodi[ZEPHYRUM].scala       = stereometria_vector(1.0f, 1.0f,
        1.0f);
    s->nodi[I]             = s->nodi[ZEPHYRUM];
    s->nodi[I].parens      = ZEPHYRUM;
    s->nodi[I].translatio  = stereometria_vector(0.0f, 1.0f, 0.0f);
    redde s;
}

/* cyclus unus cum canali uno in nodo dato */
interior StatuaCanalis*
_cyclum_addere (
      CredoContextus* c,
              Statua* s,
                 s32  nodus,
    StatuaProprietas  proprietas,
                 i32  clavium,
        constans f32* tempora,
        constans f32* valores)
{
    StatuaCanalis* k;
              i32  latus = proprietas == STATUA_ROTATIO ? IV : III;

    s->cyclorum  = I;
    s->cycli     = (StatuaCyclus*)_locus(c, magnitudo(StatuaCyclus));
    k            = (StatuaCanalis*)_locus(c, magnitudo(StatuaCanalis));
    s->cycli[ZEPHYRUM].titulus   = chorda_ex_literis("motus",
        c->piscina);
    s->cycli[ZEPHYRUM].duratio = tempora[clavium - I];
    s->cycli[ZEPHYRUM].canales = k;
    s->cycli[ZEPHYRUM].canalium = I;
    k->nodus = nodus;
    k->proprietas = proprietas;
    k->clavium = clavium;
    k->tempora = (f32*)_locus(c, clavium * magnitudo(f32));
    k->valores = (f32*)_locus(c, clavium * latus * magnitudo(f32));
    memcpy(k->tempora, tempora, clavium * magnitudo(f32));
    memcpy(k->valores, valores, clavium * latus * magnitudo(f32));
    redde k;
}

/* origo nodi in spatio statuae */
interior StereometriaVector
_origo (
    constans StereometriaMatrix* m,
                            i32  nodus)
{
    redde stereometria_punctum_transformare(m[nodus],
        stereometria_vector(0.0f, 0.0f, 0.0f), NIHIL);
}

hic_manens constans f32 TEMPORA_DUO[II]   = { 0.0f, 1.0f };
hic_manens constans f32 MOTUS_DUO[VI]     = {
    0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f
};
hic_manens constans f32 TEMPORA_TRIA[III] = { 0.0f, 0.5f, 1.0f };
hic_manens constans f32 MOTUS_TRES[IX]    = {
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f
};

interior vacuum
_sectio_quies (
    CredoContextus* c)
{
                Statua* s = _statuam_parare(c);
    StereometriaMatrix  m[II];

    _cyclum_addere(c, s, ZEPHYRUM, STATUA_TRANSLATIO, II, TEMPORA_DUO,
        MOTUS_DUO);
    CREDO_NECESSE (statua_finire(s, c->piscina));
    statua_positura(s, -I, 0.5f, m);
    CREDO_VECTOR (_origo(m, ZEPHYRUM), 0.0f, 0.0f, 0.0f);
    CREDO_VECTOR (_origo(m, I), 0.0f, 1.0f, 0.0f);
    /* cyclus extra fines = quies */
    statua_positura(s, VII, 0.5f, m);
    CREDO_VECTOR (_origo(m, I), 0.0f, 1.0f, 0.0f);
}

interior vacuum
_sectio_linearis (
    CredoContextus* c)
{
                Statua* s = _statuam_parare(c);
    StereometriaMatrix  m[II];

    _cyclum_addere(c, s, ZEPHYRUM, STATUA_TRANSLATIO, II, TEMPORA_DUO,
        MOTUS_DUO);
    CREDO_NECESSE (statua_finire(s, c->piscina));
    statua_positura(s, ZEPHYRUM, 0.5f, m);
    CREDO_VECTOR (_origo(m, ZEPHYRUM), 1.0f, 0.0f, 0.0f);
    CREDO_VECTOR (_origo(m, I), 1.0f, 1.0f, 0.0f);
    statua_positura(s, ZEPHYRUM, 0.25f, m);
    CREDO_VECTOR (_origo(m, I), 0.5f, 1.0f, 0.0f);
    statua_positura(s, ZEPHYRUM, 0.0f, m);
    CREDO_VECTOR (_origo(m, I), 0.0f, 1.0f, 0.0f);
}

interior vacuum
_sectio_gradatim (
    CredoContextus* c)
{
                Statua* s = _statuam_parare(c);
    StereometriaMatrix  m[II];
         StatuaCanalis* k;

    k = _cyclum_addere(c, s, ZEPHYRUM, STATUA_TRANSLATIO, III,
        TEMPORA_TRIA, MOTUS_TRES);
    k->gradatim = VERUM;
    CREDO_NECESSE (statua_finire(s, c->piscina));
    statua_positura(s, ZEPHYRUM, 0.25f, m);
    CREDO_VECTOR (_origo(m, I), 0.0f, 1.0f, 0.0f);
    statua_positura(s, ZEPHYRUM, 0.49f, m);
    CREDO_VECTOR (_origo(m, I), 0.0f, 1.0f, 0.0f);
    statua_positura(s, ZEPHYRUM, 0.5f, m);
    CREDO_VECTOR (_origo(m, I), 1.0f, 1.0f, 0.0f);
    statua_positura(s, ZEPHYRUM, 0.75f, m);
    CREDO_VECTOR (_origo(m, I), 1.0f, 1.0f, 0.0f);
    /* idem linearis medium inter claves dat */
    k->gradatim = FALSUM;
    statua_positura(s, ZEPHYRUM, 0.75f, m);
    CREDO_VECTOR (_origo(m, I), 1.5f, 1.0f, 0.0f);
}

interior vacuum
_sectio_involutio (
    CredoContextus* c)
{
                Statua* s = _statuam_parare(c);
    StereometriaMatrix  m[II];

    _cyclum_addere(c, s, ZEPHYRUM, STATUA_TRANSLATIO, II, TEMPORA_DUO,
        MOTUS_DUO);
    CREDO_NECESSE (statua_finire(s, c->piscina));
    statua_positura(s, ZEPHYRUM, 1.25f, m);
    CREDO_VECTOR (_origo(m, I), 0.5f, 1.0f, 0.0f);
    statua_positura(s, ZEPHYRUM, 7.25f, m);
    CREDO_VECTOR (_origo(m, I), 0.5f, 1.0f, 0.0f);
    statua_positura(s, ZEPHYRUM, -0.25f, m);
    CREDO_VECTOR (_origo(m, I), 1.5f, 1.0f, 0.0f);
    /* tempus = duratio ad initium redit (cyclus) */
    statua_positura(s, ZEPHYRUM, 1.0f, m);
    CREDO_VECTOR (_origo(m, I), 0.0f, 1.0f, 0.0f);
    /* duratio 0: clavis prima, tempore quovis */
    s->cycli[ZEPHYRUM].duratio = 0.0f;
    statua_positura(s, ZEPHYRUM, 3.7f, m);
    CREDO_VECTOR (_origo(m, I), 0.0f, 1.0f, 0.0f);
}

interior vacuum
_sectio_rotatio (
    CredoContextus* c)
{
                Statua* s = _statuam_parare(c);
    StereometriaMatrix  m[II];
    StereometriaVector  v;
                   f32  valores[VIII];

    /* identitas -> XC gradus circa y */
    memset(valores, ZEPHYRUM, magnitudo(valores));
    valores[III]  = 1.0f;
    valores[V]    = 0.70710678f;
    valores[VII]  = 0.70710678f;
    _cyclum_addere(c, s, ZEPHYRUM, STATUA_ROTATIO, II, TEMPORA_DUO,
        valores);
    CREDO_NECESSE (statua_finire(s, c->piscina));
    statua_positura(s, ZEPHYRUM, 0.5f, m);
    /* XLV gradus: axis x liberi ad (cos, 0, -sin) */
    v = stereometria_directionem_transformare(m[I],
        stereometria_vector(1.0f, 0.0f, 0.0f));
    CREDO_VECTOR (v, 0.70710678f, 0.0f, -0.70710678f);
    /* liber in axe rotationis sedet: origo manet */
    CREDO_VECTOR (_origo(m, I), 0.0f, 1.0f, 0.0f);
}

interior vacuum
_sectio_sine_canali (
    CredoContextus* c)
{
                Statua* s = _statuam_parare(c);
    StereometriaMatrix  m[II];
    StereometriaVector  v;

    /* canalis in LIBERO; parens quietem tenet */
    s->nodi[ZEPHYRUM].translatio = stereometria_vector(5.0f, 0.0f,
        0.0f);
    s->nodi[ZEPHYRUM].scala = stereometria_vector(2.0f, 2.0f, 2.0f);
    _cyclum_addere(c, s, I, STATUA_TRANSLATIO, II, TEMPORA_DUO,
        MOTUS_DUO);
    CREDO_NECESSE (statua_finire(s, c->piscina));
    statua_positura(s, ZEPHYRUM, 0.5f, m);
    CREDO_VECTOR (_origo(m, ZEPHYRUM), 5.0f, 0.0f, 0.0f);
    /* liber: translatio (1,0,0) canalis, scala parentis II */
    CREDO_VECTOR (_origo(m, I), 7.0f, 0.0f, 0.0f);
    /* scala et rotatio liberi (sine canali) quietem tenent */
    v = stereometria_directionem_transformare(m[I],
        stereometria_vector(0.0f, 1.0f, 0.0f));
    CREDO_VECTOR (v, 0.0f, 2.0f, 0.0f);
}

interior vacuum
_sectio_finire (
    CredoContextus* c)
{
                Statua* s;
    StereometriaMatrix  m[II];
         StatuaCanalis* k;

    /* liber ANTE parentem datus: ordo parentem primum ponit */
    s                         = _statuam_parare(c);
    s->nodi[ZEPHYRUM].parens  = I;
    s->nodi[ZEPHYRUM].translatio  = stereometria_vector(0.0f, 1.0f,
        0.0f);
    s->nodi[I].parens      = -I;
    s->nodi[I].translatio  = stereometria_vector(3.0f, 0.0f, 0.0f);
    CREDO_NECESSE (statua_finire(s, c->piscina));
    CREDO_AEQUALIS_I32 (s->ordo[ZEPHYRUM], I);
    CREDO_AEQUALIS_I32 (s->ordo[I], ZEPHYRUM);
    statua_positura(s, -I, 0.0f, m);
    CREDO_VECTOR (_origo(m, ZEPHYRUM), 3.0f, 1.0f, 0.0f);
    /* statua sine verticibus: sphaera nulla */
    CREDO_F32_PROXIMUS (s->radius, 0.0f, EPSILON);

    /* circulus parentum */
    s                         = _statuam_parare(c);
    s->nodi[ZEPHYRUM].parens  = I;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* nodus parens sui */
    s                  = _statuam_parare(c);
    s->nodi[I].parens  = I;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* parens extra fines */
    s                  = _statuam_parare(c);
    s->nodi[I].parens  = II;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    s                  = _statuam_parare(c);
    s->nodi[I].parens  = -II;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* rete extra fines */
    s                = _statuam_parare(c);
    s->nodi[I].rete  = ZEPHYRUM;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* canalis: nodus extra fines */
    s = _statuam_parare(c);
    k = _cyclum_addere(c, s, ZEPHYRUM, STATUA_TRANSLATIO, II,
        TEMPORA_DUO, MOTUS_DUO);
    k->nodus = II;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* canalis sine clavibus */
    s = _statuam_parare(c);
    k = _cyclum_addere(c, s, ZEPHYRUM, STATUA_TRANSLATIO, II,
        TEMPORA_DUO, MOTUS_DUO);
    k->clavium = ZEPHYRUM;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* claves decrescentes */
    s = _statuam_parare(c);
    k = _cyclum_addere(c, s, ZEPHYRUM, STATUA_TRANSLATIO, II,
        TEMPORA_DUO, MOTUS_DUO);
    k->tempora[ZEPHYRUM] = 2.0f;
    CREDO_FALSUM (statua_finire(s, c->piscina));
}

/* cubus lateris I circa originem: VIII vertices, XII trianguli */
hic_manens constans i32 CUBI_INDICES[XXXVI] = {
    0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 4, 1, 1, 4, 5,
    2, 3, 6, 3, 7, 6, 0, 2, 4, 2, 6, 4, 1, 5, 3, 3, 5, 7
};

interior vacuum
_cubum_addere (
    CredoContextus* c,
            Statua* s,
               s32  nodus)
{
    i32 v;

    s->verticum     = VIII;
    s->positiones   = (f32*)_locus(c, XXIV * magnitudo(f32));
    s->normales     = (f32*)_locus(c, XXIV * magnitudo(f32));
    s->coordinatae  = (f32*)_locus(c, XVI * magnitudo(f32));
    per (v = ZEPHYRUM; v < VIII; v++)
    {
        s->positiones[v * III]       = (v & I) ? 0.5f : -0.5f;
        s->positiones[v * III + I]   = (v & II) ? 0.5f : -0.5f;
        s->positiones[v * III + II]  = (v & IV) ? 0.5f : -0.5f;
        s->normales[v * III + I]     = 1.0f;
    }
    s->indicum = XXXVI;
    s->indices = (i32*)_locus(c, XXXVI * magnitudo(i32));
    memcpy(s->indices, CUBI_INDICES, XXXVI * magnitudo(i32));
    s->partium = I;
    s->partes = (StatuaPars*)_locus(c, magnitudo(StatuaPars));
    s->partes[ZEPHYRUM].indicum = XXXVI;
    s->partes[ZEPHYRUM].materia = -I;
    s->retium = I;
    s->retia = (StatuaRete*)_locus(c, magnitudo(StatuaRete));
    s->retia[ZEPHYRUM].partium = I;
    s->nodi[nodus].rete = ZEPHYRUM;
}

interior vacuum
_sectio_sphaera (
    CredoContextus* c)
{
                Statua* s = _statuam_parare(c);
    StereometriaMatrix  m[II];
    StereometriaVector  p;
                   i32  v;
                   f32  maxima = 0.0f;
                   f32  d;

    _cubum_addere(c, s, I);
    _cyclum_addere(c, s, ZEPHYRUM, STATUA_TRANSLATIO, II, TEMPORA_DUO,
        MOTUS_DUO);
    CREDO_NECESSE (statua_finire(s, c->piscina));
    /* sphaera QUIETIS: liber ad (0,1,0), cubus +-0.5 -> centrum
     * (0,1,0), radius sqrt(0.75). Cyclus (liber ad (2,1,0) it) eam
     * NON auget */
    CREDO_VECTOR (s->centrum, 0.0f, 1.0f, 0.0f);
    CREDO_F32_PROXIMUS (s->radius, 0.8660254f, 0.001f);
    statua_positura(s, -I, 0.0f, m);
    per (v = ZEPHYRUM; v < VIII; v++)
    {
        p = stereometria_punctum_transformare(m[I],
            stereometria_vector(s->positiones[v * III],
                s->positiones[v * III + I],
                s->positiones[v * III + II]), NIHIL);
        d = stereometria_longitudo(stereometria_subtrahere(p,
            s->centrum));
        si (d > maxima)
        {
            maxima = d;
        }
    }
    CREDO_VERUM (maxima <= s->radius + EPSILON);
    /* in cyclo cubus sphaeram relinquit */
    statua_positura(s, ZEPHYRUM, 0.9f, m);
    p = stereometria_punctum_transformare(m[I],
        stereometria_vector(0.0f, 0.0f, 0.0f), NIHIL);
    CREDO_VERUM (stereometria_longitudo(stereometria_subtrahere(p,
        s->centrum)) > s->radius);

    /* index verticis extra fines recusatur */
    s = _statuam_parare(c);
    _cubum_addere(c, s, I);
    s->indices[V] = VIII;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* indicum non multiplum III */
    s = _statuam_parare(c);
    _cubum_addere(c, s, I);
    s->partes[ZEPHYRUM].indicum = XXXV;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* pars ultra indices */
    s = _statuam_parare(c);
    _cubum_addere(c, s, I);
    s->partes[ZEPHYRUM].index_primus = III;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* materia extra fines */
    s = _statuam_parare(c);
    _cubum_addere(c, s, I);
    s->partes[ZEPHYRUM].materia = ZEPHYRUM;
    CREDO_FALSUM (statua_finire(s, c->piscina));
    /* rete ultra partes */
    s = _statuam_parare(c);
    _cubum_addere(c, s, I);
    s->retia[ZEPHYRUM].partium = II;
    CREDO_FALSUM (statua_finire(s, c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "quies", _sectio_quies, NIHIL, NIHIL, NIHIL, NIHIL },
    { "linearis", _sectio_linearis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "gradatim", _sectio_gradatim, NIHIL, NIHIL, NIHIL, NIHIL },
    { "involutio", _sectio_involutio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rotatio", _sectio_rotatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine_canali", _sectio_sine_canali, NIHIL, NIHIL, NIHIL, NIHIL },
    { "finire", _sectio_finire, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sphaera", _sectio_sphaera, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("statua", SECTIONES);
}
