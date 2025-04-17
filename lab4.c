#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>

struct node {
    int key;
    struct node *left;
    struct node *right;
};

struct node *newNode(int key) {
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->key = key;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

int height(struct node *node) {
    if (node == NULL) return -1;
    int lHeight = height(node->left);
    int rHeight = height(node->right);
    return (lHeight > rHeight ? lHeight : rHeight)+1;
}

struct node *find(struct node *node, int key) {
    if (node == NULL) return NULL;
    if (node->key == key) return node;
    if (node->key < key)
        return find(node->right, key);
    return find(node->left, key);
}

int rec[100] = {0};

void printTree(struct node* node, int depth) {
    int i;
    if(node == NULL) return;
    wprintf(L"\t");

    for(i = 0; i < depth; i++) {
        if(i == depth-1)
            wprintf(L"%ls", rec[depth-1] ? L"├───" : L"└───");
        else
            wprintf(L"%ls", rec[i] ? L"│   " : L"    ");
    }

    wprintf(L"%d\n", node->key);
    rec[depth] = 1;
    printTree(node->left, depth+1);
    rec[depth] = 0;
    printTree(node->right, depth+1);
}

void clear(struct node **n) {
    if (NULL == *n) return;
    clear(&(*n)->left);
    clear(&(*n)->right);
    free(*n);
    *n = NULL;
}

struct node *insert(struct node *node, int key) {
    if (node == NULL) {
        return newNode(key);
    }
    if (node->key == key) {
        return node;
    }

    if (node->key < key) {
        node->right = insert(node->right, key);
    }
    else {
        node->left = insert(node->left, key);
    }
    return node;
}

// Test function for newNode
void test_newNode() {
    printf("Testing newNode()...\n");

    struct node *n = newNode(42);
    assert(n != NULL);
    assert(n->key == 42);
    assert(n->left == NULL);
    assert(n->right == NULL);

    free(n);
    printf("newNode() test passed!\n");
}

// Test function for height
void test_height() {
    printf("Testing height()...\n");

    // Test empty tree
    struct node *root = NULL;
    assert(height(root) == -1);

    // Test single node tree
    root = newNode(10);
    assert(height(root) == 0);

    // Test two-level tree
    root->left = newNode(5);
    assert(height(root) == 1);

    // Test three-level tree
    root->left->right = newNode(7);
    assert(height(root) == 2);

    // Cleanup
    clear(&root);
    assert(root == NULL);

    printf("height() test passed!\n");
}

// Test function for find
void test_find() {
    printf("Testing find()...\n");

    // Create a test tree
    struct node *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);

    // Test finding existing nodes
    struct node *found = find(root, 50);
    assert(found != NULL && found->key == 50);

    found = find(root, 30);
    assert(found != NULL && found->key == 30);

    found = find(root, 70);
    assert(found != NULL && found->key == 70);

    found = find(root, 20);
    assert(found != NULL && found->key == 20);

    found = find(root, 40);
    assert(found != NULL && found->key == 40);

    // Test finding non-existent nodes
    found = find(root, 10);
    assert(found == NULL);

    found = find(root, 60);
    assert(found == NULL);

    // Cleanup
    clear(&root);
    assert(root == NULL);

    printf("find() test passed!\n");
}

// Test function for insert
void test_insert() {
    printf("Testing insert()...\n");

    struct node *root = NULL;

    // Test inserting into empty tree
    root = insert(root, 50);
    assert(root != NULL);
    assert(root->key == 50);
    assert(root->left == NULL);
    assert(root->right == NULL);

    // Test inserting smaller value (goes left)
    root = insert(root, 30);
    assert(root->left != NULL);
    assert(root->left->key == 30);

    // Test inserting larger value (goes right)
    root = insert(root, 70);
    assert(root->right != NULL);
    assert(root->right->key == 70);

    // Test inserting duplicate value (should not change tree)
    struct node *before_insert = root;
    root = insert(root, 50);
    assert(root == before_insert); // Same node returned

    // Test more complex insertions
    root = insert(root, 20); // Should go left of 30
    assert(root->left->left != NULL);
    assert(root->left->left->key == 20);

    root = insert(root, 40); // Should go right of 30
    assert(root->left->right != NULL);
    assert(root->left->right->key == 40);

    // Verify tree structure
    assert(height(root) == 2);

    // Cleanup
    clear(&root);
    assert(root == NULL);

    printf("insert() test passed!\n");
}

// Test function for clear
void test_clear() {
    printf("Testing clear()...\n");

    // Create a test tree
    struct node *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);

    // Verify tree is not empty
    assert(root != NULL);

    // Clear the tree
    clear(&root);

    // Verify tree is now empty
    assert(root == NULL);

    printf("clear() test passed!\n");
}

int main() {
    // Run tests first (in text mode)
    printf("Running BST tests...\n\n");

    test_newNode();
    test_height();
    test_find();
    test_insert();
    test_clear();

    printf("\nAll tests passed successfully!\n");

    // Now create a sample tree to visualize
    struct node *root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
    int num_values = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < num_values; i++) {
        root = insert(root, values[i]);
    }

    printf("\nCreated sample tree with %d nodes\n", num_values);
    printf("Tree height: %d\n", height(root));

    // Switch to Unicode mode for tree visualization
    _setmode(_fileno(stdout), _O_U16TEXT);
    setlocale(LC_ALL, "");

    wprintf(L"\nTree visualization:\n");
    printTree(root, 0);

    // Clean up
    clear(&root);

    return 0;
}