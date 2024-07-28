
resources/application.yml

```
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/dbname
    username: username
    password: password
    driver-class-name: com.mysql.jdbc.Driver
  jpa:
    hibernate:
      ddl-auto: update
    show-sql: true
```
