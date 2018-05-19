#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

class Card
{
public:
    enum Rank
    {
        UNKNOWN_RANK,
        ACE = 1,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        ACE2  // used for convenience, due to interchangeable nature of Ace, can represent both rank = 1 and rank = 14.
    };

    enum Suit
    {
        UNKNOWN_SUIT,
        HEART = 1,
        DIAMOND,
        CLUB,
        SPADE
    };

    Card()
    {

    }

    Card(Rank r, Suit s)
    {
        this->rank = r;
        this->suit = s;
    }


    void debug() const
    {
        Rank rank = this->rank;


        switch (rank)
        {
            case Card::ACE:
            {
                cout << "A";
                break;
            }

            case Card::TWO:
            case Card::THREE:
            case Card::FOUR:
            case Card::FIVE:
            case Card::SIX:
            case Card::SEVEN:
            case Card::EIGHT:
            case Card::NINE:
            case Card::TEN:
            {
                cout << rank;
                break;
            }

            case Card::JACK:
            {
                cout << "J";
                break;
            }

            case Card::QUEEN:
            {
                cout << "Q";
                break;
            }
            case Card::KING:
            {
                cout << "K";
                break;
            }

            case Card::UNKNOWN_RANK:
            default:
            {
                throw logic_error("Rank is expected but it's correct or not set");
                break;
            }
        }

        Card::Suit suit = this->suit;

        switch (suit)
        {
            case Card::HEART:
            {
                cout << "H ";
                break;
            }
            case Card::DIAMOND:
            {
                cout << "D ";
                break;
            }
            case Card::CLUB:
            {
                cout << "C ";
                break;
            }
            case Card::SPADE:
            {
                cout << "S ";
                break;
            }
            case Card::UNKNOWN_SUIT:
            default:
            {
                throw logic_error("Suit is expected but it's not correct or not  set");
            }
        }

    }

    friend bool operator< (const Card lhs, const Card& rhs)
    {
        int left = lhs.getRank();
        int right = rhs.getRank();

        return left<right;
    }

    friend bool operator> (const Card& lhs, const Card& rhs)
    {
        return rhs < lhs;
    }

    // Following two methods are not used but potentially might be usefull
    friend bool operator==(const Card& lhs, const Card& rhs)
    {
        int left = lhs.getRank();
        int right = rhs.getRank();

        if (left == Card::ACE && right == Card::ACE2
            ||
            left == Card::ACE2 && right == Card::ACE)
            return true;
        else
        {
            if (left == right)
                return true;
            else
                return false;
        }
    }

    friend bool operator!=(const Card& lhs, const Card& rhs)
    {
        return !(lhs == rhs);
    }

    Rank getRank() const { return rank;}
    Suit getSuit() const { return suit;}
    void setRank(Rank rank) { this->rank = rank; };
    void setSuit(Suit suit) { this->suit = suit; };
private:
    Rank rank;
    Suit suit;

};

class Deck
{
public:
    Deck()
    {
        for (int suitIndex = Card::HEART; suitIndex <= Card::SPADE; suitIndex++ )
        {
            for (int rankIndex = Card::ACE; rankIndex <= Card::KING; rankIndex++ )
            {
                cards.push_back(Card((Card::Rank)rankIndex, (Card::Suit)suitIndex));
            }
        }
    }
    ~Deck(){}


    Deck& operator=(const Deck& rhs){}

    int generate(int start, int end)
    {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(start, end);

        return dis(gen);
    }

    void shuffle(int swapCount = 512) {

        for  (int shuffleIndex = 0; shuffleIndex < swapCount; shuffleIndex++)
        {
            int indexToSwap = generate(0, size() - 1);

            Card card = cards.at(indexToSwap);
            cards.erase(cards.begin() + indexToSwap);
            cards.push_back(card);
        }

    };

    void print()
    {
        for (const Card& c : this->cards)
        {
            c.debug();
        }
        cout << endl;
    }

    size_t size()
    {
        return cards.size();
    }

    // For all card just call
    vector<Card> getCards(int number = 52)
    {
        vector<Card> hand;

        if (number <= size()) {
            hand.assign(cards.end() - number, cards.end());
            cards.erase(cards.end() - number, cards.end());
        }
        return hand;
    }

    void handOverCards(vector<Card> c)
    {
        while (c.size() != 0)
        {
            cards.push_back(c.back());
            c.pop_back();
        }
    }

    //checks if all cards passed as parameter have the same suit.
    bool isFlush(vector<Card> hand)
    {
        Card first = hand.front();
        Card::Suit firstSuit = first.getSuit();

        bool flushFlag = true;

        for (auto it = hand.begin() + 1; it != hand.end(); it++)
        {
            if (firstSuit != it->getSuit())
            {
                flushFlag = false;
                break;
            }
        }

        return flushFlag;
    }

    // checks if all 5 cards passed as a parameter have consecutive ranks. Example
    bool isStraight(vector<Card> hand)
    {

        // Exchaning first found Card::Rank::ACE to Card::Rank::ACE2
        vector<Card> hand2 = hand;
        for (auto it = hand2.begin(); it < hand2.end(); it++)
            if (it->getRank() == Card::Rank::ACE)
            {
                it->setRank(Card::Rank::ACE2);
                break;
            }

        return (isStraightInternal(hand) or isStraightInternal(hand2));
    }

private:
    vector<Card> cards;

    bool isStraightInternal(vector<Card> hand)
    {
        sort(hand.begin(), hand.end());

        bool straightFlag = true;

        for (auto it = hand.begin() ; it < hand.end() - 1; it++)
        {
            int current = it->getRank();
            auto next = it + 1;

            if (next->getRank() - current != 1)
            {
                straightFlag= false;
                break;
            }
        }
        return straightFlag;
    }
};

void print(const vector<Card>& cards)
{
    for (const Card& c : cards)
    {
        c.debug();
    }
    cout << endl;
}

vector<Card>& generateTestStraight()
{
    //                                                 Suit is not important in this context
    vector<Card> straight = { Card(Card::Rank::ACE,   Card::Suit::CLUB),
                              Card(Card::Rank::TWO,   Card::Suit::CLUB),
                              Card(Card::Rank::FOUR,  Card::Suit::CLUB),
                              Card(Card::Rank::FIVE,  Card::Suit::CLUB),
                              Card(Card::Rank::THREE, Card::Suit::CLUB) };
    return straight;
}

vector<Card>& generateTestStraight2()
{
    vector<Card> straight = { Card(Card::Rank::ACE,   Card::Suit::CLUB),
                              Card(Card::Rank::TWO,   Card::Suit::CLUB),
                              Card(Card::Rank::KING,  Card::Suit::CLUB),
                              Card(Card::Rank::QUEEN, Card::Suit::CLUB),
                              Card(Card::Rank::TEN,   Card::Suit::CLUB) };
    return straight;
}

int main()
{
    Deck deck;

    bool flushFlag = false;
    bool straightFlag = false;

/*
    straightFlag = deck.isStraight(generateTestStraight());
    if (straightFlag)
        cout << "STRAIGHT\n";

    straightFlag = deck.isStraight(generateTestStraight2());
    if (straightFlag)
        cout << "STRAIGHT\n";
*/

    do
    {
        // First argument of shuffle is swap counts
        deck.shuffle();
        //Deck::print();
        deck.print();

        // pops out number of cards from back of the deck
        auto hand = deck.getCards(5);

        print(hand);

        // Is it flush?
        flushFlag = deck.isFlush(hand);
        if (flushFlag)
            cout << "FLUSH\n";
        else
            cout << "Not FLUSH\n";

        // Is it straight?
        straightFlag = deck.isStraight(hand);
        if (straightFlag)
            cout << "STRAIGHT\n";
        else
            cout << "Not STRAIGHT\n";

        cout << "\n";
        // Puts back cards to the
        deck.handOverCards(hand);
    }
    while (!straightFlag);

    return 0;
}
