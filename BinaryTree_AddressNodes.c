//CS-531 Assignment 4
//Shiva Shukla
//G#(G01321322)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define input_size 28
#define alias_size 12

/*
The binary search tree structure for the address-alias pair.
*/
struct address_t {
   int octet[4];
   char alias[alias_size];
   struct address_t * leftChild;
   struct address_t * rightChild;
};

// Initializing the root of the address-alias binary search tree.
struct address_t * root = NULL;

//flag maintained to check if a node already exists in the BST.
int isDuplicate = 0;

/*
Method to check if there the address already exist in the BST.
*/
int checkDuplicateAddress(int address[4], struct address_t * tmp, char alias[alias_size]) {
   int flag = strcasecmp(tmp -> alias, alias) != 0 && address[0] == tmp -> octet[0] && address[1] == tmp -> octet[1] && address[2] == tmp -> octet[2] && address[3] == tmp -> octet[3];
   if (flag == 1) {
      fprintf(stderr, "%s", "\nError: Address already exists, please try again!\n");
      isDuplicate = 1;
      return 1;
   }
   return 0;
}

/*
Method to check if the new address details(alias or IP address) already exist in the BST.
*/
void checkIfNodeExists(struct address_t * newNode, struct address_t * curr, int check) {
   //check(flag) is used to not do the alias check in case of update address.
   if (curr != NULL && check && strcasecmp(curr -> alias, newNode -> alias) == 0) {
      fprintf(stderr, "%s", "\nError: Alias already exists, please try again!\n");
      isDuplicate = 1;
      return;
   } else if (curr != NULL && checkDuplicateAddress(newNode -> octet, curr, newNode -> alias) == 1) {
      return;
   } else {
      if (curr == NULL) {
         return;
      }
      checkIfNodeExists(newNode, curr -> leftChild, check);
      checkIfNodeExists(newNode, curr -> rightChild, check);
   }
}

/*
Method to check if the input is empty or invalid.
*/
int isInputValid(struct address_t * node, int checkFlag) {
   if (checkFlag && node -> alias[0] == '\0' || strlen(node -> alias) == 0) {
      fprintf(stderr, "%s", "\nError: Input is empty. Please try again!");
      return 0;
   } else {
      for (int i = 0; i < 4; i++) {
         if (node -> octet[i] < 0 || node -> octet[i] > 255) {
            fprintf(stderr, "%s", "\nError: Input is invalid, not between 0 to 255, both included. Please try again!");
            return 0;
         }
      }
   }
   return 1;
}

/*
Method to add a node in the Binary Search Tree.
*/
struct address_t * addInBST(struct address_t * root, struct address_t * node) {
   if (!root) {
      return node;
   }
   if (strcmp(node -> alias, root -> alias) <= 0) {
      root -> leftChild = addInBST(root -> leftChild, node);
   } else {
      root -> rightChild = addInBST(root -> rightChild, node);
   }
   return root;
}

/*
Method will create the initial binary search tree from the text file.
Method will also add new nodes input by user.
*/
void addAddress(char input[input_size], int a) {
   int octetInput[4];
   char aliasInput[alias_size] = "";
   struct address_t * newNode = (struct address_t * ) malloc(sizeof(struct address_t));
   //sscanf will take the input string and parse it to values for address and alias.
   sscanf(input, "%d.%d.%d.%d %s", &octetInput[0], &octetInput[1], &octetInput[2], &octetInput[3], aliasInput);
   newNode -> octet[0] = octetInput[0];
   newNode -> octet[1] = octetInput[1];
   newNode -> octet[2] = octetInput[2];
   newNode -> octet[3] = octetInput[3];
   strcpy(newNode -> alias, aliasInput);
   newNode -> leftChild = NULL;
   newNode -> rightChild = NULL;
   //checking if input is empty or invalid.
   if (isInputValid(newNode,1) == 0) {
      return;
   }
   //checking if the new node already exists (either alias or address).
   //If it exists, error is displayed and menu is redisplayed.
   struct address_t * curr = root;
   checkIfNodeExists(newNode, curr, 1);
   if (isDuplicate == 1) {
      isDuplicate = 0;
      return;
   }
   //adding the first element if root is empty.
   if (root == NULL) {
      root = newNode;
   } else {
      root = addInBST(root, newNode);
   }
   printf("\nRecord Added for alias: %s", newNode -> alias);
}

/*
Method to search a user specified alias in the BST.
*/
struct address_t * lookUpAddress(char searchAlias[alias_size], struct address_t * tmp) {
   if (tmp == NULL) {
      fprintf(stderr, "%s", "\nError: The alias does not exist!\n");
      return NULL;
   } else if (strcmp(searchAlias, tmp -> alias) == 0) {
      printf("\n%s: %d.%d.%d.%d", searchAlias, tmp -> octet[0], tmp -> octet[1], tmp -> octet[2], tmp -> octet[3]);
      return tmp;
   } else if (strcmp(searchAlias, tmp -> alias) < 0) {
      lookUpAddress(searchAlias, tmp -> leftChild);
   } else {
      lookUpAddress(searchAlias, tmp -> rightChild);
   }
}

/*
Method to update the address of an existing alias.
*/
void updateAddress(char updateAlias[alias_size]) {
   struct address_t * curr = root;
   //lookupAddress will return the node from the BST that maps the alias.
   struct address_t * tmp = lookUpAddress(updateAlias, curr);
   char updatedAddress[input_size];
   int newAddress[4];
   if (tmp != NULL) {
      printf("\nPlease enter the new address for alias-> %s: ", updateAlias);
      fgets(updatedAddress, input_size, stdin);
      //removing the new line character read by fgets.
      if (updatedAddress[strlen(updatedAddress) - 1] == '\n') {
         updatedAddress[strlen(updatedAddress) - 1] = '\0';
      }
      sscanf(updatedAddress, "%d.%d.%d.%d", & newAddress[0], & newAddress[1], & newAddress[2], & newAddress[3]);
      curr = root;
      int result = 0;
      //checking if the new address already exists.
      //If it already exists, error is displayed and the user is re-prompted for the address.
      do {
         isDuplicate = 0;
         struct address_t * tmp1 = (struct address_t * ) malloc(sizeof(struct address_t));
         strcpy(tmp1 -> alias, updateAlias);
         tmp1 -> octet[0] = newAddress[0];
         tmp1 -> octet[1] = newAddress[1];
         tmp1 -> octet[2] = newAddress[2];
         tmp1 -> octet[3] = newAddress[3];
         checkIfNodeExists(tmp1, curr, 0);
         int isValidInput = isInputValid(tmp1,0);
         result = isDuplicate || !isValidInput;
         if (result) {
            printf("\nPlease enter the new address for alias-> %s again: ", updateAlias);
            fgets(updatedAddress, input_size, stdin);
            //removing the new line character read by fgets.
            if (updatedAddress[strlen(updatedAddress) - 1] == '\n') {
               updatedAddress[strlen(updatedAddress) - 1] = '\0';
            }
            sscanf(updatedAddress, "%d.%d.%d.%d", &newAddress[0], &newAddress[1], &newAddress[2], &newAddress[3]);
         }
         curr = root;
      } while (result == 1);
      isDuplicate = 0;
      //updating the searched location with the new address.
      for (int i = 0; i < 4; i++) {
         tmp -> octet[i] = newAddress[i];
      }
      printf("\nAddress updated for alias: %s.", updateAlias);
   }
}

/*
Method to display the aliases for specific user defined location.
*/
void displayAliases(struct address_t * tmp, int a, int b) {
   if (tmp != NULL && tmp -> octet[0] == a && tmp -> octet[1] == b) {
      printf("\n%s", tmp -> alias);
   } else if (tmp==NULL) {
      return;
   }
   displayAliases(tmp -> leftChild, a, b);
   displayAliases(tmp -> rightChild, a, b);
}

/*
Display aliases for a location. Method will take input from user for desired address.
It will then call the utility method for displaying aliases.
*/
void displayAliasesForLocation() {
   int a, b;
   char input[input_size];
   printf("\nEnter locality: ");
   fgets(input,input_size,stdin);
   sscanf(input,"%d.%d",&a,&b);
   //checking the input values are within the range of valid address.
   while ((a < 0 || a > 255) || (b < 0 || b > 255)) {
      fprintf(stderr, "%s", "\nError: Please enter the values between [0-255]:");
      sscanf(input,"%d.%d",&a,&b);
   }
   struct address_t * tmp = root;
   printf("Location: %d.%d",a,b);
   displayAliases(tmp, a, b);
}

/*
Find minimum element in the tree.
*/
struct address_t * find_minimum(struct address_t * tmp) {
   if (tmp == NULL)
      return NULL;
   else if (tmp -> leftChild != NULL)
      return find_minimum(tmp -> leftChild);
   return tmp;
}

/*
Method to delete the alias. Method will take the alias name as input.
*/
struct address_t * deleteAlias(char alias[alias_size], struct address_t * tmp) {
   if (tmp == NULL){
      return NULL;
   }

   if (strcmp(alias, tmp -> alias) > 0){
      tmp -> rightChild = deleteAlias(alias, tmp -> rightChild);
   }
   else if (strcmp(alias, tmp -> alias) < 0){
      tmp -> leftChild = deleteAlias(alias, tmp -> leftChild);
   }
   else {
      if (tmp -> leftChild == NULL && tmp -> rightChild == NULL) {
         free(tmp);
         return NULL;
      } else if (tmp -> leftChild == NULL || tmp -> rightChild == NULL) {
         struct address_t * curr;
         if (tmp -> leftChild == NULL)
            curr = tmp -> rightChild;
         else
            curr = tmp -> leftChild;
         free(tmp);
         return curr;
      } else {
         struct address_t * min = find_minimum(tmp -> rightChild);
         strcpy(tmp -> alias, min -> alias);
         tmp -> rightChild = deleteAlias(min -> alias, tmp -> rightChild);
      }
   }
   return tmp;
}

/*
Method to display the binary search tree as an address-alias pair.
*/
void inorderTraversal(struct address_t * tmp) {
   if (!tmp) {
      return;
   }
   inorderTraversal(tmp -> leftChild);
   printf("\n%s %d.%d.%d.%d", tmp -> alias, tmp -> octet[0], tmp -> octet[1], tmp -> octet[2], tmp -> octet[3]);
   inorderTraversal(tmp -> rightChild);
}

/*
Method to save updated user entries to a new file.
*/
void saveToFile(FILE * fp, struct address_t * tmp) {
   if (tmp == NULL) {
      return;
   }
   saveToFile(fp, tmp -> leftChild);
   fprintf(fp, "%d.%d.%d.%d %s\n", tmp -> octet[0], tmp -> octet[1], tmp -> octet[2], tmp -> octet[3], tmp -> alias);
   saveToFile(fp, tmp -> rightChild);
}

/*
Method that will print the list of available options to the user.
*/
void displayMenu() {
   printf("\n\nEnter menu option:");
   printf("\n1. Add address\n");
   printf("2. Look up address\n");
   printf("3. Update address\n");
   printf("4. Delete Address\n");
   printf("5. Display List\n");
   printf("6. Display aliases for location\n");
   printf("7. Save to file\n");
   printf("8. Quit\n");
}

/*
The entry point of the program. It will first fetch all the records from the text file
and insert into the BST. Then it will prompt the user for an option from the available menu.
*/
int main() {
   //opening the text file containing addresses and alias.
   FILE * fptr = fopen("CS531_Inet.txt", "r");
   if (fptr == NULL) {
      fprintf(stderr, "%s", "Error! File cannot be opened.\n");
      exit(1);
   }
   char textFileLine[input_size];
   //reading from the text file line by line and adding the address and alias in the BST.
   while (fscanf(fptr, "%[^\n] ", textFileLine) != EOF) {
      addAddress(textFileLine,0);
   }
   fclose(fptr); //closing the file after creating the BST from it's data.
   int flag = 1;
   char userInput[input_size];
   struct address_t * tmp = NULL;
   while (flag) {
      displayMenu(); //display the options to the user.
      int option;
      scanf("%d", &option);
      while (getchar() != '\n');
      switch(option){
          case 1:
             printf("\nEnter IPv4 address: ");
             fgets(userInput, input_size, stdin);
             //removing the new line character read by fgets.
             if (userInput[strlen(userInput) - 1] == '\n') {
                userInput[strlen(userInput) - 1] = '\0';
             }
             char aliasInput[alias_size];
             printf("\nEnter alias: ");
             fgets(aliasInput,alias_size,stdin);
             //removing the new line character read by fgets.
             if (aliasInput[strlen(aliasInput) - 1] == '\n') {
                aliasInput[strlen(aliasInput) - 1] = '\0';
             }
             strcat(userInput,aliasInput);
             addAddress(userInput,1);
             break;
          case 2:
             printf("\nEnter Alias:");
             fgets(userInput, alias_size, stdin);
             //removing the new line character read by fgets.
             if (userInput[strlen(userInput) - 1] == '\n') {
                userInput[strlen(userInput) - 1] = '\0';
             }
             tmp = root;
             lookUpAddress(userInput, tmp);
             break;
          case 3:
             printf("\nPlease enter the alias to update the address:");
             fgets(userInput, alias_size, stdin);
             //removing the new line character read by fgets.
             if (userInput[strlen(userInput) - 1] == '\n') {
                userInput[strlen(userInput) - 1] = '\0';
             }
             updateAddress(userInput);
             break;
          case 4:
             printf("\nPlease enter the alias to delete:");
             fgets(userInput, alias_size, stdin);
             //removing the new line character read by fgets.
             if (userInput[strlen(userInput) - 1] == '\n') {
                userInput[strlen(userInput) - 1] = '\0';
             }
             tmp = root;
             struct address_t * tmpFind = lookUpAddress(userInput, tmp);
             if (tmpFind == NULL) {
                break;
             }
             printf("\nPlease confirm if you want to delete the alias-> %s [Y/N]: ", userInput);
             char option;
             scanf("%c", & option);
             if (option == 'Y' || option == 'y') {
                tmp = root;
                deleteAlias(userInput, tmp);
                printf("\nRecord for alias -> %s deleted.",userInput);
             }
             break;
          case 5:
             if (root == NULL) {
                fprintf(stderr, "%s", "\nError: No available record. List is Empty!");
                break;
             }
             tmp = root;
             inorderTraversal(tmp);
             break;
          case 6:
             displayAliasesForLocation();
             break;
          case 7:
             printf("\nPlease enter the file name to save aliases: ");
             fgets(userInput, input_size, stdin);
             //removing the new line character read by fgets.
             if (userInput[strlen(userInput) - 1] == '\n') {
                userInput[strlen(userInput) - 1] = '\0';
             }
             FILE * fp = fopen(strcat(userInput, ".txt"), "w+");
             struct address_t * tmp = root;
             saveToFile(fp, tmp);
             fclose(fp);
             printf("\nFile %s saved!", userInput);
             break;
          case 8:
             flag = 0;
             fprintf(stderr, "%s", "\nGoodBye....!\n");
             break;
          default:
             printf("\nEnter a valid option. Please try again!");
          }
   }
   return 0;
}
