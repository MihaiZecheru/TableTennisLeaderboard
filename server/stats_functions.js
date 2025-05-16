import { db_promise } from './sqlite_instance.js';

/**
 * Update Player.points_won and Player.points_lost
 * @param {*} player_id The ID of the player to update
 * @param {*} points_won The amount of points won by the player from this last game (not total points, just from this game)
 * @param {*} points_lost The amount of points lost by the player from this last game (not total points, just from this game)
 */
export async function update_point_stats_for_player(player_id, points_won, points_lost) {
  try {
    const db = await db_promise;
    await db.run(
      `UPDATE players SET points_won = points_won + ?, points_lost = points_lost + ? WHERE id = ?`,
      points_won,
      points_lost,
      player_id
    );
  } catch (err) {
    console.error('Error updating point stats for player:', err.message);
  }
}

/**
 * Increment Player.total_wins for two players
 * @param {*} player_id_1 The first player to increment Player.total_games_played for
 * @param {*} player_id_2 The second player to increment Player.total_games_played for
 */
export async function increment_total_games_played_for_players(player_id_1, player_id_2) {
  try {
    const db = await db_promise;
    await db.run(
      `UPDATE players SET total_games_played = total_games_played + 1 WHERE id = ? OR id = ?`,
      player_id_1,
      player_id_2
    );
  } catch (err) {
    console.error('Error incrementing total games played for players:', err.message);
  }
}

export async function add_win_to_player(player_id) {
  try {
    const db = await db_promise;
    await db.run(
      `UPDATE players SET total_wins = total_wins + 1 WHERE id = ?`,
      player_id
    );
  } catch (err) {
    console.error('Error adding win to player:', err.message);
  }
}

export async function add_loss_to_player(player_id) {
  try {
    const db = await db_promise;
    await db.run(
      `UPDATE players SET total_losses = total_losses + 1 WHERE id = ?`,
      player_id
    );
  } catch (err) {
    console.error('Error adding loss to player:', err.message);
  }
}

/**
 * Set Player.elo to p_new_elo. If p_new_elo is higher than Player.highest_elo, set Player.highest_elo to p_new_elo.
 * If p_new_elo is lower than Player.lowest_elo, set Player.lowest_elo to p_new_elo.
 * @param {*} p_id The ID of the player to update
 * @param {*} p_new_elo The player's new elo rating
 */
export async function update_player_elo(p_id, p_new_elo) {
  try {
    const db = await db_promise;
    await db.run(
      `UPDATE players SET elo = ?, highest_elo = CASE WHEN highest_elo < ? THEN ? ELSE highest_elo END, lowest_elo = CASE WHEN lowest_elo > ? THEN ? ELSE lowest_elo END WHERE id = ?`,
      p_new_elo,
      p_new_elo,
      p_new_elo,
      p_new_elo,
      p_new_elo,
      p_id
    );
  } catch (err) {
    console.error('Error updating player elo:', err.message);
  }
}

/**
 * @param {*} start_time Game.started_at
 * @param {*} end_time Game.ended_at
 * @returns The duration of the game in seconds
 */
function calculate_game_duration(start_time, end_time) {
  const durationMs = new Date(end_time) - new Date(start_time);
  const minutes = Math.floor(durationMs / 60000);
  const seconds = Math.floor((durationMs % 60000) / 1000);
  return minutes * 60 + seconds;
} 

export async function update_longest_game_duration_if_necessary(player_id, started_at, ended_at) {
  const duration = calculate_game_duration(started_at, ended_at);
  try {
    const db = await db_promise;
    await db.run(`UPDATE players SET longest_game_duration = CASE WHEN longest_game_duration < ? THEN ? ELSE longest_game_duration END WHERE id = ?`, duration, duration, player_id);
  } catch (err) {
    console.error('Error updating longest game duration:', err.message);
  }
}

export async function update_most_points_in_one_game_if_necessary(player_id, points_won) {
  try {
    const db = await db_promise;
    await db.run(`UPDATE players SET most_points_in_one_game = CASE WHEN most_points_in_one_game < ? THEN ? ELSE most_points_in_one_game END WHERE id = ?`, points_won, points_won, player_id);
  } catch (err) {
    console.error('Error updating most points in one game:', err.message);
  }
}

export async function update_biggest_win_margin_if_necessary(player_id, point_difference) {
  try {
    const db = await db_promise;
    await db.run(`UPDATE players SET biggest_win_margin = CASE WHEN biggest_win_margin < ? THEN ? ELSE biggest_win_margin END WHERE id = ?`, point_difference, point_difference, player_id);
  } catch (err) {
    console.error('Error updating biggest win margin:', err.message);
  }
}

export async function update_biggest_loss_margin_if_necessary(player_id, point_difference) {
  try {
    const db = await db_promise;
    await db.run(`UPDATE players SET biggest_loss_margin = CASE WHEN biggest_loss_margin < ? THEN ? ELSE biggest_loss_margin END WHERE id = ?`, point_difference, point_difference, player_id);
  } catch (err) {
    console.error('Error updating biggest loss margin:', err.message);
  }
}

export async function update_most_points_won_in_a_row_if_necessary(player_id, player_num, point_history) {
  // Calculate the longest streak of points won in a row
  let longest_streak = 0;
  let current_streak = 0;
  for (const point of point_history) {
    if (point.w === player_num) {
      current_streak++;
      if (current_streak > longest_streak) {
        longest_streak = current_streak;
      }
    } else {
      current_streak = 0;
    }
  }

  try {
    const db = await db_promise;
    await db.run(`UPDATE players SET most_points_won_in_a_row = CASE WHEN most_points_won_in_a_row < ? THEN ? ELSE most_points_won_in_a_row END WHERE id = ?`, longest_streak, longest_streak, player_id);
  } catch (err) {
    console.error('Error updating most points won in a row:', err.message);
  }
}

export async function update_most_points_lost_in_a_row_if_necessary(player_id, player_num, point_history) {
  // Calculate the longest streak of points lost in a row
  let longest_streak = 0;
  let current_streak = 0;
  for (const point of point_history) {
    if (point.w != player_num) {
      current_streak++;
      if (current_streak > longest_streak) {
        longest_streak = current_streak;
      }
    } else {
      current_streak = 0;
    }
  }

  try {
    const db = await db_promise;
    await db.run(`UPDATE players SET most_points_lost_in_a_row = CASE WHEN most_points_lost_in_a_row < ? THEN ? ELSE most_points_lost_in_a_row END WHERE id = ?`, longest_streak, longest_streak, player_id);
  } catch (err) {
    console.error('Error updating most points lost in a row:', err.message);
  }
}

export async function add_playing_time(player_id, started_at, ended_at) {
  const duration = calculate_game_duration(started_at, ended_at);
  try {
    const db = await db_promise;
    await db.run(`UPDATE players SET time_played = time_played + ? WHERE id = ?`, duration, player_id);
  } catch (err) {
    console.error('Error updating playing time:', err.message);
  }
}

export async function update_longest_win_streak_if_necessary(player_id) {
  try {
    const db = await db_promise;

    // 1. Get all games the player was involved in, ordered by time
    const games = await db.all(
      `SELECT id, winner_id, p1_id, p2_id
       FROM games
       WHERE p1_id = ? OR p2_id = ?
       ORDER BY id ASC`,
      player_id, player_id
    );

    // 2. Calculate longest win streak
    let longest_streak = 0;
    let current_streak = 0;

    for (const game of games) {
      const won = game.winner_id === player_id;

      if (won) {
        current_streak++;
        if (current_streak > longest_streak) {
          longest_streak = current_streak;
        }
      } else {
        current_streak = 0;
      }
    }

    // 3. Update DB only if the new streak is longer
    await db.run(
      `UPDATE players
       SET longest_win_streak = CASE
         WHEN longest_win_streak < ? THEN ?
         ELSE longest_win_streak
       END
       WHERE id = ?`,
      longest_streak, longest_streak, player_id
    );
  } catch (err) {
    console.error('Error updating longest win and loss streak:', err.message);
  }
}

export async function update_longest_loss_streak_if_necessary(player_id) {
  try {
    const db = await db_promise;

    // 1. Get all games the player was involved in, ordered by time
    const games = await db.all(
      `SELECT id, winner_id, p1_id, p2_id
       FROM games
       WHERE p1_id = ? OR p2_id = ?
       ORDER BY id ASC`,
      player_id, player_id
    );

    // 2. Calculate longest loss streak
    let longest_streak = 0;
    let current_streak = 0;

    for (const game of games) {
      const lost = game.winner_id !== player_id;

      if (lost) {
        current_streak++;
        if (current_streak > longest_streak) {
          longest_streak = current_streak;
        }
      } else {
        current_streak = 0;
      }
    }

    // 3. Update DB only if the new streak is longer
    await db.run(
      `UPDATE players
       SET longest_loss_streak = CASE
         WHEN longest_loss_streak < ? THEN ?
         ELSE longest_loss_streak
       END
       WHERE id = ?`,
      longest_streak, longest_streak, player_id
    );
  } catch (err) {
    console.error('Error updating longest win and loss streak:', err.message);
  }
}

export async function update_serves_won_and_lost(player_id, player_num, point_history) {
  let serves_won = 0;
  let serves_lost = 0;

  for (const point of point_history) {
    if (point.s == player_num) {
      if (point.w == player_num) {
        serves_won++;
      } else {
        serves_lost++;
      }
    }
  }

  try {
    const db = await db_promise;
    await db.run(
      `UPDATE players SET serves_won = serves_won + ?, serves_lost = serves_lost + ? WHERE id = ?`,
      serves_won,
      serves_lost,
      player_id
    );
  } catch (err) {
    console.error('Error updating serves won and lost:', err.message);
  }
}