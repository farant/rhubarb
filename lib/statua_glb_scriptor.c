/*
 * statua_glb_scriptor.c - statua in octetos GLB
 * (statua_glb_scriptor.h)
 *
 * Pars BIN primum crescit (vertices et indices cuiusque tractus,
 * imagines PNG, claves cyclorum), quaeque tabula in sectione sua
 * (bufferView) ad IV octetos aequata; JSON per json.h aedificatur
 * et accessores sectionesque nominat. Deinde continens: caput, pars
 * JSON spatiis aequata, pars BIN zeris aequata.
 */

#include "statua_glb_scriptor.h"
#include "json.h"
#include "imago_png.h"
#include <string.h>

#define GENUS_BREVE        5123
#define GENUS_INTEGRUM     5125
#define GENUS_FLUITANS     5126
#define SCOPUS_VERTICUM    34962
#define SCOPUS_INDICUM     34963
#define INVOLUTIO_REPETITA 10497
#define INVOLUTIO_MARGO    33071
#define INVOLUTIO_SPECULUM 33648
#define FILTRUM_PROXIMUM   9728
#define LIMES_BREVIS       65536
#define MODULI_MAXIMI      IX

/* vertices tractus unius in tabulis statuae */
nomen structura {
    i32 primus;
    i32 numerus;
} Tractus;

nomen structura {
            Piscina* piscina;
    constans Statua* s;
                 i8* octeti;
                i32  mensura;
                i32  capacitas;
          JsonValor* accessores;
          JsonValor* sectiones;
                i32  accessorum;
                i32  sectionum;
                b32  fractum;
} Scriptor;


/* ==================================================
 * Octeti
 * ================================================== */

interior b32
_finitus (
    f32 x)
{
    redde x == x && x <= 3.0e38f && x >= -3.0e38f;
}

interior b32
_finiti (
    constans f32* x,
             i32  numerus)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (!_finitus(x[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_octetus (
    Scriptor* sc,
          i8  x)
{
    si (sc->mensura >= sc->capacitas)
    {
        sc->fractum = VERUM;
        redde;
    }
    sc->octeti[sc->mensura++] = x;
}

interior vacuum
_integrum (
    Scriptor* sc,
         i32  x)
{
    _octetus(sc, (i8)(x & 0xFF));
    _octetus(sc, (i8)((x >> VIII) & 0xFF));
    _octetus(sc, (i8)((x >> XVI) & 0xFF));
    _octetus(sc, (i8)((x >> XXIV) & 0xFF));
}

interior vacuum
_fluitantem (
    Scriptor* sc,
         f32  x)
{
    i32 forma;

    memcpy(&forma, &x, magnitudo(forma));
    _integrum(sc, forma);
}

interior vacuum
_aequare (
    Scriptor* sc)
{
    dum (sc->mensura % IV && !sc->fractum)
    {
        _octetus(sc, ZEPHYRUM);
    }
}

interior JsonValor*
_numerus (
    Scriptor* sc,
         i32  x)
{
    redde json_integer_creare(sc->piscina, (s64)x);
}

interior JsonValor*
_realis (
    Scriptor* sc,
         f32  x)
{
    redde json_fluitans_creare(sc->piscina, (f64)x);
}

interior JsonValor*
_reales (
        Scriptor* sc,
    constans f32* x,
             i32  numerus)
{
    JsonValor* t = json_tabulatum_creare(sc->piscina);
          i32  k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        json_tabulatum_addere(t, _realis(sc, x[k]));
    }
    redde t;
}

/* sectio (bufferView) ab 'initium' ad finem currentem; index eius */
interior i32
_sectio (
    Scriptor* sc,
         i32  initium,
         i32  scopus)
{
    JsonValor* j = json_objectum_creare(sc->piscina);

    json_objectum_ponere(j, "buffer", _numerus(sc, ZEPHYRUM));
    json_objectum_ponere(j, "byteOffset", _numerus(sc, initium));
    json_objectum_ponere(j, "byteLength",
        _numerus(sc, sc->mensura - initium));
    si (scopus)
    {
        json_objectum_ponere(j, "target", _numerus(sc, scopus));
    }
    json_tabulatum_addere(sc->sectiones, j);
    redde sc->sectionum++;
}

/* accessor super sectionem totam; fines (minimum, maximum per
 * partem) si dati; index eius */
interior i32
_accessor (
              Scriptor* sc,
                   i32  sectio,
                   i32  genus,
                   i32  numerus,
    constans character* forma,
          constans f32* minima,
          constans f32* maxima,
                   i32  partes)
{
    JsonValor* j = json_objectum_creare(sc->piscina);

    json_objectum_ponere(j, "bufferView", _numerus(sc, sectio));
    json_objectum_ponere(j, "componentType", _numerus(sc, genus));
    json_objectum_ponere(j, "count", _numerus(sc, numerus));
    json_objectum_ponere(j, "type",
        json_chorda_creare_literis(sc->piscina, forma));
    si (minima && maxima)
    {
        json_objectum_ponere(j, "min", _reales(sc, minima, partes));
        json_objectum_ponere(j, "max", _reales(sc, maxima, partes));
    }
    json_tabulatum_addere(sc->accessores, j);
    redde sc->accessorum++;
}

/* tabula fluitantium (latus per elementum) in sectionem et
 * accessorem; cum_finibus: minimum et maximum per partem */
interior i32
_tabula_fluitans (
              Scriptor* sc,
          constans f32* x,
                   i32  numerus,
                   i32  latus,
    constans character* forma,
                   i32  scopus,
                   b32  cum_finibus)
{
    f32 minima[IV];
    f32 maxima[IV];
    i32 initium;
    i32 n;
    i32 k;

    _aequare(sc);
    initium = sc->mensura;
    per (k = ZEPHYRUM; k < latus; k++)
    {
        minima[k] = x[k];
        maxima[k] = x[k];
    }
    per (n = ZEPHYRUM; n < numerus; n++)
    {
        per (k = ZEPHYRUM; k < latus; k++)
        {
            f32 valor = x[n * latus + k];

            si (valor < minima[k])
            {
                minima[k] = valor;
            }
            si (valor > maxima[k])
            {
                maxima[k] = valor;
            }
            _fluitantem(sc, valor);
        }
    }
    redde _accessor(sc, _sectio(sc, initium, scopus), GENUS_FLUITANS,
        numerus, forma, cum_finibus ? minima : NIHIL,
        cum_finibus ? maxima : NIHIL, latus);
}


/* ==================================================
 * Retia
 * ================================================== */

/* vertices cuiusque tractus. Tractus ordinati (quisque supra
 * priorem incipit, ut lector et plastica dant): tractus a fine
 * prioris ad indicem suum maximum currit et ultimus ad finem
 * tabulae - tabula verticum tota et indices absoluti intacti
 * redeunt. Aliter quisque a minimo ad maximum suum (vertices
 * communes duplicantur). FALSUM si tractus sine indicibus */
interior b32
_tractus_metiri (
    constans Statua* s,
            Tractus* tractus)
{
    b32 ordinati  = VERUM;
    i32 finis     = ZEPHYRUM;
    i32 j;
    i32 k;

    per (j = ZEPHYRUM; j < s->partium; j++)
    {
        constans StatuaPars* pars = &s->partes[j];
                        i32  minimus;
                        i32  maximus;

        si (pars->indicum == ZEPHYRUM)
        {
            redde FALSUM;
        }
        minimus = s->indices[pars->index_primus];
        maximus = minimus;
        per (k = I; k < pars->indicum; k++)
        {
            i32 index = s->indices[pars->index_primus + k];

            si (index < minimus)
            {
                minimus = index;
            }
            si (index > maximus)
            {
                maximus = index;
            }
        }
        si (minimus < finis)
        {
            ordinati = FALSUM;
        }
        finis               = maximus + I;
        tractus[j].primus   = minimus;
        tractus[j].numerus  = maximus - minimus + I;
    }
    per (j = ZEPHYRUM; ordinati && j < s->partium; j++)
    {
        i32 primus     = j ? tractus[j - I].primus
            + tractus[j - I].numerus : ZEPHYRUM;
        i32 ultimus    = j + I < s->partium
            ? tractus[j].primus + tractus[j].numerus : s->verticum;

        tractus[j].primus   = primus;
        tractus[j].numerus  = ultimus - primus;
    }
    redde VERUM;
}

interior JsonValor*
_primitivum (
            Scriptor* sc,
    constans Tractus* tractus,
 constans StatuaPars* pars)
{
    constans Statua* s          = sc->s;
          JsonValor* j          = json_objectum_creare(sc->piscina);
          JsonValor* attributa  = json_objectum_creare(sc->piscina);
                b32  breves     = tractus->numerus < LIMES_BREVIS;
                i32  initium;
                i32  k;

    json_objectum_ponere(attributa, "POSITION", _numerus(sc,
        _tabula_fluitans(sc, s->positiones + tractus->primus * III,
            tractus->numerus, III, "VEC3", SCOPUS_VERTICUM, VERUM)));
    json_objectum_ponere(attributa, "NORMAL", _numerus(sc,
        _tabula_fluitans(sc, s->normales + tractus->primus * III,
            tractus->numerus, III, "VEC3", SCOPUS_VERTICUM, FALSUM)));
    json_objectum_ponere(attributa, "TEXCOORD_0", _numerus(sc,
        _tabula_fluitans(sc, s->coordinatae + tractus->primus * II,
            tractus->numerus, II, "VEC2", SCOPUS_VERTICUM, FALSUM)));
    json_objectum_ponere(j, "attributes", attributa);
    _aequare(sc);
    initium = sc->mensura;
    per (k = ZEPHYRUM; k < pars->indicum; k++)
    {
        i32 index = s->indices[pars->index_primus + k]
            - tractus->primus;

        si (breves)
        {
            _octetus(sc, (i8)(index & 0xFF));
            _octetus(sc, (i8)((index >> VIII) & 0xFF));
        }
        alioquin
        {
            _integrum(sc, index);
        }
    }
    json_objectum_ponere(j, "indices", _numerus(sc,
        _accessor(sc, _sectio(sc, initium, SCOPUS_INDICUM),
            breves ? GENUS_BREVE : GENUS_INTEGRUM, pars->indicum,
            "SCALAR", NIHIL, NIHIL, ZEPHYRUM)));
    si (pars->materia >= ZEPHYRUM)
    {
        json_objectum_ponere(j, "material",
            _numerus(sc, (i32)pars->materia));
    }
    redde j;
}

interior JsonValor*
_retia (
            Scriptor* sc,
    constans Tractus* tractus)
{
    constans Statua* s      = sc->s;
          JsonValor* retia  = json_tabulatum_creare(sc->piscina);
                i32  n;
                i32  k;

    per (n = ZEPHYRUM; n < s->retium; n++)
    {
        JsonValor* rete       = json_objectum_creare(sc->piscina);
        JsonValor* primitiva  = json_tabulatum_creare(sc->piscina);

        per (k = ZEPHYRUM; k < s->retia[n].partium; k++)
        {
            i32 pars = s->retia[n].pars_prima + k;

            json_tabulatum_addere(primitiva,
                _primitivum(sc, &tractus[pars], &s->partes[pars]));
        }
        json_objectum_ponere(rete, "primitives", primitiva);
        json_tabulatum_addere(retia, rete);
    }
    redde retia;
}


/* ==================================================
 * Materiae, texturae, imagines
 * ================================================== */

interior i32
_involutio (
    b32 repetit,
    b32 speculat)
{
    si (!repetit)
    {
        redde INVOLUTIO_MARGO;
    }
    redde speculat ? INVOLUTIO_SPECULUM : INVOLUTIO_REPETITA;
}

/* materiae cum texturis (par imaginis et moduli semel), imaginibus
 * (ordine usus primi, ut lector eas numerat) et modulis in radicem */
interior b32
_materiae (
     Scriptor* sc,
    JsonValor* radix)
{
    constans Statua* s         = sc->s;
          JsonValor* materiae  = json_tabulatum_creare(sc->piscina);
          JsonValor* texturae  = json_tabulatum_creare(sc->piscina);
          JsonValor* imagines  = json_tabulatum_creare(sc->piscina);
          JsonValor* moduli    = json_tabulatum_creare(sc->piscina);
                s32* imago_texturae;
                i32  modulus_s[MODULI_MAXIMI];
                i32  modulus_t[MODULI_MAXIMI];
                i32* par_imago;
                i32* par_modulus;
                i32  modulorum  = ZEPHYRUM;
                i32  imaginum   = ZEPHYRUM;
                i32  parium     = ZEPHYRUM;
                i32  n;
                i32  k;

    imago_texturae = (s32*)piscina_allocare(sc->piscina,
        (s->texturarum + I) * magnitudo(s32));
    par_imago      = (i32*)piscina_allocare(sc->piscina,
        (s->materiarum + I) * magnitudo(i32));
    par_modulus    = (i32*)piscina_allocare(sc->piscina,
        (s->materiarum + I) * magnitudo(i32));
    si (!imago_texturae || !par_imago || !par_modulus)
    {
        redde FALSUM;
    }
    per (n = ZEPHYRUM; n < s->texturarum; n++)
    {
        imago_texturae[n] = -I;
    }
    per (n = ZEPHYRUM; n < s->materiarum; n++)
    {
        constans StatuaMateria* m = &s->materiae[n];
                     JsonValor* j = json_objectum_creare(sc->piscina);
                     JsonValor* basis =
                         json_objectum_creare(sc->piscina);

        json_objectum_ponere(basis, "baseColorFactor",
            _reales(sc, m->color, IV));
        json_objectum_ponere(basis, "metallicFactor",
            _numerus(sc, ZEPHYRUM));
        json_objectum_ponere(basis, "roughnessFactor",
            _numerus(sc, I));
        si (m->textura >= ZEPHYRUM)
        {
            JsonValor* nexus = json_objectum_creare(sc->piscina);
                  i32  involutio_s =
                      _involutio(m->repetit_s, m->speculat_s);
                  i32 involutio_t =
                      _involutio(m->repetit_t, m->speculat_t);
                  i32 modulus;
                  i32 imago;

            si (imago_texturae[m->textura] < ZEPHYRUM)
            {
                PngFructus png = imago_png_codificare(
                    &s->texturae[m->textura], sc->piscina);
                 JsonValor* i  = json_objectum_creare(sc->piscina);
                       i32  initium;

                si (!png.successus)
                {
                    redde FALSUM;
                }
                _aequare(sc);
                initium = sc->mensura;
                per (k = ZEPHYRUM; k < png.mensura; k++)
                {
                    _octetus(sc, png.datum[k]);
                }
                json_objectum_ponere(i, "bufferView",
                    _numerus(sc, _sectio(sc, initium, ZEPHYRUM)));
                json_objectum_ponere(i, "mimeType",
                    json_chorda_creare_literis(sc->piscina,
                        "image/png"));
                json_tabulatum_addere(imagines, i);
                imago_texturae[m->textura] = (s32)imaginum++;
            }
            imago = (i32)imago_texturae[m->textura];
            per (modulus = ZEPHYRUM; modulus < modulorum; modulus++)
            {
                si (   modulus_s[modulus] == involutio_s
                    && modulus_t[modulus] == involutio_t)
                {
                    frange;
                }
            }
            si (modulus == modulorum)
            {
                JsonValor* i = json_objectum_creare(sc->piscina);

                json_objectum_ponere(i, "magFilter",
                    _numerus(sc, FILTRUM_PROXIMUM));
                json_objectum_ponere(i, "minFilter",
                    _numerus(sc, FILTRUM_PROXIMUM));
                json_objectum_ponere(i, "wrapS",
                    _numerus(sc, involutio_s));
                json_objectum_ponere(i, "wrapT",
                    _numerus(sc, involutio_t));
                json_tabulatum_addere(moduli, i);
                modulus_s[modulorum]    = involutio_s;
                modulus_t[modulorum++]  = involutio_t;
            }
            per (k = ZEPHYRUM; k < parium; k++)
            {
                si (par_imago[k] == imago && par_modulus[k] == modulus)
                {
                    frange;
                }
            }
            si (k == parium)
            {
                JsonValor* i = json_objectum_creare(sc->piscina);

                json_objectum_ponere(i, "sampler",
                    _numerus(sc, modulus));
                json_objectum_ponere(i, "source", _numerus(sc, imago));
                json_tabulatum_addere(texturae, i);
                par_imago[parium]      = imago;
                par_modulus[parium++]  = modulus;
            }
            json_objectum_ponere(nexus, "index", _numerus(sc, k));
            json_objectum_ponere(basis, "baseColorTexture", nexus);
        }
        json_objectum_ponere(j, "pbrMetallicRoughness", basis);
        si (m->emissiva)
        {
            hic_manens constans f32 ALBUS[III] = { 1.0f, 1.0f, 1.0f };

            json_objectum_ponere(j, "emissiveFactor",
                _reales(sc, ALBUS, III));
        }
        si (m->bilateralis)
        {
            json_objectum_ponere(j, "doubleSided",
                json_boolean_creare(sc->piscina, VERUM));
        }
        si (m->limen_alpha >= 0.0f)
        {
            json_objectum_ponere(j, "alphaMode",
                json_chorda_creare_literis(sc->piscina, "MASK"));
            json_objectum_ponere(j, "alphaCutoff",
                _realis(sc, m->limen_alpha));
        }
        json_tabulatum_addere(materiae, j);
    }
    si (s->materiarum)
    {
        json_objectum_ponere(radix, "materials", materiae);
    }
    si (parium)
    {
        json_objectum_ponere(radix, "textures", texturae);
        json_objectum_ponere(radix, "images", imagines);
        json_objectum_ponere(radix, "samplers", moduli);
    }
    redde VERUM;
}


/* ==================================================
 * Nodi et cycli
 * ================================================== */

interior vacuum
_nodi (
           Scriptor* sc,
          JsonValor* radix,
    constans chorda* tituli)
{
    hic_manens constans character* constans SIGNA[III] = {
        "walkable", "collidable", "water"
    };
    constans Statua* s        = sc->s;
          JsonValor* nodi     = json_tabulatum_creare(sc->piscina);
          JsonValor* scaenae  = json_tabulatum_creare(sc->piscina);
          JsonValor* scaena   = json_objectum_creare(sc->piscina);
          JsonValor* radices  = json_tabulatum_creare(sc->piscina);
                i32  n;
                i32  k;

    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        constans StatuaNodus* nodus = &s->nodi[n];
                   JsonValor* j = json_objectum_creare(sc->piscina);
                   JsonValor* liberi =
                       json_tabulatum_creare(sc->piscina);
                         f32 x[IV];
                         i32 liberorum = ZEPHYRUM;

        si (tituli && tituli[n].mensura)
        {
            json_objectum_ponere(j, "name",
                json_chorda_creare(sc->piscina, tituli[n]));
        }
        x[ZEPHYRUM]  = nodus->translatio.x;
        x[I]         = nodus->translatio.y;
        x[II]        = nodus->translatio.z;
        si (x[ZEPHYRUM] != 0.0f || x[I] != 0.0f || x[II] != 0.0f)
        {
            json_objectum_ponere(j, "translation",
                _reales(sc, x, III));
        }
        x[ZEPHYRUM]  = nodus->rotatio.x;
        x[I]         = nodus->rotatio.y;
        x[II]        = nodus->rotatio.z;
        x[III]       = nodus->rotatio.w;
        si (   x[ZEPHYRUM] != 0.0f || x[I] != 0.0f || x[II] != 0.0f
            || x[III]      != 1.0f)
        {
            json_objectum_ponere(j, "rotation", _reales(sc, x, IV));
        }
        x[ZEPHYRUM]  = nodus->scala.x;
        x[I]         = nodus->scala.y;
        x[II]        = nodus->scala.z;
        si (x[ZEPHYRUM] != 1.0f || x[I] != 1.0f || x[II] != 1.0f)
        {
            json_objectum_ponere(j, "scale", _reales(sc, x, III));
        }
        si (nodus->rete >= ZEPHYRUM)
        {
            json_objectum_ponere(j, "mesh",
                _numerus(sc, (i32)nodus->rete));
        }
        per (k = ZEPHYRUM; k < s->nodorum; k++)
        {
            si (s->nodi[k].parens == (s32)n)
            {
                json_tabulatum_addere(liberi, _numerus(sc, k));
                liberorum++;
            }
        }
        si (liberorum)
        {
            json_objectum_ponere(j, "children", liberi);
        }
        si (nodus->signa & (STATUA_SIGNUM_GRESSIBILE
            | STATUA_SIGNUM_SOLIDUM | STATUA_SIGNUM_AQUA))
        {
            JsonValor* extra = json_objectum_creare(sc->piscina);
            JsonValor* notae = json_tabulatum_creare(sc->piscina);

            per (k = ZEPHYRUM; k < III; k++)
            {
                si (nodus->signa & (I << k))
                {
                    json_tabulatum_addere(notae,
                        json_chorda_creare_literis(sc->piscina,
                            SIGNA[k]));
                }
            }
            json_objectum_ponere(extra, "tags", notae);
            json_objectum_ponere(j, "extras", extra);
        }
        json_tabulatum_addere(nodi, j);
        si (nodus->parens < ZEPHYRUM)
        {
            json_tabulatum_addere(radices, _numerus(sc, n));
        }
    }
    json_objectum_ponere(scaena, "nodes", radices);
    json_tabulatum_addere(scaenae, scaena);
    json_objectum_ponere(radix, "scene", _numerus(sc, ZEPHYRUM));
    json_objectum_ponere(radix, "scenes", scaenae);
    json_objectum_ponere(radix, "nodes", nodi);
}

interior vacuum
_cycli (
     Scriptor* sc,
    JsonValor* radix)
{
    hic_manens constans character* constans VIAE[III] = {
        "translation", "rotation", "scale"
    };
    constans Statua* s            = sc->s;
          JsonValor* animationes  = json_tabulatum_creare(sc->piscina);
                i32  n;
                i32  k;

    per (n = ZEPHYRUM; n < s->cyclorum; n++)
    {
        constans StatuaCyclus* cyclus = &s->cycli[n];
                    JsonValor* j = json_objectum_creare(sc->piscina);
                    JsonValor* canales =
                        json_tabulatum_creare(sc->piscina);
                    JsonValor* moduli =
                        json_tabulatum_creare(sc->piscina);
                          i32* tempora = (i32*)piscina_allocare(
                              sc->piscina, (cyclus->canalium + I)
                              * magnitudo(i32));
                          i32 prior;

        si (!tempora)
        {
            sc->fractum = VERUM;
            redde;
        }
        si (cyclus->titulus.mensura)
        {
            json_objectum_ponere(j, "name",
                json_chorda_creare(sc->piscina, cyclus->titulus));
        }
        per (k = ZEPHYRUM; k < cyclus->canalium; k++)
        {
            constans StatuaCanalis* canalis = &cyclus->canales[k];
                         JsonValor* modulus =
                             json_objectum_creare(sc->piscina);
                         JsonValor* nexus =
                             json_objectum_creare(sc->piscina);
                         JsonValor* scopus =
                             json_objectum_creare(sc->piscina);
                               b32 rotatio =
                                   canalis->proprietas
                                   == STATUA_ROTATIO;

            /* tempora eadem ac canalis prioris: accessor idem */
            per (prior = ZEPHYRUM; prior < k; prior++)
            {
                si (   cyclus->canales[prior].clavium
                           == canalis->clavium
                    && memcmp(cyclus->canales[prior].tempora,
                           canalis->tempora,
                           canalis->clavium * magnitudo(f32))
                           == ZEPHYRUM)
                {
                    frange;
                }
            }
            tempora[k] = prior < k ? tempora[prior]
                : _tabula_fluitans(sc, canalis->tempora,
                    canalis->clavium, I, "SCALAR", ZEPHYRUM, VERUM);
            json_objectum_ponere(modulus, "input",
                _numerus(sc, tempora[k]));
            json_objectum_ponere(modulus, "interpolation",
                json_chorda_creare_literis(sc->piscina,
                    canalis->gradatim ? "STEP" : "LINEAR"));
            json_objectum_ponere(modulus, "output", _numerus(sc,
                _tabula_fluitans(sc, canalis->valores,
                    canalis->clavium, rotatio ? IV : III,
                    rotatio ? "VEC4" : "VEC3", ZEPHYRUM, FALSUM)));
            json_tabulatum_addere(moduli, modulus);
            json_objectum_ponere(scopus, "node",
                _numerus(sc, (i32)canalis->nodus));
            json_objectum_ponere(scopus, "path",
                json_chorda_creare_literis(sc->piscina,
                    VIAE[canalis->proprietas]));
            json_objectum_ponere(nexus, "sampler", _numerus(sc, k));
            json_objectum_ponere(nexus, "target", scopus);
            json_tabulatum_addere(canales, nexus);
        }
        json_objectum_ponere(j, "channels", canales);
        json_objectum_ponere(j, "samplers", moduli);
        json_tabulatum_addere(animationes, j);
    }
    si (s->cyclorum)
    {
        json_objectum_ponere(radix, "animations", animationes);
    }
}


/* ==================================================
 * Publica
 * ================================================== */

/* statua scribi potest: numeri finiti, indices et nodi canalium
 * intra fines, tractus et retia non vacua */
interior b32
_probare (
    constans Statua* s)
{
    i32 n;
    i32 k;

    si (   !_finiti(s->positiones, s->verticum * III)
        || !_finiti(s->normales, s->verticum * III)
        || !_finiti(s->coordinatae, s->verticum * II))
    {
        redde FALSUM;
    }
    per (n = ZEPHYRUM; n < s->indicum; n++)
    {
        si (s->indices[n] >= s->verticum)
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->retium; n++)
    {
        si (s->retia[n].partium == ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        constans StatuaNodus* nodus = &s->nodi[n];

        si (   !_finitus(nodus->translatio.x)
            || !_finitus(nodus->translatio.y)
            || !_finitus(nodus->translatio.z)
            || !_finitus(nodus->rotatio.x)
            || !_finitus(nodus->rotatio.y)
            || !_finitus(nodus->rotatio.z)
            || !_finitus(nodus->rotatio.w)
            || !_finitus(nodus->scala.x) || !_finitus(nodus->scala.y)
            || !_finitus(nodus->scala.z))
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->materiarum; n++)
    {
        si (   !_finiti(s->materiae[n].color, IV)
            || !_finitus(s->materiae[n].limen_alpha))
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->cyclorum; n++)
    {
        per (k = ZEPHYRUM; k < s->cycli[n].canalium; k++)
        {
            constans StatuaCanalis* canalis = &s->cycli[n].canales[k];

            si (   canalis->clavium    == ZEPHYRUM
                || canalis->nodus < ZEPHYRUM
                || (i32)canalis->nodus >= s->nodorum
                || (i32)canalis->proprietas > (i32)STATUA_SCALA
                || !_finiti(canalis->tempora, canalis->clavium)
                || !_finiti(canalis->valores, canalis->clavium
                       * (canalis->proprietas == STATUA_ROTATIO
                           ? IV : III)))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* capacitas partis BIN: tabulae omnes, imagines (PNG numquam
 * multo maior quam pixela cruda), aequationes */
interior memoriae_index
_capacitas (
     constans Statua* s,
    constans Tractus* tractus)
{
    memoriae_index summa = (memoriae_index)(IV * M);
               i32 n;
               i32 k;

    per (n = ZEPHYRUM; n < s->partium; n++)
    {
        summa += (memoriae_index)tractus[n].numerus * XXXII
            + (memoriae_index)s->partes[n].indicum * IV + XVI;
    }
    per (n = ZEPHYRUM; n < s->texturarum; n++)
    {
        memoriae_index pixela =
            (memoriae_index)s->texturae[n].latitudo
            * s->texturae[n].altitudo;

        summa += pixela * V + pixela / IV + (memoriae_index)(IV * M);
    }
    per (n = ZEPHYRUM; n < s->cyclorum; n++)
    {
        per (k = ZEPHYRUM; k < s->cycli[n].canalium; k++)
        {
            summa += (memoriae_index)s->cycli[n].canales[k].clavium
                * XX + VIII;
        }
    }
    redde summa;
}

chorda
statua_glb_scribere (
    constans Statua* statua,
    constans chorda* tituli,
            Piscina* piscina)
{
     Scriptor  sc;
      Tractus* tractus;
    JsonValor* radix;
    JsonValor* res;
    JsonValor* plagula;
    JsonValor* plagulae;
       chorda  vacua;
       chorda  json;
       chorda  exitus;
          i32  json_mensura;
          i32  bin_mensura;
          i32  k;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   !statua || !piscina || statua->nodorum == ZEPHYRUM
        || !_probare(statua))
    {
        redde vacua;
    }
    memset(&sc, ZEPHYRUM, magnitudo(sc));
    sc.piscina  = piscina;
    sc.s        = statua;
    tractus     = (Tractus*)piscina_allocare(piscina,
        (statua->partium + I) * magnitudo(Tractus));
    si (!tractus || !_tractus_metiri(statua, tractus))
    {
        redde vacua;
    }
    sc.capacitas   = (i32)_capacitas(statua, tractus);
    sc.octeti      = (i8*)piscina_allocare(piscina, sc.capacitas);
    sc.accessores  = json_tabulatum_creare(piscina);
    sc.sectiones   = json_tabulatum_creare(piscina);
    radix          = json_objectum_creare(piscina);
    res            = json_objectum_creare(piscina);
    si (!sc.octeti || !sc.accessores || !sc.sectiones || !radix || !res)
    {
        redde vacua;
    }
    json_objectum_ponere(res, "generator",
        json_chorda_creare_literis(piscina, "rhubarb statua"));
    json_objectum_ponere(res, "version",
        json_chorda_creare_literis(piscina, "2.0"));
    json_objectum_ponere(radix, "asset", res);
    _nodi(&sc, radix, tituli);
    si (statua->retium)
    {
        json_objectum_ponere(radix, "meshes", _retia(&sc, tractus));
    }
    si (!_materiae(&sc, radix))
    {
        redde vacua;
    }
    _cycli(&sc, radix);
    _aequare(&sc);
    si (sc.fractum)
    {
        redde vacua;
    }
    si (sc.accessorum)
    {
        json_objectum_ponere(radix, "accessors", sc.accessores);
        json_objectum_ponere(radix, "bufferViews", sc.sectiones);
    }
    si (sc.mensura)
    {
        plagula   = json_objectum_creare(piscina);
        plagulae  = json_tabulatum_creare(piscina);
        json_objectum_ponere(plagula, "byteLength",
            _numerus(&sc, sc.mensura));
        json_tabulatum_addere(plagulae, plagula);
        json_objectum_ponere(radix, "buffers", plagulae);
    }
    json = json_scribere(radix, piscina);
    si (!json.datum || json.mensura == ZEPHYRUM)
    {
        redde vacua;
    }
    json_mensura  = (json.mensura + III) / IV * IV;
    bin_mensura   = sc.mensura;
    exitus.mensura  = XII + VIII + json_mensura
        + (bin_mensura ? VIII + bin_mensura : ZEPHYRUM);
    exitus.datum    = (i8*)piscina_allocare(piscina, exitus.mensura);
    si (!exitus.datum)
    {
        redde vacua;
    }
    /* continens: octeti exitus per scriptorem alterum */
    {
        Scriptor finis;

        memset(&finis, ZEPHYRUM, magnitudo(finis));
        finis.octeti     = exitus.datum;
        finis.capacitas  = exitus.mensura;
        _integrum(&finis, 0x46546C67);          /* "glTF" */
        _integrum(&finis, II);
        _integrum(&finis, exitus.mensura);
        _integrum(&finis, json_mensura);
        _integrum(&finis, 0x4E4F534A);          /* "JSON" */
        per (k = ZEPHYRUM; k < json_mensura; k++)
        {
            _octetus(&finis, k < json.mensura ? json.datum[k] : ' ');
        }
        si (bin_mensura)
        {
            _integrum(&finis, bin_mensura);
            _integrum(&finis, 0x004E4942);      /* "BIN\0" */
            per (k = ZEPHYRUM; k < bin_mensura; k++)
            {
                _octetus(&finis, sc.octeti[k]);
            }
        }
        si (finis.fractum || finis.mensura != exitus.mensura)
        {
            redde vacua;
        }
    }
    redde exitus;
}
