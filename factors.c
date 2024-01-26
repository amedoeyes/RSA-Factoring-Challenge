#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

void factor(mpz_t n, mpz_t fact) {
	if (mpz_even_p(n)) {
		mpz_set_ui(fact, 2);
		return;
	}

	mpz_set_ui(fact, 3);
	while (mpz_cmp_ui(n, 1) > 0) {
		if (mpz_divisible_p(n, fact)) return;
		mpz_add_ui(fact, fact, 2);
	}
}

void pollard_rho(mpz_t n, mpz_t fact) {
	mpz_t x, y;
	mpz_inits(x, y, NULL);

	mpz_set_ui(x, 2);
	mpz_set(y, x);
	mpz_set_ui(fact, 1);

	while (mpz_cmp_ui(fact, 1) == 0) {
		mpz_pow_ui(x, x, 2);
		mpz_add_ui(x, x, 1);
		mpz_mod(x, x, n);

		mpz_pow_ui(y, y, 2);
		mpz_add_ui(y, y, 1);
		mpz_mod(y, y, n);

		mpz_pow_ui(y, y, 2);
		mpz_add_ui(y, y, 1);
		mpz_mod(y, y, n);

		mpz_sub(fact, x, y);
		mpz_abs(fact, fact);
		mpz_gcd(fact, fact, n);
	}

	mpz_clears(x, y, NULL);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}

	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		printf("Error opening file.\n");
		return 1;
	}

	char *buf = NULL;
	size_t size = 0;
	ssize_t read = 0;

	mpz_t num, div, fact;
	mpz_inits(num, div, fact, NULL);

	while ((read = getline(&buf, &size, f)) != -1) {
		mpz_init_set_str(num, buf, 10);
		pollard_rho(num, fact);
		mpz_div(div, num, fact);
		gmp_printf("%Zd=%Zd*%Zd\n", num, div, fact);
	}

	mpz_clears(num, div, fact, NULL);
	free(buf);
	fclose(f);

	return 0;
}
