#include <iostream>
#include <vector>
#include <pthread.h>
#include <climits>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <chrono>

using namespace std;

const int MAX_THREADS = 8;

struct ThreadData {
    int thread_id;
    int start, end;
    int* array;
    int extreme_val;
    int extreme_index;
    bool ascending;
};

// T?m min ho?c max trong ðo?n ðý?c giao
void* findLocalExtreme(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int local_val = data->ascending ? INT_MAX : INT_MIN;
    int local_index = data->start;

    for (int i = data->start; i <= data->end; i++) {
        if ((data->ascending && data->array[i] < local_val) ||
            (!data->ascending && data->array[i] > local_val)) {
            local_val = data->array[i];
            local_index = i;
        }
    }

    data->extreme_val = local_val;
    data->extreme_index = local_index;

    pthread_exit(nullptr);
}

// Selection Sort song song: có th? s?p x?p tãng ho?c gi?m
void parallelSelectionSort(vector<int>& array, int num_threads, bool ascending) {
    int n = array.size();

    for (int i = 0; i < n - 1; i++) {
        pthread_t threads[MAX_THREADS];
        ThreadData thread_data[MAX_THREADS];

        int chunk_size = (n - i) / num_threads;
        int remainder = (n - i) % num_threads;

        int start = i;
        for (int t = 0; t < num_threads; t++) {
            thread_data[t].thread_id = t;
            thread_data[t].start = start;
            thread_data[t].end = start + chunk_size - 1;
            if (remainder > 0) {
                thread_data[t].end++;
                remainder--;
            }
            thread_data[t].array = array.data();
            thread_data[t].ascending = ascending;

            pthread_create(&threads[t], nullptr, findLocalExtreme, (void*)&thread_data[t]);
            start = thread_data[t].end + 1;
        }

        int global_val = ascending ? INT_MAX : INT_MIN;
        int global_index = i;

        for (int t = 0; t < num_threads; t++) {
            pthread_join(threads[t], nullptr);
            if ((ascending && thread_data[t].extreme_val < global_val) ||
                (!ascending && thread_data[t].extreme_val > global_val)) {
                global_val = thread_data[t].extreme_val;
                global_index = thread_data[t].extreme_index;
            }
        }

        swap(array[i], array[global_index]);
    }
}

// Ð?c m?ng t? file input.txt
bool readInputFile(const string& filename, vector<int>& arr) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Không th? m? file " << filename << endl;
        return false;
    }

    string line;
    getline(infile, line);
    stringstream ss(line);
    int num;
    while (ss >> num) {
        arr.push_back(num);
    }

    infile.close();
    return true;
}

// In m?ng
void printArray(const vector<int>& arr) {
    for (int x : arr) cout << x << " ";
    cout << "\n";
}

int main() {
    vector<int> original;
    string filename = "input.txt";
    int num_threads = 4;

    if (!readInputFile(filename, original)) return 1;

    // Sao chép m?ng g?c ra 2 m?ng ð? s?p x?p
    vector<int> arr_asc = original;
    vector<int> arr_desc = original;

    // S?p x?p tãng d?n
    cout << "Mang goc:\n";
    printArray(original);

    auto start_asc = chrono::high_resolution_clock::now();
    parallelSelectionSort(arr_asc, num_threads, true);
    auto end_asc = chrono::high_resolution_clock::now();

    cout << "\nMang sau khi sap xep TANG dan:\n";
    printArray(arr_asc);
    cout << "Thoi gian (tang dan): "
         << chrono::duration<double>(end_asc - start_asc).count() << " giay\n";

    // S?p x?p gi?m d?n
    auto start_desc = chrono::high_resolution_clock::now();
    parallelSelectionSort(arr_desc, num_threads, false);
    auto end_desc = chrono::high_resolution_clock::now();

    cout << "\nMang sau khi sap xep GIAM dan:\n";
    printArray(arr_desc);
    cout << "Thoi gian (giam dan): "
         << chrono::duration<double>(end_desc - start_desc).count() << " giay\n";

    return 0;
}

