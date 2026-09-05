/* probatio_briar_contextus.c - Fragmenta et transclusio (spec par.
 * 3.4): lector lineae, classificatio (id=), textura radicum cum
 * indentatione portata, tabula linearum ad suturas pinnata, tabula
 * identitatis radicis sine transclusione, fragmentum non adhibitum,
 * recusationes VI cum linea. Aurum fixa/contextus/fragmenta.contextus
 * (BRIAR_CONTEXTUS_SCRIBERE=1 cum causa nominata).
 */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_contextus.h"
#include "briar_nexus.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* FIXA =
    "briar/probationes/fixa/thistle/";
hic_manens constans character* AURUM =
    "briar/probationes/fixa/contextus/fragmenta.contextus";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior constans character*
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b)
{
    redde chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(a,
        piscina), chorda_ex_literis(b, piscina), piscina), piscina);
}

/* arbor + nexus + contextus; fragmenta_out optionale */
interior Xar*
_contexere (
               Piscina*  piscina,
   InternamentumChorda*  intern,
    constans character*  via,
                   Xar** fragmenta_out,
                   s32*  radices)
{
        character* textus;
              i32  mensura = ZEPHYRUM;
    MateriaNodus* doc;
             Xar* nexus;

    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        redde NIHIL;
    }
    doc    = briar_arbor_parsare(piscina, textus, mensura);
    nexus  = briar_nexus_texere(piscina, doc, intern);
    si (nexus == NIHIL)
    {
        redde NIHIL;
    }
    *radices = briar_contexere(piscina, nexus, fragmenta_out);
    redde nexus;
}

/* regio C n-esima (0-basata) */
interior BriarNexusRes*
_regio_c (
    Xar* nexus,
    i32  quota)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (   r->genus == BRIAR_NEXUS_REGIO
            && briar_nexus_titulus_est(r, "c"))
        {
            si (n == quota)
            {
                redde r;
            }
            n = n + I;
        }
    }
    redde NIHIL;
}

interior s32
_linea (
    constans BriarNexusRes* r,
                       i32  index)
{
    si (r->lineae == NIHIL || index >= xar_numerus(r->lineae))
    {
        redde -I;
    }
    redde (s32)*(i32*)xar_obtinere(r->lineae, index);
}

/* linea k contextus (sine '\n') */
interior chorda
_linea_contextus (
    constans BriarNexusRes* r,
                       i32  quota)
{
    chorda c = r->contextus;
       i32 i = ZEPHYRUM;
       i32 n = ZEPHYRUM;

    dum (i < c.mensura)
    {
        i32 f = i;

        dum (f < c.mensura && (character)c.datum[f] != '\n')
        {
            f = f + I;
        }
        si (n == quota)
        {
            redde chorda_sectio(c, i, f);
        }
        n = n + I;
        i = f + I;
    }
    redde chorda_ex_literis("", NIHIL);
}

interior i32
_numerus_linearum (
    chorda c)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si ((character)c.datum[i] == '\n')
        {
            n = n + I;
        }
    }
    redde n;
}

interior b32
_continet (
               Piscina* piscina,
                chorda  fenum,
    constans character* acus)
{
    redde (b32)(strstr(chorda_ut_cstr(fenum, piscina), acus) != NIHIL);
}

interior constans BriarFragmentum*
_fragmentum (
                   Xar* fragmenta,
    constans character* id)
{
    i32 i;

    per (i = ZEPHYRUM; fragmenta != NIHIL && i < xar_numerus(fragmenta);
        i++)
    {
        constans BriarFragmentum* f =
            (constans BriarFragmentum*)xar_obtinere(fragmenta, i);

        si (chorda_aequalis_literis(f->id, id))
        {
            redde f;
        }
    }
    redde NIHIL;
}

/* recusatio: regio C quota (0-basata) causam continet + lineam */
interior vacuum
_recusationem_probare (
               Piscina* piscina,
   InternamentumChorda* intern,
    constans character* fixum,
                   i32  quota,
    constans character* fragmentum,
                   i32  linea)
{
              Xar* nexus;
              s32  radices = -I;
    BriarNexusRes* r;

    nexus = _contexere(piscina, intern, _texere(piscina, FIXA,
        _texere(piscina, "adversa/", fixum)), NIHIL, &radices);
    CREDO_NON_NIHIL (nexus);
    CREDO_VERUM (radices >= ZEPHYRUM);
    r = _regio_c(nexus, quota);
    CREDO_NON_NIHIL (r);
    si (r == NIHIL)
    {
        redde;
    }
    imprimere("  %s: %.*s (linea %d)\n", fixum,
        (integer)r->causa.mensura,
        (constans character*)r->causa.datum, (integer)r->linea_erroris);
    CREDO_VERUM (_continet(piscina, r->causa, fragmentum));
    CREDO_AEQUALIS_I32 (r->linea_erroris, linea);
}

interior vacuum
_lectorem_probare (
               Piscina* piscina,
    constans character* linea,
                   s32  genus,
    constans character* id,
    constans character* praefixum)
{
    chorda i;
    chorda p;
       s32 g;

    i.datum = NIHIL; i.mensura = ZEPHYRUM;
    p = i;
    g = briar_contextus_lineam_legere(chorda_ex_literis(linea, piscina),
        &i, &p);
    CREDO_AEQUALIS_S32 (g, genus);
    si (genus == I)
    {
        CREDO_CHORDA_AEQUALIS (i, chorda_ex_literis(id, piscina));
        CREDO_CHORDA_AEQUALIS (p, chorda_ex_literis(praefixum,
            piscina));
    }
}

interior vacuum
_aurum_conferre (
    Piscina* piscina,
        Xar* nexus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)8192);
       i32 i;
    chorda genitum;

    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* r =
            (constans BriarNexusRes*)xar_obtinere(nexus, i);
        character b[48];
              i32 k;

        si (   r->genus != BRIAR_NEXUS_REGIO
            || !briar_nexus_titulus_est(r, "c") || r->est_fragmentum)
        {
            perge;
        }
        sprintf(b, "=== radix linea %d ===\n",
            (integer)r->linea_initium);
        chorda_aedificator_appendere_literis(a, b);
        chorda_aedificator_appendere_chorda(a, r->contextus);
        chorda_aedificator_appendere_literis(a, "=== lineae ===\n");
        per (k = ZEPHYRUM; r->lineae != NIHIL
            && k < xar_numerus(r->lineae);
            k++)
        {
            sprintf(b, "%s%d", k ? " " : "", (integer)_linea(r, k));
            chorda_aedificator_appendere_literis(a, b);
        }
        chorda_aedificator_appendere_literis(a, "\n");
    }
    genitum = chorda_aedificator_finire(a);
    si (getenv("BRIAR_CONTEXTUS_SCRIBERE") != NIHIL)
    {
        si (filum_scribere(AURUM, genitum))
        {
            imprimere("  AURUM SCRIPTUM: %s\n", AURUM);
        }
        alioquin
        {
            CREDO_CULPA ("aurum non scriptum");
        }
        redde;
    }
    {
        chorda aurum = filum_legere_totum(AURUM, piscina);

        CREDO_VERUM (aurum.mensura > ZEPHYRUM);
        si (!chorda_aequalis(aurum, genitum))
        {
            imprimere("  DIFFERT: %s\n--- genitum ---\n%.*s\n"
                "--- aurum ---\n%.*s\n", AURUM,
                (integer)genitum.mensura,
                (constans character*)genitum.datum,
                (integer)aurum.mensura,
                (constans character*)aurum.datum);
        }
        CREDO_CHORDA_AEQUALIS (genitum, aurum);
    }
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_briar_contextus",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans lector lineae ---\n");
    _lectorem_probare(piscina, "    <<#summa>>", I, "summa", "    ");
    _lectorem_probare(piscina, "<<#x>>  ", I, "x", "");
    _lectorem_probare(piscina, "\t<<#a-b_c>>", I, "a-b_c", "\t");
    _lectorem_probare(piscina, "  <<#x>>\r", I, "x", "  ");
    _lectorem_probare(piscina, "<<#x>> extra", -I, NIHIL, NIHIL);
    _lectorem_probare(piscina, "<<#>>", -I, NIHIL, NIHIL);
    _lectorem_probare(piscina, "<<#x", -I, NIHIL, NIHIL);
    _lectorem_probare(piscina, "a << b", ZEPHYRUM, NIHIL, NIHIL);
    _lectorem_probare(piscina, "    redde <<#x>>;", ZEPHYRUM, NIHIL,
        NIHIL);
    _lectorem_probare(piscina, "", ZEPHYRUM, NIHIL, NIHIL);
    _lectorem_probare(piscina, "<<x>>", ZEPHYRUM, NIHIL, NIHIL);

    imprimere("\n--- Probans fragmenta.thistle: classificatio, textura,"
        " tabula ---\n");
    {
                         Xar* nexus;
                         Xar* fragmenta  = NIHIL;
                         s32  radices    = -I;
               BriarNexusRes* app;
               BriarNexusRes* prob;
    constans BriarFragmentum* f;

        nexus = _contexere(piscina, intern, _texere(piscina, FIXA,
            "fragmenta.thistle"), &fragmenta, &radices);
        CREDO_NON_NIHIL (nexus);
        CREDO_AEQUALIS_S32 (radices, II);
        CREDO_NON_NIHIL (fragmenta);
        CREDO_AEQUALIS_I32 (xar_numerus(fragmenta), V);
        f = _fragmentum(fragmenta, "incrementum");
        CREDO_NON_NIHIL (f);
        CREDO_VERUM (f != NIHIL && f->regio->est_fragmentum);
        CREDO_VERUM (f != NIHIL && xar_numerus(f->usus) == I
            && *(i32*)xar_obtinere(f->usus, ZEPHYRUM) == XV);
        f = _fragmentum(fragmenta, "salutatio");
        CREDO_NON_NIHIL (f);
        CREDO_VERUM (f != NIHIL && xar_numerus(f->usus) == II
            && *(i32*)xar_obtinere(f->usus, ZEPHYRUM) == XLIX
            && *(i32*)xar_obtinere(f->usus, I) == LXX);
        f = _fragmentum(fragmenta, "otiosum");
        CREDO_NON_NIHIL (f);
        CREDO_VERUM (f != NIHIL && xar_numerus(f->usus) == ZEPHYRUM);
        /* ordo documenti */
        CREDO_VERUM (chorda_aequalis_literis(
            ((BriarFragmentum*)xar_obtinere(fragmenta, ZEPHYRUM))->id,
            "incrementum"));
        CREDO_VERUM (chorda_aequalis_literis(
            ((BriarFragmentum*)xar_obtinere(fragmenta, IV))->id,
            "otiosum"));
        /* radices: regiones C V et VI (0-basatae), non fragmenta */
        app   = _regio_c(nexus, V);
        prob  = _regio_c(nexus, VI);
        CREDO_NON_NIHIL (app);
        CREDO_NON_NIHIL (prob);
        si (app != NIHIL && prob != NIHIL)
        {
            CREDO_FALSUM (app->est_fragmentum);
            CREDO_FALSUM (prob->est_fragmentum);
            CREDO_AEQUALIS_I32 (app->linea_initium, XXXII);
            CREDO_AEQUALIS_I32 (app->linea_erroris, ZEPHYRUM);
            CREDO_NON_NIHIL (app->lineae);
            /* XXVIII lineae contenti - III transclusiones + VI textae
             * fragmentorum */
            CREDO_AEQUALIS_I32 (xar_numerus(app->lineae), XXXI);
            CREDO_AEQUALIS_I32 (_numerus_linearum(app->contextus),
                XXXI);
            /* suturae: capita (32 -> 20), summa (40 -> 13 14 9 16),
             * reditus (41), salutatio (49 -> 24), reditus (50) */
            CREDO_AEQUALIS_S32 (_linea(app, ZEPHYRUM), XX);
            CREDO_AEQUALIS_S32 (_linea(app, I), XXXIII);
            CREDO_AEQUALIS_S32 (_linea(app, VIII), XIII);
            CREDO_AEQUALIS_S32 (_linea(app, IX), XIV);
            CREDO_AEQUALIS_S32 (_linea(app, X), IX);
            CREDO_AEQUALIS_S32 (_linea(app, XI), XVI);
            CREDO_AEQUALIS_S32 (_linea(app, XII), XLI);
            CREDO_AEQUALIS_S32 (_linea(app, XX), XXIV);
            CREDO_AEQUALIS_S32 (_linea(app, XXI), L);
            CREDO_AEQUALIS_S32 (_linea(app, XXX), LIX);
            /* indentatio portata: IV radicis + IV fragmenti */
            CREDO_CHORDA_AEQUALIS (_linea_contextus(app, X),
                chorda_ex_literis("        summa = summa + numeri[i];",
                piscina));
            CREDO_CHORDA_AEQUALIS (_linea_contextus(app, ZEPHYRUM),
                chorda_ex_literis("#include \"chorda.h\"", piscina));
            CREDO_CHORDA_AEQUALIS (_linea_contextus(app, VIII),
                chorda_ex_literis("    per (i = ZEPHYRUM; i < n; i++)",
                piscina));
            CREDO_CHORDA_AEQUALIS (_linea_contextus(app, XX),
                chorda_ex_literis("    chorda salutatio = "
                "chorda_ex_literis(\"salve, fragmenta\", piscina);",
                piscina));
            CREDO_FALSUM (_continet(piscina, app->contextus, "<<#"));
            /* probatio: XVIII lineae, salutatio (70 -> 24) */
            CREDO_AEQUALIS_I32 (prob->linea_initium, LXIII);
            CREDO_AEQUALIS_I32 (xar_numerus(prob->lineae), XVIII);
            CREDO_AEQUALIS_S32 (_linea(prob, VII), XXIV);
            CREDO_AEQUALIS_S32 (_linea(prob, VIII), LXXI);
            /* fragmenta: contextus = contentum, sine tabula */
            f = _fragmentum(fragmenta, "summa");
            CREDO_VERUM (f != NIHIL && f->regio->lineae == NIHIL);
            CREDO_VERUM (f != NIHIL
                && chorda_aequalis(f->regio->contextus,
                f->regio->contentum));
        }
        _aurum_conferre(piscina, nexus);
    }

    imprimere("\n--- Probans salve.thistle: sine fragmentis, tabula"
        " identitatis ---\n");
    {
                  Xar* nexus;
                  Xar* fragmenta  = NIHIL;
                  s32  radices    = -I;
                  i32  k;
        BriarNexusRes* r;

        nexus = _contexere(piscina, intern, _texere(piscina, FIXA,
            "salve.thistle"), &fragmenta, &radices);
        CREDO_NON_NIHIL (nexus);
        CREDO_AEQUALIS_S32 (radices, II);
        CREDO_VERUM (fragmenta == NIHIL || xar_numerus(fragmenta)
            == ZEPHYRUM);
        r = _regio_c(nexus, ZEPHYRUM);
        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            CREDO_NON_NIHIL (r->lineae);
            CREDO_VERUM (chorda_aequalis(r->contextus, r->contentum));
            CREDO_AEQUALIS_I32 (xar_numerus(r->lineae),
                _numerus_linearum(r->contentum));
            per (k = ZEPHYRUM; k < xar_numerus(r->lineae); k++)
            {
                si (_linea(r, k) != (s32)(r->linea_initium + k))
                {
                    CREDO_CULPA ("tabula identitatis fracta");
                    frange;
                }
            }
            CREDO_VERUM (k == xar_numerus(r->lineae));
        }
    }

    imprimere("\n--- Probans recusationes VI ---\n");
    _recusationem_probare(piscina, intern, "fragmentum_absens.thistle",
        ZEPHYRUM, "transclusio '#nusquam' (linea 8): fragmentum non"
        " definitum", VIII);
    _recusationem_probare(piscina, intern,
        "fragmentum_circulus.thistle",
        II, "transclusio '#a' (linea 9): circulus #a -> #b -> #a", IX);
    _recusationem_probare(piscina, intern,
        "fragmentum_iteratum.thistle",
        I, "fragmentum '#bis' iteratum (prima linea 4)", VIII);
    _recusationem_probare(piscina, intern, "fragmentum_munus.thistle",
        ZEPHYRUM,
        "fragmentum '#x' (linea 4): methodus/munus in fragmento"
        " - fragmentum radix non est", IV);
    _recusationem_probare(piscina, intern, "fragmentum_sine_id.thistle",
        ZEPHYRUM, "fragmentum sine id valido (linea 4)", IV);
    _recusationem_probare(piscina, intern,
        "transclusio_malformata.thistle", I,
        "transclusio malformata (linea 12): '<<#x>>' sola in linea sua"
        " exspectata", XII);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
