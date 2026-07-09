/* officina_medulla.c - Medulla: IR registrorum linearis (M1a B)
 * Vide officina_medulla.h pro figura; vocabularium-medullae.md
 * pro nominibus signatis. */

#include "officina_medulla.h"

#include <stdio.h>
#include <string.h>

/* ==================================================
 * Auxilia interna
 * ================================================== */

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
    redde memcmp(a.datum, b.datum,
        (memoriae_index)a.mensura) == ZEPHYRUM;
}

/* ==================================================
 * Modulus + symbola + causae
 * ================================================== */

MedullaModulus*
medulla_modulum_creare (Piscina* piscina, chorda titulus)
{
    MedullaModulus* modulus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    modulus = (MedullaModulus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(MedullaModulus));
    si (modulus == NIHIL)
    {
        redde NIHIL;
    }
    memset(modulus, ZEPHYRUM, magnitudo(MedullaModulus));
    modulus->piscina = piscina;
    modulus->titulus = chorda_transcribere(titulus, piscina);
    modulus->functiones = xar_creare(piscina,
        (i32)magnitudo(MedullaFunctio*));
    modulus->data = xar_creare(piscina,
        (i32)magnitudo(MedullaDatum*));
    modulus->symbola = xar_creare(piscina,
        (i32)magnitudo(MedullaSymbolum));
    modulus->causae = xar_creare(piscina, (i32)magnitudo(chorda));
    si (modulus->functiones == NIHIL || modulus->data == NIHIL
        || modulus->symbola == NIHIL || modulus->causae == NIHIL)
    {
        redde NIHIL;
    }
    redde modulus;
}

s32
medulla_symbolum_internare (MedullaModulus* modulus, chorda titulus)
{
    i32 i;
    i32 m;
    MedullaSymbolum* symbolum;

    si (modulus == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde -I;
    }
    m = xar_numerus(modulus->symbola);
    per (i = ZEPHYRUM; i < m; i++)
    {
        symbolum = (MedullaSymbolum*)xar_obtinere(modulus->symbola,
            i);

        si (symbolum != NIHIL
            && _chordae_aequales(symbolum->titulus, titulus))
        {
            redde (s32)i;
        }
    }
    symbolum = (MedullaSymbolum*)xar_addere(modulus->symbola);
    si (symbolum == NIHIL)
    {
        redde -I;
    }
    symbolum->titulus = chorda_transcribere(titulus,
        modulus->piscina);
    symbolum->genus = MEDULLA_SYMBOLUM_EXTERNUM;
    symbolum->index = -I;
    redde (s32)(xar_numerus(modulus->symbola) - I);
}

constans MedullaSymbolum*
medulla_symbolum_obtinere (constans MedullaModulus* modulus,
    s32 index)
{
    si (modulus == NIHIL)
    {
        redde NIHIL;
    }
    redde (constans MedullaSymbolum*)xar_obtinere_s(
        modulus->symbola, index);
}

s32
medulla_causam_internare (MedullaModulus* modulus, chorda causa)
{
    i32 i;
    i32 m;
    chorda* sedes;

    si (modulus == NIHIL)
    {
        redde -I;
    }
    m = xar_numerus(modulus->causae);
    per (i = ZEPHYRUM; i < m; i++)
    {
        sedes = (chorda*)xar_obtinere(modulus->causae, i);

        si (sedes != NIHIL && _chordae_aequales(*sedes, causa))
        {
            redde (s32)i;
        }
    }
    sedes = (chorda*)xar_addere(modulus->causae);
    si (sedes == NIHIL)
    {
        redde -I;
    }
    *sedes = chorda_transcribere(causa, modulus->piscina);
    redde (s32)(xar_numerus(modulus->causae) - I);
}

constans chorda*
medulla_causam_obtinere (constans MedullaModulus* modulus, s32 index)
{
    si (modulus == NIHIL)
    {
        redde NIHIL;
    }
    redde (constans chorda*)xar_obtinere_s(modulus->causae, index);
}

/* ==================================================
 * Functio + registra + blocci
 * ================================================== */

MedullaFunctio*
medulla_functionem_creare (MedullaModulus* modulus, chorda titulus,
    s32 typus_reditus, b32 est_variadica)
{
    MedullaFunctio*  functio;
    MedullaFunctio** locus_functionis;
    MedullaSymbolum* symbolum;
    s32 index_symboli;

    si (modulus == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    index_symboli = medulla_symbolum_internare(modulus, titulus);
    si (index_symboli < ZEPHYRUM)
    {
        redde NIHIL;
    }
    symbolum = (MedullaSymbolum*)xar_obtinere_s(modulus->symbola,
        index_symboli);
    si (symbolum->genus != (s32)MEDULLA_SYMBOLUM_EXTERNUM)
    {
        redde NIHIL;   /* definitio duplex */
    }

    functio = (MedullaFunctio*)piscina_allocare(modulus->piscina,
        (memoriae_index)magnitudo(MedullaFunctio));
    si (functio == NIHIL)
    {
        redde NIHIL;
    }
    memset(functio, ZEPHYRUM, magnitudo(MedullaFunctio));
    functio->titulus = symbolum->titulus;   /* copia internata */
    functio->typus_reditus = typus_reditus;
    functio->est_variadica = est_variadica;
    functio->parametra = xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaParametrum));
    functio->blocci = xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaBloccus));
    functio->registra = xar_creare(modulus->piscina,
        (i32)magnitudo(chorda));
    functio->operanda = xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaOperandum));
    si (functio->parametra == NIHIL || functio->blocci == NIHIL
        || functio->registra == NIHIL || functio->operanda == NIHIL)
    {
        redde NIHIL;
    }

    locus_functionis = (MedullaFunctio**)xar_addere(
        modulus->functiones);
    si (locus_functionis == NIHIL)
    {
        redde NIHIL;
    }
    *locus_functionis = functio;
    symbolum->genus = MEDULLA_SYMBOLUM_FUNCTIO;
    symbolum->index = (s32)(xar_numerus(modulus->functiones) - I);
    redde functio;
}

s32
medulla_registrum_novum (MedullaFunctio* functio, chorda titulus)
{
    chorda* sedes;
    s32 index;

    si (functio == NIHIL)
    {
        redde -I;
    }
    index = (s32)xar_numerus(functio->registra);
    sedes = (chorda*)xar_addere(functio->registra);
    si (sedes == NIHIL)
    {
        redde -I;
    }
    si (titulus.mensura == ZEPHYRUM)
    {
        /* nomen automaticum tN (N = index) */
        character littera[XXXII];
        chorda temporarium;
        s32 longitudo = (s32)sprintf(littera, "t%d", (int)index);

        temporarium.datum = (i8*)littera;
        temporarium.mensura = (i32)longitudo;
        *sedes = chorda_transcribere(temporarium,
            functio->registra->piscina);
    }
    alioquin
    {
        *sedes = chorda_transcribere(titulus,
            functio->registra->piscina);
    }
    redde index;
}

s32
medulla_parametrum_addere (MedullaFunctio* functio, chorda titulus,
    s32 typus)
{
    MedullaParametrum* parametrum;
    s32 index;

    si (functio == NIHIL)
    {
        redde -I;
    }
    index = medulla_registrum_novum(functio, titulus);
    si (index < ZEPHYRUM)
    {
        redde -I;
    }
    parametrum = (MedullaParametrum*)xar_addere(functio->parametra);
    si (parametrum == NIHIL)
    {
        redde -I;
    }
    parametrum->index = index;
    parametrum->typus = typus;
    redde index;
}

constans chorda*
medulla_registrum_titulus (constans MedullaFunctio* functio,
    s32 index)
{
    si (functio == NIHIL)
    {
        redde NIHIL;
    }
    redde (constans chorda*)xar_obtinere_s(functio->registra, index);
}

s32
medulla_bloccum_creare (MedullaFunctio* functio, chorda titulus)
{
    MedullaBloccus* bloccus;

    si (functio == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde -I;
    }
    bloccus = (MedullaBloccus*)xar_addere(functio->blocci);
    si (bloccus == NIHIL)
    {
        redde -I;
    }
    bloccus->titulus = chorda_transcribere(titulus,
        functio->blocci->piscina);
    bloccus->instructiones = xar_creare(functio->blocci->piscina,
        (i32)magnitudo(MedullaInstructio));
    bloccus->terminatus = FALSUM;
    si (bloccus->instructiones == NIHIL)
    {
        redde -I;
    }
    redde (s32)(xar_numerus(functio->blocci) - I);
}

MedullaBloccus*
medulla_bloccum_obtinere (constans MedullaFunctio* functio,
    s32 index)
{
    si (functio == NIHIL)
    {
        redde NIHIL;
    }
    redde (MedullaBloccus*)xar_obtinere_s(functio->blocci, index);
}

s32
medulla_operanda_addere (MedullaFunctio* functio,
    constans MedullaOperandum* operanda, s32 numerus)
{
    s32 initium;
    s32 i;

    si (functio == NIHIL || numerus < ZEPHYRUM
        || (numerus > ZEPHYRUM && operanda == NIHIL))
    {
        redde -I;
    }
    initium = (s32)xar_numerus(functio->operanda);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        MedullaOperandum* sedes =
            (MedullaOperandum*)xar_addere(functio->operanda);

        si (sedes == NIHIL)
        {
            redde -I;
        }
        *sedes = operanda[i];
    }
    redde initium;
}

/* ==================================================
 * Fabricae operandorum
 * ================================================== */

interior MedullaOperandum
_operandum (s32 genus)
{
    MedullaOperandum operandum;

    memset(&operandum, ZEPHYRUM, magnitudo(MedullaOperandum));
    operandum.genus = genus;
    redde operandum;
}

MedullaOperandum
medulla_op_nihil (vacuum)
{
    redde _operandum(MEDULLA_OPERANDUM_NIHIL);
}

MedullaOperandum
medulla_op_registrum (s32 index)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_REGISTRUM);

    operandum.datum.index = index;
    redde operandum;
}

MedullaOperandum
medulla_op_immediatum (s64 valor)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_IMMEDIATUM);

    operandum.datum.immediatum = valor;
    redde operandum;
}

MedullaOperandum
medulla_op_immediatum_f (f64 valor)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_IMMEDIATUM_F);

    operandum.datum.immediatum_f = valor;
    redde operandum;
}

MedullaOperandum
medulla_op_symbolum (s32 index)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_SYMBOLUM);

    operandum.datum.index = index;
    redde operandum;
}

MedullaOperandum
medulla_op_bloccum (s32 index)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_BLOCCUS);

    operandum.datum.index = index;
    redde operandum;
}

MedullaOperandum
medulla_op_causa (s32 index)
{
    MedullaOperandum operandum = _operandum(MEDULLA_OPERANDUM_CAUSA);

    operandum.datum.index = index;
    redde operandum;
}

/* ==================================================
 * Emissio + disciplina terminatorum
 * ================================================== */

b32
medulla_op_terminator (s32 op)
{
    redde op == (s32)MEDULLA_OP_SALIRE
        || op == (s32)MEDULLA_OP_RAMUS
        || op == (s32)MEDULLA_OP_REDDE
        || op == (s32)MEDULLA_OP_SISTERE;
}

b32
medulla_emittere (MedullaFunctio* functio, s32 bloccus,
    constans MedullaInstructio* instructio)
{
    MedullaBloccus*    sedes_blocci;
    MedullaInstructio* sedes;

    si (functio == NIHIL || instructio == NIHIL)
    {
        redde FALSUM;
    }
    si (instructio->op < ZEPHYRUM
        || instructio->op >= (s32)MEDULLA_OP_NUMERUS)
    {
        redde FALSUM;
    }
    si (instructio->destinatio >= ZEPHYRUM
        && instructio->destinatio
            >= (s32)xar_numerus(functio->registra))
    {
        redde FALSUM;   /* registrum ignotum */
    }
    sedes_blocci = (MedullaBloccus*)xar_obtinere_s(functio->blocci,
        bloccus);
    si (sedes_blocci == NIHIL || sedes_blocci->terminatus)
    {
        redde FALSUM;
    }
    sedes = (MedullaInstructio*)xar_addere(
        sedes_blocci->instructiones);
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    *sedes = *instructio;
    si (medulla_op_terminator(instructio->op))
    {
        sedes_blocci->terminatus = VERUM;
    }
    redde VERUM;
}

b32
medulla_functio_terminata (constans MedullaFunctio* functio)
{
    i32 i;
    i32 m;

    si (functio == NIHIL)
    {
        redde FALSUM;
    }
    m = xar_numerus(functio->blocci);
    si (m == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)xar_obtinere(functio->blocci,
                i);

        si (bloccus == NIHIL || !bloccus->terminatus)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* ==================================================
 * Datum + relocationes (C8: imago + relocationes a die primo)
 * ================================================== */

MedullaDatum*
medulla_datum_creare (MedullaModulus* modulus, chorda titulus,
    i32 magnitudo_octetorum, i32 ordinatio)
{
    MedullaDatum*    datum;
    MedullaDatum**   locus_dati;
    MedullaSymbolum* symbolum;
    s32 index_symboli;

    si (modulus == NIHIL || titulus.mensura == ZEPHYRUM
        || magnitudo_octetorum < I || ordinatio < I)
    {
        redde NIHIL;
    }
    index_symboli = medulla_symbolum_internare(modulus, titulus);
    si (index_symboli < ZEPHYRUM)
    {
        redde NIHIL;
    }
    symbolum = (MedullaSymbolum*)xar_obtinere_s(modulus->symbola,
        index_symboli);
    si (symbolum->genus != (s32)MEDULLA_SYMBOLUM_EXTERNUM)
    {
        redde NIHIL;   /* definitio duplex */
    }

    datum = (MedullaDatum*)piscina_allocare(modulus->piscina,
        (memoriae_index)magnitudo(MedullaDatum));
    si (datum == NIHIL)
    {
        redde NIHIL;
    }
    memset(datum, ZEPHYRUM, magnitudo(MedullaDatum));
    datum->titulus = symbolum->titulus;
    datum->magnitudo_octetorum = magnitudo_octetorum;
    datum->ordinatio = ordinatio;
    datum->imago = (i8*)piscina_allocare(modulus->piscina,
        (memoriae_index)magnitudo_octetorum);
    datum->relocationes = xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaRelocatio));
    si (datum->imago == NIHIL || datum->relocationes == NIHIL)
    {
        redde NIHIL;
    }
    memset(datum->imago, ZEPHYRUM,
        (memoriae_index)magnitudo_octetorum);

    locus_dati = (MedullaDatum**)xar_addere(modulus->data);
    si (locus_dati == NIHIL)
    {
        redde NIHIL;
    }
    *locus_dati = datum;
    symbolum->genus = MEDULLA_SYMBOLUM_DATUM;
    symbolum->index = (s32)(xar_numerus(modulus->data) - I);
    redde datum;
}

b32
medulla_datum_scribere (MedullaDatum* datum, i32 offset,
    constans i8* octeti, i32 numerus)
{
    si (datum == NIHIL || octeti == NIHIL)
    {
        redde FALSUM;
    }
    si (offset > datum->magnitudo_octetorum
        || numerus > datum->magnitudo_octetorum - offset)
    {
        redde FALSUM;
    }
    si (numerus > ZEPHYRUM)
    {
        memcpy(datum->imago + offset, octeti,
            (memoriae_index)numerus);
    }
    redde VERUM;
}

b32
medulla_relocationem_addere (MedullaDatum* datum, i32 offset,
    s32 symbolum)
{
    MedullaRelocatio* relocatio;

    si (datum == NIHIL || symbolum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (datum->magnitudo_octetorum < VIII || offset > datum->magnitudo_octetorum - VIII)
    {
        redde FALSUM;   /* locellus 8 octetorum extra imaginem */
    }
    relocatio = (MedullaRelocatio*)xar_addere(datum->relocationes);
    si (relocatio == NIHIL)
    {
        redde FALSUM;
    }
    relocatio->offset = offset;
    relocatio->symbolum = symbolum;
    redde VERUM;
}
