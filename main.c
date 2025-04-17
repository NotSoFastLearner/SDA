#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

typedef struct node {
    int x;
    struct node *left;
    struct node *right;
} node;

// Create a new node with given value
node* create_node(int value) {
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->x = value;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// Calculate the height of the binary tree
int height(node* root) {
    // Base case: empty tree has height 0
    if (root == NULL) {
        return 0;
    }

    // Recursively calculate height of left and right subtrees
    int left_height = height(root->left);
    int right_height = height(root->right);

    // Height is the maximum path length plus 1 for current node
    return (left_height > right_height ? left_height : right_height) + 1;
}

// Find a subtree with the given root value
node* find(node* root, int value) {
    // Base cases:
    // 1. Tree is empty - return NULL (subtree not found)
    // 2. Current node has the target value - return pointer to this subtree
    if (root == NULL || root->x == value) {
        return root;
    }

    // Search in the left or right subtree based on BST property
    if (value < root->x) {
        return find(root->left, value);
    } else { // value > root->x
        return find(root->right, value);
    }
}

// Insert a subtree into the appropriate place in the main tree
node* insert(node* root, node* subtree) {
    // If main tree is empty, return the subtree as new root
    if (root == NULL) {
        return subtree;
    }

    // If subtree is NULL, nothing to insert
    if (subtree == NULL) {
        return root;
    }

    // Insert based on BST property
    if (subtree->x < root->x) {
        root->left = insert(root->left, subtree);
    } else if (subtree->x > root->x) {
        root->right = insert(root->right, subtree);
    } else {
        // Value already exists - replace the existing node

        // Save the old children
        node* old_left = root->left;
        node* old_right = root->right;

        // Replace with subtree
        *root = *subtree;
        free(subtree); // Free the subtree root as its data is now copied

        // Merge the old children
        root->left = insert(root->left, old_left);
        root->right = insert(root->right, old_right);
    }

    return root;
}


node* insert_value(node* root, int value) {
    node* new_node = create_node(value);
    return insert(root, new_node);
}

// Print nodes at a given level
void print_level(node* root, int level) {
    if (root == NULL) {
        if (level == 1) printf("- ");
        return;
    }

    if (level == 1) {
        printf("%d ", root->x);
    } else if (level > 1) {
        print_level(root->left, level - 1);
        print_level(root->right, level - 1);
    }
}

// Print the tree level by level
void print_tree(node* root) {
    if (root == NULL) {
        printf("Tree is empty\n");
        return;
    }

    int h = height(root);
    printf("Binary Tree by Levels:\n");
    for (int i = 1; i <= h; i++) {
        printf("Level %d: ", i);
        print_level(root, i);
        printf("\n");
    }
}

// Clear the tree and free memory
void clear(node** root) {
    if (root == NULL || *root == NULL) return;

    // Post-order traversal to free all nodes
    clear(&((*root)->left));
    clear(&((*root)->right));

    free(*root);
    *root = NULL;
}



// Test functions
void run_tests() {
    printf("Running BST tests...\n\n");

    // Test 1: Insert and find
    node* root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (int i = 0; i < 7; i++) {
        root = insert_value(root, values[i]);
    }

    // Test find function
    node* found_50 = find(root, 50);
    assert(found_50 != NULL && found_50->x == 50);

    node* found_30 = find(root, 30);
    assert(found_30 != NULL && found_30->x == 30);
    assert(found_30->left != NULL && found_30->left->x == 20);
    assert(found_30->right != NULL && found_30->right->x == 40);

    node* found_70 = find(root, 70);
    assert(found_70 != NULL && found_70->x == 70);
    assert(found_70->left != NULL && found_70->left->x == 60);
    assert(found_70->right != NULL && found_70->right->x == 80);

    node* not_found = find(root, 55);
    assert(not_found == NULL);

    // Test 2: Height calculation
    assert(height(NULL) == 0);
    assert(height(root) == 3);

    // Test 3: Insert subtree
    node* subtree = create_node(25);
    subtree->left = create_node(22);
    subtree->right = create_node(28);

    root = insert(root, subtree);

    node* found_25 = find(root, 25);
    assert(found_25 != NULL && found_25->x == 25);
    assert(found_25->left != NULL && found_25->left->x == 22);
    assert(found_25->right != NULL && found_25->right->x == 28);

    // Test 4: Clear
    clear(&root);
    assert(root == NULL);

    printf("All tests passed!\n\n");
}

int main(void) {
    // Run tests
    run_tests();

    return 0;
}