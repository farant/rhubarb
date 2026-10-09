/* GENERATUM a bin/norma c ex lib/vates_responsum.norma - noli manu mutare */
#include "vates_responsum_norma.h"

interior Norma* _struere_responsum (Piscina* piscina, Norma** facta);
interior Norma* _struere_usus (Piscina* piscina, Norma** facta);
interior Norma* _struere_blocus (Piscina* piscina, Norma** facta);

interior Norma*
_struere_responsum (
    Piscina* piscina,
     Norma** facta)
{
    Norma* nodi[13];

    si (facta[0])
    {
        redde facta[0];
    }
    nodi[0] = norma_objectum(piscina);
    nodi[1] = norma_textus(piscina);
    norma_campus(nodi[0], "id", nodi[1], VERUM);
    nodi[2] = norma_textus(piscina);
    norma_campus(nodi[0], "type", nodi[2], VERUM);
    nodi[3] = norma_textus(piscina);
    norma_campus(nodi[0], "role", nodi[3], VERUM);
    nodi[4] = norma_textus(piscina);
    norma_campus(nodi[0], "model", nodi[4], VERUM);
    nodi[5] = _struere_blocus(piscina, facta);
    nodi[6] = norma_tabulatum(piscina, nodi[5]);
    norma_campus(nodi[0], "content", nodi[6], VERUM);
    nodi[7] = norma_textus(piscina);
    norma_aut_nullum(nodi[7]);
    norma_campus(nodi[0], "stop_reason", nodi[7], VERUM);
    nodi[8] = norma_textus(piscina);
    norma_aut_nullum(nodi[8]);
    norma_campus(nodi[0], "stop_sequence", nodi[8], FALSUM);
    nodi[9] = norma_liberum(piscina);
    norma_campus(nodi[0], "stop_details", nodi[9], FALSUM);
    nodi[10] = _struere_usus(piscina, facta);
    norma_campus(nodi[0], "usage", nodi[10], VERUM);
    nodi[11] = norma_liberum(piscina);
    norma_campus(nodi[0], "container", nodi[11], FALSUM);
    nodi[12] = norma_liberum(piscina);
    norma_campus(nodi[0], "diagnostics", nodi[12], FALSUM);
    norma_modus(nodi[0], NORMA_NOTANDUM);
    facta[0] = nodi[0];
    redde nodi[0];
}

interior Norma*
_struere_usus (
    Piscina* piscina,
     Norma** facta)
{
    Norma* nodi[11];

    si (facta[1])
    {
        redde facta[1];
    }
    nodi[0] = norma_objectum(piscina);
    nodi[1] = norma_integer(piscina);
    norma_campus(nodi[0], "input_tokens", nodi[1], VERUM);
    nodi[2] = norma_integer(piscina);
    norma_campus(nodi[0], "output_tokens", nodi[2], VERUM);
    nodi[3] = norma_integer(piscina);
    norma_campus(nodi[0], "cache_read_input_tokens", nodi[3], FALSUM);
    nodi[4] = norma_integer(piscina);
    norma_campus(nodi[0], "cache_creation_input_tokens", nodi[4], FALSUM);
    nodi[5] = norma_objectum(piscina);
    nodi[6] = norma_integer(piscina);
    norma_campus(nodi[5], "ephemeral_5m_input_tokens", nodi[6], FALSUM);
    nodi[7] = norma_integer(piscina);
    norma_campus(nodi[5], "ephemeral_1h_input_tokens", nodi[7], FALSUM);
    norma_modus(nodi[5], NORMA_NOTANDUM);
    norma_campus(nodi[0], "cache_creation", nodi[5], FALSUM);
    nodi[8] = norma_liberum(piscina);
    norma_campus(nodi[0], "output_tokens_details", nodi[8], FALSUM);
    nodi[9] = norma_textus(piscina);
    norma_aut_nullum(nodi[9]);
    norma_campus(nodi[0], "service_tier", nodi[9], FALSUM);
    nodi[10] = norma_textus(piscina);
    norma_aut_nullum(nodi[10]);
    norma_campus(nodi[0], "inference_geo", nodi[10], FALSUM);
    norma_modus(nodi[0], NORMA_NOTANDUM);
    facta[1] = nodi[0];
    redde nodi[0];
}

interior Norma*
_struere_blocus (
    Piscina* piscina,
     Norma** facta)
{
    Norma* nodi[13];

    si (facta[2])
    {
        redde facta[2];
    }
    nodi[0] = norma_discrimen(piscina, "type");
    nodi[1] = norma_objectum(piscina);
    nodi[2] = norma_textus(piscina);
    norma_campus(nodi[1], "text", nodi[2], VERUM);
    nodi[3] = norma_liberum(piscina);
    norma_campus(nodi[1], "citations", nodi[3], FALSUM);
    norma_modus(nodi[1], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "text", nodi[1]);
    nodi[4] = norma_objectum(piscina);
    nodi[5] = norma_textus(piscina);
    norma_campus(nodi[4], "id", nodi[5], VERUM);
    nodi[6] = norma_textus(piscina);
    norma_campus(nodi[4], "name", nodi[6], VERUM);
    nodi[7] = norma_liberum(piscina);
    norma_campus(nodi[4], "input", nodi[7], VERUM);
    norma_modus(nodi[4], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "tool_use", nodi[4]);
    nodi[8] = norma_objectum(piscina);
    nodi[9] = norma_textus(piscina);
    norma_campus(nodi[8], "thinking", nodi[9], VERUM);
    nodi[10] = norma_textus(piscina);
    norma_campus(nodi[8], "signature", nodi[10], VERUM);
    norma_modus(nodi[8], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "thinking", nodi[8]);
    nodi[11] = norma_objectum(piscina);
    nodi[12] = norma_textus(piscina);
    norma_campus(nodi[11], "data", nodi[12], VERUM);
    norma_modus(nodi[11], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "redacted_thinking", nodi[11]);
    norma_modus(nodi[0], NORMA_NOTANDUM);
    facta[2] = nodi[0];
    redde nodi[0];
}

Norma*
vates_norma_responsum (
    Piscina* piscina)
{
    Norma* facta[3];
      i32  i;

    per (i = 0; i < 3; i++)
    {
        facta[i] = NIHIL;
    }
    redde _struere_responsum(piscina, facta);
}

Norma*
vates_norma_usus (
    Piscina* piscina)
{
    Norma* facta[3];
      i32  i;

    per (i = 0; i < 3; i++)
    {
        facta[i] = NIHIL;
    }
    redde _struere_usus(piscina, facta);
}

Norma*
vates_norma_blocus (
    Piscina* piscina)
{
    Norma* facta[3];
      i32  i;

    per (i = 0; i < 3; i++)
    {
        facta[i] = NIHIL;
    }
    redde _struere_blocus(piscina, facta);
}
