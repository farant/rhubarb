/* differre.c - differentia semantica unitatum C89 (instrumenta)
 *
 * Quaestio: QUID mutatum est inter duas versiones plagulae C89 -
 * in unitatibus quibus cogitas (functiones, typi, macra, data),
 * non in lineis nudis. Lexemate fundatum (silva_lexare +
 * silva_unitates): etiam plagulae quae non parsantur differuntur.
 *
 * Algorithmus:
 *   1. lexare ambas, unitates scandere, spatia byte PARTIRI:
 *      initium unitatis = trivia praecedentia lexematis primi
 *      (commentarium ducens ad unitatem sequentem pertinet, regula
 *      attachmenti lexatoris); primum ab 0, ultimum ad finem -
 *      omnis byte unitati uni pertinet.
 *   2. sigillum spatii cuiusque, identitates internatae ->
 *      differentia_seriei super UNITATES: nucleus idem qui lineas
 *      differt, granularitate tertia (unitas / linea / lexema).
 *   3. IDEM = immotae (tacent). DELETA+ADDITA sigillo aequali =
 *      MOTA (contentum idem, sede alia - quod differentia linearum
 *      structuraliter caeca est); titulo aequali = MUTATA; ceterae
 *      REMOTA / ADDITA.
 *   4. MUTATA classificantur: substantiva (lexemata codicis
 *      differunt) / documentaria (commenta sola differunt) /
 *      cosmetica (spatia alba sola). Textus = differentia linearum
 *      spatii ipsius (contextus III) - homo lineas legit, lexemata
 *      classificationi serviunt.
 *
 * Usus:   ./silva/differre.sh <vetus.c> <novum.c> [-machina]
 *         ./silva/differre.sh -git <via> [ref_vetus] [ref_novum]
 *           modus git NATIVUS (bibliotheca git, non subprocessus):
 *           ref_vetus ordinarius CAPUT; ref_novum absens = discus.
 *           Plagula ad ref absens = latus vacuum (honestum).
 *         ./silva/differre.sh -commissum [ref | vetus novus]
 *           commissum TOTUM in symbolis ('git log --stat' gradu
 *           unitatum): sine refs = CAPUT contra parentem; ref una
 *           = ea contra parentem; duae = arbor contra arborem.
 *           Plagulae C unitatim (ordines soli, sine corporibus -
 *           corpus petis per '-git <via> <vetus> <novus>');
 *           ceterae summa linearum; binariae notatae.
 * Exitus: 0 = cucurrit (differentia fractura NON est) |
 *         2 = usus malus aut plagula illegibilis
 * -machina: TSV genus, titulus, status, classificatio, +N, -M
 *           (sine textu; par fistulis - nexus.sh, vocantes);
 *           modo -commissum columna VIae praefigitur et plagulae
 *           non-C ordines 'plagula' accipiunt
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "sigillum.h"
#include "filum.h"
#include "differentia.h"
#include "git.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_unitates.h"

#include <stdio.h>
#include <string.h>

/* --------------------------------------------------
 * latus unum: plagula lexata cum partitione unitatum
 * -------------------------------------------------- */

nomen structura {
    chorda   textus;
    Xar*     lexemata;
    Xar*     unitates;     /* SilvaUnitas */
    i32*     initia;       /* byte initium unitatis k */
    i32*     fines;        /* byte finis (exclusivum) */
    vacuum** identitates;  /* sigilla internata (chorda*) */
    i32      numerus;
} DifferreLatus;

interior SilvaToken*
_lexema_ad (Xar* lexemata, i32 i);

interior SilvaToken*
_lexema_ad (Xar* lexemata, i32 i)
{
    redde *(SilvaToken**)xar_obtinere(lexemata, i);
}

interior SilvaUnitas*
_unitas_ad (Xar* unitates, i32 i);

interior SilvaUnitas*
_unitas_ad (Xar* unitates, i32 i)
{
    redde (SilvaUnitas*)xar_obtinere(unitates, i);
}

/* initium byte unitatis: trivia praecedentia lexematis primi (si
 * adsunt), alioquin lexema ipsum */
interior i32
_unitatis_initium (Xar* lexemata, constans SilvaUnitas* u);

interior i32
_unitatis_initium (Xar* lexemata, constans SilvaUnitas* u)
{
    SilvaToken* primum = _lexema_ad(lexemata, u->lexema_primum);

    si (primum->spatia_ante != NIHIL
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

interior b32
_latus_ex_textu (Piscina* piscina, InternamentumChorda* intern,
    chorda textus, constans character* titulus, DifferreLatus* latus);

interior b32
_latus_ex_textu (Piscina* piscina, InternamentumChorda* intern,
    chorda textus, constans character* titulus, DifferreLatus* latus)
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
    si (latus->initia == NIHIL || latus->fines == NIHIL
        || latus->identitates == NIHIL)
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
        character hex[SIGILLUM_HEX_MENSURA];
        chorda    hex_ch;
        chorda*   internata;

        sig = sigillum_computare(
            (constans vacuum*)(latus->textus.datum
                + latus->initia[k]),
            (memoriae_index)(latus->fines[k] - latus->initia[k]));
        sigillum_hex(&sig, hex);
        hex_ch = chorda_ex_buffer((i8*)hex, 64);
        internata = chorda_internare(intern, hex_ch);
        si (internata == NIHIL)
        {
            redde FALSUM;
        }
        latus->identitates[k] = (vacuum*)internata;
    }
    redde VERUM;
}

interior b32
_latus_parare (Piscina* piscina, InternamentumChorda* intern,
    constans character* via, DifferreLatus* latus);

interior b32
_latus_parare (Piscina* piscina, InternamentumChorda* intern,
    constans character* via, DifferreLatus* latus)
{
    si (!filum_existit(via))
    {
        fprintf(stderr, "differre: plagula non exsistit: %s\n",
            via);
        redde FALSUM;
    }
    redde _latus_ex_textu(piscina, intern,
        filum_legere_totum(via, piscina), via, latus);
}

interior chorda
_spatium (constans DifferreLatus* latus, i32 k);

interior chorda
_spatium (constans DifferreLatus* latus, i32 k)
{
    redde chorda_ex_buffer(latus->textus.datum + latus->initia[k],
        latus->fines[k] - latus->initia[k]);
}

/* --------------------------------------------------
 * classificatio mutationis
 * -------------------------------------------------- */

interior b32
_lexemata_codicis_aequalia (constans DifferreLatus* a, i32 ai,
    constans DifferreLatus* b, i32 bi);

interior b32
_lexemata_codicis_aequalia (constans DifferreLatus* a, i32 ai,
    constans DifferreLatus* b, i32 bi)
{
    constans SilvaUnitas* ua = _unitas_ad(a->unitates, ai);
    constans SilvaUnitas* ub = _unitas_ad(b->unitates, bi);
    i32 na = ua->lexema_finis - ua->lexema_primum;
    i32 nb = ub->lexema_finis - ub->lexema_primum;
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

        si (ta->genus != tb->genus
            || !chorda_aequalis(ta->valor, tb->valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_commenta_conserere (ChordaAedificator* aed, Xar* spatia);

interior vacuum
_commenta_conserere (ChordaAedificator* aed, Xar* spatia)
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

        si (trivia->genus == SILVA_LEX_COMMENTUM_CLAUSUM
            || trivia->genus == SILVA_LEX_COMMENTUM_LINEA)
        {
            chorda_aedificator_appendere_chorda(aed,
                trivia->valor);
        }
    }
}

interior chorda
_commenta_unitatis (Piscina* piscina, constans DifferreLatus* l,
    i32 k);

interior chorda
_commenta_unitatis (Piscina* piscina, constans DifferreLatus* l,
    i32 k)
{
    constans SilvaUnitas* u = _unitas_ad(l->unitates, k);
    ChordaAedificator*    aed = chorda_aedificator_creare(piscina,
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
_classificare (Piscina* piscina, constans DifferreLatus* a, i32 ai,
    constans DifferreLatus* b, i32 bi);

interior constans character*
_classificare (Piscina* piscina, constans DifferreLatus* a, i32 ai,
    constans DifferreLatus* b, i32 bi)
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

/* --------------------------------------------------
 * emissio
 * -------------------------------------------------- */

interior constans character*
_generis_titulus (constans SilvaUnitas* u);

interior constans character*
_generis_titulus (constans SilvaUnitas* u)
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

interior vacuum
_titulum_imprimere (chorda titulus);

interior vacuum
_titulum_imprimere (chorda titulus)
{
    si (titulus.mensura == 0)
    {
        imprimere("(sine titulo)");
        redde;
    }
    imprimere("%.*s", (integer)titulus.mensura,
        (constans character*)titulus.datum);
}

interior DifferentiaSumma
_summa_spatii (Piscina* piscina, chorda vetus, chorda novum);

interior DifferentiaSumma
_summa_spatii (Piscina* piscina, chorda vetus, chorda novum)
{
    DifferentiaLinearum* d = differentia_linearum(piscina, vetus,
        novum);
    DifferentiaSumma     vacua;

    si (d == NIHIL)
    {
        vacua.additae = 0;
        vacua.deletae = 0;
        redde vacua;
    }
    redde differentia_summa(d->tractus);
}

interior constans character*
_titulum_cstr (Piscina* piscina, constans character* praefixum,
    chorda titulus);

interior constans character*
_titulum_cstr (Piscina* piscina, constans character* praefixum,
    chorda titulus)
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

/* par status - a_index/b_index: -1 = latus vacat */
nomen structura {
    constans character* status;
    s32                 a_index;
    s32                 b_index;
} DifferrePar;

/* differentia unitatum -> paria statuum (MOTA sigillo, MUTATA
 * titulo, ceterae ADDITA/REMOTA); *immotae_exitus accumulat;
 * NIHIL = defectus */
interior Xar*
_paria_computare (Piscina* piscina, constans DifferreLatus* a,
    constans DifferreLatus* b, i32* immotae_exitus);

interior Xar*
_paria_computare (Piscina* piscina, constans DifferreLatus* a,
    constans DifferreLatus* b, i32* immotae_exitus)
{
    Xar* tractus;
    Xar* paria;
    Xar* remotae_ordo;
    Xar* additae_ordo;
    TabulaDispersa* sigilla_remotarum;
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
    paria = xar_creare(piscina, (i32)magnitudo(DifferrePar));
    remotae_ordo = xar_creare(piscina, (i32)magnitudo(i32));
    additae_ordo = xar_creare(piscina, (i32)magnitudo(i32));
    sigilla_remotarum = tabula_dispersa_creare_chorda(piscina, 32);
    tituli_remotarum = tabula_dispersa_creare_chorda(piscina, 32);
    si (paria == NIHIL || remotae_ordo == NIHIL
        || additae_ordo == NIHIL || sigilla_remotarum == NIHIL
        || tituli_remotarum == NIHIL)
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

    /* indices remotarum: sigillo (MOTA) et titulo (MUTATA) -
     * prima non sumpta vincit */
    sumptae = (i32*)piscina_allocare(piscina,
        (memoriae_index)(xar_numerus(remotae_ordo) == 0 ? I
            : xar_numerus(remotae_ordo)) * magnitudo(i32));
    si (sumptae == NIHIL)
    {
        redde NIHIL;
    }
    per (k = 0; k < xar_numerus(remotae_ordo); k = k + 1)
    {
        i32    ai = *(i32*)xar_obtinere(remotae_ordo, k);
        chorda sigillum_ai = *(chorda*)a->identitates[ai];
        constans SilvaUnitas* ua = _unitas_ad(a->unitates, ai);

        sumptae[k] = 0;
        /* valor tabulae = ORDINALIS k in cella propria (Xar
         * segmentatum est - arithmetica trans elementa
         * vetita) */
        si (!tabula_dispersa_continet(sigilla_remotarum,
            sigillum_ai))
        {
            i32* cella = (i32*)piscina_allocare(piscina,
                (memoriae_index)magnitudo(i32));

            si (cella == NIHIL)
            {
                redde NIHIL;
            }
            *cella = k;
            tabula_dispersa_inserere(sigilla_remotarum,
                sigillum_ai, (vacuum*)cella);
        }
        si (ua->titulus.mensura > 0
            && !tabula_dispersa_continet(tituli_remotarum,
                   ua->titulus))
        {
            i32* cella = (i32*)piscina_allocare(piscina,
                (memoriae_index)magnitudo(i32));

            si (cella == NIHIL)
            {
                redde NIHIL;
            }
            *cella = k;
            tabula_dispersa_inserere(tituli_remotarum,
                ua->titulus, (vacuum*)cella);
        }
    }

    /* additae: MOTA (sigillum idem) > MUTATA (titulus idem) >
     * ADDITA */
    per (k = 0; k < xar_numerus(additae_ordo); k = k + 1)
    {
        i32     bi = *(i32*)xar_obtinere(additae_ordo, k);
        chorda  sigillum_bi = *(chorda*)b->identitates[bi];
        constans SilvaUnitas* ub = _unitas_ad(b->unitates, bi);
        vacuum* inventum = NIHIL;
        DifferrePar* par;

        par = (DifferrePar*)xar_addere(paria);
        si (par == NIHIL)
        {
            redde NIHIL;
        }
        par->b_index = (s32)bi;
        par->a_index = -1;
        si (tabula_dispersa_invenire(sigilla_remotarum,
            sigillum_bi, &inventum))
        {
            i32 ordinis = *(i32*)inventum;

            si (!sumptae[ordinis])
            {
                sumptae[ordinis] = 1;
                par->a_index = (s32)*(i32*)xar_obtinere(
                    remotae_ordo, ordinis);
                par->status = "MOTA";
                perge;
            }
        }
        si (ub->titulus.mensura > 0
            && tabula_dispersa_invenire(tituli_remotarum,
                   ub->titulus, &inventum))
        {
            i32 ordinis = *(i32*)inventum;

            si (!sumptae[ordinis])
            {
                sumptae[ordinis] = 1;
                par->a_index = (s32)*(i32*)xar_obtinere(
                    remotae_ordo, ordinis);
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
            DifferrePar* par = (DifferrePar*)xar_addere(paria);

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

/* emissio parium: ordines (-machina TSV aut lectio humana);
 * via_machina non NIHIL = columna viae praefixa (modus
 * -commissum); corpora = textus unificatus sub MUTATA */
interior vacuum
_paria_emittere (Piscina* piscina, constans DifferreLatus* a,
    constans DifferreLatus* b, Xar* paria, b32 machina,
    constans character* via_machina, b32 corpora,
    i32* additae_totae, i32* deletae_totae);

interior vacuum
_paria_emittere (Piscina* piscina, constans DifferreLatus* a,
    constans DifferreLatus* b, Xar* paria, b32 machina,
    constans character* via_machina, b32 corpora,
    i32* additae_totae, i32* deletae_totae)
{
    i32 k;

    per (k = 0; k < xar_numerus(paria); k = k + 1)
    {
        DifferrePar* par = (DifferrePar*)xar_obtinere(paria, k);
        constans SilvaUnitas* u = par->b_index >= 0
            ? _unitas_ad(b->unitates, (i32)par->b_index)
            : _unitas_ad(a->unitates, (i32)par->a_index);
        constans character* classificatio = "-";
        DifferentiaSumma    summa;
        chorda              spatium_a;
        chorda              spatium_b;

        spatium_a.datum = NIHIL;
        spatium_a.mensura = 0;
        spatium_b = spatium_a;
        si (par->a_index >= 0)
        {
            spatium_a = _spatium(a, (i32)par->a_index);
        }
        si (par->b_index >= 0)
        {
            spatium_b = _spatium(b, (i32)par->b_index);
        }
        si (strcmp(par->status, "MOTA") == 0)
        {
            summa.additae = 0;
            summa.deletae = 0;
        }
        alioquin
        {
            summa = _summa_spatii(piscina, spatium_a, spatium_b);
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
            _titulum_imprimere(u->titulus);
            imprimere("\t%s\t%s\t+%d\t-%d\n", par->status,
                classificatio, (integer)summa.additae,
                (integer)summa.deletae);
            perge;
        }

        imprimere("%s %s ", par->status, _generis_titulus(u));
        _titulum_imprimere(u->titulus);
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
                        _titulum_cstr(piscina, "a/", u->titulus),
                        _titulum_cstr(piscina, "b/", u->titulus),
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

interior b32
_via_est_c (chorda via);

interior b32
_via_est_c (chorda via)
{
    character ultima;

    si (via.mensura < 2 || via.datum[via.mensura - 2] != '.')
    {
        redde FALSUM;
    }
    ultima = (character)via.datum[via.mensura - 1];
    redde ultima == 'c' || ultima == 'h';
}

/* massa e git per sha (chorda hex 40); vacua si sha vacua aut
 * obiectum non massa */
interior chorda
_massam_ex_git (GitRepositorium* repositorium, chorda sha,
    Piscina* piscina);

interior chorda
_massam_ex_git (GitRepositorium* repositorium, chorda sha,
    Piscina* piscina)
{
    chorda      vacua;
    GitObiectum obiectum;

    vacua.datum = NIHIL;
    vacua.mensura = 0;
    si (sha.mensura != 40)
    {
        redde vacua;
    }
    obiectum = git_obiectum_legere(repositorium,
        chorda_ut_cstr(sha, piscina), piscina);
    si (!obiectum.successus
        || obiectum.genus != GIT_OBIECTUM_MASSA)
    {
        redde vacua;
    }
    redde obiectum.datum;
}

interior b32
_est_binaria (chorda textus);

interior b32
_est_binaria (chorda textus)
{
    redde textus.mensura > 0
        && memchr(textus.datum, 0,
               (memoriae_index)textus.mensura) != NIHIL;
}

s32 principale (integer argc, character** argv)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    DifferreLatus        a;
    DifferreLatus        b;
    constans character*  via_a = NIHIL;
    constans character*  via_b = NIHIL;
    constans character*  positio_tertia = NIHIL;
    b32                  machina = FALSUM;
    b32                  git_modus = FALSUM;
    b32                  commissum_modus = FALSUM;
    Xar*                 paria;
    i32                  immotae = 0;
    i32                  additae_totae = 0;
    i32                  deletae_totae = 0;
    i32                  k;
    integer              arg;

    per (arg = 1; arg < argc; arg = arg + 1)
    {
        si (strcmp(argv[arg], "-machina") == 0)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[arg], "-git") == 0)
        {
            git_modus = VERUM;
        }
        alioquin si (strcmp(argv[arg], "-commissum") == 0)
        {
            commissum_modus = VERUM;
        }
        alioquin si (via_a == NIHIL)
        {
            via_a = argv[arg];
        }
        alioquin si (via_b == NIHIL)
        {
            via_b = argv[arg];
        }
        alioquin si (positio_tertia == NIHIL)
        {
            positio_tertia = argv[arg];
        }
    }
    si ((git_modus && commissum_modus)
        || (!commissum_modus
               && (via_a == NIHIL || (!git_modus && via_b == NIHIL))))
    {
        fprintf(stderr, "usus: differre <vetus.c> <novum.c>"
            " [-machina]\n"
            "      differre -git <via> [ref_vetus] [ref_novum]"
            " [-machina]\n"
            "      differre -commissum [ref | vetus novus]"
            " [-machina]\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("differre", 1048576);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "differre: piscina generari non potuit\n");
        redde II;
    }
    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde II;
    }
    si (commissum_modus)
    {
        /* commissum TOTUM: arbores differuntur (shas subarborum -
         * contentum tangitur solum ubi mutatum), plagulae C
         * unitatim, ceterae summa linearum */
        GitRepositorium* repositorium = git_aperire(piscina, ".");
        character        sha_veteris[GIT_SHA_HEX_MENSURA];
        character        sha_novi[GIT_SHA_HEX_MENSURA];
        GitCommissum     vetus_commissum;
        GitCommissum     novum_commissum;
        Xar*             mutatae;
        i32              paria_totae = 0;

        si (repositorium == NIHIL)
        {
            fprintf(stderr, "differre: non in repositorio git\n");
            redde II;
        }
        si (via_a != NIHIL && via_b != NIHIL)
        {
            /* refs duae: arbor contra arborem */
            si (!git_ref_resolvere(repositorium, via_a,
                sha_veteris))
            {
                fprintf(stderr, "differre: ref non resolutum:"
                    " %s\n", via_a);
                redde II;
            }
            si (!git_ref_resolvere(repositorium, via_b, sha_novi))
            {
                fprintf(stderr, "differre: ref non resolutum:"
                    " %s\n", via_b);
                redde II;
            }
        }
        alioquin
        {
            /* nulla aut una: commissum contra parentem primum */
            constans character* ref = via_a != NIHIL ? via_a
                : "HEAD";

            si (!git_ref_resolvere(repositorium, ref, sha_novi))
            {
                fprintf(stderr, "differre: ref non resolutum:"
                    " %s\n", ref);
                redde II;
            }
            novum_commissum = git_commissum_legere(repositorium,
                sha_novi, piscina);
            si (!novum_commissum.successus
                || xar_numerus(novum_commissum.parentes) == 0)
            {
                fprintf(stderr, "differre: commissum sine"
                    " parente: %s\n", ref);
                redde II;
            }
            memcpy(sha_veteris, chorda_ut_cstr(
                *(chorda*)xar_obtinere(novum_commissum.parentes,
                    0), piscina), (memoriae_index)41);
        }
        vetus_commissum = git_commissum_legere(repositorium,
            sha_veteris, piscina);
        novum_commissum = git_commissum_legere(repositorium,
            sha_novi, piscina);
        si (!vetus_commissum.successus
            || !novum_commissum.successus)
        {
            fprintf(stderr, "differre: commissum legi non"
                " potuit\n");
            redde II;
        }
        mutatae = git_arbores_differre(repositorium,
            chorda_ut_cstr(vetus_commissum.arbor, piscina),
            chorda_ut_cstr(novum_commissum.arbor, piscina),
            piscina);
        si (mutatae == NIHIL)
        {
            fprintf(stderr, "differre: arbores differri non"
                " potuerunt\n");
            redde II;
        }
        si (!machina)
        {
            imprimere("commissum %.7s contra %.7s: %d plagulae"
                " mutatae\n\n", sha_veteris, sha_novi,
                (integer)xar_numerus(mutatae));
        }
        per (k = 0; k < xar_numerus(mutatae); k = k + 1)
        {
            GitViaMutata* m = (GitViaMutata*)xar_obtinere(mutatae,
                k);
            constans character* via_cstr = chorda_ut_cstr(m->via,
                piscina);
            constans character* status_plagulae =
                m->genus == GIT_VIA_ADDITA ? "ADDITA"
                : m->genus == GIT_VIA_REMOTA ? "REMOTA"
                : "MUTATA";
            chorda textus_vetus = _massam_ex_git(repositorium,
                m->sha_vetus, piscina);
            chorda textus_novus = _massam_ex_git(repositorium,
                m->sha_novus, piscina);
            b32 binaria = _est_binaria(textus_vetus)
                || _est_binaria(textus_novus);

            si (_via_est_c(m->via) && !binaria)
            {
                DifferreLatus la;
                DifferreLatus lb;
                Xar*          paria_plagulae;

                si (!_latus_ex_textu(piscina, intern,
                        textus_vetus, via_cstr, &la)
                    || !_latus_ex_textu(piscina, intern,
                           textus_novus, via_cstr, &lb))
                {
                    redde II;
                }
                paria_plagulae = _paria_computare(piscina, &la,
                    &lb, &immotae);
                si (paria_plagulae == NIHIL)
                {
                    redde II;
                }
                si (!machina)
                {
                    imprimere("== %s  %s\n", via_cstr,
                        status_plagulae);
                }
                _paria_emittere(piscina, &la, &lb,
                    paria_plagulae, machina,
                    machina ? via_cstr : NIHIL, FALSUM,
                    &additae_totae, &deletae_totae);
                paria_totae = paria_totae
                    + xar_numerus(paria_plagulae);
                si (!machina)
                {
                    imprimere("\n");
                }
            }
            alioquin
            {
                DifferentiaSumma summa;

                summa.additae = 0;
                summa.deletae = 0;
                si (!binaria)
                {
                    summa = _summa_spatii(piscina, textus_vetus,
                        textus_novus);
                }
                additae_totae = additae_totae
                    + (i32)summa.additae;
                deletae_totae = deletae_totae
                    + (i32)summa.deletae;
                si (machina)
                {
                    imprimere(
                        "%s\tplagula\t-\t%s\t%s\t+%d\t-%d\n",
                        via_cstr, status_plagulae,
                        binaria ? "binaria" : "-",
                        (integer)summa.additae,
                        (integer)summa.deletae);
                }
                alioquin si (binaria)
                {
                    imprimere("== %s  %s  (binaria)\n\n",
                        via_cstr, status_plagulae);
                }
                alioquin
                {
                    imprimere("== %s  %s  +%d -%d\n\n", via_cstr,
                        status_plagulae, (integer)summa.additae,
                        (integer)summa.deletae);
                }
            }
        }
        si (!machina)
        {
            imprimere("differre: %d plagulae, %d paria unitatum"
                " (+%d -%d)\n", (integer)xar_numerus(mutatae),
                (integer)paria_totae, (integer)additae_totae,
                (integer)deletae_totae);
        }
        redde ZEPHYRUM;
    }
    si (git_modus)
    {
        /* latera e bibliotheca git NATIVA - nullus subprocessus */
        GitRepositorium*    repositorium = git_aperire(piscina,
            ".");
        character           sha[GIT_SHA_HEX_MENSURA];
        constans character* ref_vetus = via_b != NIHIL ? via_b
            : "HEAD";
        chorda              textus_vetus;
        chorda              textus_novum;
        b32                 inventum = FALSUM;

        si (repositorium == NIHIL)
        {
            fprintf(stderr, "differre: non in repositorio git\n");
            redde II;
        }
        si (!git_ref_resolvere(repositorium, ref_vetus, sha))
        {
            fprintf(stderr, "differre: ref non resolutum: %s\n",
                ref_vetus);
            redde II;
        }
        textus_vetus = git_massam_per_viam(repositorium, sha,
            via_a, piscina, &inventum);
        si (!inventum)
        {
            textus_vetus.datum = NIHIL;
            textus_vetus.mensura = 0;
        }
        si (positio_tertia != NIHIL)
        {
            si (!git_ref_resolvere(repositorium, positio_tertia,
                sha))
            {
                fprintf(stderr, "differre: ref non resolutum:"
                    " %s\n", positio_tertia);
                redde II;
            }
            inventum = FALSUM;
            textus_novum = git_massam_per_viam(repositorium, sha,
                via_a, piscina, &inventum);
            si (!inventum)
            {
                textus_novum.datum = NIHIL;
                textus_novum.mensura = 0;
            }
        }
        alioquin si (filum_existit(via_a))
        {
            textus_novum = filum_legere_totum(via_a, piscina);
        }
        alioquin
        {
            textus_novum.datum = NIHIL;
            textus_novum.mensura = 0;
        }
        si (!_latus_ex_textu(piscina, intern, textus_vetus, via_a,
                &a)
            || !_latus_ex_textu(piscina, intern, textus_novum,
                   via_a, &b))
        {
            redde II;
        }
    }
    alioquin si (!_latus_parare(piscina, intern, via_a, &a)
        || !_latus_parare(piscina, intern, via_b, &b))
    {
        redde II;
    }

    paria = _paria_computare(piscina, &a, &b, &immotae);
    si (paria == NIHIL)
    {
        fprintf(stderr, "differre: differentia computari non"
            " potuit\n");
        redde II;
    }

    /* emissio */
    si (xar_numerus(paria) == 0)
    {
        si (!machina)
        {
            imprimere("differre: nulla differentia (%d unitates"
                " immotae)\n", (integer)immotae);
        }
        redde ZEPHYRUM;
    }
    _paria_emittere(piscina, &a, &b, paria, machina, NIHIL, VERUM,
        &additae_totae, &deletae_totae);
    si (!machina)
    {
        imprimere("differre: %d paria (+%d -%d), %d unitates"
            " immotae\n", (integer)xar_numerus(paria),
            (integer)additae_totae, (integer)deletae_totae,
            (integer)immotae);
    }
    redde ZEPHYRUM;
}
