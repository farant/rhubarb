/* probatio_statuae_repositorium.c - statuae in volumine
 * (statuae_repositorium.h)
 *
 * addere et enumerare: ordo additionis, sigilla, octeti per massam
 * redeunt. substitutio: titulus notus locum servat, sigillum novum;
 * octeti iidem nihil scribunt. removere: evanescit, ignotus innocuus,
 * readditus in fine. tituli mali: recusati, nihil scriptum. seq:
 * crescit, et ansa ALTERA eiusdem voluminis mutationem videt (ita
 * latus vici statuam ab alio processu additam invenit).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "sigillum.h"
#include "volumen.h"
#include "statuae_repositorium.h"
#include "credo.h"
#include <string.h>

interior chorda
_ch (
        CredoContextus* c,
    constans character* litterae)
{
    redde chorda_ex_literis(litterae, c->piscina);
}

interior StatuaeInscriptio*
_inscriptio (
    Xar* index,
    i32  k)
{
    redde (StatuaeInscriptio*)xar_obtinere(index, k);
}

/* sigillum hex octetorum */
interior chorda
_sigillum (
    CredoContextus* c,
            chorda  octeti)
{
     Sigillum  s = sigillum_computare(octeti.datum, octeti.mensura);
    character* hex = (character*)piscina_allocare(c->piscina,
        SIGILLUM_HEX_MENSURA);

    sigillum_hex(&s, hex);
    redde chorda_ex_literis(hex, c->piscina);
}

interior vacuum
_sectio_addere (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "statuae_addere");
        Xar* index;
     chorda  lapis   = _ch(c, "octeti lapidis");
     chorda  fungus  = _ch(c, "octeti fungi, alii");
     chorda  redditi;
        b32  inventum = FALSUM;

    CREDO_NECESSE (v != NIHIL);
    index = statuae_enumerare(v, c->piscina);
    CREDO_NECESSE (index != NIHIL);
    CREDO_AEQUALIS_I32 (xar_numerus(index), ZEPHYRUM);
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), lapis));
    CREDO_VERUM (statuae_addere(v, _ch(c, "fungus"), fungus));
    index = statuae_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "lapis"));
    CREDO_VERUM (chorda_aequalis_literis(_inscriptio(index, I)->titulus,
        "fungus"));
    CREDO_VERUM (chorda_aequalis(
        _inscriptio(index, ZEPHYRUM)->sigillum_hex,
        _sigillum(c, lapis)));
    CREDO_VERUM (chorda_aequalis(_inscriptio(index, I)->sigillum_hex,
        _sigillum(c, fungus)));
    redditi = volumen_massam_promere(v,
        _inscriptio(index, I)->sigillum_hex, c->piscina, &inventum);
    CREDO_VERUM (inventum);
    CREDO_VERUM (chorda_aequalis(redditi, fungus));
    /* titulus cum characteribus JSON effugiendis integer redit */
    CREDO_VERUM (statuae_addere(v, _ch(c, "a \"b\" \\ c"), lapis));
    index = statuae_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), III);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, II)->titulus, "a \"b\" \\ c"));
    volumen_claudere(v);
}

interior vacuum
_sectio_substitutio (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "statuae_subst");
        Xar* index;
     chorda  vetus = _ch(c, "octeti veteres");
     chorda  novus = _ch(c, "octeti novi");
        s64  seq;

    CREDO_NECESSE (v != NIHIL);
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), vetus));
    CREDO_VERUM (statuae_addere(v, _ch(c, "fungus"), vetus));
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), novus));
    index = statuae_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "lapis"));
    CREDO_VERUM (chorda_aequalis(
        _inscriptio(index, ZEPHYRUM)->sigillum_hex,
        _sigillum(c, novus)));
    CREDO_VERUM (chorda_aequalis(_inscriptio(index, I)->sigillum_hex,
        _sigillum(c, vetus)));
    /* octeti iidem sub titulo eodem: nihil scribitur */
    seq = volumen_seq_ultimum(v);
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), novus));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    volumen_claudere(v);
}

interior vacuum
_sectio_removere (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "statuae_remov");
        Xar* index;
     chorda  octeti = _ch(c, "octeti");
        s64  seq;
        b32  inventum = FALSUM;

    CREDO_NECESSE (v != NIHIL);
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), octeti));
    CREDO_VERUM (statuae_addere(v, _ch(c, "fungus"), octeti));
    CREDO_VERUM (statuae_removere(v, _ch(c, "lapis")));
    index = statuae_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "fungus"));
    /* massa remotae manet */
    (vacuum)volumen_massam_promere(v, _sigillum(c, octeti), c->piscina,
        &inventum);
    CREDO_VERUM (inventum);
    /* titulus ignotus: VERUM, nihil scriptum */
    seq = volumen_seq_ultimum(v);
    CREDO_VERUM (statuae_removere(v, _ch(c, "nemo")));
    CREDO_VERUM (statuae_removere(v, _ch(c, "lapis")));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    /* readdita: in fine */
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), octeti));
    index = statuae_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_VERUM (chorda_aequalis_literis(_inscriptio(index, I)->titulus,
        "lapis"));
    volumen_claudere(v);
}

interior vacuum
_sectio_tituli_mali (
    CredoContextus* c)
{
     Volumen* v = volumen_temporarium(c->piscina, "statuae_tituli");
      chorda  octeti = _ch(c, "octeti");
      chorda  vacui;
      chorda  nimius;
   character  litterae[LXVI];
         s64  seq;

    CREDO_NECESSE (v != NIHIL);
    seq = volumen_seq_ultimum(v);
    memset(litterae, 'a', LXV);
    litterae[LXV]  = '\0';
    nimius         = _ch(c, litterae);
    vacui.datum    = octeti.datum;
    vacui.mensura  = ZEPHYRUM;
    CREDO_FALSUM (statuae_titulus_validus(_ch(c, "")));
    CREDO_FALSUM (statuae_titulus_validus(_ch(c, "a/b")));
    CREDO_FALSUM (statuae_titulus_validus(_ch(c, "a\nb")));
    CREDO_FALSUM (statuae_titulus_validus(nimius));
    CREDO_VERUM (statuae_titulus_validus(_ch(c, "stone golem 2")));
    nimius.mensura = LXIV;
    CREDO_VERUM (statuae_titulus_validus(nimius));
    nimius.mensura = LXV;
    CREDO_FALSUM (statuae_addere(v, _ch(c, ""), octeti));
    CREDO_FALSUM (statuae_addere(v, _ch(c, "a/b"), octeti));
    CREDO_FALSUM (statuae_addere(v, _ch(c, "a\tb"), octeti));
    CREDO_FALSUM (statuae_addere(v, nimius, octeti));
    /* octeti vacui, volumen nullum */
    CREDO_FALSUM (statuae_addere(v, _ch(c, "lapis"), vacui));
    CREDO_FALSUM (statuae_addere(NIHIL, _ch(c, "lapis"), octeti));
    CREDO_FALSUM (statuae_removere(NIHIL, _ch(c, "lapis")));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare(v, c->piscina)),
        ZEPHYRUM);
    CREDO_VERUM (statuae_enumerare(NIHIL, c->piscina) != NIHIL);
    volumen_claudere(v);
}

interior vacuum
_sectio_seq (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "statuae_seq");
    Volumen* alter;
        Xar* index;
     chorda  octeti = _ch(c, "octeti");
        s64  seq;
        s64  alterius;

    CREDO_NECESSE (v != NIHIL);
    /* ansa altera eiusdem plagulae (ut vicus et bin/statuae) */
    alter = volumen_aperire(c->piscina, volumen_via(v));
    CREDO_NECESSE (alter != NIHIL);
    seq       = volumen_seq_ultimum(v);
    alterius  = volumen_seq_ultimum(alter);
    CREDO_VERUM (seq == alterius);
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), octeti));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    CREDO_VERUM (volumen_seq_ultimum(alter) > alterius);
    index = statuae_enumerare(alter, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "lapis"));
    alterius = volumen_seq_ultimum(alter);
    CREDO_VERUM (statuae_removere(alter, _ch(c, "lapis")));
    CREDO_VERUM (volumen_seq_ultimum(v) > alterius);
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare(v, c->piscina)),
        ZEPHYRUM);
    volumen_claudere(alter);
    volumen_claudere(v);
}

interior vacuum
_sectio_transactio (
    CredoContextus* c)
{
    Volumen* v       = volumen_temporarium(c->piscina, "statuae_trans");
     chorda  octeti  = _ch(c, "octeti");

    CREDO_NECESSE (v != NIHIL);
    /* transactio vocatoris iungitur: revolutio additionem tollit */
    CREDO_VERUM (volumen_transactionem_incipere(v));
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), octeti));
    CREDO_VERUM (volumen_in_transactione(v));
    CREDO_VERUM (volumen_transactionem_revolvere(v));
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare(v, c->piscina)),
        ZEPHYRUM);
    /* sine transactione vocatoris: propria, clausa post */
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), octeti));
    CREDO_FALSUM (volumen_in_transactione(v));
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare(v, c->piscina)),
        I);
    volumen_claudere(v);
}

/* mundus-plan T8: loci - index statuarum eos non monstrat, index
 * locorum statuas non monstrat; tituli per genera ambo unici */
interior vacuum
_sectio_genera (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "statuae_genera");
        Xar* index;
     chorda  a = _ch(c, "octeti personae");
     chorda  b = _ch(c, "octeti loci");
     chorda  d = _ch(c, "octeti loci novi");
        s64  seq;

    CREDO_NECESSE (v != NIHIL);
    /* vetus (sine genere) = statua */
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), a));
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "silva"), b,
        STATUAE_GENUS_LOCUS));
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "fungus"), a,
        STATUAE_GENUS_STATUA));
    index = statuae_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "lapis"));
    CREDO_VERUM (chorda_aequalis_literis(_inscriptio(index, I)->titulus,
        "fungus"));
    index = statuae_enumerare_generis(v, STATUAE_GENUS_STATUA,
        c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_AEQUALIS_I32 (_inscriptio(index, ZEPHYRUM)->genus,
        STATUAE_GENUS_STATUA);
    index = statuae_enumerare_generis(v, STATUAE_GENUS_LOCUS,
        c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "silva"));
    CREDO_AEQUALIS_I32 (_inscriptio(index, ZEPHYRUM)->genus,
        STATUAE_GENUS_LOCUS);
    CREDO_VERUM (chorda_aequalis(
        _inscriptio(index, ZEPHYRUM)->sigillum_hex, _sigillum(c, b)));
    /* titulus in genere altero iam est: recusatur, nihil scribitur */
    seq = volumen_seq_ultimum(v);
    CREDO_FALSUM (statuae_addere_generis(v, _ch(c, "lapis"), b,
        STATUAE_GENUS_LOCUS));
    CREDO_FALSUM (statuae_addere(v, _ch(c, "silva"), a));
    CREDO_FALSUM (statuae_addere_generis(v, _ch(c, "aliquid"), a,
        (StatuaeGenus)VII));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    /* locus substituitur in genere suo; idem bis nihil scribit */
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "silva"), d,
        STATUAE_GENUS_LOCUS));
    seq = volumen_seq_ultimum(v);
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "silva"), d,
        STATUAE_GENUS_LOCUS));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    index = statuae_enumerare_generis(v, STATUAE_GENUS_LOCUS,
        c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    CREDO_VERUM (chorda_aequalis(
        _inscriptio(index, ZEPHYRUM)->sigillum_hex, _sigillum(c, d)));
    /* removere utrumque genus tollit; titulus deinde liber */
    CREDO_VERUM (statuae_removere(v, _ch(c, "silva")));
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare_generis(v,
        STATUAE_GENUS_LOCUS, c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare(v, c->piscina)),
        II);
    CREDO_VERUM (statuae_addere(v, _ch(c, "silva"), a));
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare(v, c->piscina)),
        III);
    /* genus ignotum in indice: vacuum, numquam NIHIL */
    index = statuae_enumerare_generis(v, (StatuaeGenus)VII, c->piscina);
    CREDO_NECESSE (index != NIHIL);
    CREDO_AEQUALIS_I32 (xar_numerus(index), ZEPHYRUM);
    volumen_claudere(v);
}

/* plastica-plan T6: res - genus tertium. Index personarum et
 * locorum eas non monstrat; tituli per genera TRIA unici */
interior vacuum
_sectio_res (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "statuae_res");
        Xar* index;
     chorda  a = _ch(c, "octeti personae");
     chorda  b = _ch(c, "octeti loci");
     chorda  d = _ch(c, "octeti rei");
     chorda  e = _ch(c, "octeti rei novae");
        s64  seq;

    CREDO_NECESSE (v != NIHIL);
    CREDO_AEQUALIS_I32 (STATUAE_GENERUM, III);
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"), a));
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "silva"), b,
        STATUAE_GENUS_LOCUS));
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "puteus"), d,
        STATUAE_GENUS_RES));
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "saepes"), d,
        STATUAE_GENUS_RES));
    /* personae et loci rem non vident */
    index = statuae_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "lapis"));
    index = statuae_enumerare_generis(v, STATUAE_GENUS_LOCUS,
        c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "silva"));
    /* res: ordine additionis, genere suo */
    index = statuae_enumerare_generis(v, STATUAE_GENUS_RES,
        c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "puteus"));
    CREDO_VERUM (chorda_aequalis_literis(_inscriptio(index, I)->titulus,
        "saepes"));
    CREDO_AEQUALIS_I32 (_inscriptio(index, ZEPHYRUM)->genus,
        STATUAE_GENUS_RES);
    CREDO_VERUM (chorda_aequalis(
        _inscriptio(index, ZEPHYRUM)->sigillum_hex, _sigillum(c, d)));
    /* titulus in genere alio iam est: recusatur, nihil scribitur */
    seq = volumen_seq_ultimum(v);
    CREDO_FALSUM (statuae_addere_generis(v, _ch(c, "lapis"), d,
        STATUAE_GENUS_RES));
    CREDO_FALSUM (statuae_addere_generis(v, _ch(c, "silva"), d,
        STATUAE_GENUS_RES));
    CREDO_FALSUM (statuae_addere(v, _ch(c, "puteus"), a));
    CREDO_FALSUM (statuae_addere_generis(v, _ch(c, "puteus"), b,
        STATUAE_GENUS_LOCUS));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    /* res substituitur in genere suo, locum servat; idem bis nihil
     * scribit */
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "puteus"), e,
        STATUAE_GENUS_RES));
    seq = volumen_seq_ultimum(v);
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "puteus"), e,
        STATUAE_GENUS_RES));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    index = statuae_enumerare_generis(v, STATUAE_GENUS_RES,
        c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "puteus"));
    CREDO_VERUM (chorda_aequalis(
        _inscriptio(index, ZEPHYRUM)->sigillum_hex, _sigillum(c, e)));
    /* removere rem tollit; titulus deinde liber */
    CREDO_VERUM (statuae_removere(v, _ch(c, "puteus")));
    index = statuae_enumerare_generis(v, STATUAE_GENUS_RES,
        c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "saepes"));
    CREDO_VERUM (statuae_addere(v, _ch(c, "puteus"), a));
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare(v, c->piscina)),
        II);
    volumen_claudere(v);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "addere", _sectio_addere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "substitutio", _sectio_substitutio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "removere", _sectio_removere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tituli_mali", _sectio_tituli_mali, NIHIL, NIHIL, NIHIL, NIHIL },
    { "seq", _sectio_seq, NIHIL, NIHIL, NIHIL, NIHIL },
    { "transactio", _sectio_transactio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "genera", _sectio_genera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "res", _sectio_res, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("statuae_repositorium", SECTIONES);
}
