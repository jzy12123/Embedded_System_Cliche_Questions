#include <stdio.h>
#include <stdlib.h>

// 定义链表节点
struct Node {
    int data;
    struct Node* next;
};

int main() {
    // 创建链表
    struct Node* head = NULL;
    struct Node* second = NULL;
    struct Node* third = NULL;
    
    // 分配内存
    head = (struct Node*)malloc(sizeof(struct Node));
    second = (struct Node*)malloc(sizeof(struct Node));
    third = (struct Node*)malloc(sizeof(struct Node));
    
    // 赋值和链接
    head->data = 1;
    head->next = second;
    
    second->data = 2;
    second->next = third;
    
    third->data = 3;
    third->next = NULL;
    
    // 访问元素（需要遍历）
    printf("node[2]: %d\n", second->data);
    
    // 插入元素（在第二个位置）
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = 4;
    newNode->next = second->next;
    second->next = newNode;
    
    // 删除元素（删除新插入的节点）
    second->next = newNode->next;
    free(newNode);
    
    // 释放内存
    while(head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
    
    return 0;
}