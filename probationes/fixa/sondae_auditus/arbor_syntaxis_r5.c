#include "arbor_syntaxis_communia.inc"
integer principale(vacuum)
{
    imprimere("DIRECTIVE -> \"%s\"\n", arbor_nodus_genus_nomen(ARBOR_NODUS_DIRECTIVE));
    imprimere("ERROR -> \"%s\"\n", arbor_nodus_genus_nomen(ARBOR_NODUS_ERROR));
    redde 0;
}
