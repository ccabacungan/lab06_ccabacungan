#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "movies.h"

using namespace std;

// Helper functions
void loadMoviesFromFile(const string &filename, vector<Movie> &movies);
void loadPrefixesFromFile(const string &filename, vector<string> &prefixes);
vector<Movie> findMoviesWithPrefix(const vector<Movie> &movies, const string &prefix);
void printMatchingMovies(const vector<Movie> &matchingMovies, const string &prefix);
Movie findBestMovie(const vector<Movie> &matchingMovies);

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[0] << " moviesFilename prefixFilename" << endl;
        exit(1);
    }

    vector<Movie> movies;
    loadMoviesFromFile(argv[1], movies);

    if (argc == 2) {
        for (const auto &movie : movies) {
            cout << movie.getMovieName() << ", " << fixed << setprecision(1) << movie.getRating() << endl;
        }
        return 0;
}

    vector<string> prefixes;
    loadPrefixesFromFile(argv[2], prefixes);

    vector<pair<string, Movie>> bestMovies;

    for (const auto &prefix : prefixes) {
        vector<Movie> matchingMovies = findMoviesWithPrefix(movies, prefix);

        printMatchingMovies(matchingMovies, prefix);

        if (!matchingMovies.empty()) {
            Movie bestMovie = findBestMovie(matchingMovies);
            bestMovies.emplace_back(prefix, bestMovie);
        }
    }

    for (const auto &[prefix, bestMovie] : bestMovies) {
        cout << "Best movie with prefix " << prefix << " is: " 
             << bestMovie.getMovieName() << " with rating " 
             << fixed << setprecision(1) << bestMovie.getRating() << endl;
    }

    return 0;
}

// Load movies from a file into a vector
void loadMoviesFromFile(const string &filename, vector<Movie> &movies) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file " << filename << endl;
        exit(1);
    }
 string line;
    while (getline(file, line)) {
        size_t commaPos = line.find_last_of(',');
        string movieName = line.substr(0, commaPos);
        double movieRating = stod(line.substr(commaPos + 1));

        // Remove quotes from movie name if present
        if (!movieName.empty() && movieName.front() == '"') {
            movieName = movieName.substr(1, movieName.size() - 2);
        }

        movies.emplace_back(movieName, movieRating);
    }

    file.close();
}

// Load prefixes from a file into a vector
void loadPrefixesFromFile(const string &filename, vector<string> &prefixes) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    file.close();
}

vector<Movie> findMoviesWithPrefix(const vector<Movie> &movies, const string &prefix) {
    vector<Movie> matchingMovies;

    auto start = lower_bound(movies.begin(), movies.end(), prefix,
                             [](const Movie &movie, const string &prefix) {
                                 return movie.getMovieName().substr(0, prefix.size()) < prefix;
                             });

    for (auto it = start; it != movies.end(); ++it) {
        if (it->getMovieName().substr(0, prefix.size()) == prefix) {
            matchingMovies.push_back(*it);
        } else {
            break;
        }
    }

    // Sort by rating descending, then alphabetically
    sort(matchingMovies.begin(), matchingMovies.end(), [](const Movie &a, const Movie &b) {
        return (a.getRating() > b.getRating()) ||
               (a.getRating() == b.getRating() && a.getMovieName() < b.getMovieName());
    });

    return matchingMovies;
}


// Print matching movies
void printMatchingMovies(const vector<Movie> &matchingMovies, const string &prefix) {
    if (matchingMovies.empty()) {
        cout << "No movies found with prefix " << prefix << endl;
    } else {
        for (const auto &movie : matchingMovies) {
            cout << movie.getMovieName() << ", " << fixed << setprecision(1) << movie.getRating() << endl;
        }
        cout << endl;
    }
}

Movie findBestMovie(const vector<Movie> &matchingMovies) {
    return matchingMovies.front(); // The first movie is the best due to sorted order
}


