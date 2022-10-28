#include "PokerHand.h"
#include <algorithm>
#include <functional>


bool operator==(const Card& c1, const Card& c2)
{
   return (c1.rank == c2.rank) && (c1.suit == c2.suit);
}

PokerHand::PokerHand(const std::array<Card, 5>& cards)
{
   // Check for duplicates
   if (  (cards[0] == cards[1]) || (cards[0] == cards[2])  || 
         (cards[0] == cards[3]) || (cards[0] == cards [4]) ||
         (cards[1] == cards[2]) || (cards[1] == cards[3])  ||
         (cards[1] == cards[4]) || (cards[2] == cards[3])  ||
         (cards[2] == cards[4]) || (cards[3] == cards[4]))
   {
      throw std::invalid_argument("A Poker hand cannot contain duplicate cards.");
   }

   // Add each of the supplied cards to the hand
   for (const auto& card: cards)
   {
      addCard(card.rank, card.suit);
   }
}

void PokerHand::addCard(Rank r, Suit s)
{
   const size_t index = (s - Hearts)*13 + (r - Ace);
   cards.set(index, true);
}

HandType PokerHand::evaluate() const
{
   // If the hand is one of:
   //
   // Four of a Kind; Full House; Three of a Kind; Two Pair; One Pair
   //
   // then we can determine the hand only by checking the sorted rank
   // histogram for number of instances, starting with the most populous
   // rank for the comparing hand.
   //
   // We don't have to look at the suits for those hands.

   // Build a rank histogram first
   typedef std::array<unsigned, 13> RankHistogram;
   RankHistogram rankHist;
   rankHist.fill(0);
   for (size_t i = 0; i < cards.size(); ++i)
   {
      if (cards.test(i))
      {
         ++rankHist[rankAt(i) - Ace];
      }
   }

   // Build a sorted rank histogram
   // Note that we want to keep the original histogram for later
   RankHistogram sortedRankHist = rankHist;
   std::sort(
      sortedRankHist.begin(),
      sortedRankHist.end(),
      std::greater<unsigned>()
   );

   if ((4 == sortedRankHist[0]) && (1 == sortedRankHist[1]))
   {
      return FourOfAKind;
   }

   if ((3 == sortedRankHist[0]) && (2 == sortedRankHist[1]))
   {
      return FullHouse;
   }

   if (
      (3 == sortedRankHist[0]) &&
      (1 == sortedRankHist[1]) &&
      (1 == sortedRankHist[2])
   )
   {
      return ThreeOfAKind;
   }

   if (
      (2 == sortedRankHist[0]) &&
      (2 == sortedRankHist[1]) &&
      (1 == sortedRankHist[2])
   )
   {
      return TwoPair;
   }

   if (
      (2 == sortedRankHist[0]) &&
      (1 == sortedRankHist[1]) &&
      (1 == sortedRankHist[2]) &&
      (1 == sortedRankHist[3])
   )
   {
      return OnePair;
   }

   // Check if we have a straight by looking for a sequence of adjacent ranks
   bool isStraight = false;
   unsigned adjacentCount = 0;
   auto predicate = [](const unsigned& a, const unsigned& b) {
      return (a == 1) && (b == a);
   };
   auto beg = rankHist[Ace-Ace] ? rankHist.begin() + 1 : rankHist.begin();
   auto it = std::adjacent_find(beg, rankHist.end(), predicate);
   while (rankHist.end() != it)
   {
      ++adjacentCount;
      it = std::adjacent_find(++it, rankHist.end(), predicate);
   }
   if (4 == adjacentCount)
   {
      isStraight = true;
   }
   else if (3 == adjacentCount)
   {
      // Check for a straight with the Ace as the highest card or lowest card
      isStraight =
         // Ace and King are adjacent
         (1 == rankHist[Ace-Ace]) && (1 == rankHist[King-Ace]);

      if (!isStraight)
      {
         // Ace and Two are adjacent
         isStraight = (1 == rankHist[Ace-Ace]) && (1 == rankHist[Two-Ace]);
      }
   }

   // To check for flushes, we need a suit histogram
   typedef std::array<unsigned, 4> SuitHistogram;
   SuitHistogram suitHist;
   suitHist.fill(0);
   for (size_t i = 0; i < cards.size(); ++i)
   {
      if (cards.test(i))
      {
         ++suitHist[suitAt(i) - Hearts];
      }
   }

   // Check if we have a flush
   bool isFlush = false;
   for (const auto count: suitHist)
   {
      if (5 == count)
      {
         isFlush = true;
         break;
      }
   }

   // Check for a Royal or Straight Flush
   if (isFlush && isStraight) // It is a Straight Flush
   {
      // Ace is the high card in the Straight Flush
      if (rankHist[Ace-Ace] && rankHist[King-Ace])
      {
         return RoyalFlush;
      }

      return StraightFlush;
   }

   if (isFlush)
   {
      return Flush;
   }

   if (isStraight)
   {
      return Straight;
   }

   return NoHand;
}

Rank PokerHand::rankAt(size_t index) const
{
   return static_cast<Rank>(Ace + index%13);
}

Suit PokerHand::suitAt(size_t index) const
{
   return static_cast<Suit>(Hearts + (index / 13) % 4);
}
