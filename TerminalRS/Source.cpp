#include <iostream>
#include <windows.h>

using namespace std;

HANDLE handleSending;
HANDLE handleRecieving;

DCB dcb;

LPCTSTR sendingPortName = L"COM3";
LPCTSTR recievingPortName = L"COM4";

char bufRead[256], bufWrite[256];
DWORD bytesCount;

void showHandleError(HANDLE commHandle);
bool setDCB(unsigned long BaudRate, unsigned long fParity, int Parity, int StopBits, int ByteSize, DWORD Dtr_Ctrl, DWORD Rts_Ctrl);



int main() {

	handleSending = CreateFile(sendingPortName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handleSending == INVALID_HANDLE_VALUE)	{
		cout << "Zle otwarty port wysylania" << endl;
		showHandleError(handleSending);
		cin.ignore();
		return 1;
	}
	cout << "Otwarto port wysylania: " << sendingPortName << endl;

	handleRecieving = CreateFile(recievingPortName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handleRecieving == INVALID_HANDLE_VALUE)	{
		cout << "Zle otwarty port odbierania" << endl;
		showHandleError(handleRecieving);
		CloseHandle(handleSending);
		cin.ignore();
		return 2;
	}
	cout << "Otwarto port odbierania: " << sendingPortName << endl;

	if (!setDCB(CBR_19200, TRUE, EVENPARITY, ONESTOPBIT, 7, DTR_CONTROL_ENABLE, RTS_CONTROL_ENABLE))
		return 3;

	

	cout << endl << "Nadawanie! Wpisz 'Exit' aby zakonczyc" << endl;
	
	while (bufWrite != "Exit")	{
		cin >> bufWrite;

		if (WriteFile(handleSending, bufWrite, 256, &bytesCount, 0))
			cout << "Wyslano " << bytesCount << " danych" << endl;
		else
			cout << "Nie udalo sie wyslac danych" << endl;

		if (ReadFile(handleRecieving, bufRead, 256, &bytesCount, 0) == NULL)
		{
			cout << endl << endl << "Przyszlo przez port " << recievingPortName << endl;
			cout << "koniec pliku" << endl;
			return 0;
		}
		cout << bufRead << " " << endl;
	}

	CloseHandle(handleSending);
	CloseHandle(handleRecieving);

	system("PAUSE");
	return 0;
}


void showHandleError(HANDLE commHandle) {
	cout << "Kod bledu:" << endl;
	switch ((int)commHandle) {
	case -1:
		cout << "Bledna identyfiakcja portu." << endl;
		break;
	case -12:
		cout << "Blednie okreslona szybkosc transmisji." << endl;
		break;
	case -11:
		cout << "Blednie okreslona liczba bitow danych." << endl;
		break;
	case -5:
		cout << "Blednie okreslone parametry domyslne urzadzenia." << endl;
		break;
	case -10:
		cout << "Odbiornik zablokowany." << endl;
		break;
	case -4:
		cout << "Niewlasciwe rozmiary buforow." << endl;
		break;
	case -3:
		cout << "Urzadzenie nie jest otwarte do transmisji." << endl;
		break;
	case -2:
		cout << "Urzadzenie pozostaje otwarte." << endl;
		break;
	default:
		cout << "Nieokreslony kod bledu." << endl;
		break;
	}
}

bool setDCB(unsigned long BaudRate, unsigned long fParity, int Parity, int StopBits, int ByteSize, DWORD Dtr_Ctrl, DWORD Rts_Ctrl)
{
	dcb.DCBlength = sizeof(dcb);

	dcb.BaudRate = BaudRate;
	dcb.fParity = fParity;
	dcb.Parity = Parity;
	dcb.StopBits = StopBits;
	dcb.ByteSize = ByteSize;
	dcb.fDtrControl = Dtr_Ctrl;
	dcb.fRtsControl = Rts_Ctrl;

	if (SetCommState(handleSending, &dcb)) {
		cout << "Nie udalo sie ustawic DCB dla portu wysylajacego" << endl;
		return false;
	}
	else
		cout << "Ustawiono DCB dla portu wysylajacego" << endl;

	if (SetCommState(handleRecieving, &dcb)) {
		cout << "Nie udalo sie ustawic DCB dla portu odbierajacego" << endl;
		return false;
	}
	else
		cout << "Ustawiono DCB dla portu odbierajacego" << endl;

	return true;
}