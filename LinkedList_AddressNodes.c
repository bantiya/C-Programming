//CS-531 Assignment 3
//Shiva Shukla
//G#(G01321322)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define input_size 28
#define alias_size 12

/*
The linked list structure for the address-alias pair.
*/
struct address_t{
    int octet[4];
    char alias[alias_size];
    struct address_t *next;
};

// Initializing the head of the address-alias linked list.
struct address_t *head = NULL;

/*
Method to check if there the address already exist in linked list.
*/
int checkDuplicateAddress(int address[4], struct address_t *tmp, char alias[alias_size]){
    int flag = strcasecmp(tmp->alias,alias)!=0 && address[0] == tmp->octet[0] && address[1] == tmp->octet[1] && address[2] == tmp->octet[2] && address[3] == tmp->octet[3];
    if(flag == 1){
        fprintf(stderr,"%s","Error: Address already exists, please try again!\n");
        return 1;
    }
    return 0;
}

/*
Method to check if the new address details(alias or IP address) already exist in the linked list.
*/
int checkIfNodeExists(struct address_t *newNode){
    struct address_t *curr = head;
    while(curr!=NULL){
        if(strcasecmp(curr->alias,newNode->alias) == 0){
            fprintf(stderr,"%s","Error: Alias already exists, please try again!\n");
            return 1;
        }
        else if(checkDuplicateAddress(newNode->octet, curr, newNode->alias) == 1){
            return 1;
        }
        curr = curr->next;
    }
}

/*
Method to check if the input is empty or invalid.
*/
int isInputValid(struct address_t *node){
    if(node->alias[0] == '\0' || strlen(node->alias) == 0){
        fprintf(stderr,"%s","\nError: Input is empty. Please try again!");
        return 0;
    }
    else{
        for(int i=0;i<4;i++){
            if(node->octet[i]<0 || node->octet[i]>255){
                fprintf(stderr,"%s","\nError: Input is invalid, not between 0 to 255, both included. Please try again!");
                return 0;
            }
        }
    }
    return 1;
}

/*
Method will create the initial linked list from the text file. Method will also add new nodes input by user.
*/
void addAddress(char input[input_size]){
    int octetInput[4];
    char aliasInput[alias_size];
    struct address_t *newNode = (struct address_t *)malloc(sizeof(struct address_t));
    //sscanf will take the input string and parse it to values for address and alias.
    sscanf(input,"%d.%d.%d.%d %s",&octetInput[0],&octetInput[1],&octetInput[2],&octetInput[3],aliasInput);
    newNode->octet[0] = octetInput[0];
    newNode->octet[1] = octetInput[1];
    newNode->octet[2] = octetInput[2];
    newNode->octet[3] = octetInput[3];
    strcpy(newNode->alias,aliasInput);
    newNode->next = NULL;
    //checking if input is empty or invalid.
    if(isInputValid(newNode) == 0){
        return;
    }
    //checking if the new node already exists (either alias or address).
    //If it exists, error is displayed and menu is redisplayed.
    int newNodeExists = checkIfNodeExists(newNode);
    if(newNodeExists == 1){
        return;
    }
    else if(head == NULL){
        head = newNode;
        printf("\nRecord Added for alias: %s.",newNode->alias);
    }
    else{
        struct address_t *tmp = head;
        while(tmp->next!=NULL){
            tmp = tmp->next;
        }
        tmp->next = newNode;
        printf("\nRecord Added for alias: %s",newNode->alias);
    }
}

/*
Method which will look up the linked list for the alias that is searched.
It will print the address if it exists. Else, it will print error and redisplay menu.
*/
struct address_t* lookUpAddress(char searchAlias[alias_size]){
    struct address_t *tmp = head;
    while(tmp!=NULL){
        if(strcasecmp(tmp->alias, searchAlias) == 0){
            printf("\nThe address for alias -> %s is: %d.%d.%d.%d", searchAlias, tmp->octet[0],tmp->octet[1],tmp->octet[2],tmp->octet[3]);
            return tmp;
        }
        tmp = tmp->next;
    }
    fprintf(stderr,"%s","\nError: The alias does not exist!\n");
    return NULL;
}

/*
Method to update the address of the alias. User will input the alias to be updated.
User will then give the new address as input. New address will be checked for duplicate
against the existing data.
*/
void updateAddress(char updateAlias[alias_size]){
    //lookupAddress will return the node from the linked list that maps the alias.
    struct address_t *tmp = lookUpAddress(updateAlias);
    char updatedAddress[input_size];
    int newAddress[4];
    if(tmp!=NULL){
        printf("\nPlease enter the new address for alias-> %s: ", updateAlias);
        fgets(updatedAddress,input_size,stdin);
        //removing the new line character read by fgets.
        if(updatedAddress[strlen(updatedAddress)-1]=='\n'){
                updatedAddress[strlen(updatedAddress)-1] = '\0';
        }
        sscanf(updatedAddress,"%d.%d.%d.%d",&newAddress[0],&newAddress[1],&newAddress[2],&newAddress[3]);
        struct address_t *curr = head;
        int flag=0;
        //checking if the new address already exists.
        //If it already exists, error is displayed and the user is reprompted.
        do{
            struct address_t *tmp1 = (struct address_t *)malloc(sizeof(struct address_t));
            strcpy(tmp1->alias,"temp");
            tmp1->octet[0] = newAddress[0];
            tmp1->octet[1] = newAddress[1];
            tmp1->octet[2] = newAddress[2];
            tmp1->octet[3] = newAddress[3];
            //the loop will check the linked list if it contains a duplicate address.
            while(curr!=NULL){
                    flag = checkDuplicateAddress(newAddress,curr,updateAlias);
                    //it will validate the new input.
                    if(isInputValid(tmp1) == 0){
                        flag = 1;
                        break;
                    }
                    if(flag==1){
                        break;
                    }
                    curr = curr->next;
            }
            if(flag==1){
                printf("\nPlease enter the new address for alias-> %s again: ", updateAlias);
                fgets(updatedAddress,input_size,stdin);
                //removing the new line character read by fgets.
                if(updatedAddress[strlen(updatedAddress)-1]=='\n'){
                    updatedAddress[strlen(updatedAddress)-1] = '\0';
                }
                sscanf(updatedAddress,"%d.%d.%d.%d",&newAddress[0],&newAddress[1],&newAddress[2],&newAddress[3]);
            }
            curr = head;
        }while(flag == 1);
        //updating the searched location with the new address.
        for(int i=0;i<4;i++){
            tmp->octet[i] = newAddress[i];
        }
        printf("\nAddress updated for alias: %s!",updateAlias);
    }
}

/*
Method will search the alias to be deleted. Then, it will confirm if the user wants to delete the alias.
If yes, the function will delete the alias.
*/
void deleteAlias(char deleteAlias[alias_size]){
    //lookUpAddress will get the node from the linked list.
    struct address_t *tmp = lookUpAddress(deleteAlias);
    if(tmp == NULL){
        return;
    }
    char confirm;
    printf("\nPlease confirm if you want to delete the alias, Y(yes)/N(no):");
    scanf(" %c",&confirm);
    if(confirm == 'Y' || confirm == 'y'){
        struct address_t *prev = head;
        while(prev->next->alias != tmp->alias){
            prev = prev->next;
        }
        prev->next = tmp->next;
        tmp->next = NULL;
        free(tmp);
        printf("\nRecord Deleted for alias: %s!",deleteAlias);
    }
}

/*
Method will ask the user to input x and y of the address "x.y.a.b".
Method will then display all aliases that have the given first and second part of the address.
*/
void displayAliasesForLocation(){
    int a,b;
    printf("\nPlease enter the two address values: ");
    scanf("%d",&a);
    scanf("%d",&b);
    //checking the input values are within the range of valid address.
    while((a<0 || a>255) || (b<0 || b>255)){
        fprintf(stderr,"%s","\nError: Please enter the values between [0-255]:");
        scanf("%d",&a);
        scanf("%d",&b);
    }
    struct address_t *tmp = head;
    int count = 0;
    while(tmp!=NULL){
        if(tmp->octet[0] == a && tmp->octet[1] == b){
            printf("\nAlias %d = %s",++count,tmp->alias);
        }
        tmp = tmp->next;
    }
    if(count == 0){
        fprintf(stderr,"%s","Error: Alias is not present. Please try again!\n");
    }
}

/*
Method to write the updated list to a new text file.
*/
void saveToFile(char fileName[input_size]){
    FILE *fp = fopen(strcat(fileName,".txt"),"w+");
    struct address_t *tmp = head;
    while(tmp!=NULL){
        fprintf(fp,"%d.%d.%d.%d %s\n",tmp->octet[0],tmp->octet[1],tmp->octet[2],tmp->octet[3],tmp->alias);
        tmp = tmp->next;
    }
    fclose(fp);
    printf("\nFile %s saved!",fileName);
}

/*
Method to display the linked list as an address-alias pair.
*/
void displayLinkedList(){
    if(head == NULL){
        fprintf(stderr,"%s","Error: No available record. List is Empty!");
        return;
    }
    struct address_t *tmp = head;
    int count = 0;
    while(tmp!=NULL){
        ++count;
        printf("address: %d.%d.%d.%d and Alias: %s",tmp->octet[0],tmp->octet[1],tmp->octet[2],tmp->octet[3], tmp->alias);
        printf("\n");
        tmp = tmp->next;
    }
    printf("\nTotal number of nodes in the list: %d", count);
}

/*
Method that will print the list of available options to the user.
*/
void displayMenu(){
    printf("\n\nPlease select an option[1-8]:");
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
and insert into linked list. Then it will prompt the user for an option from the available menu.
*/
int main(){
    //opening the text file containing addresses and alias.
    FILE *fptr = fopen("CS531_Inet.txt","r");
    if(fptr == NULL){
        fprintf(stderr, "%s", "Error! File cannot be opened.\n");
        exit(1);
    }
    char textFileLine[input_size];
    //reading from the text file line by line and adding the address and alias in the linked list.
    while (fscanf(fptr, "%[^\n] ", textFileLine) != EOF){
        addAddress(textFileLine);
    }
    fclose(fptr);  //closing the file after creating the linked list from it's data.
    int flag = 1;
    char userInput[input_size];
    while(flag){
        displayMenu(); //display the options to the user.
        int option;
        scanf("%d",&option);
        while(getchar()!='\n');
        switch(option){
            case 1: printf("\nPlease enter the address and the alias: ");
                    fgets(userInput, input_size, stdin);
                    //removing the new line character read by fgets.
                    if(userInput[strlen(userInput)-1]=='\n'){
                        userInput[strlen(userInput)-1] = '\0';
                    }
                    addAddress(userInput);
                    break;
            case 2: printf("\nPlease enter the alias to look up:");
                    fgets(userInput, alias_size, stdin);
                   //removing the new line character read by fgets.
                    if(userInput[strlen(userInput)-1]=='\n'){
                        userInput[strlen(userInput)-1] = '\0';
                    }
                    lookUpAddress(userInput);
                    break;
            case 3: printf("Please enter the alias to update the address:");
                    fgets(userInput, alias_size, stdin);
                    //removing the new line character read by fgets.
                    if(userInput[strlen(userInput)-1]=='\n'){
                        userInput[strlen(userInput)-1] = '\0';
                    }
                    updateAddress(userInput);
                    break;
            case 4: printf("\nPlease enter the alias to delete:");
                    fgets(userInput, alias_size, stdin);
                   //removing the new line character read by fgets.
                    if(userInput[strlen(userInput)-1]=='\n'){
                        userInput[strlen(userInput)-1] = '\0';
                    }
                    deleteAlias(userInput);
                    break;
            case 5: displayLinkedList();
                    break;
            case 6: displayAliasesForLocation();
                    break;
            case 7: printf("Please enter the file name to save aliases: ");
                    fgets(userInput,input_size,stdin);
                    //removing the new line character read by fgets.
                    if(userInput[strlen(userInput)-1]=='\n'){
                        userInput[strlen(userInput)-1] = '\0';
                    }
                    saveToFile(userInput);
                    break;
            case 8: flag = 0;
                    fprintf(stderr, "%s", "Exiting....!\n");
                    break;
            default: printf("\nEnter a valid option. Please try again!");
        }
    }
    return 0;
}
