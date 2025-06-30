# WTWR CLI – Weather-Based Outfit Recommender (C++)

![WTWR CLI Demo](./example.gif)


This is a C++ command-line version of my full-stack **WTWR (What To Wear?)** app.

It auto-detects your current location via IP, fetches the current weather using the **OpenWeather API**, and recommends an outfit based on the temperature and time of day.

---

## 🌤 Example Output

```bash
Detected Location: Baltimore (39.2904, -76.6122)

=== Weather Info ===
City: Baltimore
Temperature: 74°F / 23°C
Condition: Clouds
Time: Day
Recommendation: Wear a warm outfit.



---

## ⚙️ How It Works

- 🌍 Auto-detects your city using `ip-api.com`
- 🌡 Fetches live weather from OpenWeather
- 📦 Parses weather data using `nlohmann/json`
- 🎯 Applies logic to suggest `hot / warm / cold` outfit types based on temp

---

## 🛠 Requirements

- `g++` compiler (C++17 or newer)
- `libcurl` installed:
  - Linux: `sudo apt install libcurl4-openssl-dev`
  - macOS: `brew install curl`
- Internet connection
- API key from [OpenWeather](https://openweathermap.org/api)
- `json.hpp` from [nlohmann/json](https://github.com/nlohmann/json)

---

## 📦 Setup Instructions

### 1. Clone the Repo


git clone https://github.com/YOUR_USERNAME/wtwr-cli-cpp.git
cd wtwr-cli-cpp

2. Download JSON Dependency
Create a lib/ directory and add json.hpp:


mkdir lib
curl -o lib/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
3. Add Your OpenWeather API Key
Create a file named config.json in the root:


{
  "apiKey": "YOUR_OPENWEATHER_API_KEY"
}
🔑 Get your API key from https://openweathermap.org/api


🔧 Compile and Run
Compile:
g++ main.cpp -o wtwr_cli -I./lib -lcurl
Run:
./wtwr_cli

💡 Why I Built This
This project is a CLI-based re-implementation of my original WTWR React + Node.js app. It mirrors the core logic — but uses C++ to:

Learn how APIs work under the hood

Reinforce logic beyond front-end tools

Build something useful in a low-level language

📝 Notes
The app uses your public IP address to detect location

If you’re running from GitHub Codespaces, it may detect Boydton, VA (Azure US East)

All outfit recommendations are based on real-time weather

📎 Credits
OpenWeather API

nlohmann/json

ip-api.com

❗️ Note: `config.json` is gitignored. Please create it manually with your OpenWeather API key:
{
  "apiKey": "YOUR_KEY_HERE"
}
