import sqlite3 from 'sqlite3';
import { open } from 'sqlite';

const db_promise = open({
  filename: './TableTennis.db',
  driver: sqlite3.Database
});

export async function AddSetToDatabase(p1_id, p2_id, set_length, started_at, point_history) {
  try {
    const db = await db_promise;

    // Calculate scores based on point history
    let p1_score = 0;
    let p2_score = 0;

    for (const point of point_history) {
      if (point.winner === 1) p1_score++;
      else if (point.winner === 2) p2_score++;
    }

    // Decide winner
    let winner_id = null;
    if (p1_score > p2_score) {
      winner_id = p1_id;
    } else if (p2_score > p1_score) {
      winner_id = p2_id;
    }

    // Insert into Games table
    await db.run(
      `INSERT INTO games (p1_id, p2_id, winner_id, p1_score, p2_score, set_length, point_history, started_at)
       VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)`,
      p1_id,
      p2_id,
      winner_id,
      p1_score,
      p2_score,
      set_length,
      JSON.stringify(point_history),
      started_at,
    );

    return true;
  } catch (err) {
    console.error('Error inserting game:', err.message);
    return false;
  }
}
