/*
    One Armed Bandit - Programming Fundamentals with C++
    Copyright (C) 2016 David Boya <david@boya.se>
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
// include necessary libraries so we can use functions that are needed for this program.
#include <iostream>
#include <ctime>

// search the std library so we can skip adding the "std::" prefix for input and output streams.
using namespace std;

// define functions in this program.
void spinBandit(char array[3][3]);
void printBandit(char array[3][3]);
int countWins(int type, char array[3][3]);

int main()
{
	// define game variables.
	char banditArray[3][3];

	int account = 0;
	int add_to_account;
	int bet;

	string play_again_choice;
	string add_more_choice;

	bool play_again;
	bool add_money_again;
	bool error;

	// generate a seed from current unix timestamp.
	srand(time(0));

	// welcome message to the player.
	cout << "[One Armed Bandit] Welcome to One Armed Bandit!" << endl;
	cout << "[One Armed Bandit] You need to insert money to play." << endl;

	cout << endl << "* * * * * * * Add money * * * * * * *" << endl;
	// loop: handle add money.
	do
	{
		// loop: add money to account, 3 choices. 20, 100, 500.
		do
		{
			cout << ">> Choose how much money you want to add to your account (20 | 100 | 500): ";
			cin >> add_to_account;
			
			if((add_to_account != 20) && (add_to_account != 100) && (add_to_account != 500))
			{
				// input is invalid, value must be either '20', '100' or '500'.
				cout << "[One Armed Bandit] Invalid amount. You can only add 20, 100 or 500." << endl;
				error = true;
			}
			else if((account + add_to_account) > 1000)
			{
				// input is invalid, player can only start with a total of 1000 kr in his account.
				cout << "[One Armed Bandit] Invalid amount. You may only add " << 1000 - account << " kr more to your account." << endl;
				error = true;
			}
			else
			{
				// input is valid, add money to player account and output transaction summary, then continue.
				account += add_to_account;
				cout << "[One Armed Bandit] You added " << add_to_account << " kr to your account. Your account balance is now " << account << " kr." << endl;
				error = false;
			}
		}
		while(error);

		if(account <= 980)
		{
			// player is still allowed to add minimum 20 kr to his account. ask if he wants to add more money.
			// loop: add money again choice.
			do
			{
				cout << ">> Do you want to add more money? (y(es) | n(o)): ";
				cin >> add_more_choice;

				if(add_more_choice == "y" || add_more_choice == "yes")
				{
					// player wants to add more money.
					add_money_again = true;
					error = false;
				}
				else if(add_more_choice == "n" || add_more_choice == "no")
				{
					// player doesn't want to add more money.
					add_money_again = false;
					error = false;
				}
				else
				{
					// input is invalid, input must be either 'y', 'yes', 'n' or 'no'.
					cout << "[One Armed Bandit] Invalid choice. Please answer with 'y' / 'yes' or 'n' / 'no'." << endl;
					error = true;
				}
			}
			while(error);
		}
		else
		{
			// player cant add more money to his account since maximum is 1000 kr, continue.
			cout << "[One Armed Bandit] You've reached the maximum amount of money that you can add to your account." << endl;
			add_money_again = false;
		}
	}
	while(add_money_again);
	cout << "* * * * * * * * * * * * * * * * * * *" << endl;

	// loop: game loop.
	do
	{
		cout << endl << "* * * * * * * The bet * * * * * * * *" << endl;
		cout << "[One Armed Bandit] Your account balance is " << account << " kr." << endl;

		// loop: choose bet amount.
		do
		{
			cout << ">> Choose a bet amount (1 - " << ((account >= 50) ? 50 : account) << "): ";
			cin >> bet;

			if(bet < 1 || bet > ((account >= 50) ? 50 : account))
			{
				// input is invalid, input must be an integer between 1 - 50 (or account if account < 50)
				cout << "[One Armed Bandit] Invalid choice. Please answer with an integer between 1 - " << ((account >= 50) ? 50 : account) << "." << endl;
				error = true;
			}
			else if(bet > account)
			{
				// input is invalid, player doesn't have enough money to make the bet.
				cout << "[One Armed Bandit] Invalid amount. You dont have that much money. Your account balance is " << account << " kr." << endl;
				error = true;
			}
			else
			{
				// input is valid, output bet summary and subtract money from player account, then continue.
				cout << "[One Armed Bandit] Bet summary: You bet " << bet << " kr." << endl;
				account -= bet;
				error = false;
			}
		}
		while(error);
		cout << "* * * * * * * * * * * * * * * * * * *" << endl << endl;

		cout << "* * * * * * Bandit result * * * * * *" << endl << endl;
		// spin the bandit. this function populates the array banditArray with random letters.
		spinBandit(banditArray);

		// print the bandit result. this function prints out content of the array banditArray.
		printBandit(banditArray);

		// count row wins, column wins and diagonal wins.
		int rows = countWins(0, banditArray);
		int columns = countWins(1, banditArray);
		int diagonals = countWins(2, banditArray);
		
		// sum up row wins, column wins and diagonal wins.
		int wins = rows + columns + diagonals;

		if(wins > 0)
		{
			int money_won = 0;

			// determine how much player won according to payout table.
			if(wins >= 1 && wins <= 6)
			{
				money_won = bet * (1 << wins);
			}
			else if(wins >= 7)
			{
				money_won = bet * 128;
			}

			// print out game results
			cout << "[One Armed Bandit] Rows: " << rows << " wins." << endl;
			cout << "[One Armed Bandit] Columns: " << columns << " wins." << endl;
			cout << "[One Armed Bandit] Diagonals " << diagonals << " wins." << endl;
			cout << "[One Armed Bandit] Total wins: " << wins << " times." << endl;
			cout << "[One Armed Bandit] You won " << money_won << " SEK. Congratulations!" << endl;

			// transfer the money to the players' account.
			account += money_won;
		}
		else
		{
			cout << "[One Armed Bandit] No rows, columns or diagonal wins." << endl;
			cout << "[One Armed Bandit] You lost " << bet << " kr. Bad luck!" << endl;
		}
	
		// print out the current account balance.
		cout << "[One Armed Bandit] Your account balance is now " << account << " kr." << endl;
		cout << "* * * * * * * * * * * * * * * * * * *" << endl;

		if(account > 0)
		{
			// player has enough money to make the minimum bet (1) ask player if he wants to play again.
			// loop: play again
			do
			{
				cout << ">> Do you want to play again? (y(es) | n(o)): ";
				cin >> play_again_choice;

				if(play_again_choice == "y" || play_again_choice == "yes")
				{
					// player wants to play again.
					play_again = true;
					error = false;
				}
				else if(play_again_choice == "n" || play_again_choice == "no")
				{
					// player doesn't want to play again.
					play_again = false;
					error = false;
				}
				else
				{
					// input is invalid, input must be either 'y', 'yes', 'n' or 'no'.
					cout << "[One Armed Bandit] Invalid choice. Please answer with 'y' / 'yes' or 'n' / 'no'." << endl;
					error = true;
				}
			}
			while(error);
		}
		else
		{
			// player doesn't have enough money to make minimum bet, redundant to ask player if he wants to play again, end the game.
			cout << "[One Armed Bandit] Insufficient amount of money to make a new bet." << endl;
			play_again = false;
		}
	}
	while(play_again);
	
	// print out goodbye message.
	cout << "[One Armed Bandit] Thanks for playing One Armed Bandit. Welcome back later!" << endl;
	return 0;
}

// fill the array randomly with the symbols O, P and X
void spinBandit(char array[3][3])
{
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			switch(rand() % 3)
			{
				case 0:
					array[x][y] = 'O';
					break;
				case 1:
					array[x][y] = 'P';
					break;
				case 2:
					array[x][y] = 'X';
					break;
			}
		}
	}
}

// print the result formated to stdout
void printBandit(char array[3][3])
{
	for(int x = 0; x < 3; x++)
	{
		cout << "              ";
		for(int y = 0; y < 3; y++)
		{
			cout << "[" << array[x][y] << "]";
		}
		cout << endl;
	}
	cout << endl;
}

// count the number of winning lines in an array. 3 types ( 0 = rows | 1 = columns | 2 = diagonals )
int countWins(int type, char array[3][3])
{
	int wins = 0;

	switch(type)
	{
		case 0: // row
			for(int x = 0; x < 3; x++)
			{
				if(array[x][0] == array[x][1] && array[x][1] == array[x][2])
				{
					wins++;
				}
			}

			break;
		case 1: // columns
			for(int x = 0; x < 3; x++)
			{
				if(array[0][x] == array[1][x] && array[1][x] == array[2][x])
				{
					wins++;
				}
			}

			break;
		case 2: // diagonals
			// Diagonal win #1
			// *		(0,0)
			//   *		(1,1)
			//     *	(2,2)
			if(array[0][0] == array[1][1] && array[1][1] == array[2][2])
			{
				wins++;
			}

			// Diagonal win #2:
			//     *	(0,2)
			//   *		(1,1)
			// *		(2,0)
			if(array[0][2] == array[1][1] && array[1][1] == array[2][0])
			{
				wins++;
			}

			break;
	}

	return wins;
}
