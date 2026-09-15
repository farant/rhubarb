/* materia_coctor.c - Registrum coctum ex declaratione STML coquere
 *
 * Vide materia_coctor.h. Redditio forma domus: lineae <= LXXII,
 * commentaria notarum SUPER ordinem suum (cauda longa limitem
 * frangeret), numeri Romani in prosa commentariorum, ordines
 * tabularum sine tabulatione (formator eos non tangit). Tituli in
 * identificatores: '-' -> '_', litterae maiusculae.
 */
#include "materia_coctor.h"
#include "materia_nodus.h"
#include "stml.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "xar.h"
#include <string.h>

nomen structura {
    chorda titulus;
       s32 species;
    chorda nota;
} Locus;

nomen structura {
    chorda titulus;
    chorda nota;
       i32 offset;
       i32 numerus;
} Genus;

/* index = MateriaLocusSpecies (porta: numerus == NUMERUS_SPECIERUM) */
hic_manens constans character* constans TITULI_SPECIERUM[] = {
    "nodus", "token", "lista-nodus", "lista-token", "lista-mixta",
    "index", "referentia"
};

hic_manens constans character* constans STIRPS = "_registrum_coctum";

constans character*
materia_locus_species_titulus (
    s32 species)
{
    si (   species < ZEPHYRUM
        || species >= (s32)MATERIA_LOCUS_NUMERUS_SPECIERUM)
    {
        redde NIHIL;
    }
    redde TITULI_SPECIERUM[species];
}

s32
materia_locus_species_ex_titulo (
    chorda titulus)
{
    s32 k;

    per (k = ZEPHYRUM; k < (s32)MATERIA_LOCUS_NUMERUS_SPECIERUM; k++)
    {
        si (chorda_aequalis_literis(titulus, TITULI_SPECIERUM[k]))
        {
            redde k;
        }
    }
    redde (s32)-I;
}

b32
materia_coctio_aequalis (
    chorda  generatum,
    chorda  in_disco,
       i32* linea_divergens)
{
    i32 i;
    i32 linea = I;
    i32 minor = generatum.mensura < in_disco.mensura
        ? generatum.mensura : in_disco.mensura;

    per (i = ZEPHYRUM; i < minor; i++)
    {
        si (generatum.datum[i] != in_disco.datum[i])
        {
            *linea_divergens = linea;
            redde FALSUM;
        }
        si (generatum.datum[i] == (i8)'\n')
        {
            linea = linea + I;
        }
    }
    si (generatum.mensura != in_disco.mensura)
    {
        *linea_divergens = linea;
        redde FALSUM;
    }
    *linea_divergens = ZEPHYRUM;
    redde VERUM;
}


/* ==================================================
 * Recusatio
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

/* causa = praefixum + ' adiunctum' (vacuum licet); semper FALSUM */
interior b32
_recusare (
               Piscina* piscina,
         MateriaCoctio* coctio,
                   i32  linea,
    constans character* praefixum,
                chorda  adiunctum)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)CXXVIII);

    si (a == NIHIL)
    {
        redde FALSUM;
    }
    chorda_aedificator_appendere_literis(a, praefixum);
    si (adiunctum.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " '");
        chorda_aedificator_appendere_chorda(a, adiunctum);
        chorda_aedificator_appendere_literis(a, "'");
    }
    coctio->causa = chorda_aedificator_finire(a);
    coctio->linea = linea;
    redde FALSUM;
}

/* titulus kebab: [a-z][a-z0-9-]* */
interior b32
_titulus_sanus (
    chorda t)
{
    i32 i;

    si (t.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c        = (character)t.datum[i];
              b32 littera  = (b32)(c >= 'a' && c <= 'z');
              b32 cifra    = (b32)(c >= '0' && c <= '9');

        si (i == ZEPHYRUM && !littera)
        {
            redde FALSUM;
        }
        si (!littera && !cifra && c != '-')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* identificator C: [A-Za-z_][A-Za-z0-9_]*; maiusculum: minusculae
 * vetitae */
interior b32
_identificator_sanus (
    chorda t,
       b32 maiusculum)
{
    i32 i;

    si (t.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c      = (character)t.datum[i];
              b32 maior  = (b32)(c >= 'A' && c <= 'Z');
              b32 minor  = (b32)(c >= 'a' && c <= 'z');
              b32 cifra  = (b32)(c >= '0' && c <= '9');

        si (maiusculum && minor)
        {
            redde FALSUM;
        }
        si (i == ZEPHYRUM && cifra)
        {
            redde FALSUM;
        }
        si (!maior && !minor && !cifra && c != '_')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Lectio declarationis
 * ================================================== */

interior chorda
_attributum (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(nodus, titulus);

    redde v == NIHIL ? _vacua() : *v;
}

/* attributum necessarium radicis; identificator retiratur si petitum */
interior b32
_attributum_radicis (
               Piscina* piscina,
         MateriaCoctio* coctio,
             StmlNodus* radix,
    constans character* titulus,
                   b32  identificator,
                   b32  maiusculum,
                chorda* exitus)
{
    *exitus = _attributum(radix, titulus);
    si (exitus->mensura == ZEPHYRUM)
    {
        redde _recusare(piscina, coctio, radix->linea,
            "attributum radicis deest:",
            chorda_ex_literis(titulus, piscina));
    }
    si (identificator && !_identificator_sanus(*exitus, maiusculum))
    {
        redde _recusare(piscina, coctio, radix->linea,
            "attributum radicis non identificator:", *exitus);
    }
    redde VERUM;
}

/* liberum k-tum si elementum, aliter NIHIL (textus, commenta) */
interior StmlNodus*
_elementum (
     StmlNodus* nodus,
           i32  k)
{
    StmlNodus* l = stml_liberum_ad_indicem(nodus, k);

    si (l != NIHIL && l->genus == STML_NODUS_ELEMENTUM)
    {
        redde l;
    }
    redde NIHIL;
}

interior b32
_locum_legere (
          Piscina* piscina,
    MateriaCoctio* coctio,
        StmlNodus* nodus,
              Xar* loci,
              i32  offset)
{
     Locus* l;
    chorda  species;
       i32  k;

    si (!chorda_aequalis_literis(*nodus->titulus, "locus"))
    {
        redde _recusare(piscina, coctio, nodus->linea,
            "elementum ignotum intra genus:", *nodus->titulus);
    }
    l = (Locus*)xar_addere(loci);
    si (l == NIHIL)
    {
        redde FALSUM;
    }
    l->titulus  = _attributum(nodus, "titulus");
    l->nota     = _attributum(nodus, "nota");
    species     = _attributum(nodus, "species");
    si (!_titulus_sanus(l->titulus))
    {
        redde _recusare(piscina, coctio, nodus->linea,
            "titulus loci non kebab:", l->titulus);
    }
    l->species = materia_locus_species_ex_titulo(species);
    si (l->species < ZEPHYRUM)
    {
        redde _recusare(piscina, coctio, nodus->linea,
            "species loci ignota:", species);
    }
    /* titulus unicus intra genus (offset = primus huius generis);
     * limes sine subtractione (i32 insignatus) */
    per (k = offset; k + I < xar_numerus(loci); k++)
    {
        constans Locus* prior = (constans Locus*)xar_obtinere(loci, k);

        si (chorda_aequalis(prior->titulus, l->titulus))
        {
            redde _recusare(piscina, coctio, nodus->linea,
                "titulus loci iteratus intra genus:", l->titulus);
        }
    }
    redde VERUM;
}

interior b32
_genus_legere (
          Piscina* piscina,
    MateriaCoctio* coctio,
        StmlNodus* nodus,
              Xar* genera,
              Xar* loci)
{
     Genus* g;
       i32  n;
       i32  k;

    si (!chorda_aequalis_literis(*nodus->titulus, "genus"))
    {
        redde _recusare(piscina, coctio, nodus->linea,
            "elementum ignotum intra registrum:", *nodus->titulus);
    }
    g = (Genus*)xar_addere(genera);
    si (g == NIHIL)
    {
        redde FALSUM;
    }
    g->titulus  = _attributum(nodus, "titulus");
    g->nota     = _attributum(nodus, "nota");
    g->offset   = xar_numerus(loci);
    g->numerus  = ZEPHYRUM;
    si (!_titulus_sanus(g->titulus))
    {
        redde _recusare(piscina, coctio, nodus->linea,
            "titulus generis non kebab:", g->titulus);
    }
    per (k = ZEPHYRUM; k + I < xar_numerus(genera); k++)
    {
        constans Genus* prior = (constans Genus*)xar_obtinere(genera,
            k);

        si (chorda_aequalis(prior->titulus, g->titulus))
        {
            redde _recusare(piscina, coctio, nodus->linea,
                "titulus generis iteratus:", g->titulus);
        }
    }
    n = stml_numerus_liberorum(nodus);
    per (k = ZEPHYRUM; k < n; k++)
    {
        StmlNodus* l = _elementum(nodus, k);

        si (l == NIHIL)
        {
            perge;
        }
        si (!_locum_legere(piscina, coctio, l, loci, g->offset))
        {
            redde FALSUM;
        }
        g->numerus = g->numerus + I;
    }
    redde VERUM;
}

interior b32
_declarationem_legere (
           Piscina* piscina,
            chorda  declaratio,
     MateriaCoctio* coctio,
               Xar* genera,
               Xar* loci)
{
    InternamentumChorda* intern = internamentum_creare(piscina);
           StmlResultus  r;
              StmlNodus* radix;
                    i32  n;
                    i32  k;

    si (intern == NIHIL)
    {
        redde FALSUM;
    }
    r = stml_legere(declaratio, piscina, intern);
    si (!r.successus || r.elementum_radix == NIHIL)
    {
        redde _recusare(piscina, coctio, r.linea_erroris,
            "declaratio parsari nequit:", r.error);
    }
    radix = r.elementum_radix;
    si (!chorda_aequalis_literis(*radix->titulus, "registrum"))
    {
        redde _recusare(piscina, coctio, radix->linea,
            "radix 'registrum' exspectata, inventa:", *radix->titulus);
    }
    si (   !_attributum_radicis(piscina, coctio, radix, "grammatica",
            VERUM, FALSUM, &coctio->grammatica)
        || !_attributum_radicis(piscina, coctio, radix, "praefixum",
            VERUM, VERUM, &coctio->praefixum)
        || !_attributum_radicis(piscina, coctio, radix, "typus",
            VERUM, FALSUM, &coctio->typus)
        || !_attributum_radicis(piscina, coctio, radix, "sedes",
            FALSUM, FALSUM, &coctio->sedes))
    {
        redde FALSUM;
    }
    n = stml_numerus_liberorum(radix);
    per (k = ZEPHYRUM; k < n; k++)
    {
        StmlNodus* g = _elementum(radix, k);

        si (g == NIHIL)
        {
            perge;
        }
        si (!_genus_legere(piscina, coctio, g, genera, loci))
        {
            redde FALSUM;
        }
    }
    si (xar_numerus(genera) == ZEPHYRUM)
    {
        redde _recusare(piscina, coctio, radix->linea,
            "registrum sine generibus", _vacua());
    }
    si (xar_numerus(loci) == ZEPHYRUM)
    {
        redde _recusare(piscina, coctio, radix->linea,
            "registrum sine locis (series vacua non compilat)",
            _vacua());
    }
    coctio->numerus_generum = xar_numerus(genera);
    coctio->numerus_locorum = xar_numerus(loci);
    redde VERUM;
}


/* ==================================================
 * Redditio
 * ================================================== */

interior vacuum
_literis (
     ChordaAedificator* a,
    constans character* s)
{
    chorda_aedificator_appendere_literis(a, s);
}

interior vacuum
_chordam (
    ChordaAedificator* a,
               chorda  s)
{
    chorda_aedificator_appendere_chorda(a, s);
}

interior vacuum
_linea_nova (
    ChordaAedificator* a)
{
    chorda_aedificator_appendere_lineam_novam(a);
}

/* titulus kebab ut identificator: '-' -> '_', maiusculae */
interior vacuum
_maiusculum (
    ChordaAedificator* a,
               chorda  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c = (character)t.datum[i];

        si (c >= 'a' && c <= 'z')
        {
            c = (character)(c - 'a' + 'A');
        }
        alioquin si (c == '-')
        {
            c = '_';
        }
        chorda_aedificator_appendere_character(a, c);
    }
}

/* numerus Romanus (I..MMMM); ZEPHYRUM = 'nihil' */
interior vacuum
_romanum (
    ChordaAedificator* a,
                  i32  n)
{
    hic_manens constans i32 VALORES[] = {
        (i32)M, (i32)CM, (i32)D, (i32)CD, (i32)C, (i32)XC, (i32)L,
        (i32)XL, (i32)X, (i32)IX, (i32)V, (i32)IV, (i32)I
    };
    hic_manens constans character* constans LITTERAE[] = {
        "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V",
        "IV", "I"
    };
    i32 k;

    si (n == ZEPHYRUM)
    {
        _literis(a, "nihil");
        redde;
    }
    per (k = ZEPHYRUM; k < (i32)XIII; k++)
    {
        dum (n >= VALORES[k])
        {
            _literis(a, LITTERAE[k]);
            n = n - VALORES[k];
        }
    }
}

/* prosam involvere: lineae <= LXXII, quaeque = indentatio + (primum
 * lineae primae | ceterum) + verba spatio uno; 'claudere' clausuram
 * commentarii lineae ultimae appendit (spatium ei reservatum). Verbum
 * longius linea sola stat. */
interior vacuum
_involvere (
     ChordaAedificator* a,
    constans character* indentatio,
    constans character* primum,
    constans character* ceterum,
                chorda  textus,
                   b32  claudere)
{
    i32 latitudo  = (i32)LXXII - (claudere ? (i32)III : ZEPHYRUM);
    i32 i         = ZEPHYRUM;
    i32 columna   = ZEPHYRUM;
    b32 prima     = VERUM;

    dum (i < textus.mensura)
    {
           i32 initium;
           i32 mensura;
        chorda verbum;

        dum (i < textus.mensura && textus.datum[i] == (i8)' ')
        {
            i = i + I;
        }
        initium = i;
        dum (i < textus.mensura && textus.datum[i] != (i8)' ')
        {
            i = i + I;
        }
        mensura = i - initium;
        si (mensura == ZEPHYRUM)
        {
            frange;
        }
        si (columna == ZEPHYRUM)
        {
            constans character* p = prima ? primum : ceterum;

            _literis(a, indentatio);
            _literis(a, p);
            columna  = (i32)strlen(indentatio) + (i32)strlen(p);
            prima    = FALSUM;
        }
        alioquin si (columna + I + mensura > latitudo)
        {
            _linea_nova(a);
            _literis(a, indentatio);
            _literis(a, ceterum);
            columna = (i32)strlen(indentatio) + (i32)strlen(ceterum);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, ' ');
            columna = columna + I;
        }
        verbum.datum    = textus.datum + initium;
        verbum.mensura  = mensura;
        _chordam(a, verbum);
        columna = columna + mensura;
    }
    si (claudere)
    {
        _literis(a, " */");
    }
    _linea_nova(a);
}

/* commentarium capitis plagulae: nomen plagulae, linea vacua, prosa
 * ('Registrum generum COCTUM ... Fons ... ratio, Genera R, loci R.') */
interior b32
_caput_plagulae (
         ChordaAedificator* a,
                   Piscina* piscina,
    constans MateriaCoctio* coctio,
        constans character* suffixum,
        constans character* via_declarationis,
        constans character* ratio)
{
    ChordaAedificator* p = chorda_aedificator_creare(piscina,
        (memoriae_index)CCLVI);

    si (p == NIHIL)
    {
        redde FALSUM;
    }
    _literis(a, "/* ");
    _chordam(a, coctio->grammatica);
    _literis(a, STIRPS);
    _literis(a, suffixum);
    _linea_nova(a);
    _literis(a, " *");
    _linea_nova(a);
    _literis(p, "Registrum generum COCTUM grammaticae '");
    _chordam(p, coctio->grammatica);
    _literis(p, "' - GENERATUM, NE MANU EDITES. Fons: ");
    _literis(p, via_declarationis);
    _literis(p, " (materia/coquere.sh). ");
    _literis(p, ratio);
    _literis(p, "Genera ");
    _romanum(p, coctio->numerus_generum);
    _literis(p, ", loci ");
    _romanum(p, coctio->numerus_locorum);
    _literis(p, ".");
    _involvere(a, "", " * ", " * ", chorda_aedificator_finire(p),
        FALSUM);
    _literis(a, " */");
    _linea_nova(a);
    _linea_nova(a);
    redde VERUM;
}

interior chorda
_caput_reddere (
                   Piscina* piscina,
    constans MateriaCoctio* coctio,
              constans Xar* genera,
        constans character* via_declarationis)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)MMMM);
    i32 k;

    si (a == NIHIL)
    {
        redde _vacua();
    }
    si (!_caput_plagulae(a, piscina, coctio, ".h", via_declarationis,
            ""))
    {
        redde _vacua();
    }
    _literis(a, "#ifndef ");
    _chordam(a, coctio->praefixum);
    _literis(a, "_REGISTRUM_COCTUM_H");
    _linea_nova(a);
    _literis(a, "#define ");
    _chordam(a, coctio->praefixum);
    _literis(a, "_REGISTRUM_COCTUM_H");
    _linea_nova(a);
    _linea_nova(a);
    _literis(a, "#include \"latina.h\"");
    _linea_nova(a);
    _literis(a, "#include \"materia_registrum.h\"");
    _linea_nova(a);
    _linea_nova(a);
    _literis(a, "nomen enumeratio {");
    _linea_nova(a);
    per (k = ZEPHYRUM; k < xar_numerus(genera); k++)
    {
        constans Genus* g = (constans Genus*)xar_obtinere(
            genera, k);

        si (g->nota.mensura > ZEPHYRUM)
        {
            _involvere(a, "    ", "/* ", " * ", g->nota, VERUM);
        }
        _literis(a, "    ");
        _chordam(a, coctio->praefixum);
        _literis(a, "_GENUS_");
        _maiusculum(a, g->titulus);
        si (k == ZEPHYRUM)
        {
            _literis(a, " = 0");
        }
        _literis(a, ",");
        _linea_nova(a);
    }
    _linea_nova(a);
    _literis(a, "    ");
    _chordam(a, coctio->praefixum);
    _literis(a, "_GENUS_NUMERUS_GENERUM");
    _linea_nova(a);
    _literis(a, "} ");
    _chordam(a, coctio->typus);
    _literis(a, ";");
    _linea_nova(a);
    _linea_nova(a);
    _literis(a, "externus constans MateriaRegistrumCoctum ");
    _chordam(a, coctio->praefixum);
    _literis(a, "_REGISTRUM;");
    _linea_nova(a);
    _linea_nova(a);
    _literis(a, "#endif /* ");
    _chordam(a, coctio->praefixum);
    _literis(a, "_REGISTRUM_COCTUM_H */");
    _linea_nova(a);
    redde chorda_aedificator_finire(a);
}

interior chorda
_fontem_reddere (
                   Piscina* piscina,
    constans MateriaCoctio* coctio,
              constans Xar* genera,
              constans Xar* loci,
        constans character* via_declarationis)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)MMMM * (memoriae_index)IV);
    i32 k;
    i32 j;

    si (a == NIHIL)
    {
        redde _vacua();
    }
    si (!_caput_plagulae(a, piscina, coctio, ".c", via_declarationis,
            "Series LOCORUM plana; quodque genus fenestram suam per "
            "loci_offset + loci_numerus nominat. "))
    {
        redde _vacua();
    }
    _literis(a, "#include \"");
    _chordam(a, coctio->grammatica);
    _literis(a, STIRPS);
    _literis(a, ".h\"");
    _linea_nova(a);
    _literis(a, "#include \"materia_nodus.h\"");
    _linea_nova(a);
    _linea_nova(a);
    _literis(a, "hic_manens constans MateriaTabLocus LOCI_COCTI[] = {");
    _linea_nova(a);
    per (k = ZEPHYRUM; k < xar_numerus(genera); k++)
    {
        constans Genus* g = (constans Genus*)xar_obtinere(
            genera, k);

        si (k > ZEPHYRUM)
        {
            _linea_nova(a);
        }
        _literis(a, "    /* ");
        _chordam(a, g->titulus);
        si (g->numerus == ZEPHYRUM)
        {
            _literis(a, " (loci nulli) */");
        }
        alioquin
        {
            _literis(a, " (");
            chorda_aedificator_appendere_i32(a, g->offset);
            _literis(a, "..");
            chorda_aedificator_appendere_i32(a, g->offset + g->numerus
                - I);
            _literis(a, ") */");
        }
        _linea_nova(a);
        per (j = g->offset; j < g->offset + g->numerus; j++)
        {
            constans Locus* l = (constans Locus*)xar_obtinere(
                loci, j);

            si (l == NIHIL)
            {
                perge;   /* extra seriem: numquam per constructionem */
            }
            si (l->nota.mensura > ZEPHYRUM)
            {
                _involvere(a, "    ", "/* ", " * ", l->nota, VERUM);
            }
            _literis(a, "    { \"");
            _chordam(a, l->titulus);
            _literis(a, "\", (s32)MATERIA_LOCUS_");
            _maiusculum(a, chorda_ex_literis(
                materia_locus_species_titulus(l->species), piscina));
            _literis(a, " },");
            _linea_nova(a);
        }
    }
    _literis(a, "};");
    _linea_nova(a);
    _linea_nova(a);
    _literis(a,
        "hic_manens constans MateriaTabGenus GENERA_COCTA[] = {");
    _linea_nova(a);
    _literis(a, "    /* titulus, offset, numerus */");
    _linea_nova(a);
    per (k = ZEPHYRUM; k < xar_numerus(genera); k++)
    {
        constans Genus* g = (constans Genus*)xar_obtinere(
            genera, k);

        _literis(a, "    { \"");
        _chordam(a, g->titulus);
        _literis(a, "\", (i32)");
        chorda_aedificator_appendere_i32(a, g->offset);
        _literis(a, ", (i32)");
        chorda_aedificator_appendere_i32(a, g->numerus);
        _literis(a, " },");
        _linea_nova(a);
    }
    _literis(a, "};");
    _linea_nova(a);
    _linea_nova(a);
    _literis(a, "constans MateriaRegistrumCoctum ");
    _chordam(a, coctio->praefixum);
    _literis(a, "_REGISTRUM = {");
    _linea_nova(a);
    _literis(a, "    GENERA_COCTA,");
    _linea_nova(a);
    _literis(a, "    (i32)(magnitudo(GENERA_COCTA) / "
        "magnitudo(GENERA_COCTA[0])),");
    _linea_nova(a);
    _literis(a, "    LOCI_COCTI,");
    _linea_nova(a);
    _literis(a, "    (i32)(magnitudo(LOCI_COCTI) / "
        "magnitudo(LOCI_COCTI[0]))");
    _linea_nova(a);
    _literis(a, "};");
    _linea_nova(a);
    redde chorda_aedificator_finire(a);
}

/* sedes + '/' + grammatica + stirps + suffixum */
interior chorda
_via (
                   Piscina* piscina,
    constans MateriaCoctio* coctio,
        constans character* suffixum)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)CXXVIII);

    si (a == NIHIL)
    {
        redde _vacua();
    }
    _chordam(a, coctio->sedes);
    _literis(a, "/");
    _chordam(a, coctio->grammatica);
    _literis(a, STIRPS);
    _literis(a, suffixum);
    redde chorda_aedificator_finire(a);
}

b32
materia_registrum_coquere (
                Piscina* piscina,
                 chorda  declaratio,
     constans character* via_declarationis,
          MateriaCoctio* coctio)
{
    Xar* genera;
    Xar* loci;

    memset(coctio, ZEPHYRUM, magnitudo(*coctio));
    genera  = xar_creare(piscina, (i32)magnitudo(Genus));
    loci    = xar_creare(piscina, (i32)magnitudo(Locus));
    si (genera == NIHIL || loci == NIHIL)
    {
        redde FALSUM;
    }
    si (!_declarationem_legere(piscina, declaratio, coctio, genera,
            loci))
    {
        redde FALSUM;
    }
    coctio->caput        = _caput_reddere(piscina, coctio, genera,
        via_declarationis);
    coctio->fons         = _fontem_reddere(piscina, coctio, genera,
        loci, via_declarationis);
    coctio->via_capitis  = _via(piscina, coctio, ".h");
    coctio->via_fontis   = _via(piscina, coctio, ".c");
    redde (b32)(   coctio->caput.mensura > ZEPHYRUM
                && coctio->fons.mensura > ZEPHYRUM
                && coctio->via_capitis.mensura > ZEPHYRUM
                && coctio->via_fontis.mensura > ZEPHYRUM);
}
