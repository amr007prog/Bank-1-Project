

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const string FileName = "Clients.txt";

enum enOptions { ShowClientList = 1, AddNewClient = 2, DeleteClient = 3, UpdateClientInfo = 4, FindClient = 5, Exit = 6 };

struct stClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkToDelete = false;
};

vector <string> SplitString(string Line, string Delim = "#//#") {

    vector <string> vSplitString;

    int Pos = 0;
    
    string sWord;

    while ((Pos = Line.find(Delim)) != std::string::npos) {

        sWord = Line.substr(0, Pos);
        
        if (sWord != "") {

            vSplitString.push_back(sWord);

        }

        Line.erase(0, Pos + Delim.length());

    }

    if (Line != "") {

        vSplitString.push_back(Line);

    }

    return vSplitString;

}

stClient ConvertLineToRecord(string Line, string Delim = "#//#") {

    stClient Client;

    vector <string> vClients = SplitString(Line);

    Client.AccountNumber = vClients[0];
    Client.PinCode = vClients[1];
    Client.Name = vClients[2];
    Client.Phone = vClients[3];
    Client.AccountBalance = stod(vClients[4]);

    return Client;
}

string ConvertRecordToLine(stClient Client, string Delim = "#//#") {

    string Line = "";

    Line += Client.AccountNumber + Delim;
    Line += Client.PinCode + Delim;
    Line += Client.Name + Delim;
    Line += Client.Phone + Delim;
    Line += to_string(Client.AccountBalance);

    return Line;

}

vector <stClient> LoadClientsDataFromFile(string FileName) {

    vector <stClient> vClients;

    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {

        string Line;
        stClient Client;
        while (getline(MyFile, Line)) {

            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }

    return vClients;
}

void PrintClient(stClient Client) {

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(12) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(15) << left << Client.Phone;
    cout << "| " << setw(15) << left << Client.AccountBalance;

}

void PrintClients(vector <stClient> vClients) {

    cout << "\t\t\t\t Client List (" << vClients.size() << ") Client(s).";
    cout << "\n----------------------------------------------------------------------------------------------------\n";
    cout << "| " << setw(15) << left << "Account Number";
    cout << "| " << setw(12) << left << "Pin Code";
    cout << "| " << setw(40) << left << "Client Name";
    cout << "| " << setw(15) << left << "Phone";
    cout << "| " << setw(15) << left << "Balance";
    cout << "\n----------------------------------------------------------------------------------------------------\n";

    for (stClient C : vClients) {

        PrintClient(C);
        cout << endl;

    }

    cout << "\n\n----------------------------------------------------------------------------------------------------\n";

}

stClient ReadNewClientData(vector <stClient> vClients) {

    stClient NewClient;

 

  
        cout << "Enter Account Number: ";
        getline(cin >> ws, NewClient.AccountNumber);
        bool Exists = true;
        while (Exists) {
            Exists = false;
            for (const stClient& C : vClients) {
                if (NewClient.AccountNumber == C.AccountNumber) {
                    Exists = true;
                    break;
                }
            }

            if (!Exists) break; 

            
            cout << "Error: Client with [" << NewClient.AccountNumber << "] already exists. Enter another account number: ";
            getline(cin >> ws, NewClient.AccountNumber);
        }

    cout << "Enter PinCode: ";
    getline(cin >> ws, NewClient.PinCode);

    cout << "Enter Name: ";
    getline(cin >> ws, NewClient.Name);

    cout << "Enter Phone: ";
    getline(cin >> ws, NewClient.Phone);

    cout << "Enter AccountBalance: ";
    cin >> NewClient.AccountBalance;

    return NewClient;

}

void AddDataLineToFile(string FileName, string stDataLine) {

    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open()) {

        MyFile << stDataLine << endl;
        MyFile.close();
    }

}

void AddClient() {
    vector <stClient> vClients = LoadClientsDataFromFile(FileName);
    stClient NewClient = ReadNewClientData(vClients);
    AddDataLineToFile(FileName, ConvertRecordToLine(NewClient));
}

void AddClients() {

    cout << "=========================================\n";
    cout << "         Add New Clients Screen\n";
    cout << "=========================================\n";
    
    char AddMore = 'Y';

    do {
        system("cls");
        cout << "Adding New Clients:\n\n";
        AddClient();
        cout << "Client Added Successfully, do you want to add more? Y/N: ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');

}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClients , stClient &Client) {

    for (stClient C : vClients) {

        if (C.AccountNumber == AccountNumber) {

            Client = C;
            return true;
        }

    }
    return false;
}

string ReadAccountNumber() {
    string AccountNumber = "";

    cout << "Please Enter Account Number: ";
    cin >> AccountNumber;
    
    return AccountNumber;
}

void PrintCard(stClient Client) {

    cout << "\nThe Following Are The Client Details:\n";
    cout << "----------------------------------------\n";
    cout << "Account Number: " << Client.AccountNumber;
    cout << "\nPin Code    : " << Client.PinCode;
    cout << "\nName        : " << Client.Name;
    cout << "\nPhone       : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n--------------------------------------\n";

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient> &vClients) {
    
    for (stClient &C : vClients) {

        if (C.AccountNumber == AccountNumber) {

            C.MarkToDelete = true;
            return true;

        }

    }
    return false;
}

vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient> vClients) {

    fstream MyFile;

    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open()) {

        for (stClient C : vClients) {

            if (C.MarkToDelete == false) {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }
        MyFile.close();
    }
    return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vClients) {
    
    stClient Client;
    char Ans = 'Y';
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintCard(Client);
        cout << "\n\nAre You Sure You Want To Delete This Accout? Y/N:";    
        cin >> Ans;
        if (toupper(Ans) == 'Y') {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(FileName, vClients);

            vClients = LoadClientsDataFromFile(FileName);

            cout << "\nClient Added Successfully.";
            return true;

        }
    }
    else {
        cout << "Client With Account Number(" << AccountNumber << ") Not Found!";
        return false;
    }
}

void DeleteScreen(vector <stClient>& vClients) {

    cout << "-----------------------------------------------------------\n";
    cout << "                Delete Client Screen\n";
    cout << "-----------------------------------------------------------\n\n";
    
    string AccountNumber = ReadAccountNumber();

    DeleteClientByAccountNumber(AccountNumber, vClients);
    

}

stClient UpdateClientRecord(string AccountNumber) {

    stClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode: ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name: ";
    getline(cin >> ws, Client.Name);

    cout << "Enter Phone: ";
    getline(cin >> ws, Client.Phone);

    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClient>& vClients) {

    stClient Client;
    char Ans = 'Y';
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintCard(Client);
        cout << "\n\nAre you Sure you want to update this Client Data? Y/N: ";
        cin >> Ans;
        if (toupper(Ans) == 'Y') {

            for (stClient& C : vClients) {

                if (C.AccountNumber == AccountNumber) {

                    C = UpdateClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientsDataToFile(FileName, vClients);
            cout << "\n\nClient Data Updated Successfully.";
            return true;
        }
    }
    else {
        cout << "Client With Account Number(" << AccountNumber << ") Not Found.";
        return false;
    }
}

void UpdateClientScreen(vector <stClient> vClients) {

    cout << "-----------------------------------------------------------\n";
    cout << "                 Update Client Info Screen\n";
    cout << "-----------------------------------------------------------\n\n";
    string AccountNumber = ReadAccountNumber();

    UpdateClientByAccountNumber(AccountNumber, vClients);
   
}

void FindClientScreen(vector <stClient> vClients) {

    cout << "-----------------------------------------------------------\n";
    cout << "                 Find Client Screen\n";
    cout << "-----------------------------------------------------------\n\n";
    string AccountNumber = ReadAccountNumber();
    stClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintCard(Client);
      
    }
    else {
        cout << "Client With Account Number(" << AccountNumber << ") Not Found!";
       
    }
}

void ExitScreen() {
    cout << "\n\n------------------------------------\n";
    cout << "             Program Ends :-)           \n";
    cout << "-------------------------------------\n";
}

void MainMenu() {
    int Choice;
    bool Exit = false;
    do {
        do {
            system("cls");
            cout << "======================================================\n";
            cout << "                  Main Menu Screen\n";
            cout << "======================================================\n";
            cout << "               [1] Show Client List.\n";
            cout << "               [2] Add New Client.\n";
            cout << "               [3] Delete Client.\n";
            cout << "               [4] Update Client Info.\n";
            cout << "               [5] Find Client.\n";
            cout << "               [6] Exit\n";
            cout << "======================================================\n";
            cout << "Choose What Do You Want to do? [1 to 6]? ";

            cin >> Choice;
            Choice = (enOptions)Choice;
        } while (Choice < 1 || Choice > 6);
        vector <stClient> vClients = LoadClientsDataFromFile(FileName);
        switch (Choice) {
        case enOptions::ShowClientList:
            system("cls");
            PrintClients(vClients);
            cout << "\n\n\nPress any key to go back to main menu...";
             system("pause > 0");
            break;
        case enOptions::AddNewClient:
            system("cls");
            AddClients();
            cout << "\n\n\nPress any key to go back to main menu...";
             system("pause > 0");
            break;
        case enOptions::DeleteClient:
            system("cls");
            DeleteScreen(vClients);
            cout << "\n\n\nPress any key to go back to main menu...";
             system("pause > 0");
            break;
        case enOptions::UpdateClientInfo:
            system("cls");
            UpdateClientScreen(vClients);
            cout << "\n\n\nPress any key to go back to main menu...";
             system("pause > 0");
            break;
        case enOptions::FindClient:
            system("cls");
            FindClientScreen(vClients);
            cout << "\n\n\nPress any key to go back to main menu...";
             system("pause > 0");
            break;
        case enOptions::Exit:
            system("cls");
            ExitScreen();
            system("pause");
            Exit = true;
            break;
        }
       
    } while ( Exit != true);

}

int main()
{
    MainMenu();
}


