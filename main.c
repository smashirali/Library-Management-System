#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



struct Book
{
    int id;
    char title[50];
    char author[50];
    struct tm date;
    int isIssued; // 0=available , 1=issued
};

int addBook();
int viewAllBooks();
int searchBook();
int EditBooks();
void ChangeBoth(); 
void ChangeAuthor();
void ChangeTitle();
int deleteBook();
int IssueBook();
int CheckAvailiblity();
int ViewIssueBooks();
int ReturnBook();

int main()
{ 
    int choice;
    do
    {
        printf("   \nLibrary Managment System\n");
        printf("1: Add Book \n");
        printf("2: View All Books \n");
        printf("3: Edit Book \n");
        printf("4: Delete Book \n");
        printf("5: Issue Book \n");
        printf("6: View Issue Books \n");
         printf("7: Return Book \n");
        printf("8: Search Book \n");
        printf("0: Exit Program \n");
        printf("Enter your choice: \n");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            viewAllBooks();
            break;
        case 3:
             EditBooks(); 
            break;
        case 4:
            deleteBook(); 
            break;
        case 5:
            IssueBook(); 
            break;
        case 6:
            ViewIssueBooks();
            break;
         case 7:
            ReturnBook();
            break;           
        case 8:
            searchBook();
            break;
        case 0:
            printf("Exiting....");
            exit(0);
            break;
        default:
            printf("Invalid Choice \n");
        }
        printf("\nPress Any Key To Continue...\n");
        getch();
    } while (choice != 0);

    return 0;
}

int addBook()
{
    FILE *file;
    file = fopen("library.txt", "ab");
    struct Book book;
    srand(time(0)); // Seed the random number generator with the current time
    srand(time(0));

    // Generate a random ID number between a range, for example, 1000 to 9999
    int random_id = rand() % 9000 + 1000; // % 9000 ensures it's in the range 0-8999, then +1000 makes it 1000-9999
    book.id = random_id;
    // Clear the input buffer to consume the newline character
    // getchar(); // This will consume the leftover newline

    printf("Enter Book Title: ");
    // scanf(" %s", &book.title);
    fgets(book.title, sizeof(book.title), stdin); // Using fgets to handle spaces

    // Remove newline character from title
    size_t len = strlen(book.title);
    if (len > 0 && book.title[len - 1] == '\n')
    {
        book.title[len - 1] = '\0';
    }


    printf("Enter Book Author: ");
    fgets(book.author, sizeof(book.author), stdin); // Using fgets to handle spaces


    // Remove newline character from author
    len = strlen(book.author);
    if (len > 0 && book.author[len - 1] == '\n')
    {
        book.author[len - 1] = '\0';
    }

    // Get the current date
    time_t t = time(NULL);
    book.date = *localtime(&t);
    book.isIssued = 0;
    fwrite(&book, sizeof(struct Book), 1, file);
    fclose(file);
    printf("Book Added Successfully");
    return 0;
}

int viewAllBooks()
{
    FILE *file = fopen("library.txt", "rb");
    struct Book book;

    if (file == NULL)
    {
        printf("No Book found\n");
        return 0;
    }

    printf("\n%-10s %-30s %-30s %s \n", "ID", "Title", "Author", "Status");
    while (fread(&book, sizeof(struct Book), 1, file))
    {
        printf("\n%-10d %-30s %-30s %s", book.id, book.title, book.author, book.isIssued ? "Issued" : "Available");
    }
    fclose(file);
    return 0;
}

int searchBook()
{
    FILE *file = fopen("library.txt", "rb");
    struct Book book;
    char bookTitle[50];

    if (file == NULL)
    {
        printf("No Books Found \n");
        return -1;
    }
    //getchar();
    printf("\nEnter the title of the book to search: ");
    fgets(bookTitle, sizeof(bookTitle), stdin);

    // Remove newline character from author
    size_t len = strlen(bookTitle);
    if (len > 0 && bookTitle[len - 1] == '\n')
    {
        bookTitle[len - 1] = '\0';
    }

    while (fread(&book, sizeof(struct Book), 1, file))
    {
        if (strcmp(book.title, bookTitle) == 0)
        {
            printf("\nBook Found:\n");
            printf("ID: %d\n", book.id);
            printf("Title: %s\n", book.title);
            printf("Author: %s\n", book.author);
            printf("Status: %s\n", book.isIssued ? "Issued" : "Available");
            break;
        }
        else
        {
            printf("\nBook with title %s not found", bookTitle);
            break;
        }
    }
    fclose(file);
    return 0;
}

int EditBooks()
{
    int BookId;
    int inputChoice;

    FILE *file = fopen("library.txt", "rb+");
    struct Book book;
    int found = 0; // flag to track if book was found

    if (file == NULL)
    {
        printf("No Books Found \n");
        return -1;
    }

    // Ask for the book ID to edit
    printf("\nEnter the ID of the book to edit: ");
    scanf("%d", &BookId);

    // Iterate through the file to find the book by ID
    while (fread(&book, sizeof(struct Book), 1, file))
    {
        if (book.id == BookId)
        {
            // Book found, confirm the details
            printf("\nBook Found:\n");
            printf("ID: %d\n", book.id);
            printf("Title: %s\n", book.title);
            printf("Author: %s\n", book.author);
            found = 1;

            // Ask user which field to edit
            printf("\nWhat would you like to edit?\n");
            printf("1. Title\n");
            printf("2. Author\n");
            printf("3. Both Title and Author\n");
            printf("Enter your choice: ");
            scanf("%d", &inputChoice);
            getchar(); // to consume the newline left by scanf

            // Based on the choice, call the relevant edit function
            switch (inputChoice)
            {
            case 1:
                ChangeTitle(&book); // Pass the book to edit its title
                break;
            case 2:
                ChangeAuthor(&book); // Pass the book to edit its author
                break;
            case 3:
                ChangeBoth(&book); // Pass the book to edit both fields
                break;
            default:
                printf("Invalid Choice!\n");
                fclose(file);
                return 0;
            }

            // Move the file pointer to the correct position before rewriting the record
            fseek(file, -sizeof(struct Book), SEEK_CUR);

            // Write the updated book back to the file
            fwrite(&book, sizeof(struct Book), 1, file);
            printf("Book information updated successfully!\n");

            break;
        }
    }

    if (!found)
    {
        printf("Book with ID %d not found.\n", BookId);
    }

    fclose(file);
    return 0;
}

    void ChangeTitle (struct Book *book)
{
    printf("Enter new Title: ");
    fgets(book->title, sizeof(book->title), stdin);
    
    // Remove newline character
    size_t len = strlen(book->title);
    if (len > 0 && book->title[len - 1] == '\n')
    {
        book->title[len - 1] = '\0';
    }
    printf("Title updated to: %s\n", book->title);
}   

    void ChangeAuthor (struct Book *book)
{
    printf("Enter new Author: ");
    fgets(book->author, sizeof(book->author), stdin);
    
    // Remove newline character
    size_t len = strlen(book->author);
    if (len > 0 && book->author[len - 1] == '\n')
    {
        book->author[len - 1] = '\0';
    }
    printf("Author updated to: %s\n", book->author);
}

    void ChangeBoth (struct Book *book)
{
    // Update both title and author
    printf("Enter new Title: ");
    fgets(book->title, sizeof(book->title), stdin);
    
    // Remove newline character from title
    size_t len = strlen(book->title);
    if (len > 0 && book->title[len - 1] == '\n')
    {
        book->title[len - 1] = '\0';
    }

    printf("Enter new Author: ");
    fgets(book->author, sizeof(book->author), stdin);
    
    // Remove newline character from author
    len = strlen(book->author);
    if (len > 0 && book->author[len - 1] == '\n')
    {
        book->author[len - 1] = '\0';
    }

    printf("Title updated to: %s\n", book->title);
    printf("Author updated to: %s\n", book->author);
}

int deleteBook()
{
    int bookId;
    int found = 0;

    // Open the original file in read mode
    FILE *file = fopen("library.txt", "rb");
    if (file == NULL)
    {
        printf("No books found to delete.\n");
        return -1;
    }

    // Open a temporary file in write mode
    FILE *tempFile = fopen("temp.txt", "wb");
    if (tempFile == NULL)
    {
        printf("Unable to create temporary file.\n");
        fclose(file);
        return -1;
    }

    printf("Enter the ID of the book you want to delete: ");
    scanf("%d", &bookId);

    struct Book book;

    // Read each book from the original file
    while (fread(&book, sizeof(struct Book), 1, file))
    {
        // If the book ID matches the one to be deleted, skip it
        if (book.id == bookId)
        {
            found = 1;
            printf("Book with ID %d has been deleted successfully.\n", bookId);
            continue;
        }

        // Write all other books to the temporary file
        fwrite(&book, sizeof(struct Book), 1, tempFile);
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Check if the book was found and deleted
    if (!found)
    {
        printf("No book found with ID %d.\n", bookId);
        // Optionally, remove the temporary file if no changes were made
        remove("temp.txt");
        return -1;
    }

    // Remove the original file and rename the temp file to the original filename
    remove("library.txt");
    rename("temp.txt", "library.txt");

    return 0;
}

int IssueBook()
{
    int BookId, found = 0, availabilityCheck;
    FILE *file = fopen("library.txt", "rb");  // Open in read mode
    FILE *tempFile = fopen("temp.txt", "wb"); // Open temp file in write mode for updating
    struct Book book;

    if (file == NULL)
    {
        printf("No Books Found\n");
        return -1;
    }

    if (tempFile == NULL)
    {
        printf("Unable to create temporary file.\n");
        fclose(file);
        return -1;
    }

    // Ask for the book ID to issue
    printf("Enter the ID of the book to issue: ");
    scanf("%d", &BookId);

    // Iterate through the file to find the book by ID
    while (fread(&book, sizeof(struct Book), 1, file))
    {
        if (book.id == BookId)
        {
            // Book found, confirm the details
            printf("\nBook Found:\n");
            printf("ID: %d\n", book.id);
            printf("Title: %s\n", book.title);
            printf("Author: %s\n", book.author);
            found = 1;

            if (book.isIssued==1)
            {
                printf("Sorry, this book is already issued.\n");
            }

            else
            {
                // Book is available, proceed with issuing
                printf("The book is available. Issuing the book now...\n");
                book.isIssued = 1; // Update the status to issued

                // Here, you can also add the date of issue if you want.
                time_t t = time(NULL);
                book.date = *localtime(&t); // Update date to current date
                 printf("Book issued successfully!\n");
            }
        }
         // Write the book (whether updated or not) to the temp file
        fwrite(&book, sizeof(struct Book), 1, tempFile);
    } 

    // If no book with the given ID is found
    if (!found)
    {
        printf("No book found with ID %d.\n", BookId);
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated temp file
    if (found)
    {
        remove("library.txt");  // Delete the original file
        rename("temp.txt", "library.txt");  // Rename temp file to library.txt
    }
    else
    {
        remove("temp.txt"); // Remove temp file if no book was issued
    }

    return 0;

}

int ViewIssueBooks()
{
    FILE *file = fopen("library.txt", "rb");
    struct Book book;

    if (file == NULL)
    {
        printf("No Book found\n");
        return 0;
    }

    printf("\n%-10s %-30s %-30s %s \n", "ID", "Title", "Author", "Status");

    while (fread(&book, sizeof(struct Book), 1, file))
    {
        if (book.isIssued == 1)
        {
        printf("\n%-10d %-30s %-30s %s", book.id, book.title, book.author, book.isIssued ? "Issued" : "Available");
        }
        else
        {
            printf("No Issued Book Found");
        }
    }

    fclose(file);
    return 0;
}

int ReturnBook()
{
    int BookId, found = 0, availabilityCheck;
    FILE *file = fopen("library.txt", "rb");  // Open in read mode
    FILE *tempFile = fopen("temp.txt", "wb"); // Open temp file in write mode for updating
    struct Book book;

    if (file == NULL)
    {
        printf("No Books Found\n");
        return -1;
    }

    if (tempFile == NULL)
    {
        printf("Unable to create temporary file.\n");
        fclose(file);
        return -1;
    }

    // Ask for the book ID to issue
    printf("Enter the ID of the book to Return: ");
    scanf("%d", &BookId);

    // Iterate through the file to find the book by ID
    while (fread(&book, sizeof(struct Book), 1, file))
    {
        if (book.id == BookId)
        {
            // Book found, confirm the details
            printf("\nBook Found:\n");
            printf("ID: %d\n", book.id);
            printf("Title: %s\n", book.title);
            printf("Author: %s\n", book.author);
            found = 1;

            if (book.isIssued==1)
            {
                book.isIssued =0;
                // Here, you can also add the date of issue if you want.
                time_t t = time(NULL);
                book.date = *localtime(&t); // Update date to current date
                printf("Book Successfully Returned...\n");
            }

            else
            {
                printf("The book is already Available...\n");
            }
        }
         // Write the book (whether updated or not) to the temp file
        fwrite(&book, sizeof(struct Book), 1, tempFile);
    } 

    // If no book with the given ID is found
    if (!found)
    {
        printf("No book found with ID %d.\n", BookId);
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated temp file
    if (found)
    {
        remove("library.txt");  // Delete the original file
        rename("temp.txt", "library.txt");  // Rename temp file to library.txt
    }
    else
    {
        remove("temp.txt"); // Remove temp file if no book was issued
    }

    return 0;
}