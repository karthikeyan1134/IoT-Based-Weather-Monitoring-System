#ifndef MYHEADER_H
#define MYHEADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;
// Structure to hold city data
struct City {
    string name;
    double latitude;
    double longitude;
};

// Function to calculate distance using Haversine formula
double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371; // Earth radius in kilometers
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    
    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;
    
    double a = sin(dLat/2) * sin(dLat/2) +
               sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return R * c;
}

// Function to trim leading and trailing whitespace from a string
string trim(const string& str) {
    auto start = str.find_first_not_of(" \t\n\r\f\v");
    auto end = str.find_last_not_of(" \t\n\r\f\v");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}
string Nearest(double lat,double lon)
{
    double inputLat=lat,inputLon=lon;
    ifstream file("IN.csv");
    if (!file.is_open()) {
        //cerr << "Error opening file." << endl;
        return "Error opening file";
    }
    string line;
    vector<City> cities;
    // Reading the CSV file
    while (getline(file, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }
        stringstream ss(line);
        string name;
        string latStr, lonStr;
        if (getline(ss, name, ',') &&
            getline(ss, latStr, ',') &&
            getline(ss, lonStr, ',')) {
            
            City city;
            city.name = trim(name);
            latStr = trim(latStr);
            lonStr = trim(lonStr);

            try {
                city.latitude = stod(latStr);
                city.longitude = stod(lonStr);
            } catch (const invalid_argument& e) {
               // cerr << "Invalid argument: " << e.what() << " in line: " << line << endl;
                continue; // Skip this line and continue with the next one
            } catch (const out_of_range& e) {
               cerr << "Out of range: " << e.what() << " in line: " << line << endl;
                continue; // Skip this line and continue with the next one
            }
            cities.push_back(city);
        } else {
            cerr << "Error parsing line: " << line << endl;
        }
    }
    file.close();

    // Find the nearest city
    double minDistance = numeric_limits<double>::max();
    City nearestCity;

    for (const auto& city : cities) {
        double distance = haversine(inputLat, inputLon, city.latitude, city.longitude);
        if (distance < minDistance) {
            minDistance = distance;
            nearestCity = city;
        }
    }

    cout << "Nearest city: " << nearestCity.name << endl;
    cout << "Latitude: " << nearestCity.latitude << endl;
    cout << "Longitude: " << nearestCity.longitude << endl;
    return nearestCity.name;
}

#endif // MYHEADER_H