/* silva_expandere.c - Expansor silvae: Chunk A
 * Acta + directivae + definitiones macro.
 */

#include "silva_expandere.h"
#include "silva_lexema.h"
#include "chorda_aedificator.h"
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
    unio { constans character* c; i8* m; } u;

    u.c = via;
    temp.datum = u.m;
    temp.mensura = (i32)strlen(via);
    via_fixa = _chorda_figere(exp->piscina, temp);

    locus = (SilvaFons*)xar_addere(exp->fontes);
    si (locus == NIHIL)
    {
        redde -I;
    }
    locus->via = via_fixa;
    locus->est_syntheticus = est_syntheticus;
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
    SilvaMacroDef*    def)
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
    locus->conditio_id = ZEPHYRUM;
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

    si (tabula_dispersa_invenire(exp->macros, titulus, &valor))
    {
        redde (SilvaMacroDef*)valor;
    }
    redde NIHIL;
}


/* ==================================================
 * Processio directivarum
 * ================================================== */

/* Estne lexema verum initium directivae? (# ad initium lineae LOGICAE) */
interior b32
_est_initium_directivae (SilvaToken* token)
{
    redde (token->genus == SILVA_LEX_CANCELLUM && token->initium_lineae)
        ? VERUM : FALSUM;
}

/* Parsare definitionem: lexemata[i] est CANCELLUM; linea logica currit
 * usque ad finem (exclusive). Reddit VERUM si define/undef recognita. */
interior b32
_directivam_processare (
    SilvaExpansio* exp,
    Xar*           lexemata,
    i32            i_cancellum,
    i32            i_finis)
{
    SilvaToken* verbum;
    SilvaToken* titulus_tok;
    SilvaMacroDef* def;
    chorda* titulus;
    i32 i;

    /* # verbum titulus ... - minimum # + verbum + titulus */
    si (i_cancellum + II >= i_finis)
    {
        redde FALSUM;
    }
    verbum = *(SilvaToken**)xar_obtinere(lexemata, i_cancellum + I);
    titulus_tok = *(SilvaToken**)xar_obtinere(lexemata, i_cancellum + II);
    si (titulus_tok->genus != SILVA_LEX_IDENTIFICATOR
        && titulus_tok->genus < SILVA_LEX_AUTO)
    {
        redde FALSUM;  /* titulus debet esse identificator (vel verbum clausum) */
    }

    si (verbum->genus == SILVA_LEX_IDENTIFICATOR
        && _chorda_est_literis(verbum->valor, "undef"))
    {
        titulus = _chorda_figere(exp->piscina, titulus_tok->valor);
        tabula_dispersa_delere(exp->macros, titulus_tok->valor);
        _eventum_scribere(exp, SILVA_EVENTUM_DELETIO,
            titulus_tok->fons_index, titulus_tok->linea, titulus, NIHIL);
        redde VERUM;
    }

    si (!(verbum->genus == SILVA_LEX_IDENTIFICATOR
        && _chorda_est_literis(verbum->valor, "define")))
    {
        redde FALSUM;  /* alia directiva (include/if/...): Chunk D */
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
                    def->est_variadica = VERUM;
                }
                /* COMMA transitur */
                i++;
            }
        }
    }

    /* Corpus: reliqua lineae logicae */
    per (; i < i_finis; i++)
    {
        SilvaToken** locus;

        locus = (SilvaToken**)xar_addere(def->corpus);
        si (locus != NIHIL)
        {
            *locus = *(SilvaToken**)xar_obtinere(lexemata, i);
        }
    }

    tabula_dispersa_inserere(exp->macros, *titulus, (vacuum*)def);
    _eventum_scribere(exp, SILVA_EVENTUM_DEFINITIO,
        def->fons_index, def->linea_def, titulus, def);
    redde VERUM;
}

Xar*
silva_expansio_definitiones_colligere (
    SilvaExpansio* exp,
    Xar*           lexemata,
    Xar**          directivae_out)
{
    Xar* reliqua;
    Xar* directivae;
    SilvaToken* token;
    SilvaToken** locus;
    i32 i;
    i32 j;
    i32 n;
    i32 i_finis;

    reliqua = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    directivae = NIHIL;
    si (directivae_out != NIHIL)
    {
        directivae = xar_creare(exp->piscina, magnitudo(Xar*));
        *directivae_out = directivae;
    }

    n = xar_numerus(lexemata);
    i = ZEPHYRUM;
    dum (i < n)
    {
        token = *(SilvaToken**)xar_obtinere(lexemata, i);

        si (_est_initium_directivae(token))
        {
            /* Extensio lineae logicae: usque ad proximum initium_lineae
             * (vel EOF vel finem) */
            i_finis = i + I;
            dum (i_finis < n)
            {
                SilvaToken* t;

                t = *(SilvaToken**)xar_obtinere(lexemata, i_finis);
                si (t->initium_lineae || t->genus == SILVA_LEX_EOF)
                {
                    frange;
                }
                i_finis++;
            }

            si (_directivam_processare(exp, lexemata, i, i_finis))
            {
                /* directiva consumpta: lexemata eius servare si petitum */
                si (directivae != NIHIL)
                {
                    Xar* linea_directivae;
                    Xar** locus_lineae;

                    linea_directivae = xar_creare(exp->piscina,
                        magnitudo(SilvaToken*));
                    per (j = i; j < i_finis; j++)
                    {
                        locus = (SilvaToken**)xar_addere(linea_directivae);
                        si (locus != NIHIL)
                        {
                            *locus = *(SilvaToken**)xar_obtinere(lexemata, j);
                        }
                    }
                    locus_lineae = (Xar**)xar_addere(directivae);
                    si (locus_lineae != NIHIL)
                    {
                        *locus_lineae = linea_directivae;
                    }
                }
                i = i_finis;
                perge;
            }
            /* non recognita: transit ad reliqua (Chunk D directivas
             * alias tractabit) */
        }

        locus = (SilvaToken**)xar_addere(reliqua);
        si (locus != NIHIL)
        {
            *locus = token;
        }
        i++;
    }

    redde reliqua;
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

/* Prae-declaratio (recursio: argumenta plene expanduntur) */
interior Xar*
_expandere_plene (SilvaExpansio* exp, Xar* lexemata);

/* Colligere argumenta invocationis. lexemata[i_paren] est '('.
 * Reddit Xar de Xar* (unum per argumentum) vel NIHIL si non
 * terminata (robustitas: invocatio abicitur, nomen manet). 
 * *i_post_out = index post ')' clausam. */
interior Xar*
_argumenta_colligere (
    SilvaExpansio* exp,
    Xar*           lexemata,
    i32            i_paren,
    i32*           i_post_out)
{
    Xar* argumenta;
    Xar* currens;
    SilvaToken* t;
    i32 i;
    i32 n;
    i32 profunditas;

    argumenta = xar_creare(exp->piscina, magnitudo(Xar*));
    currens = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    profunditas = I;
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
        alioquin si (t->genus == SILVA_LEX_COMMA && profunditas == I)
        {
            Xar** locus;

            locus = (Xar**)xar_addere(argumenta);
            si (locus != NIHIL)
            {
                *locus = currens;
            }
            currens = xar_creare(exp->piscina, magnitudo(SilvaToken*));
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

Xar*
silva_expansio_generatio (
    SilvaExpansio* exp,
    Xar*           lexemata,
    b32*           mutatum_out)
{
    Xar* exitus;
    SilvaToken* token;
    SilvaMacroDef* def;
    i32 i;
    i32 n;
    b32 mutatum;

    exitus = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    mutatum = FALSUM;
    n = xar_numerus(lexemata);
    i = ZEPHYRUM;

    dum (i < n)
    {
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

                        i_post = i;
                        argumenta = _argumenta_colligere(exp, lexemata,
                            i + I, &i_post);
                        si (argumenta != NIHIL)
                        {
                            Xar* expansa;
                            Xar* arg;
                            Xar** locus;
                            i32 j;
                            i32 m;

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
        }

        _lexema_addere(exitus, token);
        i++;
    }

    si (mutatum_out != NIHIL)
    {
        *mutatum_out = mutatum;
    }
    redde exitus;
}

/* Cap generationum: assertio contra regressum caecationum, non
 * semantica (caecationes terminant; C generationes = defectus) */
#define SILVA_GENERATIONES_MAXIMAE C

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
    dum (generationes < SILVA_GENERATIONES_MAXIMAE)
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
