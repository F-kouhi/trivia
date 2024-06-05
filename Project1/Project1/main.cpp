#include<iostream>
#include<curl/curl.h>
#include <fstream>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://opentdb.com/api.php?amount=3&category=31&difficulty=easy&type=multiple");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::cout << "Response received successfully:\n";
            
            json j = json::parse(readBuffer);

            for (const auto& result : j["results"])
            {
                cout << "category: " << result["category"] << "\n";
                cout << "question: " << result["question"] << "\n";
                cout << "correct answers: \n";
                for (const auto& correct : result["correct_answer"])
                {
                    cout << correct << "\n";
                }
                cout << "incorrect answers: \n";
                for (const auto& incorrect : result["incorrect_answers"])
                {
                    cout << incorrect << "\n";
                }
                cout << "___________________________________________\n";
            }
            
        }

        curl_easy_cleanup(curl);
    }

    return 0;
}