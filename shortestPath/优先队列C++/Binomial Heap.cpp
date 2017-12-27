#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<climits>
using namespace std;
 
typedef struct BinHeapNode BinNode;
typedef struct BinHeapNode * BinHeap;
typedef struct BinHeapNode * Position;
 
//���ADT
struct BinHeapNode {
    int key;
    int degree;
    Position parent;
    Position leftChild;
    Position sibling;
};
 
//�������ڵ�ֵ����
BinHeap MakeBinHeapWithArray(int keys[], int n);
 
//�����Ѻϲ�
BinHeap BinHeapUnion(BinHeap &H1, BinHeap &H2);
 
//��H1, H2�ĸ���ϲ���һ���������ĵ��������������е�����
BinHeap BinHeapMerge(BinHeap &H1, BinHeap &H2);
 
//ʹH2��ΪH1�ĸ��ڵ�
void BinLink(BinHeap &H1, BinHeap &H2);
 
//������С���ڵ��ָ��
BinHeap BinHeapMin(BinHeap heap);
 
//���ٹؼ��ֵ�ֵ
void BinHeapDecreaseKey(BinHeap heap, BinHeap x, int key);
 
//ɾ��һ���ؼ���
BinHeap BinHeapDelete(BinHeap &heap, int key);
 
//�ҳ�һ���ؼ���
BinHeap BinHeapFind(BinHeap &heap, int key);
 
//��ӡ����ѽṹ
void PrintBinHeap(BinHeap heap);
 
//���ٶ�
void DestroyBinHeap(BinHeap &heap);
 
//�������ڵ�ֵ����
BinHeap MakeBinHeapWithArray(int keys[], int n) {
    BinHeap heap = NULL, newHeap = NULL;
    for (int i = 0; i < n; i++) {
        newHeap = (BinHeap) malloc(sizeof(BinNode));
        if (newHeap == NULL) {
            puts("Out of the Space");
            exit(1);
        }
        memset(newHeap, 0, sizeof(BinNode));
        newHeap->key = keys[i];
        if (NULL == heap) {
            heap = newHeap;
        } else {
            heap = BinHeapUnion(heap, newHeap);
            newHeap = NULL;
        }
    }
    return heap;
}
 
//�����Ѻϲ�
BinHeap BinHeapUnion(BinHeap &H1, BinHeap &H2) {
    Position heap = NULL, pre_x = NULL, x = NULL, next_x = NULL;
    heap = BinHeapMerge(H1, H2);
    if (heap == NULL) {
        return heap;
    }
 
    pre_x = NULL;
    x = heap;
    next_x = x->sibling;
 
    while (next_x != NULL) {
        if ((x->degree != next_x->degree) ||//Cases 1 and 2
            ((next_x->sibling != NULL) && (next_x->degree == next_x->sibling->degree))) {
                pre_x = x;
                x = next_x;
        } else if (x->key <= next_x->key) {//Cases 3
            x->sibling = next_x->sibling;
            BinLink(next_x, x);
        } else {//Cases 4
            if (pre_x == NULL) {
                heap = next_x;
            } else {
                pre_x->sibling = next_x;
            }
            BinLink(x, next_x);
            x = next_x;
        }
        next_x = x->sibling;
    }
    return heap;
}
 
//��H1, H2�ĸ���ϲ���һ���������ĵ��������������е�����
BinHeap BinHeapMerge(BinHeap &H1, BinHeap &H2) {
    //heap->�ѵ��׵�ַ��H3Ϊָ���¶Ѹ����
    BinHeap heap = NULL, firstHeap = NULL, secondHeap = NULL,
        pre_H3 = NULL, H3 = NULL;
 
    if (H1 != NULL && H2 != NULL){
        firstHeap = H1;
        secondHeap = H2;
        //����while��firstHeap, secondHeap, pre_H3, H3��������˳��
        while (firstHeap != NULL && secondHeap != NULL) {
            if (firstHeap->degree <= secondHeap->degree) {
                H3 = firstHeap;
                firstHeap = firstHeap->sibling;
            } else {
                H3 = secondHeap;
                secondHeap = secondHeap->sibling;
            }
 
            if (pre_H3 == NULL) {
                pre_H3 = H3;
                heap = H3;
            } else {
                pre_H3->sibling = H3;
                pre_H3 = H3;
            }
            if (firstHeap != NULL) {
                H3->sibling = firstHeap;
            } else {
                H3->sibling = secondHeap;
            }
        }//while
    } else if (H1 != NULL) {
        heap = H1;
    } else {
        heap = H2;
    }
    H1 = H2 = NULL;
    return heap;
}
 
//ʹH2��ΪH1�ĸ��ڵ�
void BinLink(BinHeap &H1, BinHeap &H2) {
    H1->parent = H2;
    H1->sibling = H2->leftChild;
    H2->leftChild = H1;
    H2->degree++;
}
 
//������С���ڵ��ָ��
BinHeap BinHeapMin(BinHeap heap) {
    Position y = NULL, x = heap;
    int min = INT_MAX;
 
    while (x != NULL) {
        if (x->key < min) {
            min = x->key;
            y = x;
        }
        x = x->sibling;
    }
    return y;
}
 
//��ȡ����С�ؼ��ֵĽ��
BinHeap BinHeapExtractMin(BinHeap &heap) {
    BinHeap pre_y = NULL, y = NULL, x = heap;
    int min = INT_MAX;
    while (x != NULL) {
        if (x->key < min) {
            min = x->key;
            pre_y = y;
            y = x;
        }
        x = x->sibling;
    }
 
    if (y == NULL) {
        return NULL;
    }
 
    if (pre_y == NULL) {
        heap = heap->sibling;
    } else {
        pre_y->sibling = y->sibling;
    }
 
    //��y���ӽ��ָ��reverse
    BinHeap H2 = NULL, p = NULL;
    x = y->leftChild;
    while (x != NULL) {
        p = x;
        x = x->sibling;
        p->sibling = H2;
        H2 = p;
        p->parent = NULL;
    }
 
    heap = BinHeapUnion(heap, H2);
    return y;
}
 
//���ٹؼ��ֵ�ֵ
void BinHeapDecreaseKey(BinHeap heap, BinHeap x, int key) {
    if(key > x->key) {
        puts("new key is greaer than current key");
        exit(1); //��Ϊ����
    }
    x->key = key;
 
    BinHeap z = NULL, y = NULL;
    y = x;
    z = x->parent;
    while(z != NULL && z->key > y->key) {
        swap(z->key, y->key);
        y = z;
        z = y->parent;
    }
}
 
//ɾ��һ���ؼ���
BinHeap BinHeapDelete(BinHeap &heap, int key) {
    BinHeap x = NULL;
    x = BinHeapFind(heap, key);
    if (x != NULL) {
        BinHeapDecreaseKey(heap, x, INT_MIN);
        return BinHeapExtractMin(heap);
    }
    return x;
}
 
//�ҳ�һ���ؼ���
BinHeap BinHeapFind(BinHeap &heap, int key) {
    Position p = NULL, x = NULL;
    p = heap;
    while (p != NULL) {
        if (p->key == key) {
            return p;
        } else {
            if((x =BinHeapFind(p->leftChild, key)) != NULL) {
                return x;
            }
            p = p->sibling;
        }
    }
    return NULL;
}
 
//��ӡ����ѽṹ
void PrintBinHeap(BinHeap heap) {
    if (NULL == heap) {
        return ;
    }
    Position p = heap;
 
    while (p != NULL) {
        printf(" (");
        printf("%d", p->key);
        //��ʾ�亢��
        if(NULL != p->leftChild) {
            PrintBinHeap(p->leftChild);
        }
        printf(") ");
 
        p = p->sibling;
    }
}       
 
int kp1[8] = {12,
               7, 25,
              15, 28, 33, 41};
 
int kp2[20] = {18,
                3, 37,
                6, 8, 29, 10, 44, 30, 23, 2, 48, 31, 17, 45, 32, 24, 50, 55};
 
int kp4[23] = {37, 41,
               10, 28, 13, 77,
               1, 6, 16, 12, 25, 8, 14, 29, 26, 23, 18, 11, 17, 38, 42, 27};
int main() {
    BinHeap H1 = NULL;
    H1 = MakeBinHeapWithArray(kp1, 7);
    puts("��һ�������H1:");
    PrintBinHeap(H1);
 
    BinHeap H2 = NULL;
    H2 = MakeBinHeapWithArray(kp2, 19);
    puts("\n\n�ڶ��������H2:");
    PrintBinHeap(H2);
 
    BinHeap H3 = NULL;
    H3 = BinHeapUnion(H1, H2);
    puts("\n\n�ϲ�H1,H2�󣬵õ�H3:");
    PrintBinHeap(H3);
 
    BinHeap H4 = NULL;
    H4 = MakeBinHeapWithArray(kp4, 22);
    puts("\n\n���ڲ�����ȡ��ɾ���Ķ����H4:");
    PrintBinHeap(H4);
 
    BinHeap extractNode = BinHeapExtractMin(H4);
    if (extractNode != NULL) {
        printf("\n\n��ȡ��С��ֵ%d��\n", extractNode->key);
        PrintBinHeap(H4);
    }
 
    extractNode = BinHeapExtractMin(H4);
    if (extractNode != NULL) {
        printf("\n\n��ȡ��С��ֵ%d��\n", extractNode->key);
        PrintBinHeap(H4);
    }
 
    extractNode = BinHeapExtractMin(H4);
    if (extractNode != NULL) {
        printf("\n\n��ȡ��С��ֵ%d��\n", extractNode->key);
        PrintBinHeap(H4);
    }
 
    BinHeapDelete(H4, 12);
    puts("\n\nɾ��12��");
    PrintBinHeap(H4);
    return 0;
}