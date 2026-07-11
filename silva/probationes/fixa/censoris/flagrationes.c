#include "latina.h"

/* CORPUS REGRESSIONIS CENSORIS - classis A (renominationes
 * SILENTES): omnis flagratio historica huius classis hic
 * reconstructa (flagrationes in git numquam commissae sunt -
 * intra sessiones sanatae; fixtura synthetica = corpus verum).
 *
 * Exemplar historicum: campus "exire" (flagratio IV, tessera
 * Phase C) - in omni TU modulari tacite "exit" renominatus,
 * suites virides, solum ordo capitum amalgamae rem fregit.
 *
 * EXSPECTANTUR: XIII flagrationes, ZEPHYRUM suspecta (benedictae
 * 2026-07-10, omnis ordo lectus): tag-def + tag-ref BIS + campus +
 * accessus BIS (exire); typedef-nomen + titulus (imprimere);
 * enumerator + parametrum in prototypo ET definitione (liberare);
 * localis (NIHIL); localis (memoriae_allocare).
 * (exceptiones probatae: principale, FILUM - tacere debent;
 * usus expressionis NIHIL linea 45 non flagrat.)
 * NB: plagula NUMQUAM compilatur - corpus parsationis solum. */

/* 1: tag structurae (exire -> exit) */
structura exire {
    /* 2: campus (flagratio IV historica ipsa) */
    integer exire;
    integer bene;
};

/* 3: nomen typi per typedef (imprimere -> printf) */
nomen structura exire imprimere;

/* 4: enumerator (liberare -> free) */
enumeratio { liberare = I, bene_valor = II };

/* exceptio FILUM: usus typi deliberatus - TACET */
hic_manens integer _adiutor(FILUM* pl, integer liberare);

/* 5: parametrum (liberare -> free) [in definitione infra] */
hic_manens integer
_adiutor(FILUM* pl, integer liberare)
{
    /* 6: variabilis localis (NIHIL -> NULL) - NB: usus eiusdem
     * verbi in expressione infra NON flagrat (positio expressionis
     * = consilium latinae; discrimen positionum in actu) */
    integer NIHIL = ZEPHYRUM;
    structura exire res;

    /* 7+8: accessus campi (exire -> exit), bis */
    res.exire = liberare;
    res.bene = NIHIL;
    si (pl == ((vacuum*)ZEPHYRUM)) redde res.exire;
    redde res.bene;
}

/* exceptio principale: omne programma id ligat - TACET */
integer principale(vacuum)
{
    /* 9: titulus (imprimere -> printf) */
imprimere:
    /* 10: variabilis localis (memoriae_allocare -> malloc) */
    {
        integer memoriae_allocare = I;

        redde _adiutor(((vacuum*)ZEPHYRUM), memoriae_allocare);
    }
}
