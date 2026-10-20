/* sonda r11: arbor2_nt_nomen / arbor2_nodus_genus_nomen pro omnibus valoribus enumerationis */
#include "arbor2_glr_communia.inc"

integer
principale(vacuum)
{
    integer v;
    integer ignota = 0;
    per (v = (integer)ARBOR2_NT_VIRGA; v <= (integer)ARBOR2_NT_INIT_DECLARATOR_LIST; v++)
    {
        constans character* t = arbor2_nt_nomen((Arbor2NonTerminalis)v);
        si (strcmp(t, "IGNOTUM") == 0) { ignota++; fprintf(stderr, "SONDA> NT %d -> %s\n", v, t); }
    }
    fprintf(stderr, "SONDA> NT valores %d, IGNOTUM %d\n", (integer)ARBOR2_NT_INIT_DECLARATOR_LIST + 1, ignota);
    ignota = 0;
    per (v = (integer)ARBOR2_NODUS_IDENTIFICATOR; v <= (integer)ARBOR2_NODUS_PRAGMA; v++)
    {
        constans character* t = arbor2_nodus_genus_nomen((Arbor2NodusGenus)v);
        si (strcmp(t, "IGNOTUM") == 0) { ignota++; fprintf(stderr, "SONDA> NODUS %d -> %s\n", v, t); }
    }
    fprintf(stderr, "SONDA> NODUS valores %d, IGNOTUM %d (INCLUDE=%d DEFINE=%d UNDEF=%d PRAGMA=%d)\n",
        (integer)ARBOR2_NODUS_PRAGMA + 1, ignota, (integer)ARBOR2_NODUS_INCLUDE, (integer)ARBOR2_NODUS_DEFINE,
        (integer)ARBOR2_NODUS_UNDEF, (integer)ARBOR2_NODUS_PRAGMA);
    redde ZEPHYRUM;
}
