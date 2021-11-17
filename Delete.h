void Extract_Root(Node &items, int s) {
    int itemssize = items.elements.size();
    cout << itemssize << "\n";
    for (int i = 0; i < s; i++) {
        items.elements.push_back(Heap[root].elements[i]);
    }
    for (int i = 0; i < sizeOfRoot - s; i++) {
        Heap[root].elements[i] = Heap[root].elements[s + i];
    }
    for (int i = sizeOfRoot - s; i < sizeOfRoot; i++)
        Heap[root].elements.pop_back();
    sizeOfRoot = sizeOfRoot - s;
}

void DeleteMin(Node &items, int siz, int id) {
    cout << "Running Partial DeleteMin from Thread: " <<id<<endl;
    // lock root;
    // #pragma omp critical
    // {
    // omp_set_lock(&writelock[root]);
    if (parital_DeleteMin(items, siz))
        return;
    cout << "Completed Partial DeleteMin from Thread: "<<id<<endl;
    remainedSize = siz - items.elements.size();
    int tar = heapSize;
    heapSize = heapSize - 1;                     
    // lock tar;
    // omp_set_lock(&writelock[tar]);
    if (Heap[tar].state == "TARGET") {
        Heap[tar].state = "MARKED";
        // unlock tar;
        // omp_unset_lock(&writelock[tar]);
        while (Heap[root].state != "AVAIL")
            ;
    } else {
        Heap[root] = Heap[tar];
        Heap[tar].state = "EMPTY";
        // unlock tar;
        // omp_unset_lock(&writelock[tar]);
        Heap[root].state = "AVAIL";
        sizeOfRoot = Heap[tar].elements.size();
        pair<Node, Node> p = Sort_Split(Heap[root], K, pBuffer, sizeOfpBuffer, K);
        Heap[root] = p.first;
        // pBuffer = p.first;
    }
    cout << "Running Partial DeleteMInHeapify from Thread: " <<id<<endl;
    DeleteMin_Heapify(root, items, remainedSize);
    // }
    cout << "Completed Partial DeleteMInHeapify from Thread: "<<id<<endl;
    // omp_unset_lock(&writelock[root]);
}
bool parital_DeleteMin(Node &items, int siz) {
    if (heapSize == 0) {
        siz = 0;
        // unlock root;
        // omp_unset_lock(&writelock[root]);
        return true;
    } else if (siz < sizeOfRoot) {
        Extract_Root(items, siz);
        // unlock tar;
        // omp_unset_lock(&writelock[tar]);
        return true;
    } else {
        items = Heap[root];
        sizeOfRoot = 0;
        if (heapSize == 1) {
            sizeOfRoot = sizeOfpBuffer;
            sizeOfpBuffer = 0;
            Sort(pBuffer, sizeOfRoot);
            Heap[root] = pBuffer;
            for (int i = 0; i < sizeOfRoot; i++)
                pBuffer.elements[i] = 0;
            // siz = min((int)items.elements.size() + sizeOfRoot, siz);
            // Extract_Root(items, siz - items.elements.size());
            // unlock root;
            return true;
        } else {
            Heap[root].state = "EMPTY";
            return false;
        }
    }
}
// Extract Root

// Return min element of node in heap[i]
int calcMinElementHeap(int i) {
    return Heap[i].elements[0];
}

// Return max element of node in heap[i]
int calcMaxElementHeap(int i) {
    int n = Heap[i].elements.size();

    return Heap[i].elements[n - 1];
}

// DeleteMin_Heapify
void DeleteMin_Heapify(int cur, Node &items,
                       int remainedSize)  // 1,items(root),0
{
    int l = 2 * cur;
    int r = 2 * cur + 1;
    if (l > heapSize || r > heapSize) return;

    // LOCK(l); LOCK(r);
    // omp_set_lock(&writelock[l]);
    // omp_set_lock(&writelock[r]);
    int maxcur = 0;
    for (auto i : Heap[cur].elements)
        maxcur = max(maxcur, i);

    if (maxcur <= min(calcMinElementHeap(l), calcMinElementHeap(r))) {
        // if (cur == root)
        //     Extract_Root(items, remainedSize);
        // UNLOCK(cur);UNLOCK(l);UNLOCK(r);
        // omp_unset_lock(&writelock[cur]);
        // omp_unset_lock(&writelock[l]);
        // omp_unset_lock(&writelock[r]);
        return;
    }

    int x;
    int y;

    if (calcMaxElementHeap(l) > calcMaxElementHeap(r)) {
        x = l;
        y = r;
    } else {
        x = r;
        y = l;
    }
    pair<Node, Node> p1 = Sort_Split(Heap[l], Heap[l].elements.size(), Heap[r], Heap[r].elements.size(), Heap[l].elements.size());

    Heap[y] = p1.first;
    Heap[x] = p1.second;

    // UNLOCK(x);
    omp_unset_lock(&writelock[x]);
    pair<Node, Node> p2 = Sort_Split(Heap[cur], Heap[cur].elements.size(), Heap[y], Heap[y].elements.size(), Heap[cur].elements.size());
    Heap[cur] = p2.first;
    Heap[y] = p2.second;

    // if (cur == root) {
    //     Extract_Root(items, remainedSize);
    // }

    // UNLOCK(cur);
    omp_unset_lock(&writelock[cur]);

    DeleteMin_Heapify(y, items, remainedSize);
}