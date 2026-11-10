/* adumbratio.c - forma adumbrationum (vide adumbratio.h).
 *
 * Gradus: stml_legere -> stml_expandere (macrones) -> arbor ambulatur:
 * quisque nodus elementi = nodus dispositionis (capsae) aut folium,
 * ordine praevio in 'loci' servatus (parens ante liberos: fundi sub
 * textu) -> dispositio_computare in cellulis VI x VIII -> pictura per
 * figura.h in Mandata. Vitium primum vincit; tunc Mandata = pagina
 * vitii. */

#include "adumbratio.h"
#include "stml.h"
#include "stml_macros.h"
#include "dispositio.h"
#include "figura.h"
#include "thema.h"
#include "fons.h"
#include "delineare_mandata.h"
#include "imago_png.h"

#include <stdio.h>
#include <string.h>

#define LITTERA          VI
#define LINEA            VIII
#define LITTERA_TITULI   VIII
#define VITIUM_MAXIMUM   CCLVI

nomen enumeratio {
    NODUS_COLUMNA = ZEPHYRUM,
    NODUS_LINEA,
    NODUS_ORDO,
    NODUS_MENSA,
    NODUS_FASCIA,
    NODUS_CAMPUS,
    NODUS_TEXTUS,
    NODUS_GLOBULUM,
    NODUS_SAGITTA,
    NODUS_CAPSA,
    NODUS_SPATIUM,
    NODUS_MANDATA
} GenusNodi;

/* attributa communia capsarum */
#define ATTR_CAPSAE \
    " latitudo altitudo spatium intervallum allineatio_x" \
    " allineatio_y fundus margo "

nomen structura {
     constans character* titulus;
                    s32  genus;
                    b32  capsa;      /* liberos elementa habet */
     constans character* attributa;  /* " a b c " permissa */
} SpeciesNodi;

hic_manens constans SpeciesNodi SPECIES[] = {
    { "columna",  NODUS_COLUMNA,  VERUM,  ATTR_CAPSAE },
    { "linea",    NODUS_LINEA,    VERUM,  ATTR_CAPSAE },
    { "ordo",     NODUS_ORDO,     VERUM,
        ATTR_CAPSAE "electus super " },
    { "mensa",    NODUS_MENSA,    VERUM,  ATTR_CAPSAE "focata " },
    { "fascia",   NODUS_FASCIA,   VERUM,  ATTR_CAPSAE },
    { "campus",   NODUS_CAMPUS,   VERUM,  ATTR_CAPSAE },
    { "textus",   NODUS_TEXTUS,   FALSUM,
        " latitudo altitudo fons color audax " },
    { "globulum", NODUS_GLOBULUM, FALSUM,
        " latitudo altitudo status textus electum obscurum " },
    { "sagitta",  NODUS_SAGITTA,  FALSUM,
        " latitudo altitudo directio obscura " },
    { "capsa",    NODUS_CAPSA,    FALSUM, " facta " },
    { "spatium",  NODUS_SPATIUM,  FALSUM, " latitudo altitudo " },
    { "mandata",  NODUS_MANDATA,  FALSUM, " latitudo altitudo " }
};

#define SPECIERUM_NUMERUS \
    ((i32)(magnitudo(SPECIES) / magnitudo(SPECIES[ZEPHYRUM])))

nomen structura {
     constans character* nomen_coloris;
             ColorThema  gradus;
} ParColoris;

hic_manens constans ParColoris COLORES[] = {
    { "fundus",      COLOR_BACKGROUND },
    { "superficies", COLOR_SUPERFICIES },
    { "textus",      COLOR_TEXT },
    { "obscurus",    COLOR_TEXT_DIM },
    { "selectio",    COLOR_SELECTION },
    { "margo",       COLOR_BORDER },
    { "accentus",    COLOR_ACCENT_PRIMARY },
    { "accentus2",   COLOR_ACCENT_SECONDARY },
    { "monitum",     COLOR_WARNING },
    { "vitium",      COLOR_ERROR },
    { "successus",   COLOR_SUCCESS }
};

#define COLORUM_NUMERUS \
    ((i32)(magnitudo(COLORES) / magnitudo(COLORES[ZEPHYRUM])))

nomen structura {
          s32  index;   /* nodus dispositionis */
    StmlNodus* nodus;
          s32  genus;
} Locus;

nomen structura {
                 Piscina* piscina;
     InternamentumChorda* intern;
              Dispositio* d;
                     Xar* loci;
                  chorda  vitium;
} Faber;


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

/* vitium primum vincit: "linea N: <nuntius> '<verbum>'" */
interior vacuum
vitium_ponere (
                 Faber* f,
                   i32  linea,
    constans character* nuntius,
                chorda  verbum)
{
    character textus[VITIUM_MAXIMUM];

    si (f->vitium.mensura > ZEPHYRUM)
    {
        redde;
    }
    sprintf(textus, "linea %u: %s '%.*s'", (insignatus integer)linea,
        nuntius,
        (integer)(verbum.mensura < C ? verbum.mensura : C),
        (constans character*)verbum.datum);
    f->vitium = chorda_ex_literis(textus, f->piscina);
}

/* characteres (UTF-8: octeti continuationis non numerantur) */
interior s32
runae (
    chorda s)
{
    i32 i;
    s32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si ((s.datum[i] & 0xC0) != 0x80)
        {
            n++;
        }
    }
    redde n;
}

/* s ad n characteres */
interior chorda
runas_praecidere (
    chorda s,
       s32 n)
{
    i32 i;
    s32 k = ZEPHYRUM;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si ((s.datum[i] & 0xC0) != 0x80)
        {
            si (k == n)
            {
                s.mensura = i;
                frange;
            }
            k++;
        }
    }
    redde s;
}

interior chorda
attributum (
    constans StmlNodus* nodus,
    constans character* titulus)
{
    chorda* a = stml_attributum_capere(nodus, titulus);

    redde a ? *a : vacua();
}

/* numerus non negativus; FALSUM si non */
interior b32
numerum_legere (
    chorda  s,
       s32* v)
{
    redde s.mensura > ZEPHYRUM && chorda_ut_s32(s, v) && *v >= ZEPHYRUM;
}

interior b32
verum_legere (
                 Faber* f,
             StmlNodus* nodus,
    constans character* titulus,
                   b32* v)
{
    chorda a = attributum(nodus, titulus);

    *v = FALSUM;
    si (a.mensura == ZEPHYRUM || chorda_aequalis_literis(a, "falsum"))
    {
        redde VERUM;
    }
    si (chorda_aequalis_literis(a, "verum"))
    {
        *v = VERUM;
        redde VERUM;
    }
    vitium_ponere(f, nodus->linea, "valor malus (verum|falsum)", a);
    redde FALSUM;
}

/* nomen coloris -> gradus; FALSUM (vitium) si ignotum */
interior b32
colorem_legere (
         Faber* f,
    StmlNodus* nodus,
        chorda  a,
    ColorThema* gradus)
{
    i32 k;

    per (k = ZEPHYRUM; k < COLORUM_NUMERUS; k++)
    {
        si (chorda_aequalis_literis(a, COLORES[k].nomen_coloris))
        {
            *gradus = COLORES[k].gradus;
            redde VERUM;
        }
    }
    vitium_ponere(f, nodus->linea, "color ignotus", a);
    redde FALSUM;
}

/* "12" | "apta" | "crescens" | "40%" */
interior b32
mensuram_legere (
                 Faber* f,
             StmlNodus* nodus,
    constans character* titulus,
     DispositioMensura* m)
{
    chorda a = attributum(nodus, titulus);
       s32 v;

    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;   /* ordinaria manet */
    }
    si (chorda_aequalis_literis(a, "apta"))
    {
        m->genus = DISPOSITIO_APTA;
        redde VERUM;
    }
    si (chorda_aequalis_literis(a, "crescens"))
    {
        m->genus = DISPOSITIO_CRESCENS;
        redde VERUM;
    }
    si (a.datum[a.mensura - I] == '%')
    {
        chorda numerus = a;

        numerus.mensura--;
        si (numerum_legere(numerus, &v) && v <= C)
        {
            m->genus = DISPOSITIO_PARS;
            m->valor = v;
            redde VERUM;
        }
    }
    alioquin si (numerum_legere(a, &v))
    {
        m->genus = DISPOSITIO_FIXA;
        m->valor = v;
        redde VERUM;
    }
    vitium_ponere(f, nodus->linea, titulus, a);
    redde FALSUM;
}

interior b32
allineationem_legere (
                    Faber* f,
                StmlNodus* nodus,
       constans character* titulus,
     DispositioAllineatio* al)
{
    chorda a = attributum(nodus, titulus);

    si (a.mensura == ZEPHYRUM || chorda_aequalis_literis(a, "initium"))
    {
        *al = DISPOSITIO_INITIUM;
        redde VERUM;
    }
    si (chorda_aequalis_literis(a, "medium"))
    {
        *al = DISPOSITIO_MEDIUM;
        redde VERUM;
    }
    si (chorda_aequalis_literis(a, "finis"))
    {
        *al = DISPOSITIO_FINIS;
        redde VERUM;
    }
    vitium_ponere(f, nodus->linea, titulus, a);
    redde FALSUM;
}

interior constans SpeciesNodi*
speciem_invenire (
    chorda titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < SPECIERUM_NUMERUS; k++)
    {
        si (chorda_aequalis_literis(titulus, SPECIES[k].titulus))
        {
            redde &SPECIES[k];
        }
    }
    redde NIHIL;
}

/* attributa omnia in indice permissorum? */
interior b32
attributa_iudicare (
                   Faber* f,
               StmlNodus* nodus,
    constans SpeciesNodi* sp)
{
    i32 k;

    per (k = ZEPHYRUM; nodus->attributa
                       && k < xar_numerus(nodus->attributa); k++)
    {
        constans StmlAttributum* a = (constans StmlAttributum*)
            xar_obtinere(nodus->attributa, k);
        character signum[LXIV];

        si (!a->titulus || a->titulus->mensura >= LX)
        {
            perge;
        }
        sprintf(signum, " %.*s ", (integer)a->titulus->mensura,
            (constans character*)a->titulus->datum);
        si (!strstr(sp->attributa, signum))
        {
            vitium_ponere(f, nodus->linea, "attributum ignotum",
                *a->titulus);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* textus (spatiis exterioribus demptis) */
interior chorda
textum_nodi (
         Faber* f,
     StmlNodus* nodus)
{
    redde stml_textus_valor(nodus, f->piscina);
}

interior b32
textus_vacuus (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (   s.datum[i] != ' ' && s.datum[i] != '\n'
            && s.datum[i] != '\t' && s.datum[i] != '\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Arbor -> dispositio
 * ================================================== */

interior b32
liberos_addere (
        Faber* f,
    StmlNodus* nodus,
          s32  parens,
          s32  genus_parentis);

/* globulus: textus ex status aut textus= */
interior b32
globuli_textum (
          Faber* f,
      StmlNodus* nodus,
         chorda* textus,
            i32* status)
{
    chorda s = attributum(nodus, "status");
       b32 v;

    *status = ZEPHYRUM;
    si (s.mensura > ZEPHYRUM)
    {
        si (chorda_aequalis_literis(s, "shipped"))
        {
            *status = FIGURA_ELECTUM;
        }
        alioquin si (chorda_aequalis_literis(s, "planned"))
        {
            *status = FIGURA_OBSCURUM;
        }
        alioquin si (!chorda_aequalis_literis(s, "partial"))
        {
            vitium_ponere(f, nodus->linea, "status ignotus", s);
            redde FALSUM;
        }
        *textus = s;
        redde VERUM;
    }
    *textus = attributum(nodus, "textus");
    si (!verum_legere(f, nodus, "electum", &v))
    {
        redde FALSUM;
    }
    *status |= v ? FIGURA_ELECTUM : ZEPHYRUM;
    si (!verum_legere(f, nodus, "obscurum", &v))
    {
        redde FALSUM;
    }
    *status |= v ? FIGURA_OBSCURUM : ZEPHYRUM;
    redde VERUM;
}

interior b32
nodum_addere (
         Faber* f,
     StmlNodus* nodus,
           s32  parens,
           s32  genus_parentis)
{
     constans SpeciesNodi* sp;
          DispositioForma  forma;
                    Locus* locus;
                   chorda  textus;
                      s32  v;
                      s32  index;
                      i32  status;

    sp = speciem_invenire(*nodus->titulus);
    si (!sp)
    {
        vitium_ponere(f, nodus->linea, "elementum ignotum",
            *nodus->titulus);
        redde FALSUM;
    }
    si (!attributa_iudicare(f, nodus, sp))
    {
        redde FALSUM;
    }
    dispositio_formam_initiare(&forma);
    forma.directio = (sp->genus == NODUS_LINEA
        || sp->genus == NODUS_ORDO)
        ? DISPOSITIO_LINEA : DISPOSITIO_COLUMNA;
    /* magnitudines ordinariae foliorum */
    commutatio (sp->genus)
    {
        casus NODUS_TEXTUS:
            textus = textum_nodi(f, nodus);
            v = chorda_aequalis_literis(attributum(nodus, "fons"),
                "titulus") ? LITTERA_TITULI : LITTERA;
            forma.latitudo.genus  = DISPOSITIO_FIXA;
            forma.latitudo.valor  = (runae(textus) * v + LITTERA - I)
                / LITTERA;
            forma.altitudo.genus  = DISPOSITIO_FIXA;
            forma.altitudo.valor  = v == LITTERA_TITULI ? II : I;
            /* in linea: altitudo lineae, textus centratur */
            si (   genus_parentis == NODUS_LINEA
                || genus_parentis == NODUS_ORDO)
            {
                forma.altitudo.genus    = DISPOSITIO_CRESCENS;
                forma.altitudo.minimum  = forma.altitudo.valor;
            }
            frange;
        casus NODUS_GLOBULUM:
            si (!globuli_textum(f, nodus, &textus, &status))
            {
                redde FALSUM;
            }
            forma.latitudo.genus  = DISPOSITIO_FIXA;
            forma.latitudo.valor  = runae(textus) + II;
            forma.altitudo.genus  = DISPOSITIO_FIXA;
            forma.altitudo.valor  = II;
            frange;
        casus NODUS_SAGITTA:
            forma.latitudo.genus  = DISPOSITIO_FIXA;
            forma.latitudo.valor  = III;
            forma.altitudo.genus  = DISPOSITIO_FIXA;
            forma.altitudo.valor  = II;
            frange;
        casus NODUS_CAPSA:
            forma.latitudo.genus  = DISPOSITIO_FIXA;
            forma.latitudo.valor  = II;
            forma.altitudo.genus  = DISPOSITIO_FIXA;
            forma.altitudo.valor  = II;
            frange;
        casus NODUS_SPATIUM:
        casus NODUS_MANDATA:
            forma.latitudo.genus  = DISPOSITIO_CRESCENS;
            forma.altitudo.genus  = DISPOSITIO_CRESCENS;
            frange;
        ordinarius:
            frange;
    }
    si (   !mensuram_legere(f, nodus, "latitudo", &forma.latitudo)
        || !mensuram_legere(f, nodus, "altitudo", &forma.altitudo))
    {
        redde FALSUM;
    }
    si (sp->capsa)
    {
        chorda a;

        a = attributum(nodus, "spatium");
        si (a.mensura > ZEPHYRUM)
        {
            si (!numerum_legere(a, &v))
            {
                vitium_ponere(f, nodus->linea, "spatium", a);
                redde FALSUM;
            }
            forma.spatium_sinistrum  = v;
            forma.spatium_dextrum    = v;
            forma.spatium_superum    = v;
            forma.spatium_inferum    = v;
        }
        a = attributum(nodus, "intervallum");
        si (a.mensura > ZEPHYRUM)
        {
            si (!numerum_legere(a, &v))
            {
                vitium_ponere(f, nodus->linea, "intervallum", a);
                redde FALSUM;
            }
            forma.intervallum = v;
        }
        si (   !allineationem_legere(f, nodus, "allineatio_x",
                   &forma.allineatio_x)
            || !allineationem_legere(f, nodus, "allineatio_y",
                   &forma.allineatio_y))
        {
            redde FALSUM;
        }
    }
    index = dispositio_addere(f->d, parens, &forma);
    locus = (Locus*)xar_addere(f->loci);
    si (index < ZEPHYRUM || !locus)
    {
        redde FALSUM;
    }
    locus->index = index;
    locus->nodus = nodus;
    locus->genus = sp->genus;
    redde sp->capsa ? liberos_addere(f, nodus, index, sp->genus)
                    : VERUM;
}

interior b32
liberos_addere (
         Faber* f,
     StmlNodus* nodus,
           s32  parens,
           s32  genus_parentis)
{
    StmlNodus* liberum;

    per (liberum = stml_primus_liberum(nodus); liberum;
         liberum = stml_frater_proximus(liberum))
    {
        /* fragmentum aut transclusio sine '#@': contentum, non
         * templum - expansio ea transit; hic vitium, numquam nihil */
        si (liberum->fragmentum)
        {
            vitium_ponere(f, liberum->linea,
                "fragmentum sine '#@' (templum: <#@nomen>)",
                liberum->titulus ? *liberum->titulus : vacua());
            redde FALSUM;
        }
        si (liberum->genus == STML_NODUS_TRANSCLUSIO)
        {
            vitium_ponere(f, liberum->linea,
                "transclusio non resoluta (templum: <<#@nomen>>)",
                stml_scribere(liberum, f->piscina, FALSUM));
            redde FALSUM;
        }
        si (liberum->genus == STML_NODUS_ELEMENTUM)
        {
            si (!nodum_addere(f, liberum, parens, genus_parentis))
            {
                redde FALSUM;
            }
        }
        alioquin si (   liberum->genus == STML_NODUS_TEXTUS
                     && !textus_vacuus(stml_textus_internus(liberum,
                            f->piscina)))
        {
            vitium_ponere(f, liberum->linea, "textus extra <textus>",
                stml_textus_valor(liberum, f->piscina));
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Pictura
 * ================================================== */

/* sub in md intra fines: coetus cum sectione, origo coetus =
 * fines.x/y (+ translatio, hic 0); sectio = fines in spatio parentis */
interior vacuum
transcribere (
             Mandata* md,
    constans Mandata* sub,
               Fines  fines,
              chorda  provenientia)
{
    i32 coetus;
    i32 k;

    coetus = (i32)mandata_coetus_incipere(md, fines, VERUM, ZEPHYRUM,
        ZEPHYRUM, I, provenientia);
    per (k = ZEPHYRUM; k < xar_numerus(sub->elementa); k++)
    {
        Mandatum* locus = (Mandatum*)xar_addere(md->elementa);

        si (locus)
        {
            *locus = *(constans Mandatum*)xar_obtinere(sub->elementa,
                k);
        }
    }
    mandata_coetus_finire(md, coetus);
}

/* elementum mandata: nomina colorum -> gradus, serializatio, lectio,
 * coetus translatus in capsam (cum sectione) */
interior b32
mandata_pingere (
          Faber* f,
        Mandata* md,
      StmlNodus* nodus,
          Fines  fines)
{
      StmlNodus* liberum;
        Mandata* sub;
         chorda  textus;
            i32  k;

    per (liberum = stml_primus_liberum(nodus); liberum;
         liberum = stml_frater_proximus(liberum))
    {
            chorda* color;
        ColorThema  gradus;
               s32  n;
         character  numerus[XVI];

        si (liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        color = stml_attributum_capere(liberum, "color");
        si (!color || chorda_ut_s32(*color, &n))
        {
            perge;
        }
        si (!colorem_legere(f, liberum, *color, &gradus))
        {
            redde FALSUM;
        }
        sprintf(numerus, "%u", (insignatus integer)gradus);
        per (k = ZEPHYRUM; k < xar_numerus(liberum->attributa); k++)
        {
            StmlAttributum* a = (StmlAttributum*)xar_obtinere(
                liberum->attributa, k);

            si (   a->titulus && chorda_aequalis_literis(*a->titulus,
                    "color"))
            {
                a->valor = chorda_internare_ex_literis(f->intern,
                    numerus);
            }
        }
    }
    textus = stml_scribere(nodus, f->piscina, FALSUM);
    sub    = mandata_legere_stml(chorda_ut_cstr(textus, f->piscina),
        f->piscina, f->intern);
    si (!sub)
    {
        vitium_ponere(f, nodus->linea, "mandata mala", *nodus->titulus);
        redde FALSUM;
    }
    transcribere(md, sub, fines, chorda_ex_literis("mandata",
        f->piscina));
    redde VERUM;
}

interior b32
locum_pingere (
             Faber* f,
           Mandata* md,
    constans Locus* l,
             Fines  fines)
{
     StmlNodus* nodus = l->nodus;
    ColorThema  gradus;
        chorda  a;
        chorda  textus;
           b32  v;
           i32  status;

    commutatio (l->genus)
    {
        casus NODUS_ORDO:
            si (!verum_legere(f, nodus, "electus", &v))
            {
                redde FALSUM;
            }
            si (v)
            {
                figura_ordinem_electum_pingere(md, fines);
            }
            alioquin
            {
                si (!verum_legere(f, nodus, "super", &v))
                {
                    redde FALSUM;
                }
                si (v)
                {
                    figura_ordinem_super_pingere(md, fines);
                }
            }
            frange;
        casus NODUS_MENSA:
            si (!verum_legere(f, nodus, "focata", &v))
            {
                redde FALSUM;
            }
            figura_mensam_pingere(md, fines, v);
            frange;
        casus NODUS_FASCIA:
            figura_fasciam_pingere(md, fines);
            frange;
        casus NODUS_CAMPUS:
            figura_campum_pingere(md, fines);
            frange;
        casus NODUS_TEXTUS:
        {
            i32 fons;
            s32 lat;
            s32 y;

            textus  = textum_nodi(f, nodus);
            gradus  = COLOR_TEXT;
            a       = attributum(nodus, "color");
            si (   a.mensura > ZEPHYRUM && !colorem_legere(f, nodus, a,
                    &gradus))
            {
                redde FALSUM;
            }
            a = attributum(nodus, "fons");
            si (   a.mensura > ZEPHYRUM
                && !chorda_aequalis_literis(a, "parvus")
                && !chorda_aequalis_literis(a, "titulus"))
            {
                vitium_ponere(f, nodus->linea, "fons ignotus", a);
                redde FALSUM;
            }
            fons  = chorda_aequalis_literis(a, "titulus") ? FONS_TITULI
                                                          : FONS_PARVUS;
            lat   = fons == FONS_TITULI ? LITTERA_TITULI : LITTERA;
            textus = chorda_transcribere(runas_praecidere(textus,
                fines.latitudo / lat), md->piscina);
            /* verticaliter centratus in capsa sua */
            y = fines.y;
            si (fines.altitudo > fons_altitudo(fons))
            {
                y += (fines.altitudo - fons_altitudo(fons)) / II;
            }
            si (!verum_legere(f, nodus, "audax", &v))
            {
                redde FALSUM;
            }
            si (v)
            {
                figura_textum_audacem(md, fines.x, y, textus,
                    figura_color_thematis(gradus));
            }
            alioquin
            {
                mandata_textus(md, fines.x, y, textus, fons,
                    figura_color_thematis(gradus));
            }
            redde VERUM;
        }
        casus NODUS_GLOBULUM:
            si (!globuli_textum(f, nodus, &textus, &status))
            {
                redde FALSUM;
            }
            figura_globulum_pingere(md, fines,
                chorda_transcribere(textus,
                md->piscina), status);
            redde VERUM;
        casus NODUS_SAGITTA:
            a = attributum(nodus, "directio");
            si (   a.mensura > ZEPHYRUM
                && !chorda_aequalis_literis(a, "sinistra")
                && !chorda_aequalis_literis(a, "dextra"))
            {
                vitium_ponere(f, nodus->linea, "directio ignota", a);
                redde FALSUM;
            }
            si (!verum_legere(f, nodus, "obscura", &v))
            {
                redde FALSUM;
            }
            figura_sagittam_pingere(md, fines,
                chorda_aequalis_literis(a,
                "sinistra") ? -I : I, v ? FIGURA_OBSCURUM : ZEPHYRUM);
            redde VERUM;
        casus NODUS_CAPSA:
            si (!verum_legere(f, nodus, "facta", &v))
            {
                redde FALSUM;
            }
            figura_capsam_pingere(md, fines.x, fines.y, v);
            redde VERUM;
        casus NODUS_MANDATA:
            redde mandata_pingere(f, md, nodus, fines);
        ordinarius:
            frange;
    }
    /* capsae: fundus, margo (super figuram capsae) */
    a = attributum(nodus, "fundus");
    si (a.mensura > ZEPHYRUM)
    {
        si (!colorem_legere(f, nodus, a, &gradus))
        {
            redde FALSUM;
        }
        mandata_rectangulum(md, fines, figura_color_thematis(gradus),
            VERUM);
    }
    a = attributum(nodus, "margo");
    si (a.mensura > ZEPHYRUM)
    {
        si (!colorem_legere(f, nodus, a, &gradus))
        {
            redde FALSUM;
        }
        mandata_rectangulum(md, fines, figura_color_thematis(gradus),
            FALSUM);
    }
    redde VERUM;
}

/* pagina vitii: fundus, "Mock-up error", nuntius per lineas */
interior Mandata*
paginam_vitii_facere (
                Piscina* piscina,
    InternamentumChorda* intern,
                    s32  latitudo,
                    s32  altitudo,
                 chorda  vitium)
{
     Mandata* md;
       Fines  totum;
         s32  columnae;
         s32  y;
      chorda  reliquum;

    md              = mandata_creare(piscina, intern);
    totum.x         = ZEPHYRUM;
    totum.y         = ZEPHYRUM;
    totum.latitudo  = latitudo;
    totum.altitudo  = altitudo;
    mandata_rectangulum(md, totum, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    mandata_textus(md, VIII, VIII, chorda_ex_literis("Mock-up error",
        piscina), FONS_TITULI, figura_color_thematis(COLOR_ERROR));
    columnae  = (latitudo - XVI) / LITTERA;
    columnae  = columnae > X ? columnae : X;
    y         = XXXII;
    reliquum  = vitium;
    dum (reliquum.mensura > ZEPHYRUM && y + LINEA <= altitudo)
    {
        chorda linea = runas_praecidere(reliquum, columnae);

        mandata_textus(md, VIII, y, chorda_transcribere(linea, piscina),
            FONS_PARVUS, figura_color_thematis(COLOR_TEXT));
        reliquum.datum    += linea.mensura;
        reliquum.mensura  -= linea.mensura;
        y                 += X;
    }
    redde md;
}

/* nuntius vitii expansionis */
interior constans character*
nuntium_expansionis (
    StmlExpansioVitium v)
{
    commutatio (v)
    {
        casus STML_EXPANSIO_FRAGMENTUM_IGNOTUM:
            redde "fragmentum ignotum";
        casus STML_EXPANSIO_FRAGMENTUM_POSTERIUS:
            redde "fragmentum posterius definitum";
        casus STML_EXPANSIO_FRAGMENTUM_GEMINUM:
            redde "fragmentum bis definitum";
        casus STML_EXPANSIO_LOCULUS_NON_IMPLETUS:
            redde "loculus non impletus";
        casus STML_EXPANSIO_ARGUMENTUM_SUPERFLUUM:
            redde "argumentum superfluum";
        casus STML_EXPANSIO_LOCULUS_IGNOTUS:
            redde "loculus ignotus";
        ordinarius:
            redde "vitium expansionis";
    }
}


/* ==================================================
 * Interfacies
 * ================================================== */

AdumbratioResultus
adumbrationem_reddere (
                 chorda  documentum,
                    s32  latitudo_ordinaria,
                    s32  altitudo_ordinaria,
                Piscina* piscina,
    InternamentumChorda* intern)
{
      AdumbratioResultus  r;
            StmlResultus  res;
    StmlExpansioResultus  ex;
               StmlNodus* radix = NIHIL;
               StmlNodus* liberum;
                   Faber  f;
         DispositioForma  forma;
                     s32  radix_d;
                     s32  columnae;
                     s32  lineae;
                     s32  v;
                     i32  k;

    memset(&r, ZEPHYRUM, magnitudo(r));
    memset(&f, ZEPHYRUM, magnitudo(f));
    f.piscina  = piscina;
    f.intern   = intern;
    f.vitium   = vacua();
    r.latitudo  = latitudo_ordinaria > ZEPHYRUM ? latitudo_ordinaria
                                                : CCLXXX;
    r.altitudo  = altitudo_ordinaria > ZEPHYRUM ? altitudo_ordinaria
                                                : CCLVI;
    res = stml_legere(documentum, piscina, intern);
    si (!res.successus || !res.radix)
    {
        vitium_ponere(&f, res.linea_erroris, "STML malum", res.error);
    }
    alioquin
    {
        ex = stml_expandere(res.radix, piscina, intern);
        si (!ex.successus || !ex.radix_expansa)
        {
            vitium_ponere(&f, ex.linea, nuntium_expansionis(ex.vitium),
                ex.fragmentum.mensura > ZEPHYRUM ? ex.fragmentum
                : ex.loculus);
        }
        alioquin
        {
            per (liberum = stml_primus_liberum(ex.radix_expansa);
                 liberum && !radix; liberum = stml_frater_proximus(
                     liberum))
            {
                si (liberum->genus == STML_NODUS_ELEMENTUM)
                {
                    radix = liberum;
                }
            }
            si (   !radix
                || !chorda_aequalis_literis(*radix->titulus,
                       "adumbratio"))
            {
                vitium_ponere(&f, radix ? radix->linea : ZEPHYRUM,
                    "radix non <adumbratio>", radix ? *radix->titulus
                    : vacua());
            }
        }
    }
    /* radix: latitudo, altitudo (pixela) */
    per (k = ZEPHYRUM; radix && f.vitium.mensura == ZEPHYRUM && k < II;
         k++)
    {
        chorda a = attributum(radix, k == ZEPHYRUM ? "latitudo"
                                                   : "altitudo");

        si (a.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (!numerum_legere(a, &v) || v < XVI)
        {
            vitium_ponere(&f, radix->linea, k == ZEPHYRUM ? "latitudo"
                : "altitudo", a);
            frange;
        }
        si (k == ZEPHYRUM)
        {
            r.latitudo = v;
        }
        alioquin
        {
            r.altitudo = v;
        }
    }
    si (f.vitium.mensura == ZEPHYRUM)
    {
        f.d     = dispositio_creare(piscina);
        f.loci  = xar_creare(piscina, (i32)magnitudo(Locus));
        dispositio_formam_initiare(&forma);
        forma.directio        = DISPOSITIO_COLUMNA;
        forma.latitudo.genus  = DISPOSITIO_CRESCENS;
        forma.altitudo.genus  = DISPOSITIO_CRESCENS;
        radix_d               = dispositio_addere(f.d, -I, &forma);
        (vacuum)liberos_addere(&f, radix, radix_d, NODUS_COLUMNA);
    }
    si (f.vitium.mensura == ZEPHYRUM)
    {
        Fines totum;

        columnae  = r.latitudo / LITTERA;
        lineae    = r.altitudo / LINEA;
        dispositio_computare(f.d, columnae, lineae, NIHIL, NIHIL);
        r.mandata       = mandata_creare(piscina, intern);
        totum.x         = ZEPHYRUM;
        totum.y         = ZEPHYRUM;
        totum.latitudo  = r.latitudo;
        totum.altitudo  = r.altitudo;
        mandata_rectangulum(r.mandata, totum, figura_color_thematis(
            COLOR_BACKGROUND), VERUM);
        per (k = ZEPHYRUM; k < xar_numerus(f.loci); k++)
        {
            constans Locus* l = (constans Locus*)xar_obtinere(f.loci,
                k);

            si (!locum_pingere(&f, r.mandata, l, dispositio_ad_pixela(
                    dispositio_fines(f.d, l->index), columnae, lineae,
                    r.latitudo, r.altitudo, LITTERA, LINEA)))
            {
                frange;
            }
        }
    }
    si (f.vitium.mensura > ZEPHYRUM)
    {
        r.mandata = paginam_vitii_facere(piscina, intern, r.latitudo,
            r.altitudo, f.vitium);
    }
    r.vitium = f.vitium;
    redde r;
}

b32
adumbrationem_png_scribere (
    constans AdumbratioResultus* r,
             constans character* via,
                        Piscina* piscina)
{
    TabulaPixelorum* t;
              Imago  im;

    si (!r || !r->mandata || !via || !piscina)
    {
        redde FALSUM;
    }
    t = tabula_pixelorum_creare_nuda(piscina, (i32)r->latitudo,
        (i32)r->altitudo);
    si (!t)
    {
        redde FALSUM;
    }
    delineare_mandata(r->mandata, t, NIHIL, NIHIL);
    redde tabula_pixelorum_in_imaginem(t, piscina, &im)
        && imago_png_scribere(&im, via, piscina).successus;
}

vacuum
adumbrationem_inserere (
     constans AdumbratioResultus* r,
                         Mandata* md,
                           Fines  fines)
{
    si (r && r->mandata && md)
    {
        transcribere(md, r->mandata, fines, chorda_ex_literis(
            "adumbratio", md->piscina));
    }
}
