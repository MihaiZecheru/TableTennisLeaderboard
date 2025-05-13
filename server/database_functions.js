import sqlite3 from 'sqlite3';
import { open } from 'sqlite';

import Game from './database_types/Game.js';
import Player from './database_types/Player.js';
import Point from './database_types/Point.js';

function parse_ph(point_history) {
  for (let i = 0; i < point_history.length; i++) {
    point_history[i] = new Point(point_history[i].s, point_history[i].w, i + 1);
  }
  return point_history;
}

const db_promise = open({
  filename: './TableTennis.db',
  driver: sqlite3.Database
});

export async function AddSetToDatabase(p1_id, p2_id, set_length, point_history, started_at, ended_at, ) {
  try {
    const db = await db_promise;

    // Calculate scores based on point history
    let p1_score = 0;
    let p2_score = 0;

    for (const point of point_history) {
      if (point.w === 1) p1_score++;
      else if (point.w === 2) p2_score++;
      else throw new Error("point_history[i].w should be either 1 or 2");
    }

    // Decide winner
    let winner_id = null;
    if (p1_score > p2_score) {
      winner_id = p1_id;
    } else if (p2_score > p1_score) {
      winner_id = p2_id;
    } else {
      throw new Error('Scores should not be equal');
    }

    // Insert into Games table
    await db.run(
      `INSERT INTO games (p1_id, p2_id, winner_id, p1_score, p2_score, set_length, point_history, started_at, ended_at)
       VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)`,
      p1_id,
      p2_id,
      winner_id,
      p1_score,
      p2_score,
      set_length,
      JSON.stringify(point_history),
      started_at,
      ended_at
    );

    return true;
  } catch (err) {
    console.error('Error inserting game:', err.message);
    return false;
  }
}

export async function GetLast50Games() {
  try {
    const db = await db_promise;
    const games = await db.all(
      `SELECT * FROM games ORDER BY id DESC LIMIT 50`
    );
    return games.map(game => new Game(game.id, game.p1_id, game.p2_id, game.winner_id, game.p1_score, game.p2_score, game.set_length, parse_ph(JSON.parse(game.point_history)), game.started_at, game.ended_at));
  } catch (err) {
    console.error('Error fetching last 50 games:', err.message);
    return null;
  }
}

export async function GetGame(id) {
  try {
    const db = await db_promise;
    const game = await db.get(
      `SELECT * FROM games WHERE id = ?`,
      id
    );
    return new Game(game.id, game.p1_id, game.p2_id, game.winner_id, game.p1_score, game.p2_score, game.set_length, parse_ph(JSON.parse(game.point_history)), game.started_at, game.ended_at);
  } catch (err) {
    console.error('Error fetching game:', err.message);
    return null;
  }
}

export async function GetPlayerById(player_id) {
  try {
    const db = await db_promise;
    const player = await db.get(
      `SELECT * FROM players WHERE id = ?`,
      player_id
    );
    return new Player(player.id, player.name, player.country, player.rank);
  } catch (err) {
    console.error('Error fetching player:', err.message);
    return null;
  }
}

export async function GetPlayers() {
  try {
    const db = await db_promise;
    const players = await db.all(
      `SELECT * FROM players`
    );
    return players.map(player => new Player(player.id, player.name, player.country, player.rank));
  } catch (err) {
    console.error('Error fetching players:', err.message);
    return [];
  }
}

/**
 * Leaderboard is calculated by comparing the win ratio of every player
 * @param {Player[]} players Parsed Player array
 */
export async function GetPlayerLeaderboard(players) {
  players.sort((a, b) => {
    const a_wr = a.game_win_ratio;
    const b_wr = b.game_win_ratio;

    if (a_wr > b_wr) return -1;
    if (a_wr < b_wr) return 1;
    return 0;
  });

  return players.map((player, index) => {
    return {
      rank: index + 1,
      player: player
    };
  });
}