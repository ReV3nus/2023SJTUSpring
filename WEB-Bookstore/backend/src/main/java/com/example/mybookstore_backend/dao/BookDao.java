package com.example.mybookstore_backend.dao;

import com.example.mybookstore_backend.entity.Book;

import java.util.List;

public interface BookDao {
    Book findOne(Integer id);
    Book AddBook(Book newBook);

    List<Book> getBooks();
    void DeleteBook(int bid);
}
