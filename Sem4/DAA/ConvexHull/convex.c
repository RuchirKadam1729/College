#include<stdio.h>
#include<math.h>
#define LEN(x,type) sizeof(x)/sizeof(type)
#include<stdlib.h>

struct Point {
    double x;
    double y;
};

void map_points(double (*func)(struct Point), int n, struct Point arr_in[n], double arr_out[n])
{
    for (int i = 0; i < n; i++)
    {
        arr_out[i] = func(arr_in[i]);
    }
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
void print_arr(double arr[], int n)
{
    printf("[");
    for (size_t i = 0; i < n; i++) {
        printf("%lf, ", arr[i]);
    }
    printf("\b\b]");
}
double atan2_point(struct Point p)
{
    return atan2(p.y, p.x);
}

int compare_by_angle(const void *ptrp1, const void *ptrp2)
{
    struct Point p1 = *(struct Point *)ptrp1;
    struct Point p2 = *(struct Point *)ptrp2;
    return (atan2_point(p1) - atan2_point(p2) > 0);
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
    //printf("+------+\n");
}

struct Point sub(struct Point p1, struct Point p2)
{
    return (struct Point){.x = p1.x-p2.x,.y = p1.y-p2.y};
}
int main(int anglec, char const *anglev[])
{
    struct Point points[10];
    int n = LEN(points, struct Point);
    
    for (size_t i = 0; i < n; i++)
    {
        points[i] = (struct Point){rand()%10,rand()%10};
    }

    
    double out[n];
    print_point_arr(points, n);
    printf("\n");
    map_points(atan2_point, n, points, out);
    print_arr(out, n);
    printf("\n");
    sort_points_by_angle(points, n);
    print_point_arr(points, n);
    printf("\n");
    struct Stack s = {.top = -1};
    
    push(&s, points[0]);
    push(&s, points[1]);
    push(&s, points[2]);
    
    for (size_t i = 3; i < n; i++)
    {
        double current_turn_angle = atan2_point(sub(points[i], s.arr[s.top]));
        double previous_turn_angle = atan2_point(sub(s.arr[s.top], s.arr[s.top - 1]));
        
        if (current_turn_angle < previous_turn_angle)
            pop(&s);
            
        push(&s, points[i]);
    }
    
    print_point_arr(s.arr,s.top+1);
    return 0;
}