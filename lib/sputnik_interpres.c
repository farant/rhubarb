/* sputnik_interpres.c - Interpres pro Sputnik
 *
 * Tree-walking evaluator pro AST Sputnik.
 */

#include "sputnik_interpres.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


/* ==================================================
 * Prototypi Interni
 * ================================================== */

interior SputnikValor _evaluare_nodum(SputnikInterpres* interp, SputnikAstNodus* nodus);
interior vacuum _executare_sententiam(SputnikInterpres* interp, SputnikAstNodus* nodus);
interior vacuum _exec_sententiam_frange(SputnikInterpres* interp, SputnikAstNodus* nodus);
interior vacuum _exec_sententiam_perge(SputnikInterpres* interp, SputnikAstNodus* nodus);
interior vacuum _exec_sententiam_incrementum(SputnikInterpres* interp, SputnikAstNodus* nodus);
interior vacuum _exec_sententiam_decrementum(SputnikInterpres* interp, SputnikAstNodus* nodus);


/* ==================================================
 * Adiutores - Value Creation
 * ================================================== */

interior SputnikValor
_valor_nihil(vacuum)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_NIHIL;
    v.ut.numerus = 0.0;
    redde v;
}

interior SputnikValor
_valor_numerus(f64 n)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_NUMERUS;
    v.ut.numerus = n;
    redde v;
}

interior SputnikValor
_valor_pecunia(s64 centesimi)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_PECUNIA;
    v.ut.pecunia = centesimi;
    redde v;
}

interior SputnikValor
_valor_chorda(chorda c)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_CHORDA;
    v.ut.chorda_valor = c;
    redde v;
}

interior SputnikValor
_valor_verum(vacuum)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_VERUM;
    v.ut.numerus = 1.0;
    redde v;
}

interior SputnikValor
_valor_falsum(vacuum)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_FALSUM;
    v.ut.numerus = 0.0;
    redde v;
}

interior SputnikValor
_valor_xar(Xar* x)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_XAR;
    v.ut.xar = x;
    redde v;
}

interior SputnikValor
_valor_objectum(TabulaDispersa* t)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_OBJECTUM;
    v.ut.objectum = t;
    redde v;
}

interior SputnikValor
_valor_functio(SputnikAstNodus* f)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_FUNCTIO;
    v.ut.functio = f;
    redde v;
}

interior SputnikValor
_valor_methodus_xar(Xar* xar, chorda titulus)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_METHODUS_XAR;
    v.ut.methodus_xar.xar = xar;
    v.ut.methodus_xar.titulus = titulus;
    redde v;
}

interior SputnikValor
_valor_entitas(Entitas* e)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_ENTITAS;
    v.ut.entitas = e;
    redde v;
}

interior SputnikValor
_valor_methodus_entitas(Entitas* entitas, chorda titulus)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_METHODUS_ENTITAS;
    v.ut.methodus_entitas.entitas = entitas;
    v.ut.methodus_entitas.titulus = titulus;
    redde v;
}

interior SputnikValor
_valor_repositorium(EntitasRepositorium* repo)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_REPOSITORIUM;
    v.ut.repositorium = repo;
    redde v;
}

interior SputnikValor
_valor_methodus_repositorium(EntitasRepositorium* repo, chorda titulus)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_METHODUS_REPOSITORIUM;
    v.ut.methodus_repositorium.repositorium = repo;
    v.ut.methodus_repositorium.titulus = titulus;
    redde v;
}

interior SputnikValor
_valor_methodus_chorda(chorda chorda_valor, chorda titulus)
{
    SputnikValor v;
    v.genus = SPUTNIK_VALOR_METHODUS_CHORDA;
    v.ut.methodus_chorda.chorda_valor = chorda_valor;
    v.ut.methodus_chorda.titulus = titulus;
    redde v;
}


/* ==================================================
 * Adiutores - Truthiness
 * ================================================== */

interior b32
_est_verum(SputnikValor* valor)
{
    commutatio (valor->genus)
    {
        casus SPUTNIK_VALOR_NIHIL:
        casus SPUTNIK_VALOR_FALSUM:
            redde FALSUM;

        casus SPUTNIK_VALOR_NUMERUS:
            redde valor->ut.numerus != 0.0;

        casus SPUTNIK_VALOR_CHORDA:
            redde valor->ut.chorda_valor.mensura > ZEPHYRUM;

        casus SPUTNIK_VALOR_VERUM:
            redde VERUM;

        casus SPUTNIK_VALOR_XAR:
        casus SPUTNIK_VALOR_OBJECTUM:
        casus SPUTNIK_VALOR_FUNCTIO:
        casus SPUTNIK_VALOR_METHODUS_XAR:
        casus SPUTNIK_VALOR_METHODUS_CHORDA:
        casus SPUTNIK_VALOR_ENTITAS:
        casus SPUTNIK_VALOR_METHODUS_ENTITAS:
        casus SPUTNIK_VALOR_REPOSITORIUM:
        casus SPUTNIK_VALOR_METHODUS_REPOSITORIUM:
            redde VERUM;

        ordinarius:
            redde FALSUM;
    }
}


/* ==================================================
 * Adiutores - Error Handling
 * ================================================== */

interior vacuum
_error(SputnikInterpres* interp, SputnikAstNodus* nodus, constans character* nuntius)
{
    si (interp->error_accidit)
    {
        redde;  /* Solum primus error */
    }

    interp->error_accidit = VERUM;
    interp->error_nuntius = chorda_ex_literis(nuntius, interp->piscina);

    si (nodus != NIHIL)
    {
        interp->error_linea = nodus->linea;
        interp->error_columna = nodus->columna;
    }
    alioquin
    {
        interp->error_linea = ZEPHYRUM;
        interp->error_columna = ZEPHYRUM;
    }
}


/* ==================================================
 * Adiutores - Scope Management
 * ================================================== */

interior SputnikAmbitus*
_creare_ambitum(SputnikInterpres* interp, SputnikAmbitus* parens)
{
    SputnikAmbitus* ambitus;

    ambitus = piscina_allocare(interp->piscina, magnitudo(SputnikAmbitus));
    si (ambitus == NIHIL)
    {
        redde NIHIL;
    }

    ambitus->variabiles = tabula_dispersa_creare_chorda(interp->piscina, XXXII);
    ambitus->constantes = tabula_dispersa_creare_chorda(interp->piscina, XVI);
    ambitus->parens = parens;

    redde ambitus;
}

interior vacuum
_intrare_ambitum(SputnikInterpres* interp)
{
    SputnikAmbitus* novus;

    novus = _creare_ambitum(interp, interp->ambitus_currens);
    si (novus != NIHIL)
    {
        interp->ambitus_currens = novus;
    }
}

interior vacuum
_exire_ambitum(SputnikInterpres* interp)
{
    si (interp->ambitus_currens != NIHIL &&
        interp->ambitus_currens->parens != NIHIL)
    {
        interp->ambitus_currens = interp->ambitus_currens->parens;
    }
}


/* ==================================================
 * Adiutores - Variable Operations
 * ================================================== */

interior b32
_definire_variabilem(SputnikInterpres* interp, chorda titulus, SputnikValor valor, b32 est_constans)
{
    SputnikValor* valor_copia;
    b32* const_flag;

    si (interp->ambitus_currens == NIHIL)
    {
        redde FALSUM;
    }

    /* Allocare copiam valoris */
    valor_copia = piscina_allocare(interp->piscina, magnitudo(SputnikValor));
    si (valor_copia == NIHIL)
    {
        redde FALSUM;
    }
    *valor_copia = valor;

    /* Inserere in variabiles */
    si (!tabula_dispersa_inserere(interp->ambitus_currens->variabiles, titulus, valor_copia))
    {
        redde FALSUM;
    }

    /* Si constans, notare */
    si (est_constans)
    {
        const_flag = piscina_allocare(interp->piscina, magnitudo(b32));
        si (const_flag != NIHIL)
        {
            *const_flag = VERUM;
            tabula_dispersa_inserere(interp->ambitus_currens->constantes, titulus, const_flag);
        }
    }

    redde VERUM;
}

interior SputnikValor*
_invenire_variabilem(SputnikInterpres* interp, chorda titulus)
{
    SputnikAmbitus* ambitus;
    vacuum* valor_ptr;

    ambitus = interp->ambitus_currens;
    dum (ambitus != NIHIL)
    {
        si (tabula_dispersa_invenire(ambitus->variabiles, titulus, &valor_ptr))
        {
            redde (SputnikValor*)valor_ptr;
        }
        ambitus = ambitus->parens;
    }

    redde NIHIL;
}

interior b32
_est_constans(SputnikInterpres* interp, chorda titulus)
{
    SputnikAmbitus* ambitus;
    vacuum* flag_ptr;

    ambitus = interp->ambitus_currens;
    dum (ambitus != NIHIL)
    {
        si (tabula_dispersa_invenire(ambitus->constantes, titulus, &flag_ptr))
        {
            redde VERUM;
        }
        /* Si variabilis in hoc ambitu, non constans */
        si (tabula_dispersa_continet(ambitus->variabiles, titulus))
        {
            redde FALSUM;
        }
        ambitus = ambitus->parens;
    }

    redde FALSUM;
}

interior b32
_assignare_variabilem(SputnikInterpres* interp, chorda titulus, SputnikValor valor)
{
    SputnikAmbitus* ambitus;
    vacuum* valor_ptr;
    SputnikValor* existens;

    ambitus = interp->ambitus_currens;
    dum (ambitus != NIHIL)
    {
        si (tabula_dispersa_invenire(ambitus->variabiles, titulus, &valor_ptr))
        {
            existens = (SputnikValor*)valor_ptr;
            *existens = valor;
            redde VERUM;
        }
        ambitus = ambitus->parens;
    }

    redde FALSUM;
}


/* ==================================================
 * Adiutores - Type Conversion
 * ================================================== */

interior chorda
_ad_chordam(SputnikInterpres* interp, SputnikValor* valor)
{
    ChordaAedificator* aed;
    chorda resultus;
    i32 i;
    i32 num;
    SputnikValor* elem;
    TabulaIterator iter;
    chorda clavis;
    vacuum* val_ptr;
    b32 primum;

    commutatio (valor->genus)
    {
        casus SPUTNIK_VALOR_NIHIL:
            redde chorda_ex_literis("null", interp->piscina);

        casus SPUTNIK_VALOR_VERUM:
            redde chorda_ex_literis("true", interp->piscina);

        casus SPUTNIK_VALOR_FALSUM:
            redde chorda_ex_literis("false", interp->piscina);

        casus SPUTNIK_VALOR_NUMERUS:
            aed = chorda_aedificator_creare(interp->piscina, XXXII);
            si (aed == NIHIL)
            {
                redde chorda_ex_literis("", interp->piscina);
            }
            /* Si integer, non ostendere decimales */
            si (floor(valor->ut.numerus) == valor->ut.numerus &&
                valor->ut.numerus >= -1e15 && valor->ut.numerus <= 1e15)
            {
                chorda_aedificator_appendere_s32(aed, (s32)valor->ut.numerus);
            }
            alioquin
            {
                chorda_aedificator_appendere_f64(aed, valor->ut.numerus, VI);
            }
            redde chorda_aedificator_finire(aed);

        casus SPUTNIK_VALOR_PECUNIA:
            {
                s64 centesimi;
                s64 pars_integra;
                s64 pars_decimalis;
                b32 negativus;

                centesimi = valor->ut.pecunia;
                negativus = centesimi < ZEPHYRUM;
                si (negativus)
                {
                    centesimi = -centesimi;
                }

                pars_integra = centesimi / C;
                pars_decimalis = centesimi % C;

                aed = chorda_aedificator_creare(interp->piscina, XXXII);
                si (aed == NIHIL)
                {
                    redde chorda_ex_literis("0.00$", interp->piscina);
                }

                si (negativus)
                {
                    chorda_aedificator_appendere_character(aed, '-');
                }
                chorda_aedificator_appendere_s32(aed, (s32)pars_integra);
                chorda_aedificator_appendere_character(aed, '.');
                si (pars_decimalis < X)
                {
                    chorda_aedificator_appendere_character(aed, '0');
                }
                chorda_aedificator_appendere_s32(aed, (s32)pars_decimalis);
                chorda_aedificator_appendere_character(aed, '$');

                redde chorda_aedificator_finire(aed);
            }

        casus SPUTNIK_VALOR_CHORDA:
            redde valor->ut.chorda_valor;

        casus SPUTNIK_VALOR_XAR:
            aed = chorda_aedificator_creare(interp->piscina, CXXVIII);
            si (aed == NIHIL)
            {
                redde chorda_ex_literis("[]", interp->piscina);
            }
            chorda_aedificator_appendere_character(aed, '[');
            num = xar_numerus(valor->ut.xar);
            per (i = ZEPHYRUM; i < num; i++)
            {
                si (i > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_literis(aed, ", ");
                }
                elem = xar_obtinere(valor->ut.xar, i);
                resultus = _ad_chordam(interp, elem);
                si (elem->genus == SPUTNIK_VALOR_CHORDA)
                {
                    chorda_aedificator_appendere_character(aed, '"');
                    chorda_aedificator_appendere_chorda(aed, resultus);
                    chorda_aedificator_appendere_character(aed, '"');
                }
                alioquin
                {
                    chorda_aedificator_appendere_chorda(aed, resultus);
                }
            }
            chorda_aedificator_appendere_character(aed, ']');
            redde chorda_aedificator_finire(aed);

        casus SPUTNIK_VALOR_OBJECTUM:
            aed = chorda_aedificator_creare(interp->piscina, CXXVIII);
            si (aed == NIHIL)
            {
                redde chorda_ex_literis("{}", interp->piscina);
            }
            chorda_aedificator_appendere_character(aed, '{');
            iter = tabula_dispersa_iterator_initium(valor->ut.objectum);
            primum = VERUM;
            dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &val_ptr))
            {
                si (!primum)
                {
                    chorda_aedificator_appendere_literis(aed, ", ");
                }
                primum = FALSUM;
                chorda_aedificator_appendere_chorda(aed, clavis);
                chorda_aedificator_appendere_literis(aed, ": ");
                elem = (SputnikValor*)val_ptr;
                resultus = _ad_chordam(interp, elem);
                si (elem->genus == SPUTNIK_VALOR_CHORDA)
                {
                    chorda_aedificator_appendere_character(aed, '"');
                    chorda_aedificator_appendere_chorda(aed, resultus);
                    chorda_aedificator_appendere_character(aed, '"');
                }
                alioquin
                {
                    chorda_aedificator_appendere_chorda(aed, resultus);
                }
            }
            chorda_aedificator_appendere_character(aed, '}');
            redde chorda_aedificator_finire(aed);

        casus SPUTNIK_VALOR_FUNCTIO:
            redde chorda_ex_literis("[function]", interp->piscina);

        casus SPUTNIK_VALOR_METHODUS_XAR:
            redde chorda_ex_literis("[array method]", interp->piscina);

        casus SPUTNIK_VALOR_METHODUS_CHORDA:
            redde chorda_ex_literis("[string method]", interp->piscina);

        casus SPUTNIK_VALOR_ENTITAS:
            /* Formattare ut [Entitas:genus:id] */
            aed = chorda_aedificator_creare(interp->piscina, CXXVIII);
            si (aed == NIHIL)
            {
                redde chorda_ex_literis("[Entitas]", interp->piscina);
            }
            chorda_aedificator_appendere_literis(aed, "[Entitas:");
            si (valor->ut.entitas != NIHIL)
            {
                chorda_aedificator_appendere_chorda(aed, *valor->ut.entitas->genus);
                chorda_aedificator_appendere_character(aed, ':');
                chorda_aedificator_appendere_chorda(aed, *valor->ut.entitas->id);
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(aed, "nihil");
            }
            chorda_aedificator_appendere_character(aed, ']');
            redde chorda_aedificator_finire(aed);

        casus SPUTNIK_VALOR_METHODUS_ENTITAS:
            redde chorda_ex_literis("[entity method]", interp->piscina);

        casus SPUTNIK_VALOR_REPOSITORIUM:
            redde chorda_ex_literis("[Repositorium]", interp->piscina);

        casus SPUTNIK_VALOR_METHODUS_REPOSITORIUM:
            redde chorda_ex_literis("[repository method]", interp->piscina);

        ordinarius:
            redde chorda_ex_literis("", interp->piscina);
    }
}


/* ==================================================
 * Built-in Functions
 * ================================================== */

interior SputnikValor
_intrinseca_print(SputnikInterpres* interp, Xar* argumenta)
{
    i32 i;
    i32 num;
    SputnikValor* arg;
    chorda s;

    num = xar_numerus(argumenta);
    per (i = ZEPHYRUM; i < num; i++)
    {
        si (i > ZEPHYRUM)
        {
            printf(" ");
        }
        arg = xar_obtinere(argumenta, i);
        s = _ad_chordam(interp, arg);
        printf("%.*s", s.mensura, s.datum);
    }
    printf("\n");

    redde _valor_nihil();
}

interior SputnikValor
_intrinseca_len(SputnikInterpres* interp, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* arg;

    si (xar_numerus(argumenta) != I)
    {
        _error(interp, nodus, "len() requirit exacte I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);

    commutatio (arg->genus)
    {
        casus SPUTNIK_VALOR_CHORDA:
            redde _valor_numerus((f64)arg->ut.chorda_valor.mensura);

        casus SPUTNIK_VALOR_XAR:
            redde _valor_numerus((f64)xar_numerus(arg->ut.xar));

        casus SPUTNIK_VALOR_OBJECTUM:
            redde _valor_numerus((f64)tabula_dispersa_numerus(arg->ut.objectum));

        ordinarius:
            _error(interp, nodus, "len() non supportat hunc genus valoris");
            redde _valor_nihil();
    }
}

interior SputnikValor
_intrinseca_typeof(SputnikInterpres* interp, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* arg;
    constans character* genus_nomen;

    si (xar_numerus(argumenta) != I)
    {
        _error(interp, nodus, "typeof() requirit exacte I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);
    genus_nomen = sputnik_valor_genus_nomen(arg->genus);

    redde _valor_chorda(chorda_ex_literis(genus_nomen, interp->piscina));
}

interior b32
_est_intrinseca(chorda titulus)
{
    si (chorda_aequalis_literis(titulus, "print"))
    {
        redde VERUM;
    }
    si (chorda_aequalis_literis(titulus, "len"))
    {
        redde VERUM;
    }
    si (chorda_aequalis_literis(titulus, "typeof"))
    {
        redde VERUM;
    }
    redde FALSUM;
}

interior SputnikValor
_vocare_intrinsecam(SputnikInterpres* interp, chorda titulus, Xar* argumenta, SputnikAstNodus* nodus)
{
    si (chorda_aequalis_literis(titulus, "print"))
    {
        redde _intrinseca_print(interp, argumenta);
    }
    si (chorda_aequalis_literis(titulus, "len"))
    {
        redde _intrinseca_len(interp, argumenta, nodus);
    }
    si (chorda_aequalis_literis(titulus, "typeof"))
    {
        redde _intrinseca_typeof(interp, argumenta, nodus);
    }

    _error(interp, nodus, "Functio intrinseca ignota");
    redde _valor_nihil();
}


/* ==================================================
 * Array Methods
 * ================================================== */

interior b32
_est_methodus_xar(chorda titulus)
{
    /* Methodi simplices (sine callback) */
    si (chorda_aequalis_literis(titulus, "length"))  redde VERUM;
    si (chorda_aequalis_literis(titulus, "push"))    redde VERUM;
    si (chorda_aequalis_literis(titulus, "pop"))     redde VERUM;
    si (chorda_aequalis_literis(titulus, "shift"))   redde VERUM;
    si (chorda_aequalis_literis(titulus, "unshift")) redde VERUM;
    si (chorda_aequalis_literis(titulus, "indexOf")) redde VERUM;
    si (chorda_aequalis_literis(titulus, "includes")) redde VERUM;
    si (chorda_aequalis_literis(titulus, "slice"))   redde VERUM;
    si (chorda_aequalis_literis(titulus, "concat"))  redde VERUM;
    si (chorda_aequalis_literis(titulus, "join"))    redde VERUM;
    /* Methodi cum callback */
    si (chorda_aequalis_literis(titulus, "map"))     redde VERUM;
    si (chorda_aequalis_literis(titulus, "filter"))  redde VERUM;
    si (chorda_aequalis_literis(titulus, "find"))    redde VERUM;
    si (chorda_aequalis_literis(titulus, "forEach")) redde VERUM;
    redde FALSUM;
}

/* Verificare si titulus est methodus chorda */
interior b32
_est_methodus_chorda(chorda titulus)
{
    si (chorda_aequalis_literis(titulus, "length"))     redde VERUM;
    si (chorda_aequalis_literis(titulus, "charAt"))     redde VERUM;
    si (chorda_aequalis_literis(titulus, "indexOf"))    redde VERUM;
    si (chorda_aequalis_literis(titulus, "includes"))   redde VERUM;
    si (chorda_aequalis_literis(titulus, "substring"))  redde VERUM;
    si (chorda_aequalis_literis(titulus, "toUpperCase")) redde VERUM;
    si (chorda_aequalis_literis(titulus, "toLowerCase")) redde VERUM;
    si (chorda_aequalis_literis(titulus, "split"))      redde VERUM;
    redde FALSUM;
}

/* length() - redde longitudinem xar */
interior SputnikValor
_methodus_xar_length(Xar* xar)
{
    redde _valor_numerus((f64)xar_numerus(xar));
}

/* push(item) - addere ad finem, redde novam longitudinem */
interior SputnikValor
_methodus_xar_push(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 i;
    i32 num;
    SputnikValor* arg;
    SputnikValor* elem;

    num = xar_numerus(argumenta);
    si (num < I)
    {
        _error(interp, nodus, "push() requirit minimum I argumentum");
        redde _valor_nihil();
    }

    per (i = ZEPHYRUM; i < num; i++)
    {
        arg = xar_obtinere(argumenta, i);
        elem = xar_addere(xar);
        si (elem == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *elem = *arg;
    }

    redde _valor_numerus((f64)xar_numerus(xar));
}

/* pop() - removere ab fine, redde elementum remotum */
interior SputnikValor
_methodus_xar_pop(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 num;
    SputnikValor* elem;
    SputnikValor resultus;

    (vacuum)argumenta;

    num = xar_numerus(xar);
    si (num == ZEPHYRUM)
    {
        _error(interp, nodus, "Non potest pop() ex xar vacuo");
        redde _valor_nihil();
    }

    elem = xar_obtinere(xar, num - I);
    resultus = *elem;
    xar_removere_ultimum(xar);

    redde resultus;
}

/* shift() - removere a principio, redde elementum remotum */
interior SputnikValor
_methodus_xar_shift(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 num;
    i32 i;
    SputnikValor* elem;
    SputnikValor* next;
    SputnikValor resultus;

    (vacuum)argumenta;

    num = xar_numerus(xar);
    si (num == ZEPHYRUM)
    {
        _error(interp, nodus, "Non potest shift() ex xar vacuo");
        redde _valor_nihil();
    }

    elem = xar_obtinere(xar, ZEPHYRUM);
    resultus = *elem;

    /* Movere omnia elementa sinistrorsum */
    per (i = ZEPHYRUM; i < num - I; i++)
    {
        elem = xar_obtinere(xar, i);
        next = xar_obtinere(xar, i + I);
        *elem = *next;
    }
    xar_removere_ultimum(xar);

    redde resultus;
}

/* unshift(item) - addere ad principium, redde novam longitudinem */
interior SputnikValor
_methodus_xar_unshift(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 num;
    i32 i;
    SputnikValor* arg;
    SputnikValor* elem;
    SputnikValor* next;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "unshift() requirit minimum I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);

    /* Addere spatium ad finem */
    elem = xar_addere(xar);
    si (elem == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    /* Movere omnia elementa dextrorsum */
    num = xar_numerus(xar);
    per (i = num - I; i > ZEPHYRUM; i--)
    {
        elem = xar_obtinere(xar, i);
        next = xar_obtinere(xar, i - I);
        *elem = *next;
    }

    /* Inserere novum elementum ad principium */
    elem = xar_obtinere(xar, ZEPHYRUM);
    *elem = *arg;

    redde _valor_numerus((f64)xar_numerus(xar));
}

/* indexOf(item) - invenire indicem, redde -1 si non inventum */
interior SputnikValor
_methodus_xar_indexOf(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 i;
    i32 num;
    SputnikValor* arg;
    SputnikValor* elem;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "indexOf() requirit I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);
    num = xar_numerus(xar);

    per (i = ZEPHYRUM; i < num; i++)
    {
        elem = xar_obtinere(xar, i);

        /* Comparatio secundum genus */
        si (elem->genus == arg->genus)
        {
            commutatio (elem->genus)
            {
                casus SPUTNIK_VALOR_NIHIL:
                    redde _valor_numerus((f64)i);
                casus SPUTNIK_VALOR_VERUM:
                casus SPUTNIK_VALOR_FALSUM:
                    redde _valor_numerus((f64)i);
                casus SPUTNIK_VALOR_NUMERUS:
                    si (elem->ut.numerus == arg->ut.numerus)
                        redde _valor_numerus((f64)i);
                    frange;
                casus SPUTNIK_VALOR_CHORDA:
                    si (chorda_aequalis(elem->ut.chorda_valor, arg->ut.chorda_valor))
                        redde _valor_numerus((f64)i);
                    frange;
                ordinarius:
                    /* Reference equality */
                    si (elem->ut.xar == arg->ut.xar)
                        redde _valor_numerus((f64)i);
                    frange;
            }
        }
    }

    redde _valor_numerus(-1.0);
}

/* includes(item) - verificare si continet, redde boolean */
interior SputnikValor
_methodus_xar_includes(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor idx;
    idx = _methodus_xar_indexOf(interp, xar, argumenta, nodus);
    si (interp->error_accidit)
        redde _valor_nihil();
    redde idx.ut.numerus >= 0.0 ? _valor_verum() : _valor_falsum();
}

/* slice(start, end?) - extrahere portionem, redde novum xar */
interior SputnikValor
_methodus_xar_slice(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 start;
    i32 end;
    i32 num;
    i32 i;
    Xar* novus;
    SputnikValor* arg;
    SputnikValor* elem;
    SputnikValor* copia;

    num = xar_numerus(xar);

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "slice() requirit minimum I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (arg->genus != SPUTNIK_VALOR_NUMERUS)
    {
        _error(interp, nodus, "slice() requirit numerum pro start");
        redde _valor_nihil();
    }
    start = (i32)arg->ut.numerus;

    /* Default end est longitudo */
    end = num;
    si (xar_numerus(argumenta) >= II)
    {
        arg = xar_obtinere(argumenta, I);
        si (arg->genus != SPUTNIK_VALOR_NUMERUS)
        {
            _error(interp, nodus, "slice() requirit numerum pro end");
            redde _valor_nihil();
        }
        end = (i32)arg->ut.numerus;
    }

    /* Tractare indices negativos */
    si (start < ZEPHYRUM) start = num + start;
    si (end < ZEPHYRUM) end = num + end;
    si (start < ZEPHYRUM) start = ZEPHYRUM;
    si (end > num) end = num;
    si (start > end) start = end;

    novus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (novus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    per (i = start; i < end; i++)
    {
        elem = xar_obtinere(xar, i);
        copia = xar_addere(novus);
        si (copia == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *copia = *elem;
    }

    redde _valor_xar(novus);
}

/* concat(other) - coniungere arrays, redde novum xar */
interior SputnikValor
_methodus_xar_concat(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 i;
    i32 num;
    Xar* novus;
    SputnikValor* arg;
    SputnikValor* elem;
    SputnikValor* copia;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "concat() requirit I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (arg->genus != SPUTNIK_VALOR_XAR)
    {
        _error(interp, nodus, "concat() requirit xar ut argumentum");
        redde _valor_nihil();
    }

    novus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (novus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    /* Copiare elementa ex primo xar */
    num = xar_numerus(xar);
    per (i = ZEPHYRUM; i < num; i++)
    {
        elem = xar_obtinere(xar, i);
        copia = xar_addere(novus);
        si (copia == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *copia = *elem;
    }

    /* Copiare elementa ex secundo xar */
    num = xar_numerus(arg->ut.xar);
    per (i = ZEPHYRUM; i < num; i++)
    {
        elem = xar_obtinere(arg->ut.xar, i);
        copia = xar_addere(novus);
        si (copia == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *copia = *elem;
    }

    redde _valor_xar(novus);
}

/* join(separator?) - coniungere ut chorda */
interior SputnikValor
_methodus_xar_join(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 i;
    i32 num;
    chorda separator;
    chorda elem_str;
    ChordaAedificator* aed;
    SputnikValor* arg;
    SputnikValor* elem;

    (vacuum)nodus;

    /* Default separator est "," */
    separator = chorda_ex_literis(",", interp->piscina);
    si (xar_numerus(argumenta) >= I)
    {
        arg = xar_obtinere(argumenta, ZEPHYRUM);
        si (arg->genus == SPUTNIK_VALOR_CHORDA)
        {
            separator = arg->ut.chorda_valor;
        }
    }

    aed = chorda_aedificator_creare(interp->piscina, CXXVIII);
    si (aed == NIHIL)
    {
        redde _valor_chorda(chorda_ex_literis("", interp->piscina));
    }

    num = xar_numerus(xar);
    per (i = ZEPHYRUM; i < num; i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(aed, separator);
        }
        elem = xar_obtinere(xar, i);
        elem_str = _ad_chordam(interp, elem);
        chorda_aedificator_appendere_chorda(aed, elem_str);
    }

    redde _valor_chorda(chorda_aedificator_finire(aed));
}

/* Forward declaration - needed for callback methods */
interior SputnikValor _vocare_functionem(SputnikInterpres*, SputnikAstNodus*, Xar*, SputnikAstNodus*);

/* map(fn) - transform each element */
interior SputnikValor
_methodus_xar_map(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 i;
    i32 num;
    SputnikValor* fn_arg;
    SputnikValor* elem;
    SputnikValor* copia;
    SputnikValor resultus;
    Xar* novus;
    Xar* args;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "map() requirit functionem");
        redde _valor_nihil();
    }

    fn_arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (fn_arg->genus != SPUTNIK_VALOR_FUNCTIO)
    {
        _error(interp, nodus, "map() requirit functionem ut argumentum");
        redde _valor_nihil();
    }

    novus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (novus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    args = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (args == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    num = xar_numerus(xar);
    per (i = ZEPHYRUM; i < num; i++)
    {
        elem = xar_obtinere(xar, i);

        /* Preparare argumenta pro callback: (element) */
        xar_vacare(args);
        copia = xar_addere(args);
        *copia = *elem;

        /* Vocare callback */
        resultus = _vocare_functionem(interp, fn_arg->ut.functio, args, nodus);
        si (interp->error_accidit)
        {
            redde _valor_nihil();
        }

        /* Addere resultum ad novum array */
        copia = xar_addere(novus);
        si (copia == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *copia = resultus;
    }

    redde _valor_xar(novus);
}

/* filter(fn) - keep elements where fn returns true */
interior SputnikValor
_methodus_xar_filter(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 i;
    i32 num;
    SputnikValor* fn_arg;
    SputnikValor* elem;
    SputnikValor* copia;
    SputnikValor resultus;
    Xar* novus;
    Xar* args;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "filter() requirit functionem");
        redde _valor_nihil();
    }

    fn_arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (fn_arg->genus != SPUTNIK_VALOR_FUNCTIO)
    {
        _error(interp, nodus, "filter() requirit functionem ut argumentum");
        redde _valor_nihil();
    }

    novus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (novus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    args = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (args == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    num = xar_numerus(xar);
    per (i = ZEPHYRUM; i < num; i++)
    {
        elem = xar_obtinere(xar, i);

        /* Preparare argumenta pro callback: (element) */
        xar_vacare(args);
        copia = xar_addere(args);
        *copia = *elem;

        /* Vocare callback */
        resultus = _vocare_functionem(interp, fn_arg->ut.functio, args, nodus);
        si (interp->error_accidit)
        {
            redde _valor_nihil();
        }

        /* Si verum, addere elementum ad novum array */
        si (_est_verum(&resultus))
        {
            copia = xar_addere(novus);
            si (copia == NIHIL)
            {
                _error(interp, nodus, "Memoria exhausta");
                redde _valor_nihil();
            }
            *copia = *elem;
        }
    }

    redde _valor_xar(novus);
}

/* find(fn) - return first element where fn returns true */
interior SputnikValor
_methodus_xar_find(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 i;
    i32 num;
    SputnikValor* fn_arg;
    SputnikValor* elem;
    SputnikValor* copia;
    SputnikValor resultus;
    Xar* args;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "find() requirit functionem");
        redde _valor_nihil();
    }

    fn_arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (fn_arg->genus != SPUTNIK_VALOR_FUNCTIO)
    {
        _error(interp, nodus, "find() requirit functionem ut argumentum");
        redde _valor_nihil();
    }

    args = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (args == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    num = xar_numerus(xar);
    per (i = ZEPHYRUM; i < num; i++)
    {
        elem = xar_obtinere(xar, i);

        /* Preparare argumenta pro callback: (element) */
        xar_vacare(args);
        copia = xar_addere(args);
        *copia = *elem;

        /* Vocare callback */
        resultus = _vocare_functionem(interp, fn_arg->ut.functio, args, nodus);
        si (interp->error_accidit)
        {
            redde _valor_nihil();
        }

        /* Si verum, redde elementum */
        si (_est_verum(&resultus))
        {
            redde *elem;
        }
    }

    /* Non inventum */
    redde _valor_nihil();
}

/* forEach(fn) - call fn for each element, return nihil */
interior SputnikValor
_methodus_xar_forEach(SputnikInterpres* interp, Xar* xar, Xar* argumenta, SputnikAstNodus* nodus)
{
    i32 i;
    i32 num;
    SputnikValor* fn_arg;
    SputnikValor* elem;
    SputnikValor* copia;
    Xar* args;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "forEach() requirit functionem");
        redde _valor_nihil();
    }

    fn_arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (fn_arg->genus != SPUTNIK_VALOR_FUNCTIO)
    {
        _error(interp, nodus, "forEach() requirit functionem ut argumentum");
        redde _valor_nihil();
    }

    args = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (args == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    num = xar_numerus(xar);
    per (i = ZEPHYRUM; i < num; i++)
    {
        elem = xar_obtinere(xar, i);

        /* Preparare argumenta pro callback: (element) */
        xar_vacare(args);
        copia = xar_addere(args);
        *copia = *elem;

        /* Vocare callback (ignore result) */
        _vocare_functionem(interp, fn_arg->ut.functio, args, nodus);
        si (interp->error_accidit)
        {
            redde _valor_nihil();
        }
    }

    redde _valor_nihil();
}

/* Dispatcher pro array methods */
interior SputnikValor
_vocare_methodum_xar(SputnikInterpres* interp, SputnikMethodusXar* meth, Xar* argumenta, SputnikAstNodus* nodus)
{
    si (chorda_aequalis_literis(meth->titulus, "length"))
        redde _methodus_xar_length(meth->xar);
    si (chorda_aequalis_literis(meth->titulus, "push"))
        redde _methodus_xar_push(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "pop"))
        redde _methodus_xar_pop(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "shift"))
        redde _methodus_xar_shift(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "unshift"))
        redde _methodus_xar_unshift(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "indexOf"))
        redde _methodus_xar_indexOf(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "includes"))
        redde _methodus_xar_includes(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "slice"))
        redde _methodus_xar_slice(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "concat"))
        redde _methodus_xar_concat(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "join"))
        redde _methodus_xar_join(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "map"))
        redde _methodus_xar_map(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "filter"))
        redde _methodus_xar_filter(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "find"))
        redde _methodus_xar_find(interp, meth->xar, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "forEach"))
        redde _methodus_xar_forEach(interp, meth->xar, argumenta, nodus);

    _error(interp, nodus, "Methodus xar ignota");
    redde _valor_nihil();
}


/* ==================================================
 * String Methods
 * ================================================== */

/* length - redde longitudinem chordae */
interior SputnikValor
_methodus_chorda_length(chorda str)
{
    redde _valor_numerus((f64)str.mensura);
}

/* charAt(index) - redde characterem ad indicem */
interior SputnikValor
_methodus_chorda_charAt(SputnikInterpres* interp, chorda str, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* arg;
    i32 index;
    chorda resultus;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "charAt() requirit I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (arg->genus != SPUTNIK_VALOR_NUMERUS)
    {
        _error(interp, nodus, "charAt() requirit numerum ut indicem");
        redde _valor_nihil();
    }

    index = (i32)arg->ut.numerus;
    resultus = chorda_character_ad(str, index, interp->piscina);
    redde _valor_chorda(resultus);
}

/* indexOf(substr) - invenire primam occurrentiam */
interior SputnikValor
_methodus_chorda_indexOf(SputnikInterpres* interp, chorda str, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* arg;
    chorda substr;
    i32 i, j;
    b32 inventum;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "indexOf() requirit I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "indexOf() requirit chordam");
        redde _valor_nihil();
    }

    substr = arg->ut.chorda_valor;

    /* Casus specialis: substr vacua */
    si (chorda_vacua(substr))
    {
        redde _valor_numerus(0.0);
    }

    /* Si substr longior quam str */
    si (substr.mensura > str.mensura)
    {
        redde _valor_numerus(-1.0);
    }

    /* Quaerere substr in str */
    per (i = ZEPHYRUM; i <= str.mensura - substr.mensura; i++)
    {
        inventum = VERUM;
        per (j = ZEPHYRUM; j < substr.mensura; j++)
        {
            si (str.datum[i + j] != substr.datum[j])
            {
                inventum = FALSUM;
                frange;
            }
        }
        si (inventum)
        {
            redde _valor_numerus((f64)i);
        }
    }

    redde _valor_numerus(-1.0);
}

/* includes(substr) - verificare si continet */
interior SputnikValor
_methodus_chorda_includes(SputnikInterpres* interp, chorda str, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor result;

    result = _methodus_chorda_indexOf(interp, str, argumenta, nodus);
    si (interp->error_accidit)
    {
        redde _valor_nihil();
    }

    si (result.ut.numerus >= 0.0)
    {
        redde _valor_verum();
    }
    redde _valor_falsum();
}

/* substring(start, end?) - extrahere partem */
interior SputnikValor
_methodus_chorda_substring(SputnikInterpres* interp, chorda str, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* arg;
    i32 start, end;
    i32 len;
    character* buffer;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "substring() requirit saltem I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (arg->genus != SPUTNIK_VALOR_NUMERUS)
    {
        _error(interp, nodus, "substring() requirit numerum ut initium");
        redde _valor_nihil();
    }
    start = (i32)arg->ut.numerus;

    /* Default end = longitudo */
    end = str.mensura;
    si (xar_numerus(argumenta) >= II)
    {
        arg = xar_obtinere(argumenta, I);
        si (arg->genus == SPUTNIK_VALOR_NUMERUS)
        {
            end = (i32)arg->ut.numerus;
        }
    }

    /* Normalizare indices (JavaScript substring behavior) */
    si (start < ZEPHYRUM) start = ZEPHYRUM;
    si (end < ZEPHYRUM) end = ZEPHYRUM;
    si (start > str.mensura) start = str.mensura;
    si (end > str.mensura) end = str.mensura;

    /* Si start > end, commutare */
    si (start > end)
    {
        i32 temp;
        temp = start;
        start = end;
        end = temp;
    }

    len = end - start;
    si (len <= ZEPHYRUM)
    {
        redde _valor_chorda(chorda_ex_literis("", interp->piscina));
    }

    buffer = piscina_allocare(interp->piscina, (memoriae_index)(len + I));
    memcpy(buffer, str.datum + start, (size_t)len);
    buffer[len] = '\0';
    redde _valor_chorda(chorda_ex_literis(buffer, interp->piscina));
}

/* toUpperCase() - convertere ad maiusculas */
interior SputnikValor
_methodus_chorda_toUpperCase(SputnikInterpres* interp, chorda str)
{
    character* buffer;
    i32 i;

    buffer = piscina_allocare(interp->piscina, (memoriae_index)(str.mensura + I));
    per (i = ZEPHYRUM; i < str.mensura; i++)
    {
        character c;
        c = (character)str.datum[i];
        si (c >= 'a' && c <= 'z')
        {
            buffer[i] = (character)(c - 'a' + 'A');
        }
        alioquin
        {
            buffer[i] = c;
        }
    }
    buffer[str.mensura] = '\0';
    redde _valor_chorda(chorda_ex_literis(buffer, interp->piscina));
}

/* toLowerCase() - convertere ad minusculas */
interior SputnikValor
_methodus_chorda_toLowerCase(SputnikInterpres* interp, chorda str)
{
    character* buffer;
    i32 i;

    buffer = piscina_allocare(interp->piscina, (memoriae_index)(str.mensura + I));
    per (i = ZEPHYRUM; i < str.mensura; i++)
    {
        character c;
        c = (character)str.datum[i];
        si (c >= 'A' && c <= 'Z')
        {
            buffer[i] = (character)(c - 'A' + 'a');
        }
        alioquin
        {
            buffer[i] = c;
        }
    }
    buffer[str.mensura] = '\0';
    redde _valor_chorda(chorda_ex_literis(buffer, interp->piscina));
}

/* split(delimiter) - dividere in xar */
interior SputnikValor
_methodus_chorda_split(SputnikInterpres* interp, chorda str, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* arg;
    chorda delimiter;
    Xar* resultus;
    chorda_fissio_fructus fissio;
    i32 i;
    SputnikValor* elem;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "split() requirit I argumentum");
        redde _valor_nihil();
    }

    arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "split() requirit chordam ut delimiter");
        redde _valor_nihil();
    }

    delimiter = arg->ut.chorda_valor;

    /* Usare chorda_fissio_chorda pro divisione */
    fissio = chorda_fissio_chorda(str, delimiter, interp->piscina);

    /* Convertere ad Xar de SputnikValor */
    resultus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (resultus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    per (i = ZEPHYRUM; i < fissio.numerus; i++)
    {
        elem = xar_addere(resultus);
        si (elem == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *elem = _valor_chorda(fissio.elementa[i]);
    }

    redde _valor_xar(resultus);
}

/* Dispatcher pro methodis chordae */
interior SputnikValor
_vocare_methodum_chorda(SputnikInterpres* interp, SputnikMethodusChorda* meth, Xar* argumenta, SputnikAstNodus* nodus)
{
    si (chorda_aequalis_literis(meth->titulus, "length"))
        redde _methodus_chorda_length(meth->chorda_valor);
    si (chorda_aequalis_literis(meth->titulus, "charAt"))
        redde _methodus_chorda_charAt(interp, meth->chorda_valor, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "indexOf"))
        redde _methodus_chorda_indexOf(interp, meth->chorda_valor, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "includes"))
        redde _methodus_chorda_includes(interp, meth->chorda_valor, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "substring"))
        redde _methodus_chorda_substring(interp, meth->chorda_valor, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "toUpperCase"))
        redde _methodus_chorda_toUpperCase(interp, meth->chorda_valor);
    si (chorda_aequalis_literis(meth->titulus, "toLowerCase"))
        redde _methodus_chorda_toLowerCase(interp, meth->chorda_valor);
    si (chorda_aequalis_literis(meth->titulus, "split"))
        redde _methodus_chorda_split(interp, meth->chorda_valor, argumenta, nodus);

    _error(interp, nodus, "Methodus chorda ignota");
    redde _valor_nihil();
}


/* ==================================================
 * Entity Methods
 * ================================================== */

/* add_relation(genus, destinatio_id) */
interior SputnikValor
_methodus_entitas_add_relation(SputnikInterpres* interp, Entitas* e, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* genus_arg;
    SputnikValor* dest_arg;
    Relatio* relatio;
    chorda* dest_id_interned;

    si (xar_numerus(argumenta) != II)
    {
        _error(interp, nodus, "add_relation requirit II argumenta: genus, destinatio_id");
        redde _valor_nihil();
    }

    genus_arg = xar_obtinere(argumenta, ZEPHYRUM);
    dest_arg = xar_obtinere(argumenta, I);

    si (genus_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Primus argumentum ad add_relation debet esse chorda");
        redde _valor_nihil();
    }

    si (dest_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Secundus argumentum ad add_relation debet esse chorda (id)");
        redde _valor_nihil();
    }

    /* Internare destinatio_id */
    dest_id_interned = chorda_internare(
        interp->intern,
        dest_arg->ut.chorda_valor);
    si (dest_id_interned == NIHIL)
    {
        _error(interp, nodus, "Non potest internare destinatio_id");
        redde _valor_nihil();
    }

    /* Addere relationem via repositorio */
    relatio = interp->repositorium->relatio_addere(
        interp->repositorium->datum,
        e,
        (constans character*)genus_arg->ut.chorda_valor.datum,
        dest_id_interned);

    si (relatio == NIHIL)
    {
        _error(interp, nodus, "Non potest addere relationem");
        redde _valor_nihil();
    }

    /* Redde id relationis */
    redde _valor_chorda(*relatio->id);
}

/* related(genus) - redde singulam entitatem vel nihil */
interior SputnikValor
_methodus_entitas_related(SputnikInterpres* interp, Entitas* e, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* genus_arg;
    Xar* relatae;
    Entitas* relata;

    si (xar_numerus(argumenta) != I)
    {
        _error(interp, nodus, "related requirit I argumentum: genus");
        redde _valor_nihil();
    }

    genus_arg = xar_obtinere(argumenta, ZEPHYRUM);

    si (genus_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad related debet esse chorda");
        redde _valor_nihil();
    }

    /* Capere entitates relatae via repositorio */
    relatae = interp->repositorium->capere_entitates_relatae(
        interp->repositorium->datum,
        e,
        &genus_arg->ut.chorda_valor);

    si (relatae == NIHIL || xar_numerus(relatae) == ZEPHYRUM)
    {
        redde _valor_nihil();
    }

    /* Redde primam entitatem */
    relata = *(Entitas**)xar_obtinere(relatae, ZEPHYRUM);
    redde _valor_entitas(relata);
}

/* related_all(genus) - redde xar de entitates */
interior SputnikValor
_methodus_entitas_related_all(SputnikInterpres* interp, Entitas* e, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* genus_arg;
    Xar* relatae;
    Xar* resultus;
    i32 i;
    i32 num;
    Entitas* relata;
    SputnikValor* elem;

    si (xar_numerus(argumenta) != I)
    {
        _error(interp, nodus, "related_all requirit I argumentum: genus");
        redde _valor_nihil();
    }

    genus_arg = xar_obtinere(argumenta, ZEPHYRUM);

    si (genus_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad related_all debet esse chorda");
        redde _valor_nihil();
    }

    /* Capere entitates relatae via repositorio */
    relatae = interp->repositorium->capere_entitates_relatae(
        interp->repositorium->datum,
        e,
        &genus_arg->ut.chorda_valor);

    /* Creare xar de SputnikValor */
    resultus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (resultus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    si (relatae != NIHIL)
    {
        num = xar_numerus(relatae);
        per (i = ZEPHYRUM; i < num; i++)
        {
            relata = *(Entitas**)xar_obtinere(relatae, i);
            elem = xar_addere(resultus);
            *elem = _valor_entitas(relata);
        }
    }

    redde _valor_xar(resultus);
}

/* remove_relation(relatio_id) */
interior SputnikValor
_methodus_entitas_remove_relation(SputnikInterpres* interp, Entitas* e, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* id_arg;
    chorda* id_interned;
    b32 successus;

    (vacuum)e;  /* Non usamus entitas directe, sed id relationis */

    si (xar_numerus(argumenta) != I)
    {
        _error(interp, nodus, "remove_relation requirit I argumentum: relatio_id");
        redde _valor_nihil();
    }

    id_arg = xar_obtinere(argumenta, ZEPHYRUM);

    si (id_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad remove_relation debet esse chorda (id)");
        redde _valor_nihil();
    }

    /* Internare id */
    id_interned = chorda_internare(
        interp->intern,
        id_arg->ut.chorda_valor);
    si (id_interned == NIHIL)
    {
        _error(interp, nodus, "Non potest internare relatio_id");
        redde _valor_nihil();
    }

    /* Delere relationem via repositorio */
    successus = interp->repositorium->relatio_delere(
        interp->repositorium->datum,
        id_interned);

    redde successus ? _valor_verum() : _valor_falsum();
}

/* add_tag(nota) */
interior SputnikValor
_methodus_entitas_add_tag(SputnikInterpres* interp, Entitas* e, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* nota_arg;
    b32 successus;

    si (xar_numerus(argumenta) != I)
    {
        _error(interp, nodus, "add_tag requirit I argumentum: nota");
        redde _valor_nihil();
    }

    nota_arg = xar_obtinere(argumenta, ZEPHYRUM);

    si (nota_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad add_tag debet esse chorda");
        redde _valor_nihil();
    }

    /* Creare C string null-terminatam */
    {
        character* nota_c;
        i32 nota_len;

        nota_len = nota_arg->ut.chorda_valor.mensura;
        nota_c = piscina_allocare(interp->piscina, (memoriae_index)(nota_len + I));
        si (nota_c == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        memcpy(nota_c, nota_arg->ut.chorda_valor.datum, (size_t)nota_len);
        nota_c[nota_len] = '\0';

        /* Addere notam via repositorio */
        successus = interp->repositorium->nota_addere(
            interp->repositorium->datum,
            e,
            nota_c);
    }

    redde successus ? _valor_verum() : _valor_falsum();
}

/* has_tag(nota) */
interior SputnikValor
_methodus_entitas_has_tag(SputnikInterpres* interp, Entitas* e, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* nota_arg;
    chorda* nota_interned;

    (vacuum)interp;

    si (xar_numerus(argumenta) != I)
    {
        _error(interp, nodus, "has_tag requirit I argumentum: nota");
        redde _valor_nihil();
    }

    nota_arg = xar_obtinere(argumenta, ZEPHYRUM);

    si (nota_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad has_tag debet esse chorda");
        redde _valor_nihil();
    }

    /* Internare nota pro comparatione */
    nota_interned = chorda_internare(
        interp->intern,
        nota_arg->ut.chorda_valor);
    si (nota_interned == NIHIL)
    {
        redde _valor_falsum();
    }

    /* Verificare si habet notam */
    redde entitas_nota_habet(e, nota_interned) ? _valor_verum() : _valor_falsum();
}

/* remove_tag(nota) */
interior SputnikValor
_methodus_entitas_remove_tag(SputnikInterpres* interp, Entitas* e, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* nota_arg;
    b32 successus;

    si (xar_numerus(argumenta) != I)
    {
        _error(interp, nodus, "remove_tag requirit I argumentum: nota");
        redde _valor_nihil();
    }

    nota_arg = xar_obtinere(argumenta, ZEPHYRUM);

    si (nota_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad remove_tag debet esse chorda");
        redde _valor_nihil();
    }

    /* Delere notam via repositorio */
    successus = interp->repositorium->nota_delere(
        interp->repositorium->datum,
        e,
        (constans character*)nota_arg->ut.chorda_valor.datum);

    redde successus ? _valor_verum() : _valor_falsum();
}

/* Dispatcher pro entity methods */
interior SputnikValor
_vocare_methodum_entitas(SputnikInterpres* interp, SputnikMethodusEntitas* meth, Xar* argumenta, SputnikAstNodus* nodus)
{
    /* Verificare repositorium */
    si (interp->repositorium == NIHIL)
    {
        _error(interp, nodus, "Repositorium non configuratum pro entitates");
        redde _valor_nihil();
    }

    si (meth->entitas == NIHIL)
    {
        _error(interp, nodus, "Entitas est nihil");
        redde _valor_nihil();
    }

    si (chorda_aequalis_literis(meth->titulus, "add_relation"))
        redde _methodus_entitas_add_relation(interp, meth->entitas, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "related"))
        redde _methodus_entitas_related(interp, meth->entitas, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "related_all"))
        redde _methodus_entitas_related_all(interp, meth->entitas, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "remove_relation"))
        redde _methodus_entitas_remove_relation(interp, meth->entitas, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "add_tag"))
        redde _methodus_entitas_add_tag(interp, meth->entitas, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "has_tag"))
        redde _methodus_entitas_has_tag(interp, meth->entitas, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "remove_tag"))
        redde _methodus_entitas_remove_tag(interp, meth->entitas, argumenta, nodus);

    _error(interp, nodus, "Methodus entitas ignota");
    redde _valor_nihil();
}


/* ==================================================
 * Repository Methods
 * ================================================== */

/* REPO.get(id) - capere entitatem per ID */
interior SputnikValor
_methodus_repo_get(SputnikInterpres* interp, EntitasRepositorium* repo, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* id_arg;
    chorda* id_internata;
    Entitas* entitas;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "REPO.get requirit argumentum id");
        redde _valor_nihil();
    }

    id_arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (id_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad REPO.get debet esse chorda");
        redde _valor_nihil();
    }

    /* Internare ID */
    id_internata = chorda_internare(interp->intern, id_arg->ut.chorda_valor);
    si (id_internata == NIHIL)
    {
        redde _valor_nihil();
    }

    entitas = repo->capere_entitatem(repo->datum, id_internata);
    si (entitas == NIHIL)
    {
        redde _valor_nihil();
    }

    redde _valor_entitas(entitas);
}

/* REPO.query(genus) - capere entitates per genus */
interior SputnikValor
_methodus_repo_query(SputnikInterpres* interp, EntitasRepositorium* repo, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* genus_arg;
    Xar* resultus;
    Xar* entitates_xar;
    i32 i;
    i32 numerus;
    Entitas* e;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "REPO.query requirit argumentum genus");
        redde _valor_nihil();
    }

    genus_arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (genus_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad REPO.query debet esse chorda");
        redde _valor_nihil();
    }

    /* Creare resultus array */
    resultus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (resultus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    /* Iterare per omnes entitates et filtrare per genus */
    /* Nota: hoc est inefficiens - futura versio debet habere indicem */
    entitates_xar = repo->legere_omnes_eventus(repo->datum);
    si (entitates_xar == NIHIL)
    {
        redde _valor_xar(resultus);
    }

    /* Usare capere_radices et traversare, vel simpliciore:
     * Iterare per hash table si accessibilis */
    /* Pro nunc, simpliciore approach: text search fallback */
    {
        Xar* found;
        character genus_buffer[CCLVI];
        i32 genus_len;

        genus_len = genus_arg->ut.chorda_valor.mensura;
        si (genus_len >= CCLVI) genus_len = CCLV;
        memcpy(genus_buffer, genus_arg->ut.chorda_valor.datum, (size_t)genus_len);
        genus_buffer[genus_len] = '\0';

        found = repo->quaerere_textum(repo->datum, genus_buffer);
        si (found != NIHIL)
        {
            numerus = xar_numerus(found);
            per (i = ZEPHYRUM; i < numerus; i++)
            {
                e = *(Entitas**)xar_obtinere(found, i);
                si (e != NIHIL && e->genus != NIHIL)
                {
                    /* Verificare genus exactum */
                    si (chorda_aequalis(*e->genus, genus_arg->ut.chorda_valor))
                    {
                        SputnikValor* elem;
                        elem = xar_addere(resultus);
                        si (elem != NIHIL)
                        {
                            *elem = _valor_entitas(e);
                        }
                    }
                }
            }
        }
    }

    redde _valor_xar(resultus);
}

/* REPO.tagged(nota) - capere entitates cum nota */
interior SputnikValor
_methodus_repo_tagged(SputnikInterpres* interp, EntitasRepositorium* repo, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* nota_arg;
    chorda* nota_internata;
    Xar* found;
    Xar* resultus;
    i32 i;
    i32 numerus;
    Entitas* e;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "REPO.tagged requirit argumentum nota");
        redde _valor_nihil();
    }

    nota_arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (nota_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad REPO.tagged debet esse chorda");
        redde _valor_nihil();
    }

    /* Internare nota */
    nota_internata = chorda_internare(interp->intern, nota_arg->ut.chorda_valor);
    si (nota_internata == NIHIL)
    {
        redde _valor_nihil();
    }

    found = repo->quaerere_cum_nota(repo->datum, nota_internata);

    /* Creare resultus array */
    resultus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (resultus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    si (found != NIHIL)
    {
        numerus = xar_numerus(found);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            e = *(Entitas**)xar_obtinere(found, i);
            si (e != NIHIL)
            {
                SputnikValor* elem;
                elem = xar_addere(resultus);
                si (elem != NIHIL)
                {
                    *elem = _valor_entitas(e);
                }
            }
        }
    }

    redde _valor_xar(resultus);
}

/* REPO.search(text) - quaerere textum */
interior SputnikValor
_methodus_repo_search(SputnikInterpres* interp, EntitasRepositorium* repo, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* text_arg;
    Xar* found;
    Xar* resultus;
    i32 i;
    i32 numerus;
    Entitas* e;
    character text_buffer[CCLVI];
    i32 text_len;

    si (xar_numerus(argumenta) < I)
    {
        _error(interp, nodus, "REPO.search requirit argumentum text");
        redde _valor_nihil();
    }

    text_arg = xar_obtinere(argumenta, ZEPHYRUM);
    si (text_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumentum ad REPO.search debet esse chorda");
        redde _valor_nihil();
    }

    /* Convertere ad C string */
    text_len = text_arg->ut.chorda_valor.mensura;
    si (text_len >= CCLVI) text_len = CCLV;
    memcpy(text_buffer, text_arg->ut.chorda_valor.datum, (size_t)text_len);
    text_buffer[text_len] = '\0';

    found = repo->quaerere_textum(repo->datum, text_buffer);

    /* Creare resultus array */
    resultus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (resultus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    si (found != NIHIL)
    {
        numerus = xar_numerus(found);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            e = *(Entitas**)xar_obtinere(found, i);
            si (e != NIHIL)
            {
                SputnikValor* elem;
                elem = xar_addere(resultus);
                si (elem != NIHIL)
                {
                    *elem = _valor_entitas(e);
                }
            }
        }
    }

    redde _valor_xar(resultus);
}

/* REPO.roots() - capere entitates radices */
interior SputnikValor
_methodus_repo_roots(SputnikInterpres* interp, EntitasRepositorium* repo, Xar* argumenta, SputnikAstNodus* nodus)
{
    Xar* found;
    Xar* resultus;
    i32 i;
    i32 numerus;
    Entitas* e;

    (vacuum)argumenta;
    (vacuum)nodus;

    found = repo->capere_radices(repo->datum);

    /* Creare resultus array */
    resultus = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (resultus == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    si (found != NIHIL)
    {
        numerus = xar_numerus(found);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            e = *(Entitas**)xar_obtinere(found, i);
            si (e != NIHIL)
            {
                SputnikValor* elem;
                elem = xar_addere(resultus);
                si (elem != NIHIL)
                {
                    *elem = _valor_entitas(e);
                }
            }
        }
    }

    redde _valor_xar(resultus);
}

/* REPO.scaffold(genus, titulus) - creare vel obtinere entitatem cum ID deterministico */
interior SputnikValor
_methodus_repo_scaffold(SputnikInterpres* interp, EntitasRepositorium* repo, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikValor* genus_arg;
    SputnikValor* titulus_arg;
    Entitas* entitas;
    character genus_buffer[CCLVI];
    character titulus_buffer[CCLVI];
    i32 len;

    si (xar_numerus(argumenta) < II)
    {
        _error(interp, nodus, "REPO.scaffold requirit II argumenta: genus, titulus");
        redde _valor_nihil();
    }

    genus_arg = xar_obtinere(argumenta, ZEPHYRUM);
    titulus_arg = xar_obtinere(argumenta, I);

    si (genus_arg->genus != SPUTNIK_VALOR_CHORDA ||
        titulus_arg->genus != SPUTNIK_VALOR_CHORDA)
    {
        _error(interp, nodus, "Argumenta ad REPO.scaffold debent esse chordae");
        redde _valor_nihil();
    }

    /* Convertere ad C strings */
    len = genus_arg->ut.chorda_valor.mensura;
    si (len >= CCLVI) len = CCLV;
    memcpy(genus_buffer, genus_arg->ut.chorda_valor.datum, (size_t)len);
    genus_buffer[len] = '\0';

    len = titulus_arg->ut.chorda_valor.mensura;
    si (len >= CCLVI) len = CCLV;
    memcpy(titulus_buffer, titulus_arg->ut.chorda_valor.datum, (size_t)len);
    titulus_buffer[len] = '\0';

    /* Vocare entitas_scaffoldare */
    entitas = repo->entitas_scaffoldare(repo->datum, genus_buffer, titulus_buffer);
    si (entitas == NIHIL)
    {
        _error(interp, nodus, "Non potest scaffoldare entitatem");
        redde _valor_nihil();
    }

    redde _valor_entitas(entitas);
}

/* Dispatcher pro repository methods */
interior SputnikValor
_vocare_methodum_repositorium(SputnikInterpres* interp, SputnikMethodusRepositorium* meth, Xar* argumenta, SputnikAstNodus* nodus)
{
    si (meth->repositorium == NIHIL)
    {
        _error(interp, nodus, "Repositorium est nihil");
        redde _valor_nihil();
    }

    si (chorda_aequalis_literis(meth->titulus, "get"))
        redde _methodus_repo_get(interp, meth->repositorium, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "query"))
        redde _methodus_repo_query(interp, meth->repositorium, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "tagged"))
        redde _methodus_repo_tagged(interp, meth->repositorium, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "search"))
        redde _methodus_repo_search(interp, meth->repositorium, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "roots"))
        redde _methodus_repo_roots(interp, meth->repositorium, argumenta, nodus);
    si (chorda_aequalis_literis(meth->titulus, "scaffold"))
        redde _methodus_repo_scaffold(interp, meth->repositorium, argumenta, nodus);

    _error(interp, nodus, "Methodus repositorii ignota");
    redde _valor_nihil();
}


/* ==================================================
 * Expression Evaluation
 * ================================================== */

interior SputnikValor
_eval_numerum(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    (vacuum)interp;
    redde _valor_numerus(nodus->numerus);
}

interior SputnikValor
_eval_pecuniam(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    (vacuum)interp;
    redde _valor_pecunia(nodus->pecunia);
}

interior SputnikValor
_eval_chordam(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    (vacuum)interp;
    redde _valor_chorda(nodus->valor);
}

interior SputnikValor
_eval_verum(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    (vacuum)interp;
    (vacuum)nodus;
    redde _valor_verum();
}

interior SputnikValor
_eval_falsum(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    (vacuum)interp;
    (vacuum)nodus;
    redde _valor_falsum();
}

interior SputnikValor
_eval_nihil(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    (vacuum)interp;
    (vacuum)nodus;
    redde _valor_nihil();
}

interior SputnikValor
_eval_identificatorem(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikValor* valor;

    valor = _invenire_variabilem(interp, nodus->valor);
    si (valor == NIHIL)
    {
        _error(interp, nodus, "Variabilis non definita");
        redde _valor_nihil();
    }

    redde *valor;
}

interior SputnikValor
_eval_binarium(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* sin_nodus;
    SputnikAstNodus* dex_nodus;
    SputnikValor sin;
    SputnikValor dex;
    ChordaAedificator* aed;

    si (xar_numerus(nodus->liberi) < II)
    {
        _error(interp, nodus, "Operatio binaria requirit II operandos");
        redde _valor_nihil();
    }

    sin_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    dex_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, I);

    /* Short-circuit pro && et || */
    si (nodus->operator == SPUTNIK_LEXEMA_ET)
    {
        sin = _evaluare_nodum(interp, sin_nodus);
        si (interp->error_accidit) redde _valor_nihil();
        si (!_est_verum(&sin))
        {
            redde sin;
        }
        redde _evaluare_nodum(interp, dex_nodus);
    }

    si (nodus->operator == SPUTNIK_LEXEMA_VEL)
    {
        sin = _evaluare_nodum(interp, sin_nodus);
        si (interp->error_accidit) redde _valor_nihil();
        si (_est_verum(&sin))
        {
            redde sin;
        }
        redde _evaluare_nodum(interp, dex_nodus);
    }

    /* Evaluare ambos operandos */
    sin = _evaluare_nodum(interp, sin_nodus);
    si (interp->error_accidit) redde _valor_nihil();

    dex = _evaluare_nodum(interp, dex_nodus);
    si (interp->error_accidit) redde _valor_nihil();

    commutatio (nodus->operator)
    {
        /* Arithmetica */
        casus SPUTNIK_LEXEMA_PLUS:
            /* Concatenatio chordarum */
            si (sin.genus == SPUTNIK_VALOR_CHORDA || dex.genus == SPUTNIK_VALOR_CHORDA)
            {
                chorda s1;
                chorda s2;
                s1 = _ad_chordam(interp, &sin);
                s2 = _ad_chordam(interp, &dex);
                aed = chorda_aedificator_creare(interp->piscina,
                    (memoriae_index)(s1.mensura + s2.mensura + I));
                si (aed == NIHIL)
                {
                    redde _valor_chorda(chorda_ex_literis("", interp->piscina));
                }
                chorda_aedificator_appendere_chorda(aed, s1);
                chorda_aedificator_appendere_chorda(aed, s2);
                redde _valor_chorda(chorda_aedificator_finire(aed));
            }
            /* Pecunia + pecunia */
            si (sin.genus == SPUTNIK_VALOR_PECUNIA && dex.genus == SPUTNIK_VALOR_PECUNIA)
            {
                redde _valor_pecunia(sin.ut.pecunia + dex.ut.pecunia);
            }
            si (sin.genus != SPUTNIK_VALOR_NUMERUS || dex.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator + requirit numeros vel chordas vel pecuniam");
                redde _valor_nihil();
            }
            redde _valor_numerus(sin.ut.numerus + dex.ut.numerus);

        casus SPUTNIK_LEXEMA_MINUS:
            /* Pecunia - pecunia */
            si (sin.genus == SPUTNIK_VALOR_PECUNIA && dex.genus == SPUTNIK_VALOR_PECUNIA)
            {
                redde _valor_pecunia(sin.ut.pecunia - dex.ut.pecunia);
            }
            si (sin.genus != SPUTNIK_VALOR_NUMERUS || dex.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator - requirit numeros vel pecuniam");
                redde _valor_nihil();
            }
            redde _valor_numerus(sin.ut.numerus - dex.ut.numerus);

        casus SPUTNIK_LEXEMA_ASTERISCUS:
            /* Pecunia * numerus vel numerus * pecunia */
            si (sin.genus == SPUTNIK_VALOR_PECUNIA && dex.genus == SPUTNIK_VALOR_NUMERUS)
            {
                redde _valor_pecunia((s64)(sin.ut.pecunia * dex.ut.numerus));
            }
            si (sin.genus == SPUTNIK_VALOR_NUMERUS && dex.genus == SPUTNIK_VALOR_PECUNIA)
            {
                redde _valor_pecunia((s64)(sin.ut.numerus * dex.ut.pecunia));
            }
            si (sin.genus != SPUTNIK_VALOR_NUMERUS || dex.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator * requirit numeros");
                redde _valor_nihil();
            }
            redde _valor_numerus(sin.ut.numerus * dex.ut.numerus);

        casus SPUTNIK_LEXEMA_DIVISIO:
            /* Pecunia / numerus = pecunia (integer division) */
            si (sin.genus == SPUTNIK_VALOR_PECUNIA && dex.genus == SPUTNIK_VALOR_NUMERUS)
            {
                si (dex.ut.numerus == 0.0)
                {
                    _error(interp, nodus, "Divisio per nihil");
                    redde _valor_nihil();
                }
                redde _valor_pecunia(sin.ut.pecunia / (s64)dex.ut.numerus);
            }
            /* Pecunia / pecunia = ratio (numerus) */
            si (sin.genus == SPUTNIK_VALOR_PECUNIA && dex.genus == SPUTNIK_VALOR_PECUNIA)
            {
                si (dex.ut.pecunia == ZEPHYRUM)
                {
                    _error(interp, nodus, "Divisio per nihil");
                    redde _valor_nihil();
                }
                redde _valor_numerus((f64)sin.ut.pecunia / (f64)dex.ut.pecunia);
            }
            si (sin.genus != SPUTNIK_VALOR_NUMERUS || dex.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator / requirit numeros");
                redde _valor_nihil();
            }
            si (dex.ut.numerus == 0.0)
            {
                _error(interp, nodus, "Divisio per nihil");
                redde _valor_nihil();
            }
            redde _valor_numerus(sin.ut.numerus / dex.ut.numerus);

        casus SPUTNIK_LEXEMA_MODULUS:
            si (sin.genus != SPUTNIK_VALOR_NUMERUS || dex.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator % requirit numeros");
                redde _valor_nihil();
            }
            si (dex.ut.numerus == 0.0)
            {
                _error(interp, nodus, "Modulus per nihil");
                redde _valor_nihil();
            }
            redde _valor_numerus(fmod(sin.ut.numerus, dex.ut.numerus));

        /* Comparatio */
        casus SPUTNIK_LEXEMA_MINOR:
            si (sin.genus == SPUTNIK_VALOR_PECUNIA && dex.genus == SPUTNIK_VALOR_PECUNIA)
            {
                redde sin.ut.pecunia < dex.ut.pecunia ? _valor_verum() : _valor_falsum();
            }
            si (sin.genus != SPUTNIK_VALOR_NUMERUS || dex.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator < requirit numeros vel pecuniam");
                redde _valor_nihil();
            }
            redde sin.ut.numerus < dex.ut.numerus ? _valor_verum() : _valor_falsum();

        casus SPUTNIK_LEXEMA_MAIOR:
            si (sin.genus == SPUTNIK_VALOR_PECUNIA && dex.genus == SPUTNIK_VALOR_PECUNIA)
            {
                redde sin.ut.pecunia > dex.ut.pecunia ? _valor_verum() : _valor_falsum();
            }
            si (sin.genus != SPUTNIK_VALOR_NUMERUS || dex.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator > requirit numeros vel pecuniam");
                redde _valor_nihil();
            }
            redde sin.ut.numerus > dex.ut.numerus ? _valor_verum() : _valor_falsum();

        casus SPUTNIK_LEXEMA_MINOR_AUT:
            si (sin.genus == SPUTNIK_VALOR_PECUNIA && dex.genus == SPUTNIK_VALOR_PECUNIA)
            {
                redde sin.ut.pecunia <= dex.ut.pecunia ? _valor_verum() : _valor_falsum();
            }
            si (sin.genus != SPUTNIK_VALOR_NUMERUS || dex.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator <= requirit numeros vel pecuniam");
                redde _valor_nihil();
            }
            redde sin.ut.numerus <= dex.ut.numerus ? _valor_verum() : _valor_falsum();

        casus SPUTNIK_LEXEMA_MAIOR_AUT:
            si (sin.genus == SPUTNIK_VALOR_PECUNIA && dex.genus == SPUTNIK_VALOR_PECUNIA)
            {
                redde sin.ut.pecunia >= dex.ut.pecunia ? _valor_verum() : _valor_falsum();
            }
            si (sin.genus != SPUTNIK_VALOR_NUMERUS || dex.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator >= requirit numeros vel pecuniam");
                redde _valor_nihil();
            }
            redde sin.ut.numerus >= dex.ut.numerus ? _valor_verum() : _valor_falsum();

        /* Aequalitas */
        casus SPUTNIK_LEXEMA_AEQUALIS_DUO:
            /* == loose equality */
            si (sin.genus != dex.genus)
            {
                /* Comparatio inter numeros et booleanos */
                si ((sin.genus == SPUTNIK_VALOR_NUMERUS &&
                     (dex.genus == SPUTNIK_VALOR_VERUM || dex.genus == SPUTNIK_VALOR_FALSUM)) ||
                    (dex.genus == SPUTNIK_VALOR_NUMERUS &&
                     (sin.genus == SPUTNIK_VALOR_VERUM || sin.genus == SPUTNIK_VALOR_FALSUM)))
                {
                    f64 n1;
                    f64 n2;
                    n1 = sin.genus == SPUTNIK_VALOR_NUMERUS ? sin.ut.numerus :
                         (sin.genus == SPUTNIK_VALOR_VERUM ? 1.0 : 0.0);
                    n2 = dex.genus == SPUTNIK_VALOR_NUMERUS ? dex.ut.numerus :
                         (dex.genus == SPUTNIK_VALOR_VERUM ? 1.0 : 0.0);
                    redde n1 == n2 ? _valor_verum() : _valor_falsum();
                }
                redde _valor_falsum();
            }
            commutatio (sin.genus)
            {
                casus SPUTNIK_VALOR_NIHIL:
                    redde _valor_verum();
                casus SPUTNIK_VALOR_VERUM:
                casus SPUTNIK_VALOR_FALSUM:
                    redde _valor_verum();
                casus SPUTNIK_VALOR_NUMERUS:
                    redde sin.ut.numerus == dex.ut.numerus ? _valor_verum() : _valor_falsum();
                casus SPUTNIK_VALOR_CHORDA:
                    redde chorda_aequalis(sin.ut.chorda_valor, dex.ut.chorda_valor) ?
                           _valor_verum() : _valor_falsum();
                casus SPUTNIK_VALOR_PECUNIA:
                    redde sin.ut.pecunia == dex.ut.pecunia ? _valor_verum() : _valor_falsum();
                ordinarius:
                    /* Reference equality pro xar, objectum, functio */
                    redde sin.ut.xar == dex.ut.xar ? _valor_verum() : _valor_falsum();
            }

        casus SPUTNIK_LEXEMA_AEQUALIS_TRIA:
            /* === strict equality */
            si (sin.genus != dex.genus)
            {
                redde _valor_falsum();
            }
            commutatio (sin.genus)
            {
                casus SPUTNIK_VALOR_NIHIL:
                    redde _valor_verum();
                casus SPUTNIK_VALOR_VERUM:
                casus SPUTNIK_VALOR_FALSUM:
                    redde _valor_verum();
                casus SPUTNIK_VALOR_NUMERUS:
                    redde sin.ut.numerus == dex.ut.numerus ? _valor_verum() : _valor_falsum();
                casus SPUTNIK_VALOR_CHORDA:
                    redde chorda_aequalis(sin.ut.chorda_valor, dex.ut.chorda_valor) ?
                           _valor_verum() : _valor_falsum();
                casus SPUTNIK_VALOR_PECUNIA:
                    redde sin.ut.pecunia == dex.ut.pecunia ? _valor_verum() : _valor_falsum();
                ordinarius:
                    redde sin.ut.xar == dex.ut.xar ? _valor_verum() : _valor_falsum();
            }

        casus SPUTNIK_LEXEMA_NON_AEQUALIS:
            /* != loose inequality */
            {
                SputnikValor aeq;
                SputnikAstNodus temp;
                temp = *nodus;
                temp.operator = SPUTNIK_LEXEMA_AEQUALIS_DUO;
                aeq = _eval_binarium(interp, &temp);
                redde aeq.genus == SPUTNIK_VALOR_VERUM ? _valor_falsum() : _valor_verum();
            }

        casus SPUTNIK_LEXEMA_STRICTE_NON_AEQ:
            /* !== strict inequality */
            {
                SputnikValor aeq;
                SputnikAstNodus temp;
                temp = *nodus;
                temp.operator = SPUTNIK_LEXEMA_AEQUALIS_TRIA;
                aeq = _eval_binarium(interp, &temp);
                redde aeq.genus == SPUTNIK_VALOR_VERUM ? _valor_falsum() : _valor_verum();
            }

        ordinarius:
            _error(interp, nodus, "Operator binarius ignotus");
            redde _valor_nihil();
    }
}

interior SputnikValor
_eval_unarium(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* operandus_nodus;
    SputnikValor operandus;

    si (xar_numerus(nodus->liberi) < I)
    {
        _error(interp, nodus, "Operatio unaria requirit operandum");
        redde _valor_nihil();
    }

    operandus_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    operandus = _evaluare_nodum(interp, operandus_nodus);
    si (interp->error_accidit) redde _valor_nihil();

    commutatio (nodus->operator)
    {
        casus SPUTNIK_LEXEMA_MINUS:
            si (operandus.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Operator unarius - requirit numerum");
                redde _valor_nihil();
            }
            redde _valor_numerus(-operandus.ut.numerus);

        casus SPUTNIK_LEXEMA_NON:
            redde _est_verum(&operandus) ? _valor_falsum() : _valor_verum();

        ordinarius:
            _error(interp, nodus, "Operator unarius ignotus");
            redde _valor_nihil();
    }
}

interior SputnikValor
_eval_ternarium(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* conditio_nodus;
    SputnikAstNodus* verum_nodus;
    SputnikAstNodus* falsum_nodus;
    SputnikValor conditio;

    si (xar_numerus(nodus->liberi) < III)
    {
        _error(interp, nodus, "Operatio ternaria requirit III operandos");
        redde _valor_nihil();
    }

    conditio_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    verum_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, I);
    falsum_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, II);

    conditio = _evaluare_nodum(interp, conditio_nodus);
    si (interp->error_accidit) redde _valor_nihil();

    si (_est_verum(&conditio))
    {
        redde _evaluare_nodum(interp, verum_nodus);
    }
    alioquin
    {
        redde _evaluare_nodum(interp, falsum_nodus);
    }
}

interior SputnikValor
_eval_assignationem(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* target_nodus;
    SputnikAstNodus* valor_nodus;
    SputnikValor valor;
    chorda titulus;

    si (xar_numerus(nodus->liberi) < II)
    {
        _error(interp, nodus, "Assignatio requirit target et valorem");
        redde _valor_nihil();
    }

    target_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    valor_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, I);

    valor = _evaluare_nodum(interp, valor_nodus);
    si (interp->error_accidit) redde _valor_nihil();

    si (target_nodus->genus == SPUTNIK_AST_IDENTIFICATOR)
    {
        titulus = target_nodus->valor;

        /* Verificare non constans */
        si (_est_constans(interp, titulus))
        {
            _error(interp, nodus, "Non potest assignare ad constantem");
            redde _valor_nihil();
        }

        si (!_assignare_variabilem(interp, titulus, valor))
        {
            _error(interp, nodus, "Variabilis non definita");
            redde _valor_nihil();
        }

        redde valor;
    }
    alioquin si (target_nodus->genus == SPUTNIK_AST_ACCESSUS_MEMBRI)
    {
        SputnikAstNodus* obj_nodus;
        SputnikValor obj;
        SputnikValor* valor_copia;

        si (xar_numerus(target_nodus->liberi) < I)
        {
            _error(interp, nodus, "Accessus membri invalidus");
            redde _valor_nihil();
        }

        obj_nodus = *(SputnikAstNodus**)xar_obtinere(target_nodus->liberi, ZEPHYRUM);
        obj = _evaluare_nodum(interp, obj_nodus);
        si (interp->error_accidit) redde _valor_nihil();

        /* Tractare entitas */
        si (obj.genus == SPUTNIK_VALOR_ENTITAS)
        {
            Entitas* e;
            chorda valor_chorda;

            e = obj.ut.entitas;
            si (e == NIHIL)
            {
                _error(interp, nodus, "Entitas est nihil");
                redde _valor_nihil();
            }

            /* Non permittere assignare ad id vel genus */
            si (chorda_aequalis_literis(target_nodus->valor, "id") ||
                chorda_aequalis_literis(target_nodus->valor, "genus"))
            {
                _error(interp, nodus, "Non potest assignare ad id vel genus entitatis");
                redde _valor_nihil();
            }

            /* Verificare repositorium */
            si (interp->repositorium == NIHIL)
            {
                _error(interp, nodus, "Repositorium non configuratum pro entitates");
                redde _valor_nihil();
            }

            /* Convertere valor ad chorda */
            valor_chorda = _ad_chordam(interp, &valor);

            /* Creare C strings null-terminatas */
            {
                character* clavis_c;
                character* valor_c;

                clavis_c = piscina_allocare(interp->piscina,
                    (memoriae_index)(target_nodus->valor.mensura + I));
                valor_c = piscina_allocare(interp->piscina,
                    (memoriae_index)(valor_chorda.mensura + I));

                si (clavis_c == NIHIL || valor_c == NIHIL)
                {
                    _error(interp, nodus, "Memoria exhausta");
                    redde _valor_nihil();
                }

                memcpy(clavis_c, target_nodus->valor.datum,
                    (size_t)target_nodus->valor.mensura);
                clavis_c[target_nodus->valor.mensura] = '\0';

                memcpy(valor_c, valor_chorda.datum,
                    (size_t)valor_chorda.mensura);
                valor_c[valor_chorda.mensura] = '\0';

                /* Ponere proprietatem via repositorio */
                si (!interp->repositorium->proprietas_ponere(
                        interp->repositorium->datum,
                        e,
                        clavis_c,
                        valor_c))
                {
                    _error(interp, nodus, "Non potest ponere proprietatem entitatis");
                    redde _valor_nihil();
                }
            }

            redde valor;
        }

        si (obj.genus != SPUTNIK_VALOR_OBJECTUM)
        {
            _error(interp, nodus, "Non potest assignare membrum non-objecti");
            redde _valor_nihil();
        }

        valor_copia = piscina_allocare(interp->piscina, magnitudo(SputnikValor));
        si (valor_copia == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *valor_copia = valor;

        tabula_dispersa_inserere(obj.ut.objectum, target_nodus->valor, valor_copia);
        redde valor;
    }
    alioquin si (target_nodus->genus == SPUTNIK_AST_ACCESSUS_INDICE)
    {
        SputnikAstNodus* obj_nodus;
        SputnikAstNodus* index_nodus;
        SputnikValor obj;
        SputnikValor index;
        SputnikValor* elem;

        si (xar_numerus(target_nodus->liberi) < II)
        {
            _error(interp, nodus, "Accessus indice invalidus");
            redde _valor_nihil();
        }

        obj_nodus = *(SputnikAstNodus**)xar_obtinere(target_nodus->liberi, ZEPHYRUM);
        index_nodus = *(SputnikAstNodus**)xar_obtinere(target_nodus->liberi, I);

        obj = _evaluare_nodum(interp, obj_nodus);
        si (interp->error_accidit) redde _valor_nihil();

        index = _evaluare_nodum(interp, index_nodus);
        si (interp->error_accidit) redde _valor_nihil();

        si (obj.genus == SPUTNIK_VALOR_XAR)
        {
            i32 idx;
            si (index.genus != SPUTNIK_VALOR_NUMERUS)
            {
                _error(interp, nodus, "Index array debet esse numerus");
                redde _valor_nihil();
            }
            idx = (i32)index.ut.numerus;
            si (idx < ZEPHYRUM || idx >= xar_numerus(obj.ut.xar))
            {
                _error(interp, nodus, "Index extra limites");
                redde _valor_nihil();
            }
            elem = xar_obtinere(obj.ut.xar, idx);
            *elem = valor;
            redde valor;
        }
        alioquin si (obj.genus == SPUTNIK_VALOR_OBJECTUM)
        {
            SputnikValor* valor_copia;
            chorda clavis;

            si (index.genus != SPUTNIK_VALOR_CHORDA)
            {
                _error(interp, nodus, "Clavis objecti debet esse chorda");
                redde _valor_nihil();
            }
            clavis = index.ut.chorda_valor;

            valor_copia = piscina_allocare(interp->piscina, magnitudo(SputnikValor));
            si (valor_copia == NIHIL)
            {
                _error(interp, nodus, "Memoria exhausta");
                redde _valor_nihil();
            }
            *valor_copia = valor;

            tabula_dispersa_inserere(obj.ut.objectum, clavis, valor_copia);
            redde valor;
        }
        alioquin
        {
            _error(interp, nodus, "Non potest assignare per indicem non-array/objecti");
            redde _valor_nihil();
        }
    }

    _error(interp, nodus, "Target assignationis invalidum");
    redde _valor_nihil();
}

interior SputnikValor
_eval_accessum_membri(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* obj_nodus;
    SputnikValor obj;
    vacuum* valor_ptr;

    si (xar_numerus(nodus->liberi) < I)
    {
        _error(interp, nodus, "Accessus membri requirit objectum");
        redde _valor_nihil();
    }

    obj_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    obj = _evaluare_nodum(interp, obj_nodus);
    si (interp->error_accidit) redde _valor_nihil();

    /* Tractare array methods */
    si (obj.genus == SPUTNIK_VALOR_XAR)
    {
        si (_est_methodus_xar(nodus->valor))
        {
            redde _valor_methodus_xar(obj.ut.xar, nodus->valor);
        }
        _error(interp, nodus, "Membrum xar ignotum");
        redde _valor_nihil();
    }

    /* Tractare string methods */
    si (obj.genus == SPUTNIK_VALOR_CHORDA)
    {
        si (_est_methodus_chorda(nodus->valor))
        {
            redde _valor_methodus_chorda(obj.ut.chorda_valor, nodus->valor);
        }
        _error(interp, nodus, "Membrum chorda ignotum");
        redde _valor_nihil();
    }

    /* Tractare entitas */
    si (obj.genus == SPUTNIK_VALOR_ENTITAS)
    {
        Entitas* e;
        chorda* prop_val;

        e = obj.ut.entitas;
        si (e == NIHIL)
        {
            _error(interp, nodus, "Entitas est nihil");
            redde _valor_nihil();
        }

        /* Proprietates speciales */
        si (chorda_aequalis_literis(nodus->valor, "id"))
        {
            redde _valor_chorda(*e->id);
        }
        si (chorda_aequalis_literis(nodus->valor, "genus"))
        {
            redde _valor_chorda(*e->genus);
        }

        /* Methodi entitatis */
        si (chorda_aequalis_literis(nodus->valor, "add_relation") ||
            chorda_aequalis_literis(nodus->valor, "related") ||
            chorda_aequalis_literis(nodus->valor, "related_all") ||
            chorda_aequalis_literis(nodus->valor, "remove_relation") ||
            chorda_aequalis_literis(nodus->valor, "add_tag") ||
            chorda_aequalis_literis(nodus->valor, "has_tag") ||
            chorda_aequalis_literis(nodus->valor, "remove_tag"))
        {
            redde _valor_methodus_entitas(e, nodus->valor);
        }

        /* Proprietates regulares - quaerere in entitate */
        {
            chorda* clavis_internata;

            /* Internare clavem ut pointer comparatio operetur */
            clavis_internata = chorda_internare(interp->intern, nodus->valor);
            si (clavis_internata != NIHIL)
            {
                prop_val = entitas_proprietas_capere(e, clavis_internata);
                si (prop_val != NIHIL)
                {
                    redde _valor_chorda(*prop_val);
                }
            }
        }

        /* Proprietas non inventum - redde nihil */
        redde _valor_nihil();
    }

    /* Tractare repositorium */
    si (obj.genus == SPUTNIK_VALOR_REPOSITORIUM)
    {
        EntitasRepositorium* repo;

        repo = obj.ut.repositorium;
        si (repo == NIHIL)
        {
            _error(interp, nodus, "Repositorium est nihil");
            redde _valor_nihil();
        }

        /* Methodi repositorii */
        si (chorda_aequalis_literis(nodus->valor, "get") ||
            chorda_aequalis_literis(nodus->valor, "query") ||
            chorda_aequalis_literis(nodus->valor, "tagged") ||
            chorda_aequalis_literis(nodus->valor, "search") ||
            chorda_aequalis_literis(nodus->valor, "roots") ||
            chorda_aequalis_literis(nodus->valor, "scaffold"))
        {
            redde _valor_methodus_repositorium(repo, nodus->valor);
        }

        _error(interp, nodus, "Methodus repositorii ignota");
        redde _valor_nihil();
    }

    si (obj.genus != SPUTNIK_VALOR_OBJECTUM)
    {
        _error(interp, nodus, "Non potest accedere membrum non-objecti");
        redde _valor_nihil();
    }

    si (tabula_dispersa_invenire(obj.ut.objectum, nodus->valor, &valor_ptr))
    {
        redde *(SputnikValor*)valor_ptr;
    }

    /* Membrum non inventum - redde nihil */
    redde _valor_nihil();
}

interior SputnikValor
_eval_accessum_indice(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* obj_nodus;
    SputnikAstNodus* index_nodus;
    SputnikValor obj;
    SputnikValor index;

    si (xar_numerus(nodus->liberi) < II)
    {
        _error(interp, nodus, "Accessus indice requirit objectum et indicem");
        redde _valor_nihil();
    }

    obj_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    index_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, I);

    obj = _evaluare_nodum(interp, obj_nodus);
    si (interp->error_accidit) redde _valor_nihil();

    index = _evaluare_nodum(interp, index_nodus);
    si (interp->error_accidit) redde _valor_nihil();

    si (obj.genus == SPUTNIK_VALOR_XAR)
    {
        i32 idx;
        SputnikValor* elem;

        si (index.genus != SPUTNIK_VALOR_NUMERUS)
        {
            _error(interp, nodus, "Index array debet esse numerus");
            redde _valor_nihil();
        }

        idx = (i32)index.ut.numerus;
        si (idx < ZEPHYRUM || idx >= xar_numerus(obj.ut.xar))
        {
            _error(interp, nodus, "Index extra limites");
            redde _valor_nihil();
        }

        elem = xar_obtinere(obj.ut.xar, idx);
        redde *elem;
    }
    alioquin si (obj.genus == SPUTNIK_VALOR_OBJECTUM)
    {
        vacuum* valor_ptr;
        chorda clavis;

        si (index.genus != SPUTNIK_VALOR_CHORDA)
        {
            _error(interp, nodus, "Clavis objecti debet esse chorda");
            redde _valor_nihil();
        }

        clavis = index.ut.chorda_valor;
        si (tabula_dispersa_invenire(obj.ut.objectum, clavis, &valor_ptr))
        {
            redde *(SputnikValor*)valor_ptr;
        }

        redde _valor_nihil();
    }
    alioquin si (obj.genus == SPUTNIK_VALOR_CHORDA)
    {
        i32 idx;
        chorda substr;

        si (index.genus != SPUTNIK_VALOR_NUMERUS)
        {
            _error(interp, nodus, "Index chordae debet esse numerus");
            redde _valor_nihil();
        }

        idx = (i32)index.ut.numerus;
        si (idx < ZEPHYRUM || idx >= obj.ut.chorda_valor.mensura)
        {
            _error(interp, nodus, "Index extra limites");
            redde _valor_nihil();
        }

        /* Creare chorda singularis characteris */
        substr.datum = obj.ut.chorda_valor.datum + idx;
        substr.mensura = I;
        redde _valor_chorda(substr);
    }

    _error(interp, nodus, "Non potest accedere per indicem hunc genus");
    redde _valor_nihil();
}

interior SputnikValor
_eval_xar_literalem(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    Xar* xar;
    i32 i;
    i32 num;
    SputnikAstNodus* elem_nodus;
    SputnikValor elem;
    SputnikValor* elem_copia;

    xar = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (xar == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        elem_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, i);
        elem = _evaluare_nodum(interp, elem_nodus);
        si (interp->error_accidit) redde _valor_nihil();

        elem_copia = xar_addere(xar);
        si (elem_copia == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *elem_copia = elem;
    }

    redde _valor_xar(xar);
}

interior SputnikValor
_eval_objectum_literalem(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    TabulaDispersa* tabula;
    i32 i;
    i32 num;
    SputnikAstNodus* clavis_nodus;
    SputnikAstNodus* valor_nodus;
    SputnikValor valor;
    SputnikValor* valor_copia;
    chorda clavis;

    tabula = tabula_dispersa_creare_chorda(interp->piscina, XVI);
    si (tabula == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i += II)
    {
        si (i + I >= num)
        {
            _error(interp, nodus, "Objectum literalis invalidum");
            redde _valor_nihil();
        }

        clavis_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, i);
        valor_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, i + I);

        /* Clavis est identificator vel chorda */
        si (clavis_nodus->genus == SPUTNIK_AST_IDENTIFICATOR)
        {
            clavis = clavis_nodus->valor;
        }
        alioquin si (clavis_nodus->genus == SPUTNIK_AST_CHORDA_LITERALIS)
        {
            clavis = clavis_nodus->valor;
        }
        alioquin
        {
            _error(interp, nodus, "Clavis objecti debet esse identificator vel chorda");
            redde _valor_nihil();
        }

        valor = _evaluare_nodum(interp, valor_nodus);
        si (interp->error_accidit) redde _valor_nihil();

        valor_copia = piscina_allocare(interp->piscina, magnitudo(SputnikValor));
        si (valor_copia == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *valor_copia = valor;

        tabula_dispersa_inserere(tabula, clavis, valor_copia);
    }

    redde _valor_objectum(tabula);
}

interior SputnikValor
_vocare_functionem(SputnikInterpres* interp, SputnikAstNodus* functio, Xar* argumenta, SputnikAstNodus* nodus)
{
    SputnikAmbitus* prior;
    i32 num_param;
    i32 num_args;
    i32 i;
    SputnikAstNodus* param_nodus;
    SputnikAstNodus* corpus;
    SputnikValor* arg;
    SputnikValor resultus;
    b32 est_sagitta;
    b32 est_expressio_corpus;

    /* Functio node: liberi = [param0, param1, ..., corpus] */
    num_param = xar_numerus(functio->liberi) - I;
    num_args = xar_numerus(argumenta);

    si (num_args != num_param)
    {
        _error(interp, nodus, "Numerus argumentorum non congruit");
        redde _valor_nihil();
    }

    /* Salvare scope currens */
    prior = interp->ambitus_currens;

    /* Creare novum scope (lexical: parens est globalis) */
    interp->ambitus_currens = _creare_ambitum(interp, interp->ambitus_globalis);
    si (interp->ambitus_currens == NIHIL)
    {
        interp->ambitus_currens = prior;
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    /* Definire parametra */
    per (i = ZEPHYRUM; i < num_param; i++)
    {
        param_nodus = *(SputnikAstNodus**)xar_obtinere(functio->liberi, i);
        arg = xar_obtinere(argumenta, i);
        _definire_variabilem(interp, param_nodus->valor, *arg, FALSUM);
    }

    /* Obtinere corpus */
    corpus = *(SputnikAstNodus**)xar_obtinere(functio->liberi, num_param);

    /* Verificare si arrow function cum expressio corpus (implicita redde) */
    est_sagitta = (functio->genus == SPUTNIK_AST_FUNCTIO_SAGITTA);
    est_expressio_corpus = (corpus->genus != SPUTNIK_AST_SENTENTIA_GREX);

    si (est_sagitta && est_expressio_corpus)
    {
        /* Arrow function cum expressio: implicit return */
        resultus = _evaluare_nodum(interp, corpus);
    }
    alioquin
    {
        /* Regular function vel arrow cum block: executare corpus */
        _executare_sententiam(interp, corpus);

        /* Capturare return value */
        si (interp->redde_activa)
        {
            resultus = interp->redde_valor;
            interp->redde_activa = FALSUM;
        }
        alioquin
        {
            resultus = _valor_nihil();
        }
    }

    /* Restaurare scope */
    interp->ambitus_currens = prior;

    redde resultus;
}

interior SputnikValor
_eval_vocationem(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* callee_nodus;
    SputnikValor callee;
    Xar* argumenta;
    i32 i;
    i32 num;
    SputnikAstNodus* arg_nodus;
    SputnikValor arg;
    SputnikValor* arg_copia;
    chorda functio_titulus;

    si (xar_numerus(nodus->liberi) < I)
    {
        _error(interp, nodus, "Vocatio requirit callee");
        redde _valor_nihil();
    }

    callee_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);

    /* Evaluare argumenta */
    argumenta = xar_creare(interp->piscina, magnitudo(SputnikValor));
    si (argumenta == NIHIL)
    {
        _error(interp, nodus, "Memoria exhausta");
        redde _valor_nihil();
    }

    num = xar_numerus(nodus->liberi);
    per (i = I; i < num; i++)
    {
        arg_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, i);
        arg = _evaluare_nodum(interp, arg_nodus);
        si (interp->error_accidit) redde _valor_nihil();

        arg_copia = xar_addere(argumenta);
        si (arg_copia == NIHIL)
        {
            _error(interp, nodus, "Memoria exhausta");
            redde _valor_nihil();
        }
        *arg_copia = arg;
    }

    /* Verificare si intrinseca */
    si (callee_nodus->genus == SPUTNIK_AST_IDENTIFICATOR)
    {
        functio_titulus = callee_nodus->valor;
        si (_est_intrinseca(functio_titulus))
        {
            redde _vocare_intrinsecam(interp, functio_titulus, argumenta, nodus);
        }
    }

    /* Evaluare callee */
    callee = _evaluare_nodum(interp, callee_nodus);
    si (interp->error_accidit) redde _valor_nihil();

    /* Tractare array method calls */
    si (callee.genus == SPUTNIK_VALOR_METHODUS_XAR)
    {
        redde _vocare_methodum_xar(interp, &callee.ut.methodus_xar, argumenta, nodus);
    }

    /* Tractare entity method calls */
    si (callee.genus == SPUTNIK_VALOR_METHODUS_ENTITAS)
    {
        redde _vocare_methodum_entitas(interp, &callee.ut.methodus_entitas, argumenta, nodus);
    }

    /* Tractare repository method calls */
    si (callee.genus == SPUTNIK_VALOR_METHODUS_REPOSITORIUM)
    {
        redde _vocare_methodum_repositorium(interp, &callee.ut.methodus_repositorium, argumenta, nodus);
    }

    /* Tractare string method calls */
    si (callee.genus == SPUTNIK_VALOR_METHODUS_CHORDA)
    {
        redde _vocare_methodum_chorda(interp, &callee.ut.methodus_chorda, argumenta, nodus);
    }

    si (callee.genus != SPUTNIK_VALOR_FUNCTIO)
    {
        _error(interp, nodus, "Non potest vocare non-functionem");
        redde _valor_nihil();
    }

    redde _vocare_functionem(interp, callee.ut.functio, argumenta, nodus);
}


/* ==================================================
 * Node Dispatcher
 * ================================================== */

interior SputnikValor
_evaluare_nodum(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    si (nodus == NIHIL || interp->error_accidit)
    {
        redde _valor_nihil();
    }

    commutatio (nodus->genus)
    {
        casus SPUTNIK_AST_NUMERUS_LITERALIS:
            redde _eval_numerum(interp, nodus);

        casus SPUTNIK_AST_PECUNIA_LITERALIS:
            redde _eval_pecuniam(interp, nodus);

        casus SPUTNIK_AST_CHORDA_LITERALIS:
            redde _eval_chordam(interp, nodus);

        casus SPUTNIK_AST_VERUM_LITERALIS:
            redde _eval_verum(interp, nodus);

        casus SPUTNIK_AST_FALSUM_LITERALIS:
            redde _eval_falsum(interp, nodus);

        casus SPUTNIK_AST_NIHIL_LITERALIS:
            redde _eval_nihil(interp, nodus);

        casus SPUTNIK_AST_IDENTIFICATOR:
            redde _eval_identificatorem(interp, nodus);

        casus SPUTNIK_AST_OPERATIO_BINARIA:
            redde _eval_binarium(interp, nodus);

        casus SPUTNIK_AST_OPERATIO_UNARIA:
            redde _eval_unarium(interp, nodus);

        casus SPUTNIK_AST_TERNARIA:
            redde _eval_ternarium(interp, nodus);

        casus SPUTNIK_AST_ASSIGNATIO:
            redde _eval_assignationem(interp, nodus);

        casus SPUTNIK_AST_ACCESSUS_MEMBRI:
            redde _eval_accessum_membri(interp, nodus);

        casus SPUTNIK_AST_ACCESSUS_INDICE:
            redde _eval_accessum_indice(interp, nodus);

        casus SPUTNIK_AST_XAR_LITERALIS:
            redde _eval_xar_literalem(interp, nodus);

        casus SPUTNIK_AST_OBJECTUM_LITERALIS:
            redde _eval_objectum_literalem(interp, nodus);

        casus SPUTNIK_AST_VOCATIO:
            redde _eval_vocationem(interp, nodus);

        casus SPUTNIK_AST_SIGNUM:
            /* Tags redde ut chordas */
            redde _valor_chorda(nodus->valor);

        casus SPUTNIK_AST_FUNCTIO_SAGITTA:
            /* Arrow functions redde ut function value */
            redde _valor_functio(nodus);

        ordinarius:
            _error(interp, nodus, "Genus nodi non evaluabile");
            redde _valor_nihil();
    }
}


/* ==================================================
 * Statement Execution
 * ================================================== */

interior vacuum
_exec_declarationem_let(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikValor valor;
    chorda titulus;

    titulus = nodus->valor;

    si (xar_numerus(nodus->liberi) > ZEPHYRUM)
    {
        SputnikAstNodus* valor_nodus;
        valor_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
        valor = _evaluare_nodum(interp, valor_nodus);
        si (interp->error_accidit) redde;
    }
    alioquin
    {
        valor = _valor_nihil();
    }

    si (!_definire_variabilem(interp, titulus, valor, FALSUM))
    {
        _error(interp, nodus, "Non potest definire variabilem");
    }
}

interior vacuum
_exec_declarationem_const(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikValor valor;
    SputnikAstNodus* valor_nodus;
    chorda titulus;

    titulus = nodus->valor;

    si (xar_numerus(nodus->liberi) < I)
    {
        _error(interp, nodus, "Const requirit valorem initialem");
        redde;
    }

    valor_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    valor = _evaluare_nodum(interp, valor_nodus);
    si (interp->error_accidit) redde;

    si (!_definire_variabilem(interp, titulus, valor, VERUM))
    {
        _error(interp, nodus, "Non potest definire constantem");
    }
}

interior vacuum
_exec_declarationem_functio(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    chorda titulus;
    SputnikValor functio;

    titulus = nodus->valor;
    functio = _valor_functio(nodus);

    si (!_definire_variabilem(interp, titulus, functio, FALSUM))
    {
        _error(interp, nodus, "Non potest definire functionem");
    }
}

interior vacuum
_exec_declarationem_entitas(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    chorda titulus;
    SputnikAstNodus* genus_nodus;
    chorda genus_path;
    Entitas* entitas;
    SputnikValor valor;

    /* Nomen variabilis */
    titulus = nodus->valor;

    /* Capere genus path ex primo infante */
    si (xar_numerus(nodus->liberi) < I)
    {
        _error(interp, nodus, "Declaratio entitas mancus genus");
        redde;
    }

    genus_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    genus_path = genus_nodus->valor;

    /* Verificare repositorium */
    si (interp->repositorium == NIHIL)
    {
        _error(interp, nodus, "Repositorium non configuratum pro entitates");
        redde;
    }

    /* Creare entitatem via repositorio */
    entitas = interp->repositorium->entitas_creare(
        interp->repositorium->datum,
        (constans character*)genus_path.datum);

    si (entitas == NIHIL)
    {
        _error(interp, nodus, "Non potest creare entitatem");
        redde;
    }

    /* Creare valorem entitas et definire variabilem */
    valor = _valor_entitas(entitas);

    si (!_definire_variabilem(interp, titulus, valor, FALSUM))
    {
        _error(interp, nodus, "Non potest definire variabilem entitas");
    }
}

interior vacuum
_exec_sententiam_si(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* conditio_nodus;
    SputnikAstNodus* tunc_nodus;
    SputnikAstNodus* alioquin_nodus;
    SputnikValor conditio;
    i32 num_liberi;

    num_liberi = xar_numerus(nodus->liberi);
    si (num_liberi < II)
    {
        _error(interp, nodus, "Sententia si requirit conditio et corpus");
        redde;
    }

    conditio_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    tunc_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, I);

    conditio = _evaluare_nodum(interp, conditio_nodus);
    si (interp->error_accidit) redde;

    si (_est_verum(&conditio))
    {
        _executare_sententiam(interp, tunc_nodus);
    }
    alioquin si (num_liberi >= III)
    {
        alioquin_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, II);
        _executare_sententiam(interp, alioquin_nodus);
    }
}

interior vacuum
_exec_sententiam_dum(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* conditio_nodus;
    SputnikAstNodus* corpus_nodus;
    SputnikValor conditio;

    si (xar_numerus(nodus->liberi) < II)
    {
        _error(interp, nodus, "Sententia dum requirit conditio et corpus");
        redde;
    }

    conditio_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    corpus_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, I);

    dum (!interp->error_accidit && !interp->redde_activa)
    {
        conditio = _evaluare_nodum(interp, conditio_nodus);
        si (interp->error_accidit) redde;

        si (!_est_verum(&conditio))
        {
            frange;
        }

        _executare_sententiam(interp, corpus_nodus);

        /* Tractare continue - reset et perge ad conditionem */
        si (interp->perge_activa)
        {
            interp->perge_activa = FALSUM;
            perge;
        }

        /* Tractare break - exit loop */
        si (interp->frange_activa)
        {
            interp->frange_activa = FALSUM;
            frange;
        }
    }
}

interior vacuum
_exec_sententiam_per(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* init_nodus;
    SputnikAstNodus* conditio_nodus;
    SputnikAstNodus* incrementum_nodus;
    SputnikAstNodus* corpus_nodus;
    SputnikValor conditio;

    si (xar_numerus(nodus->liberi) < IV)
    {
        _error(interp, nodus, "Sententia per requirit init, conditio, incrementum, corpus");
        redde;
    }

    init_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    conditio_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, I);
    incrementum_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, II);
    corpus_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, III);

    /* Novum scope pro for loop */
    _intrare_ambitum(interp);

    /* Initializatio */
    si (init_nodus != NIHIL)
    {
        _executare_sententiam(interp, init_nodus);
        si (interp->error_accidit)
        {
            _exire_ambitum(interp);
            redde;
        }
    }

    /* Loop */
    dum (!interp->error_accidit && !interp->redde_activa)
    {
        /* Conditio */
        si (conditio_nodus != NIHIL)
        {
            conditio = _evaluare_nodum(interp, conditio_nodus);
            si (interp->error_accidit)
            {
                _exire_ambitum(interp);
                redde;
            }
            si (!_est_verum(&conditio))
            {
                frange;
            }
        }

        /* Corpus */
        _executare_sententiam(interp, corpus_nodus);

        /* Tractare break - exit loop */
        si (interp->frange_activa)
        {
            interp->frange_activa = FALSUM;
            frange;
        }

        si (interp->error_accidit || interp->redde_activa)
        {
            frange;
        }

        /* Tractare continue - skip ad incrementum */
        si (interp->perge_activa)
        {
            interp->perge_activa = FALSUM;
            /* Continuare ad incrementum */
        }

        /* Incrementum */
        si (incrementum_nodus != NIHIL)
        {
            _executare_sententiam(interp, incrementum_nodus);
            si (interp->error_accidit)
            {
                frange;
            }
        }
    }

    _exire_ambitum(interp);
}

interior vacuum
_exec_sententiam_redde(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    si (xar_numerus(nodus->liberi) > ZEPHYRUM)
    {
        SputnikAstNodus* valor_nodus;
        valor_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
        interp->redde_valor = _evaluare_nodum(interp, valor_nodus);
    }
    alioquin
    {
        interp->redde_valor = _valor_nihil();
    }

    interp->redde_activa = VERUM;
}

interior vacuum
_exec_sententiam_grex(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    i32 i;
    i32 num;
    SputnikAstNodus* sententia;

    _intrare_ambitum(interp);

    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num && !interp->error_accidit && !interp->redde_activa &&
         !interp->frange_activa && !interp->perge_activa; i++)
    {
        sententia = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, i);
        _executare_sententiam(interp, sententia);
    }

    _exire_ambitum(interp);
}

interior vacuum
_exec_sententiam_expressio(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* expressio;

    si (xar_numerus(nodus->liberi) < I)
    {
        redde;
    }

    expressio = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);
    _evaluare_nodum(interp, expressio);
}

interior vacuum
_exec_sententiam_frange(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    (vacuum)nodus;
    interp->frange_activa = VERUM;
}

interior vacuum
_exec_sententiam_perge(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    (vacuum)nodus;
    interp->perge_activa = VERUM;
}

interior vacuum
_exec_sententiam_incrementum(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* target_nodus;
    SputnikValor* valor;
    chorda titulus;

    si (xar_numerus(nodus->liberi) < I)
    {
        _error(interp, nodus, "Incrementum requirit operandum");
        redde;
    }

    target_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);

    si (target_nodus->genus != SPUTNIK_AST_IDENTIFICATOR)
    {
        _error(interp, nodus, "Incrementum requirit identificatorem");
        redde;
    }

    titulus = target_nodus->valor;

    /* Verificare non constans */
    si (_est_constans(interp, titulus))
    {
        _error(interp, nodus, "Non potest incrementare constantem");
        redde;
    }

    valor = _invenire_variabilem(interp, titulus);
    si (valor == NIHIL)
    {
        _error(interp, nodus, "Variabilis non definita");
        redde;
    }

    si (valor->genus != SPUTNIK_VALOR_NUMERUS)
    {
        _error(interp, nodus, "Incrementum requirit numerum");
        redde;
    }

    valor->ut.numerus += 1.0;
}

interior vacuum
_exec_sententiam_decrementum(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    SputnikAstNodus* target_nodus;
    SputnikValor* valor;
    chorda titulus;

    si (xar_numerus(nodus->liberi) < I)
    {
        _error(interp, nodus, "Decrementum requirit operandum");
        redde;
    }

    target_nodus = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM);

    si (target_nodus->genus != SPUTNIK_AST_IDENTIFICATOR)
    {
        _error(interp, nodus, "Decrementum requirit identificatorem");
        redde;
    }

    titulus = target_nodus->valor;

    /* Verificare non constans */
    si (_est_constans(interp, titulus))
    {
        _error(interp, nodus, "Non potest decrementare constantem");
        redde;
    }

    valor = _invenire_variabilem(interp, titulus);
    si (valor == NIHIL)
    {
        _error(interp, nodus, "Variabilis non definita");
        redde;
    }

    si (valor->genus != SPUTNIK_VALOR_NUMERUS)
    {
        _error(interp, nodus, "Decrementum requirit numerum");
        redde;
    }

    valor->ut.numerus -= 1.0;
}

interior vacuum
_executare_sententiam(SputnikInterpres* interp, SputnikAstNodus* nodus)
{
    si (nodus == NIHIL || interp->error_accidit || interp->redde_activa ||
        interp->frange_activa || interp->perge_activa)
    {
        redde;
    }

    commutatio (nodus->genus)
    {
        casus SPUTNIK_AST_DECLARATIO_LET:
            _exec_declarationem_let(interp, nodus);
            frange;

        casus SPUTNIK_AST_DECLARATIO_CONST:
            _exec_declarationem_const(interp, nodus);
            frange;

        casus SPUTNIK_AST_DECLARATIO_FUNCTIO:
            _exec_declarationem_functio(interp, nodus);
            frange;

        casus SPUTNIK_AST_DECLARATIO_ENTITAS:
            _exec_declarationem_entitas(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_SI:
            _exec_sententiam_si(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_DUM:
            _exec_sententiam_dum(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_PER:
            _exec_sententiam_per(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_REDDE:
            _exec_sententiam_redde(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_GREX:
            _exec_sententiam_grex(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_EXPRESSIO:
            _exec_sententiam_expressio(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_FRANGE:
            _exec_sententiam_frange(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_PERGE:
            _exec_sententiam_perge(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_INCREMENT:
            _exec_sententiam_incrementum(interp, nodus);
            frange;

        casus SPUTNIK_AST_SENTENTIA_DECREMENT:
            _exec_sententiam_decrementum(interp, nodus);
            frange;

        ordinarius:
            /* Tentare evaluare ut expressio */
            _evaluare_nodum(interp, nodus);
            frange;
    }
}


/* ==================================================
 * Program Execution
 * ================================================== */

interior SputnikValor
_executare_programma(SputnikInterpres* interp, SputnikAstNodus* programma)
{
    i32 i;
    i32 num;
    SputnikAstNodus* sententia;
    SputnikValor ultima;

    ultima = _valor_nihil();
    num = xar_numerus(programma->liberi);

    per (i = ZEPHYRUM; i < num && !interp->error_accidit && !interp->redde_activa; i++)
    {
        sententia = *(SputnikAstNodus**)xar_obtinere(programma->liberi, i);

        /* Si expression statement, captura valorem */
        si (sententia->genus == SPUTNIK_AST_SENTENTIA_EXPRESSIO &&
            xar_numerus(sententia->liberi) > ZEPHYRUM)
        {
            SputnikAstNodus* expr;
            expr = *(SputnikAstNodus**)xar_obtinere(sententia->liberi, ZEPHYRUM);
            ultima = _evaluare_nodum(interp, expr);
        }
        alioquin
        {
            _executare_sententiam(interp, sententia);
        }
    }

    /* Si return activum, usa redde_valor */
    si (interp->redde_activa)
    {
        ultima = interp->redde_valor;
    }

    redde ultima;
}


/* ==================================================
 * Functiones Principales
 * ================================================== */

SputnikInterpresResultus
sputnik_interpretare(
    SputnikAstNodus*     radix,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SputnikInterpresResultus resultus;
    SputnikInterpres interp;

    resultus.successus = FALSUM;
    resultus.valor = _valor_nihil();
    resultus.error_nuntius.datum = NIHIL;
    resultus.error_nuntius.mensura = ZEPHYRUM;
    resultus.error_linea = ZEPHYRUM;
    resultus.error_columna = ZEPHYRUM;

    si (radix == NIHIL || piscina == NIHIL)
    {
        redde resultus;
    }

    /* Initializare interpreter */
    interp.piscina = piscina;
    interp.intern = intern;
    interp.repositorium = NIHIL;
    interp.error_accidit = FALSUM;
    interp.error_nuntius.datum = NIHIL;
    interp.error_nuntius.mensura = ZEPHYRUM;
    interp.error_linea = ZEPHYRUM;
    interp.error_columna = ZEPHYRUM;
    interp.redde_activa = FALSUM;
    interp.redde_valor = _valor_nihil();
    interp.frange_activa = FALSUM;
    interp.perge_activa = FALSUM;

    /* Creare global scope */
    interp.ambitus_globalis = _creare_ambitum(&interp, NIHIL);
    interp.ambitus_currens = interp.ambitus_globalis;

    si (interp.ambitus_globalis == NIHIL)
    {
        resultus.error_nuntius = chorda_ex_literis("Memoria exhausta", piscina);
        redde resultus;
    }

    /* Iniicere REPO variabilem globalem (null sine repositorio) */
    {
        chorda repo_nomen;
        repo_nomen = chorda_ex_literis("REPO", piscina);
        _definire_variabilem(&interp, repo_nomen, _valor_nihil(), VERUM);
    }

    /* Executare */
    resultus.valor = _executare_programma(&interp, radix);

    si (interp.error_accidit)
    {
        resultus.error_nuntius = interp.error_nuntius;
        resultus.error_linea = interp.error_linea;
        resultus.error_columna = interp.error_columna;
        redde resultus;
    }

    resultus.successus = VERUM;
    redde resultus;
}

SputnikInterpresResultus
sputnik_evaluare_ex_chorda(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SputnikParserResultus parse_res;
    SputnikInterpresResultus resultus;

    resultus.successus = FALSUM;
    resultus.valor = _valor_nihil();
    resultus.error_nuntius.datum = NIHIL;
    resultus.error_nuntius.mensura = ZEPHYRUM;
    resultus.error_linea = ZEPHYRUM;
    resultus.error_columna = ZEPHYRUM;

    /* Parsere */
    parse_res = sputnik_parsere_ex_fonte(fons, piscina, intern);
    si (!parse_res.successus)
    {
        resultus.error_nuntius = parse_res.error_nuntius;
        resultus.error_linea = parse_res.error_linea;
        resultus.error_columna = parse_res.error_columna;
        redde resultus;
    }

    /* Interpretare */
    redde sputnik_interpretare(parse_res.radix, piscina, intern);
}

SputnikInterpresResultus
sputnik_evaluare(
    constans character*  fons,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    chorda ch;
    ch = chorda_ex_literis(fons, piscina);
    redde sputnik_evaluare_ex_chorda(ch, piscina, intern);
}


/* ==================================================
 * API cum Repositorio (Entity-Aware)
 * ================================================== */

SputnikInterpresResultus
sputnik_interpretare_cum_repositorio(
    SputnikAstNodus*     radix,
    Piscina*             piscina,
    InternamentumChorda* intern,
    EntitasRepositorium* repositorium)
{
    SputnikInterpresResultus resultus;
    SputnikInterpres interp;

    resultus.successus = FALSUM;
    resultus.valor = _valor_nihil();
    resultus.error_nuntius.datum = NIHIL;
    resultus.error_nuntius.mensura = ZEPHYRUM;
    resultus.error_linea = ZEPHYRUM;
    resultus.error_columna = ZEPHYRUM;

    si (radix == NIHIL || piscina == NIHIL)
    {
        redde resultus;
    }

    /* Initializare interpreter */
    interp.piscina = piscina;
    interp.intern = intern;
    interp.repositorium = repositorium;
    interp.error_accidit = FALSUM;
    interp.error_nuntius.datum = NIHIL;
    interp.error_nuntius.mensura = ZEPHYRUM;
    interp.error_linea = ZEPHYRUM;
    interp.error_columna = ZEPHYRUM;
    interp.redde_activa = FALSUM;
    interp.redde_valor = _valor_nihil();
    interp.frange_activa = FALSUM;
    interp.perge_activa = FALSUM;

    /* Creare global scope */
    interp.ambitus_globalis = _creare_ambitum(&interp, NIHIL);
    interp.ambitus_currens = interp.ambitus_globalis;

    si (interp.ambitus_globalis == NIHIL)
    {
        resultus.error_nuntius = chorda_ex_literis("Memoria exhausta", piscina);
        redde resultus;
    }

    /* Iniicere REPO variabilem globalem */
    {
        chorda repo_nomen;
        SputnikValor repo_valor;

        repo_nomen = chorda_ex_literis("REPO", piscina);
        si (repositorium != NIHIL)
        {
            repo_valor = _valor_repositorium(repositorium);
        }
        alioquin
        {
            repo_valor = _valor_nihil();
        }
        _definire_variabilem(&interp, repo_nomen, repo_valor, VERUM);
    }

    /* Executare */
    resultus.valor = _executare_programma(&interp, radix);

    si (interp.error_accidit)
    {
        resultus.error_nuntius = interp.error_nuntius;
        resultus.error_linea = interp.error_linea;
        resultus.error_columna = interp.error_columna;
        redde resultus;
    }

    resultus.successus = VERUM;
    redde resultus;
}

SputnikInterpresResultus
sputnik_evaluare_ex_chorda_cum_repositorio(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern,
    EntitasRepositorium* repositorium)
{
    SputnikParserResultus parse_res;
    SputnikInterpresResultus resultus;

    resultus.successus = FALSUM;
    resultus.valor = _valor_nihil();
    resultus.error_nuntius.datum = NIHIL;
    resultus.error_nuntius.mensura = ZEPHYRUM;
    resultus.error_linea = ZEPHYRUM;
    resultus.error_columna = ZEPHYRUM;

    /* Parsere */
    parse_res = sputnik_parsere_ex_fonte(fons, piscina, intern);
    si (!parse_res.successus)
    {
        resultus.error_nuntius = parse_res.error_nuntius;
        resultus.error_linea = parse_res.error_linea;
        resultus.error_columna = parse_res.error_columna;
        redde resultus;
    }

    /* Interpretare */
    redde sputnik_interpretare_cum_repositorio(parse_res.radix, piscina, intern, repositorium);
}

SputnikInterpresResultus
sputnik_evaluare_cum_repositorio(
    constans character*  fons,
    Piscina*             piscina,
    InternamentumChorda* intern,
    EntitasRepositorium* repositorium)
{
    chorda ch;
    ch = chorda_ex_literis(fons, piscina);
    redde sputnik_evaluare_ex_chorda_cum_repositorio(ch, piscina, intern, repositorium);
}


/* ==================================================
 * Utilitas
 * ================================================== */

constans character*
sputnik_valor_genus_nomen(SputnikValorGenus genus)
{
    commutatio (genus)
    {
        casus SPUTNIK_VALOR_NIHIL:            redde "null";
        casus SPUTNIK_VALOR_NUMERUS:          redde "number";
        casus SPUTNIK_VALOR_CHORDA:           redde "string";
        casus SPUTNIK_VALOR_VERUM:            redde "boolean";
        casus SPUTNIK_VALOR_FALSUM:           redde "boolean";
        casus SPUTNIK_VALOR_XAR:              redde "array";
        casus SPUTNIK_VALOR_OBJECTUM:         redde "object";
        casus SPUTNIK_VALOR_FUNCTIO:          redde "function";
        casus SPUTNIK_VALOR_METHODUS_XAR:     redde "function";
        casus SPUTNIK_VALOR_METHODUS_CHORDA:  redde "function";
        casus SPUTNIK_VALOR_ENTITAS:               redde "entity";
        casus SPUTNIK_VALOR_METHODUS_ENTITAS:      redde "function";
        casus SPUTNIK_VALOR_REPOSITORIUM:          redde "repository";
        casus SPUTNIK_VALOR_METHODUS_REPOSITORIUM: redde "function";
        casus SPUTNIK_VALOR_PECUNIA:               redde "currency";
        ordinarius:                                redde "unknown";
    }
}

chorda
sputnik_valor_ad_chordam(SputnikValor* valor, Piscina* piscina)
{
    SputnikInterpres interp;

    interp.piscina = piscina;
    interp.intern = NIHIL;
    interp.error_accidit = FALSUM;

    redde _ad_chordam(&interp, valor);
}
