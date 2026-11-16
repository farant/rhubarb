/* probatio_statua_glb_scriptor.c - scriptor GLB (statua_glb_scriptor.h)
 *
 * Iter rotundum: lege, scribe, lege iterum - lectio altera primae
 * par campo pro campo (res plastica cum textura et signis, fungus
 * cum cyclis, locus planum). Determinatus. Nomina nodorum, signa,
 * involutio, excisio, emissio, bilateralis in JSON adsunt.
 * Recusationes.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "imago_typus.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_glb.h"
#include "statua_glb_scriptor.h"
#include "plastica.h"
#include "credo.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define VIA_FUNGI   "probationes/fixa/statuae/fungus.glb"
#define VIA_LAPIDIS "probationes/fixa/statuae/lapis.glb"
#define VIA_PLANI   "probationes/fixa/statuae/planum.glb"

hic_manens constans character* RES =
    "<res titulus=\"well\">"
    "<textura titulus=\"fenestra\" latitudo=\"2\""
    " colores=\"#102030 #f0c040\">ab ba</textura>"
    "<grex titulus=\"murus\" locus=\"1 0 2\" gyrus=\"30\">"
    "<capsa titulus=\"saxum\" magnitudo=\"2 1 2\" textura=\"saxum\""
    " color=\"#8f8676\" signum=\"solidum\"/>"
    "<cylindrus titulus=\"aqua\" radius=\"0.5\" altitudo=\"0.3\""
    " color=\"#2d5f8a\" signum=\"gressibile\" locus=\"0 1 0\"/>"
    "</grex>"
    "<speculum axis=\"x\">"
    "<capsa titulus=\"palus\" magnitudo=\"0.2 2 0.2\""
    " locus=\"1 0 0\" rotatio=\"10 20 30\" textura=\"fenestra\"/>"
    "</speculum>"
    "<capsa titulus=\"obex\" magnitudo=\"1 1 1\""
    " invisibilis=\"verum\" signum=\"solidum\"/>"
    "</res>";

interior Statua*
_lege_glb (
    CredoContextus* c,
            chorda  octeti)
{
    StatuaFructus f = statua_glb_legere(octeti, c->piscina);

    si (!f.successus)
    {
        printf("lectio recusata: %.*s\n", (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    redde f.statua;
}

interior b32
_f32_pares (
    constans f32* a,
    constans f32* b,
             i32  numerus)
{
    redde numerus == ZEPHYRUM
        || memcmp(a, b, numerus * magnitudo(f32)) == ZEPHYRUM;
}

/* statuae campo pro campo pares; differentiam primam imprimit */
interior b32
_differt (
    constans character* quid)
{
    printf("  differt: %s\n", quid);
    redde FALSUM;
}

interior b32
_statuae_pares (
    constans Statua* a,
    constans Statua* b)
{
    i32 n;
    i32 k;

    si (   a->nodorum    != b->nodorum || a->retium != b->retium
        || a->partium    != b->partium || a->verticum != b->verticum
        || a->indicum    != b->indicum || a->materiarum != b->materiarum
        || a->texturarum != b->texturarum
        || a->cyclorum   != b->cyclorum)
    {
        printf("  numeri: nodi %u/%u retia %u/%u partes %u/%u"
            " vertices %u/%u indices %u/%u materiae %u/%u"
            " texturae %u/%u cycli %u/%u\n",
            (insignatus)a->nodorum, (insignatus)b->nodorum,
            (insignatus)a->retium, (insignatus)b->retium,
            (insignatus)a->partium, (insignatus)b->partium,
            (insignatus)a->verticum, (insignatus)b->verticum,
            (insignatus)a->indicum, (insignatus)b->indicum,
            (insignatus)a->materiarum, (insignatus)b->materiarum,
            (insignatus)a->texturarum, (insignatus)b->texturarum,
            (insignatus)a->cyclorum, (insignatus)b->cyclorum);
        redde FALSUM;
    }
    per (n = ZEPHYRUM; n < a->nodorum; n++)
    {
        constans StatuaNodus* p = &a->nodi[n];
        constans StatuaNodus* q = &b->nodi[n];

        si (   p->parens != q->parens || p->rete != q->rete
            || p->signa  != q->signa)
        {
            redde _differt("nodus (parens, rete, signa)");
        }
        si (   memcmp(&p->translatio, &q->translatio,
                   magnitudo(p->translatio)) != ZEPHYRUM
            || memcmp(&p->scala, &q->scala, magnitudo(p->scala))
                   != ZEPHYRUM)
        {
            redde _differt("nodus (translatio, scala)");
        }
        si (memcmp(&p->rotatio, &q->rotatio, magnitudo(p->rotatio))
            != ZEPHYRUM)
        {
            redde _differt("nodus (rotatio)");
        }
    }
    si (   memcmp(a->retia, b->retia,
               a->retium * magnitudo(StatuaRete)) != ZEPHYRUM
        || memcmp(a->partes, b->partes,
               a->partium * magnitudo(StatuaPars)) != ZEPHYRUM)
    {
        redde _differt("retia aut tractus");
    }
    si (   !_f32_pares(a->positiones, b->positiones, a->verticum * III)
        || !_f32_pares(a->normales, b->normales, a->verticum * III)
        || !_f32_pares(a->coordinatae, b->coordinatae,
               a->verticum * II))
    {
        redde _differt("vertices");
    }
    si (   a->indicum
        && memcmp(a->indices, b->indices, a->indicum * magnitudo(i32))
               != ZEPHYRUM)
    {
        redde _differt("indices");
    }
    per (n = ZEPHYRUM; n < a->materiarum; n++)
    {
        constans StatuaMateria* p = &a->materiae[n];
        constans StatuaMateria* q = &b->materiae[n];

        si (   p->textura      != q->textura
            || !_f32_pares(p->color, q->color, IV)
            || p->limen_alpha  != q->limen_alpha
            || !p->emissiva    != !q->emissiva
            || !p->bilateralis != !q->bilateralis
            || !p->repetit_s   != !q->repetit_s
            || !p->repetit_t   != !q->repetit_t
            || !p->speculat_s  != !q->speculat_s
            || !p->speculat_t  != !q->speculat_t)
        {
            printf("  materia %u\n", (insignatus)n);
            redde _differt("materia");
        }
    }
    per (n = ZEPHYRUM; n < a->texturarum; n++)
    {
        si (   a->texturae[n].latitudo != b->texturae[n].latitudo
            || a->texturae[n].altitudo != b->texturae[n].altitudo
            || memcmp(a->texturae[n].pixela, b->texturae[n].pixela,
                   a->texturae[n].latitudo * a->texturae[n].altitudo
                       * IV) != ZEPHYRUM)
        {
            redde _differt("textura");
        }
    }
    per (n = ZEPHYRUM; n < a->cyclorum; n++)
    {
        constans StatuaCyclus* p = &a->cycli[n];
        constans StatuaCyclus* q = &b->cycli[n];

        si (   !chorda_aequalis(p->titulus, q->titulus)
            || p->duratio != q->duratio || p->canalium != q->canalium)
        {
            redde _differt("cyclus");
        }
        per (k = ZEPHYRUM; k < p->canalium; k++)
        {
            constans StatuaCanalis* x = &p->canales[k];
            constans StatuaCanalis* y = &q->canales[k];
                               i32  latus =
                                   x->proprietas == STATUA_ROTATIO
                                   ? IV : III;

            si (   x->nodus      != y->nodus
                || x->proprietas != y->proprietas
                || !x->gradatim  != !y->gradatim
                || x->clavium    != y->clavium
                || !_f32_pares(x->tempora, y->tempora, x->clavium)
                || !_f32_pares(x->valores, y->valores,
                       x->clavium * latus))
            {
                redde _differt("canalis");
            }
        }
    }
    redde VERUM;
}

interior b32
_continet (
        CredoContextus* c,
                chorda  octeti,
    constans character* fragmentum)
{
    redde chorda_continet(octeti,
        chorda_ex_literis(fragmentum, c->piscina));
}

interior PlasticaFructus
_res (
    CredoContextus* c)
{
    PlasticaFructus f = plastica_legere(
        chorda_ex_literis(RES, c->piscina), c->piscina);

    CREDO_NECESSE (f.successus);
    redde f;
}

/* lege -> scribe -> lege: lectio altera primae par; scriptura
 * lectionis alterius octetos eosdem dat */
interior vacuum
_iter_rotundum (
        CredoContextus* c,
    constans character* via)
{
    chorda  fons    = filum_legere_totum(via, c->piscina);
    Statua* prima;
    Statua* altera;
    chorda  scripta;
    chorda  iterum;

    CREDO_NECESSE (fons.mensura > ZEPHYRUM);
    prima    = _lege_glb(c, fons);
    scripta  = statua_glb_scribere(prima, NIHIL, c->piscina);
    CREDO_NECESSE (scripta.datum != NIHIL);
    CREDO_VERUM (scripta.mensura > XX);
    altera   = _lege_glb(c, scripta);
    CREDO_VERUM (_statuae_pares(prima, altera));
    iterum   = statua_glb_scribere(altera, NIHIL, c->piscina);
    CREDO_NECESSE (iterum.datum != NIHIL);
    CREDO_AEQUALIS_I32 (iterum.mensura, scripta.mensura);
    CREDO_VERUM (memcmp(iterum.datum, scripta.datum, scripta.mensura)
        == ZEPHYRUM);
    printf("  %s: %u -> %u octeti\n", via, (insignatus)fons.mensura,
        (insignatus)scripta.mensura);
}

interior vacuum
_sectio_continens (
    CredoContextus* c)
{
    PlasticaFructus f = _res(c);
             chorda g = statua_glb_scribere(f.statua, f.partes,
                 c->piscina);
               i32 longitudo;
               i32 json;

    CREDO_NECESSE (g.datum != NIHIL);
    CREDO_VERUM (memcmp(g.datum, "glTF", IV) == ZEPHYRUM);
    CREDO_AEQUALIS_I32 (g.datum[IV], II);
    longitudo = (i32)g.datum[VIII] | ((i32)g.datum[IX] << VIII)
        | ((i32)g.datum[X] << XVI) | ((i32)g.datum[XI] << XXIV);
    CREDO_AEQUALIS_I32 (longitudo, g.mensura);
    CREDO_AEQUALIS_I32 (g.mensura % IV, ZEPHYRUM);
    json = (i32)g.datum[XII] | ((i32)g.datum[XIII] << VIII)
        | ((i32)g.datum[XIV] << XVI) | ((i32)g.datum[XV] << XXIV);
    CREDO_AEQUALIS_I32 (json % IV, ZEPHYRUM);
    CREDO_VERUM (memcmp(g.datum + XVI, "JSON", IV) == ZEPHYRUM);
    CREDO_VERUM (memcmp(g.datum + XX + json + IV, "BIN", III)
        == ZEPHYRUM);
    CREDO_VERUM (_continet(c, g, "\"version\":\"2.0\""));
}

interior vacuum
_sectio_res (
    CredoContextus* c)
{
    PlasticaFructus f = _res(c);
             chorda g = statua_glb_scribere(f.statua, f.partes,
                 c->piscina);
            Statua* prima;
            Statua* altera;
            chorda  iterum;
               i32  n;

    CREDO_NECESSE (g.datum != NIHIL);
    prima = _lege_glb(c, g);
    /* figura statuae plasticae manet */
    CREDO_AEQUALIS_I32 (prima->nodorum, f.statua->nodorum);
    CREDO_AEQUALIS_I32 (prima->retium, f.statua->retium);
    CREDO_AEQUALIS_I32 (prima->partium, f.statua->partium);
    CREDO_AEQUALIS_I32 (prima->verticum, f.statua->verticum);
    CREDO_AEQUALIS_I32 (prima->indicum, f.statua->indicum);
    CREDO_AEQUALIS_I32 (prima->materiarum, f.statua->materiarum);
    CREDO_AEQUALIS_I32 (prima->texturarum, II);
    CREDO_VERUM (_f32_pares(prima->positiones, f.statua->positiones,
        prima->verticum * III));
    CREDO_VERUM (_f32_pares(prima->normales, f.statua->normales,
        prima->verticum * III));
    CREDO_VERUM (_f32_pares(prima->coordinatae,
        f.statua->coordinatae, prima->verticum * II));
    CREDO_VERUM (memcmp(prima->indices, f.statua->indices,
        prima->indicum * magnitudo(i32)) == ZEPHYRUM);
    per (n = ZEPHYRUM; n < prima->nodorum; n++)
    {
        CREDO_AEQUALIS_S32 (prima->nodi[n].parens,
            f.statua->nodi[n].parens);
        CREDO_AEQUALIS_S32 (prima->nodi[n].rete,
            f.statua->nodi[n].rete);
        CREDO_AEQUALIS_I32 (prima->nodi[n].signa,
            f.statua->nodi[n].signa);
    }
    /* textura genita et manu scripta pixela sua servant */
    CREDO_AEQUALIS_I32 (prima->texturae[ZEPHYRUM].latitudo, XXXII);
    CREDO_VERUM (memcmp(prima->texturae[ZEPHYRUM].pixela,
        f.statua->texturae[ZEPHYRUM].pixela, XXXII * XXXII * IV)
        == ZEPHYRUM);
    CREDO_AEQUALIS_I32 (prima->texturae[I].latitudo, II);
    CREDO_VERUM (memcmp(prima->texturae[I].pixela,
        f.statua->texturae[I].pixela, XVI) == ZEPHYRUM);
    /* obex invisus: alpha 0, limen 0.5 */
    n = prima->materiarum - I;
    CREDO_F64_PROXIMUS ((f64)prima->materiae[n].color[III], 0.0,
        1.0e-9);
    CREDO_F64_PROXIMUS ((f64)prima->materiae[n].limen_alpha, 0.5,
        1.0e-9);
    CREDO_VERUM (prima->materiae[ZEPHYRUM].repetit_s);
    CREDO_VERUM (prima->materiae[ZEPHYRUM].repetit_t);
    /* iter rotundum */
    iterum  = statua_glb_scribere(prima, f.partes, c->piscina);
    CREDO_NECESSE (iterum.datum != NIHIL);
    altera  = _lege_glb(c, iterum);
    CREDO_VERUM (_statuae_pares(prima, altera));
    CREDO_AEQUALIS_I32 (iterum.mensura, g.mensura);
    CREDO_VERUM (memcmp(iterum.datum, g.datum, g.mensura)
        == ZEPHYRUM);
}

interior vacuum
_sectio_json (
    CredoContextus* c)
{
    PlasticaFructus f = _res(c);
             chorda g = statua_glb_scribere(f.statua, f.partes,
                 c->piscina);
            chorda nuda = statua_glb_scribere(f.statua, NIHIL,
                c->piscina);

    CREDO_NECESSE (g.datum != NIHIL);
    CREDO_NECESSE (nuda.datum != NIHIL);
    /* nomina nodorum ex tabula */
    CREDO_VERUM (_continet(c, g, "\"name\":\"saxum\""));
    CREDO_VERUM (_continet(c, g, "\"name\":\"palus\""));
    CREDO_VERUM (_continet(c, g, "\"name\":\"well\""));
    CREDO_FALSUM (_continet(c, nuda, "\"name\""));
    /* signa */
    CREDO_VERUM (_continet(c, g, "\"tags\":[\"collidable\"]"));
    CREDO_VERUM (_continet(c, g, "\"tags\":[\"walkable\"]"));
    CREDO_FALSUM (_continet(c, g, "\"water\""));
    /* excisio, textura PNG, involutio repetita */
    CREDO_VERUM (_continet(c, g, "\"alphaMode\":\"MASK\""));
    CREDO_VERUM (_continet(c, g, "\"alphaCutoff\":0.5"));
    CREDO_VERUM (_continet(c, g, "\"mimeType\":\"image/png\""));
    CREDO_VERUM (_continet(c, g, "\"wrapS\":10497"));
    CREDO_VERUM (_continet(c, g, "\"wrapT\":10497"));
    CREDO_VERUM (_continet(c, g, "\"POSITION\""));
    CREDO_VERUM (_continet(c, g, "\"NORMAL\""));
    CREDO_VERUM (_continet(c, g, "\"TEXCOORD_0\""));
    CREDO_FALSUM (_continet(c, g, "\"animations\""));
}

/* materia: emissio, bilateralis, involutio ad marginem et speculata,
 * signum aquae; tractus sine materia */
interior vacuum
_sectio_materiae (
    CredoContextus* c)
{
     PlasticaFructus  f = _res(c);
              Statua  s = *f.statua;
       StatuaMateria* materiae;
         StatuaNodus* nodi;
          StatuaPars* partes;
              chorda  g;
              Statua* lecta;

    materiae = (StatuaMateria*)piscina_allocare(c->piscina,
        s.materiarum * magnitudo(StatuaMateria));
    nodi     = (StatuaNodus*)piscina_allocare(c->piscina,
        s.nodorum * magnitudo(StatuaNodus));
    partes   = (StatuaPars*)piscina_allocare(c->piscina,
        s.partium * magnitudo(StatuaPars));
    CREDO_NECESSE (materiae && nodi && partes);
    memcpy(materiae, s.materiae,
        s.materiarum * magnitudo(StatuaMateria));
    memcpy(nodi, s.nodi, s.nodorum * magnitudo(StatuaNodus));
    memcpy(partes, s.partes, s.partium * magnitudo(StatuaPars));
    s.materiae  = materiae;
    s.nodi      = nodi;
    s.partes    = partes;
    /* materia 0 (saxum, texturata): margo s, speculum t, emissiva,
     * bilateralis, limen 0.25 */
    materiae[ZEPHYRUM].repetit_s    = FALSUM;
    materiae[ZEPHYRUM].repetit_t    = VERUM;
    materiae[ZEPHYRUM].speculat_t   = VERUM;
    materiae[ZEPHYRUM].emissiva     = VERUM;
    materiae[ZEPHYRUM].bilateralis  = VERUM;
    materiae[ZEPHYRUM].limen_alpha  = 0.25f;
    /* tractus I sine materia; nodus ultimus aqua */
    partes[I].materia               = -I;
    nodi[s.nodorum - I].signa       = STATUA_SIGNUM_AQUA
        | STATUA_SIGNUM_SOLIDUM;
    g = statua_glb_scribere(&s, NIHIL, c->piscina);
    CREDO_NECESSE (g.datum != NIHIL);
    CREDO_VERUM (_continet(c, g, "\"wrapS\":33071"));
    CREDO_VERUM (_continet(c, g, "\"wrapT\":33648"));
    CREDO_VERUM (_continet(c, g, "\"doubleSided\":true"));
    CREDO_VERUM (_continet(c, g, "\"emissiveFactor\""));
    CREDO_VERUM (_continet(c, g, "\"alphaCutoff\":0.25"));
    CREDO_VERUM (_continet(c, g,
        "\"tags\":[\"collidable\",\"water\"]"));
    lecta = _lege_glb(c, g);
    CREDO_FALSUM (lecta->materiae[ZEPHYRUM].repetit_s);
    CREDO_VERUM (lecta->materiae[ZEPHYRUM].repetit_t);
    CREDO_FALSUM (lecta->materiae[ZEPHYRUM].speculat_s);
    CREDO_VERUM (lecta->materiae[ZEPHYRUM].speculat_t);
    CREDO_VERUM (lecta->materiae[ZEPHYRUM].emissiva);
    CREDO_VERUM (lecta->materiae[ZEPHYRUM].bilateralis);
    CREDO_F64_PROXIMUS ((f64)lecta->materiae[ZEPHYRUM].limen_alpha,
        0.25, 1.0e-9);
    CREDO_FALSUM (lecta->materiae[I].emissiva);
    CREDO_FALSUM (lecta->materiae[I].bilateralis);
    CREDO_AEQUALIS_S32 (lecta->partes[I].materia, -I);
    CREDO_AEQUALIS_I32 (lecta->nodi[s.nodorum - I].signa,
        STATUA_SIGNUM_AQUA | STATUA_SIGNUM_SOLIDUM);
}

interior vacuum
_sectio_fungus (
    CredoContextus* c)
{
    chorda  fons   = filum_legere_totum(VIA_FUNGI, c->piscina);
    Statua* prima  = _lege_glb(c, fons);
    chorda  g      = statua_glb_scribere(prima, NIHIL, c->piscina);

    /* fixum cyclos habet: aliter probatio nihil probat */
    CREDO_VERUM (prima->cyclorum > ZEPHYRUM);
    CREDO_VERUM (prima->cycli[ZEPHYRUM].canalium > ZEPHYRUM);
    CREDO_VERUM (prima->cycli[ZEPHYRUM].canales[ZEPHYRUM].clavium
        > I);
    CREDO_VERUM (_continet(c, g, "\"animations\""));
    _iter_rotundum(c, VIA_FUNGI);
}

interior vacuum
_sectio_lapis (
    CredoContextus* c)
{
    _iter_rotundum(c, VIA_LAPIDIS);
}

interior vacuum
_sectio_planum (
    CredoContextus* c)
{
    chorda  fons     = filum_legere_totum(VIA_PLANI, c->piscina);
    Statua* prima    = _lege_glb(c, fons);
       i32  signata  = ZEPHYRUM;
       i32  n;

    per (n = ZEPHYRUM; n < prima->nodorum; n++)
    {
        signata += (i32)(prima->nodi[n].signa != ZEPHYRUM);
    }
    /* fixum signa et texturas habet */
    CREDO_VERUM (signata > ZEPHYRUM);
    CREDO_VERUM (prima->texturarum > ZEPHYRUM);
    _iter_rotundum(c, VIA_PLANI);
}

/* cyclus gradatus et clavis ultima manu facta */
interior vacuum
_sectio_cyclus (
    CredoContextus* c)
{
    PlasticaFructus f = _res(c);
             Statua s = *f.statua;
       StatuaCyclus cyclus;
      StatuaCanalis canales[II];
                f32 tempora[III] = { 0.0f, 0.5f, 1.25f };
                f32 loci[IX]     = { 0.0f, 0.0f, 0.0f, 1.0f, 2.0f,
                    3.0f, -4.0f, 5.5f, 6.25f };
                f32 gyri[XII]    = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
             chorda  g;
             Statua* lecta;

    memset(canales, ZEPHYRUM, magnitudo(canales));
    canales[ZEPHYRUM].nodus = I;
    canales[ZEPHYRUM].proprietas = STATUA_TRANSLATIO;
    canales[ZEPHYRUM].clavium = III;
    canales[ZEPHYRUM].tempora = tempora;
    canales[ZEPHYRUM].valores = loci;
    canales[I].nodus = II;
    canales[I].proprietas = STATUA_ROTATIO;
    canales[I].gradatim = VERUM;
    canales[I].clavium = III;
    canales[I].tempora = tempora;
    canales[I].valores = gyri;
    cyclus.titulus = chorda_ex_literis("nutus", c->piscina);
    cyclus.duratio = 1.25f;
    cyclus.canales = canales;
    cyclus.canalium = II;
    s.cycli = &cyclus;
    s.cyclorum = I;
    g = statua_glb_scribere(&s, NIHIL, c->piscina);
    CREDO_NECESSE (g.datum != NIHIL);
    CREDO_VERUM (_continet(c, g, "\"name\":\"nutus\""));
    CREDO_VERUM (_continet(c, g, "\"interpolation\":\"STEP\""));
    CREDO_VERUM (_continet(c, g, "\"interpolation\":\"LINEAR\""));
    lecta = _lege_glb(c, g);
    CREDO_AEQUALIS_I32 (lecta->cyclorum, I);
    CREDO_NECESSE (lecta->cycli[ZEPHYRUM].canalium == II);
    CREDO_F64_PROXIMUS ((f64)lecta->cycli[ZEPHYRUM].duratio, 1.25,
        1.0e-9);
    CREDO_AEQUALIS_I32 (
        lecta->cycli[ZEPHYRUM].canales[ZEPHYRUM].clavium, III);
    CREDO_VERUM (_f32_pares(
        lecta->cycli[ZEPHYRUM].canales[ZEPHYRUM].tempora, tempora,
        III));
    CREDO_VERUM (_f32_pares(
        lecta->cycli[ZEPHYRUM].canales[ZEPHYRUM].valores, loci, IX));
    CREDO_FALSUM (lecta->cycli[ZEPHYRUM].canales[ZEPHYRUM].gradatim);
    CREDO_AEQUALIS_S32 (lecta->cycli[ZEPHYRUM].canales[I].nodus, II);
    CREDO_VERUM (lecta->cycli[ZEPHYRUM].canales[I].gradatim);
    CREDO_VERUM (_f32_pares(
        lecta->cycli[ZEPHYRUM].canales[I].valores, gyri, XII));
}

/* tractus qui vertices communicant: figura recta, vertices
 * duplicati */
interior vacuum
_sectio_communes (
    CredoContextus* c)
{
    f32 positiones[XII] = {
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    f32 normales[XII] = {
        0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };
    f32 coordinatae[VIII] = {
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f
    };
            i32  indices[VI] = { 0, 1, 2, 0, 2, 3 };
         Statua  s;
    StatuaNodus  nodus;
     StatuaRete  rete;
     StatuaPars  partes[II];
         chorda  g;
         Statua* lecta;

    memset(&s, ZEPHYRUM, magnitudo(s));
    memset(&nodus, ZEPHYRUM, magnitudo(nodus));
    nodus.parens = -I;
    nodus.rete = ZEPHYRUM;
    nodus.rotatio = stereometria_quaternio_identitas();
    nodus.scala = stereometria_vector(1.0f, 1.0f, 1.0f);
    rete.pars_prima = ZEPHYRUM;
    rete.partium = II;
    partes[ZEPHYRUM].index_primus = ZEPHYRUM;
    partes[ZEPHYRUM].indicum = III;
    partes[ZEPHYRUM].materia = -I;
    partes[I].index_primus = III;
    partes[I].indicum = III;
    partes[I].materia = -I;
    s.nodi = &nodus;
    s.nodorum = I;
    s.retia = &rete;
    s.retium = I;
    s.partes = partes;
    s.partium = II;
    s.positiones = positiones;
    s.normales = normales;
    s.coordinatae = coordinatae;
    s.verticum = IV;
    s.indices = indices;
    s.indicum = VI;
    CREDO_NECESSE (statua_finire(&s, c->piscina));
    g = statua_glb_scribere(&s, NIHIL, c->piscina);
    CREDO_NECESSE (g.datum != NIHIL);
    lecta = _lege_glb(c, g);
    CREDO_AEQUALIS_I32 (lecta->partium, II);
    CREDO_AEQUALIS_I32 (lecta->indicum, VI);
    /* tractus 0 vertices 0..2, tractus I vertices 0..3 */
    CREDO_AEQUALIS_I32 (lecta->verticum, VII);
    /* triangulum alterum: (0,0) (1,1) (0,1) */
    CREDO_F64_PROXIMUS ((f64)lecta->positiones[
        lecta->indices[IV] * III], 1.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)lecta->positiones[
        lecta->indices[IV] * III + I], 1.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)lecta->positiones[
        lecta->indices[V] * III], 0.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)lecta->positiones[
        lecta->indices[V] * III + I], 1.0, 1.0e-9);
}

/* tractus ordinati cum verticibus otiosis (nullus triangulus eos
 * tangit) ad fines: tabula verticum tota et indices absoluti
 * intacti redeunt */
interior vacuum
_sectio_otiosi (
    CredoContextus* c)
{
            f32  positiones[XXIV];
            f32  normales[XXIV];
            f32  coordinatae[XVI];
            i32  indices[VI];
         Statua  s;
    StatuaNodus  nodus;
     StatuaRete  rete;
     StatuaPars  partes[II];
         chorda  g;
         Statua* lecta;
            i32  k;

    per (k = ZEPHYRUM; k < XXIV; k++)
    {
        positiones[k]  = (f32)((k * VII) % XI) * 0.25f;
        normales[k]    = k % III == II ? 1.0f : 0.0f;
    }
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        coordinatae[k] = (f32)k * 0.125f;
    }
    /* tractus 0: vertices 0 1 2 (3 otiosus); tractus I: 5 6 7
     * (4 otiosus ante, nullus post) */
    indices[ZEPHYRUM]  = ZEPHYRUM;
    indices[I]         = I;
    indices[II]        = II;
    indices[III]       = V;
    indices[IV]        = VI;
    indices[V]         = VII;
    memset(&s, ZEPHYRUM, magnitudo(s));
    memset(&nodus, ZEPHYRUM, magnitudo(nodus));
    nodus.parens = -I;
    nodus.rete = ZEPHYRUM;
    nodus.rotatio = stereometria_quaternio_identitas();
    nodus.scala = stereometria_vector(1.0f, 1.0f, 1.0f);
    rete.pars_prima = ZEPHYRUM;
    rete.partium = II;
    partes[ZEPHYRUM].index_primus = ZEPHYRUM;
    partes[ZEPHYRUM].indicum = III;
    partes[ZEPHYRUM].materia = -I;
    partes[I].index_primus = III;
    partes[I].indicum = III;
    partes[I].materia = -I;
    s.nodi = &nodus;
    s.nodorum = I;
    s.retia = &rete;
    s.retium = I;
    s.partes = partes;
    s.partium = II;
    s.positiones = positiones;
    s.normales = normales;
    s.coordinatae = coordinatae;
    s.verticum = VIII;
    s.indices = indices;
    s.indicum = VI;
    CREDO_NECESSE (statua_finire(&s, c->piscina));
    g = statua_glb_scribere(&s, NIHIL, c->piscina);
    CREDO_NECESSE (g.datum != NIHIL);
    lecta = _lege_glb(c, g);
    CREDO_AEQUALIS_I32 (lecta->verticum, VIII);
    CREDO_AEQUALIS_I32 (lecta->indicum, VI);
    CREDO_VERUM (_f32_pares(lecta->positiones, positiones, XXIV));
    CREDO_VERUM (_f32_pares(lecta->normales, normales, XXIV));
    CREDO_VERUM (_f32_pares(lecta->coordinatae, coordinatae, XVI));
    CREDO_VERUM (memcmp(lecta->indices, indices, magnitudo(indices))
        == ZEPHYRUM);
}

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    PlasticaFructus  f = _res(c);
             Statua  s = *f.statua;
                f32* positiones;
         StatuaPars* partes;
             chorda  g;

    g = statua_glb_scribere(NIHIL, NIHIL, c->piscina);
    CREDO_VERUM (g.datum == NIHIL);
    CREDO_AEQUALIS_I32 (g.mensura, ZEPHYRUM);
    g = statua_glb_scribere(f.statua, NIHIL, NIHIL);
    CREDO_VERUM (g.datum == NIHIL);
    /* sine nodis */
    s.nodorum  = ZEPHYRUM;
    g          = statua_glb_scribere(&s, NIHIL, c->piscina);
    CREDO_VERUM (g.datum == NIHIL);
    /* positio non finita */
    s = *f.statua;
    positiones = (f32*)piscina_allocare(c->piscina,
        s.verticum * III * magnitudo(f32));
    CREDO_NECESSE (positiones != NIHIL);
    memcpy(positiones, s.positiones,
        s.verticum * III * magnitudo(f32));
    positiones[V]  = (f32)HUGE_VAL;
    s.positiones   = positiones;
    g              = statua_glb_scribere(&s, NIHIL, c->piscina);
    CREDO_VERUM (g.datum == NIHIL);
    /* tractus sine triangulis */
    s = *f.statua;
    partes = (StatuaPars*)piscina_allocare(c->piscina,
        s.partium * magnitudo(StatuaPars));
    CREDO_NECESSE (partes != NIHIL);
    memcpy(partes, s.partes, s.partium * magnitudo(StatuaPars));
    partes[I].indicum  = ZEPHYRUM;
    s.partes           = partes;
    g                  = statua_glb_scribere(&s, NIHIL, c->piscina);
    CREDO_VERUM (g.datum == NIHIL);
    /* integra adhuc scribitur */
    g = statua_glb_scribere(f.statua, NIHIL, c->piscina);
    CREDO_VERUM (g.datum != NIHIL);
}

/* plastica-motus-plan T8: nomina nodorum et momenta cyclorum per
 * GLB eunt et redeunt */
interior vacuum
_sectio_momenta (
    CredoContextus* c)
{
    PlasticaFructus f = _res(c);
             Statua s = *f.statua;
       StatuaCyclus cycli[II];
      StatuaCanalis canales[II];
     StatuaMomentum momenta[III];
     StatuaMomentum malum;
      StatuaFructus r;
                f32 tempora[III] = { 0.0f, 0.5f, 1.25f };
                f32 loci[IX]     = { 0.0f, 0.0f, 0.0f, 1.0f, 2.0f,
                    3.0f, -4.0f, 5.5f, 6.25f };
             chorda g;
             chorda nuda;
                i32 n;

    memset(canales, ZEPHYRUM, magnitudo(canales));
    memset(cycli, ZEPHYRUM, magnitudo(cycli));
    per (n = ZEPHYRUM; n < II; n++)
    {
        canales[n].nodus       = I;
        canales[n].proprietas  = STATUA_TRANSLATIO;
        canales[n].clavium     = III;
        canales[n].tempora     = tempora;
        canales[n].valores     = loci;
        cycli[n].duratio       = 1.25f;
        cycli[n].canales       = &canales[n];
        cycli[n].canalium      = I;
    }
    cycli[ZEPHYRUM].titulus  = chorda_ex_literis("walk", c->piscina);
    cycli[I].titulus         = chorda_ex_literis("attack", c->piscina);
    s.cycli                  = cycli;
    s.cyclorum               = II;
    /* ordine mixto data: cyclus I ante cyclum 0 */
    momenta[ZEPHYRUM].cyclus   = I;
    momenta[ZEPHYRUM].titulus  = chorda_ex_literis("ictus", c->piscina);
    momenta[ZEPHYRUM].tempus   = 0.3f;
    momenta[I].cyclus          = ZEPHYRUM;
    momenta[I].titulus         =
        chorda_ex_literis("gressus_sin", c->piscina);
    momenta[I].tempus   = 0.0f;
    momenta[II].cyclus  = ZEPHYRUM;
    momenta[II].titulus        =
        chorda_ex_literis("gressus_dex", c->piscina);
    momenta[II].tempus         = 0.625f;
    g = statua_glb_scribere_cum_momentis(&s, f.partes, momenta, III,
        c->piscina);
    CREDO_NECESSE (g.datum != NIHIL);
    CREDO_VERUM (_continet(c, g, "\"moments\":[{\"name\":\"ictus\""));
    r = statua_glb_legere(g, c->piscina);
    CREDO_NECESSE (r.successus && r.tituli != NIHIL);
    CREDO_NECESSE (r.statua->nodorum == s.nodorum);
    /* nomina: omnia redeunt, vacua vacua */
    per (n = ZEPHYRUM; n < s.nodorum; n++)
    {
        CREDO_VERUM (chorda_aequalis(r.tituli[n], f.partes[n]));
    }
    CREDO_VERUM (chorda_aequalis_literis(r.tituli[II], "saxum"));
    /* momenta: ordine cyclorum, intra cyclum ordine dato; tempus
     * f32 exacte */
    CREDO_NECESSE (r.momentorum == III && r.momenta != NIHIL);
    CREDO_AEQUALIS_I32 (r.momenta[ZEPHYRUM].cyclus, ZEPHYRUM);
    CREDO_VERUM (chorda_aequalis_literis(r.momenta[ZEPHYRUM].titulus,
        "gressus_sin"));
    CREDO_VERUM (_f32_pares(&r.momenta[ZEPHYRUM].tempus,
        &momenta[I].tempus, I));
    CREDO_AEQUALIS_I32 (r.momenta[I].cyclus, ZEPHYRUM);
    CREDO_VERUM (chorda_aequalis_literis(r.momenta[I].titulus,
        "gressus_dex"));
    CREDO_VERUM (_f32_pares(&r.momenta[I].tempus, &momenta[II].tempus,
        I));
    CREDO_AEQUALIS_I32 (r.momenta[II].cyclus, I);
    CREDO_VERUM (chorda_aequalis_literis(r.momenta[II].titulus,
        "ictus"));
    CREDO_VERUM (_f32_pares(&r.momenta[II].tempus,
        &momenta[ZEPHYRUM].tempus, I));
    /* statua ipsa par manet */
    CREDO_VERUM (_statuae_pares(_lege_glb(c,
        statua_glb_scribere(&s, f.partes, c->piscina)), r.statua));
    /* determinatus */
    nuda = statua_glb_scribere_cum_momentis(&s, f.partes, momenta,
        III, c->piscina);
    CREDO_NECESSE (nuda.mensura == g.mensura);
    CREDO_VERUM (memcmp(nuda.datum, g.datum, g.mensura) == ZEPHYRUM);
    /* sine momentis: octeti idem ac statua_glb_scribere, nullum
     * "moments" */
    nuda = statua_glb_scribere(&s, f.partes, c->piscina);
    g    = statua_glb_scribere_cum_momentis(&s, f.partes, momenta,
        ZEPHYRUM, c->piscina);
    CREDO_NECESSE (nuda.datum != NIHIL && g.mensura == nuda.mensura);
    CREDO_VERUM (memcmp(nuda.datum, g.datum, g.mensura) == ZEPHYRUM);
    CREDO_FALSUM (_continet(c, g, "moments"));
    r = statua_glb_legere(g, c->piscina);
    CREDO_NECESSE (r.successus);
    CREDO_NIHIL (r.momenta);
    CREDO_AEQUALIS_I32 (r.momentorum, ZEPHYRUM);
    /* sine nominibus: tabula adest, omnia vacua */
    r = statua_glb_legere(statua_glb_scribere(&s, NIHIL, c->piscina),
        c->piscina);
    CREDO_NECESSE (r.successus && r.tituli != NIHIL);
    per (n = ZEPHYRUM; n < s.nodorum; n++)
    {
        CREDO_AEQUALIS_I32 (r.tituli[n].mensura, ZEPHYRUM);
    }
    /* recusationes: cyclus ignotus, titulus vacuus, tempus
     * negativum, non finitum, tabula nulla cum numero */
    malum         = momenta[ZEPHYRUM];
    malum.cyclus  = II;
    CREDO_VERUM (statua_glb_scribere_cum_momentis(&s, f.partes,
        &malum, I, c->piscina).datum == NIHIL);
    malum                  = momenta[ZEPHYRUM];
    malum.titulus.mensura  = ZEPHYRUM;
    CREDO_VERUM (statua_glb_scribere_cum_momentis(&s, f.partes,
        &malum, I, c->piscina).datum == NIHIL);
    malum         = momenta[ZEPHYRUM];
    malum.tempus  = -0.25f;
    CREDO_VERUM (statua_glb_scribere_cum_momentis(&s, f.partes,
        &malum, I, c->piscina).datum == NIHIL);
    malum.tempus  = (f32)HUGE_VAL;
    CREDO_VERUM (statua_glb_scribere_cum_momentis(&s, f.partes,
        &malum, I, c->piscina).datum == NIHIL);
    CREDO_VERUM (statua_glb_scribere_cum_momentis(&s, f.partes, NIHIL,
        I, c->piscina).datum == NIHIL);
    /* malum ultimum inter bona etiam recusat */
    momenta[II].cyclus = V;
    CREDO_VERUM (statua_glb_scribere_cum_momentis(&s, f.partes,
        momenta, III, c->piscina).datum == NIHIL);
    /* momentum ad tempus 0 et in fine licet */
    momenta[II].cyclus = I;
    momenta[II].tempus = 1.25f;
    CREDO_VERUM (statua_glb_scribere_cum_momentis(&s, f.partes,
        momenta, III, c->piscina).datum != NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "continens", _sectio_continens, NIHIL, NIHIL, NIHIL, NIHIL },
    { "res", _sectio_res, NIHIL, NIHIL, NIHIL, NIHIL },
    { "json", _sectio_json, NIHIL, NIHIL, NIHIL, NIHIL },
    { "materiae", _sectio_materiae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fungus", _sectio_fungus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lapis", _sectio_lapis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "planum", _sectio_planum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cyclus", _sectio_cyclus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "momenta", _sectio_momenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "communes", _sectio_communes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "otiosi", _sectio_otiosi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("statua_glb_scriptor", SECTIONES);
}
