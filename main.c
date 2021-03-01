//Zahide Gür Taştan 150119827 
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1000

struct node
{
    int freq;
    struct node *sibling, *child, *parent;
    int degree;
    char docName[30];
};

typedef struct node docNode;

docNode *documentQueue=NULL;
int cnt = 0;

void readDirectory(char* name);
int countWord(char* file_path, char* word);
void writeMaximum(struct node* pNode);
void enqueue(docNode *pNode1);
docNode *unionHeaps(docNode *pNode, docNode *pNode1);
docNode *mergeHeaps(docNode *pNode, docNode *pNode1);
void connect(docNode *pNode, docNode *pNode1);
docNode *dequeueMax();
void display(docNode *documentQueue);

docNode *reversing(struct node *pNode);

docNode *findDegree(docNode *pNode, docNode *pNode1);
void writeDocParagraph(docNode *np);

int main()
{
    printf("Enter your keyword to search: ");
    char name[30];
    scanf("%s",name);
    readDirectory(name);
    //writeMaximum(documentQueue);
    //display(documentQueue);
    printf("\nThere is %d relevant docs.\n", cnt);
    docNode *np;
    int i;
    int j;
    int k;
    int num;
    docNode *ptr;
    ptr = (docNode*) malloc(5 * sizeof(docNode));


    if(cnt >= 5){
        for(i=0; i<5; i++){
            np = dequeueMax(documentQueue);
            printf("\nDoc %s: (%d) \n", np->docName,np->freq);
            writeDocParagraph(np);
            strcpy((ptr+i)->docName,np->docName);
            (ptr+i)->freq = np->freq;
        }
        printf("\nRelevant order: ");
        for(i = 0; i < 5; ++i)
            printf("%s (%d), ", (ptr+i)->docName, (ptr+i)->freq);
    }


    //writeRelevantParag();

    else if(cnt < 5) {
        //printf("%d",cnt);
        for (j = 0; j < cnt; j++) {
            np = dequeueMax(documentQueue);
            printf("\nDoc %s: (%d) \n", np->docName,np->freq);
            writeDocParagraph(np);
            strcpy((ptr+j)->docName,np->docName);
            (ptr+j)->freq = np->freq;
        }
        printf("Relevant order: ");
        for(j = 0; j < cnt; ++j)
            printf("%s (%d), ", (ptr+j)->docName, (ptr+j)->freq);
    }

    return 0;
}

void readDirectory(char* name){
    DIR *dirp;
    struct dirent *direntp;
    docNode *node;
    int wordcount = 0;

    dirp = opendir("../files");
    if( dirp == NULL ) {
        perror( "" );
    } else {
        for(;;) {
            direntp = readdir( dirp );
            if( direntp == NULL ) break;
            if (!strcmp (direntp->d_name, "."))
                continue;
            if (!strcmp (direntp->d_name, ".."))
                continue;

            // Create node
            char docName[30];
            strcpy(docName,direntp->d_name);
            node = (docNode *)malloc(sizeof(docNode));
            strcpy(node->docName, docName);
            wordcount = countWord(direntp->d_name, name);
            node->freq = wordcount;
            printf("%s\n",node->docName);
            printf("%d\n",node->freq);
            //**

            if(node->freq > 0 ) {
                enqueue(node);
                cnt++;
            }
        }
        closedir( dirp );
    }
}

void enqueue(docNode *node) {
    //create a node to inserting the queue.
    docNode *newNodetoInsert;
    newNodetoInsert = NULL;
    node->child=NULL;
    node->sibling=NULL;
    node->degree=0;

    //documentQueue is a final Heap after enqeueue or dequeue implementation.
    newNodetoInsert = node;
    documentQueue = unionHeaps( documentQueue , newNodetoInsert );

}

docNode* unionHeaps(docNode *heapOne, docNode *heapTwo) {
    docNode *head = mergeHeaps(heapOne, heapTwo);

    if(head == NULL){
        return head;
    }

    docNode *prev = NULL;
    docNode *x = head;
    docNode *next = x->sibling;

    while (next != NULL)
    {
        if ((x->degree != next->degree) ||
            ((next->sibling != NULL) &&
             (next->sibling)->degree ==
             x->degree))
        {
            prev = x;
            x = next;
        }

        else
        {
            if (x->freq > next->freq)
            {
                x->sibling = next->sibling;
                connect(next, x);
            }
            else
            {
                if (prev == NULL)
                    head = next;
                else {
                    prev->sibling = next;
                }
                connect(x, next);
                x = next;
            }
        }
        next = x->sibling;
    }
    return head;
}

void connect(docNode *pNode, docNode *pNode1) {
    pNode->parent = pNode1;
    pNode->sibling = pNode1->child;
    pNode1->child=pNode;
    pNode1->degree=pNode1->degree+1;
}

docNode *mergeHeaps(docNode *heapOne, docNode *heapTwo) {

    if (heapOne == NULL && heapTwo == NULL)
        return NULL;

    if(heapTwo == NULL){  //if the one heap is NULL then the merging result is other.
        return heapOne;
    }
    if(heapOne == NULL){
        return heapTwo;
    }
    docNode *temp = NULL;
    temp = findDegree(heapOne, heapTwo);

    while (heapOne != NULL && heapTwo != NULL)
    {
        if (heapOne->degree < heapTwo->degree)
            heapOne = heapOne->sibling;

        else if(heapOne->degree > heapTwo->degree){
            docNode *sib = heapTwo->sibling;
            heapTwo->sibling = heapOne;
            heapTwo = sib;
        }
        else
        {
            docNode *sib = heapOne->sibling;
            heapOne->sibling = heapTwo;
            heapOne = sib;
        }
    }
    return temp;
}

docNode *findDegree(docNode *pNode, docNode *pNode1) {
    docNode *temp = NULL;
    if (pNode->degree <= pNode1->degree)
        temp = pNode;
    else if(pNode->degree > pNode1->degree)
        temp = pNode1;

    return temp;

}

int countWord(char* file_path, char* word){
    FILE *fp;
    int count = 0;
    char dest[40] = "../files/";
    strcat(dest,file_path);

    if(NULL==(fp=fopen(dest, "r")))
        return -1;

    char buf[50];

    while( fscanf(fp, "%s", buf) != EOF )
    {
        if(strcmp((const char *) &buf, word) == 0){
            count++;
        }
    }
    fclose(fp);
    return count;
}

void writeMaximum(struct node* pNode)
{
    docNode *iter;
    if(pNode==NULL)
    {
        printf("\nheap is empty\n");
        return;
    }
    printf("\nBinomial heaps root :\n");
    iter=pNode;
    while(iter!=NULL)
    {
        printf("%d",iter->freq);
        if(iter->sibling!=NULL) {
            printf("-->");
        }
        iter = iter->sibling;
    }
    printf("\n");
}
docNode *dequeueMax(){
    if(documentQueue == NULL){
        printf("Queue is already null");
        return documentQueue;
    }
    docNode *prev = NULL;
    docNode *maxNode = documentQueue;


    int max = documentQueue->freq;

    docNode *iter = documentQueue;
    docNode *reversingResult = NULL;

    //finding the maximum element of the queue.
    while(iter->sibling != NULL){
        if(iter->sibling->freq >= max){
            max = iter->sibling->freq;
            prev = iter;
            maxNode = iter->sibling;
        }
        iter = iter->sibling;
    }
    //if there is a only one element.
    if (prev == NULL && maxNode->sibling == NULL) {
        documentQueue = NULL;
    }
    //if it is the started element of heap.
    else if(prev == NULL) {
        documentQueue = maxNode->sibling;
    }
    else{
        prev->sibling = maxNode->sibling;
    }

    reversingResult = reversing(maxNode->child);
    documentQueue = unionHeaps(reversingResult, documentQueue);

    return maxNode;
}

docNode *reversing(docNode *child) {

    docNode *prev = NULL, *next = NULL;

    while (child != NULL) {
        next = child->sibling;

        child->sibling = prev;

        prev = child;
        child = next;
    }
    child = prev;

    return child;
}
void display(docNode *rootHeap)
{
    while (rootHeap)
    {
        printf(" %d ", rootHeap->freq);
        display(rootHeap->child);
        rootHeap = rootHeap->sibling;
    }
}

void writeDocParagraph(docNode *np){
    FILE *fp;
    char s;
    char dest[50] = "../files/";
    strcat(dest,np->docName);

    fp=fopen(dest,"r");
    while((s=fgetc(fp))!=EOF) {
        printf("%c",s);
    }
    fclose(fp);
}