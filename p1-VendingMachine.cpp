// Link Parrish
// Tim Butts
// Trey Roberts
// Christian Grandy


// This program simulates the payment section of a vending machine.
// The vending machine only dispenses sodas which are priced at $1.30

#include <iostream>
using namespace std;

// Create an enumeration data type called Money
enum class Money { nickel = 5, dime = 10, quarter = 25, dollar = 100 };

const int MAXSIZE = 26;
const int PRICE = 130;

// Prints price to the screen in the format: $X.XX
// price represents an amount of money, in pennies
void PrintPrice(int price);

// Takes user input (1, 2, 3, 4) and converts it to a Money type
Money ConvertToMoney(int x);

// Prints a word to the screen stating what the Money type is
void PrintMoney(Money x);

// Prints the coins to dispense, which add up to amount
// Note: This will never need to dispense a dollar
void DispenseChange(int amount);

int main() {

	// declare money array and nessary veriables for main function
	Money money[MAXSIZE];
	int len = 0, amount = 0, input;

	// prints main menu
	cout << "vending machine\n\n";

	cout << "deposit: ";
	PrintPrice(PRICE);
	cout << " for a soda\n\n";
	cout << "1: deposit nickel   2.deposit dime   3. deposit quarter   4. deposit dollar   5. cancel transaction\n\n";

	// Loops until price is payed or they cancel 
	while (amount < PRICE) {

		cout << "enter number: ";
		cin >> input;

		// checks for non-integers
		while (cin.fail()) {

			// resets cin and clears the input buffer
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "error: not an integer\n\n";
			cout << "enter number: ";
			cin >> input;
		}

		// if the input is a deposist 
		if (input  > 0 && input < 5) {
			money[len] = ConvertToMoney(input);
			len++;

			// calculates anount deposited so far
			amount = 0;
			for (int i = 0; i < len; i++) amount += (int)money[i];
		}

		// if statment when the transaction is cancelled. Return change if necessary 
		else if (input == 5) {

			cout << "canceling transaction\n";

			// if statement to return change upon cancelation  
			if (len > 0) {
				cout << "returning change...\n> ";
				for (int i = 0; i < len; i++) PrintMoney(money[i]);
			}

			cout << endl;
			return 0;
		}

		else {
			cout << "error: invalid input\n\n";
		}

		// displays amount left to pay if there is any left to pay
		if (amount < PRICE) {
			cout << "amount left to pay: ";
			PrintPrice(PRICE - amount);
			cout << "\n\n";
		}
	}

	cout << "\ndispensing soda...\n\n";
	
	// if statement to dispense left over
	if (amount > PRICE) {
		cout << "change owed: ";
		PrintPrice(amount - PRICE);
		cout << endl;

		cout << "returning change...\n> ";
		DispenseChange(amount - PRICE);
	}

	cout << endl;
	return 0;
}

void PrintPrice(int price) {

	// prints the price given in the format $x.xx
	printf("$%d.%.2d", price / 100, price % 100);
}

void DispenseChange(int amount) {

	// initalize required integers for the function
	int quarters = 0, dimes = 0, nickels = 0;

	// while loop finds the biggest piece of change that can be returned
	while (amount > 0) {

		// check to see if we can return a quarter
		// if so, return a quarter and subtract 25 from the amount left to return
		if (amount >= 25) {
			amount -= 25;
			PrintMoney(Money::quarter);
		}

		// check to see if we can return a dime
		// if so, return a dime and subtract 10 from the amount left to return
		else if (amount >= 10) {
			amount -= 10;
			PrintMoney(Money::dime);
		}

		// check to see if we can return a nickel
		// if so, return a nickel and subtract 5 from the amount left to return
		else if (amount >= 5) {
			amount -= 5;
			PrintMoney(Money::nickel);
		}
	}
}

Money ConvertToMoney(int x) {

	// simple switch statement to return the right money type based on the number provided
	switch (x) {
	case 1: return Money::nickel;
	case 2: return Money::dime;
	case 3: return Money::quarter;
	case 4: return Money::dollar;
	}
}

void PrintMoney(Money x) {

	// simple switch statemet to print the name of the money type provided
	switch (x) {

	case Money::nickel:
		printf("nickel ");
		break;
	
	case Money::dime:
		printf("dime ");
		break;

	case Money::quarter:
		printf("quarter ");
		break;

	case Money::dollar:
		printf("dollar ");
		break;
	}
}