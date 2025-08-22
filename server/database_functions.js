import { db_promise } from './sqlite_instance.js';
import { DateTime } from 'luxon';

import Game from './database_types/Game.js';
import Player from './database_types/Player.js';
import Point from './database_types/Point.js';

function parse_ph(point_history) {
  let p1_score = 0;
  let p2_score = 0;

  for (let i = 0; i < point_history.length; i++) {
    if (point_history[i].w === 1) {
      p1_score++;
    } else if (point_history[i].w === 2) {
      p2_score++;
    } else {
      throw new Error("point_history[i].w should be either 1 or 2");
    }

    if (point_history[i].s !== 1 && point_history[i].s !== 2) {
      throw new Error("point_history[i].s should be either 1 or 2");
    }

    point_history[i] = new Point(point_history[i].s, point_history[i].w, i + 1, [p1_score, p2_score]);
  }

  return point_history;
}

export async function AddSetToDatabase(p1_id, p2_id, p1_elo_before_match, p2_elo_before_match, p1_elo_change, p2_elo_change, set_length, point_history, started_at, ended_at, ) {
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
      `INSERT INTO games (p1_id, p2_id, p1_elo_before_match, p2_elo_before_match, p1_elo_change, p2_elo_change, winner_id, p1_score, p2_score, set_length, point_history, started_at, ended_at)
       VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`,
      p1_id,
      p2_id,
      p1_elo_before_match,
      p2_elo_before_match,
      p1_elo_change,
      p2_elo_change,
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
    return games.map(game => new Game(game.id, game.p1_id, game.p2_id, game.p1_elo_before_match, game.p2_elo_before_match, game.p1_elo_change, game.p2_elo_change, game.winner_id, game.p1_score, game.p2_score, game.set_length, parse_ph(JSON.parse(game.point_history)), game.started_at, game.ended_at));
  } catch (err) {
    console.error('Error fetching last 50 games:', err.message);
    return null;
  }
}

export async function GetGameByID(id) {
  try {
    const db = await db_promise;
    const game = await db.get(
      `SELECT * FROM games WHERE id = ?`,
      id
    );
    return new Game(game.id, game.p1_id, game.p2_id, game.p1_elo_before_match, game.p2_elo_before_match, game.p1_elo_change, game.p2_elo_change, game.winner_id, game.p1_score, game.p2_score, game.set_length, parse_ph(JSON.parse(game.point_history)), game.started_at, game.ended_at);
  } catch (err) {
    console.error('Error fetching game:', err.message);
    return null;
  }
}

export async function GetAllPlayers() {
  try {
    const db = await db_promise;
    const players = await db.all(
      `SELECT * FROM players`
    );
    return players.map(player => new Player(player.id, player.name, player.elo, player.highest_elo, player.lowest_elo, player.total_games_played, player.total_wins, player.total_losses, player.points_won, player.points_lost, player.serves_won, player.serves_lost, player.longest_win_streak, player.longest_loss_streak, player.longest_game_duration, player.most_points_in_one_game, player.biggest_win_margin, player.biggest_loss_margin, player.most_points_won_in_a_row, player.most_points_lost_in_a_row, player.created_at));
  } catch (err) {
    console.error('Error fetching players:', err.message);
    return [];
  }
}

export async function GetAllPlayerNames() {
    try {
        const db = await db_promise;
        const player_names = await db.all(`SELECT name FROM players ORDER BY id`);
        return player_names.map(p => p.name);
    } catch (err) {
        console.error('Error fetching player names:', err.message);
        return null;
    }
}

export async function GetPlayerByID(player_id) {
  try {
    const db = await db_promise;
    const player = await db.get(
      `SELECT * FROM players WHERE id = ?`,
      player_id
    );
    return new Player(player.id, player.name, player.elo, player.highest_elo, player.lowest_elo, player.total_games_played, player.total_wins, player.total_losses, player.points_won, player.points_lost, player.serves_won, player.serves_lost, player.longest_win_streak, player.longest_loss_streak, player.longest_game_duration, player.most_points_in_one_game, player.biggest_win_margin, player.biggest_loss_margin, player.most_points_won_in_a_row, player.most_points_lost_in_a_row, player.created_at)
  } catch (err) {
    console.error('Error fetching player:', err.message);
    return null;
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

/**
 * Given a player ID, return the name of the opponent with whom the given player has played the most games with.
 * @param {*} player_id The ID of the player to get the stat for
 * @returns {string} The name of the opponent with whom the player has played the most games with
 */
export async function GetMostPlayedWithOpponent(player_id) {
  try {
    const db = await db_promise;
    const opponent = await db.get(
      `SELECT players.name AS opponent_name, COUNT(*) AS games_played
       FROM (
         SELECT p2_id AS opponent_id FROM games WHERE p1_id = ?
         UNION ALL
         SELECT p1_id AS opponent_id FROM games WHERE p2_id = ?
       ) AS combined
       JOIN players ON players.id = combined.opponent_id
       GROUP BY combined.opponent_id
       ORDER BY games_played DESC
       LIMIT 1`,
      player_id,
      player_id
    );
    return opponent?.opponent_name || null;
  } catch (err) {
    console.error('Error fetching opponent with most games played:', err.message);
    return null;
  }
}


/**
 * Get the amount of games played by two players together
 * @param {*} player_id_1 The ID of the first player
 * @param {*} player_id_2 The ID of the second player (the first player's opponent)
 * @returns The amount of games played by the two players together
 * @throws Error if the database query fails
 */
export async function GetCountOfGamesPlayedTogether(player_id_1, player_id_2) {
  try {
    const db = await db_promise;
    const count = await db.get(
      `SELECT COUNT(*) AS games_played
       FROM games
       WHERE (p1_id = ? AND p2_id = ?) OR (p1_id = ? AND p2_id = ?)`,
      player_id_1,
      player_id_2,
      player_id_2,
      player_id_1
    );
    return count.games_played;
  } catch (err) {
    console.error('Error fetching count of games played together:', err.message);
    return null;
  }
}

/**
 * Format the game duration from seconds to a more readable format (e.g., "5m 30s").
 * @param duration - The duration of the game in seconds.
 */
export function format_game_duration(duration) {
  const minutes = Math.floor(duration / 60);
  const seconds = duration % 60;
  return `${minutes}m ${seconds}s`;
}

/**
 * Get the win count of a player against a specific opponent.
 * @param {number} player_id - The ID of the player.
 * @param {number} opponent_id - The ID of the opponent.
 * @returns {number} The win count of the player against the opponent.
 */
export async function GetPlayerWinsAgainstOpponent(player_id, opponent_id) {
  try {
    const db = await db_promise;
    const count = await db.get(
      `SELECT COUNT(*) AS wins
       FROM games
       WHERE winner_id = ? AND ((p1_id = ? AND p2_id = ?) OR (p1_id = ? AND p2_id = ?))`,
      player_id,
      player_id,
      opponent_id,
      opponent_id,
      player_id
    );
    return count.wins;
  } catch (err) {
    console.error('Error fetching player wins against opponent:', err.message);
    return null;
  }
}

export function UtcToPst(date) {
  return DateTime.fromISO(date.replace(' ', 'T') + 'Z', { zone: 'utc' }).setZone('America/Los_Angeles').toFormat('MM/dd/yyyy');
}

/**
 * Get the Elo rating of a player, given their ID.
 */
export async function GetPlayerElo(player_id) {
  try {
    const db = await db_promise;
    const player = await db.get(
      `SELECT elo FROM players WHERE id = ?`,
      player_id
    );
    return player?.elo || null;
  } catch (err) {
    console.error('Error fetching player elo:', err.message);
    return null;
  }
}

import * as stats from './stats_functions.js';

/**
 * Update a player's stats in the database. Called after a game is uploaded.
 * The stats from the game are used to update the Player table in the database.
 * @param {*} p1_id The ID of player 1 in the game
 * @param {*} p2_id The ID of player 2 in the game
 * @param {*} p1_new_elo The new Elo rating of player 1 after the game has been played and the Elo calculation has been done
 * @param {*} p2_new_elo The new Elo rating of player 2 after the game has been played and the Elo calculation has been done
 * @param {*} point_history The point history of the game. This is used to update the stats of the players in the database.
 * @param {*} started_at The time the game started
 * @param {*} ended_at The time the game ended
 * @returns Success indicator. True for success, false for failure.
 */
export async function UpdatePlayerStats(p1_id, p2_id, p1_new_elo, p2_new_elo, point_history, started_at, ended_at) {
  try {
    // Calculate points
    let p1_points = 0;
    let p2_points = 0;
    for (const point of point_history) {
      if (point.w === 1) p1_points++;
      else if (point.w === 2) p2_points++;
    }

    // Player.elo, Player.highest_elo, Player.lowest_elo
    await stats.update_player_elo(p1_id, p1_new_elo);
    await stats.update_player_elo(p2_id, p2_new_elo);

    // Player.total_wins, Player.total_losses, Player.biggest_win_margin, Player.biggest_loss_margin
    if (p1_points > p2_points) {
      await stats.add_win_to_player(p1_id);
      await stats.add_loss_to_player(p2_id);
      await stats.update_biggest_win_margin_if_necessary(p1_id, p1_points - p2_points);
      await stats.update_biggest_loss_margin_if_necessary(p2_id, Math.abs(p2_points - p1_points));
    } else if (p2_points > p1_points) {
      await stats.add_win_to_player(p2_id);
      await stats.add_loss_to_player(p1_id);
      await stats.update_biggest_win_margin_if_necessary(p2_id, p2_points - p1_points);
      await stats.update_biggest_loss_margin_if_necessary(p1_id, Math.abs(p1_points - p2_points));
    }

    // Player.total_games_played
    await stats.increment_total_games_played_for_players(p1_id, p2_id);

    // Player.points_won, Player.points_lost
    await stats.update_point_stats_for_player(p1_id, p1_points, p2_points);
    await stats.update_point_stats_for_player(p2_id, p2_points, p1_points);

    // Player.serves_won, Player.serves_lost
    await stats.update_serves_won_and_lost(p1_id, 1, point_history);
    await stats.update_serves_won_and_lost(p2_id, 2, point_history);

    // Player.longest_win_streak, Player.longest_loss_streak
    await stats.update_longest_win_streak_if_necessary(p1_id);
    await stats.update_longest_win_streak_if_necessary(p2_id);
    await stats.update_longest_loss_streak_if_necessary(p1_id);
    await stats.update_longest_loss_streak_if_necessary(p2_id);

    // Player.longest_game_duration
    await stats.update_longest_game_duration_if_necessary(p1_id, started_at, ended_at);
    await stats.update_longest_game_duration_if_necessary(p2_id, started_at, ended_at);

    // Player.most_points_in_one_game
    await stats.update_most_points_in_one_game_if_necessary(p1_id, p1_points);
    await stats.update_most_points_in_one_game_if_necessary(p2_id, p2_points);

    // Player.most_points_won_in_a_row, Player.most_points_lost_in_a_row
    await stats.update_most_points_won_in_a_row_if_necessary(p1_id, 1, point_history);
    await stats.update_most_points_won_in_a_row_if_necessary(p2_id, 2, point_history);
    await stats.update_most_points_lost_in_a_row_if_necessary(p1_id, 1, point_history);
    await stats.update_most_points_lost_in_a_row_if_necessary(p2_id, 2, point_history);

    // Player.time_played
    await stats.add_playing_time(p1_id, started_at, ended_at);
    await stats.add_playing_time(p2_id, started_at, ended_at);
  } catch (err) {
    console.error('Error updating player stats:', err.message);
    return false;
  }

  return true;
}

export async function generate_elo_chart_url(player_id, char_line_color="#9370db") {
  try {
    const db = await db_promise;
    const games = await db.all(
      `SELECT p1_elo_before_match AS elo, id 
       FROM Games 
       WHERE p1_id = ? 
       
       UNION ALL 

       SELECT p2_elo_before_match AS elo, id 
       FROM Games 
       WHERE p2_id = ? 
       ORDER BY id ASC`,
      player_id,
      player_id
    );

    const game_count = games.length;
    if (game_count === 0) return null;

    // Get data
    const chart_labels = games.map((_, index) => index + 1);
    const chart_data = games.map((game) => game.elo);
    // The last point should be the player's current ELO
    const current_elo = await GetPlayerElo(player_id);
    chart_data.push(current_elo);
    chart_labels.push(game_count + 1);

    // Build chart
    const chart_config = {
      type: "line",
      data: {
        labels: chart_labels,
        datasets: [
          {
            label: "ELO",
            data: chart_data,
            fill: false,
            borderColor: char_line_color,
          },
        ],
      },

      options: {
        layout: { padding: { right: 30, top: 30 }},
        legend: { display: false },
        scales: {
          xAxes: [{ scaleLabel: { display: true, labelString: "Games" }}],
          yAxes: [{ scaleLabel: { display: true, labelString: "ELO" }}]
        }
      }

    };

    // Encode whole config once
    const url = `https://quickchart.io/chart?bkg=white&c=${encodeURIComponent(JSON.stringify(chart_config))}`;

    console.log(url);
    return url;
  } catch (err) {
    console.error("Error generating elo chart url:", err.message);
    return null;
  }
}