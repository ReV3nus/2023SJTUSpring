package com.example.mybookstore_backend.service;

import com.example.mybookstore_backend.entity.Book;

import java.util.List;

public interface BookService {

    Book findBookById(Integer id);

    List<Book> getBooks();
    Book SaveBook(Book b);
    void DeleteBook(int bid);
}
