/* md_extenta.c - Vide md_extenta.h. */

#include "md_extenta.h"
#include "md_arbor.h"
#include "md_stml.h"
#include "md_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "stml.h"
#include "selectio.h"
#include <string.h>

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior MdExtentaResultus
_fractum (
               Piscina* piscina,
    constans character* causa,
                chorda  detail)
{
    MdExtentaResultus  r;
    ChordaAedificator* aed;

    r.successus     = FALSUM;
    r.extenta       = NIHIL;
    r.congruentiae  = ZEPHYRUM;
    r.lexemata      = ZEPHYRUM;
    aed             = chorda_aedificator_creare(piscina, CCLVI);
    chorda_aedificator_appendere_literis(aed, causa);
    si (detail.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, " ");
        chorda_aedificator_appendere_chorda(aed, detail);
    }
    r.causa = chorda_aedificator_finire(aed);
    redde r;
}

/* lexemata arboris materiae ordine praevio (loci ordine registri,
 * listae ordine suo - idem quo scriptor emittit) */
interior vacuum
_lexemata_materiae (
                      Xar* acervus,
    constans MateriaNodus* nodus)
{
    i32 i;

    si (nodus == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _lexemata_materiae(acervus, v->datum.nodus);
        }
        alioquin si (v->genus == MATERIA_VALOR_TOKEN)
        {
            constans MateriaToken** locus =
                (constans MateriaToken**)xar_addere(acervus);

            *locus = v->datum.token;
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 n = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < n; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e == NIHIL)
                {
                    perge;
                }
                si (e->genus == MATERIA_VALOR_NODUS)
                {
                    _lexemata_materiae(acervus, e->datum.nodus);
                }
                alioquin si (e->genus == MATERIA_VALOR_TOKEN)
                {
                    constans MateriaToken** locus =
                        (constans MateriaToken**)xar_addere(acervus);

                    *locus = e->datum.token;
                }
            }
        }
    }
}

interior b32
_elementum_lexematis (
    constans StmlNodus* n,
    constans character* praefixum,
                   i32  lp)
{
    redde (b32)(   n->genus == STML_NODUS_ELEMENTUM
                && n->titulus != NIHIL
                && n->titulus->mensura >= lp
                && memcmp(n->titulus->datum, praefixum, (size_t)lp)
                    == ZEPHYRUM);
}

/* elementa lexematum documenti STML ordine praevio (tag praefixo
 * lexici; in ea non descenditur) */
interior vacuum
_lexemata_stml (
                   Xar* acervus,
             StmlNodus* n,
    constans character* praefixum,
                   i32  lp)
{
    i32 i;
    i32 num;

    si (n == NIHIL)
    {
        redde;
    }
    si (_elementum_lexematis(n, praefixum, lp))
    {
        StmlNodus** locus = (StmlNodus**)xar_addere(acervus);

        *locus = n;
        redde;
    }
    num = n->liberi != NIHIL ? xar_numerus(n->liberi) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        _lexemata_stml(acervus, *(StmlNodus**)xar_obtinere(n->liberi,
            i),
            praefixum, lp);
    }
}

/* lexema primum et ultimum (elementa) subarboris elementi congruentis */
interior vacuum
_terminos_quaerere (
             StmlNodus*  n,
    constans character*  praefixum,
                   i32   lp,
             StmlNodus** primum,
             StmlNodus** ultimum)
{
    i32 i;
    i32 num;

    si (n == NIHIL)
    {
        redde;
    }
    si (_elementum_lexematis(n, praefixum, lp))
    {
        si (*primum == NIHIL)
        {
            *primum = n;
        }
        *ultimum = n;
        redde;
    }
    num = n->liberi != NIHIL ? xar_numerus(n->liberi) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        _terminos_quaerere(*(StmlNodus**)xar_obtinere(n->liberi, i),
            praefixum, lp, primum, ultimum);
    }
}

/* index elementi lexematis in serie STML - a cursore, deinde ab initio */
interior s32
_index_lexematis (
     constans Xar* series,
        StmlNodus* n,
              i32  cursor)
{
    i32 num = xar_numerus(series);
    i32 k;

    per (k = cursor; k < num; k++)
    {
        si (*(StmlNodus**)xar_obtinere(series, k) == n)
        {
            redde (s32)k;
        }
    }
    per (k = ZEPHYRUM; k < cursor && k < num; k++)
    {
        si (*(StmlNodus**)xar_obtinere(series, k) == n)
        {
            redde (s32)k;
        }
    }
    redde (s32)-I;
}

MdExtentaResultus
md_extenta_quaerere (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* fons,
                    i32  mensura,
     constans character* selector)
{
       MdExtentaResultus r;
     MateriaLexiconRatum ratum;
      MateriaLexIudicium iudicium;
   MateriaArborConsilium consilium;
            MateriaNodus* radix;
   MateriaArborScriptura s;
            StmlResultus lectum;
         SelectioResultus sel;
                    Xar* congruentia;
                    Xar* lexemata_m;
                    Xar* lexemata_s;
     constans character* praefixum;
                     i32 lp;
                     i32 numerus;
                     i32 k;
                     i32 cursor = ZEPHYRUM;

    r.successus     = FALSUM;
    r.causa         = _vacua();
    r.extenta       = NIHIL;
    r.congruentiae  = ZEPHYRUM;
    r.lexemata      = ZEPHYRUM;

    si (!materia_lexicon_ratum_facere(&ratum, &MD_LEXICON, &iudicium))
    {
        redde _fractum(piscina, "lexicon md recusatum", _vacua());
    }
    md_stml_consilium(&consilium, &ratum);
    radix = md_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        redde _fractum(piscina, "parsura md fracta", _vacua());
    }
    s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
    si (!s.successus)
    {
        redde _fractum(piscina, "proiectio STML fracta:",
            chorda_ex_literis(s.causa != NIHIL ? s.causa : "-",
            piscina));
    }
    lectum = stml_legere(s.textus, piscina, intern);
    si (!lectum.successus)
    {
        redde _fractum(piscina, "lectio proiectionis fracta:",
            lectum.error);
    }
    sel = selectio_legere_ex_literis(selector, piscina, intern);
    si (!sel.successus)
    {
        redde _fractum(piscina, "selector recusatus:", sel.error);
    }

    /* correspondentia lexematum: materia (praevio) ~ STML (praevio) */
    praefixum   = MD_LEXICON.praefixum_tagi;
    lp          = (i32)strlen(praefixum);
    lexemata_m  = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    lexemata_s  = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    _lexemata_materiae(lexemata_m, radix);
    _lexemata_stml(lexemata_s, lectum.radix, praefixum, lp);
    numerus = xar_numerus(lexemata_m);
    si (numerus != xar_numerus(lexemata_s))
    {
        redde _fractum(piscina, "correspondentia lexematum fracta:"
            " numeri divergunt", _vacua());
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans MateriaToken* t = *(constans MateriaToken**)
            xar_obtinere(lexemata_m, k);
        constans StmlNodus* e =
            *(StmlNodus**)xar_obtinere(lexemata_s, k);
                 character tag[128];
                       i32 lt =
                           materia_arbor_lexema_tag(&ratum,
                           t->genus, tag, (i32)magnitudo(tag));

        si (   lt                                         == ZEPHYRUM
            || e->titulus->mensura                        != lt
            || memcmp(e->titulus->datum, tag, (size_t)lt) != ZEPHYRUM)
        {
            ChordaAedificator* aed = chorda_aedificator_creare(piscina,
                CCLVI);

            chorda_aedificator_appendere_literis(aed, "exspectatum ");
            chorda_aedificator_appendere_literis(aed,
                lt > ZEPHYRUM ? tag : "?");
            chorda_aedificator_appendere_literis(aed, ", inventum ");
            chorda_aedificator_appendere_chorda(aed, *e->titulus);
            redde _fractum(piscina, "correspondentia lexematum fracta:"
                " tag divergens -", chorda_aedificator_finire(aed));
        }
    }
    r.lexemata = numerus;

    congruentia = selectio_invenire_omnes(lectum.radix, sel.selectio,
        piscina);
    r.extenta = xar_creare(piscina, (i32)magnitudo(MdExtentum));
    numerus = congruentia
        != NIHIL ? xar_numerus(congruentia) : ZEPHYRUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
         StmlNodus* n = *(StmlNodus**)xar_obtinere(congruentia, k);
         StmlNodus* primum = NIHIL;
         StmlNodus* ultimum = NIHIL;
               s32  a;
               s32  b;
               i32  j;
               s32  initium      = (s32)-I;
               s32  finis        = (s32)-I;
               i32  linea        = ZEPHYRUM;
               i32  columna      = ZEPHYRUM;
               i32  linea_finis  = ZEPHYRUM;

        r.congruentiae++;
        _terminos_quaerere(n, praefixum, lp, &primum, &ultimum);
        si (primum == NIHIL)
        {
            perge;
        }
        a = _index_lexematis(lexemata_s, primum, cursor);
        b = _index_lexematis(lexemata_s, ultimum, a
            >= ZEPHYRUM ? (i32)a : cursor);
        si (a < ZEPHYRUM || b < a)
        {
            redde _fractum(piscina, "lexema congruentiae extra seriem",
                _vacua());
        }
        cursor = (i32)a;
        per (j = (i32)a; j <= (i32)b; j++)
        {
            constans MateriaToken* t = *(constans MateriaToken**)
                xar_obtinere(lexemata_m, j);

            si (   t->fons_index == MD_FONS_DERIVATUS
                || t->byte_offset < ZEPHYRUM)
            {
                perge;
            }
            si (initium < ZEPHYRUM)
            {
                initium  = t->byte_offset;
                linea    = t->linea;
                columna  = t->columna;
            }
            finis        = t->byte_offset + (s32)t->valor.mensura;
            linea_finis  = t->linea;
        }
        si (initium >= ZEPHYRUM)
        {
            MdExtentum* x = (MdExtentum*)xar_addere(r.extenta);
             character* tag = (character*)piscina_allocare(piscina,
                 (memoriae_index)n->titulus->mensura + I);

            memcpy(tag, n->titulus->datum, (size_t)n->titulus->mensura);
            tag[n->titulus->mensura]  = '\0';
            x->tag                    = tag;
            x->initium                = initium;
            x->finis                  = finis;
            x->linea                  = linea;
            x->columna                = columna;
            x->linea_finis            = linea_finis;
        }
    }
    r.successus = VERUM;
    redde r;
}
