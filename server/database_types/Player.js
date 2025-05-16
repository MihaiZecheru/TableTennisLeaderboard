class Player {
  /**
   * @param {number} id - Unique identifier for the player
   * @param {string} name - Name of the player
   * @param {number} elo - Elo rating of the player
   * @param {number} highest_elo - The highest elo that the player ever possessed
   * @param {number} lowest_elo - The lowest elo that the player ever possessed
   * @param {number} total_games_played - Total number of games played by the player
   * @param {number} total_wins - Total number of wins by the player
   * @param {number} total_losses - Total number of losses by the player
   * @param {number} points_won - Total points won by the player
   * @param {number} points_lost - Total points lost by the player
   * @param {number} serves_won - Total serves won by the player
   * @param {number} serves_lost - Total serves lost by the player
   * @param {number} longest_win_streak - Longest winning streak of the player
   * @param {number} longest_loss_streak - Longest losing streak of the player
   * @param {number} longest_game_duration - Longest game duration of the player
   * @param {number} most_points_in_one_game - Most points scored in a single game by the player
   * @param {number} biggest_win_margin - Biggest win margin of the player
   * @param {number} biggest_loss_margin - Biggest loss margin of the player
   * @param {number} most_points_won_in_a_row - Most points won in a row by the player
   * @param {number} most_points_lost_in_a_row - Most points lost in a row by the player
   * @param {number} time_played - Total time played by the player
   * @param {string} created_at - Date when the player was created in the database
   * @description Represents a player in the database with various statistics and calculated fields.
   */
  constructor(id, name, elo, highest_elo, lowest_elo, total_games_played, total_wins, total_losses, points_won, points_lost, serves_won, serves_lost, longest_win_streak, longest_loss_streak,
              longest_game_duration, most_points_in_one_game, biggest_win_margin, biggest_loss_margin, most_points_won_in_a_row, most_points_lost_in_a_row, time_played, created_at) {
    // Database
    this.id = id; // Unique identifier for the player
    this.name = name; // Name of the player
    this.created_at = created_at; // Date when the player was created in the database

    // Stats
    this.elo = elo; // Elo rating of the player
    this.highest_elo = highest_elo; // The highest elo that the player ever possessed
    this.lowest_elo = lowest_elo; // The lowest elo that the player ever possessed
    this.total_games_played = total_games_played; // Total number of games played by the player
    this.total_wins = total_wins; // Total number of wins by the player
    this.total_losses = total_losses; // Total number of losses by the player
    this.points_won = points_won; // Total points won by the player
    this.points_lost = points_lost; // Total points lost by the player
    this.serves_won = serves_won; // Total serves won by the player
    this.serves_lost = serves_lost; // Total serves lost by the player
    this.longest_win_streak = longest_win_streak; // Longest winning streak of the player
    this.longest_loss_streak = longest_loss_streak; // Longest losing streak of the player
    this.point_win_percentage = points_won / (points_won + points_lost || 1); // Point win percentage of the player
    this.longest_game_duration = longest_game_duration; // Longest game duration of the player
    this.most_points_in_one_game = most_points_in_one_game; // Most points scored in a single game by the player
    this.biggest_win_margin = biggest_win_margin; // Biggest win margin of the player
    this.biggest_loss_margin = biggest_loss_margin; // Biggest loss margin of the player
    this.most_points_won_in_a_row = most_points_won_in_a_row; // Most points won in a row by the player
    this.most_points_lost_in_a_row = most_points_lost_in_a_row; // Most points lost in a row by the player
    this.time_played = time_played;
    
    // Calculated stats
    this.game_win_ratio = total_wins / (total_games_played || 1); // Player's win ratio
    this.point_win_ratio = points_won / (points_won + points_lost || 1); // Player's point win ratio
    this.serve_win_ratio = serves_won / (serves_won + serves_lost || 1); // Player's serve win ratio
    this.total_points_played = points_won + points_lost; // Total points played by the player
  }
}

export default Player;