// Include all necessary libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

// Create struct to store player details
struct Player {
    string name;
    string role;
    double price;
    bool isOverseas;
};

// Define constants or default values
vector<Player> players;
double maxBudget = 70.0;
double usedBudget = 0.0;
int numWK = 0, numBAT = 0, numBOWL = 0, numALL = 0, numOverseas = 0;

// Handle case-sensitivities and such edge case
string makeUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Using fstream to write data to file
void saveTeam() {
    ofstream file("players_data.csv");
    if (!file) {
        cout << "Can't open the file to save!" << endl;
        return;
    }
    
    file << "Name,Role,Price,Overseas\n";
    file << fixed << setprecision(2);
    for (const auto &p : players) {
        file << p.name << "," << p.role << "," << p.price << "," << (p.isOverseas ? "YES" : "NO") << "\n";
    }
    file.close();
}

// Displaying file details
void loadTeam() {
    ifstream file("players_data.csv");
    if (!file) {
        cout << "No saved team found. Starting fresh.\n";
        return;
    }

    string line;
    bool isFirstLine = true;

    players.clear();
    usedBudget = 0.0;
    numWK = numBAT = numBOWL = numALL = numOverseas = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        stringstream ss(line);
        string name, role, overseas;
        double price;

        getline(ss, name, ',');
        getline(ss, role, ',');
        ss >> price;
        ss.ignore();
        getline(ss, overseas, ',');

        if (name == "Name" || role == "Role") continue;

        role = makeUpperCase(role);
        bool fromOverseas = (makeUpperCase(overseas) == "YES");

        players.push_back({name, role, price, fromOverseas});
        usedBudget += price;

        if (role == "WK") numWK++;
        else if (role == "BAT") numBAT++;
        else if (role == "BOWL") numBOWL++;
        else if (role == "ALL") numALL++;
        if (fromOverseas) numOverseas++;
    }
    file.close();
}

// Function to add new player
void addPlayer() {
    Player newPlayer;
    cout << "Enter player name: ";
    cin >> ws;
    getline(cin, newPlayer.name);

    cout << "Enter role (WK/BAT/BOWL/ALL): ";
    cin >> newPlayer.role;
    newPlayer.role = makeUpperCase(newPlayer.role);

    cout << "Enter price (in crores): ";
    cin >> newPlayer.price;

    cout << "Is overseas? (YES/NO): ";
    string overseas;
    cin >> overseas;
    newPlayer.isOverseas = (makeUpperCase(overseas) == "YES");

    if (usedBudget + newPlayer.price > maxBudget) {
        cout << "Not enough budget left! Can't add this player." << endl;
        return;
    }

    players.push_back(newPlayer);
    usedBudget += newPlayer.price;

    if (newPlayer.role == "WK") numWK++;
    else if (newPlayer.role == "BAT") numBAT++;
    else if (newPlayer.role == "BOWL") numBOWL++;
    else if (newPlayer.role == "ALL") numALL++;
    if (newPlayer.isOverseas) numOverseas++;

    saveTeam();
    cout << "Player added!" << endl;
}

// Function to show bought-players
void showPlayers() {
    if (players.empty()) {
        cout << "No players in team yet!" << endl;
        return;
    }

    cout << "\nTeam List:\n";
    cout << left << setw(20) << "Name" << setw(10) << "Role" << setw(10) << "Price" << setw(10) << "Overseas" << endl;
    cout << "---------------------------------------------------\n";

    for (const auto &p : players) {
        cout << left << setw(20) << p.name << setw(10) << p.role
             << setw(10) << p.price << setw(10) << (p.isOverseas ? "YES" : "NO") << endl;
    }

    cout << "\nTeam Stats:\n";
    cout << "Budget: " << maxBudget << " Cr | Spent: " << usedBudget << " Cr | Left: " << (maxBudget - usedBudget) << " Cr\n";
    cout << "WK: " << numWK << " | BAT: " << numBAT << " | BOWL: " << numBOWL << " | ALL: " << numALL << " | Overseas: " << numOverseas << "\n";
}

// Function to remove specified player
void removePlayer() {
    cout << "Enter player name to remove: ";
    string name;
    cin >> ws;
    getline(cin, name);

    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it->name == name) {
            usedBudget -= it->price;
            if (it->role == "WK") numWK--;
            else if (it->role == "BAT") numBAT--;
            else if (it->role == "BOWL") numBOWL--;
            else if (it->role == "ALL") numALL--;
            if (it->isOverseas) numOverseas--;

            players.erase(it);
            saveTeam();
            cout << "Player removed!" << endl;
            return;
        }
    }
    cout << "Couldn't find that player!" << endl;
}

int main() {
    loadTeam();
    int choice;
    while (true) {
        cout << "\n=== Cricket Team Manager ===\n";
        cout << "1. Add Player\n2. Show Team\n3. Remove Player\n4. Quit\nWhat do you want to do? ";
        cin >> choice;
        
        if (choice == 1) addPlayer();
        else if (choice == 2) showPlayers();
        else if (choice == 3) removePlayer();
        else if (choice == 4) {
            cout << "Exiting program. Bye!" << endl;
            break;
        } else {
            cout << "Invalid choice. Try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return 0;
}