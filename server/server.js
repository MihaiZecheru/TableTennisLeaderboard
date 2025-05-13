import { AddSetToDatabase, GetLast50Games, GetPlayers } from './database_functions.js';
import express from 'express';

const PORT = 4010;
const app = express();
app.use(express.json());
app.set('view engine', 'ejs');
app.set('views', './views');
app.use(express.static('public'));

/**
 * View game history. All the past games are displayed.
 * When a player clicks on a game, it will show the game details, like the stats and stuff.
 */
app.get('/', async (req, res) => {
  const games = await GetLast50Games();
  const players = await GetPlayers();
  if (!games) return res.status(500).send('Error fetching games');
  // gpn stands for "get player name"
  res.render('recent_games', { games, players, gpn: (id) => players.find(player => player.id === id)?.name || 'Unknown' });
});

/**
 * Player leaderboard. Ranked by win ratio
 */
app.get('/leaderboard', async (req, res) => {
  const players = await GetPlayers();
  
  players.sort((a, b) => {
    const a_wr = a.wins / (a.wins + a.losses);
    const b_wr = b.wins / (b.wins + b.losses);
    return b_wr - a_wr;
  });

  res.render('leaderboard', { players, gpn: (id) => players.find(player => player.id === id)?.name || 'Unknown' });
});

/**
 * Game details. When a player clicks on a game, it will show the game details, like the stats and stuff.
 */
app.get('/game/:id', async (req, res) => {
  const game_id = req.params.id;
  const players = await GetPlayers();
  const game = await GetGameById(game_id);
  if (!game) return res.status(500).send('Error fetching game - probably invalid ID');
  res.render('game_details', { game, players, gpn: (id) => players.find(player => player.id === id)?.name || 'Unknown' });
});

/**
 * Player details like stats and stuff.
 */
app.get('/player/:id', async (req, res) => {

});

/**
 * The ESP32 will send a POST request to this endpoint with the game data.
 * The server will then save the game data to the database.
 */
app.post('/api/upload-set', async (req, res) => {
  /**
   * Example payload (note player IDs start at 10):
   * { p1_id: 10, p2_id: 12, set_length: 21, started_at: "1970-01-01 00:00:42", ended_at: "1970-01-01 00:10:05",
   * point_history: [{ 's': 1, 'w': 0 }, { 's': 1, 'w': 1 }] }
   */
  const { p1_id, p2_id, set_length, point_history, started_at, ended_at } = req.body;
  const success = await AddSetToDatabase(p1_id, p2_id, set_length, point_history, started_at, ended_at);

  if (success) {
    res.status(200).send('OK');
  } else {
    res.status(400).send('ERR');
  }
});

app.listen(PORT, () => {
  console.log(`Server listening on http://localhost:${PORT}`);
});

// TODO: something like if a user is on the game details page, they can click a button to directly compare the stats of the two players that played the game