/* collidens_vendicatum.h - specimen capitis ALIENI (probatio
 * examinis MACRO_DOMESTICUM_IN_ALIENO): caput vanilla C89 quale
 * vendor scriberet - identificator 'C' (parametrum) cum macro
 * latinae collidit, ut sqlite3.h:8883 ('char C' -> 'char 100').
 * Sub ordine PRAVO (latina.h ante hoc) macro expanditur; solum
 * aut sub ordine recto = C89 validum purum. Bis adhibetur ut
 * dedup (unum diagnosticum per par) exerceatur. */
#ifndef COLLIDENS_VENDICATUM_H
#define COLLIDENS_VENDICATUM_H

typedef struct vendicatum_str vendicatum_str;

void vendicatum_appendchar(vendicatum_str* s, int N, char C);
void vendicatum_replicare(vendicatum_str* s, char C);

#endif /* COLLIDENS_VENDICATUM_H */
