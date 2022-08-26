-- Keep a log of any SQL queries you execute as you solve the mystery.


-- Check the description of the crime scene that took place on July 28, 2021 on Humphrey Street.
SELECT * FROM crime_scene_reports
    WHERE day = 28 AND month = 7 AND year = 2021 AND street = 'Humphrey Street';

-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- Littering took place at 16:36. No known witnesses.


-- Trying to get more information from the people that where present
SELECT name, transcript FROM interviews
    WHERE day = 28 AND month = 7 AND year = 2021 AND transcript LIKE '%bakery%';

-- Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
--           If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene  | I don't know the thief's name, but it was someone I recognized.
--           Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
--           In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--           The thief then asked the person on the other end of the phone to purchase the flight ticket.


-- Check for cars that left the bakery's parking lot between the 10:15am and the 10:25am
SELECT activity, license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND (minute BETWEEN 15 AND 25);

--  5P2BI95
--  94KL13X
--  6P58WS2
--  4328GD8
--  G412CB7
--  L93JTIZ
--  322W7JE
--  0NTHK55


-- Check the atm transactions that occured during the day to get the number of the thief's account
-- Check the bank account numbers to have the suspects
SELECT people.id, name, phone_number, passport_number, license_plate, bank_accounts.account_number FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
        JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
            WHERE day = 28 AND month = 7 AND year = 2021 AND transaction_type = 'withdraw' AND atm_location LIKE '%Leggett Street%';

----------+---------+----------------+-----------------+---------------+----------------+
--   id   |  name   |  phone_number  | passport_number | license_plate | account_number |
----------+---------+----------------+-----------------+---------------+----------------+
-- 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       |
-- 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       | 26013199       |
-- 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       | 16153065       |
-- 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       | 28296815       |
-- 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       | 25506511       |
-- 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       | 28500762       |
-- 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       | 76054385       |
-- 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       | 81061156       |
----------+---------+----------------+-----------------+---------------+----------------+


-- Intersect the information given by the atm transaction with the information given by the bakery
SELECT license_plate FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
        JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
            WHERE day = 28 AND month = 7 AND year = 2021 AND transaction_type = 'withdraw' AND atm_location LIKE '%Leggett Street%'
                INTERSECT
                    SELECT license_plate FROM bakery_security_logs
                        WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND (minute BETWEEN 15 AND 25);

--------------------------------+
-- license_plate                |
--------------------------------+
-- 322W7JE Diana (770) 555-1861 |
-- 4328GD8 Luca  (389) 555-5198 |
-- 94KL13X Bruce (367) 555-5533 |
-- L93JTIZ Iman  (829) 555-5269 |
--------------------------------+


-- Check which of the suspects made a call in the morning
SELECT id, caller, receiver FROM phone_calls
    WHERE day = 28 AND month = 7 AND year = 2021 AND duration <= 60
            AND caller IN ('(770) 555-1861', '(389) 555-5198', '(367) 555-5533', '(829) 555-5269');

-------+------------------------+----------------+
-- id  |     caller             |    receiver    |
-------+------------------------+----------------+
-- 233 | (367) 555-5533 (Bruce) | (375) 555-8161 |
-- 255 | (770) 555-1861 (Diana) | (725) 555-3243 |
-------+------------------------+----------------+


-- Check the flights that happened on the day after the theft a see which ones took our suspects
-- Check the earliest flight and get the thief
SELECT * FROM flights
    JOIN passengers ON passengers.flight_id = flights.id
        JOIN people ON people.passport_number = passengers.passport_number
            WHERE flights.year = 2021 AND flights.month = 7 AND flights.day = 29
                AND people.name IN ('Bruce', 'Diana')
            ORDER BY hour, minute
            LIMIT 1;


-- Check who the accomplice is
SELECT * FROM people
    WHERE phone_number = '(375) 555-8161';


-- Check the city the thief escaped to
SELECT * FROM airports
    WHERE id = 4;