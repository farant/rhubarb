/* oratio_partes.c - Vide oratio_partes.h. */

#include "oratio_partes.h"
#include "oratio_partes_en.h"
#include "oratio_lexicon.h"
#include "oratio_lexema.h"
#include "materia_arbor.h"
#include "tabula_dispersa.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_chorda (
     i8* datum,
    i32  mensura)
{
    chorda c;

    c.datum    = datum;
    c.mensura  = mensura;
    redde c;
}

interior chorda
_copia (
    Piscina* piscina,
     chorda  c)
{
    chorda d;

    d.datum = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura
        + I);
    si (d.datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(d.datum, c.datum, (size_t)c.mensura);
    }
    d.datum[c.mensura]  = ZEPHYRUM;
    d.mensura           = c.mensura;
    redde d;
}

s32
oratio_partes_locus (
           OratioClassis  classis,
      constans character* titulus)
{
    constans MateriaTabGenus* genus;
                         i32  j;

    si ((i32)classis >= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde (s32)-I;
    }
    genus = &ORATIO_REGISTRUM.genera[oratio_classis_genus(classis)];
    per (j = ZEPHYRUM; j < genus->loci_numerus; j++)
    {
        si (strcmp(ORATIO_REGISTRUM.loci[genus->loci_offset
            + j].titulus,
                titulus) == ZEPHYRUM)
        {
            redde (s32)j;
        }
    }
    redde (s32)-I;
}

chorda
oratio_partes_textus_vocabuli (
                  Piscina* piscina,
    constans MateriaNodus* vocabulum)
{
    constans MateriaValor* partes =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
                      i32 n;
                      i32 k;
                      i32 mensura = ZEPHYRUM;
                   chorda c;

    si (partes->genus != MATERIA_VALOR_LISTA)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    n = materia_valor_lista_numerus(*partes);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*partes,
            k);

        si (e != NIHIL && e->genus == MATERIA_VALOR_TOKEN)
        {
            mensura = mensura + e->datum.token->valor.mensura;
        }
    }
    c.datum = (i8*)piscina_allocare(piscina, (memoriae_index)mensura
        + I);
    si (c.datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    c.mensura = ZEPHYRUM;
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*partes,
            k);

        si (e != NIHIL && e->genus == MATERIA_VALOR_TOKEN)
        {
            memcpy(c.datum + c.mensura, e->datum.token->valor.datum,
                (size_t)e->datum.token->valor.mensura);
            c.mensura = c.mensura + e->datum.token->valor.mensura;
        }
    }
    c.datum[c.mensura] = ZEPHYRUM;
    redde c;
}

/* lexema derivatum in locum TOKEN ponere (valor copiatur) */
interior b32
_derivatum_ponere (
                  Piscina* piscina,
             MateriaNodus* nodus,
                      i32  locus,
                   chorda  valor,
    constans MateriaToken* origo)
{
    MateriaToken* t;

    si (valor.mensura == ZEPHYRUM)
    {
        redde VERUM;   /* absens */
    }
    t = oratio_lexema_derivatum(piscina, (s32)ORATIO_LEX_DERIVATUM,
        _copia(piscina, valor), origo);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_ponere(nodus, locus, materia_valor_token(t),
        MATERIA_LOCUS_TOKEN);
}

interior b32
_accidens_ponere (
          MateriaNodus* nodus,
         OratioClassis  classis,
    constans character* titulus,
                   s32  valor)
{
    s32 locus;

    si (valor < ZEPHYRUM)
    {
        redde VERUM;   /* non datum */
    }
    locus = oratio_partes_locus(classis, titulus);
    si (locus < ZEPHYRUM)
    {
        redde VERUM;   /* genus accidens non fert: omittitur */
    }
    redde materia_nodus_ponere(nodus, (i32)locus,
        materia_valor_index(valor),
        MATERIA_LOCUS_INDEX);
}

/* UMBRAE (T19d, 2026-09-07): dependens exspectatus lectionis, e
 * codicibus fontis IPSIUS - adpositio cum casu: obiectum eo casu
 * (verbum transitivum: obiectum accusativum; adiectivum/determinans:
 * caput concordans - gradu proximo). Nodus generis umbra in lista
 * 'umbrae' analysis (locus per titulum); condiciones INDEX scriptae
 * solum si datae; impletio vacua nascitur (inventum donec impleta -
 * resolutio gradus V per reponere). Lista absens non scribitur. */
interior b32
_umbram_addere (
          Piscina* piscina,
     MateriaNodus* analysis,
    OratioClassis  classis,
    OratioRelatio  relatio,
              s32  classis_umbrae,
              s32  casus_umbrae,
              s32  numerus_umbrae,
              s32  genus_umbrae)
{
    s32 locus = oratio_partes_locus(classis, "umbrae");
    MateriaNodus* umbra;

    si (locus < ZEPHYRUM)
    {
        redde VERUM;   /* genus umbras non fert */
    }
    umbra = materia_nodus_creare(piscina, (s32)ORATIO_GENUS_UMBRA,
        ORATIO_REGISTRUM.genera[ORATIO_GENUS_UMBRA].loci_numerus);
    si (   umbra == NIHIL
        || !materia_nodus_ponere(umbra, (i32)ORATIO_UMBRA_RELATIO,
            materia_valor_index((s32)relatio), MATERIA_LOCUS_INDEX))
    {
        redde FALSUM;
    }
    si (   classis_umbrae >= ZEPHYRUM
        && !materia_nodus_ponere(umbra, (i32)ORATIO_UMBRA_CLASSIS,
            materia_valor_index(classis_umbrae), MATERIA_LOCUS_INDEX))
    {
        redde FALSUM;
    }
    si (   casus_umbrae >= ZEPHYRUM
        && !materia_nodus_ponere(umbra, (i32)ORATIO_UMBRA_CASUS,
            materia_valor_index(casus_umbrae), MATERIA_LOCUS_INDEX))
    {
        redde FALSUM;
    }
    si (   numerus_umbrae >= ZEPHYRUM
        && !materia_nodus_ponere(umbra, (i32)ORATIO_UMBRA_NUMERUS,
            materia_valor_index(numerus_umbrae), MATERIA_LOCUS_INDEX))
    {
        redde FALSUM;
    }
    si (   genus_umbrae >= ZEPHYRUM
        && !materia_nodus_ponere(umbra, (i32)ORATIO_UMBRA_GENUS,
            materia_valor_index(genus_umbrae), MATERIA_LOCUS_INDEX))
    {
        redde FALSUM;
    }
    redde materia_nodus_appendere(piscina, analysis, (i32)locus,
        materia_valor_nodus(umbra), MATERIA_LOCUS_LISTA_NODUS);
}

/* T31 a: species stirpis WORDS 'IMPERS' (verbum impersonale) */
interior b32
_natura_impersonalis (
    chorda natura)
{
    i32 i;

    per (i = ZEPHYRUM; i + (i32)VI <= natura.mensura; i++)
    {
        si (memcmp(natura.datum + i, "IMPERS", (size_t)VI) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}
interior i32
_raritas (
    constans OratioDescriptio* d);
/* T31 b: species stirpis WORDS (lexema primum) = titulus? */
interior b32
_natura_est (
                chorda  natura,
    constans character* titulus)
{
    i32 l = (i32)strlen(titulus);

    redde (b32)(   natura.mensura >= l
        && memcmp(natura.datum, titulus, (size_t)l) == ZEPHYRUM
        && (natura.mensura == l || natura.datum[l] == ' '));
}

/* T31 b: verba speciei ignotae ('X': 2751 entria, mitto inter ea)
 * obiectum accipiunt; ORATIO_OBIECTUM_X=0 abrogat (mensura) */
interior b32
_obiectum_speciei_ignotae (
    vacuum)
{
    hic_manens i32 lectum   = ZEPHYRUM;
    hic_manens b32 activum  = VERUM;

    si (!lectum)
    {
        constans character* ambitus = getenv("ORATIO_OBIECTUM_X");

        activum = (b32)(ambitus == NIHIL
            || strcmp(ambitus, "0") != ZEPHYRUM);
        lectum = I;
    }
    redde activum;
}

interior b32
_umbras_ponere (
                      Piscina* piscina,
                 MateriaNodus* analysis,
    constans OratioDescriptio* d)
{
    /* ANGLICA (T19e, 2026-09-07): lectio Anglica casu caret - umbrae
     * per CLASSEM: 'to' particula et have/do auxiliaria (listae
     * ambiguae) obiectum verbum (to go, have been) */
    si (d->lingua == ORATIO_LINGUA_ANGLICA)
    {
        /* determinans caput substantivum MENSURATUM et ablatum
         * (2026-09-07): regula 'the + substantivum' lectiones nominales
         * Moby 'is'/'it's' post that/this/it praeferebat (auxiliare
         * -IX), substantiva store/care non attingebat (adiectivum
         * interiacet) */
        si (   (   d->classis == ORATIO_CLASSIS_PARTICULA
                && chorda_aequalis_literis(d->nativum,
                    "particulae-ambiguae"))
            || (   d->classis == ORATIO_CLASSIS_AUXILIARE
                && chorda_aequalis_literis(d->nativum,
                    "auxiliaria-ambigua")))
        {
            redde _umbram_addere(piscina, analysis, d->classis,
                ORATIO_RELATIO_OBIECTUM, (s32)ORATIO_CLASSIS_VERBUM,
                (s32)-I, (s32)-I, (s32)-I);
        }
        redde VERUM;
    }
        si (   d->classis           == ORATIO_CLASSIS_ADPOSITIO
            && d->casus_grammaticus >= ZEPHYRUM)
        {
        redde _umbram_addere(piscina, analysis, d->classis,
            ORATIO_RELATIO_OBIECTUM, (s32)-I, d->casus_grammaticus,
            (s32)-I, (s32)-I);
        }
    /* CAPUT (T19d beta): adiectivum et determinans caput substantivum
     * exspectant quod casu numero genere concordat - condiciones ex
     * lectione IPSA (ADJ 1 1 NOM S F -> umbra capitis NOM S F) */
    /* SUBIECTUM (T31 a, 2026-09-09, stella verbi): verbum finitum personae
     * tertiae subiectum NOMINATIVUM numero concordans exspectat; verba
     * impersonalia (WORDS IMPERS) non; personae I et II (subiecta
     * pronomina, plerumque omissa) nondum. Census auri thesaurorum VII:
     * verba finita cum subiecto explicito 40-69 % (cetera vacua =
     * INVENTUM), subiectum nominativum 77-89 % (chartae 55-61 %,
     * accusativum 32 % - acc. cum inf.), ante verbum 63-88 %, intra III
     * verba 65-74 % (chartae 30-37 %), numerus concordat 92-98 %
     * (discordia = coordinatio), nominativus proximus concordans
     * subiectum 69-81 % (chartae 45-52 %). */
    si (   d->classis     == ORATIO_CLASSIS_VERBUM
        && d->forma_verbi == (s32)ORATIO_FORMA_VERBI_FINITUM
        && !_natura_impersonalis(d->natura)
        && _raritas(d)    == ZEPHYRUM)
    {
        si (   d->persona == (s32)ORATIO_PERSONA_TERTIA
            && d->numerus >= ZEPHYRUM
            && !_umbram_addere(piscina, analysis, d->classis,
                ORATIO_RELATIO_SUBIECTUM, (s32)-I,
                (s32)ORATIO_CASUS_NOMINATIVUS, d->numerus, (s32)-I))
        {
            redde FALSUM;
        }
        /* OBIECTUM VERBI (T31 b, 2026-09-09): lectio finita activa aut
         * deponens verbi transitivi (WORDS TRANS, DEP, SEMIDEP; species
         * ignota X quoque - mitto) accusativum exspectat; passivae non
         * (obiectum subiectum factum), INTRANS IMPERS DAT GEN ABL non.
         * Census auri: verba finita activa cum obiecto explicito 33-54 %,
         * passiva 0-8 % (Seneca 25 % = deponentia Voice=Pass); obiectum
         * accusativum 89-97 %, substantivum 49-77 % pronomen 11-37 %;
         * ante verbum 54-79 %, vicinum 36-59 %, intra III 65-92 %;
         * accusativus proximus obiectum 68-79 % (chartae 52-54 %), sine
         * obiectis adpositionum 69-82 % (chartae 58-65 %). */
        si (   (   d->vox == (s32)ORATIO_VOX_ACTIVA
                || d->vox == (s32)ORATIO_VOX_DEPONENS)
            && (   _natura_est(d->natura, "TRANS")
                || _natura_est(d->natura, "DEP")
                || _natura_est(d->natura, "SEMIDEP")
                || (   _natura_est(d->natura, "X")
                    && _obiectum_speciei_ignotae()))
            && !_umbram_addere(piscina, analysis, d->classis,
                ORATIO_RELATIO_OBIECTUM_VERBI, (s32)-I,
                (s32)ORATIO_CASUS_ACCUSATIVUS, (s32)-I, (s32)-I))
        {
            redde FALSUM;
        }
        redde VERUM;
    }
    si (   (   d->classis == ORATIO_CLASSIS_ADIECTIVUM
            || d->classis == ORATIO_CLASSIS_DETERMINANS)
        && d->casus_grammaticus >= ZEPHYRUM)
    {
        redde _umbram_addere(piscina, analysis, d->classis,
            ORATIO_RELATIO_CAPUT, (s32)-I, d->casus_grammaticus,
            d->numerus, d->genus);
    }
    redde VERUM;
}

MateriaNodus*
oratio_partes_nodum_struere (
                      Piscina* piscina,
    constans OratioDescriptio* d,
        constans MateriaToken* origo)
{

    OratioGenus genus = oratio_classis_genus(d->classis);
    MateriaNodus* nodus;

    si ((i32)d->classis >= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde NIHIL;
    }
    nodus = materia_nodus_creare(piscina, (s32)genus,
        ORATIO_REGISTRUM.genera[genus].loci_numerus);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    si (   !_derivatum_ponere(piscina, nodus,
        (i32)ORATIO_ANALYSIS_LEMMA,
            d->lemma, origo)
        || (   !d->lingua_ignota
            && !materia_nodus_ponere(nodus, (i32)ORATIO_ANALYSIS_LINGUA,
                materia_valor_index((s32)d->lingua),
                MATERIA_LOCUS_INDEX))

        || !materia_nodus_ponere(nodus, (i32)ORATIO_ANALYSIS_FONS,
            materia_valor_index((s32)d->fons), MATERIA_LOCUS_INDEX)
        || !_derivatum_ponere(piscina, nodus,
        (i32)ORATIO_ANALYSIS_NATIVUM,
            d->nativum, origo)
        || !_derivatum_ponere(piscina, nodus,
        (i32)ORATIO_ANALYSIS_SENSUS,
            d->sensus, origo))
    {
        redde NIHIL;
    }
    si (   !_accidens_ponere(nodus, d->classis, "casus",
        d->casus_grammaticus)
        || !_accidens_ponere(nodus, d->classis, "numerus", d->numerus)
        || !_accidens_ponere(nodus, d->classis, "genus", d->genus)
        || !_accidens_ponere(nodus, d->classis, "persona", d->persona)
        || !_accidens_ponere(nodus, d->classis, "tempus", d->tempus)
        || !_accidens_ponere(nodus, d->classis, "modus", d->modus)
        || !_accidens_ponere(nodus, d->classis, "vox", d->vox)
        || !_accidens_ponere(nodus, d->classis, "forma-verbi",
            d->forma_verbi)
        || !_accidens_ponere(nodus, d->classis, "gradus", d->gradus)
        || !_accidens_ponere(nodus, d->classis, "species", d->species)
        || !_accidens_ponere(nodus, d->classis, "declinatio",
            d->declinatio > ZEPHYRUM ? d->declinatio : (s32)-I)
        || !_accidens_ponere(nodus, d->classis, "coniugatio",
            d->coniugatio > ZEPHYRUM ? d->coniugatio : (s32)-I))
    {
        redde NIHIL;
    }
    si (!_umbras_ponere(piscina, nodus, d))
    {
        redde NIHIL;
    }
    redde nodus;
}


/* T30 b ORDO FREQUENTIAE (2026-09-09): lectiones eiusdem CLASSIS et
 * LINGUAE inter sedes suas ordine codicis frequentiae stirpis WORDS
 * (A ante B ... F; I M N rarissimi; X ignotus medius; sine codice -
 * glossarium, unicum, regula - primae) STABILITER permutantur; ordo
 * classium et linguarum manet (classis prima eadem: primarium immotum;
 * Latinae ante Anglicas), copia lectionum eadem (nihil eliminatur).
 * Census T30 a: ordo lemmatum homonymorum (armus ante arma, caelus
 * ante caelum, tela ante telum, census ante censum) quinta pars
 * falsorum generis Senecae attingibilium. Regulae par congruens PRIMUM
 * ligant, ergo ordo ANTE regulas: evidentia casus/numeri/generis
 * eadem, lemma frequentius primum (contra priorem casuum T24, qui
 * casus permutat et ante regulas primarium laedebat). Ambitus
 * ORATIO_ORDO_FREQUENTIAE=0 abrogat (mensura ante/post sine
 * recoctione). */
hic_manens constans b32 ORDO_FREQUENTIAE = VERUM;

interior b32
_ordo_frequentiae_activus (
    vacuum)
{
    hic_manens i32 lectum   = ZEPHYRUM;
    hic_manens b32 activus  = VERUM;

    si (!lectum)
    {
        constans character* ambitus = getenv("ORATIO_ORDO_FREQUENTIAE");

        activus = (b32)(ORDO_FREQUENTIAE
            && (ambitus == NIHIL || strcmp(ambitus, "0") != ZEPHYRUM));
        lectum  = I;
    }
    redde activus;
}

interior i32
_gradus_frequentiae (
    character codex)
{
    commutatio (codex)
    {
        casus 'A':
            redde ZEPHYRUM;
        casus 'B':
            redde I;
        casus 'C':
            redde (i32)II;
        casus 'D':
            redde (i32)III;
        casus 'E':
            redde (i32)IV;
        casus 'F':
            redde (i32)V;
        casus 'I':
        casus 'M':
        casus 'N':
            redde (i32)VI;
        casus ZEPHYRUM:
            redde ZEPHYRUM;   /* sine stirpe: locus suus manet inter primas */
        ordinarius:
            redde (i32)III;   /* X ignotus: medius */
    }
}

/* T30 d RARITAS FLEXIONUM (2026-09-09): clavis altera ordinis post
 * frequentiam stirpis - lectiones verbi flexionis RARAE post ceteras
 * eiusdem stirpis. Census thesaurorum VII (aurum): (I) gerundivum
 * (VPAR FUT PASSIVE) MASCULINUM: formae -ndum/-ndi/-ndo neutrae
 * 97 % (chartae 155/155, Aquinas 139/139, Dante 66/67, Perseus 10/16;
 * CIRCSE et PROIEL gerundium sine genere aut Gdv neutrum) - WORDS
 * ACC S M primum ponit, femininum NON demotum (-nda: feminina singularis
 * contra neutra pluralia Seneca 5/2, PROIEL 5/2, Dante 3/18 - ordo
 * dictionarii F prima in tribus ex quattuor rectus; 'non neutrum'
 * mensuratum numerum Senecae et PROIEL laedebat); (II) finitum passivum personae secundae
 * (-re, -ris: 'scribere' IND/IMP 2 S) ante infinitivum eiusdem formae:
 * infinitivi -re 338/342 chartae, 137/196 Seneca (finita -re Senecae
 * imperativi deponentes sine infinitivo homographo), 187/192 PROIEL -
 * DEPONENTIA excepta: 'sequere', 'loquere', 'morere' forma passiva
 * forma sua est, demota infinitivo spurio cedebant (Seneca forma verbi
 * -15, numerus et persona -25 populo);
 * (III) participium vocativo ('date' = datus VOC S M ante 'date'
 * imperativum): participia vocativa 6/496 Seneca, 0 alibi - MENSURATA: nihil mutat (inactiva). Tabula titulata, mensura: ORATIO_ORDO_RARITATIS=0 (nulla) aut =titulus
 * (una sola). */
nomen structura {
    constans character* titulus;
                   b32  activa;
} Raritas;

hic_manens constans Raritas RARITATES[] = {
    { "gerundivum-masculinum", VERUM },
    { "passivum-secundae", VERUM },
    { "participium-vocativus", FALSUM },
    /* T31 a: (IV) flexio ipsa rarior (codex frequentiae flexionis B..F:
     * perfectum syncopatum -ere tertiae pluralis 'residere' B contra
     * -erunt A) et (V) imperativum futurum ('negato', 'expleto') -
     * lectiones finitae quas umbra subiecti promovebat super
     * infinitivum aut participium (forma verbi Senecae 978 -> 977,
     * chartae vox 999 -> 998); subiectum lectioni rarae non datur */
    { "flexio-rarior", VERUM },
    { "imperativum-futurum", VERUM }
};

interior b32
_raritas_activa (
    i32 r)
{
                   hic_manens i32  lectum   = ZEPHYRUM;
    hic_manens constans character* ambitus  = NIHIL;

    si (!lectum)
    {
        ambitus  = getenv("ORATIO_ORDO_RARITATIS");
        lectum   = I;
    }
    si (!RARITATES[r].activa)
    {
        redde FALSUM;
    }
    si (ambitus != NIHIL)
    {
        redde (b32)(strcmp(ambitus, RARITATES[r].titulus) == ZEPHYRUM);
    }
    redde VERUM;
}

/* gradus raritatis descriptionis: numerus raritatum activarum quas
 * lectio fert (verba sola) */
interior i32
_raritas (
    constans OratioDescriptio* d)
{
    i32 r = ZEPHYRUM;

    si (d->classis != ORATIO_CLASSIS_VERBUM)
    {
        redde ZEPHYRUM;
    }
    si (   _raritas_activa(ZEPHYRUM)
        && d->forma_verbi == (s32)ORATIO_FORMA_VERBI_GERUNDIVUM
        && d->genus       == (s32)ORATIO_GENUS_GRAMMATICUM_MASCULINUM)
    {
        r = r + I;
    }
    si (   _raritas_activa(I)
        && d->forma_verbi == (s32)ORATIO_FORMA_VERBI_FINITUM
        && d->persona     == (s32)ORATIO_PERSONA_SECUNDA
        && d->vox         == (s32)ORATIO_VOX_PASSIVA)
    {
        r = r + I;
    }
    si (   _raritas_activa((i32)II)
        && d->forma_verbi       == (s32)ORATIO_FORMA_VERBI_PARTICIPIUM
        && d->casus_grammaticus == (s32)ORATIO_CASUS_VOCATIVUS)
    {
        r = r + I;
    }
    si (   _raritas_activa((i32)III)
        && d->frequentia_flexionis != ZEPHYRUM
        && d->frequentia_flexionis != 'X'
        && _gradus_frequentiae(d->frequentia_flexionis) > ZEPHYRUM)
    {
        r = r + I;
    }
    si (   _raritas_activa((i32)IV)
        && d->forma_verbi == (s32)ORATIO_FORMA_VERBI_FINITUM
        && d->modus       == (s32)ORATIO_MODUS_IMPERATIVUS
        && d->tempus      == (s32)ORATIO_TEMPUS_FUTURUM)
    {
        r = r + I;
    }
    redde r;
}

/* gradus ordinis lectionis: frequentia stirpis maior, raritas flexionis
 * minor */
interior i32
_gradus_lectionis (
    constans OratioDescriptio* d)
{
    redde _gradus_frequentiae(d->frequentia) * (i32)VIII + _raritas(d);
}

/* descriptiones intra greges (classis, lingua) per gradum lectionis
 * (frequentia stirpis, raritas flexionis) stabiliter permutare; FALSUM =
 * memoria */
interior b32
_descriptiones_frequentia_ordinare (
    Piscina* scratch,
        Xar* descriptiones)
{
                 i32  n = xar_numerus(descriptiones);
    OratioDescriptio* copia;
                 i32* sedes;
                 i32* ordo;
                 b32* visa;
                 i32  i;

    si (n < (i32)II || !_ordo_frequentiae_activus())
    {
        redde VERUM;   /* raritas quoque sub ordine frequentiae */
    }
    copia = (OratioDescriptio*)piscina_allocare(scratch,
        (memoriae_index)n * (memoriae_index)magnitudo(OratioDescriptio));
    sedes = (i32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(i32));
    ordo  = (i32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(i32));
    visa  = (b32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(b32));
    si (   copia == NIHIL || sedes == NIHIL || ordo == NIHIL
        || visa  == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        copia[i] =
            *(constans OratioDescriptio*)xar_obtinere(descriptiones,
            i);
        visa[i]  = FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        i32 m = ZEPHYRUM;
        i32 j;
        i32 a;

        si (visa[i])
        {
            perge;
        }
        /* grex: sedes lectionum classis et linguae eiusdem, ordine */
        per (j = i; j < n; j++)
        {
            si (   !visa[j]
                && copia[j].classis       == copia[i].classis
                && copia[j].lingua        == copia[i].lingua
                && copia[j].lingua_ignota == copia[i].lingua_ignota
                && (copia[j].fons == ORATIO_FONS_ANALYSIS_ORTHOGRAPHIA)
                    == (copia[i].fons
                        == ORATIO_FONS_ANALYSIS_ORTHOGRAPHIA))
            {
                sedes[m]  = j;
                ordo[m]   = j;
                visa[j]   = VERUM;
                m         = m + I;
            }
        }
        /* insertio stabilis per gradum */
        per (a = I; a < m; a++)
        {
            i32 k = ordo[a];
            i32 b = a;

            dum (   b > ZEPHYRUM
                 && _gradus_lectionis(&copia[ordo[b - I]])
                     > _gradus_lectionis(&copia[k]))
            {
                ordo[b]  = ordo[b - I];
                b        = b - I;
            }
            ordo[b] = k;
        }
        per (a = ZEPHYRUM; a < m; a++)
        {
            *(OratioDescriptio*)xar_obtinere(descriptiones, sedes[a]) =
                copia[ordo[a]];
        }
    }
    redde VERUM;
}

/* T22: formae variantes formis CAPITALIBUS quoque? MENSURATUM (quinque
 * correspondentiae activae): cum capitalibus Seneca 9607, chartae
 * 20884/20617; sine 9610 (Erebo Pelei Polybo nomina manent),
 * 20885/20620, EWT +II/+I - capitalia ignota regulae capitalis manent */
hic_manens constans b32 ORTHOGRAPHIA_CAPITALIA = FALSUM;
/* interruptor totius contractus IV (mensura: basis sine variantibus) */
hic_manens constans b32 ORTHOGRAPHIA_ACTIVA = VERUM;
/* T22 b (T30 c, 2026-09-09): formae NOTAE quoque variant - lectiones
 * variantes post nativas APPENSAE (fons orthographia; ordo frequentiae
 * eas post nativas tenet, grex proprius): sancte = sanctae, bone =
 * bonae, suprascripte = suprascriptae, ipse = ipsae. Census generis
 * T30 a: formae mediae notae, quarum vocativus variantem -ae obstruebat,
 * quinta pars falsorum absentium chartarum. Custodia capitalium eadem;
 * custodia Moby NON adhibetur (forma iam Latina nota: bone, male, pure
 * Anglica quoque sunt). ORATIO_ORTHOGRAPHIA_NOTAE=0 abrogat (mensura). */
hic_manens constans b32 ORTHOGRAPHIA_NOTAE = VERUM;
/* T22 b: lectio varians formae NOTAE cuius pars orationis (WORDS,
 * stirpis) inter partes lectionum nativarum stirpium NON est omittitur
 * - 'que' (coniunctio) per 'quae' pronomen/determinans fiebat et
 * substantivum proximum utrimque ligabat (ecclesie pluralis XXXV,
 * numerus chartarum 928 -> 924); forma nota classem suam servat,
 * variantes accidentia addunt. ORATIO_ORTHOGRAPHIA_NOTAE_CLASSIS=0
 * abrogat (mensura). */
hic_manens constans b32 ORTHOGRAPHIA_NOTAE_CLASSIS = VERUM;

interior b32
_orthographia_notae_classis_activa (
    vacuum)
{
    hic_manens i32 lectum = ZEPHYRUM;
    hic_manens b32 activa = VERUM;

    si (!lectum)
    {
        constans character* ambitus =
            getenv("ORATIO_ORTHOGRAPHIA_NOTAE_CLASSIS");

        activa = (b32)(ORTHOGRAPHIA_NOTAE_CLASSIS
            && (ambitus == NIHIL || strcmp(ambitus, "0") != ZEPHYRUM));
        lectum = I;
    }
    redde activa;
}

/* pars orationis (WORDS) stirpis analysis; -I sine stirpe */
interior s32
_pars_analysis (
    constans OratioVocabulariumLa* voc,
          constans OratioAnalysis* a)
{
    constans OratioStirps* stirps;

    si (a->genus != ORATIO_ANALYSIS_STIRPS || a->stirps < ZEPHYRUM)
    {
        redde (s32)-I;
    }
    stirps = oratio_vocabularium_la_stirps(voc, a->stirps);
    redde stirps != NIHIL ? (s32)stirps->pars : (s32)-I;
}

interior b32
_orthographia_notae_activa (
    vacuum)
{
    hic_manens i32 lectum = ZEPHYRUM;
    hic_manens b32 activa = VERUM;

    si (!lectum)
    {
        constans character* ambitus =
            getenv("ORATIO_ORTHOGRAPHIA_NOTAE");

        activa = (b32)(ORTHOGRAPHIA_NOTAE
            && (ambitus == NIHIL || strcmp(ambitus, "0") != ZEPHYRUM));
        lectum = I;
    }
    redde activa;
}

b32
oratio_partes_vocabulum_annotare (
                           Piscina* piscina,
                           Piscina* scratch,
        constans OratioVocabularia* vocabularia,
                      MateriaNodus* vocabulum,
                OratioPartesCensus* census,
                   OratioDialectus  dialectus)
{
    constans MateriaValor* partes =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
    constans MateriaToken* origo;
                   chorda  textus;
                      Xar* analyses;
                      Xar* descriptiones;
                      b32  forma_nota;   /* T22 b: forma ipsa vocabulario Latino nota */
                      i32  k;
                      b32  classis_visa[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
            OratioClassis  ordo_classium[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
                      i32  numerus_classium = ZEPHYRUM;
                      b32  lingua_visa[ORATIO_LINGUA_NUMERUS];
                      b32  ignotum_additum = FALSUM;   /* T19k */
             OratioLingua  ordo_linguarum[ORATIO_LINGUA_NUMERUS];
                      i32  numerus_linguarum = ZEPHYRUM;

    si (census != NIHIL)
    {
        census->vocabula = census->vocabula + I;
    }
    si (   vocabulum->loci[ORATIO_VOCABULUM_CLASSES].genus
            != MATERIA_VALOR_NIHIL
        || partes->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*partes) == ZEPHYRUM)
    {
        redde VERUM;   /* iam annotatum aut sine partibus */
    }
    origo = materia_valor_lista_obtinere(*partes,
        ZEPHYRUM)->datum.token;
    textus = oratio_partes_textus_vocabuli(scratch, vocabulum);
    si (textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    memset(classis_visa, ZEPHYRUM, magnitudo(classis_visa));
    memset(lingua_visa, ZEPHYRUM, magnitudo(lingua_visa));
        analyses = oratio_vocabularium_la_quaerere(scratch,
            vocabularia->la,
            textus);
    descriptiones = xar_creare(scratch,
        (i32)magnitudo(OratioDescriptio));
    si (analyses == NIHIL || descriptiones == NIHIL)
    {
        redde FALSUM;
    }
    forma_nota = (b32)(xar_numerus(analyses) > ZEPHYRUM);
    /* T22 CONTRACTUS IV: forma vocabulario Latino ignota et Moby ignota
     * (custodia Anglica ut T21) per formas variantes orthographiae mediae
     * quaeritur (e > ae, h addita ...); lectiones fontem 'orthographia'
     * ferunt. ORTHOGRAPHIA_CAPITALIA: formae capitales quoque (nomina
     * medii aevi: mensuratum) */
        si (   ORTHOGRAPHIA_ACTIVA && xar_numerus(analyses) == ZEPHYRUM
            && (   ORTHOGRAPHIA_CAPITALIA
            || !(   textus.datum[ZEPHYRUM] >= 'A'
                 && textus.datum[ZEPHYRUM] <= 'Z')))
        {
        b32 anglica_nota = FALSUM;

        si (vocabularia->en != NIHIL)
        {
            Xar* nota_en = oratio_vocabularium_en_analysare(scratch,
                vocabularia->en, textus);

            si (nota_en == NIHIL)
            {
                redde FALSUM;
            }
            anglica_nota = (b32)(xar_numerus(nota_en) > ZEPHYRUM);
        }
        si (!anglica_nota)
        {
            s32  varians;
            Xar* variantes = oratio_vocabularium_la_quaerere_variantes(
                scratch, vocabularia->la, textus, &varians);

            si (variantes == NIHIL)
            {
                redde FALSUM;
            }
            si (xar_numerus(variantes) > ZEPHYRUM)
            {
                analyses = variantes;
                si (census != NIHIL)
                {
                    census->orthographia = census->orthographia + I;
                }
            }
        }
        }
    /* T22 b (T30 c): forma NOTA - lectiones variantes post nativas
     * appensae (vide ORTHOGRAPHIA_NOTAE) */
    si (   ORTHOGRAPHIA_ACTIVA && _orthographia_notae_activa()
        && forma_nota && dialectus == ORATIO_DIALECTUS_MEDIUS
        && (   ORTHOGRAPHIA_CAPITALIA
            || !(   textus.datum[ZEPHYRUM] >= 'A'
                 && textus.datum[ZEPHYRUM] <= 'Z')))
    {
        s32  varians;
        Xar* variantes =
            oratio_vocabularium_la_quaerere_variantes_notis(scratch,
            vocabularia->la, textus, &varians);
        i32 j;

        si (variantes == NIHIL)
        {
            redde FALSUM;
        }
        {
            i32 nativae = xar_numerus(analyses);

            per (j = ZEPHYRUM; j < xar_numerus(variantes); j++)
            {
                constans OratioAnalysis* a =
                    (constans OratioAnalysis*)xar_obtinere(variantes,
                    j);
                OratioAnalysis* cella;

                si (_orthographia_notae_classis_activa())
                {
                    s32 pars = _pars_analysis(vocabularia->la, a);
                    i32 i;
                    b32 inter = FALSUM;

                    per (i = ZEPHYRUM; i < nativae && !inter; i++)
                    {
                        inter = (b32)(pars >= ZEPHYRUM
                            && _pars_analysis(vocabularia->la,
                                (constans OratioAnalysis*)xar_obtinere(
                                    analyses, i)) == pars);
                    }
                    si (!inter)
                    {
                        perge;
                    }
                }
                cella = (OratioAnalysis*)xar_addere(analyses);
                si (cella == NIHIL)
                {
                    redde FALSUM;
                }
                *cella = *a;
            }
        }
        si (xar_numerus(variantes) > ZEPHYRUM && census != NIHIL)
        {
            census->orthographia_notae = census->orthographia_notae + I;
        }
    }
        per (k = ZEPHYRUM; k < xar_numerus(analyses); k++)
        {
        si (!oratio_partes_la_describere(piscina, vocabularia->la,
                (constans OratioAnalysis*)xar_obtinere(analyses, k),
                descriptiones))
        {
            redde FALSUM;
        }
        }
    /* T16: lectiones Anglicae (Moby + regulae) post Latinas appensae */
    si (vocabularia->en != NIHIL)
    {
        Xar* analyses_en = oratio_vocabularium_en_analysare(scratch,
            vocabularia->en, textus);

        si (analyses_en == NIHIL)
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < xar_numerus(analyses_en); k++)
        {
            constans OratioAnalysisEn* a =
                (constans OratioAnalysisEn*)xar_obtinere(analyses_en,
                k);

            si (!oratio_partes_en_describere(piscina, vocabularia->en,
                a,
                    descriptiones))
            {
                redde FALSUM;
            }
        }
    }
    si (   vocabularia->en != NIHIL
        && !oratio_partes_en_secundariae(piscina, textus,
        descriptiones))
    {
        redde FALSUM;
    }
    /* regula (T13/T16): vocabulum capitale quod nullus fons Latinus
     * novit - ignotum (Karolus, Taenari) aut Moby soli notum ut
     * substantivum (Bush, Hercules) - nomen proprium candidatum fonte
     * 'regula'; lingua anglica si lectiones Anglicae adsunt */
    si (   textus.datum[ZEPHYRUM] >= 'A'
        && textus.datum[ZEPHYRUM] <= 'Z')
    {
        b32 fons_latinus  = FALSUM;
        b32 proprium      = FALSUM;
        b32 substantivum  = FALSUM;
        b32 anglicum      = FALSUM;
        b32 pronomen      = FALSUM;   /* T19e: lectio pronominis */

        per (k = ZEPHYRUM; k < xar_numerus(descriptiones); k++)
        {
            constans OratioDescriptio* d =
                (constans OratioDescriptio*)xar_obtinere(descriptiones,
                k);

            si (d->fons != ORATIO_FONS_ANALYSIS_VOCABULARIUM_EN)
            {
                fons_latinus = VERUM;
            }
            si (d->classis == ORATIO_CLASSIS_NOMEN_PROPRIUM)
            {
                proprium = VERUM;
            }
            si (d->classis == ORATIO_CLASSIS_SUBSTANTIVUM)
            {
                substantivum = VERUM;
            }
            si (d->lingua == ORATIO_LINGUA_ANGLICA)
            {
                anglicum = VERUM;
            }
            si (d->classis == ORATIO_CLASSIS_PRONOMEN)
            {
                pronomen = VERUM;   /* T19e: I'm, You'll, It's (Moby N +
                                     * contractio pronominis) numquam
                                     * nomen capitale; possessivum
                                     * nominis (Debra's) et Will/May
                                     * (auxiliare) manent */
            }
        }
        si (   !fons_latinus && !proprium
            && (   xar_numerus(descriptiones) == ZEPHYRUM
                || (substantivum && !pronomen)))
        {
            OratioDescriptio* d =
                (OratioDescriptio*)xar_addere(descriptiones);

            si (d == NIHIL)
            {
                redde FALSUM;
            }
                        oratio_partes_la_capitalis(piscina, textus, d);
            si (anglicum)
            {
                d->lingua = ORATIO_LINGUA_ANGLICA;
            }
        }
    }
    /* T19k: vocabulum quod nullus fons novit lectionem IGNOTI accipit
     * (cellula in tabula cum candidatis omnibus, decretum SUDOKU
     * XXXVII): ligari potest (umbrae casus cuiusvis), classis manet
     * 'ignotum', lingua non scripta; census.ignota numerat */
    si (xar_numerus(descriptiones) == ZEPHYRUM)
    {
        OratioDescriptio* d =
            (OratioDescriptio*)xar_addere(descriptiones);

        si (d == NIHIL)
        {
            redde FALSUM;
        }
        oratio_partes_la_ignotum(piscina, textus, d);
        ignotum_additum = VERUM;
    }
    /* T30 b: ordo frequentiae intra greges classis et linguae */
    si (!_descriptiones_frequentia_ordinare(scratch, descriptiones))
    {
        redde FALSUM;
    }

    per (k = ZEPHYRUM; k < xar_numerus(descriptiones); k++)
    {
        constans OratioDescriptio* d =
            (constans OratioDescriptio*)xar_obtinere(descriptiones, k);
        MateriaNodus* nodus = oratio_partes_nodum_struere(piscina, d,
            origo);

        si (   nodus == NIHIL
            || !materia_nodus_appendere(piscina, vocabulum,
                (i32)ORATIO_VOCABULUM_ANALYSES,
                materia_valor_nodus(nodus),
                MATERIA_LOCUS_LISTA_NODUS))
        {
            redde FALSUM;
        }
        si (!classis_visa[d->classis])
        {
            classis_visa[d->classis]         = VERUM;
            ordo_classium[numerus_classium]  = d->classis;
            numerus_classium                 = numerus_classium + I;
        }
                                si (   !d->lingua_ignota
                                    && !lingua_visa[d->lingua])
                                {
            lingua_visa[d->lingua]             = VERUM;

            ordo_linguarum[numerus_linguarum]  = d->lingua;
            numerus_linguarum                  = numerus_linguarum + I;
            si (census != NIHIL)
            {
                census->vocabula_linguarum[d->lingua] =
                    census->vocabula_linguarum[d->lingua] + I;
            }
                                }
        si (census != NIHIL)
        {
                        census->analyses = census->analyses + I;
            census->classes[d->classis] = census->classes[d->classis]
                + I;
            si (!d->lingua_ignota)
            {
                census->linguae[d->lingua] =
                    census->linguae[d->lingua] + I;
            }

        }
    }
    /* compendia: classes (semper), linguae (si analyses) */
    {
        character buffer[512];
              i32 n = ZEPHYRUM;

                si (census != NIHIL && ignotum_additum)
                {
            census->ignota = census->ignota + I;
                }
        si (numerus_classium == ZEPHYRUM)
        {
            n = (i32)sprintf(buffer, "%s",
                oratio_classis_titulus(ORATIO_CLASSIS_IGNOTUM));
        }

        alioquin
        {
            per (k = ZEPHYRUM; k < numerus_classium; k++)
            {
                constans character* t = oratio_classis_titulus(
                    ordo_classium[k]);

                si (n + (i32)strlen(t) + (i32)II >= (i32)512)
                {
                    frange;
                }
                n = n + (i32)sprintf(buffer + n, "%s%s", k
                    > ZEPHYRUM ? " " : "",
                    t);
            }
        }
        {
            unio {
                character* c;
                       i8* m;
            } u;

            u.c = buffer;
            si (!_derivatum_ponere(piscina, vocabulum,
                    (i32)ORATIO_VOCABULUM_CLASSES, _chorda(u.m, n),
                    origo))
            {
                redde FALSUM;
            }
        }
        si (numerus_linguarum > ZEPHYRUM)
        {
            n = ZEPHYRUM;
            per (k = ZEPHYRUM; k < numerus_linguarum; k++)
            {
                n = n + (i32)sprintf(buffer + n, "%s%s", k
                    > ZEPHYRUM ? " " : "",
                    ORATIO_TITULI_LINGUARUM[ordo_linguarum[k]]);
            }
            {
                unio {
                    character* c;
                           i8* m;
                } u;

                u.c = buffer;
                si (!_derivatum_ponere(piscina, vocabulum,
                        (i32)ORATIO_VOCABULUM_LINGUAE, _chorda(u.m, n),
                        origo))
                {
                    redde FALSUM;
                }
            }
        }
    }
    si (census != NIHIL)
    {
        census->annotata = census->annotata + I;
    }
    redde VERUM;
}

/* encliticum annotare (T21): lectio una (descriptio TACKON), compendia
 * classis et linguae, census ut vocabulum annotatum */
interior b32
_encliticum_annotare (
                      Piscina* piscina,
                 MateriaNodus* vocabulum,
    constans OratioDescriptio* d,
        constans MateriaToken* origo,
           OratioPartesCensus* census)
{
    MateriaNodus* nodus = oratio_partes_nodum_struere(piscina, d,
        origo);
    constans character* classis = oratio_classis_titulus(d->classis);
    unio {
        constans character* c;
                        i8* m;
    } u;
    chorda valor;

    si (   nodus == NIHIL || classis == NIHIL
        || !materia_nodus_appendere(piscina, vocabulum,
            (i32)ORATIO_VOCABULUM_ANALYSES, materia_valor_nodus(nodus),
            MATERIA_LOCUS_LISTA_NODUS))
    {
        redde FALSUM;
    }
    u.c            = classis;
    valor.datum    = u.m;
    valor.mensura  = (i32)strlen(classis);
    si (!_derivatum_ponere(piscina, vocabulum,
            (i32)ORATIO_VOCABULUM_CLASSES, valor, origo))
    {
        redde FALSUM;
    }
    si (!d->lingua_ignota)
    {
        u.c            = ORATIO_TITULI_LINGUARUM[d->lingua];
        valor.datum    = u.m;
        valor.mensura  = (i32)strlen(u.c);
        si (!_derivatum_ponere(piscina, vocabulum,
                (i32)ORATIO_VOCABULUM_LINGUAE, valor, origo))
        {
            redde FALSUM;
        }
    }
    si (census != NIHIL)
    {
        census->vocabula             = census->vocabula + I;
        census->annotata             = census->annotata + I;
        census->analyses             = census->analyses + I;
        census->enclitica            = census->enclitica + I;
        census->classes[d->classis]  = census->classes[d->classis] + I;
        si (!d->lingua_ignota)
        {
            census->linguae[d->lingua] = census->linguae[d->lingua] + I;
            census->vocabula_linguarum[d->lingua] =
                census->vocabula_linguarum[d->lingua] + I;
        }
    }
    redde VERUM;
}

b32
oratio_partes_encliticum_scindere (
                          Piscina*  piscina,
                          Piscina*  scratch,
       constans OratioVocabularia*  vocabularia,
                     MateriaNodus*  vocabulum,
               OratioPartesCensus*  census,
                     MateriaNodus** encliticum)
{
    constans MateriaValor* partes =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
          constans MateriaToken* origo;
        constans OratioAnalysis* a;
    constans OratioAdditamentum* t;
                         chorda  textus;
                            Xar* analyses;
                            Xar* descriptiones;
                            i32  scissio;
             MateriaToken* hospes;
             MateriaToken* lexema;
             MateriaNodus* e;
             MateriaValor  lista;

    *encliticum = NIHIL;
    si (   vocabulum->loci[ORATIO_VOCABULUM_CLASSES].genus
            != MATERIA_VALOR_NIHIL
        || partes->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*partes) != I)
    {
        redde VERUM;   /* iam annotatum aut partes plures */
    }
    origo = materia_valor_lista_obtinere(*partes,
        ZEPHYRUM)->datum.token;
    textus = oratio_partes_textus_vocabuli(scratch, vocabulum);
    si (textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    analyses = oratio_vocabularium_la_quaerere(scratch, vocabularia->la,
        textus);
    si (analyses == NIHIL)
    {
        redde FALSUM;
    }
    si (xar_numerus(analyses) < (i32)II)
    {
        redde VERUM;   /* TACKON sine hospite non fit (contractus III) */
    }
    a = (constans OratioAnalysis*)xar_obtinere(analyses, ZEPHYRUM);
    si (a->genus != ORATIO_ANALYSIS_TACKON || a->tackon < ZEPHYRUM)
    {
        redde VERUM;
    }
    /* custodia Anglica: forma tota Moby nota (unique, mosque) manet */
    si (vocabularia->en != NIHIL)
    {
                Xar* analyses_en =
                    oratio_vocabularium_en_analysare(scratch,
                    vocabularia->en, textus);

        si (analyses_en == NIHIL)
        {
            redde FALSUM;
        }
        si (xar_numerus(analyses_en) > ZEPHYRUM)
        {
            redde VERUM;
        }
    }
        t = oratio_vocabularium_la_additamentum(vocabularia->la,
            a->tackon);
    si (   t                == NIHIL || t->forma.mensura == ZEPHYRUM
        || t->forma.mensura >= origo->valor.mensura)
    {
        redde VERUM;
    }
        /* nulla custodia nominum: 'Lucane' chartarum (= Lucanae, LXVII
     * scissiones falsas) lacuna DICTIONARII erat (lucanus adi. absens,
     * ae > e medii aevi) et glossario suppleta est - custodia
     * 'hospes nomen proprium solum' nihil mutabat (Luca bos = elephas:
     * lectio communis adest) */
    scissio  = origo->valor.mensura - t->forma.mensura;
    hospes   = oratio_lexema_pars(piscina, origo, ZEPHYRUM, scissio);
    lexema  = oratio_lexema_pars(piscina, origo, scissio,
        t->forma.mensura);
    si (hospes == NIHIL || lexema == NIHIL)
    {
        redde FALSUM;
    }
    /* hospes: partes = lexema partitum; cauda vacua (migrat) */
    lista = materia_valor_lista_appendere(piscina,
        materia_valor_lista_nova(piscina), materia_valor_token(hospes));
    si (!materia_nodus_reponere(vocabulum, (i32)ORATIO_VOCABULUM_PARTES,
            lista, MATERIA_LOCUS_LISTA_TOKEN))
    {
        redde FALSUM;
    }
    /* encliticum: nodus novus cum lexemate, cauda hospitis, lectione */
    e = materia_nodus_creare(piscina, (s32)ORATIO_GENUS_VOCABULUM,
        ORATIO_REGISTRUM.genera[ORATIO_GENUS_VOCABULUM].loci_numerus);
    lista = materia_valor_lista_appendere(piscina,
        materia_valor_lista_nova(piscina), materia_valor_token(lexema));
    si (   e == NIHIL
        || !materia_nodus_ponere(e, (i32)ORATIO_VOCABULUM_PARTES, lista,
            MATERIA_LOCUS_LISTA_TOKEN))
    {
        redde FALSUM;
    }
    si (vocabulum->loci[ORATIO_VOCABULUM_CAUDA].genus
        == MATERIA_VALOR_LISTA)
    {
        si (   !materia_nodus_ponere(e, (i32)ORATIO_VOCABULUM_CAUDA,
                vocabulum->loci[ORATIO_VOCABULUM_CAUDA],
                MATERIA_LOCUS_LISTA_TOKEN)
            || !materia_nodus_reponere(vocabulum,
                (i32)ORATIO_VOCABULUM_CAUDA,
                materia_valor_lista_nova(piscina),
                MATERIA_LOCUS_LISTA_TOKEN))
        {
            redde FALSUM;
        }
    }
    descriptiones = xar_creare(scratch,
        (i32)magnitudo(OratioDescriptio));
    si (   descriptiones              == NIHIL
        || !oratio_partes_la_describere(piscina, vocabularia->la, a,
            descriptiones)
        || xar_numerus(descriptiones) != I
        || !_encliticum_annotare(piscina, e,
            (constans OratioDescriptio*)xar_obtinere(descriptiones,
                ZEPHYRUM),
            lexema, census))
    {
        redde FALSUM;
    }
    *encliticum = e;
    redde VERUM;
}

/* insertio enclitici post elementum j listae (T21) */
nomen structura {
             i32  j;
    MateriaNodus* nodus;
} Insertio;

interior b32
_annotare (
                           Piscina* piscina,
                           Piscina* scratch,
        constans OratioVocabularia* vocabularia,
                      MateriaNodus* n,
                OratioPartesCensus* census,
                   OratioDialectus  dialectus)
{
    i32 i;

    si (n == NIHIL)
    {
        redde VERUM;
    }
    si (n->genus == (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde oratio_partes_vocabulum_annotare(piscina, scratch,
            vocabularia,
            n, census, dialectus);
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            si (!_annotare(piscina, scratch, vocabularia,
                v->datum.nodus,
                census, dialectus))
            {
                redde FALSUM;
            }
        }
                alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32  m = materia_valor_lista_numerus(*v);
            i32  j;
            Xar* insertiones = NIHIL;   /* T21: enclitica post hospites */

            per (j = ZEPHYRUM; j < m; j++)
            {
                MateriaValor* e = materia_valor_lista_obtinere(*v, j);
                MateriaNodus* encliticum = NIHIL;

                si (e == NIHIL || e->genus != MATERIA_VALOR_NODUS)
                {
                    perge;
                }
                si (   e->datum.nodus->genus
                        == (s32)ORATIO_GENUS_VOCABULUM
                    && !oratio_partes_encliticum_scindere(piscina,
                        scratch, vocabularia, e->datum.nodus, census,
                        &encliticum))
                {
                    redde FALSUM;
                }
                si (!_annotare(piscina, scratch, vocabularia,
                        e->datum.nodus, census, dialectus))
                {
                    redde FALSUM;
                }
                si (encliticum != NIHIL)
                {
                    Insertio* ins;

                    si (insertiones == NIHIL)
                    {
                        insertiones = xar_creare(scratch,
                            (i32)magnitudo(Insertio));
                    }
                    ins = insertiones != NIHIL
                        ? (Insertio*)xar_addere(insertiones) : NIHIL;
                    si (ins == NIHIL)
                    {
                        redde FALSUM;
                    }
                    ins->j      = j;
                    ins->nodus  = encliticum;
                }
            }
            si (insertiones != NIHIL)
            {
                MateriaValor nova  = materia_valor_lista_nova(piscina);
                         i32 k     = ZEPHYRUM;

                per (j = ZEPHYRUM; j < m; j++)
                {
                    nova = materia_valor_lista_appendere(piscina, nova,
                        *materia_valor_lista_obtinere(*v, j));
                    si (   k < xar_numerus(insertiones)
                        && ((Insertio*)xar_obtinere(insertiones, k))->j
                            == j)
                    {
                        nova = materia_valor_lista_appendere(piscina,
                            nova, materia_valor_nodus(
                                ((Insertio*)xar_obtinere(insertiones,
                                    k))->nodus));
                        k = k + I;
                    }
                }
                si (!materia_nodus_reponere(n, i, nova,
                        MATERIA_LOCUS_LISTA_NODUS))
                {
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}

/* T22 b: praescansio - status formae plicatae (I nota, II ignota
 * recuperata, III ignota) semel per indicem; vocabula subarboris */
interior b32
_praescandere (
                           Piscina* scratch,
        constans OratioVocabularia* vocabularia,
                    TabulaDispersa* index,
             constans MateriaNodus* n,
                               i32* recuperata,
                               i32* verba)
{
    i32 i;

    si (n == NIHIL)
    {
        redde VERUM;
    }
    si (n->genus == (s32)ORATIO_GENUS_VOCABULUM)
    {
        chorda  textus = oratio_partes_textus_vocabuli(scratch, n);
        chorda  plicata;
        vacuum* valor;
           i32* status;

        si (textus.datum == NIHIL || textus.mensura == ZEPHYRUM)
        {
            redde VERUM;
        }
        plicata = oratio_vocabularium_la_plicare(scratch, textus);
        si (plicata.datum == NIHIL)
        {
            redde FALSUM;
        }
        si (tabula_dispersa_invenire(index, plicata, &valor))
        {
            status = (i32*)valor;
        }
        alioquin
        {
            Xar* analyses = oratio_vocabularium_la_quaerere(scratch,
                vocabularia->la, textus);

            status = (i32*)piscina_allocare(scratch,
                (memoriae_index)magnitudo(i32));
            si (analyses == NIHIL || status == NIHIL)
            {
                redde FALSUM;
            }
            *status = I;
            si (xar_numerus(analyses) == ZEPHYRUM)
            {
                *status = (i32)III;
                si (   ORTHOGRAPHIA_ACTIVA
                    && (   ORTHOGRAPHIA_CAPITALIA
                        || !(   textus.datum[ZEPHYRUM] >= 'A'
                             && textus.datum[ZEPHYRUM] <= 'Z')))
                {
                    b32 anglica_nota = FALSUM;

                    si (vocabularia->en != NIHIL)
                    {
                        Xar* nota_en = oratio_vocabularium_en_analysare(
                            scratch, vocabularia->en, textus);

                        si (nota_en == NIHIL)
                        {
                            redde FALSUM;
                        }
                        anglica_nota = (b32)(xar_numerus(nota_en)
                            > ZEPHYRUM);
                    }
                    si (!anglica_nota)
                    {
                        s32  varians;
                        Xar* variantes =
                            oratio_vocabularium_la_quaerere_variantes(
                            scratch, vocabularia->la, textus, &varians);

                        si (variantes == NIHIL)
                        {
                            redde FALSUM;
                        }
                        si (xar_numerus(variantes) > ZEPHYRUM)
                        {
                            *status = (i32)II;
                        }
                    }
                }
            }
            (vacuum)tabula_dispersa_inserere(index, plicata, status);
        }
        *verba = *verba + I;
        si (*status == (i32)II)
        {
            *recuperata = *recuperata + I;
        }
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            si (!_praescandere(scratch, vocabularia, index,
                v->datum.nodus,
                    recuperata, verba))
            {
                redde FALSUM;
            }
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v,
                    j);

                si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
                    && !_praescandere(scratch, vocabularia, index,
                        e->datum.nodus, recuperata, verba))
                {
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}

b32
oratio_partes_praescandere (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
            constans MateriaNodus* radix,
                              i32* recuperata,
                              i32* verba)
{
    Piscina* scratch = piscina_generare_dynamicum("partes_praescansio",
        67108864);
    TabulaDispersa* index;
               b32  sanum;

    (vacuum)piscina;
    *recuperata  = ZEPHYRUM;
    *verba       = ZEPHYRUM;
    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    index = tabula_dispersa_creare_chorda(scratch, (i32)4096);
    sanum = index != NIHIL && _praescandere(scratch, vocabularia, index,
        radix, recuperata, verba);
    piscina_destruere(scratch);
    redde sanum;
}

/* limen dialecti medii (millesimae verborum recuperatarum): census
 * thesaurorum (formae per orthographiam recuperatae / vocabula): chartae
 * dev 602/20704 = XXIX, test 601/20727 = XXIX, Dante 140/11441 = XII;
 * Seneca 0, Aquinas 0, Perseus 5/9102 < I, PROIEL 2/14110 < I */
hic_manens constans i32 DIALECTUS_MEDIUS_MILLESIMAE = 5;

OratioDialectus
oratio_partes_dialectus_censu (
    i32 recuperata,
    i32 verba)
{
    si (verba == ZEPHYRUM || recuperata == ZEPHYRUM)
    {
        redde ORATIO_DIALECTUS_CLASSICUS;
    }
    redde recuperata * (i32)1000 >= verba * DIALECTUS_MEDIUS_MILLESIMAE
        ? ORATIO_DIALECTUS_MEDIUS : ORATIO_DIALECTUS_CLASSICUS;
}

b32
oratio_partes_annotare_dialecto (
                           Piscina* piscina,
        constans OratioVocabularia* vocabularia,
                      MateriaNodus* radix,
                OratioPartesCensus* census,
                   OratioDialectus  dialectus)
{
    Piscina* scratch = piscina_generare_dynamicum("partes_annotatio",
        67108864);
    b32 sanum;

    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    si (census != NIHIL)
    {
        memset(census, ZEPHYRUM, magnitudo(*census));
    }
    /* T22 b: dialectus ignotus - praescansio subarboris ipsius */
    si (dialectus == ORATIO_DIALECTUS_IGNOTUS)
    {
        i32 recuperata;
        i32 verba;

        si (!oratio_partes_praescandere(piscina, vocabularia, radix,
                &recuperata, &verba))
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        dialectus = oratio_partes_dialectus_censu(recuperata, verba);
    }
    si (census != NIHIL)
    {
        census->dialectus = (s32)dialectus;
    }
    sanum = _annotare(piscina, scratch, vocabularia, radix, census,
        dialectus);
    piscina_destruere(scratch);
    /* nodi novi patres accipiunt (lex materiae: comparator patres confert) */
    si (sanum)
    {
        materia_arbor_patres_figere(piscina, radix);
    }
    redde sanum;
}

b32
oratio_partes_annotare (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
                     MateriaNodus* radix,
               OratioPartesCensus* census)
{
    redde oratio_partes_annotare_dialecto(piscina, vocabularia, radix,
        census, ORATIO_DIALECTUS_IGNOTUS);
}

b32
oratio_partes_compendia_reponere (
         Piscina* piscina,
    MateriaNodus* vocabulum)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    constans MateriaValor* partes =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
    constans MateriaToken* origo;
                character  buffer[512];
                      i32  n;
                      i32  k;
                      i32  m;
                      b32  classis_visa[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
                      b32  lingua_visa[ORATIO_LINGUA_NUMERUS];
                    unio {
        character* c;
               i8* m;
    } u;

    si (   vocabulum->loci[ORATIO_VOCABULUM_CLASSES].genus
            != MATERIA_VALOR_TOKEN
        || analyses->genus                      != MATERIA_VALOR_LISTA
        || partes->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*partes) == ZEPHYRUM)
    {
        redde VERUM;
    }
    m = materia_valor_lista_numerus(*analyses);
    si (m == ZEPHYRUM)
    {
        redde VERUM;   /* 'ignotum' manet */
    }
    origo = materia_valor_lista_obtinere(*partes,
        ZEPHYRUM)->datum.token;
    memset(classis_visa, ZEPHYRUM, magnitudo(classis_visa));
    memset(lingua_visa, ZEPHYRUM, magnitudo(lingua_visa));
    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < m; k++)
    {
        constans MateriaNodus* a =
            materia_valor_lista_obtinere(*analyses, k)->datum.nodus;
                OratioClassis c =
                    oratio_genus_classis((OratioGenus)a->genus);
           constans character* t;

        si (c >= ORATIO_CLASSIS_NUMERUS_CLASSIUM || classis_visa[c])
        {
            perge;
        }
        t = oratio_classis_titulus(c);
        si (n + (i32)strlen(t) + (i32)II >= (i32)512)
        {
            frange;
        }
        n = n + (i32)sprintf(buffer + n, "%s%s", n
            > ZEPHYRUM ? " " : "", t);
        classis_visa[c] = VERUM;
    }
    u.c = buffer;
    {
        MateriaToken* t = oratio_lexema_derivatum(piscina,
            (s32)ORATIO_LEX_DERIVATUM, _copia(piscina, _chorda(u.m, n)),
            origo);

        si (   t == NIHIL
            || !materia_nodus_reponere(vocabulum,
                (i32)ORATIO_VOCABULUM_CLASSES, materia_valor_token(t),
                MATERIA_LOCUS_TOKEN))
        {
            redde FALSUM;
        }
    }
    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < m; k++)
    {
        constans MateriaNodus* a =
            materia_valor_lista_obtinere(*analyses, k)->datum.nodus;
        constans MateriaValor* l = &a->loci[ORATIO_ANALYSIS_LINGUA];
                          s32  lingua = l->genus == MATERIA_VALOR_INDEX
                              ? l->datum.index : (s32)-I;

        si (   lingua < ZEPHYRUM || lingua >= (s32)ORATIO_LINGUA_NUMERUS
            || lingua_visa[lingua])
        {
            perge;
        }
        n = n + (i32)sprintf(buffer + n, "%s%s", n
            > ZEPHYRUM ? " " : "",
            ORATIO_TITULI_LINGUARUM[lingua]);
        lingua_visa[lingua] = VERUM;
    }
    si (n > ZEPHYRUM)
    {
        MateriaToken* t = oratio_lexema_derivatum(piscina,
            (s32)ORATIO_LEX_DERIVATUM, _copia(piscina, _chorda(u.m, n)),
            origo);
        b32 scriptum = vocabulum->loci[ORATIO_VOCABULUM_LINGUAE].genus
            != MATERIA_VALOR_NIHIL;

        si (   t == NIHIL
            || !(scriptum
                ? materia_nodus_reponere(vocabulum,
                    (i32)ORATIO_VOCABULUM_LINGUAE,
                    materia_valor_token(t),
                    MATERIA_LOCUS_TOKEN)
                : materia_nodus_ponere(vocabulum,
                    (i32)ORATIO_VOCABULUM_LINGUAE,
                    materia_valor_token(t),
                    MATERIA_LOCUS_TOKEN)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}
