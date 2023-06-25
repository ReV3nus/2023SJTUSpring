package com.example.mybookstore_backend.daoimpl;

import com.example.mybookstore_backend.dao.BookDao;
import com.example.mybookstore_backend.entity.Book;
import com.example.mybookstore_backend.repository.BookRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Optional;

@Repository
public class BookDaoImpl implements BookDao {

    @Autowired
    private BookRepository bookRepository;

    @Override
    public Book findOne(Integer id) {
        Optional<Book> optional = bookRepository.findById(id);
        return optional.orElse(null);
    }
    @Override
    public Book AddBook(Book newBook){
        return bookRepository.save(newBook);
    }

    @Override
    public List<Book> getBooks() {
        return bookRepository.getBooks();
    }


    @Override
    @Transactional
    public void DeleteBook(int bid){
        bookRepository.deleteByBookId(bid);
    }


}