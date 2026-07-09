/* officina_medulla_textus.c - Forma textualis medullae (M1a C)
 * Scriptor (canon) + lector strictus. Vide .h pro contractu. */

#include "officina_medulla_textus.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================================================
 * Nomina signata (ORDO = ordo enumerationum!)
 * ================================================== */

interior constans character* OP_NOMINA[MEDULLA_OP_NUMERUS] = {
    "movere",
    "addere", "subtrahere", "multiplicare", "dividere", "residuum",
    "negare",
    "et", "vel", "aut", "complementum", "sinistrorsum",
    "dextrorsum",
    "aequalis", "inaequalis", "minor", "minor_aequalis", "maior",
    "maior_aequalis",
    "extendere", "amputare", "fluitare", "integrare", "dilatare",
    "artare",
    "legere", "scribere", "arca", "locus", "copia",
    "salire", "ramus", "redde", "vocare", "sistere"
};

interior constans character* TYPI_NOMINA[MEDULLA_TYPUS_NUMERUS] = {
    "i8", "i16", "i32", "i64",
    "s8", "s16", "s32", "s64",
    "f32", "f64"
};

constans character*
medulla_op_titulus (s32 op)
{
    si (op < ZEPHYRUM || op >= (s32)MEDULLA_OP_NUMERUS)
    {
        redde NIHIL;
    }
    redde OP_NOMINA[op];
}

constans character*
medulla_typum_titulus (s32 typus)
{
    si (typus < ZEPHYRUM || typus >= (s32)MEDULLA_TYPUS_NUMERUS)
    {
        redde NIHIL;
    }
    redde TYPI_NOMINA[typus];
}

/* ==================================================
 * SCRIPTOR - quod evomit, canon est
 * ================================================== */

interior vacuum
_chordam_evadere (ChordaAedificator* aed, chorda textus)
{
    i32 i;

    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        character c = (character)textus.datum[i];

        si (c == '\\')
        {
            chorda_aedificator_appendere_literis(aed, "\\\\");
        }
        alioquin si (c == '"')
        {
            chorda_aedificator_appendere_literis(aed, "\\\"");
        }
        alioquin si (c == '\n')
        {
            chorda_aedificator_appendere_literis(aed, "\\n");
        }
        alioquin si (c == '\t')
        {
            chorda_aedificator_appendere_literis(aed, "\\t");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed, c);
        }
    }
}

interior vacuum
_s64_scribere (ChordaAedificator* aed, s64 valor)
{
    character littera[XXXII];

    sprintf(littera, "%lld", valor);
    chorda_aedificator_appendere_literis(aed, littera);
}

interior vacuum
_operandum_scribere (ChordaAedificator* aed,
    constans MedullaModulus* modulus,
    constans MedullaFunctio* functio,
    constans MedullaOperandum* operandum)
{
    commutatio (operandum->genus)
    {
        casus (s32)MEDULLA_OPERANDUM_REGISTRUM:
        {
            constans chorda* titulus = medulla_registrum_titulus(
                functio, operandum->datum.index);

            chorda_aedificator_appendere_character(aed, '%');
            si (titulus != NIHIL)
            {
                chorda_aedificator_appendere_chorda(aed, *titulus);
            }
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_IMMEDIATUM:
        {
            _s64_scribere(aed, operandum->datum.immediatum);
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_IMMEDIATUM_F:
        {
            character littera[XL];
            i32 k;
            b32 signatum_fluitantis = FALSUM;

            sprintf(littera, "%.17g",
                operandum->datum.immediatum_f);
            /* integri toti (%.17g "100") signum fluitantis
             * servandum - aliter lector immediatum INTEGRALE
             * relegit (genus mutatur, textus byte-idem manet:
             * cautio subdola) */
            per (k = ZEPHYRUM; littera[k] != '\0'; k++)
            {
                si (littera[k] == '.' || littera[k] == 'e'
                    || littera[k] == 'E')
                {
                    signatum_fluitantis = VERUM;
                }
            }
            chorda_aedificator_appendere_literis(aed, littera);
            si (!signatum_fluitantis)
            {
                chorda_aedificator_appendere_literis(aed, ".0");
            }
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_SYMBOLUM:
        {
            constans MedullaSymbolum* symbolum =
                medulla_symbolum_obtinere(modulus,
                    operandum->datum.index);

            chorda_aedificator_appendere_character(aed, '$');
            si (symbolum != NIHIL)
            {
                chorda_aedificator_appendere_chorda(aed,
                    symbolum->titulus);
            }
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_BLOCCUS:
        {
            constans MedullaBloccus* bloccus =
                medulla_bloccum_obtinere(functio,
                    operandum->datum.index);

            chorda_aedificator_appendere_character(aed, '@');
            si (bloccus != NIHIL)
            {
                chorda_aedificator_appendere_chorda(aed,
                    bloccus->titulus);
            }
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_CAUSA:
        {
            constans chorda* causa = medulla_causam_obtinere(
                modulus, operandum->datum.index);

            chorda_aedificator_appendere_character(aed, '"');
            si (causa != NIHIL)
            {
                _chordam_evadere(aed, *causa);
            }
            chorda_aedificator_appendere_character(aed, '"');
            frange;
        }
        ordinarius:
        {
            frange;   /* NIHIL numquam scribitur */
        }
    }
}

interior vacuum
_instructionem_scribere (ChordaAedificator* aed,
    constans MedullaModulus* modulus,
    constans MedullaFunctio* functio,
    constans MedullaInstructio* instructio)
{
    chorda_aedificator_appendere_repetita(aed, ' ', IV);
    si (instructio->destinatio >= ZEPHYRUM)
    {
        constans chorda* titulus = medulla_registrum_titulus(
            functio, instructio->destinatio);

        chorda_aedificator_appendere_character(aed, '%');
        si (titulus != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed, *titulus);
        }
        chorda_aedificator_appendere_literis(aed, " = ");
    }
    chorda_aedificator_appendere_literis(aed,
        medulla_op_titulus(instructio->op));
    si (instructio->typus != MEDULLA_TYPUS_NIHIL)
    {
        chorda_aedificator_appendere_character(aed, '.');
        chorda_aedificator_appendere_literis(aed,
            medulla_typum_titulus(instructio->typus));
    }
    si (instructio->typus_secundus != MEDULLA_TYPUS_NIHIL)
    {
        chorda_aedificator_appendere_character(aed, '.');
        chorda_aedificator_appendere_literis(aed,
            medulla_typum_titulus(instructio->typus_secundus));
    }

    si (instructio->op == (s32)MEDULLA_OP_RAMUS)
    {
        chorda_aedificator_appendere_character(aed, ' ');
        _operandum_scribere(aed, modulus, functio, &instructio->a);
        chorda_aedificator_appendere_literis(aed, " -> ");
        _operandum_scribere(aed, modulus, functio, &instructio->b);
        chorda_aedificator_appendere_literis(aed, " | ");
        _operandum_scribere(aed, modulus, functio, &instructio->c);
    }
    alioquin si (instructio->op == (s32)MEDULLA_OP_VOCARE)
    {
        s32 i;

        chorda_aedificator_appendere_character(aed, ' ');
        _operandum_scribere(aed, modulus, functio, &instructio->a);
        per (i = ZEPHYRUM; i < instructio->extra_numerus; i++)
        {
            constans MedullaOperandum* argumentum =
                (constans MedullaOperandum*)xar_obtinere_s(
                    functio->operanda,
                    instructio->extra_index + i);

            chorda_aedificator_appendere_literis(aed, ", ");
            si (argumentum != NIHIL)
            {
                _operandum_scribere(aed, modulus, functio,
                    argumentum);
            }
        }
    }
    alioquin
    {
        constans MedullaOperandum* ordo[III];
        b32 primum = VERUM;
        s32 i;

        ordo[ZEPHYRUM] = &instructio->a;
        ordo[I] = &instructio->b;
        ordo[II] = &instructio->c;
        per (i = ZEPHYRUM; i < III; i++)
        {
            si (ordo[i]->genus == (s32)MEDULLA_OPERANDUM_NIHIL)
            {
                perge;
            }
            chorda_aedificator_appendere_literis(aed,
                primum ? " " : ", ");
            primum = FALSUM;
            _operandum_scribere(aed, modulus, functio, ordo[i]);
        }
    }
    chorda_aedificator_appendere_character(aed, '\n');
}

interior vacuum
_functionem_scribere (ChordaAedificator* aed,
    constans MedullaModulus* modulus,
    constans MedullaFunctio* functio)
{
    i32 i;
    i32 m;

    chorda_aedificator_appendere_literis(aed, "\nfunctio $");
    chorda_aedificator_appendere_chorda(aed, functio->titulus);
    chorda_aedificator_appendere_literis(aed, " (");
    m = xar_numerus(functio->parametra);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaParametrum* parametrum =
            (constans MedullaParametrum*)xar_obtinere(
                functio->parametra, i);
        constans chorda* titulus;

        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, ", ");
        }
        chorda_aedificator_appendere_literis(aed,
            medulla_typum_titulus(parametrum->typus));
        chorda_aedificator_appendere_literis(aed, " %");
        titulus = medulla_registrum_titulus(functio,
            parametrum->index);
        si (titulus != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed, *titulus);
        }
    }
    si (functio->est_variadica)
    {
        chorda_aedificator_appendere_literis(aed,
            (m > ZEPHYRUM) ? ", ..." : "...");
    }
    chorda_aedificator_appendere_literis(aed, ") -> ");
    si (functio->typus_reditus == MEDULLA_TYPUS_NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, "vacuum");
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(aed,
            medulla_typum_titulus(functio->typus_reditus));
    }
    chorda_aedificator_appendere_character(aed, '\n');

    m = xar_numerus(functio->blocci);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)xar_obtinere(functio->blocci,
                i);
        i32 j;
        i32 n = xar_numerus(bloccus->instructiones);

        chorda_aedificator_appendere_character(aed, '@');
        chorda_aedificator_appendere_chorda(aed, bloccus->titulus);
        chorda_aedificator_appendere_literis(aed, ":\n");
        per (j = ZEPHYRUM; j < n; j++)
        {
            constans MedullaInstructio* instructio =
                (constans MedullaInstructio*)xar_obtinere(
                    bloccus->instructiones, j);

            _instructionem_scribere(aed, modulus, functio,
                instructio);
        }
    }
}

interior vacuum
_datum_scribere (ChordaAedificator* aed,
    constans MedullaModulus* modulus, constans MedullaDatum* datum)
{
    i32 i;
    i32 m;

    chorda_aedificator_appendere_literis(aed, "\ndatum $");
    chorda_aedificator_appendere_chorda(aed, datum->titulus);
    chorda_aedificator_appendere_literis(aed, " magnitudo ");
    chorda_aedificator_appendere_i32(aed, datum->magnitudo_octetorum);
    chorda_aedificator_appendere_literis(aed, " ordinatio ");
    chorda_aedificator_appendere_i32(aed, datum->ordinatio);
    chorda_aedificator_appendere_character(aed, '\n');

    per (i = ZEPHYRUM; i < datum->magnitudo_octetorum; i++)
    {
        character littera[IV];

        si (i % XVI == ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed,
                "    octeti ");
        }
        alioquin si (i % IV == ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(aed, ' ');
        }
        sprintf(littera, "%02x", (insignatus integer)datum->imago[i]);
        chorda_aedificator_appendere_literis(aed, littera);
        si (i % XVI == XV || i == datum->magnitudo_octetorum - I)
        {
            chorda_aedificator_appendere_character(aed, '\n');
        }
    }

    m = xar_numerus(datum->relocationes);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaRelocatio* relocatio =
            (constans MedullaRelocatio*)xar_obtinere(
                datum->relocationes, i);
        constans MedullaSymbolum* symbolum =
            medulla_symbolum_obtinere(modulus, relocatio->symbolum);

        chorda_aedificator_appendere_literis(aed, "    relocatio ");
        chorda_aedificator_appendere_i32(aed, relocatio->offset);
        chorda_aedificator_appendere_literis(aed, " $");
        si (symbolum != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed,
                symbolum->titulus);
        }
        chorda_aedificator_appendere_character(aed, '\n');
    }
}

chorda
medulla_textum_scribere (Piscina* piscina,
    constans MedullaModulus* modulus)
{
    ChordaAedificator* aed;
    chorda vacua;
    i32 i;
    i32 m;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    si (piscina == NIHIL || modulus == NIHIL)
    {
        redde vacua;
    }
    aed = chorda_aedificator_creare(piscina, 4096);
    si (aed == NIHIL)
    {
        redde vacua;
    }

    chorda_aedificator_appendere_literis(aed, "modulus \"");
    _chordam_evadere(aed, modulus->titulus);
    chorda_aedificator_appendere_literis(aed, "\"\n");

    m = xar_numerus(modulus->functiones);
    per (i = ZEPHYRUM; i < m; i++)
    {
        MedullaFunctio** functio =
            (MedullaFunctio**)xar_obtinere(modulus->functiones, i);

        _functionem_scribere(aed, modulus, *functio);
    }
    m = xar_numerus(modulus->data);
    per (i = ZEPHYRUM; i < m; i++)
    {
        MedullaDatum** datum =
            (MedullaDatum**)xar_obtinere(modulus->data, i);

        _datum_scribere(aed, modulus, *datum);
    }
    redde chorda_aedificator_finire(aed);
}

/* ==================================================
 * LECTOR - strictus praeter commenta et lineas vacuas
 * ================================================== */

nomen structura {
    chorda textus;
    i32    numerus;              /* 1-basis */
} TextusLinea;

nomen structura {
    Piscina*        piscina;
    Xar*            lineae;      /* TextusLinea valore */
    i32             positio;
    MedullaModulus* modulus;
    MedullaFunctio* functio;     /* contextus currens */
    s32             bloccus;     /* -I extra bloccum */
    MedullaDatum*   datum;
    i32             offset_dati;
    i32             linea_erroris;
} TextusLector;

interior b32
_est_littera (character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_' || c == '.';
}

interior b32
_est_cifra (character c)
{
    redde c >= '0' && c <= '9';
}

interior vacuum
_spatia_praeterire (chorda linea, i32* cursor)
{
    dum (*cursor < linea.mensura
         && (character)linea.datum[*cursor] == ' ')
    {
        (*cursor)++;
    }
}

interior character
_aspicere (chorda linea, i32 cursor)
{
    si (cursor >= linea.mensura)
    {
        redde '\0';
    }
    redde (character)linea.datum[cursor];
}

interior chorda
_verbum_legere (chorda linea, i32* cursor)
{
    chorda verbum;
    i32 initium = *cursor;

    dum (*cursor < linea.mensura
         && _est_littera((character)linea.datum[*cursor]))
    {
        (*cursor)++;
    }
    verbum.datum = linea.datum + initium;
    verbum.mensura = *cursor - initium;
    redde verbum;
}

interior b32
_verbum_aequale (chorda verbum, constans character* literis)
{
    i32 longitudo = (i32)strlen(literis);

    si (verbum.mensura != longitudo)
    {
        redde FALSUM;
    }
    si (longitudo == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde memcmp(verbum.datum, literis,
        (memoriae_index)longitudo) == ZEPHYRUM;
}

interior b32
_s64_legere (chorda linea, i32* cursor, s64* valor_out)
{
    b32 negativum = FALSUM;
    s64 valor = ZEPHYRUM;   /* accumulatio NEGATIVA (S64_MIN tuta) */
    b32 ullus = FALSUM;

    si (_aspicere(linea, *cursor) == '-')
    {
        negativum = VERUM;
        (*cursor)++;
    }
    dum (*cursor < linea.mensura
         && _est_cifra((character)linea.datum[*cursor]))
    {
        valor = valor * X
            - (s64)((character)linea.datum[*cursor] - '0');
        (*cursor)++;
        ullus = VERUM;
    }
    si (!ullus)
    {
        redde FALSUM;
    }
    *valor_out = negativum ? valor : -valor;
    redde VERUM;
}

/* fructus in piscinam (fugae solutae) */
interior b32
_chordam_citatam_legere (TextusLector* lector, chorda linea, i32* cursor,
    chorda* fructus)
{
    i8* datum;
    i32 scriptum = ZEPHYRUM;

    si (_aspicere(linea, *cursor) != '"')
    {
        redde FALSUM;
    }
    (*cursor)++;
    datum = (i8*)piscina_allocare(lector->piscina,
        (memoriae_index)(linea.mensura > ZEPHYRUM
                         ? linea.mensura : I));
    si (datum == NIHIL)
    {
        redde FALSUM;
    }
    dum (*cursor < linea.mensura)
    {
        character c = (character)linea.datum[*cursor];

        si (c == '"')
        {
            (*cursor)++;
            fructus->datum = datum;
            fructus->mensura = scriptum;
            redde VERUM;
        }
        si (c == '\\')
        {
            character f;

            (*cursor)++;
            f = _aspicere(linea, *cursor);
            si (f == 'n')       { datum[scriptum] = (i8)'\n'; }
            alioquin si (f == 't')  { datum[scriptum] = (i8)'\t'; }
            alioquin si (f == '"')  { datum[scriptum] = (i8)'"'; }
            alioquin si (f == '\\') { datum[scriptum] = (i8)'\\'; }
            alioquin
            {
                redde FALSUM;   /* fuga ignota */
            }
        }
        alioquin
        {
            datum[scriptum] = (i8)c;
        }
        (*cursor)++;
        scriptum++;
    }
    redde FALSUM;   /* citatio non clausa */
}

interior s32
_registrum_invenire_aut_creare (MedullaFunctio* functio,
    chorda titulus)
{
    i32 i;
    i32 m = xar_numerus(functio->registra);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans chorda* sedes =
            (constans chorda*)xar_obtinere(functio->registra, i);

        si (sedes->mensura == titulus.mensura
            && (titulus.mensura == ZEPHYRUM
                || memcmp(sedes->datum, titulus.datum,
                       (memoriae_index)titulus.mensura) == ZEPHYRUM))
        {
            redde (s32)i;
        }
    }
    redde medulla_registrum_novum(functio, titulus);
}

interior s32
_bloccum_invenire (constans MedullaFunctio* functio, chorda titulus)
{
    i32 i;
    i32 m = xar_numerus(functio->blocci);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)xar_obtinere(functio->blocci,
                i);

        si (bloccus->titulus.mensura == titulus.mensura
            && memcmp(bloccus->titulus.datum, titulus.datum,
                   (memoriae_index)titulus.mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior s32
_op_invenire (chorda verbum)
{
    s32 i;

    per (i = ZEPHYRUM; i < (s32)MEDULLA_OP_NUMERUS; i++)
    {
        si (_verbum_aequale(verbum, OP_NOMINA[i]))
        {
            redde i;
        }
    }
    redde -I;
}

interior s32
_typum_invenire (chorda verbum)
{
    s32 i;

    per (i = ZEPHYRUM; i < (s32)MEDULLA_TYPUS_NUMERUS; i++)
    {
        si (_verbum_aequale(verbum, TYPI_NOMINA[i]))
        {
            redde i;
        }
    }
    redde -I;
}

/* numerus: fluitans si spatium characterum '.'/'e'/'E' continet */
interior b32
_numerum_legere (chorda linea, i32* cursor, MedullaOperandum* out)
{
    i32 finis = *cursor;
    b32 fluitat = FALSUM;

    si (_aspicere(linea, finis) == '-')
    {
        finis++;
    }
    dum (finis < linea.mensura)
    {
        character c = (character)linea.datum[finis];

        si (c == ' ' || c == ',')
        {
            frange;
        }
        si (c == '.' || c == 'e' || c == 'E' || c == '+')
        {
            fluitat = VERUM;
        }
        finis++;
    }
    si (fluitat)
    {
        character littera[XLVIII];
        i32 longitudo = finis - *cursor;

        si (longitudo <= ZEPHYRUM
            || longitudo >= (i32)magnitudo(littera))
        {
            redde FALSUM;
        }
        memcpy(littera, linea.datum + *cursor,
            (memoriae_index)longitudo);
        littera[longitudo] = '\0';
        *out = medulla_op_immediatum_f(strtod(littera, NIHIL));
        *cursor = finis;
        redde VERUM;
    }
    {
        s64 valor;

        si (!_s64_legere(linea, cursor, &valor))
        {
            redde FALSUM;
        }
        *out = medulla_op_immediatum(valor);
        redde VERUM;
    }
}

interior b32
_operandum_legere (TextusLector* lector, chorda linea, i32* cursor,
    MedullaOperandum* out)
{
    character c;

    _spatia_praeterire(linea, cursor);
    c = _aspicere(linea, *cursor);
    si (c == '%')
    {
        chorda verbum;
        s32 index;

        (*cursor)++;
        verbum = _verbum_legere(linea, cursor);
        si (verbum.mensura == ZEPHYRUM || lector->functio == NIHIL)
        {
            redde FALSUM;
        }
        index = _registrum_invenire_aut_creare(lector->functio,
            verbum);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        *out = medulla_op_registrum(index);
        redde VERUM;
    }
    si (c == '$')
    {
        chorda verbum;
        s32 index;

        (*cursor)++;
        verbum = _verbum_legere(linea, cursor);
        index = medulla_symbolum_internare(lector->modulus, verbum);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        *out = medulla_op_symbolum(index);
        redde VERUM;
    }
    si (c == '@')
    {
        chorda verbum;
        s32 index;

        (*cursor)++;
        verbum = _verbum_legere(linea, cursor);
        si (lector->functio == NIHIL)
        {
            redde FALSUM;
        }
        index = _bloccum_invenire(lector->functio, verbum);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;   /* bloccus ignotus */
        }
        *out = medulla_op_bloccum(index);
        redde VERUM;
    }
    si (c == '"')
    {
        chorda causa;
        s32 index;

        si (!_chordam_citatam_legere(lector, linea, cursor, &causa))
        {
            redde FALSUM;
        }
        index = medulla_causam_internare(lector->modulus, causa);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        *out = medulla_op_causa(index);
        redde VERUM;
    }
    redde _numerum_legere(linea, cursor, out);
}

interior b32
_finis_lineae (chorda linea, i32 cursor)
{
    _spatia_praeterire(linea, &cursor);
    redde cursor >= linea.mensura;
}

interior b32
_instructionem_legere (TextusLector* lector, chorda linea)
{
    i32 cursor = ZEPHYRUM;
    MedullaInstructio instructio;
    chorda verbum;

    memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
    instructio.typus = MEDULLA_TYPUS_NIHIL;
    instructio.typus_secundus = MEDULLA_TYPUS_NIHIL;
    instructio.destinatio = -I;
    instructio.extra_index = -I;
    instructio.a = medulla_op_nihil();
    instructio.b = medulla_op_nihil();
    instructio.c = medulla_op_nihil();

    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) == '%')
    {
        cursor++;
        verbum = _verbum_legere(linea, &cursor);
        si (verbum.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
        instructio.destinatio = _registrum_invenire_aut_creare(
            lector->functio, verbum);
        si (instructio.destinatio < ZEPHYRUM)
        {
            redde FALSUM;
        }
        _spatia_praeterire(linea, &cursor);
        si (_aspicere(linea, cursor) != '=')
        {
            redde FALSUM;
        }
        cursor++;
        _spatia_praeterire(linea, &cursor);
    }

    /* op[.typus[.typus]] */
    verbum = _verbum_legere(linea, &cursor);
    {
        s32 punctum_a = -I;   /* index puncti; -I absens (s32!) */
        s32 punctum_b = -I;
        i32 k;
        chorda pars;

        per (k = ZEPHYRUM; k < verbum.mensura; k++)
        {
            si ((character)verbum.datum[k] == '.')
            {
                si (punctum_a < ZEPHYRUM)
                {
                    punctum_a = (s32)k;
                }
                alioquin si (punctum_b < ZEPHYRUM)
                {
                    punctum_b = (s32)k;
                }
                alioquin
                {
                    redde FALSUM;
                }
            }
        }
        pars.datum = verbum.datum;
        pars.mensura = (punctum_a >= ZEPHYRUM) ? (i32)punctum_a
                                               : verbum.mensura;
        instructio.op = _op_invenire(pars);
        si (instructio.op < ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (punctum_a >= ZEPHYRUM)
        {
            i32 finis_partis = (punctum_b >= ZEPHYRUM)
                ? (i32)punctum_b : verbum.mensura;

            pars.datum = verbum.datum + punctum_a + I;
            pars.mensura = finis_partis - (i32)punctum_a - I;
            instructio.typus = _typum_invenire(pars);
            si (instructio.typus < ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
        si (punctum_b >= ZEPHYRUM)
        {
            pars.datum = verbum.datum + punctum_b + I;
            pars.mensura = verbum.mensura - (i32)punctum_b - I;
            instructio.typus_secundus = _typum_invenire(pars);
            si (instructio.typus_secundus < ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
    }

    si (instructio.op == (s32)MEDULLA_OP_RAMUS)
    {
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.a))
        {
            redde FALSUM;
        }
        _spatia_praeterire(linea, &cursor);
        si (_aspicere(linea, cursor) != '-'
            || _aspicere(linea, cursor + I) != '>')
        {
            redde FALSUM;
        }
        cursor += II;
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.b))
        {
            redde FALSUM;
        }
        _spatia_praeterire(linea, &cursor);
        si (_aspicere(linea, cursor) != '|')
        {
            redde FALSUM;
        }
        cursor++;
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.c))
        {
            redde FALSUM;
        }
    }
    alioquin si (instructio.op == (s32)MEDULLA_OP_VOCARE)
    {
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.a))
        {
            redde FALSUM;
        }
        instructio.extra_numerus = ZEPHYRUM;
        _spatia_praeterire(linea, &cursor);
        dum (_aspicere(linea, cursor) == ',')
        {
            MedullaOperandum argumentum;
            s32 index;

            cursor++;
            si (!_operandum_legere(lector, linea, &cursor,
                    &argumentum))
            {
                redde FALSUM;
            }
            index = medulla_operanda_addere(lector->functio,
                &argumentum, I);
            si (index < ZEPHYRUM)
            {
                redde FALSUM;
            }
            si (instructio.extra_numerus == ZEPHYRUM)
            {
                instructio.extra_index = index;
            }
            instructio.extra_numerus++;
            _spatia_praeterire(linea, &cursor);
        }
    }
    alioquin si (!_finis_lineae(linea, cursor))
    {
        /* usque ad tres operanda virgulis separata */
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.a))
        {
            redde FALSUM;
        }
        _spatia_praeterire(linea, &cursor);
        si (_aspicere(linea, cursor) == ',')
        {
            cursor++;
            si (!_operandum_legere(lector, linea, &cursor,
                    &instructio.b))
            {
                redde FALSUM;
            }
            _spatia_praeterire(linea, &cursor);
            si (_aspicere(linea, cursor) == ',')
            {
                cursor++;
                si (!_operandum_legere(lector, linea, &cursor,
                        &instructio.c))
                {
                    redde FALSUM;
                }
            }
        }
    }

    si (!_finis_lineae(linea, cursor))
    {
        redde FALSUM;   /* reliquiae post instructionem */
    }
    redde medulla_emittere(lector->functio, lector->bloccus,
        &instructio);
}

/* praescandere tituli bloccorum usque ad terminum functionis */
interior b32
_bloccos_praescandere (TextusLector* lector)
{
    i32 i;
    i32 m = xar_numerus(lector->lineae);

    per (i = lector->positio + I; i < m; i++)
    {
        constans TextusLinea* linea =
            (constans TextusLinea*)xar_obtinere(lector->lineae, i);
        i32 cursor = ZEPHYRUM;
        character c;

        _spatia_praeterire(linea->textus, &cursor);
        c = _aspicere(linea->textus, cursor);
        si (c == '@')
        {
            chorda verbum;

            cursor++;
            verbum = _verbum_legere(linea->textus, &cursor);
            si (verbum.mensura == ZEPHYRUM
                || _aspicere(linea->textus, cursor) != ':'
                || medulla_bloccum_creare(lector->functio, verbum)
                    < ZEPHYRUM)
            {
                lector->linea_erroris = linea->numerus;
                redde FALSUM;
            }
        }
        alioquin si (c != '%')
        {
            chorda verbum = _verbum_legere(linea->textus, &cursor);

            si (_verbum_aequale(verbum, "functio")
                || _verbum_aequale(verbum, "datum")
                || _verbum_aequale(verbum, "modulus"))
            {
                frange;
            }
        }
    }
    redde VERUM;
}

interior b32
_functionem_legere (TextusLector* lector, chorda linea, i32 cursor)
{
    chorda titulus;
    s32 typi_parametrorum[XXXII];
    chorda tituli_parametrorum[XXXII];
    s32 numerus_parametrorum = ZEPHYRUM;
    b32 variadica = FALSUM;
    s32 typus_reditus;
    s32 i;

    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) != '$')
    {
        redde FALSUM;
    }
    cursor++;
    titulus = _verbum_legere(linea, &cursor);
    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) != '(')
    {
        redde FALSUM;
    }
    cursor++;
    _spatia_praeterire(linea, &cursor);
    dum (_aspicere(linea, cursor) != ')')
    {
        si (numerus_parametrorum > ZEPHYRUM || variadica)
        {
            si (_aspicere(linea, cursor) != ',')
            {
                redde FALSUM;
            }
            cursor++;
            _spatia_praeterire(linea, &cursor);
        }
        si (_aspicere(linea, cursor) == '.')
        {
            /* "..." */
            si (_aspicere(linea, cursor + I) != '.'
                || _aspicere(linea, cursor + II) != '.')
            {
                redde FALSUM;
            }
            cursor += III;
            variadica = VERUM;
            _spatia_praeterire(linea, &cursor);
            perge;
        }
        si (numerus_parametrorum
            >= (s32)(magnitudo(typi_parametrorum)
                     / magnitudo(typi_parametrorum[ZEPHYRUM])))
        {
            redde FALSUM;
        }
        {
            chorda verbum = _verbum_legere(linea, &cursor);
            s32 typus = _typum_invenire(verbum);

            si (typus < ZEPHYRUM)
            {
                redde FALSUM;
            }
            _spatia_praeterire(linea, &cursor);
            si (_aspicere(linea, cursor) != '%')
            {
                redde FALSUM;
            }
            cursor++;
            typi_parametrorum[numerus_parametrorum] = typus;
            tituli_parametrorum[numerus_parametrorum] =
                _verbum_legere(linea, &cursor);
            numerus_parametrorum++;
            _spatia_praeterire(linea, &cursor);
        }
    }
    cursor++;   /* ')' */
    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) != '-'
        || _aspicere(linea, cursor + I) != '>')
    {
        redde FALSUM;
    }
    cursor += II;
    _spatia_praeterire(linea, &cursor);
    {
        chorda verbum = _verbum_legere(linea, &cursor);

        si (_verbum_aequale(verbum, "vacuum"))
        {
            typus_reditus = MEDULLA_TYPUS_NIHIL;
        }
        alioquin
        {
            typus_reditus = _typum_invenire(verbum);
            si (typus_reditus < ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
    }
    si (!_finis_lineae(linea, cursor))
    {
        redde FALSUM;
    }

    lector->functio = medulla_functionem_creare(lector->modulus,
        titulus, typus_reditus, variadica);
    lector->datum = NIHIL;
    lector->bloccus = -I;
    si (lector->functio == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus_parametrorum; i++)
    {
        si (medulla_parametrum_addere(lector->functio,
                tituli_parametrorum[i], typi_parametrorum[i])
            < ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde _bloccos_praescandere(lector);
}

interior b32
_datum_legere (TextusLector* lector, chorda linea, i32 cursor)
{
    chorda titulus;
    s64 magnitudo_octetorum;
    s64 ordinatio;
    chorda verbum;

    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) != '$')
    {
        redde FALSUM;
    }
    cursor++;
    titulus = _verbum_legere(linea, &cursor);
    _spatia_praeterire(linea, &cursor);
    verbum = _verbum_legere(linea, &cursor);
    si (!_verbum_aequale(verbum, "magnitudo"))
    {
        redde FALSUM;
    }
    _spatia_praeterire(linea, &cursor);
    si (!_s64_legere(linea, &cursor, &magnitudo_octetorum))
    {
        redde FALSUM;
    }
    _spatia_praeterire(linea, &cursor);
    verbum = _verbum_legere(linea, &cursor);
    si (!_verbum_aequale(verbum, "ordinatio"))
    {
        redde FALSUM;
    }
    _spatia_praeterire(linea, &cursor);
    si (!_s64_legere(linea, &cursor, &ordinatio)
        || !_finis_lineae(linea, cursor))
    {
        redde FALSUM;
    }
    lector->datum = medulla_datum_creare(lector->modulus, titulus,
        (i32)magnitudo_octetorum, (i32)ordinatio);
    lector->functio = NIHIL;
    lector->bloccus = -I;
    lector->offset_dati = ZEPHYRUM;
    redde lector->datum != NIHIL;
}

interior s32
_cifra_hex (character c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    redde -I;
}

interior b32
_octetos_legere (TextusLector* lector, chorda linea, i32 cursor)
{
    i8 octeti[LXIV];
    i32 numerus = ZEPHYRUM;

    per (;;)
    {
        s32 altum;
        s32 imum;

        _spatia_praeterire(linea, &cursor);
        si (cursor >= linea.mensura)
        {
            frange;
        }
        altum = _cifra_hex(_aspicere(linea, cursor));
        imum = _cifra_hex(_aspicere(linea, cursor + I));
        si (altum < ZEPHYRUM || imum < ZEPHYRUM
            || numerus >= (i32)magnitudo(octeti))
        {
            redde FALSUM;
        }
        octeti[numerus] = (i8)(altum * XVI + imum);
        numerus++;
        cursor += II;
    }
    si (numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (!medulla_datum_scribere(lector->datum, lector->offset_dati,
            octeti, (i32)numerus))
    {
        redde FALSUM;
    }
    lector->offset_dati += (i32)numerus;
    redde VERUM;
}

interior b32
_relocationem_legere (TextusLector* lector, chorda linea, i32 cursor)
{
    s64 offset;
    chorda titulus;
    s32 symbolum;

    _spatia_praeterire(linea, &cursor);
    si (!_s64_legere(linea, &cursor, &offset))
    {
        redde FALSUM;
    }
    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) != '$')
    {
        redde FALSUM;
    }
    cursor++;
    titulus = _verbum_legere(linea, &cursor);
    si (!_finis_lineae(linea, cursor))
    {
        redde FALSUM;
    }
    symbolum = medulla_symbolum_internare(lector->modulus, titulus);
    si (symbolum < ZEPHYRUM || offset < ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde medulla_relocationem_addere(lector->datum, (i32)offset,
        symbolum);
}

interior b32
_lineam_tractare (TextusLector* lector, constans TextusLinea* linea)
{
    i32 cursor = ZEPHYRUM;
    character c;
    chorda verbum;

    _spatia_praeterire(linea->textus, &cursor);
    c = _aspicere(linea->textus, cursor);

    si (c == '@')
    {
        s32 index;

        si (lector->functio == NIHIL)
        {
            redde FALSUM;
        }
        cursor++;
        verbum = _verbum_legere(linea->textus, &cursor);
        si (_aspicere(linea->textus, cursor) != ':'
            || !_finis_lineae(linea->textus, cursor + I))
        {
            redde FALSUM;
        }
        index = _bloccum_invenire(lector->functio, verbum);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        lector->bloccus = index;
        redde VERUM;
    }
    si (c == '%')
    {
        si (lector->functio == NIHIL || lector->bloccus < ZEPHYRUM)
        {
            redde FALSUM;
        }
        redde _instructionem_legere(lector, linea->textus);
    }

    {
        i32 cursor_verbi = cursor;

        verbum = _verbum_legere(linea->textus, &cursor_verbi);
        si (_verbum_aequale(verbum, "modulus"))
        {
            chorda titulus;

            si (lector->modulus != NIHIL)
            {
                redde FALSUM;
            }
            _spatia_praeterire(linea->textus, &cursor_verbi);
            si (!_chordam_citatam_legere(lector, linea->textus,
                    &cursor_verbi, &titulus)
                || !_finis_lineae(linea->textus, cursor_verbi))
            {
                redde FALSUM;
            }
            lector->modulus = medulla_modulum_creare(
                lector->piscina, titulus);
            redde lector->modulus != NIHIL;
        }
        si (lector->modulus == NIHIL)
        {
            redde FALSUM;
        }
        si (_verbum_aequale(verbum, "functio"))
        {
            redde _functionem_legere(lector, linea->textus,
                cursor_verbi);
        }
        si (_verbum_aequale(verbum, "datum"))
        {
            redde _datum_legere(lector, linea->textus,
                cursor_verbi);
        }
        si (_verbum_aequale(verbum, "octeti"))
        {
            si (lector->datum == NIHIL)
            {
                redde FALSUM;
            }
            redde _octetos_legere(lector, linea->textus,
                cursor_verbi);
        }
        si (_verbum_aequale(verbum, "relocatio"))
        {
            si (lector->datum == NIHIL)
            {
                redde FALSUM;
            }
            redde _relocationem_legere(lector, linea->textus,
                cursor_verbi);
        }
        /* instructio sine destinatione (salire/redde/...) */
        si (lector->functio == NIHIL || lector->bloccus < ZEPHYRUM)
        {
            redde FALSUM;
        }
        redde _instructionem_legere(lector, linea->textus);
    }
}

MedullaModulus*
medulla_textum_legere (Piscina* piscina, chorda textus,
    i32* linea_erroris)
{
    TextusLector lector;
    i32 i;
    i32 m;
    i32 initium = ZEPHYRUM;
    i32 numerus_lineae = I;

    si (linea_erroris != NIHIL)
    {
        *linea_erroris = ZEPHYRUM;
    }
    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    memset(&lector, ZEPHYRUM, magnitudo(TextusLector));
    lector.piscina = piscina;
    lector.bloccus = -I;
    lector.lineae = xar_creare(piscina, (i32)magnitudo(TextusLinea));
    si (lector.lineae == NIHIL)
    {
        redde NIHIL;
    }

    /* findere in lineas; vacuas et commenta (;) praetermittere */
    per (i = ZEPHYRUM; i <= textus.mensura; i++)
    {
        si (i == textus.mensura
            || (character)textus.datum[i] == '\n')
        {
            chorda contenta;
            i32 cursor = ZEPHYRUM;

            contenta.datum = textus.datum + initium;
            contenta.mensura = i - initium;
            _spatia_praeterire(contenta, &cursor);
            si (cursor < contenta.mensura
                && (character)contenta.datum[cursor] != ';')
            {
                TextusLinea* sedes = (TextusLinea*)xar_addere(lector.lineae);

                si (sedes == NIHIL)
                {
                    redde NIHIL;
                }
                sedes->textus = contenta;
                sedes->numerus = numerus_lineae;
            }
            initium = i + I;
            numerus_lineae++;
        }
    }

    m = xar_numerus(lector.lineae);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans TextusLinea* linea =
            (constans TextusLinea*)xar_obtinere(lector.lineae, i);

        lector.positio = i;
        si (!_lineam_tractare(&lector, linea))
        {
            si (linea_erroris != NIHIL)
            {
                *linea_erroris = (lector.linea_erroris > ZEPHYRUM)
                    ? lector.linea_erroris : linea->numerus;
            }
            redde NIHIL;
        }
    }
    si (lector.modulus == NIHIL && linea_erroris != NIHIL)
    {
        *linea_erroris = numerus_lineae - I;
    }
    redde lector.modulus;
}
