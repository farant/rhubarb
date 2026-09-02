/* apparatus.c - impletio clausurae et portae apparatus */

#include "postulata_posix.h"
#include "apparatus.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_expandere.h"
#include "silva_c89_oraculum.h"
#include <stdio.h>
#include <string.h>

vacuum
apparatus_causam_notare (
      CausaNumerata* series,
                i32* numerus,
 constans character* causa)
{
    i32 i;

    si (causa == NIHIL)
    {
        causa = "(sine causa)";
    }
    per (i = ZEPHYRUM; i < *numerus; i++)
    {
        si (strcmp(series[i].causa, causa) == ZEPHYRUM)
        {
            series[i].numerus++;
            redde;
        }
    }
    si (*numerus >= CAUSAE_MAXIMAE)
    {
        redde;
    }
    series[*numerus].causa    = causa;
    series[*numerus].numerus  = I;
    (*numerus)++;
}

b32
apparatus_desinit_in (
    constans character* via,
    constans character* cauda)
{
    memoriae_index m;
    memoriae_index n;

    m = strlen(via);
    n = strlen(cauda);
    si (n > m)
    {
        redde FALSUM;
    }
    redde (b32)(strcmp(via + (m - n), cauda) == ZEPHYRUM);
}

b32
apparatus_est_c (
    constans character* titulus)
{
    memoriae_index m;

    m = strlen(titulus);
    si (m < III)
    {
        redde FALSUM;
    }
    redde (b32)(titulus[m - II] == '.' && titulus[m - I] == 'c');
}

b32
apparatus_est_c_vel_h (
    constans character* titulus)
{
    memoriae_index m;

    m = strlen(titulus);
    si (m < III)
    {
        redde FALSUM;
    }
    si (   titulus[m - II] == '.'
        && (titulus[m - I] == 'c' || titulus[m - I] == 'h'))
    {
        redde VERUM;
    }
    redde FALSUM;
}

i8*
apparatus_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_exitus)
{
              FILE* pl;
                i8* buffer;
    signatus longus  mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);
    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (   mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
               != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_exitus = (i32)mensura;
    redde buffer;
}

/* Linea 'C<tab>via' aedilis in clausuram; ceterae praetereuntur.
 * Truncatio NUMQUAM tacita: numerus tectus datum simularet. */
interior vacuum
_lineam_clausurae_addere (
     Clausura* clausura,
    character* linea)
{
    memoriae_index m;

    si (linea[ZEPHYRUM] != 'C' || linea[I] != '\t')
    {
        redde;
    }
    m = strlen(linea);
    dum (   m > ZEPHYRUM
         && (linea[m - I] == '\n' || linea[m - I] == '\r'))
    {
        linea[m - I] = '\0';
        m--;
    }
    si (m <= II)
    {
        redde;
    }
    si (clausura->numerus >= CLAUSURA_MAXIMA)
    {
        clausura->truncata = VERUM;
        redde;
    }
    si (m - II >= VIA_MAXIMA)
    {
        clausura->truncata = VERUM;
        redde;
    }
    strcpy(clausura->series[clausura->numerus], linea + II);
    si (apparatus_desinit_in(linea + II, "latina.h"))
    {
        clausura->latina_inest = VERUM;
    }
    clausura->numerus++;
}

interior vacuum
_clausuram_vacuare (
    Clausura* clausura)
{
    clausura->numerus       = ZEPHYRUM;
    clausura->latina_inest  = FALSUM;
    clausura->truncata      = FALSUM;
}

vacuum
apparatus_clausuram_petere (
    constans character* radix,
    constans character* via_relativa,
              Clausura* clausura)
{
    character mandatum[1024];
    character linea[VIA_MAXIMA + 64];
        FILE* tubus;

    _clausuram_vacuare(clausura);
    sprintf(mandatum,
        "cd '%s' && ./bin/aedilis '%s' --partes 2>/dev/null",
        radix, via_relativa);
    tubus = popen(mandatum, "r");
    si (tubus == NIHIL)
    {
        redde;
    }
    dum (fgets(linea, (integer)magnitudo(linea), tubus) != NIHIL)
    {
        _lineam_clausurae_addere(clausura, linea);
    }
    pclose(tubus);
}

ClausuraeCorporis*
apparatus_clausuras_petere (
    constans character* radix,
    constans character* directorium,
               Piscina* piscina)
{
    character mandatum[1024];
    character linea[VIA_MAXIMA + 64];
    FILE*              tubus;
    ClausuraeCorporis* corpus;
    Clausura*          currens;

    corpus = (ClausuraeCorporis*)piscina_allocare(piscina,
        magnitudo(ClausuraeCorporis));
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    corpus->viae       = xar_creare(piscina, VIA_MAXIMA);
    corpus->clausurae  = xar_creare(piscina, (i32)magnitudo(Clausura));
    si (corpus->viae == NIHIL || corpus->clausurae == NIHIL)
    {
        redde NIHIL;
    }
    sprintf(mandatum,
        "cd '%s' && ./bin/aedilis --corpus '%s' --partes 2>/dev/null",
        radix, directorium);
    tubus = popen(mandatum, "r");
    si (tubus == NIHIL)
    {
        redde corpus;   /* vacuum - consumptor clamat */
    }
    currens = NIHIL;
    dum (fgets(linea, (integer)magnitudo(linea), tubus) != NIHIL)
    {
        si (linea[ZEPHYRUM] == 'F' && linea[I] == '\t')
        {
                 character* via;
            memoriae_index  m;

            via      = (character*)xar_addere(corpus->viae);
            currens  = (Clausura*)xar_addere(corpus->clausurae);
            si (via == NIHIL || currens == NIHIL)
            {
                currens = NIHIL;
                frange;
            }
            m = strlen(linea + II);
            dum (   m > ZEPHYRUM
                 && (   linea[II + m - I] == '\n'
                     || linea[II + m - I] == '\r'))
            {
                linea[II + m - I] = '\0';
                m--;
            }
            si (m >= VIA_MAXIMA)
            {
                m = VIA_MAXIMA - I;
            }
            memcpy(via, linea + II, m);
            via[m] = '\0';
            _clausuram_vacuare(currens);
            perge;
        }
        si (currens != NIHIL)
        {
            _lineam_clausurae_addere(currens, linea);
        }
    }
    pclose(tubus);
    redde corpus;
}

b32
apparatus_clausuram_ex_corpore (
    constans ClausuraeCorporis* corpus,
            constans character* via_relativa,
                      Clausura* clausura)
{
    i32 i;
    i32 n;

    _clausuram_vacuare(clausura);
    si (corpus == NIHIL)
    {
        redde FALSUM;
    }
    n = xar_numerus(corpus->viae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans character* via;

        via = (constans character*)xar_obtinere(corpus->viae, i);
        si (strcmp(via, via_relativa) == 0)
        {
            *clausura = *(constans Clausura*)xar_obtinere(
                corpus->clausurae, i);
            redde VERUM;
        }
    }
    redde FALSUM;
}

SilvaParsura*
apparatus_parsare (
                Piscina* opus,
     constans character* via,
            constans i8* fons,
                    i32  mensura,
     constans character* radix,
      constans Clausura* clausura,
                    b32  praebere,
                    s32* fons_latina_exitus)
{
    si (fons_latina_exitus != NIHIL)
    {
        *fons_latina_exitus = -I;
    }

    si (clausura != NIHIL && praebere)
    {
         SilvaExpansio* expansio;
                   i32  i;

        expansio = silva_expansio_creare(opus);
        per (i = ZEPHYRUM; i < clausura->numerus; i++)
        {
            character  via_capitis[1024];
                   i8* textus;
                  i32  m_caput;
                  s32  index_fontis;

            sprintf(via_capitis, "%s/%s", radix, clausura->series[i]);
            textus = apparatus_plagulam_legere(opus, via_capitis,
                                               &m_caput);
            si (textus == NIHIL)
            {
                perge;
            }
            /* Praebitio DUAS claves ponit (viam canonicam +
             * basename), ergo '#include "chorda.h"' resolvitur
             * quamquam viam 'include/chorda.h' damus. */
            index_fontis = silva_includendum_praebere(expansio,
                clausura->series[i], (constans character*)textus,
                m_caput);
            si (   fons_latina_exitus != NIHIL
                && apparatus_desinit_in(clausura->series[i],
                                        "latina.h"))
            {
                *fons_latina_exitus = index_fontis;
            }
        }
        redde silva_parsare_cum_expansione(opus, expansio, via,
            (constans character*)fons, mensura,
            &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    }
    redde silva_parsare(opus, via, (constans character*)fons,
        mensura, &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
}

/* Expansiones quarum DEFINITIO in fonte dato iacet.
 *
 * fons < 0 (i.e. latina.h numquam resoluta) reddit ZEPHYRUM, quod
 * est responsum rectum: si fons numquam intravit, nihil ex eo venit. */
interior i32
_lexemata_ex_fonte_numerare (
    constans SilvaParsura* parsura,
                      s32  fons)
{
    i32 numerus;
    i32 i;
    i32 quantum;

    si (   parsura == NIHIL || parsura->lexemata == NIHIL
        || fons < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    numerus = ZEPHYRUM;
    quantum = xar_numerus(parsura->lexemata);
    per (i = ZEPHYRUM; i < quantum; i++)
    {
         SilvaToken** sedes;
         SilvaToken*  corpus_macro;

        sedes = (SilvaToken**)xar_obtinere(parsura->lexemata, i);
        si (   sedes                 == NIHIL
            || *sedes                == NIHIL
            || (*sedes)->origo.genus != SILVA_ORIGO_EXPANSIO)
        {
            perge;
        }
        corpus_macro = (*sedes)->origo.datum.expansio.corpus;
        si (corpus_macro != NIHIL && corpus_macro->fons_index == fons)
        {
            numerus++;
        }
    }
    redde numerus;
}

b32
apparatus_sanus (
    constans SilvaParsura* parsura,
        constans Clausura* clausura,
                      s32  fons_latina,
                      i32* expansa_exitus)
{
    i32 expansa;

    si (expansa_exitus != NIHIL)
    {
        *expansa_exitus = ZEPHYRUM;
    }
    /* Non applicabilis: plagula plana clausuram non poscit */
    si (clausura == NIHIL || !clausura->latina_inest)
    {
        redde VERUM;
    }
    expansa = _lexemata_ex_fonte_numerare(parsura, fons_latina);
    si (expansa_exitus != NIHIL)
    {
        *expansa_exitus = expansa;
    }
    redde (b32)(expansa > ZEPHYRUM);
}
