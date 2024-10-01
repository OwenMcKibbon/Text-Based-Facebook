
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "a2_nodes.h"
#include "a2_functions.h"

/*
   Function that creates a new user and adds it to a sorted (ascending order) linked list at
   the proper sorted location. Return the head of the list.
*/
user_t *add_user(user_t *users, const char *username, int userid, const char *password)
{
    user_t *user_to_add = malloc(sizeof(user_t));
    assert(user_to_add != NULL);
    strcpy(user_to_add->username, username);
    user_to_add->userid = userid;
    strcpy(user_to_add->password, password);
    user_to_add->posts = NULL;
    user_to_add->friends = NULL;
    
    user_to_add->next = NULL;

    if (users == NULL)
    {
        user_to_add->next = users;
        return user_to_add;
    }

    for (user_t *check = users; check->next != NULL; check = check->next)
    {
        if (strcmp(check->username, username) == 0)
        {
            return NULL;
        }
    }

    if (users->next == NULL || users->userid < user_to_add->userid)
    {
        user_to_add->next = users->next;
        users->next = user_to_add;
        return users;
    }

    else 
    {
        user_t *curr = users;
        while (curr->next != NULL && user_to_add->userid > curr->next->userid)
        {
            curr = curr->next;
        }
        user_to_add->next = curr->next;
        curr->next = user_to_add;
        return users;
    }
}

/*
   Function that searches if the user is available in the database 
   Return a pointer to the user if found and NULL if not found.
*/
user_t *find_user(user_t *users, const char *username)
{
    user_t *curr = users;
    for (curr; curr != NULL; curr = curr->next)
    {
        if (strcmp(curr->username, username) == 0)
        {
            return curr;
        }
    }
    return NULL;
}


/*
   Function that creates a new friend's node.
   Return the newly created node.
*/
friend_t *create_friend(const char *username, int id)
{
    friend_t *friend_to_make = malloc(sizeof(friend_t));
    assert(friend_to_make != NULL);

    friend_to_make->id = id;
    strcpy(friend_to_make->username, username);
    friend_to_make->next = NULL;

    return friend_to_make;
}


/*
   Function that links a friend to a user. The friend's name should be added into
   a sorted (ascending order) linked list.
*/
void add_friend(user_t *user, const char *friend, int id)
{
    friend_t *friend_to_add = create_friend(friend, id);

    if (user->friends == NULL || id < user->friends->id)
    {
        friend_to_add->next = user->friends;
        user->friends = friend_to_add;
    }

    else
    {
        friend_t *curr = user->friends;

        while (curr->next != NULL && id > curr->next->id)
        {
            curr = curr->next;
        }
        friend_to_add->next = user->friends->next;
        user->friends->next = friend_to_add;
    }
}

/*
   Function that removes a friend from a user's friend list.
   Return true of the friend was deleted and false otherwise.
*/
_Bool delete_friend(user_t *user, char *friend_name)
{
    if (user == NULL || user->friends == NULL)
    {
        return false;
    }

    if (strcmp(user->friends->username, friend_name) == 0)
    {
        friend_t *friend_to_delete = user->friends;
        user->friends = user->friends->next;
        free(friend_to_delete);
        return true;
    }
    
    friend_t *curr = user->friends;
    while(curr->next != NULL && strcmp(user->friends->username, friend_name) != 0)
    {
        curr = curr->next;
    }
    if (curr->next == NULL)
    {
        return false;
    }

    friend_t *friend_to_delete = user->friends;
    user->friends = user->friends->next;
    free(friend_to_delete);
    return true;
}

/*
   Function that creates a new user's post.
   Return the newly created post.
*/
post_t *create_post(const char *text)
{
    post_t *post_to_create = malloc(sizeof(post_t));
    assert(post_to_create != NULL);
    strcpy(post_to_create->content, text);

    post_to_create->next = NULL;

    return post_to_create;
}

/*
   Function that adds a post to a user's timeline. New posts should be added following
   the stack convention (LIFO) (i.e., to the beginning of the Posts linked list).
*/
void add_post(user_t *user, const char *text)
{
    post_t *new_post = create_post(text);
    new_post->next = user->posts;
    user->posts = new_post;
}

/*
   Function that removes a post from a user's list of posts.
   Return true if the post was deleted and false otherwise.
*/
_Bool delete_post(user_t *user, int number)
{
    if (user == NULL || user->posts == NULL)
    {
        return false;
    }

    if (number == 1)
    {
        post_t *post_to_delete = user->posts;
        user->posts = user->posts->next;
        free(post_to_delete);
        return true;
    }
    int num = 1;
    post_t *curr = user->posts;
    while (curr != NULL && num < number - 1)
    {
        curr = curr->next;
        num++;
    } 

    if (curr == NULL || curr->next == NULL)
    {
        return false;
    }

    post_t *post_to_delete = user->posts;
    user->posts = user->posts->next;
    free(post_to_delete);
    return true;
}

/*
   Function that  displays a specific user's posts (All posts)
*/
void display_user_posts(user_t *user)
{
    int count = 1;
    printf("\n----------------------------------------");
    printf("\n\t%s posts\n", user->username);
    if (user->posts == NULL)
    {
        printf("\nNo posts available for %s.", user->username);
        printf("\n----------------------------------------");
        return;
    }
    for(post_t *curr = user->posts; curr != NULL; curr = curr->next)
    {
        printf("%d-", count);
        printf("%s\n", curr->content);
        count++;
    }
}


/*
   Function that displays a specific user's friends (All friends)
*/
void display_user_friends(user_t *user)
{
    printf("\nList of %s's friends: \n", user->username);
    int count = 1;

    friend_t *curr = user->friends;

    if (curr == NULL)
    {
        printf("No friends available for %s", user->username);
    }

    for (curr = user->friends; curr != NULL; curr = curr->next)
    {
        printf("\n%d- %s", count, curr->username);
        count++;
    }

}

/*
   Function that displays "number" of posts for a given user.
   After displaying "number" posts, it prompts if you want to display
   posts of the next "number" posts.
   If there are no more post or the user types “n” or “N”, the function returns. 
*/
void display_posts(user_t *users, int number)
{
    user_t *curr = users;

    for (int i = 0; i < 3; i ++)
    {
       int count = 1;
       post_t *currpost = curr->posts;
       while (currpost != NULL)
       {
            printf("%d- %s: %s\n", count, curr->username, currpost->content);
            count++;
            currpost = currpost->next;
       }

       if(curr != NULL)
       {
            curr = curr->next;
       }      

    }

    if(curr == NULL)
    {
        return;
    }

    if (curr->next != NULL)
    {
       char chose; 
       printf("Would you like to display next 3 user posts? (Y/N): ");
       scanf(" %c", &chose);
       if (chose == 'Y' || chose == 'y')
       {
            display_posts(users->next->next, number);
       }
       if (chose == 'N' || chose == 'n')
       {
        ;
       }
    }
}

/*
   Function that free all users from the database before quitting the application.
*/
void teardown(user_t *users)
{
    user_t *user_to_delete;
    while (users != NULL)
    {
        user_to_delete = users;
        users = users->next;
        free(user_to_delete->posts);
        free(user_to_delete->friends);
        free(user_to_delete);
    }
}

/*
   Function that prints the main menu with a list of options for the user to choose from
*/
void print_menu()
{
    printf("\n*****************************");
    printf("\n\t\tMAIN MENU:");
    printf("\n*****************************");
    printf("1. Register a new User\n");
    printf("2. Login with existing user's information\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        char *token3 = strtok(NULL, ",");
        users = add_user(users, token, atoi(token3), token2);
        char *username = token;
        int id = atoi(token3);

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token, rand());
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
