#ifndef VIA_H
#define VIA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ====================================================
 * VIA - Manipulatio Viarum Filorum
 *
 * Bibliotheca pro manipulatione viarum filorum
 * independens a platforma. Implementatio prima
 * pro Unix/macOS, sed API designata pro portabilitate.
 *
 * PHILOSOPHIA:
 * - Semper normalizat ad '/' (etiam in Windows postea)
 * - Omnes functiones allocant ex piscina
 * - Sequitur conventiones Unix pro casibus limitum
 * - API simplex et clara
 *
 * EXEMPLA:
 *   chorda partes[] = {
 *       chorda_ex_literis("/usr", p),
 *       chorda_ex_literis("local", p),
 *       chorda_ex_literis("bin", p)
 *   };
 *	 chorda via = via_inugere(partes, III, p);
 *	 // via = "/usr/local/bin"
 *
 *	 chorda dir = via_directorium(via, p); // "/usr/local"
 *	 chorda nom = via_nomen(via, p);
 *
 * ==================================================== */


/* ====================================================
 * Iunctio
 * ==================================================== */

/* Iungere partes viarum cum separatore
 *
 * partes: tabula chordarum ad iungendum
 * numerus: quot partes in tabula
 * piscina: piscina pro allocatione
 *
 * Reddit: chordam novam cum partibus iunctis
 *
 * EXEMPLUM:
 *   chorda p[] = {chorda_ex_literis("foo", psc),
 *   							 chorda_ex_literis("bar", psc)};
 *   chorda via = via_iungere(p, II, psc); // "foo/bar"
 */

chorda
via_iungere (
		constans chorda* partes,
							  i32  numerus,
						Piscina* piscina);


/* ====================================================
 * Normalizatio
 * ==================================================== */

/* Normalizere viam - resolvere . et .., removere // duplicatos
 * 
 * EXEMPLA:
 *   "foo/../bar" 	->  "bar"
 *   "foo/./bar"		->  "foo/bar"
 *   "foo//bar"			->  "foo/bar"
 *   "/foo/../bar" 	->	"/bar"
 *   "../foo" 			->  "../foo" (relativa manet)
 */
chorda
via_normalizare (
	   chorda  via,
    Piscina* piscina);

/* Convertere viam relativam ad absolutam
 *
 * Resolvit viam relativam contra directorium currens.
 * Si via iam absoluta est, normalizar solum.
 *
 * EXEMPLA:
 *   "foo/bar" -> "/current/working/dir/foo/bar"
 *   "/foo"		 -> "/foo"
 */
chorda
via_absoluta (
	      chorda  via,
	    Piscina* piscina);

/* ====================================================
 * Extractio
 * ==================================================== */

/* Extrahere directorium parentem (dirname)
 *
 * EXEMPLA:
 *   "/foo/bar/baz.txt" -> "/foo/bar"
 *   "/foo"							-> "/"
 *   "foo/bar" 					-> "foo"
 *   "foo.txt"					-> "."
 *   "/" 								-> "/"
 */
chorda
via_directorium (
		 chorda  via,
		Piscina* piscina);

/* Extrahere nomen fili (basename)
 *
 * EXEMPLA:
 *   "/foo/bar/baz.txt" -> "baz.txt"
 *   "/foo/bar/"				-> "foo"
 *   "/" 								-> ""
 */
chorda
via_nomen (
		 chorda  via,
		Piscina* piscina);

/* Extrahere nomen fili sine extensione (stem)
 *
 * EXEMPLA:
 *   "foo.txt" 				-> "foo"
 *   "archive.tar.gz" -> "archive.tar"
 *   ".bashrc" 				-> ".bashrc"
 *   "foo" 						-> "foo"
 */
chorda
via_nomen_radix (
		 chorda  via,
		Piscina* piscina);

/* Extrahere extensionem fili (includens punctum)
 *
 * Reddit extensionem ultimam solum.
 *
 * EXEMPLA:
 *   "foo.txt" 				-> ".txt"
 *   "archive.tar.gz" -> ".gz"
 *   ".bashrc" 				-> ""
 *   "foo" 						-> ""
 */
chorda
via_extensio (
		 chorda  via,
		Piscina* piscina);

/* ====================================================
 * Interrogatio
 * ==================================================== */

/* Verificare si via absoluta est
 *
 * Unix: incipt cum '/'
 * Windows (futura): incipit cum littera motoris (C:) vel UNC (\\)
 */
b32
via_est_absoluta (
		chorda via);

/* Verificare si via relativa est
 * Contrarium via_est_absoluta
 */
b32
via_est_relativa (
		chorda via);


/* ====================================================
 * Existentia
 * ==================================================== */

/* Verificare si filum existit
 *
 * via: via ad filum verificandum
 *
 * Reddit: VERUM si filum existit, FALSUM aliter
 *
 * NOTA: Solum verificat existentiam, non genus (directorium vel filum)
 */
b32
via_existit (
	chorda via);

/* ====================================================
 * Utilitas
 * ==================================================== */

/* Obtinere directorium currens laboris (CWD)
 *
 * Reddit: viam absolutam ad directorium currens
 * 		 		 chorda cum mensura=0 si errore
 *
 * NOTA: Usat getcwd() (Unix) vel GetCurrentDirectory() (Windows)
 */
chorda
via_directorium_currens (
		Piscina* piscina);

#endif /* VIA_H */
