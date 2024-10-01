#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_nodes.h"
#include "a2_functions.h"

int main()
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }
    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);
 
    while (true)
    {
        int choice;
        print_menu();
        scanf("%d", &choice);

        while (choice < 1 || choice > 6)
        {
            printf("\n Invalid choice. Please try again. ");
            print_menu();
            scanf("%d", &choice);
        }

        if (choice == 1)
        {
            const char username[30]; 
            const char password[15];
            int userid = 0;
            printf("\nEnter a username: ");
            scanf("%s", username);
            printf("\nEnter a userid: ");
            scanf("%d", &userid);
            printf("Enter an up to 15 characters password: ");
            scanf("%s", password);
            add_user(users, username, userid, password);
            printf("\n***** User Added! *****");
        }

        if (choice == 2)
        {
            const char usernamelog[30];
            const char passwordlog[15];
            printf("\nPlease enter your credentials");
            printf("\nUsername: ");
            scanf("%s", usernamelog);
            printf("\nPassword: ");
            scanf("%s", passwordlog);
            user_t *p = find_user(users, usernamelog);
            if (p == NULL){
                printf("\n------------------------------------------");
                printf("\n\t\tUser not found.");
                printf("\n------------------------------------------");
                break;
            }
            if (strcmp(p->password, passwordlog) != 0){
                printf("\nIncorrect password!");
                break;
            }
            else
            {
                while (true)
                {
                    printf("\n********************************\n\tWelcome %s\n********************************", p->username);
                    int selection;
                    printf("1. Manage profile (change password)\n");
                    printf("2. Manage posts (display/add/remove)\n");
                    printf("3. Manage friends (display/add/remove)\n");
                    printf("4. Display All Posts\n");
                    printf("5. Logout\n");
                    printf("\n\nYour choice: ");
                    scanf("%d", &selection);

                    if (selection == 1)
                    {
                        const char passupdate[15];
                        printf("\nEnter a new password that is up to 15 characters: ");
                        scanf("%s", passupdate);
                        strcpy(p->password, passupdate);
                    }

                    if (selection == 2)
                    {
                        while (true)
                        {
                            int chosen;
                            display_user_posts(p);
                            printf("\n1. Add a new post");
                            printf("\n2. Remove a post");
                            printf("\n3. Return to main menu");
                            printf("\n\nYour Choice: ");
                            scanf("%d", &chosen);
                            if (chosen < 1 || chosen > 3){
                                printf("\nInvalid choice. Please try again.");
                                char wrong;
                                scanf("%c", &wrong);
                            }
                            if (chosen == 1){
                                const char text[250];
                                printf("Enter your post content: ");
                                scanf(" %[^\n]s", text);
                                add_post(p, text);
                            }
                            if (chosen == 2){
                                int postnum;
                                printf("Which post do you want to delete? ");
                                scanf("%d", &postnum);
                                delete_post(p, postnum);
                            }
                            if (chosen == 3){
                                break;
                            }
                        }   
                    }

                    if (selection == 3)
                    {
                        if (p != NULL){
                            while (true){
                                int choose;
                                printf("\n-------------------------------------------");
                                printf("\n\t%s's friends",p->username);
                                printf("\n-------------------------------------------");
                                printf("\n1. Display all user's friends");
                                printf("\n2. Add a new friend");
                                printf("\n3. Delete a friend");
                                printf("\n4. Display a friend's posts");
                                printf("\n5. Return to main menu");
                                printf("\nYour choice: ");
                                scanf("%d", &choose);

                                if (choose < 1 || choose > 4){
                                    printf("\nInvalid choice. Please try again.");
                                    char wronger;
                                    scanf("%c", &wronger);
                                }

                                if (choose == 1){
                                    display_user_friends(p);
                                }

                                if (choose == 2){
                                    const char friendname[30];
                                    int friendId = 0;
                                    printf("\nEnter a new friend's name: ");
                                    scanf("%s", friendname);
                                    printf("\nEnter the friend's userid: ");
                                    scanf("%d", &friendId);
                                    if (find_user(users, friendname) == NULL){
                                        printf("User does not exist.");
                                        break;
                                    }
                                    add_friend(p, friendname, friendId);
                                    printf("\nFriend added to the list.");
                                }

                                if (choose == 3){
                                    char friendtodelete[30];
                                    display_user_friends(p);
                                    printf("\nEnter a friend's name to delete: ");
                                    scanf("%s", friendtodelete);
                                    _Bool check = delete_friend(p, friendtodelete);
                                    while (check == 0){
                                        printf("\nInvalid friend's name");
                                        break;
                                    }
                                    display_user_friends(p);

                                }

                                if (choose == 4)
                                {
                                    char friendposts[30];
                                    printf("\nChoose a friend to display their posts: ");
                                    display_user_friends(p);
                                    scanf("%s", friendposts);
                                    user_t *f = find_user(users, friendposts);
                                    display_user_posts(f);
                                }

                                if (choose == 5)
                                {
                                    break;
                                }
                            }
                        }
                    }

                    if (selection == 4)
                    {
                        display_all_posts(users);
                    }

                    if (selection == 5)
                    {
                        break;
                    }
                }
            }
        }

        if (choice == 6)
        {
            printf("\n*********************************************\n");
            printf("\tThank you for using Text-Based Facebook\n");
            printf("\t\tGoodBye!\n");
            printf("*********************************************");
            teardown(users);
            break;
        }
    }
}