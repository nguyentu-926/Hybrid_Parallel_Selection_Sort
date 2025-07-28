#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

void printArray(const vector<int>& arr) {
    for (int x : arr) cout << x << " ";
    cout << endl;
}
void selectionSortAscending(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        swap(arr[i], arr[minIdx]);
    }
}
void selectionSortDescending(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[maxIdx]) maxIdx = j;
        }
        swap(arr[i], arr[maxIdx]);
    }
}

int main() {
    vector<int> numbers;
    ifstream fin("input.txt");
    if (!fin) {
        cerr << "Không th? m? file input.txt\n";
        return 1;
    }

    int x;
    while (fin >> x) {
        numbers.push_back(x);
    }
    fin.close();

    cout << "Mang ban dau:\n";
    printArray(numbers);
    vector<int> asc = numbers;
    auto start1 = high_resolution_clock::now();
    selectionSortAscending(asc);
    auto end1 = high_resolution_clock::now();
    cout << "\nMang tang dan:\n";
    printArray(asc);
    cout << "Thoi gian sap xep tang dan: "
         << duration<double>(end1 - start1).count() << " giay\n";
    vector<int> desc = numbers;
    auto start2 = high_resolution_clock::now();
    selectionSortDescending(desc);
    auto end2 = high_resolution_clock::now();
    cout << "\nMang giam dan:\n";
    printArray(desc);
    cout << "Thoi gian sap xep giam dan: "
         << duration<double>(end2 - start2).count() << " giay\n";

    return 0;
}

