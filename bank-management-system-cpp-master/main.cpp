#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cctype>
#include <iomanip>
#include <string>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

using namespace std;

// Function declarations
int chkacc(int a);
void clearScreen();
void gotoxy(int a, int b);
void setConsoleColor();
void resetConsoleColor();
int getch_portable();
void pauseEnter();
void pauseKey();
void design(int x, int y);

// Portable single-key input
int getch_portable() {
#ifdef _WIN32
    return getch();
#else
    termios oldt{}, newt{};
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) return getchar();
    newt = oldt;
    newt.c_lflag &= static_cast<unsigned>(~(ICANON | ECHO));
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) return getchar();
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

// Cleanly wait for Enter key without ghost keystrokes
void pauseEnter() {
#ifndef _WIN32
    tcflush(STDIN_FILENO, TCIFLUSH);
#endif
    while (true) {
        int x = getch_portable();
        if (x == 13 || x == 10) break;
    }
}

// Cleanly wait for any key press
void pauseKey() {
#ifndef _WIN32
    tcflush(STDIN_FILENO, TCIFLUSH);
#endif
    getch_portable();
}

void gotoxy(int a, int b) {
#ifdef _WIN32
    COORD coord;
    coord.X = static_cast<SHORT>(a);
    coord.Y = static_cast<SHORT>(b);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    cout << "\033[" << (b + 1) << ";" << (a + 1) << "H" << flush;
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H" << flush;
#endif
}

void setConsoleColor() {
#ifdef _WIN32
    system("color f4");
#else
    cout << "\033[47;31m" << flush;
#endif
}

void resetConsoleColor() {
#ifndef _WIN32
    cout << "\033[0m" << flush;
#endif
}

void design(int x, int y) {
    char c = '-';
    if (y == 177 || y == 205) c = '=';
    else if (y == 196 || y == 45) c = '-';
    else if (y >= 32 && y <= 126) c = static_cast<char>(y);
    else c = '=';

    for (int i = 0; i <= x; i++)
        cout << c;
    cout << flush;
}

char* strupr_custom(char* s) {
    for (char* p = s; *p != '\0'; ++p) {
        *p = static_cast<char>(toupper(static_cast<unsigned char>(*p)));
    }
    return s;
}

class record {
public:
    char name[25];
    int account;
    char phone[15];
    char address[25];
    char email[35];
    char citiz[20];
    double blnc;
    char UserID[10];
};

class bms {
protected:
    char id[20];
    char password[15];
private:
    int m;
public:
    bms() : m(0) {
        memset(id, 0, sizeof(id));
        memset(password, 0, sizeof(password));
    }

    void start() {
        setConsoleColor();
        while (true) {
            clearScreen();
            gotoxy(26, 4);
            design(15, 177);
            cout << " WELCOME TO TBC BANKING SYSTEM ";
            design(15, 177);
            gotoxy(50, 8);
            cout << "ACCOUNT TYPE";
            gotoxy(44, 10);
            cout << "[1] . ADMINISTRATOR ";
            gotoxy(44, 11);
            cout << "[2] . USER ";
            gotoxy(44, 12);
            cout << "[3] . EXIT ";
            gotoxy(44, 15);
            cout << "Enter Your Choice [1-3] .... ";
            if (!(cin >> m)) {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (m == 3) {
                resetConsoleColor();
                clearScreen();
                exit(0);
            }

            if (m == 1 || m == 2) {
                clearScreen();
                if (authenticate()) {
                    if (m == 1)
                        menu();
                    else
                        transactions();
                }
            }
        }
    }

    bool authenticate() {
        for (int attempt = 1; attempt <= 3; ++attempt) {
            clearScreen();
            gotoxy(26, 4);
            design(15, 177);
            cout << " WELCOME TO TBC BANKING SYSTEM ";
            design(15, 177);
            login();

            if (verify() == 1) {
                return true;
            }

            clearScreen();
            gotoxy(35, 16);
            cout << "Incorrect username or password.";
            gotoxy(35, 18);
            cout << "Attempts remaining: " << (3 - attempt);
            if (attempt < 3) {
                gotoxy(35, 20);
                cout << "Press any key to try again...";
                pauseKey();
            }
        }

        clearScreen();
        gotoxy(35, 12);
        cout << "Too many failed attempts. Access denied.";
        gotoxy(35, 14);
        cout << "Press any key to exit...";
        pauseKey();
        resetConsoleColor();
        exit(0);
        return false;
    }

    void login() {
        gotoxy(44, 10);
        cout << "Enter The Username : " << flush;
        cin >> setw(sizeof(id)) >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        gotoxy(44, 12);
        cout << "Enter The Password : " << flush;
        int i = 0;
        while (true) {
            int ch = getch_portable();
            if (ch == 13 || ch == 10) {
                if (i == 0) continue;
                break;
            }
            if (ch == 8 || ch == 127) {
                if (i > 0) {
                    --i;
                    cout << "\b \b" << flush;
                }
            } else if (ch >= 32 && ch <= 126 && i < static_cast<int>(sizeof(password)) - 1) {
                password[i++] = static_cast<char>(ch);
                cout << '*' << flush;
            }
        }
        password[i] = '\0';
    }

    int verify() {
        bool match = false;
        if (m == 1 && strcmp(id, "admin") == 0 && strcmp(password, "admin") == 0) match = true;
        if (m == 2 && strcmp(id, "user") == 0 && strcmp(password, "user") == 0) match = true;

        if (match) {
            gotoxy(35, 17);
            design(48, 45);
            gotoxy(38, 16);
            cout << "You Have Successfully Logged In : \"" << id << "\"";
            time_t t;
            time(&t);
            gotoxy(39, 18);
            cout << "Logged In Time : " << ctime(&t);
            gotoxy(44, 22);
            cout << "Press any key to continue .... ";
            pauseKey();
            return 1;
        }
        return 0;
    }

    void menu() {
        int choice;
        while (true) {
            clearScreen();
            gotoxy(28, 4);
            design(20, 177);
            cout << " WELCOME TO MAIN MENU ";
            design(20, 177);
            gotoxy(44, 8);
            cout << "[1] . View Customer Accounts";
            gotoxy(44, 9);
            cout << "[2] . Customer Account Registration";
            gotoxy(44, 10);
            cout << "[3] . Edit Customer Account";
            gotoxy(44, 11);
            cout << "[4] . Delete Customer Account";
            gotoxy(44, 12);
            cout << "[5] . Search Customer Account";
            gotoxy(44, 13);
            cout << "[6] . Transaction";
            gotoxy(44, 14);
            cout << "[7] . Log Out !!! ";
            gotoxy(44, 15);
            cout << "[8] . About US ";
            gotoxy(43, 19);
            cout << "Please Enter Your Choice [1-8] : ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            clearScreen();
            switch (choice) {
                case 1: view(); break;
                case 2: add(); break;
                case 3: edit(); break;
                case 4: del(); break;
                case 5: srch(); break;
                case 6: transactions(); break;
                case 7: menuexit(); return;
                case 8: about(); break;
                default: break;
            }
        }
    }

    void view();
    void add();
    void edit();
    void del();
    void srch();
    void search_acc();
    void search_name();
    void transactions();
    void chkblnc();
    void deposit();
    void withdrawl();
    void transfer();
    void menuexit();
    void about();
};

int chkacc(int a) {
    record rec;
    ifstream f("record.bin", ios::in | ios::binary);
    if (!f.is_open()) return 0;

    while (f.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.account == a) {
            f.close();
            return 1;
        }
    }
    f.close();
    return 0;
}

void bms::view() {
    int i = 7;
    record rec;
    ifstream f("record.bin", ios::in | ios::binary);

    gotoxy(25, 3);
    design(25, 177);
    cout << " CUSTOMERS LIST ";
    design(25, 177);
    gotoxy(5, 5);
    cout << "A/C No.";
    gotoxy(13, 5);
    cout << "Account Name";
    gotoxy(34, 5);
    cout << "UserID";
    gotoxy(49, 5);
    cout << "Email Address";
    gotoxy(85, 5);
    cout << "Phone No.";
    gotoxy(99, 5);
    cout << "Balance";
    gotoxy(5, 6);
    design(104, 205);

    int count = 0;
    if (f.is_open()) {
        while (f.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
            count++;
            gotoxy(5, i);
            cout << rec.account;
            gotoxy(13, i);
            cout << rec.name;
            gotoxy(34, i);
            for (int r = 0; r < 10; r++) cout << static_cast<int>(rec.UserID[r]);
            gotoxy(49, i);
            cout << rec.email;
            gotoxy(85, i);
            cout << rec.phone;
            gotoxy(99, i);
            cout << fixed << setprecision(2) << rec.blnc << "$";
            i++;
        }
        f.close();
    }

    if (count == 0) {
        gotoxy(33, 9);
        cout << "[ No customer accounts registered yet! ]";
        gotoxy(28, 11);
        cout << "Please use option [2] in main menu to register an account.";
        i = 12;
    }

    gotoxy(35, i + 3);
    cout << "Press [Enter] to return back to main menu... " << flush;
    pauseEnter();
}

void bms::add() {
    char c;
    record rec;
    ofstream f("record.bin", ios::app | ios::binary);

    do {
        clearScreen();
        gotoxy(24, 4);
        design(20, 177);
        cout << " CUSTOMER ACCOUNT REGISTRATION ";
        design(20, 177);

        gotoxy(36, 8);
        cout << "[1] . Enter Your Name           : ";
        cin >> setw(sizeof(rec.name)) >> rec.name;

        gotoxy(36, 9);
        cout << "[2] . Enter Your Account Number : ";
        if (!(cin >> rec.account)) {
            cin.clear(); cin.ignore(1000, '\n');
            continue;
        }

        if (chkacc(rec.account) == 1) {
            gotoxy(36, 11);
            cout << "Account already exists! Use a unique number.";
            pauseKey();
            f.close();
            return;
        }

        gotoxy(36, 10);
        cout << "[3] . Enter Your Phone Number   : ";
        cin >> setw(sizeof(rec.phone)) >> rec.phone;

        gotoxy(36, 11);
        cout << "[4] . Enter Your Address        : ";
        cin >> setw(sizeof(rec.address)) >> rec.address;

        gotoxy(36, 12);
        cout << "[5] . Enter Your E-mail         : ";
        cin >> setw(sizeof(rec.email)) >> rec.email;

        gotoxy(36, 13);
        cout << "[6] . Enter Your Citizenship No.: ";
        cin >> setw(sizeof(rec.citiz)) >> rec.citiz;

        gotoxy(36, 14);
        cout << "[7] . Enter Amount To Deposit   : $";
        if (!(cin >> rec.blnc) || rec.blnc < 0) {
            rec.blnc = 0;
            cin.clear(); cin.ignore(1000, '\n');
        }

        srand(static_cast<unsigned>(time(nullptr)));
        for (int r = 0; r < 10; r++) rec.UserID[r] = static_cast<char>(rand() % 10);

        f.write(reinterpret_cast<char*>(&rec), sizeof(rec));

        gotoxy(38, 17);
        cout << "CUSTOMER ACCOUNT REGISTRATION SUCCESSFUL";
        gotoxy(36, 19);
        cout << "Do You Want To Add Another Record ? (Y/N) : ";
        cin >> c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (c == 'Y' || c == 'y');

    f.close();
    gotoxy(38, 22);
    cout << "Press any key to return back to main menu. ";
    pauseKey();
}

void bms::edit() {
    int a;
    while (true) {
        clearScreen();
        gotoxy(23, 4);
        design(25, 177);
        cout << " EDIT CUSTOMER ACCOUNT ";
        design(25, 177);
        gotoxy(40, 7);
        cout << "Enter Account Number To Edit (0 to Cancel): ";
        if (!(cin >> a) || a == 0) return;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (chkacc(a) == 0) {
            gotoxy(45, 12);
            cout << "Account Doesn't Exist! Press any key...";
            pauseKey();
            continue;
        }
        break;
    }

    record rec;
    ifstream f1("record.bin", ios::in | ios::binary);
    ofstream f2("new.bin", ios::out | ios::binary | ios::trunc);

    while (f1.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.account != a) {
            f2.write(reinterpret_cast<char*>(&rec), sizeof(rec));
        } else {
            clearScreen();
            gotoxy(28, 4);
            design(25, 177);
            cout << " ENTER NEW DETAILS ";
            design(25, 177);
            gotoxy(31, 8);
            cout << "[1] . Enter Your Name           : ";
            cin >> setw(sizeof(rec.name)) >> rec.name;
            gotoxy(31, 10);
            cout << "[2] . Enter Your Phone Number   : ";
            cin >> setw(sizeof(rec.phone)) >> rec.phone;
            gotoxy(31, 12);
            cout << "[3] . Enter Your Address        : ";
            cin >> setw(sizeof(rec.address)) >> rec.address;
            gotoxy(31, 14);
            cout << "[4] . Enter Your E-mail         : ";
            cin >> setw(sizeof(rec.email)) >> rec.email;
            gotoxy(31, 16);
            cout << "[5] . Enter Your Citizenship No : ";
            cin >> setw(sizeof(rec.citiz)) >> rec.citiz;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            f2.write(reinterpret_cast<char*>(&rec), sizeof(rec));
        }
    }
    f1.close();
    f2.close();

    remove("record.bin");
    rename("new.bin", "record.bin");

    gotoxy(35, 20);
    cout << "CUSTOMER ACCOUNT UPDATE SUCCESSFUL";
    gotoxy(35, 22);
    cout << "Press any key to return back to main menu. ";
    pauseKey();
}

void bms::del() {
    int a;
    while (true) {
        clearScreen();
        gotoxy(23, 4);
        design(25, 177);
        cout << " DELETE CUSTOMER ACCOUNT ";
        design(25, 177);
        gotoxy(40, 9);
        cout << "Enter Account Number To Delete (0 to Cancel): ";
        if (!(cin >> a) || a == 0) return;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (chkacc(a) == 0) {
            gotoxy(45, 14);
            cout << "Account Doesn't Exist! Press any key...";
            pauseKey();
            continue;
        }
        break;
    }

    record rec;
    ifstream f1("record.bin", ios::in | ios::binary);
    ofstream f2("new.bin", ios::out | ios::binary | ios::trunc);

    while (f1.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.account != a) {
            f2.write(reinterpret_cast<char*>(&rec), sizeof(rec));
        }
    }
    f1.close();
    f2.close();

    remove("record.bin");
    rename("new.bin", "record.bin");

    gotoxy(40, 15);
    cout << "CUSTOMER ACCOUNT DELETED SUCCESSFULLY";
    gotoxy(40, 18);
    cout << "Press any key to return back to main menu. ";
    pauseKey();
}

void bms::srch() {
    int a;
    clearScreen();
    gotoxy(28, 4);
    design(25, 177);
    cout << " SEARCH MENU ";
    design(25, 177);
    gotoxy(49, 10);
    cout << "[1] . Search By Account ";
    gotoxy(49, 12);
    cout << "[2] . Search By Name ";
    gotoxy(47, 16);
    cout << "Enter Your Choice [1-2] : ";
    if (cin >> a) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (a == 1) search_acc();
        else if (a == 2) search_name();
    } else {
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void bms::search_acc() {
    int a;
    clearScreen();
    gotoxy(23, 4);
    design(25, 177);
    cout << " SEARCH CUSTOMER ACCOUNT ";
    design(25, 177);
    gotoxy(40, 6);
    cout << "Enter Account Number To Search : ";
    if (!(cin >> a)) { cin.clear(); cin.ignore(1000, '\n'); return; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    record rec;
    bool found = false;
    ifstream f("record.bin", ios::in | ios::binary);

    if (f.is_open()) {
        while (f.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
            if (rec.account == a) {
                found = true;
                gotoxy(40, 9);
                cout << "Detail Information of " << rec.name;
                gotoxy(28, 10);
                design(65, 205);
                gotoxy(35, 11);
                cout << "[1] . Account Number : " << rec.account;
                gotoxy(35, 12);
                cout << "[2] . Name           : " << rec.name;
                gotoxy(35, 13);
                cout << "[3] . UserID         : ";
                for (int r = 0; r < 10; r++) cout << static_cast<int>(rec.UserID[r]);
                gotoxy(35, 14);
                cout << "[4] . Phone Number   : " << rec.phone;
                gotoxy(35, 15);
                cout << "[5] . Address        : " << rec.address;
                gotoxy(35, 16);
                cout << "[6] . E-mail         : " << rec.email;
                gotoxy(35, 17);
                cout << "[7] . Citizenship No : " << rec.citiz;
                gotoxy(35, 18);
                cout << "[8] . Current Balance: $" << fixed << setprecision(2) << rec.blnc;
                gotoxy(35, 19);
                cout << "[9] . Status         : " << (rec.blnc > 25 ? "Active" : "Inactive");
                gotoxy(35, 20);
                double monthlyInterest = rec.blnc * (0.06 / 12.0);
                cout << "[10]. Monthly Interest: $" << fixed << setprecision(2) << monthlyInterest;
                break;
            }
        }
        f.close();
    }

    if (!found) {
        gotoxy(45, 14);
        cout << "Account Doesn't Exist.";
    }

    gotoxy(38, 23);
    cout << "Press [ENTER] to return back. ";
    pauseEnter();
}

void bms::search_name() {
    char nam[30];
    clearScreen();
    gotoxy(23, 4);
    design(25, 177);
    cout << " SEARCH CUSTOMER ACCOUNT ";
    design(25, 177);
    gotoxy(40, 6);
    cout << "Enter Name To Search : ";
    cin >> setw(sizeof(nam)) >> nam;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    char searchNamUpper[30];
    strncpy(searchNamUpper, nam, sizeof(searchNamUpper));
    searchNamUpper[sizeof(searchNamUpper) - 1] = '\0';
    strupr_custom(searchNamUpper);

    record rec;
    bool found = false;
    ifstream f("record.bin", ios::in | ios::binary);

    if (f.is_open()) {
        while (f.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
            char recNameUpper[25];
            strncpy(recNameUpper, rec.name, sizeof(recNameUpper));
            recNameUpper[sizeof(recNameUpper) - 1] = '\0';
            strupr_custom(recNameUpper);

            if (strcmp(recNameUpper, searchNamUpper) == 0) {
                found = true;
                gotoxy(40, 9);
                cout << "Detail Information of " << rec.name;
                gotoxy(28, 10);
                design(65, 205);
                gotoxy(35, 11);
                cout << "[1] . Account Number : " << rec.account;
                gotoxy(35, 12);
                cout << "[2] . Name           : " << rec.name;
                gotoxy(35, 13);
                cout << "[3] . UserID         : ";
                for (int r = 0; r < 10; r++) cout << static_cast<int>(rec.UserID[r]);
                gotoxy(35, 14);
                cout << "[4] . Phone Number   : " << rec.phone;
                gotoxy(35, 15);
                cout << "[5] . Address        : " << rec.address;
                gotoxy(35, 16);
                cout << "[6] . E-mail         : " << rec.email;
                gotoxy(35, 17);
                cout << "[7] . Citizenship No : " << rec.citiz;
                gotoxy(35, 18);
                cout << "[8] . Current Balance: $" << fixed << setprecision(2) << rec.blnc;
                gotoxy(35, 19);
                cout << "[9] . Status         : " << (rec.blnc > 25 ? "Active" : "Inactive");
                gotoxy(35, 20);
                double monthlyInterest = rec.blnc * (0.06 / 12.0);
                cout << "[10]. Monthly Interest: $" << fixed << setprecision(2) << monthlyInterest;
                break;
            }
        }
        f.close();
    }

    if (!found) {
        gotoxy(45, 14);
        cout << "Account Doesn't Exist.";
    }

    gotoxy(38, 23);
    cout << "Press [ENTER] to return back. ";
    pauseEnter();
}

void bms::transactions() {
    int a;
    while (true) {
        clearScreen();
        gotoxy(25, 4);
        design(25, 177);
        cout << " TRANSACTION MENU ";
        design(25, 177);
        gotoxy(49, 9);
        cout << "[1] . Balance Inquiry";
        gotoxy(49, 10);
        cout << "[2] . Cash Deposit";
        gotoxy(49, 11);
        cout << "[3] . Cash Withdrawal";
        gotoxy(49, 12);
        cout << "[4] . Fund Transfer";
        gotoxy(49, 13);
        cout << (m == 1 ? "[5] . Main Menu" : "[5] . Exit");

        gotoxy(45, 17);
        cout << "Please Enter Your Choice [1-5] : ";
        if (!(cin >> a)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (a) {
            case 1: chkblnc(); break;
            case 2: deposit(); break;
            case 3: withdrawl(); break;
            case 4: transfer(); break;
            case 5: return;
            default: break;
        }
    }
}

void bms::chkblnc() {
    int a;
    clearScreen();
    gotoxy(27, 4);
    design(25, 177);
    cout << " BALANCE INQUIRY ";
    design(25, 177);
    gotoxy(42, 11);
    cout << "Enter Your Account Number : ";
    if (!(cin >> a)) { cin.clear(); cin.ignore(1000, '\n'); return; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    record rec;
    bool found = false;
    ifstream f("record.bin", ios::in | ios::binary);
    if (f.is_open()) {
        while (f.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
            if (rec.account == a) {
                found = true;
                gotoxy(40, 14);
                cout << "Account Holder : " << rec.name;
                gotoxy(40, 16);
                cout << "Your Balance is: $" << fixed << setprecision(2) << rec.blnc;
                break;
            }
        }
        f.close();
    }

    if (!found) {
        gotoxy(45, 14);
        cout << "Account Doesn't Exist.";
    }

    gotoxy(42, 20);
    cout << "Press any key to continue... ";
    pauseKey();
}

void bms::deposit() {
    int a;
    double b;
    clearScreen();
    gotoxy(29, 4);
    design(25, 177);
    cout << " CASH DEPOSIT ";
    design(25, 177);
    gotoxy(42, 10);
    cout << "Enter Your Account Number : ";
    if (!(cin >> a)) { cin.clear(); cin.ignore(1000, '\n'); return; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (chkacc(a) == 0) {
        gotoxy(45, 14);
        cout << "Account Doesn't Exist.";
        gotoxy(45, 16);
        cout << "Press any key to continue...";
        pauseKey();
        return;
    }

    gotoxy(42, 12);
    cout << "Enter Amount To Deposit   : $";
    if (!(cin >> b) || b <= 0) {
        gotoxy(42, 15);
        cout << "Invalid Amount! Must be greater than zero.";
        pauseKey();
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    record rec;
    ifstream f1("record.bin", ios::in | ios::binary);
    ofstream f2("new.bin", ios::out | ios::binary | ios::trunc);

    while (f1.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.account == a) {
            rec.blnc += b;
        }
        f2.write(reinterpret_cast<char*>(&rec), sizeof(rec));
    }
    f1.close();
    f2.close();

    remove("record.bin");
    rename("new.bin", "record.bin");

    gotoxy(44, 16);
    cout << "CASH DEPOSIT SUCCESSFUL!";
    gotoxy(44, 18);
    cout << "Press any key to continue...";
    pauseKey();
}

void bms::withdrawl() {
    int a;
    double b;
    clearScreen();
    gotoxy(25, 4);
    design(25, 177);
    cout << " CASH WITHDRAWAL ";
    design(25, 177);
    gotoxy(42, 10);
    cout << "Enter Your Account Number : ";
    if (!(cin >> a)) { cin.clear(); cin.ignore(1000, '\n'); return; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (chkacc(a) == 0) {
        gotoxy(45, 14);
        cout << "Account Doesn't Exist.";
        gotoxy(45, 16);
        cout << "Press any key to continue...";
        pauseKey();
        return;
    }

    gotoxy(42, 12);
    cout << "Enter Amount To Withdraw : $";
    if (!(cin >> b) || b <= 0) {
        gotoxy(42, 15);
        cout << "Invalid Amount! Must be greater than zero.";
        pauseKey();
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    record rec;
    bool sufficient = true;
    ifstream f1("record.bin", ios::in | ios::binary);
    ofstream f2("new.bin", ios::out | ios::binary | ios::trunc);

    while (f1.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.account == a) {
            if (rec.blnc >= b) {
                rec.blnc -= b;
                if (rec.blnc > 100) {
                    rec.blnc -= 1.0;
                }
            } else {
                sufficient = false;
            }
        }
        f2.write(reinterpret_cast<char*>(&rec), sizeof(rec));
    }
    f1.close();
    f2.close();

    if (!sufficient) {
        remove("new.bin");
        gotoxy(40, 16);
        cout << "Insufficient funds for withdrawal!";
    } else {
        remove("record.bin");
        rename("new.bin", "record.bin");
        gotoxy(42, 16);
        cout << "CASH WITHDRAWAL SUCCESSFUL!";
    }

    gotoxy(42, 19);
    cout << "Press any key to continue... ";
    pauseKey();
}

void bms::transfer() {
    int senderAcc, receiverAcc;
    double amount;
    clearScreen();
    gotoxy(27, 4);
    design(25, 177);
    cout << " CASH TRANSFER ";
    design(25, 177);

    gotoxy(38, 9);
    cout << "Enter Sender Account Number   : ";
    if (!(cin >> senderAcc) || chkacc(senderAcc) == 0) {
        gotoxy(40, 12);
        cout << "Sender Account Does Not Exist!";
        pauseKey();
        return;
    }

    gotoxy(38, 11);
    cout << "Enter Recipient Account Number: ";
    if (!(cin >> receiverAcc) || chkacc(receiverAcc) == 0) {
        gotoxy(40, 14);
        cout << "Recipient Account Does Not Exist!";
        pauseKey();
        return;
    }

    if (senderAcc == receiverAcc) {
        gotoxy(38, 14);
        cout << "Cannot transfer to the same account!";
        pauseKey();
        return;
    }

    gotoxy(38, 13);
    cout << "Enter Transfer Amount         : $";
    if (!(cin >> amount) || amount <= 0) {
        gotoxy(40, 16);
        cout << "Invalid Amount! Must be greater than zero.";
        pauseKey();
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    record rec;
    bool hasBalance = false;
    ifstream f1("record.bin", ios::in | ios::binary);
    while (f1.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.account == senderAcc && rec.blnc >= amount) {
            hasBalance = true;
            break;
        }
    }
    f1.close();

    if (!hasBalance) {
        gotoxy(40, 16);
        cout << "Transfer failed: Insufficient balance!";
        pauseKey();
        return;
    }

    f1.open("record.bin", ios::in | ios::binary);
    ofstream f2("new.bin", ios::out | ios::binary | ios::trunc);

    while (f1.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.account == senderAcc) {
            rec.blnc -= amount;
        } else if (rec.account == receiverAcc) {
            rec.blnc += amount;
        }
        f2.write(reinterpret_cast<char*>(&rec), sizeof(rec));
    }
    f1.close();
    f2.close();

    remove("record.bin");
    rename("new.bin", "record.bin");

    gotoxy(42, 17);
    cout << "CASH TRANSFER SUCCESSFUL!";
    gotoxy(40, 20);
    cout << "Press any key to continue... ";
    pauseKey();
}

void bms::menuexit() {
    clearScreen();
    gotoxy(30, 4);
    design(25, 177);
    cout << " THANK YOU ";
    design(25, 177);
    gotoxy(42, 11);
    cout << "USER            :: " << id;
    time_t t;
    time(&t);
    gotoxy(42, 13);
    cout << "Logged Out Time :: " << ctime(&t);
    gotoxy(40, 17);
    cout << "Press any key to exit...";
    pauseKey();
    resetConsoleColor();
    clearScreen();
    exit(0);
}

void bms::about() {
    clearScreen();
    gotoxy(30, 4);
    design(25, 177);
    cout << " ABOUT US ";
    design(25, 177);
    gotoxy(14, 8);
    cout << "Bank Management System Project built using C++ and POSIX standards.";
    gotoxy(45, 11);
    cout << "Members of Team Warriors:";
    gotoxy(28, 12);
    design(60, 205);
    gotoxy(48, 14);
    cout << "[1] . Aman Khadka";
    gotoxy(48, 15);
    cout << "[2] . Grusha Bhattarai";
    gotoxy(48, 16);
    cout << "[3] . Roshni Chettri";
    gotoxy(48, 17);
    cout << "[4] . Tisha Manandhar";
    gotoxy(48, 18);
    cout << "[5] . Rika Regmi";
    gotoxy(35, 21);
    cout << "Press [ENTER] to return back to main menu. ";
    pauseEnter();
}

int main() {
    bms app;
    app.start();
    return 0;
}