
void  appendTo(Node &pBuffer, int pos, Node &items) {
    int sz = items.elements.size(), i = 0;
    while (sz > 0) {
        pBuffer.elements[pos++] = items.elements[i++];
        sz--;
    }
}
void InsertHeapify(int curr, int tar, Node items) {
    // if(curr==0 || Heap[tar].state=="MARKED")return curr;
    // lock(curr) 
    // unlock(parent[curr])
    if (curr == 0)
        return;
    // omp_set_lock(&writelock[curr]);
    int sizeOfcurr = Heap[curr].elements.size();
    pair<Node, Node> p = Sort_Split(Heap[curr], sizeOfcurr, items, (items.elements).size(), sizeOfcurr);
    Heap[curr] = p.first;
    items = p.second;
    Heap[tar] = items;
    // omp_unset_lock(&writelock[curr]);
    InsertHeapify(curr / 2, curr, Heap[curr]);
}
bool partial_Insert(Node &items, int siz) {
    if (heapSize == 0) {
        Heap[root] = items;
        sizeOfRoot = siz;
        // Heap[tar].state="AVAIL";
        heapSize = heapSize + 1;
        // unlock root;
        omp_unset_lock(&writelock[root]);

        return true;
    }
    pair<Node, Node> p = Sort_Split(Heap[root], sizeOfRoot, items, siz, sizeOfRoot);
    Heap[root] = p.first;
    items = p.second;
    if (sizeOfpBuffer + siz < K) {
        appendTo(pBuffer, sizeOfpBuffer, items);
        sizeOfpBuffer = sizeOfpBuffer + siz;
        omp_unset_lock(&writelock[root]);

        // unlock root;
        return true;
    } else {
        
        Sort(pBuffer, siz);
        
        pair<Node, Node> p = Sort_Split(items, siz, pBuffer, sizeOfpBuffer, K);
        items = p.first;
        pBuffer = p.second;
        sizeOfpBuffer = sizeOfpBuffer + siz - K;
        return false;
    }
    
}
// Function Inserts a Node in the Heap
void insert(Node items, int siz, int id) {
    cout << "Running insert from Thread: " <<id<< endl;
    // #pragma omp critical
    // {
    Sort(items, siz);
    // place a lock on root;
    omp_set_lock(&writelock[root]);
    cout<<"Running partial_Insert from Thread: "<<id<<endl;
    if (partial_Insert(items, siz))
        return;
    cout<<"Completed partial_Insert from Thread: "<<id<<endl;
    heapSize = heapSize + 1;
    int tar = heapSize;
    // place a lock on tar;
    omp_set_lock(&writelock[tar]);
    Heap[tar].state="TARGET";
    // unlock tar;
    omp_unset_lock(&writelock[tar]);
    cout << "Running InsertHeapify from Thread: " <<id<< endl;
    omp_unset_lock(&writelock[root]);
    InsertHeapify(heapSize / 2, tar, items);
    // }
    cout<<"Completed InsertHeapify from Thread: "<<id<<endl;


    cout<<"Completed insert from Thread: "<<id<<endl;
    // lock tar;
    // omp_set_lock(&writelock[tar]);

    // unlock parent(cur);
    // if(Heap[tar].state=="TARGET")
    // {
    //     Heap[tar]=items;
    //     Heap[tar].state="AVAIL";
    // }
    // else
    // {
    //     Heap[root]=items;
    //     sizeOfRoot=K;
    //     Heap[root].state="AVAIL";
    //     Heap[tar].state="EMPTY";
    // }
    // omp_unset_lock(&writelock[tar]);
    // unlock tar;
}