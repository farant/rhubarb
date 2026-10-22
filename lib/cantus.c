/* cantus.c - Cantilena ex textu (vide include/cantus.h)
 *
 * DUO TRANSITUS, lex UNA: _currere primum numerat et iudicat (notae
 * NIHIL), deinde tabulam exactam e piscina implet. Transitus secundus
 * deficere non potest - idem textus, idem status.
 *
 * TEMPUS: positio in pulsibus (DCCLXVIII per notam integram) ab
 * initio segmenti temporis; quadrum = initium segmenti + rotundatio
 * (pulsus x quadra per pulsum). Mutatio 't' segmentum claudit ad
 * quadrum suum rotundatum et novum incipit. Vide lib/cantus.worklog.md.
 */
#include "cantus.h"
#include "sonus.h"

#define CANTUS_PULSUS_INTEGRAE DCCLXVIII
/* nota integra = IV quartae = CCXL / t secundae */
#define CANTUS_SECUNDAE_INTEGRAE CCXL
/* numeri longiores non leguntur (vitium, non circumvolutio) */
#define CANTUS_NUMERUS_MAXIMUS (C * M)

nomen structura {
          chorda  textus;
             i32  positio;          /* octetus proximus legendus */
             s32  octava;
             i32  ordinaria;        /* longitudo ordinaria, pulsus */
             i32  tempus;           /* quartae per minutum */
             f32  lucrum;
          chorda  praeparatum;
             s64  pulsus;           /* ab initio segmenti */
             s64  quadrum_segmenti; /* initium segmenti, quadra */
      CantusNota* notae;            /* NIHIL = transitus numerandi */
             i32  numerus;
    CantusVitium* vitium;
} CantusLector;

/* semitonia litterarum c d e f g a b */
hic_manens constans s32 SEMITONIA[VII] = { IX, XI, ZEPHYRUM, II, IV, V,
    VII };

interior b32
_vitium (
          CantusLector* l,
                   i32  offset,
    constans character* causa)
{
    l->vitium->offset  = offset;
    l->vitium->causa   = causa;
    redde FALSUM;
}

/* octetus ad positionem, minusculus; 0 post finem */
interior character
_signum (
    constans CantusLector* l,
                      i32  positio)
{
    character s;

    si (positio >= l->textus.mensura)
    {
        redde '\0';
    }
    s = (character)l->textus.datum[positio];
    si (s >= 'A' && s <= 'Z')
    {
        s = (character)(s - 'A' + 'a');
    }
    redde s;
}

interior b32
_cifra (
    character s)
{
    redde s >= '0' && s <= '9';
}

/* numerus decimalis ad positionem; FALSUM si nullus aut nimius */
interior b32
_numerum_legere (
    CantusLector* l,
             s32* valor)
{
    i32 initium  = l->positio;
    s32 v        = ZEPHYRUM;

    si (!_cifra(_signum(l, l->positio)))
    {
        redde _vitium(l, initium, "numerus exspectatus");
    }
    dum (_cifra(_signum(l, l->positio)))
    {
        si (v < CANTUS_NUMERUS_MAXIMUS)
        {
            v = v * X + (s32)(_signum(l, l->positio) - '0');
        }
        l->positio++;
    }
    si (v >= CANTUS_NUMERUS_MAXIMUS)
    {
        redde _vitium(l, initium, "numerus nimius");
    }
    *valor = v;
    redde VERUM;
}

/* numerus intra [minimum, maximum] */
interior b32
_numerum_coercitum_legere (
          CantusLector* l,
                   s32  minimum,
                   s32  maximum,
    constans character* causa,
                   s32* valor)
{
    i32 initium = l->positio;

    si (!_numerum_legere(l, valor))
    {
        redde FALSUM;
    }
    si (*valor < minimum || *valor > maximum)
    {
        redde _vitium(l, initium, causa);
    }
    redde VERUM;
}

/* puncta post longitudinem: x1.5, x1.75 - pulsus divisibiles manent */
interior b32
_puncta_legere (
    CantusLector* l,
             i32* pulsus)
{
    i32 basis   = *pulsus;
    i32 puncta  = ZEPHYRUM;

    dum (_signum(l, l->positio) == '.')
    {
        si (puncta == II)
        {
            redde _vitium(l, l->positio, "puncta plura quam II");
        }
        puncta++;
        si ((basis % (puncta == I ? (i32)II : (i32)IV)) != ZEPHYRUM)
        {
            redde _vitium(l, l->positio,
                "punctum longitudinem non dividit");
        }
        *pulsus += basis >> puncta;
        l->positio++;
    }
    redde VERUM;
}

/* mensura := [longitudo] {'.'}; sine longitudine ordinaria valet,
 * nisi 'necessaria' */
interior b32
_mensuram_legere (
    CantusLector* l,
             b32  necessaria,
             i32* pulsus)
{
    si (_cifra(_signum(l, l->positio)))
    {
        i32 initium = l->positio;
        s32 longitudo;

        si (!_numerum_legere(l, &longitudo))
        {
            redde FALSUM;
        }
        si (   longitudo < I || longitudo > LXIV
            || (longitudo & (longitudo - I)) != ZEPHYRUM)
        {
            redde _vitium(l, initium, "longitudo non est potentia II "
                                      "(1..64)");
        }
        *pulsus = (i32)CANTUS_PULSUS_INTEGRAE / (i32)longitudo;
    }
    alioquin si (necessaria)
    {
        redde _vitium(l, l->positio, "longitudo exspectata");
    }
    alioquin
    {
        *pulsus = l->ordinaria;
    }
    redde _puncta_legere(l, pulsus);
}

/* quadrum positionis (pulsus ab initio segmenti), rotundatum */
interior s64
_quadrum (
    constans CantusLector* l,
                     s64  pulsus)
{
    s64 numerator   = pulsus * (s64)SONUS_FREQUENTIA
                    * (s64)CANTUS_SECUNDAE_INTEGRAE;
    s64 denominator = (s64)CANTUS_PULSUS_INTEGRAE * (s64)l->tempus;

    /* (2n + d) / 2d = rotundatio n/d ad proximum (n >= 0) */
    redde l->quadrum_segmenti
         + ((s64)II * numerator + denominator)
             / ((s64)II * denominator);
}

interior b32
_notam_legere (
     CantusLector* l,
        character  littera)
{
    i32 offset = l->positio;
    s32 nota;
    i32 pulsus;

    l->positio++;
    nota = (s32)XII * (l->octava + I) + SEMITONIA[littera - 'a'];
    si (_signum(l, l->positio) == '#' || _signum(l, l->positio) == '+')
    {
        nota++;
        l->positio++;
    }
    alioquin si (_signum(l, l->positio) == '-')
    {
        nota--;
        l->positio++;
    }
    si (!_mensuram_legere(l, FALSUM, &pulsus))
    {
        redde FALSUM;
    }
    si (nota < ZEPHYRUM || nota > CXXVII)
    {
        redde _vitium(l, offset, "nota extra 0..127 (MIDI)");
    }
    si (l->notae != NIHIL)
    {
        CantusNota* n = &l->notae[l->numerus];

        n->nota     = (i32)nota;
        n->initium  = _quadrum(l, l->pulsus);
        n->longitudo = _quadrum(l, l->pulsus + (s64)pulsus)
            - n->initium;
        n->lucrum       = l->lucrum;
        n->praeparatum  = l->praeparatum;
        n->offset       = offset;
    }
    l->numerus++;
    l->pulsus += (s64)pulsus;
    redde VERUM;
}

interior b32
_titulum_legere (
    CantusLector* l)
{
    i32 initium = l->positio;

    dum (l->positio < l->textus.mensura)
    {
        character s = (character)l->textus.datum[l->positio];

        si (!((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z')
              || _cifra(s) || s == '_'))
        {
            frange;
        }
        l->positio++;
    }
    si (l->positio == initium)
    {
        redde _vitium(l, initium, "titulus praeparati vacuus");
    }
    l->praeparatum = chorda_sectio(l->textus, initium, l->positio);
    redde VERUM;
}

/* textum totum semel percurrit */
interior b32
_currere (
    CantusLector* l)
{
    dum (l->positio < l->textus.mensura)
    {
        character s       = _signum(l, l->positio);
              i32 offset  = l->positio;
              s32 valor;
              i32 pulsus;

        si (s == ' ' || s == '\t' || s == '\n' || s == '\r' || s == '|')
        {
            l->positio++;
        }
        alioquin si (s >= 'a' && s <= 'g')
        {
            si (!_notam_legere(l, s))
            {
                redde FALSUM;
            }
        }
        alioquin si (s == 'r')
        {
            l->positio++;
            si (!_mensuram_legere(l, FALSUM, &pulsus))
            {
                redde FALSUM;
            }
            l->pulsus += (s64)pulsus;
        }
        alioquin si (s == 'o')
        {
            l->positio++;
            si (!_numerum_coercitum_legere(l, ZEPHYRUM, VIII,
                                           "octava extra 0..8", &valor))
            {
                redde FALSUM;
            }
            l->octava = valor;
        }
        alioquin si (s == '>')
        {
            l->positio++;
            l->octava++;
        }
        alioquin si (s == '<')
        {
            l->positio++;
            l->octava--;
        }
        alioquin si (s == 'l')
        {
            l->positio++;
            si (!_mensuram_legere(l, VERUM, &pulsus))
            {
                redde FALSUM;
            }
            l->ordinaria = pulsus;
        }
        alioquin si (s == 't')
        {
            l->positio++;
            si (!_numerum_coercitum_legere(l, XX, CD,
                "tempus extra 20..400",
                                           &valor))
            {
                redde FALSUM;
            }
            /* segmentum claudit ad quadrum suum, novum incipit */
            l->quadrum_segmenti  = _quadrum(l, l->pulsus);
            l->pulsus            = ZEPHYRUM;
            l->tempus            = (i32)valor;
        }
        alioquin si (s == 'v')
        {
            l->positio++;
            si (!_numerum_coercitum_legere(l, ZEPHYRUM, C,
                                           "lucrum extra 0..100",
                                           &valor))
            {
                redde FALSUM;
            }
            l->lucrum = (f32)valor / (f32)C;
        }
        alioquin si (s == '@')
        {
            l->positio++;
            si (!_titulum_legere(l))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            redde _vitium(l, offset, "signum ignotum");
        }
    }
    redde VERUM;
}

interior vacuum
_lectorem_parare (
    CantusLector* l,
          chorda  textus,
      CantusNota* notae,
    CantusVitium* vitium)
{
    l->textus               = textus;
    l->positio              = ZEPHYRUM;
    l->octava               = IV;
    l->ordinaria            = (i32)CANTUS_PULSUS_INTEGRAE / (i32)IV;
    l->tempus               = CXX;
    l->lucrum               = 1.0f;
    l->praeparatum.mensura  = ZEPHYRUM;
    l->praeparatum.datum    = NIHIL;
    l->pulsus               = ZEPHYRUM;
    l->quadrum_segmenti     = ZEPHYRUM;
    l->notae                = notae;
    l->numerus              = ZEPHYRUM;
    l->vitium               = vitium;
}

b32
cantus_legere (
          chorda  textus,
         Piscina* piscina,
       Cantilena* cantilena,
    CantusVitium* vitium)
{
    CantusLector  l;
      CantusNota* notae = NIHIL;

    cantilena->notae      = NIHIL;
    cantilena->numerus    = ZEPHYRUM;
    cantilena->longitudo  = ZEPHYRUM;
    _lectorem_parare(&l, textus, NIHIL, vitium);
    si (!_currere(&l))
    {
        redde FALSUM;
    }
    si (l.numerus > ZEPHYRUM)
    {
        notae = (CantusNota*)piscina_allocare(piscina,
            (memoriae_index)l.numerus * magnitudo(CantusNota));
        si (notae == NIHIL)
        {
            redde _vitium(&l, ZEPHYRUM, "piscina exhausta");
        }
        _lectorem_parare(&l, textus, notae, vitium);
        (vacuum)_currere(&l);
    }
    cantilena->notae      = notae;
    cantilena->numerus    = l.numerus;
    cantilena->longitudo  = _quadrum(&l, l.pulsus);
    redde VERUM;
}


    /* ==================================================
     * IUSSA - cantilena -> iussa mixturae
     *
     * DUO GRADUS: colores omnes primum resolvuntur (in
     * iussa[i].parametra servati), sedes tum demum capiuntur -
     * praeparatum ignotum ergo nullam sedem consumit (rector
     * sedem reddere non potest).
     * ================================================== */

constans SonitusParametra*
cantus_praeparatum_sonitus (
    chorda  titulus,
    vacuum* contextus)
{
    character terminatus[LXIV];
          i32 i;

    (vacuum)contextus;
    si (titulus.mensura == ZEPHYRUM || titulus.mensura > LXIII)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < titulus.mensura; i++)
    {
        terminatus[i] = (character)titulus.datum[i];
    }
    terminatus[titulus.mensura] = '\0';
    redde sonitus_praeparatum(terminatus);
}

b32
cantus_iussa_parare (
          constans Cantilena* cantilena,
        constans CantusForma* forma,
               MixturaRector* rector,
               MixturaIussum* iussa,
                CantusVitium* vitium)
{
              CantusResolutor  resolutor;
          constans CantusNota* nota;
    constans SonitusParametra* color;
             SonitusParametra  parametra;
                          i32  i;

    resolutor = forma->resolutor != NIHIL
              ? forma->resolutor
              : cantus_praeparatum_sonitus;

    /* gradus I: colores - vitium hic nullam sedem capit */
    per (i = ZEPHYRUM; i < cantilena->numerus; i++)
    {
        nota  = &cantilena->notae[i];
        color = nota->praeparatum.mensura == ZEPHYRUM
              ? forma->color
              : resolutor(nota->praeparatum, forma->contextus);
        si (color == NIHIL)
        {
            vitium->offset = nota->offset;
            vitium->causa  = nota->praeparatum.mensura == ZEPHYRUM
                           ? "color nullus: forma sine colore"
                           : "praeparatum ignotum";
            redde FALSUM;
        }
        iussa[i].parametra = *color;
    }

    /* gradus II: iussa et sedes */
    per (i = ZEPHYRUM; i < cantilena->numerus; i++)
    {
        nota       = &cantilena->notae[i];
        parametra  = iussa[i].parametra;   /* iussum_parare omnia delet */
        mixtura_iussum_parare(&iussa[i], MIXTURA_IUSSUM_SONITUM,
                              forma->momentum + nota->initium);
        parametra.frequentia = sonus_notae_frequentia((f32)nota->nota);
        si (forma->tenere)
        {
            /* porta per longitudinem scriptam: impetus intra eam */
            parametra.mora = (f32)nota->longitudo
                / (f32)SONUS_FREQUENTIA
                           - parametra.impetus;
            si (parametra.mora < 0.0f)
            {
                parametra.mora = 0.0f;
            }
        }
        iussa[i].parametra  = parametra;
        iussa[i].lucrum     = nota->lucrum * forma->lucrum;
        iussa[i].grex       = forma->grex;
        iussa[i].semen      = forma->semen + (i64)i;
        iussa[i].ansa = mixtura_rector_capere(rector,
            forma->prioritas);
    }
    redde VERUM;
}
