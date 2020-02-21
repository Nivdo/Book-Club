//
// Created by nivdo@wincs.cs.bgu.ac.il on 11/01/2020.
//

#include "Book.h"

/**
 * a getter of the book owner name
 * @return a refrence to the name of the owner of the book
 */

const string Book::getGenre() {
    return genre;
}

const string Book::getName() {
    return name;
}

Book::Book(string name, string genre) : name(name), genre(genre) {}


