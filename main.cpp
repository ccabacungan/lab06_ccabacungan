
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include "movies.h"

using namespace std;

bool extractMovieDetails(string& line, string& title, double& score);

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Insufficient arguments. Provide at least one filename." << endl;
        cerr << "Usage: " << argv[0] << " moviesFilename [prefixFilename]" << endl;
        return 1;
    }

    ifstream movieInput(argv[1]);
    if (!movieInput.is_open()) {
        cerr << "Failed to open file: " << argv[1] << endl;
        return 1;
    }

    vector<Movie> movies;
    string line, title;
    double score;

    while (getline(movieInput, line)) {
        if (extractMovieDetails(line, title, score)) {
            movies.emplace_back(title, score);
        }
    }
    movieInput.close();

    if (argc == 2) {
        for (const auto& movie : movies) {
            cout << movie.getMovieName() << ", " << fixed << setprecision(1) << movie.getRating() << endl;
        }
        return 0;
    }

    ifstream prefixInput(argv[2]);
    if (!prefixInput.is_open()) {
        cerr << "Failed to open file: " << argv[2] << endl;
        return 1;
    }

    vector<string> prefixes;
    while (getline(prefixInput, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    prefixInput.close();

    vector<pair<string, pair<string, double>>> topMovies;
    for (const string& prefix : prefixes) {
        vector<Movie> matchingMovies;
        for (const auto& movie : movies) {
            if (movie.getMovieName().substr(0, prefix.size()) == prefix) {
                matchingMovies.push_back(movie);
            }
        }

        sort(matchingMovies.begin(), matchingMovies.end(), [](const Movie& a, const Movie& b) {
            return (a.getRating() > b.getRating()) || 
                   (a.getRating() == b.getRating() && a.getMovieName() < b.getMovieName());
        });

        if (!matchingMovies.empty()) {
            for (const auto& movie : matchingMovies) {
                cout << movie.getMovieName() << ", " << fixed << setprecision(1) << movie.getRating() << endl;
            }
            cout << endl;
            topMovies.emplace_back(prefix, make_pair(matchingMovies[0].getMovieName(), matchingMovies[0].getRating()));
        } else {
            cout << "No movies found with prefix " << prefix << endl;
        }
    }

    for (const auto& entry : topMovies) {
        cout << "Best movie with prefix " << entry.first << " is: "
             << entry.second.first << " with rating "
             << fixed << setprecision(1) << entry.second.second << endl;
    }

    return 0;
}

bool extractMovieDetails(string& line, string& title, double& score) {
    size_t delimiterPos = line.find_last_of(",");
    if (delimiterPos == string::npos) {
        return false;
    }
    title = line.substr(0, delimiterPos);
    score = stod(line.substr(delimiterPos + 1));

    if (!title.empty() && title.front() == '\"') {
        title = title.substr(1, title.size() - 2);
    }
    return true;
}

