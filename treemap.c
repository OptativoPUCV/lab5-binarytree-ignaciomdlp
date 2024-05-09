#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* newTreeMap = (TreeMap *)malloc(sizeof(TreeMap));
    if (newTreeMap == NULL) return NULL;

    newTreeMap->root = newTreeMap->current = NULL;
    newTreeMap->lower_than = lower_than;
    //new->lower_than = lower_than;
    return newTreeMap;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL || value == NULL) return;

    if (tree->root == NULL){
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
    }
    else{
        TreeNode * aux = tree->root;
        while (aux != NULL){
            if (is_equal(tree, key, aux->pair->key)) return;
            if (tree->lower_than(key, aux->pair->key) == 1){
                if (aux->left == NULL){
                    aux->left = createTreeNode(key, value);
                    aux->left->parent = aux;
                    tree->current = aux->left;
                    return;
                }
                else aux = aux->left;
            }
            else{
                if (aux->right == NULL){
                    aux->right = createTreeNode(key, value);
                    aux->right->parent = aux;
                    tree->current = aux->right;
                    return;
                }
                else aux = aux->right;
            }
        }
    }
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL) x = x->left;
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL) return; // Verificar que el árbol y el nodo sean válidos
    if (node->left == NULL && node->right == NULL){ // Caso 1: El nodo no tiene hijos
        if (node->parent == NULL) tree->root = NULL; // Si el nodo es la raíz, se establece la raíz en NULL
        else{ // Si el nodo no es la raíz
            if(node->parent->left == node) node->parent->left = NULL; 
            else node->parent->right = NULL;
        }
        free(node);
        return;
    }
    if (node->left != NULL && node->right != NULL){ // Caso 2: El nodo tiene dos hijos
        TreeNode * aux = minimum(node->right);// Se busca el nodo más pequeño en el subárbol derecho
        node->pair->key = aux->pair->key;// Se reemplaza la clave del nodo con la clave del nodo más pequeño
        node->pair->value = aux->pair->value; // Se reemplaza el valor del nodo con el valor del nodo más pequeño
        removeNode(tree, aux);
        free(node);
        return;
    }
    else{
        TreeNode * child; // Se asigna el hijo del nodo a eliminar
        if (node->left != NULL) child = node->left; // Si el nodo tiene un hijo izquierdo, se asigna el hijo izquierdo
        else child = node->right; // Si el nodo tiene un hijo derecho, se asigna el hijo derecho
        child->parent = node->parent; // Se establece el padre del hijo en el padre del nodo a eliminar
        if (node->parent == NULL) tree->root = child; // Si el nodo a eliminar es la raíz, se establece el hijo como la raíz
        else{ // Si el nodo a eliminar no es la raíz
            if (node == node->parent->left) node->parent->left = child;
            else node->parent->right = child;
        }
        free(node);
        return;
    }
}
    

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key){
    TreeNode* aux = tree->root;

    while (aux != NULL){
        if (is_equal(tree, key, aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        } 
        if (tree->lower_than(key, aux->pair->key))aux = aux->left;
        else aux = aux->right;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree){
    TreeNode* aux = tree->root;
    if (aux == NULL) return NULL;

    while (aux->left != NULL) aux = aux->left;
    tree->current = aux;
    return aux->pair;
}


Pair * nextTreeMap(TreeMap * tree){
    /*TreeNode* aux = tree->current;
    if (aux == NULL) return NULL;
    if (aux->right != NULL){
        aux = minimum(aux->right);
        tree->current = aux;
        return aux->pair;
    }
    else{
        while (aux->parent != NULL && aux == aux->parent->right){
            aux = aux->parent;
        }
        tree->current = aux->parent;
        return aux->parent->pair;
    }*/
    return NULL;
}
