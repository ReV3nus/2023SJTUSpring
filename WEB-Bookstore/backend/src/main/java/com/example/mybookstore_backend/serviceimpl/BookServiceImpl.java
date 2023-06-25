package com.example.mybookstore_backend.serviceimpl;

import com.example.mybookstore_backend.dao.BookDao;
import com.example.mybookstore_backend.entity.Book;
import com.example.mybookstore_backend.service.BookService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
public class BookServiceImpl implements BookService {

    @Autowired
    private BookDao bookDao;

    @Override
    public Book findBookById(Integer id){
        return bookDao.findOne(id);
    }

    @Override
    public List<Book> getBooks() {
        return bookDao.getBooks();
    }
    @Override
    public Book SaveBook(Book b){return bookDao.AddBook(b);}
    @Override
    @Transactional
    public void DeleteBook(int bid){bookDao.DeleteBook(bid);}
}
