/* probatio_statua_glb.c - lector GLB (statua_glb.h)
 *
 * fixa: lapis et fungus (exportationes PixMesh Frani) numeris notis
 * leguntur. numeri integri: JSON '2' ubi fluitans exspectatur.
 * approximata: BLEND, factor coloris, REPEAT, coordinatae u16
 * normalizatae, imago inserta. recusationes: plagula manu facta
 * quaeque causam suam dat. truncata et corrupta: numquam ruit.
 *
 * Plagulae manu factae: exemplar JSON cum foraminibus (_glb) super
 * BIN unum - triangulum, indices, claves, coordinatae, PNG II x I.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_glb.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define EPSILON 0.0001f

#define VIA_LAPIDIS "probationes/fixa/statuae/lapis.glb"
#define VIA_FUNGI   "probationes/fixa/statuae/fungus.glb"

/* PNG II x I: pixelum rubrum opacum, pixelum rubrum perlucidum */
hic_manens constans i8 PNG_PARVUM[LXXII] = {
    137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0,
    0, 0, 2, 0, 0, 0, 1, 8, 6, 0, 0, 0, 244, 34, 127, 138, 0, 0, 0,
    15, 73, 68, 65, 84, 120, 156, 99, 248, 207, 192, 240, 31, 136,
    25, 0, 16, 248, 2, 254, 172, 233, 70, 248, 0, 0, 0, 0, 73, 69,
    78, 68, 174, 66, 96, 130
};

#define BIN_MENSURA CLX

/* BIN commune:
 *    0  positiones III x vec3 f32: (0,0,0) (1,0,0) (0,1,0)
 *   36  indices III x u16: 0 1 2 (+ II complementi)
 *   44  tempora II x f32: 0, 1
 *   52  valores II x vec3 f32: (0,0,0) (2,0,0)
 *   76  coordinatae III x vec2 u16: (0,0) (65535,0) (0,65535)
 *   88  PNG (LXXII octeti) */
interior vacuum
_bin_implere (
    i8* bin)
{
    f32 positiones[IX] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };
    i16 indices[IV]      = { 0, 1, 2, 0 };
    f32 tempora[II]      = { 0.0f, 1.0f };
    f32 valores[VI]      = { 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f };
    i16 coordinatae[VI]  = { 0, 65535, 0, 0, 0, 65535 };

    memset(bin, ZEPHYRUM, BIN_MENSURA);
    memcpy(bin, positiones, XXXVI);
    memcpy(bin + XXXVI, indices, VIII);
    memcpy(bin + XLIV, tempora, VIII);
    memcpy(bin + LII, valores, XXIV);
    /* (0,0) (65535,0) (0,65535) */
    coordinatae[I]    = 0;
    coordinatae[II]   = 65535;
    coordinatae[III]  = 0;
    coordinatae[IV]   = 0;
    coordinatae[V]    = 65535;
    memcpy(bin + LXXVI, coordinatae, XII);
    memcpy(bin + LXXXVIII, PNG_PARVUM, LXXII);
}

interior vacuum
_u32_ponere (
     i8* p,
    i32  v)
{
    p[ZEPHYRUM]  = (i8)(v & 0xFF);
    p[I]         = (i8)((v >> VIII) & 0xFF);
    p[II]        = (i8)((v >> XVI) & 0xFF);
    p[III]       = (i8)((v >> XXIV) & 0xFF);
}

/* continens GLB ex textu JSON et BIN communi */
interior chorda
_glb_ex_json (
        CredoContextus* c,
    constans character* json)
{
    chorda g;
       i32 longitudo     = (i32)strlen(json);
       i32 json_mensura  = (longitudo + III) & ~(i32)III;
       i32 n;

    g.mensura  = XII + VIII + json_mensura + VIII + BIN_MENSURA;
    g.datum    = (i8*)piscina_allocare(c->piscina, g.mensura);
    memcpy(g.datum, "glTF", IV);
    _u32_ponere(g.datum + IV, II);
    _u32_ponere(g.datum + VIII, g.mensura);
    _u32_ponere(g.datum + XII, json_mensura);
    memcpy(g.datum + XVI, "JSON", IV);
    memcpy(g.datum + XX, json, longitudo);
    per (n = longitudo; n < json_mensura; n++)
    {
        g.datum[XX + n] = ' ';
    }
    _u32_ponere(g.datum + XX + json_mensura, BIN_MENSURA);
    memcpy(g.datum + XX + json_mensura + IV, "BIN\0", IV);
    _bin_implere(g.datum + XX + json_mensura + VIII);
    redde g;
}

/* exemplar: summum extra, nodi, attributa extra, primitivum extra,
 * numerus indicum, accessor indicum extra, plagula extra */
interior chorda
_glb (
        CredoContextus* c,
    constans character* summum,
    constans character* nodi,
    constans character* attributa,
    constans character* primitivum,
    constans character* indicum,
    constans character* accessor,
    constans character* plagula)
{
    character* json = (character*)piscina_allocare(c->piscina, 0x1000);

    sprintf(json,
        "{\"asset\":{\"version\":\"2.0\"}%s,"
        "\"scenes\":[{\"nodes\":[0]}],"
        "\"nodes\":[%s],"
        "\"meshes\":[{\"primitives\":[{\"attributes\":"
        "{\"POSITION\":0%s},\"indices\":1%s}]}],"
        "\"buffers\":[{\"byteLength\":160%s}],"
        "\"bufferViews\":["
        "{\"buffer\":0,\"byteOffset\":0,\"byteLength\":36},"
        "{\"buffer\":0,\"byteOffset\":36,\"byteLength\":6},"
        "{\"buffer\":0,\"byteOffset\":44,\"byteLength\":8},"
        "{\"buffer\":0,\"byteOffset\":52,\"byteLength\":24},"
        "{\"buffer\":0,\"byteOffset\":76,\"byteLength\":12},"
        "{\"buffer\":0,\"byteOffset\":88,\"byteLength\":72}],"
        "\"accessors\":["
        "{\"bufferView\":0,\"componentType\":5126,\"count\":3,"
        "\"type\":\"VEC3\"},"
        "{\"bufferView\":1,\"componentType\":5123,\"count\":%s,"
        "\"type\":\"SCALAR\"%s},"
        "{\"bufferView\":2,\"componentType\":5126,\"count\":2,"
        "\"type\":\"SCALAR\"},"
        "{\"bufferView\":3,\"componentType\":5126,\"count\":2,"
        "\"type\":\"VEC3\"},"
        "{\"bufferView\":4,\"componentType\":5123,\"count\":3,"
        "\"type\":\"VEC2\",\"normalized\":true}]}",
        summum, nodi, attributa, primitivum, plagula, indicum,
        accessor);
    redde _glb_ex_json(c, json);
}

/* plagula sana minima */
interior chorda
_glb_sanum (
    CredoContextus* c)
{
    redde _glb(c, "", "{\"mesh\":0}", "", "", "3", "", "");
}

#define ANIMATIO(interpolatio) \
    ",\"animations\":[{\"name\":\"it\",\"samplers\":[{\"input\":2," \
    "\"output\":3" interpolatio "}],\"channels\":[{\"sampler\":0," \
    "\"target\":{\"node\":0,\"path\":\"translation\"}}]}]"

/* recusatur, et causa fragmentum exspectatum continet (recusatio
 * ob causam ALIAM probationem non implet) */
interior vacuum
_recusatum (
        CredoContextus* c,
                chorda  g,
    constans character* fragmentum)
{
    StatuaFructus f = statua_glb_legere(g, c->piscina);

    CREDO_FALSUM (f.successus);
    CREDO_VERUM (f.statua == NIHIL);
    CREDO_VERUM (chorda_continet(f.error,
        chorda_ex_literis(fragmentum, c->piscina)));
}

interior vacuum
_sectio_lapis (
    CredoContextus* c)
{
     StatuaFructus  f;
            Statua* s;
            chorda  g = filum_legere_totum(VIA_LAPIDIS, c->piscina);
               i32  n;
               i32  excisae    = ZEPHYRUM;
               i32  emissivae  = ZEPHYRUM;

    CREDO_NECESSE (g.mensura == 196476);
    f = statua_glb_legere(g, c->piscina);
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.error.mensura, ZEPHYRUM);
    s = f.statua;
    CREDO_AEQUALIS_I32 (s->nodorum, LIV);
    CREDO_AEQUALIS_I32 (s->retium, XXX);
    CREDO_AEQUALIS_I32 (s->partium, XL);
    CREDO_AEQUALIS_I32 (s->materiarum, IX);
    CREDO_AEQUALIS_I32 (s->texturarum, I);
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].latitudo, CCLVI);
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].altitudo, CCLVI);
    CREDO_AEQUALIS_I32 (s->cyclorum, II);
    CREDO_VERUM (chorda_aequalis_literis(s->cycli[ZEPHYRUM].titulus,
        "walk"));
    CREDO_F32_PROXIMUS (s->cycli[ZEPHYRUM].duratio, 1.4f, 0.01f);
    CREDO_AEQUALIS_I32 (s->cycli[ZEPHYRUM].canalium, LXX);
    CREDO_VERUM (chorda_aequalis_literis(s->cycli[I].titulus,
        "boulder_throw"));
    CREDO_F32_PROXIMUS (s->cycli[I].duratio, 1.5833f, 0.01f);
    per (n = ZEPHYRUM; n < s->materiarum; n++)
    {
        si (s->materiae[n].limen_alpha >= 0.0f)
        {
            CREDO_F32_PROXIMUS (s->materiae[n].limen_alpha, 0.5f,
                EPSILON);
            excisae++;
        }
        si (s->materiae[n].emissiva)
        {
            emissivae++;
        }
        /* PixMesh: ad marginem, textura una */
        CREDO_FALSUM (s->materiae[n].repetit_s);
        CREDO_AEQUALIS_S32 (s->materiae[n].textura, ZEPHYRUM);
    }
    CREDO_AEQUALIS_I32 (excisae, III);
    CREDO_AEQUALIS_I32 (emissivae, I);
    CREDO_AEQUALIS_I32 (s->verticum, 864);
    CREDO_AEQUALIS_I32 (s->indicum, 1002);
    CREDO_VERUM (s->radius > 0.0f);
    /* statua nihil in octetis tenet: octeti deleti, statua manet */
    memset(g.datum, ZEPHYRUM, g.mensura);
    CREDO_VERUM (chorda_aequalis_literis(s->cycli[ZEPHYRUM].titulus,
        "walk"));
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].latitudo, CCLVI);
}

interior vacuum
_sectio_fungus (
    CredoContextus* c)
{
     StatuaFructus  f;
            Statua* s;
            chorda  g = filum_legere_totum(VIA_FUNGI, c->piscina);
               i32  n;

    CREDO_NECESSE (g.mensura == 106864);
    f = statua_glb_legere(g, c->piscina);
    CREDO_NECESSE (f.successus);
    s = f.statua;
    CREDO_AEQUALIS_I32 (s->nodorum, XLII);
    CREDO_AEQUALIS_I32 (s->retium, XXIV);
    CREDO_AEQUALIS_I32 (s->partium, XXIX);
    CREDO_AEQUALIS_I32 (s->materiarum, XVI);
    CREDO_AEQUALIS_I32 (s->verticum, 1306);
    CREDO_AEQUALIS_I32 (s->indicum, 1974);
    CREDO_AEQUALIS_I32 (s->cyclorum, I);
    CREDO_VERUM (chorda_aequalis_literis(s->cycli[ZEPHYRUM].titulus,
        "walk_bouncy"));
    CREDO_F32_PROXIMUS (s->cycli[ZEPHYRUM].duratio, 1.0f, 0.01f);
    per (n = ZEPHYRUM; n < s->materiarum; n++)
    {
        CREDO_FALSUM (s->materiae[n].emissiva);
        CREDO_VERUM (s->materiae[n].limen_alpha < 0.0f);
    }
}

interior vacuum
_sectio_minima (
    CredoContextus* c)
{
     StatuaFructus  f = statua_glb_legere(_glb_sanum(c), c->piscina);
            Statua* s;

    CREDO_NECESSE (f.successus);
    s = f.statua;
    CREDO_AEQUALIS_I32 (s->nodorum, I);
    CREDO_AEQUALIS_I32 (s->retium, I);
    CREDO_AEQUALIS_I32 (s->partium, I);
    CREDO_AEQUALIS_I32 (s->verticum, III);
    CREDO_AEQUALIS_I32 (s->indicum, III);
    CREDO_AEQUALIS_S32 (s->partes[ZEPHYRUM].materia, -I);
    CREDO_AEQUALIS_S32 (s->nodi[ZEPHYRUM].rete, ZEPHYRUM);
    CREDO_F32_PROXIMUS (s->positiones[III], 1.0f, EPSILON);
    CREDO_F32_PROXIMUS (s->positiones[VII], 1.0f, EPSILON);
    /* normales absentes per faciem: triangulum in plano xy -> +z */
    CREDO_F32_PROXIMUS (s->normales[II], 1.0f, EPSILON);
    CREDO_F32_PROXIMUS (s->normales[VIII], 1.0f, EPSILON);
    CREDO_F32_PROXIMUS (s->nodi[ZEPHYRUM].scala.x, 1.0f, EPSILON);
    CREDO_F32_PROXIMUS (s->nodi[ZEPHYRUM].rotatio.w, 1.0f, EPSILON);
    /* sine indicibus: 0 1 2 */
    f = statua_glb_legere(_glb_ex_json(c,
        "{\"asset\":{\"version\":\"2.0\"},\"nodes\":[{\"mesh\":0}],"
        "\"meshes\":[{\"primitives\":[{\"attributes\":"
        "{\"POSITION\":0}}]}],\"buffers\":[{\"byteLength\":160}],"
        "\"bufferViews\":[{\"buffer\":0,\"byteLength\":36}],"
        "\"accessors\":[{\"bufferView\":0,\"componentType\":5126,"
        "\"count\":3,\"type\":\"VEC3\"}]}"), c->piscina);
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.statua->indicum, III);
    CREDO_AEQUALIS_I32 (f.statua->indices[II], II);
    /* sine scaenis: nodi omnes servantur cum retibus */
    CREDO_AEQUALIS_S32 (f.statua->nodi[ZEPHYRUM].rete, ZEPHYRUM);
}

interior vacuum
_sectio_numeri_integri (
    CredoContextus* c)
{
         StatuaFructus  f;
                Statua* s;
    StereometriaMatrix  m[I];
    StereometriaVector  v;

    f = statua_glb_legere(_glb(c, ANIMATIO(""),
        "{\"mesh\":0,\"scale\":[2,2,2],\"translation\":[1,0,0],"
        "\"rotation\":[0,0,0,1]}", "", "", "3", "", ""), c->piscina);
    CREDO_NECESSE (f.successus);
    s = f.statua;
    CREDO_F32_PROXIMUS (s->nodi[ZEPHYRUM].scala.x, 2.0f, EPSILON);
    CREDO_F32_PROXIMUS (s->nodi[ZEPHYRUM].scala.z, 2.0f, EPSILON);
    CREDO_F32_PROXIMUS (s->nodi[ZEPHYRUM].translatio.x, 1.0f, EPSILON);
    CREDO_F32_PROXIMUS (s->nodi[ZEPHYRUM].rotatio.w, 1.0f, EPSILON);
    /* cyclus: translatio (0,0,0) -> (2,0,0) per secundam */
    CREDO_AEQUALIS_I32 (s->cyclorum, I);
    CREDO_VERUM (chorda_aequalis_literis(s->cycli[ZEPHYRUM].titulus,
        "it"));
    CREDO_F32_PROXIMUS (s->cycli[ZEPHYRUM].duratio, 1.0f, EPSILON);
    statua_positura(s, ZEPHYRUM, 0.5f, m);
    v = stereometria_punctum_transformare(m[ZEPHYRUM],
        stereometria_vector(0.0f, 0.0f, 0.0f), NIHIL);
    CREDO_F32_PROXIMUS (v.x, 1.0f, EPSILON);
    /* STEP: clavis prior tenetur */
    f = statua_glb_legere(_glb(c,
        ANIMATIO(",\"interpolation\":\"STEP\""), "{\"mesh\":0}", "",
        "", "3", "", ""), c->piscina);
    CREDO_NECESSE (f.successus);
    CREDO_VERUM (f.statua->cycli[ZEPHYRUM].canales[ZEPHYRUM].gradatim);
    /* nodus cum 'matrix' (columnis primum): scala III, translatio
     * (4,5,6) */
    f = statua_glb_legere(_glb(c, "",
        "{\"mesh\":0,\"matrix\":[3,0,0,0,0,3,0,0,0,0,3,0,4,5,6,1]}",
        "", "", "3", "", ""), c->piscina);
    CREDO_NECESSE (f.successus);
    CREDO_F32_PROXIMUS (f.statua->nodi[ZEPHYRUM].scala.y, 3.0f,
        EPSILON);
    CREDO_F32_PROXIMUS (f.statua->nodi[ZEPHYRUM].translatio.z, 6.0f,
        EPSILON);
}

#define MATERIA_APPROXIMATA \
    ",\"materials\":[{\"alphaMode\":\"BLEND\",\"doubleSided\":true," \
    "\"emissiveFactor\":[0,1,0],\"pbrMetallicRoughness\":" \
    "{\"baseColorFactor\":[0.5,1,1,1],\"baseColorTexture\":" \
    "{\"index\":0}}}],\"textures\":[{\"sampler\":0,\"source\":0}]," \
    "\"samplers\":[{\"wrapS\":10497,\"wrapT\":33648}]," \
    "\"images\":[{\"bufferView\":5,\"mimeType\":\"image/png\"}]"

interior vacuum
_sectio_approximata (
    CredoContextus* c)
{
     StatuaFructus  f;
            Statua* s;
     StatuaMateria* m;

    f = statua_glb_legere(_glb(c, MATERIA_APPROXIMATA, "{\"mesh\":0}",
        ",\"TEXCOORD_0\":4", ",\"material\":0", "3", "", ""),
        c->piscina);
    CREDO_NECESSE (f.successus);
    s = f.statua;
    CREDO_AEQUALIS_I32 (s->materiarum, I);
    CREDO_AEQUALIS_S32 (s->partes[ZEPHYRUM].materia, ZEPHYRUM);
    m = &s->materiae[ZEPHYRUM];
    CREDO_F32_PROXIMUS (m->limen_alpha, 0.5f, EPSILON);
    CREDO_F32_PROXIMUS (m->color[ZEPHYRUM], 0.5f, EPSILON);
    CREDO_F32_PROXIMUS (m->color[III], 1.0f, EPSILON);
    CREDO_VERUM (m->emissiva);
    CREDO_VERUM (m->bilateralis);
    CREDO_VERUM (m->repetit_s);
    CREDO_FALSUM (m->speculat_s);
    CREDO_VERUM (m->repetit_t);
    CREDO_VERUM (m->speculat_t);
    CREDO_AEQUALIS_S32 (m->textura, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (s->texturarum, I);
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].latitudo, II);
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].altitudo, I);
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].pixela[ZEPHYRUM], 255);
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].pixela[VII], ZEPHYRUM);
    /* coordinatae u16 normalizatae: (0,0) (1,0) (0,1) */
    CREDO_F32_PROXIMUS (s->coordinatae[ZEPHYRUM], 0.0f, EPSILON);
    CREDO_F32_PROXIMUS (s->coordinatae[II], 1.0f, EPSILON);
    CREDO_F32_PROXIMUS (s->coordinatae[V], 1.0f, EPSILON);
    /* textura sine 'sampler': glTF repetitionem ordinariam dicit */
    f = statua_glb_legere(_glb(c,
        ",\"materials\":[{\"pbrMetallicRoughness\":"
        "{\"baseColorTexture\":{\"index\":0}}}],"
        "\"textures\":[{\"source\":0}],"
        "\"images\":[{\"bufferView\":5}]", "{\"mesh\":0}", "",
        ",\"material\":0", "3", "", ""), c->piscina);
    CREDO_NECESSE (f.successus);
    CREDO_VERUM (f.statua->materiae[ZEPHYRUM].repetit_s);
    CREDO_VERUM (f.statua->materiae[ZEPHYRUM].limen_alpha < 0.0f);
    CREDO_FALSUM (f.statua->materiae[ZEPHYRUM].emissiva);
    /* extensio optionalis ignota, camera in nodo: neglecta */
    f = statua_glb_legere(_glb(c,
        ",\"extensionsUsed\":[\"KHR_ignota\"],\"cameras\":[{}]",
        "{\"mesh\":0,\"camera\":0}", "", "", "3", "", ""),
        c->piscina);
    CREDO_VERUM (f.successus);
}

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    chorda g;

    /* magica mala */
    g                  = _glb_sanum(c);
    g.datum[ZEPHYRUM]  = 'x';
    _recusatum(c, g,
        "not a GLB");
    /* versio I */
    g = _glb_sanum(c);
    _u32_ponere(g.datum + IV, I);
    _recusatum(c, g,
        "version 2");
    /* longitudo JSON ultra finem */
    g = _glb_sanum(c);
    _u32_ponere(g.datum + XII, g.mensura);
    _recusatum(c, g,
        "description part");
    /* longitudo tota ultra octetos */
    g = _glb_sanum(c);
    _u32_ponere(g.datum + VIII, g.mensura + I);
    _recusatum(c, g,
        "cut short");
    /* JSON corruptum */
    g            = _glb_sanum(c);
    g.datum[XX]  = '[';
    _recusatum(c, g,
        "not valid");
    /* vacua et brevissima */
    g.mensura = ZEPHYRUM;
    _recusatum(c, g,
        "not a GLB");
    g          = _glb_sanum(c);
    g.mensura  = XI;
    _recusatum(c, g,
        "not a GLB");
    /* pellis */
    _recusatum(c, _glb(c, ",\"skins\":[{\"joints\":[0]}]",
        "{\"mesh\":0}", "", "", "3", "", ""),
        "skinning");
    /* scopi morphici */
    _recusatum(c, _glb(c, "", "{\"mesh\":0}", "",
        ",\"targets\":[{\"POSITION\":0}]", "3", "", ""),
        "morph targets");
    /* extensio requisita */
    _recusatum(c, _glb(c, ",\"extensionsRequired\":[\"X\"]",
        "{\"mesh\":0}", "", "", "3", "", ""),
        "extension");
    /* imago externa */
    _recusatum(c, _glb(c, ",\"images\":[{\"uri\":\"a.png\"}]",
        "{\"mesh\":0}", "", "", "3", "", ""),
        "separate file");
    /* plagula externa */
    _recusatum(c, _glb(c, "", "{\"mesh\":0}", "", "", "3", "",
        ",\"uri\":\"a.bin\""),
        "separate files");
    /* lineae, non triangula */
    _recusatum(c, _glb(c, "", "{\"mesh\":0}", "", ",\"mode\":1", "3",
        "", ""),
        "plain triangles");
    /* CUBICSPLINE */
    _recusatum(c, _glb(c,
        ANIMATIO(",\"interpolation\":\"CUBICSPLINE\""), "{\"mesh\":0}",
        "", "", "3", "", ""),
        "CUBICSPLINE");
    /* canalis in pondera (scopi morphici) */
    _recusatum(c, _glb(c,
        ",\"animations\":[{\"samplers\":[{\"input\":2,\"output\":2}],"
        "\"channels\":[{\"sampler\":0,\"target\":{\"node\":0,"
        "\"path\":\"weights\"}}]}]", "{\"mesh\":0}", "", "", "3", "",
        ""),
        "morph targets");
    /* accessor sparsus */
    _recusatum(c, _glb(c, "", "{\"mesh\":0}", "", "", "3",
        ",\"sparse\":{\"count\":1}", ""),
        "sparse");
    /* numerus indicum ultra prospectum */
    _recusatum(c, _glb(c, "", "{\"mesh\":0}", "", "", "9", "", ""),
        "past the end");
    /* index verticis extra fines: coordinatae u16 ut indices
     * (65535) */
    _recusatum(c, _glb_ex_json(c,
        "{\"asset\":{\"version\":\"2.0\"},\"nodes\":[{\"mesh\":0}],"
        "\"meshes\":[{\"primitives\":[{\"attributes\":"
        "{\"POSITION\":0},\"indices\":1}]}],"
        "\"buffers\":[{\"byteLength\":160}],\"bufferViews\":["
        "{\"buffer\":0,\"byteLength\":36},"
        "{\"buffer\":0,\"byteOffset\":76,\"byteLength\":12}],"
        "\"accessors\":[{\"bufferView\":0,\"componentType\":5126,"
        "\"count\":3,\"type\":\"VEC3\"},{\"bufferView\":1,"
        "\"componentType\":5123,\"count\":3,\"type\":\"SCALAR\"}]}"),
        "vertex that is not there");
    /* prospectus ultra BIN */
    _recusatum(c, _glb_ex_json(c,
        "{\"asset\":{\"version\":\"2.0\"},\"nodes\":[{\"mesh\":0}],"
        "\"meshes\":[{\"primitives\":[{\"attributes\":"
        "{\"POSITION\":0}}]}],\"buffers\":[{\"byteLength\":160}],"
        "\"bufferViews\":[{\"buffer\":0,\"byteOffset\":150,"
        "\"byteLength\":36}],\"accessors\":[{\"bufferView\":0,"
        "\"componentType\":5126,\"count\":3,\"type\":\"VEC3\"}]}"),
        "past the end");
    /* circulus parentum */
    _recusatum(c, _glb(c, "",
        "{\"children\":[1]},{\"mesh\":0,\"children\":[0]}", "", "", "3",
        "", ""),
        "loops back");
    /* nodus duorum parentum */
    _recusatum(c, _glb(c, "",
        "{\"children\":[2]},{\"children\":[2]},{\"mesh\":0}", "", "",
        "3", "", ""),
        "two parents");
    /* liber extra fines */
    _recusatum(c, _glb(c, "", "{\"mesh\":0,\"children\":[7]}", "", "",
        "3", "", ""),
        "child that");
    /* rete extra fines */
    _recusatum(c, _glb(c, "", "{\"mesh\":4}", "", "", "3", "", ""),
        "mesh that");
    /* imago non decodificabilis (prospectus positionum ut imago) */
    _recusatum(c, _glb(c,
        ",\"materials\":[{\"pbrMetallicRoughness\":"
        "{\"baseColorTexture\":{\"index\":0}}}],"
        "\"textures\":[{\"source\":0}],"
        "\"images\":[{\"bufferView\":0}]", "{\"mesh\":0}", "",
        ",\"material\":0", "3", "", ""),
        "could not be read");
    /* nulla retia: nihil ostendendum */
    _recusatum(c, _glb_ex_json(c,
        "{\"asset\":{\"version\":\"2.0\"},\"nodes\":[{}]}"),
        "nothing to show");
}

/* omnis longitudo truncata: cruda (longitudo capitis mendax) et
 * sanata (longitudines capitis et partium ad truncatum aptatae, ut
 * lector ultra caput procedat) */
interior vacuum
_truncata_omnia (
    chorda g)
{
    Piscina* p;
     chorda  t;
        i32  longitudo;
        i32  json_mensura;
        i32  gradus;

    json_mensura = (i32)g.datum[XII] | ((i32)g.datum[XIII] << VIII)
        | ((i32)g.datum[XIV] << XVI) | ((i32)g.datum[XV] << XXIV);
    per (longitudo = ZEPHYRUM; longitudo < g.mensura;
         longitudo += gradus)
    {
        gradus     = longitudo < 0x1000 ? I : 997;
        p          = piscina_generare_dynamicum("truncata", 0x100000);
        t.mensura  = longitudo;
        t.datum    = (i8*)piscina_allocare(p, longitudo + I);
        memcpy(t.datum, g.datum, longitudo);
        (vacuum)statua_glb_legere(t, p);
        si (longitudo >= XX)
        {
            _u32_ponere(t.datum + VIII, longitudo);
            si (longitudo < XX + json_mensura)
            {
                _u32_ponere(t.datum + XII, longitudo - XX);
            }
            alioquin si (longitudo >= XX + json_mensura + VIII)
            {
                _u32_ponere(t.datum + XX + json_mensura,
                    longitudo - (XX + json_mensura + VIII));
            }
            (vacuum)statua_glb_legere(t, p);
        }
        piscina_destruere(p);
    }
}

/* octetus unus in BIN mutatus, CCC locis (generator linearis fixus):
 * indices, numeri fluitantes et imago corrupta */
interior vacuum
_corrupta_omnia (
    chorda g)
{
    Piscina* p;
     chorda  t;
        i32  json_mensura;
        i32  initium;
        i32  semen = 12345;
        i32  n;
        i32  locus;

    json_mensura = (i32)g.datum[XII] | ((i32)g.datum[XIII] << VIII)
        | ((i32)g.datum[XIV] << XVI) | ((i32)g.datum[XV] << XXIV);
    initium = XX + json_mensura + VIII;
    per (n = ZEPHYRUM; n < CCC; n++)
    {
        semen      = semen * 1103515245 + 12345;
        locus      = initium + (semen >> VIII) % (g.mensura - initium);
        p          = piscina_generare_dynamicum("corrupta", 0x100000);
        t.mensura  = g.mensura;
        t.datum    = (i8*)piscina_allocare(p, g.mensura);
        memcpy(t.datum, g.datum, g.mensura);
        t.datum[locus] = (i8)(t.datum[locus] ^ (i8)(I << (n % VIII)));
        si (n % III == ZEPHYRUM)
        {
            t.datum[locus] = 0xFF;
        }
        (vacuum)statua_glb_legere(t, p);
        piscina_destruere(p);
    }
}

interior vacuum
_sectio_truncata (
    CredoContextus* c)
{
    chorda lapis   = filum_legere_totum(VIA_LAPIDIS, c->piscina);
    chorda fungus  = filum_legere_totum(VIA_FUNGI, c->piscina);

    CREDO_NECESSE (lapis.mensura > XX && fungus.mensura > XX);
    CREDO_NON_PENDET (_truncata_omnia(lapis), 60000);
    CREDO_NON_PENDET (_truncata_omnia(fungus), 60000);
    CREDO_NON_PENDET (_truncata_omnia(_glb_sanum(c)), 60000);
}

interior vacuum
_sectio_corrupta (
    CredoContextus* c)
{
    chorda lapis   = filum_legere_totum(VIA_LAPIDIS, c->piscina);
    chorda fungus  = filum_legere_totum(VIA_FUNGI, c->piscina);

    CREDO_NECESSE (lapis.mensura > XX && fungus.mensura > XX);
    CREDO_NON_PENDET (_corrupta_omnia(lapis), 60000);
    CREDO_NON_PENDET (_corrupta_omnia(fungus), 60000);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "lapis", _sectio_lapis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fungus", _sectio_fungus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "minima", _sectio_minima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "numeri_integri", _sectio_numeri_integri, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "approximata", _sectio_approximata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "truncata", _sectio_truncata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "corrupta", _sectio_corrupta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("statua_glb", SECTIONES);
}
