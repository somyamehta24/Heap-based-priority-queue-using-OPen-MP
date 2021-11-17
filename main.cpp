#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
#include "Sort.h"
#include "Delete.h"
#include "Insert.h"

void PrintHEAP() {
    for (int i = 1; i <= heapSize; i++) {
        cout << i << " th node  ";
        for (int j = 0; j < Heap[i].elements.size(); j++)
            cout << Heap[i].elements[j] << " ";
        cout << "\n";
    }
}

void PrintpBuffer() {
    cout << endl
         << "printing Buffer" << endl;
    for (auto i : pBuffer.elements)
        cout << i << " ";
    cout << endl;
}

// Driver Function
int main() {
    omp_init_lock(writelock);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    cout << "Entert size of Node\n";
    cin >> K;
    pBuffer.elements = vector<int>(K, 1000000000);
    int q;
    cout << "Enter number of operations\n";
    cin >> q;
    vector<vector<int>> queries;
    vector<int> ans;
    int query[q];
    for (int i = 0; i < q; i++) {
        cout << "1)Insert  2)Delete \n";
        int x;
        cin >> x;
        if (x == 1)  // insert Node
        {
            query[i] = 0;
            int y;
            cout << "No. of elements you want to insert\n";
            cin >> y;
            vector<int> v;
            cout << "Enter " << y << " elements\n";
            for (int j = 0; j < y; j++) {
                int val;
                cin >> val;
                v.push_back(val);
            }
            // v.push_back(0);
            queries.push_back(v);
        } else  // delete
        {
            query[i] = 1;
            cout << "Enter the number of elements to delete\n";
            int num;
            cin >> num;
            queries.push_back({num});
        }
    }

    Node removedItems;
#pragma omp parallel for
    for (int i = 0; i < q; i++) {
        int id = omp_get_thread_num();
#pragma omp critical
        {
            if (query[i] == 1)
                DeleteMin(removedItems, queries[i][0], id);
            else {
                Node items;
                items.elements = queries[i];
                int c = queries[i].size();
                insert(items, c, id);
            }
        }
    }
    omp_destroy_lock(writelock);
    PrintHEAP();
    PrintpBuffer();

    return 0;
}