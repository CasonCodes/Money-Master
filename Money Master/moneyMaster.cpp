#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <algorithm>
using namespace std;

#pragma region Data_Structures

enum MonthName {
	january = 1,
	february,
	march,
	april,
	may,
	june,
	july,
	august,
	september,
	october,
	november,
	december
};
struct Date {
	int month = 0;
	int day = 0;
	int year = 0;
};
struct Event {
	int day = 0;
	string title = "";
	double amount = 0;
	int frequency = 2;
	bool isIncome() {
		return amount < 0;
	}
	bool hasBeenPrinted = false;
};
#pragma endregion

class BudgetBook {

private:
	Date todaysDate;
	double currentBalance = 0;
	double currentLimit = 0;
	vector<Event> events;
	vector<string> mainMenu = {
		"Quit", 
		"Set Limit", 
		"Set Balance", 
		"Income Menu", 
		"Expense Menu",
		"Display Timeline"
	};
	vector<string> incomeMenu = {
		"Back",
		"Add",
		"Edit",
		"Delete"
	};
	vector<string> expenseMenu = {
		"Back",
		"Add",
		"Edit",
		"Delete"
	};


	string monthToString(MonthName monthName) {
		switch (monthName) {
		case january: return "January";
		case february: return "February";
		case march: return "March";
		case april: return "April";
		case may: return "May";
		case june: return "June";
		case july: return "July";
		case august: return "August";
		case october: return "October";
		case september: return "September";
		case november: return "November";
		case december: return "December";
		}
	}
	int howManyDays(MonthName monthName) {
		switch (monthName) {
		case february: return 28;
		case september:
		case april:
		case june:
		case november: return 30;
		case january:
		case march:
		case may:
		case july:
		case august:
		case october:
		case december: return 31;
		}
	}
	void printTitle(string title) {
		if (title != "") {
			cout << "||========== [" << title << "] ==========||" << endl;
			cout << endl;
		}
	}
	void printPercentageBar(double balance) {
		if (balance < 0) {
			balance *= -1;
		}
		int percent = ((balance / currentLimit) * 100);
		if (currentLimit == 0) {
			percent = 0;
		}
		if (balance == currentLimit) {
			percent = 100;
		}
		cout << setw(5) << right << "[" << percent << "%]";
		percent /= 3;
		for (int p = 0; p < 150; p++) {
			/*if (p > percent) {
				cout << " ";
			}
			else {*/
			
				cout << "|";
				if (percent * 3 < p) {
					break;
				}
			//}
		}
		cout << endl;
	}
	void printEventTimeline() {
		double balance = currentBalance;
		bool yesOrNo = true;
		bool startedFrom = false;
		for (int m = todaysDate.month; m <= 12 && yesOrNo == true; m++) {
			bool printedAnEvent = false;
			printTitle(monthToString(MonthName(m)));
			for (int d = 1; d <= howManyDays(MonthName(m)); d++) {
				if (startedFrom == false) {
					d = todaysDate.day + 1;
					startedFrom = true;
				}
				for (int e = 0; e < events.size(); e++) {
					if (events[e].day == d && (events[e].frequency == 2 || (events[e].frequency == 1 && events[e].hasBeenPrinted == false))) {
						cout << setw(5) << left << events[e].day << right << setw(15) << events[e].title
							<< setw(15) << right << fixed << setprecision(2) << events[e].amount
							<< setw(11) << right << "$" << setw(10) << (balance += events[e].amount);
						printedAnEvent = true;
						events[e].hasBeenPrinted = true;
						printPercentageBar(balance);
					}
				}
			}
			if ((printedAnEvent)) {
				cout << endl;
			}
			else {
				cout << "No remaining events." << endl << endl;
			}
			cout << "Generate next month? (Y/N): ";
			char input;
			cin >> input;
			input = toupper(input);
			yesOrNo = (input == 'Y');
			cout << endl;
			if (m == 12) {
				m = 0;
			}
		}
		for (int i = 0; i < events.size(); i++) {
			events[i].hasBeenPrinted = false;
		}
	}
	void printEvent(Event event) {
		cout << setw(5) << left << event.day << right << setw(15) << event.title
			<< setw(15) << right << fixed << setprecision(2) << event.amount;
		if (event.frequency == 0) {
			cout << right << setw(15) << "Disabled" << endl;
		}
		else if (event.frequency == 1) {
			cout << right << setw(15) << "Once" << endl;
		}
		else {
			cout << right << setw(15) << "Monthly" << endl;
		}
	}
	void printEventHeader() {
		cout << setw(5) << left << "DAY"
			<< right << setw(15) << "TITLE"
			<< setw(15) << right << fixed << setprecision(2) << "AMOUNT"
			<< setw(15) << right << fixed << setprecision(2) << "FREQUENCY" << endl;
		cout << setw(5) << left << "---"
			<< right << setw(15) << "-----"
			<< setw(15) << right << fixed << setprecision(2) << "------" 
			<< setw(15) << right << fixed << setprecision(2) << "---------" << endl;
	}

	void editIncome() {
		string targetTitle;
		cout << "Title to Edit: ";
		cin >> targetTitle;
		
		bool targetFound = false;
		for (int i = 0; i < events.size(); i++) {
			if (events[i].title == targetTitle) {
				targetFound = true;
				char yesOrNo;

				cout << endl;
				cout << "Edit title? (Y/N): ";
				cin >> yesOrNo;
				yesOrNo = toupper(yesOrNo);
				if (yesOrNo == 'Y') {
					cout << " - Title: ";
					cin >> events[i].title;
				}

				cout << "Edit day? (Y/N): ";
				cin >> yesOrNo;
				yesOrNo = toupper(yesOrNo);
				if (yesOrNo == 'Y') {
					cout << " - Day: ";
					cin >> events[i].day;
				}

				cout << "Edit amount? (Y/N): ";
				cin >> yesOrNo;
				yesOrNo = toupper(yesOrNo);
				if (yesOrNo == 'Y') {
					cout << " - Amount: ";
					cin >> events[i].amount;
					if (events[i].amount > 0) {
						events[i].amount *= -1;
					}
				}

				cout << "Edit frequency? (Y/N): ";
				cin >> yesOrNo;
				yesOrNo = toupper(yesOrNo);
				if (yesOrNo == 'Y') {
					cout << " - Frequency: ";
					cin >> events[i].frequency;
				}

				break;
			}
		}
		if (targetFound) {
			cout << endl;
			cout << "Edited [" << targetTitle << "]" << endl;
		}
		else {
			cout << endl;
			cout << "Couldn't find [" << targetTitle << "]" << endl;
			cout << "Titles are case sensitive!" << endl;
			cout << endl;
		}
		system("pause");
		system("cls");
	}
	void addIncome() {
		Event newIncome;

		cout << "Title: ";
		cin >> newIncome.title;

		cout << "Day: ";
		cin >> newIncome.day;

		cout << "Amount: ";
		cin >> newIncome.amount;
		if (newIncome.amount > 0) {
			newIncome.amount *= -1;
		}

		cout << "Frequency: ";
		cin >> newIncome.frequency;

		events.push_back(newIncome);

		cout << endl;
		cout << "Added [" << newIncome.title << "]" << endl;
		cout << endl;
		system("pause");
		system("cls");
	}
	void displayIncomeScreen() {		
		int selection;
		do {
			system("cls");
			printTitle("Income Menu");
			printEvents("income");
			showMenu(incomeMenu);
			selection = getMenuSelection(incomeMenu.size());
			switch (selection) {
			case 1: return;
			case 2: addIncome(); sortEvents(); break;
			case 3: editIncome(); sortEvents(); break;
			case 4: deleteEvent(); sortEvents(); break;
			default: system("cls"); displayIncomeScreen();
			}
		} while (selection != 1);
	}


	void editExpense() {
		string targetTitle;
		cout << "Title to Edit: ";
		cin >> targetTitle;

		bool targetFound = false;
		for (int i = 0; i < events.size(); i++) {
			if (events[i].title == targetTitle) {
				targetFound = true;
				char yesOrNo;

				cout << endl;
				cout << "Edit title? (Y/N): ";
				cin >> yesOrNo;
				yesOrNo = toupper(yesOrNo);
				if (yesOrNo == 'Y') {
					cout << " - Title: ";
					cin >> events[i].title;
				}				

				cout << "Edit day? (Y/N): ";
				cin >> yesOrNo;
				yesOrNo = toupper(yesOrNo);
				if (yesOrNo == 'Y') {
					cout << " - Day: ";
					cin >> events[i].day;
				}			

				cout << "Edit amount? (Y/N): ";
				cin >> yesOrNo;
				yesOrNo = toupper(yesOrNo);
				if (yesOrNo == 'Y') {
					cout << " - Amount: ";
					cin >> events[i].amount;
					if (events[i].amount < 0) {
						events[i].amount *= -1;
					}
				}			

				cout << "Edit frequency? (Y/N): ";
				cin >> yesOrNo;
				yesOrNo = toupper(yesOrNo);
				if (yesOrNo == 'Y') {
					cout << " - Frequency: ";
					cin >> events[i].frequency;
				}				
				break;
			}
		}
		if (targetFound) {
			cout << endl;
			cout << "Edited [" << targetTitle << "]" << endl;
		}
		else {
			cout << endl;
			cout << "Couldn't find [" << targetTitle << "]" << endl;
			cout << "Titles are case sensitive!" << endl;
			cout << endl;
		}
		system("pause");
		system("cls");
	}
	void addExpense() {
		Event newExpense;

		cout << "Title: ";
		cin >> newExpense.title;

		cout << "Day: ";
		cin >> newExpense.day;

		cout << "Amount: ";
		cin >> newExpense.amount;
		if (newExpense.amount < 0) {
			newExpense.amount *= -1;
		}

		cout << "Frequency: ";
		cin >> newExpense.frequency;

		events.push_back(newExpense);

		cout << endl;
		cout << "Added [" << newExpense.title << "]" << endl;
		cout << endl;
		system("pause");
		system("cls");
	}
	void displayExpenseScreen() {		
		int selection;
		do {
			system("cls");
			printTitle("Expense Menu");
			printEvents("expense");
			showMenu(expenseMenu);
			selection = getMenuSelection(expenseMenu.size());
			switch (selection) {
			case 1: return;
			case 2: addExpense(); sortEvents(); break;
			case 3: editExpense(); sortEvents(); break;
			case 4: deleteEvent(); sortEvents(); break;
			default: system("cls"); displayExpenseScreen();
			}
		} while (selection != 1);
	}
	void deleteEvent() {
		string targetTitle;
		cout << "Title to Delete: ";
		cin >> targetTitle;

		bool targetFound = false;
		for (int i = 0; i < events.size(); i++) {
			if (events[i].title == targetTitle) {
				targetFound = true;
				events.erase(events.begin() + i);
				break;
			}
		}
		cout << endl;
		if (targetFound) {
			cout << "Deleted [" << targetTitle << "]" << endl;
		}
		else {
			cout << "Couldn't find [" << targetTitle << "]" << endl;
			cout << "Titles are case sensitive!" << endl;
		}
		cout << endl;
		system("pause");
		system("cls");
	}


	int countEvents(string type) {
		int count = 0;		
		for (int i = 0; i < events.size(); i++) {
			if (type == "income" && events[i].isIncome() == true) {								
				count++;				
			}
			else if (type == "expense" && events[i].isIncome() == false) {				
				count++;				
			}
		}
		return count;
	}
	void printEvents(string type) {		
		double total = 0;
		printEventHeader();
		for (int i = 0; i < events.size(); i++) {
			if (type == "income") {
				if (events[i].isIncome() == true) {
					Event e = events[i];
					e.amount *= -1;
					printEvent(e);
					if (e.frequency == 2) {
						total += events[i].amount;
					}
				}
			}
			else if (type == "expense") {
				if (events[i].isIncome() == false) {
					printEvent(events[i]);
					if (events[i].frequency == 2) {
						total += events[i].amount;
					}
				}
			}
		}
		if (type == "income" && total < 0) {			
			total *= -1;			
		}
		cout << "--------------------------------------------------------" << endl;
		cout << "               Total:       $" << fixed << setprecision(2) << total << endl;
		cout << endl;
	}
	int getMenuSelection(int menuSize) {
		int input;
		cout << endl << "User Selection (1-" << menuSize << "): ";
		cin >> input;
		cout << endl;
		return input;
	}
	void load() {

		// open file
		ifstream iF;
		iF.open("myEvents.txt", ifstream::in);
		
		if (iF.is_open()) {

			// load balance
			iF >> currentBalance;

			// load limit
			iF >> currentLimit;

			// load events
			do {
				Event newEvent;
				iF >> newEvent.day;
				iF >> newEvent.title;
				iF >> newEvent.amount;
				iF >> newEvent.frequency;
				if (newEvent.title != "") {
					events.push_back(newEvent);
				}
			} while (!iF.eof());

			// close file
			iF.close();
		}
	}
	void getTodaysDate() {
		struct tm newtime;
		time_t now = time(0);
		localtime_s(&newtime, &now);
		todaysDate.year = 1900 + newtime.tm_year;
		todaysDate.month = 1 + newtime.tm_mon;
		todaysDate.day = newtime.tm_mday;
	}
	static bool compareEvent(Event e1, Event e2) {
		return e1.day < e2.day;
	}
	void sortEvents() {
		sort(events.begin(), events.end(), compareEvent);
	}
	void save() {
		char yesOrNo;
		cout << "Save Changes? (Y/N): ";
		cin >> yesOrNo;
		yesOrNo = toupper(yesOrNo);
		if (yesOrNo == 'Y') {
			// open file
			ofstream oF;
			oF.open("myEvents.txt", ofstream::out | ofstream::trunc);

			// save balance
			oF << currentBalance << endl;

			// save limit
			oF << currentLimit << endl;

			// sort events
			sortEvents();

			// save events
			for (int i = 0; i < events.size(); i++) {
				oF << events[i].day << "\t\t"
					<< events[i].title << "\t\t"
					<< events[i].amount << "\t\t"
					<< events[i].frequency << endl;
			}

			// close file
			oF.close();
		}
	}
	void getCurrentLimit() {
		cout << "Set Limit: $";
		cin >> currentLimit;
		cout << endl;
	}
	void getCurrentBalance() {
		cout << "Set Balance: $";
		cin >> currentBalance;
		cout << endl;
	}
	int getPercentageUsed() {
		int answer = (currentBalance / currentLimit) * 100;
		if (answer < 0) {
			return 0;
		}
		else {
			return answer;
		}
	}
	void showMenu(vector<string> items) {		
		for (int i = 0; i < items.size(); i++) {
			cout << "\t(" << i + 1 << ") " << items[i] << endl;
		}
	}
	void displaySplashScreen() {
		//cout << "=============================================================" << endl;
		cout << endl;
		cout << "OO      OO                                OO                    OO  " << endl;
		cout << "OOOO  OOOO          OOOOOO                OO          OOOOOO    OO  " << endl;
		cout << "OO OOOO OO          OO    OO              OO          OO      OOOOOO" << endl;
		cout << "OO  OO  OO  OO  OO  OOOOOOOO  OO  OO  OOOOOO  OOOOOO  OOOOOO    OO  " << endl;
		cout << "OO      OO   OO OO  OO    OO  OO  OO  OO  OO  OO  OO  OO        OO  " << endl;
		cout << "OO      OO     OO   OOOOOO    OOOOOO  OOOOOO  OOOOOO  OOOOOO    OO  " << endl;
		cout << "--------------OO----------------------------------OO--------------------------->" << endl;
		cout << "             OO                               OOOOOO              " << endl;
		cout << endl;
		cout << "              Created by: Cason Kirschner                    " << endl;
		cout << endl;
		//cout << "=============================================================" << endl;
		cout << endl;
		
		cout << endl;
		system("pause");
		system("cls");
	}
	

public:
	void start() {
		load();
		getTodaysDate();
		displaySplashScreen();
		int selection = 0;
		do {
			system("cls");
			printTitle("Main Menu");
			cout << "Remaining Balance: $" << currentLimit - currentBalance << endl;
			cout << "Current Balance: $" << setprecision(2) << fixed << currentBalance << endl;
			cout << "Current Limit: $" << currentLimit << endl;
			cout << "Percentage Used: " << getPercentageUsed() << "%" << endl;
			
			cout << endl;
			showMenu(mainMenu);
			selection = getMenuSelection(mainMenu.size());
			switch (selection) {
			case 1: break;
			case 2: getCurrentLimit(); break;
			case 3: getCurrentBalance(); break;
			case 4: displayIncomeScreen(); break;
			case 5: displayExpenseScreen();  break;
			case 6: printEventTimeline();  break;
			}
		} while (selection != 1);
		save();
	}
};

int main() {
	BudgetBook bb;
	bb.start();
}