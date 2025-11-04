/* xar.c - "Codex Tabulae Exponentialis" - Implementatio
 *
 * Tabula crescens sine reallocatio
 */

#include "xar.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ========================================================================
 * STRUCTURAE INTERNAE
 * ======================================================================== */

/* XarLocatio - Locatio elementi in structura segmentata (interior)
 */
nomen structura XarLocatio {
		 i32  index_segmenti;         /* Quis segmentum? */
		 i32  offset_in_segmento;     /* Ubi in segmento? */
		 i32  magnitudo_segmenti;     /* Quam magnus? */
	vacuum* basis_segmenti;         /* Initium segmenti */
} XarLocatio;

/* ========================================================================
 * FUNCTIONES AUXILIARES
 * ======================================================================== */

/* Computare Magnitudinem Segmenti
 * "Computare magnitudinem segmenti usans formam conservatam"
 *
 * Notitia clavis: duo prima segmenta eadem magnitudo, deinde duplicatio
 * "Regula duplicandi"
 */
interior i32
computare_magnitudinem_segmenti(
    constans Xar* xar,
             i32  index_segmenti)
{
	i32 shift_amount;

	si (index_segmenti <= I)
    {
		/* Duo prima segmenta: eadem magnitudo */
		redde xar->magnitudo_primi;
	}

	shift_amount = index_segmenti - I;

	/* Impossibilis cum indices i32 validi
	 * Cum magnitudo_primi = XVI et indices i32 (max ~2^31),
	 * numquam opus est plus quam ~27-30 segmenta.
	 * Si hic pervenis, corruptio structurae vel error gravis.
	 */
	si (shift_amount >= XXX)
    {
		imprimere("FRACTA: xar segmentum nimis altum: %d\n", index_segmenti);
		imprimere("        (impossibilis cum indices i32 - corruptio?)\n");
		exire(I);
	}

	/* Segmenta sequentia: duplicant */
	redde xar->magnitudo_primi << shift_amount;
}

/* Allocare Segmentum
 * "Partem allocare"
 *
 * Redde: VERUM si successus, FALSUM si error
 */
interior b32
allocare_segmentum(
    Xar* xar,
    i32  index_segmenti)
{
	           i32  magnitudo_segmenti;
	memoriae_index  magnitudo_memoriae;
	        vacuum* memoria;

	si (index_segmenti >= XAR_MAXIMUS_SEGMENTORUM)
    {
		redde FALSUM;  /* Nimis multa segmenta! */
	}

	si (xar->segmenta[index_segmenti])
    {
		redde VERUM;  /* Iam allocatus */
	}

	/* Computare magnitudinem segmenti */
	magnitudo_segmenti = computare_magnitudinem_segmenti(xar, index_segmenti);
	magnitudo_memoriae = (memoriae_index)magnitudo_segmenti * xar->magnitudo_elementi;

	/* Allocare ex piscina */
	memoria = piscina_allocare(xar->piscina, magnitudo_memoriae);
	si (!memoria)
    {
		redde FALSUM;  /* Allocatio fracta */
	}

	/* Zephyrum memoriam si vexillum non ponit */
	si (!(xar->vexilla & XAR_VEXILLUM_SINE_ZEPHYRUM))
    {
		memset(memoria, ZEPHYRUM, magnitudo_memoriae);
	}

	/* Renovare statum xar */
	xar->segmenta[index_segmenti] = memoria;
	si (index_segmenti >= xar->numerus_segmentorum)
    {
		xar->numerus_segmentorum = index_segmenti + I;
	}
	/* Incrementaliter renovare capacitatem - O(1) non O(n) */
	xar->capacitas_totalis += magnitudo_segmenti;

	redde VERUM;
}

/* ========================================================================
 * FUNCTIONES CREATIONIS
 * ======================================================================== */

/* Xar Creare */
Xar*
xar_creare(
    Piscina* piscina,
        i32  magnitudo_elementi)
{
	redde xar_creare_cum_vexillis(piscina,
	                               magnitudo_elementi,
	                               XAR_PRIMUS_SEGMENTUM,
	                               XAR_VEXILLUM_ORDINARIUS);
}

/* Xar Creare Cum Magnitudine */
Xar*
xar_creare_cum_magnitudine(
    Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi)
{
	redde xar_creare_cum_vexillis(piscina,
	                               magnitudo_elementi,
	                               magnitudo_primi,
	                               XAR_VEXILLUM_ORDINARIUS);
}

/* Xar Creare Cum Vexillis
 * "Genesis tabulae exponentialis - cum omnibus optionibus"
 */
Xar*
xar_creare_cum_vexillis(
    Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla)
{
	Xar* xar;
	i32  i;

	si (!piscina || magnitudo_elementi == ZEPHYRUM)
    {
		redde NIHIL;
	}

	/* Allocare structuram xar ex piscina */
	xar = (Xar*)piscina_allocare_ordinatum(piscina, magnitudo(Xar), magnitudo(Xar));
	si (!xar)
    {
		redde NIHIL;
	}

	/* Initializare metadatum
	 * "Initium notitiae"
	 */
	xar->numerus_elementorum = ZEPHYRUM;
	xar->magnitudo_elementi  = magnitudo_elementi;
	xar->magnitudo_primi     = magnitudo_primi ? magnitudo_primi : XAR_PRIMUS_SEGMENTUM;
	xar->numerus_segmentorum = ZEPHYRUM;
	xar->capacitas_totalis   = ZEPHYRUM;
	xar->vexilla             = vexilla;
	xar->piscina             = piscina;

	/* Vacuare segmenta
	 * "Vacuare omnes indices segmentorum"
	 */
	per (i = ZEPHYRUM; i < XAR_MAXIMUS_SEGMENTORUM; i++)
    {
		xar->segmenta[i] = NIHIL;
	}

	/* Ponere titulus */
	strcpy(xar->titulus, "Xar Anonymus");

	redde xar;
}

/* Xar Destruere */
vacuum
xar_destruere(
    Xar* xar)
{
	/* Cum piscina, nihil agendum!
	 */
	(vacuum)xar;
}

/* Xar Ponere Vexilla */
vacuum
xar_ponere_vexilla(
    Xar* xar,
    i32  vexilla)
{
	si (xar)
    {
		xar->vexilla = vexilla;
	}
}

/* ========================================================================
 * LOCATIO ET ACCESSUS
 * ======================================================================== */

/* Xar Locare - Algorithmus O(1)! (interior)
 * "Invenire locum indicis"
 *
 * ALGORITHMUS:
 * 1. Via rapida pro primis duobus segmentis (computatio directa)
 * 2. Quaestio exponentia pro ceteris (sine circulatio per omnia segmenta)
 */
interior b32
xar_locare(
    constans     Xar* xar,
                 i32  index,
          XarLocatio* locatio)
{
	i32 index_segmenti;
	i32 magnitudo_segmenti;
	i32 index_adiustus;

	si (!xar || !locatio)
    {
		redde FALSUM;
	}

	/* Via rapida pro primis duobus segmentis (eadem magnitudo)
	 */
	si (index < xar->magnitudo_primi * II)
    {
		si (index < xar->magnitudo_primi)
        {
			/* Segmentum primum */
			locatio->index_segmenti     = ZEPHYRUM;
			locatio->offset_in_segmento = index;
			locatio->magnitudo_segmenti = xar->magnitudo_primi;
		}
        alioquin
        {
			/* Segmentum secundum */
			locatio->index_segmenti     = I;
			locatio->offset_in_segmento = index - xar->magnitudo_primi;
			locatio->magnitudo_segmenti = xar->magnitudo_primi;
		}
		locatio->basis_segmenti = xar->segmenta[locatio->index_segmenti];
		redde VERUM;
	}

	/* Computatio directa pro segmentis exponentialibus
	 *
	 * Usans manipulationem bitorum pro quaestio exponentia
	 */
	index_adiustus     = index - (xar->magnitudo_primi * II);
	index_segmenti     = II;
	magnitudo_segmenti = xar->magnitudo_primi * II;  /* Magnitudo segmenti 2 */

	/* Invenire segmentum usans formam crescentiae exponentialem
	 */
	dum (index_adiustus >= magnitudo_segmenti &&
         index_segmenti < XAR_MAXIMUS_SEGMENTORUM)
    {
		index_adiustus -= magnitudo_segmenti;

		/* Verificare overflow ante duplicatio
		 * Si magnitudo_segmenti > 2^30, duplicatio overflow facit
		 */
		si (magnitudo_segmenti > (0x7FFFFFFF >> I))
        {
			imprimere("FRACTA: xar magnitudo segmenti overflow: %d\n",
			          magnitudo_segmenti);
			imprimere("        (impossibilis cum indices i32)\n");
			exire(I);
		}

		magnitudo_segmenti <<= I;  /* Duplicare pro segmento proximo */
		index_segmenti++;
	}

	si (index_segmenti >= XAR_MAXIMUS_SEGMENTORUM)
    {
		redde FALSUM;  /* Index nimis magnus */
	}

	locatio->index_segmenti     = index_segmenti;
	locatio->offset_in_segmento = index_adiustus;
	locatio->magnitudo_segmenti = computare_magnitudinem_segmenti(xar, index_segmenti);
	locatio->basis_segmenti     = xar->segmenta[index_segmenti];

	redde VERUM;
}

/* Xar Obtinere
 * "Obtinere elementum"
 */
vacuum*
xar_obtinere(
    constans Xar* xar,
             i32  index)
{
	XarLocatio  locatio;
	        i8* basis;

	si (!xar || index >= xar->numerus_elementorum)
    {
		redde NIHIL;
	}

	si (!xar_locare(xar, index, &locatio))
    {
		redde NIHIL;
	}

	si (!locatio.basis_segmenti)
    {
		redde NIHIL;  /* Segmentum non allocatus */
	}

	/* Computare locum elementi
	 */
	basis = (i8*)locatio.basis_segmenti;
	redde basis + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* Xar Obtinere Vel Creare
 * "Utile pro tabulis dispersis"
 */
vacuum*
xar_obtinere_vel_creare(
    Xar* xar,
    i32  index)
{
	XarLocatio  locatio;
	        i8* basis;
	       i32  i;

	si (!xar)
    {
		redde NIHIL;
	}

	/* Invenire locatio */
	si (!xar_locare(xar, index, &locatio))
    {
		redde NIHIL;
	}

	/* Assecurare omnia segmenta usque ad hoc existunt
	 */
	per (i = ZEPHYRUM; i <= locatio.index_segmenti; i++)
    {
		si (!xar->segmenta[i])
        {
			si (!allocare_segmentum(xar, i))
            {
				redde NIHIL;
			}
		}
	}

	/* Renovare numerum si necessarium */
	si (index >= xar->numerus_elementorum)
    {
		xar->numerus_elementorum = index + I;
	}

	/* Reddere indicem elementi */
	basis = (i8*)xar->segmenta[locatio.index_segmenti];
	redde basis + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* Xar Ponere */
b32
xar_ponere(
                Xar* xar,
                i32  index,
    constans vacuum* elementum)
{
	vacuum* destinatio;

	destinatio = xar_obtinere_vel_creare(xar, index);
	si (!destinatio)
    {
		redde FALSUM;
	}

	memcpy(destinatio, elementum, xar->magnitudo_elementi);
	redde VERUM;
}

/* ========================================================================
 * ADDITIO
 * ======================================================================== */

/* Xar Addere
 * "Addere ad finem"
 */
vacuum*
xar_addere(
    Xar* xar)
{
	XarLocatio  locatio;
	       i32  index_novus;
	        i8* basis;

	si (!xar)
    {
		redde NIHIL;
	}

	index_novus = xar->numerus_elementorum;

	/* Invenire ubi elementum novum ibit */
	si (!xar_locare(xar, index_novus, &locatio))
    {
		redde NIHIL;
	}

	/* Assecurare segmentum existit */
	si (!xar->segmenta[locatio.index_segmenti])
    {
		si (!allocare_segmentum(xar, locatio.index_segmenti))
        {
			redde NIHIL;  /* Allocatio fracta */
		}
		/* Renovare locationem cum segmento novo */
		locatio.basis_segmenti = xar->segmenta[locatio.index_segmenti];
	}

	/* Incrementare numerum */
	xar->numerus_elementorum++;

	/* Reddere indicem ad elementum novum */
	basis = (i8*)locatio.basis_segmenti;
	redde basis + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* Xar Addere Multos
 * "Addere elementa multa - optimizatum pro operationibus magnis"
 */
i32
xar_addere_multos(
                Xar* xar,
    constans vacuum* elementa,
                i32  numerus)
{
	constans i8* src;
	        i32  index_initialis;
	        i32  index_finalis;
	 XarLocatio  locatio;
	        i32  i, j;
	        i32  elementa_copiata;
	        i32  residuum_segmenti;
	         i8* destinatio_ptr;
	        i32  ad_copiam;

	si (!xar || !elementa || numerus == ZEPHYRUM)
    {
		redde ZEPHYRUM;
	}

	src             = (constans i8*)elementa;
	index_initialis = xar->numerus_elementorum;
	index_finalis   = index_initialis + numerus - I;

	/* Praeallocare omnia segmenta necessaria
	 */
	si (!xar_locare(xar, index_finalis, &locatio))
    {
		redde ZEPHYRUM;  /* Index nimis magnus */
	}

	/* Assecurare omnia segmenta ab initio usque ad finem allocata sunt */
	per (i = ZEPHYRUM; i <= locatio.index_segmenti; i++)
    {
		si (!xar->segmenta[i])
        {
			si (!allocare_segmentum(xar, i))
            {
				i32 index_ultimus;

				/* Allocatio partialis - renovare numerum et reddere quantum habuimus */
				si (i == ZEPHYRUM) redde ZEPHYRUM;

				/* Computare quot elementa re vera addere possumus */
				index_ultimus = ZEPHYRUM;
				per (j = ZEPHYRUM; j < i; j++)
                {
					index_ultimus += computare_magnitudinem_segmenti(xar, j);
				}
				si (index_ultimus > index_initialis)
                {
					numerus = index_ultimus - index_initialis;
				}
                alioquin
                {
					redde ZEPHYRUM;
				}
				frange;
			}
		}
	}

	/* Renovare numerum primum */
	xar->numerus_elementorum = index_initialis + numerus;

	/* Nunc facere copias efficientes per segmentum
	 */
	xar_locare(xar, index_initialis, &locatio);
	i = ZEPHYRUM;
	elementa_copiata = ZEPHYRUM;

	dum (i < numerus)
    {
		/* Computare quot elementa in segmento currenti cabent */
		residuum_segmenti = locatio.magnitudo_segmenti - locatio.offset_in_segmento;
		ad_copiam = (numerus - i < residuum_segmenti) ? (numerus - i) : residuum_segmenti;

		/* Obtinere indicem destinationis */
		destinatio_ptr = (i8*)xar->segmenta[locatio.index_segmenti];
		destinatio_ptr += locatio.offset_in_segmento * xar->magnitudo_elementi;

		/* Copiare blocum elementorum */
		memcpy(destinatio_ptr, src + (i * xar->magnitudo_elementi),
		       ad_copiam * xar->magnitudo_elementi);

		i += ad_copiam;
		elementa_copiata += ad_copiam;

		/* Movere ad segmentum proximum si necessarium */
		si (i < numerus)
        {
			locatio.index_segmenti++;
			locatio.offset_in_segmento = ZEPHYRUM;
			locatio.magnitudo_segmenti = computare_magnitudinem_segmenti(
                                            xar,
                                            locatio.index_segmenti);
		}
	}

	redde elementa_copiata;
}

/* ========================================================================
 * ITERATIO
 * ======================================================================== */

/* Xar Iterator Initium
 * "Implementatio iteratoris"
 */
XarIterator
xar_iterator_initium(
    constans Xar* xar)
{
	XarIterator iter = {ZEPHYRUM};

	iter.xar                = xar;
	iter.index_currens      = ZEPHYRUM;
	iter.index_segmenti     = ZEPHYRUM;
	iter.offset_in_segmento = ZEPHYRUM;

	si (xar && xar->numerus_segmentorum > ZEPHYRUM)
    {
		iter.basis_segmenti = xar->segmenta[ZEPHYRUM];
		iter.finis_segmenti = computare_magnitudinem_segmenti(xar, ZEPHYRUM);
	}

	redde iter;
}

/* Xar Iterator Proximum
 * "Obtinere elementum proximum"
 */
vacuum*
xar_iterator_proximum(
    XarIterator* iter)
{
	vacuum* elementum;
	    i8* basis;

	si (!iter || !iter->xar || iter->index_currens >= iter->xar->numerus_elementorum)
    {
		redde NIHIL;
	}

	/* Verificare si oportet movere ad segmentum proximum */
	si (iter->offset_in_segmento >= iter->finis_segmenti)
    {
		iter->index_segmenti++;
		iter->offset_in_segmento = ZEPHYRUM;

		si (iter->index_segmenti < iter->xar->numerus_segmentorum)
        {
			iter->basis_segmenti = iter->xar->segmenta[iter->index_segmenti];
			iter->finis_segmenti = computare_magnitudinem_segmenti(
                                        iter->xar,
			                            iter->index_segmenti);
		}
	}

	/* Obtinere elementum currens */
	basis     = (i8*)iter->basis_segmenti;
	elementum = basis + (iter->offset_in_segmento * iter->xar->magnitudo_elementi);

	/* Promovere iteratorem */
	iter->index_currens++;
	iter->offset_in_segmento++;

	redde elementum;
}

/* Xar Iterator Finis */
b32
xar_iterator_finis(
    XarIterator* iter)
{
	redde !iter || !iter->xar || iter->index_currens >= iter->xar->numerus_elementorum;
}

/* ========================================================================
 * QUAESTIO
 * ======================================================================== */

/* Xar Invenire Index
 * "Quaestio linearis - reddere indicem"
 */
s32
xar_invenire_index(
    constans         Xar* xar,
    constans      vacuum* clavis,
              XarComparator  comparator)
{
	   i32  i;
	vacuum* elementum;

	per (i = ZEPHYRUM; i < xar->numerus_elementorum; i++)
    {
		elementum = xar_obtinere(xar, i);
		si (elementum && comparator(clavis, elementum) == ZEPHYRUM)
        {
			redde (s32)i;
		}
	}

	redde -I;  /* Non inventus */
}

/* Xar Invenire
 * "Quaestio linearis - reddere indicem ad elementum"
 */
vacuum*
xar_invenire(
    constans         Xar* xar,
    constans      vacuum* clavis,
              XarComparator  comparator)
{
	s32 index;

	index = xar_invenire_index(xar, clavis, comparator);
	si (index < ZEPHYRUM)
    {
		redde NIHIL;
	}

	redde xar_obtinere(xar, (i32)index);
}

/* Xar Quaerere Binarie
 * "Quaestio binaria - assumit tabulam ordinatam"
 */
vacuum*
xar_quaerere_binarie(
    constans         Xar* xar,
    constans      vacuum* clavis,
              XarComparator  comparator)
{
	    i32  sinister;
	    i32  dexter;
	    i32  medius;
	 vacuum* elementum;
	integer  cmp;

	sinister = ZEPHYRUM;
	dexter   = xar->numerus_elementorum - I;

	dum (sinister <= dexter)
    {
		medius = sinister + (dexter - sinister) / II;

		elementum = xar_obtinere(xar, medius);
		si (!elementum)
        {
			redde NIHIL;
		}

		cmp = comparator(clavis, elementum);

		si (cmp == ZEPHYRUM)
        {
			redde elementum;  /* Inventus! */
		}
        alioquin si (cmp < ZEPHYRUM)
        {
			si (medius == ZEPHYRUM) frange;
			dexter = medius - I;
		}
        alioquin
        {
			sinister = medius + I;
		}
	}

	redde NIHIL;  /* Non inventus */
}

/* Xar Quaerere Binarie Index
 * "Quaestio binaria - reddere indicem"
 */
s32
xar_quaerere_binarie_index(
    constans         Xar* xar,
    constans      vacuum* clavis,
              XarComparator  comparator)
{
	    i32  sinister;
	    i32  dexter;
	    i32  medius;
	 vacuum* elementum;
	integer  cmp;

	sinister = ZEPHYRUM;
	dexter   = xar->numerus_elementorum - I;

	dum (sinister <= dexter)
    {
		medius = sinister + (dexter - sinister) / II;

		elementum = xar_obtinere(xar, medius);
		si (!elementum)
        {
			redde -I;
		}

		cmp = comparator(clavis, elementum);

		si (cmp == ZEPHYRUM)
        {
			redde (s32)medius;  /* Inventus! */
		}
        alioquin si (cmp < ZEPHYRUM)
        {
			si (medius == ZEPHYRUM) frange;
			dexter = medius - I;
		}
        alioquin
        {
			sinister = medius + I;
		}
	}

	redde -I;  /* Non inventus */
}

/* ========================================================================
 * UTILITAS
 * ======================================================================== */

/* Xar Numerus */
i32
xar_numerus(
    constans Xar* xar)
{
	redde xar ? xar->numerus_elementorum : ZEPHYRUM;
}

/* Xar Capacitas */
i32
xar_capacitas(
    constans Xar* xar)
{
	redde xar ? xar->capacitas_totalis : ZEPHYRUM;
}

/* Xar Vacuum Est */
b32
xar_vacuum_est(
    constans Xar* xar)
{
	redde !xar || xar->numerus_elementorum == ZEPHYRUM;
}

/* Xar Vacare */
vacuum
xar_vacare(
    Xar* xar)
{
	si (xar)
    {
		xar->numerus_elementorum = ZEPHYRUM;
		/* Nota: segmenta manent allocata cum piscina */
	}
}

/* Xar Truncare */
vacuum
xar_truncare(Xar* xar, i32 numerus_novus)
{
	si (xar && numerus_novus < xar->numerus_elementorum)
    {
		xar->numerus_elementorum = numerus_novus;
	}
}

/* Xar Copiare Ad Tabulam */
i32
xar_copiare_ad_tabulam(
    constans Xar* xar,
          vacuum* destinatio,
             i32  initium,
             i32  numerus)
{
	    i8* destinatio_bytes;
	   i32  i;
	vacuum* src;

	destinatio_bytes = (i8*)destinatio;

	per (i = ZEPHYRUM; i < numerus; i++)
    {
		src = xar_obtinere(xar, initium + i);
		si (!src)
        {
			redde i;  /* Reddere numerum copiatum cum successu */
		}
		memcpy(
            destinatio_bytes + (i * xar->magnitudo_elementi),
            src,
            xar->magnitudo_elementi);
	}

	redde numerus;
}

/* Xar Status Imprimere
 * "Imprimere statisticas pro depuratione"
 */
vacuum
xar_status_imprimere(
    constans Xar* xar)
{
	i32 i;
	i32 mensura;
	i32 usus;
	i32 allocatus;

	si (!xar)
    {
		imprimere("Xar: NIHIL\n");
		redde;
	}

	imprimere("Xar '%s' Status:\n", xar->titulus);
	imprimere("  Numerus elementorum: %u\n", xar->numerus_elementorum);
	imprimere("  Magnitudo elementi: %u bytes\n", xar->magnitudo_elementi);
	imprimere("  Capacitas totalis: %u\n", xar->capacitas_totalis);
	imprimere("  Numerus segmentorum: %u\n", xar->numerus_segmentorum);

	imprimere("\n  Segmenta:\n");
	allocatus = ZEPHYRUM;
	per (i = ZEPHYRUM; i < xar->numerus_segmentorum; i++)
    {
		mensura = computare_magnitudinem_segmenti(xar, i);
		imprimere("    [%u]: %u elementa (%u bytes) - %s\n",
		          i, mensura, mensura * xar->magnitudo_elementi,
		          xar->segmenta[i] ? "ALLOCATUS" : "NULLUS");

		si (xar->segmenta[i])
        {
			allocatus += mensura;
		}
	}

	/* Computare usum */
	usus = ZEPHYRUM;
	per (i = ZEPHYRUM; i < xar->numerus_segmentorum && usus < xar->numerus_elementorum; i++)
    {
		mensura = computare_magnitudinem_segmenti(xar, i);
		usus += (xar->numerus_elementorum - usus > mensura) ?
		        mensura : (xar->numerus_elementorum - usus);
	}

	imprimere("\n  Usus memoriae:\n");
	imprimere("    Allocatus: %u elementa (%u bytes)\n",
	          allocatus,
	          allocatus * xar->magnitudo_elementi);
	imprimere("    Usus: %u elementa (%u bytes)\n",
	          usus,
	          usus * xar->magnitudo_elementi);
	imprimere("    Utilitas: %.1f%%\n",
	          allocatus > ZEPHYRUM ? (100.0 * usus / allocatus) : 0.0);
}
