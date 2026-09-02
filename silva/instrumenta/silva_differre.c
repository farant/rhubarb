/* silva_differre.c - machina differentiae unitatum C89
 *
 * Corpora verbatim e principalia/differre.c mota (2026-08-18);
 * doctrina et historia in differre.c + differre.worklog.md manent.
 */

#include "silva_differre.h"

#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "sigillum.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_unitates.h"

#include <stdio.h>
#include <string.h>

interior SilvaToken*
_lexema_ad (
    Xar* lexemata,
    i32  i);

interior SilvaToken*
_lexema_ad (
    Xar* lexemata,
    i32  i)
{
    redde *(SilvaToken**)xar_obtinere(lexemata, i);
}

interior SilvaUnitas*
_unitas_ad (
    Xar* unitates,
    i32  i);

interior SilvaUnitas*
_unitas_ad (
    Xar* unitates,
    i32  i)
{
    redde (SilvaUnitas*)xar_obtinere(unitates, i);
}

/* initium byte unitatis: trivia praecedentia lexematis primi (si
 * adsunt), alioquin lexema ipsum */
interior i32
_unitatis_initium (
                     Xar* lexemata,
    constans SilvaUnitas* u);

interior i32
_unitatis_initium (
                     Xar* lexemata,
    constans SilvaUnitas* u)
{
    SilvaToken* primum = _lexema_ad(lexemata, u->lexema_primum);

    si (   primum->spatia_ante != NIHIL
        && xar_numerus(primum->spatia_ante) > 0)
    {
        SilvaToken* trivia = *(SilvaToken**)xar_obtinere(
            primum->spatia_ante, 0);

        si (trivia->byte_offset >= 0)
        {
            redde (i32)trivia->byte_offset;
        }
    }
    si (primum->byte_offset >= 0)
    {
        redde (i32)primum->byte_offset;
    }
    redde ZEPHYRUM;
}

b32
silva_differre_latus_ex_textu (
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda  textus,
     constans character* titulus,
     SilvaDifferreLatus* latus)
{
    i32 k;

    si (textus.datum == NIHIL)
    {
        textus = chorda_ex_literis("", piscina);
    }
    latus->textus = textus;
    latus->lexemata = silva_lexare(piscina,
        (constans character*)latus->textus.datum,
        latus->textus.mensura, ZEPHYRUM);
    si (latus->lexemata == NIHIL)
    {
        fprintf(stderr, "differre: lexari non potuit: %s\n",
            titulus);
        redde FALSUM;
    }
    latus->unitates = silva_unitates_scandere(piscina,
        latus->lexemata);
    si (latus->unitates == NIHIL)
    {
        redde FALSUM;
    }
    latus->numerus = xar_numerus(latus->unitates);
    latus->initia = (i32*)piscina_allocare(piscina,
        (memoriae_index)(latus->numerus == 0 ? I : latus->numerus)
            * magnitudo(i32));
    latus->fines = (i32*)piscina_allocare(piscina,
        (memoriae_index)(latus->numerus == 0 ? I : latus->numerus)
            * magnitudo(i32));
    latus->identitates = (vacuum**)piscina_allocare(piscina,
        (memoriae_index)(latus->numerus == 0 ? I : latus->numerus)
            * magnitudo(vacuum*));
    latus->identitates_normatae = (vacuum**)piscina_allocare(
        piscina,
        (memoriae_index)(latus->numerus == 0 ? I : latus->numerus)
            * magnitudo(vacuum*));
    si (   latus->initia               == NIHIL || latus->fines == NIHIL
        || latus->identitates          == NIHIL
        || latus->identitates_normatae == NIHIL)
    {
        redde FALSUM;
    }

    /* partitio: primum ab 0, limites = initia computata unitatum
     * sequentium, ultimum ad finem plagulae */
    per (k = 0; k < latus->numerus; k = k + 1)
    {
        latus->initia[k] = k == 0 ? (i32)ZEPHYRUM
            : _unitatis_initium(latus->lexemata,
                  _unitas_ad(latus->unitates, k));
    }
    per (k = 0; k < latus->numerus; k = k + 1)
    {
        latus->fines[k] = k + 1 < latus->numerus
            ? latus->initia[k + 1] : latus->textus.mensura;
    }
    per (k = 0; k < latus->numerus; k = k + 1)
    {
         Sigillum  sig;
        character  hex[SIGILLUM_HEX_MENSURA];
           chorda  hex_ch;
           chorda* internata;

        sig = sigillum_computare(
            (constans vacuum*)(latus->textus.datum
                + latus->initia[k]),
            (memoriae_index)(latus->fines[k] - latus->initia[k]));
        sigillum_hex(&sig, hex);
        hex_ch     = chorda_ex_buffer((i8*)hex, 64);
        internata  = chorda_internare(intern, hex_ch);
        si (internata == NIHIL)
        {
            redde FALSUM;
        }
        latus->identitates[k] = (vacuum*)internata;
    }
    /* sigilla NORMATA: series lexematum (genus + valor) spatiis
     * exempta - identitas quae reformationem spatialem transit
     * (clavis parium sub W spatiali, 01M0D4RN3B) */
    per (k = 0; k < latus->numerus; k = k + 1)
    {
        constans SilvaUnitas* u = _unitas_ad(latus->unitates, k);
           ChordaAedificator* aed = chorda_aedificator_creare(
               piscina, (memoriae_index)128);
         Sigillum  sig;
        character  hex[SIGILLUM_HEX_MENSURA];
           chorda  normata;
           chorda* internata;
              i32  j;

        si (aed == NIHIL)
        {
            redde FALSUM;
        }
        per (j = u->lexema_primum; j < u->lexema_finis; j = j + 1)
        {
            SilvaToken* t = _lexema_ad(latus->lexemata, j);
                    i8  limites[2];

            limites[0] = (i8)((integer)t->genus + I);
            limites[1] = (i8)0x1F;
            chorda_aedificator_appendere_chorda(aed,
                chorda_ex_buffer(limites, (i32)II));
            chorda_aedificator_appendere_chorda(aed, t->valor);
            limites[0] = (i8)0x1E;
            chorda_aedificator_appendere_chorda(aed,
                chorda_ex_buffer(limites, (i32)I));
        }
        normata = chorda_aedificator_finire(aed);
        sig = sigillum_computare((constans vacuum*)normata.datum,
            (memoriae_index)normata.mensura);
        sigillum_hex(&sig, hex);
        internata = chorda_internare(intern,
            chorda_ex_buffer((i8*)hex, 64));
        si (internata == NIHIL)
        {
            redde FALSUM;
        }
        latus->identitates_normatae[k] = (vacuum*)internata;
    }
    redde VERUM;
}

chorda
silva_differre_spatium (
    constans SilvaDifferreLatus* latus,
                            i32  k)
{
    redde chorda_ex_buffer(latus->textus.datum + latus->initia[k],
        latus->fines[k] - latus->initia[k]);
}

/* --------------------------------------------------
 * classificatio mutationis
 * -------------------------------------------------- */

interior b32
_lexemata_codicis_aequalia (
    constans SilvaDifferreLatus* a,
                            i32  ai,
    constans SilvaDifferreLatus* b,
                            i32  bi);

interior b32
_lexemata_codicis_aequalia (
    constans SilvaDifferreLatus* a,
                            i32  ai,
    constans SilvaDifferreLatus* b,
                            i32  bi)
{
    constans SilvaUnitas* ua = _unitas_ad(a->unitates, ai);
    constans SilvaUnitas* ub = _unitas_ad(b->unitates, bi);
                     i32  na   = ua->lexema_finis
                         - ua->lexema_primum;
                     i32 nb   = ub->lexema_finis
                         - ub->lexema_primum;
                     i32 k;

    si (na != nb)
    {
        redde FALSUM;
    }
    per (k = 0; k < na; k = k + 1)
    {
        SilvaToken* ta = _lexema_ad(a->lexemata,
            ua->lexema_primum + k);
        SilvaToken* tb = _lexema_ad(b->lexemata,
            ub->lexema_primum + k);

        si (   ta->genus != tb->genus
            || !chorda_aequalis(ta->valor, tb->valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_commenta_conserere (
    ChordaAedificator* aed,
                  Xar* spatia);

interior vacuum
_commenta_conserere (
    ChordaAedificator* aed,
                  Xar* spatia)
{
    i32 k;

    si (spatia == NIHIL)
    {
        redde;
    }
    per (k = 0; k < xar_numerus(spatia); k = k + 1)
    {
        SilvaToken* trivia = *(SilvaToken**)xar_obtinere(spatia,
            k);

        si (   trivia->genus == SILVA_LEX_COMMENTUM_CLAUSUM
            || trivia->genus == SILVA_LEX_COMMENTUM_LINEA)
        {
            chorda_aedificator_appendere_chorda(aed,
                trivia->valor);
        }
    }
}

interior chorda
_commenta_unitatis (
                        Piscina* piscina,
    constans SilvaDifferreLatus* l,
                            i32  k);

interior chorda
_commenta_unitatis (
                        Piscina* piscina,
    constans SilvaDifferreLatus* l,
                            i32  k)
{
    constans SilvaUnitas* u = _unitas_ad(l->unitates, k);
       ChordaAedificator* aed = chorda_aedificator_creare(piscina,
           (memoriae_index)64);
    i32 j;

    per (j = u->lexema_primum; j < u->lexema_finis; j = j + 1)
    {
        SilvaToken* t = _lexema_ad(l->lexemata, j);

        _commenta_conserere(aed, t->spatia_ante);
        _commenta_conserere(aed, t->spatia_post);
    }
    redde chorda_aedificator_finire(aed);
}

interior constans character*
_classificare (
                        Piscina* piscina,
    constans SilvaDifferreLatus* a,
                            i32  ai,
    constans SilvaDifferreLatus* b,
                            i32  bi);

interior constans character*
_classificare (
                        Piscina* piscina,
    constans SilvaDifferreLatus* a,
                            i32  ai,
    constans SilvaDifferreLatus* b,
                            i32  bi)
{
    si (!_lexemata_codicis_aequalia(a, ai, b, bi))
    {
        redde "substantiva";
    }
    si (!chorda_aequalis(_commenta_unitatis(piscina, a, ai),
        _commenta_unitatis(piscina, b, bi)))
    {
        redde "documentaria";
    }
    redde "cosmetica";
}

b32
silva_differre_symbolum_ex_textu (
                  Piscina* piscina,
                   chorda  textus,
       constans character* titulus,
    SilvaDifferreSymbolum* exitus)
{
                Piscina* temporaria;
    InternamentumChorda* intern_brevis;
     SilvaDifferreLatus  latus;
      ChordaAedificator* aed;
                    i32  k;

    exitus->inventa         = FALSUM;
    exitus->textus.datum    = NIHIL;
    exitus->textus.mensura  = 0;
    exitus->sigillum_hex    = exitus->textus;
    si (textus.datum == NIHIL || textus.mensura == 0)
    {
        redde VERUM;
    }
    temporaria = piscina_generare_dynamicum(
        "silva_differre_symbolum", 1048576);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    intern_brevis = internamentum_creare(temporaria);
    si (   intern_brevis == NIHIL
        || !silva_differre_latus_ex_textu(temporaria,
               intern_brevis, textus, "symbolum", &latus))
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)256);
    si (aed == NIHIL)
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    per (k = 0; k < latus.numerus; k = k + 1)
    {
        constans SilvaUnitas* u = (constans SilvaUnitas*)
            xar_obtinere(latus.unitates, k);

        si (chorda_aequalis_literis(u->titulus, titulus))
        {
            chorda_aedificator_appendere_chorda(aed,
                silva_differre_spatium(&latus, k));
            exitus->inventa = VERUM;
        }
    }
    piscina_destruere(temporaria);
    si (!exitus->inventa)
    {
        redde VERUM;
    }
    exitus->textus = chorda_aedificator_finire(aed);
    {
         Sigillum  sig;
        character  hex[SIGILLUM_HEX_MENSURA];
               i8* datum = (i8*)piscina_allocare(piscina,
                   (memoriae_index)64);

        si (datum == NIHIL)
        {
            redde FALSUM;
        }
        sig = sigillum_computare(
            (constans vacuum*)exitus->textus.datum,
            (memoriae_index)exitus->textus.mensura);
        sigillum_hex(&sig, hex);
        memcpy(datum, hex, (memoriae_index)64);
        exitus->sigillum_hex = chorda_ex_buffer(datum, 64);
    }
    redde VERUM;
}

constans character*
silva_differre_classificare_textus (
    Piscina* piscina,
     chorda  a,
     chorda  b)
{
    Xar* la = silva_lexare(piscina,
        (constans character*)a.datum, a.mensura, ZEPHYRUM);
    Xar* lb = silva_lexare(piscina,
        (constans character*)b.datum, b.mensura, ZEPHYRUM);
    i32 k;

    si (   la              == NIHIL || lb == NIHIL
        || xar_numerus(la) != xar_numerus(lb))
    {
        redde "substantiva";
    }
    per (k = 0; k < xar_numerus(la); k = k + 1)
    {
        SilvaToken* ta = _lexema_ad(la, k);
        SilvaToken* tb = _lexema_ad(lb, k);

        si (   ta->genus != tb->genus
            || !chorda_aequalis(ta->valor, tb->valor))
        {
            redde "substantiva";
        }
    }
    {
        ChordaAedificator* aa = chorda_aedificator_creare(piscina,
            (memoriae_index)64);
        ChordaAedificator* ab = chorda_aedificator_creare(piscina,
            (memoriae_index)64);

        si (aa == NIHIL || ab == NIHIL)
        {
            redde "substantiva";
        }
        per (k = 0; k < xar_numerus(la); k = k + 1)
        {
            _commenta_conserere(aa, _lexema_ad(la, k)->spatia_ante);
            _commenta_conserere(aa, _lexema_ad(la, k)->spatia_post);
        }
        per (k = 0; k < xar_numerus(lb); k = k + 1)
        {
            _commenta_conserere(ab, _lexema_ad(lb, k)->spatia_ante);
            _commenta_conserere(ab, _lexema_ad(lb, k)->spatia_post);
        }
        si (!chorda_aequalis(chorda_aedificator_finire(aa),
            chorda_aedificator_finire(ab)))
        {
            redde "documentaria";
        }
    }
    redde "cosmetica";
}

/* --------------------------------------------------
 * emissio
 * -------------------------------------------------- */

interior constans character*
_generis_titulus (
    constans SilvaUnitas* u);

interior constans character*
_generis_titulus (
    constans SilvaUnitas* u)
{
    si (u->est_directiva)
    {
        redde "directiva";
    }
    si (u->est_functio)
    {
        redde "functio";
    }
    si (u->est_typedef)
    {
        redde "typus";
    }
    si (u->est_tag_def)
    {
        redde "tag";
    }
    redde "datum";
}

vacuum
silva_differre_titulum_imprimere (
    chorda titulus)
{
    si (titulus.mensura == 0)
    {
        imprimere("(sine titulo)");
        redde;
    }
    imprimere("%.*s", (integer)titulus.mensura,
        (constans character*)titulus.datum);
}

DifferentiaSumma
silva_differre_summa_textuum (
    Piscina* piscina,
     chorda  vetus,
     chorda  novum)
{
    DifferentiaLinearum* d = differentia_linearum(piscina, vetus,
        novum);
    DifferentiaSumma vacua;

    si (d == NIHIL)
    {
        vacua.additae = 0;
        vacua.deletae = 0;
        redde vacua;
    }
    redde differentia_summa(d->tractus);
}

constans character*
silva_differre_titulum_cstr (
               Piscina* piscina,
    constans character* praefixum,
                chorda  titulus)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)64);

    chorda_aedificator_appendere_literis(aed, praefixum);
    si (titulus.mensura == 0)
    {
        chorda_aedificator_appendere_literis(aed, "(sine titulo)");
    }
    alioquin
    {
        chorda_aedificator_appendere_chorda(aed, titulus);
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

/* --------------------------------------------------
 * paria + emissio
 * -------------------------------------------------- */

/* ordinalem in listam clavis appendere (lista creatur si abest).
 * Valor tabulae = Xar* ordinalium - copiae geminae (bracchia
 * #ifdef) singulae reperibiles manent (olim ordinalis PRIMUS
 * solus servabatur - copia secunda irreperibilis, 01M0D4RN3B). */
interior b32
_ordinalem_registrare (
           Piscina* piscina,
    TabulaDispersa* tabula,
            chorda  clavis,
               i32  ordinis);

interior b32
_ordinalem_registrare (
           Piscina* piscina,
    TabulaDispersa* tabula,
            chorda  clavis,
               i32  ordinis)
{
    vacuum* inventum = NIHIL;
       Xar* ordines;
       i32* cella;

    si (tabula_dispersa_invenire(tabula, clavis, &inventum))
    {
        ordines = (Xar*)inventum;
    }
    alioquin
    {
        ordines = xar_creare(piscina, (i32)magnitudo(i32));
        si (ordines == NIHIL)
        {
            redde FALSUM;
        }
        tabula_dispersa_inserere(tabula, clavis,
            (vacuum*)ordines);
    }
    cella = (i32*)xar_addere(ordines);
    si (cella == NIHIL)
    {
        redde FALSUM;
    }
    *cella = ordinis;
    redde VERUM;
}

/* ordinalis primus NON sumptus in lista clavis; -1 = nullus */
interior s32
_ordinalis_liber (
    TabulaDispersa* tabula,
            chorda  clavis,
      constans i32* sumptae);

interior s32
_ordinalis_liber (
    TabulaDispersa* tabula,
            chorda  clavis,
      constans i32* sumptae)
{
    vacuum* inventum = NIHIL;
       Xar* ordines;
       i32  k;

    si (!tabula_dispersa_invenire(tabula, clavis, &inventum))
    {
        redde -I;
    }
    ordines = (Xar*)inventum;
    per (k = 0; k < xar_numerus(ordines); k = k + 1)
    {
        i32 ordinis = *(i32*)xar_obtinere(ordines, k);

        si (!sumptae[ordinis])
        {
            redde (s32)ordinis;
        }
    }
    redde -I;
}

Xar*
silva_differre_paria (
                        Piscina* piscina,
    constans SilvaDifferreLatus* a,
    constans SilvaDifferreLatus* b,
                            i32* immotae_exitus)
{
               Xar* tractus;
               Xar* paria;
               Xar* remotae_ordo;
               Xar* additae_ordo;
    TabulaDispersa* sigilla_remotarum;
    TabulaDispersa* normata_remotarum;
    TabulaDispersa* tituli_remotarum;
               i32* sumptae;
               i32  k;

    tractus = differentia_seriei(piscina,
        (constans vacuum* constans*)a->identitates, a->numerus,
        (constans vacuum* constans*)b->identitates, b->numerus);
    si (tractus == NIHIL)
    {
        redde NIHIL;
    }
    paria = xar_creare(piscina, (i32)magnitudo(SilvaDifferrePar));
    remotae_ordo = xar_creare(piscina, (i32)magnitudo(i32));
    additae_ordo = xar_creare(piscina, (i32)magnitudo(i32));
    sigilla_remotarum = tabula_dispersa_creare_chorda(piscina, 32);
    normata_remotarum = tabula_dispersa_creare_chorda(piscina, 32);
    tituli_remotarum = tabula_dispersa_creare_chorda(piscina, 32);
    si (   paria             == NIHIL || remotae_ordo == NIHIL
        || additae_ordo      == NIHIL || sigilla_remotarum == NIHIL
        || normata_remotarum == NIHIL
        || tituli_remotarum  == NIHIL)
    {
        redde NIHIL;
    }
    per (k = 0; k < xar_numerus(tractus); k = k + 1)
    {
        DifferentiaTractus* t = (DifferentiaTractus*)
            xar_obtinere(tractus, k);
        i32 l;

        si (t->genus == DIFFERENTIA_IDEM)
        {
            *immotae_exitus = *immotae_exitus + t->numerus;
        }
        alioquin si (t->genus == DIFFERENTIA_DELETA)
        {
            per (l = 0; l < t->numerus; l = l + 1)
            {
                i32* cella = (i32*)xar_addere(remotae_ordo);

                si (cella == NIHIL)
                {
                    redde NIHIL;
                }
                *cella = t->index_a + l;
            }
        }
        alioquin
        {
            per (l = 0; l < t->numerus; l = l + 1)
            {
                i32* cella = (i32*)xar_addere(additae_ordo);

                si (cella == NIHIL)
                {
                    redde NIHIL;
                }
                *cella = t->index_b + l;
            }
        }
    }

    /* indices remotarum: sigillo crudo (MOTA), sigillo normato
     * et titulo (MUTATA) - listae ordinalium per clavem, primus
     * non sumptus vincit */
    sumptae = (i32*)piscina_allocare(piscina,
        (memoriae_index)(xar_numerus(remotae_ordo) == 0 ? I
            : xar_numerus(remotae_ordo)) * magnitudo(i32));
    si (sumptae == NIHIL)
    {
        redde NIHIL;
    }
    per (k = 0; k < xar_numerus(remotae_ordo); k = k + 1)
    {
           i32 ai           = *(i32*)xar_obtinere(remotae_ordo, k);
        chorda sigillum_ai  = *(chorda*)a->identitates[ai];
        chorda normatum_ai =
            *(chorda*)a->identitates_normatae[ai];
        constans SilvaUnitas* ua = _unitas_ad(a->unitates, ai);

        sumptae[k] = 0;
        si (   !_ordinalem_registrare(piscina, sigilla_remotarum,
                sigillum_ai, k)
            || !_ordinalem_registrare(piscina, normata_remotarum,
                   normatum_ai, k))
        {
            redde NIHIL;
        }
        si (   ua->titulus.mensura > 0
            && !_ordinalem_registrare(piscina, tituli_remotarum,
                   ua->titulus, k))
        {
            redde NIHIL;
        }
    }

    /* additae: MOTA (sigillum crudum idem) > MUTATA (sigillum
     * normatum idem - reformatio spatialis, etiam sine titulo) >
     * MUTATA (titulus idem) > ADDITA */
    per (k = 0; k < xar_numerus(additae_ordo); k = k + 1)
    {
           i32 bi           = *(i32*)xar_obtinere(additae_ordo, k);
        chorda sigillum_bi  = *(chorda*)b->identitates[bi];
        chorda normatum_bi =
            *(chorda*)b->identitates_normatae[bi];
        constans SilvaUnitas* ub = _unitas_ad(b->unitates, bi);
                         s32  ordinis;
            SilvaDifferrePar* par;

        par = (SilvaDifferrePar*)xar_addere(paria);
        si (par == NIHIL)
        {
            redde NIHIL;
        }
        par->b_index = (s32)bi;
        par->a_index = -1;
        ordinis = _ordinalis_liber(sigilla_remotarum, sigillum_bi,
            sumptae);
        si (ordinis >= 0)
        {
            sumptae[ordinis] = 1;
            par->a_index = (s32)*(i32*)xar_obtinere(remotae_ordo,
                (i32)ordinis);
            par->status = "MOTA";
            perge;
        }
        ordinis = _ordinalis_liber(normata_remotarum, normatum_bi,
            sumptae);
        si (ordinis >= 0)
        {
            sumptae[ordinis] = 1;
            par->a_index = (s32)*(i32*)xar_obtinere(remotae_ordo,
                (i32)ordinis);
            par->status = "MUTATA";
            perge;
        }
        si (ub->titulus.mensura > 0)
        {
            ordinis = _ordinalis_liber(tituli_remotarum,
                ub->titulus, sumptae);
            si (ordinis >= 0)
            {
                sumptae[ordinis] = 1;
                par->a_index = (s32)*(i32*)xar_obtinere(
                    remotae_ordo, (i32)ordinis);
                par->status = "MUTATA";
                perge;
            }
        }
        par->status = "ADDITA";
    }

    /* remotae non sumptae */
    per (k = 0; k < xar_numerus(remotae_ordo); k = k + 1)
    {
        si (!sumptae[k])
        {
            SilvaDifferrePar* par = (SilvaDifferrePar*)
                xar_addere(paria);

            si (par == NIHIL)
            {
                redde NIHIL;
            }
            par->status = "REMOTA";
            par->a_index = (s32)*(i32*)xar_obtinere(
                remotae_ordo, k);
            par->b_index = -1;
        }
    }
    redde paria;
}

vacuum
silva_differre_paria_emittere (
                        Piscina* piscina,
    constans SilvaDifferreLatus* a,
    constans SilvaDifferreLatus* b,
                            Xar* paria,
                            b32  machina,
             constans character* via_machina,
                            b32  corpora,
                            i32* additae_totae,
                            i32* deletae_totae)
{
    i32 k;

    per (k = 0; k < xar_numerus(paria); k = k + 1)
    {
        SilvaDifferrePar* par = (SilvaDifferrePar*)
            xar_obtinere(paria, k);
        constans SilvaUnitas* u = par->b_index >= 0
            ? _unitas_ad(b->unitates, (i32)par->b_index)
            : _unitas_ad(a->unitates, (i32)par->a_index);
        constans character* classificatio = "-";
          DifferentiaSumma  summa;
                    chorda  spatium_a;
                    chorda  spatium_b;

        spatium_a.datum    = NIHIL;
        spatium_a.mensura  = 0;
        spatium_b          = spatium_a;
        si (par->a_index >= 0)
        {
            spatium_a = silva_differre_spatium(a,
                (i32)par->a_index);
        }
        si (par->b_index >= 0)
        {
            spatium_b = silva_differre_spatium(b,
                (i32)par->b_index);
        }
        si (strcmp(par->status, "MOTA") == 0)
        {
            summa.additae = 0;
            summa.deletae = 0;
        }
        alioquin
        {
            summa = silva_differre_summa_textuum(piscina,
                spatium_a, spatium_b);
        }
        si (strcmp(par->status, "MUTATA") == 0)
        {
            classificatio = _classificare(piscina, a,
                (i32)par->a_index, b, (i32)par->b_index);
        }
        *additae_totae = *additae_totae + (i32)summa.additae;
        *deletae_totae = *deletae_totae + (i32)summa.deletae;

        si (machina)
        {
            si (via_machina != NIHIL)
            {
                imprimere("%s\t", via_machina);
            }
            imprimere("%s\t", _generis_titulus(u));
            silva_differre_titulum_imprimere(u->titulus);
            imprimere("\t%s\t%s\t+%d\t-%d\n", par->status,
                classificatio, (integer)summa.additae,
                (integer)summa.deletae);
            perge;
        }

        imprimere("%s %s ", par->status, _generis_titulus(u));
        silva_differre_titulum_imprimere(u->titulus);
        si (strcmp(par->status, "MOTA") == 0)
        {
            imprimere("  (contentum idem, sede nova)\n");
            perge;
        }
        si (strcmp(par->status, "MUTATA") == 0)
        {
            imprimere("  [%s]  +%d -%d\n", classificatio,
                (integer)summa.additae, (integer)summa.deletae);
            /* textus solum ubi codex aut commenta mutata - spatia
             * alba lineis saepe invisibilia essent */
            si (corpora)
            {
                DifferentiaLinearum* d = differentia_linearum(
                    piscina, spatium_a, spatium_b);

                si (d != NIHIL)
                {
                    chorda textus = differentia_unificata(piscina,
                        d,
                        silva_differre_titulum_cstr(piscina, "a/",
                            u->titulus),
                        silva_differre_titulum_cstr(piscina, "b/",
                            u->titulus),
                        (i32)3);

                    imprimere("%.*s", (integer)textus.mensura,
                        (constans character*)textus.datum);
                }
            }
            perge;
        }
        imprimere("  +%d -%d\n", (integer)summa.additae,
            (integer)summa.deletae);
    }
}
