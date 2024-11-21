#ifndef MOVIES_H
#define MOVIES_H

#include <string>

class Movie {
public:
    Movie(std::string n, double r);

    std::string getMovieName() const; // Mark as const
    double getRating() const;         // Mark as const

private:
    std::string movieName;
    double rating;
};

#endif
