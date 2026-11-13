/* probatio_scaena_repositorium.c - prospectus in volumine
 * (scaena_repositorium.h)
 *
 * addere et enumerare: ordo additionis, sigilla, octeti (etiam
 * nulli in medio) per promere redeunt. substitutio: titulus notus
 * locum servat; octeti iidem nihil scribunt. removere: evanescit,
 * ignotus FALSUM sine scriptura, readditus in fine. tituli mali:
 * recusati, nihil scriptum. seq: ansa altera mutationem videt.
 * transactio vocatoris iungitur. separata: index statuarum
 * prospectus non videt, nec hic statuas.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "sigillum.h"
#include "volumen.h"
#include "statuae_repositorium.h"
#include "scaena_repositorium.h"
#include "credo.h"
#include <string.h>

interior chorda
_ch (
        CredoContextus* c,
    constans character* litterae)
{
    redde chorda_ex_literis(litterae, c->piscina);
}

interior ScaenaInscriptio*
_inscriptio (
    Xar* index,
    i32  k)
{
    redde (ScaenaInscriptio*)xar_obtinere(index, k);
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
    Volumen* v = volumen_temporarium(c->piscina, "scaena_addere");
        Xar* index;
     chorda  focus   = _ch(c, "octeti foci");
     chorda  ostium  = _ch(c, "octeti ostii, alii");
     chorda  cruda;
     chorda  reddita;
         i8  octeti[VIII] = { 'S', 0, 0xFF, 0, 0, 'a', 0x0A, 0 };

    CREDO_NECESSE (v != NIHIL);
    index = scaena_enumerare(v, c->piscina);
    CREDO_NECESSE (index != NIHIL);
    CREDO_AEQUALIS_I32 (xar_numerus(index), ZEPHYRUM);
    CREDO_VERUM (scaena_addere(v, _ch(c, "hut-hearth"), focus));
    CREDO_VERUM (scaena_addere(v, _ch(c, "hut-door"), ostium));
    index = scaena_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "hut-hearth"));
    CREDO_VERUM (chorda_aequalis_literis(_inscriptio(index, I)->titulus,
        "hut-door"));
    CREDO_VERUM (chorda_aequalis(
        _inscriptio(index, ZEPHYRUM)->sigillum_hex,
        _sigillum(c, focus)));
    CREDO_VERUM (chorda_aequalis(_inscriptio(index, I)->sigillum_hex,
        _sigillum(c, ostium)));
    /* promere: quisque titulus massam SUAM reddit */
    reddita = scaena_promere(v, _ch(c, "hut-door"), c->piscina);
    CREDO_VERUM (chorda_aequalis(reddita, ostium));
    reddita = scaena_promere(v, _ch(c, "hut-hearth"), c->piscina);
    CREDO_VERUM (chorda_aequalis(reddita, focus));
    /* octeti crudi (nulli in medio et in fine) integri redeunt */
    cruda.datum    = octeti;
    cruda.mensura  = VIII;
    CREDO_VERUM (scaena_addere(v, _ch(c, "cruda_1"), cruda));
    reddita = scaena_promere(v, _ch(c, "cruda_1"), c->piscina);
    CREDO_AEQUALIS_I32 (reddita.mensura, VIII);
    CREDO_VERUM (chorda_aequalis(reddita, cruda));
    /* ignotus et volumen nullum: chorda vacua */
    reddita = scaena_promere(v, _ch(c, "nemo"), c->piscina);
    CREDO_VERUM (reddita.datum == NIHIL);
    CREDO_AEQUALIS_I32 (reddita.mensura, ZEPHYRUM);
    reddita = scaena_promere(NIHIL, _ch(c, "hut-door"), c->piscina);
    CREDO_VERUM (reddita.datum == NIHIL);
    volumen_claudere(v);
}

interior vacuum
_sectio_substitutio (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "scaena_subst");
        Xar* index;
     chorda  vetus = _ch(c, "octeti veteres");
     chorda  novus = _ch(c, "octeti novi");
        s64  seq;

    CREDO_NECESSE (v != NIHIL);
    CREDO_VERUM (scaena_addere(v, _ch(c, "focus"), vetus));
    CREDO_VERUM (scaena_addere(v, _ch(c, "ostium"), vetus));
    CREDO_VERUM (scaena_addere(v, _ch(c, "focus"), novus));
    index = scaena_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "focus"));
    CREDO_VERUM (chorda_aequalis(
        _inscriptio(index, ZEPHYRUM)->sigillum_hex,
        _sigillum(c, novus)));
    CREDO_VERUM (chorda_aequalis(_inscriptio(index, I)->sigillum_hex,
        _sigillum(c, vetus)));
    CREDO_VERUM (chorda_aequalis(scaena_promere(v, _ch(c, "focus"),
        c->piscina), novus));
    CREDO_VERUM (chorda_aequalis(scaena_promere(v, _ch(c, "ostium"),
        c->piscina), vetus));
    /* octeti iidem sub titulo eodem: nihil scribitur */
    seq = volumen_seq_ultimum(v);
    CREDO_VERUM (scaena_addere(v, _ch(c, "focus"), novus));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    /* octeti iidem sub titulo ALIO scribuntur */
    CREDO_VERUM (scaena_addere(v, _ch(c, "tertius"), novus));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    CREDO_AEQUALIS_I32 (xar_numerus(scaena_enumerare(v, c->piscina)),
        III);
    volumen_claudere(v);
}

interior vacuum
_sectio_removere (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "scaena_remov");
        Xar* index;
     chorda  octeti = _ch(c, "octeti");
     chorda  reddita;
        s64  seq;
        b32  inventum = FALSUM;

    CREDO_NECESSE (v != NIHIL);
    CREDO_VERUM (scaena_addere(v, _ch(c, "focus"), octeti));
    CREDO_VERUM (scaena_addere(v, _ch(c, "ostium"), octeti));
    CREDO_VERUM (scaena_addere(v, _ch(c, "tectum"), octeti));
    CREDO_VERUM (scaena_removere(v, _ch(c, "focus")));
    index = scaena_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "ostium"));
    CREDO_VERUM (chorda_aequalis_literis(_inscriptio(index, I)->titulus,
        "tectum"));
    reddita = scaena_promere(v, _ch(c, "focus"), c->piscina);
    CREDO_VERUM (reddita.datum == NIHIL);
    /* massa remotae manet */
    (vacuum)volumen_massam_promere(v, _sigillum(c, octeti), c->piscina,
        &inventum);
    CREDO_VERUM (inventum);
    /* titulus qui abest: FALSUM, nihil scriptum */
    seq = volumen_seq_ultimum(v);
    CREDO_FALSUM (scaena_removere(v, _ch(c, "nemo")));
    CREDO_FALSUM (scaena_removere(v, _ch(c, "focus")));
    CREDO_FALSUM (scaena_removere(v, _ch(c, "Malus Titulus")));
    CREDO_FALSUM (scaena_removere(NIHIL, _ch(c, "ostium")));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    /* readditus: in fine */
    CREDO_VERUM (scaena_addere(v, _ch(c, "focus"), octeti));
    index = scaena_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), III);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, II)->titulus, "focus"));
    volumen_claudere(v);
}

interior vacuum
_sectio_tituli_mali (
    CredoContextus* c)
{
      Volumen* v = volumen_temporarium(c->piscina, "scaena_tituli");
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
    CREDO_FALSUM (scaena_titulus_validus(_ch(c, "")));
    CREDO_FALSUM (scaena_titulus_validus(_ch(c, "a/b")));
    CREDO_FALSUM (scaena_titulus_validus(_ch(c, "a b")));
    CREDO_FALSUM (scaena_titulus_validus(_ch(c, "Hut")));
    CREDO_FALSUM (scaena_titulus_validus(_ch(c, "a.b")));
    CREDO_FALSUM (scaena_titulus_validus(_ch(c, "a\nb")));
    CREDO_FALSUM (scaena_titulus_validus(nimius));
    /* "mundus" foras significat: prospectus sic appellari nequit */
    CREDO_FALSUM (scaena_titulus_validus(_ch(c, "mundus")));
    CREDO_VERUM (scaena_titulus_validus(_ch(c, "mundus2")));
    CREDO_VERUM (scaena_titulus_validus(_ch(c, "mundu")));
    CREDO_VERUM (scaena_titulus_validus(_ch(c, "hut-door_2")));
    CREDO_VERUM (scaena_titulus_validus(_ch(c, "az09-_")));
    CREDO_VERUM (scaena_titulus_validus(_ch(c, "a")));
    nimius.mensura = LXIV;
    CREDO_VERUM (scaena_titulus_validus(nimius));
    nimius.mensura = LXV;
    CREDO_FALSUM (scaena_addere(v, _ch(c, ""), octeti));
    CREDO_FALSUM (scaena_addere(v, _ch(c, "a/b"), octeti));
    CREDO_FALSUM (scaena_addere(v, _ch(c, "Hut"), octeti));
    CREDO_FALSUM (scaena_addere(v, _ch(c, "mundus"), octeti));
    CREDO_FALSUM (scaena_addere(v, nimius, octeti));
    /* massa vacua, volumen nullum */
    CREDO_FALSUM (scaena_addere(v, _ch(c, "focus"), vacui));
    CREDO_FALSUM (scaena_addere(NIHIL, _ch(c, "focus"), octeti));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    CREDO_AEQUALIS_I32 (xar_numerus(scaena_enumerare(v, c->piscina)),
        ZEPHYRUM);
    CREDO_VERUM (scaena_enumerare(NIHIL, c->piscina) != NIHIL);
    volumen_claudere(v);
}

interior vacuum
_sectio_seq (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "scaena_seq");
    Volumen* alter;
        Xar* index;
     chorda  octeti = _ch(c, "octeti");
        s64  seq;
        s64  alterius;

    CREDO_NECESSE (v != NIHIL);
    /* ansa altera eiusdem plagulae (ut vicus et bin/scaena) */
    alter = volumen_aperire(c->piscina, volumen_via(v));
    CREDO_NECESSE (alter != NIHIL);
    seq       = volumen_seq_ultimum(v);
    alterius  = volumen_seq_ultimum(alter);
    CREDO_VERUM (seq == alterius);
    CREDO_VERUM (scaena_addere(v, _ch(c, "focus"), octeti));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    CREDO_VERUM (volumen_seq_ultimum(alter) > alterius);
    index = scaena_enumerare(alter, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    CREDO_VERUM (chorda_aequalis_literis(
        _inscriptio(index, ZEPHYRUM)->titulus, "focus"));
    CREDO_VERUM (chorda_aequalis(scaena_promere(alter,
        _ch(c, "focus"), c->piscina), octeti));
    alterius = volumen_seq_ultimum(alter);
    CREDO_VERUM (scaena_removere(alter, _ch(c, "focus")));
    CREDO_VERUM (volumen_seq_ultimum(v) > alterius);
    CREDO_AEQUALIS_I32 (xar_numerus(scaena_enumerare(v, c->piscina)),
        ZEPHYRUM);
    volumen_claudere(alter);
    volumen_claudere(v);
}

interior vacuum
_sectio_transactio (
    CredoContextus* c)
{
    Volumen* v       = volumen_temporarium(c->piscina, "scaena_trans");
     chorda  octeti  = _ch(c, "octeti");

    CREDO_NECESSE (v != NIHIL);
    /* transactio vocatoris iungitur: revolutio additionem tollit */
    CREDO_VERUM (volumen_transactionem_incipere(v));
    CREDO_VERUM (scaena_addere(v, _ch(c, "focus"), octeti));
    CREDO_VERUM (scaena_addere(v, _ch(c, "ostium"), octeti));
    CREDO_VERUM (volumen_in_transactione(v));
    CREDO_AEQUALIS_I32 (xar_numerus(scaena_enumerare(v, c->piscina)),
        II);
    CREDO_VERUM (volumen_transactionem_revolvere(v));
    CREDO_AEQUALIS_I32 (xar_numerus(scaena_enumerare(v, c->piscina)),
        ZEPHYRUM);
    /* sine transactione vocatoris: propria, clausa post */
    CREDO_VERUM (scaena_addere(v, _ch(c, "focus"), octeti));
    CREDO_FALSUM (volumen_in_transactione(v));
    CREDO_AEQUALIS_I32 (xar_numerus(scaena_enumerare(v, c->piscina)),
        I);
    volumen_claudere(v);
}

/* graphis-spec 23a: repositorium a statuis separatum */
interior vacuum
_sectio_separata (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "scaena_separata");
        Xar* index;
     chorda  statua      = _ch(c, "octeti statuae");
     chorda  prospectus  = _ch(c, "octeti prospectus");

    CREDO_NECESSE (v != NIHIL);
    CREDO_VERUM (statuae_addere(v, _ch(c, "golem"), statua));
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "silva"), statua,
        STATUAE_GENUS_LOCUS));
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "puteus"), statua,
        STATUAE_GENUS_RES));
    /* index prospectuum statuas non videt */
    CREDO_AEQUALIS_I32 (xar_numerus(scaena_enumerare(v, c->piscina)),
        ZEPHYRUM);
    CREDO_VERUM (scaena_promere(v, _ch(c, "golem"),
        c->piscina).datum == NIHIL);
    CREDO_FALSUM (scaena_removere(v, _ch(c, "golem")));
    /* titulus idem in utroque repositorio licet */
    CREDO_VERUM (scaena_addere(v, _ch(c, "golem"), prospectus));
    CREDO_VERUM (scaena_addere(v, _ch(c, "hut-door"), prospectus));
    index = scaena_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    /* index statuarum prospectus non videt, in genere nullo */
    index = statuae_enumerare(v, c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    CREDO_VERUM (chorda_aequalis(
        ((StatuaeInscriptio*)xar_obtinere(index, ZEPHYRUM))
            ->sigillum_hex, _sigillum(c, statua)));
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare_generis(v,
        STATUAE_GENUS_LOCUS, c->piscina)), I);
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare_generis(v,
        STATUAE_GENUS_RES, c->piscina)), I);
    /* remotio in uno alterum non tangit */
    CREDO_VERUM (scaena_removere(v, _ch(c, "golem")));
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare(v, c->piscina)),
        I);
    CREDO_VERUM (statuae_removere(v, _ch(c, "hut-door")));
    CREDO_AEQUALIS_I32 (xar_numerus(scaena_enumerare(v, c->piscina)),
        I);
    volumen_claudere(v);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "addere", _sectio_addere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "substitutio", _sectio_substitutio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "removere", _sectio_removere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tituli_mali", _sectio_tituli_mali, NIHIL, NIHIL, NIHIL, NIHIL },
    { "seq", _sectio_seq, NIHIL, NIHIL, NIHIL, NIHIL },
    { "transactio", _sectio_transactio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "separata", _sectio_separata, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("scaena_repositorium", SECTIONES);
}
