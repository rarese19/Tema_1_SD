#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace std::chrono;

void selectionSort(int a[], int n){
    for(int i=0; i<n-1; i++)
        for(int j=i+1; j<n; j++)
            if(a[i] > a[j])
                swap(a[i], a[j]);
}

void merge(int a[], int start, int mid, int stop){
    int fh = mid - start + 1;
    int sh = stop - mid;
    int* templeft = new int[fh];
    int* tempright = new int[sh];
    for (int i=0; i<fh; i++)
        templeft[i] = a[start+ i];
    for (int i=0; i<sh; i++)
        tempright[i] = a[mid + i + 1];
    int indexa = start;
    int indexleft = 0;
    int indexright = 0;
    while (indexleft < fh && indexright < sh){
        if (templeft[indexleft] <= tempright[indexright]){
            a[indexa] = templeft[indexleft];
            indexleft ++;
        }
        else{
            a[indexa] = tempright[indexright];
            indexright ++;
        }
        indexa ++;
    }
    if (indexright < sh)
        while (indexright < sh){
            a[indexa] = tempright[indexright];
            indexa ++;
            indexright ++;
        }
    if (indexleft < fh)
        while (indexleft < fh){
            a[indexa] = templeft[indexleft];
            indexa ++;
            indexleft ++;
        }
    delete[] templeft;
    delete[] tempright;
}

void my_mergeSort(int a[], int start, int stop){
    if (start >= stop)
        return;
    int mid = start + (stop - start) / 2;
    my_mergeSort(a, start, mid);
    my_mergeSort(a, mid + 1, stop);
    merge(a, start, mid, stop);
}

void shellSort(int a[], int n){
    for (int g=n/2; g>0; g=g/2){
        for (int i=g; i<n; i++){
            int aux = a[i], j;
            for (j=i; j>=g && a[j-g]>aux; j=j-g)
                a[j] = a[j - g];
            a[j] = aux;
        }
    }
}

void countSort(int a[], int n){
    int m = 0, l = 0;
    for(int i=0; i<n; i++)
        if(a[i] > m)
            m = a[i];
    int* count = new int[m + 1];
    int* fin = new int[n];
    for(int i=0; i<=m; i++)
        count[i] = 0;
    for(int i=0; i<n; i++)
        count[a[i]] ++;
    for(int i=0; i<=m; i++)
        for(int j=1; j<=count[i]; j++){
            fin[l] = i;
            l ++;
        }
    for(int i=0; i<n; i++)
        a[i] = fin[i];
    delete[] count;
    delete[] fin;
}

void radixSort(int a[], int n)
{
    int maxdigits = 0, max = 0;
    int p=10, q=1, exp = 1;
    for (int i=0; i<n; i++)
        if (a[i] > max)
            max = a[i];
    while (max){
        maxdigits++;
        max /= 10;
    }
    for(int j=1; j<=maxdigits; j++){
        int* digits = new int[10], *fin = new int[n];
        for(int i=0; i<10; i++)
            digits[i] = 0;
        for(int i=0; i<n; i++)
            digits[a[i] / exp % 10]++;
        digits[0]--;
        for(int i=1; i<10; i++)
            digits[i] += digits[i - 1];
        for(int i=n-1; i>=0; i--){
            fin[digits[a[i] / exp % 10]] = a[i];
            digits[a[i] / exp % 10] --;
        }
        for(int i=0; i<n; i++)
            a[i] = fin[i];

        p*=10;
        q*=10;
        exp *= 10;
        delete[] digits;
        delete[] fin;
    }
}

int testSort(int a[], int n){
    for (int i=0; i<n-1; i++)
        if (a[i] > a[i+1])
            return 0;
    return 1;
}

int* to_sort = new int[100000300];
int* sort_1 = new int[100000300];
int* sort_2 = new int[100000300];
int* sort_3 = new int[100000300];

int main(){
    srand(time(NULL));
    ifstream f("teste.txt");
    ofstream g("teste_out_10^3.txt");
    long nrTeste, nrElemente, nrMaxim;
    f>>nrTeste;
    for (int test=1; test<=nrTeste; test++){
        f>>nrElemente>>nrMaxim;
        for (long i=0; i<nrElemente; i++){
            to_sort[i] = rand() % nrMaxim;
            sort_1[i] = to_sort[i];
            sort_2[i] = to_sort[i];
            sort_3[i] = to_sort[i];
        }
        g<<"Testul "<<test<<"\t"<<"Nr elemente "<<nrElemente<<"\t"<<"Numarul maxim "<<nrMaxim<<"\n\n";
    //INTROSORT 
        auto start = high_resolution_clock::now();
        sort(sort_1, sort_1 + nrElemente);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        g<<"\tIntro Sort a avut nevoie de "<<duration.count()<<" milisecunde";
        if (testSort(sort_1, nrElemente) == 1)
            g<<" si a sortat corect.\n";
        else{
            g<<", dar nu a sortat corect.\n";
        }      
    //SELECTION SORT
        if (nrElemente <= 100000){
            start = high_resolution_clock::now();
            selectionSort(sort_2, nrElemente);
            stop = high_resolution_clock::now();
            duration = duration_cast<milliseconds>(stop - start);
            g<<"\tSelection Sort a avut nevoie de "<<duration.count()<<" milisecunde";
            if (testSort(sort_2, nrElemente) == 1)
                g<<" si a sortat corect.\n";
            else{
                g<<", dar nu a sortat corect.\n";
            }
        }
        else{
            g<<"\tTimpul de executare este prea ridicat, Selection Sort nu este bun pentru a sorta atat ";
            g<<"de multe elemente.\n";
        }
    //MERGE SORT 
        start = high_resolution_clock::now();
        my_mergeSort(sort_3, 0, nrElemente);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        g<<"\tMerge Sort a avut nevoie de "<<duration.count()<<" milisecunde";
        if (testSort(sort_3, nrElemente) == 1)
            g<<" si a sortat corect.\n";
        else{
            g<<", dar nu a sortat corect.\n";
        }
    //RESET
        for (int i=0; i<nrElemente; i++){
            sort_1[i] = to_sort[i];
            sort_2[i] = to_sort[i];
            sort_3[i] = to_sort[i];
        }
    //SHELL SORT
        start = high_resolution_clock::now();
        shellSort(sort_1, nrElemente);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        g<<"\tShell Sort a avut nevoie de "<<duration.count()<<" milisecunde";
        if (testSort(sort_1, nrElemente) == 1)
            g<<" si a sortat corect.\n";
        else{
            g<<", dar nu a sortat corect.\n";
        }
    //COUNT SORT
        start = high_resolution_clock::now();
        countSort(sort_2, nrElemente);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        g<<"\tCount Sort a avut nevoie de "<<duration.count()<<" milisecunde";
        if (testSort(sort_2, nrElemente) == 1)
            g<<" si a sortat corect.\n";
        else{
            g<<", dar nu a sortat corect.\n";
        }
    //RADIX SORT BASE 10
        start = high_resolution_clock::now();
        radixSort(sort_3, nrElemente);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        g<<"\tRadix Sort in baza 10 a avut nevoie de "<<duration.count()<<" milisecunde";
        if (testSort(sort_3, nrElemente) == 1)
            g<<" si a sortat corect.\n\n\n";
        else{
            g<<", dar nu a sortat corect.\n";
        }  
    } 
    delete[] to_sort;
    delete[] sort_1;
    delete[] sort_2;
    delete[] sort_3;
    return 0;
}

