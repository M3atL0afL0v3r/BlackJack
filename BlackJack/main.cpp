/*
Name: BlackJack
Author: David Gautier
Purpose: A simple program to simulate a game of Blackjack.
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

const int NUM_SUITS = 4;
const int NUM_RANKS = 13;
const int DECK_SIZE = NUM_SUITS * NUM_RANKS;

const std::string suitNames[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
const std::string rankNames[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };

struct Card {
	int suit;
	int rank;

	std::string toString() const
	{
		return rankNames[rank] + " of " + suitNames[suit];
	}

	int getValue() const
	{
		if (rank >= 0 && rank <= 8) return rank + 2;
		if (rank >= 9 && rank <= 11) return 10;
		return 11;
	}
};

struct Player {
	std::string playerName;
	std::vector<std::vector<Card>> hands = { {} };
	std::vector<std::string> statuses = { "Waiting" };
	int currentHand = 0;
	int money = 1000;
	int currentBet = 0;
	std::vector<int> handBets;

	void placeBet(int amount)
	{
		if (amount <= money)
		{
			money -= amount;
			handBets.push_back(amount);
		}
		else
		{
			std::cout << playerName << " doesn't have enough money!\n";
		}
	}

	void winBet(int handIndex)
	{
		money += handBets[handIndex] * 2;
	}

	void pushBet(int handIndex)
	{
		money += handBets[handIndex];
	}

	void loseBet(int handIndex)
	{
		//Money already deducted when bet was placed.
	}

	void addCard(const Card& card, int handIndex)
	{
		if (handIndex >= 0 && handIndex < hands.size())
		{
			hands[handIndex].push_back(card);
		}
	}

	void showHand(int handIndex) const
	{
		std::cout << "Player " << playerName << "'s hand " << (handIndex + 1) << ":\n";
		for (const auto& card : hands[handIndex])
		{
			std::cout << " " << card.toString() << "\n";
		}
	}

	void clearHand(int handIndex)
	{
		if (handIndex >= 0 && handIndex < hands.size()) {
			hands[handIndex].clear();
		}
	}

};

struct Dealer {
	std::vector<Card> hand;
	std::string status = "Waiting";

	void addCard(const Card& card)
	{
		hand.push_back(card);
	}

	void showHand() const
	{
		std::cout << "Dealer's hand:\n";
		for (const auto& card : hand)
		{
			std::cout << " " << card.toString() << "\n";
		}
	}

	void clearHand()
	{
		hand.clear();
	}
};

void buildDeck(Card deck[NUM_SUITS][NUM_RANKS]);
void flattenDeck(Card deck2D[NUM_SUITS][NUM_RANKS], Card flatDeck[DECK_SIZE]);
void shuffleDeck(Card flatDeck[DECK_SIZE]);
void dealCards(Card deck[], std::vector<Player>& players, Dealer& dealer, int& deckIndex);
int calculateHandValue(const std::vector<Card>& hand);
void displayGameBoard(const std::vector<Player>& players, const Dealer& dealer, bool revealDealer);
void playerTurn(Player& player, Card deck[], int& deckIndex);
void dealerTurn(Dealer& dealer, Card deck[], int& deckIndex);
void announceWinner(Dealer& dealer, std::vector<Player>& players);
int getValidatedIntInput(const std::string& prompt);


int main() {
	Card deck2D[NUM_SUITS][NUM_RANKS];
	Card flatDeck[DECK_SIZE];

	std::string stringPlayers;
	int numPlayers = 0;

	std::vector<Player> players;
	Dealer dealer;

	buildDeck(deck2D);
	flattenDeck(deck2D, flatDeck);
	shuffleDeck(flatDeck);

	while (true) {
		numPlayers = getValidatedIntInput("Enter number of players (1-7): ");
		if (numPlayers >= 1 && numPlayers <= 7) break;
		std::cout << "Number of players out of range. Try again.\n";
	}


	for (int i = 0; i < numPlayers; ++i)
	{
		players.push_back(Player{ "Player " + std::to_string(i + 1) });
	}

	int deckIndex = 0;
	dealCards(flatDeck, players, dealer, deckIndex);

	for (auto& player : players) {
		int val = calculateHandValue(player.hands[0]);
		if (val == 21) {
			player.statuses[0] = "BLACKJACK";
			std::cout << player.playerName << " has Blackjack!\n";
		}
	}


	for (auto& player : players)
	{
		std::vector<std::string> statuses = { "Playing" };
		displayGameBoard(players, dealer, false);
		playerTurn(player, flatDeck, deckIndex);
	}

	dealer.status = "Revealing";
	displayGameBoard(players, dealer, true);
	dealerTurn(dealer, flatDeck, deckIndex);
	displayGameBoard(players, dealer, true);
	announceWinner(dealer, players);

	return 0;
}

void buildDeck(Card deck[NUM_SUITS][NUM_RANKS])
{
	for (int s = 0; s < NUM_SUITS; ++s)
	{
		for (int r = 0; r < NUM_RANKS; ++r)
		{
			deck[s][r] = { s, r };
		}
	}
}

void flattenDeck(Card deck2D[NUM_SUITS][NUM_RANKS], Card flatDeck[DECK_SIZE])
{
	int index = 0;
	for (int s = 0; s < NUM_SUITS; ++s)
	{
		for (int r = 0; r < NUM_RANKS; ++r)
		{
			flatDeck[index++] = deck2D[s][r];
		}
	}
}

//Shuffle the cards
void shuffleDeck(Card flatDeck[DECK_SIZE])
{
	std::random_device rd;
	std::mt19937 rng(rd());

	std::shuffle(flatDeck, flatDeck + DECK_SIZE, rng);
}

//Deal the cards
void dealCards(Card deck[], std::vector<Player>& players, Dealer& dealer, int& deckIndex)
{
	for (int i = 0; i < 2; ++i)
	{
		for (auto& player : players) {
			player.addCard(deck[deckIndex++], player.currentHand);
		}
		displayGameBoard(players, dealer, false);
	}

	for (int i = 0; i < 2; ++i)
	{
		dealer.addCard(deck[deckIndex++]);
	}
	displayGameBoard(players, dealer, false);
}

int calculateHandValue(const std::vector<Card>& hand) 
{
	int total = 0;
	int aces = 0;

	for (const auto& card : hand) {
		total += card.getValue();
		if (card.rank == 12) { // Ace
			++aces;
		}
	}

	// Downgrade Aces from 11 to 1 as needed
	while (total > 21 && aces > 0) {
		total -= 10;
		--aces;
	}

	return total;
}

void displayGameBoard(const std::vector<Player>& players, const Dealer& dealer, bool revealDealer)
{
	std::cout << "\n========== Game Board ==========\n";

	//Show dealer
	std::cout << "-Dealer-";
	if (revealDealer)
	{
		for (const auto& card : dealer.hand)
		{
			std::cout << "[" << card.toString() << "] ";
		}
		std::cout << " Total: " << calculateHandValue(dealer.hand);
	}
	else
	{
		std::cout << "[??] ";
		if (dealer.hand.size() > 1)
		{
			std::cout << "[" << dealer.hand[1].toString() << "]";
		}
		std::cout << "Total: ? ? ? ";
	}
	std::cout << " | Status: " << dealer.status << "\n\n";

	//Show players
	for (const auto& player : players)
	{
		for (size_t i = 0; i < player.hands.size(); ++i)
		{
			std::cout << "-" << player.playerName << " (Hand " << (i + 1) << ")-";
			for (const auto& card : player.hands[i])
			{
				std::cout << "[" << card.toString() << "] ";
			}
			std::cout << "\nTotal: " << calculateHandValue(player.hands[i])
				<< " | Status: " << player.statuses[i] << "\n\n";
		}
	}

	std::cout << "================================\n";
}



//Handle player turns
void playerTurn(Player& player, Card deck[], int& deckIndex)
{
	int betAmount = 0;
	while (true) {
		betAmount = getValidatedIntInput(player.playerName + ", you have $" + std::to_string(player.money) + ". Place your bet: ");
		if (betAmount > 0 && betAmount <= player.money) break;
		std::cout << "Invalid bet amount. Try again.\n";
	}
	player.placeBet(betAmount);

	player.currentHand = 0;
	while (player.currentHand < player.hands.size())
	{
		char choice;

		while (true)
		{
			player.showHand(player.currentHand);
			int total = calculateHandValue(player.hands[player.currentHand]);
			std::cout << "Total value: " << total << "\n";

			if (total > 21)
			{
				player.statuses[player.currentHand] = "BUST";
				std::cout << player.playerName << "'s hand " << (player.currentHand + 1) << "busts!\n";
				break;
			}

			if (player.hands[player.currentHand].size() == 2 && player.hands[player.currentHand][0].getValue() == player.hands[player.currentHand][1].getValue())
			{
				std::cout << player.playerName << ", Hit, Stay, or Split? (h/s/p): ";
				std::cin >> choice;

				if (choice == 'h' || choice == 'H') {
					if (deckIndex < DECK_SIZE)
					{
						player.addCard(deck[deckIndex++], player.currentHand);
					}
					else
					{
						std::cout << "Deck is empty! Cannot draw.\n";
						break;
					}
				}
				else if (choice == 's' || choice == 'S')
				{
					player.statuses[player.currentHand] = "STAY";
					break;
				}
				else if (choice == 'p' || choice == 'P')
				{
					// Perform split
					std::vector<Card> newHand = { player.hands[player.currentHand][1] };
					player.hands[player.currentHand].pop_back();
					player.hands.push_back(newHand);
					player.statuses.push_back("Playing");

					// Deal one card to each split hand
					player.addCard(deck[deckIndex++], player.currentHand);
					player.addCard(deck[deckIndex++], player.hands.size() - 1);

					std::cout << "Split successful!\n";
					player.showHand(player.currentHand);
				}
				else
				{
					std::cout << "Invalid Selection. Please try again.\n";
				}
			}
			
			std::cout << player.playerName << ", Hit or Stay? (h/s): ";
			std::cin >> choice;

			if (choice == 'h' || choice == 'H') {
				if (deckIndex < DECK_SIZE)
				{
					player.addCard(deck[deckIndex++], player.currentHand);
				}
				else
				{
					std::cout << "Deck is empty! Cannot draw.\n";
					break;
				}
			}
			else if (choice == 's' || choice == 'S')
			{
				player.statuses[player.currentHand] = "STAY";
				break;
			}
			else
			{
				std::cout << "Invalid Selection. Please try again.\n";
			}
		}

		player.currentHand++;
	}
}

//Handle dealer turn
void dealerTurn(Dealer& dealer, Card deck[], int& deckIndex)
{

	while (calculateHandValue(dealer.hand) < 17)
	{
		std::cout << "Dealer hits...\n";
		dealer.addCard(deck[deckIndex++]);
		dealer.showHand();
	}

	if (calculateHandValue(dealer.hand) > 21)
	{
		dealer.status = "BUST";
		std::cout << "Dealer busts!\n";
	}
	else
	{
		dealer.status = "STAY";
	}
}

void announceWinner(Dealer& dealer, std::vector<Player>& players)
{
	int dealerScore = calculateHandValue(dealer.hand);
	std::cout << "\n======= Results =======\n";
	std::cout << "Dealer's total: " << dealerScore
		<< " (" << dealer.status << ")\n\n";

	for (auto& player : players) {
		for (int i = 0; i < player.hands.size(); ++i) {
			int playerScore = calculateHandValue(player.hands[i]);

			if (playerScore > 21) {
				player.loseBet(i);
				std::cout << player.playerName << " busted and lost $" << player.handBets[i] << ".\n";
			}
			else if (dealerScore > 21 || playerScore > dealerScore) {
				player.winBet(i);
				std::cout << player.playerName << " won and earned $" << player.handBets[i] * 2 << "!\n";
			}
			else if (playerScore == dealerScore) {
				player.pushBet(i);
				std::cout << player.playerName << " pushed. Bet of $" << player.handBets[i] << " returned.\n";
			}
			else {
				player.loseBet(i);
				std::cout << player.playerName << " lost $" << player.handBets[i] << ".\n";
			}
		}

		player.handBets.clear();
	}

	std::cout << "\nFinal Balances:\n";
	for (const auto& player : players) {
		std::cout << player.playerName << ": $" << player.money << "\n";
	}

	std::cout << "=======================\n";
}

int getValidatedIntInput(const std::string& prompt) {
	std::string input;
	int value;

	while (true) {
		std::cout << prompt;
		std::cin >> input;

		try {
			value = std::stoi(input);
			return value;
		}
		catch (const std::invalid_argument&) {
			std::cout << "Invalid input. Please enter a number.\n";
		}
		catch (const std::out_of_range&) {
			std::cout << "Number out of range. Try again.\n";
		}
	}
}

