/* probatio_statua_pictura.c - pictor statuae (statua_pictura.h)
 *
 * Statuae manu factae ex quadratis lateris II in plano z dato
 * (frons ad +z, oculum versus). quadratum: mensura exacta in schirmo
 * (camera + regula marginum) et margo communis sine foramine aut
 * duplici. profunditas: propius vincit. tergum: omittitur nisi
 * bilateralis. textura: quadrantes, ordo summus = t 0. excisio: alpha
 * infra limen non pingitur. lux: claritas faciei, fundus, emissiva.
 * margo: lapis numquam oram picturae tangit. prope: triangula planum
 * propinquum secantia et post oculum. vacua, determinatum.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_glb.h"
#include "statua_pictura.h"
#include "sigillum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_LAPIDIS "probationes/fixa/statuae/lapis.glb"

/* pictura LXIV x LXIV, quadratum lateris II solum: sphaera radii
 * sqrt(2), ergo dimidium latus in schirmo = XXXII * cos(15) /
 * (sqrt(2) * 1.16) = 18.84 pixela -> centra pixelorum XIII..L tecta:
 * XXXVIII per latus */
#define LATUS            LXIV
#define QUADRATI_PRIMUS  XIII
#define QUADRATI_LATUS   XXXVIII

interior vacuum*
_locus (
    CredoContextus* c,
    memoriae_index  mensura)
{
    vacuum* p = piscina_allocare(c->piscina, mensura ? mensura : I);

    memset(p, ZEPHYRUM, mensura);
    redde p;
}

/* statua: nodus unus, rete unum, 'numerus' quadrata lateris II ad
 * z[k], quodque pars sua cum materia k (materiae ordinariae: albae,
 * opacae, sine textura). Vertices: (-1,-1) (1,-1) (1,1) (-1,1);
 * coordinatae (0,1) (1,1) (1,0) (0,0) - t 0 summum */
interior Statua*
_statuam_parare (
    CredoContextus* c,
               i32  numerus,
      constans f32* z)
{
    hic_manens constans f32 ANGULI[VIII] = {
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
    };
    hic_manens constans f32 COORDINATAE[VIII] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };
    hic_manens constans i32  INDICES[VI] = { 0, 1, 2, 0, 2, 3 };
                     Statua* s = (Statua*)_locus(c, magnitudo(Statua));
                        i32  k;
                        i32  v;

    s->nodorum = I;
    s->nodi = (StatuaNodus*)_locus(c, magnitudo(StatuaNodus));
    s->nodi[ZEPHYRUM].parens = -I;
    s->nodi[ZEPHYRUM].rete = ZEPHYRUM;
    s->nodi[ZEPHYRUM].rotatio = stereometria_quaternio_identitas();
    s->nodi[ZEPHYRUM].scala = stereometria_vector(1.0f, 1.0f, 1.0f);
    s->retium = I;
    s->retia = (StatuaRete*)_locus(c, magnitudo(StatuaRete));
    s->retia[ZEPHYRUM].partium = numerus;
    s->partium = numerus;
    s->partes       = (StatuaPars*)_locus(c,
        numerus * magnitudo(StatuaPars));
    s->materiarum   = numerus;
    s->materiae     = (StatuaMateria*)_locus(c,
        numerus * magnitudo(StatuaMateria));
    s->verticum     = numerus * IV;
    s->positiones   = (f32*)_locus(c, numerus * XII * magnitudo(f32));
    s->normales     = (f32*)_locus(c, numerus * XII * magnitudo(f32));
    s->coordinatae  = (f32*)_locus(c, numerus * VIII * magnitudo(f32));
    s->indicum      = numerus * VI;
    s->indices      = (i32*)_locus(c, numerus * VI * magnitudo(i32));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        per (v = ZEPHYRUM; v < IV; v++)
        {
            s->positiones[(k * IV + v) * III] = ANGULI[v * II];
            s->positiones[(k * IV + v) * III + I] = ANGULI[v * II
                + I];
            s->positiones[(k * IV + v) * III + II]  = z[k];
            s->normales[(k * IV + v) * III + II]    = 1.0f;
            s->coordinatae[(k * IV + v) * II] =
                COORDINATAE[v * II];
            s->coordinatae[(k * IV + v) * II + I]   =
                COORDINATAE[v * II + I];
        }
        per (v = ZEPHYRUM; v < VI; v++)
        {
            s->indices[k * VI + v] = k * IV + INDICES[v];
        }
        s->partes[k].index_primus       = k * VI;
        s->partes[k].indicum            = VI;
        s->partes[k].materia            = (s32)k;
        s->materiae[k].textura          = -I;
        s->materiae[k].limen_alpha      = -1.0f;
        s->materiae[k].color[ZEPHYRUM]  = 1.0f;
        s->materiae[k].color[I]         = 1.0f;
        s->materiae[k].color[II]        = 1.0f;
        s->materiae[k].color[III]       = 1.0f;
    }
    redde s;
}

/* textura RGBA ex octetis in statuam addita; index eius */
interior s32
_texturam_addere (
    CredoContextus* c,
            Statua* s,
               i32  latitudo,
               i32  altitudo,
       constans i8* pixela)
{
    si (!s->texturae)
    {
        s->texturae = (Imago*)_locus(c, IV * magnitudo(Imago));
    }
    s->texturae[s->texturarum].latitudo = latitudo;
    s->texturae[s->texturarum].altitudo = altitudo;
    s->texturae[s->texturarum].pixela    = (i8*)_locus(c,
        (memoriae_index)latitudo * altitudo * IV);
    memcpy(s->texturae[s->texturarum].pixela, pixela,
        (memoriae_index)latitudo * altitudo * IV);
    redde (s32)s->texturarum++;
}

/* pictura LATUS x LATUS in positura quietis; visus datus */
interior StatuaPictura
_pingere (
    CredoContextus* c,
            Statua* s,
               f32  gyrus,
               f32  inclinatio,
               b32  illuminatum)
{
    StereometriaMatrix* m;
         StatuaPictura  p;
           StatuaVisus  visus;

    memset(&p, ZEPHYRUM, magnitudo(p));
    m = (StereometriaMatrix*)_locus(c,
        (s->nodorum + I) * magnitudo(StereometriaMatrix));
    statua_positura(s, -I, 0.0f, m);
    visus.gyrus        = gyrus;
    visus.inclinatio   = inclinatio;
    visus.illuminatum  = illuminatum;
    CREDO_VERUM (statua_pingere(s, m, &visus, LATUS, LATUS, &p,
        c->piscina));
    redde p;
}

interior i32
_tecta (
    constans StatuaPictura* p)
{
    i32 n = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < p->latitudo * p->altitudo; k++)
    {
        n += p->tectum[k] ? I : ZEPHYRUM;
    }
    redde n;
}

interior constans i8*
_pixelum (
    constans StatuaPictura* p,
                       i32  x,
                       i32  y)
{
    redde p->rgba + ((memoriae_index)y * p->latitudo + x) * IV;
}

hic_manens constans f32 Z_UNUM[I]  = { 0.0f };
hic_manens constans f32 Z_DUO[II]  = { 0.0f, -0.5f };

interior vacuum
_sectio_quadratum (
    CredoContextus* c)
{
           Statua* s = _statuam_parare(c, I, Z_UNUM);
    StatuaPictura  p;
    StatuaPictura  a;
    StatuaPictura  b;
              i32  x;
              i32  y;
              b32  intra;

    s->materiae[ZEPHYRUM].color[I]   = 0.0f;
    s->materiae[ZEPHYRUM].color[II]  = 0.0f;
    CREDO_NECESSE (statua_finire(s, c->piscina));
    CREDO_F32_PROXIMUS (s->radius, 1.4142135f, 0.0001f);
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (p.latitudo, LATUS);
    CREDO_AEQUALIS_I32 (_tecta(&p), QUADRATI_LATUS * QUADRATI_LATUS);
    /* quadratum plenum, axibus alineatum, in medio: tectum = rubrum
     * opacum, cetera = album opacum */
    per (y = ZEPHYRUM; y < LATUS; y++)
    {
        per (x = ZEPHYRUM; x < LATUS; x++)
        {
            intra = x >= QUADRATI_PRIMUS
                && x < QUADRATI_PRIMUS + QUADRATI_LATUS
                && y >= QUADRATI_PRIMUS
                && y < QUADRATI_PRIMUS + QUADRATI_LATUS;
            si (   (p.tectum[y * LATUS + x] != ZEPHYRUM) != intra
                || _pixelum(&p, x, y)[ZEPHYRUM] != 255
                || _pixelum(&p, x, y)[I] != (intra ? ZEPHYRUM : 255)
                || _pixelum(&p, x, y)[II] != (intra ? ZEPHYRUM : 255)
                || _pixelum(&p, x, y)[III] != 255)
            {
                CREDO_AEQUALIS_I32 (x * 1000 + y, 0xFFFFFF);
                redde;
            }
        }
    }
    /* margo communis: triangulum quodque solum, deinde ambo -
     * summa exacta (nec foramen nec pixelum bis) */
    s->partes[ZEPHYRUM].indicum = III;
    a = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    s->partes[ZEPHYRUM].index_primus = III;
    b = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_VERUM (_tecta(&a) > ZEPHYRUM && _tecta(&b) > ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_tecta(&a) + _tecta(&b),
        QUADRATI_LATUS * QUADRATI_LATUS);
    per (x = ZEPHYRUM; x < LATUS * LATUS; x++)
    {
        si (a.tectum[x] && b.tectum[x])
        {
            CREDO_AEQUALIS_I32 (x, 0xFFFFFF);
            redde;
        }
    }
    /* gyrus XLV: quadratum angustius, eadem altitudine in medio */
    s->partes[ZEPHYRUM].index_primus = ZEPHYRUM;
    s->partes[ZEPHYRUM].indicum = VI;
    a = _pingere(c, s, 45.0f, 0.0f, FALSUM);
    CREDO_VERUM (_tecta(&a) < _tecta(&p) * III / IV);
    CREDO_VERUM (_tecta(&a) > _tecta(&p) / II);
}

interior vacuum
_sectio_profunditas (
    CredoContextus* c)
{
           Statua* s = _statuam_parare(c, II, Z_DUO);
    StatuaPictura  p;
              i32  medium = LATUS / II;

    /* pars 0 (z 0, propior) rubra; pars I (z -0.5, post) caerulea,
     * pingitur ULTIMA */
    s->materiae[ZEPHYRUM].color[I]   = 0.0f;
    s->materiae[ZEPHYRUM].color[II]  = 0.0f;
    s->materiae[I].color[ZEPHYRUM]   = 0.0f;
    s->materiae[I].color[I]          = 0.0f;
    CREDO_NECESSE (statua_finire(s, c->piscina));
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, medium, medium)[ZEPHYRUM], 255);
    CREDO_AEQUALIS_I32 (_pixelum(&p, medium, medium)[II], ZEPHYRUM);
    /* a tergo (gyrus CLXXX): ambo terga, nihil pingitur */
    p = _pingere(c, s, 180.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_tecta(&p), ZEPHYRUM);
    /* bilaterales a tergo: nunc caerulea propior est */
    s->materiae[ZEPHYRUM].bilateralis = VERUM;
    s->materiae[I].bilateralis = VERUM;
    p = _pingere(c, s, 180.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, medium, medium)[ZEPHYRUM],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, medium, medium)[II], 255);
}

interior vacuum
_sectio_tergum (
    CredoContextus* c)
{
           Statua* s = _statuam_parare(c, I, Z_UNUM);
    StatuaPictura  p;
              i32  frontis;

    CREDO_NECESSE (statua_finire(s, c->piscina));
    p        = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    frontis  = _tecta(&p);
    p        = _pingere(c, s, 180.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_tecta(&p), ZEPHYRUM);
    s->materiae[ZEPHYRUM].bilateralis = VERUM;
    p = _pingere(c, s, 180.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_tecta(&p), frontis);
    /* nodus speculatus (scala x negativa): frons manet frons */
    s->materiae[ZEPHYRUM].bilateralis  = FALSUM;
    s->nodi[ZEPHYRUM].scala            = stereometria_vector(-1.0f,
        1.0f, 1.0f);
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_tecta(&p), frontis);
    p = _pingere(c, s, 180.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_tecta(&p), ZEPHYRUM);
}

/* II x II: rubrum, viride / caeruleum, flavum */
hic_manens constans i8 TEXTURA_QUATTUOR[XVI] = {
    255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255
};

/* II x I: rubrum opacum, rubrum perlucidum */
hic_manens constans i8 TEXTURA_EXCISA[VIII] = {
    255, 0, 0, 255, 255, 0, 0, 0
};

interior vacuum
_sectio_textura (
    CredoContextus* c)
{
           Statua* s = _statuam_parare(c, I, Z_UNUM);
    StatuaPictura  p;
              i32  sinister  = QUADRATI_PRIMUS + V;
              i32  dexter    = QUADRATI_PRIMUS + QUADRATI_LATUS - VI;

    s->materiae[ZEPHYRUM].textura = _texturam_addere(c, s, II, II,
        TEXTURA_QUATTUOR);
    CREDO_NECESSE (statua_finire(s, c->piscina));
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    /* summum sinistrum rubrum, summum dextrum viride, imum sinistrum
     * caeruleum, imum dextrum flavum */
    CREDO_AEQUALIS_I32 (_pixelum(&p, sinister, sinister)[ZEPHYRUM],
        255);
    CREDO_AEQUALIS_I32 (_pixelum(&p, sinister, sinister)[I], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, dexter, sinister)[ZEPHYRUM],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, dexter, sinister)[I], 255);
    CREDO_AEQUALIS_I32 (_pixelum(&p, sinister, dexter)[II], 255);
    CREDO_AEQUALIS_I32 (_pixelum(&p, sinister, dexter)[ZEPHYRUM],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, dexter, dexter)[ZEPHYRUM], 255);
    CREDO_AEQUALIS_I32 (_pixelum(&p, dexter, dexter)[I], 255);
    CREDO_AEQUALIS_I32 (_pixelum(&p, dexter, dexter)[II], ZEPHYRUM);
    /* factor coloris texturam multiplicat */
    s->materiae[ZEPHYRUM].color[ZEPHYRUM] = 0.5f;
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, sinister, sinister)[ZEPHYRUM],
        128);
    /* inclinatus (perspectiva): quadrantes locum tenent */
    s->materiae[ZEPHYRUM].color[ZEPHYRUM] = 1.0f;
    p = _pingere(c, s, 20.0f, 30.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, LATUS / II - VI, LATUS / II - V)
        [ZEPHYRUM], 255);
    CREDO_AEQUALIS_I32 (_pixelum(&p, LATUS / II + VI, LATUS / II + V)
        [I], 255);
}

interior vacuum
_sectio_excisae (
    CredoContextus* c)
{
           Statua* s = _statuam_parare(c, I, Z_UNUM);
    StatuaPictura  p;

    s->materiae[ZEPHYRUM].textura = _texturam_addere(c, s, II, I,
        TEXTURA_EXCISA);
    CREDO_NECESSE (statua_finire(s, c->piscina));
    /* opaca: alpha texturae neglegitur, totum pingitur */
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_tecta(&p), QUADRATI_LATUS * QUADRATI_LATUS);
    /* excisio: dimidium dextrum non pingitur */
    s->materiae[ZEPHYRUM].limen_alpha = 0.5f;
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_tecta(&p),
        QUADRATI_LATUS * QUADRATI_LATUS / II);
    CREDO_VERUM (p.tectum[LATUS / II * LATUS + QUADRATI_PRIMUS + II]);
    CREDO_FALSUM (p.tectum[LATUS / II * LATUS + QUADRATI_PRIMUS
        + QUADRATI_LATUS - III]);
    CREDO_AEQUALIS_I32 (_pixelum(&p, QUADRATI_PRIMUS + QUADRATI_LATUS
        - III, LATUS / II)[I], 255);
    /* factor alpha materiae totum infra limen fert */
    s->materiae[ZEPHYRUM].color[III] = 0.25f;
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_tecta(&p), ZEPHYRUM);
}

interior vacuum
_sectio_lux (
    CredoContextus* c)
{
           Statua* s = _statuam_parare(c, I, Z_UNUM);
    StatuaPictura  p;
              i32  medium = LATUS / II;
              i32  ad_lucem;
              i32  recta;

    CREDO_NECESSE (statua_finire(s, c->piscina));
    /* planum: album plenum */
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, medium, medium)[ZEPHYRUM], 255);
    /* illuminatum, facies recta ad oculum: normalis (0,0,1), lux
     * (-0.5, 0.7, 0.5) / 0.995 -> cos 0.5025 -> 0.35 + 0.65 * 0.5025
     * = 0.6766 -> 173 */
    p      = _pingere(c, s, 0.0f, 0.0f, VERUM);
    recta  = (i32)_pixelum(&p, medium, medium)[ZEPHYRUM];
    CREDO_VERUM (recta >= 172 && recta <= 174);
    CREDO_AEQUALIS_I32 (_pixelum(&p, medium, medium)[I], recta);
    /* facies ad lucem versa (gyrus -XL: normalis ad sinistram)
     * clarior */
    p         = _pingere(c, s, -40.0f, 0.0f, VERUM);
    ad_lucem  = (i32)_pixelum(&p, medium, medium)[ZEPHYRUM];
    CREDO_VERUM (ad_lucem > recta + X);
    /* a luce aversa (gyrus LXXX): fundus solus, XXXV % = 89 */
    p = _pingere(c, s, 80.0f, 0.0f, VERUM);
    CREDO_VERUM (p.tectum[medium * LATUS + medium]);
    CREDO_VERUM (_pixelum(&p, medium, medium)[ZEPHYRUM] >= 88
        && _pixelum(&p, medium, medium)[ZEPHYRUM] <= 90);
    /* emissiva lucem neglegit */
    s->materiae[ZEPHYRUM].emissiva = VERUM;
    p = _pingere(c, s, 80.0f, 0.0f, VERUM);
    CREDO_AEQUALIS_I32 (_pixelum(&p, medium, medium)[ZEPHYRUM], 255);
    /* lux visui affixa: oculus INFRA (inclinatio negativa) faciem
     * sursum ad lucem superam vertit */
    s->materiae[ZEPHYRUM].emissiva = FALSUM;
    p = _pingere(c, s, 0.0f, -40.0f, VERUM);
    CREDO_VERUM ((i32)_pixelum(&p, medium, medium)[ZEPHYRUM]
        > recta + X);
}

interior vacuum
_sectio_margo (
    CredoContextus* c)
{
    hic_manens constans f32 GYRI[IV] = { 0.0f, 90.0f, 180.0f, 270.0f };
    hic_manens constans f32 INCLINATIONES[III] = {
        -85.0f, 15.0f, 85.0f
    };
    hic_manens constans i32  MENSURAE[IV] = { 360, 450, 450, 360 };
              StatuaFructus  f;
                     Statua* s;
         StereometriaMatrix* m;
              StatuaPictura  p;
                StatuaVisus  visus;
                     chorda  g;
                        i32  forma;
                        i32  gyrus;
                        i32  inclinatio;
                        i32  cyclus;
                        i32  x;
                        i32  y;
                        i32  in_ora;
                        i32  tecta;

    g = filum_legere_totum(VIA_LAPIDIS, c->piscina);
    f = statua_glb_legere(g, c->piscina);
    CREDO_NECESSE (f.successus);
    s = f.statua;
    m = (StereometriaMatrix*)_locus(c,
        s->nodorum * magnitudo(StereometriaMatrix));
    visus.illuminatum = VERUM;
    per (forma = ZEPHYRUM; forma < II; forma++)
    {
        per (gyrus = ZEPHYRUM; gyrus < IV; gyrus++)
        {
            per (inclinatio = ZEPHYRUM; inclinatio < III; inclinatio++)
            {
                /* quies: sphaera eam tegit, ora numquam tangitur */
                statua_positura(s, -I, 0.0f, m);
                visus.gyrus       = GYRI[gyrus];
                visus.inclinatio  = INCLINATIONES[inclinatio];
                CREDO_NECESSE (statua_pingere(s, m, &visus,
                    MENSURAE[forma * II], MENSURAE[forma * II + I],
                    &p, c->piscina));
                in_ora  = ZEPHYRUM;
                tecta   = ZEPHYRUM;
                per (y = ZEPHYRUM; y < p.altitudo; y++)
                {
                    per (x = ZEPHYRUM; x < p.latitudo; x++)
                    {
                        si (!p.tectum[y * p.latitudo + x])
                        {
                            perge;
                        }
                        tecta++;
                        si (   x == ZEPHYRUM || y == ZEPHYRUM
                            || x == p.latitudo - I
                            || y == p.altitudo - I)
                        {
                            in_ora++;
                        }
                    }
                }
                CREDO_AEQUALIS_I32 (in_ora, ZEPHYRUM);
                /* et statua magna in pictura: inter VIII % et LX % */
                CREDO_VERUM (tecta > p.latitudo * p.altitudo * II
                    / XXV);
                CREDO_VERUM (tecta < p.latitudo * p.altitudo * III / V);
                /* cyclus (saxum volans picturam relinquere potest):
                 * pingitur sine ruina, statua visibilis manet */
                cyclus = (gyrus + inclinatio) % II;
                statua_positura(s, (s32)cyclus,
                    0.3f * (f32)(gyrus + I), m);
                CREDO_NECESSE (statua_pingere(s, m, &visus,
                    MENSURAE[forma * II], MENSURAE[forma * II + I],
                    &p, c->piscina));
                tecta = ZEPHYRUM;
                per (y = ZEPHYRUM; y < p.altitudo * p.latitudo; y++)
                {
                    tecta += p.tectum[y] ? I : ZEPHYRUM;
                }
                CREDO_VERUM (tecta > p.latitudo * p.altitudo / XXV);
            }
        }
    }
}

interior vacuum
_prope_pingere (
     Statua* s,
    Piscina* piscina)
{
    StereometriaMatrix m[I];
         StatuaPictura p;
           StatuaVisus visus;
                   i32 gyrus;

    statua_positura(s, -I, 0.0f, m);
    visus.inclinatio   = 10.0f;
    visus.illuminatum  = VERUM;
    per (gyrus = ZEPHYRUM; gyrus < 360; gyrus += VII)
    {
        visus.gyrus = (f32)gyrus;
        (vacuum)statua_pingere(s, m, &visus, LATUS, LATUS, &p, piscina);
    }
}

interior vacuum
_sectio_prope (
    CredoContextus* c)
{
           Statua* s = _statuam_parare(c, II, Z_DUO);
    StatuaPictura  p;
              i32  k;

    s->materiae[ZEPHYRUM].bilateralis  = VERUM;
    s->materiae[I].bilateralis         = VERUM;
    CREDO_NECESSE (statua_finire(s, c->piscina));
    /* sphaera mendax parva: geometria (latus II) oculum circumdat,
     * triangula planum propinquum secant et post oculum iacent */
    s->radius = 0.05f;
    CREDO_NON_RUIT (_prope_pingere(s, c->piscina));
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    /* quadratum immane picturam totam tegit */
    CREDO_AEQUALIS_I32 (_tecta(&p), LATUS * LATUS);
    /* a latere (gyrus XC): planum per oculum transit - nihil ruit */
    p = _pingere(c, s, 90.0f, 0.0f, FALSUM);
    CREDO_VERUM (_tecta(&p) <= LATUS * LATUS);
    /* triangulum degeneratum (tres vertices in linea): nihil */
    s = _statuam_parare(c, I, Z_UNUM);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        s->positiones[k * III + I] = 0.0f;
    }
    CREDO_NECESSE (statua_finire(s, c->piscina));
    p = _pingere(c, s, 0.0f, 0.0f, FALSUM);
    CREDO_AEQUALIS_I32 (_tecta(&p), ZEPHYRUM);
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
                Statua* s = _statuam_parare(c, I, Z_UNUM);
    StereometriaMatrix  m[I];
         StatuaPictura  p;

    CREDO_NECESSE (statua_finire(s, c->piscina));
    statua_positura(s, -I, 0.0f, m);
    CREDO_VERUM (statua_pingere(s, m, NIHIL, ZEPHYRUM, ZEPHYRUM, &p,
        c->piscina));
    CREDO_AEQUALIS_I32 (p.latitudo, ZEPHYRUM);
    CREDO_VERUM (statua_pingere(s, m, NIHIL, X, ZEPHYRUM, &p,
        c->piscina));
    CREDO_VERUM (statua_pingere(s, m, NIHIL, I, I, &p, c->piscina));
    CREDO_AEQUALIS_I32 (p.latitudo, I);
    CREDO_VERUM (p.rgba != NIHIL && p.tectum != NIHIL);
    CREDO_VERUM (statua_pingere(s, m, NIHIL, I, 100, &p, c->piscina));
    CREDO_VERUM (statua_pingere(s, m, NIHIL, 100, I, &p, c->piscina));
    /* argumenta nulla */
    CREDO_FALSUM (statua_pingere(NIHIL, m, NIHIL, X, X, &p,
        c->piscina));
    CREDO_FALSUM (statua_pingere(s, m, NIHIL, X, X, NIHIL,
        c->piscina));
    CREDO_FALSUM (statua_pingere(s, NIHIL, NIHIL, X, X, &p,
        c->piscina));
}

interior vacuum
_sectio_determinatum (
    CredoContextus* c)
{
         StatuaFructus  f;
                Statua* s;
    StereometriaMatrix* m;
         StatuaPictura  a;
         StatuaPictura  b;
                chorda  g = filum_legere_totum(VIA_LAPIDIS, c->piscina);

    f = statua_glb_legere(g, c->piscina);
    CREDO_NECESSE (f.successus);
    s = f.statua;
    m = (StereometriaMatrix*)_locus(c,
        s->nodorum * magnitudo(StereometriaMatrix));
    statua_positura(s, ZEPHYRUM, 0.35f, m);
    CREDO_NECESSE (statua_pingere(s, m, NIHIL, 360, 450, &a,
        c->piscina));
    CREDO_NECESSE (statua_pingere(s, m, NIHIL, 360, 450, &b,
        c->piscina));
    CREDO_VERUM (memcmp(a.rgba, b.rgba, 360 * 450 * IV) == ZEPHYRUM);
    CREDO_VERUM (memcmp(a.tectum, b.tectum, 360 * 450) == ZEPHYRUM);
    /* positura alia picturam mutat (cyclus vere movet) */
    statua_positura(s, ZEPHYRUM, 0.7f, m);
    CREDO_NECESSE (statua_pingere(s, m, NIHIL, 360, 450, &b,
        c->piscina));
    CREDO_VERUM (memcmp(a.rgba, b.rgba, 360 * 450 * IV) != ZEPHYRUM);
}

/* digesta (SHA-256 RGBA) lapidis: cyclus 0 ad 0.35 s, visus
 * ordinarius, 360 x 450 - fixa 2026-11-09 postquam Franus picturas
 * vidit et probavit (statuae-plan T4 / T6). Mutatio pictoris quae
 * haec movet oculo iudicanda est, deinde refigenda */
#define DIGESTUM_ILLUMINATI \
    "67937aab0f600d0f10f552c9a59dbd473be27a147e0c2b0aab5ca08cc4e13d6a"
#define DIGESTUM_PLANI \
    "b51d57ceef053ec7a3c924a9b305edf12da859ad27fe1087d5ea8c316e08046a"

interior vacuum
_sectio_fixa (
    CredoContextus* c)
{
    hic_manens constans character* constans SPERATA[II] = {
        DIGESTUM_PLANI, DIGESTUM_ILLUMINATI
    };
         StatuaFructus  f;
                Statua* s;
    StereometriaMatrix* m;
         StatuaPictura  p;
           StatuaVisus  visus;
              Sigillum  sigillum;
             character  hex[SIGILLUM_HEX_MENSURA];
                chorda  g;
                   i32  k;

    g = filum_legere_totum(VIA_LAPIDIS, c->piscina);
    f = statua_glb_legere(g, c->piscina);
    CREDO_NECESSE (f.successus);
    s = f.statua;
    m = (StereometriaMatrix*)_locus(c,
        s->nodorum * magnitudo(StereometriaMatrix));
    statua_positura(s, ZEPHYRUM, 0.35f, m);
    visus.gyrus       = STATUA_GYRUS_ORDINARIUS;
    visus.inclinatio  = STATUA_INCLINATIO_ORDINARIA;
    per (k = ZEPHYRUM; k < II; k++)
    {
        visus.illuminatum = (b32)k;
        CREDO_NECESSE (statua_pingere(s, m, &visus, 360, 450, &p,
            c->piscina));
        sigillum = sigillum_computare(p.rgba, 360 * 450 * IV);
        sigillum_hex(&sigillum, hex);
        si (strcmp(hex, SPERATA[k]) != ZEPHYRUM)
        {
            printf("  digestum %s: %s\n", k ? "illuminati" : "plani",
                hex);
        }
        CREDO_VERUM (strcmp(hex, SPERATA[k]) == ZEPHYRUM);
    }
}


/* ==================================================
 * Scaena (mundus-plan T3): statuae locatae, visus parallelus
 * ================================================== */

#define SCAENAE_LATUS   LXIV

/* scaena in positura quietis; matrices cuiusque statuae hic fiunt */
interior StatuaScaenaPictura
_scaenam_pingere (
           CredoContextus* c,
                   Statua* constans* statuae,
    constans StereometriaMatrix* locationes,
                      i32  numerus,
                      f32  gyrus,
                      f32  inclinatio,
                      s32  origo_x,
                      s32  origo_y)
{
           StatuaLocata* l;
    StatuaScaenaPictura  p;
      StatuaScaenaVisus  visus;
     StereometriaMatrix* m;
                    i32  k;

    memset(&p, ZEPHYRUM, magnitudo(p));
    l = (StatuaLocata*)_locus(c, (numerus
        + I) * magnitudo(StatuaLocata));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        m = (StereometriaMatrix*)_locus(c,
            (statuae[k]->nodorum + I) * magnitudo(StereometriaMatrix));
        statua_positura(statuae[k], -I, 0.0f, m);
        l[k].statua    = statuae[k];
        l[k].matrices  = m;
        l[k].locatio   = locationes ? locationes[k]
            : stereometria_identitas();
    }
    visus.gyrus                = gyrus;
    visus.inclinatio           = inclinatio;
    visus.pixela_per_unitatem  = 10.0f;
    visus.origo_x              = origo_x;
    visus.origo_y              = origo_y;
    visus.illuminatum          = FALSUM;
    CREDO_VERUM (statua_scaenam_pingere(l, numerus, &visus,
        SCAENAE_LATUS, SCAENAE_LATUS, &p, c->piscina));
    redde p;
}

interior memoriae_index
_scaenae_locus (
    i32 x,
    i32 y)
{
    redde (memoriae_index)y * SCAENAE_LATUS + x;
}

/* quadratum cum textura II x II (ruber, viridis / caeruleus, flavus) */
interior Statua*
_statuam_cum_textura (
    CredoContextus* c)
{
    hic_manens constans i8 QUADRANTES[XVI] = {
        255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255
    };
    Statua* s = _statuam_parare(c, I, Z_UNUM);

    s->materiae[ZEPHYRUM].textura = _texturam_addere(c, s, II, II,
        QUADRANTES);
    CREDO_VERUM (statua_finire(s, c->piscina));
    redde s;
}

interior vacuum
_sectio_scaena_proicere (
    CredoContextus* c)
{
    StatuaScaenaVisus visus;
                  f32 x            = 0.0f;
                  f32 y            = 0.0f;
                  f32 profunditas  = 0.0f;

    (vacuum)c;
    memset(&visus, ZEPHYRUM, magnitudo(visus));
    visus.pixela_per_unitatem = 10.0f;
    /* origo picturae proiectionem non movet: pixela MUNDI */
    visus.origo_x = 77;
    visus.origo_y = -5;
    statua_scaena_proicere(&visus, stereometria_vector(1.0f, 2.0f,
        3.0f),
        &x, &y, &profunditas);
    CREDO_F32_PROXIMUS (x, 10.0f, 0.0001f);
    CREDO_F32_PROXIMUS (y, -20.0f, 0.0001f);
    CREDO_F32_PROXIMUS (profunditas, -3.0f, 0.0001f);
    /* gyrus XC: +x mundi ab oculo recedit (frons statuae ad dextram
     * vertitur, ut in statua_pingere) */
    visus.gyrus = 90.0f;
    statua_scaena_proicere(&visus, stereometria_vector(0.0f, 0.0f,
        1.0f),
        &x, &y, &profunditas);
    CREDO_F32_PROXIMUS (x, 10.0f, 0.0001f);
    CREDO_F32_PROXIMUS (profunditas, 0.0f, 0.0001f);
    /* inclinatio XC: oculus desuper, +y mundi ad oculum */
    visus.gyrus       = 0.0f;
    visus.inclinatio  = 90.0f;
    statua_scaena_proicere(&visus, stereometria_vector(0.0f, 1.0f,
        0.0f),
        &x, &y, &profunditas);
    CREDO_F32_PROXIMUS (y, 0.0f, 0.0001f);
    CREDO_F32_PROXIMUS (profunditas, -1.0f, 0.0001f);
}

/* origines pixelis integris diversae: pixela communia eadem octetim
 * (rgba, tectum, profunditas) */
interior vacuum
_sectio_scaena_lapsus (
    CredoContextus* c)
{
                 Statua* s = _statuam_cum_textura(c);
    StatuaScaenaPictura  a;
    StatuaScaenaPictura  b;
                    i32  x;
                    i32  y;
                    i32  communia  = ZEPHYRUM;
                    i32  tecta     = ZEPHYRUM;
                    i32  diversa   = ZEPHYRUM;

    a = _scaenam_pingere(c, &s, NIHIL, I, 45.0f, 35.0f, -XXXII, -XXXII);
    b = _scaenam_pingere(c, &s, NIHIL, I, 45.0f, 35.0f, -XXIX, -XXXVII);
    /* pixelum mundi (wx, wy): in a ad (wx + 32, wy + 32), in b ad
     * (wx + 29, wy + 37) -> b(x, y) = a(x + 3, y - 5) */
    per (y = V; y < SCAENAE_LATUS; y++)
    {
        per (x = ZEPHYRUM; x + III < SCAENAE_LATUS; x++)
        {
            communia++;
            tecta += b.tectum[_scaenae_locus(x, y)] ? I : ZEPHYRUM;
            si (   memcmp(b.rgba + _scaenae_locus(x, y) * IV,
                       a.rgba + _scaenae_locus(x + III, y - V) * IV, IV)
                || b.tectum[_scaenae_locus(x, y)]
                   != a.tectum[_scaenae_locus(x + III, y - V)]
                || memcmp(&b.profunditas[_scaenae_locus(x, y)],
                       &a.profunditas[_scaenae_locus(x + III, y - V)],
                       magnitudo(f32)))
            {
                diversa++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (diversa, ZEPHYRUM);
    /* probatio non vacua: statua partem picturae tegit, non totam */
    CREDO_VERUM (tecta > C);
    CREDO_VERUM (tecta < communia);
}

/* duae statuae: propior tegit, tectum nominat; ordo pingendi nihil
 * mutat praeter numeros tecti */
interior vacuum
_sectio_scaena_tectum (
    CredoContextus* c)
{
                 Statua* statuae[II];
                 Statua* inversae[II];
     StereometriaMatrix  locationes[II];
     StereometriaMatrix  locationes_inversae[II];
    StatuaScaenaPictura  p;
    StatuaScaenaPictura  q;
                    i32  k;

    statuae[ZEPHYRUM]  = _statuam_parare(c, I, Z_UNUM);
    statuae[I]         = _statuam_parare(c, I, Z_UNUM);
    /* altera rubra, unitate dextrorsum et unitate remotior */
    statuae[I]->materiae[ZEPHYRUM].color[I]   = 0.0f;
    statuae[I]->materiae[ZEPHYRUM].color[II]  = 0.0f;
    CREDO_NECESSE (statua_finire(statuae[ZEPHYRUM], c->piscina));
    CREDO_NECESSE (statua_finire(statuae[I], c->piscina));
    locationes[ZEPHYRUM]  = stereometria_identitas();
    locationes[I]         = stereometria_translatio(
        stereometria_vector(1.0f, 0.0f, -1.0f));
    p = _scaenam_pingere(c, statuae, locationes, II, 0.0f, 0.0f, -XXXII,
        -XXXII);
    /* prima x in [-10, 10), altera in [0, 20); y ambae [-10, 10) */
    CREDO_AEQUALIS_I32 (p.tectum[_scaenae_locus(XXXII - V, XXXII)], I);
    CREDO_AEQUALIS_I32 (p.tectum[_scaenae_locus(XXXII + V, XXXII)], I);
    CREDO_AEQUALIS_I32 (p.tectum[_scaenae_locus(XXXII + XV, XXXII)],
        II);
    CREDO_AEQUALIS_I32 (p.tectum[_scaenae_locus(XXXII + XXV, XXXII)],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (p.tectum[_scaenae_locus(XXXII - XV, XXXII)],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (p.rgba[_scaenae_locus(XXXII + V, XXXII) * IV
        + I],
        255);
    CREDO_AEQUALIS_I32 (p.rgba[_scaenae_locus(XXXII + XV, XXXII) * IV
        + I],
        ZEPHYRUM);
    CREDO_F32_PROXIMUS (p.profunditas[_scaenae_locus(XXXII + V, XXXII)],
        0.0f, 0.0001f);
    CREDO_F32_PROXIMUS (p.profunditas[_scaenae_locus(XXXII + XV,
        XXXII)],
        1.0f, 0.0001f);
    /* vacuum: album, profunditas maxima */
    CREDO_AEQUALIS_I32 (p.rgba[ZEPHYRUM], 255);
    CREDO_VERUM (p.profunditas[ZEPHYRUM] > 1.0e20f);
    /* ordo inversus */
    inversae[ZEPHYRUM]             = statuae[I];
    inversae[I]                    = statuae[ZEPHYRUM];
    locationes_inversae[ZEPHYRUM]  = locationes[I];
    locationes_inversae[I]         = locationes[ZEPHYRUM];
    q = _scaenam_pingere(c, inversae, locationes_inversae, II, 0.0f,
        0.0f,
        -XXXII, -XXXII);
    CREDO_VERUM (memcmp(p.rgba, q.rgba,
        SCAENAE_LATUS * SCAENAE_LATUS * IV) == ZEPHYRUM);
    CREDO_VERUM (memcmp(p.profunditas, q.profunditas,
        SCAENAE_LATUS * SCAENAE_LATUS * magnitudo(f32)) == ZEPHYRUM);
    per (k = ZEPHYRUM; k < SCAENAE_LATUS * SCAENAE_LATUS; k++)
    {
        si (   (p.tectum[k] == ZEPHYRUM) != (q.tectum[k] == ZEPHYRUM)
            || (p.tectum[k] && p.tectum[k] + q.tectum[k] != III))
        {
            frange;
        }
    }
    CREDO_AEQUALIS_I32 (k, SCAENAE_LATUS * SCAENAE_LATUS);
}

/* locatio: translatio unitatis = X pixela exacte */
interior vacuum
_sectio_scaena_locatio (
    CredoContextus* c)
{
                 Statua* s = _statuam_cum_textura(c);
     StereometriaMatrix  locatio;
    StatuaScaenaPictura  a;
    StatuaScaenaPictura  b;
                    i32  x;
                    i32  y;
                    i32  diversa = ZEPHYRUM;

    locatio = stereometria_translatio(stereometria_vector(1.0f, -0.5f,
        0.0f));
    a = _scaenam_pingere(c, &s, NIHIL, I, 0.0f, 0.0f, -XXXII, -XXXII);
    b = _scaenam_pingere(c, &s, &locatio, I, 0.0f, 0.0f, -XXXII,
        -XXXII);
    /* +x mundi: X pixela dextrorsum; -0.5 y mundi: V pixela deorsum */
    per (y = V; y < SCAENAE_LATUS; y++)
    {
        per (x = X; x < SCAENAE_LATUS; x++)
        {
            si (   memcmp(b.rgba + _scaenae_locus(x, y) * IV,
                       a.rgba + _scaenae_locus(x - X, y - V) * IV, IV)
                || b.tectum[_scaenae_locus(x, y)]
                   != a.tectum[_scaenae_locus(x - X, y - V)])
            {
                diversa++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (diversa, ZEPHYRUM);
    /* quadratum [-10, 10) pixela: XX x XX tecta, textura: quadrans
     * summus sinister ruber */
    y = ZEPHYRUM;
    per (x = ZEPHYRUM; x < SCAENAE_LATUS * SCAENAE_LATUS; x++)
    {
        y += a.tectum[x] ? I : ZEPHYRUM;
    }
    CREDO_AEQUALIS_I32 (y, XX * XX);
    CREDO_AEQUALIS_I32 (a.rgba[_scaenae_locus(XXVII, XXVII) * IV], 255);
    CREDO_AEQUALIS_I32 (a.rgba[_scaenae_locus(XXVII, XXVII) * IV + I],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (a.rgba[_scaenae_locus(XXXVII, XXXVII) * IV + I],
        255);
    CREDO_AEQUALIS_I32 (a.rgba[_scaenae_locus(XXXVII, XXXVII) * IV],
        255);
}

/* tergum: a tergo visum non pingitur nisi bilateralis */
interior vacuum
_sectio_scaena_tergum (
    CredoContextus* c)
{
                 Statua* s = _statuam_parare(c, I, Z_UNUM);
    StatuaScaenaPictura  p;
                    i32  k;
                    i32  tecta = ZEPHYRUM;

    CREDO_NECESSE (statua_finire(s, c->piscina));
    p = _scaenam_pingere(c, &s, NIHIL, I, 180.0f, 0.0f, -XXXII, -XXXII);
    per (k = ZEPHYRUM; k < SCAENAE_LATUS * SCAENAE_LATUS; k++)
    {
        tecta += p.tectum[k] ? I : ZEPHYRUM;
    }
    CREDO_AEQUALIS_I32 (tecta, ZEPHYRUM);
    s->materiae[ZEPHYRUM].bilateralis = VERUM;
    p = _scaenam_pingere(c, &s, NIHIL, I, 180.0f, 0.0f, -XXXII, -XXXII);
    per (k = ZEPHYRUM; k < SCAENAE_LATUS * SCAENAE_LATUS; k++)
    {
        tecta += p.tectum[k] ? I : ZEPHYRUM;
    }
    CREDO_AEQUALIS_I32 (tecta, XX * XX);
}

interior vacuum
_sectio_scaena_vacua (
    CredoContextus* c)
{
                 Statua* s = _statuam_parare(c, I, Z_UNUM);
     StereometriaMatrix  m[II];
           StatuaLocata  l[II];
      StatuaScaenaVisus  visus;
    StatuaScaenaPictura  p;
                    i32  k;

    CREDO_NECESSE (statua_finire(s, c->piscina));
    statua_positura(s, -I, 0.0f, m);
    memset(l, ZEPHYRUM, magnitudo(l));
    l[ZEPHYRUM].statua    = s;
    l[ZEPHYRUM].matrices  = m;
    l[ZEPHYRUM].locatio   = stereometria_identitas();
    memset(&visus, ZEPHYRUM, magnitudo(visus));
    visus.pixela_per_unitatem = 10.0f;
    CREDO_VERUM (statua_scaenam_pingere(l, I, &visus, ZEPHYRUM,
        ZEPHYRUM,
        &p, c->piscina));
    CREDO_VERUM (statua_scaenam_pingere(l, I, &visus, I, I, &p,
        c->piscina));
    CREDO_AEQUALIS_I32 (p.tectum[ZEPHYRUM], I);
    /* nulla statua: pictura alba */
    CREDO_VERUM (statua_scaenam_pingere(NIHIL, ZEPHYRUM, &visus, VIII,
        VIII, &p, c->piscina));
    per (k = ZEPHYRUM; k < LXIV; k++)
    {
        si (p.tectum[k] || p.rgba[k * IV] != 255)
        {
            frange;
        }
    }
    CREDO_AEQUALIS_I32 (k, LXIV);
    /* recusationes */
    CREDO_FALSUM (statua_scaenam_pingere(l, STATUA_LOCATAE_MAXIMAE + I,
        &visus, VIII, VIII, &p, c->piscina));
    CREDO_FALSUM (statua_scaenam_pingere(l, I, NIHIL, VIII, VIII, &p,
        c->piscina));
    CREDO_FALSUM (statua_scaenam_pingere(NIHIL, I, &visus, VIII, VIII,
        &p, c->piscina));
    visus.pixela_per_unitatem = 0.0f;
    CREDO_FALSUM (statua_scaenam_pingere(l, I, &visus, VIII, VIII, &p,
        c->piscina));
    visus.pixela_per_unitatem  = 10.0f;
    l[ZEPHYRUM].statua         = NIHIL;
    CREDO_FALSUM (statua_scaenam_pingere(l, I, &visus, VIII, VIII, &p,
        c->piscina));
}

/* lapis in quiete, XLV / XXXV, XL pixela per unitatem, CXXVIII
 * quadrata in pedibus eius posita: digestum fixum + determinatum */
#define DIGESTUM_SCAENAE \
    "0d3985455f6af5d0b52063d425d1bc170e5b541e242fb7487a12dc075a4a7ce8"

interior vacuum
_sectio_scaena_fixa (
    CredoContextus* c)
{
          StatuaFructus  f;
                 Statua* s;
     StereometriaMatrix* m;
           StatuaLocata  l;
      StatuaScaenaVisus  visus;
    StatuaScaenaPictura  p;
    StatuaScaenaPictura  q;
               Sigillum  sigillum;
              character  hex[SIGILLUM_HEX_MENSURA];
                    f32  x            = 0.0f;
                    f32  y            = 0.0f;
                    f32  profunditas  = 0.0f;
                    i32  k;
                    i32  tecta = ZEPHYRUM;

    f = statua_glb_legere(filum_legere_totum(VIA_LAPIDIS, c->piscina),
        c->piscina);
    CREDO_NECESSE (f.successus);
    s = f.statua;
    m = (StereometriaMatrix*)_locus(c,
        (s->nodorum + I) * magnitudo(StereometriaMatrix));
    statua_positura(s, -I, 0.0f, m);
    l.statua    = s;
    l.matrices  = m;
    l.locatio   = stereometria_identitas();
    memset(&visus, ZEPHYRUM, magnitudo(visus));
    visus.gyrus                = 45.0f;
    visus.inclinatio           = 35.0f;
    visus.pixela_per_unitatem  = 40.0f;
    visus.illuminatum          = VERUM;
    statua_scaena_proicere(&visus, s->centrum, &x, &y, &profunditas);
    visus.origo_x = (s32)x - LXIV;
    visus.origo_y = (s32)y - LXIV;
    CREDO_NECESSE (statua_scaenam_pingere(&l, I, &visus, CXXVIII,
        CXXVIII,
        &p, c->piscina));
    CREDO_NECESSE (statua_scaenam_pingere(&l, I, &visus, CXXVIII,
        CXXVIII,
        &q, c->piscina));
    CREDO_VERUM (memcmp(p.rgba, q.rgba, CXXVIII * CXXVIII * IV)
        == ZEPHYRUM);
    per (k = ZEPHYRUM; k < CXXVIII * CXXVIII; k++)
    {
        tecta += p.tectum[k] ? I : ZEPHYRUM;
    }
    CREDO_VERUM (tecta > M);
    sigillum = sigillum_computare(p.rgba, CXXVIII * CXXVIII * IV);
    sigillum_hex(&sigillum, hex);
    si (strcmp(hex, DIGESTUM_SCAENAE) != ZEPHYRUM)
    {
        printf("  digestum scaenae: %s (tecta %u)\n", hex, tecta);
    }
    CREDO_VERUM (strcmp(hex, DIGESTUM_SCAENAE) == ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "quadratum", _sectio_quadratum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "profunditas", _sectio_profunditas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tergum", _sectio_tergum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textura", _sectio_textura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "excisio", _sectio_excisae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lux", _sectio_lux, NIHIL, NIHIL, NIHIL, NIHIL },
    { "margo", _sectio_margo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "prope", _sectio_prope, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacua", _sectio_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "determinatum", _sectio_determinatum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fixa", _sectio_fixa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "scaena_proicere", _sectio_scaena_proicere, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scaena_lapsus", _sectio_scaena_lapsus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scaena_tectum", _sectio_scaena_tectum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scaena_locatio", _sectio_scaena_locatio, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scaena_tergum", _sectio_scaena_tergum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scaena_vacua", _sectio_scaena_vacua, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scaena_fixa", _sectio_scaena_fixa, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("statua_pictura", SECTIONES);
}
