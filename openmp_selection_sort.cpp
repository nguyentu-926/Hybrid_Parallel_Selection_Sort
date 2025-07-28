#include <iostream>
#include <fstream>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

void hoanDoi(int &a, int &b) {
    int tam = a;
    a = b;
    b = tam;
}

// Ð?c m?ng t? file
int* docMang(int &n) {
    ifstream file("input.txt");
    if (!file) {
        cerr << "Khong mo duoc file input.txt\n";
        exit(1);
    }
    file >> n;
    int* a = new int[n];
    for (int i = 0; i < n; i++) file >> a[i];
    file.close();
    return a;
}

void inMang(int a[], int n) {
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << endl;
}

// S?p x?p tãng d?n (có OpenMP)
void sapXepTang(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        #pragma omp parallel
        {
            int localMin = minIdx;
            #pragma omp for nowait
            for (int j = i + 1; j < n; j++) {
                if (a[j] < a[localMin]) localMin = j;
            }
            #pragma omp critical
            {
                if (a[localMin] < a[minIdx]) minIdx = localMin;
            }
        }
        if (minIdx != i) hoanDoi(a[i], a[minIdx]);
    }
}

// S?p x?p gi?m d?n (có OpenMP)
void sapXepGiam(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        #pragma omp parallel
        {
            int localMax = maxIdx;
            #pragma omp for nowait
            for (int j = i + 1; j < n; j++) {
                if (a[j] > a[localMax]) localMax = j;
            }
            #pragma omp critical
            {
                if (a[localMax] > a[maxIdx]) maxIdx = localMax;
            }
        }
        if (maxIdx != i) hoanDoi(a[i], a[maxIdx]);
    }
}

int main() {
    int n;
    int* a = docMang(n);

    // In m?ng ban ð?u
    cout << "Mang ban dau:\n";
    inMang(a, n);
    cout << endl;

    // S?p x?p tãng d?n
    int* tang = new int[n];
    copy(a, a + n, tang);
    auto batDau1 = high_resolution_clock::now();
    sapXepTang(tang, n);
    auto ketThuc1 = high_resolution_clock::now();
    cout << "Mang sau khi sap xep tang dan:\n";
    inMang(tang, n);
    cout << "Thoi gian: " << duration_cast<milliseconds>(ketThuc1 - batDau1).count() << " ms\n\n";

    // S?p x?p gi?m d?n
    int* giam = new int[n];
    copy(a, a + n, giam);
    auto batDau2 = high_resolution_clock::now();
    sapXepGiam(giam, n);
    auto ketThuc2 = high_resolution_clock::now();
    cout << "Mang sau khi sap xep giam dan:\n";
    inMang(giam, n);
    cout << "Thoi gian: " << duration_cast<milliseconds>(ketThuc2 - batDau2).count() << " ms\n";

    // Gi?i phóng b? nh?
    delete[] a;
    delete[] tang;
    delete[] giam;
    return 0;
}

