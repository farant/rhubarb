/* cantuale.c - Liber cantilenarum (vide include/cantuale.h)
 *
 * DUO TRANSITUS super liberos radicis: primus elementa iudicat et
 * cantilenas numerat (tabula exacta), secundus quamque cantionem
 * legit et iudicat. Vitium ubicumque = liber vacuus - nihil
 * semi-lectum.
 */
#include "cantuale.h"
#include "cantuale_infixum.h"
#include "internamentum.h"
#include "stml.h"

interior b32
_vitium (
        CantualeVitium* vitium,
                chorda  titulus,
                   i32  offset,
    constans character* causa)
{
    vitium->titulus  = titulus;
    vitium->offset   = offset;
    vitium->causa    = causa;
    redde FALSUM;
}

interior chorda
_vacua (
    vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

/* [a-z0-9_]+ */
interior b32
_signum_validum (
    chorda s)
{
    i32 i;

    si (s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        character x = (character)s.datum[i];

        si (!(   (x >= 'a' && x <= 'z') || (x >= '0' && x <= '9')
              || x == '_'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_elementum_est (
             StmlNodus* nodus,
    constans character* titulus)
{
    redde nodus->genus == STML_NODUS_ELEMENTUM
        && nodus->titulus != NIHIL
        && chorda_aequalis_literis(*nodus->titulus, titulus);
}

/* genera spatio separata; tabula exacta e piscina (NIHIL si nulla).
 * FALSUM: genus invalidum */
interior b32
_genera_legere (
            chorda  valor,
           Piscina* piscina,
            Cantio* cantio)
{
    i32 transitus;
    i32 i;
    i32 n;

    cantio->genera          = NIHIL;
    cantio->genera_numerus  = ZEPHYRUM;
    per (transitus = ZEPHYRUM; transitus < II; transitus++)
    {
        n = ZEPHYRUM;
        i = ZEPHYRUM;
        dum (i < valor.mensura)
        {
            chorda genus;

            si (valor.datum[i] == ' ')
            {
                i++;
                perge;
            }
            genus.datum    = valor.datum + i;
            genus.mensura  = ZEPHYRUM;
            dum (i < valor.mensura && valor.datum[i] != ' ')
            {
                genus.mensura++;
                i++;
            }
            si (!_signum_validum(genus))
            {
                redde FALSUM;
            }
            si (cantio->genera != NIHIL)
            {
                cantio->genera[n] = genus;
            }
            n++;
        }
        si (transitus == ZEPHYRUM)
        {
            si (n == ZEPHYRUM)
            {
                redde VERUM;
            }
            cantio->genera = (chorda*)piscina_allocare(piscina,
                (memoriae_index)n * magnitudo(chorda));
            si (cantio->genera == NIHIL)
            {
                redde FALSUM;
            }
        }
    }
    cantio->genera_numerus = n;
    redde VERUM;
}

/* cantilenam unam legit et iudicat; titulus iam validus */
interior b32
_cantionem_legere (
         StmlNodus* nodus,
            chorda  titulus,
           Piscina* piscina,
            Cantio* cantio,
    CantualeVitium* vitium)
{
        StmlNodus* textus  = NIHIL;
           chorda* genera;
     CantusVitium  cv;
              i32  i;

    cantio->titulus   = titulus;
    cantio->intactus  = stml_attributum_habet(nodus, "intactus");
    genera            = stml_attributum_capere(nodus, "genera");
    si (!_genera_legere(genera != NIHIL ? *genera : _vacua(), piscina,
                        cantio))
    {
        redde _vitium(vitium, titulus, ZEPHYRUM,
                      "genus non [a-z0-9_]+");
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* liberum = stml_liberum_ad_indicem(nodus, i);

        si (liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (!_elementum_est(liberum, "textus") || textus != NIHIL)
        {
            redde _vitium(vitium, titulus, ZEPHYRUM,
                          "in cantilena solum textus unus");
        }
        textus = liberum;
    }
    si (textus == NIHIL)
    {
        redde _vitium(vitium, titulus, ZEPHYRUM,
            "cantilena sine textu");
    }
    cantio->textus = stml_textus_valor(textus, piscina);
    si (!cantus_legere(cantio->textus, piscina, &cantio->cantilena,
        &cv))
    {
        redde _vitium(vitium, titulus, cv.offset, cv.causa);
    }
    si (cantio->cantilena.numerus == ZEPHYRUM)
    {
        redde _vitium(vitium, titulus, ZEPHYRUM,
            "cantilena sine notis");
    }
    si (cantio->cantilena.numerus > CANTUALE_NOTAE_MAXIMAE)
    {
        redde _vitium(vitium, titulus,
            cantio->cantilena.notae[CANTUALE_NOTAE_MAXIMAE].offset,
            "notae plures quam XXXII");
    }
    /* '@titulus' quisque nunc resolvitur - offset ad '@' */
    per (i = ZEPHYRUM; i < cantio->cantilena.numerus; i++)
    {
        chorda p = cantio->cantilena.notae[i].praeparatum;

        si (   p.mensura > ZEPHYRUM
            && cantus_praeparatum_sonitus(p, NIHIL) == NIHIL)
        {
            redde _vitium(vitium, titulus,
                (i32)(p.datum - cantio->textus.datum) - I,
                "praeparatum ignotum");
        }
    }
    redde VERUM;
}

b32
cantuale_legere (
            chorda  stml,
           Piscina* piscina,
          Cantuale* liber,
    CantualeVitium* vitium)
{
    StmlResultus  r;
       StmlNodus* radix;
          Cantio* cantiones;
             i32  numerus  = ZEPHYRUM;
             i32  i;
             i32  j;

    liber->cantiones = NIHIL;
    liber->numerus = ZEPHYRUM;
    r = stml_legere(stml, piscina, internamentum_creare(piscina));
    si (!r.successus)
    {
        redde _vitium(vitium, _vacua(), ZEPHYRUM, "STML fractum");
    }
    radix = r.elementum_radix;
    si (radix == NIHIL || !_elementum_est(radix, "cantuale"))
    {
        redde _vitium(vitium, _vacua(), ZEPHYRUM,
                      "radix non est 'cantuale'");
    }

    /* transitus I: elementa et numerus */
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);

        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;   /* textus (spatia), commenta */
        }
        si (!_elementum_est(n, "cantilena"))
        {
            redde _vitium(vitium, _vacua(), ZEPHYRUM,
                          "in cantuale solum cantilenae");
        }
        numerus++;
    }
    si (numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    cantiones = (Cantio*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Cantio));
    si (cantiones == NIHIL)
    {
        redde _vitium(vitium, _vacua(), ZEPHYRUM, "piscina exhausta");
    }

    /* transitus II: cantiones */
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
         StmlNodus* n = stml_liberum_ad_indicem(radix, i);
            chorda* titulus;
               i32  k;

        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        titulus = stml_attributum_capere(n, "titulus");
        si (titulus == NIHIL)
        {
            redde _vitium(vitium, _vacua(), ZEPHYRUM,
                          "cantilena sine titulo");
        }
        si (!_signum_validum(*titulus))
        {
            redde _vitium(vitium, *titulus, ZEPHYRUM,
                          "titulus non [a-z0-9_]+");
        }
        per (k = ZEPHYRUM; k < j; k++)
        {
            si (chorda_aequalis(cantiones[k].titulus, *titulus))
            {
                redde _vitium(vitium, *titulus, ZEPHYRUM,
                              "titulus iteratus");
            }
        }
        si (!_cantionem_legere(n, *titulus, piscina, &cantiones[j],
                               vitium))
        {
            redde FALSUM;
        }
        j++;
    }
    liber->cantiones  = cantiones;
    liber->numerus    = numerus;
    redde VERUM;
}

b32
cantuale_ordinarium (
           Piscina* piscina,
          Cantuale* liber,
    CantualeVitium* vitium)
{
    redde cantuale_legere(cantuale_infixum_textus(piscina), piscina,
                          liber, vitium);
}

constans Cantio*
cantuale_invenire (
    constans Cantuale* liber,
               chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < liber->numerus; i++)
    {
        si (chorda_aequalis(liber->cantiones[i].titulus, titulus))
        {
            redde &liber->cantiones[i];
        }
    }
    redde NIHIL;
}

b32
cantio_generis_est (
    constans Cantio* cantio,
             chorda  genus)
{
    i32 i;

    per (i = ZEPHYRUM; i < cantio->genera_numerus; i++)
    {
        si (chorda_aequalis(cantio->genera[i], genus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

constans Cantio*
cantuale_eligere (
    constans Cantuale* liber,
               chorda  genus,
                 Sors* sors,
      constans Cantio* vitanda)
{
    i32 candidatae  = ZEPHYRUM;
    b32 inter       = FALSUM;   /* vitanda inter candidatas */
    b32 omittere;
    i32 electa;
    i32 i;

    per (i = ZEPHYRUM; i < liber->numerus; i++)
    {
        si (cantio_generis_est(&liber->cantiones[i], genus))
        {
            candidatae++;
            si (&liber->cantiones[i] == vitanda)
            {
                inter = VERUM;
            }
        }
    }
    si (candidatae == ZEPHYRUM)
    {
        redde NIHIL;
    }
    /* vitanda omittitur solum si candidata huius libri est et alia
     * exstat */
    omittere = inter && candidatae > I;
    electa = sors_intra(sors, omittere ? candidatae - I : candidatae);
    per (i = ZEPHYRUM; i < liber->numerus; i++)
    {
        constans Cantio* c = &liber->cantiones[i];

        si (!cantio_generis_est(c, genus) || (omittere && c == vitanda))
        {
            perge;
        }
        si (electa == ZEPHYRUM)
        {
            redde c;
        }
        electa--;
    }
    redde NIHIL;   /* non attingitur */
}
