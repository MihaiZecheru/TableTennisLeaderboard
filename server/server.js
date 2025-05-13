import { AddSetToDatabase, GetLast50Games } from './database_functions.js';
import express from 'express';

const PORT = 4010;
const app = express();
app.use(express.json());
app.set('view engine', 'ejs');
app.set('views', './views');
app.use(express.static('public'));

app.get('/', async (req, res) => {
  const games = await GetLast50Games();
  if (!games) return res.status(500).send('Error fetching games');
  res.render('recent_games', { games });
});

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
