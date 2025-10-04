-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Date:  July 28 2021
-- Time:  10:15am
-- Place: Humphrey Street
-- To find:
-- Who the thief is?
-- What city the thief escaped to?
-- Who the thief’s accomplice is who helped them escape?

-- Get the crime scene reports
SELECT description
FROM crime_scene_reports
WHERE street == "Humphrey Street"
    AND year == 2021 AND month == 7 AND day == 28;

-- Get the name and transcript of the desired interviews
SELECT name, transcript
FROM interviews
WHERE transcript LIKE "%thief%"
    AND year == 2021 AND month == 7 AND day == 28;

-- Now we need to look for:
-- 1. license plates
-- 2. atm records
-- 3. phone calls

-- 1. Get the owners of the cars found leaving
SELECT name  --, people.license_plate
FROM people
JOIN bakery_security_logs ON people.license_plate == bakery_security_logs.license_plate
WHERE year == 2021 AND month == 7 AND day == 28
    AND activity == "exit"
    AND hour == 10 AND minute >= 15 AND minute <= 25;

-- 2. Get the bank accounts withdrawing money at legget street
SELECT name  --, account_number
FROM bank_accounts
JOIN people ON bank_accounts.person_id == people.id
WHERE account_number IN
    (SELECT account_number
    FROM atm_transactions
    WHERE year == 2021 AND month == 7 AND day == 28
        AND atm_location == "Leggett Street"
        AND transaction_type == "withdraw");

-- 3. Get the names of people calling at that time
SELECT name  --, phone_number
FROM people
WHERE phone_number IN
    (SELECT caller
    FROM phone_calls
    WHERE year == 2021 AND month == 7 AND day == 28
        AND duration < 60);

-- Now, if we intersect 3 above stated queries, we will get a list of probable suspects, i.e., Bruce and Diana

-- Get all details of possible suspects - Bruce and Diana (we can use the result of the intersection of the above queries but thats too long and redundant)
SELECT *
FROM people
WHERE name IN ("Bruce", "Diana");

-- Get the names of the people the suspects called
-- Diana (770) 555-1861 called Philip (725) 555-3243
-- Bruce (367) 555-5533 called Robin (375) 555-8161
SELECT name
FROM people
WHERE phone_number IN
    (SELECT receiver
    FROM phone_calls
    WHERE caller IN
        (SELECT phone_number
        FROM people
        WHERE name IN ("Bruce", "Diana"))
    AND year == 2021 AND month == 7 AND day == 28
    AND duration < 60);

-- Get the id of the earliest flight leaving Fiftyville tomorrow
SELECT id
FROM flights
WHERE origin_airport_id IN
    (SELECT id
    FROM airports
    WHERE city == "Fiftyville")
AND year == 2021 AND month == 7 AND day == 29
ORDER BY hour, minute
LIMIT 1;

-- Get the destination city of the flight
SELECT city
FROM airports
WHERE id IN
    (SELECT destination_airport_id
    FROM flights
    WHERE origin_airport_id IN
        (SELECT id
        FROM airports
        WHERE city == "Fiftyville")
    AND year == 2021 AND month == 7 AND day == 29
    ORDER BY hour, minute
    LIMIT 1);

-- Get the name of the names of people on the flight
-- Bruce is on the outgoing flight
SELECT name
FROM people
WHERE passport_number IN
    (SELECT passport_number
    FROM passengers
    WHERE flight_id IN
        (SELECT id
        FROM flights
        WHERE origin_airport_id IN
            (SELECT id
            FROM airports
            WHERE city == "Fiftyville")
        AND year == 2021 AND month == 7 AND day == 29
        ORDER BY hour, minute
        LIMIT 1));

------------------------------------------------------
-- Answers:
-- Bruce is the suspect
-- He fled to New York City
-- Robin is the accomplice
