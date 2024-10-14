#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <ctime>
#include <iomanip>

using namespace std;

class Book
{
public:
    string name;
    string author;
    string serialNo;
    bool isAvailable;

    Book(string n, string a, string s)
        : name(n), author(a), serialNo(s), isAvailable(true) {}
};

class User
{
public:
    string name;
    string membershipNumber;

    User(string n, string m)
        : name(n), membershipNumber(m) {}
};

class Membership
{
public:
    string membershipNumber;
    int duration; // Duration in months (6, 12, or 24)

    Membership(string m, int d)
        : membershipNumber(m), duration(d) {}
};

class Transaction
{
public:
    string userName;
    string serialNo;
    string transactionType; // "issue" or "return"
    time_t transactionDate;

    Transaction(string u, string s, string type)
        : userName(u), serialNo(s), transactionType(type)
    {
        transactionDate = time(nullptr); // Store the current time
    }
};

class Library
{
private:
    vector<Book> books;
    vector<User> users;
    vector<Membership> memberships;
    vector<Transaction> transactions;

    const double finePerDay = 2.0; // Define fine per day for late return

public:
    void addBook(const string &name, const string &author, const string &serialNo)
    {
        books.emplace_back(name, author, serialNo);
        cout << "Book added: " << name << " by " << author << endl;
    }

    void issueBook(const string &serialNo, const string &userName)
    {
        for (auto &book : books)
        {
            if (book.serialNo == serialNo && book.isAvailable)
            {
                book.isAvailable = false;
                transactions.emplace_back(userName, serialNo, "issue");
                cout << "Book issued to " << userName << endl;
                return;
            }
        }
        cout << "Book is not available for issue." << endl;
    }

    void returnBook(const string &serialNo, const string &userName)
    {
        for (auto &book : books)
        {
            if (book.serialNo == serialNo && !book.isAvailable)
            {
                book.isAvailable = true;
                transactions.emplace_back(userName, serialNo, "return");
                cout << "Book returned: " << book.name << endl;
                return;
            }
        }
        cout << "Book not found or already available." << endl;
    }

    void addUser(const string &name, const string &membershipNumber)
    {
        users.emplace_back(name, membershipNumber);
        cout << "User added: " << name << endl;
    }

    void addMembership(const string &membershipNumber, int duration)
    {
        memberships.emplace_back(membershipNumber, duration);
        cout << "Membership added for: " << membershipNumber << " with duration " << duration << " months" << endl;
    }

    void displayBooks()
    {
        cout << "\nAvailable Books:\n";
        for (const auto &book : books)
        {
            if (book.isAvailable)
            {
                cout << "Name: " << book.name << ", Author: " << book.author << ", Serial No: " << book.serialNo << endl;
            }
        }
    }

    bool validateMembership(const string &membershipNumber)
    {
        for (const auto &member : memberships)
        {
            if (member.membershipNumber == membershipNumber)
            {
                return true;
            }
        }
        return false;
    }

    bool isBookAvailable(const string &serialNo)
    {
        for (const auto &book : books)
        {
            if (book.serialNo == serialNo)
            {
                return book.isAvailable;
            }
        }
        return false;
    }

    void displayUsers()
    {
        cout << "\nUsers:\n";
        for (const auto &user : users)
        {
            cout << "Name: " << user.name << ", Membership No: " << user.membershipNumber << endl;
        }
    }

    void displayTransactions()
    {
        cout << "\nTransactions:\n";
        for (const auto &transaction : transactions)
        {
            tm *transactionTime = localtime(&transaction.transactionDate);
            cout << "User: " << transaction.userName << ", Book Serial: " << transaction.serialNo
                 << ", Type: " << transaction.transactionType
                 << ", Date: " << put_time(transactionTime, "%Y-%m-%d %H:%M:%S") << endl;
        }
    }

    double payFine(const string &serialNo, const string &userName)
    {
        // Assuming a book is overdue if returned more than 14 days after being issued
        time_t now = time(nullptr);
        for (const auto &transaction : transactions)
        {
            if (transaction.serialNo == serialNo && transaction.userName == userName && transaction.transactionType == "issue")
            {
                double daysOverdue = difftime(now, transaction.transactionDate) / (60 * 60 * 24) - 14;
                if (daysOverdue > 0)
                {
                    double fine = daysOverdue * finePerDay;
                    cout << "Fine to be paid: $" << fine << endl;
                    return fine;
                }
                else
                {
                    cout << "No fine. Book is not overdue." << endl;
                    return 0.0;
                }
            }
        }
        cout << "No matching transaction found." << endl;
        return 0.0;
    }
};

class LibrarySystem
{
private:
    Library library;

public:
    void start()
    {
        int choice;
        do
        {
            cout << "\nLibrary Management System\n";
            cout << "1. Add Book\n";
            cout << "2. Issue Book\n";
            cout << "3. Return Book\n";
            cout << "4. Add User\n";
            cout << "5. Add Membership\n";
            cout << "6. Display Books\n";
            cout << "7. Display Users\n";
            cout << "8. Display Transactions\n";
            cout << "9. Pay Fine\n";
            cout << "0. Exit\n";
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                cout << "Enter Book Name: ";
                string name, author, serialNo;
                cin.ignore();
                getline(cin, name);
                cout << "Enter Author Name: ";
                getline(cin, author);
                cout << "Enter Serial No: ";
                getline(cin, serialNo);
                library.addBook(name, author, serialNo);
                break;
            }
            case 2:
            {
                string serialNo, userName;
                cout << "Enter Serial No: ";
                cin.ignore();
                getline(cin, serialNo);
                cout << "Enter User Name: ";
                getline(cin, userName);
                if (library.isBookAvailable(serialNo))
                {
                    library.issueBook(serialNo, userName);
                }
                else
                {
                    cout << "Book is not available for issue." << endl;
                }
                break;
            }
            case 3:
            {
                string serialNo, userName;
                cout << "Enter Serial No: ";
                cin.ignore();
                getline(cin, serialNo);
                cout << "Enter User Name: ";
                getline(cin, userName);
                library.returnBook(serialNo, userName);
                break;
            }
            case 4:
            {
                string name, membershipNumber;
                cout << "Enter User Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Membership Number: ";
                getline(cin, membershipNumber);
                library.addUser(name, membershipNumber);
                break;
            }
            case 5:
            {
                string membershipNumber;
                int duration;
                cout << "Enter Membership Number: ";
                cin.ignore();
                getline(cin, membershipNumber);
                cout << "Enter Duration (6, 12, or 24 months): ";
                cin >> duration;
                if (duration == 6 || duration == 12 || duration == 24)
                {
                    library.addMembership(membershipNumber, duration);
                }
                else
                {
                    cout << "Invalid duration. Please enter 6, 12, or 24." << endl;
                }
                break;
            }
            case 6:
                library.displayBooks();
                break;
            case 7:
                library.displayUsers();
                break;
            case 8:
                library.displayTransactions();
                break;
            case 9:
            {
                string serialNo, userName;
                cout << "Enter Serial No: ";
                cin.ignore();
                getline(cin, serialNo);
                cout << "Enter User Name: ";
                getline(cin, userName);
                library.payFine(serialNo, userName);
                break;
            }
            case 0:
                cout << "Exiting system." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        } while (choice != 0);
    }
};

int main()
{
    LibrarySystem system;
    system.start();
    return 0;
}
