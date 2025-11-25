#ifndef FILUM_H
#define FILUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ==================================================
 * Lectio Simplex
 * ================================================== */

/* Legere filum totum in chordam (ex piscina allocatum)
 * Reddit chordam cum mensura=0 si defectu */
chorda
filum_legere_totum (
	constans character* via,
	           Piscina* piscina);

/* Verificare si filum existit */
b32
filum_existit (
	constans character* via);

/* Cape mensuram fili in bytes (reddit 0 si defectu) */
memoriae_index
filum_mensura (
	constans character* via);


/* ==================================================
 * Lectio per Lineas
 * ================================================== */

nomen structura FilumLector FilumLector;

/* Aperire filum pro lectione per lineas
 * Reddit NIHIL si defectu */
FilumLector*
filum_lector_aperire (
	constans character* via,
	           Piscina* piscina);

/* Legere lineam proximam (sine charactere lineae novae)
 * Reddit VERUM si linea lecta, FALSUM ad finem vel errore
 * Linea allocata ex piscina */
b32
filum_lector_lineam_proximam (
	     FilumLector* lector,
	          chorda* linea_out);

/* Cape numerum versus currentem (pro errore reportando) */
i32
filum_lector_numerus_versus (
	FilumLector* lector);

/* Verificare si ad finem fili */
b32
filum_lector_finis (
	FilumLector* lector);

/* Claudere lectorem et liberare res
 * Nota: chordae ex piscina manent validae */
vacuum
filum_lector_claudere (
	FilumLector* lector);


/* ==================================================
 * Scriptio
 * ================================================== */

/* Modus aperire pro FilumScriptor */
nomen enumeratio {
	FILUM_MODUS_CREARE,   /* Creare/superscribere filum */
	FILUM_MODUS_APPENDERE /* Appendere ad filum existens */
} FilumModus;

nomen structura FilumScriptor FilumScriptor;

/* Aperire filum pro scriptione continua
 * Reddit NIHIL si defectu */
FilumScriptor*
filum_scriptor_aperire (
	constans character* via,
	         FilumModus modus,
	           Piscina* piscina);

/* Scribere chordam ad filum apertum */
b32
filum_scriptor_scribere (
	FilumScriptor* scriptor,
	        chorda contentum);

/* Scribere literas C ad filum apertum */
b32
filum_scriptor_scribere_literis (
	     FilumScriptor* scriptor,
	constans character* contentum);

/* Scribere lineam cum newline ad finem */
b32
filum_scriptor_lineam_scribere (
	     FilumScriptor* scriptor,
	constans character* linea);

/* Sync ad discum (fflush) */
b32
filum_scriptor_sync (
	FilumScriptor* scriptor);

/* Claudere scriptorem et liberare res */
vacuum
filum_scriptor_claudere (
	FilumScriptor* scriptor);


/* Scribere chordam ad filum (creare/superscribere)
 * Reddit VERUM in successu */
b32
filum_scribere (
	constans character* via,
	            chorda  contentum);

/* Scribere literas C ad filum (creare/superscribere) */
b32
filum_scribere_literis (
	constans character* via,
	constans character* contentum);

/* Appendere chordam ad filum existens */
b32
filum_appendere (
	constans character* via,
	            chorda  contentum);

/* Appendere literas C ad filum existens */
b32
filum_appendere_literis (
	constans character* via,
	constans character* contentum);


/* ==================================================
 * Manipulatio Filorum
 * ================================================== */

/* Delere filum
 * Reddit VERUM in successu */
b32
filum_delere (
	constans character* via);

/* Renominare/movere filum */
b32
filum_movere (
	constans character* via_vetus,
	constans character* via_nova);

/* Copiare filum */
b32
filum_copiare (
	constans character* via_fons,
	constans character* via_destinatio);


/* ==================================================
 * Interrogatio Status
 * ================================================== */

nomen structura {
	memoriae_index  mensura;         /* Mensura in bytes */
	           b32  est_directorium; /* Est directorium? */
	           b32  est_filum;       /* Est filum regulare? */
	           b32  potest_legere;   /* Legibile? */
	           b32  potest_scribere; /* Scribabile? */
} FilumStatus;

/* Cape statum/informationem fili
 * Reddit VERUM si filum existit et status captus */
b32
filum_status (
	constans character* via,
	       FilumStatus* status_out);


/* ==================================================
 * Errores
 * ================================================== */

/* Cape nuntium erroris recens (literas C, non possessus a vocante)
 * Reddit errorem descriptivum vel NIHIL si nullus error */
constans character*
filum_error_recens (
	vacuum);

#endif /* FILUM_H */
