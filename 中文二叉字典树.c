/*************************************************************************
	> File Name: 哈夫曼树.c
	> Author: luoyi
	> Mail: 596890983@qq.com
	> Created Time: 2018年02月14日 星期三 18时53分09秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#define change(a, b) { \
    Node *temp = a; \
    a = b; \
    b = temp; \
}

typedef struct DTNode {
    int flag;
    struct DTNode *lchild, *rchild;
} DTNode, *DTree;

typedef struct Node {
    int val;
    int data;
    struct Node *lchild, *rchild;
} Node, *Tree;

Node *init_halfNode(int val, int data) {
    Node *p = (Node *)malloc(sizeof(Node));
    p->data = data;
    p->val = val;
    p->lchild = p->rchild = NULL;
    return p;
}

Tree build_hftree(Tree *hftree, int n) {
    Node *p1, *p2;
    while(n > 1) {
        p1 = hftree[n - 1];
        p2 = hftree[n - 2];
        if(p1->data > p2->data) change(p1, p2);
        for(int i = 0; i < n - 2; i++) {
            if(hftree[i]->data < p1->data) {
                change(p1, p2);
                change(p1, hftree[i]);
            } else if(hftree[i]->data < p2->data) {
                change(p2, hftree[i]);
            }
        }
        Node *p = init_halfNode(' ', p1->data + p2->data);
        p->lchild = p1;
        p->rchild = p2;
        hftree[n - 2] = p;
        hftree[n - 1] = NULL;
        n--;
    }
    return hftree[n - 1];
}

void half_code1(Tree tree, char *code, int n, char *sum[100]) {
    if(!tree) return;
    if(!tree->lchild) {
        code[n] = '\0';
        for(int i = 0; i < n; i++) sum[tree->val][i] = code[i];
        return;
    }
    code[n] = '0';
    half_code1(tree->lchild, code, n + 1, sum);
    code[n] = '1';
    half_code1(tree->rchild, code, n + 1, sum);
    return;
}

void clear_halftree(Tree tree) {
    if(!tree) return;
    clear_halftree(tree->lchild);
    clear_halftree(tree->rchild);
    free(tree);
    return;
}

void print_halfcode(char *sum[256]) {
    for(int i = 0; i < 256; i++) {
        if(strcmp(sum[i], "")) {
            char c = i;
            printf("%d(%c) code is %s\n", i, c, sum[i]);
        }
    }
    return;
}

DTNode *init_datree() {
    DTNode *p = (DTNode *)malloc(sizeof(DTNode));
    p->flag = 0;
    p->lchild = p->rchild = NULL;
    return p;
}

void insert_datree(DTree dtree, char *halfcode[256],unsigned char *str) {
    DTNode *p = dtree;
    for(int i = 0; str[i]; i++) {
        int len = strlen(halfcode[str[i]]);
        char *code = (char *)calloc
        ((len + 1), sizeof(char)); 
        strncpy(code, halfcode[str[i]], len);
        for(int j = 0; j < len; j++) {
            if(code[j] == '0') {
                if(!p->lchild) {
                    p->lchild = init_datree();
                }
                p = p->lchild;
            } else {
                if(!p->rchild) {
                    p->rchild = init_datree();
                }
                p = p->rchild;
            }
        }
    }
    p->flag = 1;
    return;
}

void print_datree(DTree dtree, char *code, int n, int *cnt) {
    if(!dtree) return;
    if(dtree->flag) {
        code[n] = '\0';
        printf("%d string is %s\n", *cnt, code);
        (*cnt)++;
        return;
    }
    code[n] = '0';
    print_datree(dtree->lchild, code, n + 1, cnt);
    code[n] = '1';
    print_datree(dtree->rchild, code, n + 1, cnt);
    return;
}

int search_datree(DTree dtree, unsigned char *str, char *halfcode[256]) {
    DTNode *p = dtree;
    for(int i = 0; str[i]; i++) {
        int len = strlen(halfcode[str[i]]);
        if(len == 0) return 0;
        char code[len];
        strncpy(code, halfcode[str[i]], len);
        for(int j = 0; j < len; j++) {
            if(code[j] == '0') {
                if(!p->lchild) {
                    return 0;
                }
                p = p->lchild;
            } else {
                if(!p->rchild) {
                    return 0;
                }
                p = p->rchild;
            }
        }
    }
    return 1;
}

void clear_datree(DTree dtree) {
    if(!dtree) return;
    if(dtree->lchild) clear_datree(dtree->lchild);
    if(dtree->rchild) clear_datree(dtree->rchild);
    free(dtree);
    return;
}

int main() {
    int op;
    FILE *fin1 = fopen("company.txt", "r");
    DTree dtree = init_datree();
    char *halfcode[256];//记录256中字节的编码
    while(~scanf("%d", &op)) {
        switch(op) {
            case 1: {//times记录每个字节的值出现的次数
                FILE *fin1 = fopen("company.txt", "r");
                int *times = (int *)calloc(256, sizeof(int)), cnt = 0;
                unsigned char **str = 
                     (unsigned char **)malloc(sizeof(char *) * 8787);
                     //n个字符串,无符号，每个字节最大值为256
                for(int i = 0; i < 8787; i++) {
                    str[i] = (unsigned char *)malloc(sizeof(char) * 1000);
                    fscanf(fin1, "%s", str[i]);
                    for(int j = 0; str[i][j]; j++) {
                    //对每个字符串安字节记录次数，cnt记录有多少中字节信息
                        (times[str[i][j]] == 0) && (cnt++);
                        times[str[i][j]]++;
                    }
                }//哈夫曼树初始结点
                Tree *hftree = (Tree *)malloc(sizeof(Tree) * cnt);
                for(int i = 0, j = 0; i < 256 && j < cnt; i++) {
                    //初始化哈夫曼树结点
                    (times[i]) && (
                        hftree[j++] = init_halfNode(i, times[i]));
                }
                Tree tree;
                //建立哈夫曼树
                tree = build_hftree(hftree, cnt);
                //code记录每一次得到的编码
                char *code = (char *)malloc(sizeof(char) * 100);
                //halfcode记录所有的编码
                for(int i = 0; i < 256; i++) { 
                    halfcode[i] = (char *)calloc(100, sizeof(char));
                }
                //生成哈夫曼编码
                half_code1(tree, code, 0, halfcode);
                //打印哈夫曼编码
                print_halfcode(halfcode);
                //清空哈夫曼树树
                clear_halftree(tree);
                //将哈夫曼编码插入二叉字典树中
                for(int i = 0; i < 8787; i++) {
                    insert_datree(dtree, halfcode, str[i]);
                }
                for(int i = 0; i < 8787; i++) {
                    free(str[i]);
                }
                free(str);
                break;
            }
            case 2: {
                //输入待查询的字符串
                FILE *fin = fopen("corpus1.txt", "r");
                unsigned char *str = (unsigned char *)
                malloc(sizeof(char) * 10000);
                while(~fscanf(fin, "%s", str)) {
                    int res = search_datree(dtree, str, halfcode);
                    if(res) printf("in\n");
                    else printf("not in\n");
                }
                free(str);
                break;
            }
            case 3: {
                char *str = (char *)malloc(sizeof(char) * 100);
                int cnt = 0;
                //打印二叉树
                print_datree(dtree, str, 0, &cnt);
                break;
            }
            default: {
                printf("input op error\n");
            }
        }
    }
    clear_datree(dtree);
    return 0;
}
