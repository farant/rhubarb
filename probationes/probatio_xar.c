/* probatio_xar.c - Probationes Tabulae Exponentialis */
#include "latina.h"
#include "xar.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Comparator pro i32 */
interior integer
comparare_i32(constans vacuum* a, constans vacuum* b)
{
	i32 aa = *(constans i32*)a;
	i32 bb = *(constans i32*)b;
	redde (aa < bb) ? -I : (aa > bb) ? I : ZEPHYRUM;
}

s32 principale(vacuum)
{
	Piscina* piscina;
	    b32  praeteritus;

	/* Aperire piscinam et credonem */
	piscina = piscina_generare_dynamicum("probatio_xar", CDLVI);
	si (!piscina) 
    {
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);

	/* ==================================================
	 * Probare xar_creare
	 * ================================================== */

	{
		Xar* xar;

		imprimere("\n--- Probans xar_creare ---\n");

		xar = xar_creare(piscina, sizeof(i32));
		CREDO_NON_NIHIL(xar);
		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), ZEPHYRUM);
		CREDO_AEQUALIS_I32((i32)xar_capacitas(xar), ZEPHYRUM);
		CREDO_VERUM(xar_vacuum_est(xar));
	}

	/* ==================================================
	 * Probare xar_addere et xar_obtinere
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32* receptus;

		imprimere("\n--- Probans xar_addere et xar_obtinere ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere primum elementum */
		elem = (i32*)xar_addere(xar);
		CREDO_NON_NIHIL(elem);
		*elem = XLII;

		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), I);

		/* Recuperare elementum */
		receptus = (i32*)xar_obtinere(xar, ZEPHYRUM);
		CREDO_NON_NIHIL(receptus);
		CREDO_AEQUALIS_I32(*receptus, XLII);

		/* Addere secundum elementum */
		elem = (i32*)xar_addere(xar);
		*elem = XCIX;

		receptus = (i32*)xar_obtinere(xar, I);
		CREDO_AEQUALIS_I32(*receptus, XCIX);
	}

	/* ==================================================
	 * Probare crescentia automatica
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32  i;

		imprimere("\n--- Probans crescentia_automatica ---\n");

		xar = xar_creare_cum_magnitudine(piscina, sizeof(i32), VIII);

		/* Addere elementa satis ad coactiones crescentiae multiplices */
		per (i = ZEPHYRUM; i < C; i++) 
        {
			elem = (i32*)xar_addere(xar);
			CREDO_NON_NIHIL(elem);
			*elem = i;
		}

		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), C);

		/* Verificare omnia elementa */
		per (i = ZEPHYRUM; i < C; i++) 
        {
			elem = (i32*)xar_obtinere(xar, (i32)i);
			CREDO_NON_NIHIL(elem);
			CREDO_AEQUALIS_I32(*elem, i);
		}
	}

	/* ==================================================
	 * Probare xar_obtinere_vel_creare (dispersus)
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;

		imprimere("\n--- Probans xar_obtinere_vel_creare ---\n");

		xar = xar_creare_cum_magnitudine(piscina, sizeof(i32), XVI);

		/* Creare elementum ad indicem remotum */
		elem = (i32*)xar_obtinere_vel_creare(xar, L);
		CREDO_NON_NIHIL(elem);
		*elem = XLII;

		/* Numerus debet esse 51 nunc */
		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), LI);

		/* Verificare elementum remotum */
		elem = (i32*)xar_obtinere(xar, L);
		CREDO_NON_NIHIL(elem);
		CREDO_AEQUALIS_I32(*elem, XLII);
	}

	/* ==================================================
	 * Probare xar_ponere
	 * ================================================== */

	{
		Xar* xar;
		i32  valor;
		i32* receptus;

		imprimere("\n--- Probans xar_ponere ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		valor = CXXIII;
		CREDO_VERUM(xar_ponere(xar, X, &valor));

		receptus = (i32*)xar_obtinere(xar, X);
		CREDO_NON_NIHIL(receptus);
		CREDO_AEQUALIS_I32(*receptus, CXXIII);
	}

	/* ==================================================
	 * Probare xar_addere_multos
	 * ================================================== */

	{
		Xar* xar;
		i32  numeri[X];
		i32  i;
		i32* receptus;
		i32  additi;

		imprimere("\n--- Probans xar_addere_multos ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Parare tabulam numerorum */
		per (i = ZEPHYRUM; i < X; i++)
        {
			numeri[i] = i * II;
		}

		/* Addere omnes simul */
		additi = xar_addere_multos(xar, numeri, X);
		CREDO_AEQUALIS_I32((i32)additi, X);
		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), X);

		/* Verificare omnes */
		per (i = ZEPHYRUM; i < X; i++)
        {
			receptus = (i32*)xar_obtinere(xar, (i32)i);
			CREDO_NON_NIHIL(receptus);
			CREDO_AEQUALIS_I32(*receptus, i * II);
		}
	}

	/* ==================================================
	 * Probare xar_addere_multos trans segmenta
	 * ================================================== */

	{
		Xar* xar;
		i32  numeri[XXXII];
		i32  i;
		i32* receptus;
		i32  additi;

		imprimere("\n--- Probans xar_addere_multos trans segmenta ---\n");

		xar = xar_creare_cum_magnitudine(piscina, sizeof(i32), VIII);

		/* Parare XXXII numeros */
		per (i = ZEPHYRUM; i < XXXII; i++)
        {
			numeri[i] = i + C;
		}

		/* Addere omnes - debent transire per segmenta multa */
		additi = xar_addere_multos(xar, numeri, XXXII);
		CREDO_AEQUALIS_I32((i32)additi, XXXII);

		/* Verificare omnia correcta */
		per (i = ZEPHYRUM; i < XXXII; i++)
        {
			receptus = (i32*)xar_obtinere(xar, (i32)i);
			CREDO_AEQUALIS_I32(*receptus, i + C);
		}
	}

	/* ==================================================
	 * Probare iterator cum xar vacuo
	 * ================================================== */

	{
		        Xar* xar;
		XarIterator  iter;
		        i32* elem;

		imprimere("\n--- Probans iterator cum xar vacuo ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Iterator debet esse finis immediate */
		iter = xar_iterator_initium(xar);
		CREDO_VERUM(xar_iterator_finis(&iter));

		/* Proximum debet reddere NIHIL */
		elem = (i32*)xar_iterator_proximum(&iter);
		CREDO_NIHIL(elem);
	}

	/* ==================================================
	 * Probare iterator
	 * ================================================== */

	{
		        Xar* xar;
		XarIterator  iter;
		        i32* elem;
		        i32  numeratio;
		        i32  summa;

		imprimere("\n--- Probans iterator ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere elementa */
		per (numeratio = ZEPHYRUM; numeratio < X; numeratio++)
        {
			elem = (i32*)xar_addere(xar);
			*elem = numeratio;
		}

		/* Iterare et summare */
		iter      = xar_iterator_initium(xar);
		summa     = ZEPHYRUM;
		numeratio = ZEPHYRUM;

		dum (!xar_iterator_finis(&iter))
        {
			elem = (i32*)xar_iterator_proximum(&iter);
			CREDO_NON_NIHIL(elem);
			summa += *elem;
			numeratio++;
		}

		CREDO_AEQUALIS_I32(numeratio, X);
		CREDO_AEQUALIS_I32(summa, XLV);  /* 0+1+2+...+9 = 45 */
	}

	/* ==================================================
	 * Probare iterator trans segmenta multa
	 * ================================================== */

	{
		        Xar* xar;
		XarIterator  iter;
		        i32* elem;
		        i32  numeratio;

		imprimere("\n--- Probans iterator trans segmenta ---\n");

		xar = xar_creare_cum_magnitudine(piscina, sizeof(i32), VIII);

		/* Addere L elementa - transit per segmenta multa */
		per (numeratio = ZEPHYRUM; numeratio < L; numeratio++)
        {
			elem  = (i32*)xar_addere(xar);
			*elem = numeratio * III;
		}

		/* Iterare et verificare */
		iter      = xar_iterator_initium(xar);
		numeratio = ZEPHYRUM;

		dum (!xar_iterator_finis(&iter))
        {
			elem = (i32*)xar_iterator_proximum(&iter);
			CREDO_NON_NIHIL(elem);
			CREDO_AEQUALIS_I32(*elem, numeratio * III);
			numeratio++;
		}

		CREDO_AEQUALIS_I32(numeratio, L);
	}

	/* ==================================================
	 * Probare xar_invenire (reddere indicem ad elementum)
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32* resultus;
		i32  i;
		i32  clavis;

		imprimere("\n--- Probans xar_invenire ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere elementa */
		per (i = ZEPHYRUM; i < X; i++)
        {
			elem  = (i32*)xar_addere(xar);
			*elem = i * V;
		}

		/* Quaerere elementum existens */
		clavis   = XV;
		resultus = (i32*)xar_invenire(xar, &clavis, comparare_i32);
		CREDO_NON_NIHIL(resultus);
		CREDO_AEQUALIS_I32(*resultus, XV);

		/* Quaerere non existens */
		clavis   = VII;
		resultus = (i32*)xar_invenire(xar, &clavis, comparare_i32);
		CREDO_NIHIL(resultus);
	}

	/* ==================================================
	 * Probare xar_invenire_index (reddere indicem)
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32  i;
		i32  clavis;
		s32  index;

		imprimere("\n--- Probans xar_invenire_index ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere elementa */
		per (i = ZEPHYRUM; i < X; i++)
        {
			elem  = (i32*)xar_addere(xar);
			*elem = i * V;
		}

		/* Quaerere elementum existens */
		clavis = XV;
		index  = xar_invenire_index(xar, &clavis, comparare_i32);
		CREDO_AEQUALIS_S32(index, III);  /* 15 = 3 * 5 */

		/* Quaerere non existens */
		clavis = VII;
		index  = xar_invenire_index(xar, &clavis, comparare_i32);
		CREDO_AEQUALIS_S32(index, -I);
	}

	/* ==================================================
	 * Probare xar_quaerere_binarie
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32* resultus;
		i32  i;
		i32  clavis;

		imprimere("\n--- Probans xar_quaerere_binarie ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere elementa ordinata */
		per (i = ZEPHYRUM; i < XX; i++)
        {
			elem  = (i32*)xar_addere(xar);
			*elem = i * V;  /* 0, 5, 10, 15, 20, ... 95 */
		}

		/* Quaerere elementum in medio */
		clavis   = XXX;  /* 30 */
		resultus = (i32*)xar_quaerere_binarie(xar, &clavis, comparare_i32);
		CREDO_NON_NIHIL(resultus);
		CREDO_AEQUALIS_I32(*resultus, XXX);

		/* Quaerere primum elementum */
		clavis   = ZEPHYRUM;
		resultus = (i32*)xar_quaerere_binarie(xar, &clavis, comparare_i32);
		CREDO_NON_NIHIL(resultus);
		CREDO_AEQUALIS_I32(*resultus, ZEPHYRUM);

		/* Quaerere ultimum elementum */
		clavis   = XCV;  /* 95 */
		resultus = (i32*)xar_quaerere_binarie(xar, &clavis, comparare_i32);
		CREDO_NON_NIHIL(resultus);
		CREDO_AEQUALIS_I32(*resultus, XCV);

		/* Quaerere non existens */
		clavis   = VII;  /* 7, non in tabula */
		resultus = (i32*)xar_quaerere_binarie(xar, &clavis, comparare_i32);
		CREDO_NIHIL(resultus);

		/* Quaerere in xar vacuo */
		xar_vacare(xar);
		clavis   = X;
		resultus = (i32*)xar_quaerere_binarie(xar, &clavis, comparare_i32);
		CREDO_NIHIL(resultus);
	}

	/* ==================================================
	 * Probare xar_quaerere_binarie_index
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32  i;
		i32  clavis;
		s32  index;

		imprimere("\n--- Probans xar_quaerere_binarie_index ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere elementa ordinata */
		per (i = ZEPHYRUM; i < XX; i++)
        {
			elem  = (i32*)xar_addere(xar);
			*elem = i * V;  /* 0, 5, 10, 15, 20, ... 95 */
		}

		/* Quaerere elementum in medio */
		clavis = XXX;  /* 30 */
		index  = xar_quaerere_binarie_index(xar, &clavis, comparare_i32);
		CREDO_AEQUALIS_S32(index, VI);  /* 30 = index 6 */

		/* Quaerere primum elementum */
		clavis = ZEPHYRUM;
		index  = xar_quaerere_binarie_index(xar, &clavis, comparare_i32);
		CREDO_AEQUALIS_S32(index, ZEPHYRUM);

		/* Quaerere ultimum elementum */
		clavis = XCV;  /* 95 */
		index  = xar_quaerere_binarie_index(xar, &clavis, comparare_i32);
		CREDO_AEQUALIS_S32(index, XIX);  /* index 19 */

		/* Quaerere non existens */
		clavis = VII;  /* 7, non in tabula */
		index  = xar_quaerere_binarie_index(xar, &clavis, comparare_i32);
		CREDO_AEQUALIS_S32(index, -I);

		/* Quaerere in xar vacuo */
		xar_vacare(xar);
		clavis = X;
		index  = xar_quaerere_binarie_index(xar, &clavis, comparare_i32);
		CREDO_AEQUALIS_S32(index, -I);
	}

	/* ==================================================
	 * Probare xar_vacare
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32  i;

		imprimere("\n--- Probans xar_vacare ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere elementa */
		per (i = ZEPHYRUM; i < XX; i++) 
        {
			elem = (i32*)xar_addere(xar);
			*elem = i;
		}

		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), XX);

		/* Vacare */
		xar_vacare(xar);

		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), ZEPHYRUM);
		CREDO_VERUM(xar_vacuum_est(xar));

		/* Segmenta adhuc allocata */
		CREDO_VERUM(xar_capacitas(xar) > ZEPHYRUM);
	}

	/* ==================================================
	 * Probare xar_truncare
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32  i;

		imprimere("\n--- Probans xar_truncare ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere elementa */
		per (i = ZEPHYRUM; i < XX; i++) 
        {
			elem  = (i32*)xar_addere(xar);
			*elem = i;
		}

		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), XX);

		/* Truncare ad X */
		xar_truncare(xar, X);

		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), X);

		/* Prima X elementa adhuc accessibilia */
		per (i = ZEPHYRUM; i < X; i++) 
        {
			elem = (i32*)xar_obtinere(xar, (i32)i);
			CREDO_AEQUALIS_I32(*elem, i);
		}
	}

	/* ==================================================
	 * Probare xar_copiare_ad_tabulam
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32  destinatio[X];
		i32  i;
		i32  copiati;

		imprimere("\n--- Probans xar_copiare_ad_tabulam ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere elementa */
		per (i = ZEPHYRUM; i < XX; i++) 
        {
			elem  = (i32*)xar_addere(xar);
			*elem = i * II;
		}

		/* Copiare X elementa ab indice V */
		copiati = xar_copiare_ad_tabulam(xar, destinatio, V, X);

		CREDO_AEQUALIS_I32((i32)copiati, X);

		/* Verificare */
		per (i = ZEPHYRUM; i < X; i++) 
        {
			CREDO_AEQUALIS_I32(destinatio[i], (V + i) * II);
		}
	}

	/* ==================================================
	 * Probare vexilla
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;

		imprimere("\n--- Probans vexilla ---\n");

		/* Creare cum vexillo SINE_ZEPHYRUM */
		xar = xar_creare_cum_vexillis(piscina, sizeof(i32),
		                               XVI, XAR_VEXILLUM_SINE_ZEPHYRUM);
		CREDO_NON_NIHIL(xar);

		/* Ponere vexillum ordinarium */
		xar_ponere_vexilla(xar, XAR_VEXILLUM_ORDINARIUS);

		/* Addere debet operari */
		elem = (i32*)xar_addere(xar);
		CREDO_NON_NIHIL(elem);
	}

	/* ==================================================
	 * Probare accessus invalidos
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;

		imprimere("\n--- Probans accessus_invalidos ---\n");

		xar = xar_creare(piscina, sizeof(i32));

		/* Addere unum elementum */
		elem = (i32*)xar_addere(xar);
		*elem = XLII;

		/* Tentare obtinere extra limites */
		elem = (i32*)xar_obtinere(xar, C);
		CREDO_NIHIL(elem);

		/* Tentare obtinere cum xar NIHIL */
		elem = (i32*)xar_obtinere(NIHIL, ZEPHYRUM);
		CREDO_NIHIL(elem);
	}

	/* ==================================================
	 * Probare magnam tabulam
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32  i;

		imprimere("\n--- Probans magna_tabula ---\n");

		xar = xar_creare_cum_magnitudine(piscina, sizeof(i32), XVI);

		/* Addere D elementa */
		per (i = ZEPHYRUM; i < D; i++) 
        {
			elem = (i32*)xar_addere(xar);
			CREDO_NON_NIHIL(elem);
			*elem = i;
		}

		CREDO_AEQUALIS_I32((i32)xar_numerus(xar), D);

		/* Verificare aliqua elementa */
		elem = (i32*)xar_obtinere(xar, ZEPHYRUM);
		CREDO_AEQUALIS_I32(*elem, ZEPHYRUM);

		elem = (i32*)xar_obtinere(xar, CCL);
		CREDO_AEQUALIS_I32(*elem, CCL);

		elem = (i32*)xar_obtinere(xar, D - I);
		CREDO_AEQUALIS_I32(*elem, D - I);
	}

	/* ==================================================
	 * Probare status imprimere
	 * ================================================== */

	{
		Xar* xar;
		i32* elem;
		i32  i;

		imprimere("\n--- Probans status_imprimere ---\n");

		xar = xar_creare_cum_magnitudine(piscina, sizeof(i32), VIII);

		per (i = ZEPHYRUM; i < L; i++) 
        {
			elem  = (i32*)xar_addere(xar);
			*elem = i;
		}

		strcpy(xar->titulus, "Tabula Probationis");
		xar_status_imprimere(xar);
	}

	/* ==================================================
	 * Compendium
	 * ================================================== */

	imprimere("\n");
	credo_imprimere_compendium();

	praeteritus = credo_omnia_praeterierunt();

	/* Purificatio */
	credo_claudere();
	piscina_destruere(piscina);

	si (praeteritus) 
    {
		redde ZEPHYRUM;
	} 
    alioquin 
    {
		redde I;
	}
}
