/* silva_c89_semantica.c - Semantica C89: typi + forma + aestimator
 * (M0a Chunk A - vide silva_c89_semantica.h + consilium) */

#include "silva_c89_semantica.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_oraculum.h"
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

vacuum
silva_c89_diagnosticum_addere (
    SilvaSemantica*      sem,
    constans SilvaNodus* nodus,
    constans character*  causa)
{
    SemanticaDiagnosticum* d;

    si (sem == NIHIL)
    {
        redde;
    }
    d = (SemanticaDiagnosticum*)xar_addere(sem->diagnostica);
    si (d != NIHIL)
    {
        d->nodus = nodus;
        d->causa = causa;
    }
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
                titulus, &prior)
            && ((SemanticaSymbolum*)prior)->genus != genus)
        {
            silva_c89_diagnosticum_addere(sem, declarans,
                "redeclaratio generis diversi eodem scopo");
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
    symbolum->declarans = declarans;
    symbolum->lexema = lexema;
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
    sem->scopus_summus = _scopum_creare(piscina, NIHIL);
    sem->scopus_currens = sem->scopus_summus;
    si (sem->derivati == NIHIL || sem->symbola == NIHIL
        || sem->diagnostica == NIHIL || sem->scopus_summus == NIHIL)
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

b32
silva_c89_typedef_registrare (SilvaSemantica* sem, chorda titulus,
    TypusC89* typus)
{
    si (sem == NIHIL || typus == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
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
                "tag sine titulo sine corpore");
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
                "enumeratio sine titulo sine corpore");
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
                        "valor enumeratoris non constans");
                }
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
                            "typus nominatus ignotus");
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
                    "nodus erroris in specificatoribus");
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
        silva_c89_diagnosticum_addere(sem, NIHIL,
            "specificatores primitivi et nominati mixti");
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
            silva_c89_diagnosticum_addere(sem, NIHIL,
                "multiset primitivorum invalidum");
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
                            "mensura aciei non constans positiva");
                    }
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
                    "nodus erroris in declaratore");
                redde sem->typus_erroris;
            ordinarius:
                silva_c89_diagnosticum_addere(sem, declarator,
                    "genus declaratoris inexspectatum");
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
            "species-typi exspectata");
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
        si (tok != NIHIL)
        {
            s32 genus_symboli;

            si (est_typedef)
            {
                genus_symboli = SYMBOLUM_TYPEDEF;
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

interior vacuum
_definitionem_ambulare (SilvaSemantica* sem,
    constans SilvaNodus* definitio)
{
    b32 est_typedef = FALSUM;
    i32 repositio = ZEPHYRUM;
    TypusC89* basis;
    TypusC89* t;
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
            "declarationes K&R - parca nominata");
    }
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
    }
    _scopum_claudere(sem);
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
            _nodalem_ambulare(sem, silva_c89_si_consequens(nodus));
            _nodalem_ambulare(sem, silva_c89_si_alioquin(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_DUM:
            _nodalem_ambulare(sem, silva_c89_dum_corpus(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_FAC_DUM:
            _nodalem_ambulare(sem, silva_c89_fac_dum_corpus(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_PER:
            _nodalem_ambulare(sem, silva_c89_per_corpus(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_COMMUTATIO:
            _nodalem_ambulare(sem, silva_c89_commutatio_corpus(
                nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_CASUS:
            _listam_ambulare(sem, silva_c89_casus_sententiae(nodus));
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
        /* RAMUS_OMISSUS (cruda), ERROR, sententiae expressionum:
         * praetermissi - nihil declarant */
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
        _listam_ambulare(sem, systema->commissio->radix);
        sem->in_systemate = FALSUM;
    }
    _listam_ambulare(sem, parsura->commissio->radix);
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
        silva_c89_diagnosticum_addere(sem, NIHIL,
            "cyclus formae (structura se continens)");
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
                silva_c89_diagnosticum_addere(sem, NIHIL,
                    "forma aciei incompletae");
            }
            frange;
        casus TYPUS_C89_FUNCTIO:
            silva_c89_diagnosticum_addere(sem, NIHIL,
                "forma functionis petita");
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
                    "campi (bitfields) - forma parcata");
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

interior b32
_folium_integer_aestimare (constans SilvaNodus* nodus, s64* valor_out)
{
    SilvaValor tok_v = silva_c89_folium_integer_tok_valor(nodus);
    constans SilvaToken* tok;
    s64 valor = ZEPHYRUM;
    i32 i = ZEPHYRUM;
    i32 basis = X;

    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    tok = tok_v.datum.token;
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
    per (; i < (i32)tok->valor.mensura; i++)
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
            frange;   /* suffixa */
        }
        alioquin
        {
            redde FALSUM;
        }
        valor = valor * (s64)basis + cifra;
    }
    *valor_out = valor;
    redde VERUM;
}

interior b32
_folium_character_aestimare (constans SilvaNodus* nodus,
    s64* valor_out)
{
    SilvaValor tok_v = silva_c89_folium_character_tok_valor(nodus);
    constans SilvaToken* tok;
    character c;

    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    tok = tok_v.datum.token;
    si (tok->valor.mensura < III)   /* 'x' minimum */
    {
        redde FALSUM;
    }
    c = (character)tok->valor.datum[I];
    si (c == '\\' && tok->valor.mensura >= IV)
    {
        character e = (character)tok->valor.datum[II];

        commutatio (e)
        {
            casus 'n':  c = '\n'; frange;
            casus 't':  c = '\t'; frange;
            casus 'r':  c = '\r'; frange;
            casus '0':  c = '\0'; frange;
            casus 'a':  c = '\a'; frange;
            casus 'b':  c = '\b'; frange;
            casus 'f':  c = '\f'; frange;
            casus 'v':  c = '\v'; frange;
            casus '\\': c = '\\'; frange;
            casus '\'': c = '\''; frange;
            casus '"':  c = '"';  frange;
            ordinarius: redde FALSUM;   /* \x.. \NNN - parca */
        }
    }
    *valor_out = (s64)(signatus character)c;
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
            /* typum expressionis postulat = M0b; parca nominata */
            silva_c89_diagnosticum_addere(sem, expressio,
                "magnitudo expressionis in constante - parca M0b");
            redde FALSUM;
        ordinarius:
            redde FALSUM;
    }
}
