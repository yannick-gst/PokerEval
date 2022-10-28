#pragma once

#include <array>
#include <bitset>


enum HandType
{
   NoHand,
   OnePair,
   TwoPair,
   ThreeOfAKind,
   Straight,
   Flush,
   FullHouse,
   FourOfAKind,
   StraightFlush,
   RoyalFlush
};

enum Rank
{
   Ace = 1,
   Two,
   Three,
   Four,
   Five,
   Six,
   Seven,
   Eight,
   Nine,
   Ten,
   Jack,
   Queen,
   King
};

enum Suit
{
   Hearts = 1,
   Spades,
   Diamonds,
   Clubs
};

struct Card
{
   Rank rank = Ace;
   Suit suit = Hearts;
};

bool operator==(const Card& c1, const Card& c2);

class PokerHand
{
public:

   PokerHand(const std::array<Card, 5>& cards); // Throws if duplicates are found
   HandType evaluate() const;

private:

   // The first set of 13 bits holds Hearts
   // The second set of 13 bits holds Spades
   // The third set of 13 bits holds Diamonds
   // The last set of 13 bits holds Clubs
   std::bitset<52> cards;

   void addCard(Rank r, Suit s);

   // Returns the rank at an index in the card bitset
   Rank rankAt(size_t index) const;

   // Returns the suit at an index in the card bitset
   Suit suitAt(size_t index) const;
};
