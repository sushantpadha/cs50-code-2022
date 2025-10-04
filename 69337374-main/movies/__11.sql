-- using multiple nested queries instead of joined tables because
-- ratings has multiple `movie_id` keys for every single `id` key in movies
-- therefore, a right or full outer join is needed, which is not supported
-- i think???
SELECT title
FROM movies
WHERE id IN
    (SELECT movie_id
    FROM ratings
    WHERE movie_id IN
        (SELECT movie_id
        FROM stars
        WHERE person_id IN
            (SELECT id
            FROM people
            WHERE name == "Chadwick Boseman"))
    ORDER BY rating
    LIMIT 5);