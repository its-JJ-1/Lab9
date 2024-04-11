#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 23  // Define the size of the hash table

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

// HashNode
struct HashNode 
{
    struct RecordType data;
    struct HashNode* next;
};

// HashType
struct HashType 
{
    struct HashNode* table[HASH_SIZE];
};

// Compute the hash function
int hash(int x) 
{
    return x % HASH_SIZE;
}

// Insert a record into the hash table
void insertRecord(struct HashType* hashTable, struct RecordType record) 
{
    int index = hash(record.id);
    struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
    newNode->data = record;
    newNode->next = NULL;

    if (hashTable->table[index] == NULL) 
    {
        hashTable->table[index] = newNode;
    } 
    else 
    {
        // Collision handling: insert at the end of the linked list
        struct HashNode* currentNode = hashTable->table[index];
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
}



// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData) 
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) 
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL) 
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) 
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }
        fclose(inFile);
    }
    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz) 
{
    printf("\nRecords:\n");
    for (int i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable) 
{
    printf("\nRecords in the Hash Table:\n");
    for (int i = 0; i < HASH_SIZE; ++i) {
        printf("Index %d -> ", i);
        struct HashNode* currentNode = hashTable->table[i];
        while (currentNode != NULL) {
            printf("%d %c %d -> ", currentNode->data.id, currentNode->data.name, currentNode->data.order);
            currentNode = currentNode->next;
        }
        printf("NULL\n");
    }
}


int main(void) 
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create a hash table
    struct HashType hashTable;
    for (int i = 0; i < HASH_SIZE; ++i) {
        hashTable.table[i] = NULL;
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i) {
        insertRecord(&hashTable, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(&hashTable);

    // Free allocated memory
    for (int i = 0; i < recordSz; ++i) {
        free(pRecords + i);
    }
    free(pRecords);

    return 0;
}
