#include <stdio.h>
#include <stdlib.h>
#include "main.h"

void handleCustomerMenu(SkiResort* resort) {
    int customerChoice;
    do {
        printf("\nCustomer Menu:\n");
        printf("1. Make a reservation\n");
        printf("2. View resort data\n");
        printf("Please choose an action (1-2): ");
        scanf("%d", &customerChoice);

        switch (customerChoice) {
        case 1:
            addReservation(resort);
            break;
        case 2:
            printSkiMap(resort->theMap, resort->theMap->manager);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }

        printf("Would you like to perform another action? (1 = Yes, 0 = No): ");
        scanf("%d", &customerChoice);

    } while (customerChoice == 1);

    int saveChanges;
    printf("Do you want to save the changes? (1 = Yes, 0 = No): ");
    scanf("%d", &saveChanges);

    if (saveChanges == 1) {
        // save all changes to regular file 
        FILE* filePointer1;
        filePointer1 = fopen(REGULAR_FILE_NAME, "w");
        if (filePointer1 == NULL)
        {
            printf("Error opening Resort file , cheek if it exist\n");
            return;
        }
        if (!saveSkiResortToFile(resort, filePointer1))
            return;
        printf("Changes saved successfully to regular file\n");
        // Close the file when done
        fclose(filePointer1);

        filePointer1 = fopen(BINARY_FILE_NAME, "wb");
        // save all changes to Binary file 
        filePointer1 = fopen(BINARY_FILE_NAME, "wb");
        if (filePointer1 == NULL)
        {
            printf("Error opening Resort Binary file , cheek if it exist\n");
            return;
        }
        if (!saveSkiResortToBinaryFile(resort, filePointer1))
            return;
        printf("Changes saved successfully to Binary file\n");
        // Close the Binary file when done
        fclose(filePointer1);
    }
}

void handleManagerMenu(SkiResort* resort) {
    int managerChoice;
    int modificationChoice;
    do {
        printf("\nManager Menu:\n");
        printf("1. View existing reservations\n");
        printf("2. View site data\n");
        printf("3. View employee database\n");
        printf("4. Perform site modifications\n");
        printf("5. Print system data\n");
        printf("6. for Exit or any other number\n");
        printf("Please choose an action (1-5): ");
        scanf("%d", &managerChoice);
        switch (managerChoice) {
        case 1:
            sortReservations(resort);
            printReservationArr(resort->allReservations, resort->reservationCount);
            break; 
        case 2:
            printSkiMap(resort->theMap, resort->theMap->manager);
            break;
        case 3:
            printWorkersArr(resort->workerArr, resort->workerCount);
            break;
        case 4:
            do {
                printf("\nSite Modifications Menu:\n");
                printf("1. Add station\n");
                printf("2. Add worker\n");
                printf("3. Add route\n");
                printf("4. for Exit or any other number\n");
                printf("Please choose an action (1-3): ");
                scanf("%d", &modificationChoice);

                switch (modificationChoice) {
                case 1:
                    initStation(resort); 
                    break;
                case 2:
                    addWorker(resort);
                    break;
                case 3:
                    addRoute(resort->theMap, resort->theMap->manager);
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
                    break;
                }

                printf("Would you like to perform another site modification? (1 = Yes, 0 = No): ");
                scanf("%d", &modificationChoice);

            } while (modificationChoice == 1);

            break;
        case 5:
            printf("Printing system data...\n");
            printSkiResort(resort);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }

        printf("Would you like to perform another action? (1 = Yes, 0 = No): ");
        scanf("%d", &managerChoice);

    } while (managerChoice == 1);

    int saveChanges;
    printf("Do you want to save the changes? (1 = Yes, 0 = No): ");
    scanf("%d", &saveChanges);

    if (saveChanges == 1) {
        // save all changes to regular file 
        FILE* filePointer;
        filePointer = fopen(REGULAR_FILE_NAME, "w");
        if (filePointer == NULL)
        {
            printf("Error opening Resort file , cheek if it exist\n");
            return;
        }
        if (!saveSkiResortToFile(resort, filePointer))
            return;
        printf("Changes saved successfully to regular file\n");
        // Close the file when done
        fclose(filePointer);

       
        // save all changes to Binary file 
        filePointer = fopen(BINARY_FILE_NAME, "wb");
        if (filePointer == NULL)
        {
            printf("Error opening Resort Binary file , cheek if it exist\n");
            return;
        }
        if (!saveSkiResortToBinaryFile(resort, filePointer))
            return;
        printf("Changes saved successfully to Binary file\n");
        // Close the Binary file when done
        fclose(filePointer);
    }
}

int main() {
    // Declare variables
    int userRole;
    char* resortName = "AfratResort";
    SkiResort* resort = malloc(sizeof(SkiResort));
    if (resort == NULL) {
        printf("Error allocate memory for SkiResort\n");
        return 0;
    }
    // load all files to SkiResort
    // Initialize ski resort
    initSkiResort(resort,resortName);
    // Declare a FILE pointer
    FILE* skiResortFile;
    int fileType;
    int rightAnswer = 0;
    do {
         printf("From which type do do want load all Resort data ?\n 1)Regular file\n 2)Binary file\n");
         scanf("%d", &fileType);
         if (fileType > 0 && fileType < 3)
             rightAnswer = 1;
    } while (!rightAnswer);
   
    // Open a file for loading files (creates a new file or overwrites existing file)

    switch (fileType) {
    case 1:
        skiResortFile = fopen(REGULAR_FILE_NAME, "r");
        if (skiResortFile == NULL)
        {
            printf("Error opening Resort file , cheek if it exist or it can by empty\n");
            break;
        }
        if (!loadSkiResortFromFile(resort, skiResortFile))
            break;
        // Close the file when done
        fclose(skiResortFile);
        break;

    case 2:
        skiResortFile = fopen(BINARY_FILE_NAME, "rb");
        if (skiResortFile == NULL)
        {
            printf("Error opening Resort Binary file , cheek if it exist or it can by empty\n");
            break;
        }
        if (!loadSkiResortFromBinaryFile(resort, skiResortFile))
            break;
        // Close the file when done
        fclose(skiResortFile);
        break;

    default:
        printf("Invalid number. Please try again.\n");
        break;
    }
    

    printf("Welcome to the ski resort %s management system!\n", resortName);
    int exitAnswer = 0;
    do
    {
        // Prompt user for role
        printf("Are you a customer or a manager? \n 1)  Customer\n 2)  Manager \n");
        scanf("%d", &userRole);
        
        switch (userRole)
        {
           case 1:
                // Customer operations
                handleCustomerMenu(resort);
                exitAnswer = 1;
                 break;

           case 2:
               // Manager operations
               handleManagerMenu(resort);
               exitAnswer = 1;
               break;

          default:
               printf("Invalid role. Please try again.\n");
               break;
        }

    } while (!exitAnswer);

    

    printf("Thank you for using the ski resort management system!\n");

    // Free memory
    /*freeSkiResort(resort);*/

    return 1;
}