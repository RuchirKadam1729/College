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
    printf("{frac: %.2lf, price: %.2lf, weight: %.2lf}%c", item.fraction, item.price, item.weight, end);
}

int compare_by_ratio(const void* a, const void* b)
{
    struct item
        item1 = *(struct item*)a,
        item2 = *(struct item*)b;

    double val = ratio(item1) - ratio(item2);
    if (val == 0)
        return 0;
    if (val > 0)
        return -1;
    if (val < 0)
        return 1;
}

struct knapsack
{
    struct item* item;
    int N;
    double W;
};

double price(struct knapsack knapsack)
{
    double price = 0;
    for (size_t i = 0; i < knapsack.N; i++)
    {
        price += knapsack.item[i].fraction * knapsack.item[i].price;
    }
    return price;
}

double knapsack_greedy(struct item item[], struct knapsack* pknapsack)
{
    struct knapsack knapsack = *pknapsack;
    int w = knapsack.W;
    int i = 0;

    while (i < knapsack.N && w >= item[i].weight)
    {
        knapsack.item[i] = item[i];
        w -= item[i].weight;
        i++;
    }

    if (i < knapsack.N && w > 0)
    {
        double frac = w / item[i].weight;

        knapsack.item[i] = (struct item){ .fraction = frac, .price = item[i].price, .weight = item[i].weight };
    }
    pknapsack->N = i + 1;
    return price(knapsack);
}

int main()
{
    int N = 12;
    double W = 70;
    double price[N];
    double weight[N];
    struct item item[N];
    struct item knapsack_items[N];
    struct knapsack knapsack = { .item = knapsack_items, .N = N, .W = W };

    for (size_t i = 0; i < N; i++)
    {
        price[i] = rand() % 100;
        weight[i] = rand() % 100;
    }

    assign_price_and_weight(item, price, weight, N);

    qsort(item, N, sizeof(item[0]), compare_by_ratio);

    printf("items:\n");
    for (size_t i = 0; i < N; i++)
        print(item[i], '\n');
    printf("\n");
    double price1 = knapsack_greedy(item, &knapsack);


    printf("\nmmax price: %lf\n", price1);

    printf("Selected items in knapsack:\n");
    for (size_t i = 0; i < knapsack.N; i++)
    {
        print(knapsack.item[i], '\n');
    }

}