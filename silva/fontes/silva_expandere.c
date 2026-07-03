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
 * ================================================== */

#define SILVA_INCLUDENDI_MAXIMI XXXII

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
    SILVA_DIR_IGNOTA       /* alia (line/pragma/error/...) */
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
 * eius normaliter processantur; ceteri laminas crudas retinent. */
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
    SilvaRegio* regio;
    b32 sumptum_iam;
    i32 i_currens;

    regio = _regionem_creare(exp,
        *(SilvaToken**)xar_obtinere(lexemata, i), pater);
    si (regio == NIHIL)
    {
        redde i_finis;
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
        _directivam_capere(exp, directivae, lexemata, i_currens, i_linea_finis);

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

        /* Evaluatio (via defalta) - rami post sumptum NON evaluantur */
        si (!sumptum_iam)
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
                _directivam_capere(exp, directivae, lexemata, i_scan, lf);
                redde lf;
            }
            i_currens = i_scan;  /* ramus proximus (elif/else) */
        }
    }

    regio->est_imperfecta = VERUM;
    redde i_finis;
}

/* ==================================================
 * Chunk D - Includenda
 * ================================================== */

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
    vacuum* valor;
    i32 i_op;

    cancellum = *(SilvaToken**)xar_obtinere(lexemata, i_cancellum);
    habet_viam = FALSUM;
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
        }
        alioquin si (t->genus == SILVA_LEX_MINOR)
        {
            /* <via> - valores concatenati usque ad '>' */
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

    si (!tabula_dispersa_invenire(exp->includenda, via, &valor))
    {
        redde;  /* ignotum: via memorata, processio pergit */
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

    si (exp->profunditas_includendi >= SILVA_INCLUDENDI_MAXIMI)
    {
        inclusio->est_praetermissa = VERUM;  /* profunditas nimia */
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
    tabula_dispersa_inserere(exp->includenda, *fons->via, (vacuum*)incl);
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
    dum (generationes < SILVA_GENERATIONES_MAXIMAE)
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
