/* GENERATUM a bin/norma c ex probationes/fixa/norma/normae_fixae.norma - noli manu mutare */
#include "normae_fixae.h"

interior Norma* _struere_usus (Piscina* piscina, Norma** facta);
interior Norma* _struere_omnia (Piscina* piscina, Norma** facta);

interior Norma*
_struere_usus (
    Piscina* piscina,
     Norma** facta)
{
    Norma* nodi[5];

    si (facta[0])
    {
        redde facta[0];
    }
    nodi[0] = norma_objectum(piscina);
    nodi[1] = norma_integer(piscina);
    norma_intra(nodi[1], (s64)0L, (s64)1000000L);
    norma_campus(nodi[0], "input_tokens", nodi[1], VERUM);
    nodi[2] = norma_integer(piscina);
    norma_intra(nodi[2], (s64)0L, (s64)1000000L);
    norma_campus(nodi[0], "cache_read_input_tokens", nodi[2], FALSUM);
    nodi[3] = norma_numerus(piscina);
    norma_intra_fluitans(nodi[3], 0.1, 0.3);
    norma_campus(nodi[0], "ratio", nodi[3], VERUM);
    nodi[4] = norma_numerus(piscina);
    norma_intra_fluitans(nodi[4], -1e-300, 1e-300);
    norma_campus(nodi[0], "minima", nodi[4], FALSUM);
    norma_modus(nodi[0], NORMA_NOTANDUM);
    norma_descriptio(nodi[0], "Usus signorum.");
    facta[0] = nodi[0];
    redde nodi[0];
}

interior Norma*
_struere_omnia (
    Piscina* piscina,
     Norma** facta)
{
    Norma* nodi[22];

    si (facta[1])
    {
        redde facta[1];
    }
    nodi[0] = norma_objectum(piscina);
    nodi[1] = norma_liberum(piscina);
    norma_campus(nodi[0], "quodvis", nodi[1], VERUM);
    nodi[2] = norma_nullum(piscina);
    norma_campus(nodi[0], "nihil", nodi[2], VERUM);
    nodi[3] = norma_boolean(piscina);
    norma_aut_nullum(nodi[3]);
    norma_campus(nodi[0], "verum_falsum", nodi[3], VERUM);
    nodi[4] = norma_integer(piscina);
    norma_intra(nodi[4], (s64)-5L, (s64)5L);
    norma_campus(nodi[0], "a.b", nodi[4], VERUM);
    nodi[5] = norma_textus(piscina);
    norma_longitudo(nodi[5], (i32)1UL, (i32)40UL);
    norma_campus(nodi[0], "clāvis", nodi[5], VERUM);
    nodi[6] = norma_textus(piscina);
    norma_forma(nodi[6], "date-time");
    norma_campus(nodi[0], "dies", nodi[6], VERUM);
    nodi[7] = norma_textus(piscina);
    {
        constans character* constans licita[] = { "end_turn", "tool_use", "max_tokens", NIHIL };

        norma_electio(nodi[7], licita);
    }
    norma_aut_nullum(nodi[7]);
    norma_campus(nodi[0], "stop_reason", nodi[7], VERUM);
    nodi[8] = norma_textus(piscina);
    {
        constans character* constans licita[] = { "cum spatio", "sine", NIHIL };

        norma_electio(nodi[8], licita);
    }
    norma_campus(nodi[0], "spatiosa", nodi[8], VERUM);
    nodi[9] = norma_textus(piscina);
    norma_gignens(nodi[9], fixa_norma_gignens_ex_functione, NIHIL);
    norma_gignens_titulus(nodi[9], "ex_functione");
    norma_campus(nodi[0], "origo", nodi[9], VERUM);
    nodi[10] = norma_textus(piscina);
    {
        constans character* constans licita[] = { "a", "b", "c", NIHIL };

        norma_electio(nodi[10], licita);
    }
    nodi[11] = norma_tabulatum(piscina, nodi[10]);
    norma_longitudo(nodi[11], (i32)0UL, (i32)4UL);
    norma_campus(nodi[0], "tags", nodi[11], FALSUM);
    nodi[12] = _struere_usus(piscina, facta);
    norma_campus(nodi[0], "usus_primus", nodi[12], VERUM);
    nodi[13] = _struere_usus(piscina, facta);
    norma_campus(nodi[0], "usus_secundus", nodi[13], FALSUM);
    nodi[14] = norma_objectum(piscina);
    norma_modus(nodi[14], NORMA_APERTUM);
    norma_campus(nodi[0], "input", nodi[14], VERUM);
    nodi[15] = norma_discrimen(piscina, "type");
    nodi[16] = norma_objectum(piscina);
    nodi[17] = norma_textus(piscina);
    norma_campus(nodi[16], "text", nodi[17], VERUM);
    norma_variatio(nodi[15], "text", nodi[16]);
    nodi[18] = norma_objectum(piscina);
    nodi[19] = norma_textus(piscina);
    norma_campus(nodi[18], "id", nodi[19], VERUM);
    nodi[20] = norma_liberum(piscina);
    norma_campus(nodi[18], "input", nodi[20], VERUM);
    norma_variatio(nodi[15], "tool_use", nodi[18]);
    norma_modus(nodi[15], NORMA_NOTANDUM);
    nodi[21] = norma_tabulatum(piscina, nodi[15]);
    norma_campus(nodi[0], "content", nodi[21], VERUM);
    norma_aut_nullum(nodi[0]);
    facta[1] = nodi[0];
    redde nodi[0];
}

Norma*
fixa_norma_usus (
    Piscina* piscina)
{
    Norma* facta[2];
      i32  i;

    per (i = 0; i < 2; i++)
    {
        facta[i] = NIHIL;
    }
    redde _struere_usus(piscina, facta);
}

Norma*
fixa_norma_omnia (
    Piscina* piscina)
{
    Norma* facta[2];
      i32  i;

    per (i = 0; i < 2; i++)
    {
        facta[i] = NIHIL;
    }
    redde _struere_omnia(piscina, facta);
}
