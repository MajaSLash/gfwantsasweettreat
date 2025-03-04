#include <iostream>
#include <string>
#include <random>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string YELP_API_KEY = "YOUR_YELP_API_KEY";
const std::string YELP_API_URL = "https://api.yelp.com/v3/businesses/search";

// Function to write the response from Yelp API to a string
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to generate random values for search parameters
int getRandomValue(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// Function to perform the Yelp API search
void searchYelp(std::string term, double latitude, double longitude, int max_distance, double min_rating, int min_review_count) {
    CURL *curl;
    CURLcode res;

    // Prepare the query parameters
    std::string url = YELP_API_URL + "?term=" + term + "&latitude=" + std::to_string(latitude) +
                      "&longitude=" + std::to_string(longitude) + "&radius=" + std::to_string(max_distance) +
                      "&min_rating=" + std::to_string(min_rating) + "&min_review_count=" + std::to_string(min_review_count);

    // Initialize cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the API URL and headers (including API key)
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + YELP_API_KEY).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set up the response handling
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "cURL request failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Parse the response
            try {
                json j = json::parse(response);
                std::cout << "Found " << j["total"] << " businesses:" << std::endl;
                for (const auto& business : j["businesses"]) {
                    std::cout << "Name: " << business["name"] << std::endl;
                    std::cout << "Rating: " << business["rating"] << std::endl;
                    std::cout << "Review Count: " << business["review_count"] << std::endl;
                    std::cout << "Distance: " << business["distance"] << " meters" << std::endl;
                    std::cout << "Address: " << business["location"]["address1"] << std::endl;
                    std::cout << std::endl;
                }
            } catch (const json::exception& e) {
                std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
}

int main() {
    // Set search parameters
    std::string term = "restaurant";  // Search term (e.g., "restaurant")
    double latitude = 37.7749;        // Latitude (e.g., San Francisco)
    double longitude = -122.4194;     // Longitude (e.g., San Francisco)
    
    // Generate randomized parameters for search filters
    int max_distance = getRandomValue(1000, 10000);  // Distance in meters (random between 1 km and 10 km)
    double min_rating = getRandomValue(3, 5);         // Rating (random between 3 and 5)
    int min_review_count = getRandomValue(10, 1000);   // Min reviews count (random between 10 and 1000)

    // Perform Yelp search with randomized inputs
    searchYelp(term, latitude, longitude, max_distance, min_rating, min_review_count);

    return 0;
}