#include "Sorting.h"

bool lessThan(const Vehicle& a, const Vehicle& b, int key) {
    if (key == 1) return a.getID() < b.getID();
    if (key == 2) return a.getCapacity() < b.getCapacity();
    if (key == 3) return a.gettype() < b.gettype();
    return a.getplate() < b.getplate();
}

void insertionSort(Vehicle a[], int n, int key) {
    for (int i = 1; i < n; i++) {
        Vehicle temp = a[i];
        int j = i - 1;
        while (j >= 0 && lessThan(temp, a[j], key)) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = temp;
    }
}

void mergeParts(Vehicle a[], Vehicle tmp[], int lo, int mid, int hi, int key) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (lessThan(a[j], a[i], key)) tmp[k++] = a[j++];
        else tmp[k++] = a[i++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi) tmp[k++] = a[j++];
    for (int x = lo; x <= hi; x++) a[x] = tmp[x];
}

void merger(Vehicle a[], Vehicle tmp[], int lo, int hi, int key) {
    if (lo >= hi) return;
    int mid = (lo + hi) / 2;
    merger(a, tmp, lo, mid, key);
    merger(a, tmp, mid + 1, hi, key);
    mergeParts(a, tmp, lo, mid, hi, key);
}

void mergeSort(Vehicle a[], int n, int key) {
    if (n < 2) return;
    Vehicle* tmp = new Vehicle[n];
    merger(a, tmp, 0, n - 1, key);
    delete[] tmp;
}

int binarysearch(Vehicle q[], int T, const string& id) {
    int lo = 0, hi = T - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (q[mid].getID() == id) return mid;
        if (q[mid].getID() < id) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}
