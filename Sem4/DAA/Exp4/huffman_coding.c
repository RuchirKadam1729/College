#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct minHeapNode {
    char symb;
    int freq;
    struct minHeapNode* left, * right;
};

struct minHeap {
    struct minHeapNode* nodes[128];
    int count;
};

int compare_by_freq(const void* a, const void* b)
{
    struct minHeapNode* minHeapNode1 = *(struct minHeapNode**)a;
    struct minHeapNode* minHeapNode2 = *(struct minHeapNode**)b;

    return minHeapNode1->freq - minHeapNode2->freq;
}

#define SORTHEAP(minHeap) qsort(minHeap->nodes, minHeap->count, sizeof(struct minHeapNode*), compare_by_freq)

void insertNode(struct minHeap* minHeap, struct minHeapNode* minHeapNode)
{
    minHeap->count += 1;
    minHeap->nodes[minHeap->count - 1] = minHeapNode;
    SORTHEAP(minHeap);
}

struct minHeapNode* ExtractMin(struct minHeap* minHeap) {
    struct minHeapNode* min = minHeap->nodes[0];
    // Remove the node by shifting elements
    for (int i = 0; i < minHeap->count - 1; i++) {
        minHeap->nodes[i] = minHeap->nodes[i + 1];
    }
    minHeap->count--;
    return min;
}


void procedure(struct minHeap* minHeap)
{
    struct minHeapNode
        * minHeapNode1 = ExtractMin(minHeap),
        * minHeapNode2 = ExtractMin(minHeap);

    struct minHeapNode* internalNode = malloc(sizeof(struct minHeapNode));
    internalNode->symb = '\0';
    internalNode->freq = minHeapNode1->freq + minHeapNode2->freq;
    internalNode->left = minHeapNode1;
    internalNode->right = minHeapNode2;
    insertNode(minHeap, internalNode);
}

void print_tree(struct minHeapNode minHeapNode, char code[])
{
    if (minHeapNode.symb != '\0') {
        printf("%c: %s\n", minHeapNode.symb, code);
        return;
    }
    else {
        char code_left[100], code_right[100];
        memcpy(code_left, code, strlen(code) + 1);
        memcpy(code_right, code, strlen(code) + 1);
        print_tree(*minHeapNode.left, strcat(code_left, "0"));
        print_tree(*minHeapNode.right, strcat(code_right, "1"));
    }
}

int main(int argc, char const* argv[])
{
#define LEN(A) sizeof(A)/sizeof(A[0])
    char symb[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int freq[] = { 5, 9, 12, 13, 16, 45 };

    struct minHeap minHeap;
    minHeap.count = 0;
    struct minHeapNode minHeapNode[LEN(symb)];

    for (size_t i = 0; i < LEN(symb); i++) {
        minHeapNode[i] = (struct minHeapNode){ .symb = symb[i], .freq = freq[i], .left = NULL, .right = NULL };
        insertNode(&minHeap, &(minHeapNode[i]));
    }

    while (minHeap.count > 1) {
        procedure(&minHeap);
    }
    char code[100];
    memset(code, '\0', 100);
    print_tree(*(minHeap.nodes[0]), code);
    return 0;
}
