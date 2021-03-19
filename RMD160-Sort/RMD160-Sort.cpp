#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <cstdlib>

#define RMD160LENGTH 20

static void swap(uint8_t* a, uint8_t* b)
{
	uint8_t t[RMD160LENGTH];
	memcpy(t, a, RMD160LENGTH);
	memcpy(a, b, RMD160LENGTH);
	memcpy(b, t, RMD160LENGTH);
}

static void heapify(uint8_t* arr, int64_t n, int64_t i)
{
	int64_t largest = i;
	int64_t l = 2 * i + 1;
	int64_t r = 2 * i + 2;
	if (l < n && memcmp(arr + (l * RMD160LENGTH), arr + (largest * RMD160LENGTH), RMD160LENGTH) > 0)
		largest = l;
	if (r < n && memcmp(arr + (r * RMD160LENGTH), arr + (largest * RMD160LENGTH), RMD160LENGTH) > 0)
		largest = r;
	if (largest != i) {
		swap(arr + (i * RMD160LENGTH), arr + (largest * RMD160LENGTH));
		heapify(arr, n, largest);
	}
}

static void heapsort(uint8_t* arr, int64_t n)
{
	int64_t i;
	for (i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);
	for (i = n - 1; i > 0; i--) {
		swap(arr, arr + (i * RMD160LENGTH));
		heapify(arr, i, 0);
	}
}


static int64_t partition(uint8_t* arr, int64_t n)
{
	uint8_t pivot[RMD160LENGTH];
	int64_t j, i, t, r = (int64_t)n / 2, jaux = -1, iaux = -1, iflag, jflag;
	i = -1;
	memcpy(pivot, arr + (r * RMD160LENGTH), RMD160LENGTH);
	i = 0;
	j = n - 1;
	do {
		iflag = 1;
		jflag = 1;
		t = memcmp(arr + (i * RMD160LENGTH), pivot, RMD160LENGTH);
		iflag = (t <= 0);
		while (i < j && iflag) {
			i++;
			t = memcmp(arr + (i * RMD160LENGTH), pivot, RMD160LENGTH);
			iflag = (t <= 0);
		}
		t = memcmp(arr + (j * RMD160LENGTH), pivot, RMD160LENGTH);
		jflag = (t > 0);
		while (i < j && jflag) {
			j--;
			t = memcmp(arr + (j * RMD160LENGTH), pivot, RMD160LENGTH);
			jflag = (t > 0);
		}
		if (i < j) {
			if (i == r) {
				r = j;
			}
			else {
				if (j == r) {
					r = i;
				}
			}

			swap(arr + (i * RMD160LENGTH), arr + (j * RMD160LENGTH));
			jaux = j;
			iaux = i;
			j--;
			i++;
		}

	} while (j > i);
	if (jaux != -1 && iaux != -1) {
		if (iflag || jflag) {
			if (iflag) {
				if (r != j)
					swap(arr + (r * RMD160LENGTH), arr + ((j)*RMD160LENGTH));
				jaux = j;
			}
			if (jflag) {
				if (r != j - 1)
					swap(arr + (r * RMD160LENGTH), arr + ((j - 1) * RMD160LENGTH));
				jaux = j - 1;
			}
		}
		else {
			if (r != j)
				swap(arr + (r * RMD160LENGTH), arr + ((j)*RMD160LENGTH));
			jaux = j;
		}
	}
	else {
		if (iflag && jflag) {
			jaux = r;
		}
		else {
			if (iflag) {
				swap(arr + (r * RMD160LENGTH), arr + ((j)*RMD160LENGTH));
				jaux = j;
			}
		}
	}
	return jaux;
}


static void insertionsort(uint8_t* arr, int64_t n)
{
	int64_t j, i;
	uint8_t* arrj;
	uint8_t key[RMD160LENGTH];
	for (i = 1; i < n; i++) {
		j = i - 1;
		memcpy(key, arr + (i * RMD160LENGTH), RMD160LENGTH);
		arrj = arr + (j * RMD160LENGTH);
		while (j >= 0 && memcmp(arrj, key, RMD160LENGTH) > 0) {
			memcpy(arr + ((j + 1) * RMD160LENGTH), arrj, RMD160LENGTH);
			j--;
			if (j >= 0) {
				arrj = arr + (j * RMD160LENGTH);
			}
		}
		memcpy(arr + ((j + 1) * RMD160LENGTH), key, RMD160LENGTH);
	}
}

static void introsort(uint8_t* arr, int64_t depthLimit, int64_t n)
{
	int64_t p;
	if (n > 1) {
		if (n <= 16) {
			insertionsort(arr, n);
		}
		else {
			if (depthLimit == 0) {
				heapsort(arr, n);
			}
			else {
				p = partition(arr, n);
				if (p >= 2) {
					introsort(arr, depthLimit - 1, p);
				}
				if ((n - (p + 1)) >= 2) {
					introsort(arr + ((p + 1) * RMD160LENGTH), depthLimit - 1, n - (p + 1));
				}
			}
		}
	}
}

static void sort(uint8_t* arr, int64_t n)
{
	int64_t depthLimit = ((int64_t)ceil(log(n))) * 2;
	introsort(arr, depthLimit, n);
}

static void write_file(const uint8_t* DATA, int64_t N, int64_t L, const char* filename)
{
	FILE* fd = fopen(filename, "wb");
	if (fd == NULL) {
		printf("Error: not able to open output file: %s\n", filename);
		exit(1);
	}
	fwrite(DATA, 1, N * L, fd);
	fclose(fd);
}

static void sort_file(const char* in_filename, const char* out_filename)
{
	if (in_filename == NULL || out_filename == NULL) {
		printf("Error: file names are NULL\n");
		exit(1);
	}

	FILE* fd = fopen(in_filename, "rb");
	if (fd == NULL) {
		printf("Error: not able to open input file: %s\n", in_filename);
		exit(1);
	}

	int64_t N = 0;
	_fseeki64(fd, 0, SEEK_END);
	int64_t TOTAL = _ftelli64(fd);
	N = TOTAL / RMD160LENGTH;
	rewind(fd);

	printf("Total addresses: %llu\n", N);

	if (TOTAL >= INT64_MAX - 1) {
		printf("Error: more then INT64_MAX addresses found in the file.\n");
		exit(1);
	}

	auto* DATA = (uint8_t*)malloc(N * RMD160LENGTH);
	memset(DATA, 0, N * RMD160LENGTH);

	printf("Reading data...\n", N);
	fread(DATA, 1, N * RMD160LENGTH, fd);
	fclose(fd);
	printf("Reading data complete\n", N);

	printf("Sorting data...\n", N);
	sort(DATA, N);
	insertionsort(DATA, N);
	printf("Sorting data complete\n", N);

	printf("Saving data...\n", N);
	write_file(DATA, N, RMD160LENGTH, out_filename);
	printf("Saving data complete\n", N);

	free(DATA);
}

int main(int argc, const char* argv[])
{
	if (argc != 3) {
		printf("Error: wrong args\n");
		printf("Usage: %s in_file out_file\n", argv[0]);
		exit(1);
	}
	printf("\n");
	sort_file(argv[1], argv[2]);

	return 0;
}
