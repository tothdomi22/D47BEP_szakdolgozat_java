package com.dominik.service;

import com.dominik.entity.User;
import com.dominik.repository.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import java.util.Optional;

@Service
public class UserDetailService implements UserDetailsService {

    @Autowired
    private UserRepository userRepository;

    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        Optional<User> user = userRepository.findByUsername(username);
        if(user.isPresent()){
            var userObject = user.get();
            return org.springframework.security.core.userdetails.User.builder()
                    .username(userObject.getUsername())
                    .password(userObject.getPassword())
                    .roles(getRoles(userObject))
                    .build();
        } else {
            throw new UsernameNotFoundException(username);
        }
    }

    public Optional<Long> getUserId(User user) {
        Optional<User> foundUser = userRepository.findByUsername(user.getUsername());
        return foundUser.map(User::getId);

    }

    private String[] getRoles(User user) {
        if (user.getRole() == null) {
            return new String[]{"USER"};
        }
        return user.getRole().split(",");
    }
}
