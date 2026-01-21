/* Phase 2 test: ID ID ID ... patterns (3+ identifiers in sequence)
 * Simulates latina.h patterns like: hic_manens constans character */

/* Three identifiers: storage type declarator */
MyStorage MyType myVar;

/* Four identifiers: storage qualifier type declarator */
MyStorage MyQual MyType myVar2;

/* Maximum test: 5 unknown identifiers followed by declarator */
MyA MyB MyC MyD myFinal;
