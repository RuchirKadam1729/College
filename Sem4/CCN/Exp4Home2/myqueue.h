#define GENERATE_QUEUE_CODE_FOR_TYPE(QUEUE_T)                               \
    struct queue                                                            \
    {                                                                       \
        QUEUE_T arr[500];                                                   \
        int front;                                                          \
        int rear;                                                           \
    };                                                                      \
                                                                            \
    struct queue queue() { return (struct queue){.front = 0, .rear = -1}; } \
                                                                            \
    int isEmpty(struct queue q)                                             \
    {                                                                       \
        return q.front > q.rear;                                            \
    }                                                                       \
                                                                            \
    struct queue enqueue(struct queue q, QUEUE_T e)                         \
    {                                                                       \
        q.arr[++q.rear] = e;                                                \
        return q;                                                           \
    }                                                                       \
                                                                            \
    struct queue dequeue(struct queue q, QUEUE_T *e)                        \
    {                                                                       \
        *e = q.arr[q.front++];                                              \
        return q;                                                           \
    }                                                                       \
                                                                            \
    int queue_count(struct queue q)                                         \
    {                                                                       \
        return q.rear - q.front + 1;                                        \
    }
