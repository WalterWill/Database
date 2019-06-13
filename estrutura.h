//
// Created by willj on 12/06/2019.
//
#ifndef BANCO_ESTRUTURA_H
#define BANCO_ESTRUTURA_H
#endif //BANCO_ESTRUTURA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <stdbool.h>

//===Estruturas===
typedef struct datanode
{
    int num_ferramenta;
    char descricao[50];
}DataNode;


typedef struct node
{
    DataNode registro;
    struct node *next;
}Node;

typedef struct list
{
    int size;
    Node *head;
    Node *last;
} list;

//===Classes===

//===Prototipo das Funcoes===
list *createList();                     //Cria a lista
void push(list *lista,datanode data);   //Insere um node no inicio da lista
void pop(list *lista);                  //Apaga o primeiro node da lista
void insert(list *lista, DataNode data, int index); //Insere um node em um ponto especifico
void erase(list *lista, int index);     //Exclui um node em um ponto especifico
bool isEmpty(list *lista);              //Verifica se a lista esta vazia
int indexOf(list *lista, Node* node);   //Busca a posicao de um node
Node *atPos(list *lista, int index);    //Busca um node em uma determinada posicao
void printList(list *lista);            //Imprime toda a lista
void final(list *lista, DataNode data); //Adiciona no fim da lista
DataNode setData(int num, char descricao);  //Insere dados no DataNode - Não finalizado


//===Variaveis globais===


//===Funcoes===
list *createList()
{
    list *lista =(list*) malloc(sizeof(list));
    lista->size = 0;
    lista->head = NULL;
    lista->last = NULL;

    return lista;
}

void push(list *lista, DataNode num)
{
    Node *newNode =(Node*) malloc(sizeof(Node));

    newNode->registro = num;
    newNode->next = lista->head;
    lista->head = newNode;
    lista->last = newNode;
    lista->size++;
}

void printList(list *lista)
{

    if (isEmpty(lista)) {
        printf("Lista vazia\n");
    }
    else
    {
        Node *pointer = lista->head;
        while (pointer != NULL)
        {
            printf("%d\n", pointer->registro.num_ferramenta);
            printf("%d\n", pointer->registro.descricao);
            pointer = pointer->next;
        }
    }
}

/*
bool isEmpty(list *lista)
{
    return (lista->size == 0);  //Acho que não faz nada...Corrigir
}
*/

void pop(list *lista)
{
    if (!isEmpty(lista)) {
        Node *pointer = lista->head;
        lista->head = pointer->next;

        free(pointer);
        lista->size--;
    }
}

Node *atPos(list *lista, int index)
{
    if (index >= 0 && index < lista->size) {
        Node *pointer = lista->head;
        int i;
        for( i = 0; i < index; i++)
        {
            pointer = pointer->next;
        }
        return pointer;
    }
    printf("Indice Invalido\n");
    return NULL;
}

int indexOf(list *lista, Node *node)
{
    if(node != NULL){
        Node *pointer = lista->head;
        int index = 0;

        while(pointer != node && pointer != NULL){
            pointer = pointer->next;
            index++;
        }

        if (pointer != NULL) {
            return index;
        }
    }
    printf("Node nao pertence a lista\n");
    return -1;
}

void erase(list *lista, int index)
{
    /* Verificar se não é o primeiro da lista ou o ultimo.
     * se for, atualizar o cabeçario
     */
    if (index == 0) {
        pop(lista);
    }
    else
    {
        Node *current = atPos(lista, index);
        if (current != NULL) {
            Node *previous = atPos(lista, index - 1);
            previous->next = current->next;

            free(current);
            lista->size--;
        }
    }
}

void insert(list *lista, DataNode data, int index)
{
    /* Verificar se será inserido na ultima posição.
     * se for, executar função final
     */

    if(index == 0){
        push(lista,data);
    }
    else
    {
        Node *current = atPos(lista, index);
        if (current != NULL)
        {
            Node *previous = atPos(lista, index - 1);
            Node *newNode = (Node *)malloc(sizeof(Node));

            newNode->registro = data;

            previous->next = newNode;
            newNode->next = current;
            lista->size++;

        }
    }



}


void final(list *lista, DataNode data)
{
    if (lista->head != NULL) {
        //Adicionar ao final da lista
        Node *last = lista->last;
        if (last->next == NULL)
        {
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->registro = data;
            newNode->next = last->next;

            last->next = newNode;

            lista->size++;
            lista->last = newNode;
        }
    }
    else
    {
        push(lista, data);
    }
}

DataNode setData(int num, char desc[50])
{
    DataNode *newData = (DataNode *)malloc(sizeof(DataNode));
    newData->num_ferramenta = num;
    strcpy(newData->descricao, desc);

    return *newData;
}