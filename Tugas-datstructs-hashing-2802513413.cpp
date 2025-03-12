#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define TABLE_SIZE 10

struct HashTable
{
    char key[100]; // untuk Key/Value
} ht[TABLE_SIZE];

// fungsi inisialisasi Hash Tabel
void initializationHT()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        strcpy(ht[i].key, ""); // set data kosong
    }
}

int StoI(char key[])
{
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        sum += key[i]; // sum of ASCII values of character
    }
    return sum;
}

// Hash Function (Division)
int DivHash(char key[])
{
    int intKey = StoI(key);
    return intKey % TABLE_SIZE;
}

int firstCharHash(char key[])
{
    // cek apakah ada data
    if (key[0] == '\0')
    {
        return -1;
    }
    char firstChar = key[0];

    // cek apakah karakter pertama adalah huruf
    if (!isalpha(firstChar))
    {
        return -1;
    }
    return tolower(firstChar) - 'a';
}

// Metode Hashing tambahan

// Mid-Square Hashing
int MidSquareHash(char key[])
{
    int intKey = StoI(key);
    int square = intKey * intKey;
    return (square / 10) % TABLE_SIZE; // ambil digit tengah dari hasil kuadrat
}

// Folding Hashing
int FoldingHash(char key[])
{
    int intKey = StoI(key);
    int sum = 0;
    while (intKey > 0)
    {
        sum += intKey % 10; // ambil digit terakhir
        intKey /= 10;
    }
    return sum % TABLE_SIZE;
}

// Digit Extraction Hashing
int DigitExtractionHash(char key[])
{
    int intKey = StoI(key);
    int result = 0;
    while (intKey > 0)
    {
        result += intKey % 10; // ambil digit terakhir
        intKey /= 10;
    }
    return result % TABLE_SIZE;
}

// Rotating Hashing
int RotatingHash(char key[])
{
    int intKey = StoI(key);
    int result = 0;
    while (intKey > 0)
    {
        result = (result << 5) | (result >> (sizeof(int) * 8 - 5)); // rotasi bit
        result += intKey % 10;
        intKey /= 10;
    }
    return result % TABLE_SIZE;
}

// Fungsi Penanganan Collision

// Linear Probing
int linearProbing(int index)
{
    int i = 1;
    while (strcmp(ht[(index + i) % TABLE_SIZE].key, "") != 0 && i < TABLE_SIZE)
    {
        i++;
    }
    return (index + i) % TABLE_SIZE;
}

// Rehashing
void rehash()
{
    struct HashTable oldHT[TABLE_SIZE];
    memcpy(oldHT, ht, sizeof(ht)); // salin data lama ke oldHT
    initializationHT();             // reset tabel hash

    // Tentukan ukuran baru untuk hash table (misalnya dua kali lipat)
    int newSize = TABLE_SIZE * 2;
    struct HashTable *newHT = (struct HashTable *)malloc(sizeof(struct HashTable) * newSize);

    for (int i = 0; i < newSize; i++)
    {
        strcpy(newHT[i].key, "");
    }

    // Rehashing: pindahkan semua elemen dari oldHT ke newHT
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (strcmp(oldHT[i].key, "") != 0)
        {
            int index = DivHash(oldHT[i].key);
            newHT[index] = oldHT[i];
        }
    }

    // Pindahkan ke ht yang baru
    memcpy(ht, newHT, sizeof(struct HashTable) * newSize);
    free(newHT);
}

// Chaining (membuat linked list untuk setiap slot hash)
struct Node
{
    char key[100];
    struct Node *next;
};

struct Node *htChain[TABLE_SIZE];

void insertChaining(char key[], int (*hashFunction)(char[]))
{
    int index = hashFunction(key);
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    strcpy(newNode->key, key);
    newNode->next = htChain[index];
    htChain[index] = newNode;
    printf("Key %s dimasukkan ke index %d dengan metode Chaining.\n", key, index);
}

void displayChainingHT()
{
    printf("\nHash Table dengan Chaining:\n");
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (htChain[i] != NULL)
        {
            struct Node *temp = htChain[i];
            printf("Index %d: ", i);
            while (temp != NULL)
            {
                printf("%s -> ", temp->key);
                temp = temp->next;
            }
            printf("NULL\n");
        }
        else
        {
            printf("Index %d: Kosong\n", i);
        }
    }
}

// Insert data ke Hash Table
void insert(char key[], int (*hashFunction)(char[]), int (*collisionHandler)(int))
{
    int index = hashFunction(key);

    // Cek jika slot masih kosong, masukkan key
    if (strcmp(ht[index].key, "") == 0)
    {
        strcpy(ht[index].key, key);
        printf("Key %s dimasukkan ke index %d.\n", key, index);
    }
    // Jika terjadi collision, gunakan collision handler
    else
    {
        printf("Collision terjadi! Menggunakan metode penanganan collision...\n");
        if (collisionHandler)
        {
            index = collisionHandler(index); // mengatur ulang index jika ada collision
            strcpy(ht[index].key, key);
            printf("Key %s dimasukkan ke index %d setelah collision handling.\n", key, index);
        }
    }
}

// Fungsi untuk menampilkan Hash Table
void displayHT()
{
    printf("\nHash Table\n");
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (strcmp(ht[i].key, "") != 0)
        {
            printf("Index %d: %s\n", i, ht[i].key);
        }
        else
        {
            printf("Index %d: Kosong\n", i);
        }
    }
}

// Menu utama
void menu()
{
    int choice, collisionChoice, hashChoice;

    printf("Pilih metode Hashing:\n");
    printf("1. Division Hashing\n");
    printf("2. First Character Hashing\n");
    printf("3. Mid-Square Hashing\n");
    printf("4. Folding Hashing\n");
    printf("5. Digit Extraction Hashing\n");
    printf("6. Rotating Hashing\n");
    scanf("%d", &hashChoice);

    printf("Pilih metode Penanganan Collision:\n");
    printf("1. Linear Probing\n");
    printf("2. Rehashing\n");
    printf("3. Chaining\n");
    scanf("%d", &collisionChoice);

    // Menentukan fungsi hash yang akan dipakai
    int (*hashFunction)(char[]) = NULL;
    switch (hashChoice)
    {
    case 1:
        hashFunction = DivHash;
        break;
    case 2:
        hashFunction = firstCharHash;
        break;
    case 3:
        hashFunction = MidSquareHash;
        break;
    case 4:
        hashFunction = FoldingHash;
        break;
    case 5:
        hashFunction = DigitExtractionHash;
        break;
    case 6:
        hashFunction = RotatingHash;
        break;
    default:
        hashFunction = DivHash;
        break;
    }

    // Menentukan collision handler yang akan dipakai
    int (*collisionHandler)(int) = NULL;
    switch (collisionChoice)
    {
    case 1:
        collisionHandler = linearProbing;
        break;
    case 2:
        rehash();
        return;
        break;
    case 3:
        collisionHandler = NULL; // Chaining tidak membutuhkan handler karena memanfaatkan linked list
        break;
    default:
        collisionHandler = linearProbing;
        break;
    }

    // Input key
    char key[100];
    while (1)
    {
        printf("Masukkan key (string) atau exit untuk berhenti: ");
        scanf("%s", key);
        if (strcmp(key, "exit") == 0)
            break;
        if (collisionChoice == 3)
            insertChaining(key, hashFunction);
        else
            insert(key, hashFunction, collisionHandler);
    }

    // Tampilkan hash table
    if (collisionChoice == 3)
        displayChainingHT();
    else
        displayHT();
}

int main()
{
    initializationHT();
    menu(); // Menampilkan menu
    return 0;
}

