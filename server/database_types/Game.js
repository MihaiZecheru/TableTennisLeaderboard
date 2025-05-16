class Game {
  /**
   * @param {number} id Database ID of the game
   * @param {number} p1_id ID of player 1
   * @param {number} p2_id ID of player 2
   * @param {number} p1_elo_before_match The elo rating that player 1 had before the match
   * @param {number} p2_elo_before_match The elo rating that player 2 had before the match
   * @param {number} p1_elo_change The elo rating change for player 1 as a result of the match
   * @param {number} p2_elo_change The elo rating change for player 2 as a result of the match
   * @param {number} winner_id The ID of the player who won the match
   * @param {number} p1_score The score that player 1 achieved in the match
   * @param {number} p2_score The score that player 2 achieved in the match
   * @param {number} set_length The length of the set in points (first to 6, 11, or 21)
   * @param {Point[]} point_history Play by play history of the match
   * @param {Date} started_at The time that the match started
   * @param {Date} ended_at The time that the match ended
   */
  constructor(id, p1_id, p2_id, p1_elo_before_match, p2_elo_before_match, p1_elo_change, p2_elo_change, winner_id, p1_score, p2_score, set_length, point_history, started_at, ended_at) {
    this.id = id;
    this.p1_id = p1_id;
    this.p2_id = p2_id;
    this.p1_elo_before_match = p1_elo_before_match;
    this.p2_elo_before_match = p2_elo_before_match;
    this.p1_elo_change = p1_elo_change;
    this.p2_elo_change = p2_elo_change;
    this.winner_id = winner_id;
    this.p1_score = p1_score;
    this.p2_score = p2_score;
    this.set_length = set_length;
    this.point_history = point_history; // Array of objects with 's' and 'w' properties
    this.started_at = started_at; // Date string or timestamp
    this.ended_at = ended_at; // Date string or timestamp
    
    this.match_length = (new Date(this.ended_at).getTime() - new Date(this.started_at).getTime()) / 1000; // Duration of the game in seconds
  }
}

export default Game;