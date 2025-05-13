class Player {
  /**
   * @param {number} id
   * @param {string} name
   * @param {number} total_games_played 
   * @param {number} total_wins 
   * @param {number} total_losses 
   * @param {number} points_won 
   * @param {number} points_lost 
   * @param {number} longest_win_streak 
   * @param {number} longest_loss_streak 
   * @param {Date} created_at 
   */
  constructor(id, name, total_games_played, total_wins, total_losses, points_won, points_lost, longest_win_streak, longest_loss_streak, created_at) {
    this.id = id; // Unique identifier for the player
    this.name = name; // Name of the player
    this.total_games_played = total_games_played; // Total number of games played by the player
    this.total_wins = total_wins; // Total number of wins by the player
    this.total_losses = total_losses; // Total number of losses by the player
    this.points_won = points_won; // Total points won by the player
    this.points_lost = points_lost; // Total points lost by the player
    this.longest_win_streak = longest_win_streak; // Longest winning streak of the player
    this.longest_loss_streak = longest_loss_streak; // Longest losing streak of the player
    this.created_at = created_at; // Date when the player was created in the database

    this.game_win_ratio = total_wins / (total_games_played || 1); // Player's win ratio
    this.point_win_percentage = points_won / (points_won + points_lost || 1); // Point win percentage of the player
    this.total_points = points_won + points_lost; // Total points played by the player
  }
}

export default Player;