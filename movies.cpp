#include "movies.h"

// Constructor
Movie::Movie(std::string n, double r) : movieName(n), rating(r) {}

// Getter for movie name
std::string Movie::getMovieName() const { // Mark as const
    return movieName;
}

// Getter for rating
double Movie::getRating() const { // Mark as const
    return rating;
}
