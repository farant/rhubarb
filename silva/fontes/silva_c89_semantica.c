/* silva_c89_semantica.c - Semantica C89: typi + forma + aestimator
 * (M0a Chunk A - vide silva_c89_semantica.h + consilium) */

#include "silva_c89_semantica.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_oraculum.h"
#include "silva_c89_fluxus.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Auxiliares
 * ================================================== */

/* AMBIGUUS -> lectio canonica (politica: descensus canonicus;
 * cave - canonica generis furcae decl/expr est lectio EXPRESSIONIS) */
interior constans SilvaNodus*
_canonicum (constans SilvaNodus* nodus)
{
    i32 custos = ZEPHYRUM;

    dum (nodus != NIHIL && nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS
        && custos < XXXII)
    {
        SilvaValor interp = silva_c89_ambiguus_interpretationes(nodus);
        SilvaValor canonica = silva_c89_ambiguus_canonica(nodus);
        SilvaValor* lectio;

        si (canonica.genus != SILVA_VALOR_INDEX)
        {
            redde nodus;
        }
        lectio = silva_valor_lista_obtinere(interp,
            (i32)canonica.datum.index);
        si (lectio == NIHIL || lectio->genus != SILVA_VALOR_NODUS)
        {
            redde nodus;
        }
        nodus = lectio->datum.nodus;
        custos++;
    }
    redde nodus;
}

interior chorda
_chorda_vacua (vacuum)
{
    chorda c;

    c.mensura = ZEPHYRUM;
    c.datum = NIHIL;
    redde c;
}

/* Typatio expressionum (M0b Chunk A) - implementatio infra post
 * aestimatorem; ambulatio et tractatores his utuntur */
interior TypusC89* _expressionem_typare (SilvaSemantica* sem,
    constans SilvaNodus* nodus);
interior vacuum _valorem_typare (SilvaSemantica* sem, SilvaValor v);
interior b32 _est_acies_characterum (TypusC89* t);
interior b32 _pointee_assignabilis (TypusC89* fp, TypusC89* vp,
    b32* abiecti);
interior vacuum _valorem_typare_scalarem (SilvaSemantica* sem,
    SilvaValor v);
interior vacuum _ad_finem_annotare (SilvaSemantica* sem,
    constans SilvaNodus* nodus, TypusC89* valoris, TypusC89* finis);
interior vacuum _sentinellam_examinare (SilvaSemantica* sem,
    constans SilvaNodus* val);
interior TypusC89* _qualibus_exutum (TypusC89* typus);
interior vacuum _congeriem_typare (SilvaSemantica* sem,
    constans SilvaNodus* congeries, TypusC89* scopus_typus);
interior b32 _constans_probare (SilvaSemantica* sem,
    constans SilvaNodus* nodus, s64* valor_out);
interior b32 _fons_alienus (SilvaSemantica* sem,
    constans SilvaNodus* nodus);
interior vacuum _inutiles_examinare (SilvaSemantica* sem);

/* ==================================================
 * Diagnosticum v2 (examen, M4a chunk A)
 * ================================================== */

interior b32 _intra_ambiguum (constans SilvaNodus* nodus);

/* Tabula codicum - FONS UNICUS {causa, severitas}; ordo == ordo
 * ExamenCodex in capite (assertum magnitudinis infra). */
nomen structura {
    constans character* causa;
    s32                 severitas;
} ExamenCodexInformatio;

interior constans ExamenCodexInformatio _codices[] = {
    { "redeclaratio generis diversi eodem scopo", EXAMEN_VIOLATIO },
    { "tag sine titulo sine corpore",             EXAMEN_VIOLATIO },
    { "enumeratio sine titulo sine corpore",      EXAMEN_VIOLATIO },
    { "valor enumeratoris non constans",          EXAMEN_VIOLATIO },
    { "typus nominatus ignotus",                  EXAMEN_INFRA },
    { "nodus erroris in specificatoribus",        EXAMEN_VIOLATIO },
    { "specificatores primitivi et nominati mixti", EXAMEN_VIOLATIO },
    { "multiset primitivorum invalidum",          EXAMEN_VIOLATIO },
    { "mensura aciei non constans positiva",      EXAMEN_VIOLATIO },
    { "nodus erroris in declaratore",             EXAMEN_VIOLATIO },
    { "genus declaratoris inexspectatum",         EXAMEN_INFRA },
    { "species-typi exspectata",                  EXAMEN_VIOLATIO },
    { "declarationes K&R - parca nominata",       EXAMEN_INFRA },
    { "cyclus formae (structura se continens)",   EXAMEN_VIOLATIO },
    { "forma aciei incompletae",                  EXAMEN_VIOLATIO },
    { "forma functionis petita",                  EXAMEN_VIOLATIO },
    { "campi (bitfields) - forma parcata",        EXAMEN_INFRA },
    { "chorda lata (L) - parca nominata",         EXAMEN_INFRA },
    { "fuga invalida in chorda litterali",        EXAMEN_VIOLATIO },
    { "litterale integrum invalidum",             EXAMEN_VIOLATIO },
    { "identificator ignotus in expressione",     EXAMEN_VIOLATIO },
    { "typedef in expressione",                   EXAMEN_VIOLATIO },
    { "typedef vocatum",                          EXAMEN_VIOLATIO },
    { "operandum unarii non arithmeticum",        EXAMEN_VIOLATIO },
    { "operandum ~ non integrale",                EXAMEN_VIOLATIO },
    { "deiectio non monstratoris",                EXAMEN_VIOLATIO },
    { "operandum crementi non scalare",           EXAMEN_VIOLATIO },
    { "operanda multiplicativa non arithmetica",  EXAMEN_VIOLATIO },
    { "operanda non integralia",                  EXAMEN_VIOLATIO },
    { "operanda additiva incompatibilia",         EXAMEN_VIOLATIO },
    { "operanda subtractionis incompatibilia",    EXAMEN_VIOLATIO },
    { "operanda motus non integralia",            EXAMEN_VIOLATIO },
    { "comparatio incompatibilium",               EXAMEN_VIOLATIO },
    { "bracchia ternarii incompatibilia",         EXAMEN_VIOLATIO },
    { "subscriptio non monstratoris",             EXAMEN_VIOLATIO },
    { "vocatus non functio",                      EXAMEN_VIOLATIO },
    { "sagitta non monstratoris",                 EXAMEN_VIOLATIO },
    { "accessus non structurae",                  EXAMEN_VIOLATIO },
    { "accessus structurae incompletae",          EXAMEN_VIOLATIO },
    { "membrum ignotum",                          EXAMEN_VIOLATIO },
    { "elisio uncorum congeriei - parca nominata", EXAMEN_INFRA },
    { "assignatio classium incompatibilium",      EXAMEN_VIOLATIO },
    { "monstratores incompatibiles",              EXAMEN_VIOLATIO },
    { "quales finis abiecti",                     EXAMEN_VIOLATIO },
    { "monstrator et integer mixti",              EXAMEN_VIOLATIO },
    { "assignatio ad non-locum",                  EXAMEN_VIOLATIO },
    { "locus immutabilis",                        EXAMEN_VIOLATIO },
    { "numerus argumentorum discrepat",           EXAMEN_VIOLATIO },
    { "redde cum valore in functione vacua",      EXAMEN_VIOLATIO },
    { "redde sine valore in functione non vacua", EXAMEN_SUSPECTUM },
    { "condicio non scalaris",                    EXAMEN_VIOLATIO },
    { "vocatio implicita (extern int synthetizatum)", EXAMEN_SUSPECTUM },
    { "redeclaratio typi incompatibilis",         EXAMEN_VIOLATIO },
    { "macro domesticum in capite alieno expansum", EXAMEN_SUSPECTUM },
    { "conversio signi implicita",                EXAMEN_DOMESTICUM },
    { "conversio signi implicita (analysi stricta)", EXAMEN_DOMESTICUM },
    { "TOLERA irritum",                           EXAMEN_DOMESTICUM },
    { "comparatio signorum diversorum",           EXAMEN_DOMESTICUM },
    { "comparatio vana (semper eadem)",           EXAMEN_DOMESTICUM },
    { "chorda nuda (sine fine NUL)",              EXAMEN_DOMESTICUM },
    { "signum formati discrepat",                 EXAMEN_DOMESTICUM },
    { "sentinella negativa in functione insignata",
                                                  EXAMEN_DOMESTICUM },
    { "comparatio degradata (insignati <= 0 fit == 0)",
                                                  EXAMEN_DOMESTICUM },
    { "semita sine redditu (finis functionis cadit)",
                                                  EXAMEN_SUSPECTUM },
    { "lapsus inter casus (finis gregis apertus)",
                                                  EXAMEN_DOMESTICUM },
    { "sententia inattingibilis",                 EXAMEN_DOMESTICUM },
    { "frange aut perge extra contextum",         EXAMEN_VIOLATIO },
    { "salta ad titulum ignotum",                 EXAMEN_VIOLATIO },
    { "angustatio implicita (latitudo perditur)", EXAMEN_DOMESTICUM },
    { "variabilis inutilis",                      EXAMEN_DOMESTICUM },
    { "parametrum inutile",                       EXAMEN_DOMESTICUM }
};

/* assertum: tabula == enumeratio (acies negativa si discrepant) */
nomen character _assertum_codicum[
    (magnitudo(_codices) / magnitudo(_codices[0])
        == (memoriae_index)EXAMEN_CODEX_NUMERUS) ? I : -I];

constans character*
silva_c89_codicis_causa (s32 codex)
{
    si (codex < ZEPHYRUM || codex >= (s32)EXAMEN_CODEX_NUMERUS)
    {
        redde "codex ignotus";
    }
    redde _codices[codex].causa;
}

s32
silva_c89_codicis_severitas (s32 codex)
{
    si (codex < ZEPHYRUM || codex >= (s32)EXAMEN_CODEX_NUMERUS)
    {
        redde (s32)EXAMEN_INFRA;
    }
    redde _codices[codex].severitas;
}

/* Lexema primum nodi (repraesentans positionis): loci ordine, TOKEN
 * primum inventum; NODUS et LISTA recursive. NIHIL si nullum. */
interior SilvaToken*
_lexema_primum (constans SilvaNodus* nodus)
{
    i32 i;

    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (v->genus == SILVA_VALOR_TOKEN
            && v->datum.token != NIHIL)
        {
            redde v->datum.token;
        }
        alioquin si (v->genus == SILVA_VALOR_NODUS)
        {
            SilvaToken* t = _lexema_primum(v->datum.nodus);

            si (t != NIHIL)
            {
                redde t;
            }
        }
        alioquin si (v->genus == SILVA_VALOR_LISTA)
        {
            i32 j;
            i32 m = (i32)silva_valor_lista_numerus(*v);

            per (j = ZEPHYRUM; j < m; j++)
            {
                SilvaValor* e = silva_valor_lista_obtinere(*v, (i32)j);

                si (e == NIHIL)
                {
                    perge;
                }
                si (e->genus == SILVA_VALOR_TOKEN
                    && e->datum.token != NIHIL)
                {
                    redde e->datum.token;
                }
                alioquin si (e->genus == SILVA_VALOR_NODUS)
                {
                    SilvaToken* t = _lexema_primum(e->datum.nodus);

                    si (t != NIHIL)
                    {
                        redde t;
                    }
                }
            }
        }
    }
    redde NIHIL;
}

/* Nodus primus listae (positio pro diagnosticis listarum -
 * specificatores mixti/invalidi ipsa lista sunt) */
interior constans SilvaNodus*
_nodus_primus_listae (SilvaValor lista)
{
    i32 i;
    i32 m = silva_valor_lista_numerus(lista);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* e = silva_valor_lista_obtinere(lista, i);

        si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
        {
            redde e->datum.nodus;
        }
    }
    redde NIHIL;
}

/* Declarans typi (positio pro diagnosticis formae - cycli et acies
 * incompletae per tags eunt; functio sine declarante = NIHIL) */
interior constans SilvaNodus*
_declarans_typi (constans TypusC89* typus)
{
    dum (typus != NIHIL)
    {
        commutatio (typus->genus)
        {
            casus TYPUS_C89_STRUCTURA:
            casus TYPUS_C89_UNIO:
            casus TYPUS_C89_ENUMERATUS:
                redde typus->datum.tag.declarans;
            casus TYPUS_C89_QUALIFICATUS:
                typus = typus->datum.qualificatus.internum;
                frange;
            casus TYPUS_C89_ACIES:
                typus = typus->datum.acies.elementum;
                frange;
            ordinarius:
                redde NIHIL;
        }
    }
    redde NIHIL;
}

vacuum
silva_c89_diagnosticum_addere (
    SilvaSemantica*      sem,
    constans SilvaNodus* nodus,
    s32                  codex)
{
    silva_c89_diagnosticum_addere_cum_socio(sem, nodus, codex, NIHIL);
}

/* Corpus commune additionis: causa_structa (NIHIL = causa tabulae)
 * causam per-sedem structam permittit (conversio signi typos ambos
 * nominat) dum positio/provisionale/severitas VIA ORDINARIA
 * materializantur - exemplar MACRO_DOMESTICUM (positio manualis)
 * non iteratur. */
interior vacuum
_diagnosticum_addere_plenum (
    SilvaSemantica*      sem,
    constans SilvaNodus* nodus,
    s32                  codex,
    constans SilvaNodus* socius,
    constans character*  causa_structa)
{
    SemanticaDiagnosticum* d;

    si (sem == NIHIL)
    {
        redde;
    }
    d = (SemanticaDiagnosticum*)xar_addere(sem->diagnostica);
    si (d == NIHIL)
    {
        redde;
    }
    d->nodus = nodus;
    d->socius = socius;
    si (codex >= ZEPHYRUM && codex < (s32)EXAMEN_CODEX_NUMERUS)
    {
        d->codex = codex;
        d->causa = (causa_structa != NIHIL) ? causa_structa
                                            : _codices[codex].causa;
        d->severitas = _codices[codex].severitas;
    }
    alioquin
    {
        d->codex = -I;
        d->causa = "codex ignotus";
        d->severitas = EXAMEN_INFRA;
    }
    d->provisionale = _intra_ambiguum(nodus);
    d->via.mensura = ZEPHYRUM;
    d->via.datum = NIHIL;
    d->linea = ZEPHYRUM;
    d->columna = ZEPHYRUM;
    d->longitudo = ZEPHYRUM;
    si (nodus != NIHIL)
    {
        SilvaToken* lexema = _lexema_primum(nodus);

        si (lexema != NIHIL)
        {
            SilvaToken* radix = silva_token_radix(lexema);

            si (radix != NIHIL)
            {
                d->linea = radix->linea;
                d->columna = radix->columna;
                /* longitudo radicis IN MANU hic - extensio gratis
                 * (LEGATUS chunk 0: computatum-tum-abiectum finitur) */
                d->longitudo = radix->longitudo;
                si (sem->parsura_currens != NIHIL
                    && sem->parsura_currens->expansio != NIHIL)
                {
                    constans chorda* v = silva_fons_via(
                        sem->parsura_currens->expansio,
                        radix->fons_index);

                    si (v != NIHIL)
                    {
                        d->via = *v;
                    }
                }
            }
        }
    }
}

vacuum
silva_c89_diagnosticum_addere_cum_socio (
    SilvaSemantica*      sem,
    constans SilvaNodus* nodus,
    s32                  codex,
    constans SilvaNodus* socius)
{
    _diagnosticum_addere_plenum(sem, nodus, codex, socius, NIHIL);
}

/* ==================================================
 * Macro domesticum in capite alieno (sequela M4a, 2026-07-14)
 *
 * Classis "char C -> char 100" (sqlite3.h:8883; stb ante): caput
 * vendicatum POST latina.h inclusum - macros nostra in textu
 * alieno expanduntur et significationem tacite mutant (subcasus
 * compilans = responsum pravum sine errore, quem solum origo
 * capit). Regula: sedes invocationis (radix catenae originis) in
 * fonte ALIENO et definitio (lexema corporis) in fonte DOMESTICO
 * -> SUSPECTUM, semel per par (macro, fons alienus).
 *
 * Alienitas ex GRAPHO INCLUSIONUM: via SCRIPTA directivae
 * ("../vendor/sqlite3.h") probatur - viae fontium ipsae basename
 * sunt (praebere basename-clavatum; ordines percursus inde
 * pendent, ne mutes). Transitiva: inclusum ab alieno alienum est
 * (transitus unus sufficit - margo parentis ante margines
 * filiorum scribitur, _includendum_processare ante recursionem).
 * Fontes lexici (est_lexicon: ISO; latina in mundis cum_latina =
 * LIMES NOMINATUS, vide worklog) et synthetici numquam domestici:
 * codex vendicatus macros ordinarios (NULL...) iure adhibet.
 * Diagnosticum ad 1:1 plagulae iudicatae (linea directivae in
 * vista non est); causa in piscina structa - omnia nominat.
 * ================================================== */

/* "vendor/" ab initio aut post '/' */
interior b32
_via_aliena (constans chorda* via)
{
    i32 i;

    si (via == NIHIL || via->datum == NIHIL || via->mensura < VII)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i + VII <= via->mensura; i++)
    {
        si (memcmp(via->datum + i, "vendor/", VII) == ZEPHYRUM
            && (i == ZEPHYRUM || via->datum[i - I] == '/'))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* par emissum (dedup: macro semel per fontem alienum) */
nomen structura {
    constans chorda* titulus;
    s32              fons_alienus;
} ExamenParAlienum;

interior vacuum
_alienum_emittere (SilvaSemantica* sem, constans chorda* titulus,
    constans chorda* via_aliena, i32 linea_invocationis,
    constans chorda* via_def, constans chorda* via_princeps)
{
    SemanticaDiagnosticum* d;
    character* nuntius;
    memoriae_index capacitas;

    si (sem == NIHIL || titulus == NIHIL || titulus->datum == NIHIL
        || via_aliena == NIHIL || via_aliena->datum == NIHIL
        || via_def == NIHIL || via_def->datum == NIHIL)
    {
        redde;
    }
    capacitas = (memoriae_index)titulus->mensura
        + (memoriae_index)via_aliena->mensura
        + (memoriae_index)via_def->mensura
        + (memoriae_index)CXXVIII;
    nuntius = (character*)piscina_allocare(sem->piscina, capacitas);
    si (nuntius == NIHIL)
    {
        redde;
    }
    sprintf(nuntius, "macro domesticum '%.*s' in capite alieno"
        " expansum (%.*s:%d) - caput alienum ANTE %.*s includendum",
        (int)titulus->mensura, (constans character*)titulus->datum,
        (int)via_aliena->mensura,
        (constans character*)via_aliena->datum,
        (int)linea_invocationis,
        (int)via_def->mensura, (constans character*)via_def->datum);
    d = (SemanticaDiagnosticum*)xar_addere(sem->diagnostica);
    si (d == NIHIL)
    {
        redde;
    }
    d->nodus = NIHIL;
    d->socius = NIHIL;
    d->codex = (s32)EXAMEN_CODEX_MACRO_DOMESTICUM_IN_ALIENO;
    d->causa = nuntius;
    d->severitas =
        _codices[EXAMEN_CODEX_MACRO_DOMESTICUM_IN_ALIENO].severitas;
    d->provisionale = FALSUM;
    d->via = (via_princeps != NIHIL) ? *via_princeps
                                     : _chorda_vacua();
    d->linea = I;
    d->columna = I;
    d->longitudo = ZEPHYRUM;
}

/* Alienitas per graphum inclusionum (transitus unus - margo
 * parentis ante margines filiorum scribitur): tabulae per fontem
 * in piscinam sem. VERUM = ullus fons alienus (tabulae impletae);
 * FALSUM = TU sine vendore (via celeris, fere omnes). viae_out
 * NIHIL licet (iudicium conversionis vias non postulat). */
interior b32
_alienitates_computare (SilvaSemantica* sem,
    constans SilvaParsura* parsura, b32** alienus_out,
    constans chorda*** viae_out)
{
    SilvaExpansio* exp;
    i32 numerus_fontium;
    b32* alienus;
    constans chorda** via_scripta;
    b32 ullus_alienus;
    i32 i;

    si (sem == NIHIL || parsura == NIHIL
        || parsura->expansio == NIHIL)
    {
        redde FALSUM;
    }
    exp = parsura->expansio;
    numerus_fontium = xar_numerus(exp->fontes);
    si (numerus_fontium == ZEPHYRUM
        || xar_numerus(exp->inclusiones) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    alienus = (b32*)piscina_allocare(sem->piscina,
        (memoriae_index)numerus_fontium * magnitudo(b32));
    via_scripta = (constans chorda**)piscina_allocare(sem->piscina,
        (memoriae_index)numerus_fontium * magnitudo(chorda*));
    si (alienus == NIHIL || via_scripta == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus_fontium; i++)
    {
        alienus[i] = FALSUM;
        via_scripta[i] = NIHIL;
    }
    ullus_alienus = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(exp->inclusiones); i++)
    {
        constans SilvaInclusio* inc = (constans SilvaInclusio*)
            xar_obtinere(exp->inclusiones, i);

        si (inc == NIHIL || inc->fons_ad < ZEPHYRUM
            || inc->fons_ad >= (s32)numerus_fontium
            || alienus[inc->fons_ad])
        {
            perge;
        }
        si ((inc->fons_ex >= ZEPHYRUM
                && inc->fons_ex < (s32)numerus_fontium
                && alienus[inc->fons_ex])
            || _via_aliena(inc->via))
        {
            alienus[inc->fons_ad] = VERUM;
            via_scripta[inc->fons_ad] = inc->via;
            ullus_alienus = VERUM;
        }
    }
    *alienus_out = alienus;
    si (viae_out != NIHIL)
    {
        *viae_out = via_scripta;
    }
    redde ullus_alienus;
}

interior vacuum
_macros_domestica_in_alienis_examinare (SilvaSemantica* sem,
    constans SilvaParsura* parsura)
{
    SilvaExpansio* exp;
    i32 numerus_fontium;
    i32 numerus_lexematum;
    b32* alienus = NIHIL;
    constans chorda** via_scripta = NIHIL;
    Xar* paria;
    constans chorda* via_princeps;
    i32 i;

    si (sem == NIHIL || parsura == NIHIL
        || parsura->expansio == NIHIL
        || parsura->lexemata == NIHIL
        || parsura->fons_princeps < ZEPHYRUM)
    {
        redde;
    }
    si (!_alienitates_computare(sem, parsura, &alienus,
            &via_scripta))
    {
        redde;   /* via celeris: TU sine vendore (fere omnes) */
    }
    exp = parsura->expansio;
    numerus_fontium = xar_numerus(exp->fontes);

    paria = xar_creare(sem->piscina,
        (i32)magnitudo(ExamenParAlienum));
    si (paria == NIHIL)
    {
        redde;
    }
    via_princeps = silva_fons_via(exp, parsura->fons_princeps);
    numerus_lexematum = xar_numerus(parsura->lexemata);
    per (i = ZEPHYRUM; i < numerus_lexematum; i++)
    {
        SilvaToken* tok = *(SilvaToken**)xar_obtinere(
            parsura->lexemata, i);
        SilvaToken* cur;
        constans chorda* titulus = NIHIL;
        SilvaToken* corpus = NIHIL;
        insignatus integer custos = ZEPHYRUM;
        b32 iam_emissum;
        i32 j;

        si (tok == NIHIL
            || tok->origo.genus == SILVA_ORIGO_FONS)
        {
            perge;
        }
        /* ascensus ad radicem: expansio EXTIMA (invocatio in fonte
         * scripto) sola iudicatur - macros intra corpora domestica
         * res definientis sunt, non vendoris */
        cur = tok;
        dum (cur != NIHIL
            && cur->origo.genus != SILVA_ORIGO_FONS
            && custos < LXIV)
        {
            custos++;
            commutatio (cur->origo.genus)
            {
                casus SILVA_ORIGO_EXPANSIO:
                    titulus = cur->origo.datum.expansio.nomen_macro;
                    corpus = cur->origo.datum.expansio.corpus;
                    cur = cur->origo.datum.expansio.invocatio;
                    frange;
                casus SILVA_ORIGO_PASTA:
                    cur = cur->origo.datum.pasta.sinister;
                    frange;
                casus SILVA_ORIGO_CHORDA:
                    cur = cur->origo.datum.stringificatio.primus;
                    frange;
                ordinarius:   /* API: sine sede invocationis */
                    cur = NIHIL;
                    frange;
            }
        }
        si (cur == NIHIL || cur->origo.genus != SILVA_ORIGO_FONS
            || titulus == NIHIL || corpus == NIHIL)
        {
            perge;
        }
        si (cur->fons_index < ZEPHYRUM
            || cur->fons_index >= (s32)numerus_fontium
            || !alienus[cur->fons_index])
        {
            perge;   /* invocatio non in fonte alieno */
        }
        si (corpus->fons_index < ZEPHYRUM
            || corpus->fons_index >= (s32)numerus_fontium
            || alienus[corpus->fons_index])
        {
            perge;   /* definitio ipsa aliena - res vendoris */
        }
        {
            constans SilvaFons* fons_def = (constans SilvaFons*)
                xar_obtinere(exp->fontes, (i32)corpus->fons_index);

            si (fons_def == NIHIL || fons_def->est_lexicon
                || fons_def->est_syntheticus)
            {
                perge;   /* ISO/systema/syntheticum - non nostrum */
            }
        }
        /* dedup per (titulus, fons invocationis) */
        iam_emissum = FALSUM;
        per (j = ZEPHYRUM; j < xar_numerus(paria); j++)
        {
            constans ExamenParAlienum* p =
                (constans ExamenParAlienum*)xar_obtinere(paria, j);

            si (p != NIHIL && p->fons_alienus == cur->fons_index
                && p->titulus != NIHIL
                && p->titulus->mensura == titulus->mensura
                && memcmp(p->titulus->datum, titulus->datum,
                       (memoriae_index)titulus->mensura)
                    == ZEPHYRUM)
            {
                iam_emissum = VERUM;
                frange;
            }
        }
        si (iam_emissum)
        {
            perge;
        }
        {
            ExamenParAlienum* par =
                (ExamenParAlienum*)xar_addere(paria);

            si (par != NIHIL)
            {
                par->titulus = titulus;
                par->fons_alienus = cur->fons_index;
            }
        }
        _alienum_emittere(sem, titulus,
            via_scripta[cur->fons_index] != NIHIL
                ? via_scripta[cur->fons_index]
                : silva_fons_via(exp, cur->fons_index),
            cur->linea,
            silva_fons_via(exp, corpus->fons_index),
            via_princeps);
    }
}

/* ==================================================
 * TOLERA - suppressiones commentariis (gradus severi, 2026-07-17)
 *
 * Regulae SEVERAE (analysis stricta supra paritatem oraculi) in
 * codice ex proposito operante commentario supprimuntur:
 *   x = a - b;  / * TOLERA CONVERSIO_SIGNI_SEVERA: causa * /
 * Linea eadem aut praecedens; CAUSA OBLIGATORIA (sine causa non
 * supprimit). Codices paritatis (54) numquam suppressibiles -
 * TOLERA numquam silet quod clang reiceret. Commentaria ex triviis
 * lexematum (spatia_ante/post - commentaria sunt contentum, pin
 * VISIONIS) pigre colliguntur per parsuram. Post ambulationem
 * TOLERA quod nihil absorbuit aut sine causa aut codicem ignotum
 * nominat -> IRRITUM (lectio eslint: suppressiones putrescunt;
 * quae nihil absorbet signum est).
 * ================================================== */

nomen structura {
    i32 linea;          /* commentarii ipsius */
    s32 fons_index;
    s32 codex;          /* -I = ignotus -> IRRITUM */
    b32 habet_causam;
    b32 usus;
} ExamenTolera;

/* codices suppressibiles (SEVERI soli - paritas numquam) */
nomen structura {
    constans character* titulus;
    s32                 codex;
} ExamenTolerabilis;

interior constans ExamenTolerabilis _tolerabiles[] = {
    { "CONVERSIO_SIGNI_SEVERA",
      (s32)EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA },
    { "COMPARATIO_VANA",
      (s32)EXAMEN_CODEX_COMPARATIO_VANA },
    { "CHORDA_NUDA",
      (s32)EXAMEN_CODEX_CHORDA_NUDA },
    { "SIGNUM_FORMATI",
      (s32)EXAMEN_CODEX_SIGNUM_FORMATI },
    { "SENTINELLA_INSIGNATA",
      (s32)EXAMEN_CODEX_SENTINELLA_INSIGNATA },
    { "COMPARATIO_DEGRADATA",
      (s32)EXAMEN_CODEX_COMPARATIO_DEGRADATA },
    { "CASUS_LAPSUS",
      (s32)EXAMEN_CODEX_CASUS_LAPSUS },
    { "SENTENTIA_INATTINGIBILIS",
      (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS }
};

/* commentarium TOLERA? valor = octeti pleni delimitatoribus
 * inclusis. Forma: TOLERA <TITULUS>[: causa]. VERUM etiam codice
 * ignoto (in tabulam it -> IRRITUM nominat). */
interior b32
_tolera_legere (constans chorda* valor, s32* codex_out,
    b32* habet_causam_out)
{
    i32 i = ZEPHYRUM;
    i32 initium_tituli;
    i32 mensura_tituli;
    memoriae_index t;

    *codex_out = -I;
    *habet_causam_out = FALSUM;
    si (valor == NIHIL || valor->datum == NIHIL
        || valor->mensura < X)
    {
        redde FALSUM;
    }
    i = II;   /* post delimitatorem apertum (slash-stella aut //) */
    dum (i < valor->mensura
        && (valor->datum[i] == ' ' || valor->datum[i] == '\t'))
    {
        i++;
    }
    si (i + VI > valor->mensura
        || memcmp(valor->datum + i, "TOLERA", VI) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    i += VI;
    si (i >= valor->mensura
        || (valor->datum[i] != ' ' && valor->datum[i] != '\t'))
    {
        redde FALSUM;
    }
    dum (i < valor->mensura
        && (valor->datum[i] == ' ' || valor->datum[i] == '\t'))
    {
        i++;
    }
    initium_tituli = i;
    dum (i < valor->mensura
        && ((valor->datum[i] >= 'A' && valor->datum[i] <= 'Z')
            || valor->datum[i] == '_'
            || (valor->datum[i] >= '0' && valor->datum[i] <= '9')))
    {
        i++;
    }
    mensura_tituli = i - initium_tituli;
    si (mensura_tituli == ZEPHYRUM)
    {
        /* "TOLERA" prosa sequente (minusculae) = commentarium de
         * TOLERA, non directivum - lex corporis sui-referentis,
         * ictus QUARTUS (fragor/invalidum/scriptum ante): documenta
         * machinae ipsius "TOLERA irrita..." directiva videbantur */
        redde FALSUM;
    }
    per (t = ZEPHYRUM;
         t < magnitudo(_tolerabiles) / magnitudo(_tolerabiles[0]);
         t++)
    {
        si ((memoriae_index)strlen(_tolerabiles[t].titulus)
                == (memoriae_index)mensura_tituli
            && memcmp(valor->datum + initium_tituli,
                   _tolerabiles[t].titulus,
                   (memoriae_index)mensura_tituli) == ZEPHYRUM)
        {
            *codex_out = _tolerabiles[t].codex;
            frange;
        }
    }
    /* causa: ':' deinde character solidus quivis ante finem */
    dum (i < valor->mensura && valor->datum[i] != ':')
    {
        i++;
    }
    si (i < valor->mensura)
    {
        i++;
        dum (i < valor->mensura)
        {
            si (valor->datum[i] != ' ' && valor->datum[i] != '\t'
                && valor->datum[i] != '*' && valor->datum[i] != '/'
                && valor->datum[i] != '\n')
            {
                *habet_causam_out = VERUM;
                frange;
            }
            i++;
        }
    }
    redde VERUM;
}

/* trivia lexematis unius (ambo latera) -> tabula. Duplicata per
 * (linea, fons, codex) omissa - radix invocationis pluribus
 * lexematibus expansis COMMUNIS est. */
interior vacuum
_toleras_ex_lexemate (SilvaSemantica* sem, SilvaToken* tok)
{
    i32 latus;

    per (latus = ZEPHYRUM; latus < II; latus++)
    {
        Xar* trivia = (latus == ZEPHYRUM) ? tok->spatia_ante
                                          : tok->spatia_post;
        i32 j;

        si (trivia == NIHIL)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(trivia); j++)
        {
            SilvaToken* tr = *(SilvaToken**)xar_obtinere(
                trivia, j);
            s32 codex;
            b32 causam;
            b32 iam = FALSUM;
            i32 k;

            si (tr == NIHIL
                || ((s32)tr->genus
                        != SILVA_LEX_COMMENTUM_CLAUSUM
                    && (s32)tr->genus
                        != SILVA_LEX_COMMENTUM_LINEA))
            {
                perge;
            }
            si (!_tolera_legere(&tr->valor, &codex, &causam))
            {
                perge;
            }
            per (k = ZEPHYRUM; k < xar_numerus(sem->tolerae); k++)
            {
                ExamenTolera* d = (ExamenTolera*)xar_obtinere(
                    sem->tolerae, k);

                si (d->linea == tr->linea
                    && d->fons_index == tr->fons_index
                    && d->codex == codex)
                {
                    iam = VERUM;
                    frange;
                }
            }
            si (!iam)
            {
                ExamenTolera* e = (ExamenTolera*)xar_addere(
                    sem->tolerae);

                si (e == NIHIL)
                {
                    redde;
                }
                e->linea = tr->linea;
                e->fons_index = tr->fons_index;
                e->codex = codex;
                e->habet_causam = causam;
                e->usus = FALSUM;
            }
        }
    }
}

/* collectio pigra per parsuram (exemplar tabulae alienitatis).
 * RADIX QUOQUE examinatur (2026-07-17): commentarium ante
 * sententiam verbo latino ductam (casus/si/redde...) in lexemate
 * INVOCATIONIS equitat, quod expansio e flumine finali substituit -
 * trivia per catenam originis (silva_token_radix) sola attinguntur.
 * Inventum per gradum CASUS_LAPSUS (TOLERA supra casus numquam
 * absorbebatur, geminus C planus absorbebat); afficiebat OMNES
 * gradus TOLERA-biles in plagulis latinis. */
interior vacuum
_toleras_colligere (SilvaSemantica* sem)
{
    constans SilvaParsura* parsura = sem->parsura_currens;
    i32 i;

    si (sem->tolerae_parsura == parsura)
    {
        redde;
    }
    sem->tolerae_parsura = parsura;
    sem->tolerae = NIHIL;
    si (parsura == NIHIL || parsura->lexemata == NIHIL)
    {
        redde;
    }
    sem->tolerae = xar_creare(sem->piscina,
        (i32)magnitudo(ExamenTolera));
    si (sem->tolerae == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(parsura->lexemata); i++)
    {
        SilvaToken* tok = *(SilvaToken**)xar_obtinere(
            parsura->lexemata, i);
        SilvaToken* radix;

        si (tok == NIHIL)
        {
            perge;
        }
        _toleras_ex_lexemate(sem, tok);
        radix = silva_token_radix(tok);
        si (radix != NIHIL && radix != tok)
        {
            _toleras_ex_lexemate(sem, radix);
        }
    }
}

/* suppressio: TOLERA cum causa, codice congruente, in fonte eodem,
 * linea firing aut praecedente. Absorbens usum notat. */
interior b32
_tolera_absorbere (SilvaSemantica* sem, constans SilvaNodus* nodus,
    s32 codex)
{
    SilvaToken* lexema;
    SilvaToken* radix;
    i32 i;

    _toleras_colligere(sem);
    si (sem->tolerae == NIHIL)
    {
        redde FALSUM;
    }
    lexema = _lexema_primum(nodus);
    si (lexema == NIHIL)
    {
        redde FALSUM;
    }
    radix = silva_token_radix(lexema);
    si (radix == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(sem->tolerae); i++)
    {
        ExamenTolera* e = (ExamenTolera*)xar_obtinere(
            sem->tolerae, i);

        si (e != NIHIL && e->codex == codex && e->habet_causam
            && e->fons_index == radix->fons_index
            && (e->linea == radix->linea
                || e->linea + I == radix->linea))
        {
            e->usus = VERUM;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* passus finalis: TOLERA irrita flagrant (positio = commentarium) */
interior vacuum
_toleras_irritas_examinare (SilvaSemantica* sem,
    constans SilvaParsura* parsura)
{
    i32 i;

    si (sem == NIHIL || parsura == NIHIL)
    {
        redde;
    }
    _toleras_colligere(sem);
    si (sem->tolerae == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(sem->tolerae); i++)
    {
        ExamenTolera* e = (ExamenTolera*)xar_obtinere(
            sem->tolerae, i);
        SemanticaDiagnosticum* d;
        constans chorda* v;

        si (e == NIHIL
            || (e->codex >= ZEPHYRUM && e->habet_causam && e->usus))
        {
            perge;
        }
        d = (SemanticaDiagnosticum*)xar_addere(sem->diagnostica);
        si (d == NIHIL)
        {
            redde;
        }
        d->nodus = NIHIL;
        d->socius = NIHIL;
        d->codex = (s32)EXAMEN_CODEX_TOLERA_IRRITUM;
        si (e->codex < ZEPHYRUM)
        {
            d->causa = "TOLERA irritum: codex ignotus"
                " (suppressibiles: CONVERSIO_SIGNI_SEVERA)";
        }
        alioquin si (!e->habet_causam)
        {
            d->causa = "TOLERA irritum: sine causa"
                " (causa obligatoria - cur intentum?)";
        }
        alioquin
        {
            d->causa = "TOLERA irritum: nihil absorbet"
                " (suppressio putrida - codex mutatus?)";
        }
        d->severitas =
            _codices[EXAMEN_CODEX_TOLERA_IRRITUM].severitas;
        d->provisionale = FALSUM;
        d->via = _chorda_vacua();
        si (parsura->expansio != NIHIL)
        {
            v = silva_fons_via(parsura->expansio, e->fons_index);
            si (v != NIHIL)
            {
                d->via = *v;
            }
        }
        d->linea = e->linea;
        d->columna = I;
        d->longitudo = ZEPHYRUM;
    }
}

/* ==================================================
 * Redditor typorum (LEGATUS chunk 0, ex sessione promotus - mores
 * identici cum _typum_scribere sessionis; paritas per suitam
 * officinae probatur)
 * ================================================== */

interior b32
_typum_scribere_intus (constans TypusC89* t, character* b,
    s32* cursor, s32 capacitas)
{
    constans character* nomen_p = NIHIL;

    si (t == NIHIL || *cursor + LXIV >= capacitas)
    {
        redde FALSUM;
    }
    commutatio (t->genus)
    {
        casus TYPUS_C89_PRIMITIVUS:
            commutatio (t->datum.primitivum)
            {
                casus PRIMITIVUM_VACUUM: nomen_p = "vacuum"; frange;
                casus PRIMITIVUM_CHARACTER:
                    nomen_p = "character"; frange;
                casus PRIMITIVUM_CHARACTER_SIGNATUM:
                    nomen_p = "signatus character"; frange;
                casus PRIMITIVUM_CHARACTER_INSIGNATUM:
                    nomen_p = "insignatus character"; frange;
                casus PRIMITIVUM_BREVIS: nomen_p = "brevis"; frange;
                casus PRIMITIVUM_BREVIS_INSIGNATUM:
                    nomen_p = "insignatus brevis"; frange;
                casus PRIMITIVUM_INTEGER:
                    nomen_p = "integer"; frange;
                casus PRIMITIVUM_INTEGER_INSIGNATUM:
                    nomen_p = "insignatus integer"; frange;
                casus PRIMITIVUM_LONGUS: nomen_p = "longus"; frange;
                casus PRIMITIVUM_LONGUS_INSIGNATUM:
                    nomen_p = "insignatus longus"; frange;
                casus PRIMITIVUM_LONGUS_LONGUS:
                    nomen_p = "longus longus"; frange;
                casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
                    nomen_p = "insignatus longus longus"; frange;
                casus PRIMITIVUM_FLUITANS:
                    nomen_p = "fluitans"; frange;
                casus PRIMITIVUM_DUPLEX:
                    nomen_p = "duplex"; frange;
                casus PRIMITIVUM_DUPLEX_LONGUS:
                    nomen_p = "duplex longus"; frange;
                ordinarius: redde FALSUM;
            }
            *cursor += (s32)sprintf(b + *cursor, "%s", nomen_p);
            redde VERUM;
        casus TYPUS_C89_MONSTRATOR:
            si (!_typum_scribere_intus(t->datum.monstrator.internum,
                    b, cursor, capacitas))
            {
                redde FALSUM;
            }
            *cursor += (s32)sprintf(b + *cursor, "*");
            redde VERUM;
        casus TYPUS_C89_ACIES:
            si (!_typum_scribere_intus(t->datum.acies.elementum,
                    b, cursor, capacitas))
            {
                redde FALSUM;
            }
            si (t->datum.acies.numerus < ZEPHYRUM)
            {
                *cursor += (s32)sprintf(b + *cursor, "[]");
            }
            alioquin
            {
                *cursor += (s32)sprintf(b + *cursor, "[%ld]",
                    (longus)t->datum.acies.numerus);
            }
            redde VERUM;
        casus TYPUS_C89_QUALIFICATUS:
            si (t->datum.qualificatus.quales
                & (insignatus integer)QUALIS_CONSTANS)
            {
                *cursor += (s32)sprintf(b + *cursor, "constans ");
            }
            redde _typum_scribere_intus(
                t->datum.qualificatus.internum, b, cursor,
                capacitas);
        casus TYPUS_C89_STRUCTURA:
        casus TYPUS_C89_UNIO:
            si (*cursor + (s32)t->datum.tag.titulus.mensura + XVI
                >= capacitas)
            {
                redde FALSUM;
            }
            nomen_p = t->genus == TYPUS_C89_STRUCTURA ? "structura"
                : "unio";
            si (t->datum.tag.titulus.mensura == ZEPHYRUM)
            {
                /* anonyma quam baptisma non attigit (sine typedef) */
                *cursor += (s32)sprintf(b + *cursor, "%s <anonyma>",
                    nomen_p);
            }
            alioquin
            {
                *cursor += (s32)sprintf(b + *cursor, "%s %.*s",
                    nomen_p,
                    (int)t->datum.tag.titulus.mensura,
                    (constans character*)t->datum.tag.titulus.datum);
            }
            redde VERUM;
        casus TYPUS_C89_ENUMERATUS:
            si (*cursor + (s32)t->datum.enumeratus.titulus.mensura
                + XVI >= capacitas)
            {
                redde FALSUM;
            }
            si (t->datum.enumeratus.titulus.mensura == ZEPHYRUM)
            {
                *cursor += (s32)sprintf(b + *cursor, "enumeratio");
            }
            alioquin
            {
                *cursor += (s32)sprintf(b + *cursor,
                    "enumeratio %.*s",
                    (int)t->datum.enumeratus.titulus.mensura,
                    (constans character*)
                        t->datum.enumeratus.titulus.datum);
            }
            redde VERUM;
        casus TYPUS_C89_FUNCTIO:
            /* signatura: reditus(parametra) - additio post
             * agitationem legati (desideratum #1: "quae parametra"
             * = quaestio hover praecipua agentis). Recursio arma
             * monstratoris ad functiones gratis componit. */
            si (!_typum_scribere_intus(t->datum.functio.reditus, b,
                    cursor, capacitas))
            {
                redde FALSUM;
            }
            *cursor += (s32)sprintf(b + *cursor, "(");
            si (!t->datum.functio.est_prototypata)
            {
                /* K&R: parametra ignota - vacuae parentheses */
            }
            alioquin si (t->datum.functio.numerus_parametrorum
                == ZEPHYRUM)
            {
                *cursor += (s32)sprintf(b + *cursor, "vacuum");
            }
            alioquin
            {
                insignatus integer k;

                per (k = ZEPHYRUM;
                     k < t->datum.functio.numerus_parametrorum;
                     k++)
                {
                    si (k > ZEPHYRUM)
                    {
                        *cursor += (s32)sprintf(b + *cursor, ", ");
                    }
                    si (!_typum_scribere_intus(
                            t->datum.functio.parametra[k], b,
                            cursor, capacitas))
                    {
                        redde FALSUM;
                    }
                }
                si (t->datum.functio.est_variadica)
                {
                    *cursor += (s32)sprintf(b + *cursor, ", ...");
                }
            }
            *cursor += (s32)sprintf(b + *cursor, ")");
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

insignatus integer
silva_c89_typum_scribere (constans TypusC89* t, character* buffer,
    insignatus integer capacitas)
{
    s32 cursor = ZEPHYRUM;

    si (t == NIHIL || buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (!_typum_scribere_intus(t, buffer, &cursor, (s32)capacitas))
    {
        buffer[ZEPHYRUM] = '\0';
        redde ZEPHYRUM;
    }
    redde (insignatus integer)cursor;
}

/* ==================================================
 * Scopi + symbola (Chunk B)
 * ================================================== */

interior SemanticaScopus*
_scopum_creare (Piscina* piscina, SemanticaScopus* pater)
{
    SemanticaScopus* scopus;

    scopus = (SemanticaScopus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SemanticaScopus));
    si (scopus == NIHIL)
    {
        redde NIHIL;
    }
    scopus->pater = pater;
    scopus->ordinaria = tabula_dispersa_creare_chorda(piscina, XVI);
    scopus->tags = tabula_dispersa_creare_chorda(piscina, VIII);
    scopus->profunditas = (pater != NIHIL)
        ? pater->profunditas + I : ZEPHYRUM;
    si (scopus->ordinaria == NIHIL || scopus->tags == NIHIL)
    {
        redde NIHIL;
    }
    redde scopus;
}

interior vacuum
_scopum_aperire (SilvaSemantica* sem)
{
    SemanticaScopus* novus = _scopum_creare(sem->piscina,
        sem->scopus_currens);

    si (novus != NIHIL)
    {
        sem->scopus_currens = novus;
    }
}

interior vacuum
_scopum_claudere (SilvaSemantica* sem)
{
    si (sem->scopus_currens != NIHIL
        && sem->scopus_currens->pater != NIHIL)
    {
        sem->scopus_currens = sem->scopus_currens->pater;
    }
}

SemanticaSymbolum*
silva_c89_symbolum_invenire (SilvaSemantica* sem, chorda titulus)
{
    SemanticaScopus* scopus;

    si (sem == NIHIL)
    {
        redde NIHIL;
    }
    per (scopus = sem->scopus_currens; scopus != NIHIL;
         scopus = scopus->pater)
    {
        vacuum* valor = NIHIL;

        si (tabula_dispersa_invenire(scopus->ordinaria, titulus,
                &valor))
        {
            redde (SemanticaSymbolum*)valor;
        }
    }
    redde NIHIL;
}

/* Registratio in scopum CURRENTEM. Redeclaratio eiusdem scopi:
 * genus idem = clemens (definitiones tentativae C89 legales),
 * genus diversum = diagnosticum (constans enumeri contra
 * variabilem...); superscribitur utroque modo (lint iudicat). */
interior SemanticaSymbolum*
_symbolum_registrare (SilvaSemantica* sem, s32 genus,
    chorda titulus, TypusC89* typus, s64 valor, i32 repositio,
    constans SilvaNodus* declarans, SilvaToken* lexema)
{
    SemanticaSymbolum* symbolum;
    chorda copia;

    si (titulus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    {
        vacuum* prior = NIHIL;

        si (tabula_dispersa_invenire(sem->scopus_currens->ordinaria,
                titulus, &prior))
        {
            SemanticaSymbolum* vetus = (SemanticaSymbolum*)prior;

            si (vetus->genus != genus)
            {
                silva_c89_diagnosticum_addere(sem, declarans,
                    EXAMEN_CODEX_REDECLARATIO_GENERIS);
            }
            alioquin si ((genus == (s32)SYMBOLUM_VARIABILE
                    || genus == (s32)SYMBOLUM_FUNCTIO)
                && !vetus->ex_systemate
                /* limen systematis = limen nexus (inventum corporis
                 * chunk C: tags nominales trans parsuras distincti -
                 * lacuna in dissolutione C2 ad suturam systematis;
                 * compat trans-TU = parca nominata) */
                && typus != NIHIL && vetus->typus != NIHIL
                && !silva_c89_typi_compatibiles(vetus->typus,
                       typus))
            {
                /* chunk C: typo-compatibilitas per relationem;
                 * implicita->realis per regulas mixtas K&R
                 * reconciliatur; socius = declaratio prior */
                silva_c89_diagnosticum_addere_cum_socio(sem,
                    declarans,
                    EXAMEN_CODEX_REDECLARATIO_INCOMPATIBILIS,
                    vetus->declarans);
            }
        }
    }
    symbolum = (SemanticaSymbolum*)piscina_allocare(sem->piscina,
        (memoriae_index)magnitudo(SemanticaSymbolum));
    si (symbolum == NIHIL)
    {
        redde NIHIL;
    }
    copia = chorda_transcribere(titulus, sem->piscina);
    symbolum->genus = genus;
    symbolum->titulus = copia;
    symbolum->typus = typus;
    symbolum->valor = valor;
    symbolum->repositio = repositio;
    symbolum->profunditas = sem->scopus_currens->profunditas;
    symbolum->ex_systemate = sem->in_systemate;
    symbolum->est_implicitum = FALSUM;   /* sedes vocati ponit */
    symbolum->declarans = declarans;
    symbolum->lexema = lexema;
    symbolum->usus = FALSUM;             /* _nexum_ponere ponit */
    (vacuum)tabula_dispersa_inserere(sem->scopus_currens->ordinaria,
        copia, (vacuum*)symbolum);
    {
        SemanticaSymbolum** locus =
            (SemanticaSymbolum**)xar_addere(sem->symbola);

        si (locus != NIHIL)
        {
            *locus = symbolum;
        }
    }
    redde symbolum;
}

/* ==================================================
 * Creatio + primitivi (formae statim positae)
 * ================================================== */

interior TypusC89*
_typum_allocare (SilvaSemantica* sem, s32 genus)
{
    TypusC89* t;

    t = (TypusC89*)piscina_allocare(sem->piscina,
        (memoriae_index)magnitudo(TypusC89));
    si (t == NIHIL)
    {
        redde sem->typus_erroris;
    }
    memset(t, ZEPHYRUM, magnitudo(TypusC89));
    t->genus = genus;
    t->ex_systemate = sem->in_systemate;
    redde t;
}

/* Formae primitivorum - arm64 LP64 Apple (long double = 8!) */
interior vacuum
_formam_primitivi_ponere (TypusC89* t, s32 primitivum)
{
    i32 m = ZEPHYRUM;

    commutatio (primitivum)
    {
        casus PRIMITIVUM_VACUUM:                 m = ZEPHYRUM; frange;
        casus PRIMITIVUM_CHARACTER:
        casus PRIMITIVUM_CHARACTER_SIGNATUM:
        casus PRIMITIVUM_CHARACTER_INSIGNATUM:   m = I;    frange;
        casus PRIMITIVUM_BREVIS:
        casus PRIMITIVUM_BREVIS_INSIGNATUM:      m = II;   frange;
        casus PRIMITIVUM_INTEGER:
        casus PRIMITIVUM_INTEGER_INSIGNATUM:     m = IV;   frange;
        casus PRIMITIVUM_LONGUS:
        casus PRIMITIVUM_LONGUS_INSIGNATUM:
        casus PRIMITIVUM_LONGUS_LONGUS:
        casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM: m = VIII; frange;
        casus PRIMITIVUM_FLUITANS:               m = IV;   frange;
        casus PRIMITIVUM_DUPLEX:
        casus PRIMITIVUM_DUPLEX_LONGUS:          m = VIII; frange;
        ordinarius:                              m = ZEPHYRUM; frange;
    }
    t->forma_computata = VERUM;
    t->magnitudo_octetorum = m;
    t->ordinatio = (m > ZEPHYRUM) ? m : I;
}

SilvaSemantica*
silva_c89_semantica_creare (Piscina* piscina)
{
    SilvaSemantica* sem;
    s32 p;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    sem = (SilvaSemantica*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaSemantica));
    si (sem == NIHIL)
    {
        redde NIHIL;
    }
    memset(sem, ZEPHYRUM, magnitudo(SilvaSemantica));
    sem->piscina = piscina;
    sem->derivati = xar_creare(piscina, (i32)magnitudo(TypusC89*));
    sem->symbola = xar_creare(piscina,
        (i32)magnitudo(SemanticaSymbolum*));
    sem->diagnostica = xar_creare(piscina,
        (i32)magnitudo(SemanticaDiagnosticum));
    /* typationes: clavis = octeti monstratoris (FNV binariae-tutum) */
    sem->typationes = tabula_dispersa_creare_chorda(piscina, CCLVI);
    /* nexus symbolorum (M1a A): clavis eadem */
    sem->nexus = tabula_dispersa_creare_chorda(piscina, CCLVI);
    /* FLUXUS-0: CFG retenti (decisio Q7) */
    sem->fluxus_functionum = xar_creare(piscina,
        (i32)magnitudo(FluxusFunctionis*));
    sem->scopus_summus = _scopum_creare(piscina, NIHIL);
    sem->scopus_currens = sem->scopus_summus;
    si (sem->derivati == NIHIL || sem->symbola == NIHIL
        || sem->diagnostica == NIHIL || sem->typationes == NIHIL
        || sem->nexus == NIHIL || sem->scopus_summus == NIHIL)
    {
        redde NIHIL;
    }

    /* venenum */
    sem->typus_erroris = (TypusC89*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(TypusC89));
    si (sem->typus_erroris == NIHIL)
    {
        redde NIHIL;
    }
    memset(sem->typus_erroris, ZEPHYRUM, magnitudo(TypusC89));
    sem->typus_erroris->genus = TYPUS_C89_ERROR;
    sem->typus_erroris->forma_computata = VERUM;
    sem->typus_erroris->ordinatio = I;

    /* singuli primitivi */
    per (p = ZEPHYRUM; p < (s32)PRIMITIVUM_NUMERUS; p++)
    {
        TypusC89* t = _typum_allocare(sem, TYPUS_C89_PRIMITIVUS);

        t->datum.primitivum = p;
        _formam_primitivi_ponere(t, p);
        sem->primitivi[p] = t;
    }
    redde sem;
}

TypusC89*
silva_c89_typus_primitivum (SilvaSemantica* sem, s32 primitivum)
{
    si (sem == NIHIL || primitivum < ZEPHYRUM
        || primitivum >= (s32)PRIMITIVUM_NUMERUS)
    {
        redde (sem != NIHIL) ? sem->typus_erroris : NIHIL;
    }
    redde sem->primitivi[primitivum];
}

/* ==================================================
 * Internamentum derivatorum (aequalitas structuralis in creatione,
 * identitas monstratoris post - series linearis, complete primum)
 * ================================================== */

interior b32
_derivata_paria (constans TypusC89* a, constans TypusC89* b)
{
    si (a->genus != b->genus)
    {
        redde FALSUM;
    }
    commutatio (a->genus)
    {
        casus TYPUS_C89_MONSTRATOR:
            redde a->datum.monstrator.internum
                == b->datum.monstrator.internum;
        casus TYPUS_C89_ACIES:
            redde a->datum.acies.elementum == b->datum.acies.elementum
                && a->datum.acies.numerus == b->datum.acies.numerus;
        casus TYPUS_C89_QUALIFICATUS:
            redde a->datum.qualificatus.internum
                    == b->datum.qualificatus.internum
                && a->datum.qualificatus.quales
                    == b->datum.qualificatus.quales;
        casus TYPUS_C89_FUNCTIO:
        {
            i32 k;

            si (a->datum.functio.reditus != b->datum.functio.reditus
                || a->datum.functio.numerus_parametrorum
                    != b->datum.functio.numerus_parametrorum
                || a->datum.functio.est_variadica
                    != b->datum.functio.est_variadica
                || a->datum.functio.est_prototypata
                    != b->datum.functio.est_prototypata)
            {
                redde FALSUM;
            }
            per (k = ZEPHYRUM;
                 k < (i32)a->datum.functio.numerus_parametrorum; k++)
            {
                si (a->datum.functio.parametra[k]
                    != b->datum.functio.parametra[k])
                {
                    redde FALSUM;
                }
            }
            redde VERUM;
        }
        ordinarius:
            redde FALSUM;
    }
}

interior TypusC89*
_derivatum_internare (SilvaSemantica* sem, constans TypusC89* exemplar)
{
    i32 i;
    TypusC89* novum;

    per (i = ZEPHYRUM; i < xar_numerus(sem->derivati); i++)
    {
        TypusC89* t = *(TypusC89**)xar_obtinere(sem->derivati, i);

        si (_derivata_paria(t, exemplar))
        {
            redde t;
        }
    }
    novum = (TypusC89*)piscina_allocare(sem->piscina,
        (memoriae_index)magnitudo(TypusC89));
    si (novum == NIHIL)
    {
        redde sem->typus_erroris;
    }
    *novum = *exemplar;
    {
        TypusC89** locus = (TypusC89**)xar_addere(sem->derivati);

        si (locus != NIHIL)
        {
            *locus = novum;
        }
    }
    redde novum;
}

TypusC89*
silva_c89_typus_monstrator (SilvaSemantica* sem, TypusC89* internum)
{
    TypusC89 exemplar;

    si (sem == NIHIL || internum == NIHIL)
    {
        redde (sem != NIHIL) ? sem->typus_erroris : NIHIL;
    }
    memset(&exemplar, ZEPHYRUM, magnitudo(TypusC89));
    exemplar.genus = TYPUS_C89_MONSTRATOR;
    exemplar.datum.monstrator.internum = internum;
    redde _derivatum_internare(sem, &exemplar);
}

TypusC89*
silva_c89_typus_acies (SilvaSemantica* sem, TypusC89* elementum,
    s32 numerus)
{
    TypusC89 exemplar;

    si (sem == NIHIL || elementum == NIHIL)
    {
        redde (sem != NIHIL) ? sem->typus_erroris : NIHIL;
    }
    memset(&exemplar, ZEPHYRUM, magnitudo(TypusC89));
    exemplar.genus = TYPUS_C89_ACIES;
    exemplar.datum.acies.elementum = elementum;
    exemplar.datum.acies.numerus = numerus;
    redde _derivatum_internare(sem, &exemplar);
}

TypusC89*
silva_c89_typus_qualificatus (SilvaSemantica* sem,
    TypusC89* internum, i32 quales)
{
    TypusC89 exemplar;

    si (sem == NIHIL || internum == NIHIL)
    {
        redde (sem != NIHIL) ? sem->typus_erroris : NIHIL;
    }
    si (quales == ZEPHYRUM)
    {
        redde internum;
    }
    /* qualificatus super qualificatum: quales iunguntur */
    si (internum->genus == TYPUS_C89_QUALIFICATUS)
    {
        quales |= internum->datum.qualificatus.quales;
        internum = internum->datum.qualificatus.internum;
    }
    memset(&exemplar, ZEPHYRUM, magnitudo(TypusC89));
    exemplar.genus = TYPUS_C89_QUALIFICATUS;
    exemplar.datum.qualificatus.internum = internum;
    exemplar.datum.qualificatus.quales = quales;
    redde _derivatum_internare(sem, &exemplar);
}

TypusC89*
silva_c89_typus_functio (SilvaSemantica* sem, TypusC89* reditus,
    TypusC89** parametra, i32 numerus, b32 est_variadica,
    b32 est_prototypata)
{
    TypusC89 exemplar;

    si (sem == NIHIL || reditus == NIHIL)
    {
        redde (sem != NIHIL) ? sem->typus_erroris : NIHIL;
    }
    memset(&exemplar, ZEPHYRUM, magnitudo(TypusC89));
    exemplar.genus = TYPUS_C89_FUNCTIO;
    exemplar.datum.functio.reditus = reditus;
    exemplar.datum.functio.parametra = parametra;
    exemplar.datum.functio.numerus_parametrorum = numerus;
    exemplar.datum.functio.est_variadica = est_variadica;
    exemplar.datum.functio.est_prototypata = est_prototypata;
    redde _derivatum_internare(sem, &exemplar);
}

/* ==================================================
 * Registratio plana (Chunk A - scopus B substituet)
 * ================================================== */

/* Baptisma tag anonymi (inventum agitationis legati 2026-07-13:
 * hover in legatus_currere signaturam TOTAM perdebat quia
 * LegatusConfiguratio anonyma erat). Typedef nomen suum
 * structurae/unioni/enumerationi SINE titulo dat - exemplar clang;
 * typedef primum vincit, typi iam nominati intacti. Involucra
 * qualificata perforantur. Tutum: identitas tag NOMINALIS est (per
 * declarantem, non per titulum) et titulus in diagnosticis nusquam
 * apparet - solum redditor et indicium eum legunt. */
interior vacuum
_typedef_baptizare (SilvaSemantica* sem, chorda titulus, TypusC89* t)
{
    dum (t != NIHIL && t->genus == TYPUS_C89_QUALIFICATUS)
    {
        t = t->datum.qualificatus.internum;
    }
    si (t == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde;
    }
    si ((t->genus == TYPUS_C89_STRUCTURA
            || t->genus == TYPUS_C89_UNIO)
        && t->datum.tag.titulus.mensura == ZEPHYRUM)
    {
        t->datum.tag.titulus = chorda_transcribere(titulus,
            sem->piscina);
    }
    alioquin si (t->genus == TYPUS_C89_ENUMERATUS
        && t->datum.enumeratus.titulus.mensura == ZEPHYRUM)
    {
        t->datum.enumeratus.titulus = chorda_transcribere(titulus,
            sem->piscina);
    }
}

b32
silva_c89_typedef_registrare (SilvaSemantica* sem, chorda titulus,
    TypusC89* typus)
{
    si (sem == NIHIL || typus == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    _typedef_baptizare(sem, titulus, typus);
    redde _symbolum_registrare(sem, SYMBOLUM_TYPEDEF, titulus,
        typus, ZEPHYRUM, ZEPHYRUM, NIHIL, NIHIL) != NIHIL;
}

TypusC89*
silva_c89_typedef_invenire (SilvaSemantica* sem, chorda titulus)
{
    SemanticaSymbolum* symbolum = silva_c89_symbolum_invenire(sem,
        titulus);

    si (symbolum == NIHIL || symbolum->genus != SYMBOLUM_TYPEDEF)
    {
        redde NIHIL;
    }
    redde symbolum->typus;
}

/* ==================================================
 * Multiset primitivorum -> canonicus
 * ================================================== */

nomen structura {
    i32 vacui;
    i32 characteres;
    i32 breves;
    i32 integri;
    i32 longi;
    i32 fluitantes;
    i32 duplices;
    i32 signati;
    i32 insignati;
    i32 summa;
} NumeriPrimitivorum;

interior vacuum
_verba_numerare (NumeriPrimitivorum* n, SilvaValor verba)
{
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(verba);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(verba, i);

        si (v == NIHIL || v->genus != SILVA_VALOR_TOKEN)
        {
            perge;
        }
        commutatio ((s32)v->datum.token->genus)
        {
            casus SILVA_LEX_VOID:     n->vacui++;       frange;
            casus SILVA_LEX_CHAR:     n->characteres++; frange;
            casus SILVA_LEX_SHORT:    n->breves++;      frange;
            casus SILVA_LEX_INT:      n->integri++;     frange;
            casus SILVA_LEX_LONG:     n->longi++;       frange;
            casus SILVA_LEX_FLOAT_KW: n->fluitantes++;  frange;
            casus SILVA_LEX_DOUBLE:   n->duplices++;    frange;
            casus SILVA_LEX_SIGNED:   n->signati++;     frange;
            casus SILVA_LEX_UNSIGNED: n->insignati++;   frange;
            ordinarius: frange;
        }
        n->summa++;
    }
}

/* -1 = multiset invalidum */
interior s32
_primitivum_canonicum (constans NumeriPrimitivorum* n)
{
    si (n->signati > ZEPHYRUM && n->insignati > ZEPHYRUM)
    {
        redde -I;
    }
    si (n->vacui > ZEPHYRUM)
    {
        redde (n->summa == n->vacui && n->vacui == I)
            ? (s32)PRIMITIVUM_VACUUM : -I;
    }
    si (n->fluitantes > ZEPHYRUM)
    {
        redde (n->summa == I) ? (s32)PRIMITIVUM_FLUITANS : -I;
    }
    si (n->duplices > ZEPHYRUM)
    {
        si (n->duplices != I || n->characteres > ZEPHYRUM
            || n->breves > ZEPHYRUM || n->integri > ZEPHYRUM
            || n->signati > ZEPHYRUM || n->insignati > ZEPHYRUM)
        {
            redde -I;
        }
        si (n->longi == I)
        {
            redde (s32)PRIMITIVUM_DUPLEX_LONGUS;
        }
        redde (n->longi == ZEPHYRUM) ? (s32)PRIMITIVUM_DUPLEX : -I;
    }
    si (n->characteres > ZEPHYRUM)
    {
        si (n->characteres != I || n->breves > ZEPHYRUM
            || n->integri > ZEPHYRUM || n->longi > ZEPHYRUM)
        {
            redde -I;
        }
        si (n->signati == I)
        {
            redde (s32)PRIMITIVUM_CHARACTER_SIGNATUM;
        }
        si (n->insignati == I)
        {
            redde (s32)PRIMITIVUM_CHARACTER_INSIGNATUM;
        }
        redde (s32)PRIMITIVUM_CHARACTER;
    }
    si (n->breves > ZEPHYRUM)
    {
        si (n->breves != I || n->longi > ZEPHYRUM || n->integri > I)
        {
            redde -I;
        }
        redde (n->insignati == I)
            ? (s32)PRIMITIVUM_BREVIS_INSIGNATUM
            : (s32)PRIMITIVUM_BREVIS;
    }
    si (n->longi == II)
    {
        si (n->integri > I)
        {
            redde -I;
        }
        redde (n->insignati == I)
            ? (s32)PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM
            : (s32)PRIMITIVUM_LONGUS_LONGUS;
    }
    si (n->longi == I)
    {
        si (n->integri > I)
        {
            redde -I;
        }
        redde (n->insignati == I)
            ? (s32)PRIMITIVUM_LONGUS_INSIGNATUM
            : (s32)PRIMITIVUM_LONGUS;
    }
    si (n->longi > II || n->integri > I)
    {
        redde -I;
    }
    redde (n->insignati == I)
        ? (s32)PRIMITIVUM_INTEGER_INSIGNATUM
        : (s32)PRIMITIVUM_INTEGER;
}

/* ==================================================
 * Tags (structura/unio) + enumerationes
 * ================================================== */

interior TypusC89* _typus_ex_membris (SilvaSemantica* sem,
    TypusC89* typus, SilvaValor membra);

interior TypusC89*
_tag_invenire (SilvaSemantica* sem, chorda titulus)
{
    SemanticaScopus* scopus;

    per (scopus = sem->scopus_currens; scopus != NIHIL;
         scopus = scopus->pater)
    {
        vacuum* valor = NIHIL;

        si (tabula_dispersa_invenire(scopus->tags, titulus, &valor))
        {
            redde (TypusC89*)valor;
        }
    }
    redde NIHIL;
}

interior vacuum
_tag_registrare (SilvaSemantica* sem, chorda titulus, TypusC89* typus)
{
    chorda copia = chorda_transcribere(titulus, sem->piscina);

    (vacuum)tabula_dispersa_inserere(sem->scopus_currens->tags,
        copia, (vacuum*)typus);
}

interior TypusC89*
_tag_tractare (SilvaSemantica* sem, constans SilvaNodus* nodus,
    b32 est_unio)
{
    SilvaValor titulus_v;
    SilvaValor aperta_v;
    SilvaValor membra_v;
    b32 nominata;
    b32 definitio;
    TypusC89* typus = NIHIL;
    s32 genus_typi = est_unio ? TYPUS_C89_UNIO : TYPUS_C89_STRUCTURA;

    si (est_unio)
    {
        titulus_v = silva_c89_unio_tok_titulus(nodus);
        aperta_v = silva_c89_unio_tok_aperta(nodus);
        membra_v = silva_c89_unio_membra(nodus);
    }
    alioquin
    {
        titulus_v = silva_c89_structura_tok_titulus(nodus);
        aperta_v = silva_c89_structura_tok_aperta(nodus);
        membra_v = silva_c89_structura_membra(nodus);
    }
    /* discriminator = praesentia tok_aperta, NON numerus membrorum
     * (struct S {} definitio vacua est) */
    nominata = (titulus_v.genus == SILVA_VALOR_TOKEN);
    definitio = (aperta_v.genus == SILVA_VALOR_TOKEN);

    si (!definitio)
    {
        /* referentia: inventa aut incompleta creata */
        si (!nominata)
        {
            silva_c89_diagnosticum_addere(sem, nodus,
                EXAMEN_CODEX_TAG_SINE_CORPORE);
            redde sem->typus_erroris;
        }
        typus = _tag_invenire(sem, titulus_v.datum.token->valor);
        si (typus != NIHIL)
        {
            redde typus;
        }
        typus = _typum_allocare(sem, genus_typi);
        typus->datum.tag.titulus = chorda_transcribere(
            titulus_v.datum.token->valor, sem->piscina);
        typus->datum.tag.declarans = nodus;
        _tag_registrare(sem, titulus_v.datum.token->valor, typus);
        redde typus;
    }

    /* definitio: incompleta eiusdem tituli completur in situ;
     * completa eiusdem tituli = typus NOVUS re-registratus (limitatio
     * tabulae planae - scopus B corriget; vide worklog) */
    si (nominata)
    {
        typus = _tag_invenire(sem, titulus_v.datum.token->valor);
        si (typus != NIHIL
            && (typus->genus != genus_typi
                || typus->datum.tag.completa))
        {
            typus = NIHIL;
        }
    }
    si (typus == NIHIL)
    {
        typus = _typum_allocare(sem, genus_typi);
        typus->datum.tag.titulus = nominata
            ? chorda_transcribere(titulus_v.datum.token->valor,
                  sem->piscina)
            : _chorda_vacua();
        typus->datum.tag.declarans = nodus;
        si (nominata)
        {
            _tag_registrare(sem, titulus_v.datum.token->valor, typus);
        }
    }
    /* registratum ANTE membra - structura sui-referens */
    redde _typus_ex_membris(sem, typus, membra_v);
}

interior TypusC89* _typus_ex_specificatoribus_interior (
    SilvaSemantica* sem, SilvaValor specificatores,
    b32* est_typedef_out, i32* repositio_out);

interior TypusC89*
_typus_ex_membris (SilvaSemantica* sem, TypusC89* typus,
    SilvaValor membra)
{
    Xar* colligenda = xar_creare(sem->piscina,
        (i32)magnitudo(TypusC89Membrum));
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(membra);

    si (colligenda == NIHIL)
    {
        redde sem->typus_erroris;
    }
    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(membra, i);
        constans SilvaNodus* membrum;
        TypusC89* basis;
        SilvaValor declaratores;
        i32 k;
        i32 dn;

        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        membrum = _canonicum(elem->datum.nodus);
        si (membrum->genus != (s32)SILVA_C89_GENUS_MEMBRUM)
        {
            perge;
        }
        basis = _typus_ex_specificatoribus_interior(sem,
            silva_c89_membrum_specificatores(membrum), NIHIL, NIHIL);
        declaratores = silva_c89_membrum_declaratores(membrum);
        dn = (i32)silva_valor_lista_numerus(declaratores);
        per (k = ZEPHYRUM; k < dn; k++)
        {
            SilvaValor* dv = silva_valor_lista_obtinere(
                declaratores, k);
            constans SilvaNodus* d;
            constans SilvaNodus* declarator;
            b32 est_campus = FALSUM;
            SilvaToken* tok = NIHIL;
            TypusC89Membrum* novum;
            TypusC89* t;

            si (dv == NIHIL || dv->genus != SILVA_VALOR_NODUS)
            {
                perge;   /* separator */
            }
            d = _canonicum(dv->datum.nodus);
            declarator = d;
            si (d->genus == (s32)SILVA_C89_GENUS_CAMPUS)
            {
                SilvaValor interior_d = silva_c89_campus_declarator(d);

                est_campus = VERUM;
                typus->datum.tag.habet_campos = VERUM;
                declarator = (interior_d.genus == SILVA_VALOR_NODUS)
                    ? interior_d.datum.nodus : NIHIL;
                /* latitudo = expressio constans - typatur (M0b) */
                _valorem_typare(sem, silva_c89_campus_latitudo(d));
            }
            t = silva_c89_typus_ex_declaratore(sem, basis,
                declarator, &tok);
            novum = (TypusC89Membrum*)xar_addere(colligenda);
            si (novum != NIHIL)
            {
                novum->titulus = (tok != NIHIL)
                    ? chorda_transcribere(tok->valor, sem->piscina)
                    : _chorda_vacua();
                novum->typus = t;
                novum->offset = ZEPHYRUM;
                novum->est_campus = est_campus;
            }
        }
    }

    /* copia in seriem contiguam */
    {
        i32 n = xar_numerus(colligenda);
        TypusC89Membrum* series = NIHIL;
        i32 k;

        si (n > ZEPHYRUM)
        {
            series = (TypusC89Membrum*)piscina_allocare(sem->piscina,
                (memoriae_index)n
                    * (memoriae_index)magnitudo(TypusC89Membrum));
            si (series == NIHIL)
            {
                redde sem->typus_erroris;
            }
            per (k = ZEPHYRUM; k < (i32)n; k++)
            {
                series[k] = *(TypusC89Membrum*)xar_obtinere(
                    colligenda, k);
            }
        }
        typus->datum.tag.membra = series;
        typus->datum.tag.numerus_membrorum = (i32)n;
    }
    typus->datum.tag.completa = VERUM;
    redde typus;
}

interior TypusC89*
_enumerationem_tractare (SilvaSemantica* sem,
    constans SilvaNodus* nodus)
{
    SilvaValor titulus_v = silva_c89_enumeratio_tok_titulus(nodus);
    SilvaValor aperta_v = silva_c89_enumeratio_tok_aperta(nodus);
    b32 nominata = (titulus_v.genus == SILVA_VALOR_TOKEN);
    b32 definitio = (aperta_v.genus == SILVA_VALOR_TOKEN);
    TypusC89* typus = NIHIL;

    si (!definitio)
    {
        si (!nominata)
        {
            silva_c89_diagnosticum_addere(sem, nodus,
                EXAMEN_CODEX_ENUMERATIO_SINE_CORPORE);
            redde sem->typus_erroris;
        }
        typus = _tag_invenire(sem, titulus_v.datum.token->valor);
        si (typus != NIHIL)
        {
            redde typus;
        }
    }
    si (typus == NIHIL)
    {
        typus = _typum_allocare(sem, TYPUS_C89_ENUMERATUS);
        typus->datum.enumeratus.titulus = nominata
            ? chorda_transcribere(titulus_v.datum.token->valor,
                  sem->piscina)
            : _chorda_vacua();
        typus->datum.enumeratus.declarans = nodus;
        /* enum = int (C89) */
        typus->forma_computata = VERUM;
        typus->magnitudo_octetorum = IV;
        typus->ordinatio = IV;
        si (nominata)
        {
            _tag_registrare(sem, titulus_v.datum.token->valor, typus);
        }
    }
    si (definitio)
    {
        SilvaValor enumeratores =
            silva_c89_enumeratio_enumeratores(nodus);
        i32 i;
        i32 m = (i32)silva_valor_lista_numerus(enumeratores);
        s64 numerator = ZEPHYRUM;

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(
                enumeratores, i);
            constans SilvaNodus* e;
            SilvaValor valor_v;
            SilvaValor tok_v;

            si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            e = _canonicum(elem->datum.nodus);
            si (e->genus != (s32)SILVA_C89_GENUS_ENUMERATOR)
            {
                perge;
            }
            valor_v = silva_c89_enumerator_valor(e);
            si (valor_v.genus == SILVA_VALOR_NODUS)
            {
                s64 aestimatum = ZEPHYRUM;

                si (silva_c89_constans_aestimare(sem,
                        valor_v.datum.nodus, &aestimatum))
                {
                    numerator = aestimatum;
                }
                alioquin
                {
                    silva_c89_diagnosticum_addere(sem, e,
                        EXAMEN_CODEX_ENUMERATOR_NON_CONSTANS);
                }
                _valorem_typare(sem, valor_v);   /* expressio quoque */
            }
            tok_v = silva_c89_enumerator_tok_titulus(e);
            si (tok_v.genus == SILVA_VALOR_TOKEN)
            {
                /* constans enumeri in spatium ORDINARIUM (C89) */
                (vacuum)_symbolum_registrare(sem, SYMBOLUM_CONSTANS,
                    tok_v.datum.token->valor, typus, numerator,
                    ZEPHYRUM, e, tok_v.datum.token);
            }
            numerator = numerator + I;
        }
        typus->datum.enumeratus.completa = VERUM;
    }
    redde typus;
}

/* ==================================================
 * Specificatores -> basis
 * ================================================== */

interior i32
_quales_ex_lista (SilvaValor qualificatores)
{
    i32 quales = ZEPHYRUM;
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(qualificatores);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(qualificatores, i);

        si (v == NIHIL || v->genus != SILVA_VALOR_TOKEN)
        {
            perge;
        }
        si (v->datum.token->genus == SILVA_LEX_CONST)
        {
            quales |= QUALIS_CONSTANS;
        }
        alioquin si (v->datum.token->genus == SILVA_LEX_VOLATILE)
        {
            quales |= QUALIS_VOLATILIS;
        }
    }
    redde quales;
}

interior TypusC89*
_typus_ex_specificatoribus_interior (SilvaSemantica* sem,
    SilvaValor specificatores, b32* est_typedef_out,
    i32* repositio_out)
{
    i32 repositio = ZEPHYRUM;
    NumeriPrimitivorum numeri;
    TypusC89* nominatus = NIHIL;
    b32 habet_primitiva = FALSUM;
    b32 habet_nominatum = FALSUM;
    b32 est_typedef = FALSUM;
    i32 quales = ZEPHYRUM;
    TypusC89* basis;
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(specificatores);

    memset(&numeri, ZEPHYRUM, magnitudo(numeri));

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(specificatores, i);
        constans SilvaNodus* n;

        si (v == NIHIL)
        {
            perge;
        }
        si (v->genus == SILVA_VALOR_TOKEN)
        {
            commutatio ((s32)v->datum.token->genus)
            {
                casus SILVA_LEX_CONST:
                    quales |= QUALIS_CONSTANS;
                    frange;
                casus SILVA_LEX_VOLATILE:
                    quales |= QUALIS_VOLATILIS;
                    frange;
                casus SILVA_LEX_TYPEDEF:
                    est_typedef = VERUM;
                    frange;
                casus SILVA_LEX_STATIC:
                    repositio |= REPOSITIO_STATICA;
                    frange;
                casus SILVA_LEX_EXTERN:
                    repositio |= REPOSITIO_EXTERNA;
                    frange;
                casus SILVA_LEX_AUTO:
                    repositio |= REPOSITIO_AUTOMATA;
                    frange;
                casus SILVA_LEX_REGISTER:
                    repositio |= REPOSITIO_REGISTRI;
                    frange;
                ordinarius:
                    frange;
            }
            perge;
        }
        si (v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        n = _canonicum(v->datum.nodus);
        commutatio (n->genus)
        {
            casus (s32)SILVA_C89_GENUS_TYPUS_PRIMITIVUS:
                habet_primitiva = VERUM;
                _verba_numerare(&numeri,
                    silva_c89_typus_primitivus_tok_verba(n));
                frange;
            casus (s32)SILVA_C89_GENUS_TYPUS_NOMINATUS:
            {
                SilvaValor tok_v =
                    silva_c89_typus_nominatus_tok_titulus(n);

                habet_nominatum = VERUM;
                si (tok_v.genus == SILVA_VALOR_TOKEN)
                {
                    nominatus = silva_c89_typedef_invenire(sem,
                        tok_v.datum.token->valor);
                    si (nominatus == NIHIL)
                    {
                        silva_c89_diagnosticum_addere(sem, n,
                            EXAMEN_CODEX_TYPUS_NOMINATUS_IGNOTUS);
                    }
                }
                frange;
            }
            casus (s32)SILVA_C89_GENUS_STRUCTURA:
                habet_nominatum = VERUM;
                nominatus = _tag_tractare(sem, n, FALSUM);
                frange;
            casus (s32)SILVA_C89_GENUS_UNIO:
                habet_nominatum = VERUM;
                nominatus = _tag_tractare(sem, n, VERUM);
                frange;
            casus (s32)SILVA_C89_GENUS_ENUMERATIO:
                habet_nominatum = VERUM;
                nominatus = _enumerationem_tractare(sem, n);
                frange;
            casus (s32)SILVA_C89_GENUS_ERROR:
                silva_c89_diagnosticum_addere(sem, n,
                    EXAMEN_CODEX_ERROR_IN_SPECIFICATORIBUS);
                frange;
            ordinarius:
                frange;
        }
    }

    si (est_typedef_out != NIHIL)
    {
        *est_typedef_out = est_typedef;
    }
    si (repositio_out != NIHIL)
    {
        *repositio_out = repositio;
    }

    si (habet_primitiva && habet_nominatum)
    {
        silva_c89_diagnosticum_addere(sem,
            _nodus_primus_listae(specificatores),
            EXAMEN_CODEX_SPECIFICATORES_MIXTI);
        redde sem->typus_erroris;
    }
    si (habet_nominatum)
    {
        basis = (nominatus != NIHIL) ? nominatus : sem->typus_erroris;
    }
    alioquin si (habet_primitiva)
    {
        s32 p = _primitivum_canonicum(&numeri);

        si (p < ZEPHYRUM)
        {
            silva_c89_diagnosticum_addere(sem,
                _nodus_primus_listae(specificatores),
                EXAMEN_CODEX_MULTISET_PRIMITIVORUM);
            redde sem->typus_erroris;
        }
        basis = sem->primitivi[p];
    }
    alioquin
    {
        /* int implicitum - C89 legale (extern x;) */
        basis = sem->primitivi[PRIMITIVUM_INTEGER];
    }

    si (quales != ZEPHYRUM && basis->genus != TYPUS_C89_ERROR)
    {
        basis = silva_c89_typus_qualificatus(sem, basis, quales);
    }
    redde basis;
}

TypusC89*
silva_c89_typus_ex_specificatoribus (SilvaSemantica* sem,
    SilvaValor specificatores, b32* est_typedef_out)
{
    si (sem == NIHIL)
    {
        redde NIHIL;
    }
    redde _typus_ex_specificatoribus_interior(sem, specificatores,
        est_typedef_out, NIHIL);
}

/* ==================================================
 * Declarator -> typus (regula C intus-foras)
 * ================================================== */

interior TypusC89**
_parametra_tractare (SilvaSemantica* sem, SilvaValor parametra,
    i32* numerus_out, b32* variadica_out, b32* prototypata_out)
{
    Xar* colligenda = xar_creare(sem->piscina,
        (i32)magnitudo(TypusC89*));
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(parametra);
    b32 variadica = FALSUM;
    i32 n;
    TypusC89** series = NIHIL;

    per (i = ZEPHYRUM; i < m && colligenda != NIHIL; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(parametra, i);
        constans SilvaNodus* p;
        TypusC89* basis;
        TypusC89* t;
        SilvaValor decl_v;

        si (v == NIHIL)
        {
            perge;
        }
        si (v->genus == SILVA_VALOR_TOKEN)
        {
            si (v->datum.token->genus == SILVA_LEX_ELLIPSIS)
            {
                variadica = VERUM;
            }
            perge;   /* virgulae */
        }
        si (v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        p = _canonicum(v->datum.nodus);
        si (p->genus != (s32)SILVA_C89_GENUS_PARAMETRUM)
        {
            perge;
        }
        basis = _typus_ex_specificatoribus_interior(sem,
            silva_c89_parametrum_specificatores(p), NIHIL, NIHIL);
        decl_v = silva_c89_parametrum_declarator(p);
        t = silva_c89_typus_ex_declaratore(sem, basis,
            (decl_v.genus == SILVA_VALOR_NODUS)
                ? decl_v.datum.nodus : NIHIL,
            NIHIL);
        /* accommodationes parametrorum (C89 6.7.1):
         * acies -> monstrator elementi; functio -> monstrator */
        si (t->genus == TYPUS_C89_ACIES)
        {
            t = silva_c89_typus_monstrator(sem,
                t->datum.acies.elementum);
        }
        alioquin si (t->genus == TYPUS_C89_FUNCTIO)
        {
            t = silva_c89_typus_monstrator(sem, t);
        }
        {
            TypusC89** locus = (TypusC89**)xar_addere(colligenda);

            si (locus != NIHIL)
            {
                *locus = t;
            }
        }
    }

    n = (colligenda != NIHIL) ? (i32)xar_numerus(colligenda)
                              : ZEPHYRUM;

    /* (void) unicum = prototypata sine parametris */
    si (n == I && !variadica)
    {
        TypusC89* solus = *(TypusC89**)xar_obtinere(colligenda,
            ZEPHYRUM);

        si (solus == sem->primitivi[PRIMITIVUM_VACUUM])
        {
            *numerus_out = ZEPHYRUM;
            *variadica_out = FALSUM;
            *prototypata_out = VERUM;
            redde NIHIL;
        }
    }
    si (n == ZEPHYRUM)
    {
        /* () = non prototypata (K&R stilo) */
        *numerus_out = ZEPHYRUM;
        *variadica_out = variadica;
        *prototypata_out = FALSUM;
        redde NIHIL;
    }
    series = (TypusC89**)piscina_allocare(sem->piscina,
        (memoriae_index)n * (memoriae_index)magnitudo(TypusC89*));
    si (series != NIHIL)
    {
        i32 k;

        per (k = ZEPHYRUM; k < n; k++)
        {
            series[k] = *(TypusC89**)xar_obtinere(colligenda, k);
        }
    }
    *numerus_out = n;
    *variadica_out = variadica;
    *prototypata_out = VERUM;
    redde series;
}

TypusC89*
silva_c89_typus_ex_declaratore (SilvaSemantica* sem,
    TypusC89* basis, constans SilvaNodus* declarator,
    SilvaToken** titulus_out)
{
    si (sem == NIHIL || basis == NIHIL)
    {
        redde (sem != NIHIL) ? sem->typus_erroris : NIHIL;
    }
    si (titulus_out != NIHIL)
    {
        *titulus_out = NIHIL;
    }
    dum (declarator != NIHIL)
    {
        declarator = _canonicum(declarator);
        commutatio (declarator->genus)
        {
            casus (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS:
            {
                SilvaValor v = silva_c89_declarator_initiatus_declarator(
                    declarator);

                declarator = (v.genus == SILVA_VALOR_NODUS)
                    ? v.datum.nodus : NIHIL;
                frange;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS:
            {
                SilvaValor tok_v = silva_c89_declarator_titulus_tok_titulus(
                    declarator);

                si (titulus_out != NIHIL
                    && tok_v.genus == SILVA_VALOR_TOKEN)
                {
                    *titulus_out = tok_v.datum.token;
                }
                redde basis;
            }
            casus (s32)SILVA_C89_GENUS_PARENTHESIS:
            {
                SilvaValor v = silva_c89_parenthesis_internum(
                    declarator);

                declarator = (v.genus == SILVA_VALOR_NODUS)
                    ? v.datum.nodus : NIHIL;
                frange;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR:
            {
                i32 quales = _quales_ex_lista(
                    silva_c89_declarator_monstrator_qualificatores(
                        declarator));
                SilvaValor v = silva_c89_declarator_monstrator_internum(
                    declarator);

                basis = silva_c89_typus_monstrator(sem, basis);
                si (quales != ZEPHYRUM)
                {
                    basis = silva_c89_typus_qualificatus(sem, basis,
                        quales);
                }
                declarator = (v.genus == SILVA_VALOR_NODUS)
                    ? v.datum.nodus : NIHIL;
                frange;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS:
            {
                i32 quales = _quales_ex_lista(
                    silva_c89_declarator_abstractus_qualificatores(
                        declarator));
                SilvaValor v = silva_c89_declarator_abstractus_internum(
                    declarator);

                basis = silva_c89_typus_monstrator(sem, basis);
                si (quales != ZEPHYRUM)
                {
                    basis = silva_c89_typus_qualificatus(sem, basis,
                        quales);
                }
                declarator = (v.genus == SILVA_VALOR_NODUS)
                    ? v.datum.nodus : NIHIL;
                frange;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI:
            {
                SilvaValor mensura_v = silva_c89_declarator_aciei_mensura(
                    declarator);
                SilvaValor v = silva_c89_declarator_aciei_internum(
                    declarator);
                s32 numerus = -I;

                si (mensura_v.genus == SILVA_VALOR_NODUS)
                {
                    s64 aestimatum = ZEPHYRUM;

                    si (silva_c89_constans_aestimare(sem,
                            mensura_v.datum.nodus, &aestimatum)
                        && aestimatum > ZEPHYRUM)
                    {
                        numerus = (s32)aestimatum;
                    }
                    alioquin
                    {
                        silva_c89_diagnosticum_addere(sem, declarator,
                            EXAMEN_CODEX_MENSURA_ACIEI);
                    }
                    _valorem_typare(sem, mensura_v);   /* M0b */
                }
                basis = silva_c89_typus_acies(sem, basis, numerus);
                declarator = (v.genus == SILVA_VALOR_NODUS)
                    ? v.datum.nodus : NIHIL;
                frange;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS:
            {
                i32 numerus = ZEPHYRUM;
                b32 variadica = FALSUM;
                b32 prototypata = FALSUM;
                TypusC89** parametra = _parametra_tractare(sem,
                    silva_c89_declarator_functionis_parametra(
                        declarator),
                    &numerus, &variadica, &prototypata);
                SilvaValor v = silva_c89_declarator_functionis_internum(
                    declarator);

                basis = silva_c89_typus_functio(sem, basis, parametra,
                    numerus, variadica, prototypata);
                declarator = (v.genus == SILVA_VALOR_NODUS)
                    ? v.datum.nodus : NIHIL;
                frange;
            }
            casus (s32)SILVA_C89_GENUS_ERROR:
                silva_c89_diagnosticum_addere(sem, declarator,
                    EXAMEN_CODEX_ERROR_IN_DECLARATORE);
                redde sem->typus_erroris;
            ordinarius:
                silva_c89_diagnosticum_addere(sem, declarator,
                    EXAMEN_CODEX_GENUS_DECLARATORIS);
                redde sem->typus_erroris;
        }
    }
    redde basis;   /* abstractus: catena in NIHIL desiit */
}

TypusC89*
silva_c89_typus_ex_specie (SilvaSemantica* sem,
    constans SilvaNodus* species)
{
    TypusC89* basis;
    SilvaValor decl_v;

    si (sem == NIHIL || species == NIHIL)
    {
        redde (sem != NIHIL) ? sem->typus_erroris : NIHIL;
    }
    species = _canonicum(species);
    si (species->genus != (s32)SILVA_C89_GENUS_SPECIES_TYPI)
    {
        silva_c89_diagnosticum_addere(sem, species,
            EXAMEN_CODEX_SPECIES_TYPI_EXSPECTATA);
        redde sem->typus_erroris;
    }
    basis = _typus_ex_specificatoribus_interior(sem,
        silva_c89_species_typi_specificatores(species), NIHIL,
        NIHIL);
    decl_v = silva_c89_species_typi_declarator(species);
    redde silva_c89_typus_ex_declaratore(sem, basis,
        (decl_v.genus == SILVA_VALOR_NODUS)
            ? decl_v.datum.nodus : NIHIL,
        NIHIL);
}

i32
silva_c89_declarationem_tractare (SilvaSemantica* sem,
    constans SilvaNodus* declaratio)
{
    TypusC89* basis;
    b32 est_typedef = FALSUM;
    i32 repositio = ZEPHYRUM;
    SilvaValor declaratores;
    i32 i;
    i32 m;
    i32 numerus = ZEPHYRUM;

    si (sem == NIHIL || declaratio == NIHIL)
    {
        redde ZEPHYRUM;
    }
    declaratio = _canonicum(declaratio);
    si (declaratio->genus != (s32)SILVA_C89_GENUS_DECLARATIO)
    {
        redde ZEPHYRUM;
    }
    basis = _typus_ex_specificatoribus_interior(sem,
        silva_c89_declaratio_specificatores(declaratio),
        &est_typedef, &repositio);
    declaratores = silva_c89_declaratio_declaratores(declaratio);
    m = (i32)silva_valor_lista_numerus(declaratores);
    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* dv = silva_valor_lista_obtinere(declaratores, i);
        SilvaToken* tok = NIHIL;
        TypusC89* t;

        si (dv == NIHIL || dv->genus != SILVA_VALOR_NODUS)
        {
            perge;   /* virgulae */
        }
        t = silva_c89_typus_ex_declaratore(sem, basis,
            dv->datum.nodus, &tok);
        /* completio aciei per initiatorem (C89 3.5.7): "at the end
         * of its initializer list, the array no longer has
         * incomplete type" - dimensio EXTERIOR completur (congeries:
         * numerus elementorum - elisio in corpore ZEPHYRUM mensurata,
         * omnia plene vallata; chorda litteralis: octeti + I).
         * Sine initiatore (extern/tentativa) incompleta manet -
         * recte per normam. Typus NOVUS construitur (typi communes
         * sunt - mutatio vetita). */
        si (t != NIHIL && t->genus == TYPUS_C89_ACIES
            && t->datum.acies.numerus < ZEPHYRUM && !est_typedef)
        {
            constans SilvaNodus* d_compl =
                _canonicum(dv->datum.nodus);

            si (d_compl->genus
                == (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
            {
                SilvaValor init_v =
                    silva_c89_declarator_initiatus_initiator(
                        d_compl);

                si (init_v.genus == SILVA_VALOR_NODUS)
                {
                    constans SilvaNodus* init = _canonicum(
                        init_v.datum.nodus);
                    s32 numerus_elementorum = -I;

                    si (init->genus
                        == (s32)SILVA_C89_GENUS_CONGERIES)
                    {
                        SilvaValor elementa =
                            silva_c89_congeries_elementa(init);
                        i32 k;
                        i32 n = ZEPHYRUM;

                        per (k = ZEPHYRUM;
                             k < (i32)silva_valor_lista_numerus(
                                 elementa); k++)
                        {
                            SilvaValor* e =
                                silva_valor_lista_obtinere(
                                    elementa, k);

                            si (e != NIHIL
                                && e->genus == SILVA_VALOR_NODUS)
                            {
                                n++;
                            }
                        }
                        si (n > ZEPHYRUM)
                        {
                            numerus_elementorum = (s32)n;
                        }
                    }
                    alioquin si (init->genus
                        == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
                    {
                        chorda octeti;

                        si (silva_c89_chorda_decodere(sem->piscina,
                                init, &octeti))
                        {
                            numerus_elementorum =
                                (s32)(octeti.mensura + (i32)I);
                        }
                    }
                    si (numerus_elementorum > ZEPHYRUM)
                    {
                        t = silva_c89_typus_acies(sem,
                            t->datum.acies.elementum,
                            numerus_elementorum);
                    }
                }
            }
        }
        si (tok != NIHIL)
        {
            s32 genus_symboli;

            si (est_typedef)
            {
                genus_symboli = SYMBOLUM_TYPEDEF;
                _typedef_baptizare(sem, tok->valor, t);
            }
            alioquin si (t != NIHIL && t->genus == TYPUS_C89_FUNCTIO)
            {
                genus_symboli = SYMBOLUM_FUNCTIO;
            }
            alioquin
            {
                genus_symboli = SYMBOLUM_VARIABILE;
            }
            (vacuum)_symbolum_registrare(sem, genus_symboli,
                tok->valor, t, ZEPHYRUM, repositio,
                dv->datum.nodus, tok);
        }
        /* initiator typatur POST registrationem (int x = x; legale,
         * dextrum x novum videt - C89). Conversio ad typum
         * declaratum annotatur (B); congeries typum scopi accipit
         * in Chunk C. */
        {
            constans SilvaNodus* d = _canonicum(dv->datum.nodus);

            si (d->genus == (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
            {
                SilvaValor init_v =
                    silva_c89_declarator_initiatus_initiator(d);

                si (init_v.genus == SILVA_VALOR_NODUS)
                {
                    constans SilvaNodus* init = _canonicum(
                        init_v.datum.nodus);

                    si (init->genus == (s32)SILVA_C89_GENUS_CONGERIES)
                    {
                        /* typus scopi deorsum (M0b C, DECISUS 6) */
                        _congeriem_typare(sem, init, t);
                    }
                    alioquin
                    {
                        TypusC89* ti = _expressionem_typare(sem,
                            init);

                        si (ti != NIHIL)
                        {
                            _ad_finem_annotare(sem, init, ti, t);
                        }
                    }
                }
            }
        }
        numerus++;
    }
    redde numerus;
}

/* ==================================================
 * Ambulatio integra (Chunk B): ordine fontis, registra-ante-usum
 * ================================================== */

interior vacuum _elementum_ambulare (SilvaSemantica* sem,
    constans SilvaNodus* nodus);

interior vacuum
_listam_ambulare (SilvaSemantica* sem, SilvaValor lista)
{
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(lista);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(lista, i);

        si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
        {
            _elementum_ambulare(sem, v->datum.nodus);
        }
    }
}

interior vacuum
_nodalem_ambulare (SilvaSemantica* sem, SilvaValor v)
{
    si (v.genus == SILVA_VALOR_NODUS)
    {
        _elementum_ambulare(sem, v.datum.nodus);
    }
}

interior vacuum
_corpus_ambulare (SilvaSemantica* sem, constans SilvaNodus* corpus)
{
    _scopum_aperire(sem);
    _listam_ambulare(sem, silva_c89_corpus_elementa(corpus));
    _scopum_claudere(sem);
}

/* Functionis stratum INTIMUM catenae declaratoris (parametra
 * definitionis ibi habitant) */
interior constans SilvaNodus*
_functionis_intima (constans SilvaNodus* declarator)
{
    constans SilvaNodus* intima = NIHIL;
    i32 custos = ZEPHYRUM;

    dum (declarator != NIHIL && custos < LXIV)
    {
        SilvaValor v;

        declarator = _canonicum(declarator);
        commutatio (declarator->genus)
        {
            casus (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS:
                v = silva_c89_declarator_initiatus_declarator(
                    declarator);
                frange;
            casus (s32)SILVA_C89_GENUS_PARENTHESIS:
                v = silva_c89_parenthesis_internum(declarator);
                frange;
            casus (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR:
                v = silva_c89_declarator_monstrator_internum(
                    declarator);
                frange;
            casus (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS:
                v = silva_c89_declarator_abstractus_internum(
                    declarator);
                frange;
            casus (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI:
                v = silva_c89_declarator_aciei_internum(declarator);
                frange;
            casus (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS:
                intima = declarator;
                v = silva_c89_declarator_functionis_internum(
                    declarator);
                frange;
            ordinarius:
                redde intima;
        }
        declarator = (v.genus == SILVA_VALOR_NODUS)
            ? v.datum.nodus : NIHIL;
        custos++;
    }
    redde intima;
}

interior vacuum
_parametra_registrare (SilvaSemantica* sem,
    constans SilvaNodus* functionis)
{
    SilvaValor parametra = silva_c89_declarator_functionis_parametra(
        functionis);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(parametra);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(parametra, i);
        constans SilvaNodus* p;
        TypusC89* basis;
        TypusC89* t;
        SilvaToken* tok = NIHIL;
        SilvaValor decl_v;

        si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        p = _canonicum(v->datum.nodus);
        si (p->genus != (s32)SILVA_C89_GENUS_PARAMETRUM)
        {
            perge;
        }
        basis = _typus_ex_specificatoribus_interior(sem,
            silva_c89_parametrum_specificatores(p), NIHIL, NIHIL);
        decl_v = silva_c89_parametrum_declarator(p);
        t = silva_c89_typus_ex_declaratore(sem, basis,
            (decl_v.genus == SILVA_VALOR_NODUS)
                ? decl_v.datum.nodus : NIHIL,
            &tok);
        si (t->genus == TYPUS_C89_ACIES)
        {
            t = silva_c89_typus_monstrator(sem,
                t->datum.acies.elementum);
        }
        alioquin si (t->genus == TYPUS_C89_FUNCTIO)
        {
            t = silva_c89_typus_monstrator(sem, t);
        }
        si (tok != NIHIL)
        {
            (vacuum)_symbolum_registrare(sem, SYMBOLUM_PARAMETRUM,
                tok->valor, t, ZEPHYRUM, ZEPHYRUM, p, tok);
        }
    }
}

/* ==================================================
 * FLUXUS-0: CFG + gradus 63/66/67 (spec silva-fluxus-0-spec.md)
 * ================================================== */

/* Ligamina suturae auxiliorum: politica canonicalizationis et
 * plicatio constantium SEMANTICAE sunt, fluxus eas mutuatur (XI-1) */
interior constans SilvaNodus*
_fluxus_canonicum_ligamen (vacuum* contextus, constans SilvaNodus* nodus)
{
    (vacuum)contextus;
    redde _canonicum(nodus);
}

interior b32
_fluxus_aestimator_ligamen (vacuum* contextus,
    constans SilvaNodus* nodus, s64* valor)
{
    redde _constans_probare((SilvaSemantica*)contextus, nodus, valor);
}

/* Post ambulationem corporis (typationes plenae, reditus_currens
 * adhuc positus): CFG aedificare, retinere, gradus legere.
 * 66/67 = paritas ERRORIS clang (VIOLATIO, verdictum vertunt);
 * 63 = paritas -Wreturn-type (SUSPECTUM): margo CADIT fonte
 * attingibili in functione non vacua - attingibilitas sola formas
 * omnes idiomatum gerit (ansae infinitae plicatae, salta-ansae,
 * commutationes classificatorum - XI-6, nulla exceptio). */
interior vacuum
_fluxum_examinare (SilvaSemantica* sem, constans SilvaNodus* definitio)
{
    FluxusAuxilia aux;
    FluxusFunctionis* fluxus;
    FluxusFunctionis** locus;
    TypusC89* rc;
    i32 i;
    i32 m;

    aux.canonicum = _fluxus_canonicum_ligamen;
    aux.aestimator = _fluxus_aestimator_ligamen;
    aux.contextus = sem;
    fluxus = silva_c89_fluxus_aedificare(sem->piscina, definitio,
        &aux);
    si (fluxus == NIHIL)
    {
        redde;
    }
    locus = (FluxusFunctionis**)xar_addere(sem->fluxus_functionum);
    *locus = fluxus;

    /* codex 66: frange/perge sine contextu (clang errat) */
    m = xar_numerus(fluxus->fractiones_extra);
    per (i = ZEPHYRUM; i < m; i++)
    {
        silva_c89_diagnosticum_addere(sem,
            *(constans SilvaNodus**)xar_obtinere(
                fluxus->fractiones_extra, i),
            (s32)EXAMEN_CODEX_FRANGE_EXTRA_CONTEXTUM);
    }
    /* codex 67: salta ad titulum ignotum (clang errat) */
    m = xar_numerus(fluxus->saltus_ignoti);
    per (i = ZEPHYRUM; i < m; i++)
    {
        silva_c89_diagnosticum_addere(sem,
            *(constans SilvaNodus**)xar_obtinere(
                fluxus->saltus_ignoti, i),
            (s32)EXAMEN_CODEX_SALTA_AD_TITULUM_IGNOTUM);
    }
    /* codex 64: lapsus inter casus - margines LAPSUS quorum fons
     * attingibilis NEC caput gregis vacuum (cumulus titulorum:
     * casus 0: casus 1: - fons est caput vacuum, clang quoque
     * tacet). Flagrat AD titulum recipientem (margo.origo);
     * TOLERA-bilis (decisio Q2: TOLERA solum, idioma commenti
     * non honoratur). */
    m = xar_numerus(fluxus->bloci);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans FluxusBlocus* blocus = (constans FluxusBlocus*)
            xar_obtinere(fluxus->bloci, i);
        i32 k;
        i32 numerus_marginum = xar_numerus(blocus->margines);

        per (k = ZEPHYRUM; k < numerus_marginum; k++)
        {
            constans FluxusMargo* margo = (constans FluxusMargo*)
                xar_obtinere(blocus->margines, k);
            b32 cumulus;

            si (margo->genus != (s32)FLUXUS_MARGO_LAPSUS
                || !blocus->attingibilis)
            {
                perge;
            }
            cumulus = blocus->titulus_dux != NIHIL
                && xar_numerus(blocus->sententiae) == ZEPHYRUM;
            si (cumulus)
            {
                perge;
            }
            /* iudicium DOMESTICUM: codex alienus numquam iudicatur
             * (stb_image lapsus suos habet - census primus docuit) */
            si (_fons_alienus(sem, margo->origo))
            {
                perge;
            }
            si (_tolera_absorbere(sem, margo->origo,
                (s32)EXAMEN_CODEX_CASUS_LAPSUS))
            {
                perge;
            }
            silva_c89_diagnosticum_addere(sem, margo->origo,
                (s32)EXAMEN_CODEX_CASUS_LAPSUS);
        }
    }

    /* codex 65: sententia inattingibilis - blocus non attingibilis
     * cum sententia VERA. Vacuae et redde/frange/perge SOLAE
     * transeunt: clang eas sub-vexillis (-return/-break) separat
     * quia saltus defensivi caudales idioma sunt - paritatem cum
     * -Wunreachable-code PLANO tenemus. Flagrat ad sententiam veram
     * primam bloci; TOLERA-bilis. */
    m = xar_numerus(fluxus->bloci);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans FluxusBlocus* blocus = (constans FluxusBlocus*)
            xar_obtinere(fluxus->bloci, i);
        constans SilvaNodus* culpa = NIHIL;
        i32 k;
        i32 numerus_sententiarum;

        si (blocus->attingibilis || blocus->plicatione_exemptus)
        {
            perge;
        }
        numerus_sententiarum = xar_numerus(blocus->sententiae);
        per (k = ZEPHYRUM; k < numerus_sententiarum; k++)
        {
            constans SilvaNodus* s = *(constans SilvaNodus**)
                xar_obtinere(blocus->sententiae, k);

            si (s == NIHIL
                || s->genus == (s32)SILVA_C89_GENUS_SENTENTIA_VACUA
                || s->genus == (s32)SILVA_C89_GENUS_REDDE
                || s->genus == (s32)SILVA_C89_GENUS_FRANGE
                || s->genus == (s32)SILVA_C89_GENUS_PERGE)
            {
                perge;
            }
            culpa = s;
            frange;
        }
        si (culpa == NIHIL || _fons_alienus(sem, culpa))
        {
            perge;
        }
        si (_tolera_absorbere(sem, culpa,
            (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS))
        {
            perge;
        }
        silva_c89_diagnosticum_addere(sem, culpa,
            (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS);
    }

    /* codex 63: finis cadit in functione non vacua */
    rc = sem->reditus_currens;
    si (fluxus->cadit_attingibilis && rc != NIHIL
        && rc != sem->typus_erroris)
    {
        TypusC89* nudus = _qualibus_exutum(rc);
        b32 rc_vacuum = nudus != NIHIL
            && nudus->genus == TYPUS_C89_PRIMITIVUS
            && nudus->datum.primitivum == (s32)PRIMITIVUM_VACUUM;

        si (!rc_vacuum)
        {
            silva_c89_diagnosticum_addere(sem, definitio,
                (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU);
        }
    }
}

interior vacuum
_definitionem_ambulare (SilvaSemantica* sem,
    constans SilvaNodus* definitio)
{
    b32 est_typedef = FALSUM;
    i32 repositio = ZEPHYRUM;
    TypusC89* basis;
    TypusC89* t;
    TypusC89* reditus_prior;
    SilvaToken* tok = NIHIL;
    constans SilvaNodus* declarator = NIHIL;
    SilvaValor decl_v;
    SilvaValor kr_v;
    SilvaValor corpus_v;

    /* specificatores NIHIL = int implicitum (formae *-implicitae) */
    basis = _typus_ex_specificatoribus_interior(sem,
        silva_c89_definitio_functionis_specificatores(definitio),
        &est_typedef, &repositio);
    decl_v = silva_c89_definitio_functionis_declarator(definitio);
    si (decl_v.genus == SILVA_VALOR_NODUS)
    {
        declarator = decl_v.datum.nodus;
    }
    t = silva_c89_typus_ex_declaratore(sem, basis, declarator, &tok);
    si (tok != NIHIL)
    {
        (vacuum)_symbolum_registrare(sem, SYMBOLUM_FUNCTIO,
            tok->valor, t, ZEPHYRUM, repositio, definitio, tok);
    }
    kr_v = silva_c89_definitio_functionis_declarationes_kr(definitio);
    si (silva_valor_lista_numerus(kr_v) > ZEPHYRUM)
    {
        /* K&R - parca nominata (corpus eam continere non potest:
         * -Wstrict-prototypes -Werror; typus non prototypatus) */
        silva_c89_diagnosticum_addere(sem, definitio,
            EXAMEN_CODEX_DECLARATIONES_KR);
    }
    /* typus reditus pro conversione redde (M0b B) */
    reditus_prior = sem->reditus_currens;
    sem->reditus_currens = (t != NIHIL
        && t->genus == TYPUS_C89_FUNCTIO)
        ? t->datum.functio.reditus : NIHIL;
    _scopum_aperire(sem);
    si (declarator != NIHIL)
    {
        constans SilvaNodus* functionis = _functionis_intima(
            declarator);

        si (functionis != NIHIL)
        {
            _parametra_registrare(sem, functionis);
        }
    }
    corpus_v = silva_c89_definitio_functionis_corpus(definitio);
    si (corpus_v.genus == SILVA_VALOR_NODUS)
    {
        _corpus_ambulare(sem, corpus_v.datum.nodus);
        /* FLUXUS-0: reditus_currens adhuc positus (codex 63 eo eget) */
        _fluxum_examinare(sem, definitio);
    }
    _scopum_claudere(sem);
    sem->reditus_currens = reditus_prior;
}

interior vacuum
_elementum_ambulare (SilvaSemantica* sem, constans SilvaNodus* nodus)
{
    si (nodus == NIHIL)
    {
        redde;
    }
    nodus = _canonicum(nodus);
    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_DECLARATIO:
            (vacuum)silva_c89_declarationem_tractare(sem, nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS:
            _definitionem_ambulare(sem, nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_CORPUS:
            _corpus_ambulare(sem, nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_SI:
            _valorem_typare_scalarem(sem, silva_c89_si_conditio(
                nodus));
            _nodalem_ambulare(sem, silva_c89_si_consequens(nodus));
            _nodalem_ambulare(sem, silva_c89_si_alioquin(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_DUM:
            _valorem_typare_scalarem(sem, silva_c89_dum_conditio(
                nodus));
            _nodalem_ambulare(sem, silva_c89_dum_corpus(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_FAC_DUM:
            _valorem_typare_scalarem(sem, silva_c89_fac_dum_conditio(
                nodus));
            _nodalem_ambulare(sem, silva_c89_fac_dum_corpus(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_PER:
        {
            SilvaValor cl_v = silva_c89_per_clausula(nodus);

            si (cl_v.genus == SILVA_VALOR_NODUS)
            {
                constans SilvaNodus* cl = _canonicum(cl_v.datum.nodus);

                si (cl->genus == (s32)SILVA_C89_GENUS_PER_CLAUSULA)
                {
                    SilvaValor init_v = silva_c89_per_clausula_initium(
                        cl);

                    si (init_v.genus == SILVA_VALOR_NODUS)
                    {
                        constans SilvaNodus* init = _canonicum(
                            init_v.datum.nodus);

                        si (init->genus
                            == (s32)SILVA_C89_GENUS_DECLARATIO)
                        {
                            /* forma C99 - grammatica byte-conservativa
                             * eam fert; tractamus, lint iudicat */
                            (vacuum)silva_c89_declarationem_tractare(
                                sem, init);
                        }
                        alioquin
                        {
                            (vacuum)_expressionem_typare(sem, init);
                        }
                    }
                    _valorem_typare_scalarem(sem,
                        silva_c89_per_clausula_conditio(cl));
                    _valorem_typare(sem,
                        silva_c89_per_clausula_passus(cl));
                }
            }
            _nodalem_ambulare(sem, silva_c89_per_corpus(nodus));
            frange;
        }
        casus (s32)SILVA_C89_GENUS_COMMUTATIO:
            _valorem_typare(sem, silva_c89_commutatio_discrimen(
                nodus));
            _nodalem_ambulare(sem, silva_c89_commutatio_corpus(
                nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_CASUS:
            _valorem_typare(sem, silva_c89_casus_valor(nodus));
            _listam_ambulare(sem, silva_c89_casus_sententiae(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_REDDE:
        {
            SilvaValor val_v = silva_c89_redde_valor(nodus);
            TypusC89* rc = sem->reditus_currens;
            b32 rc_vacuum = rc != NIHIL
                && _qualibus_exutum(rc) != NIHIL
                && _qualibus_exutum(rc)->genus
                    == TYPUS_C89_PRIMITIVUS
                && _qualibus_exutum(rc)->datum.primitivum
                    == (s32)PRIMITIVUM_VACUUM;

            si (val_v.genus == SILVA_VALOR_NODUS)
            {
                constans SilvaNodus* val = _canonicum(
                    val_v.datum.nodus);
                TypusC89* tv = _expressionem_typare(sem, val);

                si (rc_vacuum)
                {
                    /* C89 6.6.6.4 constrictio (chunk C) */
                    silva_c89_diagnosticum_addere(sem, val,
                        EXAMEN_CODEX_REDDE_CUM_VALORE_IN_VACUO);
                }
                alioquin si (tv != NIHIL && rc != NIHIL)
                {
                    _ad_finem_annotare(sem, val, tv, rc);
                    _sentinellam_examinare(sem, val);
                }
            }
            alioquin si (rc != NIHIL && !rc_vacuum)
            {
                /* legale C89 - SUSPECTUM (clang-16 reicit etiam
                 * nudum; ordo EXCLUSIO chunk D) */
                silva_c89_diagnosticum_addere(sem, nodus,
                    EXAMEN_CODEX_REDDE_SINE_VALORE);
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS:
            _valorem_typare(sem,
                silva_c89_sententia_expressionis_expressio(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_ORDINARIUS:
            _listam_ambulare(sem, silva_c89_ordinarius_sententiae(
                nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_TITULATUM:
            _nodalem_ambulare(sem, silva_c89_titulatum_sententia(
                nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_CONDITIONALIS:
            _listam_ambulare(sem, silva_c89_conditionalis_rami(
                nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS:
            _listam_ambulare(sem, silva_c89_ramus_sumptus_contentum(
                nodus));
            frange;
        /* RAMUS_OMISSUS (cruda), ERROR: praetermissi */
        ordinarius:
            frange;
    }
}

SilvaSemantica*
silva_c89_semantica_analysare (Piscina* piscina,
    constans SilvaParsura* parsura)
{
    redde silva_c89_semantica_analysare_cum_systemate(piscina,
        parsura, NIHIL);
}

SilvaSemantica*
silva_c89_semantica_analysare_cum_systemate (Piscina* piscina,
    constans SilvaParsura* parsura,
    constans SilvaParsura* systema)
{
    SilvaSemantica* sem;

    si (piscina == NIHIL || parsura == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    sem = silva_c89_semantica_creare(piscina);
    si (sem == NIHIL)
    {
        redde NIHIL;
    }
    si (systema != NIHIL && systema->commissio != NIHIL)
    {
        sem->in_systemate = VERUM;
        sem->parsura_currens = systema;
        _listam_ambulare(sem, systema->commissio->radix);
        sem->in_systemate = FALSUM;
    }
    sem->parsura_currens = parsura;
    _listam_ambulare(sem, parsura->commissio->radix);
    /* materializatio est_implicitum (chunk C): canalis symbolorum
     * in flumen diagnosticorum unitum - SUSPECTUM ad sedem
     * vocationis primae (declarans); systema exclusum */
    {
        i32 i;
        i32 m = xar_numerus(sem->symbola);

        per (i = ZEPHYRUM; i < m; i++)
        {
            SemanticaSymbolum* s = *(SemanticaSymbolum**)
                xar_obtinere(sem->symbola, i);

            si (s != NIHIL && s->est_implicitum && !s->ex_systemate)
            {
                silva_c89_diagnosticum_addere(sem, s->declarans,
                    EXAMEN_CODEX_VOCATIO_IMPLICITA);
            }
        }
    }
    /* macro domesticum in capite alieno (sequela M4a): ambulatio
     * lexematum TU usoris - systema exclusum per constructionem
     * (parsura usoris sola datur) */
    _macros_domestica_in_alienis_examinare(sem, parsura);
    /* TOLERA irrita (gradus severi): suppressiones quae nihil
     * absorbuerunt aut sine causa - post ambulationem totam */
    _toleras_irritas_examinare(sem, parsura);
    /* MENU-FINALE: inutilia (69/70) - post ambulationem totam */
    _inutiles_examinare(sem);
    redde sem;
}

i32
silva_c89_semantica_oraculum_augere (
    constans SilvaSemantica* sem, SilvaOraculum* oraculum)
{
    i32 i;
    i32 numerus = ZEPHYRUM;

    si (sem == NIHIL || oraculum == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(sem->symbola); i++)
    {
        SemanticaSymbolum* symbolum =
            *(SemanticaSymbolum**)xar_obtinere(sem->symbola, i);
        b32 bene;

        si (symbolum->genus == (s32)SYMBOLUM_TYPEDEF)
        {
            bene = silva_oraculum_typum_addere(oraculum,
                symbolum->titulus);
        }
        alioquin
        {
            bene = silva_oraculum_non_typum_addere(oraculum,
                symbolum->titulus);
        }
        si (bene)
        {
            numerus++;
        }
    }
    redde numerus;
}

i32
silva_c89_symbola_numerus (constans SilvaSemantica* sem)
{
    si (sem == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde (i32)xar_numerus(sem->symbola);
}

constans SemanticaSymbolum*
silva_c89_symbolum_per_indicem (constans SilvaSemantica* sem,
    i32 index)
{
    si (sem == NIHIL || index >= xar_numerus(sem->symbola))
    {
        redde NIHIL;
    }
    redde *(SemanticaSymbolum**)xar_obtinere(sem->symbola, index);
}

i32
silva_c89_diagnostica_numerus (constans SilvaSemantica* sem)
{
    si (sem == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde (i32)xar_numerus(sem->diagnostica);
}

constans SemanticaDiagnosticum*
silva_c89_diagnosticum_per_indicem (constans SilvaSemantica* sem,
    i32 index)
{
    si (sem == NIHIL || index >= xar_numerus(sem->diagnostica))
    {
        redde NIHIL;
    }
    redde (constans SemanticaDiagnosticum*)xar_obtinere(
        sem->diagnostica, index);
}

i32
silva_c89_ambigua_indecisa_numerare (
    constans SilvaCommissio* commissio,
    constans SilvaOraculum*  oraculum)
{
    i32 i;
    i32 indecisa = ZEPHYRUM;

    si (commissio == NIHIL || oraculum == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(commissio->ambigui); i++)
    {
        SilvaNodus** slot =
            (SilvaNodus**)xar_obtinere(commissio->ambigui, i);
        SilvaResolutioResponsum responsum;

        si (slot == NIHIL || *slot == NIHIL)
        {
            perge;
        }
        responsum.victor = -I;
        responsum.discriminans = NIHIL;
        silva_c89_resolutor(*slot, oraculum, NIHIL, &responsum);
        si (responsum.victor < ZEPHYRUM)
        {
            indecisa++;
        }
    }
    redde indecisa;
}

/* ==================================================
 * Forma (pigra; arm64 LP64 Apple)
 * ================================================== */

interior i32
_sursum_ordinare (i32 valor, i32 ordinatio)
{
    si (ordinatio <= I)
    {
        redde valor;
    }
    redde ((valor + ordinatio - I) / ordinatio) * ordinatio;
}

b32
silva_c89_formam_computare (SilvaSemantica* sem, TypusC89* typus)
{
    si (sem == NIHIL || typus == NIHIL)
    {
        redde FALSUM;
    }
    si (typus->forma_computata)
    {
        redde typus->magnitudo_octetorum > ZEPHYRUM;
    }
    si (typus->in_computatione)
    {
        silva_c89_diagnosticum_addere(sem, _declarans_typi(typus),
            EXAMEN_CODEX_CYCLUS_FORMAE);
        redde FALSUM;
    }
    typus->in_computatione = VERUM;

    commutatio (typus->genus)
    {
        casus TYPUS_C89_MONSTRATOR:
            typus->magnitudo_octetorum = VIII;
            typus->ordinatio = VIII;
            frange;
        casus TYPUS_C89_QUALIFICATUS:
            si (silva_c89_formam_computare(sem,
                    typus->datum.qualificatus.internum))
            {
                typus->magnitudo_octetorum =
                    typus->datum.qualificatus.internum
                        ->magnitudo_octetorum;
                typus->ordinatio =
                    typus->datum.qualificatus.internum->ordinatio;
            }
            frange;
        casus TYPUS_C89_ACIES:
            si (typus->datum.acies.numerus > ZEPHYRUM
                && silva_c89_formam_computare(sem,
                       typus->datum.acies.elementum))
            {
                typus->magnitudo_octetorum =
                    typus->datum.acies.elementum->magnitudo_octetorum
                        * (i32)typus->datum.acies.numerus;
                typus->ordinatio =
                    typus->datum.acies.elementum->ordinatio;
            }
            alioquin
            {
                silva_c89_diagnosticum_addere(sem,
                    _declarans_typi(typus),
                    EXAMEN_CODEX_FORMA_ACIEI_INCOMPLETAE);
            }
            frange;
        casus TYPUS_C89_FUNCTIO:
            silva_c89_diagnosticum_addere(sem, _declarans_typi(typus),
                EXAMEN_CODEX_FORMA_FUNCTIONIS);
            frange;
        casus TYPUS_C89_STRUCTURA:
        casus TYPUS_C89_UNIO:
        {
            i32 k;
            i32 cursor = ZEPHYRUM;
            i32 maxima_ordinatio = I;
            i32 maxima_magnitudo = ZEPHYRUM;
            b32 bene = typus->datum.tag.completa
                && !typus->datum.tag.habet_campos
                && typus->datum.tag.numerus_membrorum > ZEPHYRUM;

            si (typus->datum.tag.habet_campos)
            {
                silva_c89_diagnosticum_addere(sem,
                    typus->datum.tag.declarans,
                    EXAMEN_CODEX_CAMPI_PARCATI);
            }
            per (k = ZEPHYRUM;
                 bene && k < (i32)typus->datum.tag.numerus_membrorum;
                 k++)
            {
                TypusC89Membrum* membrum = &typus->datum.tag.membra[k];

                si (!silva_c89_formam_computare(sem, membrum->typus))
                {
                    bene = FALSUM;
                    frange;
                }
                si (membrum->typus->ordinatio > maxima_ordinatio)
                {
                    maxima_ordinatio = membrum->typus->ordinatio;
                }
                si (typus->genus == TYPUS_C89_STRUCTURA)
                {
                    cursor = _sursum_ordinare(cursor,
                        membrum->typus->ordinatio);
                    membrum->offset = cursor;
                    cursor = cursor
                        + membrum->typus->magnitudo_octetorum;
                }
                alioquin
                {
                    membrum->offset = ZEPHYRUM;
                    si (membrum->typus->magnitudo_octetorum
                        > maxima_magnitudo)
                    {
                        maxima_magnitudo =
                            membrum->typus->magnitudo_octetorum;
                    }
                }
            }
            si (bene)
            {
                typus->ordinatio = maxima_ordinatio;
                typus->magnitudo_octetorum = _sursum_ordinare(
                    (typus->genus == TYPUS_C89_STRUCTURA)
                        ? cursor : maxima_magnitudo,
                    maxima_ordinatio);
            }
            frange;
        }
        /* PRIMITIVUS/ENUMERATUS/ERROR formas statim positas habent
         * (forma_computata VERUM) - huc numquam veniunt */
        ordinarius:
            frange;
    }

    typus->in_computatione = FALSUM;
    typus->forma_computata = VERUM;
    redde typus->magnitudo_octetorum > ZEPHYRUM;
}

/* ==================================================
 * Aestimator (expressiones constantes integrae)
 * ================================================== */

/* Litterale integrum legere: valor + basis + suffixa (M0b: typatio
 * suffixa postulat; aestimator valorem solum). FALSUM = forma
 * invalida. NB valores > s64 involvuntur (nota worklog). */
interior b32
_folium_integer_legere (constans SilvaToken* tok, s64* valor_out,
    i32* basis_out, b32* habet_u_out, i32* numerus_l_out)
{
    s64 valor = ZEPHYRUM;
    i32 i = ZEPHYRUM;
    i32 basis = X;
    b32 habet_u = FALSUM;
    i32 numerus_l = ZEPHYRUM;

    si (tok->valor.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (tok->valor.datum[ZEPHYRUM] == '0' && tok->valor.mensura > I)
    {
        character secundum = (character)tok->valor.datum[I];

        si (secundum == 'x' || secundum == 'X')
        {
            basis = XVI;
            i = II;
        }
        alioquin
        {
            basis = VIII;
            i = I;
        }
    }
    per (; i < tok->valor.mensura; i++)
    {
        character c = (character)tok->valor.datum[i];
        s64 cifra;

        si (c >= '0' && c <= '9')
        {
            cifra = (s64)(c - '0');
        }
        alioquin si (basis == XVI && c >= 'a' && c <= 'f')
        {
            cifra = (s64)(c - 'a') + X;
        }
        alioquin si (basis == XVI && c >= 'A' && c <= 'F')
        {
            cifra = (s64)(c - 'A') + X;
        }
        alioquin si (c == 'u' || c == 'U' || c == 'l' || c == 'L')
        {
            frange;   /* suffixa infra */
        }
        alioquin
        {
            redde FALSUM;
        }
        valor = valor * (s64)basis + cifra;
    }
    per (; i < tok->valor.mensura; i++)
    {
        character c = (character)tok->valor.datum[i];

        si (c == 'u' || c == 'U')
        {
            si (habet_u)
            {
                redde FALSUM;
            }
            habet_u = VERUM;
        }
        alioquin si (c == 'l' || c == 'L')
        {
            numerus_l++;
            si (numerus_l > II)
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    *valor_out = valor;
    si (basis_out != NIHIL)
    {
        *basis_out = basis;
    }
    si (habet_u_out != NIHIL)
    {
        *habet_u_out = habet_u;
    }
    si (numerus_l_out != NIHIL)
    {
        *numerus_l_out = numerus_l;
    }
    redde VERUM;
}

interior b32
_folium_integer_aestimare (constans SilvaNodus* nodus, s64* valor_out)
{
    SilvaValor tok_v = silva_c89_folium_integer_tok_valor(nodus);

    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    redde _folium_integer_legere(tok_v.datum.token, valor_out,
        NIHIL, NIHIL, NIHIL);
}

/* Fuga characteris C89 post '\\' (M0b Chunk A - \x/\NNN exparcata;
 * communis aestimatori et mensurae chordae litteralis). *cursor
 * super fugam movetur (post ultimum octetum eius); valor octeti
 * (0..255) in *valor_out. FALSUM = fuga invalida. */
interior b32
_fugam_decodere (constans chorda* textus, i32* cursor, s64* valor_out)
{
    i32 i = *cursor;
    character c;

    si (i >= textus->mensura)
    {
        redde FALSUM;
    }
    c = (character)textus->datum[i];
    commutatio (c)
    {
        casus 'n':  *valor_out = '\n'; *cursor = i + I; redde VERUM;
        casus 't':  *valor_out = '\t'; *cursor = i + I; redde VERUM;
        casus 'r':  *valor_out = '\r'; *cursor = i + I; redde VERUM;
        casus 'a':  *valor_out = '\a'; *cursor = i + I; redde VERUM;
        casus 'b':  *valor_out = '\b'; *cursor = i + I; redde VERUM;
        casus 'f':  *valor_out = '\f'; *cursor = i + I; redde VERUM;
        casus 'v':  *valor_out = '\v'; *cursor = i + I; redde VERUM;
        casus '\\': *valor_out = '\\'; *cursor = i + I; redde VERUM;
        casus '\'': *valor_out = '\''; *cursor = i + I; redde VERUM;
        casus '"':  *valor_out = '"';  *cursor = i + I; redde VERUM;
        casus '?':  *valor_out = '?';  *cursor = i + I; redde VERUM;
        ordinarius: frange;
    }
    si (c == 'x')
    {
        /* hexadecimalis: unus pluresve digiti (C89); & 0xff */
        s64 valor = ZEPHYRUM;
        i32 k = i + I;
        b32 ullus = FALSUM;

        dum (k < textus->mensura)
        {
            character h = (character)textus->datum[k];
            s64 cifra;

            si (h >= '0' && h <= '9')
            {
                cifra = (s64)(h - '0');
            }
            alioquin si (h >= 'a' && h <= 'f')
            {
                cifra = (s64)(h - 'a') + X;
            }
            alioquin si (h >= 'A' && h <= 'F')
            {
                cifra = (s64)(h - 'A') + X;
            }
            alioquin
            {
                frange;
            }
            valor = valor * XVI + cifra;
            ullus = VERUM;
            k++;
        }
        si (!ullus)
        {
            redde FALSUM;
        }
        *valor_out = valor & 0xff;
        *cursor = k;
        redde VERUM;
    }
    si (c >= '0' && c <= '7')
    {
        /* octalis: usque ad tres digitos */
        s64 valor = ZEPHYRUM;
        i32 k = i;
        i32 n = ZEPHYRUM;

        dum (k < textus->mensura && n < III)
        {
            character o = (character)textus->datum[k];

            si (o < '0' || o > '7')
            {
                frange;
            }
            valor = valor * VIII + (s64)(o - '0');
            k++;
            n++;
        }
        *valor_out = valor & 0xff;
        *cursor = k;
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_folium_character_aestimare (constans SilvaNodus* nodus,
    s64* valor_out)
{
    SilvaValor tok_v = silva_c89_folium_character_tok_valor(nodus);
    constans SilvaToken* tok;
    s64 valor;
    i32 cursor;

    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    tok = tok_v.datum.token;
    si (tok->valor.mensura < III)   /* 'x' minimum */
    {
        redde FALSUM;
    }
    si (tok->valor.datum[ZEPHYRUM] == 'L')
    {
        redde FALSUM;   /* character latus - parca */
    }
    cursor = I;
    si ((character)tok->valor.datum[cursor] == '\\')
    {
        cursor++;
        si (!_fugam_decodere(&tok->valor, &cursor, &valor))
        {
            redde FALSUM;
        }
    }
    alioquin
    {
        valor = (s64)tok->valor.datum[cursor];
        cursor++;
    }
    si (cursor + I != tok->valor.mensura)
    {
        redde FALSUM;   /* multi-characterum: ab impl. definitum - parca */
    }
    *valor_out = (s64)(signatus character)(valor & 0xff);
    redde VERUM;
}

/* truncatio ad typum integrum (conversio in expressione constante) */
interior b32
_truncare (constans TypusC89* typus, s64* valor)
{
    constans TypusC89* t = typus;
    s32 p;

    dum (t->genus == TYPUS_C89_QUALIFICATUS)
    {
        t = t->datum.qualificatus.internum;
    }
    si (t->genus == TYPUS_C89_ENUMERATUS)
    {
        p = (s32)PRIMITIVUM_INTEGER;
    }
    alioquin si (t->genus == TYPUS_C89_PRIMITIVUS)
    {
        p = t->datum.primitivum;
    }
    alioquin
    {
        redde FALSUM;
    }
    commutatio (p)
    {
        casus PRIMITIVUM_CHARACTER:
        casus PRIMITIVUM_CHARACTER_SIGNATUM:
            *valor = (s64)(signatus character)(*valor & 0xff);
            frange;
        casus PRIMITIVUM_CHARACTER_INSIGNATUM:
            *valor = *valor & 0xff;
            frange;
        casus PRIMITIVUM_BREVIS:
            *valor = (s64)(brevis)(*valor & 0xffff);
            frange;
        casus PRIMITIVUM_BREVIS_INSIGNATUM:
            *valor = *valor & 0xffff;
            frange;
        casus PRIMITIVUM_INTEGER:
            *valor = (s64)(s32)(*valor & 0xffffffff);
            frange;
        casus PRIMITIVUM_INTEGER_INSIGNATUM:
            *valor = *valor & 0xffffffff;
            frange;
        casus PRIMITIVUM_LONGUS:
        casus PRIMITIVUM_LONGUS_INSIGNATUM:
        casus PRIMITIVUM_LONGUS_LONGUS:
        casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
            frange;   /* 64 bit - nihil mutandum */
        ordinarius:
            redde FALSUM;   /* vacuum/fluitans - non integra */
    }
    redde VERUM;
}

b32
silva_c89_constans_aestimare (SilvaSemantica* sem,
    constans SilvaNodus* expressio, s64* valor_out)
{
    si (sem == NIHIL || expressio == NIHIL || valor_out == NIHIL)
    {
        redde FALSUM;
    }
    expressio = _canonicum(expressio);

    commutatio (expressio->genus)
    {
        casus (s32)SILVA_C89_GENUS_FOLIUM_INTEGER:
            redde _folium_integer_aestimare(expressio, valor_out);
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER:
            redde _folium_character_aestimare(expressio, valor_out);
        casus (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR:
        {
            SilvaValor tok_v =
                silva_c89_folium_identificator_tok_valor(expressio);
            SemanticaSymbolum* symbolum;

            si (tok_v.genus != SILVA_VALOR_TOKEN)
            {
                redde FALSUM;
            }
            symbolum = silva_c89_symbolum_invenire(sem,
                tok_v.datum.token->valor);
            si (symbolum == NIHIL
                || symbolum->genus != SYMBOLUM_CONSTANS)
            {
                redde FALSUM;
            }
            *valor_out = symbolum->valor;
            redde VERUM;
        }
        casus (s32)SILVA_C89_GENUS_PARENTHESIS:
        {
            SilvaValor v = silva_c89_parenthesis_internum(expressio);

            si (v.genus != SILVA_VALOR_NODUS)
            {
                redde FALSUM;
            }
            redde silva_c89_constans_aestimare(sem, v.datum.nodus,
                valor_out);
        }
        casus (s32)SILVA_C89_GENUS_UNARIUM:
        {
            SilvaValor op_v = silva_c89_unarium_tok_operator(
                expressio);
            SilvaValor v = silva_c89_unarium_internum(expressio);
            s64 valor = ZEPHYRUM;

            si (op_v.genus != SILVA_VALOR_TOKEN
                || v.genus != SILVA_VALOR_NODUS
                || !silva_c89_constans_aestimare(sem, v.datum.nodus,
                       &valor))
            {
                redde FALSUM;
            }
            commutatio ((s32)op_v.datum.token->genus)
            {
                casus SILVA_LEX_MINUS:      *valor_out = -valor; frange;
                casus SILVA_LEX_PLUS:       *valor_out = valor;  frange;
                casus SILVA_LEX_TILDE:      *valor_out = ~valor; frange;
                casus SILVA_LEX_EXCLAMATIO:
                    *valor_out = (valor == ZEPHYRUM) ? I : ZEPHYRUM;
                    frange;
                ordinarius:
                    redde FALSUM;   /* & * ++ -- non constantia */
            }
            redde VERUM;
        }
        casus (s32)SILVA_C89_GENUS_BINARIUM:
        {
            SilvaValor op_v = silva_c89_binarium_tok_operator(
                expressio);
            SilvaValor s_v = silva_c89_binarium_sinister(expressio);
            SilvaValor d_v = silva_c89_binarium_dexter(expressio);
            s64 s = ZEPHYRUM;
            s64 d = ZEPHYRUM;

            si (op_v.genus != SILVA_VALOR_TOKEN
                || s_v.genus != SILVA_VALOR_NODUS
                || d_v.genus != SILVA_VALOR_NODUS
                || !silva_c89_constans_aestimare(sem, s_v.datum.nodus,
                       &s)
                || !silva_c89_constans_aestimare(sem, d_v.datum.nodus,
                       &d))
            {
                redde FALSUM;
            }
            commutatio ((s32)op_v.datum.token->genus)
            {
                casus SILVA_LEX_PLUS:  *valor_out = s + d; frange;
                casus SILVA_LEX_MINUS: *valor_out = s - d; frange;
                casus SILVA_LEX_STAR:  *valor_out = s * d; frange;
                casus SILVA_LEX_SOLIDUS:
                    si (d == ZEPHYRUM)
                    {
                        redde FALSUM;
                    }
                    *valor_out = s / d;
                    frange;
                casus SILVA_LEX_PERCENTUM:
                    si (d == ZEPHYRUM)
                    {
                        redde FALSUM;
                    }
                    *valor_out = s % d;
                    frange;
                casus SILVA_LEX_SINISTRORSUM:
                    *valor_out = s << d;
                    frange;
                casus SILVA_LEX_DEXTRORSUM:
                    *valor_out = s >> d;
                    frange;
                casus SILVA_LEX_MINOR:
                    *valor_out = (s < d) ? I : ZEPHYRUM;
                    frange;
                casus SILVA_LEX_MAIOR:
                    *valor_out = (s > d) ? I : ZEPHYRUM;
                    frange;
                casus SILVA_LEX_MINOR_AEQUALIS:
                    *valor_out = (s <= d) ? I : ZEPHYRUM;
                    frange;
                casus SILVA_LEX_MAIOR_AEQUALIS:
                    *valor_out = (s >= d) ? I : ZEPHYRUM;
                    frange;
                casus SILVA_LEX_AEQUALIS_AEQUALIS:
                    *valor_out = (s == d) ? I : ZEPHYRUM;
                    frange;
                casus SILVA_LEX_NON_AEQUALIS:
                    *valor_out = (s != d) ? I : ZEPHYRUM;
                    frange;
                casus SILVA_LEX_AMPERSAND:
                    *valor_out = s & d;
                    frange;
                casus SILVA_LEX_CARET:
                    *valor_out = s ^ d;
                    frange;
                casus SILVA_LEX_BARRA:
                    *valor_out = s | d;
                    frange;
                casus SILVA_LEX_ET_ET:
                    *valor_out = (s != ZEPHYRUM && d != ZEPHYRUM)
                        ? I : ZEPHYRUM;
                    frange;
                casus SILVA_LEX_VEL_VEL:
                    *valor_out = (s != ZEPHYRUM || d != ZEPHYRUM)
                        ? I : ZEPHYRUM;
                    frange;
                ordinarius:
                    redde FALSUM;
            }
            redde VERUM;
        }
        casus (s32)SILVA_C89_GENUS_TERNARIUS:
        {
            SilvaValor c_v = silva_c89_ternarius_conditio(expressio);
            s64 conditio = ZEPHYRUM;
            SilvaValor ramus;

            si (c_v.genus != SILVA_VALOR_NODUS
                || !silva_c89_constans_aestimare(sem, c_v.datum.nodus,
                       &conditio))
            {
                redde FALSUM;
            }
            ramus = (conditio != ZEPHYRUM)
                ? silva_c89_ternarius_verum(expressio)
                : silva_c89_ternarius_falsum(expressio);
            si (ramus.genus != SILVA_VALOR_NODUS)
            {
                redde FALSUM;
            }
            redde silva_c89_constans_aestimare(sem, ramus.datum.nodus,
                valor_out);
        }
        casus (s32)SILVA_C89_GENUS_CONVERSIO:
        {
            SilvaValor typus_v = silva_c89_conversio_typus(expressio);
            SilvaValor v = silva_c89_conversio_internum(expressio);
            TypusC89* typus;
            s64 valor = ZEPHYRUM;

            si (typus_v.genus != SILVA_VALOR_NODUS
                || v.genus != SILVA_VALOR_NODUS
                || !silva_c89_constans_aestimare(sem, v.datum.nodus,
                       &valor))
            {
                redde FALSUM;
            }
            typus = silva_c89_typus_ex_specie(sem,
                typus_v.datum.nodus);
            si (!_truncare(typus, &valor))
            {
                redde FALSUM;
            }
            *valor_out = valor;
            redde VERUM;
        }
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI:
        {
            SilvaValor typus_v = silva_c89_magnitudo_typi_typus(
                expressio);
            TypusC89* typus;

            si (typus_v.genus != SILVA_VALOR_NODUS)
            {
                redde FALSUM;
            }
            typus = silva_c89_typus_ex_specie(sem,
                typus_v.datum.nodus);
            si (!silva_c89_formam_computare(sem, typus))
            {
                redde FALSUM;
            }
            *valor_out = (s64)typus->magnitudo_octetorum;
            redde VERUM;
        }
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS:
        {
            /* EXPARCATUM (M0b C): typus operandi per typationem -
             * recursio triplex aestimator<->forma<->typatio.
             * Naturalis metitur (acies manet acies - sizeof). */
            SilvaValor v = silva_c89_magnitudo_expressionis_internum(
                expressio);
            TypusC89* t;

            si (v.genus != SILVA_VALOR_NODUS)
            {
                redde FALSUM;
            }
            t = _expressionem_typare(sem, _canonicum(v.datum.nodus));
            si (t == NIHIL
                || _qualibus_exutum(t)->genus == TYPUS_C89_ERROR)
            {
                redde FALSUM;
            }
            si (!silva_c89_formam_computare(sem, t))
            {
                redde FALSUM;
            }
            *valor_out = (s64)t->magnitudo_octetorum;
            redde VERUM;
        }
        ordinarius:
            redde FALSUM;
    }
}

/* ==================================================
 * Typatio expressionum (M0b Chunk A) - tabula parallela
 * nodus -> SemanticaTypatio {naturalis, conversus}. Clavis =
 * octeti valoris monstratoris nodi (lectionis canonicae);
 * FNV super octetos binariae-tutum, fabrica _chorda sufficit.
 * Genera B/C (binarium/vocatio/...) hic solum DESCENDUNTUR
 * (liberi typantur, nodus ipse nondum) - NIHIL sine strepitu.
 * ================================================== */

nomen structura {
    i8        clavis_octeti[magnitudo(vacuum*)];  /* valor monstratoris */
    TypusC89* naturalis;
    TypusC89* conversus;    /* NIHIL = nulla conversio */
} SemanticaTypatio;

interior SemanticaTypatio*
_typationem_invenire (constans SilvaSemantica* sem,
    constans SilvaNodus* nodus)
{
    i8 octeti[magnitudo(vacuum*)];
    chorda clavis;
    vacuum* valor = NIHIL;

    memcpy(octeti, &nodus, magnitudo(octeti));
    clavis.datum = octeti;
    clavis.mensura = (i32)magnitudo(octeti);
    si (tabula_dispersa_invenire(sem->typationes, clavis, &valor))
    {
        redde (SemanticaTypatio*)valor;
    }
    redde NIHIL;
}

interior SemanticaTypatio*
_typationem_ponere (SilvaSemantica* sem, constans SilvaNodus* nodus,
    TypusC89* naturalis)
{
    SemanticaTypatio* t = _typationem_invenire(sem, nodus);
    chorda clavis;

    si (t != NIHIL)
    {
        t->naturalis = naturalis;
        redde t;
    }
    t = (SemanticaTypatio*)piscina_allocare(sem->piscina,
        (memoriae_index)magnitudo(SemanticaTypatio));
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(t->clavis_octeti, &nodus, magnitudo(t->clavis_octeti));
    t->naturalis = naturalis;
    t->conversus = NIHIL;
    clavis.datum = t->clavis_octeti;
    clavis.mensura = (i32)magnitudo(t->clavis_octeti);
    (vacuum)tabula_dispersa_inserere(sem->typationes, clavis,
        (vacuum*)t);
    redde t;
}

interior vacuum
_conversum_ponere (SilvaSemantica* sem, constans SilvaNodus* nodus,
    TypusC89* conversus)
{
    SemanticaTypatio* t = _typationem_invenire(sem, nodus);

    si (t != NIHIL)
    {
        t->conversus = conversus;
    }
}

/* ==================================================
 * Nexus symbolorum (M1a Chunk A) - tabula parallela sedes usus ->
 * symbolum, exemplar typationis idem (clavis = octeti monstratoris
 * nodi lectionis canonicae). Registratur ubi resolutio VERE fit:
 * folium-identificator + vocatus vocationis (etiam implicitus).
 * ================================================== */

nomen structura {
    i8                 clavis_octeti[magnitudo(vacuum*)];
    SemanticaSymbolum* symbolum;
} SemanticaNexus;

interior SemanticaNexus*
_nexum_invenire (constans SilvaSemantica* sem,
    constans SilvaNodus* nodus)
{
    i8 octeti[magnitudo(vacuum*)];
    chorda clavis;
    vacuum* valor = NIHIL;

    memcpy(octeti, &nodus, magnitudo(octeti));
    clavis.datum = octeti;
    clavis.mensura = (i32)magnitudo(octeti);
    si (tabula_dispersa_invenire(sem->nexus, clavis, &valor))
    {
        redde (SemanticaNexus*)valor;
    }
    redde NIHIL;
}

interior vacuum
_nexum_ponere (SilvaSemantica* sem, constans SilvaNodus* nodus,
    SemanticaSymbolum* symbolum)
{
    SemanticaNexus* n = _nexum_invenire(sem, nodus);
    chorda clavis;

    si (symbolum != NIHIL)
    {
        symbolum->usus = VERUM;   /* codices 69/70 */
    }

    si (n != NIHIL)
    {
        n->symbolum = symbolum;
        redde;
    }
    n = (SemanticaNexus*)piscina_allocare(sem->piscina,
        (memoriae_index)magnitudo(SemanticaNexus));
    si (n == NIHIL)
    {
        redde;
    }
    memcpy(n->clavis_octeti, &nodus, magnitudo(n->clavis_octeti));
    n->symbolum = symbolum;
    clavis.datum = n->clavis_octeti;
    clavis.mensura = (i32)magnitudo(n->clavis_octeti);
    (vacuum)tabula_dispersa_inserere(sem->nexus, clavis,
        (vacuum*)n);
}

/* Quales exuere (valor r) */
interior TypusC89*
_qualibus_exutum (TypusC89* typus)
{
    dum (typus != NIHIL && typus->genus == TYPUS_C89_QUALIFICATUS)
    {
        typus = typus->datum.qualificatus.internum;
    }
    redde typus;
}

/* Intra involucrum AMBIGUUM retentum? Lectiones canonicae furcarum
 * retentarum typum nominatum ut expressionem continere possunt
 * (catenae nidificatae, C1) - strepitus furcae, non codex verus;
 * clausura (Chunk D bis-analysis) et parca resolutoris eas
 * possident. Diagnostica typedef intra eas SILENTUR. */
interior b32
_intra_ambiguum (constans SilvaNodus* nodus)
{
    i32 custos = ZEPHYRUM;

    dum (nodus != NIHIL && custos < CXXVIII)
    {
        si (nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS)
        {
            redde VERUM;
        }
        nodus = nodus->pater;
        custos++;
    }
    redde FALSUM;
}

/* Vexilla qualium typi colligere (involucra multa iunguntur) */
interior i32
_quales_typi (TypusC89* typus)
{
    i32 quales = ZEPHYRUM;

    dum (typus != NIHIL && typus->genus == TYPUS_C89_QUALIFICATUS)
    {
        quales |= typus->datum.qualificatus.quales;
        typus = typus->datum.qualificatus.internum;
    }
    redde quales;
}

/* Promotio integralis C89 (LP64: int 32-bit OMNES valores char/
 * brevis capit, etiam insignatos -> int, numquam unsigned int).
 * Enumeratus -> int. Cetera immota (quales exuti). */
interior TypusC89*
_promotum (SilvaSemantica* sem, TypusC89* typus)
{
    TypusC89* t = _qualibus_exutum(typus);

    si (t == NIHIL)
    {
        redde sem->typus_erroris;
    }
    si (t->genus == TYPUS_C89_ENUMERATUS)
    {
        redde sem->primitivi[PRIMITIVUM_INTEGER];
    }
    si (t->genus == TYPUS_C89_PRIMITIVUS)
    {
        commutatio (t->datum.primitivum)
        {
            casus PRIMITIVUM_CHARACTER:
            casus PRIMITIVUM_CHARACTER_SIGNATUM:
            casus PRIMITIVUM_CHARACTER_INSIGNATUM:
            casus PRIMITIVUM_BREVIS:
            casus PRIMITIVUM_BREVIS_INSIGNATUM:
                redde sem->primitivi[PRIMITIVUM_INTEGER];
            ordinarius:
                frange;
        }
    }
    redde t;
}

/* Lapsus (decay): acies -> monstrator elementi; functio ->
 * monstrator functionis. NIHIL = nullus lapsus applicandus. */
interior TypusC89*
_lapsus (SilvaSemantica* sem, TypusC89* typus)
{
    TypusC89* t = _qualibus_exutum(typus);

    si (t == NIHIL)
    {
        redde NIHIL;
    }
    si (t->genus == TYPUS_C89_ACIES)
    {
        redde silva_c89_typus_monstrator(sem,
            t->datum.acies.elementum);
    }
    si (t->genus == TYPUS_C89_FUNCTIO)
    {
        redde silva_c89_typus_monstrator(sem, t);
    }
    redde NIHIL;
}

interior b32
_est_arithmeticum (TypusC89* typus)
{
    TypusC89* t = _qualibus_exutum(typus);

    si (t == NIHIL)
    {
        redde FALSUM;
    }
    si (t->genus == TYPUS_C89_ENUMERATUS)
    {
        redde VERUM;
    }
    redde t->genus == TYPUS_C89_PRIMITIVUS
        && t->datum.primitivum != (s32)PRIMITIVUM_VACUUM;
}

interior b32
_est_integrale (TypusC89* typus)
{
    TypusC89* t = _qualibus_exutum(typus);

    si (t == NIHIL)
    {
        redde FALSUM;
    }
    si (t->genus == TYPUS_C89_ENUMERATUS)
    {
        redde VERUM;
    }
    si (t->genus != TYPUS_C89_PRIMITIVUS)
    {
        redde FALSUM;
    }
    commutatio (t->datum.primitivum)
    {
        casus PRIMITIVUM_VACUUM:
        casus PRIMITIVUM_FLUITANS:
        casus PRIMITIVUM_DUPLEX:
        casus PRIMITIVUM_DUPLEX_LONGUS:
            redde FALSUM;
        ordinarius:
            frange;
    }
    redde VERUM;
}

/* ==================================================
 * Conversiones arithmeticae usuales (M0b Chunk B) - ordines
 * C99 pro extensione longus longus (INTENTIO DECISUS 4; C89
 * eadem logica pro subiecto suo). LP64: int 4, longus 8, ll 8.
 * ================================================== */

interior s32
_ordo_integralis (s32 p)
{
    commutatio (p)
    {
        casus PRIMITIVUM_CHARACTER:
        casus PRIMITIVUM_CHARACTER_SIGNATUM:
        casus PRIMITIVUM_CHARACTER_INSIGNATUM:
            redde (s32)I;
        casus PRIMITIVUM_BREVIS:
        casus PRIMITIVUM_BREVIS_INSIGNATUM:
            redde (s32)II;
        casus PRIMITIVUM_INTEGER:
        casus PRIMITIVUM_INTEGER_INSIGNATUM:
            redde (s32)III;
        casus PRIMITIVUM_LONGUS:
        casus PRIMITIVUM_LONGUS_INSIGNATUM:
            redde (s32)IV;
        casus PRIMITIVUM_LONGUS_LONGUS:
        casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
            redde (s32)V;
        ordinarius:
            redde ZEPHYRUM;
    }
}

interior b32
_est_insignatum_primitivum (s32 p)
{
    commutatio (p)
    {
        casus PRIMITIVUM_CHARACTER_INSIGNATUM:
        casus PRIMITIVUM_BREVIS_INSIGNATUM:
        casus PRIMITIVUM_INTEGER_INSIGNATUM:
        casus PRIMITIVUM_LONGUS_INSIGNATUM:
        casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

interior s32
_par_insignatum (s32 p)
{
    commutatio (p)
    {
        casus PRIMITIVUM_INTEGER: redde (s32)PRIMITIVUM_INTEGER_INSIGNATUM;
        casus PRIMITIVUM_LONGUS:  redde (s32)PRIMITIVUM_LONGUS_INSIGNATUM;
        casus PRIMITIVUM_LONGUS_LONGUS:
            redde (s32)PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM;
        ordinarius: redde p;
    }
}

/* Typus communis duorum operandorum arithmeticorum (UAC).
 * Praesupponit ambo arithmetica (vocator probat); quales exuti,
 * enumerati -> int per _promotum. */
interior TypusC89*
_commune_arithmeticum (SilvaSemantica* sem, TypusC89* a, TypusC89* b)
{
    s32 pa;
    s32 pb;
    s32 ra;
    s32 rb;
    b32 ia;
    b32 ib;

    a = _qualibus_exutum(a);
    b = _qualibus_exutum(b);
    pa = (a->genus == TYPUS_C89_ENUMERATUS)
        ? (s32)PRIMITIVUM_INTEGER : a->datum.primitivum;
    pb = (b->genus == TYPUS_C89_ENUMERATUS)
        ? (s32)PRIMITIVUM_INTEGER : b->datum.primitivum;

    /* scala fluitans */
    si (pa == (s32)PRIMITIVUM_DUPLEX_LONGUS
        || pb == (s32)PRIMITIVUM_DUPLEX_LONGUS)
    {
        redde sem->primitivi[PRIMITIVUM_DUPLEX_LONGUS];
    }
    si (pa == (s32)PRIMITIVUM_DUPLEX || pb == (s32)PRIMITIVUM_DUPLEX)
    {
        redde sem->primitivi[PRIMITIVUM_DUPLEX];
    }
    si (pa == (s32)PRIMITIVUM_FLUITANS
        || pb == (s32)PRIMITIVUM_FLUITANS)
    {
        redde sem->primitivi[PRIMITIVUM_FLUITANS];
    }

    /* promotiones integrales */
    a = _promotum(sem, a);
    b = _promotum(sem, b);
    pa = a->datum.primitivum;
    pb = b->datum.primitivum;
    si (pa == pb)
    {
        redde a;
    }
    ra = _ordo_integralis(pa);
    rb = _ordo_integralis(pb);
    ia = _est_insignatum_primitivum(pa);
    ib = _est_insignatum_primitivum(pb);
    si (ia == ib)
    {
        redde (ra >= rb) ? a : b;
    }
    /* signa mixta: insignatum ordinis >= vincit; signatum ordinis
     * maioris vincit SI omnes valores capit (LP64: magnitudo
     * stricte maior); alioquin par insignatum signati */
    {
        TypusC89* signatum   = ia ? b : a;
        TypusC89* insignatum = ia ? a : b;
        s32 r_signati    = ia ? rb : ra;
        s32 r_insignati  = ia ? ra : rb;

        si (r_insignati >= r_signati)
        {
            redde insignatum;
        }
        si (signatum->magnitudo_octetorum
            > insignatum->magnitudo_octetorum)
        {
            redde signatum;
        }
        redde sem->primitivi[_par_insignatum(
            signatum->datum.primitivum)];
    }
}

/* ==================================================
 * Conversio signi (gradus DOMESTICUM, 2026-07-16)
 *
 * Murus i32-insignatus = decipula domus frequentissima (~15
 * firings historica; tres in arcu QB solo POST verdictum ACCIPE).
 * Iudicium in INFUNDIBULO _conversionem_annotare sedet - omnis
 * conversio registrata per id fluit (~35 sedes: UAC ambo operanda,
 * motus, ternarius, familia _ad_finem, cast) - trias {nodus,
 * naturalis, finis} praesens, positio gratis (parsura_currens
 * posita), ordo diagnosticorum deterministicus (passus
 * post-tabulam non esset).
 *
 * Flagrat: ambo integralia, signum diversum, valores NON servati
 * (signatum -> insignatum semper [negativa pereunt]; insignatum ->
 * signatum nisi finis maior [dimidium summum perit]). u8 -> s32
 * valores servat - tacet (promotiones numquam flagrant per
 * constructionem).
 *
 * Suppressiones ordine: (1) CAST EXPLICITA - pater conversio
 * (scriptor conversionem dixit); (2) CONTEXTUS COMPARATIONIS -
 * pater binarium comparans (-Wsign-compare = phasis II nominata);
 * (3) CONSTANS CAPIT - aestimator valorem dat et valor fini cadit
 * (i32 n = 0 tacet; i32 x = -I FLAGRAT - decipula ipsa); probe
 * SILENS: diagnostica aestimatoris (formam_computare sub
 * magnitudine-typi) truncantur; (4) PER-INTERVALLUM (x & 0xff ->
 * i8, %%, >>) NON AEDIFICATA - trahenda ex contactu corporis solo.
 * In systemate numquam (regula domus codicem usoris iudicat).
 * ================================================== */

/* primitivum canonicum integrale (quales exuti); -I = non
 * integrale AUT enumeratus. LIMES NOMINATUS: enumeratus in v1 NON
 * iudicatur - typus enumeri C89 impl-definitus est et clang enums
 * non-negativos INSIGNATOS tractat (calibratio 2026-07-16: u = e
 * apud clang tacet, exemplar nostrum int-signatum flagraret =
 * falsum positivum contra oraculum). */
interior s32
_primitivum_integrale (TypusC89* typus)
{
    TypusC89* t = _qualibus_exutum(typus);

    si (t == NIHIL || !_est_integrale(t)
        || t->genus == TYPUS_C89_ENUMERATUS)
    {
        redde -I;
    }
    redde t->datum.primitivum;
}

/* Intervallum expressionis (exemplar IntRange clang, calibratio
 * 2026-07-16 contra corpus + oraculum vivum): {latitudo bitorum
 * significantium, non_negativum} structuraliter computatum -
 * constantes exacte, ternarius = unio bracchiorum, comparationes/
 * logica = {1,VERUM}, operatores bitorum et arithmetici per
 * latitudines, cetera =
 * intervallum TYPI naturalis (tabula typationum M0b). Clang eadem
 * analysi (IntRange, SemaChecking) supprimit - sine ea corpus 53
 * falsa positiva dedit (x = cond ? I : ZEPHYRUM etc.). */
nomen structura {
    i32 latitudo;        /* bits significantes */
    b32 non_negativum;   /* heuristica clang (paritas oraculi) */
    b32 non_negativum_severum;  /* analysis SANA (gradus severi,
                                 * 2026-07-17): eadem formula per
                                 * operatorem, PRAETER minus =
                                 * numquam (5-10<0). Optimismus
                                 * motus sinistri (1<<n) AMBOBUS
                                 * communis in v1 - limes nominatus.
                                 * Sedes ubi heuristica tacet sed
                                 * sana probare nequit -> codex
                                 * SEVERA (TOLERA-suppressibilis). */
} ExamenIntervallum;

interior i32
_latitudo_valoris (s64 valor)
{
    i32 lat = ZEPHYRUM;

    dum (valor > ZEPHYRUM)
    {
        lat++;
        valor >>= I;
    }
    redde lat;
}

/* probe constantis SILENS: strepitus aestimatoris (diagnostica
 * formae sub magnitudine-typi) truncatur - probe numquam vestigium
 * relinquit */
interior b32
_constans_probare (SilvaSemantica* sem, constans SilvaNodus* nodus,
    s64* valor_out)
{
    i32 ante = xar_numerus(sem->diagnostica);
    b32 constans_est = silva_c89_constans_aestimare(sem, nodus,
        valor_out);

    si (xar_numerus(sem->diagnostica) > ante)
    {
        xar_truncare(sem->diagnostica, ante);
    }
    redde constans_est;
}

/* intervallum ex typo naturali nodi (cadendum ignotum = {64, non}) */
interior ExamenIntervallum
_intervallum_typi (SilvaSemantica* sem, constans SilvaNodus* nodus)
{
    ExamenIntervallum iv;
    SemanticaTypatio* t = _typationem_invenire(sem, nodus);
    s32 p = (t != NIHIL) ? _primitivum_integrale(t->naturalis) : -I;

    si (p < ZEPHYRUM)
    {
        iv.latitudo = LXIV;
        iv.non_negativum = FALSUM;   /* ignotum - numquam supprime */
        iv.non_negativum_severum = FALSUM;
        redde iv;
    }
    iv.latitudo = (i32)(sem->primitivi[p]->magnitudo_octetorum
        * VIII);
    iv.non_negativum = _est_insignatum_primitivum(p);
    iv.non_negativum_severum = iv.non_negativum;  /* typus = sanum */
    redde iv;
}

interior ExamenIntervallum
_intervallum_expressionis (SilvaSemantica* sem,
    constans SilvaNodus* nodus, i32 profunditas)
{
    ExamenIntervallum iv;
    s64 valor = ZEPHYRUM;

    iv.latitudo = LXIV;
    iv.non_negativum = FALSUM;
    iv.non_negativum_severum = FALSUM;
    si (nodus == NIHIL || profunditas >= VIII)
    {
        redde iv;
    }
    nodus = _canonicum(nodus);

    /* constans exacta primum (probe silens) */
    si (_constans_probare(sem, nodus, &valor))
    {
        si (valor >= ZEPHYRUM)
        {
            iv.latitudo = _latitudo_valoris(valor);
            iv.non_negativum = VERUM;
            iv.non_negativum_severum = VERUM;  /* exacta */
        }
        redde iv;
    }

    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_PARENTHESIS:
        {
            SilvaValor v = silva_c89_parenthesis_internum(nodus);

            si (v.genus == SILVA_VALOR_NODUS)
            {
                redde _intervallum_expressionis(sem, v.datum.nodus,
                    profunditas + I);
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_TERNARIUS:
        {
            SilvaValor a_v = silva_c89_ternarius_verum(nodus);
            SilvaValor b_v = silva_c89_ternarius_falsum(nodus);

            si (a_v.genus == SILVA_VALOR_NODUS
                && b_v.genus == SILVA_VALOR_NODUS)
            {
                ExamenIntervallum a = _intervallum_expressionis(sem,
                    a_v.datum.nodus, profunditas + I);
                ExamenIntervallum b = _intervallum_expressionis(sem,
                    b_v.datum.nodus, profunditas + I);

                iv.latitudo = (a.latitudo > b.latitudo)
                    ? a.latitudo : b.latitudo;
                iv.non_negativum = a.non_negativum
                    && b.non_negativum;
                iv.non_negativum_severum = a.non_negativum_severum
                    && b.non_negativum_severum;
                redde iv;
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_UNARIUM:
        {
            SilvaValor op_v = silva_c89_unarium_tok_operator(nodus);
            SilvaValor in_v = silva_c89_unarium_internum(nodus);

            si (op_v.genus == SILVA_VALOR_TOKEN
                && in_v.genus == SILVA_VALOR_NODUS)
            {
                commutatio ((s32)op_v.datum.token->genus)
                {
                    casus SILVA_LEX_EXCLAMATIO:
                        iv.latitudo = I;
                        iv.non_negativum = VERUM;
                        iv.non_negativum_severum = VERUM;
                        redde iv;
                    casus SILVA_LEX_PLUS:
                        redde _intervallum_expressionis(sem,
                            in_v.datum.nodus, profunditas + I);
                    ordinarius:
                        frange;   /* -, ~, *, & -> typus infra */
                }
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_BINARIUM:
        {
            SilvaValor op_v = silva_c89_binarium_tok_operator(nodus);
            SilvaValor s_v = silva_c89_binarium_sinister(nodus);
            SilvaValor d_v = silva_c89_binarium_dexter(nodus);
            s32 op;
            ExamenIntervallum s;
            ExamenIntervallum d;

            si (op_v.genus != SILVA_VALOR_TOKEN
                || s_v.genus != SILVA_VALOR_NODUS
                || d_v.genus != SILVA_VALOR_NODUS)
            {
                frange;
            }
            op = (s32)op_v.datum.token->genus;
            si (op == SILVA_LEX_MINOR || op == SILVA_LEX_MAIOR
                || op == SILVA_LEX_MINOR_AEQUALIS
                || op == SILVA_LEX_MAIOR_AEQUALIS
                || op == SILVA_LEX_AEQUALIS_AEQUALIS
                || op == SILVA_LEX_NON_AEQUALIS
                || op == SILVA_LEX_ET_ET || op == SILVA_LEX_VEL_VEL)
            {
                iv.latitudo = I;
                iv.non_negativum = VERUM;
                iv.non_negativum_severum = VERUM;
                redde iv;
            }
            s = _intervallum_expressionis(sem, s_v.datum.nodus,
                profunditas + I);
            d = _intervallum_expressionis(sem, d_v.datum.nodus,
                profunditas + I);
            commutatio (op)
            {
                casus SILVA_LEX_AMPERSAND:
                    /* nonneg si UTRUM (bit signi purgatus - sanum) */
                    si (s.non_negativum && d.non_negativum)
                    {
                        iv.latitudo = (s.latitudo < d.latitudo)
                            ? s.latitudo : d.latitudo;
                        iv.non_negativum = VERUM;
                    }
                    alioquin si (s.non_negativum)
                    {
                        iv = s;
                    }
                    alioquin si (d.non_negativum)
                    {
                        iv = d;
                    }
                    iv.non_negativum_severum =
                        s.non_negativum_severum
                        || d.non_negativum_severum;
                    redde iv;
                casus SILVA_LEX_BARRA:
                casus SILVA_LEX_CARET:
                    iv.latitudo = (s.latitudo > d.latitudo)
                        ? s.latitudo : d.latitudo;
                    iv.non_negativum = s.non_negativum
                        && d.non_negativum;
                    iv.non_negativum_severum =
                        s.non_negativum_severum
                        && d.non_negativum_severum;
                    redde iv;
                casus SILVA_LEX_PERCENTUM:
                    /* |fructus| < |dexter| */
                    iv.latitudo = d.latitudo;
                    iv.non_negativum = s.non_negativum;
                    iv.non_negativum_severum =
                        s.non_negativum_severum;
                    redde iv;
                casus SILVA_LEX_SOLIDUS:
                    iv.latitudo = s.latitudo;
                    iv.non_negativum = s.non_negativum
                        && d.non_negativum;
                    iv.non_negativum_severum =
                        s.non_negativum_severum
                        && d.non_negativum_severum;
                    redde iv;
                casus SILVA_LEX_DEXTRORSUM:
                    redde s;
                casus SILVA_LEX_SINISTRORSUM:
                {
                    s64 motus = ZEPHYRUM;
                    b32 motus_constans = _constans_probare(sem,
                        d_v.datum.nodus, &motus);

                    si (motus_constans && motus >= ZEPHYRUM
                        && motus < LXIV)
                    {
                        iv.latitudo = s.latitudo + (i32)motus;
                    }
                    si (iv.latitudo > LXIV)
                    {
                        iv.latitudo = LXIV;
                    }
                    /* nonneg servatur (oraculum: I << n tacet);
                     * optimismus AMBOBUS communis in v1 (1<<31
                     * negativum - bracchium sanum = futurum) */
                    iv.non_negativum = s.non_negativum;
                    iv.non_negativum_severum =
                        s.non_negativum_severum;
                    redde iv;
                }
                casus SILVA_LEX_PLUS:
                casus SILVA_LEX_MINUS:
                    /* heuristica: MINUS eadem regula ac PLUS
                     * (calibratio: clang apud (1<<n)-1, prod-128,
                     * u8-u8 TACET - paritas oraculi lex est).
                     * SEVERUM: minus numquam nonneg (5-10<0) - HAEC
                     * est regula stricta v1 (17 sedes corporis,
                     * mensura 2026-07-17). */
                    iv.latitudo = ((s.latitudo > d.latitudo)
                        ? s.latitudo : d.latitudo) + I;
                    si (iv.latitudo > LXIV)
                    {
                        iv.latitudo = LXIV;
                    }
                    iv.non_negativum = s.non_negativum
                        && d.non_negativum;
                    iv.non_negativum_severum =
                        (op == SILVA_LEX_PLUS)
                        && s.non_negativum_severum
                        && d.non_negativum_severum;
                    redde iv;
                casus SILVA_LEX_STAR:
                    iv.latitudo = s.latitudo + d.latitudo;
                    si (iv.latitudo > LXIV)
                    {
                        iv.latitudo = LXIV;
                    }
                    iv.non_negativum = s.non_negativum
                        && d.non_negativum;
                    iv.non_negativum_severum =
                        s.non_negativum_severum
                        && d.non_negativum_severum;
                    redde iv;
                ordinarius:
                    frange;   /* cetera -> typus infra */
            }
            frange;
        }
        ordinarius:
            frange;
    }
    redde _intervallum_typi(sem, nodus);
}

/* Fons nodi alienus? (radix lexematis -> fons_index -> tabula
 * alienitatis, pigre per parsuram computata - viae fontium ipsae
 * BASENAME sunt [praebere], ergo graphus inclusionum via scripta
 * consulendus, non via fontis; exemplar MACRO_DOMESTICUM) */
interior b32
_fons_alienus (SilvaSemantica* sem, constans SilvaNodus* nodus)
{
    SilvaToken* lexema;
    SilvaToken* radix;

    si (sem->parsura_currens == NIHIL
        || sem->parsura_currens->expansio == NIHIL)
    {
        redde FALSUM;
    }
    si (sem->alieni_parsura != sem->parsura_currens)
    {
        b32* tabula = NIHIL;

        sem->alieni_parsura = sem->parsura_currens;
        sem->fontes_alieni = NIHIL;
        sem->fontes_alieni_numerus = ZEPHYRUM;
        si (_alienitates_computare(sem, sem->parsura_currens,
                &tabula, NIHIL))
        {
            sem->fontes_alieni = tabula;
            sem->fontes_alieni_numerus = xar_numerus(
                sem->parsura_currens->expansio->fontes);
        }
    }
    si (sem->fontes_alieni == NIHIL)
    {
        redde FALSUM;   /* TU sine vendore (fere omnes) */
    }
    lexema = _lexema_primum(nodus);
    si (lexema == NIHIL)
    {
        redde FALSUM;
    }
    radix = silva_token_radix(lexema);
    si (radix == NIHIL || radix->fons_index < ZEPHYRUM
        || radix->fons_index >= (s32)sem->fontes_alieni_numerus)
    {
        redde FALSUM;
    }
    redde sem->fontes_alieni[radix->fons_index];
}

/* ==================================================
 * CHORDA NUDA (codex 59): chorda.datum ad lectorem NUL
 * ================================================== */

interior b32
_chorda_par_literis (chorda s, constans character* litterae)
{
    i32 m = (i32)strlen(litterae);

    redde s.mensura == m
        && memcmp(s.datum, litterae, (memoriae_index)m) == ZEPHYRUM;
}

/* lectores C sine limite: larva positionum argumentorum quae
 * terminationem NUL requirunt (bit a = argumentum a). Lectores
 * mensura-limitati (strncmp/strncpy/memcmp/memcpy) CONSULTO
 * absunt - effugium legitimum sunt, ut chorda_ut_cstr et "%.*s"
 * (familia printf ad gradum formati differtur - charta communis
 * formati->argumenti ibi aedificabitur). */
nomen structura {
    constans character* titulus;
    i32                 larva;
} ExamenLectorNul;

interior constans ExamenLectorNul _lectores_nul[] = {
    { "strlen",  1u }, { "strcmp",  3u }, { "strcpy",  2u },
    { "strcat",  3u }, { "strchr",  1u }, { "strrchr", 1u },
    { "strstr",  3u }, { "strspn",  3u }, { "strcspn", 3u },
    { "strpbrk", 3u }, { "strtok",  3u }, { "atoi",    1u },
    { "atol",    1u }, { "atof",    1u }, { "strtol",  1u },
    { "strtoul", 1u }, { "strtod",  1u }, { "sscanf",  3u },
    { "puts",    1u }, { "fputs",   1u }
};

interior i32
_larva_lectoris_nul (chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM;
         i < (i32)(magnitudo(_lectores_nul)
             / magnitudo(_lectores_nul[0])); i++)
    {
        si (_chorda_par_literis(titulus, _lectores_nul[i].titulus))
        {
            redde _lectores_nul[i].larva;
        }
    }
    redde ZEPHYRUM;
}

/* Estne expressio (per casus/parentheses despecta) accessus
 * .datum/->datum cuius basis typum chordam habet? Typatio LECTA,
 * non retypata - subarbor iam typata est; retypatio annotationes
 * conversionum duplicaret. Typus tag nomine "chorda" agnoscitur
 * (10+ structurae domus agrum 'datum' habent - RegioClicca.datum
 * character[64] terminatum est; nomen agri solum non sufficit). */
interior b32
_est_datum_chordae (SilvaSemantica* sem, constans SilvaNodus* nodus)
{
    dum (VERUM)
    {
        nodus = _canonicum(nodus);
        si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
        {
            SilvaValor v = silva_c89_parenthesis_internum(nodus);

            si (v.genus != SILVA_VALOR_NODUS)
            {
                redde FALSUM;
            }
            nodus = v.datum.nodus;
            perge;
        }
        si (nodus->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
        {
            SilvaValor v = silva_c89_conversio_internum(nodus);

            si (v.genus != SILVA_VALOR_NODUS)
            {
                redde FALSUM;
            }
            nodus = v.datum.nodus;
            perge;
        }
        frange;
    }
    si (nodus->genus != (s32)SILVA_C89_GENUS_ACCESSUS)
    {
        redde FALSUM;
    }
    {
        SilvaValor tit_v = silva_c89_accessus_tok_titulus(nodus);
        SilvaValor op_v = silva_c89_accessus_tok_operator(nodus);
        SilvaValor b_v = silva_c89_accessus_basis(nodus);
        SemanticaTypatio* typatio;
        TypusC89* tb;
        TypusC89* tag;

        si (tit_v.genus != SILVA_VALOR_TOKEN
            || op_v.genus != SILVA_VALOR_TOKEN
            || b_v.genus != SILVA_VALOR_NODUS)
        {
            redde FALSUM;
        }
        si (!_chorda_par_literis(tit_v.datum.token->valor, "datum"))
        {
            redde FALSUM;
        }
        typatio = _typationem_invenire(sem,
            _canonicum(b_v.datum.nodus));
        si (typatio == NIHIL || typatio->naturalis == NIHIL)
        {
            redde FALSUM;
        }
        tb = typatio->naturalis;
        si (op_v.datum.token->genus == SILVA_LEX_SAGITTA)
        {
            TypusC89* e = _qualibus_exutum(tb);

            si (e->genus != TYPUS_C89_MONSTRATOR)
            {
                redde FALSUM;
            }
            tb = e->datum.monstrator.internum;
        }
        tag = _qualibus_exutum(tb);
        redde tag->genus == TYPUS_C89_STRUCTURA
            && _chorda_par_literis(tag->datum.tag.titulus, "chorda");
    }
}

/* Vigil codicis 62 (DORMIT usque ad CFG gradum 2 - vide sedem in
 * _comparationem_examinare) */
interior constans b32 _degradata_vigil = FALSUM;

/* Codex 61: sentinella negativa reddita e functione insignata -
 * morbus tcp/tls/biblia IN ORTU. Forma nuda (redde -1) iam paritas
 * 54 est; CASUS ((i32)-1) et clang et paritatem silet - hic
 * capitur (aestimator per casus plicat). VANA consumptores mortuos
 * capit; hic PRODUCTOREM etiam antequam consumptor ullus probet. */
interior vacuum
_sentinellam_examinare (SilvaSemantica* sem,
    constans SilvaNodus* val)
{
    TypusC89* rc = sem->reditus_currens;
    TypusC89* e;
    s32 p;
    s64 valor = ZEPHYRUM;

    si (rc == NIHIL || sem->in_systemate)
    {
        redde;
    }
    e = _qualibus_exutum(rc);
    si (e == NIHIL || e->genus == TYPUS_C89_ENUMERATUS)
    {
        redde;
    }
    p = _primitivum_integrale(rc);
    si (p < ZEPHYRUM || !_est_insignatum_primitivum(p))
    {
        redde;
    }
    /* per casus/parentheses despicere - aestimator casus NON
     * plicat (probatum empirice), et forma NUDA (redde -1) iam
     * paritas 54 est: solum forma CASU VESTITA nostra est (ea quae
     * clang et paritatem silet) */
    {
        b32 casus_visus = FALSUM;

        dum (VERUM)
        {
            val = _canonicum(val);
            si (val->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
            {
                SilvaValor v = silva_c89_parenthesis_internum(val);

                si (v.genus != SILVA_VALOR_NODUS)
                {
                    redde;
                }
                val = v.datum.nodus;
                perge;
            }
            si (val->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
            {
                SilvaValor v = silva_c89_conversio_internum(val);

                si (v.genus != SILVA_VALOR_NODUS)
                {
                    redde;
                }
                casus_visus = VERUM;
                val = v.datum.nodus;
                perge;
            }
            frange;
        }
        si (!casus_visus)
        {
            redde;   /* nuda = paritas 54, non nostra */
        }
    }
    si (!_constans_probare(sem, val, &valor) || valor >= ZEPHYRUM)
    {
        redde;
    }
    si (_fons_alienus(sem, val)
        || _tolera_absorbere(sem, val,
               (s32)EXAMEN_CODEX_SENTINELLA_INSIGNATA))
    {
        redde;
    }
    _diagnosticum_addere_plenum(sem, val,
        (s32)EXAMEN_CODEX_SENTINELLA_INSIGNATA, NIHIL, NIHIL);
}

/* ==================================================
 * SIGNUM FORMATI (codex 60): charta formati -> argumenta
 * ================================================== */

/* classes exspectatae argumentorum variadicorum */
#define FORMATUM_NIHIL          0   /* non iudicandum */
#define FORMATUM_SIGNATUM       1   /* %d %i */
#define FORMATUM_INSIGNATUM     2   /* %u */
#define FORMATUM_CHORDA_LIBERA  3   /* %s sine praecisione -> 59 */

#define FORMATI_ARGUMENTA_MAXIMA 32

/* familia printf: positio argumenti formati (scanf-familia et
 * variantes va_list EXCLUSAE v1 - directio alia / argumenta
 * invisibilia) */
nomen structura {
    constans character* titulus;
    s32                 positio_formati;
} ExamenFormator;

interior constans ExamenFormator _formatores[] = {
    { "printf", 0 }, { "fprintf", 1 }, { "sprintf", 1 }
};

interior s32
_positio_formatoris (chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM;
         i < (i32)(magnitudo(_formatores)
             / magnitudo(_formatores[0])); i++)
    {
        si (_chorda_par_literis(titulus, _formatores[i].titulus))
        {
            redde _formatores[i].positio_formati;
        }
    }
    redde -I;
}

/* Litteralem formati in octetos decodatos vertere (fragmenta
 * adiacentia + fugae - etiam \045 pro '%' recte). Redde mensuram
 * aut -I (non analysabile: latus, fuga invalida, nimis longus). */
interior s32
_formati_decodere (constans SilvaNodus* nodus, character* effusum,
    i32 capacitas)
{
    SilvaValor fragmenta = silva_c89_folium_chorda_tok_valor(nodus);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(fragmenta);
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(fragmenta, i);
        constans SilvaToken* tok;
        i32 cursor;
        i32 finis;

        si (v == NIHIL || v->genus != SILVA_VALOR_TOKEN)
        {
            perge;
        }
        tok = v->datum.token;
        si (tok->valor.mensura < II
            || tok->valor.datum[ZEPHYRUM] == 'L')
        {
            redde -I;
        }
        cursor = I;
        finis = tok->valor.mensura - I;   /* " claudens */
        dum (cursor < finis)
        {
            character octetus;

            si ((character)tok->valor.datum[cursor] == '\\')
            {
                s64 valor_fugae = ZEPHYRUM;

                cursor++;
                si (!_fugam_decodere(&tok->valor, &cursor,
                        &valor_fugae))
                {
                    redde -I;
                }
                octetus = (character)valor_fugae;
            }
            alioquin
            {
                octetus = (character)tok->valor.datum[cursor];
                cursor++;
            }
            si (n >= capacitas)
            {
                redde -I;
            }
            effusum[n] = octetus;
            n++;
        }
    }
    redde (s32)n;
}

/* Chartam formati aedificare: conversiones -> classes exspectatas
 * (exspectata) + characteres conversionum (conversiones, pro
 * nuntiis). Redde numerum argumentorum consumptorum aut -I si non
 * analysabile (conversio ignota => NIHIL iudicatur - tutum). */
interior s32
_formati_exspectata (constans SilvaNodus* nodus, s32* exspectata,
    character* conversiones)
{
    character litterae[DXII];
    s32 m = _formati_decodere(nodus, litterae, DXII);
    s32 i = ZEPHYRUM;
    s32 n = ZEPHYRUM;

    si (m < ZEPHYRUM)
    {
        redde -I;
    }
    dum (i < m)
    {
        character c = litterae[i];
        b32 praecisio_data;

        i++;
        si (c != '%')
        {
            perge;
        }
        si (i >= m)
        {
            redde -I;
        }
        si (litterae[i] == '%')
        {
            i++;
            perge;
        }
        /* vexilla */
        dum (i < m && (litterae[i] == '-' || litterae[i] == '+'
            || litterae[i] == ' ' || litterae[i] == '#'
            || litterae[i] == '0'))
        {
            i++;
        }
        /* latitudo (stellata argumentum consumit, NON iudicatur -
         * "%.*s" cum mensura idioma domus est) */
        si (i < m && litterae[i] == '*')
        {
            si (n >= FORMATI_ARGUMENTA_MAXIMA)
            {
                redde -I;
            }
            exspectata[n] = FORMATUM_NIHIL;
            conversiones[n] = '*';
            n++;
            i++;
        }
        alioquin
        {
            dum (i < m && litterae[i] >= '0' && litterae[i] <= '9')
            {
                i++;
            }
        }
        /* praecisio */
        praecisio_data = FALSUM;
        si (i < m && litterae[i] == '.')
        {
            i++;
            praecisio_data = VERUM;
            si (i < m && litterae[i] == '*')
            {
                si (n >= FORMATI_ARGUMENTA_MAXIMA)
                {
                    redde -I;
                }
                exspectata[n] = FORMATUM_NIHIL;
                conversiones[n] = '*';
                n++;
                i++;
            }
            alioquin
            {
                dum (i < m && litterae[i] >= '0'
                    && litterae[i] <= '9')
                {
                    i++;
                }
            }
        }
        /* modificatores longitudinis (signum non mutant) */
        dum (i < m && (litterae[i] == 'h' || litterae[i] == 'l'
            || litterae[i] == 'L'))
        {
            i++;
        }
        si (i >= m || n >= FORMATI_ARGUMENTA_MAXIMA)
        {
            redde -I;
        }
        c = litterae[i];
        i++;
        conversiones[n] = c;
        commutatio (c)
        {
            casus 'd':
            casus 'i':
                exspectata[n] = FORMATUM_SIGNATUM;
                frange;
            casus 'u':
                exspectata[n] = FORMATUM_INSIGNATUM;
                frange;
            casus 'x':
            casus 'X':
            casus 'o':   /* idioma repraesentationis - exempta */
            casus 'c':
            casus 'p':
            casus 'n':
            casus 'f':
            casus 'e':
            casus 'E':
            casus 'g':
            casus 'G':
                exspectata[n] = FORMATUM_NIHIL;
                frange;
            casus 's':
                /* praecisio lectionem limitat: %.Ns/%.*s tutum */
                exspectata[n] = praecisio_data
                    ? FORMATUM_NIHIL : FORMATUM_CHORDA_LIBERA;
                frange;
            ordinarius:
                redde -I;   /* conversio ignota - nihil iudica */
        }
        n++;
    }
    redde n;
}

/* Argumentum variadicum contra classem exspectatam iudicare.
 * Exemptiones (utraque directione): constantes non negativae,
 * intervalla non negativa (u8 promotum, larvae), enumerati. */
interior vacuum
_formatum_iudicare (SilvaSemantica* sem, constans SilvaNodus* nodus,
    TypusC89* typus, s32 exspectatum, character conversio)
{
    TypusC89* promotus;
    s32 p;
    b32 insignatum;
    s64 valor = ZEPHYRUM;

    si (exspectatum == FORMATUM_CHORDA_LIBERA)
    {
        si (_est_datum_chordae(sem, nodus)
            && !_tolera_absorbere(sem, nodus,
                   (s32)EXAMEN_CODEX_CHORDA_NUDA))
        {
            _diagnosticum_addere_plenum(sem, nodus,
                (s32)EXAMEN_CODEX_CHORDA_NUDA, NIHIL,
                "datum chordae sine fine NUL ad conversionem"
                " '%s' (utere \"%.*s\" cum mensura)");
        }
        redde;
    }
    si (exspectatum == FORMATUM_NIHIL || typus == NIHIL)
    {
        redde;
    }
    si (_qualibus_exutum(typus)->genus == TYPUS_C89_ENUMERATUS)
    {
        redde;   /* enumerati non iudicantur (ut ceteri gradus) */
    }
    promotus = _promotum(sem, typus);
    p = _primitivum_integrale(promotus);
    si (p < ZEPHYRUM)
    {
        redde;   /* non integrale - magnitudo res clang est */
    }
    insignatum = _est_insignatum_primitivum(p);
    si ((exspectatum == FORMATUM_SIGNATUM && !insignatum)
        || (exspectatum == FORMATUM_INSIGNATUM && insignatum))
    {
        redde;   /* congruit */
    }
    /* DECRETUM (Fran 2026-07-17): directio %d-cum-insignato NON
     * iudicatur - i32 in %d supra 2^31 solum erratum (numquam
     * practice: census primus 443 sedes, omnes hac directione,
     * nullae altera), et involutio i32 ut numerus NEGATIVUS
     * visibilis fit (%u eam ut 4e9 celaret) - habitus domus per
     * accidens diagnosticus. Sola directio %u-cum-signato flagrat:
     * -1 ut 4294967295 = effusum vere fallax. */
    si (exspectatum == FORMATUM_SIGNATUM)
    {
        redde;
    }
    /* argumentum signatum in conversione insignata: non negativum
     * idem imprimit - exemptum */
    si (_constans_probare(sem, nodus, &valor) && valor >= ZEPHYRUM)
    {
        redde;
    }
    {
        ExamenIntervallum iv = _intervallum_expressionis(sem, nodus,
            ZEPHYRUM);

        si (iv.non_negativum)
        {
            redde;
        }
    }
    si (_tolera_absorbere(sem, nodus,
            (s32)EXAMEN_CODEX_SIGNUM_FORMATI))
    {
        redde;
    }
    {
        character* nuntius = (character*)piscina_allocare(
            sem->piscina, (memoriae_index)LXXX);

        si (nuntius != NIHIL)
        {
            sprintf(nuntius, "signum formati discrepat:"
                " conversio '%%%c' cum argumento %s",
                conversio, insignatum ? "insignato" : "signato");
            _diagnosticum_addere_plenum(sem, nodus,
                (s32)EXAMEN_CODEX_SIGNUM_FORMATI, NIHIL, nuntius);
        }
    }
}

interior vacuum
_conversionem_signi_examinare (SilvaSemantica* sem,
    constans SilvaNodus* nodus, TypusC89* naturalis, TypusC89* finis)
{
    s32 p_naturalis;
    s32 p_finis;
    b32 i_naturalis;
    b32 i_finis;
    constans SilvaNodus* pater;
    s32 codex_ignis = (s32)EXAMEN_CODEX_CONVERSIO_SIGNI;

    si (sem->in_systemate)
    {
        redde;
    }
    p_naturalis = _primitivum_integrale(naturalis);
    p_finis = _primitivum_integrale(finis);
    si (p_naturalis < ZEPHYRUM || p_finis < ZEPHYRUM)
    {
        redde;   /* non integrale <-> integrale */
    }
    i_naturalis = _est_insignatum_primitivum(p_naturalis);
    i_finis = _est_insignatum_primitivum(p_finis);
    si (i_naturalis == i_finis)
    {
        redde;
    }
    pater = nodus->pater;
    si (pater != NIHIL)
    {
        si (pater->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
        {
            redde;   /* cast explicita */
        }
        si (pater->genus == (s32)SILVA_C89_GENUS_BINARIUM)
        {
            SilvaValor op_v = silva_c89_binarium_tok_operator(pater);

            si (op_v.genus == SILVA_VALOR_TOKEN)
            {
                commutatio ((s32)op_v.datum.token->genus)
                {
                    casus SILVA_LEX_MINOR:
                    casus SILVA_LEX_MAIOR:
                    casus SILVA_LEX_MINOR_AEQUALIS:
                    casus SILVA_LEX_MAIOR_AEQUALIS:
                    casus SILVA_LEX_AEQUALIS_AEQUALIS:
                    casus SILVA_LEX_NON_AEQUALIS:
                        redde;   /* -> COMPARATIO_SIGNORUM/VANA ad
                                  * nodum comparationis (phasis II
                                  * impleta 2026-07-17) */
                    ordinarius:
                        frange;
                }
            }
        }
        si (pater->genus == (s32)SILVA_C89_GENUS_ASSIGNATIO)
        {
            /* latus SINISTRUM assignationis operatae (s += u):
             * clang conversionem computationis tacite sinit (RHS
             * flagrat, f12; LHS non, calibratio lint_probatio) -
             * paritas oraculi */
            SilvaValor op_v = silva_c89_assignatio_tok_operator(
                pater);
            SilvaValor s_v = silva_c89_assignatio_sinister(pater);

            si (op_v.genus == SILVA_VALOR_TOKEN
                && (s32)op_v.datum.token->genus
                    != SILVA_LEX_ASSIGNATIO
                && s_v.genus == SILVA_VALOR_NODUS
                && _canonicum(s_v.datum.nodus) == nodus)
            {
                redde;
            }
        }
    }
    /* fons alienus (vendor) numquam iudicatur - regula domus
     * codicem DOMESTICUM iudicat (contrarium MACRO_DOMESTICUM:
     * ibi macros nostra in alieno, hic codex alienus ipse; stb
     * corpus 145 ordines dedit ante hunc custodem) */
    si (_fons_alienus(sem, nodus))
    {
        redde;
    }
    /* intervallum (exemplar IntRange): signatum -> insignatum
     * tacet si valor provabiliter non negativus (heuristica clang);
     * si heuristica tacet sed analysis SANA probare nequit ->
     * gradus SEVERUS (TOLERA-suppressibilis). insignatum ->
     * signatum tacet si latitudo intra finem cadit (constantes
     * exacte per aestimatorem - i32 n = 0 tacet, i32 x = -I
     * FLAGRAT) */
    {
        ExamenIntervallum iv = _intervallum_expressionis(sem, nodus,
            ZEPHYRUM);

        si (i_finis)
        {
            si (iv.non_negativum)
            {
                si (iv.non_negativum_severum)
                {
                    redde;   /* sane tutum */
                }
                si (_tolera_absorbere(sem, nodus,
                        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA))
                {
                    redde;   /* intentum, causa notata */
                }
                codex_ignis =
                    (s32)EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA;
            }
        }
        alioquin si (iv.latitudo
            < (i32)(sem->primitivi[p_finis]->magnitudo_octetorum
                * VIII))
        {
            redde;
        }
    }
    {
        character textus_naturalis[CXXVIII];
        character textus_finis[CXXVIII];
        insignatus integer m_naturalis = silva_c89_typum_scribere(
            naturalis, textus_naturalis,
            (insignatus integer)magnitudo(textus_naturalis));
        insignatus integer m_finis = silva_c89_typum_scribere(
            finis, textus_finis,
            (insignatus integer)magnitudo(textus_finis));
        constans character* stricta =
            (codex_ignis
                == (s32)EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA)
            ? " (stricta)" : "";

        si (m_naturalis > ZEPHYRUM && m_finis > ZEPHYRUM)
        {
            memoriae_index capacitas = (memoriae_index)m_naturalis
                + (memoriae_index)m_finis + (memoriae_index)LXIV;
            character* nuntius = (character*)piscina_allocare(
                sem->piscina, capacitas);

            si (nuntius != NIHIL)
            {
                sprintf(nuntius, "conversio signi implicita%s:"
                    " %s -> %s", stricta, textus_naturalis,
                    textus_finis);
                _diagnosticum_addere_plenum(sem, nodus, codex_ignis,
                    NIHIL, nuntius);
                redde;
            }
        }
        silva_c89_diagnosticum_addere(sem, nodus, codex_ignis);
    }
}

/* ==================================================
 * Comparationes (phasis II conversionis signi, 2026-07-17)
 *
 * SIGNORUM (paritas -Wsign-compare, calibratio viva 2026-07-17):
 * flagrat cum commune UAC INSIGNATUM est et latus promotione
 * SIGNATUM convertitur (int<u32, int<u64, short<u32, ==/!=
 * inclusa; long<u32 tacet - u32 in s64 valores servat).
 * Suppressiones: constans non-negativa in latere signato (u > 5;
 * constans NEGATIVA flagrat - divergentia consulta: clang '-1 < u'
 * tacet sed 'u < -1' flagrat, inconstans; corpus neutram habet);
 * intervallum heuristicum nonneg (u8 promotum, larvae).
 *
 * VANA (severa, TOLERA-suppressibilis): comparatio contra zephyrum
 * constantem semper eadem - X < 0 / 0 > X (semper falsum), X >= 0
 * / 0 <= X (semper verum), ubi typus communis INSIGNATUS ((u-v) >=
 * 0 = proba subfluxus fracta classica!) AUT X intervallo SANO
 * nonneg (u8 promotum). Vexilla domus hanc classem NUMQUAM tegunt
 * (-Wtautological-* extra -Wextra) et clang tautologica in MACRIS
 * supprimit - ZEPHYRUM omnes zephyros domus dat: sic cohibitio
 * mortua delineare vixit. Aestimator per macra videt (consulto).
 * ================================================== */

/* identificator planus cum titulo pari? (per parenthesin) */
interior b32
_est_identificator_par (constans SilvaNodus* nodus,
    constans chorda* titulus)
{
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    nodus = _canonicum(nodus);
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        si (v.genus != SILVA_VALOR_NODUS)
        {
            redde FALSUM;
        }
        redde _est_identificator_par(v.datum.nodus, titulus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        SilvaValor tok_v =
            silva_c89_folium_identificator_tok_valor(nodus);

        redde tok_v.genus == SILVA_VALOR_TOKEN
            && tok_v.datum.token->valor.mensura == titulus->mensura
            && memcmp(tok_v.datum.token->valor.datum,
                   titulus->datum,
                   (memoriae_index)titulus->mensura) == ZEPHYRUM;
    }
    redde FALSUM;
}

/* subtreum comparationem ORDINANTEM eiusdem identificatoris
 * continet? (per parenthesin et catenas ||/&&) */
interior b32
_continet_ordinationem (constans SilvaNodus* nodus,
    constans chorda* titulus, i32 profunditas)
{
    si (nodus == NIHIL || profunditas >= VI)
    {
        redde FALSUM;
    }
    nodus = _canonicum(nodus);
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        si (v.genus != SILVA_VALOR_NODUS)
        {
            redde FALSUM;
        }
        redde _continet_ordinationem(v.datum.nodus, titulus,
            profunditas + I);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_BINARIUM)
    {
        SilvaValor op_v = silva_c89_binarium_tok_operator(nodus);
        SilvaValor s_v = silva_c89_binarium_sinister(nodus);
        SilvaValor d_v = silva_c89_binarium_dexter(nodus);
        s32 op;

        si (op_v.genus != SILVA_VALOR_TOKEN
            || s_v.genus != SILVA_VALOR_NODUS
            || d_v.genus != SILVA_VALOR_NODUS)
        {
            redde FALSUM;
        }
        op = (s32)op_v.datum.token->genus;
        si (op == SILVA_LEX_MINOR || op == SILVA_LEX_MAIOR
            || op == SILVA_LEX_MINOR_AEQUALIS
            || op == SILVA_LEX_MAIOR_AEQUALIS)
        {
            redde _est_identificator_par(s_v.datum.nodus, titulus)
                || _est_identificator_par(d_v.datum.nodus,
                       titulus);
        }
        si (op == SILVA_LEX_VEL_VEL || op == SILVA_LEX_ET_ET)
        {
            redde _continet_ordinationem(s_v.datum.nodus, titulus,
                       profunditas + I)
                || _continet_ordinationem(d_v.datum.nodus, titulus,
                       profunditas + I);
        }
    }
    redde FALSUM;
}

/* Idiom custodiae bilateralis (DECISUS Fran 2026-07-17): X
 * identificator planus, comparatio vana intra catenam ||/&& cuius
 * frater EUNDEM X contra limitem ordinat - "X < 0 || X >= LIMES".
 * Dimidium mortuum consulto scriptum: involutio a limite altero
 * capta, custodia TENET. Cohibitiones solitariae (classis
 * cohibere - custodia quae NON tenet) semper flagrant, et X
 * compositum (u - v) semper flagrat. */
interior b32
_intra_custodiam_bilateralem (constans SilvaNodus* comparatio,
    constans SilvaNodus* x_nodus)
{
    chorda titulus;
    constans SilvaNodus* cur;
    i32 gradus;

    /* titulus X: identificator planus solum */
    {
        constans SilvaNodus* x = _canonicum(x_nodus);

        dum (x != NIHIL
            && x->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
        {
            SilvaValor v = silva_c89_parenthesis_internum(x);

            si (v.genus != SILVA_VALOR_NODUS)
            {
                redde FALSUM;
            }
            x = _canonicum(v.datum.nodus);
        }
        si (x == NIHIL || x->genus
                != (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
        {
            redde FALSUM;
        }
        {
            SilvaValor tok_v =
                silva_c89_folium_identificator_tok_valor(x);

            si (tok_v.genus != SILVA_VALOR_TOKEN)
            {
                redde FALSUM;
            }
            titulus = tok_v.datum.token->valor;
        }
    }
    cur = comparatio;
    per (gradus = ZEPHYRUM; gradus < VIII; gradus++)
    {
        constans SilvaNodus* p = cur->pater;

        si (p == NIHIL)
        {
            redde FALSUM;
        }
        si (p->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
        {
            cur = p;
            perge;
        }
        si (p->genus == (s32)SILVA_C89_GENUS_BINARIUM)
        {
            SilvaValor op_v = silva_c89_binarium_tok_operator(p);
            SilvaValor s_v = silva_c89_binarium_sinister(p);
            SilvaValor d_v = silva_c89_binarium_dexter(p);
            s32 op;
            constans SilvaNodus* frater = NIHIL;

            si (op_v.genus != SILVA_VALOR_TOKEN)
            {
                redde FALSUM;
            }
            op = (s32)op_v.datum.token->genus;
            si (op != SILVA_LEX_VEL_VEL && op != SILVA_LEX_ET_ET)
            {
                redde FALSUM;
            }
            si (s_v.genus == SILVA_VALOR_NODUS
                && d_v.genus == SILVA_VALOR_NODUS)
            {
                si (s_v.datum.nodus == cur
                    || _canonicum(s_v.datum.nodus) == cur)
                {
                    frater = d_v.datum.nodus;
                }
                alioquin
                {
                    frater = s_v.datum.nodus;
                }
            }
            si (frater != NIHIL
                && _continet_ordinationem(frater, &titulus,
                       ZEPHYRUM))
            {
                redde VERUM;
            }
            cur = p;
            perge;
        }
        redde FALSUM;   /* pater alius - extra catenam custodiae */
    }
    redde FALSUM;
}

interior vacuum
_comparationem_examinare (SilvaSemantica* sem,
    constans SilvaNodus* nodus, constans SilvaNodus* ns,
    constans SilvaNodus* nd, TypusC89* ts, TypusC89* td,
    TypusC89* commune, s32 op)
{
    TypusC89* es;
    TypusC89* ed;
    TypusC89* ec;
    b32 commune_insignatum;
    s64 valor = ZEPHYRUM;

    si (sem->in_systemate)
    {
        redde;
    }
    es = _qualibus_exutum(ts);
    ed = _qualibus_exutum(td);
    ec = _qualibus_exutum(commune);
    si (es == NIHIL || ed == NIHIL || ec == NIHIL
        || es->genus == TYPUS_C89_ENUMERATUS
        || ed->genus == TYPUS_C89_ENUMERATUS)
    {
        redde;   /* enumerati non iudicantur (v1, ut conversio) */
    }
    si (_primitivum_integrale(ts) < ZEPHYRUM
        || _primitivum_integrale(td) < ZEPHYRUM)
    {
        redde;   /* non integralia (fluitantia etc.) */
    }
    si (_fons_alienus(sem, nodus))
    {
        redde;
    }
    commune_insignatum = (ec->genus == TYPUS_C89_PRIMITIVUS)
        && _est_insignatum_primitivum(ec->datum.primitivum);

    /* VANA primum (specificior; SIGNORUM exemptione constantis
     * naturaliter exclusa) */
    {
        constans SilvaNodus* zerus = NIHIL;
        constans SilvaNodus* alter = NIHIL;
        TypusC89* typus_alterius = NIHIL;
        b32 semper_verum = FALSUM;

        commutatio (op)
        {
            casus SILVA_LEX_MINOR:          /* X < 0: falsum */
                zerus = nd; alter = ns; typus_alterius = ts;
                frange;
            casus SILVA_LEX_MAIOR_AEQUALIS: /* X >= 0: verum */
                zerus = nd; alter = ns; typus_alterius = ts;
                semper_verum = VERUM;
                frange;
            casus SILVA_LEX_MAIOR:          /* 0 > X: falsum */
                zerus = ns; alter = nd; typus_alterius = td;
                frange;
            casus SILVA_LEX_MINOR_AEQUALIS: /* 0 <= X: verum */
                zerus = ns; alter = nd; typus_alterius = td;
                semper_verum = VERUM;
                frange;
            ordinarius:
                frange;
        }
        si (zerus != NIHIL && _constans_probare(sem, zerus, &valor)
            && valor == ZEPHYRUM)
        {
            ExamenIntervallum iv = _intervallum_expressionis(sem,
                alter, ZEPHYRUM);

            si (commune_insignatum || iv.non_negativum_severum)
            {
                si (_intra_custodiam_bilateralem(nodus, alter))
                {
                    redde;   /* idiom custodiae (DECISUS) */
                }
                si (!_tolera_absorbere(sem, nodus,
                        (s32)EXAMEN_CODEX_COMPARATIO_VANA))
                {
                    character textus[CXXVIII];
                    insignatus integer m = silva_c89_typum_scribere(
                        typus_alterius, textus,
                        (insignatus integer)magnitudo(textus));

                    si (m > ZEPHYRUM)
                    {
                        memoriae_index capacitas =
                            (memoriae_index)m
                            + (memoriae_index)LXIV;
                        character* nuntius =
                            (character*)piscina_allocare(
                                sem->piscina, capacitas);

                        si (nuntius != NIHIL)
                        {
                            sprintf(nuntius, "comparatio vana:"
                                " semper %s (%s contra zephyrum)",
                                semper_verum ? "verum" : "falsum",
                                textus);
                            _diagnosticum_addere_plenum(sem, nodus,
                                (s32)EXAMEN_CODEX_COMPARATIO_VANA,
                                NIHIL, nuntius);
                            redde;
                        }
                    }
                    silva_c89_diagnosticum_addere(sem, nodus,
                        (s32)EXAMEN_CODEX_COMPARATIO_VANA);
                }
                redde;
            }
        }
    }

    /* DEGRADATA (codex 62): X <= 0 / 0 >= X insignati - non
     * tautologica (== 0 fit: comparatio ALIA, non constans - VANA
     * structuraliter caeca), sed intentio signi probabilis:
     * terminus erroris circuli delineare hanc formam gessit.
     * Positiones zeri quas VANA NON mappat sunt exacte nostrae.
     *
     * DORMIT (decretum Fran 2026-07-17): census primus 228 - forma
     * ab idiomate vacuitatis possessa (mensura <= 0 = "vacuum",
     * ~88 in tabulis generatis, 18 custodiae delineare, omnes
     * recte == 0). Acus (intentio signi arithmetici, forma termini
     * erroris) statice NON separabilis - definitiones operandi
     * requiruntur. RE-ARMATUR ut consumptor analyseos fluxus (CFG
     * gradus 2): flagra solum cum definitiones operandi
     * arithmeticae sint (accumulatio +/-), non assignationes
     * mensurarum. Vigil infra tunc praedicatum verum fit. */
    si (_degradata_vigil)
    {
        constans SilvaNodus* zerus = NIHIL;
        constans SilvaNodus* alter = NIHIL;
        TypusC89* typus_alterius = NIHIL;
        s64 valor_z = ZEPHYRUM;

        si (op == SILVA_LEX_MINOR_AEQUALIS)
        {
            /* X <= 0: zerus DEXTER (VANA sinistrum mappat) */
            zerus = nd;
            alter = ns;
            typus_alterius = ts;
        }
        alioquin si (op == SILVA_LEX_MAIOR_AEQUALIS)
        {
            /* 0 >= X: zerus SINISTER (VANA dextrum mappat) */
            zerus = ns;
            alter = nd;
            typus_alterius = td;
        }
        si (zerus != NIHIL
            && _constans_probare(sem, zerus, &valor_z)
            && valor_z == ZEPHYRUM)
        {
            ExamenIntervallum iv = _intervallum_expressionis(sem,
                alter, ZEPHYRUM);

            si (commune_insignatum || iv.non_negativum_severum)
            {
                si (!_tolera_absorbere(sem, nodus,
                        (s32)EXAMEN_CODEX_COMPARATIO_DEGRADATA))
                {
                    character textus[CXXVIII];
                    insignatus integer m = silva_c89_typum_scribere(
                        typus_alterius, textus,
                        (insignatus integer)magnitudo(textus));

                    si (m > ZEPHYRUM)
                    {
                        memoriae_index capacitas =
                            (memoriae_index)m
                            + (memoriae_index)LXIV;
                        character* nuntius =
                            (character*)piscina_allocare(
                                sem->piscina, capacitas);

                        si (nuntius != NIHIL)
                        {
                            sprintf(nuntius, "comparatio degradata:"
                                " '<= 0' insignati '== 0' est"
                                " (%s)", textus);
                            _diagnosticum_addere_plenum(sem, nodus,
                                (s32)
                                EXAMEN_CODEX_COMPARATIO_DEGRADATA,
                                NIHIL, nuntius);
                            redde;
                        }
                    }
                    silva_c89_diagnosticum_addere(sem, nodus,
                        (s32)EXAMEN_CODEX_COMPARATIO_DEGRADATA);
                }
                redde;
            }
        }
    }

    /* SIGNORUM: commune insignatum, latus signatum convertitur */
    si (!commune_insignatum)
    {
        redde;
    }
    {
        TypusC89* ps = _promotum(sem, ts);
        TypusC89* pd = _promotum(sem, td);
        constans SilvaNodus* nodus_signatus = NIHIL;
        b32 s_signatum;
        b32 d_signatum;

        s_signatum = (ps->genus == TYPUS_C89_PRIMITIVUS)
            && !_est_insignatum_primitivum(ps->datum.primitivum);
        d_signatum = (pd->genus == TYPUS_C89_PRIMITIVUS)
            && !_est_insignatum_primitivum(pd->datum.primitivum);
        si (s_signatum && !d_signatum)
        {
            nodus_signatus = ns;
        }
        alioquin si (d_signatum && !s_signatum)
        {
            nodus_signatus = nd;
        }
        alioquin
        {
            redde;   /* ambo insignata post promotionem */
        }
        si (_constans_probare(sem, nodus_signatus, &valor)
            && valor >= ZEPHYRUM)
        {
            redde;   /* constans non-negativa (u > 5) */
        }
        {
            ExamenIntervallum iv = _intervallum_expressionis(sem,
                nodus_signatus, ZEPHYRUM);

            si (iv.non_negativum)
            {
                redde;   /* heuristice nonneg (u8, larvae) */
            }
        }
        {
            character textus_s[CXXVIII];
            character textus_d[CXXVIII];
            insignatus integer m_s = silva_c89_typum_scribere(ts,
                textus_s, (insignatus integer)magnitudo(textus_s));
            insignatus integer m_d = silva_c89_typum_scribere(td,
                textus_d, (insignatus integer)magnitudo(textus_d));

            si (m_s > ZEPHYRUM && m_d > ZEPHYRUM)
            {
                memoriae_index capacitas = (memoriae_index)m_s
                    + (memoriae_index)m_d + (memoriae_index)LXIV;
                character* nuntius = (character*)piscina_allocare(
                    sem->piscina, capacitas);

                si (nuntius != NIHIL)
                {
                    sprintf(nuntius, "comparatio signorum"
                        " diversorum: %s cum %s", textus_s,
                        textus_d);
                    _diagnosticum_addere_plenum(sem, nodus,
                        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM,
                        NIHIL, nuntius);
                    redde;
                }
            }
            silva_c89_diagnosticum_addere(sem, nodus,
                (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM);
        }
    }
}

/* Conversum annotare si typus valoris re vera mutatur */
/* Capitne expressio finem angustum? (pro codice 68). Recursivum:
 * constans -> probatio intervalli EXACTA per aestimatorem (tabulae
 * -1 in s8: latitudo negativa fallax, valor exactus verus);
 * parenthesis/ternarius -> per bracchia (selectio: ' ' : (character)x
 * - clang bracchia credit); typus naturalis satis angustus ->
 * structura capit (bracchium casus vestitum); alioquin intervallum
 * (larva & capit, % non - paritas clang). */
interior constans SilvaNodus*
_nodus_valoris (SilvaValor v)
{
    redde v.genus == SILVA_VALOR_NODUS ? v.datum.nodus : NIHIL;
}

interior b32
_angustatio_capit (SilvaSemantica* sem, constans SilvaNodus* nodus,
    b32 i_finis, i32 bita_finis, i32 altitudo)
{
    s64 valor;

    si (nodus == NIHIL || altitudo > XVI)
    {
        redde FALSUM;
    }
    nodus = _canonicum(nodus);
    si (_constans_probare(sem, nodus, &valor))
    {
        si (i_finis)
        {
            s64 maximus = ((s64)I << bita_finis) - I;

            redde valor >= ZEPHYRUM && valor <= maximus;
        }
        {
            s64 dimidius = (s64)I << (bita_finis - I);

            redde valor >= -dimidius && valor <= dimidius - I;
        }
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        redde _angustatio_capit(sem,
            _nodus_valoris(silva_c89_parenthesis_internum(nodus)),
            i_finis, bita_finis, altitudo + I);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_TERNARIUS)
    {
        redde _angustatio_capit(sem,
                _nodus_valoris(silva_c89_ternarius_verum(nodus)),
                i_finis, bita_finis, altitudo + I)
            && _angustatio_capit(sem,
                _nodus_valoris(silva_c89_ternarius_falsum(nodus)),
                i_finis, bita_finis, altitudo + I);
    }
    {
        /* typus naturalis structuraliter capiens (bracchium casus
         * vestitum, sub-expressio angusta) */
        constans SemanticaTypatio* ty = _typationem_invenire(sem,
            nodus);

        si (ty != NIHIL && ty->naturalis != NIHIL)
        {
            s32 p = _primitivum_integrale(ty->naturalis);

            si (p >= ZEPHYRUM
                && _est_insignatum_primitivum(p) == i_finis
                && (i32)(sem->primitivi[p]->magnitudo_octetorum
                    * VIII) <= bita_finis)
            {
                redde VERUM;
            }
        }
    }
    {
        ExamenIntervallum iv = _intervallum_expressionis(sem, nodus,
            ZEPHYRUM);

        redde i_finis ? (iv.latitudo <= bita_finis)
                      : (iv.latitudo < bita_finis);
    }
}

/* MENU-FINALE: angustatio latitudinis EIUSDEM signi (codex 68).
 * Directio latitudinis fratris 54: trans-signum ILLE tegit (ramus
 * insignatum->signatum intervallo latitudinis iam probat - nulla
 * duplicatio). Calibratio 2026-07-17: tria sub-vexilla clang
 * (constant-conversion / implicit-int-conversion / shorten-64-to-32)
 * = codex unus noster; constantes capientes exemptae ('= 100' in
 * s8 tacet, '= 300' flagrat); clang larvam '& 0xFF' credit sed
 * '% 256' NON - intervalla NOSTRA decidunt (divergentia tutior
 * licita); casus explicitus silet. */
interior vacuum
_angustationem_examinare (SilvaSemantica* sem,
    constans SilvaNodus* nodus, TypusC89* naturalis, TypusC89* finis)
{
    s32 p_naturalis;
    s32 p_finis;
    b32 i_finis;
    i32 bita_finis;
    constans SilvaNodus* pater;

    si (sem->in_systemate)
    {
        redde;
    }
    p_naturalis = _primitivum_integrale(naturalis);
    p_finis = _primitivum_integrale(finis);
    si (p_naturalis < ZEPHYRUM || p_finis < ZEPHYRUM)
    {
        redde;
    }
    i_finis = _est_insignatum_primitivum(p_finis);
    si (_est_insignatum_primitivum(p_naturalis) != i_finis)
    {
        /* trans-signum = territorium 54 FERE: signatum->insignatum
         * non-negativum 54 exemit sed latitudo manet (parvum = 300:
         * clang -Wconstant-conversion, 300 -> 44). Solum hunc casum
         * hic iudicamus - 54 tacet, nulla duplicatio; ceteri
         * trans-signi 54 ferunt. */
        ExamenIntervallum iv_signi;

        si (!(i_finis && !_est_insignatum_primitivum(p_naturalis)))
        {
            redde;
        }
        iv_signi = _intervallum_expressionis(sem, nodus, ZEPHYRUM);
        si (!iv_signi.non_negativum)
        {
            redde;   /* 54 flagrat - dedup */
        }
    }
    bita_finis = (i32)(sem->primitivi[p_finis]->magnitudo_octetorum
        * VIII);
    si ((i32)(sem->primitivi[p_naturalis]->magnitudo_octetorum
            * VIII) <= bita_finis)
    {
        redde;   /* non angustatio */
    }
    pater = nodus->pater;
    si (pater != NIHIL)
    {
        si (pater->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
        {
            redde;   /* cast explicita */
        }
        si (pater->genus == (s32)SILVA_C89_GENUS_ASSIGNATIO)
        {
            /* latus sinistrum assignationis operatae - paritas 54 */
            SilvaValor op_v = silva_c89_assignatio_tok_operator(
                pater);
            SilvaValor s_v = silva_c89_assignatio_sinister(pater);

            si (op_v.genus == SILVA_VALOR_TOKEN
                && (s32)op_v.datum.token->genus
                    != SILVA_LEX_ASSIGNATIO
                && s_v.genus == SILVA_VALOR_NODUS
                && _canonicum(s_v.datum.nodus) == nodus)
            {
                redde;
            }
        }
    }
    si (_fons_alienus(sem, nodus))
    {
        redde;
    }
    si (_angustatio_capit(sem, nodus, i_finis, bita_finis, ZEPHYRUM))
    {
        redde;
    }
    {
        character textus_naturalis[CXXVIII];
        character textus_finis[CXXVIII];
        insignatus integer m_naturalis = silva_c89_typum_scribere(
            naturalis, textus_naturalis,
            (insignatus integer)magnitudo(textus_naturalis));
        insignatus integer m_finis = silva_c89_typum_scribere(
            finis, textus_finis,
            (insignatus integer)magnitudo(textus_finis));

        si (m_naturalis > ZEPHYRUM && m_finis > ZEPHYRUM)
        {
            memoriae_index capacitas = (memoriae_index)m_naturalis
                + (memoriae_index)m_finis + (memoriae_index)LXIV;
            character* nuntius = (character*)piscina_allocare(
                sem->piscina, capacitas);

            si (nuntius != NIHIL)
            {
                sprintf(nuntius, "angustatio implicita:"
                    " %s -> %s", textus_naturalis, textus_finis);
                _diagnosticum_addere_plenum(sem, nodus,
                    (s32)EXAMEN_CODEX_ANGUSTATIO, NIHIL, nuntius);
            }
        }
    }
}

/* MENU-FINALE: variabiles et parametra inutilia (codices 69/70,
 * paritas -Wunused-variable in -Wall / -Wunused-parameter in
 * -Wextra). Calibratio 2026-07-17: initiator cum effectu NON
 * eximit; staticus localis flagrat; (vacuum)x usus est (nexus eum
 * registrat - semantica eadem sponte). Symbola = registrationes:
 * externa localia transeunt (declarationes, non definitiones),
 * implicita transeunt, systema transit. Vocatur post ambulationem
 * totam (iuxta toleras irritas). */
interior vacuum
_inutiles_examinare (SilvaSemantica* sem)
{
    i32 i;
    i32 m = xar_numerus(sem->symbola);

    /* ABSTENTIO TU: typi nominati ignoti = contextus typorum non
     * resolvit (capsula_libri: 293 ignoti, stratum symbolorum
     * corruptum - phantasmata in capitibus). Iudicium inutilium
     * stratum symbolorum sanum postulat; degradatum abstinet. */
    {
        i32 d;
        i32 numerus_diagnosticorum = xar_numerus(sem->diagnostica);

        per (d = ZEPHYRUM; d < numerus_diagnosticorum; d++)
        {
            constans SemanticaDiagnosticum* dg =
                silva_c89_diagnosticum_per_indicem(sem, d);

            si (dg != NIHIL && dg->codex
                == (s32)EXAMEN_CODEX_TYPUS_NOMINATUS_IGNOTUS)
            {
                redde;
            }
        }
    }

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaSymbolum* s = *(SemanticaSymbolum**)
            xar_obtinere(sem->symbola, i);
        s32 codex;

        si (s == NIHIL || s->usus || s->ex_systemate
            || s->est_implicitum || s->declarans == NIHIL)
        {
            perge;
        }
        si ((s->repositio & (i32)REPOSITIO_EXTERNA) != ZEPHYRUM)
        {
            perge;
        }
        si (s->genus == (s32)SYMBOLUM_VARIABILE
            && s->profunditas > ZEPHYRUM)
        {
            codex = (s32)EXAMEN_CODEX_VARIABILIS_INUTILIS;
        }
        alioquin si (s->genus == (s32)SYMBOLUM_PARAMETRUM)
        {
            codex = (s32)EXAMEN_CODEX_PARAMETRUM_INUTILE;
        }
        alioquin
        {
            perge;
        }
        si (_fons_alienus(sem, s->declarans))
        {
            perge;
        }
        si (_intra_ambiguum(s->declarans))
        {
            /* lectio incerta: symbola phantasmata non iudicantur */
            perge;
        }
        si (s->typus == NIHIL || s->typus == sem->typus_erroris
            || _qualibus_exutum(s->typus)->genus == TYPUS_C89_ERROR)
        {
            /* typus venenatus (TU degradata - capsula_libri: 293
             * typi ignoti, symbola phantasmata in capitibus) -
             * regula veneni: degradatum non iudicatur */
            perge;
        }
        silva_c89_diagnosticum_addere(sem, s->declarans, codex);
    }
}

interior vacuum
_conversionem_annotare (SilvaSemantica* sem,
    constans SilvaNodus* nodus, TypusC89* naturalis, TypusC89* finis)
{
    si (naturalis == NIHIL || finis == NIHIL)
    {
        redde;
    }
    si (finis->genus == TYPUS_C89_ERROR
        || _qualibus_exutum(naturalis)->genus == TYPUS_C89_ERROR)
    {
        redde;
    }
    si (_qualibus_exutum(naturalis) != finis)
    {
        _conversionem_signi_examinare(sem, nodus, naturalis, finis);
        _angustationem_examinare(sem, nodus, naturalis, finis);
        _conversum_ponere(sem, nodus, finis);
    }
}

/* Operandum ut monstratorem (post lapsum): monstrator ipse,
 * acies/functio lapsae; NIHIL = non monstrator */
interior TypusC89*
_ut_monstrator (SilvaSemantica* sem, TypusC89* typus)
{
    TypusC89* t = _qualibus_exutum(typus);

    si (t == NIHIL)
    {
        redde NIHIL;
    }
    si (t->genus == TYPUS_C89_MONSTRATOR)
    {
        redde t;
    }
    si (t->genus == TYPUS_C89_ACIES || t->genus == TYPUS_C89_FUNCTIO)
    {
        redde _lapsus(sem, typus);
    }
    redde NIHIL;
}

/* Constans monstratoris nulla (C89): expressio constans integralis
 * valoris 0, aut talis ad (void*) conversa */
interior b32
_est_constans_nulla (SilvaSemantica* sem, constans SilvaNodus* nodus)
{
    s64 valor = ZEPHYRUM;

    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    nodus = _canonicum(nodus);
    si (silva_c89_constans_aestimare(sem, nodus, &valor))
    {
        redde valor == ZEPHYRUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
    {
        SilvaValor typus_v = silva_c89_conversio_typus(nodus);
        SilvaValor v = silva_c89_conversio_internum(nodus);
        TypusC89* t;

        si (typus_v.genus != SILVA_VALOR_NODUS
            || v.genus != SILVA_VALOR_NODUS)
        {
            redde FALSUM;
        }
        t = _qualibus_exutum(silva_c89_typus_ex_specie(sem,
            typus_v.datum.nodus));
        si (t->genus != TYPUS_C89_MONSTRATOR
            || _qualibus_exutum(t->datum.monstrator.internum)
                != sem->primitivi[PRIMITIVUM_VACUUM])
        {
            redde FALSUM;
        }
        redde silva_c89_constans_aestimare(sem, v.datum.nodus,
                  &valor)
            && valor == ZEPHYRUM;
    }
    redde FALSUM;
}

/* ==================================================
 * Relatio compatibilitatis (examen chunk B)
 * ================================================== */

/* Typus promotione ordinaria immotus? (reconciliatio K&R pura -
 * char/brevis -> int, fluitans -> duplex mutantur; cetera manent;
 * sine sem, sine internamento) */
interior b32
_promotione_immotum (TypusC89* typus)
{
    TypusC89* n = _qualibus_exutum(typus);

    si (n == NIHIL)
    {
        redde FALSUM;
    }
    si (n->genus == TYPUS_C89_PRIMITIVUS)
    {
        commutatio (n->datum.primitivum)
        {
            casus (s32)PRIMITIVUM_CHARACTER:
            casus (s32)PRIMITIVUM_CHARACTER_SIGNATUM:
            casus (s32)PRIMITIVUM_CHARACTER_INSIGNATUM:
            casus (s32)PRIMITIVUM_BREVIS:
            casus (s32)PRIMITIVUM_BREVIS_INSIGNATUM:
            casus (s32)PRIMITIVUM_FLUITANS:
                redde FALSUM;
            ordinarius:
                redde VERUM;
        }
    }
    redde VERUM;
}

b32
silva_c89_typi_compatibiles (TypusC89* a, TypusC89* b)
{
    i32 qa;
    i32 qb;
    TypusC89* na;
    TypusC89* nb;

    si (a == NIHIL || b == NIHIL)
    {
        redde FALSUM;
    }
    si (a == b)
    {
        redde VERUM;    /* identitas (internamentum) */
    }
    qa = _quales_typi(a);
    qb = _quales_typi(b);
    na = _qualibus_exutum(a);
    nb = _qualibus_exutum(b);
    si (na == NIHIL || nb == NIHIL)
    {
        redde FALSUM;
    }
    si (na->genus == TYPUS_C89_ERROR
        || nb->genus == TYPUS_C89_ERROR)
    {
        redde VERUM;    /* venenum absorbet - numquam re-iudica */
    }
    si (qa != qb)
    {
        redde FALSUM;   /* quales ut copiae per gradum */
    }
    si (na == nb)
    {
        redde VERUM;
    }
    /* enumeratus <-> int (6.1.2.5; impl LP64 = int) */
    si ((na->genus == TYPUS_C89_ENUMERATUS
            && nb->genus == TYPUS_C89_PRIMITIVUS
            && nb->datum.primitivum == (s32)PRIMITIVUM_INTEGER)
        || (nb->genus == TYPUS_C89_ENUMERATUS
            && na->genus == TYPUS_C89_PRIMITIVUS
            && na->datum.primitivum == (s32)PRIMITIVUM_INTEGER))
    {
        redde VERUM;
    }
    si (na->genus != nb->genus)
    {
        redde FALSUM;
    }
    commutatio (na->genus)
    {
        casus TYPUS_C89_MONSTRATOR:
            /* pointee compatibilis QUALIBUS INCLUSIS (6.1.2.6) */
            redde silva_c89_typi_compatibiles(
                na->datum.monstrator.internum,
                nb->datum.monstrator.internum);
        casus TYPUS_C89_ACIES:
            si (!silva_c89_typi_compatibiles(
                    na->datum.acies.elementum,
                    nb->datum.acies.elementum))
            {
                redde FALSUM;
            }
            si (na->datum.acies.numerus < ZEPHYRUM
                || nb->datum.acies.numerus < ZEPHYRUM)
            {
                redde VERUM;    /* sentinella: mensura ignota */
            }
            redde na->datum.acies.numerus
                == nb->datum.acies.numerus;
        casus TYPUS_C89_FUNCTIO:
            si (!silva_c89_typi_compatibiles(
                    na->datum.functio.reditus,
                    nb->datum.functio.reditus))
            {
                redde FALSUM;
            }
            si (na->datum.functio.est_prototypata
                && nb->datum.functio.est_prototypata)
            {
                i32 i;

                si (na->datum.functio.est_variadica
                        != nb->datum.functio.est_variadica
                    || na->datum.functio.numerus_parametrorum
                        != nb->datum.functio.numerus_parametrorum)
                {
                    redde FALSUM;
                }
                per (i = ZEPHYRUM;
                     i < na->datum.functio.numerus_parametrorum;
                     i++)
                {
                    /* quales summi parametrorum exuti (6.5.4.3) */
                    si (!silva_c89_typi_compatibiles(
                            _qualibus_exutum(
                                na->datum.functio.parametra[i]),
                            _qualibus_exutum(
                                nb->datum.functio.parametra[i])))
                    {
                        redde FALSUM;
                    }
                }
                redde VERUM;
            }
            si (!na->datum.functio.est_prototypata
                && !nb->datum.functio.est_prototypata)
            {
                redde VERUM;
            }
            /* mixta prototypata/K&R: non variadica, parametra
             * promotione ordinaria immota (6.1.2.6) */
            {
                TypusC89* proto =
                    na->datum.functio.est_prototypata ? na : nb;
                i32 i;

                si (proto->datum.functio.est_variadica)
                {
                    redde FALSUM;
                }
                per (i = ZEPHYRUM;
                     i < proto->datum.functio.numerus_parametrorum;
                     i++)
                {
                    si (!_promotione_immotum(
                            proto->datum.functio.parametra[i]))
                    {
                        redde FALSUM;
                    }
                }
                redde VERUM;
            }
        ordinarius:
            /* primitivi/tags: internati - identitas iam probata */
            redde FALSUM;
    }
}

s32
silva_c89_assignationem_iudicare (SilvaSemantica* sem,
    constans SilvaNodus* nodus_valoris, TypusC89* finis,
    TypusC89* valoris, s32* codex_out)
{
    TypusC89* f;
    TypusC89* v;

    si (codex_out != NIHIL)
    {
        *codex_out = -I;
    }
    si (sem == NIHIL || finis == NIHIL || valoris == NIHIL)
    {
        redde (s32)EXAMEN_LICET;    /* input fractum - absorbe */
    }
    f = _qualibus_exutum(finis);
    v = _qualibus_exutum(valoris);
    si (f == NIHIL || v == NIHIL
        || f->genus == TYPUS_C89_ERROR
        || v->genus == TYPUS_C89_ERROR)
    {
        redde (s32)EXAMEN_LICET;    /* venenum - numquam re-iudica */
    }
    si (_est_arithmeticum(f) && _est_arithmeticum(valoris))
    {
        redde (s32)EXAMEN_LICET_CONVERSIO;
    }
    si (f->genus == TYPUS_C89_ACIES
        && _est_acies_characterum(f)
        && _est_acies_characterum(valoris))
    {
        /* initiatio aciei characterum per chordam litteralem -
         * legalis C89 6.5.7 (inventum primi contactus chunk C:
         * cauda residua eam VETUISSET - classis C4 iterum) */
        redde (s32)EXAMEN_LICET;
    }
    si (f->genus == TYPUS_C89_MONSTRATOR)
    {
        TypusC89* vm;

        si (nodus_valoris != NIHIL
            && _est_constans_nulla(sem, nodus_valoris))
        {
            redde (s32)EXAMEN_LICET_CONVERSIO;
        }
        vm = _ut_monstrator(sem, valoris);
        si (vm == NIHIL)
        {
            si (codex_out != NIHIL)
            {
                *codex_out = _est_arithmeticum(valoris)
                    ? (s32)EXAMEN_CODEX_MONSTRATOR_INTEGER
                    : (s32)EXAMEN_CODEX_ASSIGNATIO_INCOMPATIBILIS;
            }
            redde (s32)EXAMEN_VETITUM;
        }
        {
            TypusC89* fp = f->datum.monstrator.internum;
            TypusC89* vp = vm->datum.monstrator.internum;
            i32 fq = _quales_typi(fp);
            i32 vq = _quales_typi(vp);
            TypusC89* fps = _qualibus_exutum(fp);
            TypusC89* vps = _qualibus_exutum(vp);
            b32 per_vacuum =
                (fps != NIHIL
                    && fps->genus == TYPUS_C89_PRIMITIVUS
                    && fps->datum.primitivum
                        == (s32)PRIMITIVUM_VACUUM)
                || (vps != NIHIL
                    && vps->genus == TYPUS_C89_PRIMITIVUS
                    && vps->datum.primitivum
                        == (s32)PRIMITIVUM_VACUUM);

            si (per_vacuum)
            {
                si ((fq & vq) == vq)    /* finis quales ⊇ valoris */
                {
                    redde (vm == f)
                        ? (s32)EXAMEN_LICET
                        : (s32)EXAMEN_LICET_CONVERSIO;
                }
                si (codex_out != NIHIL)
                {
                    *codex_out = (s32)EXAMEN_CODEX_QUALES_ABIECTI;
                }
                redde (s32)EXAMEN_VETITUM;
            }
            {
                b32 abiecti = FALSUM;

                si (_pointee_assignabilis(fp, vp, &abiecti))
                {
                    redde (vm == f)
                        ? (s32)EXAMEN_LICET
                        : (s32)EXAMEN_LICET_CONVERSIO;
                }
                si (codex_out != NIHIL)
                {
                    *codex_out = abiecti
                        ? (s32)EXAMEN_CODEX_QUALES_ABIECTI
                        : (s32)
                          EXAMEN_CODEX_MONSTRATORES_INCOMPATIBILES;
                }
            }
            redde (s32)EXAMEN_VETITUM;
        }
    }
    si (_est_arithmeticum(f))
    {
        /* f arithmeticus, valoris non: monstrator/acies/functio */
        si (codex_out != NIHIL)
        {
            *codex_out =
                (_ut_monstrator(sem, valoris) != NIHIL)
                    ? (s32)EXAMEN_CODEX_MONSTRATOR_INTEGER
                    : (s32)EXAMEN_CODEX_ASSIGNATIO_INCOMPATIBILIS;
        }
        redde (s32)EXAMEN_VETITUM;
    }
    si (f->genus == TYPUS_C89_STRUCTURA
        || f->genus == TYPUS_C89_UNIO)
    {
        si (silva_c89_typi_compatibiles(f, v))
        {
            redde (s32)EXAMEN_LICET;    /* aggregata identica */
        }
        si (codex_out != NIHIL)
        {
            *codex_out =
                (s32)EXAMEN_CODEX_ASSIGNATIO_INCOMPATIBILIS;
        }
        redde (s32)EXAMEN_VETITUM;
    }
    si (codex_out != NIHIL)
    {
        *codex_out = (s32)EXAMEN_CODEX_ASSIGNATIO_INCOMPATIBILIS;
    }
    redde (s32)EXAMEN_VETITUM;
}

/* Conversio "ad finem" (assignatio simplex / redde / initiator
 * scalaris / argumentum / elementum): SEDES iudicii (chunk C).
 * LICET -> nihil; LICET_CONVERSIO -> annotare ut semper (semitae
 * legales byte-idem); VETITUM -> diagnosticum, NOLI annotare
 * (conversus fabricatus abolitus - demissio absentem = nullam
 * conversionem legit). */
interior vacuum
_ad_finem_annotare (SilvaSemantica* sem, constans SilvaNodus* nodus,
    TypusC89* valoris, TypusC89* finis)
{
    s32 codex = -I;
    s32 licentia = silva_c89_assignationem_iudicare(sem, nodus,
        finis, valoris, &codex);

    si (licentia == (s32)EXAMEN_VETITUM)
    {
        si (codex >= ZEPHYRUM)
        {
            silva_c89_diagnosticum_addere(sem, nodus, codex);
        }
        redde;
    }
    /* LICET + LICET_CONVERSIO: annotare ut semper - probatio
     * mutationis intra annotare ordines hodiernos EXACTE reddit
     * (lapsus aciei conversum dat etiam sub LICET; aggregata
     * identica nullum, quia exutum == finis) */
    _conversionem_annotare(sem, nodus, valoris,
        _qualibus_exutum(finis));
}

/* Locus (lvalue C89): proprietas FORMAE expressionis, in poscente
 * (chunk C, sim-6 C6: NULLA repositio - tabulae demissionis
 * intactae). Identificator obiecti, chorda litteralis, deiectio,
 * subscriptio, sagitta, punctum-si-basis-locus, (locus). */
interior b32
_est_locus (SilvaSemantica* sem, constans SilvaNodus* nodus)
{
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    nodus = _canonicum(nodus);
    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR:
        {
            constans SemanticaSymbolum* s =
                silva_c89_symbolum_nodi(sem, nodus);

            si (s == NIHIL)
            {
                redde VERUM;    /* ignotus = venenatus; clemens */
            }
            redde s->genus == (s32)SYMBOLUM_VARIABILE
                || s->genus == (s32)SYMBOLUM_PARAMETRUM;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHORDA:
            redde VERUM;
        casus (s32)SILVA_C89_GENUS_PARENTHESIS:
        {
            SilvaValor v = silva_c89_parenthesis_internum(nodus);

            si (v.genus != SILVA_VALOR_NODUS)
            {
                redde FALSUM;
            }
            redde _est_locus(sem, v.datum.nodus);
        }
        casus (s32)SILVA_C89_GENUS_SUBSCRIPTIO:
            redde VERUM;
        casus (s32)SILVA_C89_GENUS_ACCESSUS:
        {
            SilvaValor op_v = silva_c89_accessus_tok_operator(nodus);
            SilvaValor b_v = silva_c89_accessus_basis(nodus);

            si (op_v.genus == SILVA_VALOR_TOKEN
                && op_v.datum.token->genus == SILVA_LEX_SAGITTA)
            {
                redde VERUM;
            }
            si (b_v.genus != SILVA_VALOR_NODUS)
            {
                redde FALSUM;
            }
            redde _est_locus(sem, b_v.datum.nodus);
        }
        casus (s32)SILVA_C89_GENUS_UNARIUM:
        {
            SilvaValor op_v = silva_c89_unarium_tok_operator(nodus);

            redde op_v.genus == SILVA_VALOR_TOKEN
                && op_v.datum.token->genus == SILVA_LEX_STAR;
        }
        ordinarius:
            redde FALSUM;
    }
}

/* Locus mutabilis = locus + typus: non const-summus (quales basis
 * in membra IAM propagati), non acies, non functio, tag completus */
interior b32
_est_locus_mutabilis (TypusC89* typus)
{
    TypusC89* n;

    si (typus == NIHIL)
    {
        redde VERUM;    /* clemens */
    }
    si (_quales_typi(typus) & QUALIS_CONSTANS)
    {
        redde FALSUM;
    }
    n = _qualibus_exutum(typus);
    si (n == NIHIL)
    {
        redde VERUM;
    }
    commutatio (n->genus)
    {
        casus TYPUS_C89_ACIES:
        casus TYPUS_C89_FUNCTIO:
            redde FALSUM;
        casus TYPUS_C89_STRUCTURA:
        casus TYPUS_C89_UNIO:
            /* incompletus = CLEMENS (inventum corporis chunk C:
             * incompletus-quia-ignotus [lexicon] indiscernibilis ab
             * incompleto vero; sedes vera = declaratio obiecti
             * incompleti, parca nominata c89-lacunae.md) */
            redde VERUM;
        ordinarius:
            redde VERUM;
    }
}

/* Pointee assignabilis (chunk D, inventum differentiale primum):
 * inclusio qualium PER GRADUS ACIERUM descendens (quales aciei =
 * quales elementi; additio tuta const per gradus - oraculum clang
 * consentit, littera C89 strictior sed praxis universalis),
 * compatibilitas plena infra gradus acierum. */
interior b32
_pointee_assignabilis (TypusC89* fp, TypusC89* vp, b32* abiecti)
{
    dum (VERUM)
    {
        i32 fq = _quales_typi(fp);
        i32 vq = _quales_typi(vp);
        TypusC89* fs = _qualibus_exutum(fp);
        TypusC89* vs = _qualibus_exutum(vp);

        si ((fq & vq) != vq)
        {
            *abiecti = VERUM;
            redde FALSUM;
        }
        si (fs == NIHIL || vs == NIHIL)
        {
            redde FALSUM;
        }
        si (fs->genus == TYPUS_C89_ACIES
            && vs->genus == TYPUS_C89_ACIES)
        {
            si (fs->datum.acies.numerus >= ZEPHYRUM
                && vs->datum.acies.numerus >= ZEPHYRUM
                && fs->datum.acies.numerus
                    != vs->datum.acies.numerus)
            {
                redde FALSUM;
            }
            fp = fs->datum.acies.elementum;
            vp = vs->datum.acies.elementum;
            perge;
        }
        redde silva_c89_typi_compatibiles(fs, vs);
    }
}

/* Monstratores comparabiles/subtrahibiles: pointee exuti
 * compatibiles aut alteruter vacuum (chunk C - profundatio sedium
 * operatorum ubi classis sola probabatur) */
interior b32
_monstratores_comparabiles (TypusC89* pa, TypusC89* pb)
{
    TypusC89* ia;
    TypusC89* ib;

    si (pa == NIHIL || pb == NIHIL
        || pa->genus != TYPUS_C89_MONSTRATOR
        || pb->genus != TYPUS_C89_MONSTRATOR)
    {
        redde VERUM;    /* clemens - classis iam probata */
    }
    ia = _qualibus_exutum(pa->datum.monstrator.internum);
    ib = _qualibus_exutum(pb->datum.monstrator.internum);
    si (ia == NIHIL || ib == NIHIL)
    {
        redde VERUM;
    }
    si ((ia->genus == TYPUS_C89_PRIMITIVUS
            && ia->datum.primitivum == (s32)PRIMITIVUM_VACUUM)
        || (ib->genus == TYPUS_C89_PRIMITIVUS
            && ib->datum.primitivum == (s32)PRIMITIVUM_VACUUM))
    {
        redde VERUM;
    }
    /* comparatio quales negligit - assignabilis utroque modo */
    {
        b32 neglectum = FALSUM;

        redde _pointee_assignabilis(
                pa->datum.monstrator.internum,
                pb->datum.monstrator.internum, &neglectum)
            || _pointee_assignabilis(
                pb->datum.monstrator.internum,
                pa->datum.monstrator.internum, &neglectum);
    }
}

/* Typus litteralis integri - C89 6.1.3.2 + extensio longus longus:
 * decimale sine suffixo: int -> long (LP64 cetera capit);
 * hex/octale sine suffixo: int -> unsigned int -> long. */
interior TypusC89*
_folium_integer_typus (SilvaSemantica* sem, constans SilvaToken* tok)
{
    s64 valor = ZEPHYRUM;
    i32 basis = X;
    b32 habet_u = FALSUM;
    i32 numerus_l = ZEPHYRUM;
    s32 p;

    si (!_folium_integer_legere(tok, &valor, &basis, &habet_u,
            &numerus_l))
    {
        redde sem->typus_erroris;
    }
    si (numerus_l >= II)
    {
        /* extensio (s64) */
        p = habet_u ? (s32)PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM
                    : (s32)PRIMITIVUM_LONGUS_LONGUS;
    }
    alioquin si (numerus_l == I)
    {
        p = habet_u ? (s32)PRIMITIVUM_LONGUS_INSIGNATUM
                    : (s32)PRIMITIVUM_LONGUS;
    }
    alioquin si (habet_u)
    {
        p = (valor >= ZEPHYRUM && valor <= (s64)0xffffffff)
            ? (s32)PRIMITIVUM_INTEGER_INSIGNATUM
            : (s32)PRIMITIVUM_LONGUS_INSIGNATUM;
    }
    alioquin si (basis == X)
    {
        p = (valor >= ZEPHYRUM && valor <= (s64)0x7fffffff)
            ? (s32)PRIMITIVUM_INTEGER
            : (s32)PRIMITIVUM_LONGUS;
    }
    alioquin
    {
        /* hex/octale: unsigned int inter int et long (C89) */
        si (valor >= ZEPHYRUM && valor <= (s64)0x7fffffff)
        {
            p = (s32)PRIMITIVUM_INTEGER;
        }
        alioquin si (valor >= ZEPHYRUM && valor <= (s64)0xffffffff)
        {
            p = (s32)PRIMITIVUM_INTEGER_INSIGNATUM;
        }
        alioquin
        {
            p = (s32)PRIMITIVUM_LONGUS;
        }
    }
    redde sem->primitivi[p];
}

interior TypusC89*
_folium_fluitans_typus (SilvaSemantica* sem, constans SilvaToken* tok)
{
    si (tok->valor.mensura > ZEPHYRUM)
    {
        character ultimum = (character)tok->valor.datum[
            tok->valor.mensura - I];

        si (ultimum == 'f' || ultimum == 'F')
        {
            redde sem->primitivi[PRIMITIVUM_FLUITANS];
        }
        si (ultimum == 'l' || ultimum == 'L')
        {
            redde sem->primitivi[PRIMITIVUM_DUPLEX_LONGUS];
        }
    }
    redde sem->primitivi[PRIMITIVUM_DUPLEX];
}

/* Octeti contentorum chordae litteralis (sine NUL claudente):
 * fragmenta (lista - adiacentia iam structuralia) percurrit,
 * fugas decodit. -I = invalida aut lata (parca + diagnosticum). */
interior s32
_chordae_litteralis_octeti (SilvaSemantica* sem,
    constans SilvaNodus* nodus)
{
    SilvaValor fragmenta = silva_c89_folium_chorda_tok_valor(nodus);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(fragmenta);
    s32 summa = ZEPHYRUM;

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(fragmenta, i);
        constans SilvaToken* tok;
        i32 cursor;
        i32 finis;

        si (v == NIHIL || v->genus != SILVA_VALOR_TOKEN)
        {
            perge;
        }
        tok = v->datum.token;
        si (tok->valor.mensura < II)
        {
            redde -I;
        }
        si (tok->valor.datum[ZEPHYRUM] == 'L')
        {
            silva_c89_diagnosticum_addere(sem, nodus,
                EXAMEN_CODEX_CHORDA_LATA);
            redde -I;
        }
        cursor = I;
        finis = tok->valor.mensura - I;   /* " claudens */
        dum (cursor < finis)
        {
            si ((character)tok->valor.datum[cursor] == '\\')
            {
                s64 ignotum = ZEPHYRUM;

                cursor++;
                si (!_fugam_decodere(&tok->valor, &cursor, &ignotum))
                {
                    silva_c89_diagnosticum_addere(sem, nodus,
                        EXAMEN_CODEX_FUGA_INVALIDA);
                    redde -I;
                }
            }
            alioquin
            {
                cursor++;
            }
            summa++;
        }
    }
    redde summa;
}

interior TypusC89*
_expressionem_typare (SilvaSemantica* sem, constans SilvaNodus* nodus)
{
    si (sem == NIHIL || nodus == NIHIL)
    {
        redde NIHIL;
    }
    nodus = _canonicum(nodus);

    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_FOLIUM_INTEGER:
        {
            SilvaValor tok_v = silva_c89_folium_integer_tok_valor(
                nodus);
            TypusC89* t;

            si (tok_v.genus != SILVA_VALOR_TOKEN)
            {
                redde NIHIL;
            }
            t = _folium_integer_typus(sem, tok_v.datum.token);
            si (t->genus == TYPUS_C89_ERROR)
            {
                silva_c89_diagnosticum_addere(sem, nodus,
                    EXAMEN_CODEX_LITTERALE_INVALIDUM);
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS:
        {
            SilvaValor tok_v = silva_c89_folium_fluitans_tok_valor(
                nodus);
            TypusC89* t;

            si (tok_v.genus != SILVA_VALOR_TOKEN)
            {
                redde NIHIL;
            }
            t = _folium_fluitans_typus(sem, tok_v.datum.token);
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER:
        {
            /* litterale characteris typum INT habet (C89 6.1.3.4);
             * L'x' idem in Darwin (wchar_t = int) */
            TypusC89* t = sem->primitivi[PRIMITIVUM_INTEGER];

            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHORDA:
        {
            s32 octeti = _chordae_litteralis_octeti(sem, nodus);
            TypusC89* t;

            si (octeti < ZEPHYRUM)
            {
                t = sem->typus_erroris;
            }
            alioquin
            {
                t = silva_c89_typus_acies(sem,
                    sem->primitivi[PRIMITIVUM_CHARACTER],
                    octeti + I);
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR:
        {
            SilvaValor tok_v =
                silva_c89_folium_identificator_tok_valor(nodus);
            SemanticaSymbolum* symbolum;
            TypusC89* t;

            si (tok_v.genus != SILVA_VALOR_TOKEN)
            {
                redde NIHIL;
            }
            symbolum = silva_c89_symbolum_invenire(sem,
                tok_v.datum.token->valor);
            si (symbolum != NIHIL)
            {
                _nexum_ponere(sem, nodus, symbolum);   /* M1a A */
            }
            si (symbolum == NIHIL)
            {
                /* vocati ignoti = extern int implicitum (Chunk C);
                 * hic locus non-vocati est */
                silva_c89_diagnosticum_addere(sem, nodus,
                    EXAMEN_CODEX_IDENTIFICATOR_IGNOTUS);
                t = sem->typus_erroris;
            }
            alioquin si (symbolum->genus == (s32)SYMBOLUM_TYPEDEF)
            {
                si (!_intra_ambiguum(nodus))
                {
                    silva_c89_diagnosticum_addere(sem, nodus,
                        EXAMEN_CODEX_TYPEDEF_IN_EXPRESSIONE);
                }
                t = sem->typus_erroris;
            }
            alioquin si (symbolum->genus == (s32)SYMBOLUM_CONSTANS)
            {
                /* constantes enumerorum typum int habent (C89) */
                t = sem->primitivi[PRIMITIVUM_INTEGER];
            }
            alioquin
            {
                t = (symbolum->typus != NIHIL) ? symbolum->typus
                                               : sem->typus_erroris;
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_PARENTHESIS:
        {
            SilvaValor v = silva_c89_parenthesis_internum(nodus);
            TypusC89* t;

            si (v.genus != SILVA_VALOR_NODUS)
            {
                redde NIHIL;
            }
            t = _expressionem_typare(sem, v.datum.nodus);
            si (t != NIHIL)
            {
                (vacuum)_typationem_ponere(sem, nodus, t);
            }
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_UNARIUM:
        {
            SilvaValor op_v = silva_c89_unarium_tok_operator(nodus);
            SilvaValor v = silva_c89_unarium_internum(nodus);
            TypusC89* intus;
            TypusC89* t = NIHIL;

            si (op_v.genus != SILVA_VALOR_TOKEN
                || v.genus != SILVA_VALOR_NODUS)
            {
                redde NIHIL;
            }
            intus = _expressionem_typare(sem, v.datum.nodus);
            commutatio ((s32)op_v.datum.token->genus)
            {
                casus SILVA_LEX_EXCLAMATIO:
                    /* ! semper int - etiam operando nondum typato */
                    t = sem->primitivi[PRIMITIVUM_INTEGER];
                    frange;
                casus SILVA_LEX_MINUS:
                casus SILVA_LEX_PLUS:
                    si (intus == NIHIL)
                    {
                        redde NIHIL;
                    }
                    si (_qualibus_exutum(intus)->genus
                        == TYPUS_C89_ERROR)
                    {
                        t = sem->typus_erroris;
                    }
                    alioquin si (!_est_arithmeticum(intus))
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_OPERANDUM_UNARII);
                        t = sem->typus_erroris;
                    }
                    alioquin
                    {
                        t = _promotum(sem, intus);
                    }
                    frange;
                casus SILVA_LEX_TILDE:
                    si (intus == NIHIL)
                    {
                        redde NIHIL;
                    }
                    si (_qualibus_exutum(intus)->genus
                        == TYPUS_C89_ERROR)
                    {
                        t = sem->typus_erroris;
                    }
                    alioquin si (!_est_integrale(intus))
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_OPERANDUM_TILDE);
                        t = sem->typus_erroris;
                    }
                    alioquin
                    {
                        t = _promotum(sem, intus);
                    }
                    frange;
                casus SILVA_LEX_AMPERSAND:
                    si (intus == NIHIL)
                    {
                        redde NIHIL;
                    }
                    /* sedes loci (chunk C): locus AUT designator
                     * functionis (&f legalis) */
                    si (_qualibus_exutum(intus)->genus
                            != TYPUS_C89_FUNCTIO
                        && _qualibus_exutum(intus)->genus
                            != TYPUS_C89_ERROR
                        && !_est_locus(sem, v.datum.nodus))
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_ASSIGNATIO_NON_LOCUS);
                    }
                    /* & naturalem capit - &acies = monstrator ad
                     * aciem (NULLUS lapsus - C89) */
                    t = silva_c89_typus_monstrator(sem, intus);
                    frange;
                casus SILVA_LEX_STAR:
                {
                    TypusC89* basis_t;

                    si (intus == NIHIL)
                    {
                        redde NIHIL;
                    }
                    basis_t = _qualibus_exutum(intus);
                    si (basis_t->genus == TYPUS_C89_ERROR)
                    {
                        t = sem->typus_erroris;
                    }
                    alioquin si (basis_t->genus == TYPUS_C89_ACIES)
                    {
                        _conversum_ponere(sem,
                            _canonicum(v.datum.nodus),
                            _lapsus(sem, intus));
                        t = basis_t->datum.acies.elementum;
                    }
                    alioquin si (basis_t->genus
                        == TYPUS_C89_MONSTRATOR)
                    {
                        t = basis_t->datum.monstrator.internum;
                    }
                    alioquin si (basis_t->genus == TYPUS_C89_FUNCTIO)
                    {
                        t = basis_t;   /* *f = designator functionis */
                    }
                    alioquin
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_DEIECTIO_NON_MONSTRATORIS);
                        t = sem->typus_erroris;
                    }
                    frange;
                }
                casus SILVA_LEX_INCREMENTUM:
                casus SILVA_LEX_DECREMENTUM:
                    si (intus == NIHIL)
                    {
                        redde NIHIL;
                    }
                    t = _qualibus_exutum(intus);
                    si (t->genus != TYPUS_C89_ERROR
                        && !_est_arithmeticum(t)
                        && t->genus != TYPUS_C89_MONSTRATOR)
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_CREMENTUM_NON_SCALARE);
                        t = sem->typus_erroris;
                    }
                    alioquin si (t->genus != TYPUS_C89_ERROR)
                    {
                        /* sedes loci (chunk C) */
                        si (!_est_locus(sem, v.datum.nodus))
                        {
                            silva_c89_diagnosticum_addere(sem,
                                nodus,
                                EXAMEN_CODEX_ASSIGNATIO_NON_LOCUS);
                        }
                        alioquin si (!_est_locus_mutabilis(intus))
                        {
                            silva_c89_diagnosticum_addere(sem,
                                nodus,
                                EXAMEN_CODEX_LOCUS_IMMUTABILIS);
                        }
                    }
                    frange;
                ordinarius:
                    redde NIHIL;
            }
            si (t != NIHIL)
            {
                (vacuum)_typationem_ponere(sem, nodus, t);
            }
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_POSTCREMENTUM:
        {
            SilvaValor v = silva_c89_postcrementum_internum(nodus);
            TypusC89* intus;
            TypusC89* t;

            si (v.genus != SILVA_VALOR_NODUS)
            {
                redde NIHIL;
            }
            intus = _expressionem_typare(sem, v.datum.nodus);
            si (intus == NIHIL)
            {
                redde NIHIL;
            }
            t = _qualibus_exutum(intus);
            si (t->genus != TYPUS_C89_ERROR
                && (_est_arithmeticum(t)
                    || t->genus == TYPUS_C89_MONSTRATOR))
            {
                /* sedes loci (chunk C) */
                si (!_est_locus(sem, v.datum.nodus))
                {
                    silva_c89_diagnosticum_addere(sem, nodus,
                        EXAMEN_CODEX_ASSIGNATIO_NON_LOCUS);
                }
                alioquin si (!_est_locus_mutabilis(intus))
                {
                    silva_c89_diagnosticum_addere(sem, nodus,
                        EXAMEN_CODEX_LOCUS_IMMUTABILIS);
                }
            }
            si (t->genus != TYPUS_C89_ERROR
                && !_est_arithmeticum(t)
                && t->genus != TYPUS_C89_MONSTRATOR)
            {
                silva_c89_diagnosticum_addere(sem, nodus,
                    EXAMEN_CODEX_CREMENTUM_NON_SCALARE);
                t = sem->typus_erroris;
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }

        casus (s32)SILVA_C89_GENUS_BINARIUM:
        {
            SilvaValor op_v = silva_c89_binarium_tok_operator(nodus);
            SilvaValor s_v = silva_c89_binarium_sinister(nodus);
            SilvaValor d_v = silva_c89_binarium_dexter(nodus);
            constans SilvaNodus* ns;
            constans SilvaNodus* nd;
            TypusC89* ts;
            TypusC89* td;
            TypusC89* t = NIHIL;

            si (op_v.genus != SILVA_VALOR_TOKEN
                || s_v.genus != SILVA_VALOR_NODUS
                || d_v.genus != SILVA_VALOR_NODUS)
            {
                _valorem_typare(sem, s_v);
                _valorem_typare(sem, d_v);
                redde NIHIL;
            }
            ns = _canonicum(s_v.datum.nodus);
            nd = _canonicum(d_v.datum.nodus);
            ts = _expressionem_typare(sem, ns);
            td = _expressionem_typare(sem, nd);
            si (ts == NIHIL || td == NIHIL)
            {
                redde NIHIL;   /* operandum nondum typabile (C) */
            }
            si (_qualibus_exutum(ts)->genus == TYPUS_C89_ERROR
                || _qualibus_exutum(td)->genus == TYPUS_C89_ERROR)
            {
                /* venenum absorbet sine cascata, sine strepitu */
                (vacuum)_typationem_ponere(sem, nodus,
                    sem->typus_erroris);
                redde sem->typus_erroris;
            }
            commutatio ((s32)op_v.datum.token->genus)
            {
                casus SILVA_LEX_STAR:
                casus SILVA_LEX_SOLIDUS:
                    si (_est_arithmeticum(ts)
                        && _est_arithmeticum(td))
                    {
                        t = _commune_arithmeticum(sem, ts, td);
                        _conversionem_annotare(sem, ns, ts, t);
                        _conversionem_annotare(sem, nd, td, t);
                    }
                    alioquin
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_MULTIPLICATIVA);
                        t = sem->typus_erroris;
                    }
                    frange;
                casus SILVA_LEX_PERCENTUM:
                casus SILVA_LEX_AMPERSAND:
                casus SILVA_LEX_CARET:
                casus SILVA_LEX_BARRA:
                    si (_est_integrale(ts) && _est_integrale(td))
                    {
                        t = _commune_arithmeticum(sem, ts, td);
                        _conversionem_annotare(sem, ns, ts, t);
                        _conversionem_annotare(sem, nd, td, t);
                    }
                    alioquin
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_OPERANDA_NON_INTEGRALIA);
                        t = sem->typus_erroris;
                    }
                    frange;
                casus SILVA_LEX_PLUS:
                {
                    TypusC89* ps = _ut_monstrator(sem, ts);
                    TypusC89* pd = _ut_monstrator(sem, td);

                    si (_est_arithmeticum(ts)
                        && _est_arithmeticum(td))
                    {
                        t = _commune_arithmeticum(sem, ts, td);
                        _conversionem_annotare(sem, ns, ts, t);
                        _conversionem_annotare(sem, nd, td, t);
                    }
                    alioquin si (ps != NIHIL && _est_integrale(td))
                    {
                        _conversionem_annotare(sem, ns, ts, ps);
                        t = ps;
                    }
                    alioquin si (pd != NIHIL && _est_integrale(ts))
                    {
                        _conversionem_annotare(sem, nd, td, pd);
                        t = pd;
                    }
                    alioquin
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_ADDITIVA_INCOMPATIBILIA);
                        t = sem->typus_erroris;
                    }
                    frange;
                }
                casus SILVA_LEX_MINUS:
                {
                    TypusC89* ps = _ut_monstrator(sem, ts);
                    TypusC89* pd = _ut_monstrator(sem, td);

                    si (_est_arithmeticum(ts)
                        && _est_arithmeticum(td))
                    {
                        t = _commune_arithmeticum(sem, ts, td);
                        _conversionem_annotare(sem, ns, ts, t);
                        _conversionem_annotare(sem, nd, td, t);
                    }
                    alioquin si (ps != NIHIL && pd != NIHIL)
                    {
                        /* monstrator - monstrator = ptrdiff_t
                         * (LP64: long - systema); profundatio
                         * (chunk C): pointee compatibiles */
                        si (!_monstratores_comparabiles(ps, pd))
                        {
                            silva_c89_diagnosticum_addere(sem,
                                nodus,
                                EXAMEN_CODEX_SUBTRACTIONIS_INCOMPATIBILIA);
                        }
                        _conversionem_annotare(sem, ns, ts, ps);
                        _conversionem_annotare(sem, nd, td, pd);
                        t = sem->primitivi[PRIMITIVUM_LONGUS];
                    }
                    alioquin si (ps != NIHIL && _est_integrale(td))
                    {
                        _conversionem_annotare(sem, ns, ts, ps);
                        t = ps;
                    }
                    alioquin
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_SUBTRACTIONIS_INCOMPATIBILIA);
                        t = sem->typus_erroris;
                    }
                    frange;
                }
                casus SILVA_LEX_SINISTRORSUM:
                casus SILVA_LEX_DEXTRORSUM:
                    /* NON UAC: typus = sinister promotus (C89) */
                    si (_est_integrale(ts) && _est_integrale(td))
                    {
                        t = _promotum(sem, ts);
                        _conversionem_annotare(sem, ns, ts, t);
                        _conversionem_annotare(sem, nd, td,
                            _promotum(sem, td));
                    }
                    alioquin
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_MOTUS_NON_INTEGRALIA);
                        t = sem->typus_erroris;
                    }
                    frange;
                casus SILVA_LEX_MINOR:
                casus SILVA_LEX_MAIOR:
                casus SILVA_LEX_MINOR_AEQUALIS:
                casus SILVA_LEX_MAIOR_AEQUALIS:
                casus SILVA_LEX_AEQUALIS_AEQUALIS:
                casus SILVA_LEX_NON_AEQUALIS:
                {
                    TypusC89* ps = _ut_monstrator(sem, ts);
                    TypusC89* pd = _ut_monstrator(sem, td);

                    t = sem->primitivi[PRIMITIVUM_INTEGER];
                    si (_est_arithmeticum(ts)
                        && _est_arithmeticum(td))
                    {
                        TypusC89* commune = _commune_arithmeticum(
                            sem, ts, td);

                        _conversionem_annotare(sem, ns, ts, commune);
                        _conversionem_annotare(sem, nd, td, commune);
                        _comparationem_examinare(sem, nodus, ns, nd,
                            ts, td, commune,
                            (s32)op_v.datum.token->genus);
                    }
                    alioquin si (ps != NIHIL && pd != NIHIL)
                    {
                        /* profundatio (chunk C): pointee
                         * compatibiles aut vacuum */
                        si (!_monstratores_comparabiles(ps, pd))
                        {
                            silva_c89_diagnosticum_addere(sem,
                                nodus,
                                EXAMEN_CODEX_COMPARATIO_INCOMPATIBILIUM);
                        }
                        _conversionem_annotare(sem, ns, ts, ps);
                        _conversionem_annotare(sem, nd, td, pd);
                    }
                    alioquin si (ps != NIHIL
                        && _est_constans_nulla(sem, nd))
                    {
                        _conversionem_annotare(sem, nd, td, ps);
                    }
                    alioquin si (pd != NIHIL
                        && _est_constans_nulla(sem, ns))
                    {
                        _conversionem_annotare(sem, ns, ts, pd);
                    }
                    alioquin
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_COMPARATIO_INCOMPATIBILIUM);
                    }
                    frange;
                }
                casus SILVA_LEX_ET_ET:
                casus SILVA_LEX_VEL_VEL:
                {
                    TypusC89* lapsum_s = _lapsus(sem, ts);
                    TypusC89* lapsum_d = _lapsus(sem, td);

                    si (lapsum_s != NIHIL)
                    {
                        _conversum_ponere(sem, ns, lapsum_s);
                    }
                    si (lapsum_d != NIHIL)
                    {
                        _conversum_ponere(sem, nd, lapsum_d);
                    }
                    t = sem->primitivi[PRIMITIVUM_INTEGER];
                    frange;
                }
                ordinarius:
                    redde NIHIL;
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_TERNARIUS:
        {
            SilvaValor v_v = silva_c89_ternarius_verum(nodus);
            SilvaValor f_v = silva_c89_ternarius_falsum(nodus);
            constans SilvaNodus* nv;
            constans SilvaNodus* nf;
            TypusC89* tv;
            TypusC89* tf;
            TypusC89* t = NIHIL;

            _valorem_typare_scalarem(sem,
                silva_c89_ternarius_conditio(nodus));
            si (v_v.genus != SILVA_VALOR_NODUS
                || f_v.genus != SILVA_VALOR_NODUS)
            {
                redde NIHIL;
            }
            nv = _canonicum(v_v.datum.nodus);
            nf = _canonicum(f_v.datum.nodus);
            tv = _expressionem_typare(sem, nv);
            tf = _expressionem_typare(sem, nf);
            si (tv == NIHIL || tf == NIHIL)
            {
                redde NIHIL;
            }
            si (_qualibus_exutum(tv)->genus == TYPUS_C89_ERROR
                || _qualibus_exutum(tf)->genus == TYPUS_C89_ERROR)
            {
                (vacuum)_typationem_ponere(sem, nodus,
                    sem->typus_erroris);
                redde sem->typus_erroris;
            }
            si (_est_arithmeticum(tv) && _est_arithmeticum(tf))
            {
                t = _commune_arithmeticum(sem, tv, tf);
                _conversionem_annotare(sem, nv, tv, t);
                _conversionem_annotare(sem, nf, tf, t);
            }
            alioquin si (_qualibus_exutum(tv) == _qualibus_exutum(tf))
            {
                t = _qualibus_exutum(tv);   /* tag/vacuum/monstrator idem */
            }
            alioquin
            {
                TypusC89* pv = _ut_monstrator(sem, tv);
                TypusC89* pf = _ut_monstrator(sem, tf);

                si (pv != NIHIL && pf != NIHIL)
                {
                    /* compositum: quales interiorum iunguntur;
                     * vacuum* vincit; alias incompatibilia */
                    TypusC89* iv = pv->datum.monstrator.internum;
                    TypusC89* ifn = pf->datum.monstrator.internum;
                    i32 quales = ZEPHYRUM;
                    TypusC89* ev;
                    TypusC89* ef;

                    si (iv->genus == TYPUS_C89_QUALIFICATUS)
                    {
                        quales |= iv->datum.qualificatus.quales;
                    }
                    si (ifn->genus == TYPUS_C89_QUALIFICATUS)
                    {
                        quales |= ifn->datum.qualificatus.quales;
                    }
                    ev = _qualibus_exutum(iv);
                    ef = _qualibus_exutum(ifn);
                    si (ev == ef)
                    {
                        t = silva_c89_typus_monstrator(sem,
                            silva_c89_typus_qualificatus(sem, ev,
                                quales));
                    }
                    alioquin si (ev == sem->primitivi[PRIMITIVUM_VACUUM]
                        || ef == sem->primitivi[PRIMITIVUM_VACUUM])
                    {
                        t = silva_c89_typus_monstrator(sem,
                            silva_c89_typus_qualificatus(sem,
                                sem->primitivi[PRIMITIVUM_VACUUM],
                                quales));
                    }
                    alioquin
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_BRACCHIA_TERNARII);
                        t = sem->typus_erroris;
                    }
                    si (t != sem->typus_erroris)
                    {
                        _conversionem_annotare(sem, nv, tv, t);
                        _conversionem_annotare(sem, nf, tf, t);
                    }
                }
                alioquin si (pv != NIHIL
                    && _est_constans_nulla(sem, nf))
                {
                    _conversionem_annotare(sem, nf, tf, pv);
                    t = pv;
                }
                alioquin si (pf != NIHIL
                    && _est_constans_nulla(sem, nv))
                {
                    _conversionem_annotare(sem, nv, tv, pf);
                    t = pf;
                }
                alioquin
                {
                    silva_c89_diagnosticum_addere(sem, nodus,
                        EXAMEN_CODEX_BRACCHIA_TERNARII);
                    t = sem->typus_erroris;
                }
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_ASSIGNATIO:
        {
            SilvaValor op_v = silva_c89_assignatio_tok_operator(nodus);
            SilvaValor s_v = silva_c89_assignatio_sinister(nodus);
            SilvaValor d_v = silva_c89_assignatio_dexter(nodus);
            constans SilvaNodus* ns;
            constans SilvaNodus* nd;
            TypusC89* ts;
            TypusC89* td;
            TypusC89* t;

            si (op_v.genus != SILVA_VALOR_TOKEN
                || s_v.genus != SILVA_VALOR_NODUS
                || d_v.genus != SILVA_VALOR_NODUS)
            {
                _valorem_typare(sem, s_v);
                _valorem_typare(sem, d_v);
                redde NIHIL;
            }
            ns = _canonicum(s_v.datum.nodus);
            nd = _canonicum(d_v.datum.nodus);
            ts = _expressionem_typare(sem, ns);
            td = _expressionem_typare(sem, nd);
            si (ts == NIHIL)
            {
                redde NIHIL;
            }
            t = _qualibus_exutum(ts);   /* valor = typus sinistri
                                         * sine qualibus */
            si (t->genus == TYPUS_C89_ERROR || td == NIHIL)
            {
                (vacuum)_typationem_ponere(sem, nodus, t);
                redde t;
            }
            /* sedes loci (chunk C): sinister = locus mutabilis;
             * cauda una - locus ante finem */
            si (!_est_locus(sem, ns))
            {
                silva_c89_diagnosticum_addere(sem, ns,
                    EXAMEN_CODEX_ASSIGNATIO_NON_LOCUS);
            }
            alioquin si (!_est_locus_mutabilis(ts))
            {
                silva_c89_diagnosticum_addere(sem, ns,
                    EXAMEN_CODEX_LOCUS_IMMUTABILIS);
            }
            commutatio ((s32)op_v.datum.token->genus)
            {
                casus SILVA_LEX_ASSIGNATIO:
                    _ad_finem_annotare(sem, nd, td, t);
                    frange;
                casus SILVA_LEX_PLUS_ASSIGNATIO:
                casus SILVA_LEX_MINUS_ASSIGNATIO:
                    si (t->genus == TYPUS_C89_MONSTRATOR)
                    {
                        frange;   /* monstrator +/- integrale */
                    }
                    /* TOLERA CASUS_LAPSUS: cadit in arithmeticam consulto */
                casus SILVA_LEX_STAR_ASSIGNATIO:
                casus SILVA_LEX_SOLIDUS_ASSIGNATIO:
                casus SILVA_LEX_PERCENTUM_ASSIGNATIO:
                casus SILVA_LEX_AMPERSAND_ASSIGNATIO:
                casus SILVA_LEX_CARET_ASSIGNATIO:
                casus SILVA_LEX_BARRA_ASSIGNATIO:
                    si (_est_arithmeticum(ts)
                        && _est_arithmeticum(td))
                    {
                        /* conversiones operationis implicitae
                         * annotantur (demissio: onera, converte,
                         * opera, reconverte, conde) */
                        TypusC89* commune = _commune_arithmeticum(
                            sem, ts, td);

                        _conversionem_annotare(sem, ns, ts, commune);
                        _conversionem_annotare(sem, nd, td, commune);
                    }
                    frange;
                casus SILVA_LEX_SINISTRORSUM_ASSIGNATIO:
                casus SILVA_LEX_DEXTRORSUM_ASSIGNATIO:
                    si (_est_integrale(ts) && _est_integrale(td))
                    {
                        _conversionem_annotare(sem, ns, ts,
                            _promotum(sem, ts));
                        _conversionem_annotare(sem, nd, td,
                            _promotum(sem, td));
                    }
                    frange;
                ordinarius:
                    frange;
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_VIRGULA:
        {
            SilvaValor d_v = silva_c89_virgula_dexter(nodus);
            constans SilvaNodus* nd;
            TypusC89* td;
            TypusC89* t;
            TypusC89* lapsum;

            _valorem_typare(sem, silva_c89_virgula_sinister(nodus));
            si (d_v.genus != SILVA_VALOR_NODUS)
            {
                redde NIHIL;
            }
            nd = _canonicum(d_v.datum.nodus);
            td = _expressionem_typare(sem, nd);
            si (td == NIHIL)
            {
                redde NIHIL;
            }
            lapsum = _lapsus(sem, td);
            si (lapsum != NIHIL)
            {
                _conversum_ponere(sem, nd, lapsum);
                t = lapsum;
            }
            alioquin
            {
                t = _qualibus_exutum(td);
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_CONVERSIO:
        {
            /* B (emendatio INTENTIO: conversio nulli assignata
             * erat; auspex eam trahit - IPSA operatio conversionis
             * est). Typus = species sine qualibus; conversus
             * interni = typus (demissio hoc legit). */
            SilvaValor typus_v = silva_c89_conversio_typus(nodus);
            SilvaValor v = silva_c89_conversio_internum(nodus);
            constans SilvaNodus* ni;
            TypusC89* ti;
            TypusC89* t;

            si (typus_v.genus != SILVA_VALOR_NODUS
                || v.genus != SILVA_VALOR_NODUS)
            {
                _valorem_typare(sem, v);
                redde NIHIL;
            }
            ni = _canonicum(v.datum.nodus);
            ti = _expressionem_typare(sem, ni);
            t = _qualibus_exutum(silva_c89_typus_ex_specie(sem,
                typus_v.datum.nodus));
            si (ti != NIHIL && t->genus != TYPUS_C89_ERROR
                && _qualibus_exutum(ti)->genus != TYPUS_C89_ERROR)
            {
                _conversionem_annotare(sem, ni, ti, t);
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS:
        {
            /* operandum typatur SINE lapsu annotato - sizeof
             * naturalem metitur (acies manet acies). Typus nodi =
             * size_t (systema: unsigned long, LP64). */
            _valorem_typare(sem,
                silva_c89_magnitudo_expressionis_internum(nodus));
            {
                TypusC89* t = sem->primitivi[
                    PRIMITIVUM_LONGUS_INSIGNATUM];

                (vacuum)_typationem_ponere(sem, nodus, t);
                redde t;
            }
        }
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI:
        {
            /* species non expressio est; typus nodi = size_t */
            TypusC89* t = sem->primitivi[PRIMITIVUM_LONGUS_INSIGNATUM];

            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_SUBSCRIPTIO:
        {
            SilvaValor b_v = silva_c89_subscriptio_basis(nodus);
            SilvaValor i_v = silva_c89_subscriptio_index(nodus);
            constans SilvaNodus* nb;
            constans SilvaNodus* ni;
            TypusC89* tb;
            TypusC89* ti;
            TypusC89* pb;
            TypusC89* pi;
            TypusC89* t;

            si (b_v.genus != SILVA_VALOR_NODUS
                || i_v.genus != SILVA_VALOR_NODUS)
            {
                _valorem_typare(sem, b_v);
                _valorem_typare(sem, i_v);
                redde NIHIL;
            }
            nb = _canonicum(b_v.datum.nodus);
            ni = _canonicum(i_v.datum.nodus);
            tb = _expressionem_typare(sem, nb);
            ti = _expressionem_typare(sem, ni);
            si (tb == NIHIL || ti == NIHIL)
            {
                redde NIHIL;
            }
            si (_qualibus_exutum(tb)->genus == TYPUS_C89_ERROR
                || _qualibus_exutum(ti)->genus == TYPUS_C89_ERROR)
            {
                (vacuum)_typationem_ponere(sem, nodus,
                    sem->typus_erroris);
                redde sem->typus_erroris;
            }
            pb = _ut_monstrator(sem, tb);
            pi = _ut_monstrator(sem, ti);
            si (pb != NIHIL && _est_integrale(ti))
            {
                _conversionem_annotare(sem, nb, tb, pb);
                t = pb->datum.monstrator.internum;
            }
            alioquin si (pi != NIHIL && _est_integrale(tb))
            {
                /* i[a] - commutativum (C89) */
                _conversionem_annotare(sem, ni, ti, pi);
                t = pi->datum.monstrator.internum;
            }
            alioquin
            {
                silva_c89_diagnosticum_addere(sem, nodus,
                    EXAMEN_CODEX_SUBSCRIPTIO_NON_MONSTRATORIS);
                t = sem->typus_erroris;
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_VOCATIO:
        {
            SilvaValor functio_v = silva_c89_vocatio_functio(nodus);
            SilvaValor argumenta = silva_c89_vocatio_argumenta(nodus);
            TypusC89* tf = NIHIL;
            TypusC89* typus_functionis = NIHIL;
            TypusC89* t;
            b32 venenata = FALSUM;
            i32 larva_nul = ZEPHYRUM;   /* codex 59 */
            s32 positio_formati = -I;   /* codex 60 */
            s32 formati_numerus = -I;
            s32 formati_exspectata[FORMATI_ARGUMENTA_MAXIMA];
            character formati_conversiones[FORMATI_ARGUMENTA_MAXIMA];
            chorda titulus_vocati;

            titulus_vocati.mensura = ZEPHYRUM;
            titulus_vocati.datum = NIHIL;

            /* --- vocatus --- */
            si (functio_v.genus == SILVA_VALOR_NODUS)
            {
                constans SilvaNodus* nf = _canonicum(
                    functio_v.datum.nodus);

                si (nf->genus
                    == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
                {
                    SilvaValor tok_v =
                        silva_c89_folium_identificator_tok_valor(nf);

                    si (tok_v.genus == SILVA_VALOR_TOKEN)
                    {
                        SemanticaSymbolum* symbolum =
                            silva_c89_symbolum_invenire(sem,
                                tok_v.datum.token->valor);

                        titulus_vocati = tok_v.datum.token->valor;
                        larva_nul = _larva_lectoris_nul(
                            titulus_vocati);
                        positio_formati = _positio_formatoris(
                            titulus_vocati);

                        si (symbolum == NIHIL)
                        {
                            /* extern int implicitum (C89 6.3.2.2)
                             * - registratur IN SEDE VOCATIONIS cum
                             * est_implicitum (emendatio C: signum
                             * laminae exsecutionis, post-hoc
                             * irrecuperabile) */
                            TypusC89* implicitum =
                                silva_c89_typus_functio(sem,
                                    sem->primitivi[PRIMITIVUM_INTEGER],
                                    NIHIL, ZEPHYRUM, FALSUM, FALSUM);

                            symbolum = _symbolum_registrare(sem,
                                SYMBOLUM_FUNCTIO,
                                tok_v.datum.token->valor, implicitum,
                                ZEPHYRUM, REPOSITIO_EXTERNA, nf,
                                tok_v.datum.token);
                            si (symbolum != NIHIL)
                            {
                                symbolum->est_implicitum = VERUM;
                            }
                            tf = implicitum;
                        }
                        alioquin si (symbolum->genus
                            == (s32)SYMBOLUM_TYPEDEF)
                        {
                            si (!_intra_ambiguum(nf))
                            {
                                silva_c89_diagnosticum_addere(sem,
                                    nf, EXAMEN_CODEX_TYPEDEF_VOCATUM);
                            }
                            tf = sem->typus_erroris;
                        }
                        alioquin
                        {
                            tf = (symbolum->typus != NIHIL)
                                ? symbolum->typus
                                : sem->typus_erroris;
                        }
                        si (symbolum != NIHIL)
                        {
                            /* M1a A: etiam implicitus nectitur */
                            _nexum_ponere(sem, nf, symbolum);
                        }
                        (vacuum)_typationem_ponere(sem, nf, tf);
                    }
                }
                alioquin
                {
                    tf = _expressionem_typare(sem, nf);
                }
                /* functio aut monstrator ad functionem */
                si (tf != NIHIL)
                {
                    TypusC89* e = _qualibus_exutum(tf);

                    si (e->genus == TYPUS_C89_ERROR)
                    {
                        venenata = VERUM;
                    }
                    alioquin si (e->genus == TYPUS_C89_FUNCTIO)
                    {
                        typus_functionis = e;
                        /* designator laberetur (lapsus annotatus) */
                        _conversum_ponere(sem, nf,
                            silva_c89_typus_monstrator(sem, e));
                    }
                    alioquin si (e->genus == TYPUS_C89_MONSTRATOR)
                    {
                        TypusC89* intus = _qualibus_exutum(
                            e->datum.monstrator.internum);

                        si (intus->genus == TYPUS_C89_FUNCTIO)
                        {
                            typus_functionis = intus;
                        }
                    }
                    si (typus_functionis == NIHIL && !venenata)
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_VOCATUS_NON_FUNCTIO);
                        venenata = VERUM;
                    }
                }
            }

            /* --- argumenta (signa COMMA praetermissa) --- */
            {
                i32 i;
                i32 m = (i32)silva_valor_lista_numerus(argumenta);
                i32 a = ZEPHYRUM;   /* index argumenti */

                per (i = ZEPHYRUM; i < m; i++)
                {
                    SilvaValor* av = silva_valor_lista_obtinere(
                        argumenta, i);
                    constans SilvaNodus* na;
                    TypusC89* ta;

                    si (av == NIHIL || av->genus != SILVA_VALOR_NODUS)
                    {
                        perge;
                    }
                    na = _canonicum(av->datum.nodus);
                    ta = _expressionem_typare(sem, na);
                    /* codex 59: chorda.datum ad lectorem NUL sine
                     * limite - solum positiones larvae, sedes domus */
                    si (larva_nul != ZEPHYRUM
                        && a < XXXII
                        && (larva_nul & (I << a)) != ZEPHYRUM
                        && !sem->in_systemate
                        && !_fons_alienus(sem, na)
                        && _est_datum_chordae(sem, na)
                        && !_tolera_absorbere(sem, na,
                               (s32)EXAMEN_CODEX_CHORDA_NUDA))
                    {
                        memoriae_index capacitas =
                            (memoriae_index)titulus_vocati.mensura
                            + (memoriae_index)LXIV;
                        character* nuntius =
                            (character*)piscina_allocare(
                                sem->piscina, capacitas);

                        si (nuntius != NIHIL)
                        {
                            sprintf(nuntius,
                                "datum chordae sine fine NUL"
                                " ad '%.*s'",
                                (int)titulus_vocati.mensura,
                                (constans character*)
                                    titulus_vocati.datum);
                            _diagnosticum_addere_plenum(sem, na,
                                (s32)EXAMEN_CODEX_CHORDA_NUDA,
                                NIHIL, nuntius);
                        }
                    }
                    /* codex 60: charta formati aedificata in sede
                     * formati, argumenta variadica contra eam
                     * iudicata */
                    si (positio_formati >= ZEPHYRUM
                        && (s32)a == positio_formati
                        && na->genus
                            == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
                    {
                        formati_numerus = _formati_exspectata(na,
                            formati_exspectata,
                            formati_conversiones);
                    }
                    alioquin si (positio_formati >= ZEPHYRUM
                        && formati_numerus > ZEPHYRUM
                        && (s32)a > positio_formati
                        && (s32)a - positio_formati - I
                            < formati_numerus
                        && !sem->in_systemate
                        && !_fons_alienus(sem, na))
                    {
                        s32 idx = (s32)a - positio_formati - I;

                        _formatum_iudicare(sem, na, ta,
                            formati_exspectata[idx],
                            formati_conversiones[idx]);
                    }
                    si (ta != NIHIL && typus_functionis != NIHIL)
                    {
                        si (typus_functionis
                                ->datum.functio.est_prototypata
                            && a < typus_functionis
                                ->datum.functio.numerus_parametrorum)
                        {
                            /* conversio ad parametrum */
                            _ad_finem_annotare(sem, na, ta,
                                typus_functionis
                                    ->datum.functio.parametra[a]);
                        }
                        alioquin
                        {
                            /* promotiones argumentorum ordinariae
                             * (variadica extra / non prototypata):
                             * fluitans->duplex, integralia parva
                             * ->int, acies/functio labuntur */
                            TypusC89* e = _qualibus_exutum(ta);
                            TypusC89* finis = NIHIL;

                            si (e->genus == TYPUS_C89_PRIMITIVUS
                                && e->datum.primitivum
                                    == (s32)PRIMITIVUM_FLUITANS)
                            {
                                finis = sem->primitivi[
                                    PRIMITIVUM_DUPLEX];
                            }
                            alioquin si (_est_integrale(ta))
                            {
                                finis = _promotum(sem, ta);
                            }
                            alioquin
                            {
                                finis = _lapsus(sem, ta);
                            }
                            si (finis != NIHIL)
                            {
                                _conversionem_annotare(sem, na, ta,
                                    finis);
                            }
                        }
                    }
                    a++;
                }
                /* aritas (chunk C): prototypatae solum;
                 * variadica = minimum partis fixae;
                 * implicitae/K&R exemptae */
                si (typus_functionis != NIHIL
                    && typus_functionis
                        ->datum.functio.est_prototypata)
                {
                    i32 postulata = typus_functionis
                        ->datum.functio.numerus_parametrorum;

                    si (typus_functionis
                            ->datum.functio.est_variadica
                            ? (a < postulata) : (a != postulata))
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_NUMERUS_ARGUMENTORUM);
                    }
                }
                (vacuum)a;
            }
            t = (typus_functionis != NIHIL)
                ? typus_functionis->datum.functio.reditus
                : sem->typus_erroris;
            si (typus_functionis == NIHIL && !venenata && tf == NIHIL)
            {
                redde NIHIL;   /* vocatus nondum typabilis */
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_ACCESSUS:
        {
            SilvaValor b_v = silva_c89_accessus_basis(nodus);
            SilvaValor op_v = silva_c89_accessus_tok_operator(nodus);
            SilvaValor tit_v = silva_c89_accessus_tok_titulus(nodus);
            constans SilvaNodus* nb;
            TypusC89* tb;
            TypusC89* tag_typus = NIHIL;
            i32 quales_basis = ZEPHYRUM;
            TypusC89* t = NIHIL;

            si (b_v.genus != SILVA_VALOR_NODUS
                || op_v.genus != SILVA_VALOR_TOKEN
                || tit_v.genus != SILVA_VALOR_TOKEN)
            {
                _valorem_typare(sem, b_v);
                redde NIHIL;
            }
            nb = _canonicum(b_v.datum.nodus);
            tb = _expressionem_typare(sem, nb);
            si (tb == NIHIL)
            {
                redde NIHIL;
            }
            si (_qualibus_exutum(tb)->genus == TYPUS_C89_ERROR)
            {
                (vacuum)_typationem_ponere(sem, nodus,
                    sem->typus_erroris);
                redde sem->typus_erroris;
            }
            si (op_v.datum.token->genus == SILVA_LEX_SAGITTA)
            {
                TypusC89* pb = _ut_monstrator(sem, tb);

                si (pb == NIHIL)
                {
                    silva_c89_diagnosticum_addere(sem, nodus,
                        EXAMEN_CODEX_SAGITTA_NON_MONSTRATORIS);
                    t = sem->typus_erroris;
                }
                alioquin
                {
                    TypusC89* intus = pb->datum.monstrator.internum;

                    quales_basis = _quales_typi(intus);
                    tag_typus = _qualibus_exutum(intus);
                }
            }
            alioquin
            {
                quales_basis = _quales_typi(tb);
                tag_typus = _qualibus_exutum(tb);
            }
            si (t == NIHIL)
            {
                si (tag_typus->genus != TYPUS_C89_STRUCTURA
                    && tag_typus->genus != TYPUS_C89_UNIO)
                {
                    silva_c89_diagnosticum_addere(sem, nodus,
                        EXAMEN_CODEX_ACCESSUS_NON_STRUCTURAE);
                    t = sem->typus_erroris;
                }
                alioquin si (!tag_typus->datum.tag.completa)
                {
                    silva_c89_diagnosticum_addere(sem, nodus,
                        EXAMEN_CODEX_ACCESSUS_INCOMPLETAE);
                    t = sem->typus_erroris;
                }
                alioquin
                {
                    /* membrum per nomen (signum crudum) */
                    chorda quaesitum = tit_v.datum.token->valor;
                    i32 k;
                    TypusC89Membrum* inventum = NIHIL;

                    per (k = ZEPHYRUM;
                         k < (i32)tag_typus
                             ->datum.tag.numerus_membrorum; k++)
                    {
                        TypusC89Membrum* membrum =
                            &tag_typus->datum.tag.membra[k];

                        si (membrum->titulus.mensura
                                == quaesitum.mensura
                            && memcmp(membrum->titulus.datum,
                                   quaesitum.datum,
                                   (memoriae_index)
                                       quaesitum.mensura)
                                == ZEPHYRUM)
                        {
                            inventum = membrum;
                            frange;
                        }
                    }
                    si (inventum == NIHIL)
                    {
                        silva_c89_diagnosticum_addere(sem, nodus,
                            EXAMEN_CODEX_MEMBRUM_IGNOTUM);
                        t = sem->typus_erroris;
                    }
                    alioquin
                    {
                        /* quales basis in membrum PROPAGANTUR
                         * (constans structura -> membra constantia) */
                        t = inventum->typus;
                        si (quales_basis != ZEPHYRUM
                            && t->genus != TYPUS_C89_ERROR)
                        {
                            t = silva_c89_typus_qualificatus(sem, t,
                                quales_basis);
                        }
                    }
                }
            }
            (vacuum)_typationem_ponere(sem, nodus, t);
            redde t;
        }
        casus (s32)SILVA_C89_GENUS_CONGERIES:
        {
            /* typus scopi (deorsum fluens) = Chunk C (DECISUS 6);
             * elementa hic typantur */
            SilvaValor elementa = silva_c89_congeries_elementa(nodus);
            i32 i;
            i32 m = (i32)silva_valor_lista_numerus(elementa);

            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* e = silva_valor_lista_obtinere(
                    elementa, i);

                si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
                {
                    (vacuum)_expressionem_typare(sem, e->datum.nodus);
                }
            }
            redde NIHIL;
        }
        ordinarius:
            redde NIHIL;   /* ERROR, genera ignota: sine strepitu */
    }
}

interior vacuum
_valorem_typare (SilvaSemantica* sem, SilvaValor v)
{
    si (v.genus == SILVA_VALOR_NODUS)
    {
        (vacuum)_expressionem_typare(sem, v.datum.nodus);
    }
}

/* Scopus scalaris (conditiones si/dum/per...): lapsus aciei/
 * functionis in conversum annotatur */
interior vacuum
_valorem_typare_scalarem (SilvaSemantica* sem, SilvaValor v)
{
    TypusC89* t;
    TypusC89* lapsum;

    si (v.genus != SILVA_VALOR_NODUS)
    {
        redde;
    }
    t = _expressionem_typare(sem, v.datum.nodus);
    si (t == NIHIL)
    {
        redde;
    }
    /* sedes scalaris (chunk C): condicio arithmetica aut
     * monstrator post lapsum (C89 6.6.4.1/6.6.5) */
    {
        TypusC89* n = _qualibus_exutum(t);

        si (n != NIHIL && n->genus != TYPUS_C89_ERROR
            && !_est_arithmeticum(t)
            && _ut_monstrator(sem, t) == NIHIL)
        {
            silva_c89_diagnosticum_addere(sem,
                _canonicum(v.datum.nodus),
                EXAMEN_CODEX_CONDICIO_NON_SCALARIS);
        }
    }
    lapsum = _lapsus(sem, t);
    si (lapsum != NIHIL)
    {
        _conversum_ponere(sem, _canonicum(v.datum.nodus), lapsum);
    }
}

interior b32
_est_acies_characterum (TypusC89* t)
{
    TypusC89* e;

    t = _qualibus_exutum(t);
    si (t == NIHIL || t->genus != TYPUS_C89_ACIES)
    {
        redde FALSUM;
    }
    e = _qualibus_exutum(t->datum.acies.elementum);
    si (e == NIHIL || e->genus != TYPUS_C89_PRIMITIVUS)
    {
        redde FALSUM;
    }
    commutatio (e->datum.primitivum)
    {
        casus PRIMITIVUM_CHARACTER:
        casus PRIMITIVUM_CHARACTER_SIGNATUM:
        casus PRIMITIVUM_CHARACTER_INSIGNATUM:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* Congeries: typus DEORSUM fluit (DECISUS 6) - typus scopi ex
 * declaratore/membro; correspondentia directa (uncis plenis)
 * typatur; ELISIO uncorum (elementum scalare ubi aggregatum
 * exspectatur) = diagnosticum nominatum = MENSURA corporis
 * (ambulatio plena elisionis parcata -> M1 demissio). Chorda
 * litteralis aciem characterum initians legalis, non elisio. */
interior vacuum
_congeriem_typare (SilvaSemantica* sem, constans SilvaNodus* congeries,
    TypusC89* scopus_typus)
{
    TypusC89* t = _qualibus_exutum(scopus_typus);
    SilvaValor elementa = silva_c89_congeries_elementa(congeries);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(elementa);
    i32 a = ZEPHYRUM;   /* index elementi (nodi soli) */

    si (t == NIHIL || t->genus == TYPUS_C89_ERROR)
    {
        /* scopus venenatus: elementa saltem typantur */
        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* e = silva_valor_lista_obtinere(elementa, i);

            si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
            {
                (vacuum)_expressionem_typare(sem, e->datum.nodus);
            }
        }
        redde;
    }
    (vacuum)_typationem_ponere(sem, congeries, t);
    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* ev = silva_valor_lista_obtinere(elementa, i);
        constans SilvaNodus* ne;
        TypusC89* finis = NIHIL;

        si (ev == NIHIL || ev->genus != SILVA_VALOR_NODUS)
        {
            perge;   /* virgulae intersertae */
        }
        ne = _canonicum(ev->datum.nodus);

        si (t->genus == TYPUS_C89_ACIES)
        {
            finis = t->datum.acies.elementum;
        }
        alioquin si (t->genus == TYPUS_C89_STRUCTURA)
        {
            finis = (a < (i32)t->datum.tag.numerus_membrorum)
                ? t->datum.tag.membra[a].typus : NIHIL;
        }
        alioquin si (t->genus == TYPUS_C89_UNIO)
        {
            finis = (a == ZEPHYRUM
                     && t->datum.tag.numerus_membrorum > ZEPHYRUM)
                ? t->datum.tag.membra[ZEPHYRUM].typus : NIHIL;
        }
        alioquin
        {
            finis = t;   /* scalaris: int x = {5} - C89 legale */
        }

        si (ne->genus == (s32)SILVA_C89_GENUS_CONGERIES)
        {
            si (finis != NIHIL)
            {
                _congeriem_typare(sem, ne, finis);
            }
            alioquin
            {
                (vacuum)_expressionem_typare(sem, ne);
            }
        }
        alioquin
        {
            TypusC89* te = _expressionem_typare(sem, ne);
            TypusC89* ef = (finis != NIHIL)
                ? _qualibus_exutum(finis) : NIHIL;

            si (ef != NIHIL
                && (ef->genus == TYPUS_C89_ACIES
                    || ef->genus == TYPUS_C89_STRUCTURA
                    || ef->genus == TYPUS_C89_UNIO))
            {
                si (ne->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA
                    && _est_acies_characterum(ef))
                {
                    /* chorda aciem characterum initians - legalis */
                }
                alioquin
                {
                    /* aggregatum exspectatum, scalare datum -
                     * MENSURA elisionis (parca nominata) */
                    silva_c89_diagnosticum_addere(sem, ne,
                        EXAMEN_CODEX_ELISIO_UNCORUM);
                }
            }
            alioquin si (te != NIHIL && finis != NIHIL)
            {
                _ad_finem_annotare(sem, ne, te, finis);
            }
        }
        a++;
    }
}

constans TypusC89*
silva_c89_typus_expressionis (constans SilvaSemantica* sem,
    constans SilvaNodus* nodus)
{
    SemanticaTypatio* t;

    si (sem == NIHIL || nodus == NIHIL)
    {
        redde NIHIL;
    }
    t = _typationem_invenire(sem, _canonicum(nodus));
    redde (t != NIHIL) ? t->naturalis : NIHIL;
}

constans TypusC89*
silva_c89_conversio_expressionis (constans SilvaSemantica* sem,
    constans SilvaNodus* nodus)
{
    SemanticaTypatio* t;

    si (sem == NIHIL || nodus == NIHIL)
    {
        redde NIHIL;
    }
    t = _typationem_invenire(sem, _canonicum(nodus));
    redde (t != NIHIL) ? t->conversus : NIHIL;
}

i32
silva_c89_typationes_numerus (constans SilvaSemantica* sem)
{
    si (sem == NIHIL || sem->typationes == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde tabula_dispersa_numerus(sem->typationes);
}

/* ==================================================
 * Exporta demissionis (M1a Chunk A)
 * ================================================== */

constans SemanticaSymbolum*
silva_c89_symbolum_nodi (constans SilvaSemantica* sem,
    constans SilvaNodus* nodus)
{
    SemanticaNexus* n;

    si (sem == NIHIL || sem->nexus == NIHIL || nodus == NIHIL)
    {
        redde NIHIL;
    }
    n = _nexum_invenire(sem, _canonicum(nodus));
    redde (n != NIHIL) ? n->symbolum : NIHIL;
}

i32
silva_c89_nexus_numerus (constans SilvaSemantica* sem)
{
    si (sem == NIHIL || sem->nexus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde tabula_dispersa_numerus(sem->nexus);
}

b32
silva_c89_chorda_decodere (Piscina* piscina,
    constans SilvaNodus* nodus, chorda* octeti_out)
{
    SilvaValor fragmenta;
    i32 i;
    i32 m;
    i32 summa_superior = ZEPHYRUM;
    i8* datum;
    i32 scriptum = ZEPHYRUM;

    si (piscina == NIHIL || nodus == NIHIL || octeti_out == NIHIL)
    {
        redde FALSUM;
    }
    nodus = _canonicum(nodus);
    si (nodus->genus != (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
    {
        redde FALSUM;
    }
    fragmenta = silva_c89_folium_chorda_tok_valor(nodus);
    m = (i32)silva_valor_lista_numerus(fragmenta);

    /* mensura superior: decodatum numquam fonte longius */
    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(fragmenta, i);

        si (v != NIHIL && v->genus == SILVA_VALOR_TOKEN)
        {
            summa_superior += v->datum.token->valor.mensura;
        }
    }
    datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(summa_superior > ZEPHYRUM
                         ? summa_superior : I));
    si (datum == NIHIL)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(fragmenta, i);
        constans SilvaToken* tok;
        i32 cursor;
        i32 finis;

        si (v == NIHIL || v->genus != SILVA_VALOR_TOKEN)
        {
            perge;
        }
        tok = v->datum.token;
        si (tok->valor.mensura < II)
        {
            redde FALSUM;
        }
        si (tok->valor.datum[ZEPHYRUM] == 'L')
        {
            redde FALSUM;   /* chorda lata - parca nominata */
        }
        cursor = I;
        finis = tok->valor.mensura - I;   /* " claudens */
        dum (cursor < finis)
        {
            si ((character)tok->valor.datum[cursor] == '\\')
            {
                s64 valor = ZEPHYRUM;

                cursor++;
                si (!_fugam_decodere(&tok->valor, &cursor, &valor))
                {
                    redde FALSUM;
                }
                datum[scriptum] = (i8)(valor & 0xff);
            }
            alioquin
            {
                datum[scriptum] = tok->valor.datum[cursor];
                cursor++;
            }
            scriptum++;
        }
    }
    octeti_out->datum = datum;
    octeti_out->mensura = scriptum;
    redde VERUM;
}
