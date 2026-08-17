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
 * Exitus: 0 = cucurrit (differentia fractura NON est) |
 *         2 = usus malus aut plagula illegibilis
 * -machina: TSV genus, titulus, status, classificatio, +N, -M
 *           (sine textu; par fistulis - nexus.sh, vocantes)
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
_latus_parare (Piscina* piscina, InternamentumChorda* intern,
    constans character* via, DifferreLatus* latus);

interior b32
_latus_parare (Piscina* piscina, InternamentumChorda* intern,
    constans character* via, DifferreLatus* latus)
{
    i32 k;

    si (!filum_existit(via))
    {
        fprintf(stderr, "differre: plagula non exsistit: %s\n",
            via);
        redde FALSUM;
    }
    latus->textus = filum_legere_totum(via, piscina);
    latus->lexemata = silva_lexare(piscina,
        (constans character*)latus->textus.datum,
        latus->textus.mensura, ZEPHYRUM);
    si (latus->lexemata == NIHIL)
    {
        fprintf(stderr, "differre: lexari non potuit: %s\n", via);
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

s32 principale (integer argc, character** argv)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    DifferreLatus        a;
    DifferreLatus        b;
    constans character*  via_a = NIHIL;
    constans character*  via_b = NIHIL;
    b32                  machina = FALSUM;
    Xar*                 paria;
    Xar*                 tractus;
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
        alioquin si (via_a == NIHIL)
        {
            via_a = argv[arg];
        }
        alioquin si (via_b == NIHIL)
        {
            via_b = argv[arg];
        }
    }
    si (via_a == NIHIL || via_b == NIHIL)
    {
        fprintf(stderr, "usus: differre <vetus.c> <novum.c>"
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
    si (!_latus_parare(piscina, intern, via_a, &a)
        || !_latus_parare(piscina, intern, via_b, &b))
    {
        redde II;
    }

    tractus = differentia_seriei(piscina,
        (constans vacuum* constans*)a.identitates, a.numerus,
        (constans vacuum* constans*)b.identitates, b.numerus);
    si (tractus == NIHIL)
    {
        fprintf(stderr, "differre: differentia computari non"
            " potuit\n");
        redde II;
    }

    /* runs -> paria statuum */
    paria = xar_creare(piscina, (i32)magnitudo(DifferrePar));
    {
        Xar* remotae_ordo = xar_creare(piscina, (i32)magnitudo(i32));
        Xar* additae_ordo = xar_creare(piscina, (i32)magnitudo(i32));
        TabulaDispersa* sigilla_remotarum =
            tabula_dispersa_creare_chorda(piscina, 32);
        TabulaDispersa* tituli_remotarum =
            tabula_dispersa_creare_chorda(piscina, 32);
        i32* sumptae;

        si (paria == NIHIL || remotae_ordo == NIHIL
            || additae_ordo == NIHIL || sigilla_remotarum == NIHIL
            || tituli_remotarum == NIHIL)
        {
            redde II;
        }
        per (k = 0; k < xar_numerus(tractus); k = k + 1)
        {
            DifferentiaTractus* t = (DifferentiaTractus*)
                xar_obtinere(tractus, k);
            i32 l;

            si (t->genus == DIFFERENTIA_IDEM)
            {
                immotae = immotae + t->numerus;
            }
            alioquin si (t->genus == DIFFERENTIA_DELETA)
            {
                per (l = 0; l < t->numerus; l = l + 1)
                {
                    i32* cella = (i32*)xar_addere(remotae_ordo);

                    si (cella == NIHIL)
                    {
                        redde II;
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
                        redde II;
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
            redde II;
        }
        per (k = 0; k < xar_numerus(remotae_ordo); k = k + 1)
        {
            i32    ai = *(i32*)xar_obtinere(remotae_ordo, k);
            chorda sigillum_ai = *(chorda*)a.identitates[ai];
            constans SilvaUnitas* ua = _unitas_ad(a.unitates, ai);

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
                    redde II;
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
                    redde II;
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
            chorda  sigillum_bi = *(chorda*)b.identitates[bi];
            constans SilvaUnitas* ub = _unitas_ad(b.unitates, bi);
            vacuum* inventum = NIHIL;
            DifferrePar* par;

            par = (DifferrePar*)xar_addere(paria);
            si (par == NIHIL)
            {
                redde II;
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
                    redde II;
                }
                par->status = "REMOTA";
                par->a_index = (s32)*(i32*)xar_obtinere(
                    remotae_ordo, k);
                par->b_index = -1;
            }
        }
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
    per (k = 0; k < xar_numerus(paria); k = k + 1)
    {
        DifferrePar* par = (DifferrePar*)xar_obtinere(paria, k);
        constans SilvaUnitas* u = par->b_index >= 0
            ? _unitas_ad(b.unitates, (i32)par->b_index)
            : _unitas_ad(a.unitates, (i32)par->a_index);
        constans character* classificatio = "-";
        DifferentiaSumma    summa;
        chorda              spatium_a;
        chorda              spatium_b;

        spatium_a.datum = NIHIL;
        spatium_a.mensura = 0;
        spatium_b = spatium_a;
        si (par->a_index >= 0)
        {
            spatium_a = _spatium(&a, (i32)par->a_index);
        }
        si (par->b_index >= 0)
        {
            spatium_b = _spatium(&b, (i32)par->b_index);
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
            classificatio = _classificare(piscina, &a,
                (i32)par->a_index, &b, (i32)par->b_index);
        }
        additae_totae = additae_totae + (i32)summa.additae;
        deletae_totae = deletae_totae + (i32)summa.deletae;

        si (machina)
        {
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
    si (!machina)
    {
        imprimere("differre: %d paria (+%d -%d), %d unitates"
            " immotae\n", (integer)xar_numerus(paria),
            (integer)additae_totae, (integer)deletae_totae,
            (integer)immotae);
    }
    redde ZEPHYRUM;
}
