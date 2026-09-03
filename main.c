#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define COIN_KINDS   5U
#define TOY_COUNT    4U
#define NAME_LEN    16U

static const uint16_t COIN_VALUE[COIN_KINDS] = { 25U, 50U, 100U, 200U, 500U };
static uint16_t coinCount[COIN_KINDS];

typedef struct {
    char     name[NAME_LEN];
    uint32_t price;
} Toy_t;

static Toy_t shop[TOY_COUNT];

static void     seedBank(void);
static void     addCoins(void);
static void     takeCoins(void);
static uint32_t bankTotal(void);
static uint32_t sumCoins(const uint16_t *counts, uint8_t n);
static uint8_t  biggestPile(void);
static void     showBank(void);
static void     drawBar(uint16_t value, uint16_t full, uint8_t width);
static void     buyToy(void);
static void     bankReport(void);

static void seedBank(void) {
    coinCount[0] = 4U;
    coinCount[1] = 2U;
    coinCount[2] = 5U;
    coinCount[3] = 1U;
    coinCount[4] = 0U;

    strncpy(shop[0].name, "Teddy Bear", NAME_LEN);
    shop[0].price = 300U;

    strncpy(shop[1].name, "Toy Car", NAME_LEN);
    shop[1].price = 500U;

    strncpy(shop[2].name, "Lego Set", NAME_LEN);
    shop[2].price = 1000U;

    strncpy(shop[3].name, "Video Game", NAME_LEN);
    shop[3].price = 2500U;

    printf("Piggy bank initialized with starting coins and shop items.\n");
}

static void addCoins(void) {
    int coinIdx = 0;
    int amount = 0;

    printf("\nAvailable coins:\n");
    for (uint8_t i = 0U; i < COIN_KINDS; i++) {
        printf(" [%u] %u piastres\n", i, COIN_VALUE[i]);
    }

    printf("Select coin index (0-4): ");
    if (scanf("%d", &coinIdx) != 1 || coinIdx < 0 || coinIdx >= (int)COIN_KINDS) {
        printf("Invalid coin index! Nothing added.\n");
        while (getchar() != '\n');
        return;
    }

    printf("How many coins to add? ");
    if (scanf("%d", &amount) != 1 || amount <= 0) {
        printf("Invalid amount! Nothing added.\n");
        while (getchar() != '\n');
        return;
    }

    coinCount[coinIdx] += (uint16_t)amount;
    printf("Added %d coin(s) of %u piastres.\n", amount, COIN_VALUE[coinIdx]);
}

static void takeCoins(void) {
    int coinIdx = 0;
    int amount = 0;

    printf("\nAvailable coins:\n");
    for (uint8_t i = 0U; i < COIN_KINDS; i++) {
        printf(" [%u] %u piastres (you have %u)\n", i, COIN_VALUE[i], coinCount[i]);
    }

    printf("Select coin index (0-4): ");
    if (scanf("%d", &coinIdx) != 1 || coinIdx < 0 || coinIdx >= (int)COIN_KINDS) {
        printf("Invalid coin index! Nothing taken.\n");
        while (getchar() != '\n');
        return;
    }

    printf("How many coins to take out? ");
    if (scanf("%d", &amount) != 1 || amount <= 0) {
        printf("Invalid amount! Nothing taken.\n");
        while (getchar() != '\n');
        return;
    }

    if ((uint16_t)amount > coinCount[coinIdx]) {
        printf("Refused! You do not have that many coins.\n");
        return;
    }

    coinCount[coinIdx] -= (uint16_t)amount;
    printf("Successfully took out %d coin(s).\n", amount);
}

static uint32_t sumCoins(const uint16_t *counts, uint8_t n) {
    if (n == 0U) {
        return 0U;
    }
    uint8_t idx = n - 1U;
    return ((uint32_t)counts[idx] * COIN_VALUE[idx]) + sumCoins(counts, idx);
}

static uint32_t bankTotal(void) {
    return sumCoins(coinCount, (uint8_t)COIN_KINDS);
}

static uint8_t biggestPile(void) {
    uint8_t maxIdx = 0U;
    for (uint8_t i = 1U; i < COIN_KINDS; i++) {
        if (coinCount[i] > coinCount[maxIdx]) {
            maxIdx = i;
        }
    }
    return maxIdx;
}

static void drawBar(uint16_t value, uint16_t full, uint8_t width) {
    if (full == 0U) full = 1U;
    uint32_t filledChars = ((uint32_t)value * width) / full;
    if (filledChars > width) filledChars = width;

    for (uint8_t i = 0U; i < width; i++) {
        if (i < filledChars) {
            putchar('#');
        } else {
            putchar('-');
        }
    }
}

static void showBank(void) {
    uint16_t maxCount = coinCount[biggestPile()];
    if (maxCount == 0U) maxCount = 1U;

    printf("\n=== Piggy Bank Contents ===\n");
    for (uint8_t i = 0U; i < COIN_KINDS; i++) {
        printf("%3u p | Count: %3u | ", COIN_VALUE[i], coinCount[i]);
        drawBar(coinCount[i], maxCount, 15U);
        printf("\n");
    }
    printf("Total Money: %u piastres\n", bankTotal());
}

static void buyToy(void) {
    uint32_t total = bankTotal();
    int choice = 0;

    printf("\n=== Toy Shop ===\n");
    for (uint8_t i = 0U; i < TOY_COUNT; i++) {
        printf(" [%u] %-12s - %u piastres\n", i, shop[i].name, shop[i].price);
    }

    printf("Select a toy to buy (0-3): ");
    if (scanf("%d", &choice) != 1 || choice < 0 || choice >= (int)TOY_COUNT) {
        printf("Invalid toy choice!\n");
        while (getchar() != '\n');
        return;
    }

    uint32_t price = shop[choice].price;
    if (total >= price) {
        printf("You can buy %s! You will have %u piastres left over.\n", shop[choice].name, total - price);
    } else {
        printf("You cannot afford %s yet. You need %u more piastres.\n", shop[choice].name, price - total);
    }
}

static void bankReport(void) {
    uint32_t totalMoney = bankTotal();
    uint16_t totalCoins = 0U;
    uint8_t affordableToys = 0U;

    for (uint8_t i = 0U; i < COIN_KINDS; i++) {
        totalCoins += coinCount[i];
    }

    for (uint8_t i = 0U; i < TOY_COUNT; i++) {
        if (totalMoney >= shop[i].price) {
            affordableToys++;
        }
    }

    uint8_t topIdx = biggestPile();

    printf("\n===================================\n");
    printf("         PIGGY BANK REPORT         \n");
    printf("===================================\n");
    printf(" Total Money     : %u piastres\n", totalMoney);
    printf(" Total Coins     : %u coins\n", totalCoins);
    printf(" Largest Pile    : %u piastres (%u coins)\n", COIN_VALUE[topIdx], coinCount[topIdx]);
    printf(" Affordable Toys : %u of %u\n", affordableToys, TOY_COUNT);
    printf("===================================\n");
}

int main(void) {
    seedBank();
    int choice = 0;

    do {
        printf("\n--- MY PIGGY BANK MENU ---\n");
        printf("1. Show Bank Contents\n");
        printf("2. Add Coins\n");
        printf("3. Take Coins\n");
        printf("4. Buy Toy Check\n");
        printf("5. Bank Report\n");
        printf("6. Reset / Seed Bank\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: showBank(); break;
            case 2: addCoins(); break;
            case 3: takeCoins(); break;
            case 4: buyToy(); break;
            case 5: bankReport(); break;
            case 6: seedBank(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid option, try again.\n"); break;
        }
    } while (choice != 0);

    return 0;
}