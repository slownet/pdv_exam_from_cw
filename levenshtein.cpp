#include <limits>
#include <climits>
#include <omp.h>
#include <iostream>
#include "levenshtein.h"


// Vyber nejmensiho prvku ze tri
unsigned int trimin(unsigned int a, unsigned int b, unsigned int c) {
    unsigned short cmin = std::min(a, b);
    return (c < cmin) ? c : cmin;
}


// Struktura implementujici tabulku dynamickeho programovani
class dynprog_table {
private:
    const unsigned int height;
    const unsigned int width;
    std::vector<unsigned short> data;

public:
    dynprog_table(unsigned int height, unsigned int width) : height(height),
                                                             width(width),
                                                             data(height * width) {}

    inline unsigned short &operator()(unsigned int i, unsigned int j) {
        return data[i * width + j];
    }
};

// Metoda pro pocitani Levenshteinovy vzdalenosti pomoci dynamickeho programovani
// Tuto metodu NEPARALELIZUJTE!
unsigned int string_distance(const std::string &a, const std::string &b) {
    unsigned int len_a = a.length();
    unsigned int len_b = b.length();

    dynprog_table table(len_b + 1, len_a + 1);

    for (unsigned int j = 0; j <= len_a; j++) {
        table(0, j) = j;
    }
    for (unsigned int i = 0; i <= len_b; i++) {
        table(i, 0) = i;
    }

    for (unsigned int i = 1; i <= len_b; i++) {
        for (unsigned int j = 1; j <= len_a; j++) {
            unsigned int diag = table(i - 1, j - 1);
            unsigned int left = table(i, j - 1);
            unsigned int up = table(i - 1, j);

            table(i, j) = trimin(left + 1, up + 1, (a[j - 1] == b[i - 1]) ? diag : diag + 1);
        }
    }

    return table(len_b, len_a);
}

long distances(const std::vector<std::string> &vector, unsigned int &maxIdx) {
    long result = 0;

    int computer_threads_count = 8;

    std::vector<unsigned int> local_max_idx(computer_threads_count);
    std::vector<unsigned int> local_max_dist(computer_threads_count);


#pragma omp parallel default(none) shared(result, local_max_idx, local_max_dist, vector)
    {
        unsigned int curr_max_distance = 0;
        unsigned int curr_max_idx = 0;

        auto chunk_size = 1 + vector.size() / omp_get_num_threads();

        unsigned int local_result = 0;

        auto thread_id = omp_get_thread_num();
        auto start = thread_id * chunk_size;
        auto end = std::min(chunk_size * (thread_id + 1), vector.size());


        for (int i = start; i < end; ++i) {
            for (int j = i + 1; j < vector.size(); ++j) {

                // Spocitame vzdalenost dvou slov od sebe
                auto distance = string_distance(vector[i], vector[j]);
                // Pokud je vzdalenost nejvetsi dosud nalezena, provedeme update indexu
                // Dulezite: pokud je maximalni vzdalenost nabyvana na nekolika indexech,
                // pamatujeme si vzdy jen ten nejmensi
                if (distance > curr_max_distance) {
                    curr_max_distance = distance;
                    curr_max_idx = i * vector.size() + j;
                }

                // Kumulujeme vzdalenosti
                local_result += distance;
            }
        }

        local_max_idx[omp_get_thread_num()] = curr_max_idx;
        local_max_dist[omp_get_thread_num()] = curr_max_distance;

#pragma omp critical
        {
            result += local_result;
        }
    }
    // Pry zvyseni rychlosti zapisujeme na odkaz jen jedno - na konci

    auto maxIndex = -1;
    auto maxDist = 0;

    for (int i = 0; i < local_max_dist.size(); i++) {
        if (local_max_dist[i] > maxDist) {
            if (local_max_idx[i] < maxIndex) {
                maxIndex = local_max_idx[i];
            }
        }
    }

    maxIdx = maxIndex;
    return result;
}

long distances_original(const std::vector<std::string> &vector, unsigned int &maxIdx) {
    long result = 0;
    unsigned int distance = 0;
    unsigned int maxDistance = 0;
    unsigned int maxIndex = 0;
    for (int i = 0; i < vector.size(); ++i)
        for (int j = i + 1; j < vector.size(); ++j) {
            // Spocitame vzdalenost dvou slov od sebe
            distance = string_distance(vector[i], vector[j]);
            // Pokud je vzdalenost nejvetsi dosud nalezena, provedeme update indexu
            // Dulezite: pokud je maximalni vzdalenost nabyvana na nekolika indexech,
            // pamatujeme si vzdy jen ten nejmensi
            if (distance > maxDistance) {
                maxIndex = i * vector.size() + j;
                maxDistance = distance;
            }
            // Kumulujeme vzdalenosti
            result += distance;
        }
    // Pry zvyseni rychlosti zapisujeme na odkaz jen jedno - na konci
    maxIdx = maxIndex;
    return result;
}