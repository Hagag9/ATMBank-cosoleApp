
#include <iostream>
#include <vector>
#include<fstream>
#include<string>
#include<cctype>
#include<iomanip>
using namespace std;
// ATM System
const string ClientsFileName = "client.txt";
enum enAtmMenue { enQWithdraw = 1, enNWithdraw = 2, enDeposite = 3, enBalance = 4, enLogOut = 5 };
struct StclientData
{
	string AcountNum;
	string  pinCode;
	string Name;
	string Phone;
	int Acountbalance = 0;
	bool MarkForDelete = false;
};
StclientData CurrentClient;
vector < StclientData> LoadClientsFromFile(string FileName);
bool FindByAcountNumber(string FileName, string AccountNumber, StclientData& client);
StclientData ConvertLineToRecord(string line, string Seperator);
string convertRecordToLine(StclientData client, string sperator);
void LogIn();
void AtmMenue();
void PerformAtmMenue(string);

int ReadNumber(string message)
{
	int number;
	cout << message;
	cin >> number;
	while (cin.fail() || number < 0)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalid Number, Enter a valid one: ";
		cin >> number;
	}
	return number;
}
vector <string>Split(string S, string delim)
{
	vector <string> Vwords;
	short pos = 0;
	string S1;
	while ((pos = S.find(delim)) != S.npos)
	{
		S1 = S.substr(0, pos);
		if (S1 != "")
		{
			Vwords.push_back(S1);
		}
		S.erase(0, pos + delim.length());
	}
	if (S != "")
	{
		Vwords.push_back(S);
	}
	return Vwords;
}

StclientData ConvertLineToRecord(string line, string Seperator)
{
	vector<string>Vwords = Split(line, Seperator);
	StclientData StClient;
	StClient.AcountNum = Vwords[0];
	StClient.pinCode = Vwords[1];
	StClient.Name = Vwords[2];
	StClient.Phone = Vwords[3];
	StClient.Acountbalance = stoi(Vwords[4]);

	return StClient;
}

vector < StclientData> LoadClientsFromFile(string FileName)
{
	vector < StclientData> Vclients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string line;
		while (getline(MyFile, line))
		{
			Vclients.push_back(ConvertLineToRecord(line, "#//#"));
		}
		MyFile.close();
	}
	return Vclients;
}

vector <StclientData> SaveClientDataToFile(string FileName, vector <StclientData>& Vclient)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
	{
		string line;
		for (StclientData& c : Vclient)
		{
			if (!(c.MarkForDelete))
			{
				line = convertRecordToLine(c, "#//#");
				MyFile << line << endl;
			}
		}
		MyFile.close();
	}
	return Vclient;
}
string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

bool FindByAcountNumber(string FileName, string AccountNumber, StclientData& client)
{
	vector < StclientData> Vclients = LoadClientsFromFile(FileName);
	for (StclientData c : Vclients)
	{
		if (c.AcountNum == AccountNumber)
		{
			client = c;
			return true;
		}
	}
	return false;
}
bool FindByAcountNumberPinCode(string AccountNumber, string PinCode, StclientData& client)
{
	vector < StclientData> Vclients = LoadClientsFromFile(ClientsFileName);
	for (StclientData c : Vclients)
	{
		if (c.AcountNum == AccountNumber && c.pinCode == PinCode)
		{
			client = c;
			return true;
		}
	}
	return false;
}
bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindByAcountNumberPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}



void PrintClientBalance(StclientData Client)
{
	cout << "|" << setw(15) << left << Client.AcountNum
		<< "|" << setw(20) << left << Client.Name
		<< "|" << setw(12) << left << Client.Acountbalance;
}


string convertRecordToLine(StclientData client, string sperator)
{
	string clientRecord = "";
	clientRecord += client.AcountNum + sperator;
	clientRecord += client.pinCode + sperator;
	clientRecord += client.Name + sperator;
	clientRecord += client.Phone + sperator;
	clientRecord += to_string(client.Acountbalance);
	return clientRecord;
}

void AddlineToFile(string FileName, string line)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << line << endl;
		MyFile.close();
	}
}

void LogOut()
{
	system("cls");
	LogIn();
}



void GoBackToMainMenue()
{
	cout << "\n\n\n\n Press any key to go back to main menu ..... ";
	system("pause>0");
	system("cls");
	AtmMenue();
}


enAtmMenue SelectMainMenueOption()
{
	int option;
	cout << "What do you want to do ? [1 to 5 ]?\n";
	cin >> option;
	return enAtmMenue(option);
}
void AtmMenue()
{
	system("cls");
	cout << "=======================================\n";
	cout << "\t\tATM Main Menue Screen \n";
	cout << "=======================================\n";
	cout << "\t[1] Quick withdraw\n";
	cout << "\t[2] Normal Withdraw \n";
	cout << "\t[3] Deposite\n";
	cout << "\t[4] Check Balance\n";
	cout << "\t[5] Log Out\n";
	cout << "=======================================\n";
	PerformAtmMenue(ClientsFileName);
}

void Deposite(string ClientFile)
{

	vector <StclientData> Vclient = LoadClientsFromFile(ClientFile);
	int Amount = ReadNumber("Please Enter Depsite Amount ? ");
	char Answer;
	cout << "\n\nAre you sure you want perform this transaction? y/n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (StclientData& c : Vclient)
		{
			if (c.AcountNum == CurrentClient.AcountNum)
			{
				c.Acountbalance = c.Acountbalance + Amount;
				SaveClientDataToFile(ClientFile, Vclient);
				cout << "Done Secsessfully ..\nNew balance is : " << c.Acountbalance << endl;
				break;
			}
		}

	}
}
void Withdraw(string FileName)
{
	vector <StclientData> Vclient = LoadClientsFromFile(FileName);
	int Amount = ReadNumber("Please Enter Withdraw Amount(Multiaple of 5) ? ");
	while (Amount % 5 != 0)
	{
		Amount = ReadNumber("Please Enter Withdraw Amount(Multiaple of 5 ? ");
	}
	while (Amount > CurrentClient.Acountbalance)
	{
		cout << "Amount Exceeds the Balance , you can Withdraw up to :" << CurrentClient.Acountbalance << endl;
		cout << "Please enter another Amount ? ";
		cin >> Amount;
	}
	cout << "\n\nAre you sure you want perform this transaction? y/n ? ";

	char Answer;
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (StclientData& c : Vclient)
		{
			if (c.AcountNum == CurrentClient.AcountNum)
			{
				c.Acountbalance = c.Acountbalance - Amount;
				SaveClientDataToFile(FileName, Vclient);
				cout << "Done Secsessfully ..\nNew balance is : " << c.Acountbalance << endl;
				break;
			}
		}
	}
}

int CheckBlanace(string FileName)
{
	if (LoadClientInfo(CurrentClient.AcountNum, CurrentClient.pinCode))
		return CurrentClient.Acountbalance;
	else return false;
}
void CheckBlanaceScreen()
{
	system("cls");
	cout << "-----------------------------------------\n";
	cout << "\t Check Balance Screen  :-) \n";
	cout << "-----------------------------------------\n";
	cout << "Your Balance Is : " << CheckBlanace(ClientsFileName);
	GoBackToMainMenue();
}

void DepositeScreen(string FileName)
{
	system("cls");
	cout << "-----------------------------------------\n";
	cout << "\t Deposite Screen  :-) \n";
	cout << "-----------------------------------------\n";
	Deposite(FileName);
	GoBackToMainMenue();
}
void NormalWithdrawScreen(string FileName)
{
	system("cls");
	cout << "-----------------------------------------\n";
	cout << "\t Withdraw Screen  :-) \n";
	cout << "-----------------------------------------\n";
	Withdraw(FileName);
	GoBackToMainMenue();
}

void QuickWithdraw()
{
	int Amount = 0;
	vector <StclientData> Vclient = LoadClientsFromFile(ClientsFileName);
	int amountNumber = ReadNumber("choose amount from [1] to [8]?");
	if (amountNumber == 1)
		Amount = 20;
	if (amountNumber == 2)
		Amount = 50;
	if (amountNumber == 3)
		Amount = 100;
	if (amountNumber == 4)
		Amount = 200;
	if (amountNumber == 5)
		Amount = 400;
	if (amountNumber == 6)
		Amount = 600;
	if (amountNumber == 7)
		Amount = 800;
	if (amountNumber == 8)
		Amount = 1000;
	if (amountNumber == 9)
		GoBackToMainMenue();
	while (Amount > CurrentClient.Acountbalance)
	{
		cout << "Amount Exceeds the Balance , you can Withdraw up to :" << CurrentClient.Acountbalance << endl;
		amountNumber = ReadNumber("Please enter another Amount ? ");

	}
	cout << "\n\nAre you sure you want perform this transaction? y/n ? ";

	char Answer;
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (StclientData& c : Vclient)
		{
			if (c.AcountNum == CurrentClient.AcountNum)
			{
				c.Acountbalance = c.Acountbalance - Amount;
				SaveClientDataToFile(ClientsFileName, Vclient);
				cout << "Done Secsessfully ..\nNew balance is : " << c.Acountbalance << endl;
				break;
			}
		}
	}
}
void QuickWithDrawScreen()
{
	system("cls");
	cout << "=======================================\n";
	cout << "\t\tQucik Withdraw Screen \n";
	cout << "=======================================\n";
	cout << "\t[1] 20 \t\t[2]50\n";
	cout << "\t[3] 100\t\t[4]200\n";
	cout << "\t[5] 400\t\t[5]600\n";
	cout << "\t[7] 800\t\t[6]1000\n";
	cout << "\t[9] Exit\n";
	cout << "=======================================\n";
	QuickWithdraw();
	GoBackToMainMenue();
}
void PerformAtmMenue(string FileName)
{
	enAtmMenue option = SelectMainMenueOption();
	switch (option)
	{
	case enAtmMenue::enBalance:
	{
		CheckBlanaceScreen();
		break;
	}
	case enAtmMenue::enDeposite:
	{
		DepositeScreen(FileName);
		break;
	}
	case enAtmMenue::enNWithdraw:
	{
		NormalWithdrawScreen(FileName);
		break;
	}
	case enAtmMenue::enQWithdraw:
	{
		QuickWithDrawScreen();
		break;
	}
	case enAtmMenue::enLogOut:
	{
		LogOut();
		break;
	}
	default:
		break;
	}
}

void LogIn()
{
	bool FailLogIn = false;
	string UserName, Password;
	do
	{
		system("cls");
		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";
		if (FailLogIn)
		{
			cout << "Invlaid Username/Password!\n";
		}
		cout << "Enter Usernmae\n";
		cin >> UserName;
		cout << "Enter Pin\n";
		cin >> Password;
		FailLogIn = !LoadClientInfo(UserName, Password);

	} while (FailLogIn);
	AtmMenue();
}




int main()
{

	LogIn();


	system("pause>0");
	return 0;
}



