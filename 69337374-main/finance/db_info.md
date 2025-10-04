## `transactions` TABLE


creation command:
```sql
CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL,
                           user_id INTEGER NOT NULL,
                           symbol TEXT NOT NULL,
                           name TEXT,
                           shares INTEGER NOT NULL,
                           price NUMERIC NOT NULL,
                           total NUMERIC NOT NULL,
                           time TEXT NOT NULL,
                           FOREIGN KEY(user_id) REFERENCES users(id)
);
```

`id` is primary key, and autoincrements for history  
`user_id` is foreign key for safety  
`price` is the price at the time of transaction kept for history  
`total` is the total calculated at the time of transaction kept for history  

### RESET `transactions` TABLE

1. clear table
    ```sql
    DELETE FROM transactions;
    ```
2. reset autoincremented id
    ```sql
    DELETE FROM sqlite_sequence WHERE name = "transactions";
    ``` 
3. reset users' cash !value is hardcoded

   replace `<user-ids>` with user ids whose cash to reset (completely reset)
    ```sql
    UPDATE users SET cash = 10000 WHERE id IN (<user-ids>);
    ```

SO Question: [SQLite Reset Primary Key Field](https://stackoverflow.com/questions/1601697)

## `assets` TABLE

creation command:
```sql
CREATE TABLE assets (id INTEGER PRIMARY KEY UNIQUE NOT NULL,
                     user_id INTEGER NOT NULL,
                     symbol TEXT NOT NULL,
                     name TEXT,
                     shares INTEGER NOT NULL,
                     FOREIGN KEY(user_id) REFERENCES users(id)
);
```

`*` `id` is primary key (autoincrement not required)  
`**` `user_id` is foreign key for safety
