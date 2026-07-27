/* silva_expandere.c - Expansor silvae
 * Chunk A: acta + directivae + definitiones macro.
 * Chunk B: generationes expansionis (caecationes).
 * Chunk C: # et ## in substitutione.
 * Chunk D: includenda + regiones conditionales + custodes.
 */

#include "silva_expandere.h"
#include "silva_conditio.h"
#include "silva_lexema.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Auxiliares
 * ================================================== */

interior b32
_chorda_est_literis (chorda c, constans character* literis)
{
    i32 mensura;

    mensura = (i32)strlen(literis);
    si (c.mensura != mensura)
    {
        redde FALSUM;
    }
    si (mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(c.datum, literis, (memoriae_index)mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

interior chorda*
_chorda_figere (Piscina* piscina, chorda c)
{
    chorda* fixa;

    fixa = (chorda*)piscina_allocare(piscina, (memoriae_index)magnitudo(chorda));
    si (fixa != NIHIL)
    {
        *fixa = c;
    }
    redde fixa;
}


/* ==================================================
 * Contextus
 * ================================================== */

SilvaExpansio*
silva_expansio_creare (Piscina* piscina)
{
    SilvaExpansio* exp;

    exp = (SilvaExpansio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaExpansio));
    si (exp == NIHIL)
    {
        redde NIHIL;
    }
    exp->piscina = piscina;
    exp->fontes = xar_creare(piscina, magnitudo(SilvaFons));
    exp->macros = tabula_dispersa_creare_chorda(piscina, LXIV);
    exp->acta = xar_creare(piscina, magnitudo(SilvaEventum));
    exp->rami = xar_creare(piscina, magnitudo(SilvaRamus*));
    exp->regiones = xar_creare(piscina, magnitudo(SilvaRegio*));
    exp->includenda = tabula_dispersa_creare_chorda(piscina, XVI);
    exp->inclusiones = xar_creare(piscina, magnitudo(SilvaInclusio));
    exp->extenta = xar_creare(piscina,
        magnitudo(SilvaExtentumInvocationis));
    exp->profunditas_includendi = ZEPHYRUM;
    exp->fons_api = -I;
    exp->tabula_activa = NIHIL;
    exp->limen_lexematum = SILVA_LIMEN_LEXEMATUM_DEFALTUM;
    exp->limen_generationum = SILVA_LIMEN_GENERATIONUM_DEFALTUM;
    exp->limen_includendi = SILVA_LIMEN_INCLUDENDI_DEFALTUM;
    exp->limen_regionum = SILVA_LIMEN_REGIONUM_DEFALTUM;
    exp->pergere = NIHIL;
    exp->pergere_datum = NIHIL;
    exp->expansio_decisa = FALSUM;
    exp->est_intermissa = FALSUM;
    exp->fines_tactae = FALSUM;
    exp->profunditas_regionum = ZEPHYRUM;
    redde exp;
}

s32
silva_fons_addere (
    SilvaExpansio*      exp,
    constans character* via,
    b32                 est_syntheticus)
{
    SilvaFons* locus;
    chorda* via_fixa;
    chorda temp;
    i8* datum_novum;
    i32 mensura;

    /* via in piscinam duplicatur - vocans chordam suam liberare potest */
    mensura = (i32)strlen(via);
    datum_novum = (i8*)piscina_allocare(exp->piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (datum_novum == NIHIL)
    {
        redde -I;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(datum_novum, via, (memoriae_index)mensura);
    }
    temp.datum = datum_novum;
    temp.mensura = mensura;
    via_fixa = _chorda_figere(exp->piscina, temp);

    locus = (SilvaFons*)xar_addere(exp->fontes);
    si (locus == NIHIL)
    {
        redde -I;
    }
    locus->via = via_fixa;
    locus->est_syntheticus = est_syntheticus;
    locus->est_lexicon = FALSUM;   /* _contextum_applicare signat */
    locus->est_custos = FALSUM;
    locus->custos_titulus = NIHIL;
    redde (s32)(xar_numerus(exp->fontes) - I);
}


/* ==================================================
 * Acta
 * ================================================== */

interior vacuum
_eventum_scribere (
    SilvaExpansio*    exp,
    SilvaEventumGenus genus,
    s32               fons_index,
    i32               linea,
    chorda*           titulus,
    SilvaMacroDef*    def,
    i32               conditio_id,
    s32               positus)
{
    SilvaEventum* locus;

    locus = (SilvaEventum*)xar_addere(exp->acta);
    si (locus == NIHIL)
    {
        redde;
    }
    locus->genus = genus;
    locus->fons_index = fons_index;
    locus->linea = linea;
    locus->titulus = titulus;
    locus->def = def;
    locus->conditio_id = conditio_id;
    locus->positus = positus;
}

TabulaDispersa*
silva_expansio_macros_ad_lineam (
    SilvaExpansio* exp,
    Piscina*       piscina,
    s32            fons_index,
    i32            linea)
{
    TabulaDispersa* tabula;
    SilvaEventum* eventum;
    i32 i;
    i32 n;

    tabula = tabula_dispersa_creare_chorda(piscina, LXIV);
    n = xar_numerus(exp->acta);
    per (i = ZEPHYRUM; i < n; i++)
    {
        eventum = (SilvaEventum*)xar_obtinere(exp->acta, i);
        si (eventum->fons_index == fons_index && eventum->linea >= linea)
        {
            frange;
        }
        si (eventum->genus == SILVA_EVENTUM_DEFINITIO)
        {
            tabula_dispersa_inserere(tabula, *eventum->titulus,
                (vacuum*)eventum->def);
        }
        alioquin
        {
            tabula_dispersa_delere(tabula, *eventum->titulus);
        }
    }
    redde tabula;
}


/* ==================================================
 * Quaestio tabulae vivae
 * ================================================== */

SilvaMacroDef*
silva_expansio_quaerere (
    SilvaExpansio* exp,
    chorda         titulus)
{
    vacuum* valor;
    TabulaDispersa* tabula;

    /* expansio positionalis tabulam temporalem substituit */
    tabula = (exp->tabula_activa != NIHIL) ? exp->tabula_activa : exp->macros;
    si (tabula_dispersa_invenire(tabula, titulus, &valor))
    {
        redde (SilvaMacroDef*)valor;
    }
    redde NIHIL;
}


/* ==================================================
 * Processio directivarum (Chunk A + D)
 * (Limen includendi: exp->limen_includendi, Phase 7)
 * ================================================== */

/* Genus directivae (internum) */
nomen enumeratio {
    SILVA_DIR_NULLA = 0,   /* # solum (directiva vacua) */
    SILVA_DIR_DEFINE,
    SILVA_DIR_UNDEF,
    SILVA_DIR_INCLUDE,
    SILVA_DIR_IF,
    SILVA_DIR_IFDEF,
    SILVA_DIR_IFNDEF,
    SILVA_DIR_ELIF,
    SILVA_DIR_ELSE,
    SILVA_DIR_ENDIF,
    SILVA_DIR_PRAGMA,      /* #pragma - vera directiva C89 (6.8.6),
                            * semantice iners: capta, consumpta,
                            * numquam in fluxum parsurae (M2d A) */
    SILVA_DIR_IGNOTA       /* alia (line/error/...) */
} SilvaDirectivaGenus;

/* Estne lexema verum initium directivae? (# ad initium lineae LOGICAE) */
interior b32
_est_initium_directivae (SilvaToken* token)
{
    redde (token->genus == SILVA_LEX_CANCELLUM && token->initium_lineae)
        ? VERUM : FALSUM;
}

/* Finis lineae logicae incipientis ad i (exclusive) */
interior i32
_lineam_finire (Xar* lexemata, i32 i, i32 n)
{
    i32 i_finis;
    SilvaToken* t;

    i_finis = i + I;
    dum (i_finis < n)
    {
        t = *(SilvaToken**)xar_obtinere(lexemata, i_finis);
        si (t->initium_lineae || t->genus == SILVA_LEX_EOF)
        {
            frange;
        }
        i_finis++;
    }
    redde i_finis;
}

interior vacuum
_lexema_addere (Xar* xar, SilvaToken* token)
{
    SilvaToken** locus;

    locus = (SilvaToken**)xar_addere(xar);
    si (locus != NIHIL)
    {
        *locus = token;
    }
}

/* Lamina: exemplar lexematum [a, b) */
interior Xar*
_lamina_capere (SilvaExpansio* exp, Xar* lexemata, i32 a, i32 b)
{
    Xar* lamina;
    i32 j;

    lamina = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    per (j = a; j < b; j++)
    {
        _lexema_addere(lamina, *(SilvaToken**)xar_obtinere(lexemata, j));
    }
    redde lamina;
}

/* Lineam directivae in directivae_out capere (si petitum) */
interior vacuum
_directivam_capere (
    SilvaExpansio* exp,
    Xar*           directivae,
    Xar*           lexemata,
    i32            a,
    i32            b)
{
    Xar** locus;

    si (directivae == NIHIL)
    {
        redde;
    }
    locus = (Xar**)xar_addere(directivae);
    si (locus != NIHIL)
    {
        *locus = _lamina_capere(exp, lexemata, a, b);
    }
}

interior b32
_chordae_aequales (chorda a, chorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(a.datum, b.datum, (memoriae_index)a.mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* Potestne lexema nomen esse in directiva? (verba clausa quoque) */
interior b32
_est_nomen_directivae (SilvaToken* token)
{
    si (token->genus == SILVA_LEX_IDENTIFICATOR)
    {
        redde VERUM;
    }
    si (token->genus >= SILVA_LEX_AUTO && token->genus <= SILVA_LEX_WHILE)
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* Classificare directivam ad i_cancellum. NB: #if et #else verba
 * clausa lexantur (genus IF/ELSE), cetera identificatores. */
interior SilvaDirectivaGenus
_directivae_genus (Xar* lexemata, i32 i_cancellum, i32 i_finis)
{
    SilvaToken* verbum;

    si (i_cancellum + I >= i_finis)
    {
        redde SILVA_DIR_NULLA;
    }
    verbum = *(SilvaToken**)xar_obtinere(lexemata, i_cancellum + I);
    si (verbum->genus == SILVA_LEX_IF)
    {
        redde SILVA_DIR_IF;
    }
    si (verbum->genus == SILVA_LEX_ELSE)
    {
        redde SILVA_DIR_ELSE;
    }
    si (verbum->genus != SILVA_LEX_IDENTIFICATOR)
    {
        redde SILVA_DIR_IGNOTA;
    }
    si (_chorda_est_literis(verbum->valor, "define"))
    {
        redde SILVA_DIR_DEFINE;
    }
    si (_chorda_est_literis(verbum->valor, "undef"))
    {
        redde SILVA_DIR_UNDEF;
    }
    si (_chorda_est_literis(verbum->valor, "include"))
    {
        redde SILVA_DIR_INCLUDE;
    }
    si (_chorda_est_literis(verbum->valor, "ifdef"))
    {
        redde SILVA_DIR_IFDEF;
    }
    si (_chorda_est_literis(verbum->valor, "ifndef"))
    {
        redde SILVA_DIR_IFNDEF;
    }
    si (_chorda_est_literis(verbum->valor, "elif"))
    {
        redde SILVA_DIR_ELIF;
    }
    si (_chorda_est_literis(verbum->valor, "endif"))
    {
        redde SILVA_DIR_ENDIF;
    }
    si (_chorda_est_literis(verbum->valor, "pragma"))
    {
        redde SILVA_DIR_PRAGMA;
    }
    redde SILVA_DIR_IGNOTA;
}

/* Processare define/undef: lexemata[i_cancellum] est CANCELLUM.
 * Reddit VERUM si processata. conditio_id = ramus continens (0 = nullus).
 * positus = longitudo reliquorum in consumptione (status ad punctum). */
interior b32
_definitionem_processare (
    SilvaExpansio*      exp,
    Xar*                lexemata,
    i32                 i_cancellum,
    i32                 i_finis,
    SilvaDirectivaGenus genus_dir,
    i32                 conditio_id,
    s32                 positus)
{
    SilvaToken* titulus_tok;
    SilvaMacroDef* def;
    chorda* titulus;
    i32 i;

    /* # verbum titulus ... - minimum # + verbum + titulus */
    si (i_cancellum + II >= i_finis)
    {
        redde FALSUM;
    }
    titulus_tok = *(SilvaToken**)xar_obtinere(lexemata, i_cancellum + II);
    si (!_est_nomen_directivae(titulus_tok))
    {
        redde FALSUM;
    }

    si (genus_dir == SILVA_DIR_UNDEF)
    {
        titulus = _chorda_figere(exp->piscina, titulus_tok->valor);
        tabula_dispersa_delere(exp->macros, titulus_tok->valor);
        _eventum_scribere(exp, SILVA_EVENTUM_DELETIO,
            titulus_tok->fons_index, titulus_tok->linea, titulus, NIHIL,
            conditio_id, positus);
        redde VERUM;
    }

    def = (SilvaMacroDef*)piscina_allocare(exp->piscina,
        (memoriae_index)magnitudo(SilvaMacroDef));
    si (def == NIHIL)
    {
        redde FALSUM;
    }
    memset(def, ZEPHYRUM, magnitudo(SilvaMacroDef));

    titulus = _chorda_figere(exp->piscina, titulus_tok->valor);
    def->titulus = titulus;
    def->fons_index = titulus_tok->fons_index;
    def->linea_def = titulus_tok->linea;
    def->corpus = xar_creare(exp->piscina, magnitudo(SilvaToken*));

    i = i_cancellum + III;

    /* Functio-similis: '(' IMMEDIATE adiacens (adiacentia cruda) */
    si (i < i_finis)
    {
        SilvaToken* paren;

        paren = *(SilvaToken**)xar_obtinere(lexemata, i);
        si (paren->genus == SILVA_LEX_PAREN_APERTA
            && paren->byte_offset == titulus_tok->byte_offset
                + (s32)titulus_tok->longitudo)
        {
            def->est_functio = VERUM;
            def->parametra = xar_creare(exp->piscina, magnitudo(chorda*));
            i++;  /* trans ( */
            dum (i < i_finis)
            {
                SilvaToken* t;

                t = *(SilvaToken**)xar_obtinere(lexemata, i);
                si (t->genus == SILVA_LEX_PAREN_CLAUSA)
                {
                    i++;
                    frange;
                }
                si (t->genus == SILVA_LEX_IDENTIFICATOR)
                {
                    chorda** locus;

                    locus = (chorda**)xar_addere(def->parametra);
                    si (locus != NIHIL)
                    {
                        *locus = _chorda_figere(exp->piscina, t->valor);
                    }
                }
                alioquin si (t->genus == SILVA_LEX_ELLIPSIS)
                {
                    /* variadica: parametrum "__VA_ARGS__" appenditur ut
                     * ultimum - cauda argumentorum ei ligatur */
                    chorda** locus;

                    def->est_variadica = VERUM;
                    locus = (chorda**)xar_addere(def->parametra);
                    si (locus != NIHIL)
                    {
                        *locus = _chorda_figere(exp->piscina,
                            chorda_ex_literis("__VA_ARGS__", exp->piscina));
                    }
                }
                /* COMMA transitur */
                i++;
            }
        }
    }

    /* Corpus: reliqua lineae logicae */
    per (; i < i_finis; i++)
    {
        _lexema_addere(def->corpus, *(SilvaToken**)xar_obtinere(lexemata, i));
    }

    tabula_dispersa_inserere(exp->macros, *titulus, (vacuum*)def);
    _eventum_scribere(exp, SILVA_EVENTUM_DEFINITIO,
        def->fons_index, def->linea_def, titulus, def, conditio_id, positus);
    redde VERUM;
}

/* Praedeclarationes (recursio mutua: fluxus <-> regiones <-> includenda) */
interior vacuum
_fluxum_processare (SilvaExpansio* exp, Xar* lexemata, i32 i_initium,
    i32 i_finis, i32 conditio_id, SilvaRegio* pater, b32 servare_eof,
    Xar* reliqua, Xar* directivae);
interior vacuum
_plagulam_processare (SilvaExpansio* exp, Xar* lexemata, b32 servare_eof,
    Xar* reliqua, Xar* directivae);

/* ==================================================
 * Chunk D - Regiones conditionales
 * ================================================== */

interior SilvaRegio*
_regionem_creare (SilvaExpansio* exp, SilvaToken* cancellum, SilvaRegio* pater)
{
    SilvaRegio* regio;

    regio = (SilvaRegio*)piscina_allocare(exp->piscina,
        (memoriae_index)magnitudo(SilvaRegio));
    si (regio == NIHIL)
    {
        redde NIHIL;
    }
    regio->fons_index = cancellum->fons_index;
    regio->linea = cancellum->linea;
    regio->rami = xar_creare(exp->piscina, magnitudo(SilvaRamus*));
    regio->pater = pater;
    regio->filiae = xar_creare(exp->piscina, magnitudo(SilvaRegio*));
    regio->est_imperfecta = FALSUM;
    regio->est_ultra_modum = FALSUM;
    regio->directiva_finis = NIHIL;
    regio->est_texta = FALSUM;

    si (pater != NIHIL)
    {
        SilvaRegio** locus;

        locus = (SilvaRegio**)xar_addere(pater->filiae);
        si (locus != NIHIL)
        {
            *locus = regio;
        }
    }
    alioquin
    {
        SilvaRegio** locus;

        locus = (SilvaRegio**)xar_addere(exp->regiones);
        si (locus != NIHIL)
        {
            *locus = regio;
        }
    }
    redde regio;
}

/* Processare regionem: lexemata[i] est CANCELLUM #if/#ifdef/#ifndef.
 * Consumit usque ad #endif parem (vel finem fluxus). Reddit indicem
 * post regionem. Via defalta: primus ramus verus sumitur - lexemata
 * eius normaliter processantur; ceteri laminas crudas retinent.
 * Profunditas ultra limen (Phase 7): regio est_ultra_modum - NULLUS
 * ramus evaluatur, omnes crudi, recursio cessat (fluxus infestus
 * acervum numquam perfodit); octeti in crudis supersunt, ergo
 * reconstructio tenet. */
interior i32
_regionem_processare_interna (
    SilvaExpansio* exp,
    Xar*           lexemata,
    i32            i,
    i32            i_finis,
    SilvaRegio*    pater,
    Xar*           reliqua,
    Xar*           directivae)
{
    SilvaRegio* regio;
    b32 sumptum_iam;
    i32 i_currens;

    regio = _regionem_creare(exp,
        *(SilvaToken**)xar_obtinere(lexemata, i), pater);
    si (regio == NIHIL)
    {
        redde i_finis;
    }
    si (exp->limen_regionum > ZEPHYRUM
        && exp->profunditas_regionum > exp->limen_regionum)
    {
        regio->est_ultra_modum = VERUM;
        exp->fines_tactae = VERUM;
    }
    sumptum_iam = FALSUM;
    i_currens = i;

    dum (i_currens < i_finis)
    {
        SilvaRamus* ramus;
        SilvaRamus** locus_rami;
        SilvaDirectivaGenus genus_dir;
        i32 i_linea_finis;
        i32 i_corpus;
        i32 i_scan;
        i32 profunditas;

        i_linea_finis = _lineam_finire(lexemata, i_currens, i_finis);
        genus_dir = _directivae_genus(lexemata, i_currens, i_linea_finis);
        /* β (sim ⑦ C2): linea structuralis NON capitur - regio eam
         * possidet (ramus->directiva infra); scriptura lineas regionum
         * non textarum ex arbore regionum colligit */

        /* Ramum creare */
        ramus = (SilvaRamus*)piscina_allocare(exp->piscina,
            (memoriae_index)magnitudo(SilvaRamus));
        si (ramus == NIHIL)
        {
            redde i_finis;
        }
        memset(ramus, ZEPHYRUM, magnitudo(SilvaRamus));
        commutatio (genus_dir)
        {
            casus SILVA_DIR_IF:     ramus->genus = SILVA_RAMUS_IF;     frange;
            casus SILVA_DIR_IFDEF:  ramus->genus = SILVA_RAMUS_IFDEF;  frange;
            casus SILVA_DIR_IFNDEF: ramus->genus = SILVA_RAMUS_IFNDEF; frange;
            casus SILVA_DIR_ELIF:   ramus->genus = SILVA_RAMUS_ELIF;   frange;
            ordinarius:             ramus->genus = SILVA_RAMUS_ELSE;   frange;
        }
        ramus->conditio_id = (i32)(xar_numerus(exp->rami) + I);
        locus_rami = (SilvaRamus**)xar_addere(exp->rami);
        si (locus_rami != NIHIL)
        {
            *locus_rami = ramus;
        }
        ramus->directiva = _lamina_capere(exp, lexemata, i_currens,
            i_linea_finis);
        ramus->regio = regio;
        si (ramus->genus != SILVA_RAMUS_ELSE)
        {
            ramus->expressio = _lamina_capere(exp, lexemata,
                i_currens + II, i_linea_finis);
        }

        /* #if 0 idioma: litteralis falsa */
        si ((ramus->genus == SILVA_RAMUS_IF
                || ramus->genus == SILVA_RAMUS_ELIF)
            && ramus->expressio != NIHIL
            && xar_numerus(ramus->expressio) == I)
        {
            SilvaToken* solum;

            solum = *(SilvaToken**)xar_obtinere(ramus->expressio, ZEPHYRUM);
            si (solum->genus == SILVA_LEX_INTEGER
                && _chorda_est_literis(solum->valor, "0"))
            {
                ramus->est_numquam = VERUM;
            }
        }

        /* Evaluatio (via defalta) - rami post sumptum NON evaluantur;
         * regio ultra modum: nullus umquam (omnes crudi) */
        si (!sumptum_iam && !regio->est_ultra_modum)
        {
            si (ramus->genus == SILVA_RAMUS_ELSE)
            {
                ramus->valor = I;
                ramus->est_evaluatum = VERUM;
            }
            alioquin si (ramus->genus == SILVA_RAMUS_IFDEF
                || ramus->genus == SILVA_RAMUS_IFNDEF)
            {
                si (ramus->expressio != NIHIL
                    && xar_numerus(ramus->expressio) > ZEPHYRUM)
                {
                    SilvaToken* operandum;

                    operandum = *(SilvaToken**)xar_obtinere(
                        ramus->expressio, ZEPHYRUM);
                    si (_est_nomen_directivae(operandum))
                    {
                        b32 definitum;

                        definitum = silva_conditio_est_definitum(exp,
                            operandum->valor);
                        si (ramus->genus == SILVA_RAMUS_IFNDEF)
                        {
                            definitum = definitum ? FALSUM : VERUM;
                        }
                        ramus->valor = definitum ? I : ZEPHYRUM;
                        ramus->est_evaluatum = VERUM;
                    }
                }
                /* operandum absens/malformatum: falsum, non evaluatum */
            }
            alioquin
            {
                b32 successus;

                successus = FALSUM;
                ramus->valor = silva_conditio_evaluare(exp,
                    ramus->expressio, &successus);
                ramus->est_evaluatum = successus;
                si (!successus)
                {
                    ramus->valor = ZEPHYRUM;  /* robustitas: falsum */
                }
            }
            ramus->est_sumptum = (ramus->valor != ZEPHYRUM) ? VERUM : FALSUM;
        }

        locus_rami = (SilvaRamus**)xar_addere(regio->rami);
        si (locus_rami != NIHIL)
        {
            *locus_rami = ramus;
        }

        /* Corpus rami: usque ad ELIF/ELSE/ENDIF parem (profunditas 0) */
        i_corpus = i_linea_finis;
        i_scan = i_corpus;
        profunditas = ZEPHYRUM;
        dum (i_scan < i_finis)
        {
            SilvaToken* t;

            t = *(SilvaToken**)xar_obtinere(lexemata, i_scan);
            si (_est_initium_directivae(t))
            {
                SilvaDirectivaGenus g;
                i32 lf;

                lf = _lineam_finire(lexemata, i_scan, i_finis);
                g = _directivae_genus(lexemata, i_scan, lf);
                si (g == SILVA_DIR_IF || g == SILVA_DIR_IFDEF
                    || g == SILVA_DIR_IFNDEF)
                {
                    profunditas++;
                }
                alioquin si (g == SILVA_DIR_ENDIF)
                {
                    si (profunditas == ZEPHYRUM)
                    {
                        frange;
                    }
                    profunditas--;
                }
                alioquin si ((g == SILVA_DIR_ELIF || g == SILVA_DIR_ELSE)
                    && profunditas == ZEPHYRUM)
                {
                    frange;
                }
                i_scan = lf;
            }
            alioquin
            {
                i_scan++;
            }
        }

        /* Fines corporis in offsetibus (sim ⑦ C1 - textura eos contra
         * extenta sententiarum comparat). Finis EXCLUSIVUS = offset
         * directivae proximae; imperfecta = apertum */
        si (i_corpus < i_scan)
        {
            SilvaToken* primum_corporis =
                *(SilvaToken**)xar_obtinere(lexemata, i_corpus);

            ramus->corpus_initium = primum_corporis->byte_offset;
        }
        alioquin
        {
            ramus->corpus_initium = -I;
        }
        si (i_scan < i_finis)
        {
            SilvaToken* post_corpus =
                *(SilvaToken**)xar_obtinere(lexemata, i_scan);

            ramus->corpus_finis = post_corpus->byte_offset;
        }
        alioquin
        {
            ramus->corpus_finis = 0x7FFFFFFF;  /* imperfecta: apertum */
        }

        si (ramus->est_sumptum)
        {
            sumptum_iam = VERUM;
            _fluxum_processare(exp, lexemata, i_corpus, i_scan,
                ramus->conditio_id, regio, VERUM, reliqua, directivae);
        }
        alioquin
        {
            ramus->lexemata_cruda = _lamina_capere(exp, lexemata,
                i_corpus, i_scan);
        }

        si (i_scan >= i_finis)
        {
            regio->est_imperfecta = VERUM;  /* EOF ante #endif */
            redde i_finis;
        }

        /* i_scan stat ad ELIF/ELSE/ENDIF */
        {
            SilvaDirectivaGenus g;
            i32 lf;

            lf = _lineam_finire(lexemata, i_scan, i_finis);
            g = _directivae_genus(lexemata, i_scan, lf);
            si (g == SILVA_DIR_ENDIF)
            {
                /* β: linea #endif regioni ipsa (sim ⑦ C1/C2) */
                regio->directiva_finis = _lamina_capere(exp, lexemata,
                    i_scan, lf);
                redde lf;
            }
            i_currens = i_scan;  /* ramus proximus (elif/else) */
        }
    }

    regio->est_imperfecta = VERUM;
    redde i_finis;
}

/* Involucrum numeratoris profunditatis (omnes viae reditus tectae) */
interior i32
_regionem_processare (
    SilvaExpansio* exp,
    Xar*           lexemata,
    i32            i,
    i32            i_finis,
    SilvaRegio*    pater,
    Xar*           reliqua,
    Xar*           directivae)
{
    i32 fructus;

    exp->profunditas_regionum++;
    fructus = _regionem_processare_interna(exp, lexemata, i, i_finis,
        pater, reliqua, directivae);
    exp->profunditas_regionum--;
    redde fructus;
}

/* ==================================================
 * Chunk D - Includenda
 * ================================================== */

/* Viam iungere et canonicam facere (cpp verus: inclusio CITATA
 * relative ad plicaturam plagulae includentis PRIMUM quaeritur -
 * 01KYJ6740K; basinomina aequalia trans plicaturas antea
 * confundebantur). Segmenta '.' cadunt, 'seg/..' plicantur,
 * praefixum absolutum '/' servatur. Vacua = iunctio incanonica
 * (supra radicem ascendit) aut apparatus deficiens - quaerens tunc
 * gradus sequentes (exacta, basename) temptat, mores pristini. */
#define SILVA_SEGMENTA_VIAE_MAXIMA LXIV

interior chorda
_via_iuncta_canonica (Piscina* piscina, chorda plicatura, chorda via)
{
    chorda effusio;
    character* crudum;
    character* datum;
    i32 crudum_m;
    i32 cacumina[SILVA_SEGMENTA_VIAE_MAXIMA];
    i32 altitudo = ZEPHYRUM;
    i32 basis = ZEPHYRUM;
    i32 scriptum;
    i32 i = ZEPHYRUM;

    effusio.datum = NIHIL;
    effusio.mensura = ZEPHYRUM;
    si (plicatura.mensura == ZEPHYRUM || via.mensura == ZEPHYRUM)
    {
        redde effusio;
    }
    crudum_m = plicatura.mensura + I + via.mensura;
    crudum = (character*)piscina_allocare(piscina,
        (memoriae_index)crudum_m);
    datum = (character*)piscina_allocare(piscina,
        (memoriae_index)crudum_m);
    si (crudum == NIHIL || datum == NIHIL)
    {
        redde effusio;
    }
    memcpy(crudum, plicatura.datum,
        (memoriae_index)plicatura.mensura);
    crudum[plicatura.mensura] = '/';
    memcpy(crudum + plicatura.mensura + I, via.datum,
        (memoriae_index)via.mensura);

    si (crudum[ZEPHYRUM] == '/')
    {
        datum[ZEPHYRUM] = '/';
        basis = I;
    }
    scriptum = basis;
    dum (i < crudum_m)
    {
        i32 seg_ini = i;
        i32 seg_m;

        dum (i < crudum_m && crudum[i] != '/')
        {
            i++;
        }
        seg_m = i - seg_ini;
        si (i < crudum_m)
        {
            i++;
        }
        si (seg_m == ZEPHYRUM)
        {
            perge;
        }
        si (seg_m == I && crudum[seg_ini] == '.')
        {
            perge;
        }
        si (seg_m == II && crudum[seg_ini] == '.'
            && crudum[seg_ini + I] == '.')
        {
            si (altitudo == ZEPHYRUM)
            {
                redde effusio;
            }
            altitudo--;
            scriptum = cacumina[altitudo];
            perge;
        }
        si (altitudo >= SILVA_SEGMENTA_VIAE_MAXIMA)
        {
            redde effusio;
        }
        cacumina[altitudo] = scriptum;
        altitudo++;
        si (scriptum > basis)
        {
            datum[scriptum] = '/';
            scriptum++;
        }
        memcpy(datum + scriptum, crudum + seg_ini,
            (memoriae_index)seg_m);
        scriptum = scriptum + seg_m;
    }
    si (scriptum == ZEPHYRUM)
    {
        redde effusio;
    }
    effusio.datum = (i8*)datum;
    effusio.mensura = scriptum;
    redde effusio;
}

/* Processare #include: viam extrahere, inclusionem memorare,
 * contentum praebitum recursive processare (reliqua hic inserta) */
interior vacuum
_includendum_processare (
    SilvaExpansio* exp,
    Xar*           lexemata,
    i32            i_cancellum,
    i32            i_finis,
    Xar*           reliqua,
    Xar*           directivae)
{
    SilvaToken* cancellum;
    SilvaInclusio* inclusio;
    SilvaIncludendum* incl;
    SilvaFons* fons;
    chorda via;
    b32 habet_viam;
    b32 citata;
    b32 inventum;
    vacuum* valor;
    i32 i_op;

    cancellum = *(SilvaToken**)xar_obtinere(lexemata, i_cancellum);
    habet_viam = FALSUM;
    citata = FALSUM;
    via.datum = NIHIL;
    via.mensura = ZEPHYRUM;
    i_op = i_cancellum + II;

    si (i_op < i_finis)
    {
        SilvaToken* t;

        t = *(SilvaToken**)xar_obtinere(lexemata, i_op);
        si (t->genus == SILVA_LEX_STRING_LIT && t->valor.mensura >= II)
        {
            /* "via" - termini remoti */
            via.datum = t->valor.datum + I;
            via.mensura = t->valor.mensura - II;
            habet_viam = VERUM;
            citata = VERUM;
        }
        alioquin si (t->genus == SILVA_LEX_MINOR)
        {
            /* forma angulata - valores concatenati usque ad '>' */
            ChordaAedificator* aed;
            b32 clausa;
            i32 j;

            aed = chorda_aedificator_creare(exp->piscina, XXXII);
            clausa = FALSUM;
            per (j = i_op + I; j < i_finis; j++)
            {
                SilvaToken* u;

                u = *(SilvaToken**)xar_obtinere(lexemata, j);
                si (u->genus == SILVA_LEX_MAIOR)
                {
                    clausa = VERUM;
                    frange;
                }
                chorda_aedificator_appendere_chorda(aed, u->valor);
            }
            si (clausa)
            {
                via = chorda_aedificator_finire(aed);
                habet_viam = VERUM;
            }
        }
        /* forma per macro expansa: differtur (vide phase-log Chunk D) */
    }

    si (!habet_viam)
    {
        redde;  /* malformata: linea capta, nihil insertum */
    }

    /* Inclusionem memorare (graphum dependentiarum - "discens").
     * NB: monstratores elementorum Xar stabiles sunt trans additiones
     * (segmenta fixa, sine reallocatio - vide xar.c); ordo
     * scribendi-ante-recursionem est claritas, non necessitas. */
    inclusio = (SilvaInclusio*)xar_addere(exp->inclusiones);
    si (inclusio == NIHIL)
    {
        redde;
    }
    inclusio->fons_ex = cancellum->fons_index;
    inclusio->via = _chorda_figere(exp->piscina, via);
    inclusio->fons_ad = -I;
    inclusio->est_praetermissa = FALSUM;
    inclusio->est_angulata = citata ? FALSUM : VERUM;

    valor = NIHIL;
    inventum = FALSUM;
    si (citata
        && (via.mensura == ZEPHYRUM || via.datum[ZEPHYRUM] != '/'))
    {
        /* cpp verus (01KYJ6740K): inclusio citata relative ad
         * plicaturam plagulae INCLUDENTIS primum quaeritur.
         * Praebenda sub via canonica plena sedent (vide
         * silva_includendum_praebere) - iunctio incanonica aut
         * ignota cadit ad gradus sequentes, mores pristini. */
        SilvaFons* includens = (SilvaFons*)xar_obtinere(exp->fontes,
            (i32)cancellum->fons_index);

        si (includens != NIHIL && includens->via != NIHIL)
        {
            chorda plicatura;
            i32 j;

            plicatura = *includens->via;
            plicatura.mensura = ZEPHYRUM;
            per (j = includens->via->mensura; j > ZEPHYRUM; j--)
            {
                si (includens->via->datum[j - I] == '/')
                {
                    plicatura.mensura = j - I;
                    frange;
                }
            }
            si (plicatura.mensura > ZEPHYRUM)
            {
                chorda iuncta = _via_iuncta_canonica(exp->piscina,
                    plicatura, via);

                si (iuncta.mensura > ZEPHYRUM
                    && tabula_dispersa_invenire(exp->includenda,
                           iuncta, &valor))
                {
                    inventum = VERUM;
                }
            }
        }
    }
    si (!inventum
        && tabula_dispersa_invenire(exp->includenda, via, &valor))
    {
        inventum = VERUM;
    }
    si (!inventum)
    {
        /* Recidiva ad BASENAME (M2d Chunk D): praebenda sub
         * basename etiam sedent (primus vincit), sed fontes veri
         * "../include/utf8.h" scribunt - quaestio exacta fallebat
         * et catena TOTA (latina.h transitive!) irresoluta
         * manebat. Includenti-relativa prior, exacta secunda,
         * basename tertia; omnes fallunt -> transitus
         * conservativus idem. */
        chorda basis = via;
        i32 j;

        per (j = via.mensura; j > I; j--)
        {
            si (via.datum[j - I] == '/')
            {
                basis.datum = via.datum + j;
                basis.mensura = via.mensura - j;
                frange;
            }
        }
        si (basis.mensura == via.mensura
            || !tabula_dispersa_invenire(exp->includenda, basis,
                   &valor))
        {
            redde;  /* ignotum: via memorata, processio pergit */
        }
    }
    incl = (SilvaIncludendum*)valor;
    inclusio->fons_ad = incl->fons_index;

    /* Custos definitus -> plagula praetermittitur (interior iam nota) */
    fons = (SilvaFons*)xar_obtinere(exp->fontes, (i32)incl->fons_index);
    si (fons->est_custos && fons->custos_titulus != NIHIL
        && silva_conditio_est_definitum(exp, *fons->custos_titulus))
    {
        inclusio->est_praetermissa = VERUM;
        redde;
    }

    si (exp->limen_includendi > ZEPHYRUM
        && exp->profunditas_includendi >= exp->limen_includendi)
    {
        inclusio->est_praetermissa = VERUM;  /* profunditas nimia */
        exp->fines_tactae = VERUM;
        redde;
    }

    exp->profunditas_includendi++;
    _plagulam_processare(exp, incl->lexemata, FALSUM, reliqua, directivae);
    exp->profunditas_includendi--;
}

s32
silva_includendum_praebere (
    SilvaExpansio*      exp,
    constans character* via,
    constans character* textus,
    i32                 mensura)
{
    SilvaIncludendum* incl;
    SilvaFons* fons;
    s32 fons_index;

    fons_index = silva_fons_addere(exp, via, FALSUM);
    si (fons_index < ZEPHYRUM)
    {
        redde -I;
    }
    incl = (SilvaIncludendum*)piscina_allocare(exp->piscina,
        (memoriae_index)magnitudo(SilvaIncludendum));
    si (incl == NIHIL)
    {
        redde -I;
    }
    incl->fons_index = fons_index;
    incl->lexemata = silva_lexare(exp->piscina, textus, mensura, fons_index);

    fons = (SilvaFons*)xar_obtinere(exp->fontes, (i32)fons_index);
    {
        /* Claves mappae DUAE ex praebitione una: via canonica
         * plena (resolutio includenti-relativa eam quaerit) +
         * basename (consumptores basename-soli, e.g. saltuarius,
         * per recidivam vivunt). Primus vincit utrobique. */
        chorda radix_iunctionis;
        chorda plena;
        chorda basis;
        i32 j;

        si (fons->via->mensura > ZEPHYRUM
            && fons->via->datum[ZEPHYRUM] == '/')
        {
            radix_iunctionis = chorda_ex_literis("/", exp->piscina);
        }
        alioquin
        {
            radix_iunctionis = chorda_ex_literis(".", exp->piscina);
        }
        plena = _via_iuncta_canonica(exp->piscina, radix_iunctionis,
            *fons->via);
        si (plena.mensura == ZEPHYRUM)
        {
            plena = *fons->via;
        }
        si (!tabula_dispersa_continet(exp->includenda, plena))
        {
            tabula_dispersa_inserere(exp->includenda, plena,
                (vacuum*)incl);
        }
        basis = plena;
        per (j = plena.mensura; j > I; j--)
        {
            si (plena.datum[j - I] == '/')
            {
                basis.datum = plena.datum + j;
                basis.mensura = plena.mensura - j;
                frange;
            }
        }
        si (basis.mensura != plena.mensura && basis.mensura > ZEPHYRUM
            && !tabula_dispersa_continet(exp->includenda, basis))
        {
            tabula_dispersa_inserere(exp->includenda, basis,
                (vacuum*)incl);
        }
    }
    redde fons_index;
}

/* ==================================================
 * Chunk D - Custodes (est_custos)
 *
 * Forma stricta: primum lexema plagulae est '#' de '#ifndef X';
 * linea logica proxima est '#define X'; #endif par nihil nisi
 * EOF sequitur. Detecta: NULLA regio - interior incondicionaliter
 * processatur (a linea #define), custos in SilvaFons memoratur.
 * ================================================== */

interior b32
_custodem_detegere (
    Xar*         lexemata,
    i32*         i_corpus_out,   /* initium lineae #define */
    i32*         i_endif_out,    /* initium lineae #endif */
    i32*         i_post_out,     /* post lineam #endif */
    SilvaToken** operandum_out)
{
    SilvaToken* tok;
    SilvaToken* operandum;
    SilvaToken* def_operandum;
    i32 n;
    i32 lf0;
    i32 lf1;
    i32 i;
    i32 profunditas;

    n = xar_numerus(lexemata);
    si (n == ZEPHYRUM)
    {
        redde FALSUM;
    }
    tok = *(SilvaToken**)xar_obtinere(lexemata, ZEPHYRUM);
    si (!_est_initium_directivae(tok))
    {
        redde FALSUM;
    }
    lf0 = _lineam_finire(lexemata, ZEPHYRUM, n);
    si (_directivae_genus(lexemata, ZEPHYRUM, lf0) != SILVA_DIR_IFNDEF)
    {
        redde FALSUM;
    }
    si (II >= lf0)
    {
        redde FALSUM;
    }
    operandum = *(SilvaToken**)xar_obtinere(lexemata, II);
    si (!_est_nomen_directivae(operandum))
    {
        redde FALSUM;
    }

    /* linea proxima: #define X (idem X) */
    si (lf0 >= n)
    {
        redde FALSUM;
    }
    tok = *(SilvaToken**)xar_obtinere(lexemata, lf0);
    si (!_est_initium_directivae(tok))
    {
        redde FALSUM;
    }
    lf1 = _lineam_finire(lexemata, lf0, n);
    si (_directivae_genus(lexemata, lf0, lf1) != SILVA_DIR_DEFINE)
    {
        redde FALSUM;
    }
    si (lf0 + II >= lf1)
    {
        redde FALSUM;
    }
    def_operandum = *(SilvaToken**)xar_obtinere(lexemata, lf0 + II);
    si (!_chordae_aequales(def_operandum->valor, operandum->valor))
    {
        redde FALSUM;
    }

    /* #endif par quaerere; post eum nihil nisi EOF */
    i = lf1;
    profunditas = ZEPHYRUM;
    dum (i < n)
    {
        tok = *(SilvaToken**)xar_obtinere(lexemata, i);
        si (_est_initium_directivae(tok))
        {
            SilvaDirectivaGenus g;
            i32 lf;

            lf = _lineam_finire(lexemata, i, n);
            g = _directivae_genus(lexemata, i, lf);
            si (g == SILVA_DIR_IF || g == SILVA_DIR_IFDEF
                || g == SILVA_DIR_IFNDEF)
            {
                profunditas++;
            }
            alioquin si (g == SILVA_DIR_ENDIF)
            {
                si (profunditas == ZEPHYRUM)
                {
                    i32 j;

                    per (j = lf; j < n; j++)
                    {
                        SilvaToken* u;

                        u = *(SilvaToken**)xar_obtinere(lexemata, j);
                        si (u->genus != SILVA_LEX_EOF)
                        {
                            redde FALSUM;  /* contentum post #endif */
                        }
                    }
                    *i_corpus_out = lf0;
                    *i_endif_out = i;
                    *i_post_out = lf;
                    *operandum_out = operandum;
                    redde VERUM;
                }
                profunditas--;
            }
            i = lf;
        }
        alioquin
        {
            i++;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Ambulator fluxus + introitus publicus
 * ================================================== */

interior vacuum
_fluxum_processare (
    SilvaExpansio* exp,
    Xar*           lexemata,
    i32            i_initium,
    i32            i_finis,
    i32            conditio_id,
    SilvaRegio*    pater,
    b32            servare_eof,
    Xar*           reliqua,
    Xar*           directivae)
{
    SilvaToken* token;
    i32 i;

    i = i_initium;
    dum (i < i_finis)
    {
        token = *(SilvaToken**)xar_obtinere(lexemata, i);

        si (_est_initium_directivae(token))
        {
            SilvaDirectivaGenus genus_dir;
            i32 i_linea_finis;

            i_linea_finis = _lineam_finire(lexemata, i, i_finis);
            genus_dir = _directivae_genus(lexemata, i, i_linea_finis);

            si (genus_dir == SILVA_DIR_DEFINE || genus_dir == SILVA_DIR_UNDEF)
            {
                si (_definitionem_processare(exp, lexemata, i, i_linea_finis,
                        genus_dir, conditio_id,
                        (s32)xar_numerus(reliqua)))
                {
                    _directivam_capere(exp, directivae, lexemata, i,
                        i_linea_finis);
                    i = i_linea_finis;
                    perge;
                }
                /* malformata: cadit ad reliqua */
            }
            alioquin si (genus_dir == SILVA_DIR_INCLUDE)
            {
                _directivam_capere(exp, directivae, lexemata, i,
                    i_linea_finis);
                _includendum_processare(exp, lexemata, i, i_linea_finis,
                    reliqua, directivae);
                i = i_linea_finis;
                perge;
            }
            alioquin si (genus_dir == SILVA_DIR_IF
                || genus_dir == SILVA_DIR_IFDEF
                || genus_dir == SILVA_DIR_IFNDEF)
            {
                i = _regionem_processare(exp, lexemata, i, i_finis,
                    pater, reliqua, directivae);
                perge;
            }
            alioquin si (genus_dir == SILVA_DIR_PRAGMA)
            {
                /* #pragma: capta ut linea directivae (scribere
                 * eam reficit), numquam in fluxum parsurae -
                 * "#pragma once" segmenta non iam frangit */
                _directivam_capere(exp, directivae, lexemata, i,
                    i_linea_finis);
                i = i_linea_finis;
                perge;
            }
            /* ELIF/ELSE/ENDIF sine regione, NULLA, IGNOTA:
             * transeunt ad reliqua (byte-conservativum) */
        }

        si (token->genus != SILVA_LEX_EOF || servare_eof)
        {
            _lexema_addere(reliqua, token);
        }
        i++;
    }
}

interior vacuum
_plagulam_processare (
    SilvaExpansio* exp,
    Xar*           lexemata,
    b32            servare_eof,
    Xar*           reliqua,
    Xar*           directivae)
{
    SilvaToken* operandum;
    i32 i_corpus;
    i32 i_endif;
    i32 i_post;
    i32 n;

    n = xar_numerus(lexemata);
    operandum = NIHIL;

    si (_custodem_detegere(lexemata, &i_corpus, &i_endif, &i_post, &operandum))
    {
        s32 fi;

        fi = operandum->fons_index;
        si (fi >= ZEPHYRUM)
        {
            SilvaFons* fons;

            fons = (SilvaFons*)xar_obtinere(exp->fontes, (i32)fi);
            fons->est_custos = VERUM;
            fons->custos_titulus = _chorda_figere(exp->piscina,
                operandum->valor);
        }

        /* custos iam definitus (rarum: iniectio ante processionem).
         * NON transparentia: per ambulatorem NORMALEM - #ifndef falsum
         * evaluat, interior ramus NON sumptus fit (lexemata_cruda -
         * "extra rationem sed numquam perditum", sim ⑥ C7), lineae
         * directivae normaliter captae. Reconstructio fontis viam
         * ordinariam reinserendorum equitat. */
        si (silva_conditio_est_definitum(exp, operandum->valor))
        {
            _fluxum_processare(exp, lexemata, ZEPHYRUM, n, ZEPHYRUM, NIHIL,
                servare_eof, reliqua, directivae);
            redde;
        }

        /* transparentia custodis: nulla regio; tres directivae captae;
         * interior (a linea #define) incondicionaliter processatur */
        _directivam_capere(exp, directivae, lexemata, ZEPHYRUM, i_corpus);
        _fluxum_processare(exp, lexemata, i_corpus, i_endif, ZEPHYRUM, NIHIL,
            servare_eof, reliqua, directivae);
        _directivam_capere(exp, directivae, lexemata, i_endif, i_post);
        _fluxum_processare(exp, lexemata, i_post, n, ZEPHYRUM, NIHIL,
            servare_eof, reliqua, directivae);
        redde;
    }

    _fluxum_processare(exp, lexemata, ZEPHYRUM, n, ZEPHYRUM, NIHIL,
        servare_eof, reliqua, directivae);
}

Xar*
silva_expansio_directivas_processare (
    SilvaExpansio* exp,
    Xar*           lexemata,
    Xar**          directivae_out)
{
    Xar* reliqua;
    Xar* directivae;

    reliqua = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    directivae = NIHIL;
    si (directivae_out != NIHIL)
    {
        directivae = xar_creare(exp->piscina, magnitudo(Xar*));
        *directivae_out = directivae;
    }
    _plagulam_processare(exp, lexemata, VERUM, reliqua, directivae);
    redde reliqua;
}


/* ==================================================
 * Iniectio macro per API
 * ================================================== */

interior s32
_fons_api_obtinere (SilvaExpansio* exp)
{
    si (exp->fons_api < ZEPHYRUM)
    {
        exp->fons_api = silva_fons_addere(exp, "<api>", VERUM);
    }
    redde exp->fons_api;
}

/* Lexare corpus in piscinam (textus duplicatur - lexemata visus in
 * eum tenent); EOF remotum. Lexemata sunt FONTIS synthetici:
 * provenientia per catenas normales fluit. */
interior Xar*
_corpus_api_lexare (SilvaExpansio* exp, constans character* textus)
{
    Xar* lexemata;
    Xar* corpus;
    i8* fixum;
    i32 mensura;
    i32 i;
    i32 n;

    mensura = (i32)strlen(textus);
    fixum = (i8*)piscina_allocare(exp->piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fixum == NIHIL)
    {
        redde NIHIL;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(fixum, textus, (memoriae_index)mensura);
    }

    lexemata = silva_lexare(exp->piscina, (constans character*)fixum,
        mensura, _fons_api_obtinere(exp));
    corpus = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    n = xar_numerus(lexemata);
    per (i = ZEPHYRUM; i < n; i++)
    {
        SilvaToken* t;

        t = *(SilvaToken**)xar_obtinere(lexemata, i);
        si (t->genus != SILVA_LEX_EOF)
        {
            _lexema_addere(corpus, t);
        }
    }
    redde corpus;
}

interior SilvaMacroDef*
_def_api_creare (
    SilvaExpansio*      exp,
    constans character* titulus,
    constans character* corpus)
{
    SilvaMacroDef* def;

    def = (SilvaMacroDef*)piscina_allocare(exp->piscina,
        (memoriae_index)magnitudo(SilvaMacroDef));
    si (def == NIHIL)
    {
        redde NIHIL;
    }
    memset(def, ZEPHYRUM, magnitudo(SilvaMacroDef));
    def->titulus = _chorda_figere(exp->piscina,
        chorda_ex_literis(titulus, exp->piscina));
    def->corpus = _corpus_api_lexare(exp, corpus);
    def->fons_index = _fons_api_obtinere(exp);
    def->linea_def = ZEPHYRUM;
    def->ex_api = VERUM;
    si (def->titulus == NIHIL || def->corpus == NIHIL)
    {
        redde NIHIL;
    }
    redde def;
}

interior vacuum
_def_api_registrare (SilvaExpansio* exp, SilvaMacroDef* def)
{
    tabula_dispersa_inserere(exp->macros, *def->titulus, (vacuum*)def);
    _eventum_scribere(exp, SILVA_EVENTUM_DEFINITIO, def->fons_index,
        ZEPHYRUM, def->titulus, def, ZEPHYRUM, ZEPHYRUM);
}

b32
silva_macro_addere (
    SilvaExpansio*      exp,
    constans character* titulus,
    constans character* corpus)
{
    SilvaMacroDef* def;

    def = _def_api_creare(exp, titulus, corpus);
    si (def == NIHIL)
    {
        redde FALSUM;
    }
    _def_api_registrare(exp, def);
    redde VERUM;
}

b32
silva_macro_functio_addere (
    SilvaExpansio*       exp,
    constans character*  titulus,
    constans character** parametra,
    constans character*  corpus)
{
    SilvaMacroDef* def;
    i32 i;

    def = _def_api_creare(exp, titulus, corpus);
    si (def == NIHIL)
    {
        redde FALSUM;
    }
    def->est_functio = VERUM;
    def->parametra = xar_creare(exp->piscina, magnitudo(chorda*));

    per (i = ZEPHYRUM; parametra != NIHIL && parametra[i] != NIHIL; i++)
    {
        chorda** locus;
        constans character* titulus_parametri;

        titulus_parametri = parametra[i];
        si (strcmp(titulus_parametri, "...") == ZEPHYRUM)
        {
            def->est_variadica = VERUM;
            titulus_parametri = "__VA_ARGS__";
        }
        locus = (chorda**)xar_addere(def->parametra);
        si (locus != NIHIL)
        {
            *locus = _chorda_figere(exp->piscina,
                chorda_ex_literis(titulus_parametri, exp->piscina));
        }
    }

    _def_api_registrare(exp, def);
    redde VERUM;
}


/* ==================================================
 * Prospectus macro - oraculum GLR
 * ================================================== */

b32
silva_expansio_prospectare (
    SilvaExpansio*   exp,
    chorda           titulus,
    SilvaProspectus* prospectus_out)
{
    SilvaMacroDef* def;

    def = silva_expansio_quaerere(exp, titulus);
    si (def == NIHIL)
    {
        redde FALSUM;
    }

    prospectus_out->genus = SILVA_LEX_EOF;
    prospectus_out->est_vacuum = VERUM;
    prospectus_out->est_recursivum = FALSUM;

    si (def->corpus != NIHIL && xar_numerus(def->corpus) > ZEPHYRUM)
    {
        SilvaToken* primum;

        primum = *(SilvaToken**)xar_obtinere(def->corpus, ZEPHYRUM);
        prospectus_out->genus = primum->genus;
        prospectus_out->est_vacuum = FALSUM;
        si (_est_nomen_directivae(primum)
            && silva_expansio_quaerere(exp, primum->valor) != NIHIL)
        {
            prospectus_out->est_recursivum = VERUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Chunk B - Generationes expansionis
 * ================================================== */

/* Potestne lexema nomen macro esse? (identificator vel verbum clausum -
 * praeprocessor verba clausa non novit) */
interior b32
_est_nomen_potentiale (SilvaToken* token)
{
    si (token->genus == SILVA_LEX_IDENTIFICATOR)
    {
        redde VERUM;
    }
    si (token->genus >= SILVA_LEX_AUTO && token->genus <= SILVA_LEX_WHILE)
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* Caecatio lexematis (NIHIL = vacua) */
interior SilvaCaecatio*
_caecatio_lexematis (SilvaToken* token)
{
    commutatio (token->origo.genus)
    {
        casus SILVA_ORIGO_EXPANSIO:
            redde token->origo.datum.expansio.caecatio;
        casus SILVA_ORIGO_PASTA:
            redde token->origo.datum.pasta.caecatio;
        casus SILVA_ORIGO_FONS:
        casus SILVA_ORIGO_CHORDA:
        casus SILVA_ORIGO_API:
        ordinarius:
            redde NIHIL;
    }
}

/* Unio caecationum: elementa ex a super b consuntur (si absentia) */
interior SilvaCaecatio*
_caecatio_unire (Piscina* piscina, SilvaCaecatio* a, SilvaCaecatio* b)
{
    dum (a != NIHIL)
    {
        si (a->titulus != NIHIL
            && !silva_caecatio_continet(b, *a->titulus))
        {
            b = silva_caecatio_extendere(piscina, b, a->titulus);
        }
        a = a->cauda;
    }
    redde b;
}

/* Congruitne corpus-lexema parametro? Reddit indicem vel -1 */
interior s32
_parametrum_quaerere (SilvaMacroDef* def, SilvaToken* token)
{
    i32 i;
    i32 n;
    chorda* param;

    si (def->parametra == NIHIL || !_est_nomen_potentiale(token))
    {
        redde -I;
    }
    n = xar_numerus(def->parametra);
    per (i = ZEPHYRUM; i < n; i++)
    {
        param = *(chorda**)xar_obtinere(def->parametra, i);
        si (param->mensura == token->valor.mensura
            && param->mensura > ZEPHYRUM
            && memcmp(param->datum, token->valor.datum,
                   (memoriae_index)param->mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* Prae-declaratio (recursio: argumenta plene expanduntur) */
interior Xar*
_expandere_plene (SilvaExpansio* exp, Xar* lexemata);

/* Colligere argumenta invocationis. lexemata[i_paren] est '('.
 * Reddit Xar de Xar* (unum per argumentum) vel NIHIL si non
 * terminata (robustitas: invocatio abicitur, nomen manet).
 * *i_post_out = index post ')' clausam.
 * scissiones_maximae: numerus scissionum comma permissarum (-1 =
 * sine fine). Pro variadicis: cauda post parametra nominata manet
 * UNUM argumentum cum lexematibus comma VERIS suis. */
interior Xar*
_argumenta_colligere (
    SilvaExpansio* exp,
    Xar*           lexemata,
    i32            i_paren,
    i32*           i_post_out,
    s32            scissiones_maximae)
{
    Xar* argumenta;
    Xar* currens;
    SilvaToken* t;
    i32 i;
    i32 n;
    i32 profunditas;
    s32 scissiones_factae;

    argumenta = xar_creare(exp->piscina, magnitudo(Xar*));
    currens = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    profunditas = I;
    scissiones_factae = ZEPHYRUM;
    n = xar_numerus(lexemata);

    per (i = i_paren + I; i < n; i++)
    {
        t = *(SilvaToken**)xar_obtinere(lexemata, i);
        si (t->genus == SILVA_LEX_EOF)
        {
            frange;  /* non terminata */
        }
        si (t->genus == SILVA_LEX_PAREN_APERTA)
        {
            profunditas++;
        }
        alioquin si (t->genus == SILVA_LEX_PAREN_CLAUSA)
        {
            profunditas--;
            si (profunditas == ZEPHYRUM)
            {
                Xar** locus;

                locus = (Xar**)xar_addere(argumenta);
                si (locus != NIHIL)
                {
                    *locus = currens;
                }
                *i_post_out = i + I;
                redde argumenta;
            }
        }
        alioquin si (t->genus == SILVA_LEX_COMMA && profunditas == I
            && (scissiones_maximae < ZEPHYRUM
                || scissiones_factae < scissiones_maximae))
        {
            Xar** locus;

            locus = (Xar**)xar_addere(argumenta);
            si (locus != NIHIL)
            {
                *locus = currens;
            }
            currens = xar_creare(exp->piscina, magnitudo(SilvaToken*));
            scissiones_factae++;
            perge;
        }
        _lexema_addere(currens, t);
    }

    redde NIHIL;  /* EOF ante ')' - invocatio imperfecta */
}

/* ==================================================
 * Chunk C - # (stringificatio) et ## (pasta)
 * Operanda parametrorum CRUDA sumuntur (C89); ceterae
 * substitutiones argumenta prae-expansa accipiunt.
 * ================================================== */

/* Suntne trivia inter lexemata consecutiva? */
interior b32
_trivia_inter (SilvaToken* prius, SilvaToken* posterius)
{
    si (prius != NIHIL && prius->spatia_post != NIHIL
        && xar_numerus(prius->spatia_post) > ZEPHYRUM)
    {
        redde VERUM;
    }
    si (posterius != NIHIL && posterius->spatia_ante != NIHIL
        && xar_numerus(posterius->spatia_ante) > ZEPHYRUM)
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* Stringificatio (#): argumentum CRUDUM -> littera chordae.
 * Cursus albi unum spatium fiunt; " et \\ effugiuntur;
 * termini exteriores nulli (per constructionem). */
interior SilvaToken*
_stringificare (
    SilvaExpansio* exp,
    Xar*           argumentum_crudum,
    SilvaToken*    invocatio,
    chorda*        nomen_macro)
{
    ChordaAedificator* aed;
    SilvaToken* t;
    SilvaToken* prius;
    SilvaToken* primus;
    chorda valor;
    i32 i;
    i32 j;
    i32 n;

    aed = chorda_aedificator_creare(exp->piscina, LXIV);
    chorda_aedificator_appendere_character(aed, '"');

    prius = NIHIL;
    primus = NIHIL;
    n = (argumentum_crudum == NIHIL) ? ZEPHYRUM : xar_numerus(argumentum_crudum);
    per (i = ZEPHYRUM; i < n; i++)
    {
        t = *(SilvaToken**)xar_obtinere(argumentum_crudum, i);
        si (primus == NIHIL)
        {
            primus = t;
        }
        si (prius != NIHIL && _trivia_inter(prius, t))
        {
            chorda_aedificator_appendere_character(aed, ' ');
        }
        per (j = ZEPHYRUM; j < t->valor.mensura; j++)
        {
            character c;

            c = (character)t->valor.datum[j];
            si (c == '"' || c == '\\')
            {
                chorda_aedificator_appendere_character(aed, '\\');
            }
            chorda_aedificator_appendere_character(aed, c);
        }
        prius = t;
    }

    chorda_aedificator_appendere_character(aed, '"');
    valor = chorda_aedificator_finire(aed);

    /* argumentum vacuum: catena originis in invocatione radicatur */
    si (primus == NIHIL)
    {
        primus = invocatio;
    }

    redde silva_token_ex_stringificatione(exp->piscina, valor, primus,
        nomen_macro);
}

/* Pasta (##): duo lexemata conglutinantur et RE-LEXANTUR.
 * Fructus unicus -> lexema pasta mundum; plures -> best-effort
 * (primum pasta, reliqua ut sunt). Emittit in exitus. */
interior vacuum
_conglutinare (
    SilvaExpansio* exp,
    SilvaToken*    sinister,
    SilvaToken*    dexter,
    chorda*        nomen_macro,
    SilvaCaecatio* hs,
    Xar*           exitus)
{
    ChordaAedificator* aed;
    chorda glutinum;
    Xar* relexata;
    SilvaToken* t;
    SilvaToken* pasta;
    i32 i;
    i32 n_reales;

    aed = chorda_aedificator_creare(exp->piscina, XXXII);
    chorda_aedificator_appendere_chorda(aed, sinister->valor);
    chorda_aedificator_appendere_chorda(aed, dexter->valor);
    glutinum = chorda_aedificator_finire(aed);

    relexata = silva_lexare(exp->piscina, (constans character*)glutinum.datum,
        glutinum.mensura, -I);

    /* numerare lexemata realia (sine EOF) */
    n_reales = xar_numerus(relexata) - I;
    si (n_reales <= ZEPHYRUM)
    {
        redde;  /* glutinum vacuum */
    }

    t = *(SilvaToken**)xar_obtinere(relexata, ZEPHYRUM);
    pasta = silva_token_ex_pasta(exp->piscina, t->genus, t->valor,
        sinister, dexter, nomen_macro, hs);
    _lexema_addere(exitus, pasta);

    /* best-effort: reliqua emittuntur ut sunt */
    per (i = I; i < n_reales; i++)
    {
        _lexema_addere(exitus,
            *(SilvaToken**)xar_obtinere(relexata, i));
    }
}

/* Item substitutionis: series lexematum unius positionis corporis */
interior Xar*
_item_capere (
    SilvaExpansio* exp,
    SilvaMacroDef* def,
    SilvaToken*    corpus_tok,
    Xar*           argumenta,   /* cruda vel expansa */
    b32*           est_param_out)
{
    s32 p;
    Xar* unum;

    *est_param_out = FALSUM;
    si (argumenta != NIHIL)
    {
        p = _parametrum_quaerere(def, corpus_tok);
        si (p >= ZEPHYRUM && p < (s32)xar_numerus(argumenta))
        {
            *est_param_out = VERUM;
            redde *(Xar**)xar_obtinere(argumenta, (i32)p);
        }
    }
    unum = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    _lexema_addere(unum, corpus_tok);
    redde unum;
}

/* Emittere seriem, quodque lexema involutum ex_expansione */
interior vacuum
_seriem_emittere (
    SilvaExpansio* exp,
    Xar*           series,
    i32            a,
    i32            b,
    SilvaToken*    invocatio,
    chorda*        nomen_macro,
    SilvaCaecatio* hs,
    Xar*           exitus)
{
    i32 i;
    SilvaToken* t;
    SilvaToken* novum;

    per (i = a; i < b; i++)
    {
        t = *(SilvaToken**)xar_obtinere(series, i);
        novum = silva_token_ex_expansione(exp->piscina, t, invocatio,
            nomen_macro,
            _caecatio_unire(exp->piscina, _caecatio_lexematis(t), hs));
        _lexema_addere(exitus, novum);
    }
}

/* Substituere: corpus definitionis ambulatur cum # et ## tractatis.
 * argumenta_expansa: pro positionibus normalibus (NIHIL = obiectum-simile);
 * argumenta_cruda: pro operandis # et ## (semantica C89). */
interior vacuum
_substituere (
    SilvaExpansio* exp,
    SilvaMacroDef* def,
    SilvaToken*    invocatio,
    Xar*           argumenta_expansa,
    Xar*           argumenta_cruda,
    Xar*           exitus)
{
    SilvaCaecatio* hs_nova;
    SilvaToken* corpus_tok;
    SilvaToken* sequens;
    i32 i;
    i32 n;

    hs_nova = silva_caecatio_extendere(exp->piscina,
        _caecatio_lexematis(invocatio), def->titulus);

    n = xar_numerus(def->corpus);
    i = ZEPHYRUM;
    dum (i < n)
    {
        corpus_tok = *(SilvaToken**)xar_obtinere(def->corpus, i);

        /* Stringificatio: # param (functio-similis solum) */
        si (corpus_tok->genus == SILVA_LEX_CANCELLUM
            && def->est_functio && i + I < n)
        {
            SilvaToken* post;
            s32 p;

            post = *(SilvaToken**)xar_obtinere(def->corpus, i + I);
            p = _parametrum_quaerere(def, post);
            si (p >= ZEPHYRUM && argumenta_cruda != NIHIL
                && p < (s32)xar_numerus(argumenta_cruda))
            {
                _lexema_addere(exitus, _stringificare(exp,
                    *(Xar**)xar_obtinere(argumenta_cruda, (i32)p),
                    invocatio, def->titulus));
                i = i + II;
                perge;
            }
        }

        /* Pasta: item ## item (## item)* - operanda cruda */
        sequens = (i + I < n)
            ? *(SilvaToken**)xar_obtinere(def->corpus, i + I) : NIHIL;
        si (sequens != NIHIL
            && sequens->genus == SILVA_LEX_CANCELLUM_CANCELLUM
            && i + II < n)
        {
            Xar* sinistra;
            b32 est_param;

            sinistra = _item_capere(exp, def, corpus_tok,
                argumenta_cruda, &est_param);
            i = i + I;  /* super primum operandum */

            dum (i < n
                && (*(SilvaToken**)xar_obtinere(def->corpus, i))->genus
                    == SILVA_LEX_CANCELLUM_CANCELLUM
                && i + I < n)
            {
                Xar* dextra;
                Xar* nova_sinistra;
                SilvaToken* op_sin;
                SilvaToken* op_dex;
                i32 j;

                dextra = _item_capere(exp, def,
                    *(SilvaToken**)xar_obtinere(def->corpus, i + I),
                    argumenta_cruda, &est_param);
                i = i + II;

                /* latus vacuum: alterum solum manet */
                si (xar_numerus(sinistra) == ZEPHYRUM)
                {
                    sinistra = dextra;
                    perge;
                }
                si (xar_numerus(dextra) == ZEPHYRUM)
                {
                    perge;
                }

                /* praecedentia sinistrae + pasta + sequentia dextrae */
                nova_sinistra = xar_creare(exp->piscina,
                    magnitudo(SilvaToken*));
                per (j = ZEPHYRUM; j < xar_numerus(sinistra) - I; j++)
                {
                    _lexema_addere(nova_sinistra,
                        *(SilvaToken**)xar_obtinere(sinistra, j));
                }
                op_sin = *(SilvaToken**)xar_obtinere(sinistra,
                    xar_numerus(sinistra) - I);
                op_dex = *(SilvaToken**)xar_obtinere(dextra, ZEPHYRUM);

                _conglutinare(exp, op_sin, op_dex, def->titulus, hs_nova,
                    nova_sinistra);

                per (j = I; j < xar_numerus(dextra); j++)
                {
                    _lexema_addere(nova_sinistra,
                        *(SilvaToken**)xar_obtinere(dextra, j));
                }
                sinistra = nova_sinistra;
            }

            /* emittere seriem confectam. Lexemata pasta iam involuta;
             * cetera involvenda. Simplex: pasta lexemata (origo PASTA)
             * transmittuntur, cetera ex_expansione involvuntur. */
            {
                i32 j;
                SilvaToken* t;

                per (j = ZEPHYRUM; j < xar_numerus(sinistra); j++)
                {
                    t = *(SilvaToken**)xar_obtinere(sinistra, j);
                    si (t->origo.genus == SILVA_ORIGO_PASTA)
                    {
                        _lexema_addere(exitus, t);
                    }
                    alioquin
                    {
                        _seriem_emittere(exp, sinistra, j, j + I,
                            invocatio, def->titulus, hs_nova, exitus);
                    }
                }
            }
            perge;
        }

        /* Positio normalis: param -> argumentum expansum; aliter corpus */
        {
            Xar* series;
            b32 est_param;

            series = _item_capere(exp, def, corpus_tok,
                argumenta_expansa, &est_param);
            _seriem_emittere(exp, series, ZEPHYRUM, xar_numerus(series),
                invocatio, def->titulus, hs_nova, exitus);
        }
        i++;
    }
}

/* ==================================================
 * Macros praedefinita (officina pre-M1, 2026-07-08): __FILE__,
 * __LINE__, __STDC__ - semantica C89 per catenam originis (radix =
 * situs invocationis, ergo __LINE__ intra corpus macro lineam
 * INVOCATIONIS reddit). Tabula usoris vincit (quaerere prius
 * temptatur); __DATE__/__TIME__ CONSULTO absunt (determinismus -
 * vide phase-log). Nota: "defined(__FILE__)" in #if adhuc FALSUM
 * est (tabulam solam inspicit) - limes notatus.
 * ================================================== */

interior b32
_valor_aequat (constans SilvaToken* token, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    si ((memoriae_index)token->valor.mensura != m)
    {
        redde FALSUM;
    }
    redde (memcmp(token->valor.datum, litterae, m) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* Chorda in piscinam figere (valor lexematis vivere debet) */
interior chorda
_chordam_figere (SilvaExpansio* exp, constans character* litterae)
{
    memoriae_index m = strlen(litterae);
    chorda c;
    i8* datum;

    datum = (i8*)piscina_allocare(exp->piscina,
        (m > ZEPHYRUM) ? m : (memoriae_index)I);
    c.mensura = ZEPHYRUM;
    c.datum = datum;
    si (datum == NIHIL)
    {
        redde c;
    }
    si (m > ZEPHYRUM)
    {
        memcpy(datum, litterae, m);
    }
    c.mensura = (i32)m;
    redde c;
}

interior chorda*
_titulum_figere (SilvaExpansio* exp, constans character* litterae)
{
    chorda* c;

    c = (chorda*)piscina_allocare(exp->piscina,
        (memoriae_index)magnitudo(chorda));
    si (c == NIHIL)
    {
        redde NIHIL;
    }
    *c = _chordam_figere(exp, litterae);
    redde c;
}

/* Substituere praedefinitum si token id est; VERUM = emissum */
interior b32
_praedefinitum_substituere (
    SilvaExpansio* exp,
    SilvaToken*    token,
    Xar*           exitus)
{
    SilvaToken* radix;
    SilvaToken* novum;

    radix = silva_token_radix(token);
    si (radix == NIHIL)
    {
        radix = token;
    }

    si (_valor_aequat(token, "__LINE__"))
    {
        character buffer[XVI];

        sprintf(buffer, "%u", radix->linea);
        novum = silva_token_ex_praedefinito(exp->piscina,
            SILVA_LEX_INTEGER, _chordam_figere(exp, buffer), token,
            _titulum_figere(exp, "__LINE__"));
    }
    alioquin si (_valor_aequat(token, "__STDC__"))
    {
        novum = silva_token_ex_praedefinito(exp->piscina,
            SILVA_LEX_INTEGER, _chordam_figere(exp, "1"), token,
            _titulum_figere(exp, "__STDC__"));
    }
    alioquin si (_valor_aequat(token, "__FILE__"))
    {
        constans chorda* via;
        ChordaAedificator* aed;
        i32 j;

        via = silva_fons_via(exp, radix->fons_index);
        si (via == NIHIL)
        {
            redde FALSUM;   /* fons ignotus: identificator manet */
        }
        aed = chorda_aedificator_creare(exp->piscina, LXIV);
        chorda_aedificator_appendere_character(aed, '"');
        per (j = ZEPHYRUM; j < via->mensura; j++)
        {
            character c = (character)via->datum[j];

            si (c == '"' || c == '\\')
            {
                chorda_aedificator_appendere_character(aed, '\\');
            }
            chorda_aedificator_appendere_character(aed, c);
        }
        chorda_aedificator_appendere_character(aed, '"');
        novum = silva_token_ex_praedefinito(exp->piscina,
            SILVA_LEX_STRING_LIT, chorda_aedificator_finire(aed),
            token, _titulum_figere(exp, "__FILE__"));
    }
    alioquin
    {
        redde FALSUM;
    }

    si (novum == NIHIL)
    {
        redde FALSUM;
    }
    _lexema_addere(exitus, novum);
    redde VERUM;
}

/* Nucleus generationis. tabula (si non NIHIL) = tabula operans
 * positionalis, activata per ambulationem (quaerere eam videt, etiam
 * in expansione argumentorum nidificata); positus_localis = positiones
 * eventorum in FLUXU HOC, remappatae in situ ad positiones exitus
 * (generatio proxima eas legit). Ambo NIHIL = semantica tabulae vivae. */
interior Xar*
_generatio_interna (
    SilvaExpansio*  exp,
    Xar*            lexemata,
    b32*            mutatum_out,
    TabulaDispersa* tabula,
    s32*            positus_localis)
{
    Xar* exitus;
    SilvaToken* token;
    SilvaMacroDef* def;
    TabulaDispersa* tabula_prior;
    i32 i;
    i32 n;
    i32 cursor;
    i32 n_acta;
    b32 mutatum;

    exitus = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    mutatum = FALSUM;
    n = xar_numerus(lexemata);
    i = ZEPHYRUM;

    tabula_prior = exp->tabula_activa;
    si (tabula != NIHIL)
    {
        exp->tabula_activa = tabula;
    }
    cursor = ZEPHYRUM;
    n_acta = xar_numerus(exp->acta);

    dum (i < n)
    {
        /* eventa ante lexema i applicare (status ad punctum) */
        si (positus_localis != NIHIL && tabula != NIHIL)
        {
            dum (cursor < n_acta && positus_localis[cursor] <= (s32)i)
            {
                SilvaEventum* eventum;

                eventum = (SilvaEventum*)xar_obtinere(exp->acta, cursor);
                si (eventum->genus == SILVA_EVENTUM_DEFINITIO)
                {
                    tabula_dispersa_inserere(tabula, *eventum->titulus,
                        (vacuum*)eventum->def);
                }
                alioquin
                {
                    tabula_dispersa_delere(tabula, *eventum->titulus);
                }
                positus_localis[cursor] = (s32)xar_numerus(exitus);
                cursor++;
            }
        }

        token = *(SilvaToken**)xar_obtinere(lexemata, i);

        si (_est_nomen_potentiale(token))
        {
            def = silva_expansio_quaerere(exp, token->valor);
            si (def != NIHIL
                && !silva_caecatio_continet(_caecatio_lexematis(token),
                       *def->titulus))
            {
                si (!def->est_functio)
                {
                    _substituere(exp, def, token, NIHIL, NIHIL, exitus);
                    mutatum = VERUM;
                    i++;
                    perge;
                }
                alioquin si (i + I < n)
                {
                    SilvaToken* proximum;

                    proximum = *(SilvaToken**)xar_obtinere(lexemata, i + I);
                    si (proximum->genus == SILVA_LEX_PAREN_APERTA)
                    {
                        Xar* argumenta;
                        i32 i_post;
                        s32 scissiones;

                        /* variadica: scissiones = parametra nominata
                         * (cauda manet unum argumentum __VA_ARGS__) */
                        scissiones = -I;
                        si (def->est_variadica)
                        {
                            scissiones =
                                (s32)xar_numerus(def->parametra) - I;
                        }

                        i_post = i;
                        argumenta = _argumenta_colligere(exp, lexemata,
                            i + I, &i_post, scissiones);
                        si (argumenta != NIHIL)
                        {
                            Xar* expansa;
                            Xar* arg;
                            Xar** locus;
                            i32 j;
                            i32 m;

                            /* Extentum strati 0 memorare (Phase 5,
                             * sim ⑥ C5): reconstructio fontis octetos
                             * invocationis [nomen..')'] petit - solum
                             * cum nomen ipsum lexema FONTIS est */
                            si (token->origo.genus == SILVA_ORIGO_FONS)
                            {
                                SilvaExtentumInvocationis* ext;

                                ext = (SilvaExtentumInvocationis*)
                                    xar_addere(exp->extenta);
                                si (ext != NIHIL)
                                {
                                    ext->invocatio = token;
                                    ext->lamina = _lamina_capere(exp,
                                        lexemata, i, i_post);
                                }
                            }

                            /* argumenta PLENE prae-expandere (S13) */
                            expansa = xar_creare(exp->piscina, magnitudo(Xar*));
                            m = xar_numerus(argumenta);

                            /* macro sine parametris + unum argumentum
                             * vacuum = invocatio vacua */
                            si (!(m == I && xar_numerus(def->parametra) == ZEPHYRUM
                                && xar_numerus(*(Xar**)xar_obtinere(argumenta, 0))
                                    == ZEPHYRUM))
                            {
                                per (j = ZEPHYRUM; j < m; j++)
                                {
                                    arg = *(Xar**)xar_obtinere(argumenta, j);
                                    locus = (Xar**)xar_addere(expansa);
                                    si (locus != NIHIL)
                                    {
                                        *locus = _expandere_plene(exp, arg);
                                    }
                                }
                            }

                            _substituere(exp, def, token, expansa, argumenta, exitus);
                            mutatum = VERUM;
                            i = i_post;
                            perge;
                        }
                        /* imperfecta: nomen manet, cadit infra */
                    }
                }
            }

            /* praedefinita (__FILE__/__LINE__/__STDC__) - solum si
             * nullum macro usoris (tabula vincit); omnia VIII
             * litterarum, "__" praefixa */
            si (def == NIHIL
                && token->valor.mensura == VIII
                && token->valor.datum[ZEPHYRUM] == '_'
                && token->valor.datum[I] == '_'
                && _praedefinitum_substituere(exp, token, exitus))
            {
                mutatum = VERUM;
                i++;
                perge;
            }
        }

        _lexema_addere(exitus, token);
        i++;
    }

    /* eventa post finem fluxus: remappare ad finem exitus */
    si (positus_localis != NIHIL)
    {
        dum (cursor < n_acta)
        {
            positus_localis[cursor] = (s32)xar_numerus(exitus);
            cursor++;
        }
    }
    exp->tabula_activa = tabula_prior;

    si (mutatum_out != NIHIL)
    {
        *mutatum_out = mutatum;
    }
    redde exitus;
}

Xar*
silva_expansio_generatio (
    SilvaExpansio* exp,
    Xar*           lexemata,
    b32*           mutatum_out)
{
    redde _generatio_interna(exp, lexemata, mutatum_out, NIHIL, NIHIL);
}

/* Limina circuituum fixorum (Phase 7): ante quamque generationem
 * inspiciuntur - volumen (limen_lexematum), numerus generationum
 * (limen_generationum, olim assertio), intermissio (pergere).
 * Limine tacto expansio CESSAT sed fluxus manet - lexemata reliqua
 * inexpansa fluunt (degradatio, non amputatio). Reddit VERUM si
 * pergendum. */
interior b32
_generationem_licere (
    SilvaExpansio* exp,
    Xar*           currens,
    i32            generationes)
{
    si (exp->pergere != NIHIL && !exp->pergere(exp->pergere_datum))
    {
        exp->est_intermissa = VERUM;
        redde FALSUM;
    }
    si (exp->limen_generationum > ZEPHYRUM
        && generationes >= exp->limen_generationum)
    {
        exp->expansio_decisa = VERUM;
        exp->fines_tactae = VERUM;
        redde FALSUM;
    }
    si (exp->limen_lexematum > ZEPHYRUM
        && (i32)xar_numerus(currens) > exp->limen_lexematum)
    {
        exp->expansio_decisa = VERUM;
        exp->fines_tactae = VERUM;
        redde FALSUM;
    }
    redde VERUM;
}

Xar*
silva_expansio_expandere (
    SilvaExpansio* exp,
    Xar*           lexemata,
    Xar**          strata_out)
{
    Xar* currens;
    Xar* strata;
    b32 mutatum;
    i32 generationes;

    strata = NIHIL;
    si (strata_out != NIHIL)
    {
        strata = xar_creare(exp->piscina, magnitudo(Xar*));
        *strata_out = strata;
    }

    currens = lexemata;
    generationes = ZEPHYRUM;
    dum (_generationem_licere(exp, currens, generationes))
    {
        Xar* exitus;

        mutatum = FALSUM;
        exitus = silva_expansio_generatio(exp, currens, &mutatum);
        si (!mutatum)
        {
            frange;
        }
        currens = exitus;
        si (strata != NIHIL)
        {
            Xar** locus;

            locus = (Xar**)xar_addere(strata);
            si (locus != NIHIL)
            {
                *locus = currens;
            }
        }
        generationes++;
    }

    redde currens;
}

interior Xar*
_expandere_plene (SilvaExpansio* exp, Xar* lexemata)
{
    redde silva_expansio_expandere(exp, lexemata, NIHIL);
}

Xar*
silva_expansio_expandere_reliqua (
    SilvaExpansio* exp,
    Xar*           reliqua,
    Xar**          strata_out)
{
    Xar* currens;
    Xar* strata;
    s32* positus_localis;
    b32 mutatum;
    i32 generationes;
    i32 n_acta;
    i32 k;

    strata = NIHIL;
    si (strata_out != NIHIL)
    {
        strata = xar_creare(exp->piscina, magnitudo(Xar*));
        *strata_out = strata;
    }

    /* positiones locales eventorum: remappantur per generationem */
    n_acta = xar_numerus(exp->acta);
    positus_localis = (s32*)piscina_allocare(exp->piscina,
        (memoriae_index)(magnitudo(s32)
            * (n_acta > ZEPHYRUM ? (memoriae_index)n_acta : I)));
    si (positus_localis == NIHIL)
    {
        redde reliqua;
    }
    per (k = ZEPHYRUM; k < n_acta; k++)
    {
        positus_localis[k] =
            ((SilvaEventum*)xar_obtinere(exp->acta, k))->positus;
    }

    currens = reliqua;
    generationes = ZEPHYRUM;
    dum (_generationem_licere(exp, currens, generationes))
    {
        TabulaDispersa* tabula;
        Xar* exitus;

        mutatum = FALSUM;
        tabula = tabula_dispersa_creare_chorda(exp->piscina, LXIV);
        exitus = _generatio_interna(exp, currens, &mutatum, tabula,
            positus_localis);
        si (!mutatum)
        {
            frange;
        }
        currens = exitus;
        si (strata != NIHIL)
        {
            Xar** locus;

            locus = (Xar**)xar_addere(strata);
            si (locus != NIHIL)
            {
                *locus = currens;
            }
        }
        generationes++;
    }

    redde currens;
}

/* ==================================================
 * Fenestrae lectionis (additiones II)
 * ================================================== */

i32
silva_fontes_numerus (constans SilvaExpansio* exp)
{
    redde xar_numerus(exp->fontes);
}

constans chorda*
silva_fons_via (constans SilvaExpansio* exp, s32 fons_index)
{
    constans SilvaFons* fons;

    si (fons_index < ZEPHYRUM
        || fons_index >= (s32)xar_numerus(exp->fontes))
    {
        redde NIHIL;
    }
    fons = (constans SilvaFons*)xar_obtinere(exp->fontes,
        (i32)fons_index);
    redde fons->via;
}

i32
silva_inclusiones_numerus (constans SilvaExpansio* exp)
{
    redde xar_numerus(exp->inclusiones);
}

/* Catena inclusionum AD fontem datum: viae ab radice ad
 * includentem proximum, " > " separatae (fons ipse exclusus - via
 * eius iam in diagnostico stat). 0 = radix ipse / ignotus /
 * spatium deficiens (buffer tunc vacuus, nihil dimidium). Custos
 * cycli: profunditas XXXII; margines plures ad eundem fontem -
 * primus vincit (indicium lectori, non probatio). */
insignatus integer
silva_inclusionis_catena_scribere (constans SilvaExpansio* exp,
    s32 fons_index, character* buffer, insignatus integer capacitas)
{
    s32 maiores[XXXII];
    i32 numerus = ZEPHYRUM;
    s32 currens;
    i32 n_incl;
    i32 i;
    insignatus integer scriptum = ZEPHYRUM;

    si (exp == NIHIL || buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    buffer[ZEPHYRUM] = '\0';
    si (fons_index < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    currens = fons_index;
    n_incl = xar_numerus(exp->inclusiones);
    dum (numerus < (i32)XXXII)
    {
        s32 pater = -I;

        per (i = ZEPHYRUM; i < n_incl; i++)
        {
            constans SilvaInclusio* incl = (constans SilvaInclusio*)
                xar_obtinere(exp->inclusiones, i);

            si (incl != NIHIL && incl->fons_ad == currens
                && incl->fons_ex != currens)
            {
                pater = incl->fons_ex;
                frange;
            }
        }
        si (pater < ZEPHYRUM)
        {
            frange;
        }
        maiores[numerus] = pater;
        numerus++;
        currens = pater;
    }
    si (numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    /* maiores[numerus-1] = radix ... maiores[0] = includens */
    per (i = numerus; i > ZEPHYRUM; i--)
    {
        constans chorda* v = silva_fons_via(exp, maiores[i - I]);
        insignatus integer vm;

        si (v == NIHIL)
        {
            perge;
        }
        vm = (insignatus integer)v->mensura;
        si (scriptum + vm + IV >= capacitas)
        {
            buffer[ZEPHYRUM] = '\0';
            redde ZEPHYRUM;
        }
        si (scriptum > ZEPHYRUM)
        {
            buffer[scriptum] = ' ';
            buffer[scriptum + I] = '>';
            buffer[scriptum + II] = ' ';
            scriptum = scriptum + III;
        }
        memcpy(buffer + scriptum, v->datum, (memoriae_index)vm);
        scriptum = scriptum + vm;
    }
    buffer[scriptum] = '\0';
    redde scriptum;
}

b32
silva_inclusio_vista (constans SilvaExpansio* exp, i32 index,
    SilvaInclusioVista* vista_out)
{
    constans SilvaInclusio* inclusio;

    si (index >= xar_numerus(exp->inclusiones))
    {
        redde FALSUM;
    }
    inclusio = (constans SilvaInclusio*)xar_obtinere(
        exp->inclusiones, index);
    vista_out->via = inclusio->via;
    vista_out->fons_ex = inclusio->fons_ex;
    vista_out->fons_ad = inclusio->fons_ad;
    vista_out->est_praetermissa = inclusio->est_praetermissa;
    vista_out->est_angulata = inclusio->est_angulata;
    redde VERUM;
}

i32
silva_rami_numerus (constans SilvaExpansio* exp)
{
    redde xar_numerus(exp->rami);
}

b32
silva_ramus_vista (constans SilvaExpansio* exp, i32 index,
    SilvaRamusVista* vista_out)
{
    constans SilvaRamus* ramus;

    si (index >= xar_numerus(exp->rami))
    {
        redde FALSUM;
    }
    ramus = *(SilvaRamus* constans*)xar_obtinere(exp->rami, index);
    vista_out->genus = ramus->genus;
    vista_out->est_sumptum = ramus->est_sumptum;
    vista_out->est_numquam = ramus->est_numquam;
    vista_out->corpus_initium = ramus->corpus_initium;
    vista_out->corpus_finis = ramus->corpus_finis;
    si (ramus->regio != NIHIL)
    {
        vista_out->fons_index = ramus->regio->fons_index;
        vista_out->linea = ramus->regio->linea;
    }
    alioquin
    {
        vista_out->fons_index = -I;
        vista_out->linea = ZEPHYRUM;
    }
    redde VERUM;
}

i32
silva_macros_numerus (constans SilvaExpansio* exp)
{
    i32 n = xar_numerus(exp->acta);
    i32 numerus = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        constans SilvaEventum* eventum =
            (constans SilvaEventum*)xar_obtinere(exp->acta, k);

        si (eventum->genus == SILVA_EVENTUM_DEFINITIO)
        {
            numerus++;
        }
    }
    redde numerus;
}

b32
silva_macro_vista (constans SilvaExpansio* exp, i32 index,
    SilvaMacroVista* vista_out)
{
    i32 n = xar_numerus(exp->acta);
    i32 visae = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        constans SilvaEventum* eventum =
            (constans SilvaEventum*)xar_obtinere(exp->acta, k);

        si (eventum->genus != SILVA_EVENTUM_DEFINITIO)
        {
            perge;
        }
        si (visae == index)
        {
            vista_out->titulus = eventum->titulus;
            vista_out->est_functio = (eventum->def != NIHIL)
                ? eventum->def->est_functio : FALSUM;
            vista_out->fons_index = eventum->fons_index;
            vista_out->linea = eventum->linea;
            /* extenta corporis ex laminis (lexemata cruda fontis -
             * positiones exactae, continuationes '\' inclusae) */
            vista_out->corpus_initium = -I;
            vista_out->corpus_finis = -I;
            si (eventum->def != NIHIL && !eventum->def->ex_api
                && eventum->def->corpus != NIHIL
                && xar_numerus(eventum->def->corpus) > ZEPHYRUM)
            {
                SilvaToken* primum = *(SilvaToken**)xar_obtinere(
                    eventum->def->corpus, ZEPHYRUM);
                SilvaToken* ultimum = *(SilvaToken**)xar_obtinere(
                    eventum->def->corpus,
                    xar_numerus(eventum->def->corpus) - I);

                si (primum != NIHIL && ultimum != NIHIL
                    && primum->byte_offset >= ZEPHYRUM
                    && ultimum->byte_offset >= ZEPHYRUM)
                {
                    vista_out->corpus_initium =
                        primum->byte_offset;
                    vista_out->corpus_finis = ultimum->byte_offset
                        + (s32)ultimum->longitudo;
                }
            }
            redde VERUM;
        }
        visae++;
    }
    redde FALSUM;
}
