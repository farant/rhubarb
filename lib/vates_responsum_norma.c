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
    Norma* nodi[12];

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
    nodi[11] = norma_liberum(piscina);
    norma_campus(nodi[0], "server_tool_use", nodi[11], FALSUM);
    norma_modus(nodi[0], NORMA_NOTANDUM);
    facta[1] = nodi[0];
    redde nodi[0];
}

interior Norma*
_struere_blocus (
    Piscina* piscina,
     Norma** facta)
{
    Norma* nodi[26];

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
    nodi[8] = norma_liberum(piscina);
    norma_campus(nodi[4], "caller", nodi[8], FALSUM);
    norma_modus(nodi[4], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "tool_use", nodi[4]);
    nodi[9] = norma_objectum(piscina);
    nodi[10] = norma_textus(piscina);
    norma_campus(nodi[9], "thinking", nodi[10], VERUM);
    nodi[11] = norma_textus(piscina);
    norma_campus(nodi[9], "signature", nodi[11], VERUM);
    norma_modus(nodi[9], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "thinking", nodi[9]);
    nodi[12] = norma_objectum(piscina);
    nodi[13] = norma_textus(piscina);
    norma_campus(nodi[12], "data", nodi[13], VERUM);
    norma_modus(nodi[12], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "redacted_thinking", nodi[12]);
    nodi[14] = norma_objectum(piscina);
    nodi[15] = norma_textus(piscina);
    norma_campus(nodi[14], "id", nodi[15], VERUM);
    nodi[16] = norma_textus(piscina);
    norma_campus(nodi[14], "name", nodi[16], VERUM);
    nodi[17] = norma_liberum(piscina);
    norma_campus(nodi[14], "input", nodi[17], VERUM);
    nodi[18] = norma_liberum(piscina);
    norma_campus(nodi[14], "caller", nodi[18], FALSUM);
    norma_modus(nodi[14], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "server_tool_use", nodi[14]);
    nodi[19] = norma_objectum(piscina);
    nodi[20] = norma_textus(piscina);
    norma_campus(nodi[19], "tool_use_id", nodi[20], VERUM);
    nodi[21] = norma_liberum(piscina);
    norma_campus(nodi[19], "content", nodi[21], VERUM);
    nodi[22] = norma_liberum(piscina);
    norma_campus(nodi[19], "caller", nodi[22], FALSUM);
    norma_modus(nodi[19], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "web_search_tool_result", nodi[19]);
    nodi[23] = norma_objectum(piscina);
    nodi[24] = norma_textus(piscina);
    norma_campus(nodi[23], "tool_use_id", nodi[24], VERUM);
    nodi[25] = norma_liberum(piscina);
    norma_campus(nodi[23], "content", nodi[25], VERUM);
    norma_modus(nodi[23], NORMA_NOTANDUM);
    norma_variatio(nodi[0], "code_execution_tool_result", nodi[23]);
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
