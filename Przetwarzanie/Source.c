#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BUFFER_LENGTH 1024

/* DEFINICJA STRUKTURY */

typedef struct image {
	int width;
	int height;
	int depth;
	int** pixels;
	char filename[1024];
} image;

/* WSZYSTKIE FUNKCJE */

int loadImage(image* im, char filename[], int tabPix[]); //WCZYTANIE OBRAZU
void printDatabase(image* database, int baseSize); //WYSWIETLENIE BAZY
void removeImage(image* database, int basePick); //USUNIECIE OBRAZU Z BAZY
int removeComments(char* filename); //USUNIECIE KOMENTARZY Z PLIKU 
void saveToFile(image* database, int basePick); //ZAPISANIE OBRAZU DO PLIKU
void rotateImage(image* database, int basePick); //OBROT OBRAZU
void applyNegative(image* database, int basePick); //EFEKT NEGATYW
void generateNoise(image* database, int basePick); //GENEROWANIE SZUMU
void filterNoise(image* database, int basePick); //FILTROWANIE SZUMU
int createHistogram(image* database, int basePick); //HISTOGRAM

/* MAIN */
int main() {

	//DEKLARACJA ZMIENNYCH
	image* database;
	database = calloc(1, sizeof(image));
	int* tabPix = 0;
	int baseSize = 0;
	int basePick = 0;
	char filename[100];
	int loop = 1, menu;

	/* MENU */
	while (loop == 1)
	{
		printf("--- FUNKCJE BAZY --- \n1. Wyswietl baze\n2. Wybierz aktywny obraz\n3. Wczytaj plik PGM do bazy\n");
		printf("4. Wyswietl aktualnie wybrany obraz\n5. Usun ostatni obraz z bazy\n6. Zapisz wybrany obraz do pliku\n");
		printf("\n\n--- EDYCJA OBRAZU ---\n7. Wykonaj obrot obrazu\n8. Wykonaj negatyw obrazu\n9. Dodaj szum typu pieprz i sol\n10. Filtruj szum\n");
		printf("11. Sporzadz histogram\n\n\n --- WYJSCIE Z PROGRAMU ---\n12. Zamknij program\n\n");
		if (scanf("%d", &menu) == 0) {
			printf("Wprowadzono zly znak.\n");
			loop = 0;
			break;
		}
		switch (menu)
		{

			/* 1. WYSWIETL BAZE */
		case 1:
			system("CLS");
			printf("Liczba obrazow w bazie: %d\n", baseSize);
			printDatabase(database, baseSize);
			printf("\n");
			break;

			/* 2. WYBIERZ AKTYWNY OBRAZ */
		case 2:
			system("CLS");
			if (baseSize == 0) {
				printf("Baza jest pusta.\n");
				break;
			}
			printDatabase(database, baseSize);
			printf("Wybierz numer obrazu na ktorym chcesz pracowac: ");
			if (scanf("%d", &basePick) == 0) {
				printf("Wprowadzono zly znak.\n");
				loop = 0;
			}
			else if (basePick > baseSize) {
				printf("Wybrano zly obraz.\n");
				basePick = 0;
			}
			break;

			/* 3. WCZYTAJ PLIK PGM */
		case 3:
			system("CLS");

			baseSize++;
			database = realloc(database, baseSize * sizeof(image));

			printf("Podaj nazwe pliku: ");
			(void)scanf("%s", &filename);
			strcpy(database[baseSize - 1].filename, filename);

			printf("%s\n", database[baseSize - 1].filename);

			if (loadImage(&database[baseSize - 1], filename, tabPix) == -1)
				break;


			printf("%d %d\n", database[baseSize - 1].width, database[baseSize - 1].height);
			printf("%d\n", database[baseSize - 1].depth);
			//for (int i = 0; i < database[baseSize - 1].height; i++) {
			//	for (int j = 0; j < database[baseSize - 1].width; j++)
			//		printf("%d ", database[baseSize - 1].pixels[i][j]);
			//	printf("\n");
			//}
			break;

			/* 4. WYSWIETL AKTUALNIE WYBRANY OBRAZ */
		case 4:
			system("CLS");
			if (basePick == 0 || baseSize == 0 || basePick > baseSize)
				printf("Nie wybrano zadnego obrazu.\n\n");
			else
				printf("Aktywny obraz: nr %d, nazwa obrazu: %s\n", basePick, database[basePick - 1].filename);
			break;

			/* 5. USUN OSTATNI OBRAZ Z BAZY */
		case 5:
			system("CLS");
			if (baseSize == 0) {
				printf("Nie ma zadnego obrazu w bazie\n");
				break;
			}

			removeImage(database, basePick - 1);
			free(database[baseSize - 1].pixels);
			for (int i = basePick - 1; i < baseSize; i++)
				database[i] = database[i+1];

			baseSize--;
			database = realloc(database, baseSize * sizeof(image));
			basePick = 0;
			break;

			/* 6. ZAPISZ OBRAZ DO PLIKU */
		case 6:
			system("CLS");
			if (basePick == 0 || baseSize == 0 || basePick > baseSize) {
				printf("Nie wybrano zadnego obrazu.\n\n");
				break;
			}
			saveToFile(database, basePick - 1);
			break;

			/* 7. OBROT OBRAZU */ 
		case 7:
			system("CLS");
			if (basePick == 0 || basePick > baseSize || baseSize == 0) {
				printf("Nie wybrano zadnego obrazu. \n\n");
				break;
			}
			rotateImage(database, basePick - 1);
			break;

			/* 8. NEGATYW */
		case 8:
			system("CLS");
			if (basePick == 0 || basePick > baseSize || baseSize == 0) {
				printf("Nie wybrano zadnego obrazu. \n\n");
				break;
			}
			applyNegative(database, basePick - 1);
			break;

			/* 9. SZUM */
		case 9:
			system("CLS");
			if (basePick == 0 || basePick > baseSize || baseSize == 0) {
				printf("Nie wybrano zadnego obrazu. \n\n");
				break;
			}
			generateNoise(database, basePick - 1);
			break;

			/* 10. FILTROWANIE SZUMU */
		case 10:
			system("CLS");
			if (basePick == 0 || basePick > baseSize || baseSize == 0) {
				printf("Nie wybrano zadnego obrazu. \n\n");
				break;
			}
			filterNoise(database, basePick - 1);
			break;

			/* 11. HISTOGRAM */
		case 11:
			system("CLS");
			if (basePick == 0 || basePick > baseSize || baseSize == 0) {
				printf("Nie wybrano zadnego obrazu. \n\n");
				break;
			}
			if (createHistogram(database, basePick - 1) == -1) {
				printf("Bledna nazwa pliku.");
				break;
			}
			break;

			/* 12. ZAMKNIJ PROGRAM */
		case 12:
			system("CLS");
			loop = 0;
			break;

		//DEFAULT
		default:
			system("CLS");
			break;
		}
	}
	printf("Zwalniam pamiec i opuszczam program.");
	free(tabPix);
	free(database);
	return 0;
}

void printDatabase(image* database, int baseSize) {
	for (int i = 0; i < baseSize; i++)
		printf("%d.: %s\n", i + 1, database[i].filename);
}

int loadImage(image* im, char filename[], int tabPix[])
{
	FILE* filePointer;
	int bufferLength = BUFFER_LENGTH;
	char buffer[BUFFER_LENGTH];

	if(removeComments(filename) == -1)
		return -1;

	filePointer = fopen("removed.pgm", "r");

	if (filePointer == NULL) {
		printf("Bledna nazwa pliku.\n");
		return -1;
	}

	fgets(buffer, bufferLength, filePointer);

	if (buffer[0] != 'P' || buffer[1] != '2') {
		printf("To nie jest plik PGM!");
		return -1;
	}

	fscanf(filePointer, "%d %d", &(im->width), &(im->height));

	fscanf(filePointer, "%d", &(im->depth));

	tabPix = malloc(im->height * im->width * sizeof(int));
	im->pixels = malloc(im->height * sizeof(*im->pixels));

	for (int i = 0; i < im->height; i++)
		im->pixels[i] = tabPix + (i * im->width);

	for (int i = 0; i < im->height; i++) {
		for (int j = 0; j < im->width; j++) {
			fscanf(filePointer, "%d", &(im->pixels[i][j]));
		}
	}
	fclose(filePointer);
}

void removeImage(image* database, int basePick) {
	if (database[basePick].depth != NULL)
		database[basePick].depth != NULL;
	if (*(database[basePick]).filename != NULL)
		*(database[basePick]).filename = NULL;
	if (database[basePick].height != NULL)
		database[basePick].height = NULL;
	if (database[basePick].width != NULL)
		database[basePick].width = NULL;
	if (database[basePick].pixels != NULL)
		database[basePick].pixels = NULL;
}

int removeComments(char* filename) {
	FILE* in;
	FILE* out;
	char c;

	in = fopen(filename, "r");

	if (in == NULL) {
		printf("Bledna nazwa pliku.\n");
		return -1;
	}

	out = fopen("removed.pgm", "w");

	while ((c = fgetc(in)) != EOF) {
		if (c == '#') {
			while ((c = fgetc(in)) != '\n');
		}
		fputc(c, out);
	}
	fclose(in);
	fclose(out);
}

void saveToFile(image* database, int basePick) {
	char filename[256];
	char type[2] = "P2";
	(void)scanf("%s", &filename);

	FILE* newFile;
	newFile = fopen(filename, "w");

	fputs("P2", newFile);
	fprintf(newFile, "\n%d %d\n%d\n", database[basePick].width, database[basePick].height, database[basePick].depth);

	for (int i = 0; i < database[basePick].height; i++) {
		for (int j = 0; j < database[basePick].width; j++)
			fprintf(newFile, "%d ", database[basePick].pixels[i][j]);
		fprintf(newFile, "\n");
	}

	fclose(newFile);
}

void rotateImage(image* database, int basePick) {
	int* allocHelper = 0;
	int* tabPix;
	int** tempTab = 0;
	int temp = 0;
	int tempik = 0;

		allocHelper = malloc(database[basePick].width * database[basePick].height * sizeof(int));
		tempTab = malloc(database[basePick].height * sizeof(*tempTab));
		for (int i = 0; i < database[basePick].height; i++)
			tempTab[i] = allocHelper + (i * database[basePick].width);

		temp = database[basePick].width;
		database[basePick].width = database[basePick].height;
		database[basePick].height = temp;

		for (int i = 0; i < database[basePick].height; i++)
			for (int j = 0; j < database[basePick].width; j++)
				tempTab[database[basePick].width - j - 1][i] = database[basePick].pixels[j][i];

		free(database[basePick].pixels);

		tabPix = malloc(database[basePick].height * database[basePick].width * sizeof(int));
		database[basePick].pixels = malloc(database[basePick].height * sizeof(database[basePick].pixels));
		for (int i = 0; i < database[basePick].height; i++)
			database[basePick].pixels[i] = tabPix + (i * database[basePick].width);


		for (int i = 0; i < database[basePick].height; i++) {
			for (int j = 0; j < database[basePick].width; j++) {
				database[basePick].pixels[i][j] = tempTab[j][i];
			}
		}

	free(tempTab);
	free(allocHelper);
}

void applyNegative(image* database, int basePick) {
	for (int i = 0; i < database[basePick].height; i++)
		for (int j = 0; j < database[basePick].width; j++)
			database[basePick].pixels[i][j] = database[basePick].depth - database[basePick].pixels[i][j];
}

void generateNoise(image* database, int basePick) {
	srand(time(NULL));
	int r = 0;
	int g = 0;
	printf("%d", r);

	for (int i = 0; i < database[basePick].height; i++)
		for (int j = 0; j < database[basePick].width; j++) {
			r = rand() % 4;
			if (r == 1) {
				g = rand() % 2;
				if (g == 0) {
					database[basePick].pixels[i][j] = 0;
				}
				else {
					database[basePick].pixels[i][j] = database[basePick].depth;
				}
			}
		}
}

void bubbleSort(int* arr) {
	int i, j;
	double swap;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9 - i; j++) {
			if (arr[j] > arr[j + 1]) {
				swap = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = swap;
			}
		}
	}
}

void filterNoise(image* database, int basePick) {
	int window[10];

	for (int i = 1; i < database[basePick].height - 1; i++)
		for (int j = 1; j <= database[basePick].width - 1; j++)
		{
			window[0] = database[basePick].pixels[i - 1][j - 1];
			window[1] = database[basePick].pixels[i - 1][j];
			window[2] = database[basePick].pixels[i][j + 1];
			window[3] = database[basePick].pixels[i][j - 1];
			window[4] = database[basePick].pixels[i][j];
			window[5] = database[basePick].pixels[i][j + 1];
			window[6] = database[basePick].pixels[i + 1][j - 1];
			window[7] = database[basePick].pixels[i + 1][j];
			window[8] = database[basePick].pixels[i + 1][j + 1];

			bubbleSort(window);

			database[basePick].pixels[i][j] = window[4];
		}
	}

int createHistogram(image* database, int basePick) {
	int* histogram = 0;
	char filename[255];
	int size = database[basePick].depth + 1;

	histogram = malloc(size * sizeof(int));
	int x = 0;

	for (int x = 0; x < size; x++) {
		histogram[x] = 0;
		for (int i = 0; i < database[basePick].height; i++) {
			for (int j = 0; j < database[basePick].width; j++) {
				if (x == database[basePick].pixels[i][j])
					histogram[x]++;
			}
		}
	}

	printf("\nPodaj nazwe pliku do ktorego zapisac histogram: ");
	scanf("%s", &filename);
	FILE* csv;
	csv = fopen(filename, "w");

	for (int x = 0; x < size; x++) {
		fprintf(csv, "%d, %d\n", x, histogram[x]);
	}
	fclose(csv);

	free(histogram);
}