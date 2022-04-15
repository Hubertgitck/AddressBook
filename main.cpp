#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <conio.h>
#include <cstdio>
#include <fstream>

using namespace std;

struct Users {
    int usersId;
    string userName, password;
};

struct Friends {
    int id, usersId;
    string name, surname, email, phoneNumber, address;
};

int readFriendsFile (vector <Friends> &friendsDatabase) {
    int numberOfLines = 0;
    string idStringToBeConverted;
    ifstream inFile("AddressBook.txt");

    numberOfLines = count(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>(), '\n');

    inFile.seekg(0);

    for (int i = 0; i < numberOfLines; i++) {
        friendsDatabase.push_back(Friends{});

        getline(inFile,idStringToBeConverted,'|');
        friendsDatabase[i].id = stoi(idStringToBeConverted);

        getline(inFile,idStringToBeConverted,'|');
        friendsDatabase[i].usersId = stoi(idStringToBeConverted);

        getline(inFile,friendsDatabase[i].name,'|');
        getline(inFile,friendsDatabase[i].surname,'|');
        getline(inFile,friendsDatabase[i].email,'|');
        getline(inFile,friendsDatabase[i].phoneNumber,'|');
        getline(inFile,friendsDatabase[i].address,'|');
        inFile.ignore();
    }
    inFile.close();
    return numberOfLines;
}

int readUsersFile (vector <Users> &usersDatabase) {
    int numberOfLines = 0;
    string idStringToBeConverted;
    ifstream inFile("UsersDatabase.txt");

    numberOfLines = count(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>(), '\n');

    inFile.seekg(0);

    for (int i = 0; i < numberOfLines; i++) {
        usersDatabase.push_back(Users{});

        getline(inFile,idStringToBeConverted,'|');
        usersDatabase[i].usersId = stoi(idStringToBeConverted);

        getline(inFile,usersDatabase[i].userName,'|');
        getline(inFile,usersDatabase[i].password,'|');
        inFile.ignore();
    }
    inFile.close();
    return numberOfLines;
}

int registration(vector <Users> usersDatabase, int numberOfUsers)
{
    Users temporary = {} ;

    if (usersDatabase.size() == 0)
        temporary.usersId = 1;
    else{
        auto lastIdIterator = usersDatabase.end();
        --lastIdIterator;
        temporary.usersId = lastIdIterator->usersId + 1;
    }

    cout << "Podaj nazwe uzytkownika: ";
    cin >> temporary.userName;
    int i = 0;

    while (i < numberOfUsers)
    {
        if (usersDatabase[i].userName == temporary.userName)
        {
            cout << "Taki uzytkownik istnieje. Wpisz Nazwe uzytkownika: ";
            cin >> temporary.userName;
            i = 0;
        }
        else
            i++;
    }
    cout << "Podaj haslo: ";
    cin >> temporary.password;

    usersDatabase.push_back(temporary);

    cout << "Konto zalozone" << '\n';
    Sleep(1000);

    ofstream outFile;
    outFile.open("UsersDatabase.txt", ios::out|ios::app);
    if (outFile.good() == true) {
        outFile << temporary.usersId << "|";
        outFile << temporary.userName << "|";
        outFile << temporary.password << "|";
        outFile << '\n';
        outFile.close();
    }
    return numberOfUsers+1;
}

int login(vector <Users> usersDatabase, int numberOfUsers)
{
    string loginUserName, loginPassword;

    cout << "Podaj nazwe uzytkownika: ";
    cin >> loginUserName;
    int i = 0;

    while (i < numberOfUsers)
    {
        if (usersDatabase[i].userName == loginUserName)
        {
            for (int tries = 0; tries < 3; tries++)
            {
                cout << "Podaj haslo. Pozostalo prob " << 3 - tries << ": ";
                cin >> loginPassword;
                if (usersDatabase[i].password == loginPassword)
                {
                    cout << "Zalogowales sie!" << '\n';
                    Sleep(1000);
                    return usersDatabase[i].usersId;
                }

            }
            cout << "podales 3 razy bledne haslo. Poczekaj 3 sekundy przed kolejna proba" << '\n';
            Sleep(3000);
            return 0;
        }
        i++;
    }
    cout << "Nie ma uzytkownika z takim loginem" << '\n';
    Sleep(1500);
    return 0;
}






void overWriteExistingFile (const vector <Friends> friendsDatabase){

    ofstream users;
    users.open("AddressBook.txt", ios::out | ios::trunc);

    for (int i = 0; i < friendsDatabase.size(); i++){
        if (users.good() == true) {
        users << friendsDatabase[i].id << "|";
        users << friendsDatabase[i].usersId << "|";
        users << friendsDatabase[i].name << "|";
        users << friendsDatabase[i].surname << "|";
        users << friendsDatabase[i].email << "|" ;
        users << friendsDatabase[i].phoneNumber << "|";
        users << friendsDatabase[i].address << "|";
        users << '\n';
        }
    }
    users.close();
}

void editFriendsDatabaseMenu (vector <Friends> friendsDatabase){
    int idOfFriendToEdit = 0;
    char select;
    cout << "Podaj ID uzytkownika do edycji: ";
    cin >> idOfFriendToEdit;

    auto editIterator = find_if(friendsDatabase.begin(),friendsDatabase.end(),[idOfFriendToEdit](const Friends& p) {
    return p.id == idOfFriendToEdit;});

    Sleep(500);
    while (1){
        system("cls");
        cout << "##########EDYCJA##########" << '\n';
        cout << "1. Imie" << '\n';
        cout << "2. Nazwisko" << '\n';
        cout << "3. Nr. Telefonu" << '\n';
        cout << "4. E-mail" << '\n';
        cout << "5. Adres" << '\n';
        cout << "6. Powrot do menu glownego" << '\n';


        cin >> select;
        cin.ignore();

        switch (select) {
        case '1':{
            string newName;
            cout << "Podaj nowe imie: ";
            cin >> newName;
            editIterator->name = newName;
            cout << "Imie uzytkownika o ID: " << idOfFriendToEdit << " zostalo edytowane. ";
            overWriteExistingFile(friendsDatabase);
            Sleep(2000);
            break;
            }

        case '2':{
            string newSurname;
            cout << "Podaj nowe nazwisko: ";
            cin >> newSurname;
            editIterator->surname = newSurname;
            cout << "Nazwisko uzytkownika o ID: " << idOfFriendToEdit << " zostalo edytowane. ";
            overWriteExistingFile(friendsDatabase);
            Sleep(2000);
            break;
            }

        case '3':{
               string newPhoneNumber;
            cout << "Podaj nowy numer telefonu: ";
            cin >> newPhoneNumber;
            editIterator->phoneNumber = newPhoneNumber;
            cout << "Nr. telefona uzytkownika o ID: " << idOfFriendToEdit << " zostal edytowany. ";
            overWriteExistingFile(friendsDatabase);
            Sleep(2000);
            break;
            }

        case '4':{
            string newEmail;
            cout << "Podaj nowy e-mail: ";
            cin >> newEmail;
            editIterator->email = newEmail;
            cout << "E-mail uzytkownika o ID: " << idOfFriendToEdit << " zostal edytowany. ";
            overWriteExistingFile(friendsDatabase);
            Sleep(2000);
            break;
            }

        case '5':{
            string newAddress;
            cout << "Podaj nowy adres: ";
            cin.ignore();
            getline(cin, newAddress);
            editIterator->address = newAddress;
            cout << "Adres uzytkownika o ID: " << idOfFriendToEdit << " zostal edytowany. ";
            overWriteExistingFile(friendsDatabase);
            Sleep(2000);
            break;
            }

        case '6':{
            goto exitEdditingMenu;
            overWriteExistingFile(friendsDatabase);
            break;
            }

        default:
            cout << "Wybierz poprawna opcje!"<< '\n';
            overWriteExistingFile(friendsDatabase);
            Sleep(2000);
            break;
        }
    }
    exitEdditingMenu:;
}


void deleteFriendFromFile (int lineToDelete){

    int inFileCurrentLine = 1;
    string line;

    ifstream inFile;
    inFile.open("AddressBook.txt");

    ofstream temporary;
    temporary.open("temp.txt",ios::out|ios::app);

    while (getline(inFile, line)){
        if (inFileCurrentLine != lineToDelete)
            temporary << line << endl;
        inFileCurrentLine++;
    }
    inFile.close();
    temporary.close();

    remove("AddressBook.txt");
    rename("temp.txt", "AddressBook.txt");
}


int deleteFriend (vector <Friends> friendsDatabase, int numberOfFriends){
    int idToDelete;
    char removeDecision;
    cout << "Wpisz ID uzytkownika, ktorego chcesz usunac: " << '\n';
    cin >> idToDelete;

    auto deleteIterator = find_if(friendsDatabase.begin(), friendsDatabase.end(), [idToDelete](const Friends& p) {
    return p.id == idToDelete;});

    cout << "Czy napewno chcesz usunac uzytkownika o ponizszych danych? " << '\n';
    cout << deleteIterator -> name  << '\n' << deleteIterator -> surname << '\n' << deleteIterator -> phoneNumber << '\n';
    cout << deleteIterator -> email << '\n' << deleteIterator -> address << '\n';
    cout << "Jesli tak wcisnij 't', jesli sie rozmysliles, nacisnij inny klawisz ";\
    cin.ignore();
    removeDecision = getchar();

    if (removeDecision == 't'){

                friendsDatabase.erase(deleteIterator);
        deleteFriendFromFile (idToDelete);

        cout << "Uzytkownik usuniety pomyslnie! Aby kontynuowac, wcisnij dowolny klawisz....";
        return numberOfFriends - 1;
        }

    else {
        cout << "Uzytkownik nie zostal usuniety. Aby kontynuowac, wcisnij dowolny klawisz....";
        return numberOfFriends;
    }
}


void displayAll (const vector <Friends> friendsDatabase){
    auto displayIterator = friendsDatabase.begin();
    while (displayIterator != friendsDatabase.end()){
        cout << '\n' << displayIterator -> id << '\n' << displayIterator -> name  << '\n' ;
        cout <<  displayIterator -> surname << '\n' <<displayIterator -> email << '\n';
        cout <<  displayIterator -> phoneNumber << '\n' << displayIterator -> address << '\n';
        displayIterator++;
    }
}



void searchByName (const vector <Friends> &friendsDatabase) {
    string nameToSearch;

    cout << "Podaj imie, ktore chcesz wyszukac" << '\n' ;
    cin >> nameToSearch;

    if (find_if(friendsDatabase.begin(), friendsDatabase.end(), [nameToSearch](const Friends& p) {
    return p.name == nameToSearch;})==friendsDatabase.end()) {
        cout << "Nie znaleziono imienia "<< nameToSearch << '\n';
        return;
        ;
    }

    auto nameIterator = friendsDatabase.begin();
    while ( ( nameIterator = find_if(nameIterator, friendsDatabase.end(), [nameToSearch](const Friends& p) {
    return p.name == nameToSearch;}))!= friendsDatabase.end()) {
        cout << '\n' << "Znaleziono wpis w kziazce: " << '\n' << nameIterator->name  << '\n' << nameIterator->surname << '\n';
        cout << nameIterator -> email << '\n' << nameIterator -> address << '\n';
        nameIterator++;
    }
}

void searchBySurname (const vector <Friends> &friendsDatabase) {
    string surnameToSearch;

    cout << "Podaj nazwisko, ktore chcesz wyszukac" << '\n' ;
    cin >> surnameToSearch;

    auto surnameIterator = friendsDatabase.begin();

    if (find_if(friendsDatabase.begin(), friendsDatabase.end(), [surnameToSearch](const Friends& p) {
    return p.surname == surnameToSearch;})==friendsDatabase.end()) {

        cout << "Nie znaleziono imienia "<< surnameToSearch << '\n';
        return;
        ;
    }

    while ( ( surnameIterator = find_if(surnameIterator, friendsDatabase.end(), [surnameToSearch](const Friends& p) {
    return p.surname == surnameToSearch;}))!= friendsDatabase.end()) {
        cout << '\n' << "Znaleziono wpis w kziazce: " << '\n' << surnameIterator->name  << '\n' << surnameIterator->surname << '\n';
        cout << surnameIterator -> email << '\n' << surnameIterator -> phoneNumber << '\n' << surnameIterator -> address << '\n';
        surnameIterator++;
    }
}

int addRecord (vector <Friends> &friendsDatabase, int numberOfFriends, int loggedUserId) {

    Friends temporary = {};     // temporary structure, which will be pushed to vector of structures

    if (friendsDatabase.size() == 0)
        temporary.id = 1;
    else{
        auto lastIdIterator = friendsDatabase.end();
        --lastIdIterator;
        temporary.id = lastIdIterator->id + 1;
    }

    temporary.usersId = loggedUserId;

    cout << "Podaj imie: "<< '\n';
    cin >> temporary.name;

    cout << "Podaj nazwisko: "<< '\n';
    cin >> temporary.surname;

    cout << "Podaj adres e-mail: " << '\n';
    cin >> temporary.email;

    cout << "Podaj numer telefonu: " << '\n';
    cin >> temporary.phoneNumber;

    cout << "Podaj adres: ul / Numer domu / Miejscowosc" << '\n';
    cin.ignore();
    getline(cin,temporary.address);

    friendsDatabase.push_back(temporary);
    cout << '\n' << "Pomyslnie dodano uzytkownika" << '\n';

    ofstream outFile;
    outFile.open("AddressBook.txt", ios::out|ios::app);
    if (outFile.good() == true) {
        outFile << temporary.id << "|";
        outFile << temporary.usersId << "|";
        outFile << temporary.name << "|";
        outFile << temporary.surname << "|";
        outFile << temporary.email << "|" ;
        outFile << temporary.phoneNumber << "|";
        outFile << temporary.address << "|";
        outFile << '\n';

        outFile.close();
    }
    return numberOfFriends+1;
}




int main() {
    vector <Friends> addressBook;
    vector <Users> usersDatabase;

    char select;
    int numberOfFriends = 0, numberOfUsers = 0;
    int loggedUserId = 0;

    numberOfFriends = readFriendsFile(addressBook);
    numberOfUsers = readUsersFile(usersDatabase);

    while(1) {
            if (loggedUserId == 0){
                system("cls");
                cout << "1. Rejestracja" << endl;
                cout << "2. Logowanie" << endl;
                cout << "9. Zakoncz program" << endl;
                cin >> select;

                switch (select){
                case '1':
                    numberOfUsers = registration(usersDatabase, numberOfUsers);
                    break;

                case '2':
                    loggedUserId = login(usersDatabase, numberOfUsers);
                    break;
                case '9':
                    exit(0);
                }
            }

            else {
                system("cls");
                cout << "1. Dodaj wpis" << '\n';
                cout << "2. Szukaj po imieniu" << '\n';
                cout << "3. Szukaj po nazwisku" << '\n';
                cout << "4. Wyswietl wszystkich uzytkownikow" << '\n';
                cout << "5. Usun adresata" << '\n';
                cout << "6. Edytuj adresata" << '\n';
                cout << "8. Wyloguj sie" << '\n';
                cout << "9. Zakoncz program" << '\n';

                cin >> select;
                cin.ignore();

                switch (select) {
                case '1':
                    numberOfFriends = addRecord(addressBook, numberOfFriends, loggedUserId);
                    Sleep(2000);
                    break;

                case '2':
                    searchByName(addressBook);
                    cout << "Aby kontynuowac, wcisnij dowolny klawisz....";
                    getch();
                    break;

                case '3':
                    searchBySurname(addressBook);
                    cout << "Aby kontynuowac, wcisnij dowolny klawisz....";
                    getch();
                    break;

                case '4':
                    cout << "Lista przyjaciol:"<< '\n';
                    displayAll(addressBook);
                    cout << "Aby kontynuowac, wcisnij dowolny klawisz....";
                    getch();
                    break;

                case '5':
                    deleteFriend(addressBook, numberOfFriends);
                    getch();
                    break;

                case '6':
                    editFriendsDatabaseMenu(addressBook);
                    break;

                case '8':
                    loggedUserId = 0; //logout
                    break;

                case '9':
                    exit(0);

                default:
                    cout << "Wybierz poprawna opcje! ";
                    Sleep(2000);
                    break;
                }
            }
    }
    return 0;
}
