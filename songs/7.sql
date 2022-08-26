SELECT AVG(songs.energy) AS Average_Energy FROM songs JOIN artists
    ON songs.artist_id = artists.id
    WHERE artists.name = "Drake";