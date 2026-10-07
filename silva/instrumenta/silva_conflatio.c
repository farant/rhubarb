/* silva_conflatio.c - Vide silva_conflatio.h. Transcriptum ex
 * briar_amalgama.c (functiones generales); porta extractionis =
 * aurum briar gamma.c octetim idem. */

#include "silva_conflatio.h"
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior b32
_incipit (
                chorda  c,
    constans character* praefixum)
{
    i32 m = (i32)strlen(praefixum);

    redde (b32)(c.mensura >= m
        && memcmp(c.datum, praefixum, (size_t)m) == ZEPHYRUM);
}

interior b32
_terminatur (
                chorda  c,
    constans character* suffixum)
{
    i32 m = (i32)strlen(suffixum);

    redde (b32)(c.mensura >= m
        && memcmp(c.datum + (c.mensura - m), suffixum, (size_t)m)
            == ZEPHYRUM);
}

/* radix clientis viae aut NIHIL - caput et fons in eodem directorio */
interior constans character*
_radix_clientis (
    constans ConflatioContextus* contextus,
                         chorda  via)
{
    i32 r;

    si (contextus->radices_clientium == NIHIL)
    {
        redde NIHIL;
    }
    per (r = ZEPHYRUM; contextus->radices_clientium[r] != NIHIL; r++)
    {
        si (_incipit(via, contextus->radices_clientium[r]))
        {
            redde contextus->radices_clientium[r];
        }
    }
    redde NIHIL;
}

/* columna quota (0-basata) lineae per tabulatores */
interior chorda
_columna (
    chorda linea,
       i32 quota)
{
    i32 i;
    i32 initium  = ZEPHYRUM;
    i32 n        = ZEPHYRUM;

    per (i = ZEPHYRUM; i <= linea.mensura; i++)
    {
        si (i == linea.mensura || (character)linea.datum[i] == '\t')
        {
            si (n == quota)
            {
                redde chorda_sectio(linea, initium, i);
            }
            n        = n + I;
            initium  = i + I;
        }
    }
    redde _vacua();
}

b32
conflatio_caput_est (
    constans ConflatioContextus* contextus,
                         chorda  via)
{
    redde _incipit(via, "include/")
        || (_radix_clientis(contextus, via) != NIHIL
            && _terminatur(via, ".h"));
}

b32
conflatio_fons_est (
    constans ConflatioContextus* contextus,
                         chorda  via)
{
    redde _incipit(via, "lib/")
        || (_radix_clientis(contextus, via) != NIHIL
            && _terminatur(via, ".c"));
}

b32
conflatio_inclusio_localis (
    chorda  linea,
    chorda* nomen_capitis)
{
    i32 p = ZEPHYRUM;
    i32 initium;

    dum (   p < linea.mensura
         && (linea.datum[p] == ' ' || linea.datum[p] == '\t'))
    {
        p = p + I;
    }
    si (p >= linea.mensura || linea.datum[p] != '#')
    {
        redde FALSUM;
    }
    p = p + I;
    dum (   p < linea.mensura
         && (linea.datum[p] == ' ' || linea.datum[p] == '\t'))
    {
        p = p + I;
    }
    si (   p + VII > linea.mensura
        || memcmp(linea.datum + p, "include", (size_t)VII) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    p = p + VII;
    dum (   p < linea.mensura
         && (linea.datum[p] == ' ' || linea.datum[p] == '\t'))
    {
        p = p + I;
    }
    si (p >= linea.mensura || linea.datum[p] != '"')
    {
        redde FALSUM;
    }
    p        = p + I;
    initium  = p;
    dum (p < linea.mensura && linea.datum[p] != '"')
    {
        p = p + I;
    }
    si (   p                   >= linea.mensura || p - initium < II
        || linea.datum[p - II] != '.' || linea.datum[p - I] != 'h')
    {
        redde FALSUM;
    }
    *nomen_capitis = chorda_sectio(linea, initium, p);
    redde VERUM;
}

chorda
conflatio_stirps (
    chorda via)
{
    i32 initium  = ZEPHYRUM;
    i32 finis    = via.mensura;
    i32 i;

    per (i = ZEPHYRUM; i < via.mensura; i++)
    {
        si ((character)via.datum[i] == '/')
        {
            initium = i + I;
        }
    }
    per (i = finis - I; i > initium; i--)
    {
        si ((character)via.datum[i] == '.')
        {
            finis = i;
            frange;
        }
    }
    redde chorda_sectio(via, initium, finis);
}


/* ==================================================
 * Statica: ordines tabulae symbolorum, per plagulam
 * ================================================== */

b32
conflatio_statica_legere (
               Piscina* piscina,
                chorda  textus,
    ConflatioContextus* contextus)
{
    i32 i = ZEPHYRUM;

    contextus->statica = tabula_dispersa_creare_chorda(piscina, 256);
    si (contextus->statica == NIHIL)
    {
        redde FALSUM;
    }
    dum (i < textus.mensura)
    {
           i32 f = i;
        chorda linea;

        dum (f < textus.mensura && (character)textus.datum[f] != '\n')
        {
            f = f + I;
        }
        linea  = chorda_sectio(textus, i, f);
        i      = f + I;
        si (   linea.mensura             == ZEPHYRUM
            || (character)linea.datum[0] == '#')
        {
            perge;
        }
        {
                       chorda  symbolum  = _columna(linea, ZEPHYRUM);
                       chorda  genus     = _columna(linea, I);
                       chorda  plagula   = _columna(linea, II);
                       vacuum* prior     = NIHIL;
                          Xar* ordines;
            ConflatioStaticum* s;

            si (   symbolum.mensura == ZEPHYRUM
                || genus.mensura    == ZEPHYRUM
                || !conflatio_fons_est(contextus, plagula))
            {
                perge;
            }
            si (tabula_dispersa_invenire(contextus->statica, plagula,
                &prior))
            {
                ordines = (Xar*)prior;
            }
            alioquin
            {
                ordines = xar_creare(piscina,
                    (i32)magnitudo(ConflatioStaticum));
                tabula_dispersa_inserere(contextus->statica, plagula,
                    (vacuum*)ordines);
            }
            s = (ConflatioStaticum*)xar_addere(ordines);
            si (s != NIHIL)
            {
                s->symbolum  = symbolum;
                s->genus     = genus;
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Ordo capitum: profunditate prima, post-ordine
 * ================================================== */

nomen structura {
                        Piscina* piscina;
    constans ConflatioContextus* contextus;
    /* via -> ConflatioPlagula* */
                 TabulaDispersa* per_viam;
                 TabulaDispersa* visa;
    /* constans ConflatioPlagula* */
                            Xar* ordo;
} Ordinatio;

interior vacuum
_caput_visitare (
                     Ordinatio* o,
     constans ConflatioPlagula* plagula)
{
    chorda c = plagula->contentum;
       i32 i = ZEPHYRUM;

    si (tabula_dispersa_continet(o->visa, plagula->via))
    {
        redde;
    }
    tabula_dispersa_inserere(o->visa, plagula->via, (vacuum*)o);
    dum (i < c.mensura)
    {
           i32 f = i;
        chorda linea;
        chorda nomen_capitis;

        dum (f < c.mensura && c.datum[f] != '\n')
        {
            f = f + I;
        }
        linea  = chorda_sectio(c, i, f);
        i      = f + I;
        si (conflatio_inclusio_localis(linea, &nomen_capitis))
        {
            /* include/ primum, deinde radices clientium */
            constans character* constans* radices =
                o->contextus->radices_clientium;
                               vacuum* v = NIHIL;
                                  s32  r;   /* -I = include/ */

            per (r = -I; v == NIHIL && (r < ZEPHYRUM
                 || (radices != NIHIL && radices[r] != NIHIL)); r++)
            {
                constans character* radix = r < ZEPHYRUM ? "include/"
                    : radices[r];
                chorda via = chorda_concatenare(chorda_ex_literis(radix,
                    o->piscina), nomen_capitis, o->piscina);

                si (!tabula_dispersa_invenire(o->per_viam, via, &v))
                {
                    v = NIHIL;
                }
            }
            si (v != NIHIL)
            {
                _caput_visitare(o, (constans ConflatioPlagula*)v);
            }
        }
    }
    {
        constans ConflatioPlagula** cella =
            (constans ConflatioPlagula**)xar_addere(o->ordo);

        si (cella != NIHIL)
        {
            *cella = plagula;
        }
    }
}

interior TabulaDispersa*
_per_viam (
    Piscina* piscina,
        Xar* clausura)
{
    TabulaDispersa* t = tabula_dispersa_creare_chorda(piscina, 256);
               i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(clausura); i++)
    {
        ConflatioPlagula* p = (ConflatioPlagula*)xar_obtinere(clausura,
            i);

        tabula_dispersa_inserere(t, p->via, (vacuum*)p);
    }
    redde t;
}

Xar*
conflatio_capita_ordinare (
                        Piscina* piscina,
    constans ConflatioContextus* contextus,
                            Xar* clausura)
{
    Ordinatio  o;
       vacuum* v = NIHIL;
          i32  i;

    o.piscina    = piscina;
    o.contextus  = contextus;
    o.per_viam   = _per_viam(piscina, clausura);
    o.visa       = tabula_dispersa_creare_chorda(piscina, 128);
    o.ordo       = xar_creare(piscina,
        (i32)magnitudo(constans ConflatioPlagula*));
    si (tabula_dispersa_invenire(o.per_viam,
        chorda_ex_literis("include/postulata_posix.h", piscina), &v))
    {
        _caput_visitare(&o, (constans ConflatioPlagula*)v);
    }
    per (i = ZEPHYRUM; i < xar_numerus(clausura); i++)
    {
        constans ConflatioPlagula* r =
            (constans ConflatioPlagula*)xar_obtinere(clausura, i);

        si (conflatio_caput_est(contextus, r->via))
        {
            _caput_visitare(&o, r);
        }
    }
    redde o.ordo;
}

Xar*
conflatio_fontes_ordinare (
                        Piscina* piscina,
    constans ConflatioContextus* contextus,
                            Xar* clausura,
                            Xar* capita)
{
               Xar* ordo      = xar_creare(piscina,
                   (i32)magnitudo(constans ConflatioPlagula*));
    TabulaDispersa* visa      = tabula_dispersa_creare_chorda(piscina,
        128);
    TabulaDispersa* per_viam  = _per_viam(piscina, clausura);
               i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(capita); i++)
    {
        constans ConflatioPlagula* c =
            *(constans ConflatioPlagula**)xar_obtinere(capita, i);
               constans character* radix = _radix_clientis(contextus,
                   c->via);
                            chorda via = chorda_concatenare(
                                chorda_concatenare(chorda_ex_literis(
                                radix != NIHIL ? radix : "lib/",
                                piscina), conflatio_stirps(c->via),
                                piscina), chorda_ex_literis(".c",
                                piscina), piscina);
                           vacuum* v = NIHIL;

        si (   tabula_dispersa_invenire(per_viam, via, &v)
            && !tabula_dispersa_continet(visa, via))
        {
            constans ConflatioPlagula** cella =
                (constans ConflatioPlagula**)xar_addere(ordo);

            *cella = (constans ConflatioPlagula*)v;
            tabula_dispersa_inserere(visa, via, v);
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(clausura); i++)
    {
        constans ConflatioPlagula* r =
            (constans ConflatioPlagula*)xar_obtinere(clausura, i);

        si (   conflatio_fons_est(contextus, r->via)
            && !tabula_dispersa_continet(visa, r->via))
        {
            constans ConflatioPlagula** cella =
                (constans ConflatioPlagula**)xar_addere(ordo);

            *cella = r;
            tabula_dispersa_inserere(visa, r->via, (vacuum*)cella);
        }
    }
    redde ordo;
}


/* ==================================================
 * Emissio
 * ================================================== */

vacuum
conflatio_plagulam_emittere (
            ChordaAedificator* aedificator,
    constans ConflatioPlagula* plagula)
{
    chorda textus  = plagula->contentum;
       i32 i       = ZEPHYRUM;

    chorda_aedificator_appendere_literis(aedificator, "#line 1 \"");
    chorda_aedificator_appendere_chorda(aedificator, plagula->via);
    chorda_aedificator_appendere_literis(aedificator, "\"\n");
    dum (i < textus.mensura)
    {
           i32 f = i;
        chorda linea;
        chorda ignotum;

        dum (f < textus.mensura && textus.datum[f] != '\n')
        {
            f = f + I;
        }
        linea = chorda_sectio(textus, i, f);
        si (!conflatio_inclusio_localis(linea, &ignotum))
        {
            chorda_aedificator_appendere_chorda(aedificator, linea);
        }
        chorda_aedificator_appendere_literis(aedificator, "\n");
        i = f + I;
    }
}

vacuum
conflatio_renominatam_emittere (
            ChordaAedificator* aedificator,
    constans ConflatioPlagula* plagula,
                          Xar* statica,
                       chorda  suffixum,
           constans character* nota)
{
    i32 k;

    si (statica != NIHIL && xar_numerus(statica) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aedificator, "/* ");
        chorda_aedificator_appendere_chorda(aedificator, plagula->via);
        chorda_aedificator_appendere_literis(aedificator, ": ");
        chorda_aedificator_appendere_literis(aedificator, nota);
        chorda_aedificator_appendere_literis(aedificator, " */\n");
        per (k = ZEPHYRUM; k < xar_numerus(statica); k++)
        {
            constans ConflatioStaticum* s =
                (constans ConflatioStaticum*)xar_obtinere(statica, k);

            si (chorda_aequalis_literis(s->genus, "macro"))
            {
                perge;   /* macra: #undef post solum */
            }
            chorda_aedificator_appendere_literis(aedificator,
                "#define ");
            chorda_aedificator_appendere_chorda(aedificator,
                s->symbolum);
            chorda_aedificator_appendere_literis(aedificator, " ");
            chorda_aedificator_appendere_chorda(aedificator,
                s->symbolum);
            chorda_aedificator_appendere_literis(aedificator, "_");
            chorda_aedificator_appendere_chorda(aedificator, suffixum);
            chorda_aedificator_appendere_literis(aedificator, "\n");
        }
    }
    conflatio_plagulam_emittere(aedificator, plagula);
    per (k = ZEPHYRUM; statica != NIHIL
        && k < xar_numerus(statica); k++)
    {
        constans ConflatioStaticum* s =
            (constans ConflatioStaticum*)xar_obtinere(statica, k);

        chorda_aedificator_appendere_literis(aedificator, "#undef ");
        chorda_aedificator_appendere_chorda(aedificator, s->symbolum);
        chorda_aedificator_appendere_literis(aedificator, "\n");
    }
}

vacuum
conflatio_fontem_emittere (
              ChordaAedificator* aedificator,
    constans ConflatioContextus* contextus,
      constans ConflatioPlagula* plagula)
{
    vacuum* v        = NIHIL;
       Xar* ordines  = NIHIL;

    si (   contextus->statica != NIHIL
        && tabula_dispersa_invenire(contextus->statica, plagula->via,
               &v))
    {
        ordines = (Xar*)v;
    }
    conflatio_renominatam_emittere(aedificator, plagula, ordines,
        conflatio_stirps(plagula->via),
        "statica per plagulam renominata");
}
