#ifndef MACHO_H
#define MACHO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ==================================================
 * MACHO - Parsitio Filorum Mach-O
 *
 * Bibliotheca pro parsendo filos Mach-O (macOS/iOS binaria).
 * Sustinet filos singulos et crassos (fat binaries).
 *
 * PHILOSOPHIA:
 * - API duplex: MachoFilum (container) et MachO (imago singula)
 * - Omnia ex piscina allocata
 * - Errores fatales pro casibus non sustentis (nunc)
 * - Solum little-endian (detectio big-endian cum errore)
 * - Fat binaries in API, sed implementatio limitata nunc
 *
 * EXEMPLUM SIMPLEX:
 *   							 Piscina* p 	  = piscina_generare_dynamicum("macho", 1024*1024);
 *     							 MachO* macho = macho_aperire("/bin/ls", p);
 *   MachoIteratorMandatum  iter  = macho_iterator_mandatorum_initium(macho);
 *   			 MandatumOnustum* cmd;
 *
 *   imprimere("CPU: %d\n", macho_genus_processoris(macho));
 *   imprimere("64-bit: %s\n", macho_est_64bit(macho) ? "sic" : "non");
 *
 *   dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
 *   {
 *       i32 genus = mandatum_genus(cmd);
 *       imprimere("  Mandatum: 0x%x\n", genus);
 *   }
 *
 *   piscina_destruere(p);
 *
 * EXEMPLUM FAT BINARY:
 *   MachoFilum* filum   = macho_filum_aperire("/usr/bin/file", p);
 *   				i32  numerus = macho_filum_numerus_imago(filum);
 *
 *   per (i32 i = ZEPHYRUM; i < numerus; i++)
 *   {
 *       MachO* imago = macho_filum_imago(filum, i);
 *       imprimere("Architectura %d: CPU=%d\n", i,
 *                 macho_genus_processoris(imago));
 *   }
 *
 * ================================================== */


/* ==================================================
 * Typi Opaci
 * ================================================== */

/* MachoFilum - Container (singulus vel crassus/fat) */
nomen structura MachoFilum MachoFilum;

/* MachO - Imago singula Mach-O */
nomen structura MachO MachO;

/* MandatumOnustum - Mandatum oneris singulum (load command) */
nomen structura MandatumOnustum MandatumOnustum;


/* ==================================================
 * Constantae - Numeri Magici
 * ================================================== */

/* Numeri magici Mach-O */
#define MACHO_MAGICA_32           0xfeedface  /* 32-bit little-endian */
#define MACHO_MAGICA_64           0xfeedfacf  /* 64-bit little-endian */
#define MACHO_MAGICA_FAT          0xcafebabe  /* Fat binary */
#define MACHO_MAGICA_FAT_64       0xcafebabf  /* Fat binary 64-bit */

/* Numeri magici big-endian (pro detectione) */
#define MACHO_MAGICA_32_BYTESWAP  0xcefaedfe
#define MACHO_MAGICA_64_BYTESWAP  0xcffaedfe
#define MACHO_MAGICA_FAT_BYTESWAP 0xbebafeca


/* ==================================================
 * Constantae - Typi Processoris (CPU Types)
 * ================================================== */

#define MACHO_CPU_TYPE_X86       VII
#define MACHO_CPU_TYPE_X86_64    (VII | 0x01000000)
#define MACHO_CPU_TYPE_ARM       XII
#define MACHO_CPU_TYPE_ARM64     (XII | 0x01000000)
#define MACHO_CPU_TYPE_POWERPC   XVIII
#define MACHO_CPU_TYPE_POWERPC64 (XVIII | 0x01000000)


/* ==================================================
 * Constantae - Genera Filorum (File Types)
 * ================================================== */

nomen enumeratio {
	MACHO_GENUS_OBIECTUM    = 0x1,  /* MH_OBJECT - .o files */
	MACHO_GENUS_EXECUTABILE = 0x2,  /* MH_EXECUTE */
	MACHO_GENUS_FVMLIB      = 0x3,  /* MH_FVMLIB */
	MACHO_GENUS_CORE        = 0x4,  /* MH_CORE */
	MACHO_GENUS_PRELOAD     = 0x5,  /* MH_PRELOAD */
	MACHO_GENUS_DYLIB       = 0x6,  /* MH_DYLIB */
	MACHO_GENUS_DYLINKER    = 0x7,  /* MH_DYLINKER */
	MACHO_GENUS_BUNDLE      = 0x8,  /* MH_BUNDLE */
	MACHO_GENUS_DSYM        = 0xa   /* MH_DSYM */
} MachoGenusFilum;


/* ==================================================
 * Constantae - Genera Mandatorum Oneris (Load Commands)
 * ================================================== */

/* Mandatum oneris typi communes */
#define MACHO_LC_SEGMENT            	0x1   /* Segmentum 32-bit */
#define MACHO_LC_SYMTAB             	0x2   /* Tabula symbolorum */
#define MACHO_LC_THREAD             	0x4   /* Thread */
#define MACHO_LC_UNIXTHREAD         	0x5   /* Unix thread */
#define MACHO_LC_DYSYMTAB           	0xb   /* Tabula symbolorum dynamica */
#define MACHO_LC_LOAD_DYLIB         	0xc   /* Onerare dylib */
#define MACHO_LC_ID_DYLIB           	0xd   /* Identitas dylib */
#define MACHO_LC_LOAD_DYLINKER      	0xe   /* Onerare dynamic linker */
#define MACHO_LC_ID_DYLINKER        	0xf   /* Identitas dynamic linker */
#define MACHO_LC_LOAD_WEAK_DYLIB    	0x18  /* Onerare dylib debilis */
#define MACHO_LC_SEGMENT_64         	0x19  /* Segmentum 64-bit */
#define MACHO_LC_UUID               	0x1b  /* UUID */
#define MACHO_LC_CODE_SIGNATURE     	0x1d  /* Signatura codicis */
#define MACHO_LC_SEGMENT_SPLIT_INFO 	0x1e  /* Info fissionis segmenti */
#define MACHO_LC_REEXPORT_DYLIB     	(0x1f | 0x80000000)  /* Re-exportare dylib */
#define MACHO_LC_ENCRYPTION_INFO    	0x21  /* Info encryptionis */
#define MACHO_LC_DYLD_INFO          	0x22  /* Info dyld */
#define MACHO_LC_DYLD_INFO_ONLY     	0x22 | 0x80000000
#define MACHO_LC_VERSION_MIN_MACOSX   0x24  /* Versio minima macOS */
#define MACHO_LC_VERSION_MIN_IPHONEOS 0x25  /* Versio minima iOS */
#define MACHO_LC_FUNCTION_STARTS    	0x26  /* Initia functionum */
#define MACHO_LC_MAIN               	0x28  /* Entry point (LC_MAIN) */
#define MACHO_LC_DATA_IN_CODE       	0x29  /* Data in code */
#define MACHO_LC_SOURCE_VERSION     	0x2a  /* Versio fontis */
#define MACHO_LC_ENCRYPTION_INFO_64 	0x2c  /* Info encryptionis 64-bit */
#define MACHO_LC_BUILD_VERSION      	0x32  /* Versio aedificationis */


/* ==================================================
 * Creatio - Filum/Container
 * ================================================== */

/* Aperire filum Mach-O ex via
 *
 * Detectit automatice si singulus vel crassus (fat).
 * Legit totum filum in piscinam.
 *
 * via:     via ad filum
 * piscina: piscina pro allocationibus
 *
 * Reddit: MachoFilum* vel NIHIL si error
 *
 * NOTA: Exibit si big-endian vel malformatus
 */
MachoFilum*
macho_filum_aperire (
	constans character* via,
	           Piscina* piscina);

/* Aperire filum Mach-O ex memoria
 *
 * Parsat buffer iam existens. Vocans debet servare
 * buffer validum dum MachoFilum utitur.
 *
 * datum:   buffer continens datum Mach-O
 * mensura: mensura bufferis in bytes
 * piscina: piscina pro allocationibus
 *
 * Reddit: MachoFilum* vel NIHIL si error
 */
MachoFilum*
macho_filum_ex_memoria (
	   constans i8* datum,
	memoriae_index  mensura,
				 Piscina* piscina);


/* ==================================================
 * Interrogatio - Filum/Container
 * ================================================== */

/* Obtinere numerum imaginum in filo
 *
 * Reddit: I pro filo singulo, N>I pro fat binary
 *
 * NOTA: Si N > I, exibit (non sustentum nunc)
 */
i32
macho_filum_numerus_imago (
	constans MachoFilum* filum);

/* Obtinere imaginem Mach-O per indicem
 *
 * filum: filum Mach-O
 * index: index imaginis (0-based)
 *
 * Reddit: MachO* vel NIHIL si index invalidus
 */
MachO*
macho_filum_imago (
	constans MachoFilum* filum,
	                i32  index);

/* Obtinere typum processoris imaginis
 *
 * filum: filum Mach-O
 * index: index imaginis
 *
 * Reddit: MACHO_CPU_TYPE_* constans
 */
i32
macho_filum_genus_processoris (
	constans MachoFilum* filum,
	                i32  index);

/* Obtinere subtypum processoris imaginis
 *
 * filum: filum Mach-O
 * index: index imaginis
 *
 * Reddit: subtypus processoris
 */
i32
macho_filum_subgenus_processoris (
	constans MachoFilum* filum,
	                i32  index);


/* ==================================================
 * Creatio - Imago Singula (Convenientia)
 * ================================================== */

/* Aperire filum singulum Mach-O
 *
 * Convenientia pro filibus singularibus.
 * Interne vocat macho_filum_aperire(), verificat
 * numerus == I, reddit imaginem primam.
 *
 * via:     via ad filum
 * piscina: piscina pro allocationibus
 *
 * Reddit: MachO* vel NIHIL si error
 *
 * NOTA: Exibit si fat binary vel malformatus
 */
MachO*
macho_aperire (
	constans character* via,
	           Piscina* piscina);

/* Aperire ex memoria
 *
 * Convenientia pro buffer singularis.
 *
 * datum:   buffer continens datum Mach-O
 * mensura: mensura bufferis
 * piscina: piscina pro allocationibus
 *
 * Reddit: MachO* vel NIHIL si error
 */
MachO*
macho_ex_memoria (
	   constans i8* datum,
	memoriae_index  mensura,
				 Piscina* piscina);


/* ==================================================
 * Interrogatio - Header Mach-O
 * ================================================== */

/* Obtinere typum processoris
 *
 * Reddit: MACHO_CPU_TYPE_X86_64, etc.
 */
i32
macho_genus_processoris (
	constans MachO* macho);

/* Obtinere subtypum processoris
 *
 * Reddit: CPU_SUBTYPE_* valor
 */
i32
macho_subgenus_processoris (
	constans MachO* macho);

/* Obtinere typum fili
 *
 * Reddit: MACHO_GENUS_EXECUTABILE, MACHO_GENUS_DYLIB, etc.
 */
i32
macho_genus_filum (
	constans MachO* macho);

/* Obtinere numerum mandatorum oneris
 *
 * Reddit: numerus load commands
 */
i32
macho_numerus_mandatorum (
	constans MachO* macho);

/* Verificare si 64-bit
 *
 * Reddit: VERUM si 64-bit, FALSUM si 32-bit
 */
b32
macho_est_64bit (
	constans MachO* macho);

/* Obtinere piscinam Mach-O
 *
 * Reddit: piscinam usatam pro allocationibus
 *
 * NOTA: Utile pro bibliothecis dependentibus (sectio, symbola)
 *       quae debent allocare ex eadem piscina
 */
Piscina*
macho_piscina (
	constans MachO* macho);

/* Obtinere datum crudum Mach-O
 *
 * Reddit: indicem ad buffer datum
 *
 * NOTA: Utile pro zero-copy accessu ad sectiones et symbola
 *       Buffer possessus a piscina
 */
constans i8*
macho_datum (
	constans MachO* macho);

/* Obtinere mensuram datum Mach-O
 *
 * Reddit: mensuram buffer in bytes
 *
 * NOTA: Utile pro validatione limitum sectionum et offsetorum
 */
memoriae_index
macho_mensura (
	constans MachO* macho);


/* ==================================================
 * Interrogatio - Dependentiae et Metadatum
 * ================================================== */

/* Obtinere bibliothecas dynamicas onerandas
 *
 * Extrahit vias omnium bibliothecarum quas Mach-O onerat.
 * Includit LC_LOAD_DYLIB, LC_LOAD_WEAK_DYLIB, LC_REEXPORT_DYLIB.
 *
 * macho:   imago Mach-O
 * numerus: exitus - numerus bibliothecarum inventarum
 * piscina: piscina pro allocationibus
 *
 * Reddit: tabulam chordarum (viae bibliothecarum), vel NIHIL si error
 *
 * EXEMPLUM:
 *   i32 numerus;
 *   chorda* dylibs = macho_obtinere_dylibs(macho, &numerus, p);
 *   per (i32 i = ZEPHYRUM; i < numerus; i++)
 *   {
 *       imprimere("  %.*s\n", dylibs[i].mensura, dylibs[i].datum);
 *   }
 */
chorda*
macho_obtinere_dylibs (
	constans MachO* macho,
	           i32* numerus,
	       Piscina* piscina);

/* Obtinere punctum ingressus (entry point)
 *
 * Extrahit offsetum fili ad principale() ex LC_MAIN mandato.
 *
 * macho:  imago Mach-O
 * offset: exitus - offset ad principale() (ex __TEXT)
 *
 * Reddit: VERUM si LC_MAIN inventum, FALSUM alioquin
 *
 * NOTA: Filos veteres utuntur LC_UNIXTHREAD loco LC_MAIN.
 *       Hoc mandatum non sustinet LC_UNIXTHREAD.
 */
b32
macho_obtinere_entry_point (
	constans MachO* macho,
	memoriae_index* offset);

/* Obtinere UUID
 *
 * Extrahit UUID ex LC_UUID mandato.
 *
 * macho: imago Mach-O
 * uuid:  exitus - buffer pro XVI bytes UUID
 *
 * Reddit: VERUM si LC_UUID inventum, FALSUM alioquin
 *
 * EXEMPLUM:
 *   i8 uuid[XVI];
 *   si (macho_obtinere_uuid(macho, uuid))
 *   {
 *       imprimere("UUID: ");
 *       per (i32 i = ZEPHYRUM; i < XVI; i++)
 *       {
 *           imprimere("%02x", (insignatus character)uuid[i]);
 *       }
 *       imprimere("\n");
 *   }
 */
b32
macho_obtinere_uuid (
	constans MachO* macho,
	            i8  uuid[XVI]);


/* ==================================================
 * Iteratio - Mandata Oneris (Load Commands)
 * ================================================== */

/* Iterator pro ambulatione mandatorum oneris */
nomen structura MachoIteratorMandatum {
	constans MachO* macho;
	           i32  index_currens;
	           i32  offset_currens;
} MachoIteratorMandatum;

/* Initializare iteratorem
 *
 * macho: imago Mach-O
 *
 * Reddit: iterator ad initium
 */
MachoIteratorMandatum
macho_iterator_mandatorum_initium (
	constans MachO* macho);

/* Obtinere mandatum proximum
 *
 * iter: iterator
 *
 * Reddit: mandatum proximum, vel NIHIL si finis
 *
 * EXEMPLUM:
 *   MachoIteratorMandatum iter = macho_iterator_mandatorum_initium(m);
 *   MandatumOnustum* cmd;
 *   dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
 *   {
 *       // processa cmd
 *   }
 */
MandatumOnustum*
macho_iterator_mandatorum_proximum (
	MachoIteratorMandatum* iter);

/* Verificare si iteratio completa
 *
 * iter: iterator
 *
 * Reddit: VERUM si finis, FALSUM alioquin
 */
b32
macho_iterator_mandatorum_finis (
	MachoIteratorMandatum* iter);


/* ==================================================
 * Interrogatio - Mandata Oneris
 * ================================================== */

/* Obtinere typum mandati
 *
 * mandatum: mandatum oneris
 *
 * Reddit: MACHO_LC_SEGMENT_64, MACHO_LC_SYMTAB, etc.
 */
i32
mandatum_genus (
	constans MandatumOnustum* mandatum);

/* Obtinere mensuram mandati
 *
 * mandatum: mandatum oneris
 *
 * Reddit: mensura totalis mandati in bytes
 */
memoriae_index
mandatum_magnitudo (
	constans MandatumOnustum* mandatum);

/* Obtinere datum crudum mandati
 *
 * Pro parsitione specifica typi in sectio.h / symbola.h
 *
 * mandatum: mandatum oneris
 *
 * Reddit: index ad datum crudum mandati
 *
 * NOTA: Vocans debet convertere ad typum correctum
 *       basatum in mandatum_genus()
 */
constans vacuum*
mandatum_datum (
	constans MandatumOnustum* mandatum);


/* ==================================================
 * Errores
 * ================================================== */

/* Obtinere nuntium erroris recens
 *
 * Reddit: literas describens errorem, vel NIHIL si nullus error
 *
 * NOTA: Nuntius non possessus a vocante (static buffer)
 */
constans character*
macho_error_recens (
	vacuum);

#endif /* MACHO_H */
