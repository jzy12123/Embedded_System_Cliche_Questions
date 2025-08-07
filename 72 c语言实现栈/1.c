// 用C语言实现一个顺序栈

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

// 初始化栈
void initStack(Stack *s) {
    s->top = -1;
}

// 判断栈是否为空
int isEmpty(Stack *s) {
    return s->top == -1;
}

// 判断栈是否已满
int isFull(Stack *s) {
    return s->top == MAX_SIZE - 1;
}

// 入栈
int push(Stack *s, int value) {
    if (isFull(s)) {
        printf("栈已满，无法入栈！\n");
        return 0;
    }
    s->data[++(s->top)] = value;
    return 1;
}

// 出栈
int pop(Stack *s, int *value) {
    if (isEmpty(s)) {
        printf("栈为空，无法出栈！\n");
        return 0;
    }
    *value = s->data[(s->top)--];
    return 1;
}

// 获取栈顶元素
int getTop(Stack *s, int *value) {
    if (isEmpty(s)) {
        printf("栈为空，无法获取栈顶元素！\n");
        return 0;
    }
    *value = s->data[s->top];
    return 1;
}

// 测试栈的功能
int main() {
    Stack s;
    int value;

    initStack(&s);

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    if (getTop(&s, &value))
        printf("栈顶元素为: %d\n", value);

    while (!isEmpty(&s)) {
        pop(&s, &value);
        printf("出栈元素: %d\n", value);
    }

    return 0;
}
