#include "PokerHand.h"

#include <fstream>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

void splitString(
   const std::string& s,
   char delim,
   std::vector<std::string>& tokens
)
{
   auto i = 0;
   auto pos = s.find(delim);
   while (pos != std::string::npos)
   {
      tokens.push_back(s.substr(i, pos-i));
      i = ++pos;
      pos = s.find(delim, pos);
      if (pos == std::string::npos)
      {
         tokens.push_back(s.substr(i, s.length()));
      }
   }
}

std::array<Card, 5> parseInputLine(const std::string& inputLine)
{
   std::vector<std::string> tokens;
   splitString(inputLine, ',', tokens);

   // Read the cards at hand
   std::array<Card, 5> cards;
   cards[0].suit = static_cast<Suit>(std::stoi(tokens[0], nullptr));
   cards[0].rank = static_cast<Rank>(std::stoi(tokens[1], nullptr));
   cards[1].suit = static_cast<Suit>(std::stoi(tokens[2], nullptr));
   cards[1].rank = static_cast<Rank>(std::stoi(tokens[3], nullptr));
   cards[2].suit = static_cast<Suit>(std::stoi(tokens[4], nullptr));
   cards[2].rank = static_cast<Rank>(std::stoi(tokens[5], nullptr));
   cards[3].suit = static_cast<Suit>(std::stoi(tokens[6], nullptr));
   cards[3].rank = static_cast<Rank>(std::stoi(tokens[7], nullptr));
   cards[4].suit = static_cast<Suit>(std::stoi(tokens[8], nullptr));
   cards[4].rank = static_cast<Rank>(std::stoi(tokens[9], nullptr));

   return cards;
}

int main(int argc, char* argv[])
{
   if (argc < 3)
   {
      std::cerr << "Expected at least two arguments. Aborting..." << std::endl;
      return 1;
   }

   try
   {
      std::ifstream is(argv[1]);
      std::ofstream os(argv[2]);
      std::string inputLine;
      while (std::getline(is, inputLine))
      {
         PokerHand hand(parseInputLine(inputLine));
         os << PokerHand(parseInputLine(inputLine)).evaluate() << std::endl;
      }
   }
   catch (const std::exception& e)
   {
      std::cerr << e.what() << std::endl;
      return 1;
   }
   catch (...)
   {
      std::cerr << "An unexpected error has occurred..." << std::endl;
      return 1;
   }

   return 0;
}
