// Compiler: Microsoft Visual C++ (MSVC)
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>
#include <map>

class Card {
public:
    Card(int value) : value_(value) {}

    auto operator<=>(const Card& other) const {
        return value_ <=> other.value_;
    }

private:
    int value_;
};

class CardDeck {
public:
    explicit CardDeck(int numSuits) : numSuits_(numSuits), currentCardIndex_(0) {
        for (int i = 0; i < numSuits_; ++i) {
            for (int j = 1; j <= 13; ++j) {
                deck_.emplace_back(j);
            }
        }
        shuffleDeck();
    }

    Card operator()() {
        if (currentCardIndex_ >= deck_.size()) {
            shuffleDeck();
            currentCardIndex_ = 0;
        }
        return deck_[currentCardIndex_++];
    }

    // Метод для гри за вказаним правилом та зберігання стопок
    void playGame(int numCards, std::vector<std::vector<Card>>& stacks) {
        Card previousCard(0);

        for (int i = 0; i < numCards; ++i) {
            Card currentCard = operator()();

            if (previousCard < currentCard) {
                stacks.back().push_back(currentCard);
                previousCard = currentCard;
            }
            else {
                // Почати нову стопку
                stacks.push_back({ currentCard });
                previousCard = currentCard;
            }
        }
    }

private:
    int numSuits_;
    std::vector<Card> deck_;
    int currentCardIndex_;

    void shuffleDeck() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        std::shuffle(deck_.begin(), deck_.end(), std::default_random_engine(std::time(nullptr)));
    }
};

int main() {
    int numSuits;
    int numCards;

    bool didReceiveCorrectInput = false;

    while (!didReceiveCorrectInput) {
        try {
            std::cout << "Enter number of  suits: ";
            std::cin >> numSuits;
            if (std::cin.fail()) {
                throw std::invalid_argument("Invalid input for # of suits");
            }
            if (numSuits <= 0)
                throw std::out_of_range("Invalid input for # of suits: it has to be greater then 0");

            std::cout << "Enter number of cards to deal: ";
            std::cin >> numCards;
            if (std::cin.fail()) {
                throw std::invalid_argument("Invalid input for # of cards to deal");
            }
            if (numCards <= 0)
                throw std::out_of_range("Invalid input for # of cards to deal: it has to be greater then 0");
            didReceiveCorrectInput = true;

        }
        catch (std::exception& error)
        {
            std::cout << "There is an error in input: " << error.what() << ". Please, repeat input, # of suits and # of cards to deal" << std::endl;
            std::cout << "have to be integers greater then 0" << std::endl;

            // очищуємо буфер вводу
            while (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
            }
        }

    }

    CardDeck deck(numSuits);
    std::vector<std::vector<Card>> stacks = { {} };  // Починаємо з однієї порожньої стопки

    deck.playGame(numCards, stacks);

    // Знайдемо % стопок з різною довжиною
    std::map<int, int> stackLengthCounts;
    for (const std::vector<Card>& stack : stacks) {
        int stackLength = stack.size();
        stackLengthCounts[stackLength]++;
    }

    int totalStacks = stacks.size();

    // Знайдемо довжину, яка зустрічалась найчастіше
    int mostFrequentLength = 0;
    int maxCount = 0;

    // Знайдемо середню довжину та медіану стопок
    double totalLength = 0.0;
    std::vector<int> stackLengths;
    for (const auto& pair : stackLengthCounts) {
        int length = pair.first;
        int count = pair.second;

        if (count > maxCount) {
            maxCount = count;
            mostFrequentLength = length;
        }

        totalLength += length * count;
        for (int i = 0; i < count; ++i) {
            stackLengths.push_back(length);
        }
    }

    double averageLength = totalLength / totalStacks;

    std::sort(stackLengths.begin(), stackLengths.end());
    double medianLength = (totalStacks % 2 == 0)
        ? (stackLengths[totalStacks / 2 - 1] + stackLengths[totalStacks / 2]) / 2.0
        : stackLengths[totalStacks / 2];
    std::cout << "1. Stack length frequency statistic:" << std::endl;
    for (int i = 1; i <= numCards; i++)
        if (stackLengthCounts[i] > 0)
            std::cout << "\t Length " << i << " : " << (double)stackLengthCounts[i] / totalStacks * 100 << std::endl;
    std::cout << "2. The most frequent length of stack: " << mostFrequentLength << std::endl;
    std::cout << "3. Average length of stack: " << averageLength << std::endl;
    std::cout << "4. Median length of stack: " << medianLength << std::endl;

    return 0;
}