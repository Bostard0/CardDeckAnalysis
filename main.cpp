#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>

class CardDeck {
public:
    explicit CardDeck(int numSuits) : numSuits_(numSuits), currentCardIndex_(0) {
        for (int i = 0; i < numSuits_; ++i) {
            for (int j = 1; j <= 13; ++j) {
                deck_.push_back(j);
            }
        }
        shuffleDeck();
    }

    int operator()() {
        if (currentCardIndex_ >= deck_.size()) {
            shuffleDeck();
            currentCardIndex_ = 0;
        }
        return deck_[currentCardIndex_++];
    }

    // Метод для визначення, чи є карта старшою за попередню
    bool isCardHigher(int currentCard, int previousCard) {
        return currentCard > previousCard;
    }

    // Метод для гри за вказаним правилом
    void playGame(int numCards) {
        std::vector<int> stack;
        int previousCard = 0;

        for (int i = 0; i < numCards; ++i) {
            int currentCard = operator()();
            std::cout << "Отримано карту: " << currentCard << std::endl;

            if (previousCard == 0 || isCardHigher(currentCard, previousCard)) {
                stack.push_back(currentCard);
                previousCard = currentCard;
            } else {
                // Вивести та очистити стопку
                std::cout << "Карти в стопці: ";
                for (int card : stack) {
                    std::cout << card << " ";
                }
                std::cout << std::endl;
                stack.clear();
                
                // Додатковий вивід для розділення стопок
                std::cout << "----------" << std::endl;

                // Додати поточну карту до нової стопки
                stack.push_back(currentCard);
                previousCard = currentCard;
            }
        }

        // Вивести останню стопку карт
        std::cout << "Остання стопка: ";
        for (int card : stack) {
            std::cout << card << " ";
        }
        std::cout << std::endl;
    }

private:
    int numSuits_;
    std::vector<int> deck_;
    int currentCardIndex_;

    void shuffleDeck() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        std::shuffle(deck_.begin(), deck_.end(), std::default_random_engine(std::time(nullptr)));
    }
};

int main() {
    int numSuits;
    int numCards;

    std::cout << "Введіть кількість мастей: ";
    std::cin >> numSuits;

    std::cout << "Введіть кількість карт для роздачі: ";
    std::cin >> numCards;

    CardDeck deck(numSuits);
    deck.playGame(numCards);

    return 0;
}
