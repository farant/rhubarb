/* chronica_nomina.c - declarationes personarum et locorum (chronica
 * C5a). Vide include/chronica_nomina.h. */

#include "chronica_nomina.h"
#include "chorda_aedificator.h"
#include "stml.h"
#include "stml_macros.h"

#include <string.h>

/* lineae in textum unum ('\n' iunctae); initia linearum */
nomen structura {
    chorda  textus;
       i32* initia;
       i32  numerus;
} Textus;

interior b32
textum_facere (
    constans Xar* lineae,
         Piscina* piscina,
          Textus* t)
{
    ChordaAedificator* a;
                  i32  i;

    t->numerus  = (i32)xar_numerus(lineae);
    t->initia   = (i32*)piscina_allocare(piscina,
        (memoriae_index)(t->numerus + I) * magnitudo(i32));
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    si (!t->initia || !a)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < t->numerus; i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, '\n');
        }
        t->initia[i] = (i32)chorda_aedificator_longitudo(a);
        chorda_aedificator_appendere_chorda(a,
            *(constans chorda*)xar_obtinere(lineae, i));
    }
    t->textus = chorda_aedificator_finire(a);
    redde VERUM;
}

/* offset -> linea, columna (quaesitio binaria) */
interior vacuum
positio (
    constans Textus* t,
                i32  offset,
                i32* linea,
                i32* columna)
{
    i32 a = ZEPHYRUM;
    i32 b = t->numerus;

    dum (b - a > I)
    {
        i32 m = (a + b) / II;

        si (t->initia[m] <= offset)
        {
            a = m;
        }
        alioquin
        {
            b = m;
        }
    }
    *linea    = a;
    *columna  = offset - (t->numerus > ZEPHYRUM ? t->initia[a]
                                                : ZEPHYRUM);
}

/* '<person' / '<place' (aut '<event' si eventa) ad i, sequente spatio,
 * '>', '/' aut '('; genus in *genus */
interior b32
tag_incipit (
         constans Textus* t,
                     i32  i,
                     b32  eventa,
    ChronicaGenusNominis* genus)
{
    hic_manens constans character* tituli[III] = {
        "person", "place", "event" };
    i32 k;

    si (i >= t->textus.mensura || t->textus.datum[i] != '<')
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < (eventa ? III : II); k++)
    {
              i32 n = (i32)strlen(tituli[k]);
              i32 j = i + I + n;
        character c;

        si (   j >= t->textus.mensura
            || memcmp(t->textus.datum + i + I, tituli[k], (size_t)n)
               != ZEPHYRUM)
        {
            perge;
        }
        c = (character)t->textus.datum[j];
        si (   c == ' ' || c == '\t' || c == '\n' || c == '>'
            || c == '/' || c == '(')
        {
            si (genus)
            {
                *genus = k
                    == ZEPHYRUM ? CHRONICA_PERSONA : CHRONICA_LOCUS;
            }
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* initium tag proximi (person, place, event) post i; finis si nullus */
interior i32
tag_proximum (
    constans Textus* t,
                i32  i)
{
    per (; i < t->textus.mensura; i++)
    {
        si (tag_incipit(t, i, VERUM, NIHIL))
        {
            redde i;
        }
    }
    redde t->textus.mensura;
}

/* '>' claudens ab i (quotae duplices respectae); -1 si nullum */
interior s32
claudens (
    constans Textus* t,
                i32  i)
{
    b32 in_quota = FALSUM;

    per (; i < t->textus.mensura; i++)
    {
        character c = (character)t->textus.datum[i];

        si (c == '"')
        {
            in_quota = !in_quota;
        }
        alioquin si (c == '>' && !in_quota)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

#define REGIO_APERTA   (-I)
#define REGIO_CAPTURA  (-II)

/* finis regionis (exclusivus) ab initio tag; REGIO_APERTA si non
 * clausa, REGIO_CAPTURA si tag summus capturans */
interior s32
regionem_finire (
    constans Textus* t,
               i32  initium)
{
    constans i8* d            = t->textus.datum;
            s32  n            = (s32)t->textus.mensura;
            s32  profunditas  = ZEPHYRUM;
            s32  j            = (s32)initium;

    dum (j < n)
    {
              s32 k;
              s32 e;
        character c;

        si (d[j] != '<')
        {
            j++;
            perge;
        }
        si (j + III < n && memcmp(d + j, "<!--", IV) == ZEPHYRUM)
        {
            per (k = j + IV; k + II < n; k++)
            {
                si (memcmp(d + k, "-->", III) == ZEPHYRUM)
                {
                    frange;
                }
            }
            si (k + II >= n)
            {
                redde REGIO_APERTA;
            }
            j = k + III;
            perge;
        }
        si (j + I < n && d[j + I] == '/')
        {
            k = claudens(t, (i32)(j + I));
            si (k < ZEPHYRUM)
            {
                redde REGIO_APERTA;
            }
            profunditas--;
            j = k + I;
            si (profunditas <= ZEPHYRUM)
            {
                redde j;
            }
            perge;
        }
        c = j + I < n ? (character)d[j + I] : '\0';
        si (!(   (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
              || c == '#' || c == '>' || c == '(' || c == '@'
              || c == '.'))
        {
            j++;
            perge;
        }
        k = claudens(t, (i32)(j + I));
        si (k < ZEPHYRUM)
        {
            redde REGIO_APERTA;
        }
        e = k - I;
        dum (e > j && (d[e] == ' ' || d[e] == '\t' || d[e] == '\n'))
        {
            e--;
        }
        si (e > j && d[e] == '/')
        {
            si (profunditas == ZEPHYRUM)
            {
                redde k + I;
            }
        }
        alioquin si (e > j && d[e] == '(')
        {
            si (profunditas == ZEPHYRUM)
            {
                redde REGIO_CAPTURA;
            }
        }
        alioquin
        {
            profunditas++;
        }
        j = k + I;
    }
    redde REGIO_APERTA;
}

interior vacuum
vitium_addere (
    constans Textus* t,
                Xar* vitia,
                i32  initium,
                i32  finis,
             chorda  causa)
{
    ChronicaVitium* v;

    si (!vitia)
    {
        redde;
    }
    v = (ChronicaVitium*)xar_addere(vitia);
    positio(t, initium, &v->linea, &v->columna);
    positio(t, finis, &v->linea_finis, &v->columna_finis);
    v->causa = causa;
}

/* spatia collapsa, extrema tonsa */
interior chorda
collapsum (
    chorda  c,
   Piscina* piscina)
{
    ChordaAedificator* a;
                  b32  spatium;
                  i32  i;

    a        = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    spatium  = FALSUM;
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character x = (character)c.datum[i];

        si (x == ' ' || x == '\t' || x == '\n' || x == '\r')
        {
            spatium = VERUM;
            perge;
        }
        si (spatium && chorda_aedificator_longitudo(a) > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        spatium = FALSUM;
        chorda_aedificator_appendere_character(a, x);
    }
    redde chorda_aedificator_finire(a);
}

/* elementum "person"/"place" -> declaratio; positio ex positu nodi
 * (offset in regione) */
interior b32
declarationem_facere (
           constans Textus* t,
                 StmlNodus* nodus,
                       i32  initium,
                       i32  finis,
                   Piscina* piscina,
        ChronicaDeclaratio* d)
{
    i32 i;

    d->genus = chorda_aequalis_literis(*nodus->titulus, "person")
             ? CHRONICA_PERSONA : CHRONICA_LOCUS;
    d->titulus = collapsum(stml_textus_fluxus(nodus, piscina), piscina);
    si (d->titulus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    d->attributa = xar_creare(piscina,
        (i32)magnitudo(ChronicaAttributum));
    per (i = ZEPHYRUM; nodus->attributa
        && i < xar_numerus(nodus->attributa); i++)
    {
        constans StmlAttributum* s = (constans StmlAttributum*)
            xar_obtinere(nodus->attributa, i);
        ChronicaAttributum* a = (ChronicaAttributum*)xar_addere(
            d->attributa);

        a->titulus  = chorda_transcribere(*s->titulus, piscina);
        a->valor    = s->valor ? chorda_transcribere(*s->valor, piscina)
                               : chorda_ex_literis("", piscina);
    }
    positio(t, initium, &d->regio_linea, &d->regio_columna);
    positio(t, finis, &d->regio_linea_finis, &d->regio_columna_finis);
    /* nodus distributus lineam rei suae tenet (1 = linea regionis
     * prima), non offset: columna = primum non-spatium lineae */
    d->linea    = d->regio_linea;
    d->columna  = d->regio_columna;
    si (   nodus->linea > I
        && d->regio_linea + nodus->linea - I <= d->regio_linea_finis)
    {
        i32 o;

        d->linea  = d->regio_linea + nodus->linea - I;
        o         = t->initia[d->linea];
        dum (   o < t->textus.mensura && (t->textus.datum[o] == ' '
                 || t->textus.datum[o] == '\t'))
        {
            o++;
        }
        d->columna = o - t->initia[d->linea];
    }
    redde VERUM;
}

/* regionem [initium, finis) per catenam STML; VERUM si declarationes
 * additae, aliter vitium */
interior vacuum
regionem_legere (
        constans Textus* t,
                    i32  initium,
                    i32  finis,
                Piscina* piscina,
    InternamentumChorda* intern,
                    Xar* declarationes,
                    Xar* vitia)
{
               StmlResultus  lectum;
    StmlDistributioResultus  distributum;
                        Xar* novae;
                        i32  i;

    lectum = stml_legere(chorda_sectio(t->textus, initium, finis),
        piscina, intern);
    si (!lectum.successus || !lectum.radix)
    {
        vitium_addere(t, vitia, initium, finis, chorda_concatenare(
            chorda_ex_literis("STML: ", piscina), lectum.error,
            piscina));
        redde;
    }
    distributum = stml_distribuere(lectum.radix, piscina, intern);
    si (!distributum.successus || !distributum.radix_distributa)
    {
        vitium_addere(t, vitia, initium, finis, chorda_ex_literis(
            distributum.vitium == STML_DISTRIBUTIO_MIXTA
            ? "distributio mixta" : "STML: distributio", piscina));
        redde;
    }
    novae = xar_creare(piscina, (i32)magnitudo(ChronicaDeclaratio));
    per (i = ZEPHYRUM; distributum.radix_distributa->liberi
        && i < xar_numerus(distributum.radix_distributa->liberi); i++)
    {
        StmlNodus* n = *(StmlNodus**)xar_obtinere(
            distributum.radix_distributa->liberi, i);

        si (   n->genus != STML_NODUS_ELEMENTUM || !n->titulus
            || (   !chorda_aequalis_literis(*n->titulus, "person")
                && !chorda_aequalis_literis(*n->titulus, "place")))
        {
            perge;
        }
        si (!declarationem_facere(t, n, initium, finis, piscina,
                (ChronicaDeclaratio*)xar_addere(novae)))
        {
            vitium_addere(t, vitia, initium, finis,
                chorda_ex_literis("nomen vacuum", piscina));
            redde;
        }
    }
    per (i = ZEPHYRUM; declarationes && i < xar_numerus(novae); i++)
    {
        *(ChronicaDeclaratio*)xar_addere(declarationes) =
            *(ChronicaDeclaratio*)xar_obtinere(novae, i);
    }
}

b32
chronica_nomina_legere (
          constans Xar* lineae,
               Piscina* piscina,
   InternamentumChorda* intern,
                   Xar* declarationes,
                   Xar* vitia)
{
    Textus t;
       i32 i;

    si (   !lineae || !piscina || !intern || !textum_facere(lineae,
            piscina, &t))
    {
        redde FALSUM;
    }
    i = ZEPHYRUM;
    dum (i < t.textus.mensura)
    {
        s32 finis;

        si (!tag_incipit(&t, i, FALSUM, NIHIL))
        {
            i++;
            perge;
        }
        finis = regionem_finire(&t, i);
        si (finis == REGIO_CAPTURA)
        {
            i32 f = tag_proximum(&t, i + I);

            vitium_addere(&t, vitia, i, f, chorda_ex_literis(
                "forma non admissa", piscina));
            i = f;
            perge;
        }
        si (finis == REGIO_APERTA)
        {
            i32 f = tag_proximum(&t, i + I);

            vitium_addere(&t, vitia, i, f, chorda_ex_literis(
                "non clausum", piscina));
            i = f;
            perge;
        }
        regionem_legere(&t, i, (i32)finis, piscina, intern,
            declarationes, vitia);
        i = (i32)finis;
    }
    redde VERUM;
}
