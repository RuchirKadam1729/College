#include <stdio.h>
#include <stdlib.h>
struct item
{
    double fraction;
    double price;
    double weight;
};

double ratio(struct item item)
{
    return item.price / item.weight;
}

void assign_price_and_weight(struct item item[], double price[], double weight[], int n)
{
    for (size_t i = 0; i < n; i++)
    {
        item[i].fraction = 1;
        item[i].price = price[i];
        item[i].weight = weight[i];
    }
}

void print(struct item item, char end)
{
    printf("{price: %.2lf, weight: %.2lf%c}", item.price, item.weight, end);
}

int compare_by_ratio(const void *a, const void *b)
{
    struct item
        item1 = *(struct item *)a,
        item2 = *(struct item *)b;

    double val = ratio(item1) - ratio(item2);
    if (val == 0) return 0;
    if (val > 0) return -1;
    if (val < 0) return 1;
}

struct knapsack
{
    struct item *item;
    int N;
    double W;
};

double price(struct knapsack knapsack)
{
    double price;
    for (size_t i = 0; i < knapsack.N; i++)
    {
        price += knapsack.item[i].fraction * knapsack.item[i].price;
    }
    return price;
}

double knapsack_greedy(struct item item[], struct knapsack knapsack)
{
    int w = knapsack.W;
    int i = 0;
    while (w >= item[i].weight)
    {
        knapsack.item[i] = item[i];
        w -= item[i].weight;
        i++;
    }

    if (w > 0)
    {
        double frac = w / item[i].weight;

        knapsack.item[i] = (struct item){.fraction = frac, .price = item[i].price, .weight = item[i].weight};
    }

    return price(knapsack);
}

int main()
{
    int N = 3;
    double W = 25;
    double price[] = {25, 24, 15};
    double weight[] = {18, 15, 10};
    struct item item[N];
    struct item knapsack_items[N];
    struct knapsack knapsack = {.item = knapsack_items, .N = N, .W = W};
    assign_price_and_weight(item, price, weight, N);

    for (size_t i = 0; i < N; i++)
        print(item[i], ' ');

    qsort(item, N, sizeof(item[0]), compare_by_ratio);
    
    printf("\n");
    for (size_t i = 0; i < N; i++)
        print(item[i], ' ');
    double price1 = knapsack_greedy(item, knapsack);
    printf("\nprice1: %lf\n", price1);

    
}