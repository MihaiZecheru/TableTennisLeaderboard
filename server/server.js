import { AddSetToDatabase } from './database_functions.js';
import express from 'express';

const PORT = 4010;
const app = express();
app.use(express.json());

app.get('/', (req, res) => {
  res.status(200).send('Server online');
});

app.post('/api/upload-set', async (req, res) => {
  /**
   * Example payload (note player IDs start at 10):
   * { p1_id: 10, p2_id: 12, set_length: 21, started_at: "", point_history:
   * [{ 's': 1, 'w': 0 }, { 's': 1, 'w': 1 }] }
   */
  const { p1_id, p2_id, set_length, point_history } = req.body;
  const success = await AddSetToDatabase(p1_id, p2_id, set_length, point_history);

  if (success) {
    res.status(200).send('OK');
  } else {
    res.status(400).send('ERR');
  }
});

app.listen(PORT, '0.0.0.0', () => {
  console.log(`Server listening on http://localhost:${PORT}`);
});
