// STEFAN MIRUNA ANDREEA 314CA
#include <stdlib.h>
#include <stdio.h>
#define MOD 10007

// functie pentru eliberarea tuturor resurselor
void free_all_resources(int ***collection, int **details, int size)
{
	for (int k = 0; k < size; k++) {
		for (int i = 0; i < details[k][0]; i++)
			free(collection[k][i]);
		free(collection[k]);
	}
	free(collection);
	for (int i = 0; i < size; i++)
		free(details[i]);
	free(details);
}

// functie care aloca si citeste o matrice
int **new_matrix(int m, int n)
{
	int **a;
	a = (int **)malloc(m * sizeof(int *));
	for (int i = 0; i < m; i++)
		a[i] = (int *)malloc(n * sizeof(int));
	//Programare defensiva
	if (!a) {
		printf("Could not allocate memory");
		for (int i = 0; i < m; i++)
			free(a[i]);
		free(a);
		exit(0);
	}
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			scanf("%d", &a[i][j]);
	return a;
}

// functie care returneaza suma elementelor unei matrice
int sum_matrix_elements(int **a, int m, int n)
{
	int s = 0;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			s = s + a[i][j];
	s = s % MOD;
	if (s < 0)
		s = s + MOD;
	return s;
}

// functie care adauga o matrice noua la finalul colectiei
/*functia adauga, in paralel, si vectorul ce contine dimensiunile si
suma elementelor matricei la finalul colectiei detaliilor despre matrice */
void resize(int ****collection, int ***details, int size, int m, int n)
{
	int ***aux = NULL;
	int **a;
	a = new_matrix(m, n);
	aux = (int ***)realloc(*collection, (size + 1) * sizeof(int **));
	//Programare defensiva
	if (!aux) {
		printf("Could not allocate memory");
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	*collection = aux;
	(*collection)[size] = a;
	int *v;
	v = (int *)malloc(3 * sizeof(int));
	v[0] = m;
	v[1] = n;
	v[2] = sum_matrix_elements(a, m, n);
	int **aux_details = NULL;
	aux_details = (int **)realloc(*details, (size + 1) * sizeof(int *));
	//Programare defensiva
	if (!aux_details) {
		printf("Could not allocate memory");
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	*details = aux_details;
	(*details)[size] = v;
}

// functia afiseaza dimensiunile matricei de pe o anumita pozitie
void print_dimensions(int **details, int index, int size)
{
	if (index >= size) {
		printf("No matrix with the given index\n");
		return;
	}
	printf("%d ", details[index][0]);
	printf("%d\n", details[index][1]);
}

// functia afiseaza matricea de pe o anumita pozitie
void print_matrix(int ***collection, int **details, int index, int size)
{
	if (index >= size || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	int m, n;
	m = details[index][0];
	n = details[index][1];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf("%d ", collection[index][i][j]);
		printf("\n");
	}
}

// functie pentru inmultirea a doua matrice
void multiply(int ****collection, int ***details, int i1, int i2, int size)
{
	if (i1 >= size || i2 >= size || i1 < 0 || i2 < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	if ((*details)[i1][1] != (*details)[i2][0]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	int **product; // matricea-produs
	product = (int **)malloc((*details)[i1][0] * sizeof(int *));
	for (int i = 0; i < (*details)[i1][0]; i++)
		product[i] = (int *)malloc((*details)[i2][1] * sizeof(int));
	if (!product) {
		printf("Could not allocate memory");
		for (int i = 0; i < (*details)[i1][0]; i++)
			free(product[i]);
		free(product);
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	for (int i = 0; i < (*details)[i1][0]; i++)
		for (int j = 0; j < (*details)[i2][1]; j++) {
			product[i][j] = 0;
			for (int k = 0; k < (*details)[i1][1]; k++) {
				int x = (*collection)[i1][i][k] % MOD;
				int y = (*collection)[i2][k][j] % MOD;
				product[i][j] = (x * y) % MOD + product[i][j];
			}
			product[i][j] = product[i][j] % MOD;
			if (product[i][j] < 0)
				product[i][j] = product[i][j] + MOD;
	}
	// se stocheaza matricea rezultat in colectie
	int ***aux = NULL;
	aux = (int ***)realloc(*collection, (size + 1) * sizeof(int **));
	if (!aux) {
		printf("Could not allocate memory");
		for (int i = 0; i < (*details)[i1][0]; i++)
			free(product[i]);
		free(product);
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	*collection = aux;
	(*collection)[size] = (int **)malloc((*details)[i1][0] * sizeof(int *));
	for (int i = 0; i < (*details)[i1][0]; i++)
		(*collection)[size][i] = (int *)malloc((*details)[i2][1] * sizeof(int));
	if (!(*collection)[size]) {
		printf("Could not allocate memory");
		for (int i = 0; i < (*details)[i1][0]; i++)
			free((*collection)[size][i]);
		free((*collection)[size]);
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	for (int i = 0; i < (*details)[i1][0]; i++)
		for (int j = 0; j < (*details)[i2][1]; j++)
			(*collection)[size][i][j] = product[i][j];
	// se stocheaza detaliile matricei rezultat in colectia de detalii
	int *v;
	v = (int *)malloc(3 * sizeof(int));
	v[0] = (*details)[i1][0]; // numarul de linii al matricei-produs
	v[1] = (*details)[i2][1]; //numarul de coloane al matricei-produs
	v[2] = sum_matrix_elements(product, (*details)[i1][0], (*details)[i2][1]);
	int **aux_details = NULL;
	aux_details = (int **)realloc(*details, (size + 1) * sizeof(int *));
	if (!aux_details) {
		printf("Could not allocate memory");
		for (int i = 0; i < (*details)[i1][0]; i++)
			free(product[i]);
		free(product);
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	*details = aux_details;
	(*details)[size] = v;
	for (int i = 0; i < (*details)[i1][0]; i++)
		free(product[i]);
	free(product);
}

// functie pentru redimensionarea unei matrice
void redimension(int ****collection, int ***details, int index, int size)
{
	// numarul de linii, respectiv de coloane dupa care se face redimensionarea
	int l, c;
	scanf("%d", &l);
	// se stocheaza intr-un vector liniile dupa care se face redimensionarea
	int *lines;
	lines = (int *)malloc(l * sizeof(int));
	for (int i = 0; i < l; i++)
		scanf("%d", &lines[i]);
	scanf("%d", &c);
	// se stocheaza intr-un vector coloanele dupa care se face redimensionarea
	int *columns;
	columns = (int *)malloc(c * sizeof(int));
	for (int i = 0; i < c; i++)
		scanf("%d", &columns[i]);
	if (index >= size || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		int **redim; //matricea rezultata in urma redimensionarii
		redim = (int **)malloc(l * sizeof(int *));
		for (int i = 0; i < l; i++)
			redim[i] = (int *)malloc(c * sizeof(int));
		//Programare defensiva
		if (!redim) {
			printf("Could not allocate memory");
			for (int i = 0; i < l; i++)
				free(redim[i]);
			free(redim);
			free_all_resources((*collection), (*details), size);
			exit(0);
		}
		for (int i = 0; i < l; i++)
			for (int j = 0; j < c; j++)
				redim[i][j] = (*collection)[index][lines[i]][columns[j]];
		/* dealocam memoria pentru vechea matrice si alocam din nou exact
		pe dimensiunile matricei redimensionate*/
		for (int i = 0; i < (*details)[index][0]; i++)
			free((*collection)[index][i]);
		free((*collection)[index]);
		(*collection)[index] = (int **)malloc(l * sizeof(int *));
		for (int i = 0; i < l; i++)
			(*collection)[index][i] = (int *)malloc(c * sizeof(int));
		if (!(*collection)[index]) {
			printf("Could not allocate memory");
			for (int i = 0; i < l; i++)
				free((*collection)[index][i]);
			free((*collection)[index]);
			exit(0);
		}
		for (int i = 0; i < l; i++)
			for (int j = 0; j < c; j++)
				(*collection)[index][i][j] = redim[i][j];
		for (int i = 0; i < l; i++)
			free(redim[i]);
		free(redim);
		// se actualizeaza vectorul cu dimensiunea si suma elementelor matricei
		(*details)[index][0] = l;
		(*details)[index][1] = c;
		(*details)[index][2] = sum_matrix_elements((*collection)[index], l, c);
	}
	free(columns);
	free(lines);
}

// functie de sortare dupa suma elementelor matricei
void sort(int ****collection, int ***details, int size)
{
	for (int i = 0; i < size - 1; i++)
		for (int j = 0; j < size - 1 - i; j++)
			if ((*details)[j][2] > (*details)[j + 1][2]) {
				// interschimbare in colectia de detalii ale matricelor
				int *aux = (*details)[j];
				(*details)[j] = (*details)[j + 1];
				(*details)[j + 1] = aux;
				// interschimbare in colectia de matrice
				int **aux2 = (*collection)[j];
				(*collection)[j] = (*collection)[j + 1];
				(*collection)[j + 1] = aux2;
			}
}

// functie care efectueaza transpusa unei matrice
void transpose_matrix(int ****collection, int ***details, int index, int size)
{
	if (index >= size) {
		printf("No matrix with the given index\n");
		return;
	}
	// matricea transpusa
	int **transp;
	transp = (int **)malloc((*details)[index][1] * sizeof(int *));
	for (int i = 0; i < (*details)[index][1]; i++)
		transp[i] = (int *)malloc((*details)[index][0] * sizeof(int));
	// Programare defensiva
	if (!transp) {
		printf("Could not allocate memory");
		for (int i = 0; i < (*details)[index][1]; i++)
			free(transp[i]);
		free(transp);
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	for (int i = 0; i < (*details)[index][0]; i++)
		for (int j = 0; j < (*details)[index][1]; j++)
			transp[j][i] = (*collection)[index][i][j];
	// se dealoca memoria vechii matrice
	for (int i = 0; i < (*details)[index][0]; i++)
		free((*collection)[index][i]);
	free((*collection)[index]);
	(*collection)[index] = transp;
	/* se interschimba, in vectorul din colectia de detalii,
	numarul liniilor cu numarul coloanelor */
	int aux;
	aux = (*details)[index][0];
	(*details)[index][0] = (*details)[index][1];
	(*details)[index][1] = aux;
}

// functie pentru inmultirea a doua matrice patratice cu aceleasi dimensiuni
void multiply_same_dimension_matrixes(int n, int **a, int **b)
{
	int **c;
	c = (int **)malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++)
		c[i] = (int *)malloc(n * sizeof(int));
	if (!c) {
		printf("Could not allocate memory");
		for (int i = 0; i < n; i++)
			free(c[i]);
		free(c);
		exit(0);
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			c[i][j] = 0;
			for (int k = 0; k < n; k++)
				c[i][j] = a[i][k] * b[k][j] + c[i][j];
			c[i][j] = c[i][j] % MOD;
			if (c[i][j] < 0)
				c[i][j] = c[i][j] + MOD;
		}
	/* Se pune matricea-produs in locul primei matrice,
	care va fi transmisa prin referinta */
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			a[i][j] = c[i][j];
			a[i][j] = a[i][j] % MOD;
			if (a[i][j] < 0)
				a[i][j] = a[i][j] + MOD;
		}
	for (int i = 0; i < n; i++)
		free(c[i]);
	free(c);
}

// functie de ridicare la putere a unei matrice in timp logaritmic
void raise(int ****collection, int ***details, int index, int k, int size)
{
	if (index >= size || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	if (k < 0) {
		printf("Power should be positive\n");
		return;
	}
	if ((*details)[index][0] != (*details)[index][1]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	int n; // numarul de linii sau de coloane
	n = (*details)[index][0];
	int **a;
	a = (*collection)[index];
	int **power; // matricea-rezultat
	power = (int **)malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++)
		power[i] = (int *)malloc(n * sizeof(int));
	if (!power) {
		printf("Could not allocate memory");
		for (int i = 0; i < n; i++)
			free(power[i]);
		free(power);
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	// se construieste matricea-unitate
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i == j)
				power[i][j] = 1;
			else
				power[i][j] = 0;
	while (k > 0) {
		if (k % 2 == 1)
			multiply_same_dimension_matrixes(n, power, a);
		multiply_same_dimension_matrixes(n, a, a);
		k = k / 2;
	}
	for (int i = 0; i < n; i++)
		free(a[i]);
	free(a);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			power[i][j] = power[i][j] % MOD;
				if (power[i][j] < 0)
					power[i][j] = power[i][j] + MOD;
		}
	// se inlocuieste in colectie vechea matrice cu matricea-rezultat
	(*collection)[index] = power;
	// se modifica suma elementelor in vectorul din colectia de detalii
	(*details)[index][2] = sum_matrix_elements(power, n, n);
}

// functia de eliminare a unei matrice din memorie
void remove_matrix(int ****collection, int ***details, int index, int size)
{
	int m;
	m = (*details)[index][0]; // numarul de linii
	if (size == 1) {
		for (int i = 0; i < m; i++)
			free((*collection)[index][i]);
		free((*collection)[index]);
		free((*details)[index]);
	return;
	}
	for (int i = 0; i < m; i++)
		free((*collection)[index][i]);
	free((*collection)[index]);
	for (int i = index; i < size - 1; i++)
		(*collection)[i] = (*collection)[i + 1];
	int ***aux = NULL;
	aux = (int ***)realloc(*collection, (size - 1) * sizeof(int **));
	if (!aux) {
		printf("Could not allocate memory");
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	*collection = aux;
	free((*details)[index]);
	for (int i = index; i < size - 1; i++)
		(*details)[i] = (*details)[i + 1];
	int **aux_details = NULL;
	aux_details = (int **)realloc(*details, (size - 1) * sizeof(int *));
	if (!aux_details) {
		printf("Could not allocate memory");
		free_all_resources((*collection), (*details), size);
		exit(0);
	}
	*details = aux_details;
}

int main(void)
{
	// numar de linii, respectiv de coloane
	int m, n;
	int index;
	// indecsii celor 2 matrice ce trebuie inmultite
	int i1, i2;
	// puterea la care se va ridica matricea
	int k;
	// colectia de matrice
	int ***collection = NULL;
	// colectia in care se retine cate un vector cu date pentru fiecare matrice
	int **details = NULL;
	// dimensiunea colectiei de matrice
	int size = 0;
	// caracterul citit de la tastatura
	char ch;
	scanf(" %c", &ch);
	while (ch != 'Q') {
		if (ch == 'L') {
			scanf("%d%d", &m, &n);
			resize(&collection, &details, size, m, n);
			size++;
		} else {
			if (ch == 'D') {
				scanf("%d", &index);
				print_dimensions(details, index, size);
		} else {
			if (ch == 'P') {
				scanf("%d", &index);
				print_matrix(collection, details, index, size);
		} else {
			if (ch == 'M') {
				// i1 = index-ul primei matrice
				// i2 = index-ul celei de-a doua matrice
				scanf("%d%d", &i1, &i2);
				multiply(&collection, &details, i1, i2, size);
				if (i1 < size && i2 < size && details[i1][1] == details[i2][0])
					size++;
		} else {
			if (ch == 'O') {
				sort(&collection, &details, size);
		} else {
			if (ch == 'T') {
				scanf("%d", &index);
				transpose_matrix(&collection, &details, index, size);
		} else {
			if (ch == 'C') {
				scanf("%d", &index);
				redimension(&collection, &details, index, size);
		} else {
			if (ch == 'R') {
				scanf("%d%d", &index, &k);
				raise(&collection, &details, index, k, size);
		} else {
			if (ch == 'F') {
				scanf("%d", &index);
				if (index >= size || index < 0) {
					printf("No matrix with the given index\n");
				} else {
					remove_matrix(&collection, &details, index, size);
					size--;
			}
		} else {
			printf("Unrecognized command\n");
		}
		}
		}
		}
		}
		}
		}
		}
		}
		scanf(" %c", &ch);
	}
	free_all_resources(collection, details, size);
	return 0;
}
