package com.dominik.repository;

import com.dominik.entity.User;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import java.util.List;

public interface UserRepository extends JpaRepository<User, Long> {
    User findByUsername(String username);

    @Query("select u.username, u.id, u.role  from User u")
    List<String> getAllUserData();


}
