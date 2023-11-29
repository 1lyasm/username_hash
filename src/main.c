#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_MODE 2
#define MAX_UNAME_LEN 256

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

double readLF() {
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

int strToNum(char *str, int unameLen, int m) {
    int num = 0;
    int i;
    for (i = 0; i < unameLen; ++i) {
        // TODO prevent overflow
        int power = unameLen - i - 1;
        int powerRes = pow(m, power);
        int charVal = str[i] - 'A' + 1;
        // printf("strToNum: num: %d\n", num);
        // printf("num += %d ^ %d * %d", m, power, charVal);
        num = num + powerRes * charVal;
    }
    return num;
}

void execAdd(char **hash, int n, int m, int lf) {
    char *uname = malloc(MAX_UNAME_LEN * sizeof(char));
    int unameLen;
    int key;
    if (uname == NULL) {
        printf("execAdd: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    printf("execAdd: called\n");
    printf("Yeni kullanıcı adını giriniz: ");
    scanf(" %s", uname);
    unameLen = strlen(uname);
    printf("execAdd: uname: %s\n", uname);
    key = strToNum(uname, unameLen, m);
    printf("execAdd: key: %d\n", key);
    free(uname);
}

void execDelete(char **hash, int n, int m, int lf) {
    printf("execDelete: called\n");
}

void execSearch(char **hash, int n, int m, int lf) {
    printf("execSearch: called\n");
}

void testStrToNum() {
    char str[] = "tommy";
    int len = strlen(str);
    int m = 7;
    int num;
    // printf("testStrToNum: sizeof(str): %ld\n", sizeof(str));
    // printf("testStrToNum: m: %d, len: %d\n", m, len);
    num = strToNum(str, len, m);
    // printf("testStrToNum: num: %d\n", num);
    assert(num == 143550);
}

int main(int argc, char **argv) {
    Mode mode = parseArgs(argc, argv);
    int n;
    double lf;
    int m;
    char resp;
    char **hash;
    testCheckPrime();
    testStrToNum();
    printMode(mode);
    n = readN();
    printf("main: n: %d\n", n);
    lf = readLF();
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
        printf("\n\t1. Ekleme\n\t2. Silme\n\t3. Arama\n\n");
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
        } else {
            printf("Geçersiz operasyon, yeniden deneyin\n");
        }
    } while (resp != 'q');
    free(hash);
    return 0;
}
