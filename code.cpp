#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <limits>

using namespace std;

enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
enum Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

// Conversion helper for string conversions (compatible with all C++ versions)
string intToString(int val) {
    ostringstream ss;
    ss << val;
    return ss.str();
}

class Card {
public:
    Suit suit;
    Rank rank;

    Card(Suit s, Rank r) : suit(s), rank(r) {}

    int getValue() const {
        if (rank >= TWO && rank <= TEN) return static_cast<int>(rank);
        if (rank >= JACK && rank <= KING) return 10;
        return 11; // Ace default
    }

    string toString() const {
        string rankStr;
        switch (rank) {
            case JACK:  rankStr = "J"; break;
            case QUEEN: rankStr = "Q"; break;
            case KING:  rankStr = "K"; break;
            case ACE:   rankStr = "A"; break;
            default:    rankStr = intToString(static_cast<int>(rank)); break;
        }

        string suitStr;
        switch (suit) {
            case HEARTS:   suitStr = "H"; break; // Plain ASCII characters avoid console rendering issues in Dev-C++
            case DIAMONDS: suitStr = "D"; break;
            case CLUBS:    suitStr = "C"; break;
            case SPADES:   suitStr = "S"; break;
        }
        return "[" + rankStr + suitStr + "]";
    }
};

class Deck {
private:
    vector<Card> cards;

public:
    Deck() {
        for (int s = HEARTS; s <= SPADES; ++s) {
            for (int r = TWO; r <= ACE; ++r) {
                cards.push_back(Card(static_cast<Suit>(s), static_cast<Rank>(r)));
            }
        }
    }

    void shuffleDeck() {
        // Random shuffling compatible with C++98 / C++03 / C++11
        for (size_t i = 0; i < cards.size(); ++i) {
            size_t j = i + rand() % (cards.size() - i);
            swap(cards[i], cards[j]);
        }
    }

    Card drawCard() {
        Card drawn = cards.back();
        cards.pop_back();
        return drawn;
    }
};

class Hand {
private:
    vector<Card> cards;

public:
    void addCard(const Card& card) {
        cards.push_back(card);
    }

    int getTotalValue() const {
        int total = 0;
        int aceCount = 0;

        for (size_t i = 0; i < cards.size(); ++i) {
            int val = cards[i].getValue();
            if (val == 11) aceCount++;
            total += val;
        }

        while (total > 21 && aceCount > 0) {
            total -= 10;
            aceCount--;
        }
        return total;
    }

    bool isBlackjack() const {
        return cards.size() == 2 && getTotalValue() == 21;
    }

    void display(bool hideFirstCard = false) const {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (i == 0 && hideFirstCard) {
                cout << "[??] ";
            } else {
                cout << cards[i].toString() << " ";
            }
        }
        if (!hideFirstCard) {
            cout << "(Total: " << getTotalValue() << ")";
        }
        cout << endl;
    }
};

int getValidBet(int balance) {
    int bet = 0;
    while (true) {
        cout << "Enter your wager (Current Balance: $" << balance << "): $";
        if (cin >> bet) {
            if (bet > 0 && bet <= balance) {
                return bet;
            } else if (bet > balance) {
                cout << "Insufficient chips! You only have $" << balance << ".\n";
            } else {
                cout << "Wager must be greater than $0.\n";
            }
        } else {
            cout << "Invalid input. Please enter a valid numerical amount.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(NULL)));

    int chipBalance = 1000;
    char playAgain = 'y';

    cout << "=================================\n";
    cout << "   WELCOME TO CASINO BLACKJACK   \n";
    cout << "=================================\n";

    while ((playAgain == 'y' || playAgain == 'Y') && chipBalance > 0) {
        cout << "\n---------------------------------\n";
        int wager = getValidBet(chipBalance);

        Deck deck;
        deck.shuffleDeck();

        Hand playerHand;
        Hand dealerHand;

        playerHand.addCard(deck.drawCard());
        dealerHand.addCard(deck.drawCard());
        playerHand.addCard(deck.drawCard());
        dealerHand.addCard(deck.drawCard());

        cout << "\nDealer's Hand: ";
        dealerHand.display(true);

        cout << "Your Hand:   ";
        playerHand.display();

        if (playerHand.isBlackjack() || dealerHand.isBlackjack()) {
            cout << "\nDealer reveals card: ";
            dealerHand.display();

            if (playerHand.isBlackjack() && dealerHand.isBlackjack()) {
                cout << "Both hit Blackjack! Push (Tie). Your bet is returned.\n";
            } else if (playerHand.isBlackjack()) {
                int payout = static_cast<int>(wager * 1.5);
                cout << "BLACKJACK! You win 3:2 payout (+$" << payout << ")!\n";
                chipBalance += payout;
            } else {
                cout << "Dealer has Blackjack! You lost $" << wager << ".\n";
                chipBalance -= wager;
            }
        } else {
            char choice;
            bool playerBust = false;
            bool turnEnded = false;

            while (!turnEnded) {
                if (playerHand.getTotalValue() == 21) {
                    cout << "\nYou reached 21!\n";
                    break;
                }

                bool canDouble = (chipBalance >= (wager * 2));
                if (canDouble) {
                    cout << "\nDo you want to (h)it, (s)tand, or (d)ouble down? ";
                } else {
                    cout << "\nDo you want to (h)it or (s)tand? ";
                }

                cin >> choice;

                if (choice == 'h' || choice == 'H') {
                    playerHand.addCard(deck.drawCard());
                    cout << "Your Hand: ";
                    playerHand.display();

                    if (playerHand.getTotalValue() > 21) {
                        cout << "Bust! You went over 21. Lost $" << wager << ".\n";
                        chipBalance -= wager;
                        playerBust = true;
                        turnEnded = true;
                    }
                } else if (choice == 's' || choice == 'S') {
                    turnEnded = true;
                } else if ((choice == 'd' || choice == 'D') && canDouble) {
                    wager *= 2;
                    cout << "Doubling Down! New Wager: $" << wager << ".\n";
                    cout << "Dealer gives you one final card...\n";
                    
                    playerHand.addCard(deck.drawCard());
                    cout << "Your Final Hand: ";
                    playerHand.display();

                    if (playerHand.getTotalValue() > 21) {
                        cout << "Bust! You went over 21 on the double. Lost $" << wager << ".\n";
                        chipBalance -= wager;
                        playerBust = true;
                    }
                    turnEnded = true;
                }
            }

            if (!playerBust) {
                cout << "\nDealer's Turn:\n";
                cout << "Dealer's Hand: ";
                dealerHand.display();

                while (dealerHand.getTotalValue() < 17) {
                    cout << "Dealer hits...\n";
                    dealerHand.addCard(deck.drawCard());
                    cout << "Dealer's Hand: ";
                    dealerHand.display();
                }

                int playerTotal = playerHand.getTotalValue();
                int dealerTotal = dealerHand.getTotalValue();

                cout << "\n--- FINAL RESULT ---\n";
                if (dealerTotal > 21) {
                    cout << "Dealer busts! You win $" << wager << "!\n";
                    chipBalance += wager;
                } else if (playerTotal > dealerTotal) {
                    cout << "You win $" << wager << "! (" << playerTotal << " vs " << dealerTotal << ")\n";
                    chipBalance += wager;
                } else if (dealerTotal > playerTotal) {
                    cout << "Dealer wins! You lost $" << wager << ". (" << dealerTotal << " vs " << playerTotal << ")\n";
                    chipBalance -= wager;
                } else {
                    cout << "Push (Tie)! Bet returned.\n";
                }
            }
        }

        cout << "Current Chip Balance: $" << chipBalance << "\n";

        if (chipBalance <= 0) {
            cout << "\nBankrupt! You have no chips remaining.\n";
            break;
        }

        cout << "\nPlay another hand? (y/n): ";
        cin >> playAgain;
    }

    cout << "\nGame Over! Final Balance: $" << chipBalance << "\n";
    return 0;
}
