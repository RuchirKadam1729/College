#include<stdio.h>
#include<math.h>
#include<stdlib.h>  
#include<stdbool.h>
#include<time.h>
#include"raylib.h"
#include<string.h>
#define LEN(x,type) sizeof(x)/sizeof(type)

#pragma region pointshit
struct Point {
    double x;
    double y;
};

struct Point p0;

struct Point sub(struct Point p1, struct Point p2)
{
    return (struct Point){.x = p1.x-p2.x, .y = p1.y-p2.y};
}

void print_point(struct Point p);
void print_point_arr(struct Point p[], int n);
void print_arr(int n, double arr[n]);

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


bool right_turn(struct Point p1, struct Point p2, struct Point p3)
{
    return turn(p1, p2, p3) == 1;
}

double distSq(struct Point p1, struct Point p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

int compare_by_angle(const void *ptrp1, const void *ptrp2)
{
    struct Point p1 = *(struct Point *)ptrp1;
    struct Point p2 = *(struct Point *)ptrp2;
    
    int theta = turn(p0, p1, p2);

    if (theta == 0)
    {
        return (distSq(p0,p2) >= distSq(p0,p1)) ? -1 : 1;
    }
    
    return (theta == -1) ? -1 : 1;
}


int idx_point_with_min_y(struct Point arr[], int n)
{
    struct Point min = (struct Point){__INT32_MAX__, __INT32_MAX__};
    int min_i = 0;
    for (size_t i = 0; i < n; i++)
    {
       if (arr[i].y < min.y || (arr[i].y < min.y && arr[i].x < min.x))
       {
            min = arr[i];
            min_i = i;
       }
    }
    return min_i;
}

#pragma endregion

#pragma region stackshit
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

void print_stack(struct Stack s);
#pragma endregion



struct Stack convex_hull(struct Point points[], int n)
{
    

    print_point_arr(points, n);
    printf("\n");

    int min_i = idx_point_with_min_y(points, n);
    
    struct Point temp = points[0];
    points[0] = points[min_i];
    points[min_i] = temp;   

    p0 = points[0];
    qsort(points+1,n-1,sizeof(struct Point),compare_by_angle);
    int m = 1;
    
    for (size_t i = 1; i < n; i++)
    {
        while (i<n-1 && turn(p0,points[i],points[i+1])==0)
            i++;

        points[m] = points[i];
        m++;
    }
    
    
    print_point_arr(points, m);
    printf("\n");

    struct Stack s = {.top = -1};
    
    push(&s, points[0]);
    push(&s, points[1]);
    push(&s, points[2]);
    for (size_t i = 3; i < m; i++)
    {
        while (right_turn(s.arr[s.top-1], s.arr[s.top], points[i]))
        {
            pop(&s);
        }
        push(&s, points[i]);
    }
    
    print_point_arr(s.arr,s.top+1);
    printf("\n");
    return s;
}


int main()
{
    srand(clock());
    #define N 1000
    struct Point points[N];
    int n = LEN(points, struct Point);
    for (size_t i = 0; i < n; i++)
    {
        points[i] = (struct Point){rand() % n -n/2, rand() % n -n/2};
    }
    
    struct Point old_points[n];
    memcpy(old_points, points, n*sizeof(struct Point));

    struct Stack s = convex_hull(points, n);

    InitWindow(1600,900,"hi");
    SetTargetFPS(144);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircle(1600/2, 900/2,10, GRAY);
        
        #define scale 800.0/N
        #define X(a) scale*a+1600/2
        #define Y(a) -scale*a + 900/2

        int point_thick = 5;
        int hull_thick = 2;
        for (size_t i = 0; i < n; i++)
        {
            DrawCircle(X(old_points[i].x),Y(old_points[i].y),point_thick,BLUE);
        }

        for (size_t i = 1; i <= s.top; i++)
        {
            DrawCircle(X(s.arr[i].x), Y(s.arr[i].y),hull_thick,RED);
            DrawLineEx((Vector2){X(s.arr[i-1].x),Y(s.arr[i-1].y)},(Vector2){X(s.arr[i].x),Y(s.arr[i].y)},hull_thick,RED);
        }
        DrawLineEx((Vector2){X(s.arr[s.top].x),Y(s.arr[s.top].y)},(Vector2){X(s.arr[0].x),Y(s.arr[0].y)},hull_thick,RED);
        EndDrawing();
    }
    CloseWindow();
    
    

    
    return 0;
}




#pragma region printing
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
void print_stack(struct Stack s)
{
    for (int i = 0; i <= s.top; i++)
    {
        printf("| ");
        print_point(s.arr[s.top - i]);
        printf(" |\n");
    }
}
#pragma endregion