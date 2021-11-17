int K, tar;  // size of nodes except root

// Definition of Node
struct Node {
    vector<int> elements;
    string state;
};

omp_lock_t writelock[1000000];

void Sort(Node &a1, int n1);
bool parital_DeleteMin(Node &items, int size);
void DeleteMin_Heapify(int cur, Node &items, int remainedSize);
// Definition of Heap
Node Heap[100000];  // child 2*i,2*i+1
const int root = 1;
int sizeOfRoot = 0, sizeOfpBuffer = 0, heapSize = 0;
Node pBuffer;

int remainedSize;

pair<Node, Node> Sort_Split(Node heap, int sizeOfRoot, Node root, int m, int n) {
    Node temp;
    temp.elements = heap.elements;
    vector<int> v = root.elements;
    m = root.elements.size();
    // n=heap.elements.size();
#pragma omp parallel for
        for (int i = 0; i < m; i++) {
            temp.elements.push_back(v[i]);
        }
    Sort(temp, m + n);
    n = min({n, K, (int)temp.elements.size()});
    Node hep, items;
#pragma omp parallel for
        for (int i = 0; i < n; i++) {
            hep.elements.push_back(temp.elements[i]);
        }
#pragma omp parallel for
        for (int i = n; i < temp.elements.size(); i++) {
            items.elements.push_back(temp.elements[i]);
        }
    return {hep, items};
}
void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

int partition (Node &ar, int low, int high)
{
    vector<int>&arr=ar.elements;
	int pivot = arr[high]; // pivot
	int i = (low - 1); // Index of smaller element
	for (int j = low; j <= high- 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot)
		{
			i++; // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void quickSort(Node &arr, int low, int high)
{
	if (low < high)
	{
		int pi = partition(arr, low, high);
		#pragma omp task firstprivate(arr,low,pi)
		{
			quickSort(arr,low, pi - 1);

		}
		{
			quickSort(arr, pi + 1, high);

		}


	}
}
// Parallel code for sorting routine
void Sort(Node &a1, int n1) {
    #pragma omp parallel
 {

 int id = omp_get_thread_num();
    int nthrds = omp_get_num_threads();
//printf("Thread is %d\n",id);
#pragma omp single nowait
	 quickSort(a1,0,n1-1);
}
    
}















/* Function to print an array */
void printArray(int arr[], int size)
{
	int i;
	for (i=0; i < size; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

// Driver program to test above functions