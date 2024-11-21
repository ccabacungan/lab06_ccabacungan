#include "movies.h"

Movie::Movie(std::string n, double r) : movieName(n), rating(r) {}

std::string Movie::getMovieName() const { 
    return movieName;
}

double Movie::getRating() const { 
    return rating;
}
