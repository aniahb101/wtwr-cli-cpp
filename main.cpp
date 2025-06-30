#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <curl/curl.h>
#include "lib/json.hpp"

using json = nlohmann::json;

// For response buffering
static std::string responseBuffer;

// Handle CURL write response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realSize = size * nmemb;
    responseBuffer.append((char*)contents, realSize);
    return realSize;
}

// HTTP GET using CURL
bool httpGet(const std::string& url, json& result) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    responseBuffer.clear();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) return false;

    try {
        result = json::parse(responseBuffer);
    } catch (...) {
        return false;
    }

    return true;
}

// Fetch user location from IP
bool fetchLocation(double& lat, double& lon, std::string& city) {
    json loc;
    if (!httpGet("http://ip-api.com/json", loc)) return false;
    lat = loc["lat"];
    lon = loc["lon"];
    city = loc["city"];
    return true;
}

// Fetch weather data from OpenWeather API
bool fetchWeatherData(double latitude, double longitude, const std::string& apiKey, json& weatherJson) {
    std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=" +
                      std::to_string(latitude) + "&lon=" + std::to_string(longitude) +
                      "&units=imperial&appid=" + apiKey;
    return httpGet(url, weatherJson);
}

// Determine weather type based on temperature
std::string defineWeatherType(int tempF) {
    if (tempF > 86)
        return "hot";
    else if (tempF >= 66)
        return "warm";
    else
        return "cold";
}

// Determine if current time is between sunrise and sunset
bool isDaytime(long sunrise, long sunset) {
    long now = std::time(nullptr);
    return (sunrise < now && now < sunset);
}

int main() {
    double lat, lon;
    std::string city;
    std::string apiKey;

    // Load API key from config
    std::ifstream configFile("config.json");
    if (!configFile.is_open()) {
        std::cerr << "Error: config.json not found. Please create it with your OpenWeather API key.\n";
        return 1;
    }

    json config;
    configFile >> config;
    apiKey = config["apiKey"];

    // Auto-detect location
    std::cout << "Detecting your location..." << std::endl;
    if (!fetchLocation(lat, lon, city)) {
        std::cerr << "Failed to detect location. Please check your network or use manual input.\n";
        return 1;
    }

    std::cout << "Detected Location: " << city << " (" << lat << ", " << lon << ")\n";

    // Request the weather data
    json weather;
    if (!fetchWeatherData(lat, lon, apiKey, weather)) {
        std::cerr << "Failed to retrieve or parse weather data." << std::endl;
        return 1;
    }

    // Process the weather data
    std::string cityName = weather["name"];
    int tempF = std::round(weather["main"]["temp"].get<double>());
    int tempC = std::round((tempF - 32) * 5.0 / 9.0);
    std::string condition = weather["weather"][0]["main"];
    std::string type = defineWeatherType(tempF);
    long sunrise = weather["sys"]["sunrise"];
    long sunset = weather["sys"]["sunset"];
    bool day = isDaytime(sunrise, sunset);

    // Output
    std::cout << "\n=== Weather Info ===" << std::endl;
    std::cout << "City: " << cityName << std::endl;
    std::cout << "Temperature: " << tempF << "°F / " << tempC << "°C" << std::endl;
    std::cout << "Condition: " << condition << std::endl;
    std::cout << "Time: " << (day ? "Day" : "Night") << std::endl;
    std::cout << "Recommendation: Wear a " << type << " outfit." << std::endl;

    return 0;
}
