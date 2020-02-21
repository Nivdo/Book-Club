//
// Created by nivdo@wincs.cs.bgu.ac.il on 11/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H

#include <string>

using namespace std;

/**
 * Book class represents a book object.
 */
class Book {

private:
    string name; //the book's name
    string genre; //the book's genre

public:
    /**
     * book constructor
     * @param name  - the book's name
     * @param genre - the book's genre
     */
    Book(string name, string genre);

    /**
     * get name method
     * @return the book's name
     */
    const string getName();

    /**
     * get genre method
     * @return the book's genre
     */
    const string getGenre();
};


#endif //BOOST_ECHO_CLIENT_BOOK_H
