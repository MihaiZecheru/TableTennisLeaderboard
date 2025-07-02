import { AddSetToDatabase, GetLast50Games, GetGameByID, GetAllPlayers, GetPlayerByID, GetMostPlayedWithOpponent, GetCountOfGamesPlayedTogether, format_game_duration, GetPlayerWinsAgainstOpponent, UtcToPst, GetPlayerElo, UpdatePlayerStats } from './database_functions.js';
import { CalculateNewElo } from './calculate_new_elo.js';
import LiveScoreWebSocket from './LiveScoreWebSocket.js';
import express from 'express';
import express_ws from 'express-ws';

const PORT = 4010;
const app = express();
app.use(express.json());
app.set('view engine', 'ejs');
app.set('views', './views');
app.use(express.static('public'));
express_ws(app);

/**
 * View game history. All the past games are displayed.
 * When a player clicks on a game, it will show the game details, like the stats and stuff.
 */
app.get('/', async (req, res) => {
  const games = await GetLast50Games();
  const players = await GetAllPlayers();
  if (!games) return res.status(500).send('Error fetching games');
  // gpn stands for "get player name"
  res.render('recent_games', { games, players, UtcToPst, gpn: (id) => players.find(player => player.id === id)?.name || 'Unknown' });
});

/**
 * Player leaderboard. Ranked by win ratio
 */
app.get('/leaderboard', async (req, res) => {
  const players = await GetAllPlayers();
  
  players.sort((a, b) => {
    return b.elo - a.elo;
  });

  res.render('leaderboard', { players, gpn: (id) => players.find(player => player.id === id)?.name || 'Unknown' });
});

/**
 * Game details. When a player clicks on a game, it will show the game details, like the stats and stuff.
 */
app.get('/game/:id', async (req, res) => {
  const game_id = req.params.id;
  const players = await GetAllPlayers();
  const game = await GetGameByID(game_id);
  if (!game) return res.status(500).send(`Error fetching game with ID=${game_id} - probably invalid ID`);
  res.render('game_details', { game, players, UtcToPst, abs: Math.abs, gpn: (id) => players.find(player => player.id === id)?.name || 'Unknown', gpid: (num) => num === 1 ? game.p1_id : game.p2_id });
});

/**
 * Player details like stats and stuff.
 */
app.get('/player/:id', async (req, res) => {
  const player_id = req.params.id;
  const player = await GetPlayerByID(player_id);
  if (!player) return res.status(500).send(`Error fetching player with ID=${player_id} - probably invalid ID`);
  const most_played_with_opponent = await GetMostPlayedWithOpponent(player_id);
  res.render('player_details', { player, most_played_with_opponent, format_game_duration });
});

/**
 * Compare two players. Expects two player IDs in the query string.
 * Example: /compare?p1=10&p2=12
 * If there are no IDs provided, the page will consist of two dropdowns that will allow the user to select two players to compare.
 * The page will then show the stats of the two players side by side.
 */
app.get('/compare-players', async (req, res) => {
  const p1_id = req.query.p1;
  const p2_id = req.query.p2;
  const players = await GetAllPlayers();

  // If both IDs are provided, show the comparison
  if (p1_id && p2_id) {
    const p1 = await GetPlayerByID(p1_id);
    const p2 = await GetPlayerByID(p2_id);
    if (!p1) return res.status(500).send(`Error fetching player with ID=${p1_id} - probably invalid ID`);
    if (!p2) return res.status(500).send(`Error fetching player with ID=${p2_id} - probably invalid ID`);
    const p1_most_played_with_opponent = await GetMostPlayedWithOpponent(p1_id);
    const p2_most_played_with_opponent = await GetMostPlayedWithOpponent(p2_id);
    const games_played_together = await GetCountOfGamesPlayedTogether(p1_id, p2_id);
    const p1_wins_against_p2 = await GetPlayerWinsAgainstOpponent(p1_id, p2_id);
    const p2_wins_against_p1 = await GetPlayerWinsAgainstOpponent(p2_id, p1_id);
    const p1_win_ratio_against_p2 = p1_wins_against_p2 / (p1_wins_against_p2 + p2_wins_against_p1);
    const p2_win_ratio_against_p1 = p2_wins_against_p1 / (p2_wins_against_p1 + p1_wins_against_p2);
    res.render('compare_players', { p1, p2, p1_most_played_with_opponent, p2_most_played_with_opponent, games_played_together, format_game_duration, p1_wins_against_p2, p2_wins_against_p1, p1_win_ratio_against_p2, p2_win_ratio_against_p1 });
  }
  // If no IDs are provided, show the dropdowns
  else {
    res.render('compare_players_dropdowns', { players });
  }
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
  
  if (!p1_id || !p2_id || !set_length || !point_history || !started_at || !ended_at) {
    return res.status(400).send('Missing required fields');
  }

  const p1_elo_before_match = await GetPlayerElo(p1_id);
  const p2_elo_before_match = await GetPlayerElo(p2_id);
  const { p1_elo_change, p2_elo_change } = CalculateNewElo(p1_elo_before_match, p2_elo_before_match, set_length, point_history);
  const p1_new_elo = p1_elo_before_match + p1_elo_change;
  const p2_new_elo = p2_elo_before_match + p2_elo_change;

  const success = await AddSetToDatabase(p1_id, p2_id, p1_elo_before_match, p2_elo_before_match, p1_elo_change, p2_elo_change, set_length, point_history, started_at, ended_at);
  const stats_success = await UpdatePlayerStats(p1_id, p2_id, p1_new_elo, p2_new_elo, point_history, started_at, ended_at);
  
  if (success && stats_success) {
    res.status(200).send('OK');
  } else {
    res.status(400).send('ERR');
  }
});

app.ws('/api/live-score', (ws, req) => LiveScoreWebSocket(ws, req));

app.get('/live-score', (req, res) => {
  // The page will connect to the live-score WebSocket through the browser
  // and modify the page via the <script> tag once connected.
  // The page will update dynamically; the <script> tag will handle the WebSocket connection.
  res.render('live_score');
});

app.listen(PORT, () => {
  console.log(`Server listening on http://localhost:${PORT}`);
});
