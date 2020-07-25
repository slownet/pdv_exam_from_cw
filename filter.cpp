#include <cmath>
#include <cstring>
#include "data_table.h"
#include <omp.h>

bool is_valid_point(int i, int j, int rows_count, int col_count) { // no longer used
    return (i < rows_count) and (j < col_count) and (i >= 0) and (j >= 0);
}

void filtering_original(data_table &table, unsigned int iterations) {
    const int S = table.getSize();

    for (int iter = 0; iter < iterations; ++iter) {
        for (int i = 0; i < S; ++i) {
            for (int j = 0; j < S; ++j) {
                int old_value = table(i, j);
                int new_value = old_value * old_value;

                // Update hodnoty pocitame na zaklade 4-okoli
                new_value += (i - 1 >= 0) ? table(i - 1, j) * table(i - 1, j) : 0;
                new_value += (j - 1 >= 0) ? table(i, j - 1) * table(i, j - 1) : 0;
                new_value += (i + 1 < S) ? table(i + 1, j) * table(i + 1, j) : 0;
                new_value += (j + 1 < S) ? table(i, j + 1) * table(i, j + 1) : 0;

                table(i, j) = std::sqrt(new_value / 3);
            }
        }
    }
}


void filtering(data_table &table, unsigned int iterations) {
    const int S = table.getSize();

    // rows and columns variables added for testing on some general matrix (non square)
    int rows = S;
    int cols = S;

    int diagonal_length = 0;
    int longest_diagonal = std::min(cols, rows);

    for (int iter = 0; iter < iterations; ++iter) {

        // diagonals starting at the first column
        for (int r = 0; r < rows; ++r) {
            auto i = r;
            auto j = 0;

            diagonal_length = std::min(diagonal_length + 1, longest_diagonal);

#pragma omp parallel for default(none) shared(table, diagonal_length, i, j, S)
            for (int k = 0; k < diagonal_length; ++k) {
                int curr_i = i - k;
                int curr_j = j + k;

                int old_value = table(curr_i, curr_j);
                int new_value = old_value * old_value;

                // Update hodnoty pocitame na zaklade 4-okoli
                new_value += (curr_i - 1 >= 0) ? table(curr_i - 1, curr_j) * table(curr_i - 1, curr_j) : 0; // n
                new_value += (curr_j - 1 >= 0) ? table(curr_i, curr_j - 1) * table(curr_i, curr_j - 1) : 0; // w
                new_value += (curr_i + 1 < S) ? table(curr_i + 1, curr_j) * table(curr_i + 1, curr_j) : 0; // s
                new_value += (curr_j + 1 < S) ? table(curr_i, curr_j + 1) * table(curr_i, curr_j + 1) : 0; // e

                table(curr_i, curr_j) = std::sqrt(new_value / 3);
            }
        }


        // diagonals starting at the last row
        for (int c = 1; c < cols; ++c) { // first one was processed in prev cycle -> column index starts from 1
            auto i = rows - 1;
            auto j = c;

            diagonal_length = (cols - c) < rows ? cols - c : longest_diagonal;

#pragma omp parallel for default(none) shared(table, diagonal_length, i, j, S)
            for (int k = 0; k < diagonal_length; ++k) {
                int curr_i = i - k;
                int curr_j = j + k;

                int old_value = table(curr_i, curr_j);
                int new_value = old_value * old_value;

                // Update hodnoty pocitame na zaklade 4-okoli
                new_value += (curr_i - 1 >= 0) ? table(curr_i - 1, curr_j) * table(curr_i - 1, curr_j) : 0; // n
                new_value += (curr_j - 1 >= 0) ? table(curr_i, curr_j - 1) * table(curr_i, curr_j - 1) : 0; // w
                new_value += (curr_i + 1 < S) ? table(curr_i + 1, curr_j) * table(curr_i + 1, curr_j) : 0; // s
                new_value += (curr_j + 1 < S) ? table(curr_i, curr_j + 1) * table(curr_i, curr_j + 1) : 0; // e

                table(curr_i, curr_j) = std::sqrt(new_value / 3);
            }
        }

        diagonal_length = 0;
    } // iterations loop
} // filtering function end
