/* tabula_monstratorum.c - tabula monstratorum (vide caput) */
#include "tabula_monstratorum.h"

/* limes ne (numerus + I) * IV in i32 exundet: II^XXVIII loci
 * (IV GB locorum) */
#define LOCI_MAXIMI ((i32)0x10000000)

nomen structura {
    constans vacuum* clavis;   /* NIHIL = locus vacuus */
             vacuum* valor;
} LocusMonstratoris;

structura TabulaMonstratorum {
    LocusMonstratoris* loci;
                  i32  capacitas;   /* potentia II */
                  i32  larva;       /* capacitas - I */
                  i32  numerus;
              Piscina* piscina;
};


/* Friatio monstratoris. Bita infima (IV) alineationis sunt et
 * omittuntur; bita alta (>> XXXII per duos gradus, ut memoriae_index
 * XXXII bitorum tutum sit) miscentur. Index loci = friatum & larva,
 * ergo bita INFIMA bene misceri debent - multiplicatio sola non
 * sufficit (bita alta solum miscet). */
interior i32
_monstratorem_friare (
    constans vacuum* clavis)
{
    memoriae_index x = (memoriae_index)clavis;
               i32 h;

    h = (i32)(x >> IV) ^ (i32)((x >> XVI) >> XVI);
    h ^= h >> XVI;
    h *= 0x45d9f3bU;
    h ^= h >> XVI;
    redde h;
}

/* locus clavis aut locus vacuus primus in serie eius (factor oneris
 * < I semper locum vacuum relinquit, ergo ansa finitur) */
interior i32
_locum_quaerere (
    constans LocusMonstratoris* loci,
                           i32  larva,
               constans vacuum* clavis)
{
    i32 i = _monstratorem_friare(clavis) & larva;

    dum (loci[i].clavis != NIHIL && loci[i].clavis != clavis)
    {
        i = (i + I) & larva;
    }
    redde i;
}

interior LocusMonstratoris*
_locos_allocare (
    Piscina* piscina,
        i32  capacitas)
{
    LocusMonstratoris* loci;
                  i32  i;

    loci = (LocusMonstratoris*)piscina_conari_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(LocusMonstratoris));
    si (loci == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < capacitas; i++)
    {
        loci[i].clavis  = NIHIL;
        loci[i].valor   = NIHIL;
    }
    redde loci;
}

TabulaMonstratorum*
tabula_monstratorum_creare (
    Piscina* piscina,
        i32  capacitas)
{
    TabulaMonstratorum* tabula;
                   i32  c = XVI;

    si (piscina == NIHIL || capacitas > LOCI_MAXIMI)
    {
        redde NIHIL;
    }
    dum (c < capacitas)
    {
        c *= II;
    }
    tabula = (TabulaMonstratorum*)piscina_conari_allocare(piscina,
        magnitudo(TabulaMonstratorum));
    si (tabula == NIHIL)
    {
        redde NIHIL;
    }
    tabula->loci = _locos_allocare(piscina, c);
    si (tabula->loci == NIHIL)
    {
        redde NIHIL;
    }
    tabula->capacitas  = c;
    tabula->larva      = c - I;
    tabula->numerus    = ZEPHYRUM;
    tabula->piscina    = piscina;
    redde tabula;
}

vacuum*
tabula_monstratorum_invenire (
    constans TabulaMonstratorum* tabula,
                constans vacuum* clavis)
{
    i32 i;

    si (tabula == NIHIL || clavis == NIHIL)
    {
        redde NIHIL;
    }
    i = _locum_quaerere(tabula->loci, tabula->larva, clavis);
    redde tabula->loci[i].valor;   /* locus vacuus: NIHIL */
}

/* duplicatio: loci novi, claves iterum positae (loci veteres in
 * piscina manent) */
interior b32
_tabulam_monstratorum_crescere (
    TabulaMonstratorum* tabula)
{
    LocusMonstratoris* novi;
                  i32  capacitas;
                  i32  larva;
                  i32  k;

    si (tabula->capacitas >= LOCI_MAXIMI)
    {
        redde FALSUM;
    }
    capacitas  = tabula->capacitas * II;
    larva      = capacitas - I;
    novi       = _locos_allocare(tabula->piscina, capacitas);
    si (novi == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < tabula->capacitas; k++)
    {
        si (tabula->loci[k].clavis != NIHIL)
        {
            novi[_locum_quaerere(novi, larva, tabula->loci[k].clavis)]
                = tabula->loci[k];
        }
    }
    tabula->loci       = novi;
    tabula->capacitas  = capacitas;
    tabula->larva      = larva;
    redde VERUM;
}

vacuum**
tabula_monstratorum_locus (
    TabulaMonstratorum* tabula,
       constans vacuum* clavis)
{
    i32 i;

    si (tabula == NIHIL || clavis == NIHIL)
    {
        redde NIHIL;
    }
    i = _locum_quaerere(tabula->loci, tabula->larva, clavis);
    si (tabula->loci[i].clavis == clavis)
    {
        redde &tabula->loci[i].valor;   /* praesens: numquam crescit */
    }
    si ((tabula->numerus + I) * IV > tabula->capacitas * III)
    {
        si (!_tabulam_monstratorum_crescere(tabula))
        {
            redde NIHIL;
        }
        i = _locum_quaerere(tabula->loci, tabula->larva, clavis);
    }
    tabula->loci[i].clavis  = clavis;
    tabula->loci[i].valor   = NIHIL;
    tabula->numerus++;
    redde &tabula->loci[i].valor;
}

i32
tabula_monstratorum_numerus (
    constans TabulaMonstratorum* tabula)
{
    si (tabula == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde tabula->numerus;
}
