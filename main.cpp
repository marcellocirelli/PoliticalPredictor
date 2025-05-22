#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

// Changes & Fixes:
// - Removed the hardcoded scoring system and it's functions
// - Added a Naive Bayes algorithm to predict and learn
// - Moved the survey questions into its own function to clean up int main()
// - Updated the output code to show your percentage of each party member based on learning
// - Thank you for the feedback, I hadn't read through the instructions properly!

// Structure to hold each question and its options
struct Question {
    std::string text;
    std::vector<std::string> options;
    std::vector<std::map<std::string, int>> weights; // not used yet
};

// These are the 8 + 1 questions and their choices. They get pushed into a vector
std::vector<Question> buildSurveyQuestions() {
    std::vector<Question> questions;
    // Question 1: Income assistance
    Question q1;
    q1.text = "What should the government's role be in assisting those of lower income?";
    q1.options = {
        "Expand welfare programs and make them easier to access",
        "Encourage educational choice (private, public, charter) and job training",
        "Minimize spending and promote private charity",
        "Implement universal basic income"
    };
    q1.weights = {
        {{"Democrat", 2}, {"Green", 2}},
        {{"Democrat", 1}, {"Libertarian", 1}},
        {{"Republican", 1}, {"Libertarian", 2}},
        {{"Libertarian", 2}, {"Republican", 1}}
    };
    questions.push_back(q1);

    // Question 2: Healthcare
    Question q2;
    q2.text = "What is your stance on healthcare?";
    q2.options = {
        "Everyone should have access to universal health care",
        "Everyone should have the option to choose between public and private health care",
        "Keep healthcare private with minimal government involvement",
        "Eliminate government involvement in healthcare"
    };
    q2.weights = {
        {{"Democrat", 2}, {"Green", 2}},
        {{"Democrat", 1}, {"Libertarian", 1}},
        {{"Republican", 1}, {"Libertarian", 2}},
        {{"Libertarian", 2}, {"Republican", 1}}};
    questions.push_back(q2);

    // Question 3: Climate change
    Question q3;
    q3.text = "What is your stance on climate change?";
    q3.options = {
        "Climate change is overblown fake news",
        "It is of utmost importance, and migration to green energy is paramount",
        "Climate change should be handled by private groups",
        "Climate change is important, and should be more regulated"
    };
    q3.weights = {
        {{"Republican", 2}},
        {{"Green", 2}, {"Democrat", 1}},
        {{"Libertarian", 2}},
        {{"Democrat", 2}, {"Green", 1}}
    };
    questions.push_back(q3);

    // Question 4: Gun Control
    Question q4;
    q4.text = "What is your stance on gun control?";
    q4.options = {
        "It is our constitutional right to own firearms",
        "Background checks should be improved, and there should be a ban on assault rifle sales",
        "Gun sellers/distributors should handle who gets to buy guns and who doesn't",
        "There is no place for guns in today's society"
    };
    q4.weights = {
        {{"Republican", 2}, {"Libertarian", 2}},
        {{"Democrat", 2}},
        {{"Libertarian", 1}},
        {{"Green", 2}, {"Democrat", 1}}
    };
    questions.push_back(q4);

    // Question 5: Education
    Question q5;
    q5.text = "What is your stance on education?";
    q5.options = {
        "We should invest more in our education, and teachers should be paid more",
        "Charter schools and private education are the way to go if you can afford it",
        "Public schools, private schools, and public education should all live in harmony",
        "Public schools should be more heavily regulated, and should ban methods that go against the American way"
    };
    q5.weights = {
        {{"Democrat", 2}, {"Green", 1}},
        {{"Republican", 1}, {"Libertarian", 1}},
        {{"Libertarian", 1}, {"Democrat", 1}},
        {{"Republican", 2}}
    };
    questions.push_back(q5);

    // Question 6: Immigration
    Question q6;
    q6.text = "What is your stance on immigration?";
    q6.options = {
        "All illegal immigrants should be deported",
        "Immigration should be better regulated, including increasing border control",
        "Immigrants should have current paths to open citizenship, but shouldn't take away from the jobs of birthright citizens",
        "Immigration is what built this country, and they should be accepted with open arms"
    };
    q6.weights = {
        {{"Republican", 2}},
        {{"Republican", 1}, {"Libertarian", 1}},
        {{"Democrat", 1}},
        {{"Democrat", 2}, {"Green", 2}}};
    questions.push_back(q6);

    // Question 7: Social rights
    Question q7;
    q7.text = "What is your stance on LGBTQ+ rights?";
    q7.options = {
        "LGBTQ+ citizens are like any other citizen and deserve equal rights and protections",
        "States and business should decide on their own how to handle their citizen/employee rights",
        "Being LGBTQ+ is against the bible",
        "I have no strong opinions, as long as they don't push their agendas on me"
    };
    q7.weights = {
        {{"Democrat", 2}, {"Green", 2}},
        {{"Libertarian", 1}},
        {{"Republican", 2}},
        {{"Republican", 1}, {"Libertarian", 1}}
    };
    questions.push_back(q7);

    // Question 8: AI and free speech
    Question q8;
    q8.text = "What are your opinions on AI and free speech";
    q8.options = {
        "The government should heavily regulate the use of AI and protect against misuse",
        "The government should censor anything that goes against the bible and constitution",
        "Businesses like Facebook and OpenAI should be free to use AI however they please",
        "I don't really care"
    };
    q8.weights = {
        {{"Democrat", 1}, {"Green", 2}},
        {{"Republican", 2}},
        {{"Libertarian", 2}, {"Republican", 1}},
        {{"Libertarian", 1}}
    };
    questions.push_back(q8);

    // Final Question: User's self-proclaimed political party
    Question q9;
    q9.text = "Which political party do you most identify with?";
    q9.options = {
        "Democrat",
        "Republican",
        "Libertarian",
        "Green"
    };
    q9.weights = {
        {{"Democrat", 2}},
        {{"Republican", 2}},
        {{"Libertarian", 2}},
        {{"Green", 2}}
    };
    questions.push_back(q9);
    return questions;
}

// Reads past responses and returns a vector for training
std::vector<std::vector<char>> readPartyFile(const std::string& filepath) {
    std::ifstream file(filepath);
    std::vector<std::vector<char>> data;
    std::string line;

    while (std::getline(file, line)) {
        std::vector<char> answers;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ',')) {
            if (!token.empty()) answers.push_back(token[0]);
        }
        data.push_back(answers);
    }

    return data;
}

// Gets the user's answer to each question
void getUserInput(const Question &q, char &choice) {
    while (true) {
        std::cout << "Your choice (A-D): ";
        std::cin >> choice;
        choice = static_cast<char>(toupper(choice));
        if (choice >= 'A' && choice < 'A' + q.options.size()) {
            break; // valid input, exit loop
        } else {
            std::cout << "Invalid choice. Please enter A, B, C, or D.\n";
        }
    }
}

// Asks the user for their self-declared political party
void getUserParty(const std::vector<Question> &questions, std::string &declaredParty) {
    const Question& finalQ = questions.back();

    std::cout << "\n" << finalQ.text << "\n";
    for (size_t i = 0; i < finalQ.options.size(); ++i) {
        std::cout << static_cast<char>('A' + i) << ". " << finalQ.options[i] << "\n";
    }

    char finalChoice;
    while (true) {
        std::cout << "Your choice (A-D): ";
        std::cin >> finalChoice;
        finalChoice = static_cast<char>(toupper(finalChoice));

        if (finalChoice >= 'A' && finalChoice < 'A' + finalQ.options.size()) {
            break; // valid input, exit loop
        } else {
            std::cout << "Invalid choice. Please enter A, B, C, or D.\n";
        }
    }
    if (finalChoice >= 'A' && finalChoice < 'A' + finalQ.options.size()) {
        declaredParty = finalQ.options[finalChoice - 'A'];
        std::cout << "Thanks! You declared: " << declaredParty << "\n";
    } else {
        std::cout << "Invalid response.\n";
    }
}

// Saves the user's answers into a file corresponding to their declared party in question 9
void saveAnswersByParty(const std::string& party, const std::vector<char>& choices) {
    std::ofstream file("../" + party + ".txt", std::ios::app);  // Append mode
    for (size_t i = 0; i < choices.size(); ++i) {
        file << choices[i];
        if (i != choices.size() - 1) file << ",";
    }
    file << "\n";
}

// Calculates the probability of a user's political alignment using Naive Bayes
double calcNaiveBayesScore(const std::vector<char>& userAnswers, const std::vector<std::vector<char>>& partyData, int answerRange = 4) {
    if (partyData.empty()) return 0.0;

    double score = 1.0;

    size_t numQuestions = userAnswers.size();

    for (int i = 0; i < numQuestions; ++i) {
        int matchCount = 0;

        for (const auto& response : partyData) {
            if (i < response.size() && response[i] == userAnswers[i]) {
                matchCount++;
            }
        }

        double probability = (matchCount + 1.0) / (static_cast<double>(partyData.size()) + static_cast<double>(answerRange));
        score *= probability;
    }

    return score;
}

// Calls calcNaiveBayesScore() and returns the predicted political party with the highest likelihood
std::string predictFromData(
    const std::vector<char>& userAnswers,
    const std::map<std::string, std::vector<std::vector<char>>>& historicalData
)

    {
    std::map<std::string, double> rawScores;
    double totalScore = 0.0;

    for (const auto& [party, data] : historicalData) {
        double score = calcNaiveBayesScore(userAnswers, data);
        rawScores[party] = score;
        totalScore += score;
    }

    // Normalize and print as percentages
    for (const auto& [party, score] : rawScores) {
        double percent = (totalScore == 0.0) ? 0.0 : (score / totalScore * 100.0);
        std::cout << "Likelihood you are a " << party << " party member: " << std::fixed << std::setprecision(2) << percent << "%\n";
    }

    // Find top-scoring party
    std::string bestParty;
    double bestScore = -1.0;
    for (const auto& [party, score] : rawScores) {
        if (score > bestScore) {
            bestScore = score;
            bestParty = party;
        }
    }


    return bestParty;
}

// Conducts the entire process
void runSurvey(const std::vector<Question>& questions) {
    std::cout << "Survey started...\n";
    std::vector<char> userAnswers;

    std::map<std::string, std::vector<std::vector<char>>> historicalData = {
        {"Democrat", readPartyFile("../Democrat.txt")},
        {"Republican", readPartyFile("../Republican.txt")},
        {"Libertarian", readPartyFile("../Libertarian.txt")},
        {"Green", readPartyFile("../Green.txt")}
    };

    for (size_t i = 0; i < questions.size() - 1; ++i) {
        const Question& q = questions[i];

        std::cout << "\nQ" << i + 1 << ": " << q.text << "\n";

        for (size_t j = 0; j < q.options.size(); ++j) {
            char optionLetter = static_cast<char>('A' + static_cast<int>(j));
            std::cout << optionLetter << ". " << q.options[j] << "\n";
        }

        // Get the users input
        char choice;
        getUserInput(q, choice);
        userAnswers.push_back(choice);
    }

    // Ask the user for their actual political affiliation
    std::string declaredParty;
    getUserParty(questions, declaredParty);
    saveAnswersByParty(declaredParty, userAnswers);

    // Predict the party with the highest score
    std::string predictedParty = predictFromData(userAnswers, historicalData);
    std::cout << "\nBased on learned data, we predict you most align with: " << predictedParty << "\n";
}

int main() {
    runSurvey(buildSurveyQuestions());
    return 0;
}
