CREATE TABLE IF NOT EXISTS Players (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(8) NOT NULL,
    elo REAL DEFAULT 1000,
    highest_elo REAL DEFAULT 1000,
    lowest_elo REAL DEFAULT 1000,
    total_games_played INTEGER DEFAULT 0,
    total_wins INTEGER DEFAULT 0,
    total_losses INTEGER DEFAULT 0,
    points_won INTEGER DEFAULT 0,
    points_lost INTEGER DEFAULT 0,
    serves_won INTEGER DEFAULT 0,
    serves_lost INTEGER DEFAULT 0,
    longest_win_streak INTEGER DEFAULT 0,
    longest_loss_streak INTEGER DEFAULT 0,
    longest_game_duration INTEGER DEFAULT 0,
    most_points_in_one_game INTEGER DEFAULT 0, -- most points scored in a single game
    biggest_win_margin INTEGER DEFAULT 0,
    biggest_loss_margin INTEGER DEFAULT 0,
    most_points_won_in_a_row INTEGER DEFAULT 0,
    most_points_lost_in_a_row INTEGER DEFAULT 0,
    time_played INTEGER DEFAULT 0,
    created_at TEXT DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS Games (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    p1_id INTEGER NOT NULL,
    p2_id INTEGER NOT NULL,
    p1_elo_before_match REAL NOT NULL, -- Player 1's elo before the game
    p2_elo_before_match REAL NOT NULL, -- Player 2's elo before the game
    p1_elo_change REAL NOT NULL, -- The change in elo for player 1 as a result of the game
    p2_elo_change REAL NOT NULL, -- The change in elo for player 2 as a result of the game
    winner_id INTEGER NOT NULL,
    p1_score INTEGER NOT NULL,
    p2_score INTEGER NOT NULL,
    set_length INTEGER NOT NULL CHECK (set_length IN (6, 11, 21)),
    started_at TEXT NOT NULL,
    ended_at TEXT DEFAULT (datetime('now')),
    point_history TEXT NOT NULL,
    FOREIGN KEY (p1_id) REFERENCES Players(id) ON DELETE CASCADE,
    FOREIGN KEY (p2_id) REFERENCES Players(id) ON DELETE CASCADE,
    FOREIGN KEY (winner_id) REFERENCES Players(id),
    CHECK (winner_id = p1_id OR winner_id = p2_id)
);

-- Add players to table
INSERT INTO Players (name) VALUES
('DELETE'),
('DELETE'),
('DELETE'),
('DELETE'),
('DELETE'),
('DELETE'),
('DELETE'),
('DELETE'),
('DELETE'),
('Chris'),
('Daria'),
('Erin'),
('Taticu');
DELETE FROM Players WHERE name = 'DELETE'; -- ID will start at 10