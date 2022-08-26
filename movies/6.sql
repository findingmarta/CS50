SELECT AVG(ratings.rating) AS Average_Rating FROM ratings JOIN movies
    ON ratings.movie_id = movies.id
    WHERE movies.year = '2012';