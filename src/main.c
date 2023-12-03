#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    In this document, there is the solution for the homework 3.
    This documents implements a CLI for a hash table and implements
    hashing related functions such as add, delete, and search. User
    can add, delete, edit, and search from the hash table by
    answering prompts from the CLI.
*/

#define N_MODE 2
#define MAX_UNAME_LEN 32
#define MAX_UNAME_BUF_LEN 256

typedef enum { Normal, Debug, Error } Mode;

typedef struct {
    char *userName;
    int deleted;
} HashEntry;

/*
 @brief Prints current configured mode

 @param mode Mode enum variable to be used for printing

 @return
*/
void printMode(Mode mode) {
    char *asStr = "error";
    if (mode == Normal) {
        asStr = "normal";
    } else if (mode == Debug) {
        asStr = "debug";
    }
    printf("printMode: Mode: %s\n", asStr);
}

/*
 @brief Converts a string to Mode enum

 @param str Input string that will be converted to Mode

 @return a Mode enum corresponding to given string
*/
Mode strToMode(char *str) {
    Mode mode;
    if (strcmp(str, "NORMAL") == 0) {
        mode = Normal;
    } else if (strcmp(str, "DEBUG") == 0) {
        mode = Debug;
    } else {
        mode = Error;
    }
    return mode;
}

/*
 @brief Given a string, converts it to a Mode enum
    and checks if it is a valid mode

 @param modeArg String containing description of a mode

 @return a Mode enum corresponding to given string
*/
Mode parseMode(char *modeArg) {
    char *modes[N_MODE] = {"NORMAL", "DEBUG"};
    int i;
    int isValid = 0;
    Mode mode;
    for (i = 0; i < N_MODE && isValid == 0; ++i) {
        if (strcmp(modes[i], modeArg) != 0) {
            isValid = 1;
            mode = strToMode(modeArg);
        }
    }
    if (isValid == 0) {
        printf("Parametre NORMAL ve ya DEBUG değil\n");
        exit(EXIT_FAILURE);
    }
    return mode;
}

/*
 @brief Parses program arguments and returns a running mode

 @param argc Number of arguments
 @param argv String array containing program arguments

 @return Program running mode
*/
Mode parseArgs(int argc, char **argv) {
    char *modeArg;
    if (argc != 2) {
        printf("Program için 1 parametre gerekli: NORMAL ve ya DEBUG\n");
        exit(EXIT_FAILURE);
    }
    modeArg = argv[1];
    return parseMode(modeArg);
}

/*
 @brief Reads n parameter from stdin

 @return N value taken from command line
*/
int readN() {
    int n = 0;
    printf("N değerini giriniz: ");
    scanf(" %d", &n);
    return n;
}

/*
 @brief Reads load factor parameter from stdin

 @return Load factor
*/
double readLf() {
    double lf;
    printf("Load factor değerini giriniz: ");
    scanf(" %lf", &lf);
    return lf;
}

/*
 @brief Checks if a given number is prime

 @param num Number to be checked

 @return 1 if number is prime, 0 if it is not
*/
int checkPrime(int num) {
    int i;
    int isPrime = 1;
    if (num <= 3) {
        return 1;
    }
    for (i = 2; i * i <= num && isPrime == 1; ++i) {
        if (num % i == 0) {
            isPrime = 0;
        }
    }
    return isPrime;
}

/*
 @brief Computes m value given n and load
    based on homework specs

 @param n Number of elements
 @param lf Load factor

 @return m value to be used in hashing
*/
int compM(int n, double lf) {
    int quotient = (int)ceil(n / lf);
    int m;
    int isPrime = 0;
    m = quotient - 1;
    while (isPrime == 0) {
        ++m;
        isPrime = checkPrime(m);
    }
    return m;
}

/*
 @brief Tests checkPrime function

 @return
*/
void testCheckPrime() {
    assert(checkPrime(2) == 1);
    assert(checkPrime(3) == 1);
    assert(checkPrime(4) == 0);
    assert(checkPrime(5) == 1);
    assert(checkPrime(6) == 0);
    assert(checkPrime(7) == 1);
    assert(checkPrime(25) == 0);
    assert(checkPrime(26) == 0);
    assert(checkPrime(97) == 1);
    assert(checkPrime(189) == 0);
}

/*
 @brief Converts a string to number key based on homework specs
    using Horner's rule

 @param str String to be converted
 @param unameLen Number of chars in the string

 @return Converted number key for the string
*/
int strToNum(char *str, int unameLen) {
    double num = 0;
    int i;
    int r = 3;
    for (i = 0; i < unameLen; ++i) {
        int power = unameLen - i - 1;
        double powerRes = pow(r, power);
        int charVal;
        charVal = str[i] - '0' + 1;
        num = num + powerRes * charVal;
    }
    while (num > INT_MAX) {
        num -= INT_MAX;
    }
    return (int)num;
}

/*
 @brief Computes output h1 function given in homework specs

 @param key Key to be used in hashing
 @param m Number of cells in hash table

 @return Output of h1 function
*/
int h1(int key, int m) { return key % m; }

/*
 @brief Computes output h2 function given in homework specs

 @param key Key to be used in hashing
 @param m Number of cells in hash table

 @return Output of h2 function
*/
int h2(int key, int m) {
    int m2 = m - 2;
    return 1 + (key % m2);
}

/*
 @brief Computes index in the hash table based on h1 value,
    h2 value, index i, and m value

 @param h1Val Output of h1 function
 @param h2Val Output of h2 function
 @param i Current iteration counter that is used in hashing functions
 @param m Number of cells in hash table

 @return Computed index in the hash table
*/
int compHashIdx(int h1Val, int h2Val, int i, int m) {
    return (h1Val + i * h2Val) % m;
}

/*
 @brief Prints the given hash table

 @param hash Hash table
 @param m Number of cells in hash table

 @return
*/
void printHash(HashEntry *hash, int m) {
    int i;
    printf("\nHash tablosu:\n");
    for (i = 0; i < m; ++i) {
        if (hash[i].userName != 0) {
            printf("\t%d:\t%s (deleted: %d)\n", i, hash[i].userName,
                   hash[i].deleted);
        } else {
            printf("\t%d:\t(null)\n", i);
        }
    }
}

/*
 @brief Frees the hash table and its substructures

 @param hash Hash table
 @param m Number of cells in hash table

 @return
*/
void freeHash(HashEntry *hash, int m) {
    int i;
    for (i = 0; i < m; ++i) {
        free(hash[i].userName);
    }
    free(hash);
}

/*
 @brief Adds a username to the hash table based on homework specs

 @param hash Hash table
 @param n Number of original elements
 @param m Number of cells in hash table
 @param lf Load factor
 @param uname Username to be added
 @param unameLen Length of username to be added
 @param shouldPrintHash Flag representing whether hash table should
    be printed after adding
 @param mode Program running mode

 @return Index of the newly added element, -1 if could not add
*/
int add(HashEntry *hash, int n, int m, double lf, char *uname, int unameLen,
        int shouldPrintHash, Mode mode) {
    int key;
    int i = 0;
    int inserted = 0;
    int hashIdx;
    int h1Val;
    int h2Val;
    int exists = 0;
    key = strToNum(uname, unameLen);
    h1Val = h1(key, m);
    h2Val = h2(key, m);
    if (mode == Debug) {
        printf("h1(\"%s\") = %d\n", uname, h1Val);
        printf("h2(\"%s\") = %d\n", uname, h2Val);
    }
    while (inserted == 0 && exists == 0 && i < m) {
        hashIdx = compHashIdx(h1Val, h2Val, i, m);
        if (hash[hashIdx].userName == 0) {
            hash[hashIdx].userName = malloc((unameLen + 1) * sizeof(char));
            if (hash[hashIdx].userName == NULL) {
                printf("add: malloc failed\n");
                exit(EXIT_FAILURE);
            }
            strcpy(hash[hashIdx].userName, uname);
            inserted = 1;
        } else if (strcmp(hash[hashIdx].userName, uname) == 0) {
            exists = 1;
        }
        if (hash[hashIdx].deleted == 1 &&
            strcmp(hash[hashIdx].userName, uname) == 0) {
            hash[hashIdx].deleted = 0;
            inserted = 1;
        }
        if (inserted == 0 && exists == 0 && mode == Debug) {
            printf("%s kelimesi %d. adrese yerleştirilemedi\n", uname, hashIdx);
        }
        ++i;
    }
    if (inserted == 1) {
        printf("Elemanınız (%s) %d. adrese yerleştirildi.\n", uname, hashIdx);
        if (shouldPrintHash) {
            printHash(hash, m);
        }
    } else if (exists == 1) {
        printf(
            "Eleman (%s) tabloda mevcut olduğu için ekleme işlemi yapılmadı\n",
            uname);
        if (shouldPrintHash) {
            printHash(hash, m);
        }
        printHash(hash, m);
        hashIdx = -1;
    } else {
        printf("Tablo dolu olduğu için %s elemanı tabloya yerleştirilemedi.\n",
               uname);
        printHash(hash, m);
        hashIdx = -1;
    }
    return hashIdx;
}

/*
 @brief Takes a username from stdin and deletes it from the
    hash table if possible

 @param hash Hash table
 @param n Number of original elements
 @param m Number of cells in hash table
 @param lf Load factor
 @param mode Program running mode

 @return
*/
void delete(HashEntry *hash, int n, int m, double lf, Mode mode) {
    int unameLen;
    char *uname = calloc(MAX_UNAME_BUF_LEN, sizeof(char));
    int h1Val;
    int h2Val;
    int deleted = 0;
    int i = 0;
    int hashIdx = 0;
    int key;
    if (uname == NULL) {
        printf("delete: calloc failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Silinmesi istediğiniz kullanıcının adının giriniz: ");
    scanf(" %s", uname);
    unameLen = strlen(uname);
    if (unameLen >= MAX_UNAME_LEN) {
        uname[MAX_UNAME_LEN] = 0;
        unameLen = MAX_UNAME_LEN;
    }
    key = strToNum(uname, unameLen);
    h1Val = h1(key, m);
    h2Val = h2(key, m);
    if (mode == Debug) {
        printf("h1(\"%s\") = %d\n", uname, h1Val);
        printf("h2(\"%s\") = %d\n", uname, h2Val);
    }
    while (deleted == 0 && i < m) {
        hashIdx = compHashIdx(h1Val, h2Val, i, m);
        if (hash[hashIdx].userName != 0 && hash[hashIdx].deleted == 0 &&
            strcmp(uname, hash[hashIdx].userName) == 0) {
            hash[hashIdx].deleted = 1;
            deleted = 1;
        } else if (mode == Debug) {
            printf("%s kelimesi %d. adresten silinemedi\n", uname, hashIdx);
        }
        ++i;
    }
    if (deleted == 1) {
        printf("%d adresindeki %s elemanı silindi.\n", hashIdx, uname);
        printHash(hash, m);
    } else {
        printf("%s elemanı tabloda bulunmuyor\n", uname);
        printHash(hash, m);
    }
    free(uname);
}

/*
 @brief Takes a username from stdin and finds it in the
    hash table if possible

 @param hash Hash table
 @param n Number of original elements
 @param m Number of cells in hash table
 @param lf Load factor
 @param mode Program running mode

 @return
*/
void search(HashEntry *hash, int n, int m, double lf, Mode mode) {
    int key;
    int h1Val;
    int h2Val;
    int found = 0;
    int i = 0;
    int unameLen;
    int hashIdx;
    char *uname = calloc(MAX_UNAME_BUF_LEN, sizeof(char));
    if (uname == NULL) {
        printf("search: calloc failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Aramak istediğiniz kullanıcıyı giriniz: ");
    scanf(" %s", uname);
    unameLen = strlen(uname);
    if (unameLen >= MAX_UNAME_LEN) {
        uname[MAX_UNAME_LEN] = 0;
        unameLen = MAX_UNAME_LEN;
    }
    key = strToNum(uname, unameLen);
    h1Val = h1(key, m);
    h2Val = h2(key, m);
    if (mode == Debug) {
        printf("h1(\"%s\") = %d\n", uname, h1Val);
        printf("h2(\"%s\") = %d\n", uname, h2Val);
    }
    while (found == 0 && i < m) {
        hashIdx = compHashIdx(h1Val, h2Val, i, m);
        if (hash[hashIdx].userName != 0 && hash[hashIdx].deleted == 0 &&
            strcmp(hash[hashIdx].userName, uname) == 0) {
            found = 1;
        } else if (mode == Debug) {
            printf("%s kelimesi %d. adreste bulunamadı.\n", uname, hashIdx);
        }
        ++i;
    }
    if (found == 1) {
        printf("%s elemanı %d adresinde bulunmaktadır.\n", uname, hashIdx);
    } else {
        printf("%s elemanı tabloda bulunamadı.\n", uname);
    }
    free(uname);
}

/*
 @brief Tests strToNum function

 @return
*/
void testStrToNum() {
    char str[] = "tommy";
    int len = strlen(str);
    int num;
    num = strToNum(str, len);
    assert(num == 8135);
}

/*
 @brief Tests compHashIdx function

 @return
*/
void testCompHashIdx() {
    int key, i, m;
    key = 39782;
    i = 0;
    m = 5;
    assert(compHashIdx(h1(key, m), h2(key, m), i, m) == 2);
    i = 1;
    assert(compHashIdx(h1(key, m), h2(key, m), i, m) == 0);
    i = 2;
    assert(compHashIdx(h1(key, m), h2(key, m), i, m) == 3);
}

/*
 @brief Edits the hash table by moving undeleted elements from
    old hash table to a new hash table based on homework specs

 @param hash Hash table
 @param n Number of original elements
 @param m Number of cells in hash table
 @param lf Load factor
 @param mode Program running mode

 @return New hash table
*/
HashEntry *edit(HashEntry *hash, int n, int m, double lf, Mode mode) {
    int i;
    HashEntry *newHash = calloc(m, sizeof(HashEntry));
    if (newHash == NULL) {
        printf("edit: calloc failed\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < m; ++i) {
        if (mode == Debug && hash[i].deleted == 1) {
            printf("%s elemanı eski tabloda silinmişti\n", hash[i].userName);
        }
        if (hash[i].userName != 0 && hash[i].deleted == 0) {
            int newIdx = add(newHash, n, m, lf, hash[i].userName,
                             strlen(hash[i].userName), 0, mode);
            if (mode == Debug) {
                printf("%s elemanı eski tabloda silinmemişti",
                       hash[i].userName);

                printf(
                    ", eski tablodaki adresi: %d, yeni tablodaki adresi: %d\n",
                    i, newIdx);
            }
        }
    }
    printf("Yeni hash tablosu:\n");
    printHash(newHash, m);
    freeHash(hash, m);
    return newHash;
}

/*
 @brief Main function that runs CLI and corresponding hashing
    functions based on user input and displays results

 @param argc Argument count
 @param argv Arguments

 @return Program exit status
*/
int main(int argc, char **argv) {
    Mode mode = parseArgs(argc, argv);
    int n;
    double lf;
    int m;
    char resp;
    HashEntry *hash;
    testCheckPrime();
    testStrToNum();
    testCompHashIdx();
    n = readN();
    lf = readLf();
    m = compM(n, lf);
    hash = calloc(m, sizeof(HashEntry));
    if (hash == NULL) {
        printf("main: calloc failed\n");
        exit(EXIT_FAILURE);
    }
    do {
        int opId;
        printf(
            "\n\t1. Ekleme\n\t2. Silme\n\t3. Arama\n\t4. Düzenle\n\t5. "
            "Göster\n\t6. "
            "Çıkış\n\n");
        printf("İşlem: ");
        scanf(" %c", &resp);
        opId = resp - '0';
        if (opId == 1) {
            int unameLen;
            char *uname = calloc(MAX_UNAME_BUF_LEN, sizeof(char));
            if (uname == NULL) {
                printf("add: calloc failed\n");
                exit(EXIT_FAILURE);
            }
            printf("Yeni kullanıcı adını giriniz: ");
            scanf(" %s", uname);
            unameLen = strlen(uname);
            if (unameLen >= MAX_UNAME_LEN) {
                uname[MAX_UNAME_LEN] = 0;
                unameLen = MAX_UNAME_LEN;
            }
            add(hash, n, m, lf, uname, unameLen, 1, mode);
            free(uname);
        } else if (opId == 2) {
            delete (hash, n, m, lf, mode);
        } else if (opId == 3) {
            search(hash, n, m, lf, mode);
        } else if (opId == 4) {
            hash = edit(hash, n, m, lf, mode);
        } else if (opId == 5) {
            printHash(hash, m);
        } else if (opId != 6) {
            printf("Geçersiz operasyon, yeniden deneyin\n");
        }
    } while (resp != '6');
    freeHash(hash, m);
    return 0;
}
