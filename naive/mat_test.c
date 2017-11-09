#include "matrix.h"
#include "../testing/shared.c"

void (*matrix_functions[NUM_OPS]) = {dot_product, matrix_power, matrix_multiply, matrix_scale, matrix_add};

matrix* generate_random_matrix(int r, int c) {
	matrix *m;
	allocate_matrix(&m, r, c);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			set_loc(m, i, j, (float)drand48() * MAX_RAND_VALUE + .5);
		}
	}
	return m;
}

void write_matrices_to_file(matrix** matrices, int count, const char* filename) {
	FILE *f = fopen(filename, "w+");
	matrix *m;
	float *arr;
	fprintf(f, "%i\n", count);
	for (int i = 0; i < count; i++) {
		m = matrices[i];
		arr = malloc((m->dim.rows) * (m->dim.cols) * sizeof(float));
		get_matrix_as_array(arr, m);
		fprintf(f, "%i %i ", m->dim.rows, m->dim.cols);
		for (int j = 0; j < m->dim.rows * m->dim.cols; j++) {
			fprintf(f, "%a\t", arr[j]);
		}
		fprintf(f, "\n");
		free(arr);
	}
	fclose(f);
}

matrix** test_read_write_all_dims() {
	int i, count, dim_idx;
	count = DEFAULT_NUM_EACH_SIZE * NUM_SIZES;
	matrix** write_matrices = malloc(sizeof(matrix*) * count);
	for (dim_idx = 0; dim_idx < NUM_SIZES; dim_idx++) {
		int rows = row_numbers[dim_idx];
		int cols = column_numbers[dim_idx];
		for (i = 0; i < DEFAULT_NUM_EACH_SIZE; i++) {
			write_matrices[dim_idx * DEFAULT_NUM_EACH_SIZE + i] = generate_random_matrix(rows, cols);
		}
	}
	write_matrices_to_file(write_matrices, count, all_matrices_filename);
	matrix** read_matrices = read_matrices_from_file(all_matrices_filename);
	assert(check_equality(write_matrices, read_matrices, count));
	for (i = 0; i < count; i++) {
		free_matrix(read_matrices[i]);
	}
	free(read_matrices);
	return write_matrices;
}

void write_duration(long duration) {
	FILE *f = fopen(naive_duration_file, "w+");
	fprintf(f, "%li\n", duration);
	fclose(f);
}	

int main() {
	// matrix *m1, *m2, *m3, *m4;
	// float ans;

	// allocate_matrix(&m1, 2, 2);
	// set_loc(m1, 0, 0, 3);
	// set_loc(m1, 1, 1, 4);
	// print_matrix(m1);

	// allocate_matrix(&m2, 2, 2);
	// matrix_power(m1, 2, m2);
	// print_matrix(m2);
	// matrix_scale(m2, 2, m2);
	// print_matrix(m2);
	// print_multiplication(m1, m2);


	// allocate_matrix(&m3, 2, 1);
	// allocate_matrix(&m4, 2, 1);
	// set_loc(m3, 0, 0, 1);
	// set_loc(m4, 0, 0, 1);
	// dot_product(m3, m4, &ans);
	// printf("%.2f\n", ans);

	long start, end, duration;
	duration = 0;
	matrix **matrix_ans, **test_matrices; 
	int num_results;// = DEFAULT_NUM_EACH_SIZE*2 + (DEFAULT_NUM_EACH_SIZE * (DEFAULT_NUM_EACH_SIZE-1)/2) * 2;

	test_matrices = test_read_write_all_dims();
	printf("%s\n", "READING AND WRITING SANITY CHECK COMPLETE");

	start = timer();
	num_results = perform_all_ops(test_matrices, DEFAULT_NUM_EACH_SIZE * NUM_SIZES, &matrix_ans); //int perform_all_ops(matrix** matrices, int count, matrix ***matrix_ans_pointer)
	end = timer();
	printf("Start is: %li, End is: %li\n", start, end);
	duration += (end-start);
	printf("Duration is %li\n\n", duration);
	printf("%s\n", "DONE PERFORMING NAIVE OPERATIONS");
	write_matrices_to_file(matrix_ans, num_results, all_matrices_ops_filename);
	
	write_duration(duration);
	return 0;
}