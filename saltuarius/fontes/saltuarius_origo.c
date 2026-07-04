/* saltuarius_origo.c - Implementatio catenae originis */

#include "saltuarius_origo.h"
#include "chorda_aedificator.h"

SaltuariusOrigo*
saltuarius_origo_creare (Piscina* persistens)
{
    SaltuariusOrigo* origo;

    origo = (SaltuariusOrigo*)piscina_allocare_ordinatum(persistens,
        (memoriae_index)magnitudo(SaltuariusOrigo), IV);
    si (origo == NIHIL)
    {
        redde NIHIL;
    }
    origo->arena = piscina_generare_dynamicum("salt_origo", 65536);
    si (origo->arena == NIHIL)
    {
        redde NIHIL;
    }
    origo->nota = piscina_notare(origo->arena);
    origo->numerus = ZEPHYRUM;
    origo->selectio = ZEPHYRUM;
    origo->apertum = FALSUM;
    redde origo;
}

/* Chorda ex SilvaChorda (aliae - vita = vita exp) */
interior chorda
_ex_silva (constans SilvaChorda* s)
{
    chorda c;

    c.mensura = (s != NIHIL) ? s->mensura : ZEPHYRUM;
    c.datum = (s != NIHIL) ? s->datum : NIHIL;
    redde c;
}

/* Titulum ordinis aedificare: praefixum + valor + " via:linea" */
interior chorda
_titulum (SaltuariusOrigo* origo, constans character* praefixum,
    chorda medium, chorda via, i32 linea)
{
    ChordaAedificator* aed = chorda_aedificator_creare(origo->arena,
        CCLVI);
    chorda fructus;

    si (aed == NIHIL)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum = NIHIL;
        redde fructus;
    }
    chorda_aedificator_appendere_literis(aed, praefixum);
    si (medium.mensura > ZEPHYRUM)
    {
        /* praecisio ad XL runas (tabella angusta) */
        i32 mensura = (medium.mensura < XL) ? medium.mensura : XL;
        i32 k;

        per (k = ZEPHYRUM; k < mensura; k++)
        {
            chorda_aedificator_appendere_character(aed,
                (character)medium.datum[k]);
        }
    }
    si (via.mensura > ZEPHYRUM)
    {
        /* basename viae (tabella angusta) */
        s32 initium = ZEPHYRUM;
        s32 k;

        per (k = (s32)via.mensura - I; k >= ZEPHYRUM; k--)
        {
            si (via.datum[k] == '/')
            {
                initium = k + I;
                frange;
            }
        }
        chorda_aedificator_appendere_literis(aed, "  ");
        per (k = initium; k < (s32)via.mensura; k++)
        {
            chorda_aedificator_appendere_character(aed,
                (character)via.datum[k]);
        }
        chorda_aedificator_appendere_character(aed, ':');
        chorda_aedificator_appendere_i32(aed, linea);
    }
    fructus = chorda_aedificator_spectare(aed);
    redde fructus;
}

interior vacuum
_gradum_addere (SaltuariusOrigo* origo, SaltuariusGradusGenus genus,
    chorda titulus, b32 saltabile, chorda via, s32 fons_index,
    i32 linea)
{
    SaltuariusGradus* gradus;

    si (origo->numerus >= SALT_GRADUS_MAXIMI)
    {
        redde;
    }
    gradus = &origo->gradus[origo->numerus];
    gradus->genus = genus;
    gradus->titulus = titulus;
    gradus->saltabile = saltabile;
    gradus->via = via;
    gradus->fons_index = fons_index;
    gradus->linea = linea;
    origo->numerus++;
}

/* Catenam lexematis ambulare (deorsum ad FONS) et gradus addere */
interior vacuum
_catenam_ambulare (SaltuariusOrigo* origo,
    constans SilvaExpansio* exp, SilvaToken* token)
{
    SilvaToken* t = token;
    i32 profunditas = ZEPHYRUM;

    dum (t != NIHIL && profunditas < SALT_GRADUS_MAXIMI)
    {
        si (t->origo.genus == SILVA_ORIGO_FONS)
        {
            constans SilvaChorda* via = silva_fons_via(exp,
                t->fons_index);

            _gradum_addere(origo, SALT_GRADUS_SCRIPTUM,
                _titulum(origo, "scriptum  ", _ex_silva(NIHIL),
                    _ex_silva(via), t->linea),
                (via != NIHIL) ? VERUM : FALSUM, _ex_silva(via),
                t->fons_index, t->linea);
            redde;
        }
        si (t->origo.genus == SILVA_ORIGO_EXPANSIO)
        {
            SilvaToken* corpus = t->origo.datum.expansio.corpus;
            constans SilvaChorda* via = NIHIL;
            i32 linea = ZEPHYRUM;
            s32 fons = -I;

            si (corpus != NIHIL)
            {
                via = silva_fons_via(exp, corpus->fons_index);
                linea = corpus->linea;
                fons = corpus->fons_index;
            }
            _gradum_addere(origo, SALT_GRADUS_EXPANSIO,
                _titulum(origo, "<- expansio ",
                    _ex_silva(t->origo.datum.expansio.nomen_macro),
                    _ex_silva(via), linea),
                (via != NIHIL) ? VERUM : FALSUM, _ex_silva(via),
                fons, linea);
            t = t->origo.datum.expansio.invocatio;
        }
        alioquin si (t->origo.genus == SILVA_ORIGO_PASTA)
        {
            _gradum_addere(origo, SALT_GRADUS_ALIUS,
                _titulum(origo, "<- pasta ##",
                    _ex_silva(t->origo.datum.pasta.nomen_macro),
                    _ex_silva(NIHIL), ZEPHYRUM),
                FALSUM, _ex_silva(NIHIL), -I, ZEPHYRUM);
            t = t->origo.datum.pasta.sinister;
        }
        alioquin si (t->origo.genus == SILVA_ORIGO_CHORDA)
        {
            _gradum_addere(origo, SALT_GRADUS_ALIUS,
                _titulum(origo, "<- chorda #",
                    _ex_silva(t->origo.datum.stringificatio
                        .nomen_macro),
                    _ex_silva(NIHIL), ZEPHYRUM),
                FALSUM, _ex_silva(NIHIL), -I, ZEPHYRUM);
            t = t->origo.datum.stringificatio.primus;
        }
        alioquin
        {
            _gradum_addere(origo, SALT_GRADUS_ALIUS,
                _titulum(origo, "<- api", _ex_silva(NIHIL),
                    _ex_silva(NIHIL), ZEPHYRUM),
                FALSUM, _ex_silva(NIHIL), -I, ZEPHYRUM);
            redde;
        }
        profunditas++;
    }
}

/* Tegitne maiorum FONS lexematis offset (quaestio prorsum strati 0) */
interior b32
_maiorum_tegit (SilvaToken* token, s32 offset)
{
    SilvaToken* t = token;
    i32 gradus = ZEPHYRUM;

    dum (t != NIHIL && gradus < SALT_GRADUS_MAXIMI)
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
            SilvaToken* inv = t->origo.datum.expansio.invocatio;

            si (inv != NIHIL && inv->origo.genus == SILVA_ORIGO_FONS
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
            redde FALSUM;
        }
        gradus++;
    }
    redde FALSUM;
}

b32
saltuarius_origo_aedificare (SaltuariusOrigo* origo,
    SaltuariusLiber* liber, constans SaltuariusNexus* nexus)
{
    s32 offset;
    constans SilvaExpansio* exp;

    piscina_reficere(origo->arena, origo->nota);
    origo->numerus = ZEPHYRUM;
    origo->selectio = ZEPHYRUM;
    origo->apertum = FALSUM;

    si (liber->parsura == NIHIL
        || liber->parsura->expansio == NIHIL)
    {
        redde FALSUM;
    }
    exp = liber->parsura->expansio;
    offset = saltuarius_liber_cursor_offset(liber);
    si (offset < ZEPHYRUM)
    {
        redde FALSUM;
    }

    si (liber->stratum_currens == ZEPHYRUM)
    {
        /* PRORSUM: primum lexema finale cuius maiorum tegit */
        SilvaXar* fluxus = liber->parsura->lexemata;
        i32 n = silva_xar_numerus(fluxus);
        i32 k;
        SilvaToken* inventum = NIHIL;

        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaToken* t = *(SilvaToken**)silva_xar_obtinere(
                fluxus, k);

            /* solum expansa (FONS directa trivialia sunt) */
            si (t->origo.genus != SILVA_ORIGO_FONS
                && _maiorum_tegit(t, offset))
            {
                inventum = t;
                frange;
            }
        }
        si (inventum == NIHIL)
        {
            redde FALSUM;
        }
        _gradum_addere(origo, SALT_GRADUS_FIT,
            _titulum(origo, "fit  ", _ex_silva(&inventum->valor),
                _ex_silva(NIHIL), ZEPHYRUM),
            FALSUM, _ex_silva(NIHIL), -I, ZEPHYRUM);
        _catenam_ambulare(origo, exp, inventum);
    }
    alioquin
    {
        /* DEORSUM: lexema sub cursore ex mappa positionum */
        constans SaltuariusStratum* visus = saltuarius_liber_stratum(
            liber, nexus);
        SilvaToken* token = NIHIL;
        i32 k;

        per (k = ZEPHYRUM; k < visus->numerus_positionum; k++)
        {
            si (offset >= (s32)visus->positiones[k].initium
                && offset < (s32)visus->positiones[k].finis)
            {
                token = visus->positiones[k].token;
                frange;
            }
        }
        si (token == NIHIL)
        {
            redde FALSUM;
        }
        _gradum_addere(origo, SALT_GRADUS_CAPUT,
            _titulum(origo, "lexema  ", _ex_silva(&token->valor),
                _ex_silva(NIHIL), ZEPHYRUM),
            FALSUM, _ex_silva(NIHIL), -I, ZEPHYRUM);
        _catenam_ambulare(origo, exp, token);
    }

    si (origo->numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    origo->apertum = VERUM;
    redde VERUM;
}

vacuum
saltuarius_origo_claudere (SaltuariusOrigo* origo)
{
    origo->apertum = FALSUM;
}

vacuum
saltuarius_origo_movere (SaltuariusOrigo* origo, s32 delta)
{
    s32 nova = origo->selectio + delta;

    si (nova < ZEPHYRUM)
    {
        nova = ZEPHYRUM;
    }
    si (nova >= (s32)origo->numerus)
    {
        nova = (s32)origo->numerus - I;
    }
    origo->selectio = nova;
}

constans SaltuariusGradus*
saltuarius_origo_saltus (constans SaltuariusOrigo* origo)
{
    si (!origo->apertum || origo->selectio < ZEPHYRUM
        || origo->selectio >= (s32)origo->numerus)
    {
        redde NIHIL;
    }
    si (!origo->gradus[origo->selectio].saltabile)
    {
        redde NIHIL;
    }
    redde &origo->gradus[origo->selectio];
}
