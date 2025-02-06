#include<stdio.h>
#include<math.h>
#define LEN(x,type) sizeof(x)/sizeof(type)
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

struct Point {
    double x;
    double y;
};

void print_point(struct Point p);
void print_point_arr(struct Point p[], int n);
void print_arr(int n, double arr[n]);


struct Point p0 = {0,0};

struct Point sub(struct Point p1, struct Point p2)
{
    return (struct Point){.x = p1.x-p2.x,.y = p1.y-p2.y};
}

int turn(struct Point p1, struct Point p2, struct Point p3)
{
    struct Point 
    dp1 = sub(p2, p1),
    dp2 = sub(p3, p2);

    int det = (dp1.x * dp2.y) - (dp1.y * dp2.x);

    if (det > 0) {
        return 1;
    }
    else if (det == 0) {
        return 0;
    }
    else {
        return -1;
    }
}

int compare_by_angle(const void *ptrp1, const void *ptrp2)
{
    struct Point p1 = *(struct Point *)ptrp1;
    struct Point p2 = *(struct Point *)ptrp2;
    
    return turn(p0, p1, p2);
}

double sort_points_by_angle(struct Point points[], int n)
{
    qsort(points, n, sizeof(struct Point), compare_by_angle);
}


struct Stack {
    struct Point arr[500];
    int top;
};

void push(struct Stack *s, struct Point p)
{
    s->arr[++s->top] = p; 
}
struct Point pop(struct Stack *s)
{
    return s->arr[s->top--];
}
struct Point peek(struct Stack s)
{
    return s.arr[s.top];
}
void print_stack(struct Stack s)
{
    for (int i = 0; i <= s.top; i++)
    {
        printf("| ");
        print_point(s.arr[s.top - i]);
        printf(" |\n");
    }
}


bool right_turn(struct Point p1, struct Point p2, struct Point p3)
{
    return turn(p1, p2, p3) < 0;
}

int main()
{
    struct Point points[10];
    int n = LEN(points, struct Point);
    
    for (size_t i = 0; i < n; i++)
    {
        points[i] = (struct Point){rand()%10,rand()%10};
    }

    
    double out[n];

    sort_points_by_angle(points, n);
    print_point_arr(points, n);
    printf("\n");

    struct Stack s = {.top = -1};
    
    push(&s, points[0]);
    push(&s, points[1]);
    push(&s, points[2]);

    for (size_t i = 3; i < n; i++)
    {
        while (right_turn(s.arr[s.top], s.arr[s.top-1], s.arr[s.top-2]))
        {
            struct Point last = pop(&s);
            pop(&s);
            push(&s, last);
        }
        push(&s, points[i]);
    }
    

    print_point_arr(s.arr,s.top+1);
    printf("\n");
    return 0;
}

void print_point(struct Point p)
{
    printf("(%.2lf, %.2lf)",p.x,p.y);
}
void print_point_arr(struct Point p[], int n) 
{
    printf("[");
    for (int i = 0; i < n; i++) {
        print_point(p[i]);
        printf(", ");
    }
    printf("\b\b]");
}
void print_arr(int n, double arr[n])
{
    printf("[");
    for (size_t i = 0; i < n; i++) {
        printf("%lf, ", arr[i]);
    }
    printf("\b\b]");
}