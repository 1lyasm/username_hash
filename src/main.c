#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_MODE 2
#define MAX_UNAME_LEN 32
#define MAX_UNAME_BUF_LEN 256

typedef enum { Normal, Debug, Error } Mode;

typedef struct {
    char *userName;
    int deleted;
} HashEntry;

void printMode(Mode mode) {
    char *asStr = "error";
    if (mode == Normal) {
        asStr = "normal";
    } else if (mode == Debug) {
        asStr = "debug";
    }
    printf("printMode: Mode: %s\n", asStr);
}

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

Mode parseArgs(int argc, char **argv) {
    char *modeArg;
    if (argc != 2) {
        printf("Program için 1 parametre gerekli: NORMAL ve ya DEBUG\n");
        exit(EXIT_FAILURE);
    }
    modeArg = argv[1];
    return parseMode(modeArg);
}

int readN() {
    int n = 0;
    printf("N değerini giriniz: ");
    scanf(" %d", &n);
    return n;
}

double readLf() {
    double lf;
    printf("Load factor değerini giriniz: ");
    scanf(" %lf", &lf);
    return lf;
}

int checkPrime(int num) {
    int i;
    int isPrime = 1;
    // printf("checkPrime: called with num: %d\n", num);
    if (num <= 3) {
        // printf("checkPrime: num (%d) <= 3\n", num);
        return 1;
    }
    for (i = 2; i * i <= num && isPrime == 1; ++i) {
        // printf("checkPrime: i: %d\n", i);
        if (num % i == 0) {
            // printf("checkPrime: i (%d) divides num (%d)\n", i, num);
            isPrime = 0;
        }
    }
    // printf("checkPrime: isPrime: %d\n", isPrime);
    return isPrime;
}

int compM(int n, double lf) {
    int quotient = (int)ceil(n / lf);
    int m;
    int isPrime = 0;
    // printf("compM: quotient: %d\n", quotient);
    m = quotient - 1;
    while (isPrime == 0) {
        ++m;
        isPrime = checkPrime(m);
    }
    return m;
}

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

int strToNum(char *str, int unameLen) {
    double num = 0;
    int i;
    int r = 3;
    for (i = 0; i < unameLen; ++i) {
        int power = unameLen - i - 1;
        double powerRes = pow(r, power);
        int charVal;
        // printf("strToNum: powerRes: %lf\n", powerRes);
        charVal = str[i] - '0' + 1;
        // printf("add: charVal: %d\n", charVal);
        num = num + powerRes * charVal;
        // printf("strToNum: num: %lf\n", num);
    }
    while (num > INT_MAX) {
        num -= INT_MAX;
    }
    return (int)num;
}

int h1(int key, int m) { return key % m; }

int h2(int key, int m) {
    int m2 = m - 2;
    return 1 + (key % m2);
}

int compHashIdx(int h1Val, int h2Val, int i, int m) {
    return (h1Val + i * h2Val) % m;
}

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

void freeHash(HashEntry *hash, int m) {
    int i;
    for (i = 0; i < m; ++i) {
        free(hash[i].userName);
    }
    free(hash);
}

void add(HashEntry *hash, int n, int m, double lf) {
    char *uname;
    int unameLen;
    int key;
    int i = 0;
    int inserted = 0;
    int hashIdx;
    int h1Val;
    int h2Val;
    uname = calloc(MAX_UNAME_BUF_LEN, sizeof(char));
    if (uname == NULL) {
        printf("add: calloc failed\n");
        exit(EXIT_FAILURE);
    }
    // printf("add: called\n");
    printf("Yeni kullanıcı adını giriniz: ");
    scanf(" %s", uname);
    unameLen = strlen(uname);
    // printf("add: unameLen: %d\n", unameLen);
    if (unameLen >= MAX_UNAME_LEN) {
        uname[MAX_UNAME_LEN] = 0;
        unameLen = MAX_UNAME_LEN;
    }
    // printf("add: unameLen: %d\n", unameLen);
    // printf("add: uname: %s\n", uname);
    key = strToNum(uname, unameLen);
    // printf("add: key: %d\n", key);
    h1Val = h1(key, m);
    // printf("add: h1Val: %d\n", h1Val);
    h2Val = h2(key, m);
    // printf("add: h2Val: %d\n", h2Val);
    while (inserted == 0 && i < m) {
        hashIdx = compHashIdx(h1Val, h2Val, i, m);
        // printf("add: hashIdx: %d\n", hashIdx);
        if (hash[hashIdx].userName == 0) {
            hash[hashIdx].userName = malloc((unameLen + 1) * sizeof(char));
            if (hash[hashIdx].userName == NULL) {
                printf("add: malloc failed\n");
                exit(EXIT_FAILURE);
            }
            strcpy(hash[hashIdx].userName, uname);
            inserted = 1;
        }
        if (hash[hashIdx].deleted == 1 &&
            strcmp(hash[hashIdx].userName, uname) == 0) {
            hash[hashIdx].deleted = 0;
            inserted = 1;
        }
        ++i;
    }
    if (inserted == 1) {
        printf("Elemanınız %d. adrese yerleştirildi.\n", hashIdx);
        printHash(hash, m);
    } else {
        printf("Eleman tabloda mevcut olduğu için ekleme işlemi yapılmadı\n");
        printHash(hash, m);
    }
    free(uname);
}

void delete(HashEntry *hash, int n, int m, double lf) {
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
    while (deleted == 0 && i < m) {
        hashIdx = compHashIdx(h1Val, h2Val, i, m);
        if (hash[hashIdx].userName != 0 && hash[hashIdx].deleted == 0 &&
            strcmp(uname, hash[hashIdx].userName) == 0) {
            hash[hashIdx].deleted = 1;
            deleted = 1;
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

void search(HashEntry *hash, int n, int m, double lf) {
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
    while (found == 0 && i < m) {
        hashIdx = compHashIdx(h1Val, h2Val, i, m);
        if (hash[hashIdx].userName != 0 && hash[hashIdx].deleted == 0 &&
            strcmp(hash[hashIdx].userName, uname) == 0) {
            found = 1;
        }
        ++i;
    }
    if (found == 1) {
        printf("%s elemanı %d adresinde bulunmaktadır.\n", uname, hashIdx);
    } else {
        printf("%s elemanı tabloda bulunamadı.\n", uname);
    }
}

void testStrToNum() {
    char str[] = "tommy";
    int len = strlen(str);
    int num;
    // printf("testStrToNum: sizeof(str): %ld\n", sizeof(str));
    // printf("testStrToNum: m: %d, len: %d\n", m, len);
    num = strToNum(str, len);
    // printf("testStrToNum: num: %d\n", num);
    assert(num == 8135);
}

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

void edit(HashEntry *hash, int n, int m, double lf) {
    printf("edit: called\n");
}

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
    // printMode(mode);
    n = readN();
    // printf("main: n: %d\n", n);
    lf = readLf();
    // printf("main: lf: %lf\n", lf);
    m = compM(n, lf);
    // printf("main: m: %d\n", m);
    hash = calloc(m, sizeof(HashEntry));
    if (hash == NULL) {
        printf("main: malloc failed\n");
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
        // printf("main: resp: %c\n", resp);
        opId = resp - '0';
        if (opId == 1) {
            add(hash, n, m, lf);
        } else if (opId == 2) {
            delete (hash, n, m, lf);
        } else if (opId == 3) {
            search(hash, n, m, lf);
        } else if (opId == 4) {
            edit(hash, n, m, lf);
        } else if (opId == 5) {
            printHash(hash, m);
        } else if (opId != 6) {
            printf("Geçersiz operasyon, yeniden deneyin\n");
        }
    } while (resp != '6');
    freeHash(hash, m);
    return 0;
}
