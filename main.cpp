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
bool hasPrefix(const std::string& movieName, const std::string& prefix);

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

    stable_sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
        string nameA = a.getMovieName();
        string nameB = b.getMovieName();
        transform(nameA.begin(), nameA.end(), nameA.begin(), ::tolower);
        transform(nameB.begin(), nameB.end(), nameB.begin(), ::tolower);
        return nameA < nameB;
    });

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
            if (hasPrefix(movie.getMovieName(), prefix)) {
                matchingMovies.push_back(movie);
            }
        }

        stable_sort(matchingMovies.begin(), matchingMovies.end(), [](const Movie& a, const Movie& b) {
            if (a.getRating() != b.getRating()) {
                return a.getRating() > b.getRating();
            }
            string nameA = a.getMovieName(), nameB = b.getMovieName();
            transform(nameA.begin(), nameA.end(), nameA.begin(), ::tolower);
            transform(nameB.begin(), nameB.end(), nameB.begin(), ::tolower);
            return nameA < nameB; 
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

bool hasPrefix(const std::string& movieName, const std::string& prefix) {
    if (prefix.size() > movieName.size()) {
        return false;
    }
    return std::equal(prefix.begin(), prefix.end(), movieName.begin(),
                      [](char a, char b) { return tolower(a) == tolower(b); });
}


/* 
    First, the goal is to read the movie data from the files. As, we iterate through each movie, getline() is used to read the movie in 
    which has a run time of O(l). Following, extractMovieDetails is implemented which utilizes find_last_of() and substr() which both
    have a run time of O(l). So, in order to read all movies it has a run time of O(n * l) as n represents the number of movies that are
    read. In order to sort the movies, its run time is O(n log n) due to stable_sort. Sorting the movies by string comparison, takes O(l)--
    the slowest time. Therefore, the time it takes is O (n * l * log n). Processing the prefixes of m prefixes has a slow run time of O(l) 
    due to hasPrefix. When iterating through all the movies, n, the run time becomes O(n * l). To sort matching movies, there are, at best,
    k, matching movies. So the total run time becomes O(k * log k). And since there are m total movies, it becomes O (m * k * log k).

    Taking all of that into consideration, our total run time becomes its sum of O(n * l * log n + m * n * l + m * k log k). The term of
    O(m * n * l) is most prevalent and that will be the slowest run time.

    input_20_random run time: 0.087 seconds
    input_100_random run time: 0.115 seconds
    input_1000_random run time: 0.278 seconds
    input_76920_random run time: 24.073 seconds

    The movies data structure stores n movies. The title has length l. The space complexity to store movies is O(n * l). Prefixes are 
    stored as m prefixes with length l leaving a space complexity of O(m * l). MatchingMovies could hold up to n values. Therefore, its 
    complexity is O(n). Topmovies would be the same, but at O(m). Taking all of that into consideration, the final space complexity would be
    O(n * l + m * l).

    My code was created to optimize fast time. I did this by sorting the movies and checking prefixes one by one. Retrospectively, I
    understand how this may not have been the most optimal approach and I could have used alternative or additional data structures to 
    increase space usage. Likewise, I attempted to minimize space, but I now know I didn't need to store the movies all at once. But, I know
    that could have increased the time complexity. Ultimately, it was about finding that sweet spot between the two. I believe it was harder
    to achieve low time complexity as sorting movies and filtering through the prefixes led to an increase in complexity.
*/