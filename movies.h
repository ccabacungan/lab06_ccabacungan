#ifndef MOVIES_H
#define MOVIES_H

#include <string>

class Movie {
public:
    Movie(std::string n, double r);

    std::string getMovieName() const; // Marked as const
    double getRating() const;         // Marked as const

private:
    std::string movieName;
    double rating;
};

#endif
