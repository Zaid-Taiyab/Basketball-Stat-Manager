//
// Created by zaidt on 6/23/2024.
//

// header files
#include <stdio.h> // include standard input/output files
#include <string.h> // include string
#include <stdbool.h> // include booloean file
#include <stdlib.h> // include library functions
#include <ctype.h>  // include to make characters uppercase or lower case

// Player struct
struct PlayerNode {
    char playerName[100]; // string created to store every player's name
    char team[100];       // created this string to store the name of the team of player
    double number;     // this variable basically contains any number depending on what is chosen, (for example if we are choosing points, then this contains points and so on)
    struct PlayerNode* left; // left child of the node of the bst
    struct PlayerNode* right; // right child of the node of the bst
    struct PlayerNode* parent; // parent child of the node of the bst
};

// Struct for containing the points, assists and rebounds
struct BasketballStats {
    struct PlayerNode* pointsBST; // the bst of the points
    struct PlayerNode* assistsBST;// the bst of the assists
    struct PlayerNode* reboundsBST;// the bst of the rebounds
};

// Function declarations
struct PlayerNode* createPlayerNode(char playerName[], char team[], double stat); // function to create the playerNode
struct PlayerNode* insertNode(struct PlayerNode* root, struct PlayerNode* newNode);// insert node function implemented similarly to the insert function in the previous question
struct PlayerNode* deleteNode(struct PlayerNode* root, char playerName[]); // delete node function implemented similarly to the insert function in the previous question
struct PlayerNode* findMin(struct PlayerNode* root);  // finding the minimum key in the binary search tree implemented in the same way as in question 1
struct PlayerNode* findMax(struct PlayerNode* root);  // finding the maximum key in the binary search tree implemented in the same way as in question 1
struct PlayerNode* findSuccessor(struct PlayerNode* node); // function to find the successor of the node implemented like in question 1
struct PlayerNode* findNode(struct PlayerNode* root, char playerName[]); // search and find a specific node
void displayPlayer(struct PlayerNode* player); // display players and his stats
void displayInOrder(struct PlayerNode* root);  // display the player in order of their stats
void displayHighestStats(struct BasketballStats* stats); // displaying the highest stats of the players
void displayLowestStats(struct BasketballStats* stats); // displaying the lowest stats of the players
void listTripleDoubles(struct BasketballStats* stats);  // listing the tripledoubles
void listTripleDoublesHelper(struct PlayerNode* pointsRoot, struct PlayerNode* assistsRoot, struct PlayerNode* reboundsRoot); // helper function for the earlier function
void displayAllPlayersDescending(struct PlayerNode* root); // displaying all the players with highest on the top and lowest on the bottom

// Function to create a new player node
struct PlayerNode* createPlayerNode(char playerName[], char team[], double stat) {
    struct PlayerNode* newNode = (struct PlayerNode*) malloc (sizeof(struct PlayerNode)); // creating a new node on the heap
    if (newNode != NULL) {
        strcpy(newNode->playerName, playerName); // copying the name
        strcpy(newNode->team, team); // copying the team
        newNode->number = stat;  // assigning the statistic of player
        newNode->left = newNode->right = newNode->parent = NULL; // since it is a new node, all the pointers of it are null initially
    }
    return newNode; // return the new node
}

// Function to insert a player node into BST
struct PlayerNode* insertNode(struct PlayerNode* root, struct PlayerNode* newNode) {
    struct PlayerNode* parent = NULL; // creating parent node
    struct PlayerNode* current = root; // current node to navigate
    // Find the correct position to insert the new node
    while (current != NULL) {
        parent = current;
        if (newNode->number < current->number)
            current = current->left; // if the stat is less, put it in left child
        else
            current = current->right; // if the stat is greater, put it in right child
    }
    // Set parent for the new node
    newNode->parent = parent;
    // Insert the new node
    if (parent == NULL) {
        return newNode; // return newNode since the tree is empty
    } else if (newNode->number < parent->number) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    return root;
}

// Function to delete a player node from BST
struct PlayerNode* deleteNode(struct PlayerNode* root, char playerName[]) {
    struct PlayerNode* nodeToDelete = findNode(root, playerName);
    if (nodeToDelete == NULL) { // no node is associated with this player
        printf("No such player\n"); // print the error message
        return root; // return the root
    }
    if (nodeToDelete->left == NULL && nodeToDelete->right == NULL) {
        // Case 1: Node to delete has no children
        if (nodeToDelete->parent == NULL) {
            // Node to delete is root and has no children
            free(nodeToDelete);
            return NULL;
        }
        if (nodeToDelete == nodeToDelete->parent->left) {
            nodeToDelete->parent->left = NULL;
        } else {
            nodeToDelete->parent->right = NULL;
        }
    } else if (nodeToDelete->left != NULL && nodeToDelete->right != NULL) {
        // Case 3: Node to delete has two children
        struct PlayerNode* successor = findSuccessor(nodeToDelete);
        // Copy successor's data to nodeToDelete
        strcpy(nodeToDelete->playerName, successor->playerName);
        strcpy(nodeToDelete->team, successor->team);
        nodeToDelete->number = successor->number;
        // Delete successor
        if (successor->parent->left == successor) {
            successor->parent->left = NULL;
        } else {
            successor->parent->right = NULL;
        }
        free(successor);
    } else {
        // Case 2: Node to delete has one child
        struct PlayerNode* child = (nodeToDelete->left != NULL) ? nodeToDelete->left : nodeToDelete->right;
        if (nodeToDelete->parent == NULL) {
            // Node to delete is root and has one child
            child->parent = NULL;
            free(nodeToDelete);
            return child;
        }
        if (nodeToDelete == nodeToDelete->parent->left) {
            nodeToDelete->parent->left = child;
        } else {
            nodeToDelete->parent->right = child;
        }
        child->parent = nodeToDelete->parent;
        free(nodeToDelete);
    }
    return root;
}

// Function to find the node with minimum value in a subtree
struct PlayerNode* findMin(struct PlayerNode* root) {
    if (root == NULL){
        return NULL;
    }
    while (root->left != NULL) { // iterate until we reach the lowest value of the tree
        root = root->left;
    }
    return root;
}

// Function to find the node with maximum value in a subtree
struct PlayerNode* findMax(struct PlayerNode* root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->right != NULL) { // iterate until we reach the highest value of the tree
        root = root->right;
    }
    return root;
}

// Function to find the successor of a node
struct PlayerNode* findSuccessor(struct PlayerNode* x) {
    if (x == NULL)
        return NULL;
    // Case 1: If the node has a right subtree, find the minimum in that subtree
    if (x->right != NULL) {
        return findMin(x->right);
    }
    // Case 2: Traverse up the tree to find the successor
    struct PlayerNode* successor = NULL;
    struct PlayerNode* current = x->right;  // Start from the right child
    while (current != NULL) {
        if (x->number < current->number) {
            successor = current;
            current = current->left;
        } else if (x->number > current->number) {
            current = current->right;
        } else {
            break;
        }
    }
    return successor;
}

// Function to find a player node by name
struct PlayerNode* findNode(struct PlayerNode* root, char playerName[]) {
    while (root != NULL) {
        int playerCompared = strcmp(playerName, root->playerName);
        if (playerCompared < 0) {
            root = root->left;
        } else if (playerCompared > 0) {
            root = root->right;
        } else {
            return root;
        }
    }
    return NULL;
}

// Function to display player information
void displayPlayer(struct PlayerNode* player) {
    printf("Name: %s, Team: %s, number: %.2f\n", player->playerName, player->team, player->number);
}

// Function to display players in descending order of points
void displayInOrder(struct PlayerNode* root) {
    if (root != NULL) {
        displayInOrder(root->right);
        displayPlayer(root);
        displayInOrder(root->left);
    }
}

// Function to display player with highest points, assists, and rebounds
void displayHighestStats(struct BasketballStats* stats) {
    printf("Player with Highest Points: ");
    struct PlayerNode* maxPointsPlayer = findMax(stats->pointsBST);
    if (maxPointsPlayer != NULL)
        displayPlayer(maxPointsPlayer);
    else
        printf("No player found.\n");
    printf("Player with Highest Assists: ");
    struct PlayerNode* maxAssistsPlayer = findMax(stats->assistsBST);
    if (maxAssistsPlayer != NULL)
        displayPlayer(maxAssistsPlayer);
    else
        printf("No player found.\n");
    printf("Player with Highest Rebounds: ");
    struct PlayerNode* maxReboundsPlayer = findMax(stats->reboundsBST);
    if (maxReboundsPlayer != NULL)
        displayPlayer(maxReboundsPlayer);
    else
        printf("No player found.\n");
}

// Function to display player with lowest points, assists, and rebounds
void displayLowestStats(struct BasketballStats* stats) {
    printf("Player with Lowest Points: ");
    struct PlayerNode* minPointsPlayer = findMin(stats->pointsBST);
    if (minPointsPlayer != NULL)
        displayPlayer(minPointsPlayer);
    else
        printf("No player found.\n");
    printf("Player with Lowest Assists: ");
    struct PlayerNode* minAssistsPlayer = findMin(stats->assistsBST);
    if (minAssistsPlayer != NULL)
        displayPlayer(minAssistsPlayer);
    else
        printf("No player found.\n");
    printf("Player with Lowest Rebounds: ");
    struct PlayerNode* minReboundsPlayer = findMin(stats->reboundsBST);
    if (minReboundsPlayer != NULL)
        displayPlayer(minReboundsPlayer);
    else
        printf("No player found.\n");
}

// Function to list names of players with triple doubles
void listTripleDoubles(struct BasketballStats* stats) {
    printf("Players with Triple Doubles:\n");
    listTripleDoublesHelper(stats->pointsBST, stats->assistsBST, stats->reboundsBST);
}

// Helper function to recursively list names of players with triple doubles
void listTripleDoublesHelper(struct PlayerNode* pointsRoot, struct PlayerNode* assistsRoot, struct PlayerNode* reboundsRoot) {
    if (pointsRoot != NULL) {
        listTripleDoublesHelper(pointsRoot->left, assistsRoot, reboundsRoot);
        // Check if this player has triple doubles
        if (pointsRoot->number >= 10 && findNode(assistsRoot, pointsRoot->playerName) != NULL &&
            findNode(reboundsRoot, pointsRoot->playerName) != NULL) {
            printf("%s from %s\n", pointsRoot->playerName, pointsRoot->team);
            listTripleDoublesHelper(pointsRoot->right, assistsRoot, reboundsRoot);
        }
    }
}
// Function to display all players in Points BST in descending order of points
void displayAllPlayersDescending(struct PlayerNode *root) {
    printf("Players in descending order of Points:\n");
    displayInOrder(root);
}

// main function of the program
int main() {
        struct BasketballStats stats; // create new struct to store the stats
        stats.pointsBST = stats.assistsBST = stats.reboundsBST = NULL; // initialize all 3 binary search trees
        int choice, dataType; // variables for choice for the menu and for the data type being stored respectively
        char playerName[100], teamName[100]; // strings for player and team names respectively
        double number; // variable to store whatever statistic the user enters (points, assist, rebounds)
        while (true) {
            printf("\nBasketball Player numbers\n");
            printf("-----------------------------------\n");
            printf("1. Add Player Data\n");
            printf("2. Delete a Player\n");
            printf("3. Display Player with Highest Stats\n");
            printf("4. Display Player with Lowest Stats\n");
            printf("5. List Players with Triple Doubles\n");
            printf("6. Display All Players in Descending Order of Points\n");
            printf("7. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    printf("Enter Data Type (1: Points, 2: Assists, 3: Rebounds): ");
                    scanf("%d", &dataType);
                    printf("Enter Player Name: ");
                    getchar(); // Clear newline character from buffer
                    fgets(playerName, sizeof(playerName), stdin);
                    playerName[strcspn(playerName, "\n")] = '\0'; // Remove newline from input
                    printf("Enter Team Name: ");
                    fgets(teamName, sizeof(teamName), stdin);
                    teamName[strcspn(teamName, "\n")] = '\0'; // Remove newline from input
                    printf("\n");
                    printf("Enter Player Statistic: ");
                    scanf("%lf", &number);
                    switch (dataType) {
                        case 1:
                            stats.pointsBST = insertNode(stats.pointsBST,createPlayerNode(playerName, teamName, number)); // add to the tree of points
                            break;
                        case 2:
                            stats.assistsBST = insertNode(stats.assistsBST,createPlayerNode(playerName, teamName, number)); // add to the tree of assists
                            break;
                        case 3:
                            stats.reboundsBST = insertNode(stats.reboundsBST,createPlayerNode(playerName, teamName, number)); // add to the tree of rebounds
                            break;
                        default:
                            printf("Invalid data type!\n"); // error message for invalid types
                            break;
                    }
                    break;
                case 2:
                    printf("Enter Data Type (1: Points, 2: Assists, 3: Rebounds): ");
                    scanf("%d", &dataType);
                    printf("Enter Player Name to Delete: ");
                    getchar(); // Clear newline character from buffer
                    fgets(playerName, sizeof(playerName), stdin);
                    playerName[strcspn(playerName, "\n")] = '\0'; // Remove newline from input
                    switch (dataType) {
                        case 1:
                            stats.pointsBST = deleteNode(stats.pointsBST, playerName);
                            break;
                        case 2:
                            stats.assistsBST = deleteNode(stats.assistsBST, playerName);
                            break;
                        case 3:
                            stats.reboundsBST = deleteNode(stats.reboundsBST, playerName);
                            break;
                        default:
                            printf("Invalid data type!\n");
                            break;
                    }
                    break;
                case 3:
                    displayHighestStats(&stats);
                    break;
                case 4:
                    displayLowestStats(&stats);
                    break;
                case 5:
                    listTripleDoubles(&stats);
                    break;
                case 6:
                    displayAllPlayersDescending(stats.pointsBST);
                    break;
                case 7:
                    printf("Exiting program...\n");
                    return 0;
                default:
                    printf("Invalid choice! Please enter a number between 1 and 7.\n");
                    break;
            }
        }
}

