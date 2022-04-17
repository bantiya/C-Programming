#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>

#define buffer_size 27
#define number_of_strings 10

/**
Method to check if length of the string is in the range [1,25].
*/
bool checkStringLength(char* test){

    //check if string has length greater than 25 or if string is empty.
    if(strlen(test)>(buffer_size-2)){
        while ((getchar()) != '\n'); //flushing the input stream before taking new input if input length is not valid.
        fprintf(stderr, "%s", "Error: String has length greater than 25! Please re-enter:\n");
        return false;
    }
    else if(test[0] == '\0'){
        fprintf(stderr, "%s", "Error: String is empty! Please re-enter:\n");
        return false;
    }
    return true;
}

/**
Method to check if the string contains some invalid characters.
*/
bool checkInvalidCharacters(char* test){
    bool flag = true;
    for(int i=0;i<strlen(test);i++){
            if(test[i] == '#' || test[i] == '$' || test[i] == '%' || test[i] == '^' || test[i] == '@' || test[i] == '(' || test[i] == ')'){
                fprintf(stderr, "%s %c %s","Error:", test[i] , "is an illegal character - please re-enter:\n");
                flag = false;
            }
    }
    return flag;
}

/**
Method to check if duplicate string is entered.
*/
bool checkDuplicateString(char **sample, char *test, int n){
    bool flag = true;
    for(int i=0;i<n;i++){
        if(strcmp(sample[i],test)== 0){
            fprintf(stderr, "%s","Error: Duplicate String, please re-enter:\n");
            flag = false;
        }
    }
    return flag;
}

/**
Method to sort the strings in ascending order of ascii values.
*/
void sortArray(char **sample){
    for(int i=0;i<number_of_strings;i++){
        char *key = sample[i];
        int j = i-1;
        while(j>=0 && strcmp(sample[j],key)>0){
            sample[j+1] = sample[j];
            j--;
        }
        sample[j+1] = key;
    }
}

/**
Method to print the array in ascending order.
*/
void printSortedArrayAscending(char **test){
    for(int i=0;i<number_of_strings;i++){
        printf("%s",test[i]);
    }
}

/**
Method to print the array in descending order.
*/
void printSortedArrayDescending(char **test){
    for(int i=number_of_strings-1;i>=0;i--){
        printf("%s",test[i]);
    }
}

/**
Method to check if user entered valid specification for sorting. 'A': ascending, 'D': descending.
*/
bool isValidSortOrder(char sortOrder){
    if(sortOrder == 'A' || sortOrder == 'D'){
        return true;
    }
    fprintf(stderr, "%s","\nError: Please enter valid sorting order: ");
    return false;
}

/**
The main method. User will input 10 strings that will be validated.
Then strings will be sorted according to ascii values.
User will input the desired sorting order.
*/
int main() {
    char **sample;
    printf("Enter 10 character strings: \n");
    sample = (char**)malloc(number_of_strings*sizeof(char*)); //allocating memory for rows for storing 10 strings.
    for(int i=0;i<number_of_strings;i++){
        printf("Enter String %d:",i+1);
        sample[i] = (char*)malloc((buffer_size-1)*sizeof(char));//allocating memory for each string with length = 25.
        fgets(sample[i],buffer_size,stdin);
        //removing the new line character read by fgets.
        if(sample[i][strlen(sample[i])-1]=='\n'){
            sample[i][strlen(sample[i])-1] = '\0';
        }
        bool flag = false;
        while(!flag){
            flag = checkStringLength(sample[i]) && checkInvalidCharacters(sample[i]) && checkDuplicateString(sample, sample[i], i);
            if(flag == false){
                printf("Enter String %d:",i+1);
                fgets(sample[i],buffer_size,stdin);
                //removing the new line character read by fgets.
                if(sample[i][strlen(sample[i])-1]=='\n'){
                    sample[i][strlen(sample[i])-1] = '\0';
                }
            }
        }
    }
    sortArray(sample); //sorting the strings based on ascii values: lexicographically.
    char sortOrder;
    printf("Print character strings in Ascending or Descending order: ");
    scanf("%c",&sortOrder); //user will input the desired sorting order.
    printf("\n");

    while(!isValidSortOrder(sortOrder)){
            scanf("%c",&sortOrder);
    }

    //using switch to print the sorted strings in either ascending or descending order.
    switch(sortOrder){
        case 'A' : printSortedArrayAscending(sample);
                   break;
        case 'B' : printSortedArrayDescending(sample);
                   break;
        default:  printSortedArrayAscending(sample);
    }

    printf("\nString with lowest ascii value: %s",sample[0]);
    printf("String with highest ascii value: %s",sample[number_of_strings-1]);
    free(sample);
    return 0;
}
