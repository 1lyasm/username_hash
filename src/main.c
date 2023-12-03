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
    printf("compM: quotient: %d\n", quotient);
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
    int num = 0;
    int i;
    int r = 3;
    for (i = 0; i < unameLen; ++i) {
        int power = unameLen - i - 1;
        double powerRes = pow(r, power);
        int charVal;
        printf("strToNum: powerRes: %lf\n", powerRes);
        charVal = str[i] - 'A' + 1;
        printf("execAdd: charVal: %d\n", charVal);
        num = num + (int)powerRes * charVal;
        printf("strToNum: num: %d\n", num);
    }
    return num;
}

int h1(int key, int m) { return key % m; }

int h2(int key, int m) {
    int m2 = m - 2;
    return 1 + (key % m2);
}

int compHashIdx(int h1Val, int h2Val, int i, int m) {
    return (h1Val + i * h2Val) % m;
}

void printHash(char **hash, int m) {
    int i;
    printf("Hash tablosu:\n");
    for (i = 0; i < m; ++i) {
        if (hash[i] != 0) {
            printf("\t%d:\t%s\n", i, hash[i]);
        } else {
            printf("\t%d:\t(null)\n", i);
        }
    }
}

void freeHash(char **hash, int m) {
    int i;
    for (i = 0; i < m; ++i) {
        free(hash[i]);
    }
    free(hash);
}

void execAdd(char **hash, int n, int m, double lf) {
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
        printf("execAdd: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    printf("execAdd: called\n");
    printf("Yeni kullanıcı adını giriniz: ");
    scanf(" %s", uname);
    unameLen = strlen(uname);
    if (unameLen >= MAX_UNAME_LEN) {
        uname[MAX_UNAME_LEN - 1] = 0;
        unameLen = MAX_UNAME_LEN;
    }
    printf("execAdd: unameLen: %d\n", unameLen);
    printf("execAdd: uname: %s\n", uname);
    key = strToNum(uname, unameLen);
    printf("execAdd: key: %d\n", key);
    h1Val = h1(key, m);
    printf("execAdd: h1Val: %d\n", h1Val);
    h2Val = h2(key, m);
    printf("execAdd: h2Val: %d\n", h2Val);
    while (inserted == 0 && i < m) {
        hashIdx = compHashIdx(h1Val, h2Val, i, m);
        printf("execAdd: hashIdx: %d\n", hashIdx);
        if (hash[hashIdx] == 0) {
            hash[hashIdx] = uname;
            inserted = 1;
        }
        ++i;
    }
    if (inserted == 1) {
        printf("Kullanıcı %s %d. indise yerleştirildi\n", uname, hashIdx);
        printHash(hash, m);
    } else {
        printf("Kullanıcı %s tabloya yerleştirilemedi, tabloda boş yer yok\n",
               uname);
        printHash(hash, m);
    }
}

void execDelete(char **hash, int n, int m, double lf) {
    printf("execDelete: called\n");
}

void execSearch(char **hash, int n, int m, double lf) {
    printf("execSearch: called\n");
}

void testStrToNum() {
    char str[] = "tommy";
    int len = strlen(str);
    int num;
    // printf("testStrToNum: sizeof(str): %ld\n", sizeof(str));
    // printf("testStrToNum: m: %d, len: %d\n", m, len);
    num = strToNum(str, len);
    // printf("testStrToNum: num: %d\n", num);
    assert(num == 143550);
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

void execEdit(char **hash, int n, int m, double lf) {
    printf("execEdit: called\n");
}

int main(int argc, char **argv) {
    Mode mode = parseArgs(argc, argv);
    int n;
    double lf;
    int m;
    char resp;
    char **hash;
    testCheckPrime();
    // testStrToNum();
    testCompHashIdx();
    printMode(mode);
    n = readN();
    printf("main: n: %d\n", n);
    lf = readLf();
    printf("main: lf: %lf\n", lf);
    m = compM(n, lf);
    printf("main: m: %d\n", m);
    hash = calloc(m, sizeof(char *));
    if (hash == NULL) {
        printf("main: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    do {
        int opId;
        printf(
            "\n\t1. Ekleme\n\t2. Silme\n\t3. Arama\n\t4. Düzenle\n\t5. "
            "Çıkış\n\n");
        printf("İşlem: ");
        scanf(" %c", &resp);
        printf("main: resp: %c\n", resp);
        opId = resp - '0';
        if (opId == 1) {
            execAdd(hash, n, m, lf);
        } else if (opId == 2) {
            execDelete(hash, n, m, lf);
        } else if (opId == 3) {
            execSearch(hash, n, m, lf);
        } else if (opId == 4) {
            execEdit(hash, n, m, lf);
        } else if (opId != 5) {
            printf("Geçersiz operasyon, yeniden deneyin\n");
        }
    } while (resp != '5');
    freeHash(hash, m);
    return 0;
}