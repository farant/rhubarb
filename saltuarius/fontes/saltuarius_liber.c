/* saltuarius_liber.c - Implementatio fili aperti (Phase B + C)
 *
 * Phase C: stratum = (textus, lineae, classis) triplum - via
 * picturae Phase B in QUOVIS strato immutata currit. Stratum 0 =
 * plagula ut scripta; k>0 materializata ex parsura->strata[k-1]
 * (pigre, in commutatione strati - accessores stratum activum
 * PARATUM semper legunt).
 */

#include "saltuarius_liber.h"
#include "utf8.h"
#include <string.h>

/* Index linearum: unus ambulatus (communis strato 0 et
 * materializatis) */
interior vacuum
_lineas_aedificare (Piscina* piscina, chorda textus,
    SaltuariusLinea** lineae_out, i32* numerus_out)
{
    i32 numerus = I;
    i32 k;
    SaltuariusLinea* lineae;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == '\n')
        {
            numerus++;
        }
    }
    lineae = (SaltuariusLinea*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)numerus
            * (memoriae_index)magnitudo(SaltuariusLinea), IV);
    si (lineae == NIHIL)
    {
        *lineae_out = NIHIL;
        *numerus_out = ZEPHYRUM;
        redde;
    }
    {
        i32 linea = ZEPHYRUM;
        i32 initium = ZEPHYRUM;

        per (k = ZEPHYRUM; k <= textus.mensura; k++)
        {
            si (k == textus.mensura || textus.datum[k] == '\n')
            {
                i32 mensura = k - initium;

                /* <tolera codex="SUBTRACTIO_COMPARATA" (>k prorsum ab initio scandit: numquam ante initium */
                si (mensura > ZEPHYRUM
                    && textus.datum[initium + mensura - I] == '\r')
                {
                    mensura--;
                }
                lineae[linea].offset = (i32)initium;
                lineae[linea].mensura = mensura;
                linea++;
                initium = k + I;
                si (k == textus.mensura)
                {
                    frange;
                }
            }
        }
    }
    *lineae_out = lineae;
    *numerus_out = numerus;
}

/* Stratum activum (semper paratum - commutatio materializat) */
interior constans SaltuariusStratum*
_activum (constans SaltuariusLiber* liber)
{
    redde &liber->strata_visus[liber->stratum_currens];
}

SaltuariusLiber*
saltuarius_liber_aperire (Piscina* persistens,
    SaltuariusNexus* nexus, chorda via, chorda textus)
{
    SaltuariusLiber* liber;
    memoriae_index initium_arenae;

    liber = (SaltuariusLiber*)piscina_allocare_ordinatum(persistens,
        (memoriae_index)magnitudo(SaltuariusLiber), IV);
    si (liber == NIHIL)
    {
        redde NIHIL;
    }
    initium_arenae = (memoriae_index)textus.mensura * III + 262144;
    liber->piscina = piscina_generare_dynamicum("salt_liber",
        initium_arenae);
    si (liber->piscina == NIHIL)
    {
        redde NIHIL;
    }
    liber->arena_silvae = NIHIL;
    liber->via = chorda_transcribere(via, liber->piscina);
    liber->textus = chorda_transcribere(textus, liber->piscina);
    liber->classis = NIHIL;
    liber->est_colorata = FALSUM;
    liber->cursor_linea = ZEPHYRUM;
    liber->cursor_columna = ZEPHYRUM;
    liber->volumen_y = ZEPHYRUM;
    liber->volumen_x = ZEPHYRUM;
    liber->parsura = NIHIL;
    liber->strata_visus = NIHIL;
    liber->numerus_stratorum = I;
    liber->stratum_currens = ZEPHYRUM;

    _lineas_aedificare(liber->piscina, liber->textus,
        &liber->lineae, &liber->numerus_linearum);
    si (liber->lineae == NIHIL)
    {
        piscina_destruere(liber->piscina);
        redde NIHIL;
    }

    /* coloratio + parse: .c/.h */
    si (saltuarius_nexus_est_fons_c(liber->via)
        && liber->textus.mensura > ZEPHYRUM)
    {
        liber->arena_silvae = silva_piscina_generare_dynamicum(
            "salt_liber_silva",
            (memoriae_index)textus.mensura * IV + 1048576);
        si (liber->arena_silvae != NIHIL)
        {
            liber->classis = (i8*)piscina_allocare(liber->piscina,
                (memoriae_index)liber->textus.mensura);
            si (liber->classis != NIHIL)
            {
                saltuarius_nexus_classificare(nexus,
                    liber->arena_silvae, liber->textus,
                    liber->classis);
                liber->est_colorata = VERUM;
            }

            /* PARSE (Phase C): lexemata textum ALIANT - textus in
             * arena libri, lexemata in arena silvae, ambae
             * co-morientes. Titulus = via (pro fons tabula). */
            {
                character* titulus = (character*)piscina_allocare(
                    liber->piscina,
                    (memoriae_index)liber->via.mensura + I);

                si (titulus != NIHIL)
                {
                    memcpy(titulus, liber->via.datum,
                        (memoriae_index)liber->via.mensura);
                    titulus[liber->via.mensura] = '\0';
                    liber->parsura = saltuarius_nexus_parsare(nexus,
                        liber->arena_silvae, titulus,
                        liber->textus);
                }
            }

            /* OMISSUM: rami non sumpti fusci (extenta OCTETI -
             * additiones II; stratum 0 solum) */
            si (liber->parsura != NIHIL && liber->classis != NIHIL
                && liber->parsura->expansio != NIHIL)
            {
                constans SilvaExpansio* exp =
                    liber->parsura->expansio;
                i32 n = silva_rami_numerus(exp);
                i32 k;

                per (k = ZEPHYRUM; k < n; k++)
                {
                    SilvaRamusVista vista;

                    si (silva_ramus_vista(exp, k, &vista)
                        && !vista.est_sumptum
                        && vista.fons_index
                            == liber->parsura->fons_princeps
                        && vista.corpus_initium >= ZEPHYRUM
                        && vista.corpus_finis
                            > vista.corpus_initium)
                    {
                        i32 initium = (i32)vista.corpus_initium;
                        i32 finis = (i32)vista.corpus_finis;
                        i32 o;

                        si (finis > liber->textus.mensura)
                        {
                            finis = liber->textus.mensura;
                        }
                        per (o = initium; o < finis; o++)
                        {
                            liber->classis[o] =
                                (i8)SALT_CLASSIS_OMISSUM;
                        }
                    }
                }
            }
        }
    }

    /* strata_visus: [0] = plagula ut scripta; k>0 pigra */
    {
        i32 n_strata = ZEPHYRUM;

        si (liber->parsura != NIHIL
            && liber->parsura->strata != NIHIL)
        {
            n_strata = silva_xar_numerus(liber->parsura->strata);
        }
        liber->numerus_stratorum = I + (i32)n_strata;
        liber->strata_visus = (SaltuariusStratum*)
            piscina_allocare_ordinatum(liber->piscina,
                (memoriae_index)liber->numerus_stratorum
                    * (memoriae_index)magnitudo(SaltuariusStratum),
                IV);
        si (liber->strata_visus == NIHIL)
        {
            liber->numerus_stratorum = I;
            /* sine tabula stratorum liber inutilis */
            piscina_destruere(liber->piscina);
            si (liber->arena_silvae != NIHIL)
            {
                silva_piscina_destruere(liber->arena_silvae);
            }
            redde NIHIL;
        }
        {
            i32 k;

            per (k = ZEPHYRUM; k < liber->numerus_stratorum; k++)
            {
                liber->strata_visus[k].parata = FALSUM;
                liber->strata_visus[k].positiones = NIHIL;
                liber->strata_visus[k].numerus_positionum =
                    ZEPHYRUM;
            }
        }
        liber->strata_visus[ZEPHYRUM].parata = VERUM;
        liber->strata_visus[ZEPHYRUM].textus = liber->textus;
        liber->strata_visus[ZEPHYRUM].lineae = liber->lineae;
        liber->strata_visus[ZEPHYRUM].numerus_linearum =
            liber->numerus_linearum;
        liber->strata_visus[ZEPHYRUM].classis = liber->classis;
    }
    redde liber;
}

/* Iunctio tuta: lexemata expansa trivia invocationis NON
 * hereditant (spatium in lexemate substituto attachatum erat) -
 * sine hoc "constans Piscina" fit "constPiscina" (morsus manualis
 * strati I). Regula: identificatoria adiacentia aut operatores
 * iterati spatium unum accipiunt. */
interior b32
_glutinosum (i8 a, i8 b)
{
    b32 a_ident = ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')
        || (a >= '0' && a <= '9') || a == '_') ? VERUM : FALSUM;
    b32 b_ident = ((b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z')
        || (b >= '0' && b <= '9') || b == '_') ? VERUM : FALSUM;

    si (a_ident && b_ident)
    {
        redde VERUM;
    }
    si (a == b && (a == '+' || a == '-' || a == '&' || a == '|'
        || a == '<' || a == '>' || a == '='))
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* Materializatio strati k>0: serializare fluxum (trivia ante +
 * valor + trivia post per lexema; dominus unus - nihil bis),
 * classis ex genere, mappa positionum lexematum veterum;
 * IUNCTIO TUTA ubi trivia perierunt (expansio) */
interior vacuum
_stratum_materializare (SaltuariusLiber* liber,
    constans SaltuariusNexus* nexus, s32 stratum)
{
    SaltuariusStratum* visus = &liber->strata_visus[stratum];
    SilvaXar* fluxus = *(SilvaXar**)silva_xar_obtinere(
        liber->parsura->strata, (i32)(stratum - I));
    i32 n = silva_xar_numerus(fluxus);
    i32 summa_octetorum = ZEPHYRUM;
    i32 numerus_positionum = ZEPHYRUM;
    i32 k;

    /* transitus I: mensurae */
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaToken* t = *(SilvaToken**)silva_xar_obtinere(fluxus, k);
        i32 j;

        si (t->spatia_ante != NIHIL)
        {
            i32 m = silva_xar_numerus(t->spatia_ante);

            per (j = ZEPHYRUM; j < m; j++)
            {
                summa_octetorum += (*(SilvaToken**)
                    silva_xar_obtinere(t->spatia_ante, j))
                    ->valor.mensura;
            }
        }
        si (t->valor.mensura > ZEPHYRUM)
        {
            summa_octetorum += t->valor.mensura;
            numerus_positionum++;
        }
        si (t->spatia_post != NIHIL)
        {
            i32 m = silva_xar_numerus(t->spatia_post);

            per (j = ZEPHYRUM; j < m; j++)
            {
                summa_octetorum += (*(SilvaToken**)
                    silva_xar_obtinere(t->spatia_post, j))
                    ->valor.mensura;
            }
        }
    }

    /* + n: spatia iunctionis tutae possibilia */
    visus->textus.datum = (i8*)piscina_allocare(liber->piscina,
        (memoriae_index)(summa_octetorum + (i32)n + I));
    visus->classis = (i8*)piscina_allocare(liber->piscina,
        (memoriae_index)(summa_octetorum + (i32)n + I));
    visus->positiones = (SaltuariusPositio*)
        piscina_allocare_ordinatum(liber->piscina,
            (memoriae_index)(numerus_positionum > ZEPHYRUM
                ? numerus_positionum : I)
                * (memoriae_index)magnitudo(SaltuariusPositio), IV);
    si (visus->textus.datum == NIHIL || visus->classis == NIHIL
        || visus->positiones == NIHIL)
    {
        visus->textus.mensura = ZEPHYRUM;
        visus->parata = VERUM;   /* degradatum sed paratum */
        redde;
    }

    /* transitus II: octeti + classis + positiones */
    {
        i32 cursor = ZEPHYRUM;
        i32 positio = ZEPHYRUM;

        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaToken* t = *(SilvaToken**)silva_xar_obtinere(
                fluxus, k);
            i32 j;

            si (t->spatia_ante != NIHIL)
            {
                i32 m = silva_xar_numerus(t->spatia_ante);

                per (j = ZEPHYRUM; j < m; j++)
                {
                    SilvaToken* trivium = *(SilvaToken**)
                        silva_xar_obtinere(t->spatia_ante, j);
                    i32 o;

                    per (o = ZEPHYRUM; o < trivium->valor.mensura;
                        o++)
                    {
                        visus->textus.datum[cursor + o] =
                            trivium->valor.datum[o];
                        visus->classis[cursor + o] =
                            saltuarius_nexus_classis(nexus,
                                trivium);
                    }
                    cursor += trivium->valor.mensura;
                }
            }
            si (t->valor.mensura > ZEPHYRUM)
            {
                i32 o;

                /* iunctio tuta */
                si (cursor > ZEPHYRUM
                    && _glutinosum(
                        visus->textus.datum[cursor - I],
                        t->valor.datum[ZEPHYRUM]))
                {
                    visus->textus.datum[cursor] = ' ';
                    visus->classis[cursor] =
                        (i8)SALT_CLASSIS_PLANUM;
                    cursor++;
                }
                visus->positiones[positio].token = t;
                visus->positiones[positio].initium = cursor;
                per (o = ZEPHYRUM; o < t->valor.mensura; o++)
                {
                    visus->textus.datum[cursor + o] =
                        t->valor.datum[o];
                    visus->classis[cursor + o] =
                        saltuarius_nexus_classis(nexus, t);
                }
                cursor += t->valor.mensura;
                visus->positiones[positio].finis = cursor;
                positio++;
            }
            si (t->spatia_post != NIHIL)
            {
                i32 m = silva_xar_numerus(t->spatia_post);

                per (j = ZEPHYRUM; j < m; j++)
                {
                    SilvaToken* trivium = *(SilvaToken**)
                        silva_xar_obtinere(t->spatia_post, j);
                    i32 o;

                    per (o = ZEPHYRUM; o < trivium->valor.mensura;
                        o++)
                    {
                        visus->textus.datum[cursor + o] =
                            trivium->valor.datum[o];
                        visus->classis[cursor + o] =
                            saltuarius_nexus_classis(nexus,
                                trivium);
                    }
                    cursor += trivium->valor.mensura;
                }
            }
        }
        visus->textus.mensura = (i32)cursor;
        visus->numerus_positionum = positio;
    }

    _lineas_aedificare(liber->piscina, visus->textus,
        &visus->lineae, &visus->numerus_linearum);
    visus->parata = VERUM;
}

constans SaltuariusStratum*
saltuarius_liber_stratum_activum (constans SaltuariusLiber* liber)
{
    redde _activum(liber);
}

constans SaltuariusStratum*
saltuarius_liber_stratum (SaltuariusLiber* liber,
    constans SaltuariusNexus* nexus)
{
    SaltuariusStratum* visus =
        &liber->strata_visus[liber->stratum_currens];

    si (!visus->parata && liber->parsura != NIHIL
        && liber->stratum_currens > ZEPHYRUM)
    {
        _stratum_materializare(liber, nexus,
            liber->stratum_currens);
    }
    redde visus;
}

vacuum
saltuarius_liber_stratum_ponere (SaltuariusLiber* liber,
    s32 stratum)
{
    si (stratum < ZEPHYRUM)
    {
        stratum = ZEPHYRUM;
    }
    si (stratum >= (s32)liber->numerus_stratorum)
    {
        stratum = (s32)liber->numerus_stratorum - I;
    }
    liber->stratum_currens = stratum;
}

vacuum
saltuarius_liber_destruere (SaltuariusLiber* liber)
{
    SilvaPiscina* arena_silvae = liber->arena_silvae;
    Piscina* piscina = liber->piscina;

    liber->arena_silvae = NIHIL;
    liber->piscina = NIHIL;
    liber->classis = NIHIL;
    liber->lineae = NIHIL;
    liber->numerus_linearum = ZEPHYRUM;
    liber->parsura = NIHIL;
    liber->strata_visus = NIHIL;
    liber->numerus_stratorum = ZEPHYRUM;
    si (arena_silvae != NIHIL)
    {
        silva_piscina_destruere(arena_silvae);
    }
    si (piscina != NIHIL)
    {
        piscina_destruere(piscina);
    }
}

chorda
saltuarius_liber_linea (constans SaltuariusLiber* liber, s32 index)
{
    constans SaltuariusStratum* visus = _activum(liber);
    chorda vacua;

    si (index < ZEPHYRUM || !visus->parata
        || index >= (s32)visus->numerus_linearum)
    {
        vacua.mensura = ZEPHYRUM;
        vacua.datum = NIHIL;
        redde vacua;
    }
    redde chorda_sectio(visus->textus,
        (i32)visus->lineae[index].offset,
        (i32)visus->lineae[index].offset
            + visus->lineae[index].mensura);
}

i32
saltuarius_liber_linea_runae (constans SaltuariusLiber* liber,
    s32 index)
{
    chorda linea = saltuarius_liber_linea(liber, index);

    si (linea.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (i32)utf8_numerare_runas(linea.datum,
        (s32)linea.mensura);
}

/* Numerus linearum strati activi (pro claudendo cursoris) */
interior i32
_lineae_activae (constans SaltuariusLiber* liber)
{
    constans SaltuariusStratum* visus = _activum(liber);

    redde visus->parata ? visus->numerus_linearum : I;
}

vacuum
saltuarius_liber_movere (SaltuariusLiber* liber, s32 delta_linea)
{
    s32 nova = liber->cursor_linea + delta_linea;
    i32 runae;
    i32 lineae = _lineae_activae(liber);

    si (nova < ZEPHYRUM)
    {
        nova = ZEPHYRUM;
    }
    si (nova >= (s32)lineae)
    {
        nova = (s32)lineae - I;
    }
    liber->cursor_linea = nova;
    runae = saltuarius_liber_linea_runae(liber, nova);
    si (liber->cursor_columna > (s32)runae)
    {
        liber->cursor_columna = (s32)runae;
    }
}

vacuum
saltuarius_liber_movere_col (SaltuariusLiber* liber, s32 delta)
{
    s32 nova = liber->cursor_columna + delta;
    i32 runae = saltuarius_liber_linea_runae(liber,
        liber->cursor_linea);

    si (nova < ZEPHYRUM)
    {
        nova = ZEPHYRUM;
    }
    si (nova > (s32)runae)
    {
        nova = (s32)runae;
    }
    liber->cursor_columna = nova;
}

vacuum
saltuarius_liber_primum (SaltuariusLiber* liber)
{
    liber->cursor_linea = ZEPHYRUM;
    liber->cursor_columna = ZEPHYRUM;
}

vacuum
saltuarius_liber_ultimum (SaltuariusLiber* liber)
{
    i32 lineae = _lineae_activae(liber);

    si (lineae > ZEPHYRUM)
    {
        liber->cursor_linea = (s32)lineae - I;
        liber->cursor_columna = ZEPHYRUM;
    }
}

vacuum
saltuarius_liber_aptare (SaltuariusLiber* liber,
    i32 altitudo_fenestrae, i32 latitudo_fenestrae)
{
    si (altitudo_fenestrae > ZEPHYRUM)
    {
        si (liber->cursor_linea < liber->volumen_y)
        {
            liber->volumen_y = liber->cursor_linea;
        }
        si (liber->cursor_linea
            >= liber->volumen_y + (s32)altitudo_fenestrae)
        {
            liber->volumen_y = liber->cursor_linea
                - (s32)altitudo_fenestrae + I;
        }
        si (liber->volumen_y < ZEPHYRUM)
        {
            liber->volumen_y = ZEPHYRUM;
        }
    }
    si (latitudo_fenestrae > ZEPHYRUM)
    {
        si (liber->cursor_columna < liber->volumen_x)
        {
            liber->volumen_x = liber->cursor_columna;
        }
        si (liber->cursor_columna
            >= liber->volumen_x + (s32)latitudo_fenestrae)
        {
            liber->volumen_x = liber->cursor_columna
                - (s32)latitudo_fenestrae + I;
        }
        si (liber->volumen_x < ZEPHYRUM)
        {
            liber->volumen_x = ZEPHYRUM;
        }
    }
}

/* ==================================================
 * Cursor <-> offset + vestigium identitatis (C2)
 * ================================================== */

s32
saltuarius_liber_cursor_offset (constans SaltuariusLiber* liber)
{
    constans SaltuariusStratum* visus = _activum(liber);
    constans SaltuariusLinea* linea;
    constans i8* cursor;
    constans i8* finis;
    s32 runae;

    si (!visus->parata || liber->cursor_linea < ZEPHYRUM
        || liber->cursor_linea >= (s32)visus->numerus_linearum)
    {
        redde -I;
    }
    linea = &visus->lineae[liber->cursor_linea];
    cursor = visus->textus.datum + linea->offset;
    finis = cursor + linea->mensura;
    runae = liber->cursor_columna;
    dum (cursor < finis && runae > ZEPHYRUM)
    {
        cursor = utf8_proxima_runa(cursor, finis);
        runae--;
    }
    redde (s32)(i32)(memoriae_index)(cursor - visus->textus.datum);
}

vacuum
saltuarius_liber_cursor_ad_offset (SaltuariusLiber* liber,
    s32 offset)
{
    constans SaltuariusStratum* visus = _activum(liber);
    s32 linea = ZEPHYRUM;
    s32 k;

    si (!visus->parata || offset < ZEPHYRUM)
    {
        redde;
    }
    /* linea continens (ambulatus linearis - satis) */
    per (k = ZEPHYRUM; k < (s32)visus->numerus_linearum; k++)
    {
        si (offset >= (s32)visus->lineae[k].offset)
        {
            linea = k;
        }
        alioquin
        {
            frange;
        }
    }
    liber->cursor_linea = linea;
    {
        constans SaltuariusLinea* l = &visus->lineae[linea];
        constans i8* initium = visus->textus.datum + l->offset;
        constans i8* meta = visus->textus.datum + offset;
        constans i8* finis = initium + l->mensura;
        s32 columna = ZEPHYRUM;
        constans i8* cursor = initium;

        si (meta > finis)
        {
            meta = finis;
        }
        dum (cursor < meta)
        {
            cursor = utf8_proxima_runa(cursor, finis);
            columna++;
        }
        liber->cursor_columna = columna;
    }
    saltuarius_liber_movere(liber, ZEPHYRUM);
    saltuarius_liber_movere_col(liber, ZEPHYRUM);
}

/* Tegitne maiorum FONS lexematis offset datum (in plagula
 * radicali)? Catena per invocationes/corpora ambulatur. */
interior b32
_fons_tegit (constans SilvaToken* token, s32 offset)
{
    constans SilvaToken* t = token;
    i32 gradus = ZEPHYRUM;

    dum (t != NIHIL && gradus < XVI)
    {
        si (t->origo.genus == SILVA_ORIGO_FONS)
        {
            redde (t->byte_offset >= ZEPHYRUM
                && offset >= (s32)t->byte_offset
                && offset < (s32)t->byte_offset
                    + (s32)t->longitudo) ? VERUM : FALSUM;
        }
        si (t->origo.genus == SILVA_ORIGO_EXPANSIO)
        {
            /* invocatio = situs usus (latus fontis) */
            constans SilvaToken* inv = t->origo.datum.expansio
                .invocatio;

            si (inv != NIHIL
                && inv->origo.genus == SILVA_ORIGO_FONS
                && inv->byte_offset >= ZEPHYRUM
                && offset >= (s32)inv->byte_offset
                && offset < (s32)inv->byte_offset
                    + (s32)inv->longitudo)
            {
                redde VERUM;
            }
            t = inv;
        }
        alioquin si (t->origo.genus == SILVA_ORIGO_PASTA)
        {
            t = t->origo.datum.pasta.sinister;
        }
        alioquin si (t->origo.genus == SILVA_ORIGO_CHORDA)
        {
            t = t->origo.datum.stringificatio.primus;
        }
        alioquin
        {
            redde FALSUM;   /* API */
        }
        gradus++;
    }
    redde FALSUM;
}

vacuum
saltuarius_liber_stratum_transferre (SaltuariusLiber* liber,
    constans SaltuariusNexus* nexus, s32 stratum_novum)
{
    s32 offset_vetus = saltuarius_liber_cursor_offset(liber);
    s32 stratum_vetus = liber->stratum_currens;
    f64 proportio = ZEPHYRUM;

    si (_activum(liber)->parata
        && _activum(liber)->numerus_linearum > ZEPHYRUM)
    {
        proportio = (f64)liber->cursor_linea
            / (f64)_activum(liber)->numerus_linearum;
    }

    saltuarius_liber_stratum_ponere(liber, stratum_novum);
    si (liber->stratum_currens == stratum_vetus)
    {
        redde;
    }
    /* materializare si opus (accessor id facit) */
    (vacuum)saltuarius_liber_stratum(liber,
        (constans SaltuariusNexus*)nexus);

    si (offset_vetus >= ZEPHYRUM && liber->parsura != NIHIL)
    {
        constans SaltuariusStratum* novus = _activum(liber);

        si (stratum_vetus == ZEPHYRUM
            && liber->stratum_currens > ZEPHYRUM)
        {
            /* 0 -> k: primum lexema cuius maiorum offset tegit */
            i32 k;

            per (k = ZEPHYRUM; k < novus->numerus_positionum; k++)
            {
                si (_fons_tegit(novus->positiones[k].token,
                    offset_vetus))
                {
                    saltuarius_liber_cursor_ad_offset(liber,
                        (s32)novus->positiones[k].initium);
                    redde;
                }
            }
        }
        alioquin si (liber->stratum_currens == ZEPHYRUM)
        {
            /* k -> 0: maiorum FONS lexematis sub cursore */
            constans SaltuariusStratum* vetus =
                &liber->strata_visus[stratum_vetus];
            i32 k;

            per (k = ZEPHYRUM; k < vetus->numerus_positionum; k++)
            {
                si (offset_vetus
                        >= (s32)vetus->positiones[k].initium
                    && offset_vetus
                        < (s32)vetus->positiones[k].finis)
                {
                    SilvaToken* radix = silva_token_radix(
                        vetus->positiones[k].token);

                    si (radix != NIHIL
                        && radix->byte_offset >= ZEPHYRUM)
                    {
                        saltuarius_liber_cursor_ad_offset(liber,
                            (s32)radix->byte_offset);
                        redde;
                    }
                    frange;
                }
            }
        }
        alioquin
        {
            /* k -> m: radix communis */
            constans SaltuariusStratum* vetus =
                &liber->strata_visus[stratum_vetus];
            SilvaToken* radix = NIHIL;
            i32 k;

            per (k = ZEPHYRUM; k < vetus->numerus_positionum; k++)
            {
                si (offset_vetus
                        >= (s32)vetus->positiones[k].initium
                    && offset_vetus
                        < (s32)vetus->positiones[k].finis)
                {
                    radix = silva_token_radix(
                        vetus->positiones[k].token);
                    frange;
                }
            }
            si (radix != NIHIL)
            {
                per (k = ZEPHYRUM; k < novus->numerus_positionum;
                    k++)
                {
                    si (silva_token_radix(
                        novus->positiones[k].token) == radix)
                    {
                        saltuarius_liber_cursor_ad_offset(liber,
                            (s32)novus->positiones[k].initium);
                        redde;
                    }
                }
            }
        }
        /* refugium: linea proportionalis */
        {
            constans SaltuariusStratum* n = _activum(liber);

            liber->cursor_linea = (s32)(proportio
                * (f64)n->numerus_linearum);
            liber->cursor_columna = ZEPHYRUM;
            saltuarius_liber_movere(liber, ZEPHYRUM);
        }
    }
}
