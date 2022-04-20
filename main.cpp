#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <conio.h>
#include <cstdio>
#include <fstream>
#include <istream>

using namespace std;

struct Users {
    int usersId;
    string userName, password;
};

struct Friends {
    int id, usersId;
    string name, surname, email, phoneNumber, address;
};

int readFriendsFile (vector <Friends> &friendsDatabase, int loggedUsersId) {
    int numberOfLines = 0, numberOfFriends = 0;
    int temporaryFriendsIdConvertedToInt = 0, temporaryUsersIdConvertedToInt = 0;

    string friendsIdStringToBeConverted, usersIdStringToBeConverted, trashLine;

    ifstream inFile("AddressBook.txt",std::ios::binary);

    numberOfLines = count(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>(), '\n');

    inFile.seekg(0);

    for (int i = 0; i < numberOfLines; i++) {

        getline(inFile,friendsIdStringToBeConverted,'|');
        temporaryFriendsIdConvertedToInt = stoi(friendsIdStringToBeConverted);

        getline(inFile,usersIdStringToBeConverted,'|');
        temporaryUsersIdConvertedToInt = stoi(usersIdStringToBeConverted);

        if (temporaryUsersIdConvertedToInt == loggedUsersId){
            friendsDatabase.push_back(Friends{});

            friendsDatabase[numberOfFriends].id = temporaryFriendsIdConvertedToInt;
            friendsDatabase[numberOfFriends].usersId = temporaryUsersIdConvertedToInt;

            getline(inFile,friendsDatabase[numberOfFriends].name,'|');
            getline(inFile,friendsDatabase[numberOfFriends].surname,'|');
            getline(inFile,friendsDatabase[numberOfFriends].email,'|');
            getline(inFile,friendsDatabase[numberOfFriends].phoneNumber,'|');
            getline(inFile,friendsDatabase[numberOfFriends].address,'|');
            inFile.ignore();

            numberOfFriends++;
        }
        else
            getline(inFile,trashLine,'\n');
    }
    inFile.close();
    return numberOfFriends;
}

int readLastFriendsId (){
    string workString = "", lastFriendsIdString = "";
    int lastFriendsId = 0;

    ifstream inFile("AddressBook.txt");
    //totalNumberOfFriends = count(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>(), '\n');
    while (getline(inFile, workString)){
        if (inFile.peek() == EOF){
            auto position = workString.find('|');
            lastFriendsIdString = workString.substr(0, position);
            lastFriendsId = stoi(lastFriendsIdString);
        }
    }

    inFile.close();
    return lastFriendsId;
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

int registration(vector <Users> &usersDatabase, int numberOfUsers)
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

void overWriteExistingFile (const vector <Friends> friendsDatabase, auto editIterator, int loggedUsersId, int idFriendToBeEddited){
    int numberOfLines = 0, friendsIdConvertedToInt = 0, usersIdConvertedToInt = 0;
    string friendsIdString = "", usersIdString = "", workLine = "";

    ifstream inFile("AddressBook.txt");
    ofstream temp;
    temp.open("newTempUsers.txt",ios::out|ios::app);

    numberOfLines = count(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>(), '\n');
    inFile.seekg(0);

    for (int i = 0; i < numberOfLines; i++) {

        getline(inFile,friendsIdString,'|');
        friendsIdConvertedToInt = stoi(friendsIdString);

        getline(inFile,usersIdString,'|');
        usersIdConvertedToInt = stoi(usersIdString);

        if (usersIdConvertedToInt == loggedUsersId && friendsIdConvertedToInt == idFriendToBeEddited){

            temp << friendsIdString << '|' << usersIdString << '|';
            temp << editIterator->name  << '|' << editIterator->surname << '|' << editIterator->email << '|' << editIterator->phoneNumber << '|' << editIterator->address<< '|' << '\n';
            getline(inFile,workLine, '\n');
        }
        else{
            temp << friendsIdString << '|' << usersIdString << '|';
            getline(inFile,workLine,'\n');
            temp << workLine << '\n';
        }

    }
    inFile.close();
    temp.close();

    remove("AddressBook.txt");
    rename("newTempUsers.txt", "AddressBook.txt");
}

void editFriendsDatabaseMenu (vector <Friends> friendsDatabase, int loggedUserId){
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
            overWriteExistingFile(friendsDatabase, editIterator, loggedUserId, idOfFriendToEdit);
            Sleep(2000);
            break;
            }

        case '2':{
            string newSurname;
            cout << "Podaj nowe nazwisko: ";
            cin >> newSurname;
            editIterator->surname = newSurname;
            cout << "Nazwisko uzytkownika o ID: " << idOfFriendToEdit << " zostalo edytowane. ";
            overWriteExistingFile(friendsDatabase, editIterator, loggedUserId, idOfFriendToEdit);
            Sleep(2000);
            break;
            }

        case '3':{
               string newPhoneNumber;
            cout << "Podaj nowy numer telefonu: ";
            cin >> newPhoneNumber;
            editIterator->phoneNumber = newPhoneNumber;
            cout << "Nr. telefona uzytkownika o ID: " << idOfFriendToEdit << " zostal edytowany. ";
            overWriteExistingFile(friendsDatabase, editIterator, loggedUserId, idOfFriendToEdit);
            Sleep(2000);
            break;
            }

        case '4':{
            string newEmail;
            cout << "Podaj nowy e-mail: ";
            cin >> newEmail;
            editIterator->email = newEmail;
            cout << "E-mail uzytkownika o ID: " << idOfFriendToEdit << " zostal edytowany. ";
            overWriteExistingFile(friendsDatabase, editIterator, loggedUserId, idOfFriendToEdit);
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
            overWriteExistingFile(friendsDatabase, editIterator, loggedUserId, idOfFriendToEdit);
            Sleep(2000);
            break;
            }

        case '6':{
            goto exitEdditingMenu;
            overWriteExistingFile(friendsDatabase, editIterator, loggedUserId, idOfFriendToEdit);
            break;
            }

        default:
            cout << "Wybierz poprawna opcje!"<< '\n';
            overWriteExistingFile(friendsDatabase, editIterator, loggedUserId, idOfFriendToEdit);
            Sleep(2000);
            break;
        }
    }
    exitEdditingMenu:;
}


void deleteFriendFromFile (int friendsIdToDelete){

    string line,friendsIdString;
    int friendsIdConvertedToInt;

    ifstream inFile;
    inFile.open("AddressBook.txt");

    ofstream temporary;
    temporary.open("temp.txt",ios::out|ios::app);

    while (getline(inFile, friendsIdString,'|')){
        friendsIdConvertedToInt = stoi(friendsIdString);
        if (friendsIdConvertedToInt != friendsIdToDelete){
            temporary << friendsIdString << '|';
            getline(inFile, line, '\n');
            temporary << line << '\n';
        }
        else
            getline(inFile,line, '\n');
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

    if (deleteIterator != friendsDatabase.end()){
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
    else{
        cout << "U¿ytkownik o podanym ID nie istnieje lub nie masz praw do jego usuniecia.";
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

int addRecord (vector <Friends> &friendsDatabase, int numberOfFriends, int loggedUserId, int totalNumberOfFriends) {

    Friends temporary = {};     // temporary structure, which will be pushed to vector of structures

    temporary.id = totalNumberOfFriends + 1;

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
    int numberOfFriends = 0, numberOfUsers = 0, lastFriendsId = 0;
    int loggedUserId = 0;

    numberOfUsers = readUsersFile(usersDatabase);
    lastFriendsId = readLastFriendsId();

    while(1) {
            if (loggedUserId == 0){
                system("cls");
                cout << "1. Rejestracja" << '\n';
                cout << "2. Logowanie" << '\n';
                cout << "9. Zakoncz program" << '\n';
                cin >> select;

                switch (select){
                case '1':
                    numberOfUsers = registration(usersDatabase, numberOfUsers);
                    break;

                case '2':
                    loggedUserId = login(usersDatabase, numberOfUsers);
                    numberOfFriends = readFriendsFile(addressBook, loggedUserId);
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
                    numberOfFriends = addRecord(addressBook, numberOfFriends, loggedUserId, lastFriendsId);
                    lastFriendsId++;
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
                    lastFriendsId = readLastFriendsId();
                    getch();
                    break;

                case '6':
                    editFriendsDatabaseMenu(addressBook, loggedUserId);
                    break;

                case '8':
                    loggedUserId = 0; //logout
                    addressBook.clear();
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
