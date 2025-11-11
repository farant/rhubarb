/* cum_functione.c - Test fixture with multiple functions and symbols */

#include <stdio.h>

static int addere(int a, int b)
{
	return a + b;
}

static int multiplicare(int a, int b)
{
	return a * b;
}

int calculare_summa(int n)
{
	int summa = 0;
	int i;

	for (i = 0; i <= n; i++)
	{
		summa = addere(summa, i);
	}

	return summa;
}

int main(void)
{
	int summa;
	int productum;

	summa = addere(3, 4);
	productum = multiplicare(5, 6);

	printf("Summa: %d\n", summa);
	printf("Productum: %d\n", productum);
	printf("Calculatio: %d\n", calculare_summa(10));

	return 0;
}
